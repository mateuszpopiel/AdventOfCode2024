#pragma once
#include <functional>

template <class T> struct Coordinates {
  T x, y;

  constexpr Coordinates() : x(0), y(0) {}
  constexpr Coordinates(const T &first, const T &second) : x(first), y(second) {}

  constexpr Coordinates operator+(const Coordinates &other) const {
    return {static_cast<T>(x + other.x), static_cast<T>(y + other.y)};
  }
  constexpr Coordinates operator-(const Coordinates &other) const { return {x - other.x, y - other.y}; }
  constexpr Coordinates operator-(const T &shift) const { return {x - shift, y - shift}; }
  constexpr Coordinates operator*(const T &factor) const { return {x * factor, y * factor}; }
  constexpr Coordinates operator/(const T &divisor) const { return {x / divisor, y / divisor}; }
  constexpr Coordinates operator%(const Coordinates &modulus) const { return {x % modulus.x, y % modulus.y}; }
  constexpr Coordinates operator%(const T &modulus) const { return {x % modulus, y % modulus}; }
  constexpr Coordinates &operator+=(const Coordinates &other) { return *this + other; }

  constexpr bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
  constexpr bool operator!=(const Coordinates &other) const { return !(*this == other); }
  constexpr bool operator<(const Coordinates &other) const { return x < other.x || (x == other.x && y < other.y); }
  constexpr bool operator>(const Coordinates &other) const { return other < *this; }
  constexpr bool operator<=(const Coordinates &other) const { return !(*this > other); }
  constexpr bool operator>=(const Coordinates &other) const { return !(*this < other); }
};

namespace std {
template <class T> struct hash<Coordinates<T>> {
  std::size_t operator()(const Coordinates<T> &coords) const noexcept {
    return std::hash<T>()(coords.x) ^ (std::hash<T>()(coords.y) << 1);
  }
};
} // namespace std
