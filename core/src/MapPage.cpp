#include "MapPage.h"
#include "glm/glm.hpp"

extern int width;
extern int height;

// The fragment shader color is different, but everything else is the same as Game.cpp
void MapPage::initShaders() {
    // Source for the vertex shader
    const char* vertexSource = R"glsl(
		#version 450 core

		layout (location = 0) in vec2 pos;
		uniform vec2 scale; // This will scale our coordinates in pixels (0 < x,y < width,height) to opengl coordinates (-1 < x,y < 1)

		void main()
		{
		  gl_Position = vec4(scale.xy * pos.xy - 1, 0.0, 1.0);
		}
	)glsl";

    // Source for the fragment shader
    const char* fragmentSource = R"glsl(
		// fragment shader
		#version 450 core

		out vec4 FragColor;

		void main()
		{
		  FragColor = vec4(0.3,0.6,0.5,1.0);
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
    // 2 ints are read at a time (x, y)
    // The the type is float (GL_INT)
    // GL_FALSE means the data should not be normalized
    // The size to be read in bytes (2 * sizeof(int))
    // Offset isn't used yet since there's only one attribute in 'vertices'
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2 * sizeof(int), (void*)0);

    // Enable the position vertex attribute
    glEnableVertexAttribArray(0);

    // Enable the Vertex Object Array
    glBindVertexArray(vao);

    // Set the scale based on the width and height
    int scaleID = glGetUniformLocation(shaderProgram, "scale");
    glUniform2f(scaleID, (float)2 / width, (float)2 / height);
}

 
MapPage::MapPage(SDL_Window* window) :
Page(window) {
	mMap = new Map(glm::vec2(0, 0), 0);
	initShaders(); // New OpenGL context created, so we need to set up the shaders for this context
}

MapPage::MapPage(SDL_Window* window, Map* map) :
Page(window) {
	mMap = map;
	initShaders(); // New OpenGL context created, so we need to set up the shaders for this context
}



// Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
void MapPage::setMap(Map* map) {
	mMap = map;
}

void MapPage::render() {
	// Show the current context
	SDL_GL_MakeCurrent(mWindow, mPageContext);
	glClearColor(0.1, 0.9, 0.59, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Traverse all tiles in the Map
	for (int i = 0; i < mMap->getNumTiles(); i++) {
		// Render each tile of the map!
		int* coords = mMap->mTileArray[i].getCoords(); // Get ptr to the tile coordinates

		// Buffer and draw tile
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(int), coords, GL_DYNAMIC_DRAW); 
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	SDL_GL_SwapWindow(mWindow); // Show the buffer by bringing it to the front
}