#define SDL_MAIN_HANDLED
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

unsigned int buildShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    SDL_Log("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
  }

  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    SDL_Log("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
  }

  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    SDL_Log("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

unsigned int buildVertexArray(float* vertices, int count) {
  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

int main(int argc, char **argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "A window", // window title
      0,
      0,
      SCR_WIDTH,        // width, in pixels
      SCR_HEIGHT,       // height, in pixels
      SDL_WINDOW_OPENGL // flags - see below
  );

  if (window == NULL)
  {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error: %s\n", SDL_GetError());
    return 1;
  }

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error: %s\n", glewGetErrorString(err));
    return 1;
  }

  // Check that the window was successfully created
  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!GLEW_VERSION_1_3)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error: Could not get GLEW_VERSION_1_3");
    return 1;
  }

  SDL_Log("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

  float vertices[] = {
      // first triangle
      -0.9f, -0.5f, 0.0f, // left
      -0.0f, -0.5f, 0.0f, // right
      -0.45f, 0.5f, 0.0f, // top
                          // second triangle
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f,   // top
  };

  size_t nVertices = sizeof(nVertices);
  unsigned int shaderProgram = buildShader(vertexShaderSource, fragmentShaderSource);
  unsigned int VAO = buildVertexArray(vertices, sizeof(vertices));

  SDL_bool loopShouldStop = SDL_FALSE;

  while (!loopShouldStop)
  {
    // input
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        loopShouldStop = SDL_TRUE;
        break;
      }
    }

    // update

    // begin draw
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);       
    glDrawArrays(GL_TRIANGLES, 0, nVertices); 

    // end draw
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
