//---------------------------------------------------------------------------

#ifndef ConnectionsH
#define ConnectionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

//---------------------------------------------------------------------------
// Ras Stuff
//---------------------------------------------------------------------------
#define		OLDWINVER WINVER
#undef		WINVER
#define		WINVER 0x400

#ifndef UNLEN
#include <lmcons.h>
#endif

#define RAS_MaxDeviceType     16
#define RAS_MaxPhoneNumber    128
#define RAS_MaxIpAddress      15
#define RAS_MaxIpxAddress     21

#if (WINVER >= 0x400)
#define RAS_MaxEntryName      256
#define RAS_MaxDeviceName     128
#define RAS_MaxCallbackNumber RAS_MaxPhoneNumber
#else
#define RAS_MaxEntryName      20
#define RAS_MaxDeviceName     32
#define RAS_MaxCallbackNumber 48
#endif

#define RAS_MaxAreaCode       10
#define RAS_MaxPadType        32
#define RAS_MaxX25Address     200
#define RAS_MaxFacilities     200
#define RAS_MaxUserData       200
#define RAS_MaxReplyMessage   1024
#define RAS_MaxDnsSuffix      256

DECLARE_HANDLE( HRASCONN );
#define LPHRASCONN HRASCONN*

#if (WINVER >= 0x501)
#define RASCF_AllUsers		0x00000001
#define RASCF_GlobalCreds	0x00000002
#endif

// Identifies an active RAS connection.  (See RasEnumConnections)

#define RASCONNW struct tagRASCONNW
RASCONNW
{
    DWORD    dwSize;
    HRASCONN hrasconn;
    WCHAR    szEntryName[ RAS_MaxEntryName + 1 ];

#if (WINVER >= 0x400)
    WCHAR    szDeviceType[ RAS_MaxDeviceType + 1 ];
    WCHAR    szDeviceName[ RAS_MaxDeviceName + 1 ];
#endif
#if (WINVER >= 0x401)
    WCHAR    szPhonebook [ MAX_PATH ];
    DWORD    dwSubEntry;
#endif
#if (WINVER >= 0x500)
    GUID    guidEntry;
#endif
#if (WINVER >= 0x501)
	DWORD	dwFlags;
	LUID    luid;
#endif
};

#define RASCONNA struct tagRASCONNA
RASCONNA
{
    DWORD    dwSize;
    HRASCONN hrasconn;
    CHAR     szEntryName[ RAS_MaxEntryName + 1 ];

#if (WINVER >= 0x400)
	 CHAR     szDeviceType[ RAS_MaxDeviceType + 1 ];
    CHAR     szDeviceName[ RAS_MaxDeviceName + 1 ];
#endif
#if (WINVER >= 0x401)
    CHAR     szPhonebook [ MAX_PATH ];
    DWORD    dwSubEntry;
#endif
#if (WINVER >= 0x500)
    GUID     guidEntry;
#endif
#if (WINVER >= 0x501)
	DWORD	dwFlags;
	LUID    luid;
#endif
};

#ifdef UNICODE
#define RASCONN RASCONNW
#else
#define RASCONN RASCONNA
#endif

#define LPRASCONNW RASCONNW*
#define LPRASCONNA RASCONNA*
#define LPRASCONN  RASCONN*


// Enumerates intermediate states to a connection.  (See RasDial)

#define RASCS_PAUSED 0x1000
#define RASCS_DONE   0x2000

#define RASCONNSTATE enum tagRASCONNSTATE
RASCONNSTATE
{
    RASCS_OpenPort = 0,
    RASCS_PortOpened,
    RASCS_ConnectDevice,
    RASCS_DeviceConnected,
    RASCS_AllDevicesConnected,
    RASCS_Authenticate,
	 RASCS_AuthNotify,
    RASCS_AuthRetry,
    RASCS_AuthCallback,
    RASCS_AuthChangePassword,
    RASCS_AuthProject,
    RASCS_AuthLinkSpeed,
    RASCS_AuthAck,
    RASCS_ReAuthenticate,
    RASCS_Authenticated,
    RASCS_PrepareForCallback,
    RASCS_WaitForModemReset,
    RASCS_WaitForCallback,
    RASCS_Projected,

#if (WINVER >= 0x400)
    RASCS_StartAuthentication,
    RASCS_CallbackComplete,
    RASCS_LogonNetwork,
#endif
    RASCS_SubEntryConnected,
    RASCS_SubEntryDisconnected,

    RASCS_Interactive = RASCS_PAUSED,
    RASCS_RetryAuthentication,
    RASCS_CallbackSetByCaller,
    RASCS_PasswordExpired,
#if (WINVER >= 0x500)
    RASCS_InvokeEapUI,
#endif

    RASCS_Connected = RASCS_DONE,
    RASCS_Disconnected
};

#define LPRASCONNSTATE RASCONNSTATE*


// Describes the status of a RAS connection.  (See RasConnectionStatus)

#define RASCONNSTATUSW struct tagRASCONNSTATUSW
RASCONNSTATUSW
{
    DWORD        dwSize;
    RASCONNSTATE rasconnstate;
    DWORD        dwError;
    WCHAR        szDeviceType[ RAS_MaxDeviceType + 1 ];
    WCHAR        szDeviceName[ RAS_MaxDeviceName + 1 ];
#if (WINVER >= 0x401)
    WCHAR        szPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
#endif
};

#define RASCONNSTATUSA struct tagRASCONNSTATUSA
RASCONNSTATUSA
{
    DWORD        dwSize;
    RASCONNSTATE rasconnstate;
    DWORD        dwError;
    CHAR         szDeviceType[ RAS_MaxDeviceType + 1 ];
    CHAR         szDeviceName[ RAS_MaxDeviceName + 1 ];
#if (WINVER >= 0x401)
    CHAR         szPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
#endif
};

#ifdef UNICODE
#define RASCONNSTATUS RASCONNSTATUSW
#else
#define RASCONNSTATUS RASCONNSTATUSA
#endif

#define LPRASCONNSTATUSW RASCONNSTATUSW*
#define LPRASCONNSTATUSA RASCONNSTATUSA*
#define LPRASCONNSTATUS  RASCONNSTATUS*

// Describes connection establishment parameters.  (See RasDial)

#define RASDIALPARAMSW struct tagRASDIALPARAMSWRASDIALPARAMS
RASDIALPARAMSW
{
	 DWORD dwSize;
	 WCHAR szEntryName[ RAS_MaxEntryName + 1 ];
	 WCHAR szPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
	 WCHAR szCallbackNumber[ RAS_MaxCallbackNumber + 1 ];
	 WCHAR szUserName[ UNLEN + 1 ];
	 WCHAR szPassword[ PWLEN + 1 ];
	 WCHAR szDomain[ DNLEN + 1 ];
#if (WINVER >= 0x401)
	 DWORD dwSubEntry;
	 ULONG_PTR dwCallbackId;
#endif
};

#define RASDIALPARAMSA struct tagRASDIALPARAMSA
RASDIALPARAMSA
{
	 DWORD dwSize;
	 CHAR  szEntryName[ RAS_MaxEntryName + 1 ];
	 CHAR  szPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    CHAR  szCallbackNumber[ RAS_MaxCallbackNumber + 1 ];
	 CHAR  szUserName[ UNLEN + 1 ];
    CHAR  szPassword[ PWLEN + 1 ];
    CHAR  szDomain[ DNLEN + 1 ];
#if (WINVER >= 0x401)
	 DWORD dwSubEntry;
	 ULONG_PTR dwCallbackId;
#endif
};

#ifdef UNICODE
#define RASDIALPARAMS RASDIALPARAMSW
#else
#define RASDIALPARAMS RASDIALPARAMSA
#endif

#define LPRASDIALPARAMSW RASDIALPARAMSW*
#define LPRASDIALPARAMSA RASDIALPARAMSA*
#define LPRASDIALPARAMS  RASDIALPARAMS*

#if (WINVER >= 0x500)

#define RASEAPINFO struct tagRASEAPINFO
RASEAPINFO
{
    DWORD dwSizeofEapInfo;
    BYTE  *pbEapInfo;
};

#endif

// Describes extended connection establishment options.  (See RasDial)

#define RASDIALEXTENSIONS struct tagRASDIALEXTENSIONS
RASDIALEXTENSIONS
{
	 DWORD     dwSize;
	 DWORD     dwfOptions;
	 HWND      hwndParent;
	 ULONG_PTR reserved;
#if (WINVER >= 0x500)
	 ULONG_PTR reserved1;
	 RASEAPINFO RasEapInfo;
#endif
};

#define LPRASDIALEXTENSIONS RASDIALEXTENSIONS*

// 'dwfOptions' bit flags.

#define RDEOPT_UsePrefixSuffix           0x00000001
#define RDEOPT_PausedStates              0x00000002
#define RDEOPT_IgnoreModemSpeaker        0x00000004
#define RDEOPT_SetModemSpeaker           0x00000008
#define RDEOPT_IgnoreSoftwareCompression 0x00000010
#define RDEOPT_SetSoftwareCompression    0x00000020
#define RDEOPT_DisableConnectedUI        0x00000040
#define RDEOPT_DisableReconnectUI        0x00000080
#define RDEOPT_DisableReconnect          0x00000100
#define RDEOPT_NoUser                    0x00000200
#define RDEOPT_PauseOnScript             0x00000400
#define RDEOPT_Router                    0x00000800

#if (WINVER >= 0x500)
#define RDEOPT_CustomDial                0x00001000
#endif

#if (WINVER >= 0x501)
#define RDEOPT_UseCustomScripting        0x00002000
#endif


//
// This flag when set in the RASENTRYNAME structure
// indicates that the phonebook to which this entry
// belongs is a system phonebook.
//
#define REN_User                         0x00000000
#define REN_AllUsers                     0x00000001


// Describes an enumerated RAS phone book entry name.  (See RasEntryEnum)

#define RASENTRYNAMEW struct tagRASENTRYNAMEW
RASENTRYNAMEW
{
    DWORD dwSize;
    WCHAR szEntryName[ RAS_MaxEntryName + 1 ];

#if (WINVER >= 0x500)
    //
    // If this flag is REN_AllUsers then its a
    // system phonebook.
    //
    DWORD dwFlags;
    WCHAR szPhonebookPath[MAX_PATH + 1];
#endif
};

#define RASENTRYNAMEA struct tagRASENTRYNAMEA
RASENTRYNAMEA
{
    DWORD dwSize;
    CHAR  szEntryName[ RAS_MaxEntryName + 1 ];

#if (WINVER >= 0x500)
	 DWORD dwFlags;
    CHAR  szPhonebookPath[MAX_PATH + 1];
#endif

};

#ifdef UNICODE
#define RASENTRYNAME RASENTRYNAMEW
#else
#define RASENTRYNAME RASENTRYNAMEA
#endif

#define LPRASENTRYNAMEW RASENTRYNAMEW*
#define LPRASENTRYNAMEA RASENTRYNAMEA*
#define LPRASENTRYNAME  RASENTRYNAME*


// Protocol code to projection data structure mapping.

#define RASPROJECTION enum tagRASPROJECTION
RASPROJECTION
{
    RASP_Amb = 0x10000,
	 RASP_PppNbf = 0x803F,
    RASP_PppIpx = 0x802B,
    RASP_PppIp = 0x8021,
#if (WINVER >= 0x500)
    RASP_PppCcp = 0x80FD,
#endif
    RASP_PppLcp = 0xC021,
    RASP_Slip = 0x20000
};

#define LPRASPROJECTION RASPROJECTION*


// Describes the result of a RAS AMB (Authentication Message Block)
// projection.  This protocol is used with NT 3.1 and OS/2 1.3 downlevel
// RAS servers.

#define RASAMBW struct tagRASAMBW
RASAMBW
{
	 DWORD dwSize;
	 DWORD dwError;
	 WCHAR szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
	 BYTE  bLana;
};

#define RASAMBA struct tagRASAMBA
RASAMBA
{
    DWORD dwSize;
	 DWORD dwError;
    CHAR  szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
    BYTE  bLana;
};

#ifdef UNICODE
#define RASAMB RASAMBW
#else
#define RASAMB RASAMBA
#endif

#define LPRASAMBW RASAMBW*
#define LPRASAMBA RASAMBA*
#define LPRASAMB  RASAMB*


// Describes the result of a PPP NBF (NetBEUI) projection.

#define RASPPPNBFW struct tagRASPPPNBFW
RASPPPNBFW
{
    DWORD dwSize;
    DWORD dwError;
    DWORD dwNetBiosError;
    WCHAR szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
    WCHAR szWorkstationName[ NETBIOS_NAME_LEN + 1 ];
    BYTE  bLana;
};

#define RASPPPNBFA struct tagRASPPPNBFA
RASPPPNBFA
{
    DWORD dwSize;
    DWORD dwError;
	 DWORD dwNetBiosError;
    CHAR  szNetBiosError[ NETBIOS_NAME_LEN + 1 ];
    CHAR  szWorkstationName[ NETBIOS_NAME_LEN + 1 ];
    BYTE  bLana;
};

#ifdef UNICODE
#define RASPPPNBF RASPPPNBFW
#else
#define RASPPPNBF RASPPPNBFA
#endif

#define LPRASPPPNBFW RASPPPNBFW*
#define LPRASPPPNBFA RASPPPNBFA*
#define LPRASPPPNBF  RASPPPNBF*


// Describes the results of a PPP IPX (Internetwork Packet Exchange)
// projection.

#define RASPPPIPXW struct tagRASIPXW
RASPPPIPXW
{
    DWORD dwSize;
    DWORD dwError;
    WCHAR szIpxAddress[ RAS_MaxIpxAddress + 1 ];
};


#define RASPPPIPXA struct tagRASPPPIPXA
RASPPPIPXA
{
    DWORD dwSize;
    DWORD dwError;
    CHAR  szIpxAddress[ RAS_MaxIpxAddress + 1 ];
};

#ifdef UNICODE
#define RASPPPIPX RASPPPIPXW
#else
#define RASPPPIPX RASPPPIPXA
#endif

#define LPRASPPPIPXW RASPPPIPXW*
#define LPRASPPPIPXA RASPPPIPXA*
#define LPRASPPPIPX  RASPPPIPX*


// Describes the results of a PPP IP (Internet) projection.


#if (WINVER >= 0x500)

// RASPPPIP 'dwOptions' and 'dwServerOptions' flags.

#define RASIPO_VJ       0x00000001

#endif

#define RASPPPIPW struct tagRASPPPIPW
RASPPPIPW
{
    DWORD dwSize;
    DWORD dwError;
	 WCHAR szIpAddress[ RAS_MaxIpAddress + 1 ];

#ifndef WINNT35COMPATIBLE

	 // This field was added between Windows NT 3.51 beta and Windows NT 3.51
	 // final, and between Windows 95 M8 beta and Windows 95 final.  If you do
	 // not require the server address and wish to retrieve PPP IP information
	 // from Windows NT 3.5 or early Windows NT 3.51 betas, or on early Windows
	 // 95 betas, define WINNT35COMPATIBLE.
	 //
	 // The server IP address is not provided by all PPP implementations,
	 // though Windows NT server's do provide it.

	 WCHAR szServerIpAddress[ RAS_MaxIpAddress + 1 ];

#endif
#if (WINVER >= 0x500)
	 DWORD dwOptions;
	 DWORD dwServerOptions;
#endif
};

#define RASPPPIPA struct tagRASPPPIPA
RASPPPIPA
{
    DWORD dwSize;
	 DWORD dwError;
    CHAR  szIpAddress[ RAS_MaxIpAddress + 1 ];

#ifndef WINNT35COMPATIBLE

	 // See RASPPPIPW comment.

	 CHAR  szServerIpAddress[ RAS_MaxIpAddress + 1 ];

#endif
#if (WINVER >= 0x500)
	 DWORD dwOptions;
	 DWORD dwServerOptions;
#endif
};

#ifdef UNICODE
#define RASPPPIP RASPPPIPW
#else
#define RASPPPIP RASPPPIPA
#endif

#define LPRASPPPIPW RASPPPIPW*
#define LPRASPPPIPA RASPPPIPA*
#define LPRASPPPIP  RASPPPIP*


// Describes the results of a PPP LCP/multi-link negotiation.


#if (WINVER >= 0x500)

// RASPPPLCP 'dwAuthenticatonProtocol' values.

#define RASLCPAP_PAP          0xC023
#define RASLCPAP_SPAP         0xC027
#define RASLCPAP_CHAP         0xC223
#define RASLCPAP_EAP          0xC227

// RASPPPLCP 'dwAuthenticatonData' values.

#define RASLCPAD_CHAP_MD5     0x05
#define RASLCPAD_CHAP_MS      0x80
#define RASLCPAD_CHAP_MSV2    0x81

// RASPPPLCP 'dwOptions' and 'dwServerOptions' flags.

#define RASLCPO_PFC           0x00000001
#define RASLCPO_ACFC          0x00000002
#define RASLCPO_SSHF          0x00000004
#define RASLCPO_DES_56        0x00000008
#define RASLCPO_3_DES         0x00000010

#endif


#define RASPPPLCPW struct tagRASPPPLCPW
RASPPPLCPW
{
	 DWORD dwSize;
	 BOOL  fBundled;

#if (WINVER >= 0x500)
	 DWORD dwError;
    DWORD dwAuthenticationProtocol;
    DWORD dwAuthenticationData;
    DWORD dwEapTypeId;
    DWORD dwServerAuthenticationProtocol;
    DWORD dwServerAuthenticationData;
	 DWORD dwServerEapTypeId;
    BOOL  fMultilink;
    DWORD dwTerminateReason;
    DWORD dwServerTerminateReason;
    WCHAR szReplyMessage[RAS_MaxReplyMessage];
    DWORD dwOptions;
    DWORD dwServerOptions;
#endif

};

#define RASPPPLCPA struct tagRASPPPLCPA
RASPPPLCPA
{
    DWORD dwSize;
    BOOL  fBundled;

#if (WINVER >= 0x500)
    DWORD dwError;
    DWORD dwAuthenticationProtocol;
    DWORD dwAuthenticationData;
	 DWORD dwEapTypeId;
    DWORD dwServerAuthenticationProtocol;
    DWORD dwServerAuthenticationData;
    DWORD dwServerEapTypeId;
    BOOL  fMultilink;
    DWORD dwTerminateReason;
    DWORD dwServerTerminateReason;
    CHAR  szReplyMessage[RAS_MaxReplyMessage];
    DWORD dwOptions;
    DWORD dwServerOptions;
#endif

};


#ifdef UNICODE
#define RASPPPLCP RASPPPLCPW
#else
#define RASPPPLCP RASPPPLCPA
#endif

#define LPRASPPPLCPW RASPPPLCPW*
#define LPRASPPPLCPA RASPPPLCPA*
#define LPRASPPPLCP  RASPPPLCP*


// Describes the results of a SLIP (Serial Line IP) projection.

#define RASSLIPW struct tagRASSLIPW
RASSLIPW
{
    DWORD dwSize;
    DWORD dwError;
    WCHAR szIpAddress[ RAS_MaxIpAddress + 1 ];
};


#define RASSLIPA struct tagRASSLIPA
RASSLIPA
{
    DWORD dwSize;
    DWORD dwError;
    CHAR  szIpAddress[ RAS_MaxIpAddress + 1 ];
};

#ifdef UNICODE
#define RASSLIP RASSLIPW
#else
#define RASSLIP RASSLIPA
#endif

#define LPRASSLIPW RASSLIPW*
#define LPRASSLIPA RASSLIPA*
#define LPRASSLIP  RASSLIP*

#if (WINVER >= 0x500)

// Describes the results of a PPP CCP (Compression Control Protocol)
// projection.


// RASPPPCCP 'dwCompressionAlgorithm' values.

#define RASCCPCA_MPPC         0x00000006
#define RASCCPCA_STAC         0x00000005

// RASPPPCCP 'dwOptions' values.

#define RASCCPO_Compression       0x00000001
#define RASCCPO_HistoryLess       0x00000002
#define RASCCPO_Encryption56bit   0x00000010
#define RASCCPO_Encryption40bit   0x00000020
#define RASCCPO_Encryption128bit  0x00000040

#define RASPPPCCP struct tagRASPPPCCP
RASPPPCCP
{
	 DWORD dwSize;
	 DWORD dwError;
	 DWORD dwCompressionAlgorithm;
	 DWORD dwOptions;
	 DWORD dwServerCompressionAlgorithm;
	 DWORD dwServerOptions;
};

#define LPRASPPPCCP  RASPPPCCP*

#endif

// If using RasDial message notifications, get the notification message code
// by passing this string to the RegisterWindowMessageA() API.
// WM_RASDIALEVENT is used only if a unique message cannot be registered.

#define RASDIALEVENT    "RasDialEvent"
#define WM_RASDIALEVENT 0xCCCD

// Prototypes for caller's RasDial callback handler.  Arguments are the
// message ID (currently always WM_RASDIALEVENT), the current RASCONNSTATE and
// the error that has occurred (or 0 if none).  Extended arguments are the
// handle of the RAS connection and an extended error code.
//
// For RASDIALFUNC2, subsequent callback notifications for all
// subentries can be cancelled by returning FALSE.

typedef VOID (WINAPI *RASDIALFUNC)( UINT, RASCONNSTATE, DWORD );
typedef VOID (WINAPI *RASDIALFUNC1)( HRASCONN, UINT, RASCONNSTATE, DWORD, DWORD );
typedef DWORD (WINAPI *RASDIALFUNC2)( ULONG_PTR, DWORD, HRASCONN, UINT, RASCONNSTATE, DWORD, DWORD );


// Information describing a RAS-capable device.

#define RASDEVINFOW struct tagRASDEVINFOW
RASDEVINFOW
{
	 DWORD    dwSize;
	 WCHAR    szDeviceType[ RAS_MaxDeviceType + 1 ];
	 WCHAR    szDeviceName[ RAS_MaxDeviceName + 1 ];
};

#define RASDEVINFOA struct tagRASDEVINFOA
RASDEVINFOA
{
	 DWORD    dwSize;
    CHAR     szDeviceType[ RAS_MaxDeviceType + 1 ];
    CHAR     szDeviceName[ RAS_MaxDeviceName + 1 ];
};

#ifdef UNICODE
#define RASDEVINFO RASDEVINFOW
#else
#define RASDEVINFO RASDEVINFOA
#endif

#define LPRASDEVINFOW RASDEVINFOW*
#define LPRASDEVINFOA RASDEVINFOA*
#define LPRASDEVINFO  RASDEVINFO*

// RAS country information (currently retrieved from TAPI).

#define RASCTRYINFO struct RASCTRYINFO
RASCTRYINFO
{
	 DWORD   dwSize;
	 DWORD   dwCountryID;
	 DWORD   dwNextCountryID;
	 DWORD   dwCountryCode;
	 DWORD   dwCountryNameOffset;
};

// There is currently no difference between
// RASCTRYINFOA and RASCTRYINFOW.  This may
// change in the future.

#define RASCTRYINFOW   RASCTRYINFO
#define RASCTRYINFOA   RASCTRYINFO

#define LPRASCTRYINFOW RASCTRYINFOW*
#define LPRASCTRYINFOA RASCTRYINFOW*
#define LPRASCTRYINFO  RASCTRYINFO*

// A RAS IP address.

#define RASIPADDR struct RASIPADDR
RASIPADDR
{
	 BYTE a;
	 BYTE b;
	 BYTE c;
	 BYTE d;
};

#if (WINVER >= 0x500)

#define ET_None         0  // No encryption
#define ET_Require      1  // Require Encryption
#define ET_RequireMax   2  // Require max encryption
#define ET_Optional     3  // Do encryption if possible. None Ok.

#endif

#define VS_Default		0   // default (PPTP for now)
#define VS_PptpOnly	    1	// Only PPTP is attempted.
#define VS_PptpFirst	2   // PPTP is tried first.
#define VS_L2tpOnly 	3	// Only L2TP is attempted.
#define VS_L2tpFirst	4	// L2TP is tried first.

// A RAS phone book entry.

#define RASENTRYA struct tagRASENTRYA
RASENTRYA
{
	 DWORD       dwSize;
    DWORD       dwfOptions;
    //
    // Location/phone number.
    //
    DWORD       dwCountryID;
    DWORD       dwCountryCode;
    CHAR        szAreaCode[ RAS_MaxAreaCode + 1 ];
    CHAR        szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    DWORD       dwAlternateOffset;
    //
    // PPP/Ip
    //
    RASIPADDR   ipaddr;
    RASIPADDR   ipaddrDns;
    RASIPADDR   ipaddrDnsAlt;
    RASIPADDR   ipaddrWins;
    RASIPADDR   ipaddrWinsAlt;
    //
    // Framing
    //
	 DWORD       dwFrameSize;
    DWORD       dwfNetProtocols;
    DWORD       dwFramingProtocol;
    //
    // Scripting
	 //
    CHAR        szScript[ MAX_PATH ];
    //
    // AutoDial
    //
    CHAR        szAutodialDll[ MAX_PATH ];
    CHAR        szAutodialFunc[ MAX_PATH ];
    //
	 // Device
    //
    CHAR        szDeviceType[ RAS_MaxDeviceType + 1 ];
    CHAR        szDeviceName[ RAS_MaxDeviceName + 1 ];
    //
    // X.25
    //
    CHAR        szX25PadType[ RAS_MaxPadType + 1 ];
    CHAR        szX25Address[ RAS_MaxX25Address + 1 ];
    CHAR        szX25Facilities[ RAS_MaxFacilities + 1 ];
    CHAR        szX25UserData[ RAS_MaxUserData + 1 ];
    DWORD       dwChannels;
    //
    // Reserved
    //
    DWORD       dwReserved1;
    DWORD       dwReserved2;
#if (WINVER >= 0x401)
    //
    // Multilink
    //
    DWORD       dwSubEntries;
	 DWORD       dwDialMode;
    DWORD       dwDialExtraPercent;
    DWORD       dwDialExtraSampleSeconds;
    DWORD       dwHangUpExtraPercent;
    DWORD       dwHangUpExtraSampleSeconds;
    //
    // Idle timeout
    //
    DWORD       dwIdleDisconnectSeconds;
#endif

#if (WINVER >= 0x500)

    //
    // Entry Type
    //
    DWORD       dwType;

    //
	 // Encryption type
    //
    DWORD       dwEncryptionType;

    //
    // CustomAuthKey to be used for EAP
	 //
    DWORD       dwCustomAuthKey;

    //
    // Guid of the connection
	 //
    GUID        guidId;

    //
    // Custom Dial Dll
    //
    CHAR        szCustomDialDll[MAX_PATH];

    //
    // DwVpnStrategy
    //
    DWORD       dwVpnStrategy;
#endif

#if (WINVER >= 0x501)
	//
	// More RASEO_* options
	//
	DWORD		dwfOptions2;

	//
	// For future use
	//
	DWORD       dwfOptions3;

	CHAR		szDnsSuffix[RAS_MaxDnsSuffix];

	DWORD       dwTcpWindowSize;

	CHAR        szPrerequisitePbk[MAX_PATH];

	CHAR        szPrerequisiteEntry[RAS_MaxEntryName + 1];

	DWORD       dwRedialCount;

	DWORD       dwRedialPause;
#endif

};

#define RASENTRYW struct tagRASENTRYW
RASENTRYW
{
    DWORD       dwSize;
    DWORD       dwfOptions;
    //
    // Location/phone number
    //
    DWORD       dwCountryID;
    DWORD       dwCountryCode;
    WCHAR       szAreaCode[ RAS_MaxAreaCode + 1 ];
    WCHAR       szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    DWORD       dwAlternateOffset;
    //
    // PPP/Ip
	 //
    RASIPADDR   ipaddr;
    RASIPADDR   ipaddrDns;
    RASIPADDR   ipaddrDnsAlt;
    RASIPADDR   ipaddrWins;
	 RASIPADDR   ipaddrWinsAlt;
    //
    // Framing
    //
    DWORD       dwFrameSize;
    DWORD       dwfNetProtocols;
    DWORD       dwFramingProtocol;
    //
    // Scripting
    //
    WCHAR       szScript[ MAX_PATH ];
	 //
    // AutoDial
    //
    WCHAR       szAutodialDll[ MAX_PATH ];
    WCHAR       szAutodialFunc[ MAX_PATH ];
    //
    // Device
    //
    WCHAR       szDeviceType[ RAS_MaxDeviceType + 1 ];
    WCHAR       szDeviceName[ RAS_MaxDeviceName + 1 ];
    //
    // X.25
    //
    WCHAR       szX25PadType[ RAS_MaxPadType + 1 ];
	 WCHAR       szX25Address[ RAS_MaxX25Address + 1 ];
    WCHAR       szX25Facilities[ RAS_MaxFacilities + 1 ];
    WCHAR       szX25UserData[ RAS_MaxUserData + 1 ];
    DWORD       dwChannels;
    //
	 // Reserved
    //
    DWORD       dwReserved1;
    DWORD       dwReserved2;
#if (WINVER >= 0x401)
    //
    // Multilink
    //
    DWORD       dwSubEntries;
    DWORD       dwDialMode;
    DWORD       dwDialExtraPercent;
    DWORD       dwDialExtraSampleSeconds;
    DWORD       dwHangUpExtraPercent;
    DWORD       dwHangUpExtraSampleSeconds;
    //
    // Idle timeout
    //
    DWORD       dwIdleDisconnectSeconds;
#endif

#if (WINVER >= 0x500)
    //
	 // Entry Type
    //
    DWORD       dwType;

    //
    // EncryptionType
    //
    DWORD       dwEncryptionType;

    //
    // CustomAuthKey to be used for EAP
    //
    DWORD       dwCustomAuthKey;

    //
    // Guid of the connection
    //
    GUID        guidId;

    //
    // Custom Dial Dll
    //
    WCHAR       szCustomDialDll[MAX_PATH];

    //
    // Vpn Strategy
    //
    DWORD       dwVpnStrategy;
#endif

#if (WINVER >= 0x501)
	//
	// More RASEO_* options
	//
	DWORD		dwfOptions2;

	//
	// For future use
	//
	DWORD       dwfOptions3;

	WCHAR		szDnsSuffix[RAS_MaxDnsSuffix];

	DWORD       dwTcpWindowSize;

	WCHAR       szPrerequisitePbk[MAX_PATH];

	WCHAR       szPrerequisiteEntry[RAS_MaxEntryName + 1];

	DWORD       dwRedialCount;

	DWORD       dwRedialPause;
#endif
};

#ifdef UNICODE
#define RASENTRY RASENTRYW
#else
#define RASENTRY RASENTRYA
#endif

#define LPRASENTRYW RASENTRYW*
#define LPRASENTRYA RASENTRYA*
#define LPRASENTRY  RASENTRY*

// RASENTRY 'dwfOptions' bit flags.

#define RASEO_UseCountryAndAreaCodes    0x00000001
#define RASEO_SpecificIpAddr            0x00000002
#define RASEO_SpecificNameServers       0x00000004
#define RASEO_IpHeaderCompression       0x00000008
#define RASEO_RemoteDefaultGateway      0x00000010
#define RASEO_DisableLcpExtensions      0x00000020
#define RASEO_TerminalBeforeDial        0x00000040
#define RASEO_TerminalAfterDial         0x00000080
#define RASEO_ModemLights               0x00000100
#define RASEO_SwCompression             0x00000200
#define RASEO_RequireEncryptedPw        0x00000400
#define RASEO_RequireMsEncryptedPw      0x00000800
#define RASEO_RequireDataEncryption     0x00001000
#define RASEO_NetworkLogon              0x00002000
#define RASEO_UseLogonCredentials       0x00004000
#define RASEO_PromoteAlternates         0x00008000

#if (WINVER >= 0x401)
#define RASEO_SecureLocalFiles          0x00010000
#endif

#if (WINVER >= 0x500)
#define RASEO_RequireEAP                0x00020000
#define RASEO_RequirePAP                0x00040000
#define RASEO_RequireSPAP               0x00080000
#define RASEO_Custom                    0x00100000

#define RASEO_PreviewPhoneNumber        0x00200000
#define RASEO_SharedPhoneNumbers        0x00800000
#define RASEO_PreviewUserPw             0x01000000
#define RASEO_PreviewDomain             0x02000000
#define RASEO_ShowDialingProgress       0x04000000
#define RASEO_RequireCHAP               0x08000000
#define RASEO_RequireMsCHAP             0x10000000
#define RASEO_RequireMsCHAP2            0x20000000
#define RASEO_RequireW95MSCHAP          0x40000000
#define RASEO_CustomScript              0x80000000
#endif

#if (WINVER >= 0x501)
//
// RASENTRY dwfOptions2 bit flags
//
#define RASEO2_SecureFileAndPrint       0x00000001
#define RASEO2_SecureClientForMSNet     0x00000002
#define RASEO2_DontNegotiateMultilink   0x00000004
#define RASEO2_DontUseRasCredentials    0x00000008
#define RASEO2_UsePreSharedKey          0x00000010
#define RASEO2_Internet                 0x00000020
#define RASEO2_DisableNbtOverIP         0x00000040
#define RASEO2_UseGlobalDeviceSettings  0x00000080
#define RASEO2_ReconnectIfDropped       0x00000100
#define RASEO2_SharePhoneNumbers        0x00000200
#endif

// RASENTRY 'dwProtocols' bit flags.

#define RASNP_NetBEUI                   0x00000001
#define RASNP_Ipx                       0x00000002
#define RASNP_Ip                        0x00000004

// RASENTRY 'dwFramingProtocols' bit flags.

#define RASFP_Ppp                       0x00000001
#define RASFP_Slip                      0x00000002
#define RASFP_Ras                       0x00000004

// RASENTRY 'szDeviceType' default strings.

#define RASDT_Modem                     TEXT("modem")
#define RASDT_Isdn                      TEXT("isdn")
#define RASDT_X25                       TEXT("x25")
#define RASDT_Vpn                       TEXT("vpn")
#define RASDT_Pad                       TEXT("pad")
#define RASDT_Generic                   TEXT("GENERIC")
#define RASDT_Serial        			TEXT("SERIAL")
#define RASDT_FrameRelay                TEXT("FRAMERELAY")
#define RASDT_Atm                       TEXT("ATM")
#define RASDT_Sonet                     TEXT("SONET")
#define RASDT_SW56                      TEXT("SW56")
#define RASDT_Irda                      TEXT("IRDA")
#define RASDT_Parallel                  TEXT("PARALLEL")
#if (WINVER >= 0x501)
#define RASDT_PPPoE                     TEXT("PPPoE")
#endif


// The entry type used to determine which UI properties
// are to be presented to user.  This generally corresponds
// to a Connections "add" wizard selection.
//
#define RASET_Phone     1  // Phone lines: modem, ISDN, X.25, etc
#define RASET_Vpn       2  // Virtual private network
#define RASET_Direct    3  // Direct connect: serial, parallel
#define RASET_Internet  4  // BaseCamp internet
#if (WINVER >= 0x501)
#define RASET_Broadband 5  // Broadband
#endif

// Old AutoDial DLL function prototype.
//
// This prototype is documented for backward-compatibility
// purposes only.  It is superceded by the RASADFUNCA
// and RASADFUNCW definitions below.  DO NOT USE THIS
// PROTOTYPE IN NEW CODE.  SUPPORT FOR IT MAY BE REMOVED
// IN FUTURE VERSIONS OF RAS.

typedef BOOL (WINAPI *ORASADFUNC)( HWND, LPSTR, DWORD, LPDWORD );

#if (WINVER >= 0x400)

// Flags for RasConnectionNotification().

#define RASCN_Connection        0x00000001
#define RASCN_Disconnection     0x00000002
#define RASCN_BandwidthAdded    0x00000004
#define RASCN_BandwidthRemoved  0x00000008

#endif

#if (WINVER >= 0x401)
// RASENTRY 'dwDialMode' values.

#define RASEDM_DialAll                  1
#define RASEDM_DialAsNeeded             2

// RASENTRY 'dwIdleDisconnectSeconds' constants.

#define RASIDS_Disabled                 0xffffffff
#define RASIDS_UseGlobalValue           0

// AutoDial DLL function parameter block.
//
#define RASADPARAMS struct tagRASADPARAMS
RASADPARAMS
{
	 DWORD       dwSize;
	 HWND        hwndOwner;
	 DWORD       dwFlags;
	 LONG        xDlg;
	 LONG        yDlg;
};

#define LPRASADPARAMS RASADPARAMS*

// AutoDial DLL function parameter block 'dwFlags.'
//
#define RASADFLG_PositionDlg            0x00000001

// Prototype AutoDial DLL function.

typedef BOOL (WINAPI *RASADFUNCA)( LPSTR, LPSTR, LPRASADPARAMS, LPDWORD );
typedef BOOL (WINAPI *RASADFUNCW)( LPWSTR, LPWSTR, LPRASADPARAMS, LPDWORD );

#ifdef UNICODE
#define RASADFUNC RASADFUNCW
#else
#define RASADFUNC RASADFUNCA
#endif

// A RAS phone book multilinked sub-entry.

#define RASSUBENTRYA struct tagRASSUBENTRYA
RASSUBENTRYA
{
    DWORD       dwSize;
    DWORD       dwfFlags;
    //
    // Device
    //
    CHAR        szDeviceType[ RAS_MaxDeviceType + 1 ];
    CHAR        szDeviceName[ RAS_MaxDeviceName + 1 ];
    //
    // Phone numbers
    //
    CHAR        szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    DWORD       dwAlternateOffset;
};

#define RASSUBENTRYW struct tagRASSUBENTRYW
RASSUBENTRYW
{
    DWORD       dwSize;
    DWORD       dwfFlags;
    //
    // Device
    //
    WCHAR       szDeviceType[ RAS_MaxDeviceType + 1 ];
    WCHAR       szDeviceName[ RAS_MaxDeviceName + 1 ];
    //
    // Phone numbers
	 //
    WCHAR       szLocalPhoneNumber[ RAS_MaxPhoneNumber + 1 ];
    DWORD       dwAlternateOffset;
};

#ifdef UNICODE
#define RASSUBENTRY RASSUBENTRYW
#else
#define RASSUBENTRY RASSUBENTRYA
#endif

#define LPRASSUBENTRYW RASSUBENTRYW*
#define LPRASSUBENTRYA RASSUBENTRYA*
#define LPRASSUBENTRY  RASSUBENTRY*

// Ras{Get,Set}Credentials structure.  These calls
// supercede Ras{Get,Set}EntryDialParams.

#define RASCREDENTIALSA struct tagRASCREDENTIALSA
RASCREDENTIALSA
{
    DWORD dwSize;
    DWORD dwMask;
    CHAR szUserName[ UNLEN + 1 ];
    CHAR szPassword[ PWLEN + 1 ];
    CHAR szDomain[ DNLEN + 1 ];
};

#define RASCREDENTIALSW struct tagRASCREDENTIALSW
RASCREDENTIALSW
{
    DWORD dwSize;
    DWORD dwMask;
    WCHAR szUserName[ UNLEN + 1 ];
    WCHAR szPassword[ PWLEN + 1 ];
    WCHAR szDomain[ DNLEN + 1 ];
};

#ifdef UNICODE
#define RASCREDENTIALS RASCREDENTIALSW
#else
#define RASCREDENTIALS RASCREDENTIALSA
#endif

#define LPRASCREDENTIALSW RASCREDENTIALSW*
#define LPRASCREDENTIALSA RASCREDENTIALSA*
#define LPRASCREDENTIALS  RASCREDENTIALS*

// RASCREDENTIALS 'dwMask' values.

#define RASCM_UserName          0x00000001
#define RASCM_Password          0x00000002
#define RASCM_Domain            0x00000004
#if (WINVER >= 501)
#define RASCM_DefaultCreds       0x00000008
#define RASCM_PreSharedKey       0x00000010
#define RASCM_ServerPreSharedKey 0x00000020
#define RASCM_DDMPreSharedKey    0x00000040
#endif

// AutoDial address properties.

#define RASAUTODIALENTRYA struct tagRASAUTODIALENTRYA
RASAUTODIALENTRYA
{
	 DWORD dwSize;
	 DWORD dwFlags;
	 DWORD dwDialingLocation;
	 CHAR szEntry[ RAS_MaxEntryName + 1];
};

#define RASAUTODIALENTRYW struct tagRASAUTODIALENTRYW
RASAUTODIALENTRYW
{
	 DWORD dwSize;
	 DWORD dwFlags;
	 DWORD dwDialingLocation;
	 WCHAR szEntry[ RAS_MaxEntryName + 1];
};

#ifdef UNICODE
#define RASAUTODIALENTRY RASAUTODIALENTRYW
#else
#define RASAUTODIALENTRY RASAUTODIALENTRYA
#endif

#define LPRASAUTODIALENTRYW RASAUTODIALENTRYW*
#define LPRASAUTODIALENTRYA RASAUTODIALENTRYA*
#define LPRASAUTODIALENTRY  RASAUTODIALENTRY*

// AutoDial control parameter values for
// Ras{Get,Set}AutodialParam.

#define RASADP_DisableConnectionQuery           0
#define RASADP_LoginSessionDisable              1
#define RASADP_SavedAddressesLimit              2
#define RASADP_FailedConnectionTimeout          3
#define RASADP_ConnectionQueryTimeout           4

#endif // (WINVER >= 0x401)

#if (WINVER >= 0x500)

// RasGetEapUserIdentity bit flags.
// These have the same values as the RAS_EAP_FLAG_ flags in raseapif.h

#define RASEAPF_NonInteractive          0x00000002
#define RASEAPF_Logon                   0x00000004
#define RASEAPF_Preview                 0x00000008

// RasGetEapUserIdentity structure.

#define RASEAPUSERIDENTITYA struct tagRASEAPUSERIDENTITYA
RASEAPUSERIDENTITYA
{
    CHAR        szUserName[ UNLEN + 1 ];
    DWORD       dwSizeofEapInfo;
    BYTE        pbEapInfo[ 1 ];
};

#define RASEAPUSERIDENTITYW struct tagRASEAPUSERIDENTITYW
RASEAPUSERIDENTITYW
{
    WCHAR       szUserName[ UNLEN + 1 ];
    DWORD       dwSizeofEapInfo;
    BYTE        pbEapInfo[ 1 ];
};

#ifdef UNICODE
#define RASEAPUSERIDENTITY RASEAPUSERIDENTITYW
#else
#define RASEAPUSERIDENTITY RASEAPUSERIDENTITYA
#endif

#define LPRASEAPUSERIDENTITYW RASEAPUSERIDENTITYW*
#define LPRASEAPUSERIDENTITYA RASEAPUSERIDENTITYA*

#endif // (WINVER >= 0x500)

#if (WINVER >= 0x0501)

#define RASCOMMSETTINGS struct tagRASCOMMSETTINGS
RASCOMMSETTINGS
{
    DWORD     dwSize;
    BYTE      bParity;
    BYTE      bStop;
    BYTE      bByteSize;
    BYTE      bAlign;
};

typedef DWORD (WINAPI *PFNRASSETCOMMSETTINGS) (
									 HANDLE hPort,
                            RASCOMMSETTINGS *pRasCommSettings,
                            PVOID  pvReserved
                            );

#define RASCUSTOMSCRIPTEXTENSIONS struct tagRASCUSTOMSCRIPTEXTENSIONS
RASCUSTOMSCRIPTEXTENSIONS
{
    DWORD                    dwSize;                  
    PFNRASSETCOMMSETTINGS    pfnRasSetCommSettings;
};

#endif

typedef DWORD (__stdcall *TRasGetEntryDialParams)(LPCSTR, LPRASDIALPARAMSA, LPBOOL);
typedef DWORD (__stdcall *TRasSetEntryDialParams)(LPCSTR, LPRASDIALPARAMSA, BOOL);
typedef DWORD (__stdcall *TRasDial)(LPRASDIALEXTENSIONS, LPCSTR, LPRASDIALPARAMSA, DWORD, LPVOID, LPHRASCONN);
typedef DWORD (__stdcall *TRasGetProjectionInfo)(HRASCONN, RASPROJECTION, LPVOID, LPDWORD);
typedef DWORD (__stdcall *TRasGetErrorString)(UINT, LPSTR, DWORD);
typedef DWORD (__stdcall *TRasHangUp)(HRASCONN);
typedef DWORD (__stdcall *TRasGetConnectStatus)(HRASCONN, LPRASCONNSTATUSA);
typedef DWORD (__stdcall *TRasEnumEntries)(LPCSTR, LPCSTR, LPRASENTRYNAMEA, LPDWORD, LPDWORD);
typedef DWORD (__stdcall *TRasEnumConnections)(LPRASCONNA, LPDWORD, LPDWORD);
typedef DWORD (__stdcall *TRasCreatePhonebookEntry)(HWND, LPCSTR);

#undef		WINVER
#define		WINVER OLDWINVER
#undef		OLDWINVER
//---------------------------------------------------------------------------
// ~Ras Stuff
//---------------------------------------------------------------------------
const MaxEntries = 255;
class TRASDial
{
private:
	HINSTANCE hLib;

	TRasGetEntryDialParams		RasGetEntryDialParams;
	TRasSetEntryDialParams		RasSetEntryDialParams;
	TRasDial							RasDial;
	TRasGetProjectionInfo		RasGetProjectionInfo;
	TRasGetErrorString			RasGetErrorString;
	TRasHangUp						RasHangUp;
	TRasGetConnectStatus			RasGetConnectStatus;
	TRasEnumEntries				RasEnumEntries;
	TRasEnumConnections			RasEnumConnections;
	TRasCreatePhonebookEntry	RasCreatePhonebookEntry;
public:
	TRASDial();
	virtual			~TRASDial();

	HRASCONN			Dial(AnsiString Entry, AnsiString UserName, AnsiString Password, AnsiString &ErrorMsg);
	bool				HangUp(HRASCONN Handle);
	bool				GetEntries(TStrings *Strings);
	bool				CreateEntry();
	HRASCONN			GetConnected(AnsiString Entry);
	bool				GetEntryDetails(AnsiString Entry, AnsiString &UserName, AnsiString &Password);

	AnsiString		ClientIP, ServerIP;
};
//---------------------------------------------------------------------------
class TDBDetails
{
private:
	TRASDial			RASDial;
	bool				GetConnected();
	HRASCONN			GetRASHandle();
	AnsiString		GetDBFilePath();
public:
	AnsiString		DBName;
	bool				CreateDB;	// Indicates that this is the first time it is run and can create a db if required
	__property		AnsiString DBFilePath = {read=GetDBFilePath};	// Extracts the path from the connection string.
	bool				RemoteConnection;
	AnsiString		RASEntry;
	AnsiString		RASUserName;
	AnsiString		RASPassword;
	__property		HRASCONN RASHandle = {read=GetRASHandle};

	AnsiString		ClientIP, ServerIP;

	__property		bool Connected = {read=GetConnected};
	bool				Dial(AnsiString &ErrorMsg);
	bool				HangUp();
	bool				GetEntryDetails(AnsiString &UserName, AnsiString &Password);
};
//---------------------------------------------------------------------------
class TConnectionDetails
{
private:
	TRASDial			RASDial;
public:
	AnsiString		CompanyName;
	AnsiString		ReportHeader;			// For Purchase Orders
	AnsiString		DeliveryAddress;		// For Purchase Orders
	AnsiString		OrderInstructions;	// For Purchase Orders
	TDBDetails		MenuMateDB, StockDB, ChefMateDB;
	AnsiString		ServerPath;		// Menu commit path. Don't include the "Menu Import" in the path.
	AnsiString		MenuSavePath;	// Save menus here. Could be either a central server or the office.
//	bool				IncludeGST;
	bool				HideStocktakeOnHand;

	double			DefaultMenuGST;
	double			DefaultStockGST;

	AnsiString		GSTNumber;

	bool				SingleLocation;

    bool DontShowItemCostInPurchaseOrder;

	AnsiString		DefaultLocation;

	AnsiString		PayrollSystem;
	AnsiString		PayrollExportPath;

	AnsiString		AccountSystem;
	AnsiString		MYOBPath;
	AnsiString		MYOBJobCode;
	AnsiString		SalesInvoiceExportAccount;
	AnsiString		SalesInvoiceExportType;

	AnsiString		StocktakePath;
	AnsiString		StocktakeExportFile;
	AnsiString		StocktakeImportFile;
	AnsiString		VIPCreditName; // cww
	int				StocktakeBarcodePos;
	int				StocktakeQtyPos;

	bool				UseSerialBarcodeReader;
	int				SerialBarcodeReaderPort;

	bool				RASCreateEntry();
	bool				RASGetEntries(TStrings *Strings);

	bool				LoadSettings();
    int				SettingDecimalPlaces;
   int              AutoPrintStockTransfer;


   bool AutoPrintStockTransferAudit;
   bool AutoPrintReceiveTransferAudit;
   bool SuppliersFromDefaultLocationsOnly;

 //  int NoOfPriceLevels;

};
//---------------------------------------------------------------------------
extern TConnectionDetails CurrentConnection;
//---------------------------------------------------------------------------
#endif
