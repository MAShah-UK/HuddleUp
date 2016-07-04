#pragma once

#include <QSize>
#include <QPoint>

namespace Qt_Gen
{

class DirectionalProperties
{
    // False means we want vertical properties.
    bool isHorizontal = true;

public:

    DirectionalProperties(bool isHorizontal = true);
    void setDirection(bool isHorizontal = true);
    bool isDirHorizontal();

    int operator()    (const QSize&   val);
    int operator()    (const QPoint&  val);
    double operator() (const QPointF& val);
};

}
