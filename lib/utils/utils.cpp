#include "utils.hpp"

int clamp(int value, int minValue, int maxValue, bool wrap) {
  if (value < minValue) {
    return wrap ? maxValue : minValue;
  } else if (value > maxValue) {
    return wrap ? minValue : maxValue;
  }
  return value;
}