#include "Qt_General.h"

#include <qDebug>
#include <QSize>
#include <QPoint>
#include <QPalette>
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

// Stateless:

void Qt_Gen::setBackgroundColor(QWidget* widget, const QColor& color)
{
    QPalette p;
    p.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(p);
}

int Qt_Gen::min(const QSize& size)
{
    return size.width() < size.height() ? size.width() : size.height();
}

int Qt_Gen::max(const QSize& size)
{
    return size.width() > size.height() ? size.width() : size.height();
}

int Qt_Gen::random(int low, int high, int seed)
{
    /* This function will return the same value if the low, high, and seed
     * values are unchanged between calls. This is likely to happen when
     * the function is called within the same milisecond in realtime.
     */

    qsrand(seed);
    return (qrand() % (high - low + 1)) + low;
}

QSize Qt_Gen::sizePerc(double percentage)
{
    static QSize maxSize = QSize(0, 0);

    if (maxSize.isNull())
    {
        maxSize = QApplication::desktop()->availableGeometry().size();

        for (int i = 2; i <= QApplication::desktop()->screenCount(); ++i)
        {
            if (max(QApplication::desktop()->availableGeometry(i).size()) > max(maxSize))
                maxSize = QApplication::desktop()->availableGeometry(i).size();
        }
    }

    return percentage/100 * maxSize;
}

QSize Qt_Gen::sizePerc(double percentage, const QSize &ofSize)
{
    return percentage/100 * ofSize;
}

QSize Qt_Gen::sizePerc(const QPoint& percentages, const QSize& ofSize)
{
    return {int( percentages.x()/100 * ofSize.width() ),
            int( percentages.y()/100 * ofSize.height() )};
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

QElapsedTimer Qt_Gen::DebugTimer::timer;

void Qt_Gen::DebugTimer::tick()
{
    timer.start();
}

void Qt_Gen::DebugTimer::tock()
{
    qDebug() << "Time elapsed: " << timer.elapsed() << " ms.";
}

void Qt_Gen::tick() { DebugTimer::tick(); }
void Qt_Gen::tock() { DebugTimer::tock(); }
