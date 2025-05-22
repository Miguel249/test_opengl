#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexAttribute {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    size_t offset;
    GLuint divisor;
};

class Mesh {
public:
    Mesh(
        const void *vertexData,
        GLsizeiptr vertexSize,
        const std::vector<VertexAttribute> &vertexAttributes,
        const GLuint *indices = nullptr,
        GLsizeiptr indexSize = 0,
        const void *instanceData = nullptr,
        GLsizeiptr instanceSize = 0,
        const std::vector<VertexAttribute> &instanceAttributes = {});

    ~Mesh();

    void bind() const;

    static void unbind();

    [[nodiscard]] GLuint getVAO() const { return m_VAO; };

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_instanceVBO;

    bool m_hasEBO = false;
    bool m_hasInstance = false;
};
