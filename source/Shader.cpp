#include <glad/glad.h>
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName){
    std::string vertexPath = "../source/shaders/";
    vertexPath += vertexShaderName;
    std::string fragmentPath = "../source/shaders/";
    fragmentPath += fragmentShaderName;
    std::cout <<vertexPath<<std::endl;
    std::cout <<fragmentPath<<std::endl;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::stringstream vertexStream;
    std::stringstream fragmentStream;
    try{
        vertexFile.open(vertexPath, std::ios::in);
        fragmentFile.open(fragmentPath, std::ios::in);
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
    }
    catch (std::ifstream::failure e){
        std::cout << "SHADER NOT SUCCESSFULLY READ" << std::endl;
    }

    std::string vSource = vertexStream.str();
    std::string fSource = fragmentStream.str();
    const char* vertexSource = vSource.c_str();
    const char* fragmentSource = fSource.c_str();
    std::cout << vertexSource << std::endl;
    std::cout << fragmentSource << std::endl;
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
    glUseProgram(ID);
}