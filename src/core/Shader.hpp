#pragma once

#include <string>
#include "fwd.hpp"

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const glm::mat4 &value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;

private:
    unsigned int m_programId;

    static void checkCompileErrors(unsigned int shader, const std::string &type);
};
