#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

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

int main(void)
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
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // Définir les positions des vertices du carré (en 2D)
        float positions[] = {
            -0.5f, -0.5f,  // 0
             0.5f, -0.5f,  // 1
             0.5f,  0.5f,  // 2
            -0.5f,  0.5f   // 3
        };

        // Définir les indices pour dessiner un carré avec deux triangles
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        // Générer et lier le VBO (Vertex Buffer Object)
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
		va.AddBuffer(vb, layout);

        // Générer et lier le IBO (Index Buffer Object)
        IndexBuffer ib(indices, 6);

        // Charger les shaders
        ShaderProgramSource source = ParseShader("src/05-Project/abstract/res/shaders/Basic.shader");
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        struct Color {
            float R;
            float G;
            float B;
            float A;
        };

        Color color = { 0.2f, 0.3f, 0.8f, 1.0f };
        // Définir les couleurs clés pour la transition
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

            // Vérifier si on a atteint la couleur cible et passer à la suivante
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

            // Mettre à jour la couleur dans le shader
            GLCall(glUniform4f(location, color.R, color.G, color.B, color.A));

            // Lier le VAO et dessiner le carré avec les indices
			va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
            GLCall(glBindVertexArray(0));

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        // Nettoyer
        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}
