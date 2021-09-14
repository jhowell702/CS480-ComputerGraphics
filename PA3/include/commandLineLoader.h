#include <iostream>


std::string* commandLineLoader(int argc, char **argv){

	std::cout << "Shader file names by default are vert.glsl and frag.glsl" << std::endl;

	//if no command line arguments given, return and output what type of error

        if(argc == 1){
	std::cout << "Invalid command line arguments given. Valid flags -v 'vertex shader filename' and -f 'fragment shader filename' are necessary." << std::endl;
		return NULL;
	}

	//make dynamic string array

	std::string * shaderNames = new std::string[2];
	shaderNames[0] = "Invalid";
	shaderNames[1] = "Invalid";

	//set values to Invalid, if they're still invalid later, we know we didn't see -v or -f flags

	for(int x = 0; x < argc; ++x){

	//for all argv strings, if it's -v or -f, take next argv as file name

		if(strcmp(argv[x], "-v") == 0){
			if(x + 1 < argc){
				shaderNames[0] = argv[x + 1];
			}
		}

		if(strcmp(argv[x], "-f") == 0){
			if(x + 1 < argc){
				shaderNames[1] = argv[x + 1];
			} else{
				std::cout << "Flag -f provided but no file name given!" << std::endl;
				std::cout << "Current frag shader file name is frag.glsl" << std::endl;
			}
		}
	}

	//if still invalid, then flags weren't found

	if(shaderNames[0] == "Invalid" || shaderNames[1] == "Invalid"){
		std::cout << "Invalid flags given, missing either -v 'filename' or -f 'filename'" << std::endl;
		return NULL;
	}

	return shaderNames;

}
