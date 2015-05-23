// -----------------------------------------------------------------------
///
///  @file:   Vector.test.cpp
///
///  @author: Doug Reitz\n
///           https://github.com/dreitz/ \n
///
///  @date:   20-May-2015\n
///
///  @brief   Unit test for Vector class
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
#define BOOST_TEST_MODULE TestSuite
#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include "Vector.h"
#include <boost/test/unit_test.hpp>




BOOST_AUTO_TEST_CASE( vector_test )
{
    Csi::Vector v(1.0, 2.0, 3.0);
    BOOST_CHECK_EQUAL(v.x, 1.0);
    BOOST_CHECK_EQUAL(v.y, 2.0);
    BOOST_CHECK_EQUAL(v.z, 3.0);


    Csi::Vector v2;
    v2 = v;
    BOOST_CHECK(v==v2);

    Csi::Vector v3(v2-v);
    BOOST_CHECK_EQUAL(v3.x, 0.0);
    BOOST_CHECK_EQUAL(v3.y, 0.0);
    BOOST_CHECK_EQUAL(v3.z, 0.0);

    v3 = v+v2;
    BOOST_CHECK_EQUAL(v3.x, 2.0);
    BOOST_CHECK_EQUAL(v3.y, 4.0);
    BOOST_CHECK_EQUAL(v3.z, 6.0);

    v3 -= v;
    BOOST_CHECK_EQUAL(v3.x, 1.0);
    BOOST_CHECK_EQUAL(v3.y, 2.0);
    BOOST_CHECK_EQUAL(v3.z, 3.0);

    double dot = v*v2;
    BOOST_CHECK_EQUAL(dot, 14.0);

    Csi::Vector v4 = v3*4.0;
    BOOST_CHECK_EQUAL(v4.x, 4.0);
    BOOST_CHECK_EQUAL(v4.y, 8.0);
    BOOST_CHECK_EQUAL(v4.z, 12.0);

    BOOST_CHECK_EQUAL(v3.GetMagnitude(), sqrt(1.0+4.0+9.0));






}




