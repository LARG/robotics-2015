#ifndef CLASSIFICATION_WINDOW_GZ6WWOJ1
#define CLASSIFICATION_WINDOW_GZ6WWOJ1

#include <QtGui/QMainWindow>
#include "ui_classification_window.h"
#include <sensor_msgs/Image.h>

#define IMAGE_HEIGHT 480
#define IMAGE_WIDTH 640

namespace color_classifier {

  struct Rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  struct Yuv {
    uint8_t y;
    uint8_t u;
    uint8_t v;
  };

  typedef Rgb RgbImage[IMAGE_HEIGHT][IMAGE_WIDTH];
  typedef uint8_t SegImage[IMAGE_HEIGHT][IMAGE_WIDTH];
  typedef uint8_t ColorTable[128][128][128];

  enum Color {
    UNDEFINED,
    ORANGE,
    PINK,
    BLUE,
    GREEN,
    WHITE,
    YELLOW,
    NUM_COLORS
  };

  enum Image {
    RGB,
    SEG
  };

  enum ClickMode {
    ADD,
    DELETE
  };

  class ClassificationWindow : public QMainWindow {
  Q_OBJECT

  public:
    ClassificationWindow(QWidget *parent = 0);
    ~ClassificationWindow();

    void ReadSettings(); // Load up qt program settings at startup
    void WriteSettings(); // Save qt program settings when closing

    void closeEvent(QCloseEvent *event); // Overloaded function
    void changeImage(sensor_msgs::ImageConstPtr image);

    void drawRgbImage(ImageWidget *widget);
    void drawSegImage(ImageWidget *widget);

    void segmentImage(bool useTempColorTable);
    void redrawImages(bool useTempColorTable = false);

    void setColor(int color);
    void updateStatus();

    inline int max(int a, int b);
    inline int min(int a, int b);

  public slots:
    void on_bigImage_clicked(int x, int y, int button);
    void on_bigImage_mouseXY(int x, int y);
    void on_rawImage_clicked(int x, int y, int button);
    void on_segImage_clicked(int x, int y, int button);

    void on_addRadio_clicked();
    void on_deleteRadio_clicked();

    void on_orangeButton_clicked();
    void on_pinkButton_clicked();
    void on_blueButton_clicked();
    void on_greenButton_clicked();
    void on_whiteButton_clicked();
    void on_yellowButton_clicked();

    void on_colorCombo_currentIndexChanged(int index);

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();

  private:
      Ui::ClassificationWindow ui;
      std::string colorTableFilename;

      RgbImage rgbImage;
      SegImage segImage;
      ColorTable colorTable;
      ColorTable tempColorTable; //For undo

      QRgb segColors[NUM_COLORS];
      std::string segColorNames[NUM_COLORS];

      Image imageSelected;
      ClickMode clickMode;
      Color currentColor;

  };

}  

#endif /* end of include guard: CLASSIFICATION_WINDOW_GZ6WWOJ1 */
