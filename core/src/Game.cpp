#include <SDL.h>
#include <Page.h>
#include <glad/glad.h>
#include <stdio.h>

SDL_Window* window;
SDL_GLContext gl_context;

const char* vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = ourColor;\n"
	"}\n\0";

// Use sdl_die when an SDL error occurs to print out the error and exit
// argument err_msg can be anything, but try to keep it related to the error
void sdl_die(const char* err_msg)
{
	printf("%s: %s\n", err_msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}

// This takes care of initializing everything SDL needs to begin running
void init() {
	// Initialize video mode of SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sdl_die("Error initializing SDL video mode :(");
	}

	// This was all from the sample code, I haven't looked into it yet
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create the flags for creating the window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	// Create a new centered game window based on window_flags (1280x720 for now)
	window = SDL_CreateWindow("Game Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

	// Create the Game opengl context
	gl_context = SDL_GL_CreateContext(window);

	// Load function pointers for OpenGL
	gladLoadGL();
}

int render() {
	// Show the current context
	SDL_GL_MakeCurrent(window, gl_context);

	//////////////////////////////
	// RENDERING CODE GOES HERE //
	//////////////////////////////
	// Everything before the ending ///////'s is just test code and will eventually be deleted

	// Clear the buffer with a grey background
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(window);
	
	//////////////////////////////

	return 0;
}


int main(int argc, char* argv[]) {
	// Initialize necessary SDL objects
	init();

	// Create and compile vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for vertex shader compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
	}

	// Create and compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for fragment shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
	}

	// Link shaders into a shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}

	// Clean up no longer needed shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices array for triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// Create vertex array object and vertex buffer object
	unsigned int VAO = 0, VBO = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Set current activate VAO
	glBindVertexArray(VAO);

	// Set current active VBO and store vertices data into it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Specify interpretation of vertex buffer data and enable vertex's location attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set current activate shader program
	glUseProgram(shaderProgram);

	///////////////
	// Game loop //
	///////////////
	bool close_window = false;
	while (!close_window) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				close_window = true;
			}
		}

		float timeValue = SDL_GetTicks() / 1000.0;
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		///////////////
		// RENDER TEST
		// This loop will render whatever occurs in render()
		render();
		///////////////
	}

	// Take care of deleting SDL objects and cleanly exit 
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
