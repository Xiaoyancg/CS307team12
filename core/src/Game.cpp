#include <Game.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include <MapPage.h>

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE


// by Larry
namespace Core
{

    int Game::width = 1280;
    int Game::height = 720;


    std::string Game::GetGameName ()
    {
        return std::string ( this->gameName );
    }

    void Game::SetGameName ( std::string newName )
    {
        this->gameName = newName;
    }


    void Game::SetAuthor ( std::string newAuthor )
    {
        this->author = newAuthor;
    }

    std::string Game::GetAuthor ()
    {
        return this->author;
    }


    void Game::SetVersion ( std::string newVersion )
    {
        this->version = newVersion;
    }

    std::string Game::GetVersion ()
    {
        return this->version;
    }


    void Game::SetLMTime ( std::string time )
    {
        this->lMTime = time;
    }

    void Game::SetLMTime ()
    {
        time_t rawtime;
        struct tm *timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        this->lMTime = std::string ( asctime ( timeinfo ) );
    }


    std::string Game::GetLMTime ()
    {
        return this->lMTime;
    }

    void Game::SetNote ( std::string newNote )
    {
        this->note = newNote;
    }


    std::string Game::GetNote ()
    {
        return this->note;
    }

    // =========================
    // CONSTRUCTOR

    Game::Game ( GLuint *o )
    {
        texcbo = o;
        useFramebuffer = true;
        this->gameName = "editortestname";
    }
    Game::Game ():Game ( "empty" )
    { }

    Game::Game ( std::string gameName ) : gameName ( gameName )
    {
        useFramebuffer = false;
    }

    Game::Game ( nlohmann::json &json )
    {

    }
    // =========================
    // ATTRIBUTES OPERATION

    // =========================
    // PROPERTY OPERATION

    // =========================
    // MEMBER OPERATION
    Page *Game::addPage ( Page *p )
    {
        this->pageList.push_back ( p );
        return p;
    }
    Page *Core::Game::createPage ( std::string n )
    {
        Page *p = new Page ( n );
        return addPage ( p );
    }

    MapPage *Game::createMapPage ( std::string n )
    {
        MapPage *mp = new MapPage ( n );
        return ( MapPage * ) addPage ( mp );
    }
    MapPage *Game::createMapPage ( std::string n, Map *m )
    {
        MapPage *mp = new MapPage ( n, m );
        return ( MapPage * ) addPage ( mp );
    }
    // =========================
    // UTILITY OPERATION



// Larry out

    using json = nlohmann::json;

    Game *Game::parse ( json &root )
    {
        std::unique_ptr<Game> game{ new Game };
        game->SetGameName ( root.at ( "GameName" ).get<std::string> () );
        game->SetAuthor ( root.at ( "Author" ).get<std::string> () );
        game->SetVersion ( root.at ( "Version" ).get<std::string> () );
        game->SetLMTime ( root.at ( "LastModifiedTime" ).get<std::string> () );
        game->SetNote ( root.at ( "Note" ).get<std::string> () );
        for ( json pageJson : root.at ( "PageList" ).get<std::vector<json>> () )
        {
            std::unique_ptr<Page> page{ new Page };

        }

        return new Game;
    }


    nlohmann::json *Game::serialize ()
    {
        json j;
        j["FileType"] = "Parchment Game Data";
        j["GameName"] = GetGameName ();
        j["Author"] = GetAuthor ();
        j["Version"] = GetVersion ();
        j["LastModifiedTime"] = GetLMTime ();
        j["Note"] = GetNote ();
        for ( Page *p : this->pageList )
        {
            j["PageList"][p->GetName ()]["PageName"] = p->GetName ();
            for ( Entity *e : p->getEntityList () )
            {
                j["PageList"][p->GetName ()]
                    ["EntityList"][e->getName ()]
                    ["EntityName"] = e->getName ();
            }
        }
        return new json ( j );
    }

    // Use sdl_die when an SDL error occurs to print out the error and exit
    // argument err_msg can be anything, but try to keep it related to the error
    void Game::sdl_die ( const char *err_msg )
    {
        printf ( "%s: %s\n", err_msg, SDL_GetError () );
        SDL_Quit ();
        exit ( 1 );
    }


    void Game::initShader ()
    {
        // Create viewport with the default w/h (same as the window size)
        glViewport ( 0, 0, width, height );

// Source for the vertex shader
        const char *vertexSource = R"glsl(
		#version 450 core

		layout (location = 0) in vec2 pos;
		uniform vec2 scale; // This will scale our coordinates in pixels (0 < x,y < width,height) to opengl coordinates (-1 < x,y < 1)

		void main()
		{
		  gl_Position = vec4(scale.xy * pos.xy - 1, 0.0, 1.0);
		}
	)glsl";

    // Source for the fragment shader
        const char *fragmentSource = R"glsl(
		// fragment shader
		#version 330 core

		out vec4 FragColor;

		void main()
		{
		  FragColor = vec4(1.0,1.0,1.0,1.0);
		}
	)glsl";

    // Create vertex and fragment shaders
        GLuint vertexShaderID, fragmentShaderID;
        vertexShaderID = glCreateShader ( GL_VERTEX_SHADER );
        fragmentShaderID = glCreateShader ( GL_FRAGMENT_SHADER );

        // Set the source code of the shaders
        glShaderSource ( vertexShaderID, 1, &vertexSource, NULL );
        glShaderSource ( fragmentShaderID, 1, &fragmentSource, NULL );

        // Debug variables
        int success;
        char infoLog[512];

        // Compile the vertex shader, or print an error
        glCompileShader ( vertexShaderID );
        glGetShaderiv ( vertexShaderID, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog ( vertexShaderID, 512, NULL, infoLog );
            printf ( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n>%s\n", infoLog );
        };

        // Compile the fragment shader, or print an error
        glCompileShader ( fragmentShaderID );
        glGetShaderiv ( fragmentShaderID, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog ( fragmentShaderID, 512, NULL, infoLog );
            printf ( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n>%s\n", infoLog );
        };


        // Create new shader program
        shaderProgram = glCreateProgram ();

        // Attach shaders to the new shader program
        glAttachShader ( shaderProgram, vertexShaderID );
        glAttachShader ( shaderProgram, fragmentShaderID );

        // Links the attached shaders in the program to their correct shader processors
        glLinkProgram ( shaderProgram );

        // Delete the created shaders, now that they have already been linked
        glDeleteShader ( vertexShaderID );
        glDeleteShader ( fragmentShaderID );

        // Use the newly created shader program!
        glUseProgram ( shaderProgram );

        // Create Vertex Buffer Object and Vertex Array Object
        GLuint vbo, vao;
        glGenVertexArrays ( 1, &vao );
        glGenBuffers ( 1, &vbo );

        // Bind the VAO and VBO
        glBindVertexArray ( vao );
        glBindBuffer ( GL_ARRAY_BUFFER, vbo );

        // Set the format of the vertices array in opengl so it can properly read it
        // The arguments are 
        // 0 to denote the 'position' vertex attribute in the vertex shader
        // 2 ints are read at a time (x, y)
        // The the type is float (GL_INT)
        // GL_FALSE means the data should not be normalized
        // The size to be read in bytes (2 * sizeof(int))
        // Offset isn't used yet since there's only one attribute in 'vertices'
        glVertexAttribPointer ( 0, 2, GL_INT, GL_FALSE, 2 * sizeof ( int ), ( void * ) 0 );

        // Enable the position vertex attribute
        glEnableVertexAttribArray ( 0 );

        // Enable the Vertex Object Array
        glBindVertexArray ( vao );

        // Set the scale based on the width and height
        int scaleID = glGetUniformLocation ( shaderProgram, "scale" );
        glUniform2f ( scaleID, ( float ) 2 / width, ( float ) 2 / height );


        // Larry framebuffer
        if ( useFramebuffer )
        {
            glGenFramebuffers ( 1, &fbo );
            glBindFramebuffer ( GL_FRAMEBUFFER, fbo );

            // texbuffer is done in editor
            // unsigned int texcbo; // texture color buffer obj
            // glGenTextures ( 1, &texcbo );
            glBindTexture ( GL_TEXTURE_2D, *texcbo );
            glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glBindTexture ( GL_TEXTURE_2D, 0 );

            glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texcbo, 0 );

            glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
        }
    }

    // This takes care of initializing everything SDL needs to begin running
    void Game::init ()
    {
// Initialize video mode of SDL
        if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 )
        {
            sdl_die ( "Error initializing SDL video mode :(" );
        }

        // Most of this was from the sample code, I haven't looked into it yet
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_FLAGS, 0 );
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

        // Use OpenGL 4.6
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute ( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

        SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 24 );
        SDL_GL_SetAttribute ( SDL_GL_STENCIL_SIZE, 8 );

        // Create the flags for creating the window
        SDL_WindowFlags window_flags = ( SDL_WindowFlags ) ( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );

        // Create a new centered game window based on window_flags (1280x720 for now)
        window = SDL_CreateWindow ( "Game Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags );

        // Create the Game opengl context
        gl_context = SDL_GL_CreateContext ( window );

        // Setup function pointers for OpenGL
        gladLoadGL ();

    }


    void Game::render ()
    {
        if ( useFramebuffer )
        {
            glBindFramebuffer ( GL_FRAMEBUFFER, fbo );
        }
        glClearColor ( 0.1, 0.2, 0.59, 1.0 );
        glClear ( GL_COLOR_BUFFER_BIT );

        if ( currentPage != nullptr )
        {
            glm::vec4 pageColor = currentPage->GetBackgroundColor ();
            glClearColor ( pageColor.r, pageColor.g, pageColor.b, pageColor.a );
            glClear ( GL_COLOR_BUFFER_BIT );

            currentPage->render ();
        }

            glBindFramebuffer ( GL_FRAMEBUFFER, 0 );

    }
    void Game::run ()
    {

    }


    Entity *entityInteractive;
    Entity *entityTallThin;
    Entity *entityShortWide;
    Entity *entityVeryShortWide;
    Entity *entityOrigin;
    Page *entityPage;
    MapPage *mapPage1;
    MapPage *mapPage2;


    void Game::s1test ()
    {

        ///////////////
        // ENTITY TEST (This is here just for demo purposes)
        // This will be moved within an actual page once sprites are implemented.
        // For now it's separated from a page because all rendered objects are a single color and there
        // would be no way to see a difference from a Tile on the map and an Entity

        entityPage = this->createPage ( "entityPage" );
        entityPage->SetBackgroundColor ( 0.1, 0.2, 0.59, 1.0 );

        entityInteractive = entityPage->createEntity ( "interactive", glm::vec2 ( 50, 50 ), glm::vec2 ( 64, 64 ), 0, 0 );
        entityTallThin = entityPage->createEntity ( "interactive", glm::vec2 ( 200, 200 ), glm::vec2 ( 32, 64 ), 0, 0 );
        entityShortWide = entityPage->createEntity ( "shortWide", glm::vec2 ( 500, 500 ), glm::vec2 ( 64, 32 ), 0, 0 );
        entityVeryShortWide = entityPage->createEntity ( "veryShortWide", glm::vec2 ( 640, 100 ), glm::vec2 ( 128, 16 ), 0, 0 );
        entityOrigin = entityPage->createEntity ( "origin", glm::vec2 ( 1000, 300 ), glm::vec2 ( 128, 128 ), 0, 0 );
        ///////////////

        ///////////////
        // MAP TEST
        int spriteMap[] = {
              1,  2,  3,  4,
              5,  6,  7,  8,
             11, 22, 33, 44,
             55, 66, 77, 88
        };

        Map *map1 = new Map ( glm::vec2 ( 4, 4 ), 64 );
        map1->setMapTileSpritesFromArray ( spriteMap );
        Map *map2 = new Map ( glm::vec2 ( 16, 16 ), 32 );

        // Here are the 2 ways to make MapPages with set maps
        mapPage1 = this->createMapPage ( "1", map1 );
        mapPage2 = this->createMapPage ( "2" );
        mapPage2->setMap ( map2 ); // Sets empty map page 2's map

        // very important
        currentPage = mapPage1;
    }
    void Game::destroy ()
    {
        // Take care of deleting SDL objects and cleanly exit 
        SDL_GL_DeleteContext ( gl_context );
        SDL_DestroyWindow ( window );
        SDL_Quit ();
    }
    int Game::coreMain ( int argc, char *argv[] )
    {

        // Initdialize OpenGL and necessary SDL objects
        init ();
        // Create the shaders
        initShader ();

        s1test ();
        //serialize ();

        mainLoop ();

        destroy ();
        return 0;
    }

    ///////////////
    // Game loop //
    ///////////////
    void Game::mainLoop ()
    {

        SDL_Event event;
        int close_window = false;
        while ( !close_window )
        {
    // Input handling!
            while ( SDL_PollEvent ( &event ) )
            {
                switch ( event.type )
                {
    // Handle window closing
                    case SDL_QUIT:
                        close_window = true;
                        break;
                        // Handle Window events
                    case SDL_WINDOWEVENT:
                        // Handle resizing the window
                        if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
                        {
                            width = event.window.data1; // Set the width to the resized width
                            height = event.window.data2; // Set the height to the resized height
                            SDL_SetWindowSize ( window, width, height ); // Set the new window dimensions

                             // Set the new viewport size (this determines the size of the opengl -1 < pt < 1 coordinate system)
                            glViewport ( 0, 0, width, height );

                        // Preserve dimensions of objects after resize
                        // Set the scale based on the width and height of the screen
                            int scaleID = glGetUniformLocation ( shaderProgram, "scale" );
                            glUniform2f ( scaleID, ( float ) 2 / width, ( float ) 2 / height );

                            SDL_GL_SwapWindow ( window ); // Show the resized window
                        }
                        break;
                        // Handle Keypresses
                    case SDL_KEYDOWN:
                        glm::vec2 loc = entityInteractive->getLocation ();
                        glm::vec2 scale = entityInteractive->getScale ();
                        float moveBy = 5;
                        int scaleBy = 3;

                        switch ( event.key.keysym.sym )
                        {
// Control Entity movement in the interactive demo
// Handle left arrow key
                            case SDLK_LEFT:
                                entityInteractive->setLocation ( glm::vec2 ( loc.x - moveBy, loc.y ) ); // Move left
                                break;
                                // Handle right arrow key
                            case SDLK_RIGHT:
                                entityInteractive->setLocation ( glm::vec2 ( loc.x + moveBy, loc.y ) ); // Move right
                                break;
                                // Handle up arrow key
                            case SDLK_UP:
                                entityInteractive->setLocation ( glm::vec2 ( loc.x, loc.y + moveBy ) ); // Move up
                                break;
                                // Handle down arrow key
                            case SDLK_DOWN:
                                entityInteractive->setLocation ( glm::vec2 ( loc.x, loc.y - moveBy ) ); // Move down
                                break;

                                // Control Entity scaling in the interactive demo
                            case SDLK_a: // a key is Scale up, for now
                                entityInteractive->setScale ( glm::vec2 ( scale.x + scaleBy, scale.y + scaleBy ) );
                                break;
                            case SDLK_z: // z key is Scale down, for now
                                // Make sure not to scale into the negatives
                                if ( scale.x - scaleBy >= 0 && scale.y - scaleBy >= 0 )
                                {
                                    entityInteractive->setScale ( glm::vec2 ( scale.x - scaleBy, scale.y - scaleBy ) );
                                }
                                break;

                                // Control demo pages. Press '1' to see map1, '2', to see map2, and '3' to see the initial interactive demo
                            case SDLK_1:
                                currentPage = mapPage1;
                                break;
                            case SDLK_2:
                                currentPage = mapPage2;
                                break;
                            case SDLK_3:
                                currentPage = entityPage;
                                break;
                        }
                }
            }

        // Show the current context
        SDL_GL_MakeCurrent ( window, gl_context );



            render ();

        SDL_GL_SwapWindow ( window ); // Show the entities by bringing showing the back buffer
        ///////////////



            // Error checking! This will only print out an error if one is detected each loop
            GLenum err ( glGetError () );
            if ( err != GL_NO_ERROR )
            {
                std::string error;

                switch ( err )
                {
                    case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
                    case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
                    case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
                    case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
                }

                printf ( "GL_%s\n", error.c_str () );
            }
#ifdef __TEST_CORE
            coreError = glGetError ();
            break;
#endif // __TEST_CORE
        }


    }
}
