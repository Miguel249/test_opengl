#include "Mesh.hpp"

Mesh::Mesh(const void *vertexData,
           const GLsizeiptr vertexSize,
           const std::vector<VertexAttribute> &vertexAttributes,
           const GLuint *indices,
           const GLsizeiptr indexSize,
           const void *instanceData,
           const GLsizeiptr instanceSize,
           const std::vector<VertexAttribute> &instanceAttributes)
    : m_VAO(0), m_VBO(0), m_EBO(0), m_instanceVBO(0) {
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, GL_STATIC_DRAW);

    for (const auto &[index, size, type, normalized, stride, offset, divisor]: vertexAttributes) {
        glVertexAttribPointer(index, size, type, normalized, stride,
                              reinterpret_cast<void *>(offset));
        glEnableVertexAttribArray(index);
        if (divisor > 0) {
            glVertexAttribDivisor(index, divisor);
        }
    }

    // EBO (opcional)
    if (indices && indexSize > 0) {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
        m_hasEBO = true;
    }

    // Instanced VBO (opcional)
    if (instanceData && instanceSize > 0 && !instanceAttributes.empty()) {
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceSize, instanceData, GL_STATIC_DRAW);

        for (const auto &[index, size, type, normalized, stride, offset, divisor]: instanceAttributes) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, size, type, normalized, stride,
                                  reinterpret_cast<void *>(offset));
            glVertexAttribDivisor(index, divisor);
        }

        m_hasInstance = true;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    if (m_hasEBO)
        glDeleteBuffers(1, &m_EBO);
    if (m_hasInstance)
        glDeleteBuffers(1, &m_instanceVBO);
}

Mesh::Mesh(Mesh &&other) noexcept
    : m_VAO(other.m_VAO),
      m_VBO(other.m_VBO),
      m_EBO(other.m_EBO),
      m_instanceVBO(other.m_instanceVBO),
      m_hasEBO(other.m_hasEBO),
      m_hasInstance(other.m_hasInstance) {
    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_EBO = 0;
    other.m_instanceVBO = 0;
    other.m_hasEBO = false;
    other.m_hasInstance = false;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
    if (this != &other) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        if (m_hasEBO)
            glDeleteBuffers(1, &m_EBO);
        if (m_hasInstance)
            glDeleteBuffers(1, &m_instanceVBO);

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_EBO = 0;
        other.m_instanceVBO = 0;
        other.m_hasEBO = false;
        other.m_hasInstance = false;
    }

    return *this;
}

void Mesh::bind() const {
    glBindVertexArray(m_VAO);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}
