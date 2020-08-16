#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QFont>

class GraphicsItem : public QGraphicsItem
{
public:
    struct GridPos
    {
        int row;
        int col;

        GridPos(const int row = 0, const int col = 0) :
            row(row), col(col) {}

        bool operator== (const GridPos& pos) const
        {
            return (this->row == pos.row) && (this->col == pos.col);
        }

        static GridPos fromString(const QString& data,
                                  const QString& sep = ",")
        {
            const QStringList data_list = data.split(sep);
            return (data_list.count() == 2) ?
            GridPos(data_list.at(0).toInt(), data_list.at(1).toInt()) :
            GridPos(-1, -1);
        }

        void reset()
        {
            row = -1;
            col = -1;
        }
    };

public:
    GraphicsItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override { return bounding_rect_; };

    GridPos gridPos() const { return grid_pos_; }

    void setGridPos(const GridPos& pos);
    void setRect(const QRectF& rect);
    void setFont(const QFont& font);
    void setText(const QString& text);
    void updateTextRect(const float width, const float height);
    void appendMainGridPoses(const GridPos& pos);
    void relocation();
    void resetGridPos();
    void setHovered(const bool hovered);

    QPointF sceneCenterPos() const;
    QPointF rightCenterPos() const;
    QPointF leftCenterPos() const;
    QPointF topCenterPos() const;
    QPointF bottomCenterPos() const;

    // r - reference
    const GridPos& rgridPos() const { return grid_pos_; }
    const QRectF& rboundingRect() const { return bounding_rect_; }
    const QFont& rfont() const;
    const QString& rtext() const { return text_; }
    const QRectF& rtextRect() const { return text_rect_; }
    const GridPos& rmainGridPos() const { return main_grid_poses_.first(); }
    const int& rradius() const { return radius_; }

    bool mainGridPosContains(const GridPos& pos) const;

private:
    GridPos grid_pos_;
    QRectF bounding_rect_;
    QFont font_;
    QString text_;
    QRectF text_rect_;
    QList<GridPos> main_grid_poses_;
    // radius value in percents
    const int radius_{10};

};

#endif // GRAPHICSITEM_H
