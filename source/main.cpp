#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
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

      GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "InitGL", nullptr, nullptr);
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
   Shader shader = Shader("coordinates.vs", "twoTex.fs");
   if (!window) {
      return 0;
   }
   int width, height, nrChannels;
   int width2, height2, nrChannels2;
   stbi_set_flip_vertically_on_load(1);
   unsigned char *data = stbi_load("../source/textures/wall.jpg", &width, &height, &nrChannels, 0);
   unsigned char *data2 = stbi_load("../source/textures/face.png", &width2, &height2, &nrChannels2, 0);
   // std::cout << data2 << std::endl;
   // std::cout << data << std::endl;
   unsigned int texture1, texture2;
   glGenTextures(1, &texture1);
   // std::cout << "Texture0: " << texture1 << " Texture1: " << texture2 << std::endl;
   glBindTexture(GL_TEXTURE_2D, texture1);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // set texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
   glGenerateMipmap(GL_TEXTURE_2D);

   glGenTextures(1, &texture2);
   glBindTexture(GL_TEXTURE_2D, texture2);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // set texture filtering parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
   glGenerateMipmap(GL_TEXTURE_2D);
   float vertices[] = {
      //positions       //colors          //textures
     -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
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
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
   glEnableVertexAttribArray(2);

   //unbind VBO and VAO
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
   

   // Set the clear color to a nice green
   glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
   fprintf(stdout, "Status: Using GLFW %s\n", glfwGetVersionString());
   glBindVertexArray(VAO);
   shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
   shader.setInt("ourTexture1", 0);
   shader.setInt("ourTexture2", 1);
   
   //creating model, view and projection matrix
   //model matrix 
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
   //view matrix
   glm::mat4 view = glm::mat4(1.0f);
   // note that we're translating the scene in the reverse direction of where we want to move
   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); 
   //projection matrix
   glm::mat4 projection;
   projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);

            // bind textures on corresponding texture units
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      // glm::mat4 trans = glm::mat4(1.0f);
      // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
      shader.setMat4("model", glm::value_ptr(model));
      shader.setMat4("view", glm::value_ptr(view));
      shader.setMat4("projection", glm::value_ptr(projection));
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
