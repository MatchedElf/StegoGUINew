#ifndef STEGO_H
#define STEGO_H
#endif // !STEGO_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <bitset>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <complex>
#include <random>
#include <JuceHeader.h>

#define PI 3.1415926

using namespace std;

typedef struct RGB {
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE rgbReserved;
};
//
BYTE sat(float x);
//
RGB MakeColor(BYTE r, BYTE g, BYTE b);
//
float coef(int i);
//
void DCT(RGB** pixels, float** result, int x, int y);
//
void IDCT(RGB** pixels, float** result, int x, int y);
//
void DFT(RGB** pixels, complex<double>** result, int x, int y);
//
void IDFT(RGB** pixels, complex<double>** result, int x, int y);
//
void PSNR(RGB** orig, RGB** re, long double& r, long double& g, long double& b, int height, int width);
//
double AverageIntensity(RGB** orig, int height, int width);
//
double CorrCoef(RGB** orig, RGB** re, int height, int width);
//
vector<float**> encodeDCT(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key);
//
string decodeDCT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat, vector<int> key);
//
vector<complex<double>**> encodeDFT(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key);
//
string decodeDFT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeLSB(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size);
//
string decodeLSB(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat);
//
RGB** ReadFile(const char* _filename, int& h, int& w, int& size, juce::String& retStr1);
//
FILE* Create_File(const char* _filename, const char* _origFile);
//
vector<int> CreateKey(const char* _filename, int size, int vectSize, bool blocks);
//
vector<int> ReadKey(const char* _filename, vector<bitset<8>> vect);
//
void WriteToFile(FILE* newFile, RGB** pixels, int height, int width);
//
vector<bitset<8>> ReadWord(const char* _filename, int& word_size);
//
double DolyaBitov(vector<bitset<8>> _orig, vector<bitset<8>> _new);
//
void CreateDiffFile(const char* _filename1, const char* _filename2, const char* _newfile);