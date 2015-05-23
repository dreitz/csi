// -----------------------------------------------------------------------
///
///  @file:   Vector.cpp
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
#include "Vector.h"

namespace Csi
{

//----------------------------------------------------
//
// The following are methods to manage points
// and elements
//
//----------------------------------------------------

Vector& Vector::operator=(char* str)
{
   std::istringstream iss(str);
   iss >> x >> y >> z;
}

std::string Vector::ToString() const
{
   std::ostringstream oss;
   oss << x<< " " << y << " "<< z;
   return oss.str();
}

std::ostream& operator<< (std::ostream& os, const Vector &v)
{
   os << v.ToString();
   return os;
}

} // end namespace Csi




