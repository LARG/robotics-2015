#include <tool/KeyframeWidget.h>
#include <tool/UTMainWnd.h>
#include <common/Util.h>

#define DEFAULT_FILE (UTMainWnd::dataDirectory() + "/keyframe.yaml")
  
void KeyframeItem::updateName() {
  keyframe_.name = txtName->toPlainText().toStdString();
  lblName->setText(txtName->toPlainText());
}

void KeyframeItem::updateFrames(int frames) {
  keyframe_.frames = frames;
  lblFrames->setText(QString::number(frames) + " ms");
}

void KeyframeItem::activate() {
  lblName->setVisible(false);
  lblFrames->setVisible(false);
  txtName->setVisible(true);
  spnFrames->setVisible(true);
}

void KeyframeItem::deactivate() {
  lblName->setVisible(true);
  lblFrames->setVisible(true);
  txtName->setVisible(false);
  spnFrames->setVisible(false);
}
    
void KeyframeItem::init(QListWidgetItem* item) {
  connect(txtName, SIGNAL(textChanged()), this, SLOT(updateName()));
  connect(spnFrames, SIGNAL(valueChanged(int)), this, SLOT(updateFrames(int)));
  txtName->setPlainText(QString::fromStdString(keyframe_.name));
  lblName->setText(txtName->toPlainText());
  spnFrames->setValue(keyframe_.frames);
  lblFrames->setText(QString::number(keyframe_.frames) + " ms");
  
  item->setSizeHint(QSize(100, 45));
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  
  auto keyframeBox = static_cast<QListWidget*>(parent());
  keyframeBox->addItem(item);
  keyframeBox->setItemWidget(item, this);
  deactivate();
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
  connect(btnShow, SIGNAL(clicked()), this, SLOT(show()));
  connect(keyframeBox, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateItem(QListWidgetItem*)));
  connect(keyframeBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(activate(QListWidgetItem*)));
  connect(keyframeBox, SIGNAL(itemSelectionChanged()), this, SLOT(deactivateCurrent()));
  connect(rdoTorso, SIGNAL(toggled(bool)), this, SLOT(supportBaseUpdated(bool)));
  connect(rdoLeftFoot, SIGNAL(toggled(bool)), this, SLOT(supportBaseUpdated(bool)));
  connect(rdoRightFoot, SIGNAL(toggled(bool)), this, SLOT(supportBaseUpdated(bool)));
  connect(rdoSensor, SIGNAL(toggled(bool)), this, SLOT(supportBaseUpdated(bool)));
  keyframeTimer_ = new QTimer(this);
  connect(keyframeTimer_, SIGNAL(timeout()), this, SLOT(playNextFrame()));
  keyframeTimer_->setSingleShot(true);
  activated_ = NULL;
}

void KeyframeWidget::updateItem(QListWidgetItem* item) {
  if(auto kitem = dynamic_cast<KeyframeItem*>(keyframeBox->itemWidget(item))) {
    kitem->updateName();
  }
}

void KeyframeWidget::save() {
  KeyframeSequence ks;
  for(int i = 0; i < keyframeBox->count(); i++) {
    auto kitem = static_cast<KeyframeItem*>(keyframeBox->itemWidget(keyframeBox->item(i)));
    ks.keyframes.push_back(kitem->keyframe());
  }
  ks.save(DEFAULT_FILE);
}

void KeyframeWidget::reload() {
  KeyframeSequence ks;
  keyframeBox->clear();
  ks.load(DEFAULT_FILE);
  for(auto kf : ks.keyframes) {
    auto item = new QListWidgetItem(keyframeBox);
    auto kfitem = new KeyframeItem(keyframeBox, item, kf);
  }
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
  auto item = new QListWidgetItem(keyframeBox);
  auto kfitem = new KeyframeItem(keyframeBox, item, kf);
}

void KeyframeWidget::deleteKeyframe() {
  for(auto item : keyframeBox->selectedItems()) {
    auto kfitem = static_cast<KeyframeItem*>(keyframeBox->itemWidget(item));
    if(activated_ == kfitem) activated_ == NULL;
    delete item;
  }
}

void KeyframeWidget::updateMemory(MemoryCache cache) {
  cache_ = cache;
}

void KeyframeWidget::play() {
  currentKeyframe_ = 0;
  currentFrame_ = 0;
  keyframeTimer_->start(100);
}

void KeyframeWidget::show() {
  for(auto item : keyframeBox->selectedItems()) {
    auto kfitem = static_cast<KeyframeItem*>(keyframeBox->itemWidget(item));
    emit showingKeyframe(kfitem->keyframe());
    break;
  }
}

void KeyframeWidget::playNextFrame() {
  // Clear selections from keyframes that have been played
  if(currentKeyframe_ > 0) {
    auto item = keyframeBox->item(currentKeyframe_ - 1);
    item->setSelected(false);
  }

  // Stop if we've played motions between each pair
  if(currentKeyframe_ >= keyframeBox->count() - 1) {
    for(int i = 0; i < keyframeBox->count(); i++)
      keyframeBox->item(i)->setSelected(false);
    return;
  }

  // Pull the first keyframe in the pair
  auto sitem = keyframeBox->item(currentKeyframe_);
  sitem->setSelected(true);
  auto kfstart = static_cast<KeyframeItem*>(keyframeBox->itemWidget(sitem));
  auto& start = kfstart->keyframe();
  
  // If we've played the transition, increment and start on the next pair
  if(currentFrame_ >= start.frames) {
    currentKeyframe_++;
    currentFrame_ = 0;
    playNextFrame();
    return;
  }
  
  // If we haven't finished the transition, pull the second keyframe in the pair
  auto fitem = keyframeBox->item(currentKeyframe_ + 1);
  fitem->setSelected(true);
  auto kffinish = static_cast<KeyframeItem*>(keyframeBox->itemWidget(fitem));
  auto& finish = kffinish->keyframe();

  // Emit the start, end, and # of frames
  emit playingSequence(start, finish, currentFrame_);
  currentFrame_++;
  keyframeTimer_->start(10);
}

void KeyframeWidget::activate(QListWidgetItem* item) {
  if(activated_) activated_->deactivate();
  activated_ = static_cast<KeyframeItem*>(keyframeBox->itemWidget(item));
  activated_->activate();
}

void KeyframeWidget::deactivateCurrent() {
  if(activated_) activated_->deactivate();
  activated_ = NULL;
}

void KeyframeWidget::supportBaseUpdated(bool) {
  if(rdoTorso->isChecked()) emit updatedSupportBase(SupportBase::TorsoBase);
  else if(rdoLeftFoot->isChecked()) emit updatedSupportBase(SupportBase::LeftFoot);
  else if(rdoRightFoot->isChecked()) emit updatedSupportBase(SupportBase::RightFoot);
  else if(rdoSensor->isChecked()) emit updatedSupportBase(SupportBase::SensorFoot);
}
