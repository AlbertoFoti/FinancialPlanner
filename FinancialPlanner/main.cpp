#include "Settings.hpp"
#include "FinancialPlanner.hpp"

// OpenGL glad, GLFW  (Renderer)
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int glfwSetup(GLFWwindow* &window);

/**
 * @brief Main : Entry point for program control (update and render loop)
 * Prepares window and other graphical components through some rendering API (ex. glfw/opengl3/glad, Vulkan, DirectX)
 *
 * @return int 
 */
int main()
{
    GLFWwindow* window;

    glfwSetup(window);

	// Financial Planner Instantiation
	FinancialPlanner myFinancialPlanner;
    myFinancialPlanner.Init(window, GLSL_VERSION);

	// Financial Planner core (Update(), Render(), Shutdown())
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		myFinancialPlanner.NewFrame();
		myFinancialPlanner.Update();
		myFinancialPlanner.Render();
		glfwSwapBuffers(window);
	}
	myFinancialPlanner.Shutdown();

	return 0;
}

int glfwSetup(GLFWwindow* &window) {
    // Setup Window
    if (!glfwInit())
        throw("Unable to initialize OpenGL");

    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (window == NULL) {
        throw("Unable to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw("Unable to context to OpenGL");

    // Screen setup
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);
}