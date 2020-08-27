#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QObject>
#include <QTimer>

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

    static QString toString(const GridPos& pos)
    {
        return QString("%1/%2").arg(pos.row).arg(pos.col);
    }

    void reset()
    {
        row = -1;
        col = -1;
    }
};

class GraphicsItemTimer : public QObject
{
    Q_OBJECT
public:
    GraphicsItemTimer(bool& is_hint_enabled, const GridPos& pos) :
        is_hint_enabled_(is_hint_enabled),
        pos_(pos) {}

signals:
    void update(const GridPos& pos);

public slots:
    void showHint()
    {
        timer_ = new QTimer();
        timer_->setSingleShot(true);
        connect(timer_, &QTimer::timeout, this, &GraphicsItemTimer::hideHint);
        is_hint_enabled_ = true;
        emit update(pos_);
        timer_->start(1000);
    }

    void stop()
    {
        if (timer_)
        {
            timer_->stop();
            hideHint();
        }
    }

    void hideHint()
    {
        is_hint_enabled_ = false;
        delete timer_;
        emit update(pos_);
    }

private:
    QTimer* timer_;
    bool& is_hint_enabled_;
    const GridPos& pos_;
};

class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override { return bounding_rect_; };

    GridPos gridPos() const { return grid_pos_; }

    void setGridPos(const GridPos& pos);
    void setRect(const QRectF& rect);
    void setFont(const QFont& font);
    void setText(const QString& text);
    void setHint(const QString& hint);
    void appendMainGridPoses(const GridPos& pos);
    void relocation();
    void resetGridPos();
    void setHovered(const bool hovered);
    virtual bool setItemData(const QString& data) = 0;

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
    const QString& rhint() const { return hint_; }
    const GridPos& rmainGridPos() const { return main_grid_poses_.first(); }
    const int& rradius() const { return radius_; }
    const QString& rdisplayedText() const;

    bool mainGridPosContains(const GridPos& pos) const;
    GraphicsItemTimer& timer() { return timer_; }

    void showHint();

private:
    GraphicsItemTimer timer_;
    GridPos grid_pos_;
    QRectF bounding_rect_;
    QFont font_;
    QString text_;
    QString hint_;
    bool is_hint_enable_{false};
    QList<GridPos> main_grid_poses_;
    // radius value in percents
    const int radius_{10};

};

#endif // GRAPHICSITEM_H
