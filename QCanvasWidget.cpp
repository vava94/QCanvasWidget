/**
 * Created by Vitaliy Kiselyov on 31.03.2021.
 * https://github.com/vava94/QCanvasWidget
 * vitkiselyov@gmail.com
 */

#include "QCanvasWidget.h"

QCanvasWidget::QCanvasWidget(QWidget *parent) {
    mImageToDraw = QImage();
}

void QCanvasWidget::clear(QColor backgroundColor) {
    mImageToDraw = QImage(width(), height(), QImage::Format_RGB888);
    mImageToDraw.fill(mDefaultColor);
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
    painter.drawPixmap(mStartX, mStartY, QPixmap::fromImage(mImageToDraw));
}

void QCanvasWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    if (mImageToDraw.isNull() || mOriginalBitmap.isNull()) {
        clear();
    } else {
        setImage(mOriginalBitmap, mCurrentArgument);
    }
}

void QCanvasWidget::setImage(QBitmap &bitmap, DrawArguments argument) {
    mOriginalBitmap = bitmap;
    mCurrentArgument = argument;
    switch (argument) {
        case ORIGINAL_SIZE:
            mImageToDraw = mOriginalBitmap.toImage();
            break;
        case FIT:
            mImageToDraw = mOriginalBitmap.scaled(width(),height()).toImage();
            break;
        case FIT_BY_WIDTH:
            mImageToDraw = mOriginalBitmap.scaledToWidth(width()).toImage();
            break;
        case FIT_BY_HEIGHT:
            mImageToDraw = mOriginalBitmap.scaledToHeight(height()).toImage();
            break;
    }
}

void QCanvasWidget::setImage(uchar *data, int width, int height, QImage::Format format, DrawArguments argument) {
    auto bitmap = QBitmap::fromData(QSize(width, height), data, format);
    setImage(bitmap, argument);
}

QCanvasWidget::~QCanvasWidget() noexcept {
    mOriginalBitmap.clear();
    mImageToDraw = QImage();
}