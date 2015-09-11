#pragma once

#include <QtGui>
#include <QWidget>

#include <tool/ConfigWidget.h>
#include <memory/MemoryCache.h>
#include <common/KeyframeSequence.h>
#include "ui_KeyframeWidget.h"
#include "ui_KeyframeItem.h"

class KeyframeItem : public QWidget, public Ui_KeyframeItem {
  Q_OBJECT
  public:
    KeyframeItem(QListWidget* parent, QListWidgetItem* item, Keyframe keyframe) : QWidget(parent), keyframe_(keyframe) {
      setupUi(this);
      init(item);
    }
    const Keyframe& keyframe() { return keyframe_; }
    void init(QListWidgetItem* item);
  public slots:
    void updateName();
    void updateDelay(int delay);
    void activate();
    void deactivate();
  private:
    Keyframe keyframe_;
};

class KeyframeWidget : public ConfigWidget, public Ui_KeyframeWidget {
  Q_OBJECT
  public:
    KeyframeWidget(QWidget* parent);
    void updateMemory(MemoryCache cache);

  signals:
    void playingKeyframe(const Keyframe& keyframe);

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
    void play();
    void playNextKeyframe();
    void activate(QListWidgetItem *item);
    void deactivateCurrent();

  private:
    int currentKeyframe_;
    MemoryCache cache_;
    QTimer* keyframeTimer_;
    KeyframeItem* activated_;
};

