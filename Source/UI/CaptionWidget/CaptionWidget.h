#pragma once

#include <QLabel>
#include <QPen>

/*
 * To use this widget generator create an instance of CWProperties and set the
 * parent widget and the path to the image file. Then set that maxScreenPercentage
 * that you expect the image to take when the application is maximised. Set the other
 * required data. Create an instance of CaptionWidget and pass the CWProperties
 * instance via the constructor. Use the () operator to output a label.
 *
 * An image path and mainText must be provided.
 * maxScreenPercentage and parent should be carefuelly considered.
 * subText is optional as are the styling and spacing options.
 *
 * This class requires a valid reference to CWProperties to function.
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

    QString imagePath;

    enum ESizeType
    {
        ST_Absolute, // Total size will be set size.
        ST_Image     // Image will be at set size.
    } sizeType = ST_Absolute;
    QSize targetSize = {-1, -1}; // The enum applies to the negative values here.

    struct SpacingData
    {
        int imageText       = 0;
        int mainAndSubText  = 5;
    } spacing;

    struct DesignData
    {
        QSize borderRadius = {-1, -1}; // <0 is automatic, radius scales with image.
        QPen borderPen     = {QBrush(QColor(0, 0, 0)), 7};
        QColor bgColor     = {220, 220, 220};
    private:
        friend class CaptionWidget;
        QSize _borderRadius; // The calculated result.
    } image, text; // These refer to the image and text areas of the widget.

    struct TextData
    {
        QFont font;
        QColor color = {0, 0, 0};
        QString text;
    } mainText, subText;

    // Main functions.

    void calculateDimensions();
    QImage loadScaledImage();
    void editImage(const QImage& image);
    void editText();
    void sortLabels();

    // Helpers.

    void calculateBorderRadius(DesignData& target, const QSize& source);
    void drawText(const TextData& textData, const QPoint& position);
    QLabel* setLabel(QLabel* label, const QImage* const image, const QSize& size);

    void drawBorder(QImage& target, const DesignData& design,
                    const QSize& targetSize, const QImage* const image = nullptr);

    // Events

    void resizeEvent(QResizeEvent* re) override;

public:
    CaptionWidget(QWidget* parent = nullptr);
    void setup();
};
