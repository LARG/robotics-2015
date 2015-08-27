#include <QtGui>
#include "MotionWindow.h"

//#include "../../core/common/Common.h"
using namespace std;

MotionWindow::MotionWindow() : QWidget() {
  QGridLayout *layout = new QGridLayout;

  robot_=new MotionWidget(this);
 
  layout->addWidget(robot_, 0, 0, 5, 8);
        
  setLayout(layout);
  resize(1000,600);
  setWindowTitle(tr("Motion System"));

  connect(robot_, SIGNAL(modeChanged(QString)), this, SLOT(updateTitle(QString)));
}

MotionWindow::~MotionWindow() {
  delete robot_;
}


void MotionWindow::update(Memory* mem) {
  robot_->updateMemory(mem);
  robot_->update();
}

void MotionWindow::updateTitle(QString ti) {
  setWindowTitle("Motion System - "+ti);
}
