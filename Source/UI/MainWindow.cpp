#include "MainWindow.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QLabel>
#include <QPainter>
#include <QScreen>

#include "SlideWidgets/SlideWidgets.h"

MainWindow::MainWindow()
{
    rootWidget = new QStackedWidget(this);
    setCentralWidget(rootWidget);

    // windowSize = {geometry().width(), geometry().height()};
    isWideScreen = geometry().width() > geometry().height() ? true : false;

    //setupNotifBackgound();
    //setupChatsWideLayout();
    //setupChatsTallLayout();

    // Call resizeEvent to ensure correct layout.
    resize(geometry().width(), geometry().height());

    resize(300, 200);

    slideWidget = new SlideWidgets(this, SlideWidgets::StyleVariant::queue);

    QWidget* W1 = new QWidget();
    QWidget* W2 = new QWidget();
    QWidget* W3 = new QWidget();
    QWidget* W4 = new QWidget();
    QWidget* W5 = new QWidget();

    setBackgroundColor(W1, Qt::red);
    setBackgroundColor(W2, Qt::blue);
    setBackgroundColor(W3, Qt::green);
    setBackgroundColor(W4, Qt::magenta);
    setBackgroundColor(W5, Qt::cyan);

    W1->resize(150, 500);
    W2->resize(440, 200);
    W4->resize(150, 300);
    W5->resize(200, 250);

    slideWidget->addWidget(QList<QWidget*>{W1, W2, W3, W4, W5});
    slideWidget->setUpscaling();
    slideWidget->setSpacing(20);

    rootWidget->addWidget(slideWidget);
}

void MainWindow::setupNotifBackgound()
{
    int perc_20 = percToPix(20);

    notifBackground = QPixmap(perc_20, perc_20);
    notifBackground.fill(QColor(0, 0, 0, 0));

    QPainter painter(&notifBackground);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, perc_20, perc_20);

    painter.setPen(QPen(QBrush(Qt::white), 0));
    painter.setFont(QFont("Times", perc_20, 400));
    painter.drawText(QRect(0, 0, perc_20, perc_20), "0", QTextOption(Qt::AlignCenter));
}

void MainWindow::setupChatsWideLayout()
{
    // Create and add layout to stacked widget.
    chatsWideWidget = new QWidget(this);
    setBackgroundColor(chatsWideWidget, Qt::red);

    rootWidget->addWidget(chatsWideWidget);

}

void MainWindow::setupChatsTallLayout()
{
    // Create and add layout to stacked widget.
    chatsTallWidget = new QWidget(this);
    setBackgroundColor(chatsTallWidget, Qt::white);

    rootWidget->addWidget(chatsTallWidget);

    // Customise the tall layout.

    // Set menu bar.
    QLabel* chatsIcon    = createCustomLabel(":/icons/menuBar_Chats.png"   , 10);
    QLabel* mediaIcon    = createCustomLabel(":/icons/menuBar_Media.png"   , 10);
    QLabel* settingsIcon = createCustomLabel(":/icons/menuBar_Settings.png", 10);
    QLabel* searchIcon   = createCustomLabel(":/icons/menuBar_Search.png"  , 10);

    QHBoxLayout* menuBar = new QHBoxLayout;
    menuBar->addWidget(chatsIcon);
    menuBar->addWidget(mediaIcon);
    menuBar->addWidget(settingsIcon);
    menuBar->addWidget(searchIcon);

    // Set menu notifications bar.

    QLabel* chatsNotifs = createCustomLabel(notifBackground, 5);

    // Set up final layout.
    QVBoxLayout* tallRootLayout = new QVBoxLayout(chatsTallWidget);
    tallRootLayout->addLayout(menuBar);
    tallRootLayout->addWidget(chatsNotifs);
}

void MainWindow::setupMediaWideLayout()
{

}

void MainWindow::setupMediaTallLayout()
{

}

void MainWindow::setupSettingsWideLayout()
{

}

void MainWindow::setupSettingsTallLayout()
{

}

void MainWindow::setBackgroundColor(QWidget * widget, const QColor& color)
{
    QPalette p;
    p.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(p);
}

QLabel* MainWindow::createCustomLabel(const QPixmap& image, int screenPercent, Qt::Alignment alignment)
{
    QLabel* iconLabel = new QLabel;

    // Note: Might not need this line. I'm assuming this anti-aliases the image.
    iconLabel->setPixmap(image.scaledToHeight(percToPix(screenPercent), Qt::SmoothTransformation));
    iconLabel->setAlignment(alignment);
    iconLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    return iconLabel;
}

QLabel* MainWindow::createCustomLabel(const QString &path, int screenPercent, Qt::Alignment alignment)
{
    return createCustomLabel(QPixmap(path), screenPercent, alignment);
}

void MainWindow::resizeEvent(QResizeEvent * resizeEvent)
{
    QSize windowSize = resizeEvent->size();
    double windowRatio = (double)windowSize.width() / windowSize.height();

    // > 1 wide-screen, < 1 tall screen.
    if (windowRatio > 1)
    {
       // rootWidget->setCurrentWidget(chatsWideWidget);
    }
    else
    {
       // rootWidget->setCurrentWidget(chatsTallWidget);
    }
}

bool MainWindow::event(QEvent* event)
{
    switch (event->type())
    {

    case QEvent::Type::KeyPress :
        slideWidget->setTarget(2, 2000);
    break;

    default:
        QWidget::event(event);
    }

    return true;
}

int MainWindow::percToPix(double percent, QSize ofSize)
{
    if (ofSize.height() == 0)
    {
        return isWideScreen ? percent/100 * geometry().width() :
                              percent/100 * geometry().height();
    }

    else
    {
        return isWideScreen ? percent/100 * ofSize.width() :
                              percent/100 * ofSize.height();
    }
}



