#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    void setDir(bool newState);
    void setSpin(bool newState);

    bool getDir();
    bool getSpin();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    glm::vec3 location;
     
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    bool dirFlag;
    bool spinFlag;

    float angle;
    float cirAngle;
};

#endif /* OBJECT_H */
