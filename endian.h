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

#ifndef _ENDIAN_H
#define _ENDIAN_H
#include <QGLViewer/qglviewer.h>
using namespace std;


//this file contains definitions for endian functionality
//NOTE: This only fixes endians, and is not complete for all
//architectures. It'll work where short is 2 bytes and int is 4 bytes
//If that's not the case, it'll probably explode.

//a BIG thanks to the Quake 2 source code here

extern bool BigEndianSystem;

void InitEndian( void );		//makes use of a clever trick in Quake 2

short ShortSwap( short s );
short ShortNoSwap( short s );

int LongSwap( int i );
int LongNoSwap( int i );

float FloatSwap( float f );
float FloatNoSwap( float f );
unsigned char* unpackint(int packd);
unsigned char* unpackFloat( float f );
unsigned char* unpackFloatbis( float f );
int pack_int(unsigned char* unpackd);
float packFloat( unsigned char *dat);
float packFloatbis( unsigned char dat[]);



//Use these functions
extern short (*BigShort) ( short s );
extern short (*LittleShort) ( short s );
extern int (*BigLong) ( int i );
extern int (*LittleLong) ( int i );
extern float (*BigFloat) ( float f );
extern float (*LittleFloat) ( float f );
extern int (*packToInt) (unsigned char*);

//Helper method to emulate GLSL
float fract2(float value);
unsigned char ConvertChar(float value);
float PackToFloat(unsigned char x, unsigned char y, unsigned char z);
void UnPackFloat(float src, float &r, float &g, float &b);
void DoTest(float r, float g, float b);




#endif
