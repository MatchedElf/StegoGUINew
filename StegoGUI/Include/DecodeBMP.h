#pragma once
#include "lodepng.h"

#include <iostream>

unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp);
//
int doDecode(const wchar_t* in, const char* out);