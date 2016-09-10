#include "CaptionWidget.h"

#include <QPainter>
#include <QDebug>

#include "../../Helpers/Qt_General.h"
#include "../../Helpers/Math_General.h"

void CaptionWidget::calculateDimensions()
{
    SpacingData& sD = spacing;

    dims.mainAndSubText = !subText.text.isEmpty() ? sD.mainAndSubText : 0;
    dims.mainTextBR = QFontMetrics(mainText.font).tightBoundingRect(mainText.text);
    dims.subTextBR = QFontMetrics(subText.font).tightBoundingRect(subText.text);

    // To make sure text is at an appropriate distance from the border.
    dims.radius = text.borderRadius;
    int minTextHeight = Math::min(dims.mainTextBR.height(), dims.subTextBR.height()) * 2;
    dims.radius.setHeight(Math::max(dims.radius.height(), minTextHeight));
    dims.radius.setWidth(Math::max(dims.radius.width(), minTextHeight));

    dims.textBoxHeight = sD.imageText + dims.radius.height() + dims.mainTextBR.height() +
                         sD.mainAndSubText + dims.subTextBR.height();
}

QImage CaptionWidget::loadScaledImage()
{
    QImage loadImage(imagePath);
    QSize imageSize;

    // Calculate required image size.

    bool posW = size.width() > 0;
    bool posH = size.height() > 0;

    if (posW) imageSize.setWidth(size.width());

    switch (sizeType)
    {
    case ST_Absolute :
        if (posH) imageSize.setHeight(size.height() - dims.textBoxHeight);
    break;

    case ST_Image :
         if (posH) imageSize.setHeight(size.height());
    break;
    }

    if      (posW && posH) // Fix x and y.
             return loadImage.scaled(imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else if  (!posW && !posH) // Auto x and y.
             return loadImage;
    else if (posW) // Fix x, Auto y.
             return loadImage.scaledToWidth(imageSize.width(), Qt::SmoothTransformation);
    else // Auto w, fix y.
             return loadImage.scaledToHeight(imageSize.height(), Qt::SmoothTransformation);
}

void CaptionWidget::editImage(const QImage& loadImage)
{
    calculateBorderRadius(image, loadImage.size());
    QSize size(loadImage.width(), loadImage.height());

    // Draw image portion of widget.

    imageIM = QImage(loadImage.width(), loadImage.height(), QImage::Format_ARGB32);
    imageIM.fill(QColor(0, 0, 0, 0));

    drawBorder(imageIM, image, size, &loadImage);
}

void CaptionWidget::editText()
{
    const QSize& PMSize = imageIM.size();
    calculateBorderRadius(text, PMSize);
    QSize size(PMSize.width(), dims.textBoxHeight);

    // Draw text portion of widget.

    drawBorder(textIM, text, size);

    // Draw text.

    int mainTextSpacing = dims.radius.height()/2 + dims.mainTextBR.height();
    drawText(mainText, {dims.radius.width()/2, mainTextSpacing});

    int subTextSpacing = mainTextSpacing + dims.subTextBR.height() +
                         dims.mainAndSubText;
    drawText(subText, {dims.radius.width()/2, subTextSpacing});
}

void CaptionWidget::createLabel()
{
    setAlignment(Qt::AlignCenter);
    resize(imageIM.width(), imageIM.height());
    setPixmap(QPixmap::fromImage(textIM));
    setScaledContents(true);
}

void CaptionWidget::calculateBorderRadius(DesignData& target, const QSize& source)
{
    QSize& userBR = target.borderRadius;
    QSize& _BR    = target._borderRadius;

    if (userBR.width() < 0 || userBR.height() < 0)
    {
        _BR.setWidth(source.width()/7); // 7 was decided arbitrarily.
        _BR.setHeight(_BR.width()); // To get a circular border we make xy equal.
    }
}

void CaptionWidget::drawText(const TextData& textData, const QPoint& position)
{
    QPainter painter(&textIM);

    painter.setFont(textData.font);
    painter.setPen(textData.color);
    painter.drawText(position, textData.text);
}

void CaptionWidget::drawBorder(QImage& target, const DesignData& design,
                               const QSize& size, const QImage* const image)
{
    target = QImage(size, QImage::Format_ARGB32);
    target.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPainterPath clipPath;
    clipPath.addRoundedRect({QPoint(), size},
                            design._borderRadius.width(), design._borderRadius.height());

    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(clipPath);
    painter.fillRect(QRect(QPoint(), size), design.bgColor);

    if (image)
        painter.drawImage(0, 0, *image);

    painter.setPen(design.borderPen);
    painter.drawRoundedRect(QRect(QPoint(), size),
                            design._borderRadius.width(), design._borderRadius.height());
}

CaptionWidget::CaptionWidget(QWidget* parent)
    : QLabel(parent)
{
    mainText.font.setPointSize(15);
    mainText.font.setBold(true);
    mainText.text = "mainText";

    subText.text = "subText";
}

void CaptionWidget::setup()
{
    calculateDimensions();
    editImage(loadScaledImage());
    editText();
    createLabel();
}
