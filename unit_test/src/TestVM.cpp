#include "gtest/gtest.h"
#include <TestVM.h>


TEST ( TEST_VM, ReadFile )
{
    std::string s ( ReadFile ( std::string ( "D:\\2021\\307\\307project\\CS307team12\\core\\CoreMain.cpp" ) ) );
    EXPECT_TRUE ( s.size () > 0 );
}



TEST ( TEST_VM, WriteFile )
{
    EXPECT_TRUE ( !WriteFile ( "testFile.txt", "content\ncontent2" ) );
}

TEST ( TEST_VM, WR )
{
    WriteFile ( "TestFile.txt", "content1\ncontent2" );
    std::string s ( ReadFile ( std::string ( "TestFile.txt" ) ) );
    printf ( "%s\n", s.c_str () );
    // write file will write a new line at the begining
    EXPECT_TRUE ( s.compare ( "\ncontent1\ncontent2" ) );
}



/*

TEST(TEST_VM, ) {

}

*/
