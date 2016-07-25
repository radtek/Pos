//---------------------------------------------------------------------------

#ifndef ErrorPrinterH
#define ErrorPrinterH
//---------------------------------------------------------------------------
// Error thrown when a printing error occurs.
class EErrorPrinter
{
public:
	int ErrNum;
   __fastcall EErrorPrinter(int inErrNum);
   __fastcall ~EErrorPrinter();
};

#endif
