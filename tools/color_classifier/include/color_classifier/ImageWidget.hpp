#ifndef IMAGEWIDGET_DTQR5H3Z
#define IMAGEWIDGET_DTQR5H3Z

#include <QWidget>
#include <QImage>

class QPaintEvent;

namespace color_classifier {

  class ImageWidget : public QWidget {
  Q_OBJECT
      
    public:
     ImageWidget(QWidget *parent);
     void reduceImageSize(int factor);

     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
    
    QImage* img;
     
    signals:
      void clicked(int x, int y, int button);
      void mouseXY(int x, int y);
  };

}

#endif /* end of include guard: IMAGEWIDGET_DTQR5H3Z */
