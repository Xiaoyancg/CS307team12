#include "gtest/gtest.h"
#include "Game.h"
#include <string>
unsigned int pageError = 1;
unsigned int gameError =1;

TEST ( GAMETEST, game )
{
  int argc = 2;
  char *argv[2];
  argv[0] = "core";
  argv[1] = "test";
  core_main ( argc, argv );
  EXPECT_EQ ( gameError, GL_NO_ERROR );
}
TEST ( GameTest, page )
{
  int argc = 2;
  char *argv[2];
  argv[0] = "core";
  argv[1] = "test";
  core_main ( argc, argv );
  EXPECT_EQ ( pageError, GL_NO_ERROR );
}
