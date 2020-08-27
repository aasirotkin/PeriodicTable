#include "periodictablescene.h"
#include "chemicalelement.h"
#include "textitem.h"

#include <QFile>
#include <QTextStream>
#include <QTransform>
#include <QGraphicsSceneMouseEvent>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

PeriodicTableScene::PeriodicTableScene(QObject *parent) :
    QGraphicsScene(parent)
{
    createChemicalItemsFromResFile_();
    createPeriodGroupItems_();
    initMaxLengthText_();
    rearrange_();
}

void PeriodicTableScene::scaleItems(const int width, const int height)
{
    max_scene_width_ = static_cast<float>(width);
    max_scene_height_ = static_cast<float>(height);
    item_width_ = max_scene_width_ / cols_count_;
    item_height_ = max_scene_height_ / rows_count_;

    const QRectF item_rect = QRectF(0.0, 0.0, item_width_, item_height_);
    const QFont font = createFont_();
    for (auto* base_item : items())
    {
        GraphicsItem* item = static_cast<GraphicsItem*>(base_item);
        item->setRect(item_rect);
        item->setFont(font);
        item->relocation();
    }
}

void PeriodicTableScene::mouseLeaveEvent()
{
    resetHoveredItem_();
}

void PeriodicTableScene::createChemicalItemsFromResFile_()
{
    QFile file(":/data/periodic_data.csv");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString element_data = stream.readLine();
            ChemicalElement* item = new ChemicalElement();
            item->setItemData(element_data);
            addItem(item);
        }
    }
}

void PeriodicTableScene::createPeriodGroupItems_()
{
    QFile file(":/data/period_group_data.csv");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString period_group_data = stream.readLine();
            if (period_group_data.isEmpty())
                continue;
            TextItem* item = new TextItem();
            item->setItemData(period_group_data);
            addItem(item);
        }
    }
}

void PeriodicTableScene::initMaxLengthText_()
{
    max_text_length_ = QString("");
    for (auto* item : items())
    {
        GraphicsItem* element = static_cast<GraphicsItem*>(item);
        if (element->rtext().length() > max_text_length_.length())
        {
            max_text_length_ = element->rtext();
        }
    }
}

QFont PeriodicTableScene::createFont_()
{
    QFont font(QString(""));
    float point_size_f = 0.9;
    int length = 0;
    int height = 0;
    do
    {
        point_size_f += 0.1;
        font.setPointSizeF(point_size_f);
        // fm - font metrics
        const QFontMetrics fm(font);
        length = fm.horizontalAdvance(max_text_length_);
        height = fm.height();
    }
    while(length < item_width_ && height < item_height_ && point_size_f < 50.0);
    return font;
}

void PeriodicTableScene::rearrange_()
{
    srand(time(NULL));

    QList<GraphicsItem::GridPos> poses = createGridNumbers_(
                rows_count_,
                cols_count_);

    int i = 0;
    for (auto* item : items())
    {
        GraphicsItem* element = static_cast<GraphicsItem*>(item);
        element->setGridPos(poses.at(i));
//        element->setGridPos(element->rmainGridPos());
        ++i;
    }
}

QList<GraphicsItem::GridPos> PeriodicTableScene::createGridNumbers_(
        const int max_row, const int max_col) const
{
    QList<GraphicsItem::GridPos> poses;

    while (poses.count() < items_count_)
    {
        GraphicsItem::GridPos value{rand() % max_row, rand() % max_col};
        if (!poses.contains(value))
        {
            poses.append(value);
        }
    }

    return poses;
}

GraphicsItem *PeriodicTableScene::itemAt_(const QPointF &point) const
{
    return static_cast<GraphicsItem*>(itemAt(point, QTransform()));
}

GraphicsItem::GridPos PeriodicTableScene::gridPos_(
        const QPointF &point) const
{
    const int rows_behind = static_cast<int>(point.y() / item_height_);
    const int cols_behind = static_cast<int>(point.x() / item_width_);
    return {rows_behind, cols_behind};
}

QPointF PeriodicTableScene::nearestPos_(const QPointF &point) const
{
    const auto [row, col] = gridPos_(point);
    return {item_width_ * col, item_height_ * row};
}

bool PeriodicTableScene::itemExistsAt_(const QPointF &point) const
{
    return itemAt_(point) != nullptr;
}

void PeriodicTableScene::checkHorizontalPos_(
        QPointF &pos, const int delta_x, const int base_x) const
{
    if (delta_x > 0.0 || delta_x < 0.0)
    {
        QPointF check_point = (delta_x > 0.0) ?
                    current_item_->rightCenterPos() :
                    current_item_->leftCenterPos();

        check_point.rx() += delta_x;
        if (itemExistsAt_(check_point))
        {
            pos.setX(base_x);
        }
    }
}

void PeriodicTableScene::checkVerticalPos_(
        QPointF &pos, const int delta_y, const int base_y) const
{
    if (delta_y > 0.0 || delta_y < 0.0)
    {
        QPointF check_point = (delta_y > 0.0) ?
                    current_item_->bottomCenterPos() :
                    current_item_->topCenterPos();

        check_point.ry() += delta_y;
        if (itemExistsAt_(check_point))
        {
            pos.setY(base_y);
        }
    }
}

void PeriodicTableScene::checkBoundingRect_(QPointF &pos) const
{
    const double max_width = max_scene_width_ - item_width_;
    const double max_height = max_scene_height_ - item_height_;
    pos.rx() = std::max(0.0, pos.rx());
    pos.ry() = std::max(0.0, pos.ry());
    pos.rx() = std::min(max_width, pos.rx());
    pos.ry() = std::min(max_height, pos.ry());
}

void PeriodicTableScene::mouseMoveEvent_(const QPointF &delta)
{
    const QPointF scene_pos = current_item_->scenePos();
    QPointF pos{scene_pos + delta};

    checkHorizontalPos_(pos, delta.x(), scene_pos.x());
    checkVerticalPos_(pos, delta.y(), scene_pos.y());
    checkBoundingRect_(pos);

    current_item_->setPos(pos);
}

void PeriodicTableScene::mouseReleaseEvent_()
{
    const QPointF center_pos = current_item_->sceneCenterPos();
    current_item_->setPos(nearestPos_(center_pos));
    current_item_->setGridPos(gridPos_(center_pos));
    current_item_->setZValue(0.0);
    current_item_->setHovered(false);
    current_item_ = nullptr;
}

void PeriodicTableScene::resetHoveredItem_()
{
    if (hovered_item_)
    {
        hovered_item_->setHovered(false);
        update(hovered_item_->sceneBoundingRect());
        hovered_item_ = nullptr;
    }
}

void PeriodicTableScene::mouseHoveredEvent_(const QPointF &pos)
{
    GraphicsItem* item = itemAt_(pos);
    if (item)
    {
        if (hovered_item_ && item != hovered_item_)
        {
            hovered_item_->setHovered(false);
            update(hovered_item_->sceneBoundingRect());
        }
        hovered_item_ = item;
        hovered_item_->setHovered(true);
        update(hovered_item_->sceneBoundingRect());
    }
    else
    {
        resetHoveredItem_();
    }
}

void PeriodicTableScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    current_item_ = itemAt_(event->scenePos());
    if (current_item_)
    {
        current_item_->setZValue(1.0);
        current_item_->resetGridPos();
        current_item_->setHovered(false);
    }
    QGraphicsScene::mousePressEvent(event);
}

void PeriodicTableScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (current_item_)
    {
        if (itemAt_(event->scenePos()) == current_item_)
        {
            const QPointF delta{event->scenePos() - event->lastScenePos()};
            mouseMoveEvent_(delta);
        }
        else
        {
            mouseReleaseEvent_();
        }
    }
    else
    {
        mouseHoveredEvent_(event->scenePos());
    }
}

void PeriodicTableScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (current_item_)
    {
        mouseReleaseEvent_();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
