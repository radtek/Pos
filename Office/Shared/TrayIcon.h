//---------------------------------------------------------------------------
#ifndef TrayIconH
#define TrayIconH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <memory>
//---------------------------------------------------------------------------
#define WM_TRAYNOTIFY  (WM_USER + 1001)
//---------------------------------------------------------------------------
enum TTrayInfoIcon {tiNone, tiError, tiWarning, tiInfo};
//---------------------------------------------------------------------------
struct V2NOTIFYICONDATA
{
	DWORD	cbSize;
	HWND	hWnd;
	UINT	uID;
	UINT	uFlags;
	UINT	uCallbackMessage;
	HICON	hIcon;
	TCHAR	szTip[128];
	DWORD	dwState;
	DWORD	dwStateMask;
	TCHAR	szInfo[256];
	union
	{
		UINT uTimeout;
		UINT uVersion;
	};
	TCHAR	szInfoTitle[64];
	DWORD	dwInfoFlags;
};
//---------------------------------------------------------------------------
// uFlags setting for tray ballon
const int NIF_INFO					= 0x10;
// Icons for try baloon
const int NIIF_NONE					= 0x00;
const int NIIF_INFO					= 0x01;
const int NIIF_WARNING				= 0x02;
const int NIIF_ERROR					= 0x03;
const int NIIF_NOSOUND				= 0x10;	// XP Only
// LParam in WM_TRAYNOTIFY set to owner window
const int NIN_BALLOONSHOW			= WM_USER + 2;
const int NIN_BALLOONHIDE			= WM_USER + 3;
const int NIN_BALLOONTIMEOUT		= WM_USER + 4;
const int NIN_BALLOONUSERCLICK	= WM_USER + 5;
const int NIN_SELECT					= WM_USER + 0;
const int NINF_KEY					= 0x01;
const int NIN_KEYSELECT				= NIN_SELECT | NINF_KEY;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TTrayNotifyEvent)(int uID);
typedef void __fastcall (__closure *TTrayCloseQueryEvent)(int uID, bool &CanClose);
typedef void __fastcall (__closure *TTrayMouseMoveEvent)(int uID, Classes::TShiftState Shift, int X, int Y);
typedef void __fastcall (__closure *TTrayMouseEvent)(int uID, TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
//---------------------------------------------------------------------------
class TTrayIcon
{
public:
	TTrayIcon(TForm *Owner, TImageList *ImageList, AnsiString Tip);
	~TTrayIcon();

	bool						Visible();
	void						SetVisible(bool Visible);
	void						Show();
	void						Hide();
	void						SetIconIndex(int Index);
	void						TrayIconInfo(AnsiString Message, AnsiString Title, TTrayInfoIcon Icon, int Timeout=10000);
	int						ID();

	TTrayNotifyEvent		OnClick;
	TTrayNotifyEvent		OnDblClick;

	TTrayCloseQueryEvent	OnAppCloseQuery;
	TTrayNotifyEvent		OnAppClose;

	TTrayMouseMoveEvent	OnMouseMove;
	TTrayMouseEvent		OnMouseDown;
	TTrayMouseEvent		OnMouseUp;

	TTrayNotifyEvent		OnInfoTimeout;
	TTrayNotifyEvent		OnInfoClick;
private:
	UINT						fTaskbarCreatedMsg;

	HWND						fCallbackHandle;
	TForm						*fOwner;
	void __fastcall		WndProc(TMessage &Msg);

	std::auto_ptr<TTimer>				fClickDelay;
	TImageList								*fImageList;
	std::auto_ptr<Graphics::TIcon>	fTrayIcon;
	int										fIconIndex;

	int						fID;
	static int				uID;

	AnsiString				fTip;

	void						fAddIcon();
	void						fRemoveIcon();
	void						fModifyTrayIconHint();
	void						fModifyTrayIcon();
	void						fTrayIconInfo(AnsiString Message, AnsiString Title, TTrayInfoIcon Icon, int Timeout);

	TShiftState				ShiftState();
	void						fDoMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
	void						fDoMouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
	void						fDoMouseMove(TShiftState Shift, int X, int Y);
	void						fDblClick();
	void						fClick();

	void __fastcall		fOnTimer(TObject *Sender);

	void						fInfoTimeout();
	void						fInfoClick();
};
//---------------------------------------------------------------------------
#endif
