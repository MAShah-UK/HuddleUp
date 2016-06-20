#include "SW_StyleVariant.h"

#include <QWidget>

#include "SlideWidgets.h"
#include "Extra/Math_General.h"

// SW_StyleVariant

SW_StyleVariant::SW_StyleVariant(SlideWidgets* parent)
    : parent(parent)
{}

// SW_StyleVariant_Queue

SW_StyleVariant_Queue::SW_StyleVariant_Queue(SlideWidgets* parent)
    : SW_StyleVariant(parent)
{}

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

float SW_StyleVariant_Queue::scaleFactor(QWidget* widget)
{
    return ((double)parent->size().height() - 2*parent->widgetSpacing) /
           parent->initialWidgetsSize[parent->widgets.indexOf(widget)].height();
}

// SW_StyleVariant_Central

SW_StyleVariant_Single::SW_StyleVariant_Single(SlideWidgets* parent)
    : SW_StyleVariant(parent)
{}

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

float SW_StyleVariant_Single::scaleFactor(QWidget* widget)
{
    float scaleFactor = 1;

    const QSize& dim  = parent->size();
    const QSize& widgetDim = widget->size();
    const QSize& OwidgetDim  = parent->initialWidgetsSize[parent->widgets.indexOf(widget)];
    int widgetSpacing = parent->widgetSpacing;

    // We need to scale based on the smaller gap to fit the widget.
    if (dim.height() - widgetDim.height() < dim.width() - widgetDim.width())
        scaleFactor = ((double)dim.height() - 2*widgetSpacing) /
                      OwidgetDim.height();
    else
        scaleFactor = ((double)dim.width() - 2*widgetSpacing) /
                      OwidgetDim.width();

    return scaleFactor;
}
