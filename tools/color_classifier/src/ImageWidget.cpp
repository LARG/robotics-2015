#include <QtGui>
#include <color_classifier/ImageWidget.hpp>
#include <iostream>

#define IMAGE_X 640
#define IMAGE_Y 480

namespace color_classifier {

  ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent) {
    img = new QImage(IMAGE_X,IMAGE_Y,QImage::Format_RGB32);
    img->fill(Qt::black);
  }

  void ImageWidget::reduceImageSize(int factor){
    img = new QImage(IMAGE_X/factor,IMAGE_Y/factor,QImage::Format_RGB32);
    img->fill(Qt::black);
  }
   
  void ImageWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);

    painter.drawImage(event->rect(), *img);
    painter.end();
  }

  void ImageWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton) return;
    float x=(float)event->x()/(float)width()*IMAGE_X;
    float y=(float)event->y()/(float)height()*IMAGE_Y;
    emit clicked((int)x,(int)y, (int) event->button());
  }

  void ImageWidget::mouseMoveEvent(QMouseEvent *event) {
    float x=(float)event->x()/(float)width()*IMAGE_X;
    float y=(float)event->y()/(float)height()*IMAGE_Y;
    emit mouseXY((int)x,(int)y);
  }

}
