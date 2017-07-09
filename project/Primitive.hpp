#pragma once

#include <glm/glm.hpp>

class Primitive {
public:
  virtual ~Primitive();
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius)
      : m_pos(pos), m_radius(radius)
    {}
    virtual ~NonhierSphere();

    const glm::vec3 m_pos;
    const double m_radius;
};

class NonhierBox : public Primitive {
public:
    NonhierBox(const glm::vec3& pos, double size)
      : m_pos(pos), dims(size)
    {}
    NonhierBox(const glm::vec3& pos, const glm::vec3& dims)
      : m_pos(pos), dims(dims)
    {}

    virtual ~NonhierBox();

    const glm::vec3 m_pos;
    const glm::dvec3 dims;
};

class Sphere : public NonhierSphere {
public:
    Sphere() : NonhierSphere(glm::vec3(0), 1) {}
    virtual ~Sphere();
};

class Cube : public NonhierBox {
public:
    Cube() : NonhierBox(glm::vec3(0), 1) {}
    virtual ~Cube();
};