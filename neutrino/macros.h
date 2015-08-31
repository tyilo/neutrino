#pragma once

#include <algorithm>

#define CLAMP(x_min, x, x_max) std::min(std::max((x), (x_min)), (x_max))
#define BETWEEN(x_min, x, x_max) ((x_min) <= (x) && (x) <= (x_max))

#define BEGIN_END(x) (x).begin(), (x).end()
