#ifndef VECTOR_H_XKUIVULE
#define VECTOR_H_XKUIVULE

#include <math.h>

#include "astc/dcheck.h"

template <typename T>
union vec3_t {
 public:
  vec3_t() {}
  vec3_t(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

  struct {
    T x, y, z;
  };
  struct {
    T r, g, b;
  };
  T components[3];
};

typedef vec3_t<float> vec3f_t;
typedef vec3_t<int> vec3i_t;

template <typename T>
vec3_t<T> operator+(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return result;
}

template <typename T>
vec3_t<T> operator-(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

template <typename T>
vec3_t<T> operator*(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = a.x * b.x;
  result.y = a.y * b.y;
  result.z = a.z * b.z;
  return result;
}

template <typename T>
vec3_t<T> operator*(vec3_t<T> a, T b) {
  vec3_t<T> result;
  result.x = a.x * b;
  result.y = a.y * b;
  result.z = a.z * b;
  return result;
}

template <typename T>
vec3_t<T> operator/(vec3_t<T> a, T b) {
  vec3_t<T> result;
  result.x = a.x / b;
  result.y = a.y / b;
  result.z = a.z / b;
  return result;
}

template <typename T>
vec3_t<T> operator/(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = a.x / b.x;
  result.y = a.y / b.y;
  result.z = a.z / b.z;
  return result;
}

template <typename T>
bool operator==(vec3_t<T> a, vec3_t<T> b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
bool operator!=(vec3_t<T> a, vec3_t<T> b) {
  return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T>
T dot(vec3_t<T> a, vec3_t<T> b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T quadrance(vec3_t<T> a) {
  return dot(a, a);
}

template <typename T>
T norm(vec3_t<T> a) {
  return static_cast<T>(sqrt(quadrance(a)));
}

template <typename T>
T distance(vec3_t<T> a, vec3_t<T> b) {
  return norm(a - b);
}

template <typename T>
T qd(vec3_t<T> a, vec3_t<T> b) {
  return quadrance(a - b);
}

template <typename T>
vec3_t<T> signorm(vec3_t<T> a) {
  T x = norm(a);
  DCHECK(x != 0.0);
  return a / x;
}

template <typename T>
vec3_t<T> min(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = min(a.x, b.x);
  result.y = min(a.y, b.y);
  result.z = min(a.z, b.z);
  return result;
}

template <typename T>
vec3_t<T> max(vec3_t<T> a, vec3_t<T> b) {
  vec3_t<T> result;
  result.x = max(a.x, b.x);
  result.y = max(a.y, b.y);
  result.z = max(a.z, b.z);
  return result;
}

template <typename T>
T qd_to_line(vec3_t<T> m, vec3_t<T> k, T kk, vec3_t<T> p) {
  T t = dot(p - m, k) / kk;
  vec3_t<T> q = k * t + m;
  return qd(p, q);
}

#endif /* end of include guard: VECTOR_H_XKUIVULE */
