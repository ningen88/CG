#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include "render.h"
#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "OPENGL ERROR: " << error << std::endl;
    }
}


int main(void){

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, "OGL_WINDOWS", NULL, NULL);

    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR FAILED TO INITIALIZE GLEW!!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {                   
        -0.5f, -0.5f, 0.0f, 0.0f,  //index 0     x, y, texture coord1, texture coord2
         0.5f, -0.5f, 1.0f, 0.0f,  //index 1
         0.5f,  0.5f, 1.0f, 1.0f,  //index 2
        -0.5f,  0.5f, 0.0f, 1.0f   //index 3
    };

    unsigned int indices[] = {                           //index that specify the writing order of the vertices
        0,1,2,
        2,3,0
    };

    glEnable(GL_BLEND);                                                    //enable blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                     //blending -> glBlendFunc(src, dst);

    VertexArray va;                                                        //create a VertexArray object 
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));                     //create a VertexBuffer object 
    VertexBufferLayout layout;                                             //create a VertexBufferLayout object
    layout.push<float>(2);                                                 //set buffer layout to 2 float for the position attributes
    layout.push<float>(2);                                                 //same for texture coordinate (more info in vertexBufferLayout.h and vertexArray.cpp)

    va.addBuffer(vb, layout);                                              //add the VertexBuffer vb with the layout, in the VertexArray va

    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);    //projection matrix (orthographic projection) the values are the boundary
   
    IndexBuffer ib(indices,6);                                             //create an IndexBuffer object

    Shader shader("res/shaders/basic.shader");                             //load the file and create the shader
    shader.bind();                                                         //bind the shader using the glUseProgram function and install it as part of current rendering state
    shader.setUniform4f("u_color", 0.5f, 0.1f, 0.1f, 1.0f);                //specify the value of the uniform variable, in this case the color

    shader.setUniformMat4f("u_MVP", proj);                                 //specify the value of the uniform variable, in this case the projection matrix

    Texture texture("res/textures/avatar_1.png");                          //load the image and create the texture
    texture.bind();                                                        //bind the texture using glBindTexture function and give it a texture slot (default 0)
    shader.setUniform1i("u_texture", 0);                                   //specify the value of the uniform variable, in this case the texture in slot 0
    
    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    float r = 0.0f;
    float increment = 0.05f;

    Render render;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){

        /* Render here */
  
        render.clear();
        shader.bind();
        shader.setUniform4f("u_color", r, 0.1f, 0.1f, 1.0f);     
        render.draw(va,ib,shader);                                                //draw the object based on the vertices in va, the order in ib and color in shader
 

        if (r > 1.0f) {
            increment = -0.05;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }
        
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
