// © Copyright 2010 - 2018 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef Mezz_Foundation_BinaryBufferTests_h
#define Mezz_Foundation_BinaryBufferTests_h

/// @file
/// @brief This file tests the functionality of the BinaryBuffer class.

#include "MezzTest.h"

#include "BinaryBuffer.h"

Mezzanine::Boole BufferCompare(const Mezzanine::UInt8* First,
                               const Mezzanine::Char8* Second,
                               Mezzanine::SizeType Count);
Mezzanine::Boole BufferCompare(const Mezzanine::UInt8* First,
                               const Mezzanine::Char8* Second,
                               Mezzanine::SizeType Count)
{
    const Mezzanine::UInt8* SecondCasted = reinterpret_cast<const Mezzanine::UInt8*>(Second);
    while( Count-- > 0 )
    {
        if( *(First++) != *(SecondCasted++) ) {
            return false;
        }
    }
    return true;
}

AUTOMATIC_TEST_GROUP(BinaryBufferTests,BinaryBuffer)
{
    using namespace Mezzanine;

    BinaryBuffer::Byte* NullByte = nullptr;

    {//Construction
        BinaryBuffer DefaultTest;
        TEST_EQUAL("BinaryBuffer()-Binary",
                   NullByte,DefaultTest.Binary);
        TEST_EQUAL("BinaryBuffer()-Size",
                   0u,DefaultTest.Size);

        UInt8* TestBuf = new UInt8[6];
        *(TestBuf + 0) = 'T';
        *(TestBuf + 1) = 'e';
        *(TestBuf + 2) = 's';
        *(TestBuf + 3) = 't';
        *(TestBuf + 4) = '.';
        *(TestBuf + 5) = '\0';

        BinaryBuffer OwnershipTest(TestBuf,6);
        TEST_EQUAL("BinaryBuffer(UInt8*,const_SizeType)-Binary",
                   true,OwnershipTest.Binary == TestBuf);
        TEST_EQUAL("BinaryBuffer(UInt8*,const_SizeType)-Size",
                   6u,OwnershipTest.Size);

        BinaryBuffer CopyTest(OwnershipTest);
        TEST_EQUAL("BinaryBuffer(const_BinaryBuffer&)-Binary",
                   true,BufferCompare(CopyTest.Binary,"Test.",6));
        TEST_EQUAL("BinaryBuffer(const_BinaryBuffer&)-Size",
                   6u,CopyTest.Size);

        BinaryBuffer MoveTest( std::move(CopyTest) );
        TEST_EQUAL("BinaryBuffer(BinaryBuffer&&)-SrcBinary",
                   NullByte,CopyTest.Binary);
        TEST_EQUAL("BinaryBuffer(BinaryBuffer&&)-SrcSize",
                   0u,CopyTest.Size);
        TEST_EQUAL("BinaryBuffer(BinaryBuffer&&)-DestBinary",
                   true,BufferCompare(MoveTest.Binary,"Test.",6));
        TEST_EQUAL("BinaryBuffer(BinaryBuffer&&)-DestSize",
                   6u,MoveTest.Size);

        BinaryBuffer SizeTest(12);
        TEST_EQUAL("BinaryBuffer(const_SizeType)-Binary",
                   true,SizeTest.Binary != nullptr);
        TEST_EQUAL("BinaryBuffer(const_SizeType)-Size",
                   12u,SizeTest.Size);

        String TestParam("Longer Test.");
        BinaryBuffer StringTest(TestParam);
        TEST_EQUAL("BinaryBuffer(const_String&)-Binary",
                   true,BufferCompare(StringTest.Binary,"Longer Test.",12));
        TEST_EQUAL("BinaryBuffer(const_String&)-Size",
                   12u,StringTest.Size);
    }//Construction

    {//Operators
        String CopyTestStr("Copy Test.");
        BinaryBuffer CopySrc(CopyTestStr);
        BinaryBuffer CopyDest;
        CopyDest = CopySrc;
        TEST_EQUAL("operator=(const_BinaryBuffer&)-SrcBinary",
                   true,BufferCompare(CopySrc.Binary,"Copy Test.",10));
        TEST_EQUAL("operator=(const_BinaryBuffer&)-SrcSize",
                   CopyTestStr.size(),CopySrc.Size);
        TEST_EQUAL("operator=(const_BinaryBuffer&)-DestBinary",
                   true,BufferCompare(CopyDest.Binary,"Copy Test.",10));
        TEST_EQUAL("operator=(const_BinaryBuffer&)-DestSize",
                   CopyTestStr.size(),CopyDest.Size);

        String MoveTestStr("Move Test.");
        BinaryBuffer MoveSrc(MoveTestStr);
        BinaryBuffer MoveDest;
        MoveDest = std::move( MoveSrc );
        TEST_EQUAL("operator=(BinaryBuffer&&)-SrcBinary",
                   NullByte,MoveSrc.Binary);
        TEST_EQUAL("operator=(BinaryBuffer&&)-SrcSize",
                   0u,MoveSrc.Size);
        TEST_EQUAL("operator=(BinaryBuffer&&)-DestBinary",
                   true,BufferCompare(MoveDest.Binary,"Move Test.",10));
        TEST_EQUAL("operator=(BinaryBuffer&&)-DestSize",
                   MoveTestStr.size(),MoveDest.Size);

        BinaryBuffer SegmentBase;
        BinaryBuffer FirstSegment(String("First Segment."));
        BinaryBuffer SecondSegment(String(" Second Segment."));
        BinaryBuffer ThirdSegment(String(" Third Segment."));
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-InitBinary",
                   NullByte,SegmentBase.Binary);
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-InitSize",
                   0u,SegmentBase.Size);
        SegmentBase += FirstSegment;
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-FirstBinary",
                   true,BufferCompare(SegmentBase.Binary,"First Segment.",14));
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-FirstSize",
                   14u,SegmentBase.Size);
        SegmentBase += SecondSegment;
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-SecondBinary",
                   true,BufferCompare(SegmentBase.Binary,"First Segment. Second Segment.",30));
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-SecondSize",
                   30u,SegmentBase.Size);
        SegmentBase += ThirdSegment;
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-ThirdBinary",
                   true,BufferCompare(SegmentBase.Binary,"First Segment. Second Segment. Third Segment.",45));
        TEST_EQUAL("operator+=(const_BinaryBuffer&)-ThirdSize",
                   45u,SegmentBase.Size);

        TEST_THROW("operator=(const_BinaryBuffer&)-Throw",
                   std::invalid_argument,
                   [&CopyDest](){ CopyDest = CopyDest; });
        TEST_THROW("operator=(BinaryBuffer&&)-Throw",
                   std::invalid_argument,
                   [&MoveDest](){ MoveDest = MoveDest; });
    }//Operators

    {//Element Access
        String AccessString("WoNdEr wOrLd!");
        BinaryBuffer AccessBuffer(AccessString);
        const BinaryBuffer ConstAccessBuffer(AccessString);

        TEST_EQUAL("operator[](const_SizeType)-First",
                   'W',AccessBuffer[0]);
        TEST_EQUAL("operator[](const_SizeType)-Second",
                   'o',AccessBuffer[1]);
        TEST_EQUAL("operator[](const_SizeType)-Third",
                   'N',AccessBuffer[2]);

        TEST_EQUAL("operator[](const_SizeType)_const-First",
                   'd',ConstAccessBuffer[3]);
        TEST_EQUAL("operator[](const_SizeType)_const-Second",
                   'E',ConstAccessBuffer[4]);
        TEST_EQUAL("operator[](const_SizeType)_const-Third",
                   'r',ConstAccessBuffer[5]);

        TEST_EQUAL("at(const_SizeType)-First",
                   'w',AccessBuffer.at(7));
        TEST_EQUAL("at(const_SizeType)-Second",
                   'O',AccessBuffer.at(8));
        TEST_EQUAL("at(const_SizeType)-Third",
                   'r',AccessBuffer.at(9));

        TEST_EQUAL("at(const_SizeType)_const-First",
                   'L',ConstAccessBuffer.at(10));
        TEST_EQUAL("at(const_SizeType)_const-Second",
                   'd',ConstAccessBuffer.at(11));
        TEST_EQUAL("at(const_SizeType)_const-Third",
                   '!',ConstAccessBuffer.at(12));

        TEST_THROW("at(const_SizeType)-Throw",
                   std::out_of_range,
                   [&AccessBuffer](){ UInt8 Char = AccessBuffer.at(1000); std::cerr << Char; });
        TEST_THROW("at(const_SizeType)_const-Throw",
                   std::out_of_range,
                   [&ConstAccessBuffer](){ UInt8 Char = ConstAccessBuffer.at(1000); std::cerr << Char; });
    }//Element Access

    {//Utility
        BinaryBuffer CreateTest;
        CreateTest.Size = 10;
        TEST_EQUAL("CreateBuffer()-Before-Binary",
                   NullByte,CreateTest.Binary);
        TEST_EQUAL("CreateBuffer()-Before-Size",
                   10u,CreateTest.Size);
        CreateTest.CreateBuffer();
        TEST_EQUAL("CreateBuffer()-After-Binary",
                   true,CreateTest.Binary != nullptr);
        TEST_EQUAL("CreateBuffer()-After-Size",
                   10u,CreateTest.Size);

        CreateTest.DeleteBuffer(100);
        TEST_EQUAL("DeleteBuffer(const_SizeType)-100-Binary",
                   NullByte,CreateTest.Binary);
        TEST_EQUAL("DeleteBuffer(const_SizeType)-100-Size",
                   100u,CreateTest.Size);
        CreateTest.DeleteBuffer();
        TEST_EQUAL("DeleteBuffer(const_SizeType)-Default-Binary",
                   NullByte,CreateTest.Binary);
        TEST_EQUAL("DeleteBuffer(const_SizeType)-Default-Size",
                   0u,CreateTest.Size);

        BinaryBuffer InitialLine(String("Initial Line."));
        BinaryBuffer NextLine(String(" Next Line."));
        BinaryBuffer LastLine(String(" Last Line."));
        TEST_EQUAL("Concatenate(const_Byte*,const_SizeType)-Init-Binary",
                   true,BufferCompare(InitialLine.Binary,"Initial Line.",13));
        TEST_EQUAL("Concatenate(const_Byte*,const_SizeType)-Init-Size",
                   13u,InitialLine.Size);
        InitialLine.Concatenate(NextLine.Binary,NextLine.Size);
        TEST_EQUAL("Concatenate(const_Byte*,const_SizeType)-Binary",
                   true,BufferCompare(InitialLine.Binary,"Initial Line. Next Line.",24));
        TEST_EQUAL("Concatenate(const_Byte*,const_SizeType)-Size",
                   24u,InitialLine.Size);
        InitialLine.Concatenate(LastLine);
        TEST_EQUAL("Concatenate(const_BinaryBuffer&)-Binary",
                   true,BufferCompare(InitialLine.Binary,"Initial Line. Next Line. Last Line.",35));
        TEST_EQUAL("Concatenate(const_BinaryBuffer&)-Size",
                   35u,InitialLine.Size);

        String FifteenLetters("Fifteen Letters!");
        BinaryBuffer StringTest(FifteenLetters);
        TEST_EQUAL("ToString()_const",
                   true,StringTest.ToString() == FifteenLetters);
        TEST_EQUAL("GetSize()_const",
                   16u,StringTest.GetSize());
    }//Utility
}

#endif

