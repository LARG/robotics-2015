#pragma once

#include <tool/simulation/ObservationGenerator.h>
#include <tool/simulation/IsolatedBehaviorSimulation.h>

class AttackerSimulation : public IsolatedBehaviorSimulation {
  public:
    AttackerSimulation();
    void simulationStep() override;
    void resetPlayer();
    void resetField();
};
