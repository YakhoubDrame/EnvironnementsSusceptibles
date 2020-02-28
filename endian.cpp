/*
Copyright (C) 2003  Promit Roy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "endian.h"

typedef unsigned char byte;

bool BigEndianSystem;

short (*BigShort) ( short s );
short (*LittleShort) ( short s );
int (*BigLong) ( int i );
int (*LittleLong) ( int i );
float (*BigFloat) ( float f );
float (*LittleFloat) ( float f );


//adapted from Quake 2 source

short ShortSwap( short s )
{
    byte b1, b2;

    b1 = s & 255;
    b2 = (s >> 8) & 255;

    return (b1 << 8) + b2;
}

short ShortNoSwap( short s )
{
    return s;
}



int LongSwap (int i)
{
    byte b1, b2, b3, b4;

    b1 = i & 255;
    b2 = ( i >> 8 ) & 255;
    b3 = ( i>>16 ) & 255;
    b4 = ( i>>24 ) & 255;

    return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

int LongNoSwap( int i )
{
    return i;
}



float FloatSwap( float f )
{
    union
    {
        float f;
        byte b[4];
    } dat1, dat2;


    dat1.f = f;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];
    return dat2.f;
}

float FloatNoSwap( float f )
{
    return f;
}

unsigned char* unpackFloat( float f )
{
    union unpackedfloat
    {
        float f;
        unsigned char b[4];
    };

    unpackedfloat* dat = new unpackedfloat;

    dat->f = f;

    cout << " Oricon : " << (int)dat->b[0]
         << "  " << (int)dat->b[1]
         << "  " << (int)dat->b[2]
         << "  " << (int)dat->b[3]
         << endl;

    return dat->b;
}
unsigned char* unpackFloatbis( float f )
{
    union
    {
        float f;
        unsigned char b[4];
    } dat1, dat2;

    dat1.f = f;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];


    return dat2.b;
}

float packFloat( unsigned char* dat )
{
    union
    {
        float f;
        unsigned char b[4];
    } dat1;

    dat1.b[0] = dat[0];
    dat1.b[1] = dat[1];
    dat1.b[2] = dat[2];
    dat1.b[3] = dat[3];

    return dat1.f;
}


float packFloatbis( unsigned char dat[4] )
{
    union
    {
        float f;
        unsigned char b[4];
    } dat1;

    dat1.b[0] = dat[3];
    dat1.b[1] = dat[2];
    dat1.b[2] = dat[1];
    dat1.b[3] = dat[0];

    return dat1.f;
}

void InitEndian( void )
{
    //clever little trick from Quake 2 to determine the endian
    //of the current system without depending on a preprocessor define

    byte SwapTest[2] = { 1, 0 };

    if( *(short *) SwapTest == 1 )
    {
        //little endian
        BigEndianSystem = false;

        //set func pointers to correct funcs
        BigShort = ShortSwap;
        LittleShort = ShortNoSwap;
        BigLong = LongSwap;
        LittleLong = LongNoSwap;
        BigFloat = FloatSwap;
        LittleFloat = FloatNoSwap;

    }
    else
    {
        //big endian
        BigEndianSystem = true;

        BigShort = ShortNoSwap;
        LittleShort = ShortSwap;
        BigLong = LongNoSwap;
        LittleLong = LongSwap;
        BigFloat = FloatSwap;
        LittleFloat = FloatNoSwap;
    }
}

//Helper method to emulate GLSL
float fract2(float value)
{
    return (float)fmod(value, 1.0f);
}

//Helper method to go from a float to packed char
unsigned char ConvertChar(float value)
{
    //Scale and bias
    value = (value + 1.0f) * 0.5f;
    return (unsigned char)(value*255.0f);
}

//Pack 3 values into 1 float
float PackToFloat(unsigned char x, unsigned char y, unsigned char z)
{
    unsigned int packedColor = (x << 16) | (y << 8) | z;
    float packedFloat = (float) ( ((double)packedColor) / ((double) (1 << 24)) );

    return packedFloat;
}

int pack_int(unsigned char* unpackd){

    int Int32 = 0;
    Int32 = (Int32 << 8) + unpackd[0];
    Int32 = (Int32 << 8) + unpackd[1];
    Int32 = (Int32 << 8) + unpackd[2];
    Int32 = (Int32 << 8) + unpackd[3];

    return Int32;

}

unsigned char* unpackint(int packd){

    unsigned char* unpackd = new unsigned char[4];

    unpackd[3] = packd & 255;
    unpackd[2] = (packd >> 8) & 255;
    unpackd[1] = (packd >> 16) & 255;
    unpackd[0] = (packd >> 24) & 255;
    return unpackd;

}

//UnPack 3 values from 1 float
void UnPackFloat(float src, float &r, float &g, float &b)
{
    r = fract2(src);
    g = fract2(src * 256.0f);
    b = fract2(src * 65536.0f);

    //Unpack to the -1..1 range
    r = (r * 2.0f) - 1.0f;
    g = (g * 2.0f) - 1.0f;
    b = (b * 2.0f) - 1.0f;
}

//Test pack/unpack 3 values
void DoTest(float r, float g, float b)
{
    float outR, outG, outB;

    printf("Testing %f %f %f\n",r, g, b);

    //Pack
    float result = PackToFloat(ConvertChar(r), ConvertChar(g), ConvertChar(b));

    //Unpack
    UnPackFloat(result, outR, outG, outB);

    printf("Result %f %f %f\n",outR, outG, outB);
    printf("Diff   %f %f %f\n\n",r-outR, g-outG, b-outB);
}
