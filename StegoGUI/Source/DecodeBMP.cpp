#include "../Include/DecodeBMP.h"

unsigned decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp)
{
   static const unsigned MINHEADER = 54; //minimum BMP header size

   if (bmp.size() < MINHEADER) return (unsigned int) - 1;
   if (bmp[0] != 'B' || bmp[1] != 'M') return 1; //It's not a BMP file if it doesn't start with marker 'BM'
   unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
   //read width and height from BMP header
   w = bmp[18] + bmp[19] * 256;
   h = bmp[22] + bmp[23] * 256;
   //read number of channels from BMP header
   if (bmp[28] != 24 && bmp[28] != 32) return 2; //only 24-bit and 32-bit BMPs are supported.
   unsigned numChannels = bmp[28] / 8;

   //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
   //to make it a multiple of 4 bytes.
   unsigned scanlineBytes = w * numChannels;
   if (scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

   unsigned dataSize = scanlineBytes * h;
   if (bmp.size() < dataSize + pixeloffset) return 3; //BMP file too small to contain all pixels

   image.resize(w * h * 4);

   for (unsigned y = 0; y < h; y++)
      for (unsigned x = 0; x < w; x++) {
         //pixel start byte position in the BMP
         unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
         //pixel start byte position in the new raw image
         unsigned newpos = 4 * y * w + 4 * x;
         if (numChannels == 3) {
            image[newpos + 0] = bmp[bmpos + 2]; //R
            image[newpos + 1] = bmp[bmpos + 1]; //G
            image[newpos + 2] = bmp[bmpos + 0]; //B
            image[newpos + 3] = 255;            //A
         }
         else {
            image[newpos + 0] = bmp[bmpos + 2]; //R
            image[newpos + 1] = bmp[bmpos + 1]; //G
            image[newpos + 2] = bmp[bmpos + 0]; //B
            image[newpos + 3] = bmp[bmpos + 3]; //A
         }
      }
   return 0;
}

int doDecode(const wchar_t* in, const char* out)
{
   std::vector<unsigned char> bmp;
   lodepng::load_file(bmp, in);
   std::vector<unsigned char> image;
   unsigned w, h;
   unsigned error = decodeBMP(image, w, h, bmp);

   if (error) {
      std::cout << "BMP decoding error " << error << std::endl;
      return 0;
   }

   std::vector<unsigned char> png;
   error = lodepng::encode(png, image, w, h);

   if (error) {
      std::cout << "PNG encoding error " << error << ": " << lodepng_error_text(error) << std::endl;
      return 0;
   }

   lodepng::save_file(png, out);
   return 0;
}
