#pragma once

#include <QWidget>

// Change to this when SlideWidgets is complete
// and seperate copy is stored from project for
// future use.
// #include "Extra/Math_General.h"

#include "../../Extra/Math_General.h"

/*
 * This container allows inserting and sliding through widgets.
 * Widgets will automatically downscale to the SlideWidget's
 * dimensions. Optionally they can be upscaled.
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
    int totalInputDisp = -100; // Reset to 0.

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
    using MI = Math::Interpolate;
    MI targetInterp;
    MI::interpType interpType = MI::IT_smooth;

    void processFlick();
    QTimer* flickTimer = nullptr;
    double currentInputPos = 0;
    QList<double> inputPositions;

protected slots:

    void processTarget();
    void recordInputPos();

public:

    enum class StyleVariant  {queue, single};
    enum class Interpolation {linear, sinusoidal, smooth};

    SlideWidgets(QWidget* parent = nullptr, StyleVariant sVar = StyleVariant::queue);
    ~SlideWidgets();

    void addWidget(QWidget* newWidget, bool placeAfter = true, QWidget* target = nullptr);
    void addWidget(QList<QWidget*> newWidgets, bool placeAfter = true, QWidget* target = nullptr);
    void removeWidget(QWidget* target = nullptr, bool shouldDelete = true);
    void clearWidgets(bool shouldDelete = true);
    void setWidgetSize(QWidget* widget, const QSize& widgetSize);
    void setSpacing(int spacing);
    void setUpscaling(bool val = true);
    void setStyleVariant(StyleVariant sVar = StyleVariant::queue);
    void setTarget(double displacement, int duration = 500);
    void setTarget(QWidget* target, int duration = 500);
    void setTarget(int index, int duration = 500);
    void setInterpolation(Interpolation interp);
};
