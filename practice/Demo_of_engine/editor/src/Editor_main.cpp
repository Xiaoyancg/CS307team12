// this is the minimum demo of engine
// By Larry
// Please read README before compile

// include required libraries
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <SDL.h>
#include <glad/glad.h>
#include <SOIL.h>

// normal libraries
#include <string>
#include <streambuf>
#include <fstream>
#include <iostream>
#include <stdio.h>

// Our libraries
#include "FileManager.h";

int main(int argc, char **argv)
{

    // set up SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return -1;
    }

    // Set OPENGL Attributes to SDL.
    // See https://wiki.libsdl.org/SDL_GLattr for enum details
    // use gl 4.6 glsl 460. Although imgui has only functions naming opengl3, but for now it works. as long as it works, i'll keep 4 here.
    const char *glsl_version = "#version 460";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // opengl buffer details
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window with graphics context
    // sdl windows flags (attributes)
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // create a window for opengl. opengl can't create a window. we use sdl to create window
    SDL_Window *window =
        SDL_CreateWindow("minimum demo for imgui with sdl2 on opengl",
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // x and y pos. here we put it centered
                         800, 600,
                         window_flags // flags of sdl windows
        );
    // create a gl context for further rendering in the window(should be a opengl sdl window). and make it current
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    // set up the opengl context for rendering in to an opengl window
    SDL_GL_MakeCurrent(window, gl_context);
    // Use this function to set the swap interval for the current OpenGL context.
    // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync
    SDL_GL_SetSwapInterval(1);
    // this opengl context is just the background, we should render game in a ImGui window!
    // ... Or Not. we can fixed the ImGui window, and render context in the middle of the sdl window. This is easier.

    // Initialize Opengl loader
    // we use glad, NOT GLAD2!
    // I really don't know the difference.  the source code are same
    if (gladLoadGL() == 0)
    {
        printf("Failed to initialize OpenGL loader!\n");
        return -1;
    }

    // Setup Dear Imgui context
    IMGUI_CHECKVERSION();
    // can't find document of ImGui::CreateContext. Just use it anyway
    ImGui::CreateContext();
    // ImGuiIO: Communicate most settings and inputs/outputs to Dear ImGui using this structure.
    ImGuiIO &io = ImGui::GetIO();
    // ???? from the provided demo
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // alternative: Classic

    // Setup Platform/Renderer backends
    // these two functions are from imgui_impl_*.h it's in the backend folder in imgui-master
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // clear color, opengl use clear color to clear the context for the next drawing
    // if we don't clear the context, when you move the imgui window, it's trace will be left on the context.
    // Also, for the convenience, we use the vector class from ImGui. ImVec4.
    // clear_color is a RGBA color, Red Green Blue and alpha. read more:https://en.wikipedia.org/wiki/RGBA_color_model
    // Every color in opengl stored as vector. can be vec3 or vec4.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // pass the clear color to opengl server. for further usage
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                        0.5f, 0.5f, 0.0f, 1.0f, 1.0f};

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //    glBindVertexArray(vao);

    // array but use one first
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channels;
    unsigned char *imagedata = SOIL_load_image("1.png", &width, &height, &channels, SOIL_LOAD_AUTO);
    if (imagedata)
    {
        std::cout << channels << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(imagedata);
    const char *vscs, *fscs;
    std::string vss, fss;
    std::ifstream vsfs("vertex.vert");
    std::ifstream fsfs("fragment.frag");
    vss = std::string(std::istreambuf_iterator<char>(vsfs),
                      std::istreambuf_iterator<char>());
    fss = std::string(std::istreambuf_iterator<char>(fsfs),
                      std::istreambuf_iterator<char>());
    vsfs.close();
    fsfs.close();
    vscs = vss.c_str();
    fscs = fss.c_str();

    unsigned int vs, fs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vscs, NULL);
    glShaderSource(fs, 1, &fscs, NULL);
    glCompileShader(vs);

    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);

        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };
    glCompileShader(fs);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glUseProgram(program);

    // set texture uniform
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int texcbo; // texture color buffer obj
    glGenTextures(1, &texcbo);
    glBindTexture(GL_TEXTURE_2D, texcbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texcbo, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Main loop
    // Every opengl program or any program IDK need a main loop to keep it running.
    // In each loop, it redraw the context and re-evaluate the variables as needed.
    // Here we run loop for each time interval. However, for other case, we can run loop when event happened(key stroke)
    // As for the game loop, we call a game conditional function in each loop.
    // the controller function shouldn't be called here as in my experience. When a key is pressed, we update the data.
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // In the demo, it says we should pass all input to dear imgui based onio.WantCaptureMouse/Keyboard
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = true; // exit after the loop
            }
            // if window is closed
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            {
                done = true;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tid);
        // Rendering
        // render the ImGui windows
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        // use the clear color we passed to opengl before to clear the context
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Rendering
        // render the sdl windows
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        // use the clear color we passed to opengl before to clear the context
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Draw ImGui windows
        // Start the dear Imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // show a really simple imgui window
        ImGui::Begin("minimum demo ImGui window");
        ImGui::Text("Hello world");
        if (ImGui::Button("Close the program"))
        {
            done = true;
        }
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void *)texcbo,
            ImVec2(ImGui::GetCursorScreenPos()),
            ImVec2(ImGui::GetCursorScreenPos().x + 400,
                   ImGui::GetCursorScreenPos().y + 300),
            ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        // this command does not render that imgui window, we need to use opengl to render imgui
        ImGui::Render();

        // use opengl to render the imgui window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // what we just draw just stored in the buffer, we need to switch the display and the buffer to show what we just drawn.
        SDL_GL_SwapWindow(window);

#ifdef __TEST_EDITOR
        done = true;

        break;
#endif // __TEST_EDITOR
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}