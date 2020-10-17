#pragma once

#include "logger.hpp"

class test_class {
public:
  test_class() { log::INFO("in ctr"); }
  ~test_class() { log::INFO("delete cls"); }
};
