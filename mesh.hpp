#pragma once

#include <string>
#include <vector>
#include <memory>

class Texture2D;

namespace kazakami
{

typedef unsigned char uchar;
class PMXLoader;

struct Vertex
{
  float pos[3];
  float normal[3];
  float UV[2];
  int boneIndex1, boneIndex2, boneIndex3, boneIndex4;
  float weight1, weight2, weight3, weight4;
};

struct IK
{
  int linkBoneIndex;
  uchar angleLimit;
  float lowerLimit[3];
  float upperLimit[3];
};
struct Bone
{
  std::string name;
  std::string nameEng;
  float pos[3];
  int parentBoneIndex;
  int transformHierarchy;
  ushort boneFlag;
  float posOffset[3];
  int connectedBoneIndex;
  int grantParentBoneIndex;
  float grantRatio;
  float axisDirectVector[3];
  float xAxisDirectVector[3];
  float zAxisDirectVector[3];
  int keyValue;
  int IKTargetBoneIndex;
  int IKLoopNum;
  float IKLimitAngle;
  int IKLinkNum;
  std::vector<IK> IKs;
};
  
class Mesh
{
  std::vector<Vertex> verteces;
  std::vector<Bone> bones;
  std::unique_ptr<PMXLoader> pmx;
  std::vector<std::shared_ptr<Texture2D>> textures;
public:
  Mesh();
  void Draw();
  int LoadPMX(const std::string & directoryName, const std::string & filename);
};
  
}
