#pragma once

#include <QWidget>

class TallRootWidget : public QWidget
{
    class QVBoxLayout* layout;

    void setupMenuBar();

    void setupChatsMenu();
    void setupMediaMenu();
    void setupSettingsMenu();
    void setupSearchMenu();

public:

    TallRootWidget(QWidget* parent);
};
