#include "Vector.hpp"

namespace kazakami
{


//三次元空間内の三点A(ax, ay, az), B(bx, by, bz), C(cx, cy, cz)
//から成る面の法線ベクトルn(x, y, z)を計算し、返す。
void GetNormal(double & x, double & y, double & z,
	       double ax, double ay, double az,
	       double bx ,double by, double bz,
	       double cx, double cy, double cz)
{
  x = (by - ay)*(cz - az) - (bz - az)*(cy - ay);
  y = (bz - az)*(cx - ax) - (bx - ax)*(cz - az);
  z = (bx - ax)*(cy - ay) - (by - ay)*(cx - ax);
}

//上の関数をVector3dにまとめたもの
Vector3d GetNormal(const Vector3d & a,
		   const Vector3d & b,
		   const Vector3d & c)
{
  double x, y, z;
  GetNormal(x, y, z,
	    a.X(), a.Y(), a.Z(),
	    b.X(), b.Y(), b.Z(),
	    c.X(), c.Y(), c.Z());
  //std::cout << x << ", " << y << ", " << z << std::endl;
  return Vector3d(x, y, z);
}


}

