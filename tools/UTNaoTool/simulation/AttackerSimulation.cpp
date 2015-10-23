#include <tool/simulation/AttackerSimulation.h>
#include <common/FieldConfiguration.h>
#include <tool/UTMainWnd.h>

typedef IsolatedBehaviorSimulation IBSim;
typedef ObjectConfiguration OP;

#define BALL_X 0
#define BALL_Y 0
#define PLAYER_X 0
#define PLAYER_Y 850
#define _ORIENT 90
#define getObject(obj,idx) \
  auto& obj = gtcache_.world_object->objects_[idx];

AttackerSimulation::AttackerSimulation() : IBSim(true, CHASER) {
  FieldConfiguration config;
  if(!config.loadFromFile(UTMainWnd::dataDirectory() + "/asim_config.yaml")) {
    config = {
      {WO_BALL, OP(BALL_X, BALL_Y)},
      {WO_BEACON_BLUE_YELLOW, OP(-100, -1250)},
      {WO_BEACON_PINK_YELLOW, OP(900, -1250)},
      {WO_BEACON_YELLOW_PINK, OP(400, 1250)}
    };
  }
  config.saveToFile(UTMainWnd::dataDirectory() + "/asim_config.yaml");
  resetPlayer();
  bcache_.robot_state->role_ = KEEPER;
  gtcache_.game_state->setState(PLAYING);
  bcache_.game_state->setState(PLAYING);
  config.place(bcache_.world_object);
  config.place(gtcache_.world_object);
  physics_.setObjects(gtcache_.world_object);
  sim_.initLocalization();
}

void AttackerSimulation::simulationStep() {
  physics_.step();
  sim_.processFrame(gtcache_.world_object, gtcache_.game_state);
}

void AttackerSimulation::resetPlayer() {
  getObject(player, gtcache_.robot_state->WO_SELF);
  int side = rand_.sampleB() ? -1 : 1;
  player.loc = Point2D(PLAYER_X, side * PLAYER_Y);
}
