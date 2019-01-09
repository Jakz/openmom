#pragma once

#include "common/Typedefs.h"

#include <numeric>
#include <vector>

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


#pragma mark Unit Related

/*
This utility class is used to manage a vector of multiple values for a single unit,
this is useful for combat related formulas which generate damage, make rolls on separate
figures in many occasions
*/
class unit_figure_value : public std::vector<value_t>
{
public:
  template<typename T> using value_generator = std::function<value_type(const T*, size_t)>;

  unit_figure_value(size_t size) : std::vector<value_type>(size, 0) { }

  unit_figure_value(size_t size, std::function<value_type(size_t)> generator) : unit_figure_value(size)
  {
    for (size_t i = 0; i < size; ++i)
      (*this)[i] = generator(i);
  }

  /*TODO: requires #include unit_figure_value(aliveOnly ? unit->getUnit()->getAliveFigures() : unit->getUnit()->getFigures()) */
  unit_figure_value(size_t size, const combat::CombatUnit* unit, value_generator<combat::CombatUnit> generator) : unit_figure_value(size)
  {
    generate<combat::CombatUnit>(unit, generator);
  }

  unit_figure_value(size_t size, const Unit* unit, value_generator<Unit> generator) : unit_figure_value(size)
  {
    generate<Unit>(unit, generator);
  }

  using std::vector<value_t>::erase;
  void erase(size_t index) { std::vector<value_t>::erase(begin() + index); }

  value_type sum() const { return std::accumulate(begin(), end(), 0); }
  value_type max() const { return *std::max_element(begin(), end()); }

  void forEach(std::function<void(value_type&)> lambda) { std::for_each(begin(), end(), lambda); }

  void clampNegativeValuesToZero() { forEach([](value_type& v) { v = std::max(v, 0); }); }

  unit_figure_value& operator-=(value_t v) { std::for_each(begin(), end(), [v](value_t& value) { value -= v; }); return *this;  }
  unit_figure_value& operator+=(value_t v) { std::for_each(begin(), end(), [v](value_t& value) { value -= v; }); return *this;  }

  unit_figure_value operator-(const unit_figure_value& other) const
  {
    assert(size() == other.size());

    unit_figure_value result(*this);
    for (size_t i = 0; i < size(); ++i)
      result[i] -= other[i];
    return result;
  };

  unit_figure_value& operator-=(const unit_figure_value& other)
  {
    assert(size() == other.size());

    for (size_t i = 0; i < size(); ++i)
      (*this)[i] = other[i];

    return *this;
  };

  template<typename T>
  void generate(const T* unit, value_generator<T> generator)
  {
    for (size_t i = 0; i < size(); ++i)
      (*this)[i] = generator(unit, i);
  }
};
