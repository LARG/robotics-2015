#pragma once

#include <QtGui>
#include <QWidget>

#include <tool/ConfigWidget.h>
#include <memory/MemoryCache.h>
#include <common/KeyframeSequence.h>
#include "ui_KeyframeWidget.h"

class KeyframeItem : public QObject, public QListWidgetItem {
  Q_OBJECT
  public:
    KeyframeItem(QListWidget* parent, Keyframe keyframe) : QListWidgetItem(parent), keyframe_(keyframe) {
      init();
    }
    const Keyframe& keyframe() { return keyframe_; }
    void init();
  public slots:
    void updateName();
  private:
    Keyframe keyframe_;
};

class KeyframeWidget : public ConfigWidget, public Ui_KeyframeWidget {
  Q_OBJECT
  public:
    KeyframeWidget(QWidget* parent);
    void updateMemory(MemoryCache cache);

  protected:
    void loadConfig(const ToolConfig& config) { }
    void saveConfig(ToolConfig& config) { }

  protected slots:
    void save();
    void reload();
    void saveAs();
    void load();
    void addKeyframe();
    void deleteKeyframe();
    void updateItem(QListWidgetItem* item);

  private:
    MemoryCache cache_;
};

