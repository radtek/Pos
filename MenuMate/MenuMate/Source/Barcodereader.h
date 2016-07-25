//---------------------------------------------------------------------------

#ifndef BarcodereaderH
#define BarcodereaderH

#include <CPort.hpp>
//---------------------------------------------------------------------------
class TMMBarcodeReader
{
	private:
	TComPort *Port;
		bool Busy;
		bool CloseingPort;
		bool OpeningPort;
		void __fastcall GetData(TObject *Sender,int Count);
	public:
		AnsiString LastBarCode;
		UnicodeString LastException;
		int PortNumber;
		TMMBarcodeReader();
		virtual ~TMMBarcodeReader();
		bool Open(int PortNum);
		void Close();
};

#endif
 