#pragma once

#include <QLabel>
#include <QPainter>

class QLabel;
class CWProperties;
class QWidget;
class QImage;

/*
 * To use this widget generator create an instance of CWProperties and set the
 * parent widget and the paths to the image files. Then set that maxScreenPercentage
 * that you expect the image to take when the application is maximised. Set the other
 * required data. Create an instance of CaptionWidget and pass the CWProperties
 * instance via the constructor. Use the () operator with the appropriate index to
 * output a finalised QLabel representing the caption widget.
 *
 * An image path and mainText must be provided.
 * maxScreenPercentage and parent should be carefuelly considered.
 * subText is optional as are the styling and spacing options.
 */

class CWProperties
{
public:

    QWidget* parent;
    QList<QString> imagePaths;

    double maxScreenPercentage = 10.0; // Sets the resolution for the imported image.
    int spacingImageText       = 0;
    int spacingMainAndSubText  = 5;

    struct DesignData
    {
        QSize borderRadius  = {-1, -1}; // <0 is automatic, radius scales with image.
        QPen borderPen      = {QBrush(QColor(0, 0, 0)), 7};
        QColor bgColor      = {220, 220, 220};
    } image, text; // These refer to the image and text areas of the widget.

    struct TextData
    {
        QFont font;
        QColor color = {0, 0, 0};
        QString text;
    } mainText, subText;

    CWProperties(QWidget* parent = nullptr);
};


class CaptionWidget
{
    CWProperties CWProps;

    // Main functions.

    QImage loadScaledImage(int index);
    QPixmap editImage(const QImage& image);
    QPixmap addText(const QPixmap& pixmap);
    QLabel* createLabel(const QPixmap& pixmap);

    // Helpers.

    void calculateBorderRadius(QSize& target, const QSize& source);

    void drawText(QPainter& painter, const CWProperties::TextData& textData,
                  const QPoint& position);

    void drawBorder(QPainter& painter, const CWProperties::DesignData& design,
                    const QRect& border, const QImage* const image = nullptr);

public:
    CaptionWidget(const CWProperties& CWProps);
    CWProperties properties();
    void properties(const CWProperties& CWProps);
    QLabel* operator()(int index = 0);
};
