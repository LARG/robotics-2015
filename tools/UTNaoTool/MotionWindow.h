#ifndef MOTION_WINDOW_H
#define MOTION_WINDOW_H

#include <QWidget>

#include <vector>

//#include <memory/AbstractMemory.h>
#include <memory/Memory.h>
#include <math/Vector3.h>

#include "MotionGLWidget.h" 

class QWidget;
class QCheckBox;

class MotionWindow : public QWidget {
 Q_OBJECT

  public:
  MotionWindow();
  ~MotionWindow();

  void update(Memory* mem);

  MotionGLWidget* robot_;

  public slots:
  void updateTitle(QString title);

};

#endif
