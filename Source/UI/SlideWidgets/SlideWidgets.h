#pragma once

#include <QWidget>

#include "Extra/Math_General.h"

/*
 * This container allows inserting and sliding through widgets.
 * Widgets will automatically downscale to the SlideWidget's
 * dimensions. Optionally they can be upscaled.
 *
 * Consider the following:
 * - Fix: Find an alternative way to resize child widgets than forcing
 *        clients to use a member function.
 * - Fix: Widgets auto-sliding when SlideWidget is resized.
 * - Fix: processTarget distance will be incorreect when scaling during animation.
 * - Bug: scaling not correct - width issue? ✓
 * - Bug: vertical scaling is incorrect until window is vertically resized. ✓
 * - Add: rubber banding effect when user scrolls too far.
 * - Add: ability to flick widgets.
 * - Add: "setTarget" member function that scrolls to desired widget.
 * - Add: a scrollbar showing how far the user has scrolled.
 * - Add: fadeout effects.
 * - Add: different positioning styles (one at a time, one in the centre, etc).
 */

class SlideWidgets : public QWidget
{
    Q_OBJECT

    friend class SW_StyleVariant;
    friend class SW_StyleVariant_Queue;
    friend class SW_StyleVariant_Single;

    class SW_StyleVariant* sVariant = nullptr;
    QList<QWidget*> widgets;
    QList<QSize> initialWidgetsSize;

    void moveWidgets();
    float TWDScalingRatio = 0;
    int totalInputDisp = 0;

    void resizeWidgets();
    bool shouldUpscale = false;
    int widgetSpacing = 10;
    int totalWidgetDistance = 0;

    void removeWidget_internal(QWidget* target = nullptr, bool shouldDelete = true);
    void removeWidget_impl(QWidget* target = nullptr, bool shouldDelete = true);

    void processInputDisplacement(int inputPos);
    bool isDragging = false;
    int prevInputPos = 0;

    bool event(QEvent* ev);

    // For processTarget
    class QTimer* targetTimer = nullptr;
    QWidget* slideTarget = nullptr;
    Math::Interpolate targetInterp;

protected slots:
    void processTarget();

public:

    enum class StyleVariant {Queue, Single};

    SlideWidgets(QWidget* parent = nullptr, StyleVariant sVar = StyleVariant::Queue);
    ~SlideWidgets();

    void addWidget(QWidget* newWidget, bool placeAfter = true, QWidget* target = nullptr);
    void addWidget(QList<QWidget*> newWidgets, bool placeAfter = true, QWidget* target = nullptr);
    void removeWidget(QWidget* target = nullptr, bool shouldDelete = true);
    void clearWidgets(bool shouldDelete = true);
    void setWidgetSize(QWidget* widget, const QSize& widgetSize);
    void setSpacing(int spacing);
    void setUpscaling(bool val = true);
    void setStyleVariant(StyleVariant sVar = StyleVariant::Queue);
    void setTarget(QWidget* target, int duration = 500);
    void setTarget(int index, int duration = 500);
};
