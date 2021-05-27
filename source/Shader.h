#include <string>
class Shader{
    public:
        Shader(const char* vertexShaderName, const char* fragmentShaderName);
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        unsigned int ID;
    private:
};