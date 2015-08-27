#include <tool/ConfigWidget.h>
#include <tool/UTMainWnd.h>

ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent) {
  connect(UTMainWnd::inst(), SIGNAL(loadingConfig(const ToolConfig&)), this, SLOT(baseLoadConfig(const ToolConfig&)));
  connect(UTMainWnd::inst(), SIGNAL(savingConfig(ToolConfig&)), this, SLOT(saveConfig(ToolConfig&)));
}

void ConfigWidget::baseLoadConfig(const ToolConfig& config) {
  loading_ = true;
  loadConfig(config);
  loading_ = false;
}
