#include "Qt_General.h"

// DirectionalProperties

Qt_Gen::DirectionalProperties::DirectionalProperties(bool isHorizontal)
    : isHorizontal(isHorizontal)
{}

void Qt_Gen::DirectionalProperties::setDirection(bool isHorizontal)
{
    DirectionalProperties::isHorizontal = isHorizontal;
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
