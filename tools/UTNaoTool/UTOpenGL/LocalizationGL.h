#ifndef LOCALIZATION_GL_H
#define LOCALIZATION_GL_H

#include <QGLViewer/qglviewer.h>
#include "BasicGL.h"
#include "ObjectsGL.h"
#include "RobotGL.h"

#include <math/Pose3D.h>

#include <memory/WorldObjectBlock.h>
#include <memory/OdometryBlock.h>
#include <memory/RobotStateBlock.h>
#include <memory/LocalizationBlock.h>

class LocalizationGL {
public:
  void drawUncertaintyEllipse(Point2D loc, Point2D sd);
  void drawUncertaintyEllipse(Point2D loc, Eigen::Matrix2f cov);
  void drawUncertaintyEllipse(Pose2D pose, Eigen::Matrix2f cov);
  void drawUncertaintyAngle(Point2D loc, double ori, double sdOri);
  void drawUncertaintyAngle(Pose2D pose, double var);


  void drawRotatedUncertaintyEllipse(Point2D loc, double p00, double p01, double p10, double p11);
  void drawRotatedUncertaintyEllipse(Point2D loc, Point2D sd, double rot);
  void SolveQuadratic(double* x, double *y, double a, double b, double c);

  void drawRelativeObjects(WorldObjectBlock* gtObjects, WorldObjectBlock* beliefObjects, RobotStateBlock* robotState);
  void drawRelativeObjectUncerts(WorldObjectBlock* gtObjects, WorldObjectBlock* beliefObjects, RobotStateBlock* robotState, LocalizationBlock* localization);
  void drawObservationLine(Vector3<float> origin, Vector3<float> end, RGB color);
 
  // Particle filter specific drawing  
  void drawOdometry(Point2D loc, AngRad ori, OdometryBlock* odometry);

  BasicGL basicGL;
  ObjectsGL objectsGL;
  RobotGL robotGL;
};


#endif


