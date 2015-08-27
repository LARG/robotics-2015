#ifndef MAIN_WINDOW_D9KV76TW
#define MAIN_WINDOW_D9KV76TW

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "classification_window.hpp"
#include <sensor_msgs/Image.h>

namespace color_classifier {

  class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ReadSettings(); // Load up qt program settings at startup
    void WriteSettings(); // Save qt program settings when closing

    void closeEvent(QCloseEvent *event);
    void show(); 

  public slots:
    void on_actionOpen_Bag_triggered();
    void on_currentFrameSpin_valueChanged(int value);
    void on_frameSlider_sliderMoved(int value);

  private:
      Ui::ClassificationTool ui;
      ClassificationWindow classWindow;

      std::vector<sensor_msgs::ImageConstPtr> images;
  };

} 

#endif /* end of include guard: MAIN_WINDOW_D9KV76TW */
