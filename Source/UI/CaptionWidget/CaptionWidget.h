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

    QList<QString> imagePaths;
    QWidget* parent = nullptr;

    QString mainText;
    QString subText;



    // Dimensions

    double maxScreenPercentage = 10.0; // Used to set resolution for the pixel map.
    QPoint imageBorderRadius   = {-1, -1}; // <0 is automatic, radius scales with image.
    QPoint textBorderRadius    = {-1, -1};
    int imageTextSpacing       = 10;

    // Colors/Design

    QPen imageBorderPen = {QBrush(QColor(0, 0, 0)), 5};
    QPen textBorderPen  = {QBrush(QColor(0, 0, 0)), 5};

    QColor imageBGColor  = {220, 220, 220};
    QColor textBGColor   = {250, 250, 250};
    QColor mainTextColor = {50, 50, 50};
    QColor subTextColor  = {75, 75, 75};
};


class CaptionWidget
{
    CWProperties CWProps;

    QImage loadScaledImage(int index);
    QPixmap editImage(QImage& image);
    QLabel* createLabel(QPixmap& pixmap);

public:
    CaptionWidget(const CWProperties& CWProps);
    CWProperties properties();
    void properties(const CWProperties& CWProps);
    QLabel* operator()(int index = 0);
};
