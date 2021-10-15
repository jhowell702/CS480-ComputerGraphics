#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(std::string * fileNames)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, fileNames))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  if(fileNames[2] == "Invalid"){
	validObject == false;
  }else{
	validObject == true;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // set counter to be 0
  counter = 0;
  simCounter = 5;
  currObject = "Sun";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Ren`derer backends
    const char* glsl_version = "#version 150";

    ImGui_ImplSDL2_InitForOpenGL(m_window->getSDLWindow(), m_window->getContext());
    ImGui_ImplOpenGL3_Init(glsl_version);

  // No errors
  return true;
}	

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

        ImGuiIO& io = ImGui::GetIO();
 
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
	    if (ImGui::IsKeyDown(41)){
		m_running = false;
	    }
            if (event.type == SDL_QUIT)
                m_running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window->getSDLWindow()))
                m_running = false;
        }

        // Update and render the graphics
	m_graphics->Update(m_DT);
	m_graphics->Render();

	//bool to see if we clicked a button this frame
	bool buttonClicked = false;

	//imgui init stuff
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	//start menu
	ImGui::Begin("Controls");

        ImGui::SetWindowPos(ImVec2(50,50), true);
	ImGui::SetWindowSize(ImVec2(300,100), true);

	

	if(ImGui::Button("<-")){
		lastObject();
	}
	ImGui::SameLine();
	ImGui::Text("Current Focus: ");
	ImGui::SameLine();
	ImGui::Text(currObject.c_str());
	ImGui::SameLine();
	if(ImGui::Button("->")){
		nextObject();
	}

	if(ImGui::Button("<- ##simdec")){
		incSimSpeed();
	}
	ImGui::SameLine();
	ImGui::Text("Simulation Speed: ");
	ImGui::SameLine();
	ImGui::Text(" %d ", simCounter);
	ImGui::SameLine();
	if(ImGui::Button("-> ##siminc")){
		decSimSpeed();
	}


    	ImGui::End();

    	ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
  }
}

void Engine::nextObject(){

	if(counter + 1 < 11){
		counter++;
	}else{
		counter = 0;
	}	
	setObject();

}

void Engine::lastObject(){

	if(counter - 1 >= 0){
		counter--;
	}else{
		counter = 10;
	}	
	setObject();

}

void Engine::incSimSpeed(){

	if(simCounter - 1 > -11){
		simCounter--;
		m_graphics->incSpinSim(100);
		m_graphics->incRotPlanSim(1000);
		m_graphics->incRotMoonSim(100);
	}
}

void Engine::decSimSpeed(){

	if(simCounter + 1 < 11){
		simCounter++;
		m_graphics->incSpinSim(-100);
		m_graphics->incRotPlanSim(-1000);
		m_graphics->incRotMoonSim(-100);
	}
}

void Engine::setObject(){


	switch(counter){

		case 0:
			m_graphics->getCamera()->setFocus("Sun", glm::vec3(0.0, 500.0, -5.0));
			currObject = "Sun";
		break;

		case 1:
			m_graphics->getCamera()->setFocus("Mercury", glm::vec3(0.0, 30.0, -16.0));
			currObject = "Mercury";
		break;
		case 2:
			m_graphics->getCamera()->setFocus("Venus", glm::vec3(0.0, 20.0, -48.0));
			currObject = "Venus";
		break;

		case 3:
			m_graphics->getCamera()->setFocus("Earth", glm::vec3(0.0, 20.0, -48.0));
			currObject = "Earth";

		break;
		case 4:
			m_graphics->getCamera()->setFocus("Mars", glm::vec3(0.0, 10.0, -48.0));
			currObject = "Mars";
		break;
		case 5:
			m_graphics->getCamera()->setFocus("Jupiter", glm::vec3(0.0, 40.0, -68.0));
			currObject = "Jupiter";
		break;
		case 6:
			m_graphics->getCamera()->setFocus("Saturn", glm::vec3(0.0, 40.0, -68.0));
			currObject = "Saturn";
		break;
		case 7:
			m_graphics->getCamera()->setFocus("Uranus", glm::vec3(0.0, 20.0, -48.0));
			currObject = "Uranus";
		break;
		case 8:
			m_graphics->getCamera()->setFocus("Neptune", glm::vec3(0.0, 20.0, -48.0));
			currObject = "Neptune";
		break;
		case 9:
			m_graphics->getCamera()->setFocus("Pluto", glm::vec3(0.0, 5.0, -20.0));
			currObject = "Pluto";
		break;
		case 10:
			m_graphics->getCamera()->setFocus("Sun", glm::vec3(0.0, 800.0, -5.0));
			currObject = "Sun Far";
		break;
	}

}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
