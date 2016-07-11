#pragma once

#include <QWidget>

// Change to this when SlideWidget is complete
// and seperate copy is stored from project for
// future use.
// #include "Helpers/Math_General.h"

#include "../../Helpers/Math_General.h"
#include "../../Helpers/Qt_General.h"

/*
 * This container allows inserting and sliding through widgets.
 * Widgets will automatically downscale to the SlideWidget's
 * dimensions. Optionally they can be upscaled.
 */

class SWProperties
{
public:

    enum StyleVariants {SV_Queue, SV_Single};

    bool   shouldUpscale       = false; // Maintains aspect ratio.
    bool   isHorizontal        = true;  // False lays widgets vertically.
    bool   isDraggable         = true;  // False here also means not flickable.
    bool   isFlickable         = true;
    int    flickDuration       = 400;   // msec
    int    spacing             = 10;    // pixels
    double flickSensitivity    = 1.0;   // < 1 slower, > 1 faster
    StyleVariants styleVariant = SV_Queue;
};

class SlideWidget : public QWidget
{
    Q_OBJECT

    friend class SW_StyleVariant;
    friend class SW_StyleVariant_Queue;
    friend class SW_StyleVariant_Single;

    class SW_StyleVariant* sVariant = nullptr;
    Qt_Gen::DirectionalProperties dirProp;
    QList<QWidget*> widgets;
    QList<QSize> initialWidgetsSize;
    SWProperties SWProps;

    void setStyleVariant(SWProperties::StyleVariants sVar);

    void moveWidgets();
    float TWDScalingRatio = 0;
    int   totalInputDisp  = 0;

    void resizeWidgets();
    int  totalWidgetDistance = 0;

    void removeWidget_internal(QWidget* target = nullptr, bool shouldDelete = true);
    void removeWidget_impl(QWidget* target = nullptr, bool shouldDelete = true);

    void processInputDisplacement(int inputPos);
    bool isDragging   = false;
    int  prevInputPos = 0;

    bool event(QEvent* ev);

    // For processTarget
    class QTimer* targetTimer = nullptr;
    using MI = Math::Interpolate;
    MI targetInterp;
    MI::interpType interpType;
    bool isFlicking = false;

    double inputVelocity();

    void processFlick();
    QTimer* inputTimer = nullptr;
    double currentInputPos = 0;
    QList<double> inputPositions;

    // For linked SWs.
    QList<SlideWidget*> linkedSW;

protected slots:

    void processTarget();
    void recordInputPos();

public:

    SlideWidget(QWidget* parent = nullptr, const SWProperties& SWProps = SWProperties());
    ~SlideWidget();

    void addWidget(QList<QWidget*> newWidgets, bool placeAfter = true, QWidget* target = nullptr);
    void addWidget(std::initializer_list<QWidget*> newWidgets, bool placeAfter = true, QWidget* target = nullptr);
    void addWidget(QWidget* newWidget, bool placeAfter = true, QWidget* target = nullptr);
    void removeWidget(QWidget* target = nullptr, bool shouldDelete = true);
    void clearWidgets(bool shouldDelete = true);
    void setWidgetSize(QWidget* widget, const QSize& widgetSize);
    SWProperties properties();
    void properties(const SWProperties& props);
    void setTarget(double displacement, int duration = 500);
    void setTarget(QWidget* target, int duration = 500);
    void setTarget(int index, int duration = 500);
    void linkSW(SlideWidget* other);
    void unlinkSW(SlideWidget* other);
};
