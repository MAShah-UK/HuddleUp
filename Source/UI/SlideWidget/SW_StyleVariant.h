#pragma once

#include <QList>

#include "../../Helpers/Qt_General.h"

/*
 * There are the following style variants:
 * Queue  - displays all widgets in a queue.
 * Single - displays one widget at a time.
 */

class QWidget;
class QSize;
class SlideWidget;

class SW_StyleVariant
{
protected:

    SlideWidget* parent;
    Qt_Gen::DirectionalProperties& dirProp;

public:

    SW_StyleVariant(SlideWidget* parent);
    virtual ~SW_StyleVariant() {}

    virtual void  moveWidgets() = 0;
    virtual int   totalWidgetDistance() = 0;
    virtual void  processFlickDisp(double velocity) = 0;

};

class SW_StyleVariant_Queue : public SW_StyleVariant
{

public:
    SW_StyleVariant_Queue(SlideWidget* parent);
    void  moveWidgets() override;
    int   totalWidgetDistance() override;
    void  processFlickDisp(double velocity);
};

class SW_StyleVariant_Single : public SW_StyleVariant
{
public:
    SW_StyleVariant_Single(SlideWidget* parent);
    void  moveWidgets() override;
    int   totalWidgetDistance() override;
    void  processFlickDisp(double velocity) override;
};
