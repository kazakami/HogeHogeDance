#pragma once

#include "Vector.hpp"
#include "Matrix4x4d.hpp"

namespace kazakami
{

class Quaternion
{
  friend std::ostream & operator << (std::ostream&, const Quaternion&);
  double innerProduct(const Vector3d & a,
		      const Vector3d & b) const;
  Vector3d outerProduct(const Vector3d & a,
			const Vector3d & b) const;
public:
  double t;
  Vector3d v;
  Quaternion();
  Quaternion(double w,
	     double x,
	     double y,
	     double z);
  Quaternion(double w,
	     const Vector3d & vec);
  double Norm() const;
  Quaternion Conjugate() const;
  Quaternion operator *(const Quaternion & q) const;
  Quaternion & operator *=(const Quaternion & q);
  Vector3d getVector() const;
  Matrix4x4d toMat() const;
};

//変形させない四元数
inline Quaternion IdentityQuaternion()
{
  return Quaternion(1, 0, 0, 0);
}

inline std::ostream & operator << (std::ostream & os,
				   const Quaternion & q)
{
  os << "(" << q.t << "; "
     << q.v.X() << ", "
     << q.v.Y() << ", " 
     << q.v.Z() << ")";
  return os;
}

}//namespace kazakami
