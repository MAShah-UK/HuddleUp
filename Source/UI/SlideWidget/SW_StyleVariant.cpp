#include "SW_StyleVariant.h"

#include <QWidget>
#include <QtMath>

#include "SlideWidget.h"
#include "Helpers/Math_General.h"

// SW_StyleVariant

SW_StyleVariant::SW_StyleVariant(SlideWidget* parent)
    : parent(parent), dirProp(parent->dirProp)
{}

// SW_StyleVariant_Queue

SW_StyleVariant_Queue::SW_StyleVariant_Queue(SlideWidget* parent)
    : SW_StyleVariant(parent)
{}

void SW_StyleVariant_Queue::moveWidgets()
{
    int prev_x = parent->totalInputDisp;

    for (QWidget* widget : parent->widgets)
    {
        if (parent->dirProp.isDirHorizontal())
            widget->move(prev_x, parent->size().height()/2 - widget->size().height()/2);
        else
            widget->move(parent->size().width()/2 - widget->size().width()/2, prev_x);

        prev_x += dirProp(widget->size()) + parent->SWProps.spacing;
    }
}

int SW_StyleVariant_Queue::totalWidgetDistance()
{
    // To offset the extra spacing introduced in the loop.
    int newTotalWidgetDistance = -parent->SWProps.spacing;

    for (QWidget* widget : parent->widgets)
        newTotalWidgetDistance += dirProp(widget->size()) + parent->SWProps.spacing;

    return newTotalWidgetDistance;
}

void SW_StyleVariant_Queue::processFlickDisp(double velocity)
{
    if (Math::tolerance(velocity, 0, 1))
        return;

    parent->setTarget(Math::abs(parent->totalInputDisp) - velocity,
                      parent->SWProps.flickDuration);
}

// SW_StyleVariant_Single

SW_StyleVariant_Single::SW_StyleVariant_Single(SlideWidget* parent)
    : SW_StyleVariant(parent)
{}

void SW_StyleVariant_Single::moveWidgets()
{
    int index = 0;
    for (QWidget* widget : parent->widgets)
    {
        int prev_x = parent->totalInputDisp + (index + 0.5)*dirProp(parent->size()) -
                     dirProp(widget->size())/2;

        if (parent->dirProp.isDirHorizontal())
            widget->move(prev_x, parent->size().height()/2 - widget->size().height()/2);
        else
            widget->move(parent->size().width()/2 - widget->size().width()/2, prev_x);

        ++index;
    }
}

int SW_StyleVariant_Single::totalWidgetDistance()
{
    // Each widget will occupy its own block, and each block has the same size.
    return parent->widgets.size() * dirProp(parent->size());
}

void SW_StyleVariant_Single::processFlickDisp(double velocity)
{
    double targetWidget = (double)abs(parent->totalInputDisp) /
                          dirProp(parent->size()) + 0.5;

    int multiplier = 0;

    if      (velocity < -1)
             multiplier = 1;
    else if (velocity > 1)
             multiplier = -1;

    targetWidget += 0.5 * multiplier;

    parent->setTarget((int)targetWidget, parent->SWProps.flickDuration);
}
