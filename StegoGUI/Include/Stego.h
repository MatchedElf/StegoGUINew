#ifndef STEGO_H
#define STEGO_H

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
#include <cstdint>
//#include "RGB.h"
#include "Dwt2d.hpp"

#define PI 3.1415926

#define MONO

namespace Stego
{
    enum AlgNum
    {
        DCT = 1,
        DFT = 2,
        LSB = 3,
        DCT_KOCH = 4,
        HAAR = 5,
        HAAR_KOCH = 6
    };
}
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
RGB makeColor(BYTE r, BYTE g, BYTE b);
//
double coef(int i);
//
void DCT(uint8_t** pixels, double** result, int x, int y);
//
void IDCT(uint8_t** pixels, double** result, int x, int y);
//
void DFT(uint8_t** pixels, complex<double>** result, int x, int y);
//
void IDFT(uint8_t** pixels, complex<double>** result, int x, int y);
//
long double PSNR(uint8_t** orig, uint8_t** re, int height, int width);
//
long double MSE(uint8_t** orig, uint8_t** re, int height, int width);
//
double averageIntensity(uint8_t** orig, int height, int width);
//
double corrCoef(uint8_t** orig, uint8_t** re, int height, int width);
//
double dispersy(dwt::FloatMatrix orig, int row, int column, int& highestRow, int& highestColumn);
//
void encodeDCT(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key);
//
string decodeDCT(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeDFT(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key);
//
string decodeDFT(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeDCTKoch(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key);
//
string decodeDCTKoch(int height, int width, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, int difference, vector<int> key);
//
void encodeLSB(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size);
//
string decodeLSB(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat);
//
void HaarWavelet(uint8_t** pixels, uint8_t** result, int x, int y);
//
void IHaarWavelet(uint8_t** pixels, double** result, int x, int y);
//
void HaarWaveletFull(uint8_t** pixels, uint8_t** result, int x, int y);
//
void IHaarWaveletFull(uint8_t** pixels, uint8_t** result, int x, int y);
//
void encodeHaar(int width, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, bitset<16> secr_size, double difference, vector<int> key);
//
string decodeHaar(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key);
//
void encodeHaarKoch(int width, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, bitset<16> secr_size, double difference, vector<int> key);
//
string decodeHaarKoch(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, double difference, vector<int> key);
//
RGB** readFile(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1);
//
uint8_t** readFileMono(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1);
//
FILE* createFileBmp(const char* _filename, const wchar_t* _origFile);
//
FILE* CreateFileFromMono(const char* _filename, const wchar_t* _origFile);
//
vector<int> CreateKey(const char* _filename, int size, int vectSize, Stego::AlgNum alg);
//
vector<int> ReadKey(const char* _filename, vector<bitset<8>> vect);
//
void WriteToFile(FILE* newFile, RGB** pixels, int height, int width);
//
void WriteToFileMono(FILE* newFile, uint8_t** pixels, int height, int width);
//
vector<bitset<8>> ReadWord(const wchar_t* _filename, string& message);
//
double DolyaBitov(vector<bitset<8>> _orig, vector<bitset<8>> _new);
//
void CreateDiffFile(const wchar_t* _filename1, const wchar_t* _filename2, const char* _newfile);
//
void CreateDiffFileMono(const wchar_t* _filename1, const wchar_t* _filename2, const char* _newfile);
//
void normalizeForDisplay(double** data, uint8_t**res);
#endif // !STEGO_H