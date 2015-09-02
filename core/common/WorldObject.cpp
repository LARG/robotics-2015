#include "WorldObject.h"

// Create the array of objects
//WorldObject worldObjects[NUM_WORLD_OBJS];

WorldObject::WorldObject() {
  seen = false;

  frameLastSeen = -1;
  type = WO_INVALID;
  visionDistance=0.0;
  visionElevation=0.0;
  visionBearing=0.0;
  distance=0.0;
  elevation=0.0;
  bearing=0.0;
  relVel=Vector2D(0.0, 0.0);
  loc = Point2D(0, 0);
  height = 0;
  lineLoc = LineSegment(loc, loc);
  orientation=0.0;
  sd= Point2D(0.0, 0.0);
  sdOrientation=0.0;
  absVel= Point2D(0.0, 0.0);
  imageCenterX=0;
  imageCenterY=0;
  fieldLineIndex = -1;
  fromTopCamera = false;
}

WorldObject::~WorldObject() {
}

bool WorldObject::isUnknown() const {
  return isUnknownPost() || isUnknownGoalCenter() || isUnknownGoal() || isUnknownIntersection() || isUnknownLine() || isUnknownPenaltyCross();
}

bool WorldObject::isLine() const {
  return isUnknownLine() || isKnownLine();
}

bool WorldObject::isKnownLine() const {
  return (type >= LINE_OFFSET && type < (LINE_OFFSET + NUM_LINES));
}

bool WorldObject::isLandmark() const {
  return (type >= LANDMARK_OFFSET && type < (LANDMARK_OFFSET + NUM_LANDMARKS));
}

bool WorldObject::isGoal() const {
  return (type >= WO_OWN_GOAL && type <= WO_UNKNOWN_GOALPOST);
}

bool WorldObject::isGoalCenter() const {
  return (type == WO_OPP_GOAL || type == WO_OWN_GOAL || type == WO_UNKNOWN_GOAL);
}

bool WorldObject::isGoalPost() const {
  return (isGoal() && !isGoalCenter());
}

bool WorldObject::isUnknownPost() const {
  return (type >= WO_UNKNOWN_LEFT_GOALPOST && type <= WO_UNKNOWN_GOALPOST);
}

bool WorldObject::isUnknownGoal() const {
  return (type >= WO_UNKNOWN_GOAL && type <= WO_UNKNOWN_GOALPOST);
}

bool WorldObject::isUnknownGoalCenter() const {
  return type == WO_UNKNOWN_GOAL;
}

bool WorldObject::isOwnGoal() const {
  return (type == WO_OWN_GOAL || type == WO_OWN_RIGHT_GOALPOST || type == WO_OWN_LEFT_GOALPOST);
}

bool WorldObject::isOppGoal() const {
  return (type == WO_OPP_GOAL || type == WO_OPP_RIGHT_GOALPOST || type == WO_OPP_LEFT_GOALPOST);
}

bool WorldObject::isBall() const {
  return (type == WO_BALL);
}

bool WorldObject::isIntersection() const {
  return isT() || isL();
}

bool WorldObject::isT() const {
  return (type >= WO_OPP_PEN_LEFT_T && type <= WO_OWN_PEN_LEFT_T);
}

bool WorldObject::isL() const {
  return (type >= WO_OPP_FIELD_LEFT_L && type <= WO_OWN_FIELD_LEFT_L);
}

bool WorldObject::isUnknownIntersection() const {
  return (type >= WO_UNKNOWN_L_1 && type <= WO_UNKNOWN_T_2);
}

bool WorldObject::isUnknownT() const {
  return (type >= WO_UNKNOWN_T_1 && type <= WO_UNKNOWN_T_2);
}

bool WorldObject::isUnknownL() const {
  return (type >= WO_UNKNOWN_L_1 && type <= WO_UNKNOWN_L_2);
}

bool WorldObject::isUnknownLine() const {
  return (type >= WO_UNKNOWN_FIELD_LINE_1 && type <= WO_UNKNOWN_FIELD_LINE_4);
}

bool WorldObject::isTeammate() const {
  return (type >= WO_TEAM_FIRST && type <= WO_TEAM_LAST);
}

bool WorldObject::isOpponent() const {
  return (type >= WO_OPPONENT_FIRST && type <= WO_OPPONENT_LAST);
}

bool WorldObject::isCenterCircle() const {
  return type == WO_CENTER_CIRCLE;
}

bool WorldObject::isUnknownPenaltyCross() const {
  return type == WO_UNKNOWN_PENALTY_CROSS;
}

bool WorldObject::isKnownPenaltyCross() const {
  return (type == WO_OPP_PENALTY_CROSS || type == WO_OWN_PENALTY_CROSS);
}

bool WorldObject::isUnique() const {
  return isCenterCircle() || isKnownLine() || isKnownPenaltyCross();  // Center circle and known lines
}

bool WorldObject::isPenaltySideline() const {
  return (type >= WO_PENALTY_TOP_OPP && type <= WO_PENALTY_BOTTOM_OWN);
}

bool WorldObject::isAmbiguous() const {
  return (isUnknownGoal() || isUnknownIntersection() || isUnknownLine() || isUnknownPenaltyCross());
}

bool WorldObject::isHorizontalLine() const {
  return type >= WO_OPP_GOAL_LINE && type <= WO_OWN_BOTTOM_GOALBAR;
}

bool WorldObject::isVerticalLine() const {
  return type >= WO_TOP_SIDE_LINE && type <= WO_OWN_RIGHT_GOALBAR;
}

bool WorldObject::isBeacon() const {
  return type >= WO_BEACON_BLUE_YELLOW && type <= WO_BEACON_YELLOW_PINK;
}

void WorldObject::reset(){
  seen = false;
  fieldLineIndex = -1;
}
