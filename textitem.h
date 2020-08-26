#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "graphicsitem.h"

class TextItem : public GraphicsItem
{
public:
    TextItem(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    bool setItemData(const QString &data) override;
};

#endif // TEXTITEM_H
