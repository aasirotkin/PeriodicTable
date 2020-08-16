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

    void setTextData(const QString& data);
};

#endif // TEXTITEM_H
