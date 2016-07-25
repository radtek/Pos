//---------------------------------------------------------------------------

#ifndef ZipProgH
#define ZipProgH
//---------------------------------------------------------------------------

#ifdef ALLOW_2G
#define TProgressSize __int64
#else
#define TProgressSize int          // - will allow uncompressed size to almost 2G
//  TProgressSize  unsinged;   // - will allow uncompressed size to almost 4G
#endif

enum ProgressType {NewFile, ProgressUpdate, EndOfBatch, TotalFiles2Process,
    								 TotalSize2Process, NewExtra, ExtraUpdate };

namespace Zipprogress
{


#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif
class PACKAGE TProgressDetails : public TObject
{
protected:
  ProgressType FProgType;
  int FItemCount;
  __int64 FWritten;
  __int64 FTotalSize;
  __int64 FTotalPosition;
  unsigned FItemSize;
  unsigned FItemPosition;
  AnsiString FItemName;
  int FItemNumber;
  int FItemPerCent;
  int FTotalPerCent;
  int __fastcall GetItemPerCent(void);
  int __fastcall GetTotalPerCent(void);
public:
    __fastcall TProgressDetails(void) {};
		__fastcall virtual ~TProgressDetails(void);
    __property int ItemPerCent ={read=GetItemPerCent};
    __property int TotalPerCent ={read=GetTotalPerCent};
    __property ProgressType Order = {read=FProgType};
    __property int TotalCount = {read=FItemCount};
    __property __int64 BytesWritten = {read=FWritten};
    __property __int64 TotalSize = {read=FTotalSize};
    __property __int64 TotalPosition = {read=FTotalPosition};
    __property unsigned ItemSize = {read=FItemSize};
    __property unsigned ItemPosition = {read=FItemPosition};
    __property AnsiString ItemName = {read=FItemName};
    __property int ItemNumber = {read=FItemNumber};
};

class __declspec(delphiclass) TProgDetails : public TProgressDetails
{
public:
  __fastcall TProgDetails(void) {};
	__fastcall virtual ~TProgDetails( void ) {};
  void __fastcall Clear(void);
  void __fastcall Advance(unsigned adv);
  void __fastcall SetCount(unsigned count);
  void __fastcall SetSize(__int64 FullSize);
  void __fastcall SetItem(const AnsiString fname, unsigned fsize);
  void __fastcall SetEnd(void);
  void __fastcall AdvanceXtra(unsigned adv);
  void __fastcall SetItemXtra(int xtype, const AnsiString xmsg, unsigned fsize);
  void __fastcall Wrote(unsigned bytes);
};

#pragma option -a.


}	/* namespace Zipprogress */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Zipprogress;
#endif

#endif

