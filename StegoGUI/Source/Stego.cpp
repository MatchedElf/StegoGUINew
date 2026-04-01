#include "../Include/Stego.h"

BYTE sat(double x)
{
	BYTE ret = (BYTE)x;
	if (x < 0) return 0;
	if (x > 255) return 255;
	else return (BYTE)x;
}
//
RGB makeColor(BYTE r, BYTE g, BYTE b) {
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
void DCT(uint8_t** pixels, double** result, int x, int y) {
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
			double cu = coef(u);
			double cv = coef(v);
			double sum = 0;
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					sum += (double)(pixels[x + k][y + l] * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16));
				}
			}
			result[u][v] = cu * cv * sum;
		}
	}
}
//
void IDCT(uint8_t** pixels, double** result, int x, int y) {
	for (int k = 0; k < 8; k++) {
		for (int l = 0; l < 8; l++) {
			double sum = 0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					sum += (double)(coef(u) * coef(v) * result[u][v] * cos((2 * k + 1) * u * PI / 16) * cos((2 * l + 1) * v * PI / 16));
				}
			}
			pixels[x + k][y + l] = sat(sum);
		}
	}

}
//
void DFT(uint8_t** pixels, complex<double>** result, int x, int y) {
	for (int u = 0; u < 8; u++) {
		for (int v = 0; v < 8; v++) {
			complex<double> sum(0.0, 0.0);
			complex<double> NN(64.0, 0.0);
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					complex<double> pixel(pixels[x + k][y + l], 0.0);
					complex<double> e(cos(2 * PI * (u * k / 8.0 + v * l / 8.0)), -sin(2 * PI * (u * k / 8.0 + v * l / 8.0)));
					sum += pixel * e;
				}
			}
			result[u][v] = sum / NN;
		}
	}
}
//
void IDFT(uint8_t** pixels, complex<double>** result, int x, int y) {
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
			pixels[x + k][y + l] = sat((double)sum.real());
		}
	}
}
//
long double PSNR(uint8_t** orig, uint8_t** re, int height, int width) {
	long double znam = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			znam += pow((orig[i][j] - re[i][j]), 2);
		}
	}
	return 10 * log10(height * width * pow(pow(2, 8) - 1, 2) / znam);

}
double averageIntensity(uint8_t** orig, int height, int width)
{
	double ret = 0.0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			ret += orig[i][j];
		}
	}
	return ret / (width * height);
}
//
double corrCoef(uint8_t** orig, uint8_t** re, int height, int width)
{
	double origAv = averageIntensity(orig, height, width);
	double reAv = averageIntensity(re, height, width);
	double chisl = 0.0;
	//
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			chisl += (orig[i][j] - origAv) * (re[i][j] - reAv);
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
			origSum += pow((orig[i][j] - origAv), 2);
		}
	}
	double reSum = 0.0;
	//
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			reSum += pow((re[i][j] - reAv), 2);
		}
	}
	znam = sqrt(origSum * reSum);
	return chisl / znam;
}
//
void encodeDCT(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key) {
	vector<double**> matrixes;
	cout << "Before DCT" << endl;
	int count = 0;
	//difference = 0;
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
string decodeDCT(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
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
void encodeDFT(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, complex<double> difference, vector<int> key) {
	vector<complex<double>**> matrixes;
	cout << "Before DFT" << endl;
	int count = 0;
	//difference = 0;
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
string decodeDFT(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
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
void encodeDCTKoch(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size, int difference, vector<int> key)
{
	vector<double**> matrixes;
	cout << "Before DCT" << endl;
	//difference = 0;
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
string decodeDCTKoch(int height, int width, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, int difference, vector<int> key)
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
void encodeLSB(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, bitset<16> secr_size) {
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
					pixelsNew[index / width][index % width] |= 1;
				}
				else {
					pixelsNew[index / width][index % width] &= ~(1);
				}
			}
			else
			{
				if ((vect[i / 8][i % 8] == 1))
				{
					pixelsNew[index / width][index % width] |= 1;
				}
				else {
					pixelsNew[index / width][index % width] &= ~(1);
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
string decodeLSB(int width, uint8_t** pixelsNew, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat) {
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
		channelByte = pixelsNew[index / width][index % width];
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
// Добавьте эти функции в stego.cpp

// Прямое вейвлет-преобразование Хаара для блока 8x8
void HaarWavelet(uint8_t** pixels, double** result, int x, int y) {
	double temp[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i][j] = (double)pixels[x + i][y + j];
		}
	}

	for (int i = 0; i < 8; i++) {
		int step = 8;
		while (step > 1) {
			int halfStep = step / 2;
			for (int j = 0; j < halfStep; j++) {
				double a = temp[i][j * 2];
				double b = temp[i][j * 2 + 1];
				temp[i][j] = (a + b) / sqrt(2.0);
				temp[i][j + halfStep] = (a - b) / sqrt(2.0);
			}
			step = halfStep;
		}
	}

	for (int j = 0; j < 8; j++) {
		double col[8];
		for (int i = 0; i < 8; i++) {
			col[i] = temp[i][j];
		}

		int step = 8;
		while (step > 1) {
			int halfStep = step / 2;
			for (int i = 0; i < halfStep; i++) {
				double a = col[i * 2];
				double b = col[i * 2 + 1];
				col[i] = (a + b) / sqrt(2.0);
				col[i + halfStep] = (a - b) / sqrt(2.0);
			}
			step = halfStep;
		}

		for (int i = 0; i < 8; i++) {
			double orig = (double)pixels[x + i][y + j];
			double tmp = col[i];
			result[i][j] = col[i];
		}
	}
}

// Обратное вейвлет-преобразование Хаара для блока 8x8
void IHaarWavelet(uint8_t** pixels, double** result, int x, int y) {
	double temp[8][8];

	// Копируем коэффициенты
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i][j] = result[i][j];
		}
	}

	// Обратное преобразование по столбцам
	for (int j = 0; j < 8; j++) {
		double col[8];
		for (int i = 0; i < 8; i++) {
			col[i] = temp[i][j];
		}

		int step = 1;
		while (step < 8) {
			int halfStep = step;
			step = step * 2;
			for (int i = 0; i < halfStep; i++) {
				double a = col[i];
				double b = col[i + halfStep];
				col[i * 2] = (a + b) / sqrt(2.0);
				col[i * 2 + 1] = (a - b) / sqrt(2.0);
			}
		}

		for (int i = 0; i < 8; i++) {
			temp[i][j] = col[i];
		}
	}

	// Обратное преобразование по строкам
	for (int i = 0; i < 8; i++) {
		int step = 1;
		while (step < 8) {
			int halfStep = step;
			step = step * 2;
			double row[8];
			for (int j = 0; j < 8; j++) {
				row[j] = temp[i][j];
			}

			for (int j = 0; j < halfStep; j++) {
				double a = row[j];
				double b = row[j + halfStep];
				temp[i][j * 2] = (a + b) / sqrt(2.0);
				temp[i][j * 2 + 1] = (a - b) / sqrt(2.0);
			}
		}
	}

	// Записываем результат обратно в пиксели
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			BYTE tmp = sat(temp[i][j]);
			pixels[x + i][y + j] = sat(temp[i][j]);			
		}
	}
}

void HaarWaveletFull(uint8_t** pixels, uint8_t** result, int x, int y)
{
	double** temp = new double* [512];
	for (int z = 0; z < 512; z++) {
		temp[z] = new double[512];
	}
	double** res = new double* [512];
	for (int z = 0; z < 512; z++) {
		res[z] = new double[512];
	}
	// Копируем значения синего канала
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			temp[i][j] = (double)pixels[i][j];
		}
	}
	double min{ 0.0 }, max{ 0.0 };
	// Применяем преобразование Хаара по строкам
	for (int i = 0; i < 512; i++) {
		int step = 512;
		while (step > 1) {
			int halfStep = step / 2;
			for (int j = 0; j < halfStep; j++) {
				double a = temp[i][j * 2];
				double b = temp[i][j * 2 + 1];
				temp[i][j] = (a + b) / sqrt(2.0);  // Низкочастотная составляющая
				temp[i][j + halfStep] = (a - b) / sqrt(2.0);  // Высокочастотная составляющая
			}
			step = halfStep;
		}
	}

	// Применяем преобразование Хаара по столбцам
	for (int j = 0; j < 512; j++) {
		double col[512];
		for (int i = 0; i < 512; i++) {
			col[i] = temp[i][j];
		}

		int step = 512;
		while (step > 1) {
			int halfStep = step / 2;
			for (int i = 0; i < halfStep; i++) {
				double a = col[i * 2];
				double b = col[i * 2 + 1];
				col[i] = (a + b) / sqrt(2.0);
				col[i + halfStep] = (a - b) / sqrt(2.0);
			}
			step = halfStep;
		}

		for (int i = 0; i < 512; i++) {
			double orig = (double)pixels[i][j];
			double tmp = col[i];
			if (col[i] < min)
				min = col[i];
			if (col[i] > max)
				max = col[i];
			res[i][j] = col[i];
		}
	}
	normalizeForDisplay(res, result);
	for (int i = 0; i < 512; i++)
		delete[] temp[i];
	delete[] temp;

}

void IHaarWaveletFull(uint8_t** pixels, uint8_t** result, int x, int y)
{
}

// Внедрение с использованием вейвлет-преобразования Хаара
void encodeHaar(int width, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, bitset<16> secr_size, double difference, vector<int> key) {
	vector<double**> matrixes;
	dwt::FloatMatrix floatRes;
	uint8_t** res = new uint8_t * [width];
		for (int z = 0; z < width; z++) {
			res[z] = new uint8_t[width];
		}
	cout << "Before Haar Wavelet" << endl;
	//HaarWaveletFull(pixelsNew, res, 0, 0);
	dwt::dwt2d(pixelsNew, res, width, width, dwt::Wavelet::HAAR);
	floatRes = dwt::dwt2d_float(pixelsNew, width, width, dwt::Wavelet::HAAR);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < width; j++)
			pixelsWavelet[i][j] = res[i][j];

	//int count = 0;
	//while (count < vect.size() * 8) {
	//	uint8_t** res = new uint8_t* [512];
	//	for (int z = 0; z < 512; z++) {
	//		res[z] = new uint8_t[512];
	//	}
	//	HaarWaveletFull(pixelsNew, res, 8 * (key[count] / (width / 8)), 8 * (key[count] % (width / 8)));
	//	//matrixes.push_back(res);
	//	count++;
	//}

	cout << "After Haar Wavelet" << endl;

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
					floatRes[index / width][index % width] += difference;
				}
				else {
					floatRes[index / width][index % width] -= difference;
				}
			}
			else
			{
				if ((vect[i / 8][i % 8] == 1))
				{
					floatRes[index / width][index % width] += difference;
				}
				else {
					floatRes[index / width][index % width] -= difference;
				}
			}
		}
	}
	//for (int i = 0; i < width; i++)
	//	for (int j = 0; j < width; j++)
	//	{
	//		if((i > width / 2) && (j > width / 2))
	//			floatRes[i][j] += 10;
	//	}
			
	//int indCount = 0;
	//while (indCount < vect.size() * 8) {
	//	IHaarWavelet(pixelsNew, matrixes[indCount], 8 * (key[indCount] / (width / 8)), 8 * (key[indCount] % (width / 8)));
	//	indCount++;
	//}
	dwt::idwt2d(floatRes, pixelsNew, width, width, dwt::Wavelet::HAAR);

	cout << "After Inverse Haar Wavelet" << endl;
	for (int i = 0; i < width; i++)
		delete[] res[i];
	//
	delete[] res;
	return;
}

// Извлечение с использованием вейвлет-преобразования Хаара
string decodeHaar(int height, int width, uint8_t** pixels, uint8_t** pixelsNew, uint8_t** pixelsWavelet, vector<bitset<8>> vect, vector<bitset<8>>& vectSzhat, vector<int> key) {
	bitset<8> read;
	bitset<16> readSize;

	dwt::FloatMatrix floatResOrig;
	dwt::FloatMatrix floatResNew;

	uint8_t** res = new uint8_t * [width];
	for (int z = 0; z < width; z++) {
		res[z] = new uint8_t[width];
	}
	dwt::dwt2d(pixelsNew, res, width, width, dwt::Wavelet::HAAR);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < width; j++)
			pixelsWavelet[i][j] = res[i][j];

	floatResOrig = dwt::dwt2d_float(pixels, width, width, dwt::Wavelet::HAAR);
	floatResNew = dwt::dwt2d_float(pixelsNew, width, width, dwt::Wavelet::HAAR);

	bool stop = false;
	int pixCount = 0;
	int bits = 1000;
	string result = "";

	while(pixCount < vect.size() * 8)
	{
		int index = key[pixCount];
		double cf1 = floatResOrig[index / width][index % width];
		double cf2 = floatResNew[index / width][index % width];

		if (pixCount < 16) 
		{
			if (cf1 < cf2) readSize[pixCount] = 1;
			else readSize[pixCount] = 0;
			pixCount++;

			if (pixCount == 16) 
			{
				bits = readSize.to_ulong();
				cout << "bits = " << bits << endl;
			}
		}
		else 
		{
			if (cf1 < cf2) 
				read[pixCount % 8] = 1;
			else 
				read[pixCount % 8] = 0;
			pixCount++;

			if ((pixCount % 8) == 0) 
			{
				if (!stop)
					result += read.to_ulong();
				vectSzhat.push_back(read);
			}

			if (pixCount == (bits * 8 + 16))
				stop = true;

			if (pixCount == vect.size() * 8) break;
		}
	}

	cout << "After decoding Haar Wavelet" << endl;
	return result;
}
//
RGB** readFile(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1)
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
			pixels[indCount / w][indCount % w] = makeColor(r, g, b);
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
			pixels[indCount / w][indCount % w] = makeColor(r, g, b);
			indCount++;
		}
	}
	fclose(_file);
	return pixels;
}
uint8_t** readFileMono(const wchar_t* _filename, int& h, int& w, int& size, juce::String& retStr1)
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
	retStr += "bfOffBits = ";
	retStr += juce::String(to_string(bmfHeader.bfOffBits));
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
	uint8_t** pixels = new uint8_t * [h + 5];
	for (int i = 0; i < h + 2; i++) pixels[i] = new uint8_t[w + 5];
	//
	int depth = (bmiHeader.biBitCount == 24) ? 3 : 4;
	uint8_t color;
	int indCount = 0;
	int byteCount = 0;
	int imageOffset = bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	while (fread(&color, 1, sizeof(color), _file) > 0) {
		if (byteCount++ < imageOffset)
			continue;
		pixels[indCount / w][indCount % w] = color;
		indCount++;
	}
	fclose(_file);
	return pixels;
}
//
FILE* createFileBmp(const char* _filename, const wchar_t* _origFile)
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

	int imageOffset = bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	//imageOffset = 12;
	uint8_t header;
	int byteCount = 0;
	while (byteCount++ < imageOffset)
	{
		if (fread((LPSTR)&header, 1, sizeof(header), origFile) != sizeof(header)) {
			fputs("File info header corrupted?", stderr);
			return NULL;
		}
		int tmp = fwrite(&header, 1, sizeof(header), newFile);

	}
	
	fclose(origFile);
	return newFile;
}
FILE* CreateFileFromMono(const char* _filename, const wchar_t* _origFile)
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
	bmiHeader.biBitCount = 24;
	bmiHeader.biClrUsed = 0;
	fwrite(&bmfHeader, 1, sizeof(bmfHeader), newFile);
	fwrite(&bmiHeader, 1, sizeof(bmiHeader), newFile);
	int imageOffset = bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	//imageOffset = 12;
	uint8_t header;
	int byteCount = 0;
	while (byteCount++ < imageOffset)
	{
		if (fread((LPSTR)&header, 1, sizeof(header), origFile) != sizeof(header)) {
			fputs("File info header corrupted?", stderr);
			return NULL;
		}
		int tmp = fwrite(&header, 1, sizeof(header), newFile);

	}

	fclose(origFile);
	return newFile;
}
//
vector<int> CreateKey(const char* _filename, int size, int vectSize, Stego::AlgNum alg)
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
		if (alg == Stego::DCT ||
			alg == Stego::DFT || 
			alg == Stego::DCT_KOCH)
		{
			rand_num = gen() % (size / 64);
		}
		else if(alg == Stego::LSB)
		{
			rand_num = gen() % size;
		}
		else if (alg == Stego::HAAR)
		{
			do {
				rand_num = gen() % (size / 2) + (size / 2);
			} while (rand_num % (int)sqrt(size) < (int)sqrt(size) / 2);
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
void WriteToFileMono(FILE* newFile, uint8_t** pixels, int height, int width)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fwrite(&pixels[i][j], 1, sizeof(uint8_t), newFile);
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
	RGB** pixels1 = readFile(_filename1, height, width, size, info);
	RGB** pixels2 = readFile(_filename2, height, width, size, info); 
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
	FILE* newFile = createFileBmp(_newfile, _filename1);
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
void CreateDiffFileMono(const wchar_t* _filename1, const wchar_t* _filename2, const char* _newfile)
{
	int height;
	int width;
	int size;
	juce::String info;
	//
	uint8_t** pixels1 = readFileMono(_filename1, height, width, size, info);
	uint8_t** pixels2 = readFileMono(_filename2, height, width, size, info);
	//
	RGB** pixelsNew = new RGB * [height + 2];
	for (int i = 0; i < height + 2; i++) pixelsNew[i] = new RGB[width + 1];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pixels1[i][j] != pixels2[i][j])
			{
				int diff = (int)fabs(pixels1[i][j] - pixels2[i][j]);
				//
				pixelsNew[i][j].red = sat((double)(0 + 100 * ((diff - 2))));
				pixelsNew[i][j].green = sat((double)(255 - 200 * (diff - 1)));
				pixelsNew[i][j].blue = sat((double)(255 * (diff > 1) - 100 * (diff - 2)));
			}
			else
			{
				pixelsNew[i][j].red = pixels1[i][j];
				pixelsNew[i][j].green = pixels1[i][j];
				pixelsNew[i][j].blue = pixels1[i][j];
			}
		}
	}
	FILE* newFile = CreateFileFromMono(_newfile, _filename1);
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
void normalizeForDisplay(double** data, uint8_t** res)
{
	double minVal = data[0][0];
	double maxVal = data[0][0];

	// Находим min и max
	for (int y = 0; y < 512; y++) {
		for (int x = 0; x < 512; x++) {
			if (data[y][x] < minVal) minVal = data[y][x];
			if (data[y][x] > maxVal) maxVal = data[y][x];
		}
	}

	// Нормализуем в диапазон [0, 255]
	double range = maxVal - minVal;
	if (range == 0) range = 1;

	for (int y = 0; y < 512; y++) {
		for (int x = 0; x < 512; x++) {
			// Для коэффициентов детализации используем абсолютные значения
			// и увеличиваем контраст для лучшей видимости
			double val = data[y][x];

			// Усиливаем контраст для LH, HL, HH
			if (x >= 512 / 2 || y >= 512 / 2) {
				val = std::abs(val) * 4; // Усиление для лучшей видимости
			}

			int normVal = static_cast<int>(((val - minVal) / range) * 255);
			if (normVal < 0) normVal = 0;
			if (normVal > 255) normVal = 255;
			res[y][x] = normVal;
		}
	}
}