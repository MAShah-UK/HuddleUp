#include "CaptionWidget.h"

#include <QLabel>
#include <QApplication>
#include <QPalette>
#include <QDesktopWidget>
#include <QDebug>
#include <QPainter>

#include "../../Helpers/Qt_General.h"

#include "../../Helpers/Math_General.h"

QLabel* CaptionWidget::operator()(const CWProperties& CWProps, int index, QWidget* parent)
{
    // Get image and perform calculations.

    int maxLength = CWProps.length;
    if (!Math::tolerance(CWProps.maxScreenPercentage, 0, 0.1))
    {
        // If initially the widget is 10% of the window size, then we
        // expect it to be, at most, 10% of the available screen size.
        QSize maxSize = QApplication::desktop()->availableGeometry().size();
        maxLength     = Qt_Gen::max(maxSize) * CWProps.maxScreenPercentage / 100;
    }

    QImage image(CWProps.paths.at(index));
    if (image.width() > image.height())
        image = image.scaledToWidth(maxLength, Qt::SmoothTransformation);
    else
        image = image.scaledToHeight(maxLength, Qt::SmoothTransformation);

    int offX = (maxLength - image.size().width())  / 2.0;
    int offY = (maxLength - image.size().height()) / 2.0;

    // Apply border to image.

    QPixmap canvas(image.size.width(), image.size().height());
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

    QLabel* label = new QLabel(CWProps.parent);
    label->setAlignment(Qt::AlignCenter);
    label->resize(maxLength, maxLength);
    label->setPixmap(canvas);
    label->setScaledContents(true);

    return label;
}
