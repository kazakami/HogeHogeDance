#include "Matrix4x4d.hpp"


namespace kazakami
{


  Matrix4x4d::Matrix4x4d()
  {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
	data[i][j] = 0;
  }

  Matrix4x4d::Matrix4x4d(double a0, double a1, double a2, double a3,
			 double b0, double b1, double b2, double b3,
			 double c0, double c1, double c2, double c3,
			 double d0, double d1, double d2, double d3)
  {
    data[0][0] = a0; data[0][1] = a1; data[0][2] = a2; data[0][3] = a3;
    data[1][0] = b0; data[1][1] = b1; data[1][2] = b2; data[1][3] = b3;
    data[2][0] = c0; data[2][1] = c1; data[2][2] = c2; data[2][3] = c3;
    data[3][0] = d0; data[3][1] = d1; data[3][2] = d2; data[3][3] = d3;
  }

  double& Matrix4x4d::At(int m, int n)
  {
    return data[m][n];
  }
  const double& Matrix4x4d::At(int m, int n) const
  {
    return data[m][n];
  }

  double Matrix4x4d::Get(int m, int n) const
  {
    return data[m][n];
  }

  void Matrix4x4d::Set(int m, int n, double dat)
  {
    data[m][n] = dat;
  }
  /*
double Matrix4x4d::Det() const
{
    return 0;
}
  */

  Matrix4x4d Matrix4x4d::operator +(const Matrix4x4d& mat) const
  {
    Matrix4x4d m = *this;
    m += mat;
    return m;
  }

  Matrix4x4d & Matrix4x4d::operator +=(const Matrix4x4d& mat)
  {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
	data[i][j] += mat.Get(i, j);
    return *this;
  }

  Matrix4x4d Matrix4x4d::operator -(const Matrix4x4d& mat) const
  {
    Matrix4x4d m = *this;
    m -= mat;
    return m;
  }

  Matrix4x4d& Matrix4x4d::operator -=(const Matrix4x4d& mat)
  {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
	data[i][j] -= mat.Get(i, j);
    return *this;
  }

  Matrix4x4d Matrix4x4d::operator *(const Matrix4x4d& mat) const
  {
    Matrix4x4d m;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
	for (int k = 0; k < 4; k++)
	  m.At(i, j) += data[i][k] * mat.Get(k, j);
    return m;
  }

  Matrix4x4d& Matrix4x4d::operator *=(const Matrix4x4d& mat)
  {
    *this = (*this) * mat;
    return *this;
  }


Matrix4x4d IdentityMat()
{
  return Matrix4x4d(1, 0, 0, 0,
		    0, 1, 0, 0,
		    0, 0, 1, 0,
		    0, 0, 0, 1);
}

Matrix4x4d ParallelProj(double z)
{
  return Matrix4x4d(-1, 0, 0, 0,
		    0, -1, 0, 0,
		    0, 0, 0, -z,
		    0, 0, 0, 1);
}

Matrix4x4d PerspectiveProj(double z)
{
  return Matrix4x4d(1, 0, 0,   0,
		    0, 1, 0,   0,
		    0, 0, 1,   0,
		    0, 0, 1/z, 0);
}

Matrix4x4d TranslateMat(double x, double y, double z)
{
  return Matrix4x4d(1, 0, 0, x,
		    0, 1, 0, y,
		    0, 0, 1, z,
		    0, 0, 0, 1);
}

Matrix4x4d ScaleMat(double x, double y, double z)
{
  return Matrix4x4d(x, 0, 0, 0,
		    0, y, 0, 0,
		    0, 0, z, 0,
		    0, 0, 0, 1);
}

Matrix4x4d RotateMat(double angle, double x, double y, double z)
{
  double norm = sqrt(x * x + y * y + z * z);
  if (norm  < 0.001)
    return IdentityMat();
  x /= norm;
  y /= norm;
  z /= norm;
  double s = sin(angle);
  double c = cos(angle);
  return Matrix4x4d
    (x*x*(1-c) + c  , x*y*(1-c) - z*s, z*x*(1-c) + y*s, 0,
     x*y*(1-c) + z*s, y*y*(1-c) + c  , y*z*(1-c) - x*s, 0,
     z*x*(1-c) - y*s, y*z*(1-c) + x*s, z*z*(1-c) + c  , 0,
     0              , 0              , 0              , 1);
}


}
