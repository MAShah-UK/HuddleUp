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

QImage CaptionWidget::loadScaledImage()
{

    Math::clamp(CWProps.maxScreenPercentage, 0, 100);
    int maxLength = Qt_Gen::max(Qt_Gen::sizePerc(CWProps.maxScreenPercentage));

    QImage image(CWProps.imagePath);
    if (image.width() > image.height())
        image = image.scaledToWidth(maxLength, Qt::SmoothTransformation);
    else
        image = image.scaledToHeight(maxLength, Qt::SmoothTransformation);

    return image;
}

QPixmap CaptionWidget::editImage(const QImage& image)
{
    // Calculate dimensions.

    calculateBorderRadius(CWProps.image.borderRadius, image.size());
    QRect border(0, 0, image.width(), image.height());

    // Draw image portion of widget.

    QPixmap canvas(image.width(), image.height());
    canvas.fill(QColor(0, 0, 0, 0));

    QPainter painter(&canvas);
    drawBorder(painter, CWProps.image, border, &image);

    return canvas;
}

QPixmap CaptionWidget::addText(const QPixmap& pixmap)
{
    // Calculate dimensions.

    calculateBorderRadius(CWProps.text.borderRadius, pixmap.size());
    QSize radius = CWProps.text.borderRadius;

    int spacingMainAndSubText = 0;
    if (!CWProps.subText.text.isEmpty()) spacingMainAndSubText = CWProps.spacingMainAndSubText;

    QRect mainTextBR = QFontMetrics(CWProps.mainText.font).tightBoundingRect(CWProps.mainText.text);
    QRect subTextBR = QFontMetrics(CWProps.subText.font).tightBoundingRect(CWProps.subText.text);

    // To make sure text is at an appropriate distance from the border.
    int minTextHeight = Math::min(mainTextBR.height(), subTextBR.height());
    radius.setHeight(Math::max(radius.height(), minTextHeight));
    radius.setWidth(Math::max(radius.width(), minTextHeight));

    int netHeight = CWProps.spacingImageText + radius.height() + mainTextBR.height() +
                    spacingMainAndSubText + subTextBR.height();

    QRect border(0, pixmap.height() + CWProps.spacingImageText,
                 pixmap.width(), netHeight - CWProps.spacingImageText);

    // Draw text portion of widget.

    QPixmap canvas(pixmap.width(), netHeight + pixmap.height());
    canvas.fill(QColor(0, 0, 0, 0)); // Default is black.

    QPainter painter(&canvas);
    painter.drawPixmap(0, 0, pixmap);
    drawBorder(painter, CWProps.text, border);

    // Draw text.

    int mainTextSpacing = pixmap.height() + CWProps.spacingImageText +
                          radius.height()/2 + mainTextBR.height();
    drawText(painter, CWProps.mainText, {radius.width()/2, mainTextSpacing});

    int subTextSpacing = mainTextSpacing + subTextBR.height() +
                         spacingMainAndSubText;
    drawText(painter, CWProps.subText, {radius.width()/2, subTextSpacing});

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

void CaptionWidget::calculateBorderRadius(QSize& target, const QSize& source)
{
    if (target.width() < 0 || target.height() < 0)
    {
        target.setWidth(source.width()/7); // 7 was decided arbitrarily.
        target.setHeight(target.width()); // To get a circular border we make xy equal.
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
    clipPath.addRoundedRect(border, design.borderRadius.width(), design.borderRadius.height());

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(clipPath);
    painter.fillRect(border, design.bgColor);

    if (image)
        painter.drawImage(0, 0, *image);

    painter.setPen(design.borderPen);
    painter.drawRoundedRect(border, design.borderRadius.width(), design.borderRadius.height());
}

CaptionWidget::CaptionWidget(CWProperties& CWProps)
    : CWProps(CWProps)
{}

QLabel* CaptionWidget::getLabel()
{
    QImage image   = loadScaledImage();
    QPixmap pixmap = editImage(image);
    pixmap         = addText(pixmap);
    QLabel* label  = createLabel(pixmap);

    return label;
}
