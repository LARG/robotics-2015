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

std::vector<WorldObjectType> 
  FIELD_A_BEACONS = {
    WO_BEACON_BLUE_YELLOW, WO_BEACON_PINK_YELLOW, WO_BEACON_PINK_BLUE
  },
  FIELD_B_BEACONS = {
    WO_BEACON_YELLOW_BLUE, WO_BEACON_YELLOW_PINK, WO_BEACON_BLUE_PINK
  }
;
vector<Point2D> BEACON_COORDINATES = { Point2D(-100, -1250), Point2D(900, -1250), Point2D(400, 1250) };

AttackerSimulation::AttackerSimulation() : IBSim(true, CHASER) {
  FieldConfiguration config = {
    {WO_BALL, OP(BALL_X, BALL_Y)}
  };
  bcache_.robot_state->role_ = CHASER;
  gtcache_.game_state->setState(PLAYING);
  bcache_.game_state->setState(PLAYING);
  resetPlayer();
  config.place(bcache_.world_object);
  config.place(gtcache_.world_object);
  physics_.setObjects(gtcache_.world_object);
  resetPlayer();
  resetField();
}

void AttackerSimulation::simulationStep() {
  getObject(player, gtcache_.robot_state->WO_SELF);
  physics_.step();
  sim_.processFrame(gtcache_.world_object, gtcache_.game_state);
}

void AttackerSimulation::resetPlayer() {
  getObject(player, gtcache_.robot_state->WO_SELF);
  int side = rand_.sampleB() ? -1 : 1;
  player.loc = Point2D(PLAYER_X, side * PLAYER_Y);
  player.orientation = rand_.sampleU(0.0, side * -M_PI);
  printf("Initialized attacker at %2.f,%2.f @ %2.2f\n", player.loc.x, player.loc.y, player.orientation * RAD_T_DEG);
}

void AttackerSimulation::resetField() {
  bool fieldA = rand_.sampleB();
  auto used = fieldA ? FIELD_A_BEACONS : FIELD_B_BEACONS;
  auto hidden = fieldA ? FIELD_B_BEACONS : FIELD_A_BEACONS;
  for(int i = 0; i < used.size(); i++) {
    getObject(b, used[i]);
    b.loc = BEACON_COORDINATES[i];
  }
  for(int i = 0; i < hidden.size(); i++) {
    getObject(b, hidden[i]);
    b.loc = Point2D(10000,10000);
  }
}
