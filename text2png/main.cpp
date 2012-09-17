//
//  main.cpp
//  text2png
//
//  Created by Joshua Moerman on 11/12/11.
//  Copyright 2011 Vadovas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "png.hpp"
#include "Convert0rx.hpp"

int main (int argc, const char * argv[]) try {
	if (argc != 2) throw std::runtime_error("You must provide exactly 1 argument");
	
	Convert0rx c;
	{	// we will stream EVERYTHING, that's why we need two passes.
		std::ifstream file(argv[1]);
		if (!file) throw std::runtime_error("File could not be opened");
		c.analyze(file);
	}
	
	{	// lazy way of rewinding the stream ;D.
		std::ifstream file(argv[1]);
		if (!file) throw std::runtime_error("File could not be opened");
		png::colored_ostream image(c.width, c.height, std::string(argv[1]) + ".png");
		c.process(file, image);
	}
	
} catch (std::exception & e){
	std::cout << e.what() << std::endl;
}

