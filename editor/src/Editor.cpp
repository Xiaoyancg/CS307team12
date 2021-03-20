#include <Editor.h>
#include <VMTool.h>
#include "Game.h"
#ifdef __TEST_EDITOR
#include <TestEditor.h>
#endif // __TEST_EDITOR

static void ShowExampleAppMainMenuBar();
bool running = true;
ImGui::FileBrowser saveDialog;
ImGui::FileBrowser openDialog;
ImGui::FileBrowser delDialog;
ImGui::FileBrowser importDialog;

// ====================================
// Global Variable

// bool array to track the selections made on main menu bar
static bool selection[COUNT];

// main texture color buffer object
GLuint *texcbo = nullptr;

// Game object
Core::Game *game = nullptr;

// Current page pointer
Core::Page *currPage = nullptr;

// project directory
std::string dir = "";

// Game Name
std::string gameName = "empty";

// is game saved before (for save)
bool isSaved = false;

// ===============================
// Main function

int EditorMain(int argc, char *argv[])
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
    SDL_Window *window = SDL_CreateWindow(
        "Parchment",                                    // Main Window Title
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
        1280, 720,                                      // CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
        window_flags                                    // flags of sdl windows
    );
    // create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    // set up the opengl context for rendering in to an opengl window
    SDL_GL_MakeCurrent(window, gl_context);

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
    ImGuiIO &io = ImGui::GetIO();
    io.WantCaptureMouse = true;
    io.WantCaptureKeyboard = true;
    // resize from window edge flag
    io.ConfigWindowsResizeFromEdges = true;
    // multiple viewports flag
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // docking flag
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // keep docking bound to shift
    io.ConfigDockingWithShift = true;


    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // alternative: Classic

    // Setup Platform/Renderer backends
    // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init();

    bool showDemoWindow = false;
    // clear color, opengl use clear color to clear the context for the next drawing
    // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
    // Also, for the convenience, we use the vector class from ImGui. ImVec4.
    // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
    // Every color in opengl stored as vector. can be vec3 or vec4.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //set the default splash screen state to open
    selection[SPLASHSCREEN] = true;

    while (running)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            ImGui_ImplSDL2_ProcessEvent(&evt);
            if (evt.type == SDL_QUIT)
            {
                running = false;
            }
            if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE && evt.window.windowID == SDL_GetWindowID(window))
            {
                running = false;
            }
            if (evt.type == SDL_KEYDOWN)
            {
                if (game != nullptr)
                {
                    game->handleInput(evt);
                }
            }
        }
        // Draw ImGui windows
        // Start the dear Imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        if (!showDemoWindow)
        {
            // turn the main viewport into a docking one to allow for docking
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            ShowExampleAppMainMenuBar();
        }

        if (showDemoWindow)
        {
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

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
        // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
        SDL_GL_SwapWindow(window);

#ifdef __TEST_EDITOR
        if (dobreak)
            running = false;
#endif // __TEST_EDITOR
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
#ifdef __TEST_EDITOR
    selection[SAVEAS] = true;
#endif // __TEST_EDITOR

    //deletion flag if an entity, map, or page is deleted
    bool delete_success = false;

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

    // Game view
    if (selection[GAMEVIEW])
    {
        // possibly implement a new function here for readability purposes
        if (game != nullptr)
        {
            // set the windows default size
            ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            // the game view window itself
            ImGui::Begin("Game View", &selection[GAMEVIEW]);

            //ImVec2 dims = ImGui::GetWindowSize();

            // Get size of drawable space on the window, instead of the entire size of the window
            ImVec2 canvas_size = ImGui::GetContentRegionAvail(); 

            glViewport(0, 0, game->width, game->height); // Set viewport to the Game dimensions

            game->render();                             // Render Game with new viewport size

            //glViewport(0, 0, (int)dims.x, (int)dims.y); // Reset viewport size
            //ImGui::Image((void *)(*texcbo), ImVec2(dims.x, dims.y), ImVec2(0, 1), ImVec2(1, 0));

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size
            ImGui::Image((void*)(*texcbo), ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

    //object tree
    if (selection[OBJECTTREE])
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Object Tree", &selection[OBJECTTREE]))
        {

        }
        ImGui::End();
    }
    
    //this isnt really a "selection", it opens by default
    
    if (selection[SPLASHSCREEN])
    {
        int dwWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
        int dwHeight = GetSystemMetrics(SM_CYSCREEN) / 2;
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        if (ImGui::Begin("Parchment", &selection[SPLASHSCREEN]))
        {
            // Here we will implement a render window type thing I assume to hold the slpash screen
            ImGui::Text("Parchment Splash Screen");
        }
        ImGui::End();
    }

    // Entity editor
    if (selection[ENTITYEDITOR])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        static char entity_name[128] = "";
        bool entity_info = false;
        if (ImGui::Begin("Entity Editor", &selection[ENTITYEDITOR]))
        {
            ImGui::Text("Enter Entity Name:");
            ImGui::InputText(" ", entity_name, IM_ARRAYSIZE(entity_name));
            if (ImGui::Button("Create New Entity"))
            {
                currPage->createEntity(entity_name);
                // memset to clear the buffer after use
                memset(entity_name, 0, 128);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Entity"))
            {
                size_t original = game->getCurrPage()->getEntityList().size();
                currPage->deleteEntity(entity_name);
                if (currPage->getEntityList().size() < original)
                {
                    delete_success = true;
                    // memset to clear the buffer after use
                    memset(entity_name, 0, 128);
                }
            }
            if (ImGui::Button("Show Entity Information"))
            {
                entity_info = true;
            }
        }
        
        if (entity_info)
        {
            ImGui::OpenPopup("Entity Information");
            entity_info = false;
        }

        // Entity information popup
        if (ImGui::BeginPopup("Entity Information"))
        {
            std::vector<Core::Entity*> elist = currPage->getEntityList();
            ImGui::Text("Entity Names: ");
            for (Core::Entity* e : elist)
            {
                ImGui::Text(e->getName().c_str());
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    // Page editor
    if (selection[PAGEEDITOR])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        static char page_name[128] = "";
        bool page_info = false;
        if (ImGui::Begin("Page Editor", &selection[PAGEEDITOR]))
        {
            ImGui::Text("Enter Page Name:");
            ImGui::InputText(" ", page_name, IM_ARRAYSIZE(page_name));
            const char* page_options[] = { "Page", "Menu" };
            static int current_item = 0;
            if (ImGui::BeginListBox(""))
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
            if (ImGui::Button("Create This Page"))
            {
                game->createPage(page_name);
                // memset to clear the buffer after use
                memset(page_name, 0, 128);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Page"))
            {
                size_t original = game->getPageList()->size();
                game->deletePage(page_name);
                if (game->getPageList()->size() < original)
                {
                    delete_success = true;
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
            ImGui::Text("Page Names: ");
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
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        static char script_name[128] = "";
        bool script_info = false;
        if (ImGui::Begin("Script Editor", &selection[SCRIPTEDITOR]))
        {
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
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        bool sprite_info = false;
        if (ImGui::Begin("Sprite Editor", &selection[SPRITEEDITOR]))
        {
            if (ImGui::Button("Import Sprite"))
            {
                importDialog = ImGui::FileBrowser(
                    ImGuiFileBrowserFlags_NoTitleBar);
                importDialog.SetTypeFilters({ ".jpg", ".png" });
                importDialog.Open();
            }
            if (ImGui::Button("Show Sprite Information"))
            {
                sprite_info = true;
            }
        }

        ImGui::End();
    }

    // Map editor
    static char map_name[128] = "";
    bool map_info = false;
    if (selection[MAPEDITOR])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        // map editor
        if (ImGui::Begin("Map Editor", &selection[MAPEDITOR]))
        {
            ImGui::Text("Enter Map Name:");
            ImGui::InputText(" ", map_name, IM_ARRAYSIZE(map_name));
            if (ImGui::Button("Create New Map"))
            {
                game->createMapPage(map_name);
                // memset to clear the buffer after use
                memset(map_name, 0, 128);
            }
            if (ImGui::Button("Delete This Map"))
            {
                // TODO remove map function
                // memset to clear the buffer after use
                memset(map_name, 0, 128);
            }
            if (ImGui::Button("Show Map Information "))
            {
                // TODO show map list?
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
                game = new Core::Game(texcbo);
                currPage = game->getCurrPage();
                game->initShader();
                selection[GAMEVIEW] = true;
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
                    /* TODO (for sprint 2?): this is a really ghetto implementation atm. ideally
                    if the user clicks SAVE they should get the SAVE AS popup if they haven't saved
                    before (to specify a name and directory). otherwise, if they click SAVE and have
                    previously specified a desired name/directory, the file should be saved with the
                    same name & directory as before. right now if they click SAVE the project saves
                    with a placeholder name/directory.
                    */
                    if (isSaved)
                    {
                        nlohmann::json *content = game->serialize();
                        WriteFile(dir, (content->dump()));
                        // pointer deletion
                        delete (content);
                        selection[SAVEPOPUP] = true;
                    }
                    else
                    {
                        ImGui::OpenPopup("Save As");
                    }
                }
                if (ImGui::MenuItem("Save As"))
                {
                    selection[SAVEAS] = true;
                }
            }
            ImGui::EndMenu();
        }

        // Add menu
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Object Tree", "", &selection[OBJECTTREE]);
            ImGui::MenuItem("Game View", "", &selection[GAMEVIEW]);
            ImGui::MenuItem("Entity Editor", "", &selection[ENTITYEDITOR]);
            ImGui::MenuItem("Page Editor", "", &selection[PAGEEDITOR]);
            ImGui::MenuItem("Map Editor", "", &selection[MAPEDITOR]);
            ImGui::MenuItem("Script Editor", "", &selection[SCRIPTEDITOR]);
            ImGui::MenuItem("Sprite Editor", "", &selection[SPRITEEDITOR]);
            ImGui::EndMenu();
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
        game = new Core::Game(*j, texcbo);
        // pointer deletion
        delete (j);
        gameName = game->getGameName();
        currPage = game->getCurrPage();
        game->initShader();
        selection[GAMEVIEW] = true;
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

    // import dialog selection return
    importDialog.Display();
    if (importDialog.HasSelected())
    {
        // temporary output lines - connect to importing function - possibly link to sprite obj?
        printf ( "(printf) Selected File: %s\n", importDialog.GetSelected ().string ().c_str () );
        std::cout << "(cout) Selected File: " << importDialog.GetSelected().string() << std::endl;
        importDialog.ClearSelected();
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