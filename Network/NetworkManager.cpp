
#include "StdAfx.h"
#include "../OpenGLFramework/GameControl.h"
#include "NetworkManager.h"

unsigned int __stdcall RecvUDPThread( void *lpParam );
GLvoid AnalyseRecvPacket( SPacketUDP &sPacket, CNetworkManager *sNetManager );

unsigned int __stdcall SendTCPTransfer( void *lpParam );
unsigned int __stdcall RecvTCPTransfer( void *lpParam );

CNetworkManager::CNetworkManager()
{
	::SecureZeroMemory( m_cMyHostName, sizeof( m_cMyHostName ) );
	m_pMyIP = NULL;
	m_hRecvUDPThread = NULL;
	m_hSendTCPTransfer = NULL;
	m_hRecvTCPTransfer = NULL;
	m_uiSocketUDP = INVALID_SOCKET;

	m_bInitSockets = GL_TRUE;
	WSADATA wsaData;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		m_bInitSockets = GL_FALSE;
	
	CheckMyIPAndHost();
	//CreateUDPSocket();
}

CNetworkManager::~CNetworkManager()
{
	StopRecvUDPThread();

	closesocket( m_uiSocketUDP );

	if( m_bInitSockets )
		WSACleanup();
}

GLvoid CNetworkManager::CheckMyIPAndHost()
{
	if( !m_bInitSockets )
		return;

	PHOSTENT pHostInfo = NULL;
	if( gethostname( m_cMyHostName, sizeof( m_cMyHostName ) ) == 0 ) {
		pHostInfo = gethostbyname( m_cMyHostName );
		if( pHostInfo ) {
			int i = 0;
			//while( pHostInfo->h_addr_list[ i ] != NULL ) {
			m_pMyIP = inet_ntoa( *( struct in_addr* )pHostInfo->h_addr_list[ i ] );
			//	__LOG_FORMATTED("My IP %d = %s", i, m_pMyIP );
			//	++i;
			//}
		}
	}
}

GLboolean CNetworkManager::CreateUDPSocket()
{
	if( !m_bInitSockets )
		return GL_FALSE;

	//na wypadek ponownego tworzenia gniazda gdy juz istnieje
	bool bWasRecvUDPThread = false;
	if( m_uiSocketUDP != INVALID_SOCKET ) {
		if( StopRecvUDPThread() )
			bWasRecvUDPThread = true;
		
		closesocket( m_uiSocketUDP );
	}

	m_uiSocketUDP = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( m_uiSocketUDP == INVALID_SOCKET ) {
		__LOG_LAST_ERROR( _T("Create UDP socket") );
		return GL_FALSE;
	}

	int opt = 1;
	setsockopt( m_uiSocketUDP, SOL_SOCKET, SO_BROADCAST, ( char* )&opt, sizeof( opt ) );
	opt = PACKET_SIZE * 10;
	setsockopt( m_uiSocketUDP, SOL_SOCKET, SO_RCVBUF, ( char* )&opt, sizeof( opt ) );
	setsockopt( m_uiSocketUDP, SOL_SOCKET, SO_SNDBUF, ( char* )&opt, sizeof( opt ) );
	//
	m_sRecvAddr.sin_family = AF_INET;
	m_sRecvAddr.sin_port = htons( ( unsigned short )PORT_UDP );
	m_sRecvAddr.sin_addr.s_addr = htonl( INADDR_ANY );
//	m_sRecvAddr.sin_addr.S_un.S_addr = inet_addr( m_pMyIP );
	//
	if( bind( m_uiSocketUDP, ( SOCKADDR* )&m_sRecvAddr, sizeof( m_sRecvAddr ) ) == SOCKET_ERROR ) {
		__LOG_LAST_ERROR( _T("Bidn UDP socket") );
		return GL_FALSE;
	}

	if( bWasRecvUDPThread )
		StartRecvUDPThread();

	return GL_TRUE;
}

GLboolean CNetworkManager::StartRecvUDPThread()
{
	if( !m_bInitSockets )
		return GL_FALSE;
	
	if( m_hRecvUDPThread || m_uiSocketUDP == INVALID_SOCKET )
		return GL_FALSE; //watek juz jest || gniazdo nie istnieje
	
	unsigned int uiThreadId;
	m_hRecvUDPThread = ( HANDLE )::_beginthreadex( NULL, 0, &RecvUDPThread, ( void* )this, 0, &uiThreadId );
	return m_hRecvUDPThread ? GL_TRUE : GL_FALSE;
}

GLboolean CNetworkManager::StopRecvUDPThread()
{
	if( !m_bInitSockets )
		return GL_FALSE;

	if( !m_hRecvUDPThread )
		return GL_FALSE;

	if( ::TerminateThread( m_hRecvUDPThread, 0 ) == 0 )
		return GL_FALSE;
	
	::CloseHandle( m_hRecvUDPThread );
	m_hRecvUDPThread = NULL;
	
	return GL_TRUE;
}

unsigned int __stdcall RecvUDPThread( void *lpParam )
{
	CNetworkManager *sNetManager = ( CNetworkManager* )lpParam;
	if( !sNetManager )
		return 1;

	char cRecvBuffer[ PACKET_SIZE * 3 ];
	int iRecvBufferSize = PACKET_SIZE;
	//
	char cMsgBuffer[ PACKET_SIZE ];
	int iMsgBufferCounter = 0;
	//
	SPacketUDP sPacket = { 0 };
	int iAddrInSize = sizeof( sockaddr_in );

	bool bRecvLoop = true;
	while( bRecvLoop ) {
		int iRecvBytes = recvfrom( sNetManager->GetSocketUDP(),
								   ( char* )&cRecvBuffer,
								   iRecvBufferSize,
								   0,
								   ( SOCKADDR* )sNetManager->GetRecvAddr(),
								   &iAddrInSize );
		if( iRecvBytes == SOCKET_ERROR ) {
			__LOG_LAST_ERROR( _T("Receive UDP socket") );
			continue;
		}

		for( int i = 0; i < iRecvBytes; ++i ) {
			if( iMsgBufferCounter > PACKET_SIZE )
				break;
			
			cMsgBuffer[ iMsgBufferCounter ] = cRecvBuffer[ i ];
			++iMsgBufferCounter;
			if( iMsgBufferCounter > PACKET_SIZE - DATA_SIZE - 4 ) {
				// znajdz doklejany ciag do danych: PROTOCOL_END_DATA
				bool bIsPacket = true;
				for( int j = 0; j < HEADER_SIZE; ++j ) {
					if( cMsgBuffer[ iMsgBufferCounter - HEADER_SIZE + j ] != PROTOCOL_END_DATA[ j ] ) {
						bIsPacket = false;
						break;
					}
				}
				if( bIsPacket ) {
					memcpy_s( &sPacket, sizeof( sPacket ), cMsgBuffer, iMsgBufferCounter );
					::SecureZeroMemory( cMsgBuffer, sizeof( cMsgBuffer ) );
					iMsgBufferCounter = 0;

					AnalyseRecvPacket( sPacket, sNetManager );
				}
			}
		}
	}
	
	return 0;
}

GLvoid AnalyseRecvPacket( SPacketUDP &sPacket, CNetworkManager *sNetManager )
{
	//sprawdz czy to nasz pakiet
	for( int i = 0; i < HEADER_SIZE; ++i ) {
		if( sPacket.cHeader[ i ] != PROTOCOL_HEADER[ i ] )
			return;
	}

	// czy pakiet jest przeznaczony dla nas
	if( sPacket.uiIPReceiver != INADDR_BROADCAST && sPacket.uiIPReceiver != inet_addr( sNetManager->GetMyIP() ) )
		return;

	// obciecie "EndD"
	sPacket.iDataLength -= 4;
	if( sPacket.iDataLength >= DATA_SIZE )
		sPacket.cData[ DATA_SIZE - 1 ] = 0;
	else if( sPacket.iDataLength >= 0 )
		sPacket.cData[ sPacket.iDataLength ] = 0;
	else
		::SecureZeroMemory( sPacket.cData, DATA_SIZE );

	//TODO: Tutaj obs³uga pakietu
	//CGameControl::GetInstance()->GetMyClass()->AnalyseRecvPacket( sPacket );
}

GLboolean CNetworkManager::SendUDP( int iPacketType, const void *pData, int iDataLength, const char *cIPReceiver )
{
	if( !m_bInitSockets || m_uiSocketUDP == INVALID_SOCKET || ( pData == NULL && iDataLength != 0 ) || cIPReceiver == NULL )
		return GL_FALSE;

	return SendUDP( iPacketType, pData, iDataLength, inet_addr( cIPReceiver ) );
}

GLboolean CNetworkManager::SendUDP( int iPacketType, const void *pData, int iDataLength, unsigned int uiIPReceiver /*= INADDR_BROADCAST*/ )
{
	if( !m_bInitSockets || m_uiSocketUDP == INVALID_SOCKET || ( pData == NULL && iDataLength != 0 ) )
		return GL_FALSE;

	if( iDataLength >= DATA_SIZE - 4 )
		return GL_FALSE;

	//uzupelnij strukture do wyslania
	SPacketUDP sPacket = { 0 };
	for( int i = 0; i < HEADER_SIZE; ++i )
		sPacket.cHeader[ i ] = PROTOCOL_HEADER[ i ];
	sPacket.iPacketType = iPacketType;
	sPacket.uiIPSender = inet_addr( m_pMyIP );
	sPacket.uiIPReceiver = uiIPReceiver;

	if( pData )
		memcpy_s( sPacket.cData, DATA_SIZE, pData, iDataLength );
	for( int i = 0; i < HEADER_SIZE; ++i )
		sPacket.cData[ iDataLength + i ] = PROTOCOL_END_DATA[ i ];
	sPacket.iDataLength = iDataLength + 4;
	//sPacket.cData = np. "[Jakies dane]PROTOCOL_END_DATA", czyli trzeba odci¹æ PROTOCOL_END_DATA i zamiast tego wstawiæ '\0'

	m_sSendAddr.sin_family = AF_INET;
	m_sSendAddr.sin_port = htons( ( unsigned short )PORT_UDP );
	//m_sSendAddr.sin_addr.S_un.S_addr = uiIPReceiver; // <- normalnie to ma byc
	// TODO: przywrocic linijke wyzej a ponizej usunac - to tylko dla testow aby dzialalo wysylanie
	m_sSendAddr.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1" );
	//
	int iSentBytes = sendto( m_uiSocketUDP,
							 ( const char* )&sPacket,
							 PACKET_SIZE - DATA_SIZE + sPacket.iDataLength,
							 0,
							 ( SOCKADDR* )&m_sSendAddr,
							 sizeof( sockaddr_in ) );

	if( iSentBytes == SOCKET_ERROR ) {
		__LOG_LAST_ERROR( _T("Send UDP packet") );
		return GL_FALSE;
	}

	return GL_TRUE;
}

char* CNetworkManager::IPntoa( unsigned int uiIP )
{
	if( uiIP == 0 )
		return "";

	in_addr addr;
	addr.S_un.S_addr = uiIP;
	return inet_ntoa( addr );
}

wchar_t* CNetworkManager::IPntow( unsigned int uiIP )
{
	char* cIP = IPntoa( uiIP );
	return CUHF::ConvertCharToWchar( cIP, ( int )strlen( cIP ) );
}

LPTSTR CNetworkManager::IPntoaw( unsigned int uiIP )
{
#ifdef UNICODE
	return IPntow( uiIP );
#else
	return IPntoa( uiIP );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// TCP ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

GLboolean CNetworkManager::StartSendTCPTransfer( STransferTCPData *in_sTransferTCPData )
{
	if( !m_bInitSockets )
		return GL_FALSE;

	if( m_hSendTCPTransfer )
		return GL_FALSE; //watek juz jest

	unsigned int uiThreadId;
	m_hSendTCPTransfer = ( HANDLE )::_beginthreadex( NULL, 0, &SendTCPTransfer, ( void* )in_sTransferTCPData, 0, &uiThreadId );

	return m_hSendTCPTransfer ? GL_TRUE : GL_FALSE;
}

GLboolean CNetworkManager::StopSendTCPTransfer()
{
	if( !m_bInitSockets )
		return GL_FALSE;

	if( !m_hSendTCPTransfer )
		return GL_FALSE;

	if( ::TerminateThread( m_hSendTCPTransfer, 0 ) == 0 )
		return GL_FALSE;
	
	::CloseHandle( m_hSendTCPTransfer );
	m_hSendTCPTransfer = NULL;
	
	return GL_TRUE;
}

GLboolean CNetworkManager::StartRecvTCPTransfer( STransferTCPData *out_sTransferTCPData )
{
	if( !m_bInitSockets )
		return GL_FALSE;

	if( m_hRecvTCPTransfer )
		return GL_FALSE; //watek juz jest

	unsigned int uiThreadId;
	m_hRecvTCPTransfer = ( HANDLE )::_beginthreadex( NULL, 0, &RecvTCPTransfer, ( void* )out_sTransferTCPData, 0, &uiThreadId );

	return m_hRecvTCPTransfer ? GL_TRUE : GL_FALSE;
}

GLboolean CNetworkManager::StopRecvTCPTransfer()
{
	if( !m_bInitSockets )
		return GL_FALSE;

	if( !m_hRecvTCPTransfer )
		return GL_FALSE;

	if( ::TerminateThread( m_hRecvTCPTransfer, 0 ) == 0 )
		return GL_FALSE;
	
	::CloseHandle( m_hRecvTCPTransfer );
	m_hRecvTCPTransfer = NULL;
	
	return GL_TRUE;
}

// Funkcja wysy³aj¹ca jednorazowo uniwersalne dane po TCP
unsigned int __stdcall SendTCPTransfer( void *lpParam )
{
	int iResult = 0;
	STransferTCPData *lpTransfer = ( STransferTCPData* )lpParam;
	if( !lpTransfer )
		goto finish;

	lpTransfer->bSender = true;
	
	SOCKADDR_IN sHostAddr;
	char cBuffer[ 2050 ];
	int iBytes = 0; // ilosc wyslanych bajtow

	SOCKET uiConnectSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( uiConnectSocket == INVALID_SOCKET ) {
		__LOG_LAST_ERROR( _T("SendTCPTransfer: Create TCP socket") );
		goto finish;
	}
	
	sHostAddr.sin_family = AF_INET;
	sHostAddr.sin_port = htons( ( unsigned short )lpTransfer->iPortNumber );
	sHostAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	//sHostAddr.sin_addr.S_un.S_addr = inet_addr( lpTransfer->strIPUser );
	//memset( &( sHostAddr.sin_zero ), '\0', 8 );

	::Sleep( 500 );
	if( connect( uiConnectSocket, ( struct sockaddr* )&sHostAddr, sizeof( SOCKADDR ) ) == INVALID_SOCKET ) {
		__LOG_LAST_ERROR( _T("SendTCPTransfer: Connect TCP") );
		closesocket( uiConnectSocket );
		goto finish;
	}

	while( iBytes < lpTransfer->iDataLength ) {
		int iCounter = -1;
		while( iCounter < 2048 && iBytes < lpTransfer->iDataLength ) {
			++iBytes;
			cBuffer[ ++iCounter ] = lpTransfer->pData[ iBytes ];
		}

		if( send( uiConnectSocket, cBuffer, iBytes, 0 ) == SOCKET_ERROR ) {
			__LOG_LAST_ERROR( _T("SendTCPTransfer: Send TCP") );
			closesocket( uiConnectSocket );
			goto finish;
		}
	}
	iResult = 1;
	closesocket( uiConnectSocket );

finish:

	if( lpTransfer->Callback ) {
		lpTransfer->Callback( iResult, lpTransfer );
	}

	return 0;
}

// Funkcja odbieraj¹ca jednorazowo uniwersalne dane po TCP.
unsigned int __stdcall RecvTCPTransfer( void *lpParam )
{
	int iResult = 0;
	STransferTCPData *lpTransfer = ( STransferTCPData* )lpParam;
	if( !lpTransfer )
		goto finish;

	lpTransfer->bSender = false;
	
	SOCKET uiListenSocket, uiAcceptSocket;
	SOCKADDR_IN sHostAddr;

	uiListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( uiListenSocket == INVALID_SOCKET ) {
		__LOG_LAST_ERROR( _T("RecvTCPTransfer: Create TCP socket") );
		goto finish;
	}

	sHostAddr.sin_family = AF_INET;
	sHostAddr.sin_port = htons( ( unsigned short )lpTransfer->iPortNumber );
	sHostAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	//sHostAddr.sin_addr.S_un.S_addr = inet_addr( lpTransfer->strMojIP );
	//memset( &( HostAddr.sin_zero ), '\0', 8 ); 

	if( bind( uiListenSocket, ( SOCKADDR* )&sHostAddr, sizeof( SOCKADDR ) ) != 0 ) {
		__LOG_LAST_ERROR( _T("RecvTCPTransfer: Bind TCP") );
		closesocket( uiListenSocket );
		goto finish;
	}

	listen( uiListenSocket, 1 );

	uiAcceptSocket = accept( uiListenSocket, NULL, NULL );
	if( uiAcceptSocket == INVALID_SOCKET ) {
		__LOG_LAST_ERROR( _T("RecvTCPTransfer: Accept TCP") );
		closesocket( uiAcceptSocket );
		closesocket( uiListenSocket );
		goto finish;
	}

	char cBuffer[ 4100 ];
	int iRecvBytes;

	if( lpTransfer->pData )
		delete [] lpTransfer->pData;
	lpTransfer->pData = NULL;
	lpTransfer->iDataLength = 0;
	
	while( ( iRecvBytes = recv( uiAcceptSocket, cBuffer, 4096, 0 ) ) != 0 ) {
		int iActualBufforSize = lpTransfer->iDataLength + iRecvBytes;
		if( iActualBufforSize > lpTransfer->iDataLength ) { //<- w sumie zawsze true
			//zapisz aktualne dane do tempa
			char *cTempBuf = NULL;
			if( lpTransfer->iDataLength ) {
				cTempBuf = new char[ lpTransfer->iDataLength ];
				memcpy_s( cTempBuf, lpTransfer->iDataLength, lpTransfer->pData, lpTransfer->iDataLength );
			}

			//powieksz bufor do aktualnej wielkosci
			if( lpTransfer->pData )
				delete [] lpTransfer->pData;
			lpTransfer->pData = new char[ iActualBufforSize ];

			//wrzuc do bufora to co jest juz w tempie
			for( int i = 0; i < lpTransfer->iDataLength; ++i )
				lpTransfer->pData[ i ] = cTempBuf[ i ];

			//usun tempa
			if( cTempBuf )
				delete [] cTempBuf;
		}
		for( int i = lpTransfer->iDataLength, j = 0; i < iActualBufforSize && j < iRecvBytes; ++i, ++j )
			lpTransfer->pData[ i ] = cBuffer[ j ];

		lpTransfer->iDataLength = iActualBufforSize;
	}
	closesocket( uiAcceptSocket );
	closesocket( uiListenSocket );

	iResult = 1;

finish:

	if( lpTransfer->Callback )
		lpTransfer->Callback( iResult, lpTransfer );

	return 0;
}