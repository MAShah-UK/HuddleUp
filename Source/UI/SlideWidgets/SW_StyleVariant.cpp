#include "SW_StyleVariant.h"

#include <QWidget>
#include <QtMath>
#include <QDebug>

#include "SlideWidgets.h"
#include "Extra/Math_General.h"

// SW_StyleVariant

SW_StyleVariant::SW_StyleVariant(SlideWidgets* parent)
    : parent(parent)
{}

// SW_StyleVariant_Queue

SW_StyleVariant_Queue::SW_StyleVariant_Queue(SlideWidgets* parent)
    : SW_StyleVariant(parent)
{
    flickTime = 750;
}

void SW_StyleVariant_Queue::moveWidgets()
{
    int prev_x = parent->totalInputDisp;

    for (QWidget* widget : parent->widgets)
    {
        widget->move(prev_x, parent->size().height()/2 - widget->size().height()/2);
        prev_x += widget->size().width() + parent->widgetSpacing;
    }
}

int SW_StyleVariant_Queue::totalWidgetDistance()
{
    // To offset the extra spacing introduced in the loop.
    int newTotalWidgetDistance = -parent->widgetSpacing;

    for (QWidget* widget : parent->widgets)
    {
        newTotalWidgetDistance += widget->size().width() + parent->widgetSpacing;
    }

    return newTotalWidgetDistance;
}

void SW_StyleVariant_Queue::processFlickDisp(double velocity)
{
    parent->setTarget( (double)abs(parent->totalInputDisp) - velocity*1.5,
                      flickTime);
}

// SW_StyleVariant_Single

SW_StyleVariant_Single::SW_StyleVariant_Single(SlideWidgets* parent)
    : SW_StyleVariant(parent)
{
    flickTime = 250;
}

void SW_StyleVariant_Single::moveWidgets()
{
    int index = 0;
    for (QWidget* widget : parent->widgets)
    {
        int prev_x = parent->totalInputDisp + (index + 0.5)*parent->size().width() -
                     widget->size().width()/2;
        widget->move(prev_x, parent->size().height()/2 - widget->size().height()/2);
        ++index;
    }
}

int SW_StyleVariant_Single::totalWidgetDistance()
{
    // Each widget will occupy its own block - each block has the same size.
    return parent->widgets.size() * parent->size().width();
}

void SW_StyleVariant_Single::processFlickDisp(double velocity)
{
    double targetWidget = (double)abs(parent->totalInputDisp) /
                          parent->size().width() + 0.5;

    int multiplier = 0;

    if      (velocity < -1)
             multiplier = 1;
    else if (velocity > 1)
             multiplier = -1;

    targetWidget += 0.5 * multiplier;

        qDebug() << targetWidget;

    parent->setTarget((int)targetWidget, flickTime);
}
