#ifndef PERIODICTABLESCENE_H
#define PERIODICTABLESCENE_H

#include "graphicsitem.h"

#include <QGraphicsScene>

class PeriodicTableScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PeriodicTableScene(QObject *parent = nullptr);

    void scaleItems(const int width, const int height);
    void mouseLeaveEvent();

private:
    void createChemicalItemsFromResFile_();
    void createPeriodGroupItems_();
    void rearrange_();
    QList<GraphicsItem::GridPos> createGridNumbers_(
            const int max_row, const int max_col) const;
    GraphicsItem* itemAt_(const QPointF& point) const;
    GraphicsItem::GridPos gridPos_(const QPointF &point) const;
    QPointF nearestPos_(const QPointF& point) const;
    bool itemExistsAt_(const QPointF& point) const;
    void checkHorizontalPos_(
            QPointF& pos, const int delta_x, const int base_x) const;
    void checkVerticalPos_(
            QPointF& pos, const int delta_y, const int base_y) const;
    void checkBoundingRect_(QPointF& pos) const;
    void mouseMoveEvent_(const QPointF& delta);
    void mouseReleaseEvent_();
    void resetHoveredItem_();
    void mouseHoveredEvent_(const QPointF& pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    static const int chemical_items_count{118};
    static const int text_items_count{33};
    static const int items_count_{chemical_items_count + text_items_count};
    static const int rows_count_{10};
    static const int cols_count_{20};

private:
    float item_width_;
    float item_height_;
    float max_scene_width_;
    float max_scene_height_;

private:
    GraphicsItem* current_item_{nullptr};
    GraphicsItem* hovered_item_{nullptr};
};

#endif // PERIODICTABLESCENE_H
