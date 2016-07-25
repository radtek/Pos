//---------------------------------------------------------------------------
#ifndef MM_RemoteBrowseH
#define MM_RemoteBrowseH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <Graphics.hpp>
#include <Vector>
#include "SerialData.h"
//---------------------------------------------------------------------------
namespace RemoteBrowser
{
//---------------------------------------------------------------------------
const int MSG_RBRWS_RESET				= 0x01000010;
const int MSG_RBRWS_DRIVES				= 0x01000020;
const int MSG_RBRWS_FOLDERS			= 0x01000030;
const int MSG_RBRWS_ICONS				= 0x01000040;
//---------------------------------------------------------------------------
enum TNodeType {

	ntUnknown,
	ntMyComputer,
	ntFolder,
	ntFile
};
//---------------------------------------------------------------------------
enum TBrowseFilter {

	bfFixedDrives		= 0x0001,
	bfRemovableDrives	= 0x0002,
	bfCDROM				= 0x0004,
	bfNetwork			= 0x0008,
	bfFolders			= 0x0010,
	bfFiles				= 0x0020,

	bfAll					= 0xFFFF
};
//---------------------------------------------------------------------------
enum TIconSize { isLarge, isSmall };
//---------------------------------------------------------------------------
class TNodeSettings
{
public:
	TNodeSettings() : IconSize(isSmall), NodeType(ntUnknown), RemoteNode(NULL), IconIndex(-1), SelectedIconIndex(-1) {}
	TNodeSettings(const TNodeSettings &Rhs);
	TNodeSettings(const TNodeSettings *Rhs);

	AnsiString				DisplayName;
	AnsiString				Path;
	bool						HasSubFolders;
	TNodeType				NodeType;
	int						IconIndex;
	int						SelectedIconIndex;

	class TRBNode *		RemoteNode;

	TIconSize				IconSize;

	TNodeSettings & operator = (const TNodeSettings &Rhs);

	void operator >> (TSerialData &SerialData) const;
	TNodeSettings& operator << (const TSerialData &SerialData);
};
//---------------------------------------------------------------------------
typedef std::vector<TNodeSettings*> TRBNodes;
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif

