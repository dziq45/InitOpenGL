class Shader{
    public:
        Shader(const char* vertexShaderName, const char* fragmentShaderName);
        void use();
    private:
        unsigned int ID;
};