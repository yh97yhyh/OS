/**
 *  file    ConsoleShell.h
 *  date    2009/01/31
 *  author  kkamagui 
 *          Copyright(c)2008 All rights reserved by kkamagui
 *  brief   �ܼ� �п� ���õ� ��� ����
 */

#ifndef __CONSOLESHELL_H__
#define __CONSOLESHELL_H__

#include "Types.h"

////////////////////////////////////////////////////////////////////////////////
//
// ��ũ��
//
////////////////////////////////////////////////////////////////////////////////
#define CONSOLESHELL_MAXCOMMANDBUFFERCOUNT  300
#define CONSOLESHELL_PROMPTMESSAGE          "MINT64>"

// ���ڿ� �����͸� �Ķ���ͷ� �޴� �Լ� ������ Ÿ�� ����
typedef void ( * CommandFunction ) ( const char* pcParameter );


////////////////////////////////////////////////////////////////////////////////
//
// ����ü
//
////////////////////////////////////////////////////////////////////////////////
// 1����Ʈ�� ����
#pragma pack( push, 1 )

// ���� Ŀ�ǵ带 �����ϴ� �ڷᱸ��
typedef struct kShellCommandEntryStruct
{
    // Ŀ�ǵ� ���ڿ�
    char* pcCommand;
    // Ŀ�ǵ��� ����
    char* pcHelp;
    // Ŀ�ǵ带 �����ϴ� �Լ��� ������
    CommandFunction pfFunction;
} SHELLCOMMANDENTRY;

// �Ķ���͸� ó���ϱ����� ������ �����ϴ� �ڷᱸ��
typedef struct kParameterListStruct
{
    // �Ķ���� ������ ��巹��
    const char* pcBuffer;
    // �Ķ������ ����
    int iLength;
    // ���� ó���� �Ķ���Ͱ� �����ϴ� ��ġ
    int iCurrentPosition;
} PARAMETERLIST;

#pragma pack( pop )

////////////////////////////////////////////////////////////////////////////////
//
// �Լ�
//
////////////////////////////////////////////////////////////////////////////////
// ���� �� �ڵ�
void kStartConsoleShell( void );
void kExecuteCommand( const char* pcCommandBuffer );
void kInitializeParameter( PARAMETERLIST* pstList, const char* pcParameter );
int kGetNextParameter( PARAMETERLIST* pstList, char* pcParameter );

// Ŀ�ǵ带 ó���ϴ� �Լ�
static void kHelp( const char* pcParameterBuffer );
static void kCls( const char* pcParameterBuffer );
static void kShowTotalRAMSize( const char* pcParameterBuffer );
static void kStringToDecimalHexTest( const char* pcParameterBuffer );
static void kShutdown( const char* pcParamegerBuffer );
static void kSetTimer( const char* pcParameterBuffer );
static void kWaitUsingPIT( const char* pcParameterBuffer );
static void kReadTimeStampCounter( const char* pcParameterBuffer );
static void kMeasureProcessorSpeed( const char* pcParameterBuffer );
static void kShowDateAndTime( const char* pcParameterBuffer );
static void kCreateTestTask( const char* pcParameterBuffer );
static void kChangeTaskPriority( const char* pcParameterBuffer );
static void kShowTaskList( const char* pcParameterBuffer );
static void kKillTask( const char* pcParameterBuffer );
static void kCPULoad( const char* pcParameterBuffer );
static void kTestMutex( const char* pcParameterBuffer );
static void kCreateThreadTask( void );
static void kTestThread( const char* pcParameterBuffer );
static void kShowMatrix( const char* pcParameterBuffer );
static void kTestPIE( const char* pcParameterBuffer );
static void kShowDyanmicMemoryInformation( const char* pcParameterBuffer );
static void kTestSequentialAllocation( const char* pcParameterBuffer );
static void kTestRandomAllocation( const char* pcParameterBuffer );
static void kRandomAllocationTask( void );
static void kShowHDDInformation( const char* pcParameterBuffer );
static void kReadSector( const char* pcParameterBuffer );
static void kWriteSector( const char* pcParameterBuffer );
static void kMountHDD( const char* pcParameterBuffer );
static void kFormatHDD( const char* pcParameterBuffer );
static void kShowFileSystemInformation( const char* pcParameterBuffer );
static void kCreateFileInRootDirectory( const char* pcParameterBuffer );
static void kDeleteFileInRootDirectory( const char* pcParameterBuffer );
static void kShowRootDirectory( const char* pcParameterBuffer );
static void kWriteDataToFile( const char* pcParameterBuffer );
static void kReadDataFromFile( const char* pcParameterBuffer );
static void kTestFileIO( const char* pcParameterBuffer );
static void kFlushCache( const char* pcParameterBuffer );
static void kTestPerformance( const char* pcParameterBuffer );



static void kPrintUL( const char* pcParameterBuffer );
static void kAddUser( const char* pcParameterBuffer );
static void kChangepasswd( const char* pcParameterBuffer );

static void kDeleteUser( const char* pcParameterBuffer );
void passwdkeyinput(int pwnum);
static void kChangeUser(const char *pcParameterBuffer);
static void kExitUser(const char *pcParameterBuffer);
static void kUidChange(const char *pcParameterBuffer);

#endif /*__CONSOLESHELL_H__*/
