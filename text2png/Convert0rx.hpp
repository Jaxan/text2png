//
//  Tonemapper.hpp
//  text2png
//
//  Created by Joshua Moerman on 11/12/11.
//  Copyright 2011 Vadovas. All rights reserved.
//

#ifndef text2png_Tonemapper_hpp
#define text2png_Tonemapper_hpp

#include <iostream>
#include "png.hpp"

struct Convert0rx {
	Convert0rx()
	: width(0)
	, height(0)
	{
		for(int i = 0; i < 256; ++i){
			if('A' <= i && i <= 'Z'){
				double norm = (i - 'A') / ('Z' - 'A' + 1.0);
				colors[i] = color(norm * 0.8 + 0.2, 0.0, 0.0);
			}
			else if('a' <= i && i <= 'z'){
				double norm = (i - 'a') / ('z' - 'a' + 1.0);
				colors[i] = color(norm * 0.5 + 0.5, norm * 0.8 + 0.2, 0.0);
			}
			else if('0' <= i && i <= '9'){
				double norm = (i - '0') / ('9' - '0' + 1.0);
				colors[i] = color(0.0, norm * 0.8 + 0.2, norm * 0.5 + 0.5);
			}
			else if(isspace(i)){
				colors[i] = color(0, 0, 0);
			}
			else if(iscntrl(i)){
				colors[i] = color(255, i % 128, 255);
			}
			else if(ispunct(i)){
				colors[i] = color(128 + i, 128 + i, 128 + i);
			}
			else {
				colors[i] = color(0, i, 0);
			}
		}
	}
	
	void analyze(std::istream & input){
		std::string str;
		while(std::getline(input, str)){
			width = std::max(width, (uint32_t)str.size());
			++height;
		}
	}
	
	void process(std::istream & input, png::colored_ostream & image) const {
		std::string str;
		while(std::getline(input, str)){
			int i = 0;
			for (; i < str.size(); ++i) {
				image << colors[str[i]];
			}
			for(; i < width; ++i) {
				image << colors[' '];
			}
		}
	}
	
	uint32_t width;
	uint32_t height;
	
	typedef png::colored_ostream::pixel color;
	color colors[256];
};

#endif
