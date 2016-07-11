#pragma once

#include <QLabel>

class QLabel;
class CWProperties;
class QWidget;

class CWProperties
{
public:

    // Must provide.
    QWidget* parent;
    QList<QString> paths;
    int length; // Used only when maxScreenPercentage is set to 0.
    double maxScreenPercentage; // Used to set resolution for the pixel map.

    // Optional.
};


class CaptionWidget
{
public:
    QLabel* operator()(const CWProperties& CWProps, int index = 0, QWidget* parent = nullptr);
};
