#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "Vector.hpp"
#include "Quaternion.hpp"

namespace kazakami
{


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;



struct FrameData
{
  //char boneName[15];
  std::string boneName;
  unsigned int frame;
  float boneX;
  float boneY;
  float boneZ;
  float qX;
  float qY;
  float qZ;
  float qW;
  char parm[64];
};


//あるボーンについてのモーションデータを扱う。
class BoneMotionData
{
  struct frameData
  {
    uint frame;
    Vector3d pos;
    Quaternion rotation;
  };
  std::string name;
  std::vector<frameData> data;
  uint maxFrame;
public:
  BoneMotionData(const std::string & n);
  const std::string & GetName() const;
  uint GetMaxFrame() const;
  Quaternion GetRotation(uint frame) const;
  Vector3d GetPos(uint frame) const;
  void AddFrame(uint frame,
		float p0, float p1, float p2,
		float qw, float qx, float qy, float qz);
  void check() const
  {
    uint i = 0;
    for (const auto f : data)
    {
      if (i > f.frame)
	exit(1);
      else
	i = f.frame;
    }
  }
  void sort()
  {
    struct LessFrame
    {
      bool operator()(const frameData & L,
		      const frameData & R) const
      { return L.frame < R.frame; }
    };
    std::sort(data.begin(), data.end(), LessFrame());
  }
  void hoge() const
  {
    for (const auto & f : data)
    {
      std::cout << f.frame << " ";
    }
    std::cout << std::endl;
  }
};

class vmdLoader
{
  char hoge[30];
  char modelName[20];
  uint frameNum;
  std::vector<FrameData> frameData;
  FrameData readFrameData(std::ifstream * ifs) const;
  uint maxFrame;
  //boneMotionにおいてボーンの名前からindexを取得する為のmap
  std::map<std::string, int> boneMotionIndexMap;
public:
  uint GetMaxFrame() const;
  std::vector<BoneMotionData> boneMotion;
  bool Load(const char * filename);
  bool Load(const std::string & filename);
  void Init();
};



std::string sjis_to_utf8(const char *pAnsiStr);

}//namespace kazakami
