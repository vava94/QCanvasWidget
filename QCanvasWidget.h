/**
 * Created by Vitaliy Kiselyov on 31.03.2021.
 * https://github.com/vava94/QCanvasWidget
 * vitkiselyov@gmail.com
 */

#ifndef QCANVASWIDGET_H
#define QCANVASWIDGET_H

#include <QImage>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QBitmap>

class QCanvasWidget: public QWidget {

    Q_OBJECT

public:
    enum DrawArguments {
        ORIGINAL_SIZE,
        FIT,
        FIT_BY_WIDTH,
        FIT_BY_HEIGHT
    } ;

    explicit QCanvasWidget(QWidget *parent = 0);
    ~QCanvasWidget();

    void clear() { clear(mDefaultColor); }
    void clear(QColor backgroundColor);
    void drawImage(QBitmap& bitmap, DrawArguments argument = DrawArguments::ORIGINAL_SIZE);
    void drawImage(uchar *data, int width, int height, QImage::Format format = QImage::Format_RGBA8888, DrawArguments argument = DrawArguments::ORIGINAL_SIZE);
    void setImage(QBitmap& bitmap, DrawArguments argument = DrawArguments::ORIGINAL_SIZE);
    void setImage(uchar *data, int width, int height, QImage::Format format = QImage::Format_RGBA8888, DrawArguments argument = DrawArguments::ORIGINAL_SIZE);

protected:

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

private:
    int mStartX = 0, mStartY = 0;

    DrawArguments mCurrentArgument = DrawArguments::ORIGINAL_SIZE;

    QBitmap mOriginalBitmap;
    QColor mDefaultColor = QColor::fromRgb(25,25,25);
    QImage mImageToDraw;

    void scaleImage(DrawArguments argument);

};


#endif //QCANVASWIDGET_H
