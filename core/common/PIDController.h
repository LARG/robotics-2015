#ifndef PIDCONTROLLER_AE3MN2H0
#define PIDCONTROLLER_AE3MN2H0

#include <math/Vector3.h>

class PIDController {
public:
  PIDController();
  PIDController(float cp, float ci, float cd, float eps = 0);
  float update(float current, float target = 0);
  void setParams(float cp, float ci, float cd);
  void setParams(const Vector3<float> &params);
  void cropCumulative(float max);
  void reset();

private:
  float cp_;
  float ci_;
  float cd_;
  float eps_;

  float current_error_; // corr. cp_
  float cumulative_error_; // corr. ci_
  float previous_error_; // corr. cd_
};

#endif /* end of include guard: PIDCONTROLLER_AE3MN2H0 */
