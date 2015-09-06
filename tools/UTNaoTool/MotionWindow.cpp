#include <tool/MotionWindow.h>

using namespace std;

MotionWindow::MotionWindow(QMainWindow* pa) : ConfigWindow(pa) {
  setupUi(this);
}

void MotionWindow::updateMemory(Memory* mem) {
  motion_->updateMemory(mem);
  motion_->update();
  cache_ = MemoryCache::read(mem);
  keyframes_->updateMemory(cache_);
}
