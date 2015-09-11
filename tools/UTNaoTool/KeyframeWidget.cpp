#include <tool/KeyframeWidget.h>
#include <tool/UTMainWnd.h>
#include <common/Util.h>

#define DEFAULT_FILE (UTMainWnd::dataDirectory() + "/keyframe.yaml")
  
void KeyframeItem::updateName() {
  keyframe_.name = txtName->toPlainText().toStdString();
  lblName->setText(txtName->toPlainText());
}

void KeyframeItem::updateDelay(int delay) {
  keyframe_.delay = delay;
  lblDelay->setText(QString::number(delay) + " ms");
}

void KeyframeItem::activate() {
  lblName->setVisible(false);
  lblDelay->setVisible(false);
  txtName->setVisible(true);
  spnDelay->setVisible(true);
}

void KeyframeItem::deactivate() {
  lblName->setVisible(true);
  lblDelay->setVisible(true);
  txtName->setVisible(false);
  spnDelay->setVisible(false);
}
    
void KeyframeItem::init(QListWidgetItem* item) {
  connect(txtName, SIGNAL(textChanged()), this, SLOT(updateName()));
  connect(spnDelay, SIGNAL(valueChanged(int)), this, SLOT(updateDelay(int)));
  txtName->setPlainText(QString::fromStdString(keyframe_.name));
  lblName->setText(txtName->toPlainText());
  spnDelay->setValue(keyframe_.delay);
  lblDelay->setText(QString::number(keyframe_.delay) + " ms");
  
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
  connect(keyframeBox, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateItem(QListWidgetItem*)));
  connect(keyframeBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(activate(QListWidgetItem*)));
  connect(keyframeBox, SIGNAL(itemSelectionChanged()), this, SLOT(deactivateCurrent()));
  keyframeTimer_ = new QTimer(this);
  connect(keyframeTimer_, SIGNAL(timeout()), this, SLOT(playNextKeyframe()));
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
  keyframeTimer_->start(500);
}

void KeyframeWidget::playNextKeyframe() {
  if(currentKeyframe_ > 0) {
    auto item = keyframeBox->item(currentKeyframe_ - 1);
    item->setSelected(false);
  }
  if(currentKeyframe_ >= keyframeBox->count()) {
    keyframeTimer_->stop();
    return;
  }
  auto item = keyframeBox->item(currentKeyframe_);
  auto kfitem = static_cast<KeyframeItem*>(keyframeBox->itemWidget(item));
  item->setSelected(true);
  emit playingKeyframe(kfitem->keyframe());
  currentKeyframe_++;
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
