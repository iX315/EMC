#ifndef UTILS_H
#define UTILS_H

int clamp(int value, int minValue, int maxValue, bool wrap = false) {
  if (value < minValue) {
    return wrap ? maxValue : minValue;
  } else if (value > maxValue) {
    return wrap ? minValue : maxValue;
  }
  return value;
}

#endif // UTILS_H