#include "textitem.h"

#include <QPainter>

TextItem::TextItem(QGraphicsItem *parent) :
    GraphicsItem(parent)
{

}

void TextItem::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!mainGridPosContains(gridPos()))
    {
        QPainterPath path;
        path.addRoundedRect(rboundingRect(), rradius(), rradius(),
                            Qt::RelativeSize);
        painter->fillPath(path, QBrush(Qt::white));
        painter->drawPath(path);
    }
    painter->setFont(rfont());
    painter->drawText(rboundingRect(), Qt::AlignCenter, rdisplayedText());
}

bool TextItem::setItemData(const QString &data)
{
    static const QString sep{" "};
    const QStringList data_list = data.split(sep);
    setText(data_list.first());
    for (int i = 1; i < data_list.count(); ++i)
    {
        appendMainGridPoses(GridPos::fromString(data_list.at(i)));
    }
    setHint(GridPos::toString(rmainGridPos()));
    return true;
}
