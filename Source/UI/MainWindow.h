#pragma once

#include <QMainWindow>

class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    class QStackedWidget* rootSW;
    class WideRootWidget* wideRW;
    class TallRootWidget* tallRW;

    QWidget* chatsWideWidget;
    QWidget* chatsTallWidget;
    QWidget* mediaWideWidget;
    QWidget* mediaTallWidget;
    QWidget* settingsWideWidget;
    QWidget* settingsTallWidget;
    QWidget* searchWideWidget;
    QWidget* searchTallWidget;

    bool event(QEvent* event);

public:

    MainWindow();
};
