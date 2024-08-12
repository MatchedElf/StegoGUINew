#include "../Include/Stego.h"

BYTE sat(double x)
{
	if (x < 0) return 0;
	if (x > 255) return 255;
	else return (BYTE)x;
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
double coef(int i) {
	if (i == 0) return (double)sqrt(1.0 / 8.0);
	else return (double)sqrt(2.0 / 8.0);
}
//
void DCT(RGB** pixels, double** result, int x, int y) {
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
			double cu = coef(u);
			double cv = coef(v);
			double sum = 0;
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					sum += (double)(pixels[x + k][y + l].blue * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16));
				}
			}
			result[u][v] = cu * cv * sum;
		}
	}
}
//
void IDCT(RGB** pixels, double** result, int x, int y) {
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			double sum = 0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					sum += (double)(coef(u) * coef(v) * result[u][v] * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16));
				}
			}
			pixels[x + k][y + l].blue = sat(sum);
		}
	}

}
//
void DFT(RGB** pixels, complex<double>** result, int x, int y) {
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
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
		}
	}
}
//
void IDFT(RGB** pixels, complex<double>** result, int x, int y) {
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
			pixels[x + k][y + l].blue = sat((double)sum.real());
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
		}
	}
	b = 10 * log10(height * width * pow(pow(2, 8) - 1, 2) / znam);
}
double AverageIntensity(RGB** orig, int height, int width)
{
	double ret = 0.0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			ret += orig[i][j].blue;
		}
	}
	return ret / (width * height);
}
//
double CorrCoef(RGB** orig, RGB** re, int height, int width)
{
	double origAv = AverageIntensity(orig, height, width);
	double reAv = AverageIntensity(re, height, width);
	double chisl = 0.0;
	//
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			chisl += (orig[i][j].blue - origAv) * (re[i][j].blue - reAv);
		}
	}
	//
	double znam = 0.0;
	double origSum = 0.0;
	//
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			origSum += pow((orig[i][j].blue - origAv), 2);
		}
	}
	double reSum = 0.0;
	//
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			reSum += pow((re[i][j].blue - reAv), 2);
		}
	}
	znam = sqrt(origSum * reSum);
	return chisl / znam;
}
//
void encodeDCT(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key) {
	vector<double**> matrixes;
	cout << "Before DCT" << endl;
	int count = 0;
	while (count < vect.size() * 8) {
		double** res = new double* [8];
		for (int z = 0; z < 8; z++) {
			res[z] = new double[8];
		}
		DCT(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
		matrixes.push_back(res);
		count++;

	}
	cout << "After DCT" << endl;
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
	while (indCount < vect.size() * 8) {
		IDCT(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
		indCount++;

	}
	/*ofstream keyFile("matrixes.txt");
	if (!(keyFile.is_open())) {
		cout << "Error while opening file." << endl;
	}
	for (int i = 0; i < matrixes.size(); i++)
	{
		keyFile << "=============================================================\n";
		for (int j = 0; j < 8; j++)
		{
			for (int z = 0; z < 8; z++)
			{
				keyFile << matrixes[i][j][z] << " ";
			}
			keyFile << "\n";
		}
	}
	keyFile.close();*/
	for (int i = 0; i < matrixes.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			delete[] matrixes[i][j];
		}
		delete[] matrixes[i];
	}
	cout << "After IDCT" << endl;
	return;

}
//
string decodeDCT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
	bitset<8> read;
	bitset<16> readSize;
	//
	bool stop = false;
	int pixCount = 0;
	int bits = 1000;
	string result = "";
	for (int x = 0; x < height; x += 8) {
		//if (pixCount == (bits * 8 + 16)) break;
		if (pixCount == vect.size() * 8) break;
		for (int y = 0; y < width; y += 8) {
			double** res = new double* [8];
			for (int z = 0; z < 8; z++) {
				res[z] = new double[8];
			}
			if (width > 8 * (key[pixCount] / (width / 8)))
				DCT(pixels, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			else
				return "Error! " + result;
			double cf1 = res[4][3];
			DCT(pixelsNew, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
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
					if(!stop)
						result += read.to_ulong();
					vectSzhat.push_back(read);
				}
				if (pixCount == (bits * 8 + 16)) 
					stop = true;
				if (pixCount == vect.size() * 8) break;
			}
		}
	}
	cout << "After decoding DCT" << endl;
	return result;
}
//
void encodeDFT(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key) {
	vector<complex<double>**> matrixes;
	cout << "Before DFT" << endl;
	int count = 0;
	while (count < vect.size() * 8) {
		complex<double>** res = new complex<double>*[8];
		for (int z = 0; z < 8; z++) {
			res[z] = new complex<double>[8];
		}
		DFT(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
		matrixes.push_back(res);
		count++;

	}
	cout << "After DFT" << endl;
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
	while (indCount < vect.size() * 8) {
		IDFT(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
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
	cout << "After IDFT" << endl;
	return;


}
//
string decodeDFT(int height, int width, RGB** pixels, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
	bitset<8> read;
	bitset<16> readSize;
	//
	bool stop = false;
	int pixCount = 0;
	int bits = 1000;
	string result = "";
	for (int x = 0; x < height; x += 8) {
		//if (pixCount == (bits * 8 + 16)) break;
		if (pixCount == vect.size() * 8) break;
		for (int y = 0; y < width; y += 8) {
			complex<double>** res = new complex<double>*[8];
			for (int z = 0; z < 8; z++) {
				res[z] = new complex<double>[8];
			}
			if (width > 8 * (key[pixCount] / (width / 8)))
				DFT(pixels, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			else
				return "Error! " + result;
			complex<double> cf1 = res[4][3];
			DFT(pixelsNew, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
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
					if(!stop)
						result += read.to_ulong();
					vectSzhat.push_back(read);
					//cout << result << endl;
				}
				if (pixCount == (bits * 8 + 16)) 
					stop = true;
				if (pixCount == vect.size() * 8) break;
			}
		}
	}
	cout << "After decoding DFT" << endl;
	return result;
}
void encodeDCTKoch(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key)
{
	vector<double**> matrixes;
	cout << "Before DCT" << endl;
	int count = 0;
	while (count < vect.size() * 8) {
		double** res = new double* [8];
		for (int z = 0; z < 8; z++) {
			res[z] = new double[8];
		}
		DCT(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
		matrixes.push_back(res);
		count++;

	}
	cout << "After DCT" << endl;
	int pixCount = 0;
	while ((pixCount / 8) < vect.size()) {
		double abs = fabs(matrixes[pixCount][4][3] - matrixes[pixCount][3][4]);
		double tmp = matrixes[pixCount][4][3];
		double tmp2 = matrixes[pixCount][3][4];
		if (1) {
			int bit = 0;
			if (pixCount < 16) bit = secr_size[pixCount];
			else bit = vect[pixCount / 8][pixCount % 8];
			if (bit == 1) {
				if (abs <= difference)
				{
					//int chhh = 0;
				}
				else
				{
					int sign = 0;
					if (fabs(tmp) > fabs(tmp2))
						sign = (matrixes[pixCount][4][3] < 0) ? -1 : +1;
					else
						sign = (matrixes[pixCount][3][4] < 0) ? -1 : +1;
					matrixes[pixCount][4][3] = (fabs(matrixes[pixCount][4][3]) >= fabs(matrixes[pixCount][3][4]))
						? matrixes[pixCount][4][3] + ((abs - difference) / 2 + 0.5) * (-sign)
						: matrixes[pixCount][4][3] + ((abs - difference) / 2 + 0.5) * sign;
					//
					//sign = (matrixes[pixCount][3][4] < 0) ? -1 : +1;
					matrixes[pixCount][3][4] = (fabs(matrixes[pixCount][3][4]) > fabs(tmp))
						? matrixes[pixCount][3][4] + ((abs - difference) / 2 + 0.5) * (-sign)
						: matrixes[pixCount][3][4] + ((abs - difference) / 2 + 0.5) * sign;
					double abs1 = fabs(matrixes[pixCount][4][3] - matrixes[pixCount][3][4]);
					sign = 0;
					if (abs1 > difference)
					{
						sign = 0;
					}
					if (abs1 > 6.8)
					{
						sign = 0;
					}
				}
			}
			else {
				if (abs > difference)
				{
					//int chh = 0;
				}
				else
				{
					int sign = 0;
					if(fabs(tmp) > fabs(tmp2))
						sign = (matrixes[pixCount][4][3] < 0) ? -1 : +1;
					else
						sign = (matrixes[pixCount][3][4] < 0) ? -1 : +1;
					matrixes[pixCount][4][3] = (fabs(matrixes[pixCount][4][3]) >= fabs(matrixes[pixCount][3][4]))
						? matrixes[pixCount][4][3] + (difference / 2 + 1) * sign
						: matrixes[pixCount][4][3] + (difference / 2 + 1) * (-sign);
					//
					//sign = (matrixes[pixCount][3][4] < 0) ? -1 : +1;
					matrixes[pixCount][3][4] = (fabs(matrixes[pixCount][3][4]) > fabs(tmp))
						? matrixes[pixCount][3][4] + (difference / 2 + 1) * sign
						: matrixes[pixCount][3][4] + (difference / 2 + 1) * (-sign);
					double abs1 = fabs(matrixes[pixCount][4][3] - matrixes[pixCount][3][4]);
					sign = 0;
					if (abs1 <= difference)
					{
						sign = 0;
					}
					if (abs1 < 9.5)
					{
						sign = 0;
					}
				}
			}
		}
		pixCount++;
	}
	int indCount = 0;
	while (indCount < vect.size() * 8) {
		IDCT(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
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
	cout << "After IDCT" << endl;
	return;
}
string decodeDCTKoch(int height, int width, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, int difference, vector<int> key)
{
	bitset<8> read;
	bitset<16> readSize;
	//
	bool stop = false;
	int pixCount = 0;
	int bits = 1000;
	string result = "";
	for (int x = 0; x < height; x += 8) {
		//if (pixCount == (bits * 8 + 16)) break;
		if (pixCount == vect.size() * 8) break;
		for (int y = 0; y < width; y += 8) {
			double** res = new double* [8];
			for (int z = 0; z < 8; z++) {
				res[z] = new double[8];
			}
			//DCT(pixels, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			if (width > 8 * (key[pixCount] / (width / 8)))
				DCT(pixelsNew, res, 8 * (key[pixCount] / (width / 8)), 8 * (key[pixCount] % (width / 8)));
			else
				return "Error! " + result;
			double cf1 = res[4][3];
			double cf2 = res[3][4];
			for (int z = 0; z < 8; z++) {
				delete[] res[z];
			}
			delete[] res;
			double abs = fabs(cf1 - cf2);
			if (pixCount < 16) {
				if (abs > difference) 
					readSize[pixCount] = 0;
				else 
					readSize[pixCount] = 1;
				pixCount++;
				if (pixCount == 16) {
					bits = readSize.to_ulong();
					cout << "bits = " << bits << endl;
				}
			}
			else {
				if (abs > difference) 
					read[pixCount % 8] = 0;
				else 
					read[pixCount % 8] = 1;
				if (read[pixCount % 8] != vect[(pixCount / 8)][pixCount % 8])
				{
					//int sign = 0;
				}
				pixCount++;
				if ((pixCount % 8) == 0) {
					if(!stop)
						result += read.to_ulong();
					vectSzhat.push_back(read);
					if (vect[(pixCount / 8) - 1] != vectSzhat[(pixCount / 8) - 1])
					{
						//int sign = 0;
					}
				}
				if (pixCount == (bits * 8 + 16)) 
					stop = true;
				if (pixCount == vect.size() * 8) break;
			}
		}
	}
	cout << "After decoding DCT" << endl;
	return result;
}
//
void encodeLSB(int width, RGB** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size) {
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
				}
				else {
					pixelsNew[index / width][index % width].blue &= ~(1);
				}
			}
			else
			{
				if ((vect[i / 8][i % 8] == 1))
				{
					pixelsNew[index / width][index % width].blue |= 1;
				}
				else {
					pixelsNew[index / width][index % width].blue &= ~(1);
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
string decodeLSB(int width, RGB** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat) {
	BYTE channelByte;
	ifstream keyRead1("key.txt");
	if (!(keyRead1.is_open())) {
		cout << "Error while opening file." << endl;
	}
	//
	bool stop = false;
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
				if(!stop)
					result += read.to_ulong();
				vectSzhat.push_back(read);
			}
			if (pixCount == (bits * 8 + 16)) 
				stop = true;
		}
	}
	return result;
}
//
RGB** ReadFile(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	FILE* _file = _wfopen(_filename, L"rb");
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
	retStr += juce::String((std::wstring(L"Размер = ")).c_str());
	retStr += juce::String(to_string(bmfHeader.bfSize));
	retStr += juce::String((std::wstring(L" байт")).c_str());
	retStr += "\n";
	retStr += juce::String((std::wstring(L"Ширина = ")).c_str());
	retStr += juce::String(to_string(bmiHeader.biWidth));
	retStr += juce::String((std::wstring(L" пикселей")).c_str());
	retStr += "\n";
	retStr += juce::String((std::wstring(L"Высота = ")).c_str());
	retStr += juce::String(to_string(bmiHeader.biHeight));
	retStr += juce::String((std::wstring(L" пикселей")).c_str());
	retStr += "\n";
	retStr += "bibit = ";
	retStr += juce::String(to_string(bmiHeader.biBitCount));
	retStr += "\n";
	retStr += "biclr = ";
	retStr += juce::String(to_string(bmiHeader.biClrUsed));
	retStr += "\n";
	h = bmiHeader.biHeight;
	w = bmiHeader.biWidth;
	size = bmiHeader.biWidth * bmiHeader.biHeight;
	//
	if ((size < 100) || (h < 32) || (h > 5000) || (w < 32) || (w > 5000))
	{
		retStr += "Bad file!!!\n";
		fclose(_file);
		retStr1 = retStr;
		return NULL;
	}
	retStr1 = retStr;
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
FILE* Create_File(const char* _filename, const wchar_t* _origFile)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	FILE* origFile = _wfopen(_origFile, L"rb");
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
	random_device gen;
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
			rand_num = gen() % (size / 64);
		}
		else
		{
			rand_num = gen() % size;
		}
		if (count(key.begin(), key.end(), rand_num) == 0) {
			key.push_back(rand_num);
			keyFile << rand_num << " ";
			i++;
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
vector<bitset<8>> ReadWord(const wchar_t* _filename, string& message)
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
		secr += "\n";
	}
	//
	message = secr;
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
void CreateDiffFile(const wchar_t* _filename1, const wchar_t* _filename2, const char* _newfile)
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
				int diff = (int)fabs(pixels1[i][j].blue - pixels2[i][j].blue);
				//
				pixelsNew[i][j].red = sat((double)(0 + 100 * ( (diff - 2))));
				pixelsNew[i][j].green = sat((double)(255 - 200 * (diff - 1)));
				pixelsNew[i][j].blue = sat((double)(255 * (diff > 1) - 100 * (diff - 2)));
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