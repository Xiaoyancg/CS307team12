#include "Editor.h"


Editor::Editor() {
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
    io->ConfigDockingWithShift = true;

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
    windowList[OBJECTTREE] = new ObjectTreeWindow(this, default_size);
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
    selection[SPLASHSCREEN] = true;

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
        ShowMainMenuBar();
        for (auto window : windowList) {
            if (window != nullptr) {
                window->draw();
            }
        }

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

    //this isn't really a "selection", it opens by default
    if (selection[SPLASHSCREEN])
    {
        ImGui::SetNextWindowSize(ImVec2(500 * 1.5, 400 * 1.5), ImGuiCond_Appearing);
        //ImGui::SetNextWindowContentSize(ImVec2(500*1.5, 360*1.5));
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        //ImGui::OpenPopup("Splash");
        if (ImGui::Begin("Splash", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
        {
            // Load from file
            int image_width = 0;
            int image_height = 0;
            unsigned char *image_data = stbi_load("./parchment_splash.png", &image_width, &image_height, NULL, 4);

            if (image_data == NULL)
            {
#ifdef __TEST_EDITOR
                splashImageFail = true;
#endif
                // Fail safe
                ImGui::Text("Parchment Splash Screen Failed to Load");
            }

            else
            {
                // Create a OpenGL texture identifier
                GLuint image_texture;
                glGenTextures(1, &image_texture);
                glBindTexture(GL_TEXTURE_2D, image_texture);

                // Setup filtering parameters for display
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

                // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
                stbi_image_free(image_data);

                //ImVec2 canvas_size = ImGui::GetContentRegionAvail();
                ImVec2 canvas_size = ImVec2(500 * 1.5, 363 * 1.5);
                ImGui::Image((void *)(intptr_t)image_texture, canvas_size);

                // Display Parchment version and current date-time
                ImGui::Text("Parchment Editor v0.20");
                ImGui::SameLine(ImGui::GetWindowWidth() - 180);
                auto time = std::chrono::system_clock::now();
                std::time_t current_time = std::chrono::system_clock::to_time_t(time);
                ImGui::Text(std::ctime(&current_time));
            }
            // CLOSE SPLASH ON CLICK OUTSIDE LIKE A POPUP BUT NOT BECAUSE FUCK ME I GUESS
            if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            {
                selection[SPLASHSCREEN] = false;
            }

            ImGui::End();
#ifdef __TEST_EDITOR

#endif
        }
        ImGui::PopStyleVar();
    }

    // Page editor
    if (selection[PAGEEDITOR])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(default_size, ImGuiCond_FirstUseEver);

        static char page_name[128] = "";
        static char chosen_type[128] = "";
        bool page_info = false;
        if (ImGui::Begin("Page Editor", &selection[PAGEEDITOR]))
        {
            ImGui::Text("Enter Page Name:");
            ImGui::PushItemWidth(200);
            ImGui::InputText(" ", page_name, IM_ARRAYSIZE(page_name));
            const char *page_options[] = {"Page", "Menu"};
            static int current_item = 0;
            ImGui::Text("Select Page Type:");
            if (ImGui::BeginListBox("", ImVec2(200, 2 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < IM_ARRAYSIZE(page_options); n++)
                {
                    const bool is_selected = (current_item == n);
                    if (ImGui::Selectable(page_options[n], is_selected))
                        current_item = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Create New Page"))
            {
                if (strlen(page_name) != 0)
                {
                    if (strcmp(page_options[current_item], "Menu") == 0)
                    {
                        //UNDO
                        std::string pname = page_name;
                        auto action = [this, pname]() {
                            game->createMenuPage(pname);
                        };
                        auto restore = [this, pname]() {
                            game->deletePage(pname);
                        };
                        pushAction(action, restore);
                        action();
                        //ENDUNDO
                        // memset to clear the buffer after use
                        memset(page_name, 0, 128);
                    }
                    else
                    {
                        //UNDO
                        std::string pname = page_name;
                        auto action = [this, pname]() {
                            game->createPage(pname);
                        };
                        auto restore = [this, pname]() {
                            game->deletePage(pname);
                        };
                        pushAction(action, restore);
                        action();
                        //ENDUNDO
                        // memset to clear the buffer after use
                        memset(page_name, 0, 128);
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Page"))
            {
                size_t original = game->getPageList()->size();
                //UNDO
                std::string pname = page_name;
                int idx = 0;
                auto &pList = *game->getPageList();
                for (int i = 0; i < pList.size(); i++)
                {
                    if (pList[i]->getName() == page_name)
                    {
                        idx = i;
                        break;
                    }
                }
                Core::Page savedPage = *pList[idx];
                auto action = [this, pname]() {
                    game->deletePage(pname);
                };
                auto restore = [this, idx, savedPage]() {
                    Core::Page *newPage = new Core::Page(savedPage);
                    game->getPageList()->insert(game->getPageList()->begin() + idx, newPage);
                };
                action();
                //ENDUNDO
                if (game->getPageList()->size() < original)
                {
                    delete_success = true;
                    pushAction(action, restore); // UNDO
                    // memset to clear the buffer after use
                    memset(page_name, 0, 128);
                }
            }
            if (ImGui::Button("Show Page Information"))
            {
                page_info = true;
            }
        }

        if (page_info)
        {
            ImGui::OpenPopup("Page Information");
            page_info = false;
        }

        // Page information popup
        if (ImGui::BeginPopup("Page Information"))
        {
            ImGui::Text("Page Name:");
            ImGui::SameLine();
            ImGui::Text(currPage->getName().c_str());
            std::vector<Core::Page *> plist = *game->getPageList();
            ImGui::Text("");
            ImGui::Text("Page List: ");
            for (Core::Page *p : plist)
            {
                ImGui::Text(p->getName().c_str());
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    // Script editor
    if (selection[SCRIPTEDITOR])
    {
        // set the windows default size
        ImGui::SetNextWindowSize(default_size, ImGuiCond_FirstUseEver);

        static char script_name[128] = "";
        bool script_info = false;
        if (ImGui::Begin("Script Editor", &selection[SCRIPTEDITOR]))
        {
            ImGui::PushItemWidth(200);
            ImGui::InputText(" ", script_name, IM_ARRAYSIZE(script_name));
            if (ImGui::Button("Create New Script"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Script"))
            {
            }
            if (ImGui::Button("Link This Script"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Script Information"))
            {
                script_info = true;
            }
        }

        ImGui::End();
    }

    // Sprite editor
    if (selection[SPRITEEDITOR])
    {
        // set the windows default size
        ImGui::SetNextWindowSize(default_size, ImGuiCond_FirstUseEver);

        // NOTE: Sprite name and ID do NOT have to be set to import a sprite. By default, the editor will use the filename if no name is given,
        //       and will find and return the next usable ID if none is explicitly requested on Sprite creation.
        static char sprite_name[128] = "";
        static char spriteIDInput[128] = "";
        int spriteID = -1; // -1 means the ID has not been sent (can't default to 0 because 0 is a valid ID)
        bool sprite_info = false;
        if (ImGui::Begin("Sprite Editor", &selection[SPRITEEDITOR]))
        {

            /////////////////////////////////
            ImGui::PushItemWidth(200);
            ImGui::Text("Enter Sprite Name:");
            ImGui::InputText("  ", sprite_name, IM_ARRAYSIZE(sprite_name));

            ImGui::PushItemWidth(200);

            ImGui::Text("Set Sprite ID:");
            ImGui::InputText(" ", spriteIDInput, IM_ARRAYSIZE(spriteIDInput), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::Text(currentComponent[CUR_SPRITE].c_str());
            if (ImGui::Button("Import Sprite"))
            {
                importDialog = ImGui::FileBrowser(
                    ImGuiFileBrowserFlags_NoTitleBar);
                importDialog.SetTypeFilters({".jpg", ".png"});
                importDialog.Open();
            }

            if (ImGui::Button("Show Sprite Information"))
            {
                sprite_info = true;
            }
            if (ImGui::Button("Delete This Sprite"))
            {
                // TODO: Implement sprite deletion
            }
        }

        // Check if the input is null (using != "" in this situation causes issues)
        if (spriteIDInput[0] != 0)
        {
            spriteID = atoi(spriteIDInput);
        }

        if (sprite_info)
        {
            ImGui::OpenPopup("Sprite Information");
            sprite_info = false;
        }

        // Sprite information popup
        if (ImGui::BeginPopup("Sprite Information"))
        {
            // Sprite name, dimensions, ID?
            if (currentComponent[CUR_SPRITE] == "No Component Selected")
            {
                ImGui::Text("Current Sprite Name: None");
            }
            else
            {
                // Show all sprites
                for (auto &[key, value] : game->getSprites())
                {
                    // Any sprite referenced in the .gdata file will exist in game->getSprites, but may not have been loaded into memory yet.
                    // This is just preventing referencing a null pointer. Once the sprite with the correct ID is loaded, this should correctly show its info
                    if (value)
                    {
                        // Show the current sprite name
                        std::string sprite_info = std::to_string(key).append(": ").append(value->getName());
                        ImGui::Text(sprite_info.c_str());
                    }
                }
            }
            ImGui::EndPopup();
        }

        // Sprite import dialog
        // NOTE: I had to move this here from the bottom section because this needs access to sprite_name
        importDialog.Display();
        if (importDialog.HasSelected())
        {
            // extract just the file name from the selected path
            std::string fileName = importDialog.GetSelected().string().substr(importDialog.GetSelected().string().find_last_of('\\', std::string::npos) + 1, std::string::npos);
            std::string filePath = std::filesystem::relative(importDialog.GetSelected()).string();

            if (sprite_name[0] != 0)
            {
                currentComponent[CUR_SPRITE] = sprite_name;
            }
            else
            {
                currentComponent[CUR_SPRITE] = fileName;
            }

            if (spriteID >= 0)
            {
                game->createSprite(currentComponent[CUR_SPRITE], filePath, spriteID);
            }
            else
            {
                game->createSprite(currentComponent[CUR_SPRITE], filePath);
            }

            importDialog.ClearSelected();
            memset(sprite_name, 0, 128);
            memset(spriteIDInput, 0, 128);
        }

        ImGui::End();
    }

    // Map editor
    static char map_name[128] = "";
    static int dim1 = 0;
    static int dim2 = 0;
    static int tileSize = 0;
    bool map_info = false;
    Core::MapPage *map_page = NULL;
    Core::Map *new_map = NULL;
    if (selection[MAPEDITOR])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(default_size, ImGuiCond_FirstUseEver);

        // map editor
        if (ImGui::Begin("Map Editor", &selection[MAPEDITOR]))
        {
            ImGui::Text("Enter Map Name:");
            ImGui::PushItemWidth(200);
            ImGui::InputText(" ", map_name, IM_ARRAYSIZE(map_name));
            ImGui::Text("Rows:    ");
            ImGui::PushItemWidth(100);
            ImGui::SameLine();
            ImGui::SliderInt("##1", &dim1, 0, 50);
            ImGui::Text("Columns: ");
            ImGui::SameLine();
            ImGui::SliderInt("##2", &dim2, 0, 50);
            ImGui::Text("Tile size: ");
            ImGui::SameLine();
            ImGui::SliderInt("##3", &tileSize, 0, 128);
            if (ImGui::Button("Create New Map"))
            {
                //creates a new map with map_name specified by user and dimensions as specified by user
                //UNDO
                std::string mname = map_name;
                int savedDim1 = dim1;
                int savedDim2 = dim2;
                auto action = [this, &new_map, &map_page, mname, savedDim1, savedDim2]() {
                    new_map = new Core::Map(mname, glm::vec2(savedDim1, savedDim2), 64);
                    map_page = game->createMapPage(mname, new_map);
                    new_map->setName(mname);
                    new_map->setDimensions(glm::vec2(savedDim1, savedDim2));
                };
                auto restore = [this, mname]() {
                    game->deletePage(mname);
                };
                pushAction(action, restore);
                action();
                //ENDUNDO
                //TODO: render new map
                // SETUP THE MAP CBO IF NEEDED
                currMap = game->createMapOnDefaultMapPage(map_name, dim2, dim1, tileSize);
                memset(map_name, 0, 128);
                windowList[MAPVIEW]->setVisible(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Map"))
            {
                //creates a map with 0x0 dimensions and an empty name
                /*
                memset(map_name, 0, 128);
                new_map = new Core::Map(map_name, glm::vec2(dim1, dim2), 64);
                map_page = game->createMapPage(map_name, new_map);
                new_map->setName(map_name);
                new_map->setDimensions(glm::vec2(dim1, dim2));
                delete_success = true;
                */
                dim1 = 0;
                dim2 = 0;
                tileSize = 0;
                game->deleteDefaultMapPageCurrentMap();
                currMap = nullptr;
                delete_success = true;
            }
            if (ImGui::Button("Show Map Information "))
            {
                map_info = true;
            }

            if (map_info)
            {
                ImGui::OpenPopup("Map Information");
                map_info = false;
            }

            // Map information popup
            if (ImGui::BeginPopup("Map Information"))
            {
                ImGui::Text("Map Name:");
                ImGui::SameLine();
                if (currMap != nullptr)
                {
                    ImGui::Text(currMap->getName().c_str());
                }
                else
                {
                    ImGui::Text("no map selected");
                }
                ImGui::Text("Dimensions:");
                ImGui::SameLine();
                glm::ivec2 dims;
                if (currMap != nullptr)
                {
                    dims = currMap->getDimensions();
                }
                else
                {
                    dims = glm::ivec2(0, 0);
                }
                ImGui::Text("%i Columns x %i Rows", dims.x, dims.y);

                if (currMap != nullptr)
                {
                    ImGui::Text("Tile size: %i", currMap->getTileSize());
                }
                else
                {
                    ImGui::Text("Tile size: 0");
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

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
    if (ImGui::BeginMainMenuBar())
    {
        // File menu
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project"))
            {
                texcbo = new GLuint();
                glGenTextures(1, texcbo);
                maptexcbo = new GLuint();
                glGenTextures(1, maptexcbo);
                game = new Core::Game(texcbo, maptexcbo);
                currPage = game->getCurrPage();
                game->initShader();
                windowList[GAMEVIEW]->setVisible(true);
                currentComponent[CUR_ENTITY] = "No Component Selected";
                // When user new project, it won't save
                // User should call save manually
                // selection[SAVEAS] = true;
            }
            if (ImGui::MenuItem("Open Project"))
            {
                openDialog = ImGui::FileBrowser(
                    ImGuiFileBrowserFlags_NoTitleBar);
                openDialog.SetTypeFilters({".gdata"});
                openDialog.Open();
            }
            if (ImGui::MenuItem("Delete Project"))
            {
                selection[DELETEPROJECT] = true;
            }
            /*if ( ImGui::MenuItem ( "Export Project" ) )
            {
                // call export function from VM team
                // not yet implemented as of sprint 1
            }*/

            // Can't save empty game
            // save button can be grey in the future ( no need to implement)
            if (game != nullptr)
            {
                if (ImGui::MenuItem("Save"))
                {
                    if (isSaved)
                    {
                        nlohmann::json *content = game->serialize();
                        WriteFile(dir, (content->dump(2)));
                        // pointer deletion
                        delete (content);
                        selection[SAVEPOPUP] = true;
                    }
                    else
                    {
                        selection[SAVEAS] = true;
                    }
                }
                if (ImGui::MenuItem("Save As"))
                {
                    selection[SAVEAS] = true;
                }
            }
            ImGui::EndMenu();
        }

        //only display the edit and view menus if a project exists
        if (game != nullptr)
        {
            // Edit menu
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z"))
                {
                    undo();
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y"))
                {
                    redo();
                }
                ImGui::EndMenu();
            }

            //view menu
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Object Tree", "", windowList[OBJECTTREE]->getVisiblePtr());
                ImGui::MenuItem("Game View", "", windowList[GAMEVIEW]->getVisiblePtr());
                ImGui::MenuItem("Entity Editor", "", &selection[ENTITYEDITOR]);
                ImGui::MenuItem("Page Editor", "", &selection[PAGEEDITOR]);
                ImGui::MenuItem("Map Editor", "", &selection[MAPEDITOR]);
                ImGui::MenuItem("Script Editor", "", &selection[SCRIPTEDITOR]);
                ImGui::MenuItem("Sprite Editor", "", &selection[SPRITEEDITOR]);
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }

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

    /*
     *  ========================
     *  POPUPS
     *  ========================
     */

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
