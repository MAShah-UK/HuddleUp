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

    // General properties

    bool imageDisplay = true;
    bool textDisplay  = true;

    QWidget* parent = nullptr;
    QList<QString> imagePaths;
    QString textMain = "This is mainText";
    QString textSub = "This is subText";

    // Dimensions

    double maxScreenPercentage = 10.0; // Used to set resolution for the pixel map.
    QPoint imageBorderRadius   = {-1, -1}; // <0 is automatic, radius scales with image.
    QPoint textBorderRadius    = {-1, -1};
    int spacingImageText       = 5;
    int spacingMainAndSubText  = 5;

    // Colors/Design

    QPen imageBorderPen = {QBrush(QColor(0, 0, 0)), 5};
    QPen textBorderPen  = {QBrush(QColor(0, 0, 0)), 5};

    QFont textMainFont = QFont("arial", 50);
    QFont textSubFont;

    QColor imageBGColor  = {220, 220, 220};
    QColor textBGColor   = {250, 250, 250};
    QColor mainTextColor = {0, 0, 0};
    QColor subTextColor  = {50, 50, 50};
};


class CaptionWidget
{
    CWProperties CWProps;

    QImage loadScaledImage(int index);
    QPixmap editImage(const QImage& image);
    QPixmap addText(const QPixmap& pixmap);
    QLabel* createLabel(const QPixmap& pixmap);

public:
    CaptionWidget(const CWProperties& CWProps);
    CWProperties properties();
    void properties(const CWProperties& CWProps);
    QLabel* operator()(int index = 0);
};
