#include "Quaternion.hpp"


namespace kazakami
{

Quaternion::Quaternion()
  :t(0), v(0, 0, 0)
{
}

Quaternion::Quaternion(double w,
		       double x,
		       double y,
		       double z)
  :t(w), v(x, y, z)
{
}

Quaternion::Quaternion(double w,
		       const Vector3d & vec)
  :t(w), v(vec)
{
}


double Quaternion::innerProduct(const Vector3d & a,
				const Vector3d & b) const
{
  return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
}

Vector3d Quaternion::outerProduct(const Vector3d & a,
				  const Vector3d & b) const
{
  return Vector3d(a.Y() * b.Z() - a.Z() * b.Y(),
		  a.Z() * b.X() - a.X() * b.Z(),
		  a.X() * b.Y() - a.Y() * b.X());
}

double Quaternion::Norm() const
{
  return t * t
    + v.X() * v.X()
    + v.Y() * v.Y()
    + v.Z() * v.Z();
}

Quaternion Quaternion::Conjugate() const
{
  return Quaternion(-t, -v.X(), -v.Y(), -v.Z());
}

Quaternion Quaternion::operator *(const Quaternion & q) const
{
  Quaternion tmp = *this;
  return tmp *= q;
}

Quaternion & Quaternion::operator *=(const Quaternion & q)
{
  double a = t;
  t = t * q.t - innerProduct(v, q.v);
  v = a * q.v + q.t * v + outerProduct(v, q.v);
  return *this;
}

Matrix4x4d Quaternion::toMat() const
{
  double w = t;
  double x = v.X();
  double y = v.Y();
  double z = v.Z();
  //if (z == 1) exit(1);//return IdentityMat();
  //*
  return Matrix4x4d
    (1 - 2*y*y - 2*z*z,     2*x*y - 2*w*z,     2*x*z + 2*w*y, 0,
         2*x*y + 2*w*z, 1 - 2*x*x - 2*z*z,     2*y*z - 2*w*x, 0,
         2*x*z - 2*w*y,     2*y*z + 2*w*x, 1 - 2*x*x - 2*y*y, 0,
                     0,                 0,                 0, 1);
  //*/
  /*
  return Matrix4x4d
    (1 - 2*y*y - 2*z*z,     2*x*y + 2*w*z,     2*x*z - 2*w*y, 0,
         2*x*y - 2*w*z, 1 - 2*x*x - 2*z*z,     2*y*z + 2*w*x, 0,
         2*x*z + 2*w*y,     2*y*z - 2*w*x, 1 - 2*x*x - 2*y*y, 0,
                     0,                 0,                 0, 1);
  */
}


}//namespace kazakami


//test code
/*
int main()
{
  kazakami::Quaternion q(1, 1, 2, 3);
  kazakami::Quaternion q2(2, 3, 5, 1);
  q *= q2;
  std::cout << q << std::endl;
  return 0;
}
*/
