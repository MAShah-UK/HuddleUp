#pragma once

#include <QLabel>
#include <QPainter>

class QLabel;
class CWProperties;
class QWidget;
class QImage;

class CWProperties
{
public:

    QWidget* parent = nullptr;
    QList<QString> imagePaths;

    double maxScreenPercentage = 10.0; // Used to set resolution for the pixel map.
    int spacingImageText       = 5;
    int spacingMainAndSubText  = 5;

    struct DesignData
    {
        QSize borderRadius = {-1, -1}; // <0 is automatic, radius scales with image.
        QPen borderPen      = {QBrush(QColor(0, 0, 0)), 5};
        QColor bgColor      = {220, 220, 220};
    } image, text; // These refer to the image and text areas of the widget.

    struct TextData
    {
        QFont font;
        QColor color = {0, 0, 0};
        QString text = "Sample text.";
    } mainText, subText;
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
                    const QRectF& border, const QImage* const image = nullptr);

public:
    CaptionWidget(const CWProperties& CWProps);
    CWProperties properties();
    void properties(const CWProperties& CWProps);
    QLabel* operator()(int index = 0);
};
