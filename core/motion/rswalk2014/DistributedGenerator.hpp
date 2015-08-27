#pragma once

#include <map>
#include <string>
#include "Generator.hpp"

class DistributedGenerator : Generator {
   public:
      explicit DistributedGenerator(std::string path);
      ~DistributedGenerator();
      virtual JointValues makeJoints(ActionCommand::All* request,
                                     Odometry* odometry,
                                     const SensorValues &sensors,
                                     BodyModel &bodyModel,
                                     float ballX,
                                     float ballY);
      virtual bool isActive();
      void reset();
      void readOptions(std::string path); //const boost::program_options::variables_map &config);
      bool isStopping;
      bool isStanding();
      bool walkIsActive();
      bool isLinedUp();
   public: // Made public -Josiah
      Generator* bodyGenerators[ActionCommand::Body::NUM_ACTION_TYPES];
      Generator* headGenerator;
      ActionCommand::Body::ActionType current_generator;
      ActionCommand::Body::ActionType prev_generator;
      ActionCommand::Body::ActionType requestedDive;
};
