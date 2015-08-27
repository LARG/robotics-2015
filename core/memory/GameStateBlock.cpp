#include <memory/GameStateBlock.h>

void GameStateBlock::setState(int state) {
  // If we've heard the whistle recently don't go back into set
  if(whistleElapsedTime() <= 20 && state == SET && state_ == PLAYING) return;

  // If we've heard the whistle recently and they try to penalize us, go into set
  const int whistleWindow = 30;
  static_assert(whistleWindow <= 40, "Don't increase the whistle window past 40 or you risk getting lost after a legitimate penalty.");
  if(whistleElapsedTime() <= whistleWindow && state == PENALISED && (state_ == PLAYING || state_ == SET))
    state = SET;

  // Otherwise proceed normally
  if(state != state_) {
    stateStartTime_ = time(NULL);
    prevstate_ = state_;
    state_ = state;
  }
}

void GameStateBlock::whistleOverride() {
  whistleTime = time(NULL);
  setState(PLAYING);
}
