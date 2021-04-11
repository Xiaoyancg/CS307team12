#include "Editor.h"

#include "UndoRedo.h"
#include "VMTool.h"

#include <glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "editorWindows.h"



//////////////////////////////////////////
// GUI HANDLER FUNCTIONS

void Editor::initializeGraphics()
{
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
    this->sdlWindow = SDL_CreateWindow(
        "Parchment",                                    // Main Window Title
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
        1280, 720,                                      // CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
        window_flags                                    // flags of sdl windows
    );
    // create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
    this->gl_context = SDL_GL_CreateContext(sdlWindow);
    // set up the opengl context for rendering in to an opengl window
    SDL_GL_MakeCurrent(sdlWindow, gl_context);

#ifdef __TEST_EDITOR
    SDLInitError = std::string(SDL_GetError());
#endif // __TEST_EDITOR

    // init opengl loader
    gladLoadGL();

#ifdef __TEST_EDITOR
    OpenGLInitError = glGetError();
#endif // __TEST_EDITOR

    // Setup Dear Imgui context
    IMGUI_CHECKVERSION();
    // can't find document of ImGui::CreateContext. Just use it anyway
    ImGui::CreateContext();
    // ImGuiIO: Communicate most settings and inputs/outputs to Dear ImGui using this structure.
    io = &ImGui::GetIO();
    io->WantCaptureMouse = true;
    io->WantCaptureKeyboard = true;
    // resize from window edge flag
    io->ConfigWindowsResizeFromEdges = true;
    // multiple viewports flag
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // docking flag
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // keep docking bound to shift

    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // alternative: Classic

    // Setup Platform/Renderer backends
    // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, gl_context);
    ImGui_ImplOpenGL3_Init();
} // initializeGraphics()

void Editor::initializeFramebuffer() {
    glGenTextures(1, &mTexCBO);
    glGenFramebuffers(1, &mFBO);
    glBindTexture(GL_TEXTURE_2D, mTexCBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexCBO, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::cleanupGraphics()
{
    // IMGUI Clean Up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // SDL Clean Up
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
} // cleanupGraphics()

void Editor::createWindows() {
    mainMenuBar = new MainMenuBar(this);

    windowList.resize(SELECT_COUNT);
    windowList[GAMEVIEW] = new GameWindow(this, default_size, mTexCBO, mFBO);
    windowList[MAPVIEW] = new MapWindow(this, default_size, mTexCBO, mFBO);

    windowList[ENTITYEDITOR] = new EntityEditor(this, default_size);
    windowList[PAGEEDITOR] = new PageEditor(this, default_size);
    windowList[SCRIPTEDITOR] = new ScriptEditor(this, default_size);
    windowList[SPRITEEDITOR] = new SpriteEditor(this, default_size);
    windowList[MAPEDITOR] = new MapEditor(this, default_size);
    windowList[LOGICEDITOR] = new LogicEditor(this, default_size);

    windowList[OBJECTTREE] = new ObjectTree(this, default_size);
    windowList[SPLASHSCREEN] = new SplashWindow(this, default_size);
} // createWindows()

void Editor::processInput()
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        ImGui_ImplSDL2_ProcessEvent(&evt);
        if (evt.type == SDL_QUIT)
        {
            running = false;
        }
        if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE && evt.window.windowID == SDL_GetWindowID(sdlWindow))
        {
            running = false;
        }
        
        if (evt.type == SDL_KEYDOWN)
        {
            if (evt.key.keysym.sym == SDLK_LCTRL)
            {
                ctrl_pressed = true;
            }
            if (ctrl_pressed)
            {
                if (evt.key.keysym.sym == SDLK_z)
                {
                    undo();
                }
                if (evt.key.keysym.sym == SDLK_y)
                {
                    redo();
                }
            }
            if (game != nullptr && windowList[GAMEVIEW]->isFocused() && gameRunning)
            {
                game->handleInput(evt);
            }
        }
        if (evt.type == SDL_KEYUP)
        {
            if (evt.key.keysym.sym == SDLK_LCTRL)
            {
                ctrl_pressed = false;
            }
        }
        // Handle mouse clicks
        /*if (evt.type == SDL_MOUSEBUTTONDOWN)
        {
            // Left mouse click
            int x, y;
            if (evt.button.button == SDL_BUTTON_LEFT)
            {
                // TEMP
                
                //    SDL_GetMouseState(&x, &y);
                //    printf("Click at (%d, %d)\n", x, y);
                    
            }
        }*/
    }
} // processInput()


// ===============================
// Main function
void Editor::run()
{
#ifdef __TEST_EDITOR
    game = Core::s1Game();
#endif
    initializeGraphics();
    initializeFramebuffer();
    createWindows();
    currentComponent.resize(COMP_COUNT);

    // clear color, opengl use clear color to clear the context for the next drawing
    // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
    // Also, for the convenience, we use the vector class from ImGui. ImVec4.
    // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
    // Every color in opengl stored as vector. can be vec3 or vec4.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //set the default splash screen state to open
    windowList[SPLASHSCREEN]->setVisible(true);

    running = true;
    while (running)
    {
        processInput();

        // Draw ImGui windows
        // Start the dear Imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(this->sdlWindow);
        ImGui::NewFrame();

        // turn the main viewport into a docking one to allow for docking
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        mainMenuBar->draw();
        for (auto window : windowList) {
            if (window != nullptr) {
                window->draw();
            }
        }
        drawPopups();

#ifdef __TEST_EDITOR
        if (testbool)
        {
            return;
        }
#endif

        // this command does not render that imgui window, we need to use opengl to render imgui
        ImGui::Render();
        // render the ImGui windows
        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        // use the clear color we passed to opengl before to clear the context
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        // use opengl to render the imgui window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
        // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
        SDL_GL_SwapWindow(sdlWindow);

#ifdef __TEST_EDITOR
        if (dobreak)
            running = false;
#endif // __TEST_EDITOR
    }

    cleanupGraphics();
} // run()

void Editor::drawPopups() {
    // Successful save popup
    if (saveSuccessPopup) {
        ImGui::OpenPopup("save_success_popup");
        saveSuccessPopup = false;
    }
    if (ImGui::BeginPopup("save_success_popup"))
    {
        ImGui::Text("Project saved successfully!");
        ImGui::EndPopup();
    }

    // Successful deletion popup
    if (deleteSuccessPopup) {
        ImGui::OpenPopup("delete_success_popup");
        deleteSuccessPopup = false;
    }
    if (ImGui::BeginPopup("delete_success_popup"))
    {
        ImGui::Text("Deletion successful!");
        ImGui::EndPopup();
    }
} // drawPopups()



//////////////////////////////////////////
// GAME HANDLER FUNCTIONS

void Editor::createGame() {
	//createTexCBO();
	//createMapTexCBO();
	game = new Core::Game();
	windowList[GAMEVIEW]->setVisible(true);
	for (int i = 0; i < COMP_COUNT; i++)
    {
        currentComponent[i] = "No Component Selected";
    }

	// When user new project, it won't save
	// User should call save manually
}

void Editor::loadGame(const std::string filePath) {
    if (game != nullptr) {
        freeGame();
    }
    gameFilePath = filePath;
    nlohmann::json *j = readGameDataFile(gameFilePath);
    game = new Core::Game(*j);
    // pointer deletion
    delete j;
    windowList[GAMEVIEW]->setVisible(true);
}

void Editor::saveGame() {
    nlohmann::json *content = game->serialize();
    WriteFile(gameFilePath, (content->dump(2)));
	// pointer deletion
	delete content;
}

void Editor::saveGameAs(const std::string filePath) {
    gameFilePath = filePath;
    saveGame();
}

void Editor::freeGame() {
    currentComponent.clear();
    mTexCBO = 0;//TODO: FREE

    delete game;
    game = nullptr;

    gameFilePath.clear();
}

void Editor::setGameRunning(bool value) {
    gameRunning = value;
}
