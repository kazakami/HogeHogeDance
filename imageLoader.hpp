#pragma once


#include <vector>
#include <string>

/*
const char * filenameTGA = "/export/home/012/a0126218/tmp/MarisaV12/E_R.tga";
const char * filenameBMP = "/export/home/012/a0126218/tmp/MarisaV12/a.sph";
const char * filenamePNG = "/export/home/012/a0126218/tmp/MarisaV12/Kami_F5.png";
*/

namespace kazakami
{

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;


struct tgaHeader
{
  uchar IDfieldLength;
  uchar ColorMapType;
  uchar ImageType;
  ushort ColorMapIndex;
  ushort ColorMapLength;
  uchar ColorMapSize;
  ushort ImageOriginX;
  ushort ImageOriginY;
  ushort ImageWidth;
  ushort ImageHeight;
  uchar BitPerPixel;
  uchar Discripter;
};

struct bmpFileHeader
{
  uchar bfType1;
  uchar bfType2;
  uint bfSize;
  ushort bfReserved1;
  ushort bfReserved2;
  uint bfOffBits;
};

struct bmpInformationHeader
{
  uint biSize;
  int biWidth;
  int biHeight;
  ushort biPlanes;
  ushort biBitCount;
  uint biCompression;
  uint biSizeImage;
  int biXPisPerMeter;
  int biYPisPerMeter;
  uint biClrUsed;
  uint biCirImportant;
};



class imageLoader
{
private:
  int sx, sy;
  uint dataSize;

  template <class T>
  std::vector<T> evilInTheMirror(const std::vector<T> & vec, uint width, uint height, uint depth)
  {
    std::vector<T> data;
    data.reserve(width * height * depth);
    for (uint i = 0; i < height; i++)
    {
      uint pos = width * depth * (height - i - 1);
      for (uint j = 0; j < width * depth; j++)
	data.push_back(vec[pos + j]);
    }
    return data;
  }

  void owata(const std::string &  str);
  
  tgaHeader readTGAHeader(std::ifstream * ifs);
  bmpFileHeader readBMPFileHeader(std::ifstream * ifs);
  bmpInformationHeader readBMPInformationHeader(std::ifstream * ifs);

public:
  std::vector<uchar> data;
  std::vector<uchar> BGR_data() const;
  //拡張子によってファイル形式を判定して適切な読み込みルーチンを呼び出す。
  int load(const std::string & filename);
  int loadPNG(const std::string & filename);
  int loadPNG(const char * filename);
  int loadBMP(const std::string & filename);
  int loadBMP(const char * filename);
  int loadTGA(const std::string & filename);
  int loadTGA(const char * filename);
  uint Width() const;
  uint Height() const;
  uint Size() const;
  const std::vector<uchar> & GetData() const;
  imageLoader()
    :sx(0), sy(0), dataSize(0), data(){}

};




}
