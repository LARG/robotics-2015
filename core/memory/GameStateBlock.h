#ifndef GAMESTATE_
#define GAMESTATE_

#include "MemoryBlock.h"
#include <common/States.h>
#include <ctime>

struct GameStateBlock : public MemoryBlock {
public:
  GameStateBlock():
    isPenaltyKick(false)
  {
    header.version = 2;
    header.size = sizeof(GameStateBlock);


    prevstate_ = state_ = INITIAL;
    ourKickOff = true;
    isFirstHalf = 1;
    gameContTeamNum=1;
    ourScore = 0;
    opponentScore = 0;
    secsTillUnpenalised = 0;
    dropInTime = -1.0;
    lastOutBy = 0;
    secsRemaining = 600;
    frameReceived = 0;
    lastStateChangeFromButton = false;
    lastTimeLeftPenalized = -1;
    whistleTime =  0;
  }

  int state() { return state_; }
  int prevstate() { return prevstate_; }
  bool change() { return stateElapsedTime() == 0; }
  void setState(int state);
  int stateElapsedTime() {
    return time(NULL) - stateStartTime_;
  }
  int stateStartTime() {
    return stateStartTime_;
  }
  void whistleOverride();
  int whistleElapsedTime() { return time(NULL) - whistleTime; }

  // state of the robot (playing, ready, etc)

  // What team number are we on
  int gameContTeamNum;

  bool isPenaltyKick;

  // Game controller memory
  bool ourKickOff;
  int secsRemaining;
  int ourScore;
  int opponentScore;
  int secsTillUnpenalised;
  int isFirstHalf;   
  int lastOutBy;           
  int dropInTime;

  int frameReceived;
  int whistleTime;

  bool lastStateChangeFromButton;
  float lastTimeLeftPenalized;
private:
  int state_, prevstate_;
  int stateStartTime_;
};

#endif 
