#include <iostream>
#include <fstream>
#include <sstream>

#include <memory>
#include <vector>
#include <algorithm>

//#include <clocale>
//#include <cstdlib>

//#include <stdlib.h>
#include <string.h>
#include <string>
//#include <locale.h>

#include <iconv.h>

#include "vmdLoader.hpp"

//const char * filename = "/home/kazakami/tmp/MoriyaStepA.vmd";



namespace kazakami
{

std::string sjis_to_utf8(const char *pAnsiStr)
{
  if (!pAnsiStr) return NULL;

  //参考にしました。
  //http://d.hatena.ne.jp/kaorun55/20100312/1268413354
  //const uint MAX_BUF = strlen(pAnsiStr)*2;
  const static uint MAX_BUF = 1024;
  char inbuf[MAX_BUF+1] = { 0 };
  char outbuf[MAX_BUF+1] = { 0 };
  //char * inbuf = new char[MAX_BUF+1];
  //char * outbuf = new char[MAX_BUF+1];
  //std::unique_ptr<char[]> inbuf(new char[MAX_BUF+1]);
  //std::unique_ptr<char[]> outbuf(new char[MAX_BUF+1]);
  char*in = inbuf;
  char*out = outbuf;
  size_t in_size = (size_t)MAX_BUF;
  size_t out_size = (size_t)MAX_BUF;
  iconv_t ic = iconv_open("UTF-8", "SJIS");

  memcpy( in, pAnsiStr, strlen(pAnsiStr) );

  iconv( ic, &in, &in_size, &out, &out_size );
  iconv_close(ic);

  std::string str(outbuf);

  //delete[] inbuf;
  //delete[] outbuf;
  return str;

  /*
  if (setlocale(LC_ALL, "ja_JP.SJIS") == NULL)
  {
    std::cerr << "please sjis locale" << std::endl;
    exit(1);
  }

  int len = strlen(pAnsiStr);
  
  auto pUcsStr = std::unique_ptr<wchar_t[]>(new wchar_t[len+1]);;

  int num = mbstowcs(pUcsStr.get(), pAnsiStr, len+1);


  //ワイド文字をマルチバイト文字に変換するよ!!
  //UTF-16 -> UTF-8
  //参考にしました
  //http://kamoland.com/wiki/wiki.cgi?ShiftJIS%A1%A4Unicode%28UTF-8%29
  std::string str;
  for (int i = 0; i < num; i++)
  {
    wchar_t s = pUcsStr[i];
    if ((unsigned short)s <= 0x007f)
    {
      str.push_back(s & 0x007f);
    }
    else if ((unsigned short)s <= 0x07ff)
    {
      str.push_back(((s & 0x07C0) >> 6 ) | 0xc0);
      str.push_back((s & 0x003f) | 0x80);
    }
    else
    {
      str.push_back(((s & 0xf000) >> 12) | 0xe0);
      str.push_back(((s & 0x0fc0) >> 6) | 0x80);
      str.push_back((s & 0x003f) | 0x80);
    }
  }

  //std::cout << str << std::endl;

  return str;
  */
}


BoneMotionData::BoneMotionData(const std::string & n)
  :name(n),
   maxFrame(0)
{
}

Quaternion BoneMotionData::GetRotation(uint frame) const
{
  int num = data.size();
  if (num == 1 && data[0].frame < frame)
    return data[0].rotation;
  for (int i = 0; i < num-1; i ++)
  {
    if (data[i].frame <= frame && data[i+1].frame > frame)
    {
      double s = (double)(data[i+1].frame - frame)
	/ (data[i+1].frame - data[i].frame);
      Quaternion q;
      q.t = data[i].rotation.t * s
	+ data[i+1].rotation.t * (1 - s);
      q.v = blend(s, data[i].rotation.v,
		  1-s, data[i+1].rotation.v);
      return q;
    }
  }
  return IdentityQuaternion();
}

Vector3d BoneMotionData::GetPos(uint frame) const
{
  int num = data.size();
  if (num == 1 && data[0].frame < frame)
    return data[0].pos;
  for (int i = 0; i < num-1; i ++)
  {
    if (data[i].frame <= frame && data[i+1].frame > frame)
    {
      double s = (double)(data[i+1].frame - frame)
	/ (data[i+1].frame - data[i].frame);
      return blend(s, data[i].pos,
		   1-s, data[i+1].pos);
    }
  }
  return Vector3d(0, 0, 0);
}

const std::string & BoneMotionData::GetName() const
{
  return name;
}

void BoneMotionData::AddFrame(uint frame,
			      float p0, float p1, float p2,
			      float qw, float qx, float qy, float qz)
{
  frameData f = 
    {frame,
     Vector3d(p0, p1, p2),
     Quaternion(qw, qx, qy, qz)
    };
  data.push_back(f);
  if (frame > maxFrame)
    maxFrame = frame;
}

uint BoneMotionData::GetMaxFrame() const
{
  return maxFrame;
}

uint vmdLoader::GetMaxFrame() const
{
  return maxFrame;
}

void vmdLoader::Init()
{
  for (const auto & f : frameData)
  {
    const auto it = boneMotionIndexMap.find(f.boneName);
    if (it != boneMotionIndexMap.end())
    {
      //すでにある
      const int index = it->second;
      boneMotion[index].AddFrame(f.frame,
				 f.boneX, f.boneY, f.boneZ,
				 f.qW, f.qX, f.qY, f.qZ);
    }
    else
    {
      //まだない
      boneMotion.push_back(BoneMotionData(f.boneName));
      const int index = boneMotion.size() - 1;
      boneMotionIndexMap.insert(std::make_pair(f.boneName, index));
      boneMotion[index].AddFrame(f.frame,
				 f.boneX, f.boneY, f.boneZ,
				 f.qW, f.qX, f.qY, f.qZ);
    }
  }
  for (auto & b : boneMotion)
  {
    b.sort();
    b.check();
    if (b.GetMaxFrame() > maxFrame )
      maxFrame = b.GetMaxFrame();
  }
  std::cout << "Motion Length : " << maxFrame << std::endl;
  /*
  for (const auto & b : boneMotion)
  {
    std::cout << b.GetName() << std::endl;
    b.hoge();
  }
  */
}

FrameData vmdLoader::readFrameData(std::ifstream * ifs) const
{
  FrameData f;
  char boneNameCharArr[15];
  ifs->read((char*)(boneNameCharArr), 15);
  f.boneName = sjis_to_utf8(boneNameCharArr);
  ifs->read((char*)(&f.frame), 4);
  ifs->read((char*)(&f.boneX), 4);
  ifs->read((char*)(&f.boneY), 4);
  ifs->read((char*)(&f.boneZ), 4);
  ifs->read((char*)(&f.qX), 4);
  ifs->read((char*)(&f.qY), 4);
  ifs->read((char*)(&f.qZ), 4);
  ifs->read((char*)(&f.qW), 4);
  ifs->read((char*)(f.parm), 64);
  /*
  std::cout << "(" << f.boneName << ")";
  std::cout << f.frame;
  std::cout << " (" << f.boneX
	    << ", " << f.boneY
	    << ", " << f.boneZ << ")  ";
  std::cout << " (" << f.qX
	    << "; " << f.qY
	    << ", " << f.qZ
	    << ", " << f.qW << ")  " << std::endl;
  //*/
  return f;
}

bool vmdLoader::Load(const std::string & filename)
{
  return Load(filename.c_str());
}

bool vmdLoader::Load(const char * filename)
{
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs)
  {
    std::cerr << "can not openfile : " << filename << std::endl;
    return false;
  }
  //Vocaloid Motion Data 0002
  ifs.read((char*)hoge, 30);
  //std::cout << hoge << std::endl;
  //モデル名
  ifs.read((char*)modelName, 20);
  //std::cout << kazakami::sjis_to_utf8(modelName) << std::endl;
  //フレームデータ数
  ifs.read((char*)(&frameNum), 4);
  //std::cout << "frameNum : " << frameNum << std::endl;


  for (uint i = 0; i < frameNum; i++)
    frameData.push_back(readFrameData(&ifs));

  Init();
  
  return true;
}



}//namespace kazakami


//test code


/*
int main(int argc, char * argv[])
{
  std::string filename;
  if (argc >= 2)
    filename = argv[1];
  else
  {
    std::cerr << "please file name" << std::endl;
    exit(1);
  }

  kazakami::vmdLoader vmd;

  vmd.Load(filename);
  
  return 0;
}

//*/

