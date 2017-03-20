#include "CaptionWidget.h"

#include <QPainter>
#include <QDebug>
#include <QResizeEvent>
#include <QVBoxLayout>

#include "../../Helpers/Qt_General.h"
#include "../../Helpers/Math_General.h"

void CaptionWidget::calculateDimensions()
{
    dims.mainAndSubText = !subText.text.isEmpty() ? spacing.mainAndSubText : 0;
    dims.mainTextBR = QFontMetrics(mainText.font).tightBoundingRect(mainText.text);
    dims.subTextBR = QFontMetrics(subText.font).tightBoundingRect(subText.text);

    // To make sure text is at an appropriate distance from the border.
    dims.radius = textDD.borderRadius;

    int textHeight = Math::max(dims.mainTextBR.height(), dims.subTextBR.height()) / 1.5;

    dims.radius.setHeight(Math::max(dims.radius.height(), textHeight));
    dims.radius.setWidth(Math::max(dims.radius.width(), textHeight));

    dims.textBoxHeight = dims.radius.height()*2 + dims.mainTextBR.height() +
                         spacing.mainAndSubText + dims.subTextBR.height();
}

void CaptionWidget::loadScaledImage()
{
    // Calculate required image size.

    bool posW = targetSize.width() > 0;
    bool posH = targetSize.height() > 0;

    QSize imageSize;
    if (posW) imageSize.setWidth(targetSize.width());

    switch (sizeType)
    {
    case ST_Absolute :
        if (posH) imageSize.setHeight(targetSize.height() - dims.textBoxHeight - spacing.imageText);
    break;

    case ST_Image :
         if (posH) imageSize.setHeight(targetSize.height());
    break;
    }

    // If the image doesn't load put the question mark in its place.
    if (image.isNull())
    {
        image = QImage({200, 200}, QImage::Format_ARGB32);
        image.fill(imageDD.bgColor);

        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(mainText.color);

        QFont font(mainText.font);
        font.setBold(true);
        font.setPixelSize(200);
        painter.setFont(font);

        painter.drawText(QRect(QPoint(), QSize{200, 200}), "?", QTextOption(Qt::AlignCenter));

        imageSR = ISR_Scale;
    }

    // TODO: Fix logic here, non square images will scale wierdly.
    if      (posW && posH) // Fix x and y.
             image = image.scaled(imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    else if (!posW && !posH) // Auto x and y.
             ; // Do nothing.

    else if (posW) // Fix x, Auto y.
             image = image.scaledToWidth(imageSize.width(), Qt::SmoothTransformation);

    else // Auto w, fix y.
             image = image.scaledToHeight(imageSize.height(), Qt::SmoothTransformation);
}

void CaptionWidget::editImage(const QSize& size)
{
    calculateBorderRadius(imageDD, size);

    // Draw image portion of widget.

    imageIM = QImage(size, QImage::Format_ARGB32);
    imageIM.fill(QColor(0, 0, 0, 0));

    drawBorder(imageIM, imageDD, size, &image);
}

void CaptionWidget::editText()
{
    const QSize& PMSize = imageIM.size();
    calculateBorderRadius(textDD, PMSize);
    QSize size(PMSize.width(), dims.textBoxHeight);

    // Draw text portion of widget.

    drawBorder(textIM, textDD, size);

    // Draw text.

    int mainTextSpacing = dims.radius.height() + dims.mainTextBR.height();
    drawText(mainText, {dims.radius.width(), mainTextSpacing});

    int subTextSpacing = mainTextSpacing + dims.subTextBR.height() +
                         dims.mainAndSubText;
    drawText(subText, {dims.radius.width(), subTextSpacing});
}

void CaptionWidget::sortLabels()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(spacing.imageText);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(imageL);
    layout->addWidget(textL);
    setLayout(layout);
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

    else
        _BR = userBR;
}

void CaptionWidget::drawText(const TextData& textData, const QPoint& position)
{
    QPainter painter(&textIM);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setFont(textData.font);
    painter.setPen(textData.color);
    painter.drawText(position, textData.text);
}

QLabel* CaptionWidget::setLabel(QLabel* label, const QImage* const image, const QSize& size)
{
    if (image) label->setPixmap(QPixmap::fromImage(*image));
    label->setAlignment(Qt::AlignCenter);
    label->resize(size);
    label->setScaledContents(true);

    return label;
}

void CaptionWidget::updateLabels()
{
    setLabel(imageL, &imageIM, imageIM.size());
    setLabel(textL, &textIM, textIM.size());
    setLabel(this, nullptr, {imageL->width(),
                imageL->height() + textL->height() + spacing.imageText});

    layout()->setSpacing(spacing.imageText);
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
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setClipPath(clipPath);
    painter.fillRect(QRect(QPoint(), size), design.bgColor);

    if (image)
    {
        QRect rect;

        if (imageSR == ISR_Clip)
            rect = {(size.width()  - image->size().width())  / 2,
                    (size.height() - image->size().height()) / 2,
                     image->size().width(), image->size().height()};

        else // ISR_Scale
            rect = {QPoint(), size};

        painter.drawImage(rect, *image);
    }

    painter.setPen(design.borderPen);
    painter.drawRoundedRect(QRect(QPoint(), size),
                            design._borderRadius.width(), design._borderRadius.height());
}

void CaptionWidget::resizeEvent(QResizeEvent* re)
{
    const QSize& newS   = re->size();
    const QSize& imageS = {newS.width(), newS.height() - dims.textBoxHeight - spacing.imageText};
    const QSize& textS  = {newS.width(), dims.textBoxHeight};

    editImage(imageS);
    setLabel(imageL, &imageIM, imageS);

    editText();
    setLabel(textL, &textIM, textS);
}

CaptionWidget::CaptionWidget(QWidget* parent)
    : QLabel(parent)
{
    mainText.font.setPointSize(15);
    mainText.font.setBold(true);
    mainText.text = "mainText";

    subText.text = "subText";

    imageL = new QLabel;
    textL  = new QLabel;
    sortLabels();
}

void CaptionWidget::setup()
{
    calculateDimensions();
    loadScaledImage();
    editImage(image.size());
    editText();
    updateLabels();
}
