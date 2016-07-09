#pragma once

#include <QWidget>

class TallRootWidget : public QWidget
{
    class QVBoxLayout* layout;

    void setupMenuBar();
    class SlideWidget* slideWidget;

    void setupChatsMenu();
    void setupMediaMenu();
    void setupSettingsMenu();
    void setupSearchMenu();

public:

    TallRootWidget(QWidget* parent);
};
