#include "Headers/game_core.h"
#include <iostream>
#include <exception>
#include<vector>




void main() {
	try {
		core::runAND();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
}
	

