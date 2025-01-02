#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex shader source code
// This shader transforms the position of each vertex into clip space
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader source code
// This shader sets the color of each fragment (pixel)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"\tFragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
    // Initialize GLFW
    glfwInit();

    // Configure GLFW to use OpenGL 3.3 and the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Define vertices for a triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Bottom-left vertex
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Bottom-right vertex
         0.0f,  0.5f * float(sqrt(3)) * 2 /3, 0.0f,   // Top vertex
		 -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner-left vertex
		 0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner-right vertex
		 0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Inner-bottom vertex
    };

    GLuint indices[] = {
		0, 3, 5, // Lower-left triangle
		3, 2, 4, // Lower-right triangle
		5, 4, 1  // Upper triangle
    };

    // Create a window with GLFW
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the created window the current OpenGL context
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers using GLAD
    gladLoadGL();

    // Define the viewport dimensions
    glViewport(0, 0, 800, 800);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete shader objects once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set the clear color (background color)
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    // Declare and generate a Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    // Bind the VAO (stores vertex attribute configurations)
    glBindVertexArray(VAO);

    // Bind the VBO and buffer the vertex data into it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO (optional, for safety)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO (optional, for safety)
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Swap buffers to display the cleared color
    glfwSwapBuffers(window);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Process events
        glfwPollEvents();

        // Clear the screen with the set clear color
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Bind the VAO and draw the triangle
        glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap buffers to display the rendered frame
        glfwSwapBuffers(window);
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Destroy the window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
