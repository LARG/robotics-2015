#ifndef _WORLD_OBJECT_H
#define _WORLD_OBJECT_H

#include <math/Geometry.h>
#include <common/Enum.h>

/**
 * \enum The types of world objects that exist.
 */
ENUM(WorldObjectType,   // Types of objects
  WO_BALL, 

  //@{
  /// Self and Teammates
  WO_TEAM1,
  WO_TEAM2,
  WO_TEAM3,
  WO_TEAM4,
  WO_TEAM5,
  //@}

  // Opponents?
  WO_OPPONENT1,
  WO_OPPONENT2,
  WO_OPPONENT3,
  WO_OPPONENT4,
  WO_OPPONENT5,

  // Coaches
  WO_TEAM_COACH,
  WO_OPPONENT_COACH,
  WO_TEAM_LISTENER,

  /// center cirlce
  WO_CENTER_CIRCLE,

  WO_BEACON_BLUE_YELLOW,
  WO_BEACON_YELLOW_BLUE,
  WO_BEACON_BLUE_PINK,
  WO_BEACON_PINK_BLUE,
  WO_BEACON_PINK_YELLOW,
  WO_BEACON_YELLOW_PINK,

  // 2 Landmark goals
  WO_OWN_GOAL,
  WO_OPP_GOAL,

  WO_OWN_LEFT_GOALPOST,
  WO_OPP_LEFT_GOALPOST,

  WO_OWN_RIGHT_GOALPOST,
  WO_OPP_RIGHT_GOALPOST,

  // Unknown goals and goal posts
  WO_UNKNOWN_GOAL,
  WO_UNKNOWN_LEFT_GOALPOST,
  WO_UNKNOWN_RIGHT_GOALPOST,
  WO_UNKNOWN_GOALPOST,

  // Field Corners
  WO_UNKNOWN_L_1,
  WO_UNKNOWN_L_2,
  WO_UNKNOWN_T_1,
  WO_UNKNOWN_T_2,

  // L intersections (yellow to blue, top to bottom)
  WO_OPP_FIELD_LEFT_L,
  WO_OPP_FIELD_RIGHT_L,
  WO_OPP_PEN_LEFT_L,
  WO_OPP_PEN_RIGHT_L,
  WO_OWN_PEN_RIGHT_L,
  WO_OWN_PEN_LEFT_L,
  WO_OWN_FIELD_RIGHT_L,
  WO_OWN_FIELD_LEFT_L,

  WO_OPP_BACK_RIGHT_GOAL_L,
  WO_OPP_BACK_LEFT_GOAL_L,
  
  WO_OWN_BACK_RIGHT_GOAL_L,
  WO_OWN_BACK_LEFT_GOAL_L,

  // T intersections (yellow to blue, top to bottom)
  WO_OPP_PEN_LEFT_T,
  WO_OPP_PEN_RIGHT_T,
  WO_CENTER_TOP_T,
  WO_CENTER_BOTTOM_T,
  WO_OWN_PEN_RIGHT_T,
  WO_OWN_PEN_LEFT_T,

  WO_OPP_FRONT_RIGHT_GOAL_T,
  WO_OPP_FRONT_LEFT_GOAL_T,

  WO_OWN_FRONT_RIGHT_GOAL_T,
  WO_OWN_FRONT_LEFT_GOAL_T,

  WO_UNKNOWN_FIELD_LINE_1,
  WO_UNKNOWN_FIELD_LINE_2,
  WO_UNKNOWN_FIELD_LINE_3,
  WO_UNKNOWN_FIELD_LINE_4,


  // Horizontal Lines (right to left)
  WO_OPP_GOAL_LINE,          
  WO_OPP_PENALTY,
  WO_CENTER_LINE,
  WO_OWN_PENALTY,
  WO_OWN_GOAL_LINE,

  WO_OPP_BOTTOM_GOALBAR,
  WO_OWN_BOTTOM_GOALBAR,

  // Vertical Lines (top to bottom)
  WO_TOP_SIDE_LINE,    
  WO_PENALTY_TOP_OPP,   
  WO_PENALTY_TOP_OWN,
  WO_PENALTY_BOTTOM_OPP, 
  WO_PENALTY_BOTTOM_OWN,       
  WO_BOTTOM_SIDE_LINE,   
  
  WO_OPP_LEFT_GOALBAR,
  WO_OPP_RIGHT_GOALBAR,
  WO_OWN_LEFT_GOALBAR,
  WO_OWN_RIGHT_GOALBAR,

  // penalty crosses
  WO_UNKNOWN_PENALTY_CROSS,
  WO_OWN_PENALTY_CROSS,
  WO_OPP_PENALTY_CROSS,

  // cluster of robots
  WO_ROBOT_CLUSTER,

  NUM_WORLD_OBJS,

  WO_INVALID
);


const int WO_OPPONENT_FIRST=WO_OPPONENT1;
const int WO_OPPONENT_LAST=WO_OPPONENT5;

const int WO_TEAM_FIRST=WO_TEAM1;
const int WO_TEAM_FIELD_FIRST=WO_TEAM2;
const int WO_TEAM_LAST=WO_TEAM5;
const int NUM_PLAYERS = WO_TEAM_LAST - WO_TEAM_FIRST + 1;

const int WO_PLAYERS_FIRST=WO_TEAM_FIRST;
const int WO_PLAYERS_LAST=WO_OPPONENT_LAST;
const int WO_ROBOTS_LAST=WO_OPPONENT_COACH;

// some constants to easily see which class each type is
const int WO_GOAL_FIRST = WO_OWN_GOAL;
const int WO_GOAL_LAST = WO_UNKNOWN_GOALPOST;
const int LANDMARK_OFFSET = WO_CENTER_CIRCLE;
const int NUM_LANDMARKS = WO_OPP_RIGHT_GOALPOST - LANDMARK_OFFSET + 1;
const int INTERSECTION_OFFSET =  WO_OPP_FIELD_LEFT_L;
const int NUM_INTERSECTIONS = WO_OWN_FRONT_LEFT_GOAL_T - INTERSECTION_OFFSET + 1;
const int LINE_OFFSET =  WO_OPP_GOAL_LINE;
const int NUM_LINES = WO_OWN_RIGHT_GOALBAR - LINE_OFFSET + 1;

const int L_INT_OFFSET = WO_OPP_FIELD_LEFT_L;
const int T_INT_OFFSET = WO_OPP_PEN_LEFT_T;

const int NUM_L_INTS = 8;
const int NUM_T_INTS = 6;

const int CROSS_OFFSET = WO_OWN_PENALTY_CROSS;
const int NUM_CROSSES = 2;

const int NUM_UNKNOWN_L = 2;
const int NUM_UNKNOWN_T = 2;
const int NUM_UNKNOWN_LINES = 4;
const int NUM_UNKNOWN_POSTS = 3;


/**
 * A WorldObject is used to refer to any object in the game. This
 * encompasses everything from robots, the ball, the goals, etc.
 * Everything interactable on the field is a world object.
 */
class WorldObject {
public:

  WorldObject();
  ~WorldObject();

  // quick functions to determine which class of object this is
  bool isLine() const;
  bool isLandmark() const;
  bool isGoal() const;
  bool isGoalCenter() const;
  bool isGoalPost() const;
  bool isUnknown() const;
  bool isUnknownPost() const;
  bool isUnknownGoalCenter() const;
  bool isUnknownGoal() const;
  bool isBall() const;
  bool isIntersection() const;
  bool isT() const;
  bool isL() const;
  bool isTeammate() const;
  bool isOpponent() const;
  bool isUnknownIntersection() const;
  bool isUnknownLine() const;
  bool isCenterCircle() const;
  bool isUnknownPenaltyCross() const;
  bool isKnownPenaltyCross() const;
  bool isUnique() const;
  bool isAmbiguous() const;
  bool isPenaltySideline() const;
  bool isOwnGoal() const;
  bool isOppGoal() const;
  bool isKnownLine() const;
  bool isUnknownT() const;
  bool isUnknownL() const;
  bool isHorizontalLine() const;
  bool isVerticalLine() const;
  bool isBeacon() const;

  void reset();

  bool seen;
  int frameLastSeen;
  WorldObjectType type, utype;


  // These are relative to the robot and come directly from vision
  float visionDistance; ///< Distance from the object to the robot, as estimated directly by vision.
  float visionElevation; ///< not used
  AngRad visionBearing; ///< The relative angle between this object and the robot estimated directly by vision.
  float visionConfidence; ///< The confidence of the visionDistance and visionBearing estimates.

  // These are relative to the robot but come from the filtered localization
  float distance; ///< Relative distance to the robot.
  float elevation; ///< How high off the ground this object is relative to the robot.
  AngRad bearing; ///< The relative angle between this object and the robot.
  //float sdDistance;   // Commented out ones are things we havn't decided on
  //float sdElevation;
  //AngRad sdBearing;

  Vector2D relVel; ///< The relative velocity between the robot and this object.
  
  //Vector2D sdRelVel;

  // These are the global position after localization (including confidence)
  // or permanent locations for landmarks and unmovable objects
  Point2D loc; ///< The object's location in global field coordinates.
  Point2D endLoc;
  float height; ///< The hight of the object from the field floor.

  // for lines
  // for unknown lines this is the relative x,y (x in front, y to left) of the end points of the observed line segment
  // for known lines this is the location on the field
  LineSegment lineLoc;

  // for seen lines
  Point2D visionPt1; ///< For lines, the first seen point.
  Point2D visionPt2; ///< For lines, the second seen point.
  LineSegment visionLine; ///< For lines, the line segment.

  // heights of beacons/goals/etc in the world
  float lowerHeight; ///< The height of the lower part of this object.
  float upperHeight; ///< The height of the upper part of this object.

  AngRad orientation;
  Point2D sd;
  AngRad sdOrientation;
  Vector2D absVel;

  Point2D relPos;
  AngRad relOrientation;

  // Vision
  int imageCenterX; ///< The object's center x in the frame image.
  int imageCenterY; ///< The object's center y in the frame image.
  float radius; ///< The radius of this object in the frame image.
  int fieldLineIndex;
  int ballBlobIndex;

  bool fromTopCamera; ///< True if observed from top cam, false if from bottom
};

typedef std::vector<WorldObject> ObjectSet;

#endif
