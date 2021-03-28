
#include <glad/glad.h>
namespace Core
{
    // The class to store buffer objects from editor
    // if the game class constructed in editor mode, this class will be used
    class RenderTarget
    {
    private:
        GLuint *mmaincbo;
        GLuint *mpagecbo;
        GLuint *mentitycbo;

    public:
        void setMain(GLuint *);
        void setPage(GLuint *);
        void setEntity(GLuint *);
        GLuint getMain();
        GLuint getPage();
        GLuint getEntity();
        RenderTarget(GLuint *, GLuint *, GLuint *);
    };

}