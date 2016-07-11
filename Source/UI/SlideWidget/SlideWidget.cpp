#include "SlideWidget.h"

#include <QMouseEvent>
#include <QTouchEvent>
#include <QTimer>
#include <QtMath>

#include "SW_StyleVariant.h"
#include "Helpers/Math_General.h"

void SlideWidget::moveWidgets()
{
    if(!widgets.size()) return;

    // Multiplying the displacement by the scaling ratio should
    // prevent the widgets from sliding when scaling the main
    // widget, but it doesn't seem to work. It does for central.
    targetInterp.initialY *= TWDScalingRatio;
    targetInterp.finalY   *= TWDScalingRatio;
    totalInputDisp *= TWDScalingRatio;
    TWDScalingRatio = 1;

    // Steps in 'SW_StyleVariant Calculations.png'.
    totalInputDisp = Math::max(totalInputDisp, dirProp(size()) - totalWidgetDistance);

    // We don't want positive displacement as there will be a gap behind the first widget.
    totalInputDisp = Math::min(totalInputDisp, 0);

    sVariant->moveWidgets();

    if (isDragging || isFlicking)
    {
        for (SlideWidget* SW : linkedSW)
        {
            double ratio = (double)Math::abs(totalInputDisp) / totalWidgetDistance;
            double ratioOther = (double)Math::abs(SW->totalInputDisp) / SW->totalWidgetDistance;

            if (Math::tolerance(ratio, ratioOther, 0.1))
                continue;

            SW->totalInputDisp = ratio * SW->totalWidgetDistance;
            SW->moveWidgets();
        }
    }
}

void SlideWidget::resizeWidgets()
{
    if (!widgets.size()) return;

    for (QWidget* widget : widgets)
    {
        const QSize& widgetDim = widget->size();
        QSize OwidgetDim  = initialWidgetsSize[widgets.indexOf(widget)];

        double scaleFactor = 0;

        // We need to scale based on the smaller gap to ensure that widgets fit.
        if (size().height() - widgetDim.height() < size().width() - widgetDim.width())
            scaleFactor = ((double)size().height() - 2*SWProps.spacing) /
                          OwidgetDim.height();
        else
            scaleFactor = ((double)size().width() - 2*SWProps.spacing) /
                          OwidgetDim.width();

        if (scaleFactor <= 1 ||
            SWProps.shouldUpscale && scaleFactor > 1)
        {
            OwidgetDim *= scaleFactor;
            widget->resize(OwidgetDim);
        }
    }

    // Attempt to reposition based on window scale factor.
    int newTotalWidgetDistance = sVariant->totalWidgetDistance();
    if (!totalWidgetDistance) totalWidgetDistance = newTotalWidgetDistance;
    TWDScalingRatio = (double)newTotalWidgetDistance / totalWidgetDistance;
    totalWidgetDistance = newTotalWidgetDistance;

    moveWidgets();
}

void SlideWidget::removeWidget_internal(QWidget* target, bool shouldDelete)
{
    if (!target) return;

    removeWidget_impl(target, shouldDelete);
}

void SlideWidget::removeWidget_impl(QWidget* target, bool shouldDelete)
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

void SlideWidget::processInputDisplacement(int inputPos)
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

bool SlideWidget::event(QEvent* ev)
{
    // If the current input position needs to be updated.
    switch(ev->type())
    {

    case QEvent::Type::MouseButtonPress :
    case QEvent::Type::MouseMove :
    {
         QMouseEvent* me = static_cast<QMouseEvent*>(ev);
         currentInputPos = dirProp(me->globalPos());
    }
    break;

    case QEvent::Type::TouchBegin :
    case QEvent::Type::TouchUpdate :
    {
         QTouchEvent* te = static_cast<QTouchEvent*>(ev);
         currentInputPos = dirProp(te->touchPoints().first().screenPos());
    }
    break;

    default:
         ;// Leave this here to prevent a 100+ warnings.
    }

    // Further handle input or other events.
    switch(ev->type())
    {

    case QEvent::Type::Resize :

        resizeWidgets();

        inputPositions.clear();
        processFlick();

    break;

    case QEvent::Type::MouseButtonPress :
    case QEvent::Type::TouchBegin :

        targetTimer->stop();
        inputPositions.clear();
        inputTimer->start(5);

    break;

    case QEvent::Type::MouseMove :
    case QEvent::Type::TouchUpdate :

         if (SWProps.isDraggable)
            processInputDisplacement(currentInputPos);

    break;

    case QEvent::Type::MouseButtonRelease :
    case QEvent::Type::TouchEnd :

         isDragging = false;
         inputTimer->stop();
         processFlick();

    break;

    default:
        return QWidget::event(ev);
    }

    return true;
}

double SlideWidget::inputVelocity()
{
    // We remove the first point since it will
    // be zeroed out in the loop below anyway.
    double initialPos = 0;
    if (inputPositions.size() > 0)
    {
        initialPos = inputPositions.first();
        inputPositions.removeFirst();
    }

    double avg = 0;

    // Convert absolute positions to change in position.
    for (double val : inputPositions)
        avg += val - initialPos;

    // Units are pixels per 20 ms.
    // Since four readings will usually be available.
    avg /= Math::max(inputPositions.size(), 1);

    // Units are pixels per second.
    avg /= 0.02;

    return avg;
}

void SlideWidget::processFlick()
{
    if (!SWProps.isFlickable) return;

    isFlicking = true;
    sVariant->processFlickDisp(inputVelocity() * SWProps.flickSensitivity);
}

void SlideWidget::processTarget()
{
    interpType = isFlicking ? MI::IT_exponential : MI::IT_smooth;

    totalInputDisp = -targetInterp.interpolatedValue(true, interpType);

    if (targetInterp.outOfXRange())
    {
        isFlicking = false;
        targetTimer->stop();

        totalInputDisp = -targetInterp.finalY;
    }

    else if (totalInputDisp > 0 ||
            totalInputDisp < dirProp(size()) - totalWidgetDistance)
    {
        isFlicking = false;
        targetTimer->stop();
    }

    moveWidgets();
}

void SlideWidget::recordInputPos()
{
    inputPositions.append(currentInputPos);
    if (inputPositions.size() > 5) inputPositions.removeFirst();
}

SlideWidget::SlideWidget(QWidget* parent, const SWProperties& SWProps)
    : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    targetTimer = new QTimer(this);
    connect(targetTimer, &QTimer::timeout, this, &SlideWidget::processTarget);

    inputTimer = new QTimer(this);
    connect(inputTimer, &QTimer::timeout, this, &SlideWidget::recordInputPos);

    properties(SWProps);
}

SlideWidget::~SlideWidget()
{
    clearWidgets();

    delete sVariant;
    delete targetTimer;
    delete inputTimer;
}

// If target is not specific the widget is inserted last in the queue
// if placeAfter is true, otherwise it's placed to the front of the queue.
void SlideWidget::addWidget(QList<QWidget*> newWidgets, bool placeAfter, QWidget* target)
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
            if (placeAfter) offset = widgets.size();

        widgets.insert(offset, newWidget);
        initialWidgetsSize.insert(offset, newWidget->size());

        newWidget->setParent(this);
    }

    resizeWidgets();
}

void SlideWidget::addWidget(std::initializer_list<QWidget*> newWidgets, bool placeAfter, QWidget* target)
{
    QList<QWidget*> widgets;
    widgets.reserve( newWidgets.size() );

    for (QWidget* widget : newWidgets)
        widgets.append(widget);

    addWidget(widgets, placeAfter, target);
}

void SlideWidget::addWidget(QWidget* newWidget, bool placeAfter, QWidget* target)
{
    addWidget(QList<QWidget*>{newWidget}, placeAfter, target);
}

// If no target then last widget is removed.
void SlideWidget::removeWidget(QWidget* target, bool shouldDelete)
{
    if(widgets.empty()) return;

    removeWidget_impl(target, shouldDelete);

    resizeWidgets();
}

void SlideWidget::clearWidgets(bool shouldDelete)
{
    if (shouldDelete)
    {
        for (QWidget* widget : widgets)
            delete widget;
    }

    widgets.clear();
    initialWidgetsSize.clear();
}

void SlideWidget::setWidgetSize(QWidget* widget, const QSize& widgetSize)
{
    widget->resize(widgetSize);
    initialWidgetsSize[widgets.indexOf(widget)] = widgetSize;
}

SWProperties SlideWidget::properties()
{
    return SWProps;
}

void SlideWidget::properties(const SWProperties& props)
{
    dirProp.setDirection(props.isHorizontal);

    if (props.styleVariant != SWProps.styleVariant ||
        !sVariant)
        setStyleVariant(props.styleVariant);

    SWProps = props;

    resizeWidgets();
}

void SlideWidget::setStyleVariant(SWProperties::StyleVariants sVar)
{
    if (sVariant) delete sVariant;

    switch (sVar)
    {
    case SWProperties::SV_Queue :
        sVariant = new SW_StyleVariant_Queue(this);
    break;

    case SWProperties::SV_Single :
        sVariant = new SW_StyleVariant_Single(this);
    break;
    }
}

void SlideWidget::setTarget(double displacement, int duration)
{
    /* x-axis is time, y-axis is displacement.
     * initialY is the totalInputDisp as the goal is to
     * make that the same as the target widgets x-postion.
     * This will result in the widget being placed in
     * the middle of the parent.
     */
    constexpr int frameTime = 16; // 60 FPS

    targetInterp.initialX   = frameTime;
    targetInterp.finalX     = duration;
    targetInterp.initialY   = abs(totalInputDisp);
    targetInterp.finalY     = Math::clamp(displacement, 0,
                              totalWidgetDistance - dirProp(size()) );

    targetInterp.currentX   = frameTime;
    targetInterp.incrementX = frameTime;

    inputTimer->stop();
    targetTimer->start(frameTime);
}

void SlideWidget::setTarget(QWidget* target, int duration)
{
    if (widgets.indexOf(target) == -1)
        return;

    double displacement = abs(totalInputDisp) + dirProp(target->pos()) +
                          dirProp(target->size())/2 - dirProp(size())/2;

    setTarget(displacement, duration);
}

void SlideWidget::setTarget(int index, int duration)
{
    if (index > widgets.size() - 1 ||
        index < 0)
       return;

    setTarget(widgets[index], duration);
}

void SlideWidget::linkSW(SlideWidget* other)
{
    if (linkedSW.contains(other)) return;
    linkedSW.append(other);
}

void SlideWidget::unlinkSW(SlideWidget* other)
{
    linkedSW.removeOne(other);
}
