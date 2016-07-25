//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <values.h>
#include "ZipProg.h"
#include "ZipStrs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


const int MAX_PERCENT = MAXINT/10000;
//----------------------------------------------------------------------------
int __fastcall TProgressDetails::GetItemPerCent(void)
{
  if((FItemSize > 0) && (FItemPosition > 0))
  {
    if(FItemSize < MAX_PERCENT)
      return (100 * FItemPosition) / FItemSize;
    else return FItemPosition / (FItemSize / 100);
	}
  else return 0;
}
//----------------------------------------------------------------------------
int __fastcall TProgressDetails::GetTotalPerCent(void)
{
  if((FTotalSize > 0) && (FTotalPosition > 0))
    return static_cast<int>((100 * FTotalPosition) / FTotalSize);
  else return 0;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::Clear(void)
{
  FProgType = EndOfBatch;
  FItemCount = 0;
  FWritten = 0;
  FTotalSize = 0;
  FTotalPosition = 0;
  FItemSize = 0;
  FItemPosition = 0;
  FItemName = "";
  FItemNumber = 0;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::AdvanceXtra(unsigned adv)
{
  FItemPosition += adv;
  FProgType = ExtraUpdate;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetItemXtra(int xtype, const AnsiString xmsg, unsigned fsize)
{
  FItemName = ZipLoadStr(PR_Progress + xtype, xmsg);
  FItemSize = fsize;
  FItemPosition = 0;
  FProgType = NewExtra;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::Advance(unsigned adv)
{
  FTotalPosition += adv;
  FItemPosition += adv;
  FProgType = ProgressUpdate;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetCount(unsigned count)
{
  Clear();
  FItemCount = count;
  FItemNumber = 0;
  FProgType = TotalFiles2Process;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetSize(__int64 FullSize)
{
  FTotalSize = FullSize;
  FTotalPosition = 0;
  FItemName = "";
  FItemSize = 0;
  FItemPosition = 0;
  FProgType = TotalSize2Process;
  FWritten = 0;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetItem(const AnsiString fname, unsigned fsize)
{
  ++FItemNumber;
  FItemName = fname;
  FItemSize = fsize;
  FItemPosition = 0;
  FProgType = NewFile;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::SetEnd(void)
{
  FItemName = "";
  FItemSize = 0;
  FProgType = EndOfBatch;
}
//----------------------------------------------------------------------------
void __fastcall TProgDetails::Wrote(unsigned bytes)
{
  FWritten += bytes;
}
//----------------------------------------------------------------------------
__fastcall TProgressDetails::~TProgressDetails( void )
{
  FItemName = "";
}
