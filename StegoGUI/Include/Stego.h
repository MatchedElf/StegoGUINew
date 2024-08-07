#ifndef STEGO_H
#define STEGO_H
#endif // !STEGO_H

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

struct RGB {
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE rgbReserved;
};
//
BYTE sat(double x);
//
RGB MakeColor(BYTE r, BYTE g, BYTE b);
//
double coef(int i);
//
void DCT(RGB** pixels, double** result, int x, int y);
//
void IDCT(RGB** pixels, double** result, int x, int y);
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
void encodeDCT(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key);
//
string decodeDCT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeDFT(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key);
//
string decodeDFT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeDCTKoch(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key);
//
string decodeDCTKoch(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat, int difference, vector<int> key);
//
void encodeLSB(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size);
//
string decodeLSB(int width, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat);
//
RGB** ReadFile(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1);
//
FILE* Create_File(const char* _filename, const wchar_t* _origFile);
//
vector<int> CreateKey(const char* _filename, int size, int vectSize, bool blocks);
//
vector<int> ReadKey(const char* _filename, vector<bitset<8>> vect);
//
void WriteToFile(FILE* newFile, RGB** pixels, int height, int width);
//
vector<bitset<8>> ReadWord(const wchar_t* _filename, string& message);
//
double DolyaBitov(vector<bitset<8>> _orig, vector<bitset<8>> _new);
//
void CreateDiffFile(const wchar_t* _filename1, const wchar_t* _filename2, const char* _newfile);