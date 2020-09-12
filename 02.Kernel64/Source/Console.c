
#include <stdarg.h>
#include "Console.h"
#include "Keyboard.h"

// �ܼ��� ������ �����ϴ� �ڷᱸ��
CONSOLEMANAGER gs_stConsoleManager = { 0, };

/**
 *  �ܼ� �ʱ�ȭ
 */
void kInitializeConsole( int iX, int iY )
{
    // �ڷᱸ���� ��� 0���� �ʱ�ȭ
    kMemSet( &gs_stConsoleManager, 0, sizeof( gs_stConsoleManager ) );
    
    // Ŀ�� ��ġ ����
    kSetCursor( iX, iY );
}

/**
 *  Ŀ���� ��ġ�� ����
 *      ���ڸ� ����� ��ġ�� ���� ����
 */
void kSetCursor( int iX, int iY ) 
{
    int iLinearValue;

    // Ŀ���� ��ġ�� ���
    iLinearValue = iY * CONSOLE_WIDTH + iX;

    // CRTC ��Ʈ�� ��巹�� ��������(��Ʈ 0x3D4)�� 0x0E�� �����Ͽ�
    // ���� Ŀ�� ��ġ �������͸� ����
    kOutPortByte( VGA_PORT_INDEX, VGA_INDEX_UPPERCURSOR );
    // CRTC ��Ʈ�� ������ ��������(��Ʈ 0x3D5)�� Ŀ���� ���� ����Ʈ�� ���
    kOutPortByte( VGA_PORT_DATA, iLinearValue >> 8 );

    // CRTC ��Ʈ�� ��巹�� ��������(��Ʈ 0x3D4)�� 0x0F�� �����Ͽ�
    // ���� Ŀ�� ��ġ �������͸� ����
    kOutPortByte( VGA_PORT_INDEX, VGA_INDEX_LOWERCURSOR );
    // CRTC ��Ʈ�� ������ ��������(��Ʈ 0x3D5)�� Ŀ���� ���� ����Ʈ�� ���
    kOutPortByte( VGA_PORT_DATA, iLinearValue & 0xFF );

    // ���ڸ� ����� ��ġ ������Ʈ
    gs_stConsoleManager.iCurrentPrintOffset = iLinearValue;
}

/**
 *  ���� Ŀ���� ��ġ�� ��ȯ
 */
void kGetCursor( int *piX, int *piY )
{
    *piX = gs_stConsoleManager.iCurrentPrintOffset % CONSOLE_WIDTH;
    *piY = gs_stConsoleManager.iCurrentPrintOffset / CONSOLE_WIDTH;
}

/**
 *  printf �Լ��� ���� ����
 */
void kPrintf( const char* pcFormatString, ... )
{
    va_list ap;
    char vcBuffer[ 1024 ];
    int iNextPrintOffset;

    // ���� ���� ����Ʈ�� ����ؼ� vsprintf()�� ó��
    va_start( ap, pcFormatString );
    kVSPrintf( vcBuffer, pcFormatString, ap );
    va_end( ap );
    
    // ���� ���ڿ��� ȭ�鿡 ���
    iNextPrintOffset = kConsolePrintString( vcBuffer );
    
    // Ŀ���� ��ġ�� ������Ʈ
    kSetCursor( iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH );
}

/**
 *  \n, \t�� ���� ���ڰ� ���Ե� ���ڿ��� ����� ��, ȭ����� ���� ����� ��ġ�� 
 *  ��ȯ
 */
int kConsolePrintString( const char* pcBuffer )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDRESS;
    int i, j;
    int iLength;
    int iPrintOffset;
    
    // ���ڿ��� ����� ��ġ�� ����
    iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;

    // ���ڿ��� ���̸�ŭ ȭ�鿡 ���
    iLength = kStrLen( pcBuffer );    
    for( i = 0 ; i < iLength ; i++ )
    {
        // ���� ó��
        if( pcBuffer[ i ] == '\n' )
        {
            // ����� ��ġ�� 80�� ��� �÷����� �ű�
            // ���� ������ ���� ���ڿ��� ����ŭ ���ؼ� ���� �������� ��ġ��Ŵ
            iPrintOffset += ( CONSOLE_WIDTH - ( iPrintOffset % CONSOLE_WIDTH ) );
        }
        // �� ó��
        else if( pcBuffer[ i ] == '\t' )
        {
            // ����� ��ġ�� 8�� ��� �÷����� �ű�
            iPrintOffset += ( 8 - ( iPrintOffset % 8 ) );
        }
        // �Ϲ� ���ڿ� ���
        else
        {
            // ���� �޸𸮿� ���ڿ� �Ӽ��� �����Ͽ� ���ڸ� ����ϰ�
            // ����� ��ġ�� �������� �̵�
            pstScreen[ iPrintOffset ].bCharactor = pcBuffer[ i ];
            pstScreen[ iPrintOffset ].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            iPrintOffset++;
        }
        
        // ����� ��ġ�� ȭ���� �ִ�(80 * 25)�� ������� ��ũ�� ó��
        if( iPrintOffset >= ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) )
        {
            // ���� ������ ������ �������� ���� ���� ����
            kMemCpy( CONSOLE_VIDEOMEMORYADDRESS, 
                     CONSOLE_VIDEOMEMORYADDRESS + CONSOLE_WIDTH * sizeof( CHARACTER ),
                     ( CONSOLE_HEIGHT - 1 ) * CONSOLE_WIDTH * sizeof( CHARACTER ) );

            // ���� ������ ������ �������� ä��
            for( j = ( CONSOLE_HEIGHT - 1 ) * ( CONSOLE_WIDTH ) ; 
                 j < ( CONSOLE_HEIGHT * CONSOLE_WIDTH ) ; j++ )
            {
                // ���� ���
                pstScreen[ j ].bCharactor = ' ';
                pstScreen[ j ].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
            }
            
            // ����� ��ġ�� ���� �Ʒ��� ������ ó������ ����
            iPrintOffset = ( CONSOLE_HEIGHT - 1 ) * CONSOLE_WIDTH;
        }
    }
    return iPrintOffset;
}

/**
 *  ��ü ȭ���� ����
 */
void kClearScreen( void )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDRESS;
    int i;
    
    // ȭ�� ��ü�� �������� ä���, Ŀ���� ��ġ�� 0, 0���� �ű�
    for( i = 0 ; i < CONSOLE_WIDTH * CONSOLE_HEIGHT ; i++ )
    {
        pstScreen[ i ].bCharactor = ' ';
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
    }
    
    // Ŀ���� ȭ�� ������� �̵�
    kSetCursor( 0, 0 );
}

/**
 *  getch() �Լ��� ����
 */
BYTE kGetCh( void )
{
    KEYDATA stData;
    
    // Ű�� ������������ �����
    while( 1 )
    {
        // Ű ť�� �����Ͱ� ���ŵ� ������ ���
        while( kGetKeyFromKeyQueue( &stData ) == FALSE )
        {
            kSchedule();
        }
        
        // Ű�� ���ȴٴ� �����Ͱ� ���ŵǸ� ASCII �ڵ带 ��ȯ
        if( stData.bFlags & KEY_FLAGS_DOWN )
        {
            return stData.bASCIICode;
        }
    }
}

/**
 *  ���ڿ��� X, Y ��ġ�� ���
 */
void kPrintStringXY( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) CONSOLE_VIDEOMEMORYADDRESS;
    int i;
    
    // ���� �޸� ��巹������ ���� ����� ��ġ�� ���
    pstScreen += ( iY * CONSOLE_WIDTH ) + iX;
    // ���ڿ��� ���̸�ŭ ������ ���鼭 ���ڿ� �Ӽ��� ����
    for( i = 0 ; pcString[ i ] != 0 ; i++ )
    {
        pstScreen[ i ].bCharactor = pcString[ i ];
        pstScreen[ i ].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
    }
}

