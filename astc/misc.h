#ifndef ASTC_MISC_H_
#define ASTC_MISC_H_

#include <math.h>

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

inline int min4_index(int is[4]) {
  int group0 = is[0] < is[1] ? 0 : 1;
  int group1 = is[2] < is[3] ? 2 : 3;

  return is[group0] < is[group1] ? group0 : group1;
}

#endif  // ASTC_MISC_H_
