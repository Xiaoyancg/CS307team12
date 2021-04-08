#include "Editor.h"
#include "editorWindows.h"


Editor::Editor() : mainMenuBar(this) {
    memset(selection, 0, sizeof(selection));
}

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
    //io->ConfigDockingWithShift = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // alternative: Classic

    // Setup Platform/Renderer backends
    // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, gl_context);
    ImGui_ImplOpenGL3_Init();
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
}

void Editor::createWindows() {
    windowList.resize(SELECT_COUNT);
    windowList[GAMEVIEW] = new GameWindow(this, default_size);
    windowList[MAPVIEW] = new MapWindow(this, default_size);

    windowList[ENTITYEDITOR] = new EntityEditor(this, default_size);
    windowList[PAGEEDITOR] = new PageEditor(this, default_size);
    windowList[SCRIPTEDITOR] = new ScriptEditor(this, default_size);
    windowList[SPRITEEDITOR] = new SpriteEditor(this, default_size);
    windowList[MAPEDITOR] = new MapEditor(this, default_size);

    windowList[OBJECTTREE] = new ObjectTree(this, default_size);
    windowList[SPLASHSCREEN] = new SplashWindow(this, default_size);
}

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
            if (game != nullptr)
            {
                if (windowList[GAMEVIEW]->isFocused())
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
        if (evt.type == SDL_MOUSEBUTTONDOWN)
        {
            // Left mouse click
            int x, y;
            if (evt.button.button == SDL_BUTTON_LEFT)
            {
                // TEMP
                /*
                    SDL_GetMouseState(&x, &y);
                    printf("Click at (%d, %d)\n", x, y);
                    */
            }
        }
    }
}

// ===============================
// Main function

void Editor::run()
{
#ifdef __TEST_EDITOR
    game = Core::s1Game();
    currPage = game->getCurrPage();
#endif
    initializeGraphics();
    createWindows();

    // clear color, opengl use clear color to clear the context for the next drawing
    // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
    // Also, for the convenience, we use the vector class from ImGui. ImVec4.
    // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
    // Every color in opengl stored as vector. can be vec3 or vec4.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Initialize current components to none selected
    for (int i = 0; i < COMP_COUNT; i++)
    {
        currentComponent.push_back("No Component Selected");
    }

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
        mainMenuBar.draw();
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
}

void Editor::drawPopups() {
    // Delete project popup
    if (ImGui::BeginPopup("Delete Project"))
    {
        ImGui::Text("Are you sure you want to delete a project? Click outside of this popup to cancel.");
        if (ImGui::Button("Yes"))
        {
            delDialog = ImGui::FileBrowser(
                ImGuiFileBrowserFlags_NoTitleBar);
            delDialog.Open();
        }
        ImGui::EndPopup();
    }

    // Save as project popup
    static char name[128] = "";
    if (ImGui::BeginPopup("Save As"))
    {
        ImGui::Text("Enter the name of your project.");
        ImGui::InputText("", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("Save"))
        {
            // init file browser ( update info every time it was opened)
            saveDialog = ImGui::FileBrowser(
                ImGuiFileBrowserFlags_NoTitleBar |
                ImGuiFileBrowserFlags_SelectDirectory |
                ImGuiFileBrowserFlags_CreateNewDir);
            saveDialog.Open();

            if (!saveDialog.IsOpened())
            {
                gameName = std::string(name);
                // memset to clear the buffer after use
                memset(name, 0, 128);
            }
        }
        ImGui::EndPopup();
    }

    // Successful save popup
    if (ImGui::BeginPopup("Saved Successfully"))
    {
        ImGui::Text("Project saved successfully!");
        ImGui::EndPopup();
    }

    // Successful Project deletion popup
    if (ImGui::BeginPopup("Deleted Successfully"))
    {
        ImGui::Text("Project deleted successfully!");
        ImGui::EndPopup();
    }

    // Successful deletion popup
    if (ImGui::BeginPopup("Delete Successful"))
    {
        ImGui::Text("Deletion successful!");
        ImGui::EndPopup();
    }
}

void Editor::ShowMainMenuBar()
{
#ifdef __TEST_EDITOR
    selection[SAVEAS] = testbool;
#endif // __TEST_EDITOR


    /*
     *  ========================
     *  SELECTION OPTIONS
     *  ========================
     */

    // open save as popup
    if (selection[SAVEAS])
    {
        ImGui::OpenPopup("Save As");
#ifdef __TEST_EDITOR
        isSaveAsOpen = ImGui::IsPopupOpen("Save As");
#endif // __TEST_EDITOR
        selection[SAVEAS] = false;
    }

#ifdef __TEST_EDITOR
    windowList[OBJECTTREE]->setVisible(true);
#endif

    
    

    // Calls saved successfully popup on project save
    if (selection[SAVEPOPUP])
    {
        ImGui::OpenPopup("Saved Successfully");
        selection[SAVEPOPUP] = false;
    }

    // Calls delete successfully popup on successful project deletion
    if (selection[DELETEPOPUP])
    {
        ImGui::OpenPopup("Deleted Successfully");
        selection[DELETEPOPUP] = false;
    }

    /*
     *  ========================
     *  CONDITIONAL POPUP TRIGGERS
     *  ========================
     */

    // Open delete project popup
    if (selection[DELETEPROJECT])
    {
        ImGui::OpenPopup("Delete Successful");
        ImGui::OpenPopup("Delete Project");
        selection[DELETEPROJECT] = false;
    }

    // Open delete element popup
    if (delete_success)
    {
        ImGui::OpenPopup("Delete Successful");
        delete_success = false;
    }

    /*
     *  ========================
     *  MAIN MENU BAR
     *  ========================
     */

    // main menu bar code
    

    /*
     *  ========================
     *  FILE DIALOGS
     *  ========================
     */

    // Save dialog selection return
    saveDialog.Display();
    if (saveDialog.HasSelected())
    {
        //printf ( "(printf) Selected Directory: %s\n", saveDialog.GetSelected ().string ().c_str () );
        //std::cout << "(cout) Selected Directory: " << saveDialog.GetSelected ().string () << std::endl;
        dir = std::string(saveDialog.GetSelected().string()).append("//").append(gameName).append(".gdata");
        nlohmann::json *content = game->serialize();
        WriteFile(dir, (content->dump(2)));
        // pointer deletion
        delete (content);
        isSaved = true;
        saveDialog.ClearSelected();
        selection[SAVEPOPUP] = true;
    }

    // Open dialog selection return
    openDialog.Display();
    if (openDialog.HasSelected())
    {
        //printf ( "(printf) Selected File: %s\n", openDialog.GetSelected ().string ().c_str () );
        //std::cout << "(cout) Selected File: " << openDialog.GetSelected ().string () << std::endl;
        nlohmann::json *j = readGameDataFile(openDialog.GetSelected().string());
        dir = openDialog.GetSelected().string();
        texcbo = new GLuint();
        glGenTextures(1, texcbo);
        maptexcbo = new GLuint();
        glGenTextures(1, maptexcbo);
        game = new Core::Game(*j, texcbo, maptexcbo);
        // pointer deletion
        delete (j);
        gameName = game->getGameName();
        currPage = game->getCurrPage();
        game->initShader();
        windowList[GAMEVIEW]->setVisible(true);
        isSaved = true;

        openDialog.ClearSelected();
    }

    // Delete dialog selection return
    delDialog.Display();
    if (delDialog.HasSelected())
    {
        DeleteFile(delDialog.GetSelected().string().c_str());
        delDialog.ClearSelected();
        selection[DELETEPOPUP] = true;
    }
}
