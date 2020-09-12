/**
 *  file    List.h
 *  date    2009/02/23
 *  author  kkamagui 
 *          Copyright(c)2008 All rights reserved by kkamagui
 *  brief   ����Ʈ�� ���õ� �Լ��� ������ ��� ����
 */

#ifndef __LIST_H__
#define __LIST_H__

#include "Types.h" 

////////////////////////////////////////////////////////////////////////////////
//
// ����ü
//
////////////////////////////////////////////////////////////////////////////////
// 1����Ʈ�� ����
#pragma pack( push, 1 )

// �����͸� �����ϴ� �ڷᱸ��
// �ݵ�� �������� ���� �պκп� ��ġ�ؾ� ��
typedef struct kListLinkStruct
{
    // ���� �������� ��巹���� �����͸� �����ϱ� ���� ID
    void* pvNext;
    QWORD qwID;
} LISTLINK;

/*
// ����Ʈ�� ����� �����͸� �����ϴ� ��
// �ݵ�� ���� �պκ��� LISTLINK�� �����ؾ� ��
struct kListItemExampleStruct
{
    // ����Ʈ�� �����ϴ� �ڷᱸ��
    LISTLINK stLink;
    
    // �����͵�
    int iData1;
    char cData2;
};
*/

// ����Ʈ�� �����ϴ� �ڷᱸ��
typedef struct kListManagerStruct
{
    // ����Ʈ �������� ��
    int iItemCount;

    // ����Ʈ�� ù ��°�� ������ �������� ��巹��
    void* pvHeader;
    void* pvTail;
} LIST;

#pragma pack( pop )


////////////////////////////////////////////////////////////////////////////////
//
// �Լ�
//
////////////////////////////////////////////////////////////////////////////////
void kInitializeList( LIST* pstList );
int kGetListCount( const LIST* pstList );
void kAddListToTail( LIST* pstList, void* pvItem );
void kAddListToHeader( LIST* pstList, void* pvItem );
void* kRemoveList( LIST* pstList, QWORD qwID );
void* kRemoveListFromHeader( LIST* pstList );
void* kRemoveListFromTail( LIST* pstList );
void* kFindList( const LIST* pstList, QWORD qwID );
void* kGetHeaderFromList( const LIST* pstList );
void* kGetTailFromList( const LIST* pstList );
void* kGetNextFromList( const LIST* pstList, void* pstCurrent );

#endif /*__LIST_H__*/
