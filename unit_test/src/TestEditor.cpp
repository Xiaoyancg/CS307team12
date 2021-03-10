#include "gtest/gtest.h"
#include <Editor.h>
std::string SDLInitError;
unsigned int OpenGLInitError;
std::string menuError;
bool isSaveAsOpen;
bool dobreak = true;
TEST(TEST_EDITOR, SDLInit) {
    EditorMain ( 0,  (char**)NULL );
    EXPECT_TRUE(SDLInitError.size() == 0);
}

TEST ( TEST_EDITOR, OpenGLInit )
{
    EditorMain ( 0, ( char ** ) NULL );
    EXPECT_TRUE ( OpenGLInitError == GL_NO_ERROR );
}

TEST ( TEST_EDITOR, SAVE_AS )
{

    //dobreak = false;
    EditorMain ( 0, ( char ** ) NULL );
    EXPECT_TRUE ( isSaveAsOpen );
}

