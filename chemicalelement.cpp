#include "chemicalelement.h"

#include <QPainter>

ChemicalElement::ChemicalElement(QGraphicsItem *parent) :
    GraphicsItem(parent)
{

}

void ChemicalElement::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path;
    path.addRoundedRect(rboundingRect(), rradius(), rradius(),
                        Qt::RelativeSize);
    if (!mainGridPosContains(gridPos()))
    {
        painter->fillPath(path, QBrush(Qt::white));
    }
    else
    {
        painter->fillPath(path, element_data_.color);
    }
    painter->drawPath(path);
    painter->setFont(rfont());
    painter->drawText(rboundingRect(), Qt::AlignCenter, rdisplayedText());
}

bool ChemicalElement::setItemData(const QString &data)
{
    static const QString sep{" "};
    const QStringList data_list = data.split(sep);
    if (data_amount_ != data_list.count())
    {
        return false;
    }
    element_data_.number = data_list.at(0).toInt();
    element_data_.period = data_list.at(1).toInt();
    element_data_.group = data_list.at(2).toInt();
    element_data_.symbol = data_list.at(3);
    element_data_.name = data_list.at(4);
    element_data_.color = ElementData::colorFromString(data_list.at(5));

    appendMainGridPoses(makeGridPos_(element_data_.period,
                                     element_data_.group,
                                     element_data_.number));

    setText(element_data_.symbol);
    setHint(GridPos::toString(rmainGridPos()));

    return true;
}

GridPos ChemicalElement::makeGridPos_(
        const int period, const int group, const int number)
{
    static const QList<int> seven_asterisk{58, 59, 60, 61, 62, 63, 64,
                                           65, 66, 67, 68, 69, 70, 71};
    static const QList<int> eight_asterisk{90, 91, 92, 93, 94, 95, 96,
                                           97, 98, 99, 100, 101, 102, 103};
    static const QList<int> right_numbers{1, 3, 4, 11, 12, 19, 20, 21,
                                          37, 38, 39, 55, 56, 57, 87, 88, 89};
    if (seven_asterisk.contains(number))
    {
        return {period + 2, group + 2 + number - 58};
    }
    else if (eight_asterisk.contains(number))
    {
        return {period + 2, group + 2 + number - 90};
    }
    else if (!right_numbers.contains(number))
    {
        return {period, group + 1};
    }
    else
    {
        return {period, group};
    }
}
