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

////////////////////////////////////////////////////////////////

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, 
								collisionConfiguration); 

	dynamicsWorld->setGravity(btVector3(0, 0, 0));



////////////////////////////////////////////////////////////////

  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, fileNames, dynamicsWorld))
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
  gameState = true;

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
	dynamicsWorld->stepSimulation(m_DT, 10);

	m_graphics->Update(m_DT);
	m_graphics->Render();

	//bool to see if we clicked a button this frame
	bool buttonClicked = false;

	//imgui init stuff
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	if(m_graphics->getObject("Cube")->lives <= 0){
		gameState = false;
	}

	if(gameState){

	//start menu
	ImGui::Begin("Controls");

        ImGui::SetWindowPos(ImVec2(50,50), true);
	ImGui::SetWindowSize(ImVec2(350,300), true);

	ImGui::Text("Lives Remaining: ");
	ImGui::SameLine();
	ImGui::Text("%d", m_graphics->getObject("Cube")->lives);

	ImGui::Text("Score: ");
	ImGui::SameLine();
	ImGui::Text("%d", m_graphics->getObject("Cube")->score);

	ImGui::Text("Controls:");
	ImGui::Text("Q - Left Paddle");
	ImGui::Text("E - Right Paddle");
	ImGui::Text("Space - Starter or:");


	ImGui::Text("Lighting:");

	if(!m_graphics->shaderSwitch){
		ImGui::Text("Shader: Vertex   |");
	}else{
		ImGui::Text("Shader: Fragment |");
	}
	ImGui::SameLine();
	if(ImGui::Button("Switch Shaders")){
		if(m_graphics->shaderSwitch != 0){
			m_graphics->shaderSwitch = 0;
		}else{
			m_graphics->shaderSwitch = 1;
		}
		m_graphics->reverseLights();
		m_graphics->SetUniforms();
	}


	ImGui::Text("Ambient");
	ImGui::SameLine();
	if(ImGui::Button("-##AmbientMinus")){
		m_graphics->decreaseAmbient();
	}
	ImGui::SameLine();
	if(ImGui::Button("+##AmbientPlus")){
		m_graphics->increaseAmbient();
	}



	ImGui::Text("Specular");
	ImGui::SameLine();
	if(ImGui::Button("-##SpecularPlus")){
		m_graphics->decreaseSpecular();
	}
	ImGui::SameLine();
	if(ImGui::Button("+##SpecularPlus")){
		m_graphics->increaseSpecular();
	}


	ImGui::Text("Red Light : ");
	ImGui::SameLine();
	if(ImGui::Button("On##RedOn")){
		m_graphics->light1on();
	}
	ImGui::SameLine();
	if(ImGui::Button("Off##RedOff")){
		m_graphics->light1off();
	}

	ImGui::Text("Blue Light: ");
	ImGui::SameLine();
	if(ImGui::Button("On##BlueOn")){
		m_graphics->light2on();
	}
	ImGui::SameLine();
	if(ImGui::Button("Off##BlueOff")){
		m_graphics->light2off();
	}



	if(ImGui::IsKeyReleased(7)){

		m_graphics->getCube()->setForce( btVector3(5, 0, 0) );
		dynamicsWorld->setGravity(btVector3(.25, -1, 0));

	}else if(ImGui::IsKeyReleased(4)){

		m_graphics->getCube()->setForce( btVector3(0, 0, -.75) );

	}else if(ImGui::IsKeyReleased(26)){

		m_graphics->getCube()->setForce( btVector3(.75, 0, 0) );

	}else if(ImGui::IsKeyReleased(22)){

		m_graphics->getCube()->setForce( btVector3(-.75, 0, 0) );

	}

    	ImGui::End();

}else{

	ImGui::Begin(":(");

	ImGui::SetWindowPos(ImVec2(400,200), true);
	ImGui::SetWindowSize(ImVec2(350,300), true);

	ImGui::Text("Game Over");
	ImGui::Text("Score: ");
	ImGui::SameLine();
	ImGui::Text("%d", m_graphics->getObject("Cube")->score);
	if(ImGui::Button("New Game")){
		gameState = true;
		m_graphics->getObject("Cube")->score = 0;
		m_graphics->getObject("Cube")->lives = 3;
	}

	ImGui::End();

}


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
