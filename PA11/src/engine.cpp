#include "engine.h"
#include <string>

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

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

	dynamicsWorld->setGravity(btVector3(0, -1, 0));



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
  gameState = -1;
  launchPower = 0;
  roundNum = 0;

	scoreboard = new int*[5];
	for(int i = 0; i < 5; ++i){
		scoreboard[i] = new int[3];
		for(int c = 0; c < 3; ++c){
			scoreboard[i][c] = 0;
		}
	}

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



////////////////////////////

	if(gameState > 0){

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	window_flags |= ImGuiWindowFlags_NoResize;
	bool* p_open = NULL;

	//start menu
	ImGui::Begin("Controls", p_open, window_flags);

        ImGui::SetWindowPos(ImVec2(400,500), true);
	ImGui::SetWindowSize(ImVec2(350,120), true);


	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("Controls Bar", tab_bar_flags))
            {
               
                if (ImGui::BeginTabItem("Control Widgets"))
                {


////////////////////////////////////////////////
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(0, 128, 255, 255));  // Set a background color

				float progress_saturated = IM_CLAMP(launchPower, 0.0f, 20.0f);
				char buf[32];
				sprintf(buf, "%d/%d", (int)(progress_saturated), 20);
				ImGui::ProgressBar(launchPower / 20, ImVec2(0.f, 0.f), buf);
				ImGui::SameLine();
				ImGui::Text("Launch Power");
				ImGui::PopStyleColor();
	
				btTransform trans;
				m_graphics->getCube()->getRigidBody()->getMotionState()->getWorldTransform(trans);
				float currentX = trans.getOrigin().getZ();
				ImGui::SliderFloat("Position", &currentX, -20.0f, 20.0f, "|", ImGuiSliderFlags_NoInput);
				float currentRotation = -1 * m_graphics->getCube()->getRotAngle();
				ImGui::SliderFloat("Rotation", &currentRotation, -20.0f, 20.0f, "|", ImGuiSliderFlags_NoInput);


//////////////////////////////////////////////////	

                    ImGui::EndTabItem();
                }

				if (ImGui::BeginTabItem("Controls")){

					ImGui::Text("Controls:");
					ImGui::Text("A: Move ball left");
					ImGui::Text("D: Move ball right");
					ImGui::Text("W: Hold and release to launch");
					ImGui::Text("S: To recall ball after throwing");

                    ImGui::EndTabItem();
                }

		        if (ImGui::BeginTabItem("Lighting Controls"))
		            {
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
				ImGui::Text("Light 1 : ");
				ImGui::SameLine();
				if(ImGui::Button("On##RedOn")){
					m_graphics->light1on();
				}
				ImGui::SameLine();
				if(ImGui::Button("Off##RedOff")){
					m_graphics->light1off();
				}

				ImGui::Text("Light 2: ");
				ImGui::SameLine();
				if(ImGui::Button("On##BlueOn")){
					m_graphics->light2on();
				}
				ImGui::SameLine();
				if(ImGui::Button("Off##BlueOff")){
					m_graphics->light2off();
				}


				ImGui::EndTabItem();
			}
                ImGui::EndTabBar();
            }



    	ImGui::End();
////////////////////////////////////
	window_flags |= ImGuiWindowFlags_NoScrollbar;
		ImGui::Begin("Score Menu", p_open, window_flags);

        ImGui::SetWindowPos(ImVec2(50,50), true);
		ImGui::SetWindowSize(ImVec2(425,80), true);
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit;
        enum ContentsType { CT_Text, CT_FillButton };
        static int contents_type = CT_Text;

	if (ImGui::BeginTable("table1", 6, flags))
        {
            // Display headers so we can inspect their interaction with borders.
            // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)

            ImGui::TableSetupColumn(" ");
            ImGui::TableSetupColumn("Round 1");
            ImGui::TableSetupColumn("Round 2");
            ImGui::TableSetupColumn("Round 3");
            ImGui::TableSetupColumn("Round 4");
            ImGui::TableSetupColumn("Round 5");
            ImGui::TableHeadersRow();
            
            for (int row = 0; row < 3; row++)
            {
                ImGui::TableNextRow();

                for (int column = 0; column < 6; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    char buf[32];

						if(column == 0){
						switch(row){
							case 0:
								sprintf(buf, "Round Total:"); break;
							case 1:
								sprintf(buf, "Ball 1 Score:"); break;
							case 2:
								sprintf(buf, "Ball 2 Score:"); break;
							}
						}else{
							sprintf(buf, "%d", scoreboard[column - 1][row]);
						}
                        ImGui::TextUnformatted(buf);
                }
            }
            ImGui::EndTable();
        }

	btTransform trans;
	m_graphics->getCube()->getRigidBody()->getMotionState()->getWorldTransform(trans);
	float currentX = trans.getOrigin().getZ();

	switch(gameState){
	
	//moving ball first round	
	case 1:

		if(ImGui::IsKeyPressed(7) && m_graphics->getCube()->launched != true){
					m_graphics->getCube()->rightMove = true;
		}else if(ImGui::IsKeyReleased(7)){
			m_graphics->getCube()->rightMove = false;
		}else if(ImGui::IsKeyPressed(4) && m_graphics->getCube()->launched != true){
				m_graphics->getCube()->leftMove = true;
		}else if(ImGui::IsKeyReleased(4)){
				m_graphics->getCube()->leftMove = false;
		}else if(ImGui::IsKeyPressed(26)){
			
			float test = launchPower + m_DT * M_PI/100;
			if(test <= 20){
				launchPower += m_DT * M_PI/100;
			}else{
				launchPower = 20;
			};
			m_graphics->getCube()->launched = true;

		}else if(ImGui::IsKeyReleased(26)){
			float rotation = glm::radians(m_graphics->getCube()->getRotAngle());

			float x = glm::cos(rotation) * launchPower;
			float z = glm::sin(rotation) * launchPower;

			m_graphics->getCube()->setForce( btVector3(x, 0, -z) );
			launchPower = 0;
			gameState = 2;

		}else if(ImGui::IsKeyReleased(21)){
			if(m_graphics->getCube()->getRigidBody()->getLinearVelocity().isZero()){
				m_graphics->getCube()->resetPos();
				for(int x = 1; x < 11; x++){
					m_graphics->getObject("Penguin" + to_string(x))->resetPos();
				}
			}
		}else if(ImGui::IsKeyPressed(20)){
			if(m_graphics->getCube()->getRotAngle() < 20){
				m_graphics->getCube()->setRotAngle(m_graphics->getCube()->getRotAngle() + 1);
			}
		}else if(ImGui::IsKeyPressed(8)){
			if(m_graphics->getCube()->getRotAngle() > -20){
				m_graphics->getCube()->setRotAngle(m_graphics->getCube()->getRotAngle() - 1);
			}
			
		}
			break;


		case 2:
			//if linear velocity is 0, next gamestate
			if(m_graphics->getCube()->getRigidBody()->getLinearVelocity().isZero()){
				gameState = 3;
			}
			break;

		case 3:
		{
		//wait on user input
			if(ImGui::IsKeyReleased(22)){
				gameState = 4;
			}
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoScrollbar;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool* p_open = NULL;
			ImGui::Begin("Continue", p_open,window_flags);
			ImGui::SetWindowPos(ImVec2(800,500), true);
			ImGui::SetWindowSize(ImVec2(35,35), true);
        	ImGui::PushButtonRepeat(false);
        	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { 
				gameState = 4;
			};
			ImGui::End();
			break;
		}
		case 4:
		{
		//input received, reset and count
			m_graphics->getCube()->resetPos();
			int counter = 0;
			for(int x = 1; x < 11; x++){
				if(m_graphics->getObject("Penguin" + to_string(x))->checkIfFell()){
					counter++;
				}
			}
			scoreboard[roundNum][1] = counter;
			gameState = 5;
			break;
		}
		case 5:
		{
		//move second ball
		if(ImGui::IsKeyPressed(7) && m_graphics->getCube()->launched != true){
				m_graphics->getCube()->rightMove = true;
		}else if(ImGui::IsKeyReleased(7)){
				m_graphics->getCube()->rightMove = false;
		}else if(ImGui::IsKeyPressed(4) && m_graphics->getCube()->launched != true){
				m_graphics->getCube()->leftMove = true;
		}else if(ImGui::IsKeyReleased(4)){
				m_graphics->getCube()->leftMove = false;
			}else if(ImGui::IsKeyPressed(26)){
				launchPower += m_DT * M_PI/100;
				m_graphics->getCube()->launched = true;

			}else if(ImGui::IsKeyReleased(26)){
				float rotation = glm::radians(m_graphics->getCube()->getRotAngle());

				float x = glm::cos(rotation) * launchPower;
				float z = glm::sin(rotation) * launchPower;

				m_graphics->getCube()->setForce( btVector3(x, 0, -z) );
				launchPower = 0;
				gameState = 6;

			}else  if(ImGui::IsKeyPressed(20)){
				m_graphics->getCube()->setRotAngle(m_graphics->getCube()->getRotAngle() + 1);
			}else if(ImGui::IsKeyPressed(8)){
				m_graphics->getCube()->setRotAngle(m_graphics->getCube()->getRotAngle() - 1);	
			}

			break;
		}
		case 6:
		{
		//launched second ball, waiting to stop
				if(m_graphics->getCube()->getRigidBody()->getLinearVelocity().isZero()){
				gameState = 7;					
				}
			break;
		}
		case 7:
		{
		//ball stopped waiting for user input
			if(ImGui::IsKeyReleased(22)){
				gameState = 8;
			}

				ImGuiWindowFlags window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoTitleBar;
				window_flags |= ImGuiWindowFlags_NoScrollbar;
				window_flags |= ImGuiWindowFlags_NoResize;
				bool* p_open = NULL;
				ImGui::Begin("Continue", p_open,window_flags);
				ImGui::SetWindowPos(ImVec2(800,500), true);
				ImGui::SetWindowSize(ImVec2(35,35), true);
		    	ImGui::PushButtonRepeat(false);
		    	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { 
					gameState = 8;
				};
				ImGui::End();

			break;
		}
		case 8:
		{
		//input received
			int counter = 0;
			for(int x = 1; x < 11; x++){
				if(m_graphics->getObject("Penguin" + to_string(x))->checkIfFell()){
					counter++;
				}
			}
			m_graphics->getCube()->resetPos();
			for(int x = 1; x < 11; x++){
				m_graphics->getObject("Penguin" + to_string(x))->resetPos();
			}
			scoreboard[roundNum][2] = counter - scoreboard[roundNum][1];
			if(counter != 0){			
				scoreboard[roundNum][0] = counter;
			}else{
				scoreboard[roundNum][0] = scoreboard[roundNum][1];
			}

			gameState = 1;
			roundNum++;

			if(roundNum == 5){
				gameState = 0;
			}

			break;
		}

	}

	ImGui::End();

}else if(gameState == 0){

	ImGui::Begin("Game Over!");

	ImGui::SetWindowPos(ImVec2(400,200), true);
	ImGui::SetWindowSize(ImVec2(350,100), true);

	int counter = 0;
	for(int i = 0; i < 5; i++){
		counter += scoreboard[i][0];
	}

	string line1 = "Game Over";
	string line2 = "Number of Penguins You Get to Eat!: " + to_string(counter);

    textc(line1);
	ImGui::Text(" ");
    textc(line2);
	ImGui::Text(" ");

    float font_size = ImGui::GetFontSize() * 8 / 2;
    ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );

	if(ImGui::Button("New Game")){
		gameState = 1;
		roundNum = 0;
		for(int r = 0; r < 3; r++){
			for(int c = 0; c < 5; c++){
				scoreboard[r][c] = 0;
			}
		}
	}

	ImGui::End();


}else if(gameState == -1){
	
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;
	bool* p_open = NULL;

	ImGui::Begin("StartGame", p_open, window_flags);
	ImGui::SetWindowPos(ImVec2(400,200), true);
	ImGui::SetWindowSize(ImVec2(350,200), true);

	string line1 = "Penguin Bowling";
	string line2 = "by";
	string line3 = "Elizabeth Kish, Justin Howell, Noah Doddridge";
	string line4 = "Start Game by Pressing Enter";
	string line5 = "Or Click Below";
	string line6 = "Knock as many penguins into your home";
    string line7 = "...so you can eat them later";

    textc(line1);
	ImGui::Text(" ");
    textc(line2);
	ImGui::Text(" ");
    textc(line3);
	ImGui::Text(" ");
    textc(line4);
	ImGui::Text(" ");
    textc(line5);
	ImGui::Text(" ");


    float font_size = ImGui::GetFontSize() * 10 / 2;
    ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );
	if(ImGui::Button("Start Game")){
		gameState = 1;
	}
	if(ImGui::IsKeyPressed(40)){
				gameState = 1;
	}
	ImGui::Text(" ");
    textc(line6);
	ImGui::Text(" ");
    textc(line7);
	ImGui::Text(" ");

	ImGui::End();	

}

    	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::textc(string in){
    float font_size = ImGui::GetFontSize() * in.length() / 2;
    ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );

    ImGui::Text(in.c_str());
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
