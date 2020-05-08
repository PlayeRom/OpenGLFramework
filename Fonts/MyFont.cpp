// MyFont.cpp: implementation of the CMyFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyFont::CMyFont()
{

}

CMyFont::~CMyFont()
{

}

GLvoid CMyFont::DrawCharacter( GLint iChar, GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fScale )
{
	glPushMatrix();
	fX *= fScale;
	glTranslatef( fX, fY, fZ );
	glScalef( fScale, fScale, fScale );
	if( iChar == 32 ) {}//spacja
	else if( iChar == 33 ) {//!
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.5f,0.5f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 34 ) {//"
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,1.5f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.25f,1.5f );
		glEnd();
	}
	else if( iChar == 35 ) {//#
		glBegin( GL_LINES);
			glVertex2f( 0.25f,1.5f );
			glVertex2f( 0.25f,0.5f );
			glVertex2f( 0.75f,1.5f );
			glVertex2f( 0.75f,0.5f );
			//poziome
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 1.0f,0.75f );
		glEnd();
	}
	else if( iChar == 36 ) {//$
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.25f );
			glVertex2f( 0.5f,-0.25f );
		glEnd();
	}
	else if( iChar == 37 ) {//%
		glBegin( GL_LINES);
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.25f,1.75f );
			glVertex2f( 0.75f,0.25f );
		glEnd();
	}
	else if( iChar == 39 ) {//'
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,1.5f );
		glEnd();
	}
	else if( iChar == 40 ) {//(
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.5f,2.25f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.5f );
			glVertex2f( 0.0f,0.5f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.5f,-0.25f );
		glEnd();
	}
	else if( iChar == 41 ) {// )
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.25f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.5f,1.5f );
			glVertex2f( 0.5f,0.5f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,-0.25f );
		glEnd();
	}
	else if( iChar == 42 ) {//*
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.5f );
			glVertex2f( 0.5f,0.5f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.0f,0.75f );
		glEnd();
	}
	else if( iChar == 43 ) {//+
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.5f );
			glVertex2f( 0.5f,0.5f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 44 ) {//,
		glBegin( GL_LINES);
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.0f,-0.25f );
		glEnd();
	}
	else if( iChar == 45 ) {//-
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 46 ) {//.
		glBegin( GL_POINTS);
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 47 ) {// /
		glBegin( GL_LINES);
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == 48 ) {//0
		glBegin( GL_LINE_LOOP );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.25f,0.25f );
			glVertex2f( 0.75f,1.75f );
		glEnd();
	}
	else if( iChar == 49 ) {//1
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.5f );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 50 ) {//2
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.5f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 51 ) {//3
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 52 ) {//4
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.5f );
			glVertex2f( 1.0f,0.5f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.75f,0.0f );
		glEnd();
	}
	else if( iChar == 53 ) {//5
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
	}
	else if( iChar == 54 ) {//6
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
		glEnd();
	}
	else if( iChar == 55 ) {//7
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 56 ) {//8
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 57 ) {//9
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,1.25f );
		glEnd();
	}
	else if( iChar == 58 ) {// :
		glBegin( GL_POINTS);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 59 ) {// ;
		glBegin( GL_POINTS);
			glVertex2f( 0.0f,1.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.0f,-0.25f );
		glEnd();
	}
	else if( iChar == 60 ) {// <
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.75f,1.5f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.75f,0.0f );
		glEnd();
	}
	else if( iChar == 61 ) {// =
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 1.0f,0.75f );
		glEnd();
	}
	else if( iChar == 62 ) {// >
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.5f );
			glVertex2f( 0.75f,0.75f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 63 ) {// ?
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.5f,0.5f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 65 ) {//A
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == -91 ) {//•
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.25f,-0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 97 ) {//a
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == -71 ) {//π
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.25f,-0.25f );
		glEnd();
	}
	else if( iChar == 66 ) {//B
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 98 ) {//b
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.0f,1.0f );
		glEnd();
	}
	else if( iChar == 67 ) {//C
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
	}
	else if( iChar == -58 ) {//∆
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.75f,2.25f );
		glEnd();
	}
	else if( iChar == 99 ) {//c
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
	}
	else if( iChar == -26 ) {//Ê
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.75f,1.25f );
		glEnd();
	}
	else if( iChar == 68 ) {//D
		glBegin( GL_LINE_LOOP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
		glEnd();
	}
	else if( iChar == 100 ) {//d
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 69 ) {//E
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == -54 ) {// 
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.25f,-0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 101 ) {//e
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.5f );
			glVertex2f( 1.0f,0.5f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
	}
	else if( iChar == -22 ) {//Í
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.5f );
			glVertex2f( 1.0f,0.5f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,-0.25f );
		glEnd();
	}
	else if( iChar == 70 ) {//F
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.75f,1.0f );
		glEnd();
	}
	else if( iChar == 102 ) {//f
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.25f,1.75f );
			glVertex2f( 0.25f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.5f,1.0f );
		glEnd();
	}
	else if( iChar == 71 ) {//G
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.5f,1.0f );
		glEnd();
	}
	else if( iChar == 103 ) {//g
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,-0.25f );
			glVertex2f( 0.25f,-0.5f );
			glVertex2f( 0.75f,-0.5f );
			glVertex2f( 1.0f,-0.25f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 72 ) {//H
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 104 ) {//h
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 73 ) {//I
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 105 ) {//i
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.0f,1.5f );
		glEnd();
	}
	else if( iChar == 74 ) {//J
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == 106 ) {//j
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.5f,-0.25f );
			glVertex2f( 0.25f,-0.5f );
			glVertex2f( 0.0f,-0.5f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.5f,1.5f );
		glEnd();
	}
	else if( iChar == 75 ) {//K
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 107 ) {//k
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.5f );
			glVertex2f( 0.75f,0.0f );
		glEnd();
	}
	else if( iChar == 76 ) {//L
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 108 ) {//l
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == -93 ) {//£
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f(-0.25f,0.75f );
			glVertex2f( 0.25f,1.25f );
		glEnd();
	}
	else if( iChar == -77 ) {//≥
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.25f,0.25f );
			glVertex2f( 0.5f,0.0f );
			glVertex2f( 0.75f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.5f,1.25f );
		glEnd();
	}
	else if( iChar == 77 ) {//M
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 109 ) {//m
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.5f,0.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 78 ) {//N
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == -47 ) {//—
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.75f,2.25f );
		glEnd();
	}
	else if( iChar == 110 ) {//n
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == -15 ) {//Ò
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
			//
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.75f,1.25f );
		glEnd();
	}
	else if( iChar == 79 ) {//O
		glBegin( GL_LINE_LOOP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
	}
	else if( iChar == -45 ) {//”
		glBegin( GL_LINE_LOOP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.75f,2.25f );
		glEnd();
	}
	else if( iChar == 111 ) {//o
		glBegin( GL_LINE_LOOP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
	}
	else if( iChar == -13 ) {//Û
		glBegin( GL_LINE_LOOP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.75f,1.25f );
		glEnd();
	}
	else if( iChar == 80 ) {//P
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.0f,1.0f );
		glEnd();
	}
	else if( iChar == 112 ) {//p
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,-0.5f );
		glEnd();
	}
	else if( iChar == 81 ) {//Q
		glBegin( GL_LINE_LOOP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.75f,0.25f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 113 ) {//q
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,-0.5f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 82 ) {//R
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 1.0f,1.25f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 114 ) {//r
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.5f,1.0f );
		glEnd();
	}
	else if( iChar == 83 ) {//S
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
	}
	else if( iChar == -116 ) {//å
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.75f );
			glVertex2f( 0.75f,2.0f );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.0f,1.75f );
			glVertex2f( 0.0f,1.25f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.75f,2.25f );
		glEnd();
	}
	else if( iChar == 115 ) {//s
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
	}
	else if( iChar == -100 ) {//ú
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,0.75f );
			glVertex2f( 0.75f,1.0f );
			glVertex2f( 0.25f,1.0f );
			glVertex2f( 0.0f,0.75f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.75f,1.25f );
		glEnd();
	}
	else if( iChar == 84 ) {//T
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 116 ) {//t
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.25f,2.0f );
			glVertex2f( 0.25f,0.25f );
			glVertex2f( 0.5f,0.0f );
			glVertex2f( 0.75f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.5f );
			glVertex2f( 0.5f,1.5f );
		glEnd();
	}
	else if( iChar == 85 ) {//U
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == 117 ) {//u
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.25f );
			glVertex2f( 0.25f,0.0f );
			glVertex2f( 0.75f,0.0f );
			glVertex2f( 1.0f,0.25f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 86 ) {//V
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.5f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == 118 ) {//v
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.5f,0.0f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 87 ) {//W
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
	}
	else if( iChar == 119 ) {//w
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 0.5f,0.5f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,1.0f );
		glEnd();
	}
	else if( iChar == 88 ) {//X
		glBegin( GL_LINES);
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 120 ) {//x
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,0.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 89 ) {//Y
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 1.0f,2.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 121 ) {//y
		glBegin( GL_LINE_STRIP );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.25f,-0.5f );
			glVertex2f( 0.0f,-0.5f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 90 ) {//Z
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 122 ) {//z
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == -113 ) {//è
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.75f,2.25f );
		glEnd();
	}
	else if( iChar == -97 ) {//ü
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_LINES);
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.75f,1.25f );
		glEnd();
	}
	else if( iChar == -81 ) {//Ø
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.5f,2.25f );
		glEnd();
	}
	else if( iChar == -65 ) {//ø
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 1.0f,1.0f );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
		glBegin( GL_POINTS);
			glVertex2f( 0.5f,1.5f );
		glEnd();
	}
	else if( iChar == 91 ) {//[
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.25f,2.25f );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,-0.25f );
			glVertex2f( 0.25f,-0.25f );
		glEnd();
	}
	else if( iChar == 92 ) {//* \ */
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 93 ) {//]
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.25f );
			glVertex2f( 0.25f,2.25f );
			glVertex2f( 0.25f,-0.25f );
			glVertex2f( 0.0f,-0.25f );
		glEnd();
	}
	else if( iChar == 95 ) {//_
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,0.0f );
			glVertex2f( 1.0f,0.0f );
		glEnd();
	}
	else if( iChar == 123 ) {//{
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.5f,2.0f );
			glVertex2f( 0.25f,1.75f );
			glVertex2f( 0.25f,1.25f );
			glVertex2f( 0.0f,1.0f );
			glVertex2f( 0.25f,0.75f );
			glVertex2f( 0.25f,0.25f );
			glVertex2f( 0.5f,0.0f );
		glEnd();
	}
	else if( iChar == 124 ) {//|
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	else if( iChar == 125 ) {//}
		glBegin( GL_LINE_STRIP );
			glVertex2f( 0.0f,2.0f );
			glVertex2f( 0.25f,1.75f );
			glVertex2f( 0.25f,1.25f );
			glVertex2f( 0.5f,1.0f );
			glVertex2f( 0.25f,0.75f );
			glVertex2f( 0.25f,0.25f );
			glVertex2f( 0.0f,0.0f );
		glEnd();
	}
	glPopMatrix();
}

GLvoid CMyFont::DrawText( LPCTSTR lpText, GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor, GLfloat fSize, GLfloat fScale )
{
	//parametry - tekst, pozycja od ktorej zaczac rysowac, kolor tekstu, rozmiar=gruboúÊ lini
	glDisable( GL_TEXTURE_2D );
	glColor4ub( GetRValue( crColor ), GetGValue( crColor ), GetBValue( crColor ), 0 );
	glLineWidth( fSize );
	glPointSize( fSize + 0.3f );
	//
	GLfloat fNext = 0.15f;
	int ch;
	while( *lpText != 0 ) {
		DrawCharacter( *lpText, fX, fY, fZ, fScale );
		ch = *lpText;
		if( ch == 73||ch == 105||ch == 33||ch == 39||ch == 44||ch == 58||ch == 59||ch == 46||ch == 124 )
			fX += 0.25f + fNext;//Ii!,.:'| //szerokosc znaku = 0
		else if( ch == 34||ch == 91||ch == 93 )
			fX += 0.5f + fNext; //"[]  //szerokosc znaku = 0.25
		else if( ch == 40||ch == 41||ch == 106||ch == 108||ch == 114||ch == 60||ch == 62||ch == 123||ch == 125 )
			fX += 0.75f + fNext;//()jlr<>{}  //szerokosc znaku = 0.5
		else if( ch == 102||ch == 107||ch == 116||ch == -77 )
			fX += 1.0f + fNext;//fkt≥  //szerokosc znaku = 0.75
		else fX += 1.25f + fNext; //szerokosc znaku = 1.0
		++lpText;
	}
	glLineWidth( 1.0f );
	glPointSize( 1.0f );
}

GLvoid CMyFont::DrawTextFormat( GLfloat fX, GLfloat fY, GLfloat fZ, COLORREF crColor, GLfloat fSize, GLfloat fScale, LPCTSTR fmt, ...)
{
	if( fmt == NULL )
		return;

	TCHAR cText[ 256 ];
	va_list	ap;

	va_start( ap, fmt );
	_vstprintf_s( cText, 256, fmt, ap );
	va_end( ap );

	DrawText( cText, fX, fY, fZ, crColor, fSize, fScale );
}