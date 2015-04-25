#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>
#include <memory>

namespace kazakami
{

  //############
  //2次元ベクトル
  //############
  template<class T>
class Vector2
  {
  public:
    Vector2();
    Vector2(T, T);

    Vector2<T> operator +(const Vector2<T>&);
    Vector2<T>& operator +=(const Vector2<T>&);
    Vector2<T> operator -(const Vector2<T>&);
    Vector2<T>& operator -=(const Vector2<T>&);

    void Set(T, T);
    void Add(T, T);
    void Add(const Vector2<T>&);
    double Norm() const;
    void Normalise();
    void Copy(const Vector2<T>&);

    T X;
    T Y;

  };


  template<class T>
  inline std::ostream & operator <<(std::ostream & os, const Vector2<T> & vec)
  {
    os << "x : " << vec.X
       << "   y : " << vec.Y;
    return os;
  }
  template<class T>
  inline Vector2<T> Normalise(const Vector2<T> & vec)
  {
    double norm = vec.Norm();
    return Vector2<T>(vec.X / norm, vec.Y / norm);
  }
  template<class T>
  inline Vector2<T> operator *(const Vector2<T> & vec, double a)
  {
    return Vector2<T>(vec.X * a, vec.Y * a);
  }
  template<class T>
  inline Vector2<T> operator *(double a, const Vector2<T> & vec)
  {
    return Vector2<T>(vec.X * a, vec.Y * a);
  }

  template<class T> Vector2<T>::Vector2()
  {
    X = T();
    Y = T();
  }
  template<class T> Vector2<T>::Vector2(T x, T y)
  {
    X = x;
    Y = y;
  }

  template<class T>
  inline Vector2<T> Vector2<T>::operator +(const Vector2<T> & vec)
  {
    return Vector2<T>(X + vec.X, Y + vec.Y);
  }

  template<class T>
  inline Vector2<T>& Vector2<T>::operator +=(const Vector2<T> & vec)
  {
    X += vec.X;
    Y += vec.Y;
    return *this;
  }

  template<class T>
  inline Vector2<T> Vector2<T>::operator -(const Vector2<T> & vec)
  {
    return Vector2<T>(X - vec.X, Y - vec.Y);
  }

  template<class T>
  inline Vector2<T>& Vector2<T>::operator -=(const Vector2<T> & vec)
  {
    X -= vec.X;
    Y -= vec.Y;
    return *this;
  }

  template<class T> void Vector2<T>::Set(T x, T y)
  {
    X = x;
    Y = y;
  }
  template<class T> void Vector2<T>::Add(T x, T y)
  {
    X += x;
    Y += y;
  }
  template<class T> void Vector2<T>::Add(const Vector2<T> & vec)
  {
    X += vec.X;
    Y += vec.Y;
  }
  template<class T> double Vector2<T>::Norm() const
  {
    return sqrt(X*X + Y*Y);
  }
  template<class T> void Vector2<T>::Normalise()
  {
    double norm = this->Norm();
    X /= norm;
    Y /= norm;
  }
  template<class T> void Vector2<T>::Copy(const Vector2<T> & v)
  {
    X = v.X;
    Y = v.Y;
  }






  //############
  //3次元ベクトル
  //############
  template<class T>
class Vector3
  {
  public:
    Vector3();
    Vector3(const Vector3<T>&);
    Vector3(T, T, T);

    Vector3<T>& operator =(const Vector3<T>&);
    Vector3<T> operator +(const Vector3<T>&) const;
    Vector3<T>& operator +=(const Vector3<T>&);
    Vector3<T> operator -(const Vector3<T>&) const;
    Vector3<T>& operator -=(const Vector3<T>&);

    void Set(T, T, T);
    void Set(const Vector3<T>&);
    void Blend(T, const Vector3<T>&, T, const Vector3<T>&);
    //void Blend(T, const Vector3<T>&, T, const Vector3<T>&,
    //           T, const Vector3<T>&, T, const Vector3<T>&);
    void Add(T, T, T);
    void Add(const Vector3<T>&);
    void Sub(const Vector3<T>&);
    double Norm() const;
    void Normalise();
    void Copy(const Vector3<T>&);

    T & X();
    const T & X() const;
    T & Y();
    const T & Y() const;
    T & Z();
    const T & Z() const;
    //std::unique_ptr<T[]> data;
    T data[3];
    /*
    T _X;
    T _Y;
    T _Z;
    */
  };

  template<class T>
  inline T & Vector3<T>::X()
  {
    return data[0];
  }
  template<class T>
  inline const T & Vector3<T>::X() const
  {
    return data[0];
  }
  template<class T>
  inline T & Vector3<T>::Y()
  {
    return data[1];
  }
  template<class T>
  inline const T & Vector3<T>::Y() const
  {
    return data[1];
  }
  template<class T>
  inline T & Vector3<T>::Z()
  {
    return data[2];
  }
  template<class T>
  inline const T & Vector3<T>::Z() const
  {
    return data[2];
  }


  template<class T>
  inline std::ostream & operator <<(std::ostream & os, const Vector3<T> & vec)
  {
    os << "x : " << vec.X()
       << "   y : " << vec.Y()
       << "   z : " << vec.Z();
    return os;
  }
  template<class T>
  inline Vector3<T> Normalise(const Vector3<T> & vec)
  {
    double norm = vec.Norm();
    return Vector3<T>(vec.X() / norm, vec.Y() / norm, vec.Z() / norm);
  }
  template<class T>
  inline Vector3<T> operator *(const Vector3<T> & vec, double a)
  {
    return Vector3<T>(vec.X() * a, vec.Y() * a, vec.Z() * a);
  }
  template<class T>
  inline Vector3<T> operator *(double a, const Vector3<T> & vec)
  {
    return Vector3<T>(vec.X() * a, vec.Y() * a, vec.Z() * a);
  }

  template<class T> Vector3<T>::Vector3()
  {
    data[0] = T();
    data[1] = T();
    data[2] = T();
  }

  template<class T> Vector3<T>::Vector3(const Vector3<T> & vec)
  {
    data[0] = vec.X();
    data[1] = vec.Y();
    data[2] = vec.Z();
  }

  template<class T> Vector3<T>::Vector3(T x, T y, T z)
  {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  
  template<class T>
  Vector3<T>& Vector3<T>::operator =(const Vector3<T>& vec)
  {
    data[0] = vec.X();
    data[1] = vec.Y();
    data[2] = vec.Z();
    return *this;
  }

  template<class T>
  inline Vector3<T> Vector3<T>::operator +(const Vector3<T> & vec) const
  {
    return Vector3<T>(data[0] + vec.X(), data[1] + vec.Y(), data[2] + vec.Z());
  }

  template<class T>
  inline Vector3<T>& Vector3<T>::operator +=(const Vector3<T> & vec)
  {
    data[0] += vec.X();
    data[1] += vec.Y();
    data[2] += vec.Z();
    return *this;
  }

  template<class T>
  inline Vector3<T> Vector3<T>::operator -(const Vector3<T> & vec) const
  {
    return Vector3<T>(data[0] - vec.X(), data[1] - vec.Y(), data[2] - vec.Z());
  }

  template<class T>
  inline Vector3<T>& Vector3<T>::operator -=(const Vector3<T> & vec)
  {
    data[0] -= vec.X();
    data[1] -= vec.Y();
    data[2] -= vec.Z();
    return *this;
  }

  template<class T>
  inline void Vector3<T>::Set(T x, T y, T z)
  {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }
  template<class T>
  inline void Vector3<T>::Set(const Vector3<T> & vec)
  {
    data[0] = vec.X();
    data[1] = vec.Y();
    data[2] = vec.Z();
  }
  template<class T>
  inline void Vector3<T>::Blend(T a, const Vector3<T> & va, T b, const Vector3<T> & vb)
  {
    data[0] = va.X() * a + vb.X() * b;
    data[1] = va.Y() * a + vb.Y() * b;
    data[2] = va.Z() * a + vb.Z() * b;
  }
  template<class T>
  inline void Vector3<T>::Add(T x, T y, T z)
  {
    data[0] += x;
    data[1] += y;
    data[2] += z;
  }
  template<class T>
  inline void Vector3<T>::Add(const Vector3<T>  & vec)
  {
    data[0] += vec.X();
    data[1] += vec.Y();
    data[2] += vec.Z();
  }
  template<class T>
  inline void Vector3<T>::Sub(const Vector3<T>  & vec)
  {
    data[0] -= vec.X();
    data[1] -= vec.Y();
    data[2] -= vec.Z();
  }
  template<class T>
  inline double Vector3<T>::Norm() const
  {
    return sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
  }
  template<class T>
  inline void Vector3<T>::Normalise()
  {
    double norm = this->Norm();
    data[0] /= norm;
    data[1] /= norm;
    data[2] /= norm;
  }
  template<class T>
  inline void Vector3<T>::Copy(const Vector3<T> & v)
  {
    data[0] = v.X();
    data[1] = v.Y();
    data[2] = v.Z();
  }





  //############
  //4次元ベクトル
  //############
template<class T>
class Vector4
{
public:
  Vector4();
  Vector4(const Vector3<T>&, T);
  Vector4(T, T, T, T);
  
  Vector4<T> operator +(const Vector4<T>&);
  Vector4<T>& operator +=(const Vector4<T>&);
  Vector4<T> operator -(const Vector4<T>&);
  Vector4<T>& operator -=(const Vector4<T>&);
  
  void Set(T, T, T, T);
  void Add(T, T, T, T);
  void Add(const Vector4<T>&);
  double Norm() const;
  void Normalise();
  void Copy(const Vector4<T>&);
  
  T X;
  T Y;
  T Z;
  T W;
};
  
template<class T>
inline std::ostream & operator <<(std::ostream & os, const Vector4<T> & vec)
{
  os << "x : " << vec.X
     << "   y : " << vec.Y
     << "   z : " << vec.Z
     << "   w : " << vec.W;
  return os;
}
template<class T>
inline Vector4<T> Normalise(const Vector4<T> & vec)
{
  double norm = vec.Norm();
  return Vector4<T>(vec.X / norm, vec.Y / norm, vec.Z / norm, vec.W / norm);
}
template<class T>
inline Vector4<T> operator *(const Vector4<T> & vec, double a)
{
  return Vector4<T>(vec.X * a, vec.Y * a, vec.Z * a, vec.W * a);
}
template<class T>
inline Vector4<T> operator *(double a, const Vector4<T> & vec)
{
  return Vector4<T>(vec.X * a, vec.Y * a, vec.Z * a, vec.W * a);
}
  
template<class T> Vector4<T>::Vector4()
{
  X = T();
  Y = T();
  Z = T();
  W = T();
}

template<class T> Vector4<T>::Vector4(const Vector3<T> & vec, T w)
{
  X = vec.X();
  Y = vec.Y();
  Z = vec.Z();
  W = w;
}

template<class T> Vector4<T>::Vector4(T x, T y, T z, T w)
{
  X = x;
  Y = y;
  Z = z;
  W = w;
}

template<class T>
inline Vector4<T> Vector4<T>::operator +(const Vector4<T> & vec)
{
  return Vector4<T>(X + vec.X, Y + vec.Y, Z + vec.Z, W + vec.W);
}

template<class T>
inline Vector4<T>& Vector4<T>::operator +=(const Vector4<T> & vec)
{
  X += vec.X;
  Y += vec.Y;
  Z += vec.Z;
  W += vec.W;
  return *this;
}

template<class T>
inline Vector4<T> Vector4<T>::operator -(const Vector4<T> & vec)
{
  return Vector4<T>(X - vec.X, Y - vec.Y, Z - vec.Z, W - vec.W);
}

template<class T>
inline Vector4<T>& Vector4<T>::operator -=(const Vector4<T> & vec)
{
  X -= vec.X;
  Y -= vec.Y;
  Z -= vec.Z;
  W -= vec.W;
  return *this;
}
  
template<class T>
inline void Vector4<T>::Set(T x, T y, T z, T w)
{
  X = x;
  Y = y;
  Z = z;
  W = w;
}
template<class T>
inline void Vector4<T>::Add(T x, T y, T z, T w)
{
  X += x;
  Y += y;
  Z += z;
  W += w;
}
template<class T>
inline void Vector4<T>::Add(const Vector4<T>  & vec)
{
  X += vec.X;
  Y += vec.Y;
    Z += vec.Z;
    W += vec.W;
}
template<class T>
inline double Vector4<T>::Norm() const
{
  return sqrt(X*X + Y*Y + Z*Z + W*W);
}
template<class T>
inline void Vector4<T>::Normalise()
{
  double norm = this->Norm();
  X /= norm;
  Y /= norm;
  Z /= norm;
  W /= norm;
}
template<class T>
inline void Vector4<T>::Copy(const Vector4<T> & v)
{
  X = v.X;
  Y = v.Y;
  Z = v.Z;
  W = v.W;
}



//typedef Vector3<float> VECTOR3F;
//typedef Vector3<float> Vector3f;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;


void GetNormal(double & x, double & y, double & z,
	       double ax, double ay, double az,
	       double bx ,double by, double bz,
	       double cx, double cy, double cz);

Vector3d GetNormal(const Vector3d & a,
		   const Vector3d & b,
		   const Vector3d & c);


inline Vector2d blend(double a, const Vector2d & va,
		      double b, const Vector2d & vb)
{
  return Vector2d(va.X * a + vb.X * b,
		  va.Y * a + vb.Y * b);
}


inline Vector3d blend(double a, const Vector3d & va,
		      double b, const Vector3d & vb)
{
  return Vector3d(va.X() * a + vb.X() * b,
		  va.Y() * a + vb.Y() * b,
		  va.Z() * a + vb.Z() * b);
}
inline Vector3d blend(double a, const Vector3d & va,
		      double b, const Vector3d & vb,
		      double c, const Vector3d & vc,
		      double d, const Vector3d & vd)
{
  return Vector3d(va.X() * a + vb.X() * b + vc.X() * c + vd.X() * d,
		  va.Y() * a + vb.Y() * b + vc.Y() * c + vd.Y() * d,
		  va.Z() * a + vb.Z() * b + vc.Z() * c + vd.Z() * d);
}


}

#endif // VECTOR_H
