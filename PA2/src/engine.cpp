
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

bool Engine::Initialize(std::string * shadNames)
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
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT,shadNames))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();


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

	//set menu pos
        ImGui::SetWindowPos(ImVec2(113,15), true);
	ImGui::SetWindowSize(ImVec2(575,200), true);

	//menu controls
	ImGui::Text("Keyboard Controls:");          
	ImGui::Text("A Button/Mouse Buttons: Reverse direction and spin of cube");
	ImGui::Text("Left/Right Arrow Keys : Change rotation direction");
	ImGui::Text("Up/Down Arrow Keys:   : Change rotation and spin speed");

	//display current rotation direction
	ImGui::Text("Currently Rotating: ");
	if(m_graphics->GetPlanet()->getDir()){
		ImGui::SameLine();
		ImGui::Text("Clockwise        ");
	}else{
		ImGui::SameLine();
		ImGui::Text("Counterclockwise ");
	}

	//on the same line show rotation speed with two buttons to increment and decrement
	ImGui::SameLine();
	ImGui::Text("Rotation Speed: ");
	ImGui::SameLine();
	if(ImGui::Button(" - ##Rot")){
		buttonClicked = true;
		m_graphics->GetPlanet()->IncrementRotationSpeed(100);
	}

	//we are changing what the angle is by dividing, so divide here by 100000 for user readability
	ImGui::SameLine();
	ImGui::Text(" %d ", 100000/(m_graphics->GetPlanet()->GetRotationSpeed()));

	ImGui::SameLine();
	if(ImGui::Button(" + ##Rot")){
		buttonClicked = true;
		m_graphics->GetPlanet()->IncrementRotationSpeed(-100);
	}


	//display current spinning direction
	ImGui::Text("Currently Spinning: ");
	if(m_graphics->GetPlanet()->getSpin()){
		ImGui::SameLine();
		ImGui::Text("Clockwise        ");
	}else{
		ImGui::SameLine();
		ImGui::Text("Counterclockwise ");
	}

	//show spin speed with two buttons to increment and decrement spin speed
	ImGui::SameLine();
	ImGui::Text("Spin Speed:     ");
	ImGui::SameLine();
	if(ImGui::Button(" - ##Spin")){
		buttonClicked = true;
		m_graphics->GetPlanet()->IncrementSpinSpeed(100);
	}

	//we are changing what the angle is by dividing, so divide here by 100000 for user readability
	ImGui::SameLine();
	ImGui::Text(" %d ", 100000/(m_graphics->GetPlanet()->GetSpinSpeed()));

	ImGui::SameLine();
	if(ImGui::Button(" + ##Spin")){
		buttonClicked = true;
		m_graphics->GetPlanet()->IncrementSpinSpeed(-100);
	}

	//buttons to flip, get current direction flag, and reverse it
	ImGui::Text("Click the buttons below to reverse them individually:");

	if(ImGui::Button("Reverse Rotation")){
		buttonClicked = true;
		m_graphics->GetPlanet()->setDir(!m_graphics->GetPlanet()->getDir());
	}
	ImGui::SameLine();
	if(ImGui::Button("Reverse Spin")){
		buttonClicked = true;
		m_graphics->GetPlanet()->setSpin(!m_graphics->GetPlanet()->getSpin());
	}


	//if any mouse button is down, flip both rotation and spin direction
	//but only if a button has not already been clicked to avoid flipping twice

	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++){
		if(ImGui::IsMouseReleased(i) && buttonClicked == false){
			m_graphics->GetPlanet()->setDir(!m_graphics->GetPlanet()->getDir());
			m_graphics->GetPlanet()->setSpin(!m_graphics->GetPlanet()->getSpin());
		}
	}

	//flip both rotation and spin direction if the A key is pressed

	if(ImGui::IsKeyReleased(4)){
		m_graphics->GetPlanet()->setDir(!m_graphics->GetPlanet()->getDir());
		m_graphics->GetPlanet()->setSpin(!m_graphics->GetPlanet()->getSpin());
	}

	//if right arrow, set direction to be clockwise
	if(ImGui::IsKeyReleased(79)){
		m_graphics->GetPlanet()->setDir(1);
	}

	//if left arrow, set direction to be counterclockwise
	if(ImGui::IsKeyReleased(80)){
		m_graphics->GetPlanet()->setDir(0);	
	}

	//if up arrow is pressed, increment rotation speed
	if(ImGui::IsKeyReleased(81)){
		m_graphics->GetPlanet()->IncrementRotationSpeed(100);
	}

	//if up arrow is pressed, decrement rotation speed
	if(ImGui::IsKeyReleased(82)){
		m_graphics->GetPlanet()->IncrementRotationSpeed(-100);
	}

	//end imgui window and render to buffer as well

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
