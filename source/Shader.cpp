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

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setMat4(const std::string &name, float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}