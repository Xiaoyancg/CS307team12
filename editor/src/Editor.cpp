#include <SDL.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imfilebrowser.h>
#include <stdio.h>
#include <iostream>

#ifdef __TEST_EDITOR
#include <TestEditor.h>
#endif // __TEST_EDITOR

static void ShowExampleAppMainMenuBar();
bool running = true;
ImGui::FileBrowser saveDialog = ImGui::FileBrowser(
    ImGuiFileBrowserFlags_NoTitleBar |
    ImGuiFileBrowserFlags_SelectDirectory |
    ImGuiFileBrowserFlags_CreateNewDir
);
ImGui::FileBrowser openDialog = ImGui::FileBrowser(
    ImGuiFileBrowserFlags_NoTitleBar
);


int EditorMain(int argc, char* argv[]) {
    // Set Up SDL2
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // opengl buffer details
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window with graphics context
    // sdl windows flags (attributes)
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI);
    // create a window for opengl. opengl can't create a window. we use sdl to create window
SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x and y pos. here we put it centered - CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
    1280, 720,
    window_flags // flags of sdl windows
);
// create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
SDL_GLContext gl_context = SDL_GL_CreateContext(window);
// set up the opengl context for rendering in to an opengl window
SDL_GL_MakeCurrent(window, gl_context);

#ifdef __TEST_EDITOR
SDLInitError = std::string ( SDL_GetError () );
#endif // __TEST_EDITOR


// init opengl loader
gladLoadGL();

#ifdef __TEST_EDITOR
OpenGLInitError = glGetError ();
#endif // __TEST_EDITOR


// Setup Dear Imgui context
IMGUI_CHECKVERSION();
// can't find document of ImGui::CreateContext. Just use it anyway
ImGui::CreateContext();
// ImGuiIO: Communicate most settings and inputs/outputs to Dear ImGui using this structure.
ImGuiIO& io = ImGui::GetIO();

// Setup Dear ImGui style
ImGui::StyleColorsDark(); // alternative: Classic

// Setup Platform/Renderer backends
// these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
ImGui_ImplOpenGL3_Init();

bool showDemoWindow = true;
// clear color, opengl use clear color to clear the context for the next drawing
// if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
// Also, for the convenience, we use the vector class from ImGui. ImVec4.
// clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
// Every color in opengl stored as vector. can be vec3 or vec4.
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

openDialog.SetTypeFilters({ ".h", ".cpp" });

while (running) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        ImGui_ImplSDL2_ProcessEvent(&evt);
        if (evt.type == SDL_QUIT) {
            running = false;
        }
        if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE && evt.window.windowID == SDL_GetWindowID(window)) {
            running = false;
        }
    }

    // Draw ImGui windows
    // Start the dear Imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    if (!showDemoWindow) {
        // test main menu bar
        ShowExampleAppMainMenuBar();
    }

    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // this command does not render that imgui window, we need to use opengl to render imgui
    ImGui::Render();
    // render the ImGui windows
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    // use the clear color we passed to opengl before to clear the context
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    // use opengl to render the imgui window
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
    SDL_GL_SwapWindow(window);
}

// IMGUI Clean Up
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplSDL2_Shutdown();
ImGui::DestroyContext();

// SDL Clean Up
SDL_GL_DeleteContext(gl_context);
SDL_DestroyWindow(window);
SDL_Quit();

return 0;
}

static void ShowExampleAppMainMenuBar()
{
    //bool array to track the selections made on main menu bar
    static bool selection[3];

    //open new project/save as popup
    if (selection[0])
    {
        ImGui::OpenPopup("Save As");
        selection[0] = false;
    }
    //open delete project popup
    if (selection[1])
    {
        ImGui::OpenPopup("Delete Project");
        selection[1] = false;
    }
    //game view window creation
    if (selection[2])
    {
        // possibly implement a new function here for readability purposes
        ImGui::Begin("Game View Window");
        ImGui::End();
    }

    // main menu bar code
    if (ImGui::BeginMainMenuBar())
    {
        // File menu
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project"))
            {
                selection[0] = true;
            }
            if (ImGui::MenuItem("Open Project"))
            {
                openDialog.Open();
            }
            if (ImGui::MenuItem("Save"))
            {
                //call save function from VM team
            }
            if (ImGui::MenuItem("Save As"))
            {
                selection[0] = true;
            }
            if (ImGui::MenuItem("Delete Project"))
            {
                selection[1] = true;
            }
            ImGui::EndMenu();
        }

        // Add menu
        if (ImGui::BeginMenu("Add"))
        {
            ImGui::MenuItem("Game View Window", "", &selection[2]);

            if (ImGui::MenuItem("Page Editor"))
            {
                
            }
            if (ImGui::MenuItem("Entity Editor"))
            {

            }
            ImGui::EndMenu();
        }
        //ImGui::IsItemVisible ();
        ImGui::EndMainMenuBar();
    }

    // save dialog selection return
    saveDialog.Display();
    if (saveDialog.HasSelected())
    {
        printf("(printf) Selected Directory: %s\n", saveDialog.GetSelected().string().c_str());
        std::cout << "(cout) Selected Directory: " << saveDialog.GetSelected().string() << std::endl;
        saveDialog.ClearSelected();
    }

    // open dialog selection return
    openDialog.Display();
    if (openDialog.HasSelected())
    {
        printf("(printf) Selected File: %s\n", openDialog.GetSelected().string().c_str());
        std::cout << "(cout) Selected File: " << openDialog.GetSelected().string() << std::endl;
        openDialog.ClearSelected();
    }

    // delete project popup
    if (ImGui::BeginPopup("Delete Project"))
    {
        ImGui::Text("Are you sure you want to delete a project? Click outside of this popup to cancel.");
        if (ImGui::Button("Yes"))
        {
            //connect to VM team delete function
        }
        ImGui::EndPopup();
    }

    // save project popup
    static char name[128] = "";
    if (ImGui::BeginPopup("Save As"))
    {
        ImGui::Text("Enter the name of your project."); 
        ImGui::InputText("", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("Save"))
        {
            saveDialog.Open();
            //connect to VM save function utilizing saveDialog selected LOCATION and buffered NAME
        }
#ifdef __TEST_EDITOR
        isSaveAsOpen = ImGui::IsPopupOpen ("Save As");
#endif // __TEST_EDITOR

        ImGui::EndPopup();
    }
}