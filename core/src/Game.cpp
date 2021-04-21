#pragma once
#include <Game.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "Camera.h"

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

static GLfloat clear_color[4];

namespace Core
{
    int Game::width = 1280;
    int Game::height = 720;

    Game::Game(std::string gameName) : gameName(gameName)
    {
        initialize();
        setCurrentPage(addPage(new Page("emptyPage")));
    }

    Game::Game(nlohmann::json &json)
    {
        initialize();
        parse(json);
    }

    Game::Game(const Game &other) : gameName(other.gameName),
                                    author(other.author),
                                    version(other.version),
                                    lMTime(other.lMTime),
                                    note(other.note),
                                    _logicManager(other._logicManager),
                                    mCamera(new Camera(*other.mCamera)),
                                    currPageIdx(other.currPageIdx),
                                    pageList(other.pageList.size()),
                                    mapList(other.mapList.size()),
                                    inDisplayList(other.inDisplayList),
                                    window(other.window),
                                    gl_context(other.gl_context),
                                    keyboardState(other.keyboardState),
                                    shaderProgram(other.shaderProgram),
                                    mGameSprites(other.mGameSprites)
    {
        // TODO: restore current page, map, camera, and ctrl entity
        for (int i = 0; i < other.pageList.size(); i++)
        {
            pageList[i] = new Page(*other.pageList[i]);
            pageList[i]->setGame(this);
        }
        for (int i = 0; i < other.mapList.size(); i++)
        {
            mapList[i] = new Map(*other.mapList[i]);
        }
    }

    Game::~Game()
    {
        for (auto page : pageList)
        {
            delete page;
        }
        delete mCamera;
    }

    void Game::initialize()
    {
        Entity::mGameSprites = &mGameSprites;
        MapPage::mGameSprites = &mGameSprites;

        _logicManager = LogicManager(&pageList);
        // Create the shaders
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        {
            initContext();
        }
        mCamera = new Camera();
        initShader();
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
        p->setGame(this);
        this->pageList.push_back(p);
        return p;
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
                if (ptr - pageList.begin() == currPageIdx)
                {
                    currPageIdx = -1;
                }
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
                if (ptr - pageList.begin() == currPageIdx)
                {
                    currPageIdx = -1;
                }
                Page *p = *ptr;
                pageList.erase(ptr);
                delete (p);
                p = nullptr;
                break;
            }
        }
    }

    int Game::addMap(Map *map)
    {
        mapList.push_back(map);
        return mapList.size() - 1;
    }

    Map *Game::getMap(int idx)
    {
        return idx >= 0 && idx < mapList.size() ? mapList[idx] : nullptr;
    }

    void Game::deleteMap(std::string name)
    {
        for (auto iter = mapList.begin(); iter != mapList.end(); iter++)
        {
            if ((*iter)->getName() == name)
            {
                for (auto page : pageList)
                {
                    Core::MapPage *mapPage = dynamic_cast<Core::MapPage *>(page);
                    if (mapPage != nullptr && mapPage->getMapIndex() == iter - mapList.begin())
                    {
                        mapPage->setMapIndex(-1);
                    }
                }
                mapList.erase(iter);
                return;
            }
        }
    }

    void Game::deleteMap(Map *map)
    {
        for (auto iter = mapList.begin(); iter != mapList.end(); iter++)
        {
            if ((*iter) == map)
            {
                for (auto page : pageList)
                {
                    Core::MapPage *mapPage = dynamic_cast<Core::MapPage *>(page);
                    if (mapPage != nullptr && mapPage->getMapIndex() == iter - mapList.begin())
                    {
                        mapPage->setMapIndex(-1);
                    }
                }
                mapList.erase(iter);
                return;
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

    unsigned int Game::createPartialSprite(std::string name, int spriteID, int spritesheet, glm::ivec2 location, glm::ivec2 dimensions)
    {
        // Return OpenGL ID of the new sprite
        return mGameSprites.createPartialSprite(name, spriteID, mGameSprites.atSheetID(spritesheet), location, dimensions);
    }
    unsigned int Game::createLoopingSprite(std::string name, int spriteID, int spritesheet, int numImages, float speed, glm::ivec2 loc, glm::ivec2 dims, int xpad)
    {
        return mGameSprites.createLoopingSprite(name, spriteID, mGameSprites.atSheetID(spritesheet), numImages, speed, loc, dims, xpad);
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

    // SpriteSheet stuff
    unsigned int Game::createSpriteSheet(std::string name, std::string filename)
    {
        // Return OpenGL ID of the new sprite
        return mGameSprites.createSpriteSheet(name, filename);
    }
    unsigned int Game::createSpriteSheet(std::string name, std::string filename, int id)
    {
        return mGameSprites.createSpriteSheet(name, filename, id);
    }

    void Game::deleteSpriteSheet(int id)
    {
        mGameSprites.deleteSpriteSheet(id);
    }

    SpriteSheet *Game::getSpriteSheetFromID(int id)
    {
        return mGameSprites.atSheetID(id);
    }

    std::unordered_map<int, SpriteSheet *> Game::getSpriteSheets()
    {
        return mGameSprites.getSpriteSheets();
    }

    void Game::renderSpriteSheet(SpriteSheet *spritesheet)
    {
        if (spritesheet->getOpenGLTextureID() >= 0)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            // Create camera and set it to fit the spritesheet perfectly
            Camera *camera = new Camera();
            glm::vec2 dims = spritesheet->getDimensions();
            camera->setDimensions(dims.x, dims.y);
            camera->offsetPosition(glm::vec2(dims.x / 2, dims.y / 2));

            // Set the camera in the shader
            camera->use();

            glBindTexture(GL_TEXTURE_2D, spritesheet->getOpenGLTextureID());

            float verts[16];
            // P1
            verts[0] = 0;      // Top left x
            verts[1] = dims.y; // Top left y
            verts[2] = 0;
            verts[3] = 1;

            // P2
            verts[4] = 0; // Bottom left x
            verts[5] = 0; // Bottom left y
            verts[6] = 0;
            verts[7] = 0;

            // P3
            verts[8] = dims.x; // Top right x
            verts[9] = dims.y; // Top right y
            verts[10] = 1;
            verts[11] = 1;

            // P4
            verts[12] = dims.x; // Bottom right x
            verts[13] = 0;      // Bottom right y
            verts[14] = 1;
            verts[15] = 0;

            glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), verts, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // Unbind the current sprite
            glBindTexture(GL_TEXTURE_2D, 0);

            // Set line color
            GLfloat color[] = {1.0f, 0.0f, 0.0f};
            glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, color);
        }
    }

    //* -------------------- LOGIC WRAPPER ------------------- *//

    Signal *Game::createSignal()
    {
        return _logicManager.createSignal();
    }

    Script *Game::createScript(std::string name)
    {
        return _logicManager.createScript(name);
    }

    Logic *Game::createLogic()
    {
        return _logicManager.createLogic();
    }

    void Game::deleteSignal(int signalId)
    {
        _logicManager.deleteSignal(signalId);
    }
    void Game::deleteLogic(int logicId)
    {
        _logicManager.deleteLogic(logicId);
    }

    void Game::deleteScript(int scriptId)
    {
        _logicManager.deleteScript(scriptId);
    }

    std::vector<Script> *Game::getScriptsList()
    {
        return _logicManager.getScriptList();
    }

    std::vector<Logic> *Game::getLogicList()
    {
        return _logicManager.getLogicList();
    }
    std::vector<Signal> *Game::getSignalList()
    {
        return _logicManager.getSignalList();
    }
    using json = nlohmann::json;

    //* -------------------- JSON PARSING ------------------- *//

    Game *Game::parse(nlohmann::json &root)
    {
        this->setGameName(root.at(std::string("GameName")).get<std::string>());
        this->setAuthor(root.at("Author").get<std::string>());
        this->setVersion(root.at("Version").get<std::string>());
        this->setLMTime(root.at("LastModifiedTime").get<std::string>());
        this->setNote(root.at("Note").get<std::string>());
        try
        {
            auto pageVec = root.at("pageList").get<std::vector<json>>();
            for (auto &pageJson : pageVec)
            {
                Page *page = Page::fromJSON(pageJson);
                page->setGame(this);
                pageList.push_back(page);
            }
            setCurrentPage(0);
        }
        catch (const std::exception &e)
        {
            std::cerr << "error: " << e.what() << std::endl;
        }
        try
        {
            auto mapVec = root.at("maps").get<std::vector<json>>();
            for (json &mapJson : mapVec)
            {
                Map *map = Map::fromJSON(mapJson);
                mapList.push_back(map);
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
        setCurrentPage(0);

        return this;
    }

    json Game::serialize()
    {
        json j;
        j["FileType"] = "Parchment Game Data";
        j["GameName"] = getGameName();
        j["Author"] = getAuthor();
        j["Version"] = getVersion();
        j["LastModifiedTime"] = getLMTime();
        j["Note"] = getNote();
        // pages
        std::vector<json> pageVector;
        for (Page *p : pageList)
        {
            pageVector.push_back(p->serialize());
        }
        j["pageList"] = pageVector;

        // Sprites
        std::vector<json> spriteVector = mGameSprites.serialize();
        j["spriteList"] = spriteVector;

        json logicManagerjs;
        std::vector<json> signalVector;
        for (auto s : *(_logicManager.getSignalList()))
        {
            nlohmann::json sj;
            sj["SignalName"] = s.getSignalName();
            sj["signalId"] = s.getSignalId();
            sj["SignalType"] = s.getSignalTypeString();
            nlohmann::json si;
            switch (s.getSignalType())
            {
            case SignalType::Custom:
                si["targetLogicList"] = s.getSignal().customSignal.getTargetLogicList();
                break;
                // key signal is system signal, will not be stored

            default:
                break;
            }
            sj["signal"] = si;
            signalVector.push_back(sj);
        }
        logicManagerjs["signalList"] = signalVector;

        std::vector<json> logicVector;
        for (auto l : *(_logicManager.getLogicList()))
        {
            nlohmann::json lj;
            lj["LogicName"] = l.getLogicName();
            lj["logicId"] = l.getLogicId();
            lj["SignalType"] = l.getSignalTypeString();
            lj["ScriptType"] = l.getScriptTypeString();
            lj["targetScriptList"] = l.getTargetScriptList();
            nlohmann::json loi;
            switch (l.getSignalType())
            {
            case SignalType::Custom:
                break;
            case SignalType::Key:
                loi["key"] = (Sint32)(l.getLogic().keyLogic.getKey());
                loi["KeyType"] = (l.getLogic().keyLogic.getKeyType() == SDL_PRESSED) ? std::string("Press") : std::string("Release");
            default:
                break;
            }
            lj["logic"] = loi;
            logicVector.push_back(lj);
        }

        logicManagerjs["logicList"] = logicVector;
        std::vector<json> scriptVector;
        for (auto s : *(_logicManager.getScriptList()))
        {
            nlohmann::json scj;
            scj["ScriptName"] = s.getScriptName();
            scj["scriptId"] = s.getScriptId();
            scj["ScriptType"] = s.getScriptTypeString();
            nlohmann::json sci;
            switch (s.getScriptType())
            {
            case ScriptType::Custom:
                sci["addTargetSignalList"] =
                    s.getScript().scriptCustom.getAddTargetSignalList();
                sci["addTargetLogicList"] =
                    s.getScript().scriptCustom.getAddTargetLogicList();
                sci["addTargetScriptList"] =
                    s.getScript().scriptCustom.getAddTargetScriptList();
                sci["removeTargetSignalList"] =
                    s.getScript().scriptCustom.getRemoveTargetSignalList();
                sci["removeTargetLogicList"] =
                    s.getScript().scriptCustom.getRemoveTargetLogicList();
                sci["removeTargetScriptList"] =
                    s.getScript().scriptCustom.getRemoveTargetScriptList();
                break;
            case ScriptType::MoveConstantly:
                sci["targetPageId"] = s.getScript().scriptMoveConstantly.getTargetPageId();
                sci["targetEntityList"] = s.getScript().scriptMoveConstantly.getTargetEntityList();
                sci["movement"] = {s.getScript().scriptMoveConstantly.getMovement().x,
                                   s.getScript().scriptMoveConstantly.getMovement().y};
            default:
                break;
            }
            scj["script"] = sci;
            scriptVector.push_back(scj);
        }
        logicManagerjs["scriptList"] = scriptVector;
        j["logicManager"] = logicManagerjs;
        // TODO start point
        json startPoint;
        // TODO
        // j["startPoint"] = startPoint;
        return j;
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

            uniform mat4 camera;

            out vec2 TexCoord;

		    void main()
		    {
                gl_Position = camera * vec4(pos.xy, 0.0, 1.0);

                TexCoord = vec2(textureCoords.x,  1-textureCoords.y);
		    }
	    )glsl";

        // Source for the fragment shader
        const char *fragmentSource = R"glsl(
		    #version 330 core

            in vec2 TexCoord;

            uniform sampler2D texture1;
            uniform vec3 color ;

		    out vec4 FragColor;

		    void main()
		    {
		      FragColor = texture(texture1, TexCoord) * vec4(color, 1.0);
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
        // The the type is int (GL_INT)
        // GL_FALSE means the data should not be normalized
        // Spread between each set of attributes (4 * sizeof(int))
        //// Offset isn't used yet since there's only one attribute in 'vertices'

        // attribute ptr for position coords
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        // attribute ptr for texture coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

        // Enable the vertex attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // Enable textures (we use GL_TEXTURE0, we don't need more than 1 at a time)
        glActiveTexture(GL_TEXTURE0);

        // Enable the Vertex Object Array
        glBindVertexArray(vao);

        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // Set texture uniform

        // Make the default texture white. This allows any non-textured objects using the default texture to be rendered in color with the color uniform
        glBindTexture(GL_TEXTURE_2D, 0);
        GLubyte texData[] = {255, 255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
        return currPageIdx != -1 ? pageList[currPageIdx] : nullptr;
    }

    void Game::handleWindowEvent(SDL_Event event)
    {
        glm::vec2 loc;
        glm::vec2 scale;
        if (getCurrPage()->getCtrlEntity() != nullptr)
        {
            auto currCtrlEntity = getCurrPage()->getCtrlEntity();
            loc = currCtrlEntity->getLocation();
            scale = currCtrlEntity->getScale();
            int scaleBy = 3;

            // Control entity movement and reshape
            switch (event.key.keysym.sym)
            {
                // Control Entity scaling in the interactive demo
            case SDLK_z: // z key is Scale up, for now
                currCtrlEntity->setScale(glm::vec2(scale.x + scaleBy, scale.y + scaleBy));
                break;

            case SDLK_x: // x key is Scale down, for now
                // Make sure not to scale into the negatives
                if (scale.x - scaleBy >= 0 && scale.y - scaleBy >= 0)
                {
                    currCtrlEntity->setScale(glm::vec2(scale.x - scaleBy, scale.y - scaleBy));
                }
                break;
            }
        }

        int move_camera = 10;

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

        // Theses are here for test purposes
        case SDLK_a:
            //((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicsansmsgras.ttf"));
            mCamera->offsetPosition(glm::vec2(move_camera, 0));
            break;
        case SDLK_d:
            //((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicz.ttf"));
            mCamera->offsetPosition(glm::vec2(-move_camera, 0));
            break;
        case SDLK_w:
            //((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicsansmsgras.ttf"));
            mCamera->offsetPosition(glm::vec2(0, -move_camera));
            break;
        case SDLK_s:
            //((MenuPage*)getCurrPage())->getMenu()->setFont(new Font("../../../../resource/comicz.ttf"));
            mCamera->offsetPosition(glm::vec2(0, move_camera));
            break;
        case SDLK_q:
            mCamera->setZoom(mCamera->getZoom() - 0.1f);
            break;
        case SDLK_e:
            mCamera->setZoom(mCamera->getZoom() + 0.1f);
            break;
        }
    }
    void Game::setCurrentPage(Page *p)
    {
        for (int i = 0; i < pageList.size(); i++)
        {
            if (p == pageList[i])
            {
                currPageIdx = i;
                break;
            }
        }
    }
    void Game::setCurrentPage(int idx)
    {
        currPageIdx = (idx >= 0 && idx < pageList.size()) ? idx : -1;
    }

    bool Game::isKeyPressed(SDL_Keycode kc)
    {
        return keyboardState[SDL_GetScancodeFromKey(kc)] == 1;
    }

    void Game::update(float dt)
    {
        keyboardState = SDL_GetKeyboardState(nullptr);
        getCurrPage()->update(dt);
    }

    // only render graphics so can be used in editor
    // TODO: Now we only have one currpage so there's no much different between using this render and use the renderer in page class. But in design it could render all pages in the current page list
    void Game::render()
    {
        // Set Camera matrix uniform
        mCamera->use();

        GLfloat color[] = {1.0f, 1.0f, 1.0f};
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, color);

        glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        if (getCurrPage() != nullptr)
        {
            // TODO: enable bg color
            //glm::vec4 pageColor = currPage->GetBackgroundColor ();
            //glClearColor ( pageColor.r, pageColor.g, pageColor.b, pageColor.a );
            glClear(GL_COLOR_BUFFER_BIT);

            getCurrPage()->render();
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
        //auto lastTime = std::chrono::steady_clock::now();
        SDL_Event event;
        bool close_window = false;
        while (!close_window)
        {
            if (coreTimer.getPassedTimeFromStampInSec() < deltaCoreTime)
            {
                continue;
            }
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
                        SDL_GL_SwapWindow(window); // Show the resized window
                    }
                    break;
                case SDL_KEYDOWN:
                    // Handle Keypresses
                    _logicManager.sendSignal(Signal(-100, SignalType::Key, "SDL_KEY",
                                                    SignalUnion(SignalKey(event.key))));
                    //handleWindowEvent(event);
                    // TODO: Mouse
                }
            }
            _logicManager.checkCurrSignalList();
            _logicManager.runCurrScriptList();
            coreTimer.stamp();
            // auto now = std::chrono::steady_clock::now();
            //float dt = (float)(now - lastTime).count() / std::chrono::steady_clock::period::den;
            //lastTime = now;
            //update(dt);
            if (FPSTimer.getPassedTimeFromStampInSec() < deltaFPSTime)
            {
                continue;
            }
            render();

            // Show the entities by bringing showing the back buffer
            SDL_GL_SwapWindow(window);
            FPSTimer.stamp();
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
