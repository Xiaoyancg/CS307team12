#include <Editor.h>
#include <VMTool.h>
#include "Game.h"
#ifdef __TEST_EDITOR
#include <TestEditor.h>
#endif // __TEST_EDITOR

static void ShowExampleAppMainMenuBar ();
bool running = true;
ImGui::FileBrowser saveDialog = ImGui::FileBrowser (
    ImGuiFileBrowserFlags_NoTitleBar |
    ImGuiFileBrowserFlags_SelectDirectory |
    ImGuiFileBrowserFlags_CreateNewDir
);
ImGui::FileBrowser openDialog = ImGui::FileBrowser (
    ImGuiFileBrowserFlags_NoTitleBar
);

// bool array to track the selections made on main menu bar
// TODO: swap the dumb magic number system for an enum that is easier to read - place it in the header
static bool selection[7];

GLuint *texcbo;
Core::Game *game;
int EditorMain ( int argc, char *argv[] )
{
    // Set Up SDL2
    SDL_Init ( SDL_INIT_VIDEO );

    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_FLAGS, 0 );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

    // opengl buffer details
    SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute ( SDL_GL_STENCIL_SIZE, 8 );

    // Create window with graphics context
    // sdl windows flags (attributes)
    SDL_WindowFlags window_flags =
        ( SDL_WindowFlags ) ( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI );
    // create a window for opengl. opengl can't create a window. we use sdl to create window
    SDL_Window *window = SDL_CreateWindow ( "Parchment",
                                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x and y pos. here we put it centered - CURRENTLY OVERRIDDEN BY THE MAXIMIZED FLAG
                                            1280, 720,
                                            window_flags // flags of sdl windows
    );
    // create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
    SDL_GLContext gl_context = SDL_GL_CreateContext ( window );
    // set up the opengl context for rendering in to an opengl window
    SDL_GL_MakeCurrent ( window, gl_context );

#ifdef __TEST_EDITOR
    SDLInitError = std::string ( SDL_GetError () );
#endif // __TEST_EDITOR


    // init opengl loader
    gladLoadGL ();

#ifdef __TEST_EDITOR
    OpenGLInitError = glGetError ();
#endif // __TEST_EDITOR


    // Setup Dear Imgui context
    IMGUI_CHECKVERSION ();
    // can't find document of ImGui::CreateContext. Just use it anyway
    ImGui::CreateContext ();
    // ImGuiIO: Communicate most settings and inputs/outputs to Dear ImGui using this structure.
    ImGuiIO &io = ImGui::GetIO ();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark (); // alternative: Classic

    // Setup Platform/Renderer backends
    // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
    ImGui_ImplSDL2_InitForOpenGL ( window, gl_context );
    ImGui_ImplOpenGL3_Init ();

    bool showDemoWindow = false;
    // clear color, opengl use clear color to clear the context for the next drawing
    // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
    // Also, for the convenience, we use the vector class from ImGui. ImVec4.
    // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
    // Every color in opengl stored as vector. can be vec3 or vec4.
    ImVec4 clear_color = ImVec4 ( 0.45f, 0.55f, 0.60f, 1.00f );

    openDialog.SetTypeFilters ( { ".h", ".cpp" } );

    // set the default game view window state to open
    selection[2] = true;

    texcbo = new GLuint ();
    glGenTextures ( 1, texcbo );
    game = new Core::Game ( texcbo );
    game->initShader ();
    game->s1test ();
    while ( running )
    {
        SDL_Event evt;
        while ( SDL_PollEvent ( &evt ) )
        {
            ImGui_ImplSDL2_ProcessEvent ( &evt );
            if ( evt.type == SDL_QUIT )
            {
                running = false;
            }
            if ( evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE && evt.window.windowID == SDL_GetWindowID ( window ) )
            {
                running = false;
            }
            if (evt.type == SDL_KEYDOWN) {
                game->handleInput(evt);
            }
        }
        // Draw ImGui windows
        // Start the dear Imgui frame
        ImGui_ImplOpenGL3_NewFrame ();
        ImGui_ImplSDL2_NewFrame ( window );
        ImGui::NewFrame ();
        if ( !showDemoWindow )
        {
            // test main menu bar
            ShowExampleAppMainMenuBar ();
        }

        if ( showDemoWindow )
        {
            ImGui::ShowDemoWindow ( &showDemoWindow );
        }

        // this command does not render that imgui window, we need to use opengl to render imgui
        ImGui::Render ();
        // render the ImGui windows
        glViewport ( 0, 0, ( int ) io.DisplaySize.x, ( int ) io.DisplaySize.y );
        // use the clear color we passed to opengl before to clear the context
        glClearColor ( clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w );
        glClear ( GL_COLOR_BUFFER_BIT );
        // use opengl to render the imgui window
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
        // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
        SDL_GL_SwapWindow ( window );



#ifdef __TEST_EDITOR
        if ( dobreak )  running = false;
#endif // __TEST_EDITOR

    }

    // IMGUI Clean Up
    ImGui_ImplOpenGL3_Shutdown ();
    ImGui_ImplSDL2_Shutdown ();
    ImGui::DestroyContext ();

    // SDL Clean Up
    SDL_GL_DeleteContext ( gl_context );
    SDL_DestroyWindow ( window );
    SDL_Quit ();

    return 0;
}

static void ShowExampleAppMainMenuBar()
{
#ifdef __TEST_EDITOR
    selection[0] = true;
#endif // __TEST_EDITOR

    // open new project/save as popup
    if (selection[0])
    {
        ImGui::OpenPopup("Save As");

#ifdef __TEST_EDITOR
        isSaveAsOpen = ImGui::IsPopupOpen("Save As");
#endif // __TEST_EDITOR

        selection[0] = false;
    }
    // open delete project popup
    if (selection[1])
    {
        ImGui::OpenPopup("Delete Project");
        selection[1] = false;
    }
    // game view window creation
    if (selection[2])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        // the game view window itself
        ImGui::Begin("Game View", &selection[2]);
        GLuint t = *texcbo;
        ImVec2 dims = ImGui::GetWindowSize();
        glViewport(0, 0, game->width, game->height); // Set viewport to the Game dimensions
        game->render(); // Render Game with new viewport size
        glViewport(0, 0, dims.x, dims.y); // Reset viewport size
        ImGui::Image((void*)t, ImVec2(dims.x, dims.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
    //deletion flag if an entity, map, or page is deleted
    bool delete_success = false;
    if (selection[3])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        // entity editor
        static char entity_name[128] = "";
        bool entity_info = false;
        if (ImGui::Begin("Entity Editor", &selection[3]))
        {
            ImGui::InputText("", entity_name, IM_ARRAYSIZE(entity_name));
            if (ImGui::Button("Create New Entity"))
            {
                game->currentPage->createEntity(entity_name);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Entity"))
            {
                delete_success = true;
            }
            if (ImGui::Button("Show Entity Information"))
            {
                entity_info = true;
                ImGui::OpenPopup("Entity Information");
            }
            ImGui::End();
        }

        if (entity_info)
        {
            ImGui::OpenPopup("Entity Information");
            entity_info = false;
        }

        if (ImGui::BeginPopup("Entity Information"))
        {
            std::vector <Core::Entity*> elist = game->currentPage->getEntityList();
            ImGui::Text("Entity Names: ");
            for (Core::Entity* e : elist)
            {
                ImGui::Text(e->getName().c_str());
            }
            ImGui::EndPopup();
        }
    }
    if (selection[4])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        // page editor
        static char page_name[128] = "";
        bool page_info = false;
        if (ImGui::Begin("Page Editor", &selection[4]))
        {
            ImGui::InputText("", page_name, IM_ARRAYSIZE(page_name));
            if (ImGui::Button("Create This Page"))
            {
                game->createPage(page_name);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Page"))
            {
                game->deletePage(page_name);
                delete_success = true;
            }
            if (ImGui::Button("Show Page Information"))
            {
                page_info = true;
                ImGui::OpenPopup("Page Information");
            }
            ImGui::End();
        }

        if (delete_success)
        {
            ImGui::OpenPopup("Delete Successful");
            delete_success = false;
        }

        if (ImGui::BeginPopup("Delete Successful"))
        {
            ImGui::Text("Deletion successful!");
            ImGui::EndPopup();
        }

        if (page_info)
        {
            ImGui::OpenPopup("Page Information");
            page_info = false;
        }

        if (ImGui::BeginPopup("Page Information"))
        {
            ImGui::Text("Page Name:");
            ImGui::SameLine();
            ImGui::Text(game->currentPage->GetName().c_str());
            std::vector <Core::Page*> plist = *game->getPageList();
            ImGui::Text("Page Names: ");
            for (Core::Page* p : plist)
            {
                ImGui::Text(p->GetName().c_str());
            }
            ImGui::EndPopup();
        }
    }

    //map editor
    static char map_name[128] = "";
    bool map_info = false;
    if (selection[5])
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

        // map editor
        if (ImGui::Begin("Map Editor", &selection[5]))
        {
            if (ImGui::Button("Create New Map"))
            {
                game->createMapPage(map_name);
            }
            if (ImGui::Button("Delete This Map"))
            {

            }
            if (ImGui::Button("Show Map Information "))
            {
            }
            ImGui::End();
        }
    }
    //calls saved successfully popup on project save
    if (selection[6])
    {
        ImGui::OpenPopup("Saved Successfully");
        selection[6] = false;
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
            if (ImGui::MenuItem("Delete Project"))
            {
                selection[1] = true;
            }
            if (ImGui::MenuItem("Export Project"))
            {
                // call export function from VM team
            }
            if (ImGui::MenuItem("Save"))
            {
                /* TODO (for sprint 2?): this is a really ghetto implementation atm. ideally
                if the user clicks SAVE they should get the SAVE AS popup if they haven't saved
                before (to specify a name and directory). otherwise, if they click SAVE and have
                previously specified a desired name/directory, the file should be saved with the
                same name & directory as before. right now if they click SAVE the project saves
                with a placeholder name/directory.
                */
                nlohmann::json content = game->serialize();
                WriteFile("New Game Project", (content.dump()));
                selection[6] = true;
            }
            if (ImGui::MenuItem("Save As"))
            {
                selection[0] = true;
            }
            ImGui::EndMenu();
        }

        // Add menu
        if (ImGui::BeginMenu("Add"))
        {
            ImGui::MenuItem("Game View", "", &selection[2]);
            ImGui::MenuItem("Entity Editor", "", &selection[3]);
            ImGui::MenuItem("Page Editor", "", &selection[4]);
            ImGui::MenuItem("Map Editor", "", &selection[5]);
            ImGui::EndMenu();
        }
        // ImGui::IsItemVisible ();
        ImGui::EndMainMenuBar();
    }

    // save dialog selection return
    std::string dir = "";
    saveDialog.Display ();
    if ( saveDialog.HasSelected () )
    {
        printf ( "(printf) Selected Directory: %s\n", saveDialog.GetSelected ().string ().c_str () );
        std::cout << "(cout) Selected Directory: " << saveDialog.GetSelected ().string () << std::endl;
        dir = saveDialog.GetSelected().string();
        saveDialog.ClearSelected ();
    }

    // open dialog selection return
    openDialog.Display ();
    if ( openDialog.HasSelected () )
    {
        printf ( "(printf) Selected File: %s\n", openDialog.GetSelected ().string ().c_str () );
        std::cout << "(cout) Selected File: " << openDialog.GetSelected ().string () << std::endl;
        dir = openDialog.GetSelected().string();
        openDialog.ClearSelected ();
    }

    // delete project popup
    if ( ImGui::BeginPopup ( "Delete Project" ) )
    {
        ImGui::Text ( "Are you sure you want to delete a project? Click outside of this popup to cancel." );
        if ( ImGui::Button ( "Yes" ) )
        {
            //connect to VM team delete function
            saveDialog.Open ();
        }
        ImGui::EndPopup ();
    }

    // save project popup
    //this can also be used/called when SAVE AS is successful
    static char name[128] = "";
    if (ImGui::BeginPopup("Save As"))
    {
        ImGui::Text ( "Enter the name of your project." );
        ImGui::InputText ( "", name, IM_ARRAYSIZE ( name ) );
        if ( ImGui::Button ( "Save" ) )
        {
            // connect to VM save function utilizing saveDialog selected LOCATION and buffered NAME
            nlohmann::json content = game->serialize();
            std::cout << name;
            WriteFile(name, (content.dump()));
            selection[6] = true;
            // memset to clear the buffer after use
            memset(name, 0, 128);
        }
        ImGui::EndPopup ();
    }
    if ( ImGui::BeginPopup ( "Saved Successfully" ) )
    {
        ImGui::Text ( "Project saved successfully!" );
        ImGui::EndPopup ();
    }
    /* this can be used when project deletion is successful
    if (ImGui::BeginPopup("Deleted Successfully"))
    {
        ImGui::Text("Project deleted successfully!");
        ImGui::EndPopup();
    } */
}


