#include <Game.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <MapPage.h>

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{

    int Game::width = 1280;
    int Game::height = 720;

    Game::Game(std::string gameName) : gameName(gameName)
    {
        initialize();
        this->setCurrentPage(this->createPage("emptyPage"));
    }

    Game::Game(nlohmann::json &json)
    {
        initialize();
        this->parse(json);
    }

    void Game::initialize() {
        mGameMapPage = new MapPage("Default MapPage");
        Entity::mGameSprites = &mGameSprites;
        MapPage::mGameSprites = &mGameSprites;

        _logicManager = LogicManager(&pageList, &currPage);

    }

    // =========================
    // ATTRIBUTES OPERATION

    std::string Game::getGameName()
    {
        return this->gameName;
    }

    void Game::setGameName(std::string newName)
    {
        this->gameName = newName;
    }

    void Game::setAuthor(std::string newAuthor)
    {
        this->author = newAuthor;
    }

    std::string Game::getAuthor()
    {
        return this->author;
    }

    void Game::setVersion(std::string newVersion)
    {
        this->version = newVersion;
    }

    std::string Game::getVersion()
    {
        return this->version;
    }

    void Game::setLMTime(std::string time)
    {
        this->lMTime = time;
    }

    void Game::setLMTime()
    {
        time_t rawtime;
        struct tm *timeinfo = new struct tm();
        time(&rawtime);
        localtime_s(timeinfo, &rawtime);
        char c[256];
        asctime_s(c, 256, timeinfo);
        this->lMTime = std::string(c);
    }

    std::string Game::getLMTime()
    {
        return this->lMTime;
    }

    void Game::setNote(std::string newNote)
    {
        this->note = newNote;
    }

    std::string Game::getNote()
    {
        return this->note;
    }

    // =========================
    // MEMBER OPERATION
    Page *Game::addPage(Page *p)
    {
        this->pageList.push_back(p);
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
    Map *Game::createMapOnDefaultMapPage(std::string name, int cols, int rows, int tilesize)
    {
        if (mGameMapPage)
        {
            return mGameMapPage->addMap(new Map(name, glm::vec2(cols, rows), tilesize));
        }
        return nullptr;
    }
    void Game::deleteDefaultMapPageCurrentMap()
    {
        mGameMapPage->deleteCurrMap();
    }
    MenuPage *Game::createMenuPage(std::string name, Menu *menu)
    {
        MenuPage *mp = new MenuPage(name, menu);
        return (MenuPage *)addPage(mp);
    }
    MenuPage *Game::createMenuPage(std::string name)
    {
        MenuPage *mp = new MenuPage(name);
        return (MenuPage *)addPage(mp);
    }
    MenuPage *Game::createMenuPage()
    {
        MenuPage *mp = new MenuPage();
        return (MenuPage *)addPage(mp);
    }
    std::vector<Page *>& Game::getPageList()
    {
        return pageList;
    }

    void Game::deletePage(Page *dp)
    {
        // TODO Use something like:
        // std::vector<Page*>::iterator ptr = find(pageList.begin(), pageList.end(), dp);
        // if (ptr != pageList.end())
        for (std::vector<Page *>::iterator ptr = pageList.begin();
             ptr != pageList.end();
             ptr++)
        {
            if (*ptr == dp)
            {
                pageList.erase(ptr);
                delete (dp);
                dp = nullptr;
                break;
            }
        }
    }

    void Game::deletePage(std::string s)
    {
        for (std::vector<Page *>::iterator ptr = pageList.begin();
             ptr != pageList.end();
             ptr++)
        {
            if (!(*ptr)->getName().compare(s))
            {
                Page *p = *ptr;
                pageList.erase(ptr);
                delete (p);
                p = nullptr;
                break;
            }
        }
    }

    unsigned int Game::createSprite(std::string name, std::string filename)
    {
        // Return OpenGL ID of the new sprite
        return mGameSprites.createSprite(name, filename);
    }
    unsigned int Game::createSprite(std::string name, std::string filename, int id)
    {
        return mGameSprites.createSprite(name, filename, id);
    }

    void Game::deleteSprite(int id)
    {
        mGameSprites.deleteSprite(id);
    }

    Sprite *Game::getSpriteFromID(int id)
    {
        return mGameSprites.atID(id);
    }

    std::unordered_map<int, Sprite *> Game::getSprites()
    {
        return mGameSprites.getSprites();
    }

    std::vector<Map *> Game::getDefaultMapPageMaps()
    {
        return mGameMapPage->getMaps();
    }

    MapPage *Game::getDefaultMapPage()
    {
        return mGameMapPage;
    }

    void Game::renderDefaultMapPage()
    {
        mGameMapPage->render();
    }

    //* -------------------- LOGIC WRAPPER ------------------- *//

    Signal *Game::createSignal()
    {
        return _logicManager.createSignal();
    }

    Script *Game::createScript()
    {
        return _logicManager.createScript();
    }

    Logic *Game::createLogic()
    {
        return _logicManager.createLogic();
    }

    void Game::deleteLogic(int logicId)
    {
        _logicManager.deleteLogic(logicId);
    }
    void Game::deleteSignal(int signalId)
    {
        _logicManager.deleteSignal(signalId);
    }
    void Game::deleteScript(int scriptId)
    {
        _logicManager.deleteSignal(scriptId);
    }
    Game *Game::parse(nlohmann::json &root)
    {
        this->setGameName(root.at(std::string("GameName")).get<std::string>());
        this->setAuthor(root.at("Author").get<std::string>());
        this->setVersion(root.at("Version").get<std::string>());
        this->setLMTime(root.at("LastModifiedTime").get<std::string>());
        this->setNote(root.at("Note").get<std::string>());
        try
        {
            auto pageVec = root.at("pageList").get<std::vector<nlohmann::json>>();
            for (nlohmann::json pageJson : pageVec)
            {
                this->pageList.push_back(Page::parse(pageJson));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "error: " << e.what() << std::endl;
        }
        mGameSprites = SpriteManager::SpriteManager();

        if (root.contains("spriteList"))
        {
            mGameSprites.parse(root.at("spriteList"));
        }
        // TODO: Logic
        if (root.contains("logicManager"))
        {
            // TODO
            _logicManager.parse(root.at("logicManager"));
        }
        if (root.contains("startPoint"))
        {
            // TODO
        }
        // TODO parse should set current
        // but the info of current in json is not implemented yet
        setCurrentPage(this->pageList.at(0));

        return this;
    }

    nlohmann::json *Game::serialize()
    {
        // TODO: bg color ? I don't think that's necessary
        nlohmann::json *ret = new nlohmann::json;
        nlohmann::json &j = *ret;
        j["FileType"] = "Parchment Game Data";
        j["GameName"] = getGameName();
        j["Author"] = getAuthor();
        j["Version"] = getVersion();
        j["LastModifiedTime"] = getLMTime();
        j["Note"] = getNote();
        // pages
        std::vector<nlohmann::json> pageVector;
        for (Page *p : this->pageList)
        {
            nlohmann::json pj;
            pj["PageName"] = p->getName();

            // Determine if Page is a MenuPage. Kinda junk implementation
            if (!strcmp(typeid(*p).name(), "class Core::MenuPage"))
            {
                pj["isMenu"] = true;
            }
            else
            {
                pj["isMenu"] = false;
            }

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
                if (e->isControlledEntity())
                {
                    ej["control"] = true;
                }
                entityVector.push_back(ej);
            }
            pj["entityList"] = entityVector;
            pageVector.push_back(pj);
        }
        j["pageList"] = pageVector;

        // Sprites
        std::vector<nlohmann::json> spriteVector;
        for (auto &[key, value] : mGameSprites.getSprites())
        {
            Sprite &s = *value;
            nlohmann::json sj;
            sj["SpriteName"] = s.getName();
            sj["FileName"] = s.getFileName();
            sj["SpriteID"] = s.getSpriteID();
            spriteVector.push_back(sj);
        }
        if (spriteVector.size() > 0)
        {
            j["spriteList"] = spriteVector;
        }

        //TODO LogicManager
        nlohmann::json logicManagerjs;
        std::vector<nlohmann::json> logicVector;
        //TODO for each logic push to logic vector
        logicManagerjs["logicList"] = logicVector;
        std::vector<nlohmann::json> scriptVector;
        //TODO for each script push to script Vector
        logicManagerjs["scriptList"] = scriptVector;
        std::vector<nlohmann::json> signalVector;
        // TODO for each signal ...
        logicManagerjs["signalList"] = signalVector;
        j["logicManager"] = logicManagerjs;

        // TODO start point
        nlohmann::json startPoint;
        j["startPoint"] = startPoint;
        return ret;
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
        glViewport(0, 0, width, height);

        // Source for the vertex shader
        const char *vertexSource = R"glsl(
		    #version 330 core

		    layout (location = 0) in vec2 pos;
            layout (location = 1) in vec2 textureCoords;

		    uniform vec2 scale; // This will scale our coordinates in pixels (0 < x,y < width,height) to opengl coordinates (-1 < x,y < 1)

            out vec2 TexCoord;

		    void main()
		    {
		      gl_Position = vec4(scale.xy * pos.xy - 1, 0.0, 1.0);
                TexCoord = vec2(textureCoords.x,  1-textureCoords.y);
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
        shaderProgram = glCreateProgram();

        // Attach shaders to the new shader program
        glAttachShader(shaderProgram, vertexShaderID);
        glAttachShader(shaderProgram, fragmentShaderID);

        // Links the attached shaders in the program to their correct shader processors
        glLinkProgram(shaderProgram);

        // Delete the created shaders, now that they have already been linked
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        // Use the newly created shader program!
        glUseProgram(shaderProgram);

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

        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // Set texture uniform

        // Set the scale based on the width and height
        int scaleID = glGetUniformLocation(shaderProgram, "scale");
        glUniform2f(scaleID, (float)2 / width, (float)2 / height);
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
        window = SDL_CreateWindow("Game Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

        // Create the Game opengl context
        gl_context = SDL_GL_CreateContext(window);

        // Setup function pointers for OpenGL
        gladLoadGL();

        // Show the current context
        SDL_GL_MakeCurrent(window, gl_context);
    }

    Page *Game::getCurrPage()
    {
        return this->currPage;
    }

    void Game::handleInput(SDL_Event event)
    {
        glm::vec2 loc;
        glm::vec2 scale;
        if (currPage->getCtrlEntity() != nullptr)
        {
            auto currCtrlEntity = currPage->getCtrlEntity();
            loc = currCtrlEntity->getLocation();
            scale = currCtrlEntity->getScale();
            float moveBy = 5;
            int scaleBy = 3;

            // Control entity movement and reshape
            switch (event.key.keysym.sym)
            {
            // Control Entity movement in the interactive demo
            // Handle left arrow key
            case SDLK_LEFT:
                currCtrlEntity->setLocation(glm::vec2(loc.x - moveBy, loc.y)); // Move left
                break;
                // Handle right arrow key
            case SDLK_RIGHT:
                currCtrlEntity->setLocation(glm::vec2(loc.x + moveBy, loc.y)); // Move right
                break;
                // Handle up arrow key
            case SDLK_UP:
                currCtrlEntity->setLocation(glm::vec2(loc.x, loc.y + moveBy)); // Move up
                break;
                // Handle down arrow key
            case SDLK_DOWN:
                currCtrlEntity->setLocation(glm::vec2(loc.x, loc.y - moveBy)); // Move down
                break;

                // Control Entity scaling in the interactive demo
            case SDLK_a: // a key is Scale up, for now
                currCtrlEntity->setScale(glm::vec2(scale.x + scaleBy, scale.y + scaleBy));
                break;

            case SDLK_z: // z key is Scale down, for now
                // Make sure not to scale into the negatives
                if (scale.x - scaleBy >= 0 && scale.y - scaleBy >= 0)
                {
                    currCtrlEntity->setScale(glm::vec2(scale.x - scaleBy, scale.y - scaleBy));
                }
                break;
            }
        }

        // Control pages switch.
        switch (event.key.keysym.sym)
        {
        // use 1 to look the previous page
        case SDLK_1:
            // have to check begin and end here
            break;
        // use 2 to look the next page
        case SDLK_2:
            break;
            /*
            // Theses are here to test switching fonts
        case SDLK_f:
            ((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicsansmsgras.ttf"));
            break;
        case SDLK_g:
            ((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicz.ttf"));
            break;
            */
        }
    }
    void Game::setCurrentPage(Page *p)
    {
        this->currPage = p;
    }

    // only render graphics so can be used in editor
    // TODO: Now we only have one currpage so there's no much different between using this render and use the renderer in page class. But in design it could render all pages in the current page list
    void Game::render()
    {
        glClearColor(0.1, 0.2, 0.59, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        if (currPage != nullptr)
        {
            // TODO: enable bg color
            //glm::vec4 pageColor = currPage->GetBackgroundColor ();
            //glClearColor ( pageColor.r, pageColor.g, pageColor.b, pageColor.a );
            glClear(GL_COLOR_BUFFER_BIT);

            currPage->render();
        }
    }

    void Game::run()
    {

        mainLoop();

        destroy();
    }

    void Game::destroy()
    {
        // Take care of deleting SDL objects and cleanly exit
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
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
                    // Handle window closing
                case SDL_QUIT:
                    close_window = true;
                    break;
                    // Handle Window events
                case SDL_WINDOWEVENT:
                    // Handle resizing the window
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        width = event.window.data1;               // Set the width to the resized width
                        height = event.window.data2;              // Set the height to the resized height
                        SDL_SetWindowSize(window, width, height); // Set the new window dimensions

                        // Set the new viewport size (this determines the size of the opengl -1 < pt < 1 coordinate system)
                        glViewport(0, 0, width, height);

                        // Preserve dimensions of objects after resize
                        // Set the scale based on the width and height of the screen
                        int scaleID = glGetUniformLocation(shaderProgram, "scale");
                        glUniform2f(scaleID, (float)2 / width, (float)2 / height);

                        SDL_GL_SwapWindow(window); // Show the resized window
                    }
                    break;
                    // Handle Keypresses
                case SDL_KEYDOWN:
                    handleInput(event);
                }
            }

            render();

            // Show the entities by bringing showing the back buffer
            SDL_GL_SwapWindow(window);

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
