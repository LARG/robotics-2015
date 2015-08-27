#ifndef LOGEDITOR_WINDOW_H
#define LOGEDITOR_WINDOW_H

#include <vector>
#include <map>
#include <sstream>

#include <memory/Logger.h>
#include <memory/LogReader.h>
#include "ui_LogEditorWindow.h"
#include "annotations/AnnotationGroup.h"

class FrameListWidgetItem : public QListWidgetItem {
  private:
    int frame_;
    std::string directory_;
    void init(std::string directory, int frame){
      frame_ = frame;
      directory_ = directory;
      std::string display = directory;
      std::stringstream ss;
      ss << display << ": " << frame;
      setText(QString::fromStdString(ss.str()));
    }
  public:
    FrameListWidgetItem(std::string directory, int frame) {
      init(directory,frame);
    }
    FrameListWidgetItem(const FrameListWidgetItem& other) : QListWidgetItem(other) {
      init(other.directory_, other.frame_);
    }
    std::string getFile() {
      return directory_;
    }
    int getFrame() {
      return frame_;
    }
};

class LogEditorWindow : public QMainWindow, public Ui_LogEditorWindow {
  Q_OBJECT

  private:
    bool textUpdating_;
    Log* log_;
    std::map<std::string, Log* > loaded_logs_;
    Log* lookupLog(std::string);
    AnnotationGroup* lookupGroup(Log*);
    AnnotationGroup *new_group_, *loaded_group_;
    std::map<Log*, AnnotationGroup*> log_annotations_;

  public:
    LogEditorWindow(QMainWindow* pa);
    Logger *logWriter;
    QMainWindow* parent;
    void keyPressEvent(QKeyEvent *event);

  signals:
    void prevFrame();
    void nextFrame();

  public slots:
    void loadLogs();
    void combineAllLogs();
    void addAllFrames();
    void addFrames();
    void delFrames();
    void clearFrames();

    void saveLog();
    void closeLog();

    void addShot(QListWidgetItem* item);
    void removeShot(QListWidgetItem* item);
    void loadLog(QListWidgetItem* item);

    void handleItemSelectionChanged();
};

#endif

