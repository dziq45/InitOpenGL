#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Shader.h"

namespace {
   void errorCallback(int error, const char* description) {
      fprintf(stderr, "GLFW error %d: %s\n", error, description);
   }

   GLFWwindow* initialize() {
      int glfwInitRes = glfwInit();
      if (!glfwInitRes) {
         fprintf(stderr, "Unable to initialize GLFW\n");
         return nullptr;
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

      GLFWwindow* window = glfwCreateWindow(1280, 720, "InitGL", nullptr, nullptr);
      if (!window) {
         fprintf(stderr, "Unable to create GLFW window\n");
         glfwTerminate();
         return nullptr;
      }

      glfwMakeContextCurrent(window);

      int gladInitRes = gladLoadGL();
      if (!gladInitRes) {
         fprintf(stderr, "Unable to initialize glad\n");
         glfwDestroyWindow(window);
         glfwTerminate();
         return nullptr;
      }

      return window;
   }
}

int main(int argc, char* argv[]) {
   glfwSetErrorCallback(errorCallback);
   GLFWwindow* window = initialize();
   Shader shader = Shader("firstShader.vs", "firstShader.fs");
   if (!window) {
      return 0;
   }
   
   float vertices[] = {
     -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
     0.0f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,0.0f, 0.0f, 1.0f
   };

   //creating vertex buffer object
   unsigned int VBO;
   glGenBuffers(1, &VBO);
   
   //vertex array object
   unsigned int VAO;
   glGenVertexArrays(1, &VAO);  

   //binding
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   //we have to specyify how OPENGL should interpret vertex 
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
   glEnableVertexAttribArray(1);

   //unbind VBO and VAO
   glBindBuffer(GL_ARRAY_BUFFER, 0); 
   glBindVertexArray(0);

   // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
   

   // Set the clear color to a nice green
   glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
   fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());
   glBindVertexArray(VAO);
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);

      shader.use();
       // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}
