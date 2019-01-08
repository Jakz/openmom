#pragma once

#include "common/Typedefs.h"

#pragma mark Geometric Types

using int_type = s16;
struct Size;

struct Point
{
  int_type x;
  int_type y;

  Point() noexcept : x(-1), y(-1) { }
  Point(int_type x, int_type y) noexcept : x(x), y(y) { }
  Point(const Size& size);
  template<typename T, typename std::enable_if<std::is_base_of<Point, T>::value, int>::type = 0>
  Point(const T& other) : x(other.x), y(other.y) { }

  bool operator!=(const Point& o) const { return !(*this == o); }
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }

  Point delta(int_type x, int_type y) const { return Point(this->x + x, this->y + y); }

  Point& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }
  Point& operator+=(int_type i) { x += i; y += i; return *this; }

  Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
  Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); }

  Point operator*(float v) const { return Point(static_cast<int_type>(x*v), static_cast<int_type>(y*v)); }

  Point operator-(int_type v) const { return Point(x + v, y + v); }

  bool isValid() const { return x != -1; }

  static Point INVALID;
  static Point ZERO;
};

struct Size
{
  int_type w;
  int_type h;

  Size() : w(0), h(0) { }
  Size(int_type w, int_type h) : w(w), h(h) { }
  Size(const Point& point);
  template<typename T, typename std::enable_if<std::is_base_of<Size, T>::value, int>::type = 0>
  Size(const Size& other) : w(other.w), h(other.h) { }

  bool operator==(const Size& size) const { return w == size.w && h == size.h; }
  bool operator!=(const Size& size) const { return !this->operator==(size); }

  Size operator*(int_type v) const { return Size(this->w*v, this->h*v); }
  Size& operator*=(int_type v) { this->w *= v; this->h *= v; return *this; }

  Size operator/(int_type v) const { return Size(this->w / v, this->h / v); }
  Size& operator/=(int_type v) { this->w /= v; this->h /= v; return *this; }
};

using Coord = Point;

struct Rect
{
  Point origin;
  Size size;

  Rect(int_type x, int_type y, int_type w, int_type h) : origin(x, y), size(w, h) { }
  Rect(const Point& origin, const Size& size) : origin(origin), size(size) { }

  inline int_type right() const { return origin.x + size.w; }
  inline int_type left() const { return origin.x; }
  inline int_type top() const { return origin.y; }
  inline int_type bottom() const { return origin.y + size.h; }

  bool contains(const Point& point) const
  {
    return point.x >= left() && point.x < right() && point.y >= top() && point.y <= bottom();
  }
};

inline Point::Point(const Size& size) : x(size.w), y(size.h) { }
inline Size::Size(const Point& point) : w(point.x), h(point.y) { }

#pragma mark Hash maps
