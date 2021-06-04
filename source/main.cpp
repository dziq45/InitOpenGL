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

//SCREEN SETTINGS
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCREEN_WIDTH/2, lastY = SCREEN_HEIGHT/2;
float fov = 45.0f;
bool firstMouse = true;
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
   void processInput(GLFWwindow* window, float deltaTime){
      const float cameraSpeed = 2.3f * deltaTime;
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
         cameraPos += cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
         cameraPos -= cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
         cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
         cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
   }

   void mouse_callback(GLFWwindow* window, double xpos, double ypos){

      if (firstMouse) // initially set to true
      {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
      }

      float xoffset = xpos - lastX;
      float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
      lastX = xpos;
      lastY = ypos;

      const float sensitivity = 0.05f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw   += xoffset;
      pitch += yoffset;

      if(pitch > 89.0f)
        pitch =  89.0f;
      if(pitch < -89.0f)
        pitch = -89.0f;

           //camera modification based on mouse
      glm::vec3 direction;
      direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      direction.y = sin(glm::radians(pitch));
      direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      cameraFront = glm::normalize(direction);

   }
   void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
      fov -= (float)yoffset;
      if (fov < 1.0f)
        fov = 1.0f;
      if (fov > 45.0f)
        fov = 45.0f; 
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
   std::cout << "siemano" << std::endl;
   unsigned int texture1, texture2;
   glGenTextures(1, &texture1);
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
     -0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

     -0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f,-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

      0.0f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
     -0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

      0.0f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
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
   
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback); 
   //creating model, view and projection matrix


   // //view matrix
   // glm::mat4 view = glm::mat4(1.0f);
   // // note that we're translating the scene in the reverse direction of where we want to move
   // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); 
   //projection matrix
   glm::mat4 projection;

   float deltaTime = 0.0f;	// Time between current frame and last frame
   float lastFrame = 0.0f; // Time of last frame

   glEnable(GL_DEPTH_TEST);
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // bind textures on corresponding texture units
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
      // glm::mat4 trans = glm::mat4(1.0f);
      // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
         //model matrix 
      glm::mat4 model = glm::mat4(1.0f);
      // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.5f, 1.0f, 0.0f)); 

      glm::mat4 view;
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);    
      shader.setMat4("model", glm::value_ptr(model));
      shader.setMat4("view", glm::value_ptr(view));
      shader.setMat4("projection", glm::value_ptr(projection));
      shader.use();
       // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
      glDrawArrays(GL_TRIANGLES, 0, 12);
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;  
      processInput(window, deltaTime);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}
