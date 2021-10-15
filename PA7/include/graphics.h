#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, std::string* fileNames);
    void Update(unsigned int dt);
    void Render();

    Object * LoadObjects(std::string fileName);
    void LoadFromConfig(std::string* fileNames);
    unsigned int * loadTextures(std::string texFileName);

    std::string objName;

    Camera * getCamera(){return m_camera;};
    Object * getObject(std::string name){return m_objects[name];};

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    map<std::string, Object * > m_objects;
    std::vector<Object *> l_objects;

};

#endif /* GRAPHICS_H */
