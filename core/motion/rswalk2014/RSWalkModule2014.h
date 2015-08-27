#pragma once

#include <motion/WalkModule.h>
#include <common/RobotInfo.h>
#include <common/WorldObject.h>//for rswalk2014
#include <memory/Memory.h>//for rswalk2014
#include <memory/WalkRequestBlock.h>
#include "WalkEnginePreProcessor.hpp"
#include "ClippedGenerator.hpp"
#include "BodyModel.hpp"
#include "types/Odometry.hpp"
#include "perception/kinematics/Kinematics.hpp"
///#include "Tools/RingBufferWithSum.h"
//#include "Tools/NormDist.h"


class WalkingEngine;

class Pose2D;
//class Pose2DBH;

class BodyModelBlock;
class FrameInfoBlock;
class GameStateBlock;
class JointBlock;
class JointCommandBlock;
class KickRequestBlock;
class OdometryBlock;
class RobotInfoBlock;
class SensorBlock;
class WalkInfoBlock;
class WalkParamBlock;
class WalkRequestBlock;
class WorldObjectBlock; //for rswalk2014
class SpeechBlock;
class GameStateBlock;

class RSWalkModule2014: public WalkModule {
    public:
        RSWalkModule2014();
        ~RSWalkModule2014();

        void specifyMemoryDependency();
        void specifyMemoryBlocks();
        void initSpecificModule();

	void processTargetModeWalk(ActionCommand::Body &body);
	void processWalkRequest(ActionCommand::Body &body);
        void processFrame();
	void readOptions(std::string path);
        void handleStepIntoKick();

    private:
/*        inline float circle(float x)
        {
            return 1 - sqrt(1-pow(x,2));
        }

        int sgn(float val)
        {
            if(val >= 0)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        void processWalkParams();
        void processWalkRequest(); */
        void setArms(Joints angles, float timeInSeconds);
        void getArmsForState(int state, Joints angles);
        void determineStartingArmState();

//        void setMassCalibration();
//        void setRobotDimensions();

        void selectivelySendStiffness();

//        void setPose2D(Pose2D &dest, const Pose2DBH &src);

        bool doingSlowStand();
        void doSlowStand();

        bool shouldStartSlowStand();
        void startSlowStand();
//        float standJointErr(int joint);

        bool readyToStartKickAfterStep();

    private:
	// Memory blocks
        FrameInfoBlock *frame_info_;
        JointBlock *raw_joints_;
        JointBlock *joints_;
        JointCommandBlock *commands_;
        KickRequestBlock *kick_request_;
        OdometryBlock *odometry_;
        RobotInfoBlock *robot_info_;
        SensorBlock *sensors_;
        WalkInfoBlock *walk_info_;
        WalkParamBlock *walk_params_;
        WalkRequestBlock *walk_request_;
        BodyModelBlock* body_model_;        
	WorldObjectBlock* world_objects_;//for rswalk2014
	SpeechBlock *speech_;
	GameStateBlock *game_state_;

	// Walk member variables
	ClippedGenerator* clipper;
	WalkEnginePreProcessor* generator;
	BodyModel bodyModel;
	Kinematics kinematics;
	Odometry odometry_disp;
	bool wasKicking;

	float prevForward, prevLeft, prevTurn;

	// standing value for RS - Josiah
	volatile bool standing;
	WalkRequestBlock::Motion prev_command;

	// For lag. Not sure if we will use - Josiah
	std::vector<Odometry> odometryBuffer; 
	std::vector<SensorValues> sensorBuffer;

/*
        // Stand Stability
        const float UKP = 0.04f;
        const float UKD = 0.02f;
        const float KP = 0.30f;
        const float KD = 0.05f;
        const float RKP = 0.12f;
        const float RKD = 0.025f;
        const float SMOOTH = 0.65;
        const float HIPWAIT = 5;
        const float XACCEL = 0.10f;
        const float YACCEL = 0.15f;
        const float WAIT = 2;
        const float HIPSTAND = (DEG_T_RAD * -35.0f);
        const float HIPROLL = (DEG_T_RAD * -0.26f);
        const float ANKLEROLL = (DEG_T_RAD * 0.0f);
        const float ANKLEPITCH = (DEG_T_RAD * 0.0f);
        const float MAXANKLE = (DEG_T_RAD * 10.0f);
        const float MINANKLE = (DEG_T_RAD * -10.0f);
        const float FORWARDHIP = (DEG_T_RAD * -55.0f);
        const float BACKWARDHIP = (DEG_T_RAD * -25.0f);
        const float MAXHIP = (DEG_T_RAD * 10.0f);
        const float MINHIP = (DEG_T_RAD * -10.0f);
        const float ABSMAXROLL = (DEG_T_RAD * 20.0f);
        const float ABSMINROLL = (DEG_T_RAD * -20.0f);
        const float DETECT = 0.1f;
        const float FLYING_DETECT = 0.4f;
        const float epsilon_fsr = 1.20f;

        bool left_foot_active = true;
        float fsr_front_;
        float fsr_back_;
        float fsr_right_;
        float fsr_left_;

        float x_prev_error_;
        float y_prev_error_;
        float hip_pitch_prev_angle_ = HIPSTAND;
        float hip_roll_prev_angle_ = HIPROLL;
        float hipframewait = HIPWAIT;

        //RingBufferWithSumRS<float, 10>* left_foot;
        //RingBufferWithSumRS<float, 10>* right_foot;

        // Walking Stability
        //NormDist<float, 10, 100>* sagittal_normdist;
        //NormDist<float, 10, 100>* coronal_normdist;
*/
        float turn_limit = 0.65f;
        float walking_turn_limit = 0.35f;
        const float walking_turn_threshold = 0.90f;
        const float backwards_limit = -0.75f;
/*
        // Direction change Walk
        bool dir_change_walk = false;
        int current_sgn = 0;
        float last_dir_change_speed = 0;
        float last_dir_speed = 0;
        int dir_change_speed = 0;
        int dir_change_wait = 0;
        int dir_change_wait_duration = 100;
        int dir_change_threshold = 10;

        // Walking Stabilty / Speed
        int instable_index = -1;
        int frames_slow_walk = 5;
        const int instable_wait = 10;
        const float deltaRate = 5.0f;
        const float MaxDurationDelta = 0.1f;
        const float BaseStepDuration = 525;
        const float MinStepDuration = 600;
        const float MaxStepDuration = 425;

        // zscore / speed_percentage - the fsr stability walk thresholds
        const float stability_limit[1] =
        {
            {0.85},
        };

        const float stepduration_limit[1] =
        {
            {0.50},
        };

        // General Stability Constants
        const int foot_contacts[8][2] = 
        {
            {fsrLFL,fsrLFR}, // left foot - front
            {fsrLRL,fsrLRR}, // left foot - back
            {fsrLFR,fsrLRR}, // left foot - right
            {fsrLFL,fsrLRL}, // left foot - left
            {fsrRFL,fsrRFR}, // right foot - front
            {fsrRRL,fsrRRR}, // right foot - back
            {fsrRFR,fsrRRR}, // right foot - right
            {fsrRFL,fsrRRL}, // right foot - left
        };

        const std::string contactNames[4] = {"left","right","front", "back"};
*/
        float slow_stand_start;
        float slow_stand_end;
        float walk_requested_start_time;

        //static const int jointMapping[NUM_JOINTS];
        int utJointToRSJoint[NUM_JOINTS];
	int utSensorToRSSensor[NUM_SENSORS];	

        float kick_distance_;
        float kick_angle_;
        bool prev_kick_active_;
        bool arms_close_to_targets_;
        int arm_state_;
        float arm_state_change_;
        float last_walk_or_stand_;

        Joints armStart;

        enum StepIntoKickState {
            PERFORMING,
            FINISHED_WITH_STEP,
            NONE
        };

        StepIntoKickState step_into_kick_state_;
        float time_step_into_kick_finished_;

	// Target walk
	float x_target;
	float y_target;
	float angle_target;
	bool target_walk_active;
	ActionCommand::Body::Foot kick_foot;

	// motion calibration
        double window_size = 0.002; // 1/# of frames, this needs tuning
        // threshold for calibration: if change of gyro reading over last n frames is small enough
        // note that this is rad/sec, not rad/frame     
        double delta_threshold = 1.0; // this needs tuning
	double reset = 10.0;
	double avg_gyroX = 0.0;
	double avg_delta_gyroX = 10.0; // this influence calibration speed at the first time when start motion
	double offsetX = 0.0;
	double last_gyroX;

	double avg_gyroY = 0.0;
	double avg_delta_gyroY = 10.0; 
	double offsetY = 0.0;	
	double last_gyroY;
	float last_gyroY_time;
	float last_gyroX_time;

	double calibration_write_time = -1.0;
	double last_calibration_write = -1.0;
	
	int calX_count = 0; //number of calibration performed
	int calY_count = 0;

	bool hasWalked = false;
    public:
        static const float STAND_ANGLES[NUM_JOINTS]; 
};
