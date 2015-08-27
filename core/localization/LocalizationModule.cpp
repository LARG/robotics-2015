#include <localization/LocalizationModule.h>

LocalizationModule::LocalizationModule() : tlogger_(textlogger) {
}

void LocalizationModule::loadParams(LocalizationParams params) {
  params_ = params;
}

void LocalizationModule::specifyMemoryDependency() {
  requiresMemoryBlock("world_objects");
  requiresMemoryBlock("localization");
  requiresMemoryBlock("vision_frame_info");
  requiresMemoryBlock("robot_state");
  requiresMemoryBlock("game_state");
}

void LocalizationModule::specifyMemoryBlocks() {
  getOrAddMemoryBlock(cache_.world_object,"world_objects");
  getOrAddMemoryBlock(cache_.localization_mem,"localization");
  getOrAddMemoryBlock(cache_.frame_info,"vision_frame_info");
  getOrAddMemoryBlock(cache_.robot_state,"robot_state");
  getOrAddMemoryBlock(cache_.game_state,"game_state");
}

void LocalizationModule::initSpecificModule() {
}

void LocalizationModule::initFromMemory() {
}

void LocalizationModule::initFromWorld() {
}

void LocalizationModule::reInit() {
}

void LocalizationModule::processFrame() {
}
