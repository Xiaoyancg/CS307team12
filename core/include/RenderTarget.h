
#include <glad/glad.h>
namespace Core
{
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