//---------------------------------------------------------------------------

#ifndef PrintJobH
#define PrintJobH
//---------------------------------------------------------------------------
class TPrintJob
{
private:
public:
	struct SDeviceHeader
   {
		//--------POS Header--------
      TDeviceDB *Device;
      TConnectPrinter *Connection;
      EProcessingErrorType Error;
      UnicodeString ErrorMsg;
   }Header;

	void AddLines(UnicodeString Data);
	void AddRightJustify(UnicodeString Col1, UnicodeString Col2);
	void AddCentered(UnicodeString Col1);
   void AddLine(UnicodeString Data);
   void Add(UnicodeString Data);
   void Tab();
	void Cut(bool Full);
   void DrawLine();
   void Reverse(unsigned char Lines);
   void NewLine();
	void Reset();
	void Open();
   void KickDraw();
	void Close();
	void Font(bool inLarge,bool inBold,bool inUnderLined, bool inRed = false);
	// Printer Specifics.
	TPrintJob(PrintJobType inJobType,TDeviceReal *Device,TConnectPrinter *Connect);
	~TPrintJob();
   TMemoryStream *JobStream;
   bool JobInProgress;
   // Set by the formating routine if the Job Contains more than just a header.
   bool ContainsData;
	PrintJobType JobType;
	DeviceType JobReceivedFrom;
   bool DataAdded;
   bool PreFormatted;  // Used in the pase ruetine by the printer handling the job
                       // where the Print Jobs Stream Contains
                       // raw data (Strings)to be sent stright to the printer
                       // so do not pase just add to que.

   void Clear();
};
#endif
