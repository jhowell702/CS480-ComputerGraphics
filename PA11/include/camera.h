#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void Update(glm::vec3 focusObject);
    std::string getFocus(){return focusObject;};
    void setFocus(std::string in_focus, glm::vec3 in_cameraLoc){
				focusObject = in_focus;
				 cameraLoc = in_cameraLoc;};
  private:
    std::string focusObject;
    glm::vec3 cameraLoc;

    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
