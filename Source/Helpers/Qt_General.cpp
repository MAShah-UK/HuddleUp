#include "Qt_General.h"

#include <qDebug>
#include <QSize>
#include <QPoint>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>

// Stateless:

void Qt_Gen::setBackgroundColor(QWidget* widget, const QColor& color)
{
    QPalette p;
    p.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(p);
}

QSize Qt_Gen::sizePerc(double percentage, const QSize& ofSize)
{
    return {int( percentage/100 * ofSize.width() ),
            int( percentage/100 * ofSize.height() )};
}

int Qt_Gen::min(const QSize& size)
{
    return size.width() < size.height() ? size.width() : size.height();
}

int Qt_Gen::max(const QSize& size)
{
    return size.width() > size.height() ? size.width() : size.height();
}

QLabel* Qt_Gen::iconWidget(const QString& path, int length, int percentage, QWidget* parent)
{
    // Get image and perform calculations.

    int maxLength = length;
    if (percentage)
    {
        // If initially the widget is 10% of the window size, then we
        // expect it to be, at most, 10% of the available screen size.
        QSize maxSize = QApplication::desktop()->availableGeometry().size();
        maxLength     = Qt_Gen::max(maxSize) * percentage / 100;
    }

    QImage image(path);
    image = image.scaledToHeight(maxLength, Qt::SmoothTransformation);

    int offX = (maxLength - image.size().width())  / 2.0;
    int offY = (maxLength - image.size().height()) / 2.0;

    // Apply border to image.

    QPixmap canvas(maxLength, maxLength);
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPainterPath clip;
    clip.addRoundedRect(0, 0, maxLength, maxLength, maxLength/5, maxLength/5);

    QPainter paint(&canvas);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setClipPath(clip);
    paint.fillRect(0, 0, maxLength, maxLength, QColor(220, 220, 220));
    paint.drawImage(offX, offY, image);
    paint.setPen(QPen(QBrush(Qt::black), maxLength/100*3));
    paint.drawRoundedRect(0, 0, maxLength, maxLength, maxLength/5, maxLength/5);

    // Create widget to display it.

    QLabel* label = new QLabel(parent);
    label->setAlignment(Qt::AlignCenter);
    label->resize(maxLength, maxLength);
    label->setPixmap(canvas);
    label->setScaledContents(true);

    return label;
}

// DirectionalProperties

Qt_Gen::DirectionalProperties::DirectionalProperties(bool isHorizontal)
    : isHorizontal(isHorizontal)
{}

void Qt_Gen::DirectionalProperties::setDirection(bool isHorizontal)
{
    DirectionalProperties::isHorizontal = isHorizontal;
}

bool Qt_Gen::DirectionalProperties::isDirHorizontal()
{
    return isHorizontal;
}

int Qt_Gen::DirectionalProperties::operator() (const QSize& val)
{
    return isHorizontal ? val.width() : val.height();
}

int Qt_Gen::DirectionalProperties::operator() (const QPoint& val)
{
    return isHorizontal ? val.x() : val.y();
}

double Qt_Gen::DirectionalProperties::operator() (const QPointF& val)
{
    return isHorizontal ? val.x() : val.y();
}
