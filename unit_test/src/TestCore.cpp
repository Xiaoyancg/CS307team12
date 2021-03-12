#include <TestCore.h>
#include "VMTool.h"
#include "gtest/gtest.h"

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
    Core::Game g;
    g.coreMain ( argc, argv );
    EXPECT_EQ ( coreError, GL_NO_ERROR );
}

TEST ( TEST_CORE, page )
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    Core::Game g;
    g.coreMain ( argc, argv );
    EXPECT_EQ ( pageError, GL_NO_ERROR );
}


TEST ( TEST_CORE, entity )
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    Core::Game g;
    g.coreMain ( argc, argv );
    EXPECT_EQ ( entityError, GL_NO_ERROR );
}


TEST ( TEST_GAME, Game_SetGetName )
{
    Core::Game g ( "hello" );
    std::string s ( g.GetGameName () );
    EXPECT_TRUE ( !s.compare ( "hello" ) );
}

TEST ( TEST_CORE, Serialize )
{
    Core::Game g ( "hello" );
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    g.coreMain ( argc, argv );
    nlohmann::json a = g.serialize ();
    WriteFile ( "testSinGame.gdata", a.dump () );
}


TEST(TEST_CORE, Parse) {
    nlohmann::json gamefile = R"(
        {
            "GameName": "example name",
            "Author": "example author",
            "Version": "example version",
            "LastModifiedTime": "example Date Time",
            "Note": "example Note",
            "PageList": []
        }
    )"_json;

    Core::Game* game = Core::Game::parse(gamefile);

    EXPECT_EQ(game->GetGameName(), "example name");
    EXPECT_EQ(game->GetAuthor(), "example author");
    EXPECT_EQ(game->GetVersion(), "example version");
    EXPECT_EQ(game->GetLMTime(), "example Date Time");
    EXPECT_EQ(game->GetNote(), "example Note");
}
