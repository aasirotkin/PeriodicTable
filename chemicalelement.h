#ifndef RECTITEM_H
#define RECTITEM_H

#include "graphicsitem.h"

class ChemicalElement : public GraphicsItem
{
public:
    struct ElementData
    {
        int number;
        int period;
        int group;
        QString symbol;
        QString name;
        QColor color;

        static QColor colorFromString(const QString& data,
                                      const QString& sep = ",")
        {
            QStringList data_list = data.split(sep);
            return (data_list.count() == 3) ?
                        QColor(data_list.at(0).toInt(),
                               data_list.at(1).toInt(),
                               data_list.at(2).toInt()) :
                        Qt::black;
        }
    };

public:
    ChemicalElement(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    bool setElementData(const QString& data);

private:
    GridPos makeGridPos_(const int period, const int group, const int number);

private:
    // amount of ElementData fields
    const int data_amount_{6};
    ElementData element_data_;

};

#endif // RECTITEM_H
