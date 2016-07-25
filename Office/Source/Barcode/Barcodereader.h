//---------------------------------------------------------------------------

#ifndef BarcodereaderH
#define BarcodereaderH

#define UWM_CARDSWIPE WM_USER + 6

#include <CPort.hpp>
//---------------------------------------------------------------------------
class TMMBarcodeReader
{
private:
	TComPort		*Port;
	bool			Busy;
	bool			CloseingPort;
	bool			OpeningPort;
	AnsiString	LastBarCode;
	void __fastcall GetData(TObject *Sender,int Count);
public:
	TMMBarcodeReader();
	virtual ~TMMBarcodeReader();

	bool	Open(int PortNum);
	void	Close();
};

#endif
 