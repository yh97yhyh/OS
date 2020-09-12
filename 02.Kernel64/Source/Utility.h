/**
 *  file    Utility.h
 *  date    2009/01/17
 *  author  kkamagui 
 *          Copyright(c)2008 All rights reserved by kkamagui
 *  brief   OS���� ����� ��ƿ��Ƽ �Լ��� ���õ� ����
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdarg.h>
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////
//
//  ��ũ��
//
////////////////////////////////////////////////////////////////////////////////
#define MIN( x, y )     ( ( ( x ) < ( y ) ) ? ( x ) : ( y ) )
#define MAX( x, y )     ( ( ( x ) > ( y ) ) ? ( x ) : ( y ) )

////////////////////////////////////////////////////////////////////////////////
//
//  �Լ�
//
////////////////////////////////////////////////////////////////////////////////
void kMemSet( void* pvDestination, BYTE bData, int iSize );
int kMemCpy( void* pvDestination, const void* pvSource, int iSize );
int kMemCmp( const void* pvDestination, const void* pvSource, int iSize );
BOOL kSetInterruptFlag( BOOL bEnableInterrupt );
void kCheckTotalRAMSize( void );
QWORD kGetTotalRAMSize( void );
void kReverseString( char* pcBuffer );
long kAToI( const char* pcBuffer, int iRadix );
QWORD kHexStringToQword( const char* pcBuffer );
long kDecimalStringToLong( const char* pcBuffer );
int kIToA( long lValue, char* pcBuffer, int iRadix );
int kHexToString( QWORD qwValue, char* pcBuffer );
int kDecimalToString( long lValue, char* pcBuffer );
int kSPrintf( char* pcBuffer, const char* pcFormatString, ... );
int kVSPrintf( char* pcBuffer, const char* pcFormatString, va_list ap );
QWORD kGetTickCount( void );
void kSleep( QWORD qwMillisecond );

char* kStrCpy(char* dest, const char* source);
int kStrCmp(char *arr1, char *arr2);
char* kStrCat(char* str1,char* str2);
////////////////////////////////////////////////////////////////////////////////
//
//  ��Ÿ
//
////////////////////////////////////////////////////////////////////////////////
extern volatile QWORD g_qwTickCount;

#endif /*__UTILITY_H__*/
