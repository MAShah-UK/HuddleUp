#pragma once

#include <QMainWindow>

class QStackedWidget;
class SlideWidgets;
class QWidget;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // Setup once from the constructor.

    QStackedWidget* rootWidget;
    SlideWidgets* slideWidget;
    QWidget* chatsWideWidget;
    QWidget* chatsTallWidget;
    QWidget* MediaWideWidget;
    QWidget* MediaTallWidget;
    QWidget* SettingsWideWidget;
    QWidget* SettingsTallWidget;
    QWidget* SearchWideWidget;
    QWidget* SearchTallWidget;

    QPixmap notifBackground;

    void setupNotifBackgound();
    void setupChatsWideLayout();
    void setupChatsTallLayout();
    void setupMediaWideLayout();
    void setupMediaTallLayout();
    void setupSettingsWideLayout();
    void setupSettingsTallLayout();

    void setBackgroundColor(QWidget* widget, const QColor& color);
    QLabel* createCustomLabel(const QPixmap &image, int screenPercent, Qt::Alignment alignment = Qt::AlignCenter);
    QLabel* createCustomLabel(const QString &path, int screenPercent, Qt::Alignment alignment = Qt::AlignCenter);

    // Events

    void resizeEvent(QResizeEvent *);
    bool event(QEvent* event);

    // Update when required.

    QSize windowSize;
    bool isWideScreen;

    // General functions.

    // Converts the requested window percentage to equivalent number of pixels.
    // If second argument not supplied will assume current size.
    int percToPix(double percent, QSize ofSize = QSize(0, 0));

public:

    MainWindow();
};
