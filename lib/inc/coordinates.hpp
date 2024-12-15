#pragma once

template <class T> struct Coordinates {
  T x;
  T y;

  constexpr Coordinates operator+(const Coordinates &other) const { return {x + other.x, y + other.y}; }
  constexpr Coordinates operator-(const Coordinates &other) const { return {x - other.x, y - other.y}; }
  constexpr Coordinates operator-(const T &shift) const { return {x - shift, y - shift}; }
  constexpr Coordinates operator*(const T &factor) const { return {x * factor, y * factor}; }
  constexpr Coordinates operator/(const T &divisor) const { return {x / divisor, y / divisor}; }
  constexpr Coordinates operator%(const Coordinates &modulus) const { return {x % modulus.x, y % modulus.y}; }
  constexpr Coordinates operator%(const T &modulus) const { return {x % modulus, y % modulus}; }

  constexpr bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
  constexpr bool operator!=(const Coordinates &other) const { return !(*this == other); }
  constexpr bool operator<(const Coordinates &other) const { return x < other.x || (x == other.x && y < other.y); }
  constexpr bool operator>(const Coordinates &other) const { return other < *this; }
  constexpr bool operator<=(const Coordinates &other) const { return !(*this > other); }
  constexpr bool operator>=(const Coordinates &other) const { return !(*this < other); }
};
