#pragma once

#include <QLabel>
#include <QPen>

/*
 * An image path and mainText must be provided.
 * setup() must be called to visually update any changes.
 * subText is optional as are the styling and spacing options.
 */

class CaptionWidget : public QLabel
{
private:

    struct dimensions // Calculated.
    {
        QRect mainTextBR; // Bounding rect for both texts.
        QRect subTextBR;
        QSize radius;     // Distance from border to mainText.
        int mainAndSubText;
        int textBoxHeight;
    } dims;

    QImage imageIM;
    QImage textIM;

    QLabel* imageL;
    QLabel* textL;

public:

    QImage image;

    enum ESizeType
    {
        ST_Widget, // targetSize height will be total widget height.
        ST_Image     // targetSize height will be image height.
    } sizeType = ST_Widget;
    QSize targetSize = {-1, -1}; // The enum applies to the negative values here.

    enum EImageSizeResponse
    {
        ISR_Clip,
        ISR_Scale
    } imageSR = ISR_Scale;

    struct SpacingData
    {
        int imageText       = 0;
        int mainAndSubText  = 5;
    } spacing;

    struct DesignData
    {
        // Set curvature of border corners.
        QSize borderRadius = {-1, -1}; // <0 is automatic, radius scales with image.
        QPen borderPen     = {QBrush(QColor(0, 0, 0)), 7};
        QColor bgColor     = {255, 255, 255};
    private:
        friend class CaptionWidget;
        QSize _borderRadius; // The automatic calculated result.
    } imageDD, textDD; // These refer to the image and text areas of the widget.

    struct TextData
    {
        QFont font;
        QColor color = {0, 0, 0};
        QString text;
    } mainText, subText;

    // Main functions.

    void calculateDimensions();
    void loadScaledImage();
    void editImage(const QSize& size);
    void editText();
    void sortLabels();

    // Helpers.

    void calculateBorderRadius(DesignData& target, const QSize& source);
    void drawText(const TextData& textData, const QPoint& position);
    QLabel* setLabel(QLabel* label, const QImage* const imageDD, const QSize& size);
    void updateLabels();

    void drawBorder(QImage& target, const DesignData& design,
                    const QSize& targetSize, const QImage* const imageDD = nullptr);

    // Events

    void resizeEvent(QResizeEvent* re) override;

public:
    CaptionWidget(QWidget* parent = nullptr);
    void setup();
};
