#include <iostream>

#include "engine.h"

#include "commandLineLoader.h"

int main(int argc, char **argv)
{

  //create std string pointer to a new array of strings as created by commandLineLoaders()
  //element 1 is vertex file name, element 2 is fragment file name

  std::string* commandLineArgs = commandLineLoader(argc, argv);

  //commandLineArgs will be NULL if there is invalid input

  if(commandLineArgs == NULL){
	std::cout << "Current shader files are vert.glsl and frag.glsl" << std::endl;
	return 0;

  }else{

  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 1200, 700);

  //pass in file names into engine initialization, then graphics, then shader initialization

  if(!engine->Initialize(commandLineArgs))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
