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
    clip.addRoundedRect(0, 0, image.width(), image.height(),
                        imageBorderRadius.x(), imageBorderRadius.y());

    QPainter paint(&canvas);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.setClipPath(clip);
    paint.fillRect(0, 0, image.width(), image.height(), CWProps.imageBGColor);
    paint.drawImage(0, 0, image);
    paint.setPen(CWProps.imageBorderPen);
    paint.drawRoundedRect(0, 0, image.width(), image.height(),
                          imageBorderRadius.x(), imageBorderRadius.y());

    return canvas;
}

QPixmap CaptionWidget::addText(QPixmap& pixmap)
{
    QPixmap canvas(pixmap.width(), pixmap.height() * 2);
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPoint textBorderRadius = CWProps.textBorderRadius;
    if (textBorderRadius.x() < 0 || textBorderRadius.y() < 0)
    {
        textBorderRadius.setX(pixmap.width()/5);
        textBorderRadius.setY(pixmap.height()/5);
    }

    QPainterPath clip;
    clip.addRoundedRect(0, pixmap.height(), pixmap.width(), pixmap.height(),
                        textBorderRadius.x(), textBorderRadius.y());

    QPainter paint(&canvas);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.drawPixmap(0, 0, pixmap);
    paint.setClipPath(clip);
    paint.fillRect(0, pixmap.height(), pixmap.width(), pixmap.height(),
                   CWProps.textBGColor);

    paint.setFont(CWProps.mainTextFont);
    QRect textBR = QFontMetrics(CWProps.mainTextFont).tightBoundingRect(CWProps.mainText);
    int mainTextSpacing = pixmap.height() + textBorderRadius.y() + textBR.height();
    paint.drawText(textBorderRadius.x(), mainTextSpacing, CWProps.mainText);

    paint.setFont(CWProps.subTextFont);
    textBR = QFontMetrics(CWProps.subTextFont).tightBoundingRect(CWProps.subText);
    int subTextSpacing = mainTextSpacing + textBR.height() + CWProps.mainAndSubTextSpacing;
    paint.drawText(textBorderRadius.x(), subTextSpacing, CWProps.subText);

    paint.setPen(CWProps.textBorderPen);
    paint.drawRoundedRect(0, pixmap.height(), pixmap.width(), pixmap.height(),
                          textBorderRadius.x(), textBorderRadius.y());


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
    QImage image    = loadScaledImage(index);
    QPixmap pixmap1 = editImage(image);
    QPixmap pixmap2 = addText(pixmap1);
    QLabel* label   = createLabel(pixmap2);

    return label;
}
