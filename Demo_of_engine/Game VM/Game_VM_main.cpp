#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>


#include <stdio.h>

int main ( int argc, char **argv )
{
  // init SDL
  if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER ) != 0 )
  {
    printf ( "SDL_init error: %s\n", SDL_GetError () );
    return -1;
  }

  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_FLAGS, 0 );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

  // opengl buffer details
  SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );
  SDL_GL_SetAttribute ( SDL_GL_STENCIL_SIZE, 8 );

  // Create window
  // SDL window flags(attributes that define window)
  SDL_WindowFlags window_flags =
    ( SDL_WindowFlags ) ( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
  SDL_Window *window = SDL_CreateWindow ( "Editor Demo",
                                         // x and y pos, both is center
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          // width, height
                                          800, 600,
                                          window_flags
  );
  // create a gl context for further rendering in the window
  SDL_GLContext gl_context = SDL_GL_CreateContext ( window );
  // set up the opengl context for rendering in to an opengl window
  SDL_GL_MakeCurrent ( window, gl_context );
  // Use this function to set the swap interval for teh current OpenGL context.
  // 0 for immediate updates, 1 for updates synchronized with teh vertical retrace,
  // -1 for adaptive vsync
  SDL_GL_SetSwapInterval ( 1 );
  
  // Initialize Opengl loader, glad
  if ( gladLoadGL () == 0 )
  {
    printf ( "Failed to initialize OpenGL loader!\n" );
    return -1;
  }

  glm::vec4 clear_color = glm::vec4 ( 0.1f, 0.9f, 0.1f, 1.0f );
  glClearColor ( clear_color.r, clear_color.g, clear_color.b, clear_color.a );
  

  // main loop
  bool done = false;
  while ( !done )
  {
    SDL_Event event;
    while ( SDL_PollEvent ( &event ) )
    {
      if ( event.type == SDL_QUIT )
      {
        done = true; // exit after the loop
      }
      // if window is closed
      if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
           event.window.windowID == SDL_GetWindowID (window))
      {
        done = true;
      }
    }
    // use the clear color to clear
    glClear ( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow ( window );
  }
  // clean up
  SDL_GL_DeleteContext ( gl_context );
  SDL_DestroyWindow ( window );
  SDL_Quit ();
  return 0;
}
