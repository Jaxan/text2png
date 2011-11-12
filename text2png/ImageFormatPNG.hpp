//
//  ImageFormatPNG.hpp
//  AwesomeAttractorND
//
//  Created by Joshua Moerman on 11/12/11.
//  Copyright 2011 Vadovas. All rights reserved.
//

#ifndef AwesomeAttractorND_ImageFormatPNG_hpp
#define AwesomeAttractorND_ImageFormatPNG_hpp

#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <png.h>

namespace ImageFormats {
	namespace png{
		struct pixel {
			pixel()
			: red(0)
			, green(0)
			, blue(0)
			{}
			
			pixel(double red, double green, double blue)
			: red(clamp(255*red))
			, green(clamp(255*green))
			, blue(clamp(255*blue))
			{}
			
			pixel(int red, int green, int blue)
			: red(clamp(red))
			, green(clamp(green))
			, blue(clamp(blue))
			{}
			
			void swapRB(){
				std::swap(red, blue);
			}
			
		private:
			uint8_t clamp(int n){
				return std::min(255, std::max(0, n));
			}
			
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		};
		
		struct png_stream{
			typedef pixel pixel;
			
			png_stream(uint32_t width, uint32_t height, std::string filename)
			: fp(0)
			, png_ptr(0)
			, info_ptr(0)
			, row(width)
			, x(0)
			{
				fp = fopen(filename.c_str(), "wb");
				if(!fp) throw std::runtime_error("Could not open file");
				
				png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
				if(!png_ptr) throw std::runtime_error("PNG structure could not be allocated");
				
				info_ptr = png_create_info_struct(png_ptr);
				if(!info_ptr) throw std::runtime_error("PNG information structure could not be allocated");
				
				png_init_io(png_ptr, fp);
				
				png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
				
				png_set_compression_level(png_ptr, 9);
				
				png_write_info(png_ptr, info_ptr);
			}
			
			~png_stream(){
				png_write_end(png_ptr, info_ptr);
				png_destroy_info_struct(png_ptr, &info_ptr);
				fclose(fp);
			}
			
			png_stream& operator<<(pixel const & p){
				row[x] = p;
				++x;
				if(x >= row.size()){
					png_write_row(png_ptr, reinterpret_cast<unsigned char const*>(row.data()));
					x = 0;
				}
				
				return *this;
			}
			
		private:
			FILE* fp;
			png_structp png_ptr;
			png_infop info_ptr;
			
			std::vector<pixel> row;
			uint32_t x;
		};
	}
}

#endif
