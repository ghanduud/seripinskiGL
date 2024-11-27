


#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include <cmath>

using namespace std;
using namespace glm;

enum DrawingMode
{
    Points,
    Lines,
    FilledTriangle
};

struct Vertex
{
    vec3 Position;
    vec3 Color;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::FilledTriangle;
std::vector<Vertex> SierpinskiVertices;

void GenerateSierpinskiTriangle(vec3 v1, vec3 v2, vec3 v3, int depth)
{
    if (depth == 0)
    {
        // Base case: Create a black triangle
        SierpinskiVertices.push_back({ v1, vec3(0.0f, 0.0f, 0.0f) }); // Black
        SierpinskiVertices.push_back({ v2, vec3(0.0f, 0.0f, 0.0f) }); // Black
        SierpinskiVertices.push_back({ v3, vec3(0.0f, 0.0f, 0.0f) }); // Black
        return;
    }

    // Calculate midpoints
    vec3 mid1 = (v1 + v2) * 0.5f;
    vec3 mid2 = (v2 + v3) * 0.5f;
    vec3 mid3 = (v3 + v1) * 0.5f;

    // Recursively subdivide
    GenerateSierpinskiTriangle(v1, mid1, mid3, depth - 1);
    GenerateSierpinskiTriangle(mid1, v2, mid2, depth - 1);
    GenerateSierpinskiTriangle(mid3, mid2, v3, depth - 1);
}


void CreateSierpinskiTriangle(int depth)
{
    // Define the initial triangle (equilateral triangle in 2D space)
    vec3 v1 = vec3(-0.8f, -0.8f, 0.0f); // Bottom-left
    vec3 v2 = vec3(0.8f, -0.8f, 0.0f);  // Bottom-right
    vec3 v3 = vec3(0.0f, 0.8f, 0.0f);   // Top

    // Clear previous vertices
    SierpinskiVertices.clear();

    // Generate the Sierpiński Triangle vertices
    GenerateSierpinskiTriangle(v1, v2, v3, depth);

    // Create the VBO and upload data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, SierpinskiVertices.size() * sizeof(Vertex), SierpinskiVertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3)));
    glEnableVertexAttribArray(1);
}


void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error initializing GLEW\n";
        getchar();
        return 1;
    }

    cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // Compile shaders and create program
    CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);

    // Generate the Sierpiński Triangle
    CreateSierpinskiTriangle(5); // Change depth as needed

    glClearColor(0, 0.5, 0.5, 1);

    return 0;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

   
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   

    glDrawArrays(GL_TRIANGLES, 0, SierpinskiVertices.size());
}

int main()
{
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sierpinski Triangle", sf::Style::Close, context);

    if (Init()) return 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        Render();
        window.display();
    }
    return 0;
}
