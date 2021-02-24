// this is the mimimum demo for game vm
// by Larry

#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>


#include <stdio.h>
#include <string> 
#include <streambuf>  
#include <fstream>
#include <iostream>


float vertices[] = { -0.5f, -0.5f, 0.0f,
                      0.5f, -0.5f, 0.0f, 
                      0.0f, 0.5f, 0.0f };



int init ()
{
  printf ( "hello\n" );
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


  std::cerr << "OK: Init sdl\n" << std::endl;
  return 0;
}

void mainLoop ( SDL_Window *window )
{
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
           event.window.windowID == SDL_GetWindowID ( window ) )
      {
        done = true;
      }
    }

    // use the clear color to clear
    glClear ( GL_COLOR_BUFFER_BIT );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
    SDL_GL_SwapWindow ( window );
  }

}

int main ( int argc, char **argv )
{
  if ( init () == -1 )
  {
    return -1;
  }

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

  unsigned int vbo, vao;
  glGenVertexArrays ( 1, &vao );
  glGenBuffers ( 1, &vbo );
  glBindVertexArray ( vao );
  glBindBuffer ( GL_ARRAY_BUFFER, vbo );
  glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertices ), vertices, GL_STATIC_DRAW );
  glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( float ), ( void * ) 0 );
  glEnableVertexAttribArray ( 0 );
  glBindVertexArray ( vao );

  std::cerr << "OK: init vertexarray and buffer array\n" << std::endl;

  const char *vscs, *fscs; // vertex/fragment shader const string
  std::string vss;
  std::string fss;
  try
  {
    char vsn[] = "vertex.vert"; // vertex shader name
    char fsn[] = "fragment.frag";// fragment shader name
    std::ifstream vsfs ( vsn ); // vertex shader file stream
    if ( vsfs.is_open() )
    {
      printf ( "vsfsopen\n" );
    }
    std::ifstream fsfs ( fsn ); // fragment shader file stream
    vss = std::string ( std::istreambuf_iterator<char> ( vsfs ), std::istreambuf_iterator<char> () ); // fragment shader string
    fss = std::string ( std::istreambuf_iterator<char> ( fsfs ), std::istreambuf_iterator<char> () ); // fragment shader string
    vsfs.close ();
    fsfs.close ();
    vscs = vss.c_str ();
    fscs = fss.c_str ();
  }
  catch ( const std::exception & )
  {
    std::cerr << "can't load shader file" << std::endl;
  }

  
  std::cout << "vss is :\n" << vss << std::endl;
  std::cout << "fss is :\n" << fss << std::endl;

  unsigned int vs, fs;
  vs = glCreateShader ( GL_VERTEX_SHADER );
  fs = glCreateShader ( GL_FRAGMENT_SHADER );
  glShaderSource ( vs, 1, &vscs, NULL );
  glShaderSource ( fs, 1, &fscs, NULL );
  glCompileShader ( vs );
  int success;
  char infoLog[512];

  glGetShaderiv ( vs, GL_COMPILE_STATUS, &success );
  if ( !success )
  {
    glGetShaderInfoLog ( vs, 512, NULL, infoLog );

    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  };
  glCompileShader ( fs );

  glGetShaderiv ( fs, GL_COMPILE_STATUS, &success );
  if ( !success )
  {
    glGetShaderInfoLog ( fs, 512, NULL, infoLog );

    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  };


  unsigned int program = glCreateProgram ();
  glAttachShader ( program, vs );
  glAttachShader ( program, fs );
  glLinkProgram ( program );
  glDeleteShader ( vs );
  glDeleteShader ( fs );
  glUseProgram ( program );

  std::cerr << "OK: load, compile, use program\n" << std::endl;

  glm::vec4 clear_color = glm::vec4 ( 0.1f, 0.9f, 0.1f, 1.0f );
  glClearColor ( clear_color.r, clear_color.g, clear_color.b, clear_color.a );

  mainLoop ( window );

  // clean up
  SDL_GL_DeleteContext ( gl_context );
  SDL_DestroyWindow ( window );
  SDL_Quit ();
  return 0;
}
