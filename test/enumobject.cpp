/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/

#include <ponder/errors.hpp>
#include <ponder/enumget.hpp>
#include <ponder/enumobject.hpp>
#include <ponder/enum.hpp>
#include "catch.hpp"

namespace EnumObjectTest
{
    enum MyEnum
    {
        Zero = 0,
        One  = 1,
        Two  = 2
    };
    
    enum MyEnum2
    {
        Zero2 = 0,
        One2  = 1,
        Two2  = 2
    };
    
    enum MyUndeclaredEnum
    {
        Undeclared
    };
    
    void declare()
    {
        ponder::Enum::declare<MyEnum>("EnumObjectTest::MyEnum")
            .value("Zero", Zero)
            .value("One",  One)
            .value("Two",  Two);
        
        ponder::Enum::declare<MyEnum2>("EnumObjectTest::MyEnum2")
            .value("Zero", Zero2)
            .value("One",  One2)
            .value("Two",  Two2);
    }
}

PONDER_TYPE(EnumObjectTest::MyUndeclaredEnum)
PONDER_AUTO_TYPE(EnumObjectTest::MyEnum, &EnumObjectTest::declare)
PONDER_AUTO_TYPE(EnumObjectTest::MyEnum2, &EnumObjectTest::declare)

using namespace EnumObjectTest;

//-----------------------------------------------------------------------------
//                         Tests for ponder::EnumObject
//-----------------------------------------------------------------------------

TEST_CASE("Enum objects")
{
    ponder::EnumObject zero(Zero);
    ponder::EnumObject one(One);
    ponder::EnumObject two(Two);

    SECTION("has names")
    {
        REQUIRE(zero.name() == "Zero");
        REQUIRE(one.name() == "One");
        REQUIRE(two.name() == "Two");
    }

    SECTION("has values")
    {
        REQUIRE(zero.value() == Zero);
        REQUIRE(one.value() == One);
        REQUIRE(two.value() == Two);
    }
    
    SECTION("wrap an Enum type")
    {
        REQUIRE(zero.getEnum() == ponder::enumByType<MyEnum>());
        REQUIRE(one.getEnum()  == ponder::enumByType<MyEnum>());
        REQUIRE(two.getEnum()  == ponder::enumByType<MyEnum>());
    }
    
    SECTION("can be compared ==")
    {
        ponder::EnumObject zero2(Zero2);
        ponder::EnumObject one2(One2);
        ponder::EnumObject two2(Two2);

        REQUIRE(zero == ponder::EnumObject(Zero));
        REQUIRE(one  == ponder::EnumObject(One));
        REQUIRE(two  == ponder::EnumObject(Two));

        REQUIRE((zero == one) == false);
        REQUIRE((one  == two) == false);
        REQUIRE((two  == zero) == false);

        REQUIRE((zero == zero2) == false); // same value and name, different metaenum
        REQUIRE((one  == one2) == false);
        REQUIRE((two  == two2) == false);
    }
    
    SECTION("can be compared <")
    {
        REQUIRE((zero < zero) == false);
        REQUIRE(zero < one);
        REQUIRE(zero < two);

        REQUIRE((one < zero) == false);
        REQUIRE((one < one) == false);
        REQUIRE(one < two);

        REQUIRE((two < zero) == false);
        REQUIRE((two < one) == false);
        REQUIRE((two < two) == false);
    }
    
    SECTION("must be declared")
    {
        // The meta-enum of MyUndeclaredEnum is *not* declared
        REQUIRE_THROWS_AS(ponder::EnumObject obj(Undeclared), ponder::EnumNotFound);
    }    
}
    









