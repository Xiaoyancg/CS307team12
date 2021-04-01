#include "gtest/gtest.h"
#include <Editor.h>
std::string SDLInitError;
unsigned int OpenGLInitError;
std::string menuError;
bool isSaveAsOpen;
bool dobreak = true;
unsigned int splasherror;
bool splashImageFail = false;

TEST(TEST_EDITOR, U4_0)
{
    EditorMain();
    EXPECT_EQ(splasherror, GL_NO_ERROR);
}

TEST(TEST_EDITOR, U4_1)
{
    EditorMain();
    EXPECT_FALSE(splashImageFail);
}

TEST(TEST_EDITOR, SDLInit)
{
    EditorMain();
    EXPECT_TRUE(SDLInitError.size() == 0);
}

TEST(TEST_EDITOR, OpenGLInit)
{
    EditorMain();
    EXPECT_TRUE(OpenGLInitError == GL_NO_ERROR);
}

TEST(TEST_EDITOR, SAVE_AS)
{

    //dobreak = false;
    EditorMain();
    EXPECT_TRUE(isSaveAsOpen);
}
