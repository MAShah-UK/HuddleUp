#include "SlideWidgets.h"

#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QTimer>
#include <QtMath>

#include "SW_StyleVariant.h"
#include "Extra/Math_General.h"

void SlideWidgets::moveWidgets()
{
    if(!widgets.size()) return;

    // Multiplying the displacement by the scaling ratio should
    // prevent the widgets from sliding when scaling the main
    // widget, but it doesn't seem to work. It does for central.
    targetInterp.initialY *= TWDScalingRatio;
    targetInterp.finalY   *= TWDScalingRatio;
    totalInputDisp *= TWDScalingRatio;
    TWDScalingRatio = 1;

    // The logic is demonstrated in 'SW_StyleVariant Calculations.png'.
    totalInputDisp = Math::max(totalInputDisp, size().width() - totalWidgetDistance);

    // We don't want positive displacement as there will be a gap behind the first widget.
    totalInputDisp = Math::min(totalInputDisp, 0);

    sVariant->moveWidgets();
}

void SlideWidgets::resizeWidgets()
{
    if (!widgets.size()) return;

    for (QWidget* widget : widgets)
    {
        QSize OwidgetDim  = initialWidgetsSize[widgets.indexOf(widget)];

        double scaleFactor = sVariant->scaleFactor(widget);

        if (scaleFactor <= 1 ||
            shouldUpscale && scaleFactor > 1)
        {
            OwidgetDim *= scaleFactor;
            widget->resize(OwidgetDim);
        }
    }

    int newTotalWidgetDistance = sVariant->totalWidgetDistance();
    if (!totalWidgetDistance) totalWidgetDistance = newTotalWidgetDistance;
    TWDScalingRatio = (float)newTotalWidgetDistance / totalWidgetDistance;
    totalWidgetDistance = newTotalWidgetDistance;

    moveWidgets();
}

void SlideWidgets::removeWidget_internal(QWidget* target, bool shouldDelete)
{
    if (!target) return;

    removeWidget_impl(target, shouldDelete);
}

void SlideWidgets::removeWidget_impl(QWidget* target, bool shouldDelete)
{
    int targetIndex = widgets.indexOf(target);

    if (targetIndex > -1)
    {
        if (shouldDelete) delete widgets[targetIndex];
        widgets.removeOne(target);
        initialWidgetsSize.removeAt(targetIndex);
    }

    else
    {
        if (shouldDelete) delete widgets.last();
        widgets.removeLast();
        initialWidgetsSize.removeLast();
    }
}

void SlideWidgets::processInputDisplacement(int inputPos)
{
    int deltaPos = 0;

    if (!isDragging)
    {
        prevInputPos = inputPos;
        isDragging = true;
    }

    else
    {
        deltaPos = inputPos - prevInputPos;
        prevInputPos = inputPos;
    }

    if (totalInputDisp > 0 &&
        deltaPos >= 0) return;

    if (totalInputDisp <= -totalWidgetDistance &&
        deltaPos <= 0) return;

    totalInputDisp += deltaPos;

    moveWidgets();
}

bool SlideWidgets::event(QEvent* ev)
{
    switch(ev->type())
    {
    case QEvent::Type::Resize :
        resizeWidgets();
    break;

    case QEvent::Type::MouseButtonPress :
    case QEvent::Type::TouchBegin :

        // Every 10 ms we will record the mouse's position.
        // Less than 5 readings will result in no flick.
        // Otherwise the velocity is calculated.
        targetTimer->stop();
        inputPositions.clear();
        flickTimer->start(10);

    break;

    case QEvent::Type::MouseMove :
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(ev);
        currentInputPos = me->globalPos().x();
        processInputDisplacement(currentInputPos);
    }
    break;

    case QEvent::Type::TouchUpdate :
    {
        QTouchEvent* te = static_cast<QTouchEvent*>(ev);
        currentInputPos = te->touchPoints().first().screenPos().x();
        processInputDisplacement(currentInputPos);
    }
    break;

    case QEvent::Type::MouseButtonRelease :
    case QEvent::Type::TouchEnd :
        isDragging = false;
        flickTimer->stop();
        processFlick();
    break;

    default:
        return QWidget::event(ev);
    }

    return true;
}

void SlideWidgets::processFlick()
{
    if (inputPositions.size() < 5) return;

    for (double val : inputPositions)
    {
        qDebug() << val;
    }

    // We remove the first point since it will
    // be zeroed out in the loop below anyway.
    double initialPos = inputPositions.first();
    inputPositions.removeFirst();

    double avg = 0;

    // Convert absolute positions to change in position.
    for (double val : inputPositions)
    {
        avg += val - initialPos;
        //qDebug() << val - initialPos;
    }

    // Units are pixels per 40 ms.
    avg /= inputPositions.size();
   // qDebug() << "avg: " << avg;
    //qDebug() << "";

    avg *= 15;

    setTarget(Math::abs(totalInputDisp) - avg, 750);
}

void SlideWidgets::processTarget()
{
    totalInputDisp = -targetInterp.interpolatedValue(true, interpType);

    if (targetInterp.outOfRange())
    {
        targetTimer->stop();
        totalInputDisp = -targetInterp.finalY;
    }

    else if (totalInputDisp > 0 ||
            totalInputDisp < size().width() - totalWidgetDistance)
            targetTimer->stop();

    moveWidgets();
}

void SlideWidgets::recordInputPos()
{
    inputPositions.append(currentInputPos);
    if (inputPositions.size() > 5) inputPositions.removeFirst();
}

SlideWidgets::SlideWidgets(QWidget* parent, StyleVariant sVar)
{
    setParent(parent);
    setAttribute(Qt::WA_AcceptTouchEvents);

    setStyleVariant(sVar);

    targetTimer = new QTimer(this);
    connect(targetTimer, &QTimer::timeout, this, &SlideWidgets::processTarget);

    flickTimer = new QTimer(this);
    connect(flickTimer, &QTimer::timeout, this, &SlideWidgets::recordInputPos);
}

SlideWidgets::~SlideWidgets()
{
    clearWidgets();

    delete sVariant;
    delete targetTimer;
    delete flickTimer;
}

// If target is not specific the widget is inserted last in the queue
// if placeAfter is true, otherwise it's placed to the front of the queue.
void SlideWidgets::addWidget(QWidget* newWidget, bool placeAfter, QWidget* target)
{
    addWidget(QList<QWidget*>{newWidget}, placeAfter, target);
}

void SlideWidgets::addWidget(QList<QWidget*> newWidgets, bool placeAfter, QWidget* target)
{
    for (QWidget* newWidget : newWidgets)
    {
        // Returns -1 if item doesn't exist otherwise index is returned.
        int newWidgetIndex = widgets.indexOf(newWidget);

        // If a pre-existing widget is being supplied then
        // it's assumed that the client wants to reorder.
        if (newWidgetIndex != -1) removeWidget_internal(newWidget, false);

        int targetIndex = widgets.indexOf(target);

        // Offset is used to figure out where to insert the widget in the queue.
        int offset = 0;

        if (targetIndex > -1)
        {
            offset = targetIndex;
            if (placeAfter) offset += 1;
        }

        else
        {
            if (placeAfter) offset = widgets.size();
        }

        widgets.insert(offset, newWidget);
        initialWidgetsSize.insert(offset, newWidget->size());

        newWidget->setParent(this);
    }

    resizeWidgets();
}

// If no target then last widget is removed.
void SlideWidgets::removeWidget(QWidget* target, bool shouldDelete)
{
    if(widgets.empty()) return;

    removeWidget_impl(target, shouldDelete);

    resizeWidgets();
}

void SlideWidgets::clearWidgets(bool shouldDelete)
{
    if (shouldDelete)
    {
        for (QWidget* widget : widgets)
            delete widget;
    }

    widgets.clear();
    initialWidgetsSize.clear();
}

void SlideWidgets::setWidgetSize(QWidget* widget, const QSize& widgetSize)
{
    widget->resize(widgetSize);
    initialWidgetsSize[widgets.indexOf(widget)] = widgetSize;
}

void SlideWidgets::setSpacing(int spacing)
{
    if (spacing == widgetSpacing) return;
    widgetSpacing = spacing;
    resizeWidgets();
}

void SlideWidgets::setUpscaling(bool val)
{
    if (val == shouldUpscale) return;
    shouldUpscale = val;
    resizeWidgets();
}

void SlideWidgets::setStyleVariant(SlideWidgets::StyleVariant sVar)
{
    if (sVariant) delete sVariant;

    switch (sVar)
    {
    case StyleVariant::queue :
        sVariant = new SW_StyleVariant_Queue(this);
    break;

    case StyleVariant::single :
        sVariant = new SW_StyleVariant_Single(this);
    break;
    }
}

void SlideWidgets::setTarget(double displacement, int duration)
{
    /* x-axis is time, y-axis is displacement.
     * initialY is the totalInputDisp as the goal is to
     * make that the same as the target widgets x-postion.
     * This will result in the widget being placed in
     * the middle of the parent.
     */
    const int frameTime = 16;

    targetInterp.initialX   = frameTime;
    targetInterp.finalX     = duration;
    targetInterp.initialY   = Math::abs(totalInputDisp);
    targetInterp.finalY     = displacement;

    targetInterp.currentX   = frameTime;
    targetInterp.incrementX = frameTime;

    targetTimer->start(frameTime); // 60 FPS
}

void SlideWidgets::setTarget(QWidget* target, int duration)
{
    if (widgets.indexOf(target) == -1)
        return;

    qDebug() << totalInputDisp;
    double displacement = Math::abs(totalInputDisp) + target->pos().x() +
                          target->size().width()/2 - size().width()/2;

    setTarget(displacement, duration);
}

void SlideWidgets::setTarget(int index, int duration)
{
    if (index > widgets.size() - 1) return;
    setTarget(widgets[index], duration);
}

void SlideWidgets::setInterpolation(SlideWidgets::Interpolation interp)
{
    switch (interp)
    {
    case Interpolation::linear :
        interpType = MI::IT_linear;
    break;

    case Interpolation::sinusoidal :
        interpType = MI::IT_sinusoidal;
    break;

    case Interpolation::smooth :
        interpType = MI::IT_smooth;
    break;
    }
}
