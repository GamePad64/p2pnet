/*
 * RunAllTests.cpp
 *
 *  Created on: 07.01.2013
 *      Author: gamepad
 */

#include "gtest/gtest.h"
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
