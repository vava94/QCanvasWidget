/**
 * Created by Vitaliy Kiselyov on 31.03.2021.
 * https://github.com/vava94/QCanvasWidget
 * vitkiselyov@gmail.com
 */

#include "QCanvasWidget.h"

#include <utility>


QCanvasWidget::QCanvasWidget(QWidget *parent) {
    mImageToDraw = QImage();
    mBackgroundImage = QImage();
}

void QCanvasWidget::clear(QColor backgroundColor) {
    mSourceImage = QImage();
    update();
}

void QCanvasWidget::drawImage(QBitmap &bitmap, DrawArguments argument) {
    setImage(bitmap, argument);
    update();
}

void QCanvasWidget::drawImage(uchar *data, int width, int height, QImage::Format format, DrawArguments argument) {
    setImage(data, width, height, format, argument);
    update();
}

void QCanvasWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    if (mImageToDraw.width() < width()) {
        mStartX = (int)((width() - mImageToDraw.width()) * 0.5);
    } else {
        mStartX = 0;
    }
    if (mImageToDraw.height() < height()) {
        mStartY = (int)((height() - mImageToDraw.height()) * 0.5);
    } else {
        mStartY = 0;
    }
    painter.drawPixmap(0,0,QPixmap::fromImage(mBackgroundImage));
    painter.drawPixmap(mStartX, mStartY, QPixmap::fromImage(mImageToDraw));

}

void QCanvasWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    if (mImageToDraw.isNull() || mSourceImage.isNull()) {
        clear();
    } else {
        setImage(mSourceImage, mCurrentArgument);
    }
    mBackgroundImage = QImage(width(), height(), QImage::Format_RGB888);
    mBackgroundImage.fill(mDefaultColor);
}

void QCanvasWidget::setImage(QBitmap &bitmap, DrawArguments argument) {
    setImage(bitmap.toImage(), argument);
}

void QCanvasWidget::setImage(QImage image, DrawArguments argument) {
    mSourceImage = std::move(image);
    mCurrentArgument = argument;
    if(!width() || mSourceImage.isNull()) return;
    switch (argument) {
        case ORIGINAL_SIZE:
            mImageToDraw = mSourceImage;
            break;
        case FILL:
            mImageToDraw = mSourceImage.scaled(width(),height());
            break;
        case FIT:
            if ((mSourceImage.width() * 1.0 / mSourceImage.height()) > (width() * 1.0 / height())) {
                mImageToDraw = mSourceImage.scaledToWidth(width());
            }
            else {
                mImageToDraw = mSourceImage.scaledToHeight(height());
            }
            break;
        case FIT_BY_WIDTH:
            mImageToDraw = mSourceImage.scaledToWidth(width());
            break;
        case FIT_BY_HEIGHT:
            mImageToDraw = mSourceImage.scaledToHeight(height());
            break;
        case SPAN:
            if ((mSourceImage.width() * 1.0 / mSourceImage.height()) < (width() * 1.0 / height())) {
                mImageToDraw = mSourceImage.scaledToWidth(width());
            }
            else {
                mImageToDraw = mSourceImage.scaledToHeight(height());
            }
            break;
    }
}

void QCanvasWidget::setImage(uchar *data, int width, int height, QImage::Format format, DrawArguments argument) {
    mSourceImage = QImage(data, width, height, format);
    setImage(mSourceImage, argument);
}

QCanvasWidget::~QCanvasWidget() noexcept {
    mSourceImage = QImage();
    mImageToDraw = QImage();
}