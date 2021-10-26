#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(std::string* fileNames);
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

    void incSimSpeed();
    void decSimSpeed();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    std::string vertShadName;
    std::string fragShadName;
    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    bool validObject;

    btBroadphaseInterface *broadphase;
    btDiscreteDynamicsWorld *dynamicsWorld;
    btSequentialImpulseConstraintSolver *solver;
    btCollisionDispatcher *dispatcher;	
    btDefaultCollisionConfiguration *collisionConfiguration;

    int counter;
    std::string currObject;

    int simCounter;
};

#endif // ENGINE_H
