#include "Application.h"

Application::Application(int w, int h) : width(w), height(h) {
    init();
    setupTexture();
    setupFramebuffer();
}

Application::~Application() {
    glDeleteTextures(1, &textureID);
}

void Application::init() {
    // initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
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
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    std::cout << "Initalized GLFW" << std::endl;

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return;
    }

    std::cout << "Initalized GLAD" << std::endl;

    // Set callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
}

void Application::run(Renderer& renderer) {

    std::cout << "Running" << std::endl;
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update the texture with new data
        //std::vector<glm::vec3> pixels(width * height, glm::vec3(0.0f, 1.0f, 0.0f));
        std::vector<glm::vec3> pixels(width * height, glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.render(pixels);
        this->updateTexture(pixels);

        // OpenGL
        glViewport(0, 0, width, height);
    
        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        this->renderTexture();
    
        // Swap front and back buffers
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::renderTexture() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // if not already bound
    glBlitFramebuffer(0, 0, width, height, 0, height, width, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Application::updateTexture(const std::vector<glm::vec3>& pixels) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels.data());
}

void Application::setupFramebuffer() {
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
