#include "Application.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Camera.h"

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

Application::Application(Renderer& r) : renderer(r) {}

Application::~Application() {
  // ImGui Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteTextures(1, &textureID);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

int Application::init() {
  // initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return 1;  // typically couldnt create window
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__  // MacOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create a GLFW window
  window = glfwCreateWindow(900, 900, "pathtracer-cmake", NULL, NULL);
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
  glfwSetWindowUserPointer(window, this);
  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(window, key_callback);

  setupTexture();
  setupFramebuffer();

  // Setup ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

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
      -1.0f, -1.0f, 0.0f, 0.0f,  // Bottom Left
      1.0f,  -1.0f, 1.0f, 0.0f,  // Bottom Right
      1.0f,  1.0f,  1.0f, 1.0f,  // Top Right
      -1.0f, 1.0f,  0.0f, 1.0f   // Top Left
  };

  // Upload vertex data to the VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set vertex attribute pointers
  glEnableVertexAttribArray(0);  // Position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void*)0);

  glEnableVertexAttribArray(1);  // Texture coordinate attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void*)(2 * sizeof(GLfloat)));

  // Unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Application::run() {
  if (!window) {
    std::cerr << "Please initialize the application with init()." << std::endl;
    return;
  }

  // Load, compile, and link shaders (You need to implement these functions)
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, FRAG_SHADER_SOURCE);
  GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  std::cout << "Running" << std::endl;

  std::vector<glm::vec3> pixels(renderer.width * renderer.height, glm::vec3(0.0f));

  // ImGui variables
  bool continuous_render = false;
  float deltaTime = 0.0f;

  // Main loop
  while (!glfwWindowShouldClose(window)) {

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // get deltatime and update camera
    deltaTime = ImGui::GetIO().DeltaTime;
    renderer.scene->camera->update(deltaTime);

    // ImGui render button
    ImGui::Begin("Render Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (continuous_render || ImGui::Button("Render") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R))) {
      renderer.render(&pixels);
      this->updateTexture(pixels);
    }
    ImGui::Checkbox("Continuous Render", &continuous_render);

    ImGui::SeparatorText("Renderer Settings");
    ImGui::SliderInt("Samples", &renderer.samples, 1, 1000, "%d", ImGuiSliderFlags_Logarithmic);

    if(ImGui::SliderInt("Jitter", &renderer.jitter, 1, 100, "%d", ImGuiSliderFlags_Logarithmic))
      renderer.generateRays();

    if(ImGui::SliderInt("Width/Height", &renderer.width, 5, 1000)) {
      renderer.height = renderer.width;
      pixels.resize(renderer.width * renderer.height);
      renderer.generateRays();
      this->setupTexture();
      ImGui::End(); ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      continue;
    }

    if(ImGui::SliderInt("Random Map Size", (int*)&renderer.random_map_size, 1, 6480499, "%d", ImGuiSliderFlags_Logarithmic))
      renderer.generateRandoms();

    ImGui::SliderInt("Recursion Depth", &renderer.recursion_depth, 1, 64, "%d", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Light Loss", &renderer.light_loss, 0.0f, 1.0f);
    ImGui::SliderFloat("Min Distance", &renderer.min_dist, 0.00001f, 1.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Max Distance", &renderer.max_dist, 1.0f, 1000.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
    

    ImGui::SeparatorText("Stats");
    ImGui::Text("%.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("dt: %.3f (%.1f FPS)", deltaTime, 1.0f / deltaTime);
    ImGui::Text("Cam: %.3f, %.3f, %.3f", renderer.scene->camera->position.x, renderer.scene->camera->position.y, renderer.scene->camera->position.z);

    ImGui::End();

    // Clear the screen
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

    // Bind the VAO and draw the quad
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  Application* app = (Application*)glfwGetWindowUserPointer(window);

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  // movement
  if (key == GLFW_KEY_W) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.w = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.w = false;
  }
  if (key == GLFW_KEY_A) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.a = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.a = false;
  }
  if (key == GLFW_KEY_S) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.s = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.s = false;
  }
  if (key == GLFW_KEY_D) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.d = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.d = false;
  }
  if (key == GLFW_KEY_E) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.e = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.e = false;
  }
  if (key == GLFW_KEY_Q) {
    if (action == GLFW_PRESS)
      app->renderer.scene->camera->controls.q = true;
    else if (action == GLFW_RELEASE)
      app->renderer.scene->camera->controls.q = false;
  }

  app->renderer.scene->camera->needUpdate = true;
}

void Application::setupTexture() {

  glDeleteTextures(1, &textureID);

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Initialize texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer.width, renderer.height, 0, GL_RGB, GL_FLOAT,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // change to GL_LINEAR for blending (better output), set to GL_NEAREST for truth
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::updateTexture(const std::vector<glm::vec3>& pixels) {
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer.width, renderer.height, 0, GL_RGB, GL_FLOAT,
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

GLuint Application::createShaderProgram(GLuint vertexShader,
                                        GLuint fragmentShader) {
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
