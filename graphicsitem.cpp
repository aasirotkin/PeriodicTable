#include "graphicsitem.h"

GraphicsItem::GraphicsItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    timer_(is_hint_enable_, grid_pos_)
{
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

void GraphicsItem::setGridPos(const GridPos &pos)
{
    grid_pos_ = pos;
}

void GraphicsItem::setRect(const QRectF &rect)
{
    bounding_rect_ = rect;
}

void GraphicsItem::setFont(const QFont &font)
{
    font_ = font;
}

void GraphicsItem::setText(const QString &text)
{
    text_ = text;
}

void GraphicsItem::setHint(const QString &hint)
{
    hint_ = hint;
}

void GraphicsItem::appendMainGridPoses(const GridPos &pos)
{
    main_grid_poses_.append(pos);
}

void GraphicsItem::relocation()
{
    setPos(bounding_rect_.width() * grid_pos_.col,
           bounding_rect_.height() * grid_pos_.row);
}

void GraphicsItem::resetGridPos()
{
    grid_pos_.reset();
}

void GraphicsItem::setHovered(const bool hovered)
{
    font_.setBold(hovered);
}

QPointF GraphicsItem::sceneCenterPos() const
{
    return sceneBoundingRect().center();
}

QPointF GraphicsItem::rightCenterPos() const
{
    return mapToScene(
                bounding_rect_.width(),
                0.5 * bounding_rect_.height());
}

QPointF GraphicsItem::leftCenterPos() const
{
    return mapToScene(
                0.0,
                0.5 * bounding_rect_.height());
}

QPointF GraphicsItem::topCenterPos() const
{
    return mapToScene(
                0.5 * bounding_rect_.width(),
                0.0);
}

QPointF GraphicsItem::bottomCenterPos() const
{
    return mapToScene(
                0.5 * bounding_rect_.width(),
                bounding_rect_.height());
}

const QFont &GraphicsItem::rfont() const
{
    return font_;
}

const QString &GraphicsItem::rdisplayedText() const
{
    return (is_hint_enable_) ? hint_ : text_;
}

bool GraphicsItem::mainGridPosContains(const GridPos &pos) const
{
    return main_grid_poses_.contains(pos);
}

void GraphicsItem::showHint()
{
    if (!is_hint_enable_)
    {
        timer_.showHint();
    }
    else
    {
        timer_.stop();
    }
}
