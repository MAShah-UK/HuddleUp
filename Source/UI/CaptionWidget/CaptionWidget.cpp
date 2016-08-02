#include "CaptionWidget.h"

#include <QLabel>
#include <QPalette>
#include <QDebug>

#include "../../Helpers/Qt_General.h"

#include "../../Helpers/Math_General.h"

QImage CaptionWidget::loadScaledImage(int index)
{
    int maxLength = Qt_Gen::max(Qt_Gen::sizePerc(CWProps.maxScreenPercentage));

    QImage image(CWProps.imagePaths.at(index));
    if (image.width() > image.height())
        image = image.scaledToWidth(maxLength, Qt::SmoothTransformation);
    else
        image = image.scaledToHeight(maxLength, Qt::SmoothTransformation);

    return image;
}

QPixmap CaptionWidget::editImage(QImage& image)
{
    QPixmap canvas(image.width(), image.height());
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPoint imageBorderRadius = CWProps.imageBorderRadius;
    if (imageBorderRadius.x() < 0 || imageBorderRadius.y() < 0)
    {
        imageBorderRadius.setX(image.width()/5);
        imageBorderRadius.setY(image.height()/5);
    }

    QPainterPath clip;
    clip.addRoundedRect(0, 0, image.width(), image.height(), imageBorderRadius.x(), imageBorderRadius.y());

    QPainter paint(&canvas);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setClipPath(clip);
    paint.fillRect(0, 0, image.width(), image.height(), CWProps.imageBGColor);
    paint.drawImage(0, 0, image);
    paint.setPen(CWProps.imageBorderPen);
    paint.drawRoundedRect(0, 0, image.width(), image.height(), imageBorderRadius.x(), imageBorderRadius.y());

    return canvas;
}

QLabel* CaptionWidget::createLabel(QPixmap& pixmap)
{
    QLabel* label = new QLabel(CWProps.parent);
    label->setAlignment(Qt::AlignCenter);
    label->resize(pixmap.width(), pixmap.height());
    label->setPixmap(pixmap);
    label->setScaledContents(true);

    return label;
}

CaptionWidget::CaptionWidget(const CWProperties& CWProps)
    : CWProps(CWProps)
{}

CWProperties CaptionWidget::properties()
{
    return CWProps;
}

void CaptionWidget::properties(const CWProperties& CWProps)
{
    CaptionWidget::CWProps = CWProps;
}

QLabel* CaptionWidget::operator()(int index)
{
    return createLabel(editImage( loadScaledImage(index) ));
}
