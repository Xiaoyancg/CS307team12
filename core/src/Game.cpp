#include <SDL.h>
#include <Page.h>
#include <Entity.h>
#include <glad/glad.h>
#include <stdio.h>
#include <vector>

SDL_Window* window;
SDL_GLContext gl_context;

std::vector<Entity> entities;

int width = 1280; // Width of the window, used in Entity.cpp
int height= 720; // Height of the window, used in Entity.cpp

// Use sdl_die when an SDL error occurs to print out the error and exit
// argument err_msg can be anything, but try to keep it related to the error
void sdl_die(const char* err_msg)
{
	printf("%s: %s\n", err_msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}


void initShaders() {
	// Source for the vertex shader
	const char* vertexSource = R"glsl(
		#version 450 core

		layout (location = 0) in vec2 pos;


		void main()
		{
		  gl_Position = vec4(pos, 0.0, 1.0);
		}
	)glsl";

	// Source for the fragment shader
	const char* fragmentSource = R"glsl(
		// fragment shader
		#version 450 core

		out vec4 FragColor;

		void main()
		{
		  FragColor = vec4(1.0,1.0,1.0,1.0);
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
	unsigned int shaderProgram = glCreateProgram();

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
	// 2 floats are read at a time (x, y)
	// The the type is float (GL_FLOAT)
	// GL_FALSE means the data should not be normalized
	// The size to be read in bytes (2 * sizeof(float))
	// Offset isn't used yet since there's only one attribute in 'vertices'
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Enable the position vertex attribute
	glEnableVertexAttribArray(0);

	// Enable the Vertex Object Array
	glBindVertexArray(vao);
}

// This takes care of initializing everything SDL needs to begin running
void init() {
	// Initialize video mode of SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sdl_die("Error initializing SDL video mode :(");
	}

	// Most of this was from the sample code, I haven't looked into it yet
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Use OpenGL 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create the flags for creating the window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	// Create a new centered game window based on window_flags (1280x720 for now)
	window = SDL_CreateWindow("Game Core", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

	// Create the Game opengl context
	gl_context = SDL_GL_CreateContext(window);

	// Load opengl? I can barely find information/documentation about glad
	gladLoadGL();

	// Create the shaders
	initShaders();

}


int render() {
	// Show the current context
	SDL_GL_MakeCurrent(window, gl_context);


	//////////////////////////////
	// RENDERING CODE GOES HERE //
	//////////////////////////////
	// Everything before the ending ///////'s is just test code and will eventually be deleted

	// Clear the buffer with a red background
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window); // Show the buffer by bringing it to the front
	SDL_Delay(1000); // Wait 1 sec before continuing

	// Clear the buffer with a dark red background
	glClearColor(0.5, 0.1, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window); // Show the buffer by bringing it to the front
	SDL_Delay(1000); // Wait 1 sec before continuing
	//////////////////////////////

	return 0;
}


int main(int argc, char* argv[]) {
	// Initialize OpenGL and necessary SDL objects
	init();

	SDL_Event event;
	int close_window = false;
	///////////////
	// Game loop //
	///////////////

	///////////////
	// ENTITY TEST
	Entity entity1(glm::vec2(0.25, 0.25), glm::vec2(64, 64), 0, 0);
	entities.push_back(entity1);
	Entity entity2(glm::vec2(0.50, 0.50), glm::vec2(64, 64), 0, 0);
	entities.push_back(entity2);
	Entity entity3(glm::vec2(0.75, 0.75), glm::vec2(64, 64), 0, 0);
	entities.push_back(entity3);
	Entity entity4(glm::vec2(-0.25, -0.25), glm::vec2(64, 64), 0, 0);
	entities.push_back(entity4);
	Entity entity5(glm::vec2(1.025, 1.025), glm::vec2(64, 64), 0, 0);
	entities.push_back(entity5);
	///////////////

	while (!close_window) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				close_window = true;
			}
		}

		
		///////////////
		// PAGE TEST
		// This is just a test to make sure page rendering is correctly set up
		Page page(window);
		page.render();
		///////////////
		
		render(); // Render the Game render function

		///////////////
		// ENTITY TEST
		// This is just a test to make sure Entity rendering is correctly set up
		glClearColor(0.3, 0.2, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		for (Entity entity : entities) {
			entity.render();
		}

		SDL_GL_SwapWindow(window); // Show the entities by bringing showing the back buffer
		///////////////

		SDL_Delay(1000); // Wait 1 sec before continuing


	}

	// Take care of deleting SDL objects and cleanly exit 
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
