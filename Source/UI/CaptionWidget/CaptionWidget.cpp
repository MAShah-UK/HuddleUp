#include "CaptionWidget.h"

#include <QPainter>
#include <QDebug>

#include "../../Helpers/Qt_General.h"
#include "../../Helpers/Math_General.h"

CWProperties::CWProperties()
{
    mainText.font.setPointSize(15);
    mainText.font.setBold(true);
    mainText.text = "mainText";

    subText.text = "subText";
}

void CaptionWidget::calculateDimensions()
{
    CWProperties::SpacingData& sD = CWP.spacing;

    dims.mainAndSubText = !CWP.subText.text.isEmpty() ? sD.mainAndSubText : 0;
    dims.mainTextBR = QFontMetrics(CWP.mainText.font).tightBoundingRect(CWP.mainText.text);
    dims.subTextBR = QFontMetrics(CWP.subText.font).tightBoundingRect(CWP.subText.text);

    // To make sure text is at an appropriate distance from the border.
    dims.radius = CWP.text.borderRadius;
    int minTextHeight = Math::min(dims.mainTextBR.height(), dims.subTextBR.height()) * 2;
    dims.radius.setHeight(Math::max(dims.radius.height(), minTextHeight));
    dims.radius.setWidth(Math::max(dims.radius.width(), minTextHeight));

    dims.textBoxHeight = sD.imageText + dims.radius.height() + dims.mainTextBR.height() +
                         sD.mainAndSubText + dims.subTextBR.height();
}

QImage CaptionWidget::loadScaledImage()
{
    QImage image(CWP.imagePath);
    QSize imageSize;

    // Calculate required image size.

    bool posW = CWP.size.width() > 0;
    bool posH = CWP.size.height() > 0;

    if (posW) imageSize.setWidth(CWP.size.width());

    switch (CWP.sizeType)
    {
    case CWProperties::ST_Absolute :
        if (posH) imageSize.setHeight(CWP.size.height() - dims.textBoxHeight);
    break;

    case CWProperties::ST_Image :
         if (posH) imageSize.setHeight(CWP.size.height());
    break;
    }

    if      (posW && posH) // Fix x and y.
             return image.scaled(imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else if  (!posW && !posH) // Auto x and y.
             return image;
    else if (posW) // Fix x, Auto y.
             return image.scaledToWidth(imageSize.width(), Qt::SmoothTransformation);
    else // Auto w, fix y.
             return image.scaledToHeight(imageSize.height(), Qt::SmoothTransformation);
}

QPixmap CaptionWidget::editImage(const QImage& image)
{
    calculateBorderRadius(CWP.image, image.size());
    QRect border(0, 0, image.width(), image.height());

    // Draw image portion of widget.

    QPixmap canvas(image.width(), image.height());
    canvas.fill(QColor(0, 0, 0, 0));

    QPainter painter(&canvas);
    drawBorder(painter, CWP.image, border, &image);

    return canvas;
}

QPixmap CaptionWidget::addText(const QPixmap& pixmap)
{
    calculateBorderRadius(CWP.text, pixmap.size());
    QRect border(0, pixmap.height() + CWP.spacing.imageText,
                 pixmap.width(), dims.textBoxHeight - CWP.spacing.imageText);

    // Draw text portion of widget.

    QPixmap canvas(pixmap.width(), dims.textBoxHeight + pixmap.height());
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPainter painter(&canvas);
    painter.drawPixmap(0, 0, pixmap);
    drawBorder(painter, CWP.text, border);

    // Draw text.

    int mainTextSpacing = pixmap.height() + CWP.spacing.imageText +
                          dims.radius.height()/2 + dims.mainTextBR.height();
    drawText(painter, CWP.mainText, {dims.radius.width()/2, mainTextSpacing});

    int subTextSpacing = mainTextSpacing + dims.subTextBR.height() +
                         dims.mainAndSubText;
    drawText(painter, CWP.subText, {dims.radius.width()/2, subTextSpacing});

    return canvas;
}

QLabel* CaptionWidget::createLabel(const QPixmap& pixmap)
{
    QLabel* label = new QLabel(CWP.parent);
    label->setAlignment(Qt::AlignCenter);
    label->resize(pixmap.width(), pixmap.height());
    label->setPixmap(pixmap);
    label->setScaledContents(true);

    return label;
}

void CaptionWidget::calculateBorderRadius(CWProperties::DesignData& target,
                                          const QSize& source)
{
    QSize& userBR = target.borderRadius;
    QSize& _BR    = target._borderRadius;

    if (userBR.width() < 0 || userBR.height() < 0)
    {
        _BR.setWidth(source.width()/7); // 7 was decided arbitrarily.
        _BR.setHeight(_BR.width()); // To get a circular border we make xy equal.
    }
}

void CaptionWidget::drawText(QPainter& painter, const CWProperties::TextData& textData,
                             const QPoint& position)
{
    painter.setFont(textData.font);
    painter.setPen(textData.color);
    painter.drawText(position, textData.text);
}

void CaptionWidget::drawBorder(QPainter& painter, const CWProperties::DesignData& design,
                               const QRect& border, const QImage* const image)
{
    QPainterPath clipPath;
    clipPath.addRoundedRect(border, design._borderRadius.width(), design._borderRadius.height());

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(clipPath);
    painter.fillRect(border, design.bgColor);

    if (image)
        painter.drawImage(0, 0, *image);

    painter.setPen(design.borderPen);
    painter.drawRoundedRect(border, design._borderRadius.width(), design._borderRadius.height());
}

CaptionWidget::CaptionWidget(CWProperties& CWP)
    : CWP(CWP)
{}

QLabel* CaptionWidget::getLabel()
{
                     calculateDimensions();
    QImage image   = loadScaledImage();
    QPixmap pixmap = editImage(image);
    pixmap         = addText(pixmap);
    QLabel* label  = createLabel(pixmap);

    return label;
}
