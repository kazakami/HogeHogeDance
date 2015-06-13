#include <iostream>
#include <fstream>
#include <sstream>

#include <memory>
#include <png.h>

#include <GL/glut.h>


#include "imageLoader.hpp"


//visual c++でfopenを使うため。
#ifdef _WIN32
#pragma warning ( disable : 4996 )
#endif

namespace kazakami
{


tgaHeader imageLoader::readTGAHeader(std::ifstream * ifs)
{
  tgaHeader th;
  ifs->read((char*)(&th.IDfieldLength), 1);
  ifs->read((char*)(&th.ColorMapType), 1);
  ifs->read((char*)(&th.ImageType), 1);
  ifs->read((char*)(&th.ColorMapIndex), 2);
  ifs->read((char*)(&th.ColorMapLength), 2);
  ifs->read((char*)(&th.ColorMapSize), 1);
  ifs->read((char*)(&th.ImageOriginX), 2);
  ifs->read((char*)(&th.ImageOriginY), 2);
  ifs->read((char*)(&th.ImageWidth), 2);
  ifs->read((char*)(&th.ImageHeight), 2);
  ifs->read((char*)(&th.BitPerPixel), 1);
  ifs->read((char*)(&th.Discripter), 1);
  return th;
}


bmpFileHeader imageLoader::readBMPFileHeader(std::ifstream * ifs)
{
  bmpFileHeader bfh;
  ifs->read((char*)(&bfh.bfType1), 1);
  ifs->read((char*)(&bfh.bfType2), 1);
  ifs->read((char*)(&bfh.bfSize), 4);
  ifs->read((char*)(&bfh.bfReserved1), 2);
  ifs->read((char*)(&bfh.bfReserved2), 2);
  ifs->read((char*)(&bfh.bfOffBits), 4);
  return bfh;
}


bmpInformationHeader imageLoader::readBMPInformationHeader(std::ifstream * ifs)
{
  bmpInformationHeader bih;
  ifs->read((char*)(&bih.biSize), 4);
  if (bih.biSize != 40)
  {
    std::cerr << "nueee" << std::endl;
    exit(1);
  }
  ifs->read((char*)(&bih.biWidth), 4);
  ifs->read((char*)(&bih.biHeight), 4);
  ifs->read((char*)(&bih.biPlanes), 2);
  ifs->read((char*)(&bih.biBitCount), 2);
  ifs->read((char*)(&bih.biCompression), 4);
  ifs->read((char*)(&bih.biSizeImage), 4);
  ifs->read((char*)(&bih.biXPisPerMeter), 4);
  ifs->read((char*)(&bih.biYPisPerMeter), 4);
  ifs->read((char*)(&bih.biClrUsed), 4);
  ifs->read((char*)(&bih.biCirImportant), 4);
  return bih;
}


int imageLoader::load(const std::string & filename)
{
  //単純に拡張子しか見ない。
  const auto ext = filename.substr(filename.find_last_of('.'));
  if (ext == ".bmp" || ext == ".sph") //.sphの中身は.bmpファイルだったよ
    return loadBMP(filename);
  else if (ext == ".tga")
    return loadTGA(filename);
  else if (ext == ".png")
    return loadPNG(filename);
  else
  {
    std::cerr << "No such a extension" << std::endl;
    exit(1);
    return -1;
  }
}

int imageLoader::loadPNG(const std::string & filename)
{
  return loadPNG(filename.c_str());
}

int imageLoader::loadPNG(const char * filename)
{
  FILE *fp = fopen(filename, "rb");
  if (!fp)
    owata("cannot open file");

  char header[8];
  if (fread(header, 1, 8, fp) < 8)
    owata("can not file header");
  if (png_sig_cmp((png_bytep)header, 0, 8))
    owata("it`s not png fle");
  //std::cerr << "png" << std::endl;
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    owata("png_create_read_struct fail in making png struct");

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    owata("png_create_info_struct fail in making info struct");
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    owata("png_create_info_struct fail in making end info struct");
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    owata("");
  }
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  //read
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);

  sx = (int)png_get_image_width(png_ptr, info_ptr);
  sy = (int)png_get_image_height(png_ptr, info_ptr);

  auto pngColorType = png_get_color_type(png_ptr, info_ptr);
  
  if (pngColorType == PNG_COLOR_TYPE_RGB_ALPHA)
  {
    dataSize = sx * sy * 4;

    png_byte** row_pointers;
    row_pointers = png_get_rows(png_ptr, info_ptr);
    
    data.reserve(dataSize);
    //img = new uchar[dataSize];
    
    for (int j = 0; j < sy; j++)
    {
      for (int i = 0; i < sx; i++)
      {
	uchar b = std::min((int)row_pointers[j][i*4+0], 255);
	uchar g = std::min((int)row_pointers[j][i*4+1], 255);
	uchar r = std::min((int)row_pointers[j][i*4+2], 255);
	uchar a = std::min((int)row_pointers[j][i*4+3], 255);
	data.push_back(b);
	data.push_back(g);
	data.push_back(r);
	data.push_back(a);
      }
    }
  }
  else if (pngColorType == PNG_COLOR_TYPE_RGB)
  {
    dataSize = sx * sy * 4;

    png_byte ** row_pointers;
    row_pointers = png_get_rows(png_ptr, info_ptr);

    data.reserve(dataSize);

    for (int j = 0; j < sy; j++)
    {
      for (int i = 0; i < sx; i++)
      {
	uchar b = std::min((int)row_pointers[j][i*3+0], 255);
	uchar g = std::min((int)row_pointers[j][i*3+1], 255);
	uchar r = std::min((int)row_pointers[j][i*3+2], 255);
	uchar a = 255;
	data.push_back(b);
	data.push_back(g);
	data.push_back(r);
	data.push_back(a);
      }
    }
  }
  else
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    owata("Not supported color type");
  }




  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

  //data = evilInTheMirror(data, sx, sy, 4);

  return 0;
}

int imageLoader::loadBMP(const std::string & filename)
{
  return loadBMP(filename.c_str());
}


int imageLoader::loadBMP(const char * filename)
{
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs)
    exit(1);

  //bmpFileHeader bfh = readBMPFileHeader(&ifs);
  readBMPFileHeader(&ifs);
  //std::cout << bfh.bfType1 << bfh.bfType2 << std::endl;
  
  bmpInformationHeader bih = readBMPInformationHeader(&ifs);
  //std::cout << bih.biWidth << "x" << bih.biHeight << std::endl;
  //std::cout << "bit : " << bih.biBitCount << std::endl;
  //std::cout << "comp : " << bih.biCompression << std::endl;

  if (bih.biCompression != 0)
  {
    std::cerr << "This file was compressed." << std::endl;
    std::cerr << "File name : " << filename << std::endl;
    exit(1);
  }

  if (bih.biBitCount == 8)
  {
    std::vector<std::tuple<uchar, uchar, uchar>> palette;

    for (int i = 0; i < 256; i++)
    {
      uchar b, g, r;
      ifs.read((char*)(&b), 1);
      ifs.read((char*)(&g), 1);
      ifs.read((char*)(&r), 1);
      ifs.seekg(1, std::ios_base::cur);
      palette.emplace_back(b, g, r);
    }
    uint padSize = (bih.biWidth * (bih.biBitCount / 4)) % 4;
    dataSize = bih.biWidth * bih.biHeight * 4;//BGRA
    data.reserve(dataSize);

    for (int i = 0; i < bih.biHeight; i++)
    {
      for (int j = 0; j < bih.biWidth; j++)
      {
	uchar number;
	ifs.read((char*)(&number), 1);
	const auto pal = palette[(int)number];
	data.push_back(std::get<0>(pal));
	data.push_back(std::get<1>(pal));
	data.push_back(std::get<2>(pal));
	data.push_back(255);//Alpha
      }
      ifs.seekg(padSize, std::ios_base::cur);
    }
  }
  else if (bih.biBitCount == 24)
  {
    uint padSize = (bih.biWidth * (bih.biBitCount / 4)) % 4;
    dataSize = bih.biWidth * bih.biHeight * 4;//BGRA
    data.reserve(dataSize);
    
    for (int i = 0; i < bih.biHeight; i++)
    {
      for (int j = 0; j < bih.biWidth; j++)
      {
	uchar b, g, r;
	ifs.read((char*)(&b), 1);
	ifs.read((char*)(&g), 1);
	ifs.read((char*)(&r), 1);
	data.push_back(b);
	data.push_back(g);
	data.push_back(r);
	data.push_back(255);//Alpha
      }
      ifs.seekg(padSize, std::ios_base::cur);
    }   
  }
  else if (bih.biBitCount == 32)
  {
    dataSize = bih.biWidth * bih.biHeight * 4;//BGRA
    data.reserve(dataSize);
    
    for (int i = 0; i < bih.biHeight; i++)
    {
      for (int j = 0; j < bih.biWidth; j++)
      {
	uchar b, g, r;
	ifs.read((char*)(&b), 1);
	ifs.read((char*)(&g), 1);
	ifs.read((char*)(&r), 1);
	ifs.seekg(1, std::ios_base::cur);
	data.push_back(b);
	data.push_back(g);
	data.push_back(r);
	data.push_back(255);//Alpha
      }
    }   

  }
  else
  {
    std::cerr << "Bit Count should be 24 or 32!!" << std::endl;
    std::cerr << "This file's bit count is " << bih.biBitCount << std::endl;
    std::cerr << "File name : " << filename << std::endl;
    exit(1);
  }
  

  sx = bih.biWidth;
  sy = bih.biHeight;

  data = evilInTheMirror(data, sx, sy, 4);
  
  //for (uint i = 0; i < dataSize; i++) img[i] = data[i];
  

  return 0;
}

int imageLoader::loadTGA(const std::string & filename)
{
  return loadTGA(filename.c_str());
}


int imageLoader::loadTGA(const char * filename)
{
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs)
  {
    std::cerr << "No such a file : " << filename << std::endl;
    exit(1);
  }
  
  std::string str;

  tgaHeader th = readTGAHeader(&ifs);
  //std::cout << (int)th.BitPerPixel << std::endl;
  //std::cout << th.ImageWidth << "x" << th.ImageHeight << std::endl;
  //std::cout << "bit : " << (int)th.BitPerPixel << std::endl;
  dataSize = ((int)th.BitPerPixel / 8) * th.ImageWidth * th.ImageHeight;
  sx = th.ImageWidth;
  sy = th.ImageHeight;
  data.reserve(dataSize);
  //img = new uchar[dataSize];

  if (th.ImageType == 0x02) //full color 24bit
  {
    for (uint i = 0; i < dataSize; i++)
    {
      uchar d;
      ifs.read((char*)(&d), 1);
      data.push_back(d);
    }
  }
  else if (th.ImageType == 0x0a) //full color RLE
  {
    while (data.size() != dataSize)
    {
      uchar length;
      ifs.read((char*)(&length), 1);
      //続くかどうか。最上位ビットが1
      if (length >> 7)
      {
	//uchar l = (length & 0b01111111) + 1;
	uchar l = (length & 0x7f) + 1;
	uchar b, g, r, a;
	ifs.read((char*)(&b), 1);
	ifs.read((char*)(&g), 1);
	ifs.read((char*)(&r), 1);
	ifs.read((char*)(&a), 1);
	for (uchar i = 0; i < l; i++)
        {
	  data.push_back(b);
	  data.push_back(g);
	  data.push_back(r);
	  data.push_back(a);
	}
      }
      else
      {
	//uchar l = (length & 0b01111111) + 1;
	uchar l = (length & 0x7f) + 1;
	for (uchar i = 0; i < l; i++)
        {
	  uchar b, g, r, a;
	  ifs.read((char*)(&b), 1);
	  ifs.read((char*)(&g), 1);
	  ifs.read((char*)(&r), 1);
	  ifs.read((char*)(&a), 1);
	  data.push_back(b);
	  data.push_back(g);
	  data.push_back(r);
	  data.push_back(a);	
	}
      }
    }
  }
  else
  {
    std::cerr << "pueeeeee" << std::endl;
    exit(1);
  }

  //std::cout << data.size() << std::endl;
  //std::cout << dataSize << std::endl;

  data = evilInTheMirror(data, sx, sy, 4);

  //for (uint i = 0; i < dataSize; i++) img[i] = data[i];

  ifs.close();

  return 0;
}



uint imageLoader::Width() const
{
  return sx;
}
uint imageLoader::Height() const
{
  return sy;
}
uint imageLoader::Size() const
{
  return dataSize;
}

void imageLoader::owata(const std::string & str)
{
  std::cerr << str << std::endl;
  exit(1);
}

const std::vector<uchar> & imageLoader::GetData() const
{
  return data;
}

std::vector<uchar> imageLoader::BGR_data() const
{
  std::vector<uchar> d;
  for (int j = 0; j < sy; j++)
    for (int i = 0; i < sx; i++)
    {
      d.push_back(data[j * sx * 4 + i * 4 + 2]);
      d.push_back(data[j * sx * 4 + i * 4 + 1]);
      d.push_back(data[j * sx * 4 + i * 4 + 0]);
    }
  return d;
}


}

//test code

/*

unsigned char * img;
uint sx, sy;


//------ render_viewer functions ------------------
static void redraw_image(void)
{
// Just for Safe
  glPushMatrix();{

    // Just for Safe Again
    glLoadIdentity();

    // With Identity Projection Matrix,
    // Window Screen Range is (-1,-1)[bottomleft] to (1,1)[upright].
    // Write the Buffer from (-1,-1), bottom left.
    glRasterPos3d(-1.0, -1.0, 0);
    
    
    glDrawPixels(sx, sy, GL_BGRA, GL_UNSIGNED_BYTE,
                 (const GLvoid *)(img));
    
    glPopMatrix();
  }


  // Double Buffering
  glutSwapBuffers();
}




int main(int argc, char* argv[])
{

  kazakami::imageLoader picture;
  picture.loadPNG("/home/kazakami/tmp/MarisaV12/MarisaV12/Kami_F5.png");
  img = new unsigned char[picture.Size()];
  sx = picture.Width();
  sy = picture.Height();
  for (uint i = 0; i < picture.Size(); i++)
    img[i] = picture.data[i];

  // Prepareing the GL and GLUT
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(sx, sy);
  glutCreateWindow("LE4 CG");
  glutDisplayFunc(redraw_image);
  glutIdleFunc(NULL);

  // MainLoop
  glutMainLoop();


  return 0;
}

*/

