#include "Application.h"

std::string VERTEX_SHADER_SOURCE = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

std::string FRAG_SHADER_SOURCE = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}
)";

Application::Application(int w, int h) : width(w), height(h) {}

Application::~Application() {
  glDeleteTextures(1, &textureID);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

int Application::init() {
  // initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return 1; // typically couldnt create window
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__  // MacOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create a GLFW window
  window = glfwCreateWindow(width, height, "pathtracer-cmake", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 2;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  std::cout << "Initalized GLFW" << std::endl;

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    printf("Failed to initialize OpenGL context\n");
    return 3;
  }

  std::cout << "Initalized GLAD" << std::endl;

  // Set callbacks
  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(window, key_callback);

  setupTexture();
  setupFramebuffer();

  return 0;
}

void Application::setupFramebuffer() {
  // Generate and bind a VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Generate and bind a VBO
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  // Define the vertex data
  GLfloat vertices[] = {
      // Positions    // Texture Coords
      -1.0f, -1.0f,     0.0f, 0.0f, // Bottom Left
      1.0f, -1.0f,    1.0f, 0.0f, // Bottom Right
      1.0f, 1.0f,  1.0f, 1.0f, // Top Right
      -1.0f, 1.0f,   0.0f, 1.0f  // Top Left
  };
  
  // Upload vertex data to the VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  // Set vertex attribute pointers
  glEnableVertexAttribArray(0); // Position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  
  glEnableVertexAttribArray(1); // Texture coordinate attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  
  // Unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0);
}

void Application::run(Renderer& renderer) {
  if (!window) {
    std::cerr << "Please initialize the application with init()." << std::endl;
    return;
  }

  // Load, compile, and link shaders (You need to implement these functions)
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, FRAG_SHADER_SOURCE);
  GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  std::cout << "Running" << std::endl;

  std::vector<glm::vec3> pixels(width * height, glm::vec3(0.0f));
  // Main loop
  while (!glfwWindowShouldClose(window)) {

    // Update the texture with new data
    renderer.render(&pixels);
    
    this->updateTexture(pixels);

    // Clear the screen
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

    // Bind the VAO and draw the quad
    glBindVertexArray(vao);
    glDisable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    // Swap buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Application::setupTexture() {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Initialize texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::updateTexture(const std::vector<glm::vec3>& pixels) {
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               pixels.data());
}

GLuint Application::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error compiling shader: " << infoLog << std::endl;
        return 0;
    }

    return shader;
}

GLuint Application::createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        return 0;
    }

    // Shaders can be detached and deleted after linking
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
