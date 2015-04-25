#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <memory>

#include "pmxLoader.hpp"

namespace kazakami
{

//TextBufを読み込むよ!!たぶん!!つらかった!!
//ヒント : ワイド文字とマルチバイト文字
std::string PMXLoader::readTextBuf(std::ifstream * ifs) const
{
  int length;
  ifs->read((char*)(&length), 4);
  //std::cout << length << std::endl;
  std::vector<wchar_t> data;
  for (int i = 0; i < length/2; i++)
  {
    char16_t h1;
    ifs->read((char*)(&h1), 2);
    data.push_back(h1);
    //std::cout << (int)h1 << " ";
  }
  //data.push_back(0);
 

  //ワイド文字をマルチバイト文字に変換するよ!!
  //参考にしました
  //http://kamoland.com/wiki/wiki.cgi?ShiftJIS%A1%A4Unicode%28UTF-8%29
  std::string str;
  for (wchar_t s : data)
  {
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
  //std::cout << d << std::endl;
  return str;
}

int PMXLoader::readNumber(std::ifstream * ifs, uint length) const
{
  int ret;
  switch (length)
  {
  case 1:
    {
      char i;
      ifs->read((char*)(&i), 1);
      ret = (int)i;
      break;
    }
  case 2:
    {
      short i;
      ifs->read((char*)(&i), 2);
      ret = (int)i;
      break;
    }
  case 4:
    {
      int i;
      ifs->read((char*)(&i), 4);
      ret = (int)i;
      break;
    }
  default:
    std::cerr << "invalid read length" << std::endl;
    exit(1);
    break;
  }
  return ret;
}


PMXVertex PMXLoader::readPMDVertex(std::ifstream * ifs) const
{
  PMXVertex p;
  ifs->read((char*)(p.pos), 12);
  ifs->read((char*)(p.normal), 12);
  ifs->read((char*)(p.UV), 8);
  
  //追加UVの処理とかした方がよい。でもまだしない。


  ifs->read((char*)(&p.weightType), 1);

  switch (p.weightType)
  {
  case 0://BDEF1
    p.boneIndex1 = readNumber(ifs, boneIndexSize);
    break;
  case 1://BDEF2
    p.boneIndex1 = readNumber(ifs, boneIndexSize);
    p.boneIndex2 = readNumber(ifs, boneIndexSize);
    ifs->read((char*)(&p.weight1), 4);
    break;
  case 2://BDEF4
    p.boneIndex1 = readNumber(ifs, boneIndexSize);
    p.boneIndex2 = readNumber(ifs, boneIndexSize);
    p.boneIndex3 = readNumber(ifs, boneIndexSize);
    p.boneIndex4 = readNumber(ifs, boneIndexSize);
    ifs->read((char*)(&p.weight1), 4);
    ifs->read((char*)(&p.weight2), 4);
    ifs->read((char*)(&p.weight3), 4);
    ifs->read((char*)(&p.weight4), 4);
    break;
  case 3://SDEF
    p.boneIndex1 = readNumber(ifs, boneIndexSize);
    p.boneIndex2 = readNumber(ifs, boneIndexSize);
    ifs->read((char*)(&p.weight1), 4);
    ifs->read((char*)(p.C), 12);
    ifs->read((char*)(p.R0), 12);
    ifs->read((char*)(p.R1), 12);
    break;
  }

  ifs->read((char*)(&p.edge), 4);

  return p;
}

PMXMaterial PMXLoader::readMaterial(std::ifstream * ifs) const
{
  PMXMaterial p;
  p.name = readTextBuf(ifs);
  p.nameEng = readTextBuf(ifs);
  ifs->read((char*)(p.diffuse), 16);
  ifs->read((char*)(p.specular), 12);
  ifs->read((char*)(&(p.shininess)), 4);
  ifs->read((char*)(p.ambient), 12);
  ifs->read((char*)(&(p.drawFlag)), 1);
  ifs->read((char*)(p.edgeColor), 16);
  ifs->read((char*)(&p.edgeSize), 4);
  p.normalTexture = readNumber(ifs, textureIndexSize);
  //std::cout << "texNumber : " << p.normalTexture << std::endl;
  p.sphereTexture = readNumber(ifs, textureIndexSize);
  ifs->read((char*)(&p.sphereMode), 1);
  ifs->read((char*)(&p.sharedToonFlag), 1);
  if (p.sharedToonFlag == 0)
    p.toonTexture = readNumber(ifs, textureIndexSize);
  else if (p.sharedToonFlag == 1)
    ifs->read((char*)(&p.sharedToonTexture), 1);
  else
  {
    std::cerr << "error on shared toon texture";
    exit(1);
  }
  p.memo = readTextBuf(ifs);
  ifs->read((char*)(&p.vertexNum), 4);
  return p;
}


PMXBone PMXLoader::readBone(std::ifstream * ifs) const
{
  PMXBone b;
  b.name = readTextBuf(ifs);
  b.nameEng = readTextBuf(ifs);
  ifs->read((char*)(b.pos), 12);
  b.parentBoneIndex = readNumber(ifs, boneIndexSize);
  ifs->read((char*)(&b.transformHierarchy), 4);
  ifs->read((char*)(&b.boneFlag), 2);
  //std::cout << std::hex << std::setw(4) << std::setfill('0') << b.boneFlag << std::dec << std::endl;
  if (b.boneFlag & 0x0001) //接続先1
  {
    b.connectedBoneIndex = readNumber(ifs, boneIndexSize);
  }
  else //接続先0
  {
    ifs->read((char*)(b.posOffset), 12);
  }
  if (b.boneFlag & 0x0300) //回転付与1または移動付与1
  {
    b.grantParentBoneIndex = readNumber(ifs, boneIndexSize);
    ifs->read((char*)(&b.grantRatio), 4);
  }
  if (b.boneFlag & 0x0400) //軸固定1
  {
    ifs->read((char*)(b.axisDirectVector), 12);
  }
  if (b.boneFlag & 0x0800) //ローカル軸1
  {
    ifs->read((char*)(b.xAxisDirectVector), 12);
    ifs->read((char*)(b.zAxisDirectVector), 12);
  }
  if (b.boneFlag & 0x2000) //外部親変形1
  {
    ifs->read((char*)(&b.keyValue), 4);
  }
  if (b.boneFlag & 0x0020) //IK 1
  {
    b.IKTargetBoneIndex = readNumber(ifs, boneIndexSize);
    ifs->read((char*)(&b.IKLoopNum), 4);
    ifs->read((char*)(&b.IKLimitAngle), 4);
    ifs->read((char*)(&b.IKLinkNum), 4);
    for (int i = 0; i < b.IKLinkNum; i++)
    {
      IKLink ik;
      ik.linkBoneIndex = readNumber(ifs, boneIndexSize);
      ifs->read((char*)(&ik.angleLimit), 1);
      if (ik.angleLimit)
      {
	ifs->read((char*)(ik.lowerLimit), 12);
	ifs->read((char*)(ik.lowerLimit), 12);
      }
      b.IKLinks.push_back(std::move(ik));
    }
  }
  return b;
}

int PMXLoader::readPMX(const std::string & filename)
{
  return readPMX(filename.c_str());
}

int PMXLoader::readPMX(const char * filename)
{
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  
  if (!ifs)
  {
    std::cerr << "no such file : " << filename << std::endl;
    exit(1);
  }
  
  
  //ヘッダ読み込み
  {
    //ファイル形式
    //4バイト分だけど、文字列をして表示してたいので、
    //一つ余分にメモリー確保してヌル文字入れてる。
    unsigned char filetype[5];
    filetype[4] = '\0';
    ifs.read((char*)(filetype), sizeof(filetype) - 1);
    //std::cout << filetype << std::endl;

    //ver
    float version;
    ifs.read((char*)(&version), sizeof(version));
    //std::cout << "Ver : " << version << std::endl;

    //後続するデータサイズ 8固定だってさ
    unsigned char dataSize;
    ifs.read((char*)(&dataSize), sizeof(dataSize));
    //std::cout << "data size : " << (int)dataSize << std::endl;
    if ((int)dataSize != 8)
    {
      std::cerr << "incorrect data size" << std::endl;
      return 1;
    }

    //バイト列(メタデータみたいなのが入ってる)
    unsigned char metaData[8];
    ifs.read((char*)metaData, sizeof(metaData));
    /*
    std::cout << "encode type         : " << (int)metaData[0] << std::endl
	      << "additional UV num   : " << (int)metaData[1] << std::endl
	      << "vertex index size   : " << (int)metaData[2] << std::endl
	      << "texture index size  : " << (int)metaData[3] << std::endl
	      << "material index size : " << (int)metaData[4] << std::endl
	      << "bone index size     : " << (int)metaData[5] << std::endl
	      << "mof index size      : " << (int)metaData[6] << std::endl
	      << "goutai index size   : " << (int)metaData[7] << std::endl;
    */

    boneIndexSize = (int)metaData[5];
    textureIndexSize = (int)metaData[3];
    vertexIndexSize = (int)metaData[2];
    
    //モデル名とかの読み取り
    auto modelName = readTextBuf(&ifs);
    //std::cout << modelName << std::endl;
    auto modelNameEng = readTextBuf(&ifs);
    //std::cout << modelNameEng << std::endl;
    auto comment = readTextBuf(&ifs);
    //std::cout << comment << std::endl;
    auto commentEng = readTextBuf(&ifs);
    //std::cout << commentEng << std::endl;
  }

  //頂点の読み込み
  {
    ifs.read((char*)(&vertexNum), 4);
    //std::cout << "vertexNum : " << vertexNum << std::endl;
    for (int i = 0; i < vertexNum; i++)
    {
      //std::cout << i << std::endl;
      auto v = readPMDVertex(&ifs);
      /*
      im.AddVertex({v.pos[0], v.pos[1], v.pos[2]},
		   {v.normal[0], v.normal[1], v.normal[2]},
		   {v.UV[0], v.UV[1]});
      */
      vertexes.push_back(std::move(v));
    }
  }

  //面の読み込み
  {
    ifs.read((char*)(&faceNum), 4);
    //std::cout << "faceNum : " << faceNum << std::endl;
        for (int i = 0; i < faceNum; i++)
    {
      int index = readNumber(&ifs, vertexIndexSize);
      faceIndex.push_back(index);
    }
  }

  //テクスチャの読み込み
  {
    ifs.read((char*)(&texNum), 4);
    //std::cout << "texNum : " << texNum << std::endl;
    for (int i = 0; i < texNum; i++)
    {
      auto name = readTextBuf(&ifs);
      //std::cout << name << std::endl;
      textureNames.push_back(std::move(name));
    }
  }

  //材質の読み込み これからかく かいた!!
  {
    ifs.read((char*)(&mtrNum), 4);
    //std::cout << "mtrNum : " << mtrNum << std::endl;
    for (int i = 0; i < mtrNum; i++)
    {
      auto m = readMaterial(&ifs);
      //std::cout << m.name << std::endl;
      materials.push_back(std::move(m));
    }
  }

  //ボーン
  {
    ifs.read((char*)(&boneNum), 4);
    //std::cout << "boneNum : " << boneNum << std::endl;
    for (int i = 0; i < boneNum; i++)
    {
      auto b = readBone(&ifs);
      //std::cout << i << " : " << b.name << std::endl;
      /*
      if (b.boneFlag & 0x0001)
	std::cout << " -> " << b.connectedBoneIndex;
      else
	std::cout << " : " << b.pos[0]
		  << ", " << b.pos[1]
		  << ", " << b.pos[2];
      std::cout << std::endl;
      */
      bones.push_back(std::move(b));
    }
  }

  //モーフ そのうちかく
  {
    ifs.read((char*)(&morphNum), 4);
    //std::cout << "morphNum : " << morphNum << std::endl;
  }


  ifs.close();
  return 0;
}


}

