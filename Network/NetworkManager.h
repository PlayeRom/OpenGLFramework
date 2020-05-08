#pragma once

#define PORT_UDP			3467	//TODO: umiescic w opcjach
#define PORT_TCP			3468	//TODO: umiescic w opcjach
#define DATA_SIZE			1024
#define PROTOCOL_HEADER		"oGLf"	//size = HEADER_SIZE
#define PROTOCOL_END_DATA	"EndD"	//size = HEADER_SIZE
#define HEADER_SIZE			4		//Wielkoœæ PROTOCOL_HEADER i PROTOCOL_END_DATA
#define HOSTNAME_SIZE		64		//64 bo tyle mozna podac w systemie

/// Typ wyliczeniowy rodzajów pakietów.
enum EPacketType
{
	/// Pakiet wysy³any na chat informuj¹cy o do³¹czeniu u¿ytkownika.
	/** SPacketUDP::cData zawiera pe³ny komuniat, np: "[12:00] Pseudonim joined".	*/
	EPacketTypeJoinUser,

	/// Pakiet wiadomoœci wys³anej na chat.
	/** SPacketUDP::cData zawiera pe³ny komuniat, np: "[12:00] <Pseudonim> Treœæ wiadoœci".	*/
	EPacketTypeChatMsg,

	/// Pakiet wysy³any co 5 sekund informuj¹cy wszystkich o stworzeniu gry.
	/** SPacketUDP::cData zawiera strukturê SPacketCreateEcho. */
	EPacketTypeCreateEcho,

	/// Pakiet informauj¹cy o do³¹czeniu siê gracza do gry.
	/** SPacketUDP::cData zawiera strukturê SPacketJoin. */
	EPacketTypeJoin,

	/// Pakiet informuj¹cy do³¹czaj¹cego, ¿e za³o¿yciel gry ju¿ nie oczekuje na po³¹czenie.
	/** SPacketUDP::cData puste. */
	EPacketTypeCreatorNotWait,

	/// Pekiet informuj¹cy do³¹czaj¹cego o akceptacji po³¹czenia przez za³o¿yciela gry.
	/** SPacketUDP::cData puste. */
	EPacketTypeAcceptJoin,

	/// Pakiet informuj¹cy do³¹czaj¹cego o odrzuceniu po³¹czenia przez za³o¿yciela gry.
	/** SPacketUDP::cData zawiera pseudonim gracza tworz¹cego grê. */
	EPacketTypeRejectJoin,

	/// Pakiet informuj¹cy za³o¿yciela gry o zawi¹zaniu transakcji po³¹czenia.
	/** SPacketUDP::cData puste. */
	EPacketTypeTransactionOfConnectDone,

	/// Pakiet informuj¹cy za³o¿yciela gry o rozpoczêciu gry.
	/** SPacketUDP::cData puste. */
	EPacketTypeStartMultiGame
};

/// Uniwersalna struktura, która zawsze jest wysy³ana protoko³em UDP.
struct SPacketUDP
{
	char cHeader[ HEADER_SIZE ];			///< Nag³ówek identyfikacyjny. Zawsze PROTOCOL_HEADER.
	int iPacketType;						///< Typ pakietu.
	unsigned int uiIPSender;				///< Adres IP nadawcy pakietu.
	unsigned int uiIPReceiver;				///< Adres IP odbiorcy pakietu.

	int iDataLength;						///< Wielkoœæ przesy³anych danych.
	unsigned char cData[DATA_SIZE];			///< Przesy³ane dane. Musi koñczyæ siê PROTOCOL_END_DATA.
};

#define PACKET_SIZE sizeof( SPacketUDP )

/// Struktura przekazywana do w¹tku wysy³aj¹cego i odbieraj¹cego transfer TCP.
struct STransferTCPData
{
	bool bSender;							///< Flaga informuj¹ca czy jestem nadawc¹ czy odbiorc¹ danych.
	int iPortNumber;						///< Numer portu, na którym nast¹pi po³¹czenie.
	char *pData;							///< WskaŸnik na dane do wys³ania/odebrania przez transfer TCP.
	int iDataLength;						///< Wielkoœæ (w bajtach) danych zawartych w pData;
	
	/// WskaŸnik na funkcjê, która zostanie wywo³ana po zakoñczeniu w¹tku wysy³aj¹cego/odbieraj¹cego.
	/**  Funkcja ta musi przyjmowaæ dwa parametry. Pierwszy typu \p int bêd¹cy rezultatem dzia³ania w¹tku. Je¿eli
	funkcja otrzyma wartoœæ 0 to praca w¹tku zakoñczy³a siê niepowodzeniem. Je¿eli 1 to praca w¹tku zakoñczy³a
	siê pomyœlnie. Drugi parametr to wskaŸnik na w³as¹ strukturê STransferTCPData.
	Zamiast funkcji mo¿emy tak¿e podaæ tutaj wartoœæ \p NULL. */
	void( *Callback )( int, STransferTCPData* );
};

class CNetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager();

	GLboolean CreateUDPSocket();
	GLboolean StartRecvUDPThread();
	GLboolean StopRecvUDPThread();
	GLboolean SendUDP( int iPacketType, const void *pData, int iDataLength, const char *cIPReceiver );
	GLboolean SendUDP( int iPacketType, const void *pData, int iDataLength, unsigned int uiIPReceiver = INADDR_BROADCAST );

	SOCKET GetSocketUDP()		{ return m_uiSocketUDP; }
	SOCKADDR_IN* GetRecvAddr()	{ return &m_sRecvAddr; }

	char* GetHostName()			{ return m_cMyHostName; }
	char* GetMyIP()				{ return m_pMyIP; }

	/// Metoda konwertuj¹ca adres IP z liczby na tekst w UNICODE lub ASCII.
	/** \param[in] uiIP Liczba reprezentuj¹ca adres IP. 
	\return £acuch znaków adresu IP otrzymanego z liczby, np: _T("192.168.0.1"). */
	LPTSTR IPntoaw( unsigned int uiIP );

	///////////////////// TCP //////////////////////////////////////////////

	GLboolean StartSendTCPTransfer( STransferTCPData *in_sTransferTCPData );
	GLboolean StopSendTCPTransfer();
	GLboolean StartRecvTCPTransfer( STransferTCPData *out_sTransferTCPData );
	GLboolean StopRecvTCPTransfer();

private:
	GLboolean m_bInitSockets;
	GLvoid CheckMyIPAndHost();
	char m_cMyHostName[ 256 ];
	char *m_pMyIP;

	/// Uchwyt gniazda protoko³u UDP.
	SOCKET m_uiSocketUDP;

	SOCKADDR_IN	m_sRecvAddr;	//UDP odbieraj¹cy
	SOCKADDR_IN	m_sSendAddr;	//UDP wysy³aj¹cy

	/// Uchwyt do w¹tku odbieraj¹cego pakiety UDP.
	HANDLE m_hRecvUDPThread;

	/// Uchwyt do w¹tku transmituj¹cego dane na protokole TCP. 
	HANDLE m_hSendTCPTransfer;

	/// Uchwyt do w¹tku odbieraj¹cego dane z transmisji na protokole TCP. 
	HANDLE m_hRecvTCPTransfer;

	/// Metoda konwertuj¹ca adres IP z liczby na tekst w ASCII.
	/** \param[in] uiIP Liczba reprezentuj¹ca adres IP. 
	\return £añcuch znaków adresu IP otrzymanego z liczby, np: "192.168.0.1". */
	char* IPntoa( unsigned int uiIP );

	/// Metoda konwertuj¹ca adres IP z liczby na tekst w UNICODE.
	/** \param[in] uiIP Liczba reprezentuj¹ca adres IP. 
	\return £añcuch znaków adresu IP otrzymanego z liczby, np: _T("192.168.0.1"). */
	wchar_t* IPntow( unsigned int uiIP );
};
