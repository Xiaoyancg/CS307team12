// this is the minimum demo for SDL and opengl and imgui
// By Larry
// Please read README before compile

// include required libraries
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <SDL.h>
#include <glad/glad.h>

// normal libraries
#include <stdio.h>

int main ( int argc, char **argv )
{

// set up SDL2
  if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER ) != 0 )
  {
    printf ( "SDL_Init error: %s\n", SDL_GetError () );
    return -1;
  }


  // Set OPENGL Attributes to SDL.
  // See https://wiki.libsdl.org/SDL_GLattr for enum details
  // use gl 4.6 glsl 460. Although imgui has only functions naming opengl3, but for now it works. as long as it works, i'll keep 4 here.
  const char *glsl_version = "#version 460";
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_FLAGS, 0 );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 6 );

  // opengl buffer details
  SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );
  SDL_GL_SetAttribute ( SDL_GL_STENCIL_SIZE, 8 );

  // Create window with graphics context
  // sdl windows flags (attributes)
  SDL_WindowFlags window_flags =
    ( SDL_WindowFlags ) ( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
  // create a window for opengl. opengl can't create a window. we use sdl to create window
  SDL_Window *window =
    SDL_CreateWindow ( "minimum demo for imgui with sdl2 on opengl",
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x and y pos. here we put it centered
                       800, 600,
                       window_flags // flags of sdl windows
    );
// create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
  SDL_GLContext gl_context = SDL_GL_CreateContext ( window );
  // set up the opengl context for rendering in to an opengl window
  SDL_GL_MakeCurrent ( window, gl_context );
  // Use this function to set the swap interval for the current OpenGL context. 
  // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync
  SDL_GL_SetSwapInterval ( 1 );
  // this opengl context is just the background, we should render game in a ImGui window!
  // ... Or Not. we can fixed the ImGui window, and render context in the middle of the sdl window. This is easier.


  // Initialize Opengl loader
  // we use glad, NOT GLAD2! 
  // I really don't know the difference.  the source code are same
  if ( gladLoadGL () == 0 )
  {
    printf ( "Failed to initialize OpenGL loader!\n" );
    return -1;
  }

  // Setup Dear Imgui context
  IMGUI_CHECKVERSION ();
  // can't find document of ImGui::CreateContext. Just use it anyway
  ImGui::CreateContext ();
  // ImGuiIO: Communicate most settings and inputs/outputs to Dear ImGui using this structure.
  ImGuiIO &io = ImGui::GetIO ();
  // ???? from the provided demo
  ( void ) io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark (); // alternative: Classic

  // Setup Platform/Renderer backends
  // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
  ImGui_ImplSDL2_InitForOpenGL ( window, gl_context );
  ImGui_ImplOpenGL3_Init ( glsl_version );

  // clear color, opengl use clear color to clear the context for the next drawing
  // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
  // Also, for the convenience, we use the vector class from ImGui. ImVec4.
  // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
  // Every color in opengl stored as vector. can be vec3 or vec4.
  ImVec4 clear_color = ImVec4 ( 0.45f, 0.55f, 0.60f, 1.00f );
  // pass the clear color to opengl server. for further usage
  glClearColor ( clear_color.x, clear_color.y, clear_color.z, clear_color.w );


  // Main loop
  // Every opengl program or any program IDK need a main loop to keep it running.
  // In each loop, it redraw the context and re-evaluate the variables as needed.
  // Here we run loop for each time interval. However, for other case, we can run loop when event happened(key stroke)
  // As for the game loop, we call a game conditional function in each loop.
  // the controller function shouldn't be called here as in my experience. When a key is pressed, we update the data.
  bool done = false;
  while ( !done )
  {
// Poll and handle events (inputs, window resize, etc.)
// In the demo, it says we should pass all input to dear imgui based onio.WantCaptureMouse/Keyboard
    SDL_Event event;
    while ( SDL_PollEvent ( &event ) )
    {
      if ( event.type == SDL_QUIT )
      {
        done = true; // exit after the loop
      }
      // if window is closed
      if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID ( window ) )
      {
        done = true;
      }
    }

    // Draw ImGui windows
    // Start the dear Imgui frame
    ImGui_ImplOpenGL3_NewFrame ();
    ImGui_ImplSDL2_NewFrame ( window );
    ImGui::NewFrame ();

    // show a really simple imgui window
    ImGui::Begin ( "minimum demo ImGui window" );
    ImGui::Text ( "Hello world" );
    if ( ImGui::Button ( "Close the program" ) )
    {
      done = true;
    }
    ImGui::End ();


    // Rendering
    // render the ImGui windows
    glViewport ( 0, 0, ( int ) io.DisplaySize.x, ( int ) io.DisplaySize.y );
    // use the clear clor we passed to opengl before to clear the context
    glClear ( GL_COLOR_BUFFER_BIT );

    // this command does not render that imgui window, we need to use opengl to render imgui
    ImGui::Render ();
    // use opengl to render the imgui window
    ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
    // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
    SDL_GL_SwapWindow ( window );

  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown ();
  ImGui_ImplSDL2_Shutdown ();
  ImGui::DestroyContext ();

  SDL_GL_DeleteContext ( gl_context );
  SDL_DestroyWindow ( window );
  SDL_Quit ();

  return 0;
}