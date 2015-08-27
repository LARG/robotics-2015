#include <cstddef>
#include <memory/Memory.h>
#include <memory/MemoryCache.h>
#include <memory/WorldObjectBlock.h>
#include <memory/RobotStateBlock.h>

MemoryCache::MemoryCache() {
  clear();
}

MemoryCache::MemoryCache(Memory* memory) {
  fill(memory);
}

MemoryCache::MemoryCache(Memory& memory) {
  fill(memory);
}

void MemoryCache::clear() {
  world_object = NULL;
  localization_mem = NULL;
  team_packets = NULL;
  frame_info = NULL;
  robot_state = NULL;
  game_state = NULL;
  odometry = NULL;
  joint = NULL;
  behavior = NULL;
  sonar = NULL;
  delayed_localization = NULL;
  body_model = NULL;
  image = NULL;
  robot_vision = NULL;
  robot_info = NULL;
  sensor = NULL;
  opponent_mem = NULL;
  behavior_params = NULL;
  walk_request = NULL;
  kick_request = NULL;
  walk_params = NULL;
  joint_command = NULL;
  walk_info = NULL;
  audio_processing = NULL;

  sim_truth = NULL;

  memory = NULL;
}

void MemoryCache::fill(Memory& memory) {
  fill(&memory);
}

void MemoryCache::fill(Memory* memory) {
  memory->getOrAddBlockByName(world_object, "world_objects");
  memory->getOrAddBlockByName(localization_mem, "localization");
  memory->getOrAddBlockByName(team_packets, "team_packets");
  memory->getOrAddBlockByName(frame_info, "vision_frame_info");
  memory->getOrAddBlockByName(robot_state, "robot_state");
  memory->getOrAddBlockByName(game_state, "game_state");
  memory->getOrAddBlockByName(odometry, "vision_odometry");
  memory->getOrAddBlockByName(joint, "vision_joint_angles");
  memory->getOrAddBlockByName(behavior, "behavior");
  memory->getOrAddBlockByName(sonar, "vision_processed_sonar");
  memory->getOrAddBlockByName(delayed_localization, "delayed_localization");
  memory->getOrAddBlockByName(body_model, "body_model");
  memory->getOrAddBlockByName(image, "raw_image");
  memory->getOrAddBlockByName(camera, "camera_info");
  memory->getOrAddBlockByName(robot_vision, "robot_vision");
  memory->getOrAddBlockByName(robot_info, "robot_info");
  memory->getOrAddBlockByName(sensor, "processed_sensors");
  memory->getOrAddBlockByName(opponent_mem, "opponents");
  memory->getOrAddBlockByName(behavior_params, "behavior_params");
  memory->getOrAddBlockByName(walk_request, "vision_walk_request");
  memory->getOrAddBlockByName(kick_request, "vision_kick_request");
  memory->getOrAddBlockByName(walk_params, "vision_al_walk_param");
  memory->getOrAddBlockByName(joint_command, "vision_joint_commands");
  memory->getOrAddBlockByName(walk_info, "vision_walk_info");
  memory->getOrAddBlockByName(sim_truth, "sim_truth_data");
  memory->getOrAddBlockByName(audio_processing, "audio_processing");

  this->memory = memory;
}

MemoryCache MemoryCache::read(Memory& memory) {
  return read(&memory);
}

MemoryCache MemoryCache::read(Memory* memory) {
  MemoryCache cache;
  memory->getBlockByName(cache.world_object, "world_objects", false);
  memory->getBlockByName(cache.localization_mem, "localization", false);
  memory->getBlockByName(cache.team_packets, "team_packets", false);
  memory->getBlockByName(cache.frame_info, "vision_frame_info", false);
  memory->getBlockByName(cache.robot_state, "robot_state", false);
  memory->getBlockByName(cache.game_state, "game_state", false);
  memory->getBlockByName(cache.odometry, "vision_odometry", false);
  memory->getBlockByName(cache.joint, "vision_joint_angles", false);
  memory->getBlockByName(cache.behavior, "behavior", false);
  memory->getBlockByName(cache.sonar, "vision_processed_sonar", false);
  memory->getBlockByName(cache.delayed_localization, "delayed_localization", false);
  memory->getBlockByName(cache.body_model, "body_model", false);
  memory->getBlockByName(cache.image, "raw_image", false);
  memory->getBlockByName(cache.camera, "camera_info", false);
  memory->getBlockByName(cache.robot_vision, "robot_vision", false);
  memory->getBlockByName(cache.robot_info, "robot_info", false);
  memory->getBlockByName(cache.sensor, "processed_sensors", false);
  memory->getBlockByName(cache.opponent_mem, "opponents", false);
  memory->getBlockByName(cache.behavior_params, "behavior_params", false);
  memory->getBlockByName(cache.walk_request, "vision_walk_request", false);
  memory->getBlockByName(cache.kick_request, "vision_kick_request", false);
  memory->getBlockByName(cache.walk_params, "vision_al_walk_param", false);
  memory->getBlockByName(cache.joint_command, "vision_joint_commands", false);
  memory->getBlockByName(cache.walk_info, "vision_walk_info", false);
  memory->getBlockByName(cache.sim_truth, "sim_truth_data", false);
  memory->getBlockByName(cache.audio_processing, "audio_processing", false);
  return cache;
}

MemoryCache MemoryCache::create(int team, int player) {
  Memory* memory = new Memory(false, MemoryOwner::TOOL_MEM, team, player);
  MemoryCache cache(memory);
  cache.world_object->init();
  cache.robot_state->WO_SELF = cache.robot_state->global_index_ = player;
  cache.robot_state->team_ = team;
  return cache;
}
