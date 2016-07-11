#pragma once

class QSize;
class QPoint;
class QPointF;
class QWidget;
class QColor;
class QString;
class QPixmap;
class QLabel;

namespace Qt_Gen
{

// Stateless:

void setBackgroundColor(QWidget* widget, const QColor& color);
QSize sizePerc(double percentage, const QSize& ofSize);
int min(const QSize& size);
int max(const QSize& size);

// With state:

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
