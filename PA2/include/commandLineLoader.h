#include <iostream>


std::string* commandLineLoader(int argc, char **argv){

	std::string * shaderNames = new std::string[2];

	for(int x = 0; x < argc; ++x){

		if(strcmp(argv[x], "-v") == 0){
			if(x + 1 < argc){
				shaderNames[0] = argv[x + 1];
			} else{
				std::cout << "Flag -v provided but no file name given!" << std::endl;
			}
		}

		if(strcmp(argv[x], "-f") == 0){
			if(x + 1 < argc){
				shaderNames[1] = argv[x + 1];
			} else{
				std::cout << "Flag -f provided but no file name given!" << std::endl;
			}
		}

	}


	return shaderNames;

}
