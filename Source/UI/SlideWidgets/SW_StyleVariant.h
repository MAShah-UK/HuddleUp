#pragma once

#include <QList>

/*
 * There are the following style variants:
 * Queue  - displays all widgets in a queue.
 * Single - displays one widget at a time.
 */

class QWidget;
class QSize;
class SlideWidgets;

class SW_StyleVariant
{
protected:
    SlideWidgets* parent;

public:

    SW_StyleVariant(SlideWidgets* parent);
    virtual ~SW_StyleVariant() {}

    virtual void moveWidgets() = 0;
    virtual int totalWidgetDistance() = 0;
    virtual float scaleFactor(QWidget* widget) = 0;

};

class SW_StyleVariant_Queue : public SW_StyleVariant
{
public:
    SW_StyleVariant_Queue(SlideWidgets* parent);
    void moveWidgets() override;
    int totalWidgetDistance() override;
    float scaleFactor(QWidget* widget) override;
};

class SW_StyleVariant_Single : public SW_StyleVariant
{
public:
    SW_StyleVariant_Single(SlideWidgets* parent);
    void moveWidgets() override;
    int totalWidgetDistance() override;
    float scaleFactor(QWidget* widget) override;
};
