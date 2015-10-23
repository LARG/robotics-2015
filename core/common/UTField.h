#ifndef _FIELD_H
#define _FIELD_H

#include "WorldObject.h"
#include <stdio.h>
#include <array>

//#ifdef interface_H
//cout << "Field_h";
//#endif


// setup constants for field size, landmark locations, etc

const float BALL_RADIUS = 31;

const float FIELD_Y = 3000;
const float FIELD_X = 1900 * 2;
const float GRASS_Y = 4000;
const float GRASS_X = 5000;

const float HALF_FIELD_Y = FIELD_Y/2.0;
const float HALF_FIELD_X = FIELD_X/2.0;
const float HALF_GRASS_Y = GRASS_Y/2.0;
const float HALF_GRASS_X = GRASS_X/2.0;

const float GOAL_Y = 900;
const float GOAL_POST_WIDTH = 70;
const float GOAL_WIDTH = GOAL_Y + GOAL_POST_WIDTH;
const float GOAL_X = 500;
const float HALF_GOAL_Y = GOAL_Y / 2.0;
const float PENALTY_Y = 1400;
const float PENALTY_X =  650;
const float CIRCLE_DIAMETER = 1500;
const float CIRCLE_RADIUS = CIRCLE_DIAMETER / 2.0;
const float LINE_WIDTH = 50;

const float GOAL_HEIGHT = 325;

const float FIELD_CENTER_X = 0;
const float FIELD_CENTER_Y = 0;

const float PENALTY_CROSS_X = HALF_FIELD_X - 1300;
const float PENALTY_MARK_SIZE = 106;

const float CIRCLE_HEX_LENGTH = 2.0*(CIRCLE_RADIUS*sinf(DEG_T_RAD*30.0));

// Some rectangles

const Rectangle FIELD =
Rectangle( Point2D( -FIELD_X / 2,  FIELD_Y / 2 ),
	   Point2D(  FIELD_X / 2, -FIELD_Y / 2 ) );

const Rectangle GRASS =
Rectangle( Point2D( -GRASS_X / 2,  GRASS_Y / 2 ),
	   Point2D(  GRASS_X / 2, -GRASS_Y / 2 ) );

// circle and cross points
const Point2D circleLocation = Point2D(0, 0);
const Point2D oppCrossLocation = Point2D(PENALTY_CROSS_X, 0);
const Point2D ownCrossLocation = Point2D(-PENALTY_CROSS_X, 0);


// Landmark locations
const Point2D landmarkLocation[] = {
  Point2D(0, 0),  // center circle

  Point2D(HALF_FIELD_X, HALF_FIELD_Y),       //  WO_BEACON_BLUE_YELLOW
  Point2D(HALF_FIELD_X, -HALF_FIELD_Y),      //  WO_BEACON_YELLOW_BLUE,
  Point2D(0, HALF_FIELD_Y),                  //  WO_BEACON_BLUE_PINK
  Point2D(0, -HALF_FIELD_Y),                  //  WO_BEACON_PINK_BLUE
  Point2D(-HALF_FIELD_X, HALF_FIELD_Y),      //  WO_BEACON_PINK_YELLOW
  Point2D(-HALF_FIELD_X, -HALF_FIELD_Y),     //  WO_BEACON_YELLOW_PINK,

  Point2D( -FIELD_X / 2, 0),                 // WO_OWN_GOAL
  Point2D( FIELD_X / 2, 0 ),                 // WO_OPP_GOAL

  Point2D( -FIELD_X / 2, -GOAL_Y / 2),       // WO_OWN_LEFT_GOALPOST
  Point2D( FIELD_X / 2, GOAL_Y / 2 ),        // WO_OPP_LEFT_GOALPOST

  Point2D( -FIELD_X / 2, GOAL_Y / 2),        // WO_OWN_RIGHT_GOALPOST
  Point2D( FIELD_X / 2, -GOAL_Y / 2)         // WO_OPP_RIGHT_GOALPOST
};
  

#ifndef SWIG
template<typename T, typename... Args>
auto make_array(T arg, Args... args) -> std::array<T, 1 + sizeof...(Args)>
{
     std::array<T, 1 + sizeof...(Args)> a = { arg, args ... };
     return a;
}
#endif

const auto lineLocationStarts = make_array(
  Point2D(-HALF_FIELD_X, PENALTY_Y/2),
  Point2D(-HALF_FIELD_X + PENALTY_X, PENALTY_Y/2),
  Point2D(-HALF_FIELD_X, PENALTY_Y/2),
  Point2D(-HALF_FIELD_X, -PENALTY_Y/2),
  Point2D(HALF_FIELD_X, PENALTY_Y/2),
  Point2D(HALF_FIELD_X - PENALTY_X, PENALTY_Y/2),
  Point2D(HALF_FIELD_X, PENALTY_Y/2),
  Point2D(HALF_FIELD_X, -PENALTY_Y/2),
  Point2D(0, HALF_FIELD_Y)
);

const auto lineLocationEnds = make_array(
  Point2D(-HALF_FIELD_X, -PENALTY_Y/2),
  Point2D(-HALF_FIELD_X + PENALTY_X, -PENALTY_Y/2),
  Point2D(-HALF_FIELD_X + PENALTY_X, PENALTY_Y/2),
  Point2D(-HALF_FIELD_X + PENALTY_X, -PENALTY_Y/2),
  Point2D(HALF_FIELD_X, -PENALTY_Y/2),
  Point2D(HALF_FIELD_X - PENALTY_X, -PENALTY_Y/2),
  Point2D(HALF_FIELD_X - PENALTY_X, PENALTY_Y/2),
  Point2D(HALF_FIELD_X - PENALTY_X, -PENALTY_Y/2),
  Point2D(0, -HALF_FIELD_Y)
);

#endif
