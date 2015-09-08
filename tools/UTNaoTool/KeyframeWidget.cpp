#include <tool/KeyframeWidget.h>
#include <tool/UTMainWnd.h>
#include <common/Util.h>

#define DEFAULT_FILE (UTMainWnd::dataDirectory() + "/keyframe.yaml")
  
void KeyframeItem::updateName() {
  keyframe_.name = this->text().toStdString();
}
    
void KeyframeItem::init() {
  setText(QString::fromStdString(keyframe_.name));
  setFlags(this->flags() | Qt::ItemIsEditable);
}

KeyframeWidget::KeyframeWidget(QWidget* parent) : ConfigWidget(parent) {
  setupUi(this);
  connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
  connect(btnReload, SIGNAL(clicked()), this, SLOT(reload()));
  connect(btnSaveAs, SIGNAL(clicked()), this, SLOT(saveAs()));
  connect(btnLoad, SIGNAL(clicked()), this, SLOT(load()));
  connect(btnAdd, SIGNAL(clicked()), this, SLOT(addKeyframe()));
  connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteKeyframe()));
  connect(btnPlay, SIGNAL(clicked()), this, SLOT(play()));
  connect(keyframeBox, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateItem(QListWidgetItem*)));
  keyframeTimer_ = new QTimer(this);
  connect(keyframeTimer_, SIGNAL(timeout()), this, SLOT(playNextKeyframe()));
}

void KeyframeWidget::updateItem(QListWidgetItem* item) {
  if(auto kitem = dynamic_cast<KeyframeItem*>(item)) {
    kitem->updateName();
  }
}

void KeyframeWidget::save() {
  KeyframeSequence ks;
  for(int i = 0; i < keyframeBox->count(); i++) {
    auto kitem = static_cast<KeyframeItem*>(keyframeBox->item(i));
    ks.keyframes.push_back(kitem->keyframe());
  }
  ks.save(DEFAULT_FILE);
}

void KeyframeWidget::reload() {
  KeyframeSequence ks;
  keyframeBox->clear();
  ks.load(DEFAULT_FILE);
  for(auto kf : ks.keyframes)
    keyframeBox->addItem(new KeyframeItem(keyframeBox, kf));
}

void KeyframeWidget::saveAs() {
}

void KeyframeWidget::load() {
}

void KeyframeWidget::addKeyframe() {
  if(cache_.joint == NULL) {
    fprintf(stderr, "No joint block, can't add keyframes.\n");
    return;
  }
  auto kf = Keyframe(ssprintf("Keyframe %i", keyframeBox->count()));
  for(int i = 0; i < NUM_JOINTS; i++)
    kf.joints[i] = cache_.joint->values_[i];
  auto item = new KeyframeItem(keyframeBox, kf);
  keyframeBox->addItem(item);
}

void KeyframeWidget::deleteKeyframe() {
  for(auto item : keyframeBox->selectedItems()) {
    delete item;
  }
}

void KeyframeWidget::updateMemory(MemoryCache cache) {
  cache_ = cache;
}

void KeyframeWidget::play() {
  currentKeyframe_ = 0;
  keyframeTimer_->start(500);
}

void KeyframeWidget::playNextKeyframe() {
  if(currentKeyframe_ > 0) {
    auto kfitem = static_cast<KeyframeItem*>(keyframeBox->item(currentKeyframe_ - 1));
    kfitem->setSelected(false);
  }
  if(currentKeyframe_ >= keyframeBox->count()) {
    keyframeTimer_->stop();
    return;
  }
  auto kfitem = static_cast<KeyframeItem*>(keyframeBox->item(currentKeyframe_));
  kfitem->setSelected(true);
  emit playingKeyframe(kfitem->keyframe());
  currentKeyframe_++;
}
