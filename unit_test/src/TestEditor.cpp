#include "gtest/gtest.h"
#include <Editor.h>
std::string SDLInitError;
unsigned int OpenGLInitError;
std::string menuError;
bool isSaveAsOpen;
TEST(TEST_EDITOR, SDLInit) {
    EditorMain ( 0,  (char**)NULL );
    EXPECT_TRUE(SDLInitError.size() == 0);
}


