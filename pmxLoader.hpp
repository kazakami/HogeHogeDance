#pragma once


#include <vector>
#include <string>

namespace kazakami
{

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;


struct PMXVertex
{
  float pos[3];
  float normal[3];
  float UV[2];
  unsigned char weightType;
  int boneIndex1, boneIndex2, boneIndex3, boneIndex4;
  float weight1, weight2, weight3, weight4;
  float C[3];
  float R0[3];
  float R1[3];
  float edge;
};


struct PMXMaterial
{
  std::string name;
  std::string nameEng;
  float diffuse[4];
  float specular[3];
  float shininess;
  float ambient[3];
  uchar drawFlag;
  float edgeColor[4];
  float edgeSize;
  int normalTexture;
  int sphereTexture;
  uchar sphereMode;
  uchar sharedToonFlag;
  int toonTexture;
  uchar sharedToonTexture;
  std::string memo;
  int vertexNum;
};


struct IKLink
{
  int linkBoneIndex;
  uchar angleLimit;
  float lowerLimit[3];
  float upperLimit[3];
};

struct PMXBone
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
  std::vector<IKLink> IKLinks;
};


class PMXLoader
{
  std::string readTextBuf(std::ifstream * ifs) const;
  PMXVertex readPMDVertex(std::ifstream * ifs) const;
  PMXMaterial readMaterial(std::ifstream * ifs) const;
  PMXBone readBone(std::ifstream * ifs) const;
  int readNumber(std::ifstream * ifs, uint length) const;
  int textureIndexSize;
  int boneIndexSize;
  int vertexIndexSize;
public:
  int vertexNum;
  int faceNum;
  int texNum;
  int mtrNum;
  int boneNum;
  int morphNum;
  std::vector<PMXVertex> vertexes;
  std::vector<int> faceIndex;
  std::vector<std::string> textureNames;
  std::vector<PMXMaterial> materials;
  std::vector<PMXBone> bones;
  int readPMX(const char * filename);
  int readPMX(const std::string & filename);
};

}
