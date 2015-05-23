// -----------------------------------------------------------------------
///
///  @file:   Vector.h
///
///  @author: Doug Reitz\n
///           https://github.com/dreitz/ \n
///
///  @date:   20-May-2015\n
///
///  @brief   Represents a 3 dimensional (x,y,z) vector
///
///  @details
///
///  Copyright The MIT License (MIT)
///
///            Copyright (c) 2015 Doug Reitz
///
///            Permission is hereby granted, free of charge, to any person obtaining a copy
///            of this software and associated documentation files (the "Software"), to deal
///            in the Software without restriction, including without limitation the rights
///            to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///            copies of the Software, and to permit persons to whom the Software is
///            furnished to do so, subject to the following conditions:
///
///            The above copyright notice and this permission notice shall be included in
///            all copies or substantial portions of the Software.
///
///            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///            FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///            LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
///            THE SOFTWARE.
///
//-------------------------------------------------------------------------
#ifndef _Csi_Vector_h_
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <inttypes.h>

namespace Csi
{

///@brief   Represents a 3 dimensional (x,y,z) vector
struct Vector
{
   double x,y,z;
   Vector() :x(0),y(0),z(0){}
   Vector(const Vector& r):x(r.x), y(r.y), z(r.z){}
   Vector(const double& a, const double& b, const double& c) {x=a; y=b; z=c;}
   Vector& operator-=(const Vector& r)  { x -= r.x; y -= r.y; z -= r.z; return *this; }
   Vector& operator+=(const Vector& r)  { x += r.x; y += r.y; z += r.z; return *this; }
   /// Subtract operator returns a Vector; does not affect vector
   const Vector operator-(const Vector& r)  const { return Vector(x-r.x, y-r.y, z-r.z); }
   /// Addition operator returns a Vector; does not affect vector
   const Vector operator+(const Vector& r)  const { return Vector(x+r.x, y+r.y, z+r.z); }
   /// Dot product operator returns a double; does not affect the vector
   const double operator*(const Vector& r) const { return (x*r.x + y*r.y + z*r.z); }
   /// @brief Multiply vector by a scale factor; does ot affect the current vector
   const Vector operator*(const double& s)  const { return Vector(s*x, s*y, s*z); }
   /// @brief Return vector in string format;
   double GetMagnitude() const { return sqrt(x*x+y*y+z*z);}
   std::string ToString() const;
   bool operator==(const Vector& r) const { return (x==r.x && y==r.y && z==r.z); }

   /// @brief Assign a char* string of three values to a vector
   Vector& operator=(char *str);
};

/// @brief Vector comparison for use in an stl:set
struct VectorComp
{
   /// @brief Comparator operator for use in an stl:set
   bool operator() (const Vector& lhs, const Vector& rhs) const {return lhs.ToString()<rhs.ToString();}
};
/// @brief Used for printing a vector
std::ostream& operator<< (std::ostream& os, const Vector &v);

} // end namespace Csi
#endif // _Csi_Vector_h_



