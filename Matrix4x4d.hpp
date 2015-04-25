#ifndef MATRIX4X4D_HPP
#define MATRIX4X4D_HPP

#include "Vector.hpp"


namespace kazakami
{

class Matrix4x4d
{
  friend Matrix4x4d operator *(const Matrix4x4d&, double);
  friend Matrix4x4d operator *(double, const Matrix4x4d&);
  friend Vector4d operator *(const Vector4d&, const Matrix4x4d&);
  friend Vector4d operator *(const Matrix4x4d&, const Vector4d&);
  friend std::ostream & operator << (std::ostream&, const Matrix4x4d&);

public:
  Matrix4x4d();
  Matrix4x4d(double, double, double, double,
	     double, double, double, double,
	     double, double, double, double,
	     double, double, double, double);
  Matrix4x4d(const Vector4d&, const Vector4d&,
	     const Vector4d&, const Vector4d&);
  double& At(int m, int n);
  const double & At(int m, int n) const;
  double Get(int m, int n) const;
  //double Det() const;
  void Set(int m, int n, double dat);
  //void SetScale(double x, double, y, double z);
  //void SetRotate(double x, double y, double z);
  //void SetTranslate(double x, double y, double z);

  Matrix4x4d operator +(const Matrix4x4d&) const;
  Matrix4x4d& operator +=(const Matrix4x4d&);
  Matrix4x4d operator -(const Matrix4x4d&) const;
  Matrix4x4d& operator -=(const Matrix4x4d&);
  Matrix4x4d operator *(const Matrix4x4d&) const;
  Matrix4x4d& operator *=(const Matrix4x4d&);


private:
  double data[4][4];
  /*
   * 00 01 02 03
   * 10 11 12 13
   * 20 21 22 23
   * 30 31 32 33
   */
};


inline std::ostream & operator << (std::ostream & os,const Matrix4x4d & mat)
{
  os << mat.data[0][0] << " " << mat.data[0][1] << " " << mat.data[0][2] << " " << mat.data[0][3] << std::endl
     << mat.data[1][0] << " " << mat.data[1][1] << " " << mat.data[1][2] << " " << mat.data[1][3] << std::endl
     << mat.data[2][0] << " " << mat.data[2][1] << " " << mat.data[2][2] << " " << mat.data[2][3] << std::endl
     << mat.data[3][0] << " " << mat.data[3][1] << " " << mat.data[3][2] << " " << mat.data[3][3] << std::endl
     << std::endl;
  return os;
}


inline Matrix4x4d operator *(const Matrix4x4d& mat, double a)
{
  return Matrix4x4d
    (a * mat.data[0][0], a * mat.data[0][1], a * mat.data[0][2], a * mat.data[0][3],
     a * mat.data[1][0], a * mat.data[1][1], a * mat.data[1][2], a * mat.data[1][3],
     a * mat.data[2][0], a * mat.data[2][1], a * mat.data[2][2], a * mat.data[2][3],
     a * mat.data[3][0], a * mat.data[3][1], a * mat.data[3][2], a * mat.data[3][3]);
}
inline Matrix4x4d operator *(double a, const Matrix4x4d& mat)
{
  return Matrix4x4d
    (a * mat.data[0][0], a * mat.data[0][1], a * mat.data[0][2], a * mat.data[0][3],
     a * mat.data[1][0], a * mat.data[1][1], a * mat.data[1][2], a * mat.data[1][3],
     a * mat.data[2][0], a * mat.data[2][1], a * mat.data[2][2], a * mat.data[2][3],
     a * mat.data[3][0], a * mat.data[3][1], a * mat.data[3][2], a * mat.data[3][3]);
}
  
  
inline Vector4d operator *(const Vector4d& vec, const Matrix4x4d& mat)
{
  return Vector4d
    (mat.data[0][0]*vec.X + mat.data[0][1] * vec.Y + mat.data[0][2] * vec.Z + mat.data[0][3] * vec.W,
     mat.data[1][0]*vec.X + mat.data[1][1] * vec.Y + mat.data[1][2] * vec.Z + mat.data[1][3] * vec.W,
     mat.data[2][0]*vec.X + mat.data[2][1] * vec.Y + mat.data[2][2] * vec.Z + mat.data[2][3] * vec.W,
     mat.data[3][0]*vec.X + mat.data[3][1] * vec.Y + mat.data[3][2] * vec.Z + mat.data[3][3] * vec.W
     );
}
inline Vector4d operator *(const Matrix4x4d& mat, const Vector4d& vec)
{
  return Vector4d
    (mat.data[0][0]*vec.X + mat.data[0][1] * vec.Y + mat.data[0][2] * vec.Z + mat.data[0][3] * vec.W,
     mat.data[1][0]*vec.X + mat.data[1][1] * vec.Y + mat.data[1][2] * vec.Z + mat.data[1][3] * vec.W,
     mat.data[2][0]*vec.X + mat.data[2][1] * vec.Y + mat.data[2][2] * vec.Z + mat.data[2][3] * vec.W,
     mat.data[3][0]*vec.X + mat.data[3][1] * vec.Y + mat.data[3][2] * vec.Z + mat.data[3][3] * vec.W
     );
}

Matrix4x4d IdentityMat();

Matrix4x4d ParallelProj(double z);

Matrix4x4d PerspectiveProj(double z);

Matrix4x4d TranslateMat(double x, double y, double z);

Matrix4x4d ScaleMat(double x, double y, double z);

Matrix4x4d RotateMat(double angle, double x, double y, double z);

  
}



#endif // MATRIX4X4_HPP
