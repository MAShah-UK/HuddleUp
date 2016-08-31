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

QPixmap CaptionWidget::editImage(const QImage& image)
{
    QPixmap canvas(image.width(), image.height());
    canvas.fill(QColor(0, 0, 0, 0));

    QPoint imageBorderRadius = CWProps.imageBorderRadius;
    if (imageBorderRadius.x() < 0 || imageBorderRadius.y() < 0)
    {
        imageBorderRadius.setX(image.width()/5);
        imageBorderRadius.setY(imageBorderRadius.x()); // To get a circular border.
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

QPixmap CaptionWidget::addText(const QPixmap& pixmap)
{
    // Calculate dimensions.

    QPoint textBorderRadius = CWProps.textBorderRadius;
    if (textBorderRadius.x() < 0 || textBorderRadius.y() < 0)
    {
        textBorderRadius.setX(pixmap.width()/5);
        textBorderRadius.setY(textBorderRadius.x()); // To get a circular border.
    }

    QRect mainTextBR = QFontMetrics(CWProps.textMainFont).tightBoundingRect(CWProps.textMain);
    QRect subTextBR = QFontMetrics(CWProps.textSubFont).tightBoundingRect(CWProps.textSub);

    int netHeight = CWProps.spacingImageText + textBorderRadius.y() + mainTextBR.height() +
                    CWProps.spacingMainAndSubText + subTextBR.height();

    QRectF border(0, pixmap.height() + CWProps.spacingImageText,
                  pixmap.width(), netHeight - CWProps.spacingImageText);

    // Begin drawing.

    QPixmap canvas(pixmap.width(), netHeight + pixmap.height());
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPainterPath clipPath;
    clipPath.addRoundedRect(border, textBorderRadius.x(), textBorderRadius.y());

    QPainter paint(&canvas);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.drawPixmap(0, 0, pixmap);
    paint.setClipPath(clipPath);
    paint.fillRect(border, CWProps.textBGColor);

    // Draw text.

    int mainTextSpacing = pixmap.height() + CWProps.spacingImageText +
                          textBorderRadius.y()/2 + mainTextBR.height();
    paint.setFont(CWProps.textMainFont);
    paint.setPen(CWProps.mainTextColor);
    paint.drawText(textBorderRadius.x()/2, mainTextSpacing, CWProps.textMain);

    int subTextSpacing = mainTextSpacing + subTextBR.height() + CWProps.spacingMainAndSubText;
    paint.setFont(CWProps.textSubFont);
    paint.setPen(CWProps.subTextColor);
    paint.drawText(textBorderRadius.x()/2, subTextSpacing, CWProps.textSub);

    paint.setPen(CWProps.textBorderPen);
    paint.drawRoundedRect(border, textBorderRadius.x(), textBorderRadius.y());

    return canvas;
}

QLabel* CaptionWidget::createLabel(const QPixmap& pixmap)
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
    QImage image   = loadScaledImage(index);
    QPixmap pixmap = editImage(image);
    pixmap         = addText(pixmap);
    QLabel* label  = createLabel(pixmap);

    return label;
}
