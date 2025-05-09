#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f,  // 3
        };

        // Définir les indices pour dessiner un carré avec deux triangles
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        // Générer et lier le VBO (Vertex Buffer Object)
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
		va.AddBuffer(vb, layout);

        // Générer et lier le IBO (Index Buffer Object)
        IndexBuffer ib(indices, 6);

        Shader Shader("src/07-Texture/res/shaders/Basic.shader");
        Shader.Bind();

        Texture texture("src/07-Texture/res/textures/texture.png");
        texture.Bind();
        Shader.SetUniform1i("u_Texture", 0);

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
        Shader.SetUniforms4f("u_Color", color.R, color.G, color.B, color.A);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        Shader.UnBind();

        pas[0] = (keyColors[keyColorIndex][0] - color.R) * precision;
        pas[1] = (keyColors[keyColorIndex][1] - color.G) * precision;
        pas[2] = (keyColors[keyColorIndex][2] - color.B) * precision;
        pas[3] = (keyColors[keyColorIndex][3] - color.A) * precision;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

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
            Shader.Bind();
            Shader.SetUniforms4f("u_Color", color.R, color.G, color.B, color.A);

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
    }

    glfwTerminate();
    return 0;
}
