#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]" << "(" << error << "):" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Error compiling shader with id: " << id << " and type: " << type << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int colorWheel(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Sync to monitor refresh rate

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew" << std::endl;
        return -1;
    }

    // D�finir les positions des vertices du carr� (en 2D)
    float positions[] = {
        -0.5f, -0.5f,  // 0
         0.5f, -0.5f,  // 1
         0.5f,  0.5f,  // 2
        -0.5f,  0.5f   // 3
    };

    // D�finir les indices pour dessiner un carr� avec deux triangles
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao, vbo, ibo;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // G�n�rer et lier le VBO (Vertex Buffer Object)
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    // G�n�rer et lier le IBO (Index Buffer Object)
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    // D�finir l'attribut du vertex (position)
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    // Charger les shaders
    ShaderProgramSource source = ParseShader("src/05-Project/colorWheel/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    struct Color {
        float R;
        float G;
        float B;
        float A;
    };

    Color color = { 0.2f, 0.3f, 0.8f, 1.0f };
    // D�finir les couleurs cl�s pour la transition
    float keyColors[][4] = {
        {1.0f, 0.0f, 0.0f, 1.0f},  // Rouge
        {0.0f, 1.0f, 0.0f, 1.0f},  // Vert
        {0.0f, 0.0f, 1.0f, 1.0f}   // Bleu
    };
	int NbColors = sizeof(keyColors) / sizeof(keyColors[0]);
    int keyColorIndex = 0;
    float precision = 0.01f; // Le pas pour ajuster la transition de couleur

	float pas[4] = { 0.01f, 0.01f, 0.01f, 0.01f }; // Le pas pour ajuster la transition de couleur

    // Passer la couleur au shader
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);

    pas[0] = (keyColors[keyColorIndex][0] - color.R) * precision;
    pas[1] = (keyColors[keyColorIndex][1] - color.G) * precision;
    pas[2] = (keyColors[keyColorIndex][2] - color.B) * precision;
    pas[3] = (keyColors[keyColorIndex][3] - color.A) * precision;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Transitionner vers la couleur suivante
        color.R += pas[0];
        color.G += pas[1];
        color.B += pas[2];
        color.A += pas[3];

        // V�rifier si on a atteint la couleur cible et passer � la suivante
        if (fabs(keyColors[keyColorIndex][0] - color.R) < precision &&
            fabs(keyColors[keyColorIndex][1] - color.G) < precision &&
            fabs(keyColors[keyColorIndex][2] - color.B) < precision)
        {
            keyColorIndex = (keyColorIndex + 1) % NbColors;  // Faire un tour complet des couleurs

            pas[0] = (keyColors[keyColorIndex][0] - color.R) * precision;
            pas[1] = (keyColors[keyColorIndex][1] - color.G) * precision;
            pas[2] = (keyColors[keyColorIndex][2] - color.B) * precision;
            pas[3] = (keyColors[keyColorIndex][3] - color.A) * precision;
        }

        // Mettre � jour la couleur dans le shader
        GLCall(glUniform4f(location, color.R, color.G, color.B, color.A));

        // Lier le VAO et dessiner le carr� avec les indices
        GLCall(glBindVertexArray(vao));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        GLCall(glBindVertexArray(0));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    // Nettoyer
    glDeleteProgram(shader);
    GLCall(glDeleteBuffers(1, &vbo));
    GLCall(glDeleteBuffers(1, &ibo));
    GLCall(glDeleteVertexArrays(1, &vao));

    glfwTerminate();
    return 0;
}
