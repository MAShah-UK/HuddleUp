#include "Qt_General.h"

#include <qDebug>
#include <QSize>
#include <QPoint>
#include <QPalette>
#include <QWidget>

// Stateless:

void Qt_Gen::setBackgroundColor(QWidget* widget, const QColor& color)
{
    QPalette p;
    p.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(p);
}

QSize Qt_Gen::sizePerc(const QPoint& percentages, const QSize& ofSize)
{
    return {int( percentages.x()/100 * ofSize.width() ),
            int( percentages.y()/100 * ofSize.height() )};
}

int Qt_Gen::min(const QSize& size)
{
    return size.width() < size.height() ? size.width() : size.height();
}

int Qt_Gen::max(const QSize& size)
{
    return size.width() > size.height() ? size.width() : size.height();
}

// DirectionalProperties

Qt_Gen::DirectionalProperties::DirectionalProperties(bool isHorizontal)
    : isHorizontal(isHorizontal)
{}

void Qt_Gen::DirectionalProperties::setDirection(bool isHorizontal)
{
    DirectionalProperties::isHorizontal = isHorizontal;
}

void Qt_Gen::DirectionalProperties::setDirection(const QSize& size)
{
    isHorizontal = size.width() > size.height() ? true : false;
}

bool Qt_Gen::DirectionalProperties::isDirHorizontal()
{
    return isHorizontal;
}

int Qt_Gen::DirectionalProperties::operator() (const QSize& val)
{
    return isHorizontal ? val.width() : val.height();
}

int Qt_Gen::DirectionalProperties::operator() (const QPoint& val)
{
    return isHorizontal ? val.x() : val.y();
}

double Qt_Gen::DirectionalProperties::operator() (const QPointF& val)
{
    return isHorizontal ? val.x() : val.y();
}
