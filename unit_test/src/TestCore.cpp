#include <TestCore.h>

#include <filesystem>

#include "VMTool.h"
#include "Game.h"
#include "Sprint1.h"
#include "Sprite.h"
#include "gtest/gtest.h"
unsigned int pageError = 1;
unsigned int coreError = 1;
unsigned int entityError = 1;
bool enableTest = true;

TEST(PRODUCE_GDATA, s1)
{
    Core::Game *game = Core::s1Game();
    WriteFile("s1game.gdata", game->serialize()->dump(2));
}
TEST(TEST_CORE, game)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    Core::Game g("test");
    //g.coreMain ( argc, argv );
    EXPECT_EQ(coreError, GL_NO_ERROR);
}

TEST(TEST_CORE, page)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    Core::Game g("test");
    //g.coreMain ( argc, argv );
    EXPECT_EQ(pageError, GL_NO_ERROR);
}

TEST(TEST_CORE, entity)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "core";
    argv[1] = "test";
    Core::Game g("test");
    //g.coreMain ( argc, argv );
    EXPECT_EQ(entityError, GL_NO_ERROR);
}

TEST(TEST_GAME, Game_SetgetName)
{
    Core::Game g("hello");
    std::string s(g.getGameName());
    EXPECT_TRUE(!s.compare("hello"));
}
TEST(TEMPTEST, supertemp)
{
    Core::Game g("wulala");
    g.init();
    g.initShader();
    //g.s1test ();
    json *j = g.serialize();

    WriteFile("wulala.gdata", j->dump(2));
}

//TEST ( TEST_CORE, Serialize )
//{
//    Core::Game g ( "test_gameName" );
//    g.SetAuthor("test_author");
//    g.SetLMTime("test_lmtime");
//    g.SetNote("test_note");
//    g.SetVersion("test_version");
//
//    Core::Page* page = new Core::Page;
//    page->setName("test_pageName");
//    page->SetBackgroundColor(0.1, 0.2, 0.3, 0.4);
//
//    Core::Entity* entity = new Core::Entity("test_entityName");
//    entity->setLocation(glm::vec2(0.1, 0.2));
//    entity->setScale(glm::vec2(0.3, 0.4));
//    entity->setRotation(360);
//    entity->setSpriteID(999);
//
//    page->addEntity(entity);
//
//    g.getPageList()->push_back(page);
//
//    nlohmann::json* actual = g.serialize ();
//    nlohmann::json expected = R"(
//        {
//            "FileType": "Parchment Game Data",
//            "GameName": "test_gameName",
//            "Author": "test_author",
//            "Version": "test_version",
//            "LastModifiedTime": "test_lmtime",
//            "Note": "test_note",
//            "PageList": [
//                {
//                    "Name": "test_pageName",
//                    "EntityList": [
//                        {
//                            "Name": "test_entityName",
//                            "Entity ID": 999,
//                            "location": [ 0.1, 0.2 ],
//                            "scale": [ 0.3, 0.4 ],
//                            "rotation": 360.0
//                        }
//                    ]
//                }
//            ]
//        }
//    )"_json;
//
//    std::cout << *actual << std::endl;
//    std::cout << expected << std::endl;
//
//    EXPECT_EQ(*actual, expected);
//}
//
//
//TEST(TEST_CORE, Parse) {
//    nlohmann::json gamefile = R"(
//        {
//            "GameName": "example name",
//            "Author": "example author",
//            "Version": "example version",
//            "LastModifiedTime": "example Date Time",
//            "Note": "example Note",
//            "PageList": [
//                {
//                    "Name": "example Page",
//                    "EntityList": [
//                        {
//                            "Name": "example Entity Name",
//                            "Entity ID": 1,
//                            "location": [ 0.5, 0.5 ],
//                            "scale": [ 1.0, 2.0 ],
//                            "rotation": 1.0,
//                        }
//                    ]
//                }
//            ]
//        }
//    )"_json;
//
//    Core::Game* game = Core::Game::parse(gamefile);
//
//    EXPECT_EQ(game->getGameName(), "example name");
//    EXPECT_EQ(game->getAuthor(), "example author");
//    EXPECT_EQ(game->getVersion(), "example version");
//    EXPECT_EQ(game->getLMTime(), "example Date Time");
//    EXPECT_EQ(game->getNote(), "example Note");
//
//    auto pages = game->getPageList();
//    EXPECT_EQ(pages->size(), 1);
//    EXPECT_EQ((*pages)[0]->getName(), "example Page");
//
//    auto entities = (*pages)[0]->getEntityList();
//    EXPECT_EQ(entities.size(), 1);
//    EXPECT_EQ(entities[0]->getName(), "example Entity Name");
//    EXPECT_EQ(entities[0]->getSpriteID(), 1);
//    EXPECT_EQ(entities[0]->getLocation(), glm::vec2(0.5, 0.5));
//    EXPECT_EQ(entities[0]->getScale(), glm::vec2(1.0, 2.0));
//    EXPECT_EQ(entities[0]->getRotation(), 1.0);
//}

//TEST(TEST_CORE, LoadNonexistantImage)
//{
//Core::Sprite nonexistantSprite("name", "this_file_does_not_exist.png");
//EXPECT_EQ(nonexistantSprite.getOpenGLTextureID(), -1);
//}

//TEST(TEST_CORE, ImplTestImageExists)
//{
//EXPECT_TRUE(std::filesystem::exists("test_image.png"));
//}

//TEST(TEST_CORE, LoadImage)
//{
//Core::Sprite sprite("name", "test_image.png");
//EXPECT_EQ(sprite.getDimensions().x, 256);
//EXPECT_EQ(sprite.getDimensions().y, 256);
//EXPECT_NE(sprite.getOpenGLTextureID(), -1);
//}
