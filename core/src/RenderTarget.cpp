#include "RenderTarget.h"

namespace Core
{
    RenderTarget::RenderTarget(GLuint *main, GLuint *page, GLuint *entity)
    {
        mmaincbo = main;
        mpagecbo = page;
        mentitycbo = entity;
    }
    void RenderTarget::setEntity(GLuint *entity)
    {
        mentitycbo = entity;
    }
    void RenderTarget::setPage(GLuint *page)
    {
        mpagecbo = page;
    }
    void RenderTarget::setMain(GLuint *main)
    {
        mmaincbo = main;
    }
    GLuint RenderTarget::getMain()
    {
        return *mmaincbo;
    }
    GLuint RenderTarget::getEntity()
    {
        return *mentitycbo;
    }
    GLuint RenderTarget::getPage()
    {
        return *mpagecbo;
    }
}