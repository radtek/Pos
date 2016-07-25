//---------------------------------------------------------------------------
#pragma hdrstop

#include "MM_RemoteBrowse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace RemoteBrowser;
//---------------------------------------------------------------------------
TNodeSettings::TNodeSettings(const TNodeSettings &Rhs)// : Icon(NULL)
{
	operator = (Rhs);
}
//---------------------------------------------------------------------------
TNodeSettings::TNodeSettings(const TNodeSettings *Rhs)// : Icon(NULL)
{
	operator = (*Rhs);
}
//---------------------------------------------------------------------------
TNodeSettings & TNodeSettings::operator = (const TNodeSettings &Rhs)
{
	DisplayName			= Rhs.DisplayName;
	Path					= Rhs.Path;
	HasSubFolders		= Rhs.HasSubFolders;
	NodeType				= Rhs.NodeType;
	IconIndex			= Rhs.IconIndex;
	SelectedIconIndex	= Rhs.SelectedIconIndex;
	RemoteNode			= Rhs.RemoteNode;
	IconSize				= Rhs.IconSize;
/*	if (Rhs.Icon)
	{
		Icon = new TIcon;
		Icon->Assign(Rhs.Icon);
	}
	if (Rhs.SelectedIcon)
	{
		SelectedIcon = new TIcon;
		SelectedIcon->Assign(Rhs.SelectedIcon);
	}*/
	return *this;
}
//---------------------------------------------------------------------------
void TNodeSettings::operator >> (TSerialData &SerialData) const
{
	SerialData << DisplayName;
	SerialData << Path;
	SerialData << HasSubFolders;
	SerialData << NodeType;
	SerialData << IconIndex;
	SerialData << SelectedIconIndex;
	SerialData << RemoteNode;
	SerialData << IconSize;
/*	if (Icon)
	{
		SerialData << true;
		SerialData << Icon;
	}
	else
	{
		SerialData << false;
	}
	if (SelectedIcon)
	{
		SerialData << true;
		SerialData << SelectedIcon;
	}
	else
	{
		SerialData << false;
	}*/
}
//---------------------------------------------------------------------------
TNodeSettings& TNodeSettings::operator << (const TSerialData &SerialData)
{
	SerialData >> DisplayName;
	SerialData >> Path;
	SerialData >> HasSubFolders;
	SerialData >> NodeType;
	SerialData >> IconIndex;
	SerialData >> SelectedIconIndex;
	SerialData >> RemoteNode;
	SerialData >> IconSize;

/*	bool ContainsIcon;
	SerialData >> ContainsIcon;
	if (ContainsIcon)
	{
		Icon = new TIcon();
		SerialData >> Icon;
	}
	SerialData >> ContainsIcon;
	if (ContainsIcon)
	{
		SelectedIcon = new TIcon();
		SerialData >> SelectedIcon;
	} */
	return *this;
}
//---------------------------------------------------------------------------
