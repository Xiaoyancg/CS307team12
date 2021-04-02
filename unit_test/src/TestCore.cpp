#include <TestCore.h>

#include <filesystem>

#include "VMTool.h"
#include "Game.h"
#include "Sprint2.h"
#include "Sprite.h"
#include "GameVM.h"
#include "gtest/gtest.h"
unsigned int pageError = 1;
unsigned int coreError = 1;
unsigned int entityError = 1;
bool enableTest = true;

TEST(TEST_CORE, U2_0)
{
    Core::s2Game();
    EXPECT_EQ(coreError, GL_NO_ERROR);
}

TEST(TEST_CORE, U2_1)
{
    Core::Game *g = Core::s1Game();
    g->run();
    EXPECT_EQ(pageError, GL_NO_ERROR);
}

TEST(TEST_CORE, U2_2)
{
    Core::s2Game();
    EXPECT_EQ(entityError, GL_NO_ERROR);
}

TEST(TEST_CORE, U3_0)
{
    GameVMMain();
    EXPECT_EQ(coreError, GL_NO_ERROR);
}

TEST(TEST_CORE, U3_1)
{
    GameVMMain();
    EXPECT_EQ(pageError, GL_NO_ERROR);
}

TEST(TEST_CORE, U3_2)
{
    GameVMMain();
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
    g.initContext();
    g.initShader();
    //g.s1test ();
    json *j = g.serialize();

    WriteFile("wulala.gdata", j->dump(2));
}

// user story 1
TEST(TEST_CORE, Serialize)
{
    Core::Game g("test_gameName");
    g.setAuthor("test_author");
    g.setLMTime("test_lmtime");
    g.setNote("test_note");
    g.setVersion("test_version");

    Core::Page *page = new Core::Page;
    page->setName("test_pageName");
    page->SetBackgroundColor(0.1, 0.2, 0.3, 0.4);

    Core::Entity *entity = new Core::Entity("test_entityName");
    entity->setLocation(glm::vec2(1.0, 2.0));
    entity->setScale(glm::vec2(1.0, 1.0));
    entity->setRotation(360);
    entity->setSpriteID(999);

    page->addEntity(entity);

    g.getPageList()->push_back(page);

    nlohmann::json *actual = g.serialize();
    nlohmann::json expected = R"(
        {
            "FileType": "Parchment Game Data",
            "GameName": "test_gameName",
            "Author": "test_author",
            "Version": "test_version",
            "LastModifiedTime": "test_lmtime",
            "Note": "test_note",
            "pageList": [
                {
                    "PageName": "test_pageName",
                    "entityList": [
                        {
                            "EntityName": "test_entityName",
                            "spriteID": 999,
                            "location": [ 1.0, 2.0 ],
                            "scale": [ 1.0, 1.0 ],
                            "rotation": 360.0
                        }
                    ]
                }
            ]
        }
    )"_json;

    std::cout << *actual << std::endl;
    std::cout << expected << std::endl;

    EXPECT_EQ(*actual, expected);
}

// user story 1
TEST(TEST_CORE, Parse)
{
    nlohmann::json expected = R"(
        {
            "FileType": "Parchment Game Data",
            "GameName": "test_gameName",
            "Author": "test_author",
            "Version": "test_version",
            "LastModifiedTime": "test_lmttime",
            "Note": "test_note",
            "pageList": [
                {
                    "PageName": "test_pageName",
                    "entityList": [
                        {
                            "EntityName": "test_entityName",
                            "spriteID": 999,
                            "location": [ 0.5, 0.5 ],
                            "scale": [ 1.0, 2.0 ],
                            "rotation": 1.0
                        }
                    ]
                }
            ]
        }
    )"_json;

    Core::Game *game = new Core::Game(expected);

    EXPECT_EQ(game->getGameName(), "test_gameName");
    EXPECT_EQ(game->getAuthor(), "test_author");
    EXPECT_EQ(game->getVersion(), "test_version");
    EXPECT_EQ(game->getLMTime(), "test_lmttime");
    EXPECT_EQ(game->getNote(), "test_note");

    auto pages = game->getPageList();
    EXPECT_EQ(pages->size(), 1);
    EXPECT_EQ((*pages)[0]->getName(), "test_pageName");

    auto entities = (*pages)[0]->getEntityList();
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0]->getName(), "test_entityName");
    EXPECT_EQ(entities[0]->getSpriteID(), 999);
    EXPECT_EQ(entities[0]->getLocation(), glm::vec2(0.5, 0.5));
    EXPECT_EQ(entities[0]->getScale(), glm::vec2(1.0, 2.0));
    EXPECT_EQ(entities[0]->getRotation(), 1.0);
}

TEST(TEST_CORE, LoadNonexistantImage)
{
    Core::Sprite nonexistantSprite("name", "this_file_does_not_exist.png");
    EXPECT_EQ(nonexistantSprite.getOpenGLTextureID(), -1);
}

//TEST(TEST_CORE, ImplTestImageExists)
//{
//    EXPECT_TRUE(std::filesystem::exists("test_image.png"));
//}
//
//TEST(TEST_CORE, LoadImage)
//{
//    Core::Sprite sprite("name", "test_image.png");
//    EXPECT_EQ(sprite.getDimensions().x, 256);
//    EXPECT_EQ(sprite.getDimensions().y, 256);
//    EXPECT_NE(sprite.getOpenGLTextureID(), -1);
//}
