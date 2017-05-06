#ifndef MATH_H_S3TMCKJW
#define MATH_H_S3TMCKJW

#include <math.h>

template <typename T>
T min(T a, T b) {
  return a <= b ? a : b;
}
template <typename T>
T max(T a, T b) {
  return a >= b ? a : b;
}

template <typename T>
T clamp(T a, T b, T x) {
  if (x < a) {
    return a;
  }

  if (x > b) {
    return b;
  }

  return x;
}

inline bool approx_equal(float x, float y, float epsilon) {
  return fabs(x - y) < epsilon;
}

#endif /* end of include guard: MATH_H_S3TMCKJW */
