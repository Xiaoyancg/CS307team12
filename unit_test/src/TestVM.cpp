#include <TestVM.h>

#include "gtest/gtest.h"

TEST ( TEST_VM, ReadFile )
{
    std::string s ( ReadFile ( std::string ( "D:\\2021\\307\\307project\\CS307team12\\core\\CoreMain.cpp" ) ) );
    EXPECT_TRUE ( s.size () > 0 );
}



TEST ( TEST_VM, WriteFile )
{
    EXPECT_TRUE ( !WriteFile ( "testFile.txt", "content\ncontent2" ) );
}

TEST ( TEST_VM, WriteReadFile )
{
    WriteFile ( "TestFile.txt", "content1\ncontent2" );
    std::string s ( ReadFile ( std::string ( "TestFile.txt" ) ) );
    printf ( "%s\n", s.c_str () );
    // write file will write a new line at the begining
    EXPECT_TRUE ( s.compare ( "\ncontent1\ncontent2" ) );
}

TEST ( TEST_VM, ProduceDataFile )
{
    Game g = Game();
    g.SetName ( "name" );
    EXPECT_TRUE ( !ProduceDataFile ( &g ) );
}

TEST(TEST_VM, ConstructGame) {
    Game g = ConstructGame ( "name.gdata" );
    EXPECT_TRUE ( !std::string ( g.GetName () ).compare ( "name" ) );
}

TEST(TEST_VM, ProduceDataFileConstructGame) {
    Game g = Game ();
    g.SetName ( "huh" );
    ProduceDataFile ( &g );
    Game r = ConstructGame ( std::string ( "huh.gdata" ) );
    EXPECT_TRUE ( !r.GetName ().compare ( "huh" ) );
}

/*

TEST(TEST_VM, ) {

}

*/
