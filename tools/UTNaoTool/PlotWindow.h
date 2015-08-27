#ifndef PLOTWINDOW_QPYBZ7XI
#define PLOTWINDOW_QPYBZ7XI

#include <QWidget>

#include <vector>

#include <memory/Memory.h>
#include <memory/Log.h>
#include <math/Vector3.h>
#include <common/RobotDimensions.h>
#include <common/RingBufferWithSum.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>

class QWidget;
class QCheckBox;

class PlotWindow : public QWidget {
  Q_OBJECT
public:
  PlotWindow();
  ~PlotWindow();

  void update(Memory* mem);
  void setMemoryLog(Log* memory_log);
  void addLine(std::string name, std::string color);
  
  void populateGraphableBlock(Memory &memory);

protected:
  void mousePressEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  Log* memory_log_;
  QwtPlot plot_;
  std::vector<QwtPlotCurve*> plot_curves_;
  QwtPlotMarker plot_marker_;
  QwtPlotPanner *panner_;
  QwtPlotMagnifier *magnifier_;
  QwtLegend *legend_;

  std::vector<double> times_;

  RobotDimensions robot_dimensions_;
  RingBufferWithSum<float,3> sensor_com_acc_buffer_;

Q_SIGNALS:
  void gotoSnapshot(int index);
  void prevSnapshot();
  void nextSnapshot();
  void play();
  void pause();
};

#endif /* end of include guard: PLOTWINDOW_QPYBZ7XI */
