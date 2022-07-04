#pragma once

#include <cstdlib>

#define CONSISTENT_ASSERT(CONDITION) \
{                                    \
  if (!(CONDITION)) {                \
    std::abort(); \
  }                                  \
}