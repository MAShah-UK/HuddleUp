#include "MainWindow.h"

#include <qDebug>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QResizeEvent>

#include "WideUI/WideRootWidget.h"
#include "TallUI/TallRootWidget.h"

MainWindow::MainWindow()
{
    // Setup container widgets first.

    rootSW = new QStackedWidget(this);
    setCentralWidget(rootSW);

    wideRW = new WideRootWidget(rootSW);
    tallRW = new TallRootWidget(rootSW);

    rootSW->addWidget(wideRW);
    rootSW->addWidget(tallRW);

    // Call resizeEvent to ensure correct layout.
    resize(geometry().width(), geometry().height());
}

bool MainWindow::event(QEvent* event)
{
    switch (event->type())
    {

    case QEvent::Type::Resize :
    {
        QSize windowSize   = static_cast<QResizeEvent*>(event)->size();
        double windowRatio = (double)windowSize.width() / windowSize.height();

        // > 1 wide-screen, < 1 tall screen.
        if (windowRatio > 1)
            rootSW->setCurrentWidget(tallRW); // Change back to wideRW when done testing.
        else
            rootSW->setCurrentWidget(tallRW);
    }
    break;

    default:
        QWidget::event(event);
    }

    return true;
}



