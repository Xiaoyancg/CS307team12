#include "gtest/gtest.h"
#include "Game.h"
#include <string>
unsigned int pageError = 1;
unsigned int coreError = 1;
unsigned int entityError = 1;
bool enableTest = true;
TEST ( TEST_CORE, game )
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    coreMain ( argc, argv );
    EXPECT_EQ ( coreError, GL_NO_ERROR );
}

TEST ( TEST_CORE, page )
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    coreMain ( argc, argv );
    EXPECT_EQ ( pageError, GL_NO_ERROR );
}


TEST ( TEST_CORE, entity )
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    coreMain ( argc, argv );
    EXPECT_EQ ( entityError, GL_NO_ERROR );
}
