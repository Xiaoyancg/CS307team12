#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include "Game.h"

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
    //* ----------------------- Global Variables ----------------------- */

    struct GameState gstate;
    struct GameResource gresource;
    struct EditorParam geditorParam;

    // FIXME: move to core Each class that renders sprites needs a reference to
    // the same SpriteManager as this class. Whenever a new class is added that
    // renders sprites, its reference must be set here.

    // =========================
    // ATTRIBUTES OPERATION

    // =========================
    // PROPERTY OPERATION

    // =========================
    // MEMBER OPERATION
    Page *Game::addPage(Page *p)
    {
        gresource.pageList->push_back(p);
        return p;
    }
    Page *Core::Game::createPage(std::string n)
    {
        Page *p = new Page(n);
        return addPage(p);
    }
    MapPage *Game::createMapPage()
    {
        MapPage *mp = new MapPage();
        return (MapPage *)addPage(mp);
    }
    MapPage *Game::createMapPage(std::string n)
    {
        MapPage *mp = new MapPage(n);
        return (MapPage *)addPage(mp);
    }
    MapPage *Game::createMapPage(std::string n, Map *m)
    {
        MapPage *mp = new MapPage(n, m);
        return (MapPage *)addPage(mp);
    }
    std::vector<Page *> *Game::getPageList()
    {
        return gresource.pageList;
    }

    void Game::deletePage(Page *dp)
    {
        // TODO Use something like: std::vector<Page*>::iterator ptr =
        // find(pageList.begin(), pageList.end(), dp); if (ptr !=
        // pageList.end())
        for (std::vector<Page *>::iterator ptr = gresource.pageList->begin();
             ptr != gresource.pageList->end();
             ptr++)
        {
            if (*ptr == dp)
            {
                gresource.pageList->erase(ptr);
                delete (dp);
                dp = nullptr;
                break;
            }
        }
    }

    void Game::deletePage(std::string s)
    {
        for (std::vector<Page *>::iterator ptr = gresource.pageList->begin();
             ptr != gresource.pageList->end();
             ptr++)
        {
            if (!(*ptr)->getName().compare(s))
            {
                Page *p = *ptr;
                gresource.pageList->erase(ptr);
                delete (p);
                p = nullptr;
                break;
            }
        }
    }

    void Game::deleteSprite(int id)
    {
        gresource.spriteManager->deleteSprite(id);
    }

    Sprite *Game::getSpriteFromID(int id)
    {
        return gresource.spriteManager->atID(id);
    }

    std::unordered_map<int, Sprite *> Game::getSprites()
    {
        return gresource.spriteManager->getSprites();
    }

    //* ------------------ UTILITY OPERATION ----------------- *//

    Game *Game::parse(nlohmann::json &root)
    {
        nlohmann::json j = nlohmann::json{{"name", "hello"}};
        auto k = j.at("name");

        this->setGameName(root.at(std::string("GameName")).get<std::string>());
        this->setAuthor(root.at("Author").get<std::string>());
        this->setVersion(root.at("Version").get<std::string>());
        this->setLMTime(root.at("LastModifiedTime").get<std::string>());
        this->setNote(root.at("Note").get<std::string>());
        try
        {
            auto pageVec = root.at("PageList").get<std::vector<nlohmann::json>>();
            for (nlohmann::json pageJson : pageVec)
            {
                gresource.pageList->push_back(Page::parse(pageJson));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "error: " << e.what() << std::endl;
        }

        gresource.spriteManager = new SpriteManager();

        if (root.end() != root.find("SpriteList"))
        {
            gresource.spriteManager->parse(root.at("SpriteList"));
        }

        if (root.end() != root.find("SpriteEmptyIDVector"))
        {
            gresource.spriteManager->setEmptyIDV(
                root.at("SpriteEmptyIDVector").get<std::vector<int>>());
        }
        // parse should set current
        // but the info of current in json is not implemented yet
        setCurrentPage(gresource.pageList->at(0));
        // lack info of ctrlENtity
        // TODO:
        gstate.currPage->setCtrlEntity(gstate.currPage->getEntityList().at(0));
        return this;
    }

    nlohmann::json *Game::serialize()
    {
        // TODO: bg color ? I don't think that's necessary
        nlohmann::json j;
        j["FileType"] = "Parchment Game Data";
        j["GameName"] = getGameName();
        j["Author"] = getAuthor();
        j["Version"] = getVersion();
        j["LastModifiedTime"] = getLMTime();
        j["Note"] = getNote();
        // pages
        std::vector<nlohmann::json> pageVector;
        for (Page *p : *(gresource.pageList))
        {
            nlohmann::json pj;
            pj["PageName"] = p->getName();
            // entities
            std::vector<nlohmann::json> entityVector;
            for (Entity *e : p->getEntityList())
            {
                nlohmann::json ej;
                ej["EntityName"] = e->getName();
                ej["location"] = {e->getLocation().x, e->getLocation().y};
                ej["scale"] = {e->getScale().x, e->getScale().y};
                ej["rotation"] = e->getRotation();
                ej["spriteID"] = e->getSpriteID();
                entityVector.push_back(ej);
            }
            pj["EntityList"] = entityVector;
            pageVector.push_back(pj);
        }
        j["PageList"] = pageVector;
        // Sprites
        std::vector<nlohmann::json> spriteVector;
        std::unordered_map<int, Sprite *> spriteMap =
            gresource.spriteManager->getSprites();
        j["NumSprites"] = gresource.spriteManager->getNumSprites();
        // nlohmann::json only support vector array
        j["SpriteEmptyIDVector"] = gresource.spriteManager->getEmptyIDV();
        for (auto sit = spriteMap.begin(); sit != spriteMap.end(); ++sit)
        {
            Sprite &s = *(sit->second);
            nlohmann::json sj;
            sj["SpriteName"] = s.getName();
            sj["FileName"] = s.getFileName();
            sj["SpriteID"] = s.getSpriteID();
            spriteVector.push_back(sj);
        }
        j["SpriteList"] = spriteVector;

        return new nlohmann::json(j);
    }

    // Use sdl_die when an SDL error occurs to print out the error and exit
    // argument err_msg can be anything, but try to keep it related to the error
    void Game::sdl_die(const char *err_msg)
    {
        printf("%s: %s\n", err_msg, SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    void Game::initShader()
    {
        // Create viewport with the default w/h (same as the window size)
        glViewport(0, 0, gstate.width, gstate.height);

        // Source for the vertex shader
        const char *vertexSource = R"glsl(
		    #version 330 core

		    layout (location = 0) in vec2 pos;
            layout (location = 1) in vec2 textureCoords;

            // This will scale our coordinates in pixels 
            // (0 < x,y < width,height) to opengl coordinates (-1 < x,y < 1)
		    uniform vec2 scale; 

            out vec2 TexCoord;

		    void main()
		    {
		      gl_Position = vec4(scale.xy * pos.xy - 1, 0.0, 1.0);
                TexCoord = vec2(textureCoords.x,  textureCoords.y);
		    }
	    )glsl";

        // Source for the fragment shader
        const char *fragmentSource = R"glsl(
		    #version 330 core

            in vec2 TexCoord;

            uniform sampler2D texture1;

		    out vec4 FragColor;

		    void main()
		    {
		      FragColor = texture(texture1, TexCoord);
		    }
	    )glsl";

        // Create vertex and fragment shaders
        GLuint vertexShaderID, fragmentShaderID;
        vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Set the source code of the shaders
        glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
        glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);

        // Debug variables
        int success;
        char infoLog[512];

        // Compile the vertex shader, or print an error
        glCompileShader(vertexShaderID);
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n>%s\n", infoLog);
        };

        // Compile the fragment shader, or print an error
        glCompileShader(fragmentShaderID);
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n>%s\n", infoLog);
        };

        // Create new shader program
        gstate.shaderProgram = glCreateProgram();

        // Attach shaders to the new shader program
        glAttachShader(gstate.shaderProgram, vertexShaderID);
        glAttachShader(gstate.shaderProgram, fragmentShaderID);

        // Links the attached shaders in the program to their correct shader processors
        glLinkProgram(gstate.shaderProgram);

        // Delete the created shaders, now that they have already been linked
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        // Use the newly created shader program!
        glUseProgram(gstate.shaderProgram);

        // Create Vertex Buffer Object and Vertex Array Object
        GLuint vbo, vao;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Bind the VAO and VBO
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Set the format of the vertices array in opengl so it can properly read it
        // The arguments are
        // 0 to denote the 'position' vertex attribute in the vertex shader
        // 2 ints are read at a time (x, y)
        // The the type is float (GL_INT)
        // GL_FALSE means the data should not be normalized
        // Spread between each set of attributes (4 * sizeof(int))
        //// Offset isn't used yet since there's only one attribute in 'vertices'

        // attribute ptr for position coords
        glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 4 * sizeof(int), (void *)0);
        // attribute ptr for texture coords
        glVertexAttribPointer(1, 2, GL_INT, GL_FALSE, 4 * sizeof(int), (void *)(2 * sizeof(int)));

        // Enable the vertex attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Enable textures (we use GL_TEXTURE0, we don't need more than 1 at a time)
        glActiveTexture(GL_TEXTURE0);

        // Enable the Vertex Object Array
        glBindVertexArray(vao);

        glUniform1i(glGetUniformLocation(gstate.shaderProgram, "texture1"), 0); // Set texture uniform

        // Set the scale based on the width and height
        int scaleID = glGetUniformLocation(gstate.shaderProgram, "scale");
        glUniform2f(scaleID, (float)2 / gstate.width, (float)2 / gstate.height);

        // if in editor mode
        if (gstate.editorMode)
        {
            glGenFramebuffers(1, &(geditorParam.fbo));
            glBindFramebuffer(GL_FRAMEBUFFER, geditorParam.fbo);

            // texbuffer is done in editor
            // unsigned int texcbo; // texture color buffer obj
            // glGenTextures ( 1, &texcbo );
            glBindTexture(GL_TEXTURE_2D, *(geditorParam.texcbo));
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gstate.width, gstate.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, *(geditorParam.texcbo), 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void Game::initContext()
    {
        // Initialize video mode of SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            sdl_die("Error initializing SDL video mode :(");
        }

        // Most of this was from the sample code, I haven't looked into it yet
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Use OpenGL 4.6
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // Create the flags for creating the window
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        // Create a new centered game window based on window_flags (1280x720 for now)
        gstate.window = SDL_CreateWindow("Game Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gstate.width, gstate.height, window_flags);

        // Create the Game opengl context
        gstate.gl_context = SDL_GL_CreateContext(gstate.window);

        // Setup function pointers for OpenGL
        gladLoadGL();

        // Show the current context
        SDL_GL_MakeCurrent(gstate.window, gstate.gl_context);
    }

    ////void Game::handleInput(SDL_Event event)
    ////{
    ////    glm::vec2 loc;
    ////    glm::vec2 scale;
    ////    if (setCurrCtrlEntity(gstate.currPage->getCtrlEntity()) != nullptr)
    ////    {
    ////        loc = gstate.currCtrlEntity->getLocation();
    ////        scale = currCtrlEntity->getScale();
    ////        float moveBy = 5;
    ////        int scaleBy = 3;
    ////        // Control entity movement and reshape
    ////        switch (event.key.keysym.sym)
    ////        {
    ////        // Control Entity movement in the interactive demo
    ////        // Handle left arrow key
    ////        case SDLK_LEFT:
    ////            currCtrlEntity->setLocation(glm::vec2(loc.x - moveBy, loc.y)); // Move left
    ////            break;
    ////            // Handle right arrow key
    ////        case SDLK_RIGHT:
    ////            currCtrlEntity->setLocation(glm::vec2(loc.x + moveBy, loc.y)); // Move right
    ////            break;
    ////            // Handle up arrow key
    ////        case SDLK_UP:
    ////            currCtrlEntity->setLocation(glm::vec2(loc.x, loc.y + moveBy)); // Move up
    ////            break;
    ////            // Handle down arrow key
    ////        case SDLK_DOWN:
    ////            currCtrlEntity->setLocation(glm::vec2(loc.x, loc.y - moveBy)); // Move down
    ////            break;
    ////            // Control Entity scaling in the interactive demo
    ////        case SDLK_a: // a key is Scale up, for now
    ////            currCtrlEntity->setScale(glm::vec2(scale.x + scaleBy, scale.y + scaleBy));
    ////            break;
    ////        case SDLK_z: // z key is Scale down, for now
    ////            // Make sure not to scale into the negatives
    ////            if (scale.x - scaleBy >= 0 && scale.y - scaleBy >= 0)
    ////            {
    ////                currCtrlEntity->setScale(glm::vec2(scale.x - scaleBy, scale.y - scaleBy));
    ////            }
    ////            break;
    ////        }
    ////    }
    ////    // Control pages switch.
    ////    switch (event.key.keysym.sym)
    ////    {
    ////    // use 1 to look the previous page
    ////    case SDLK_1:
    ////        // have to check begin and end here
    ////        if (!_isBegin(_currPitr))
    ////        {
    ////            moveCurrentPage(_currPitr - 1);
    ////        }
    ////        break;
    ////    // use 2 to look the next page
    ////    case SDLK_2:
    ////        if (_isBeforeEnd(_currPitr))
    ////        {
    ////            moveCurrentPage(_currPitr + 1);
    ////        }
    ////        break;
    ////    }
    ////}

    // only render graphics so can be used in editor
    // TODO: Now we only have one currpage so there's no much different between using this render and use the renderer in page class. But in design it could render all pages in the current page list
    void Game::render()
    {
        if (gstate.editorMode)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, geditorParam.fbo);
        }
        glClearColor(0.1, 0.2, 0.59, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (gstate.currPage != nullptr)
        {
            // TODO: enable bg color
            //glm::vec4 pageColor = currPage->GetBackgroundColor ();
            //glClearColor ( pageColor.r, pageColor.g, pageColor.b, pageColor.a );
            glClear(GL_COLOR_BUFFER_BIT);

            gstate.currPage->render();
        }
        else
        {
            // render black
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Game::run()
    {
        // Initialize OpenGL and necessary SDL objects
        initContext();

        // Create the shaders
        initShader();

        mainLoop();

        destroy();
    }

    void Game::destroy()
    {
        // Take care of deleting SDL objects and cleanly exit
        SDL_GL_DeleteContext(gstate.gl_context);
        SDL_DestroyWindow(gstate.window);
        SDL_Quit();
    }

    /* -------------------------------- Game loop ------------------------------- */

    void Game::mainLoop()
    {

        SDL_Event event;
        bool close_window = false;
        while (!close_window)
        {
            // Input handling!
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                case SDL_KEYUP:;
                    gstate.signalHandler.sendSignal(Signal(SignalVariant{KeySignal(event.key)}, SignalType::key));
                    break;

                default:
                    break;
                }

                // default handler, change later
                switch (event.type)
                {
                    // Handle window closing
                case SDL_QUIT:
                    close_window = true;
                    break;
                    // Handle Window events
                case SDL_WINDOWEVENT:
                    // Handle resizing the window
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        // Set the width to the resized width
                        gstate.width = event.window.data1;
                        // Set the height to the resized height
                        gstate.height = event.window.data2;
                        // Set the new window dimensions
                        SDL_SetWindowSize(gstate.window, gstate.width, gstate.height);

                        // Set the new viewport size (this determines the size
                        // of the opengl -1 < pt < 1 coordinate system)
                        glViewport(0, 0, gstate.width, gstate.height);

                        // Preserve dimensions of objects after resize
                        // Set the scale based on the width and height of the screen
                        int scaleID = glGetUniformLocation(gstate.shaderProgram, "scale");
                        glUniform2f(scaleID, (float)2 / gstate.width, (float)2 / gstate.height);

                        SDL_GL_SwapWindow(gstate.window); // Show the resized window
                    }
                    break;
                }
            }

            render();

            // Show the entities by bringing showing the back buffer
            SDL_GL_SwapWindow(gstate.window);

            // Error checking! This will only print out an error if one is detected each loop
            GLenum err(glGetError());
            if (err != GL_NO_ERROR)
            {
                std::string error;

                switch (err)
                {
                case GL_INVALID_OPERATION:
                    error = "INVALID_OPERATION";
                    break;
                case GL_INVALID_ENUM:
                    error = "INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    error = "INVALID_VALUE";
                    break;
                case GL_OUT_OF_MEMORY:
                    error = "OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    error = "INVALID_FRAMEBUFFER_OPERATION";
                    break;
                }

                printf("GL_%s\n", error.c_str());
            }
#ifdef __TEST_CORE
            coreError = glGetError();
            break;
#endif // __TEST_CORE
        }
    }
}