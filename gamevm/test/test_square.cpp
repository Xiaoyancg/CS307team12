#include "GameVM.h"
#include "gtest/gtest.h"

TEST(Square, Square2Is4) {
	EXPECT_EQ(square(2), 4);
}