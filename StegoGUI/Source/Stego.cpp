#include "../Include/Stego.h"

BYTE sat(float x)
{
	if (x < 0) return 0;
	if (x > 255) return 255;
	else return x;
}
//
RGB MakeColor(BYTE r, BYTE g, BYTE b) {
	RGB color;
	color.red = r;
	color.green = g;
	color.blue = b;
	return color;
}
//
float coef(int i) {
	if (i == 0) return sqrt(1.0 / 8.0);
	else return sqrt(2.0 / 8.0);
}
//
void DCP(RGB** pixels, float** result, int x, int y) {
	// Öèêë çàïîëíåíèÿ ÄÊÏ ìàòðèöû 
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
			//Öèêë ñóììèðîâàíèÿ ýëåìåíòîâ äëÿ îäíîãî ýëåìåíòà ÄÊÏ
			float cu = coef(u);
			float cv = coef(v);
			float sum = 0;
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					sum += pixels[x + k][y + l].blue * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16);
				}
			}
			result[u][v] = cu * cv * sum;
			//result[u][v] = 0;
		}
	}
}
//
void ODCP(RGB** pixels, float** result, int x, int y) {
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			float sum = 0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					sum += coef(u) * coef(v) * result[u][v] * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16);
				}
			}
			pixels[x + k][y + l].blue = sat(sum);
		}
	}

}
//
void DFP(RGB** pixels, complex<double>** result, int x, int y) {
	// Öèêë çàïîëíåíèÿ ÄPF ìàòðèöû 
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
			//Öèêë ñóììèðîâàíèÿ ýëåìåíòîâ äëÿ îäíîãî ýëåìåíòà ÄÊÏ
			//float sum = 0;
			complex<double> sum(0.0, 0.0);
			complex<double> NN(64.0, 0.0);
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					complex<double> pixel(pixels[x + k][y + l].blue, 0.0);
					complex<double> e(cos(2 * PI * (u * k / 8.0 + v * l / 8.0)), -sin(2 * PI * (u * k / 8.0 + v * l / 8.0)));
					sum += pixel * e;
				}
			}
			result[u][v] = sum / NN;
			//result[u][v] = 0;
		}
	}
}
//
void ODFP(RGB** pixels, complex<double>** result, int x, int y) {
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			complex<double> sum(0.0, 0.0);
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					complex<double> pixel(result[u][v].real(), result[u][v].imag());
					complex<double> e(cos(2 * PI * (u * k / 8.0 + v * l / 8.0)), sin(2 * PI * (u * k / 8.0 + v * l / 8.0)));
					sum += pixel * e;
				}
			}
			pixels[x + k][y + l].blue = sat(sum.real());
		}
	}
}
//
void PSNR(RGB** orig, RGB** re, long double& r, long double& g, long double& b, int height, int width) {
	long double znam = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			znam += pow((orig[i][j].red - re[i][j].red), 2);
		}
	}
	r = 10 * log10(height * width * pow(pow(2, 8) - 1, 2) / znam);

	znam = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			znam += pow((orig[i][j].green - re[i][j].green), 2);
		}
	}
	g = 10 * log10(height * width * pow(pow(2, 8) - 1, 2) / znam);

	znam = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			znam += pow((orig[i][j].blue - re[i][j].blue), 2);
			//cout << (float)orig[i][j].blue << " - " << (float)re[i][j].blue << endl;
			//if (orig[i][j].blue != re[i][j].blue) cout << "!=" << endl;
		}
	}
	b = 10 * log10(height * width * pow(pow(2, 8) - 1, 2) / znam);
}
//
double CorrCoef(RGB** orig, RGB** re, int height, int width)
{
	return 0.0;
}
//
vector<float**> encodeDCP(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key) {
	vector<float**> matrixes;
	cout << "Before DCP" << endl;
	int count = 0;
	/*for (int i = 0; i < height; i += 8) {
		for (int j = 0; j < width; j += 8) {
			float** res = new float* [8];
			for (int z = 0; z < 8; z++) {
				res[z] = new float[8];
			}
			DCP(pixelsNew, res, i, j);
			matrixes.push_back(res);
			count++;
		}
		if (count >= vect.size() * 8) break;
	}*/
	while (count < vect.size() * 8) {
		float** res = new float* [8];
		for (int z = 0; z < 8; z++) {
			res[z] = new float[8];
		}
		DCP(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
		matrixes.push_back(res);
		count++;

	}
	cout << "After DCP" << endl;
	int pixCount = 0;
	while ((pixCount / 8) < vect.size()) {

		if (pixCount < 16) {
			if (secr_size[pixCount] == 1) {
				matrixes[pixCount][4][3] += difference;
			}
			else {
				matrixes[pixCount][4][3] -= difference;
			}
		}
		else {
			if ((vect[pixCount / 8][pixCount % 8] == 1)) {
				matrixes[pixCount][4][3] += difference;
			}
			else {
				matrixes[pixCount][4][3] -= difference;
			}
		}
		pixCount++;
	}
	/*int indCount = 0;
	for (int i = 0; i < height; i += 8) {
		for (int j = 0; j < width; j += 8) {
			ODCP(pixelsNew, matrixes[indCount], i, j);
			indCount++;
		}
		if (indCount >= vect.size() * 8) break;
	}*/
	int indCount = 0;
	while (indCount < vect.size() * 8) {
		ODCP(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
		indCount++;

	}
	for (int i = 0; i < matrixes.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			delete[] matrixes[i][j];
		}
		delete[] matrixes[i];
	}
	cout << "After ODCP" << endl;
	return matrixes;

}
//
string decodeDCP(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat, vector<int> key) {
	bitset<8> read;
	bitset<16> readSize;

	int pixCount = 0;
	int bits = 1000;
	string result = "";
	for (int x = 0; x < height; x += 8) {
		if (pixCount == (bits * 8 + 16)) break;
		if (pixCount == vect.size() * 8) break;
		for (int y = 0; y < width; y += 8) {
			float** res = new float* [8];
			for (int z = 0; z < 8; z++) {
				res[z] = new float[8];
			}
			DCP(pixels, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			//DCP(pixels, res, x, y);
			double cf1 = res[4][3];
			DCP(pixelsNew, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			//DCP(pixelsNew, res, x, y);
			double cf2 = res[4][3];
			for (int z = 0; z < 8; z++) {
				delete[] res[z];
			}
			delete[] res;
			if (pixCount < 16) {

				if (cf1 < cf2) readSize[pixCount] = 1;
				else readSize[pixCount] = 0;
				pixCount++;
				if (pixCount == 16) {
					bits = readSize.to_ulong();
					cout << "bits = " << bits << endl;
				}
			}
			else {
				if (cf1 < cf2) read[pixCount % 8] = 1;
				else read[pixCount % 8] = 0;
				pixCount++;
				if ((pixCount % 8) == 0) {
					result += read.to_ulong();
					//cout << result << endl;
					if (flag) {
						vectSzhat.push_back(read);
					}
				}
				if (pixCount == (bits * 8 + 16)) break;
				if (pixCount == vect.size() * 8) break;
			}
		}
	}
	cout << "After decoding DCP" << endl;
	return result;
}
//
vector<complex<double>**> encodeFurie(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key) {
	vector<complex<double>**> matrixes;
	cout << "Before DFP" << endl;
	int count = 0;
	/*for (int i = 0; i < height; i += 8) {
		for (int j = 0; j < width; j += 8) {
			complex<double>** res = new complex<double>*[8];
			for (int z = 0; z < 8; z++) {
				res[z] = new complex<double>[8];
			}
			DFP(pixelsNew, res, i, j);
			matrixes.push_back(res);
			count++;

		}
		if (count >= vect.size() * 8) break;
	}*/
	while (count < vect.size() * 8) {
		complex<double>** res = new complex<double>*[8];
		for (int z = 0; z < 8; z++) {
			res[z] = new complex<double>[8];
		}
		DFP(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
		matrixes.push_back(res);
		count++;

	}
	cout << "After DFP" << endl;
	int pixCount = 0;
	while ((pixCount / 8) < vect.size()) {

		if (pixCount < 16) {
			if (secr_size[pixCount] == 1) {
				matrixes[pixCount][4][3] += difference;
			}
			else {
				matrixes[pixCount][4][3] -= difference;
			}
		}
		else {
			if ((vect[pixCount / 8][pixCount % 8] == 1)) {
				matrixes[pixCount][4][3] += difference;
			}
			else {
				matrixes[pixCount][4][3] -= difference;
			}
		}
		pixCount++;
	}
	int indCount = 0;
	/*for (int i = 0; i < height; i += 8) {
		for (int j = 0; j < width; j += 8) {
			ODFP(pixelsNew, matrixes[indCount], i, j);
			indCount++;
		}
		if (indCount >= vect.size() * 8) break;
	}*/
	while (indCount < vect.size() * 8) {
		ODFP(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
		indCount++;

	}
	for (int i = 0; i < matrixes.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			delete[] matrixes[i][j];
		}
		delete[] matrixes[i];
	}
	cout << "After ODFP" << endl;
	return matrixes;


}
//
string decodeFurie(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
	bitset<8> read;
	bitset<16> readSize;

	int pixCount = 0;
	int bits = 1000;
	string result = "";
	for (int x = 0; x < height; x += 8) {
		if (pixCount == (bits * 8 + 16)) break;
		if (pixCount == vect.size() * 8) break;
		for (int y = 0; y < width; y += 8) {
			complex<double>** res = new complex<double>*[8];
			for (int z = 0; z < 8; z++) {
				res[z] = new complex<double>[8];
			}
			//DFP(pixels, res, x, y);
			DFP(pixels, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			complex<double> cf1 = res[4][3];
			//DFP(pixelsNew, res, x, y);
			DFP(pixelsNew, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			complex<double> cf2 = res[4][3];
			for (int z = 0; z < 8; z++) {
				delete[] res[z];
			}
			delete[] res;
			if (pixCount < 16) {

				if (cf1.real() < cf2.real()) readSize[pixCount] = 1;
				else readSize[pixCount] = 0;
				pixCount++;
				if (pixCount == 16) {
					bits = readSize.to_ulong();
					cout << "bits = " << bits << endl;
				}
			}
			else {
				if (cf1.real() < cf2.real()) read[pixCount % 8] = 1;
				else read[pixCount % 8] = 0;
				pixCount++;
				if ((pixCount % 8) == 0) {
					result += read.to_ulong();
					vectSzhat.push_back(read);
					//cout << result << endl;
				}
				if (pixCount == (bits * 8 + 16)) break;
				if (pixCount == vect.size() * 8) break;
			}
		}
	}
	cout << "After decoding DFP" << endl;
	return result;
}
//
void encodeLSB(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size) {
	ifstream keyRead("key.txt");
	if (!(keyRead.is_open())) {
		cout << "Error while opening file." << endl;
	}
	for (int i = 0; i < vect.size() * 8; i++) {
		int index;
		int channel = 0;
		keyRead >> index;
		if (channel == 0) {
			if (i < 16) {
				if (secr_size[i] == 1) {
					pixelsNew[index / width][index % width].blue |= 1;
					//colorsNewBits[0] = 255;
					//colorsNewBits[1] = 255;
					//colorsNewBits[2] = 255;
				}
				else {
					pixelsNew[index / width][index % width].blue &= ~(1);
					//colorsNewBits[0] = 0;
					//colorsNewBits[1] = 0;
					//colorsNewBits[2] = 0;
				}
			}
			else
			{
				if ((vect[i / 8][i % 8] == 1))
				{
					pixelsNew[index / width][index % width].blue |= 1;
					//colorsNewBits[0] = 255;
					//colorsNewBits[1] = 255;
					//colorsNewBits[2] = 255;
				}
				else {
					pixelsNew[index / width][index % width].blue &= ~(1);
					//colorsNewBits[0] = 0;
					//colorsNewBits[1] = 0;
					//colorsNewBits[2] = 0;
				}
			}
		}
		else if (channel == 1) {

		}
		else {

		}
	}
	cout << "After LSB" << endl;
}
//
string decodeLSB(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, boolean flag, vector<bitset<8>>& vectSzhat) {
	BYTE channelByte;
	ifstream keyRead1("key.txt");
	if (!(keyRead1.is_open())) {
		cout << "Error while opening file." << endl;
	}
	//
	bitset<8> read;
	bitset<16> readSize;
	//
	int bits = 1000;
	string result = "";
	int pixCount = 0;
	for (int i = 0; i < vect.size() * 8; i++) {
		int index;
		keyRead1 >> index;
		channelByte = pixelsNew[index / width][index % width].blue;
		if (pixCount < 16) {
			readSize[pixCount] = channelByte & 1;
			pixCount++;
			if (pixCount == 16) {
				bits = readSize.to_ulong();
				cout << "bits = " << bits << endl;
			}
		}
		else {
			read[pixCount % 8] = channelByte & 1;
			pixCount++;
			if ((pixCount % 8) == 0) {
				result += read.to_ulong();
				//cout << result << endl;
				if (flag) {
					vectSzhat.push_back(read);
				}
			}
			if (pixCount == (bits * 8 + 16)) break;
		}
	}
	return result;
	//
}
//
RGB** ReadFile(const char* _filename, int& h, int& w, int& size, juce::String& retStr1)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	FILE* _file = fopen(_filename, "rb");
	if (_file == NULL) {
		fputs("File opening error.", stderr);
		return NULL;
	}
	if (fread((LPSTR)&bmfHeader, 1, sizeof(bmfHeader), _file) != sizeof(bmfHeader)) {
		fputs("File header corrupted?", stderr);
		return NULL;
	}
	if (fread((LPSTR)&bmiHeader, 1, sizeof(bmiHeader), _file) != sizeof(bmiHeader)) {
		fputs("File info header corrupted?", stderr);
		return NULL;
	}
	juce::String retStr = "";
	retStr += juce::String((std::wstring(L"Size = ")).c_str());
	retStr += juce::String(to_string(bmfHeader.bfSize));
	retStr += juce::String((std::wstring(L" bytes")).c_str());
	retStr += "\n";
	retStr += juce::String((std::wstring(L"Width = ")).c_str());
	retStr += juce::String(to_string(bmiHeader.biWidth));
	retStr += juce::String((std::wstring(L" pixels")).c_str());
	retStr += "\n";
	retStr += juce::String((std::wstring(L"Height = ")).c_str());
	retStr += juce::String(to_string(bmiHeader.biHeight));
	retStr += juce::String((std::wstring(L" pixels")).c_str());
	retStr += "\n";
	retStr += "bibit = ";
	retStr += juce::String(to_string(bmiHeader.biBitCount));
	retStr += "\n";
	retStr += "biclr = ";
	retStr += juce::String(to_string(bmiHeader.biClrUsed));
	retStr += "\n";
	retStr1 = retStr;
	h = bmiHeader.biHeight;
	w = bmiHeader.biWidth;
	size = bmiHeader.biWidth * bmiHeader.biHeight;
	//
	RGB** pixels = new RGB * [h + 2];
	for (int i = 0; i < h + 2; i++) pixels[i] = new RGB[w + 1];
	//
	int depth = (bmiHeader.biBitCount == 24) ? 3 : 4;
	char colors[3];
	char colors1[4];
	BYTE r, g, b;
	int indCount = 0;
	if (depth == 3)
	{
		while (fread(&colors, 1, sizeof(colors), _file) > 0) {
			r = colors[2];
			g = colors[1];
			b = colors[0];
			//
			pixels[indCount / w][indCount % w] = MakeColor(r, g, b);
			indCount++;
		}
	}
	else
	{
		while (fread(&colors1, 1, sizeof(colors1), _file) > 0) {
			r = colors1[2];
			g = colors1[1];
			b = colors1[0];
			//
			pixels[indCount / w][indCount % w] = MakeColor(r, g, b);
			indCount++;
		}
	}
	fclose(_file);
	return pixels;
}
//
FILE* Create_File(const char* _filename, const char* _origFile)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	FILE* origFile = fopen(_origFile, "rb");
	FILE* newFile = fopen(_filename, "wb");
	if (origFile == NULL) {
		fputs("File opening error.", stderr);
		return NULL;
	}
	if (fread((LPSTR)&bmfHeader, 1, sizeof(bmfHeader), origFile) != sizeof(bmfHeader)) {
		fputs("File header corrupted?", stderr);
		return NULL;
	}
	if (fread((LPSTR)&bmiHeader, 1, sizeof(bmiHeader), origFile) != sizeof(bmiHeader)) {
		fputs("File info header corrupted?", stderr);
		return NULL;
	}
	if (newFile == NULL) {
		fputs("File creating error.", stderr);
		return NULL;
	}
	fwrite(&bmfHeader, 1, sizeof(bmfHeader), newFile);
	fwrite(&bmiHeader, 1, sizeof(bmiHeader), newFile);
	fclose(origFile);
	return newFile;
}
//
vector<int> CreateKey(const char* _filename, int size, int vectSize, bool blocks)
{
	ofstream keyFile(_filename);
	if (!(keyFile.is_open())) {
		cout << "Error while opening file." << endl;
		return { 0 };
	}
	vector<int> key(0);
	int i = 0;
	cout << "Key vect creation" << endl;
	while (i < vectSize * 8) {
		int rand_num = 0;
		if (blocks)
		{
			//rand_num = rand() * (size / 32767) % (size / 64);
			rand_num = rand() % (size / 64);

		}
		else
		{
			rand_num = rand() * (size / RAND_MAX) % size;
			//rand_num = rand() % size;
		}
		if (count(key.begin(), key.end(), rand_num) == 0) {
			key.push_back(rand_num);
			keyFile << rand_num << " ";
			i++;
			//if (i % (vect.size() * 8 / 100) == 0) {
			//	cout << i / (vect.size() * 8 / 100) << "%" << endl;
			//}
		}
	}
	keyFile.close();
	return key;
}
vector<int> ReadKey(const char* _filename, vector<bitset<8>> vect)
{
	vector<int> ret;
	ifstream keyRead1(_filename);
	if (!(keyRead1.is_open())) {
		cout << "Error while opening file." << endl;
	}
	for (int i = 0; i < vect.size() * 8; i++)
	{
		int index;
		keyRead1 >> index;
		ret.push_back(index);
	}

	return ret;
}
//
void WriteToFile(FILE* newFile, RGB** pixels, int height, int width)
{
	char colors[3];
	//
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			colors[2] = pixels[i][j].red;
			colors[1] = pixels[i][j].green;
			colors[0] = pixels[i][j].blue;
			//
			fwrite(&colors, 1, sizeof(colors), newFile);
		}
	}
}
//
vector<bitset<8>> ReadWord(const char* _filename, int& word_size)
{
	ifstream wordFile(_filename);
	if (!(wordFile.is_open())) {
		cout << "Error while opening file." << endl;
		return { 0 };
	}
	string secr = "";
	string line;
	while (getline(wordFile, line))
	{
		secr += line;
	}
	//wordFile >> secr;
	word_size = secr.size();
	vector<bitset<8>> vect;
	bitset<16> secr_size(secr.size());
	bitset<8> empty;
	vect.push_back(empty);
	vect.push_back(empty);
	for (int i = 0; i < secr.size(); i++) {
		bitset<8> letter(secr[i]);
		vect.push_back(letter);
	}
	return vect;
}
//
double DolyaBitov(vector<bitset<8>> _orig, vector<bitset<8>> _new)
{
	int count = 0;
	int countAll = 0;
	for (int i = 2; i < _orig.size(); i++) {
		for (int j = 0; j < 8; j++) {
			if (i >= _new.size())
			{
				count++;
			}
			else if (_orig[i][j] != _new[i][j]) {
				count++;
			}
			countAll++;
		}
	}
	double ret = (double)count / countAll;
	return ret;
}
void CreateDiffFile(const char* _filename1, const char* _filename2, const char* _newfile)
{
	int height;
	int width;
	int size;
	juce::String info;
	//
	RGB** pixels1 = ReadFile(_filename1, height, width, size, info);
	RGB** pixels2 = ReadFile(_filename2, height, width, size, info);
	//
	RGB** pixelsNew = new RGB * [height + 2];
	for (int i = 0; i < height + 2; i++) pixelsNew[i] = new RGB[width + 1];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pixels1[i][j].blue != pixels2[i][j].blue)
			{
				int diff = fabs(pixels1[i][j].blue - pixels2[i][j].blue);

				pixelsNew[i][j].red = sat(0 + 100 * ( (diff - 2)));
				pixelsNew[i][j].green = sat(255 - 200 * (diff - 1));
				pixelsNew[i][j].blue = sat(255 * (diff > 1) - 100 * (diff - 2));
				/*if (fabs(pixels1[i][j].blue - pixels2[i][j].blue) >=1)
				{
					pixelsNew[i][j].red = 255;
					pixelsNew[i][j].green = 255;
					pixelsNew[i][j].blue = 255;
				}*/
				/*else
				{
					pixelsNew[i][j].red = 0;
					pixelsNew[i][j].green = 0;
					pixelsNew[i][j].blue = 220;
				}*/
			}
			else
			{
				pixelsNew[i][j].red = pixels1[i][j].red;
				pixelsNew[i][j].green = pixels1[i][j].green;
				pixelsNew[i][j].blue = pixels1[i][j].blue;
			}
		}
	}
	FILE* newFile = Create_File(_newfile, _filename1);
	WriteToFile(newFile, pixelsNew, height, width);
	//
	for (int i = 0; i < height + 2; i++)
	{
		delete[] pixels1[i];
		delete[] pixels2[i];
		delete[] pixelsNew[i];
	}
	//
	delete[] pixels1;
	delete[] pixels2;
	delete[] pixelsNew;
}
//