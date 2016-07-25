//---------------------------------------------------------------------------
#pragma hdrstop

#include <Windows.h>
#include "TrayIcon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int TTrayIcon::uID = 1000;
//---------------------------------------------------------------------------
TTrayIcon::TTrayIcon(TForm *Owner, TImageList *ImageList, AnsiString Tip) :
		fClickDelay(new TTimer(NULL)),
		fOwner(Owner),
		fImageList(ImageList),
		fTip(Tip),
		fTrayIcon(new Graphics::TIcon),
		fIconIndex(0),
		OnClick(NULL),
		OnDblClick(NULL),
		OnAppCloseQuery(NULL),
		OnAppClose(NULL),
		OnMouseMove(NULL),
		OnMouseDown(NULL),
		OnMouseUp(NULL),
		OnInfoTimeout(NULL),
		OnInfoClick(NULL)
{
	fTaskbarCreatedMsg		= RegisterWindowMessage("TaskbarCreated");
	fID							= uID++;
	fClickDelay->Interval	= GetDoubleClickTime() + 50;
	fClickDelay->OnTimer		= fOnTimer;
	fCallbackHandle			= AllocateHWnd(WndProc);
	try
	{
		fAddIcon();
	}
	catch (...)
	{
		DeallocateHWnd(fCallbackHandle);
		throw;
	}
}
//---------------------------------------------------------------------------
TTrayIcon::~TTrayIcon()
{
	fRemoveIcon();
	DeallocateHWnd(fCallbackHandle);
}
//---------------------------------------------------------------------------
bool TTrayIcon::Visible()
{
	return !IsIconic(Application->Handle);
}
//---------------------------------------------------------------------------
void TTrayIcon::SetVisible(bool Visible)
{
	if (Visible)
	{
		Show();
	}
	else
	{
		Hide();
	}
}
//---------------------------------------------------------------------------
void TTrayIcon::Show()
{
	if (!Visible())
	{
		Application->Restore();
	}
	Application->BringToFront();
	fOwner->Visible = true;
}
//---------------------------------------------------------------------------
void TTrayIcon::Hide()
{
	EnableWindow(Application->Handle, true);
	Application->Minimize();
	ShowWindow(Application->Handle, SW_HIDE);
}
//---------------------------------------------------------------------------
void TTrayIcon::SetIconIndex(int Index)
{
	fIconIndex = Index;
	fModifyTrayIcon();
}
//---------------------------------------------------------------------------
void TTrayIcon::TrayIconInfo(AnsiString Message, AnsiString Title, TTrayInfoIcon Icon, int Timeout)
{
	fTrayIconInfo(Message, Title, Icon, Timeout);
}
//---------------------------------------------------------------------------
int TTrayIcon::ID()
{
	return fID;
}
//---------------------------------------------------------------------------
TShiftState TTrayIcon::ShiftState()
{
	TShiftState ShiftState;
	if (GetKeyState(VK_SHIFT) < 0)	ShiftState << ssShift;
	if (GetKeyState(VK_CONTROL) < 0)	ShiftState << ssCtrl;
	if (GetKeyState(VK_MENU) < 0)		ShiftState << ssAlt;

	return ShiftState;
}
//---------------------------------------------------------------------------
void __fastcall TTrayIcon::WndProc(TMessage &Message)
{
	if ((Message.Msg == fTaskbarCreatedMsg) & (fTaskbarCreatedMsg != 0))
	{
		// Oops, explorer crashed. Put ourselves back in the tray.
		fRemoveIcon();
		fAddIcon();
	}
	else if (Message.Msg == WM_QUERYENDSESSION)
	{
		bool CanClose = true;

		if (OnAppCloseQuery)
		{
			OnAppCloseQuery(fID, CanClose);
		}
		Message.Result = CanClose;
		return;
	}
	else if (Message.Msg == WM_ENDSESSION)
	{
		if ((bool)Message.WParam)
		{
			if (OnAppClose)
			{
				OnAppClose(fID);
			}
			fRemoveIcon();
//			Message.Result = true;
//			Application->Terminate();	// Maybe just sent to Application? Test.
		}
		return;
	}
	else if (Message.Msg == WM_TRAYNOTIFY)
	{
		switch(Message.LParam)
		{
			case WM_MOUSEMOVE:
			{
				TShiftState Shift = ShiftState();
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseMove(Shift, Pt.x, Pt.x);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				fClickDelay->Enabled = true;

				TShiftState Shift = ShiftState() << ssLeft;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseDown(mbLeft, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_LBUTTONUP:
			{
				TShiftState Shift = ShiftState() << ssLeft;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseUp(mbLeft, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_LBUTTONDBLCLK:
			{
				fClickDelay->Enabled = false;
				fDblClick();
				break;
			}
			case WM_RBUTTONDOWN:
			{
				fClickDelay->Enabled = true;

				TShiftState Shift = ShiftState() << ssRight;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseDown(mbRight, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_RBUTTONUP:
			{
				TShiftState Shift = ShiftState() << ssRight;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseUp(mbRight, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_RBUTTONDBLCLK:
			{
				fClickDelay->Enabled = false;
				fDblClick();
				break;
			}
			case WM_MBUTTONDOWN:
			{
				fClickDelay->Enabled = true;

				TShiftState Shift = ShiftState() << ssMiddle;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseDown(mbMiddle, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_MBUTTONUP:
			{
				TShiftState Shift = ShiftState() << ssMiddle;
				TPoint Pt;
				GetCursorPos(&Pt);
				fDoMouseUp(mbMiddle, Shift, Pt.x, Pt.y);
				break;
			}
			case WM_MBUTTONDBLCLK:
			{
				fClickDelay->Enabled = false;
				fDblClick();
				break;
			}
			case NIN_BALLOONUSERCLICK:
			{
				fInfoClick();
				break;
			}
			case NIN_BALLOONTIMEOUT:
			{
				fInfoTimeout();
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTrayIcon::fOnTimer(TObject *Sender)
{
	fClickDelay->Enabled = false;
	fClick();
}
//---------------------------------------------------------------------------
void TTrayIcon::fAddIcon()
{
	fImageList->GetIcon(fIconIndex, fTrayIcon.get());

	NOTIFYICONDATA IconData		= {0};

	IconData.cbSize				= sizeof(NOTIFYICONDATA);
	IconData.uID    				= fID;
	IconData.hWnd   				= fCallbackHandle;
	IconData.uFlags 				= NIF_MESSAGE|NIF_ICON|NIF_TIP;
	IconData.uCallbackMessage	= WM_TRAYNOTIFY;
	IconData.hIcon  				= fTrayIcon->Handle;
	lstrcpy(IconData.szTip, fTip.c_str());

	Shell_NotifyIcon(NIM_ADD,&IconData);
}
//-------------------------------------------------------
void TTrayIcon::fModifyTrayIcon()
{
	fImageList->GetIcon(fIconIndex, fTrayIcon.get());

	NOTIFYICONDATA IconData		= {0};

	IconData.cbSize				= sizeof(NOTIFYICONDATA);
	IconData.uID					= fID;
	IconData.hWnd					= fCallbackHandle;
	IconData.uFlags				= NIF_ICON;
	IconData.hIcon  				= fTrayIcon->Handle;

	Shell_NotifyIcon(NIM_MODIFY, &IconData);
}
//-------------------------------------------------------
void TTrayIcon::fRemoveIcon()
{
	NOTIFYICONDATA IconData		= {0};

	IconData.cbSize				= sizeof(NOTIFYICONDATA);
	IconData.uID    				= fID;
	IconData.hWnd   				= fCallbackHandle;

	Shell_NotifyIcon(NIM_DELETE, &IconData);
}
//-------------------------------------------------------
void TTrayIcon::fModifyTrayIconHint()
{
	NOTIFYICONDATA IconData		= {0};

	IconData.cbSize				= sizeof(NOTIFYICONDATA);
	IconData.uID					= fID;
	IconData.hWnd					= fCallbackHandle;
	IconData.uFlags				= NIF_TIP;
	lstrcpy(IconData.szTip, fTip.c_str());

	Shell_NotifyIcon(NIM_MODIFY, &IconData);
}
//-------------------------------------------------------
void TTrayIcon::fTrayIconInfo(AnsiString Message, AnsiString Title, TTrayInfoIcon Icon, int Timeout)
{
	V2NOTIFYICONDATA IconData	= {0};
	IconData.cbSize				= sizeof(V2NOTIFYICONDATA);
	IconData.uID					= fID;
	IconData.hWnd					= fCallbackHandle;
	IconData.uCallbackMessage	= WM_TRAYNOTIFY;
	IconData.uFlags				= NIF_INFO;
	IconData.uTimeout				= Timeout;

	switch (Icon)
	{
		case tiNone:		IconData.dwInfoFlags = NIIF_NONE;		break;
		case tiInfo:		IconData.dwInfoFlags = NIIF_INFO;		break;
		case tiWarning:	IconData.dwInfoFlags = NIIF_WARNING;	break;
		case tiError:		IconData.dwInfoFlags = NIIF_ERROR;		break;
	}
	strcpy(IconData.szInfo, Message.c_str());
	strcpy(IconData.szInfoTitle, Title.c_str());
	Shell_NotifyIcon(NIM_MODIFY, (_NOTIFYICONDATAA *)&IconData);
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fDoMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (IsIconic(Application->Handle))
	{
		SetForegroundWindow(fOwner->Handle);
	}
	else
	{
		Application->BringToFront();
	}
	if (OnMouseUp)
	{
		OnMouseUp(fID, Button, Shift, X, Y);
	}
	PostMessage(fOwner->Handle, WM_NULL, 0,0);
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fDoMouseDown(TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (IsIconic(Application->Handle))
	{
		SetForegroundWindow(fOwner->Handle);
	}
	else
	{
		Application->BringToFront();
	}
	if (OnMouseDown)
	{
		OnMouseDown(fID, Button, Shift, X, Y);
	}
	PostMessage(fOwner->Handle, WM_NULL, 0,0);
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fDoMouseMove(TShiftState Shift, int X, int Y)
{
	if (OnMouseMove)
	{
		OnMouseMove(fID, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fDblClick()
{
	if (OnDblClick)
	{
		OnDblClick(fID);
	}
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fClick()
{
	if (OnClick)
	{
		OnClick(fID);
	}
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fInfoTimeout()
{
	if (OnInfoTimeout)
	{
		OnInfoTimeout(fID);
	}
}
//---------------------------------------------------------------------------
inline void TTrayIcon::fInfoClick()
{
	if (IsIconic(Application->Handle))
	{
		SetForegroundWindow(fOwner->Handle);
	}
	else
	{
		Application->BringToFront();
	}
	if (OnInfoClick)
	{
		OnInfoClick(fID);
	}
}
//---------------------------------------------------------------------------
