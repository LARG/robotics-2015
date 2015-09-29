#include "ObjectsGL.h"
#include <common/Field.h>

#include <iostream>
using namespace std;

void ObjectsGL::drawGreenCarpet() {
  glBegin(GL_POLYGON); 
  basicGL.colorRGB(0,100,0);
  glVertex3f (-HALF_GRASS_X/FACT,-HALF_GRASS_Y/FACT,-1);
  glVertex3f (-HALF_GRASS_X/FACT,HALF_GRASS_Y/FACT,-1);
  glVertex3f (HALF_GRASS_X/FACT,HALF_GRASS_Y/FACT,-1);
  glVertex3f (HALF_GRASS_X/FACT,-HALF_GRASS_Y/FACT,-1); 
  glEnd();  
}

void ObjectsGL::drawFieldLine(Point2D start, Point2D end) {
  basicGL.useFieldLineWidth();
  basicGL.colorRGB(Colors::White);
  basicGL.translate(0,0,2);
  basicGL.drawLine(start, end);
}

void ObjectsGL::drawIntersection(Point2D p, float alpha) {
  glPushMatrix();
  basicGL.colorRGBAlpha(Colors::Pink,alpha);  
  basicGL.translate(p,0.0);
  basicGL.drawSphere(BALL_RADIUS);
  glPopMatrix();;
}

void ObjectsGL::drawPenaltyCross(Point2D p, float alpha){
  glPushMatrix();
  basicGL.colorRGBAlpha(Colors::White,alpha);
  Point2D px1 = p;
  px1.x -= 50;
  Point2D px2 = p;
  px2.x += 50;
  Point2D py1 = p;
  py1.y -= 50;
  Point2D py2 = p;
  py2.y += 50;
  basicGL.drawLine(px1, px2);
  basicGL.drawLine(py1, py2);
  glPopMatrix();
}

void ObjectsGL::drawCenterCircle(Point2D p, float alpha){
  glPushMatrix();
  basicGL.useFieldLineWidth();
  basicGL.colorRGB(Colors::White);
  basicGL.colorRGBAlpha(Colors::White,alpha);  
  basicGL.translate(p,0.0);
  basicGL.drawCircle(CIRCLE_RADIUS);
  glPopMatrix();
}

void ObjectsGL::drawLinePoint(Point2D p, float alpha) {
  glPushMatrix();
  basicGL.colorRGBAlpha(Colors::White,alpha);  
  basicGL.translate(p,0.0);
  basicGL.drawSphere(BALL_RADIUS);
  glPopMatrix();;
}

void ObjectsGL::drawBall(Point2D p, float alpha) {
  drawBallColor(p,alpha,Colors::Orange);
}

void ObjectsGL::drawBallColor(Point2D p, float alpha, RGB color){
  glPushMatrix();
  basicGL.colorRGBAlpha(color,alpha);
  basicGL.translate(p,BALL_RADIUS);
  basicGL.drawSphere(BALL_RADIUS);
  glPopMatrix();
}
  

void ObjectsGL::drawBallVel(Point2D p, Vector2D vel, float alpha) {
  drawBallVelColor(p, vel, alpha, Colors::Red);
}

void ObjectsGL::drawBallVelColor(Point2D p, Vector2D vel, float alpha, RGB color) {
  glPushMatrix();
  basicGL.translate(0,0,25);
  basicGL.colorRGBAlpha(color,alpha);
  basicGL.setLineWidth(50);
  basicGL.drawLine(p, p+vel);
  glPopMatrix();
}

void ObjectsGL::drawGoal(Point2D p1, float alpha) {
  basicGL.colorRGBAlpha(Colors::Blue, alpha);
  drawGoal(p1);
}

void ObjectsGL::drawPost(Point2D p1, float alpha) {
  basicGL.colorRGBAlpha(Colors::Blue, alpha);
  drawGoalPost(p1);
}

void ObjectsGL::drawGoalPost(Point2D p) {
 glPushMatrix();  
 basicGL.translate(p);
 basicGL.drawCylinder(50.0f,GOAL_HEIGHT);
 glPopMatrix();  
}

void ObjectsGL::drawCrossBar(Point2D goalCenter) {
 glPushMatrix();  

 basicGL.translate(goalCenter,0.95*GOAL_HEIGHT);
 basicGL.rotateXDeg(90.0);
 basicGL.drawCylinder(30.0f,GOAL_Y/2.0);
 basicGL.rotateXDeg(-180.0);
 basicGL.drawCylinder(39.0f,GOAL_Y/2.0);
  
 glPopMatrix();  
}

void ObjectsGL::drawGoal(Point2D goalCenter) { 
  Point2D post;
  post.x=goalCenter.x;
  post.y=goalCenter.y+GOAL_Y/2.0;
  drawGoalPost(post); //left

  post.y=goalCenter.y-GOAL_Y/2.0;
  drawGoalPost(post); //right

  drawCrossBar(goalCenter);
}

void ObjectsGL::drawBeacon(Point2D p, RGB topColor, RGB bottomColor, float alpha) {
  p /= FACT;
  basicGL.colorRGBAlpha(topColor, alpha);
  basicGL.drawCylinder(p.x, p.y, 300 / FACT, p.x, p.y, 400 / FACT, 110 / FACT);
  basicGL.colorRGBAlpha(bottomColor, alpha);
  basicGL.drawCylinder(p.x, p.y, 200 / FACT, p.x, p.y, 300 / FACT, 110 / FACT);
  basicGL.colorRGBAlpha(Colors::White, alpha);
  basicGL.drawCylinder(p.x, p.y, 0, p.x, p.y, 200 / FACT, 110 / FACT);
}

