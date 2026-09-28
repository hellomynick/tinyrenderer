#include <cmath>
struct vec3 {
  float x = 0, y = 0, z = 0;

  float &operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
  float operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
};

inline vec3 operator+(const vec3 &a, const vec3 &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline vec3 operator-(const vec3 &a, const vec3 &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline vec3 operator*(const vec3 &a, int s) {
  return {a.x * s, a.y * s, a.z * s};
}

inline float dot(const vec3 &a, const vec3 &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3 &a, const vec3 &b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

inline float norm(const vec3 &v) { return std::sqrt(dot(v, v)); }
