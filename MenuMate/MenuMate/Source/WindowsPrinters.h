//---------------------------------------------------------------------------
#ifndef WindowsPrintersH
#define WindowsPrintersH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <vector>
#include <set>
#include "GraphicPrinterSettings.h"
//---------------------------------------------------------------------------
namespace WinPrinters
{
//---------------------------------------------------------------------------
// Structure returned by EnumPrinters.
// Then use PrinterName for most other fuctions.
//---------------------------------------------------------------------------
struct TWinPrinter
{
	UnicodeString		ServerName;		// Null if local printer.
	UnicodeString		PrinterName;	// Path to use for printing.
	UnicodeString		ShareName;		// Shared as...
	UnicodeString		PortName;		// LPT1 etc.
	UnicodeString		DriverName;		// 'Generic / Text' etc.
	UnicodeString		Comment;
	UnicodeString		Location;
};

typedef std::vector<TWinPrinter> TWinPrinters;
//---------------------------------------------------------------------------
struct TWinPrintJob
{
	UnicodeString		DocumentName;
	DWORD				JobID;
	DWORD				Status;
	TDateTime		Submitted;
};
typedef std::vector<TWinPrintJob> TWinPrintJobs;
//---------------------------------------------------------------------------
enum TPrintJobStatus { jsUnknown, jsNotFound, jsWaiting, jsPrinting, jsError, jsComplete };
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int EnumPrinters(TWinPrinters &WinPrinters);
// Provide a TWinPrinters object to fill with avalable printers for this PC.
//---------------------------------------------------------------------------
//	Usage Example: EnumPrinters
//
//	WinPrinters::TWinPrinters Printers;
//	ListBox1->Items->Clear();
//	WinPrinters::EnumPrinters(Printers);
//	for (unsigned index=0; index<Printers.size(); index++)
//	{
//		ListBox1->Items->Add(Printers[index].PrinterName);
//	}
//---------------------------------------------------------------------------
bool PrinterOnline(const UnicodeString &PrinterName);
// First checkes to see if printer exists and isn't paused.
// Then checkes the job queue to see if there are any failed jobs. (These stall the printer)
//---------------------------------------------------------------------------
//	Usage Example: PrinterOnline
//
//	if (WinPrinters::PrinterOnline(Printers[ListBox1->ItemIndex].PrinterName))
//	{
//		Label1->Caption = "Status: Online";
//	}
//	else
//	{
//		Label1->Caption = "Status: Offline";
//	}
//---------------------------------------------------------------------------
bool EnumPrintJobs(const UnicodeString &PrinterName, TWinPrintJobs &WinPrintJobs);
// Use if you need a list of jobs in the queue and their status.
//---------------------------------------------------------------------------
//	Usage Example: EnumPrintJobs
//
//	WinPrinters::TWinPrintJobs WinPrintJobs;
//	WinPrinters::EnumPrintJobs(Printers[ListBox1->ItemIndex].PrinterName, WinPrintJobs);
//	for (unsigned index=0; index<WinPrintJobs.size(); index++)
//	{
//		TListItem *ListItem = ListView1->Items->Add();
//		ListItem->Caption = WinPrintJobs[index].DocumentName;
//		ListItem->SubItems->Add(WinPrintJobs[index].JobID);
//
//		UnicodeString Status;
//
//		if (WinPrintJobs[index].Status & JOB_STATUS_PRINTING)	Status	+= " - Printing";
//		if (WinPrintJobs[index].Status & JOB_STATUS_PAUSED)	Status	+= " - Paused";
//		if (WinPrintJobs[index].Status & JOB_STATUS_ERROR)		Status	+= " - Error";
//		if (WinPrintJobs[index].Status & JOB_STATUS_DELETING)	Status	+= " - Deleting";
//		if (WinPrintJobs[index].Status & JOB_STATUS_SPOOLING)	Status	+= " - Spooling";
//		if (WinPrintJobs[index].Status & JOB_STATUS_OFFLINE)	Status	+= " - Offline";
//		if (WinPrintJobs[index].Status & JOB_STATUS_PAPEROUT)	Status	+= " - Paper Out";
//		if (WinPrintJobs[index].Status & JOB_STATUS_PRINTED)	Status	+= " - Printed";
//
//		ListItem->SubItems->Add(Status.SubString(3, Status.Length() - 2));
//		ListItem->SubItems->Add(IntToStr(int(HourSpan(WinPrintJobs[index].Submitted , Now()) * 3600.0)));
//---------------------------------------------------------------------------
TPrintJobStatus GetPrintJobStatus(const UnicodeString &PrinterName, int JobID);
// Track a job added with "PrintRaw" using "JobID".
// Will usually return jsPrinting immediately after printing followed by
// 	jsNotFound as the job has been removed from the queue.
//	jsWaiting will be returned if it not at the top of the queue. One at a time!
//	jsError will be returned if the printer couldn't print it.
//	jsUnknown will be returned if the printer is gone (e.g. computer turned off).
//---------------------------------------------------------------------------
//	Usage Example: GetPrintJobStatus
//
//	if (JobID)
//	{
//		switch (WinPrinters::GetPrintJobStatus(Printers[ListBox1->ItemIndex].PrinterName, JobID))
//		{
//			case jsUnknown:	Label2->Caption = "Job " + IntToStr(JobID) + ": Unknown";	break;
//			case jsNotFound:	Label2->Caption = "Job " + IntToStr(JobID) + ": Not Found";	break;
//			case jsWaiting:	Label2->Caption = "Job " + IntToStr(JobID) + ": Waiting";	break;
//			case jsPrinting:	Label2->Caption = "Job " + IntToStr(JobID) + ": Printing";	break;
//			case jsError:		Label2->Caption = "Job " + IntToStr(JobID) + ": Error";		break;
//			case jsComplete:	Label2->Caption = "Job " + IntToStr(JobID) + ": Complete";	break;
//		}
//	}
//---------------------------------------------------------------------------
bool PrintRaw(const UnicodeString &PrinterName, UnicodeString DocName, AnsiString PrintData,
				int &JobID, bool PrintIfOffline=false);

bool PrintGraphics(const UnicodeString &PrinterName, UnicodeString DocName, UnicodeString PrintData,
		int &JobID, TGraphicPrinterSettings &Settings,bool PrintIfOffline=false);
      
void DrawReceiptHeader(TPrinter *Printer,TGraphicPrinterSettings &Settings);
void DrawBackGround(TPrinter *Printer,TGraphicPrinterSettings &Settings);
// Prints raw text (including control chars). Will set JobID so the job can be tracked.
// Returns whether the job is added to the spooler or not.
// Leave out last param to return false if an earlier failed job has stalled the queue.
//---------------------------------------------------------------------------
//	Usage Example: PrintRaw
//
//	if (!WinPrinters::PrintRaw(Printers[ListBox1->ItemIndex].PrinterName, "My Test Doc " + IntToStr(DocCount++), Printout, JobID))
//	{
//		ShowMessage("Printer's crapped out!");
//	}
//---------------------------------------------------------------------------
class TJobTrackerThread;
class TPrintJobTracker;
// ContinueTracking is ignored if the TimedOut == false
typedef void __fastcall (__closure *TJobTrackerEvent)(TPrintJobTracker *Sender,
		const UnicodeString &PrinterName, int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking);
//---------------------------------------------------------------------------
class TPrintJobTracker
{
//---------------------------------------------------------------------------
// Will poll print queue looking at JobID.
//	Will give up after Timeout ms.
// Completes when job in error or job completed/gone
//---------------------------------------------------------------------------
public:
	static TPrintJobTracker* Create(const UnicodeString &PrinterName, int JobID, TJobTrackerEvent OnPrintJobStatus, int Timeout=0);
	~TPrintJobTracker();

	int					JobID();
	TPrintJobStatus	CurrentStatus();
private:
	TPrintJobTracker(const UnicodeString &PrinterName, int JobID, TJobTrackerEvent JobTrackerEvent, int Timeout, int Interval=250);
	TJobTrackerThread *JobTrackerThread;

	bool					fDeleting;
	
	UnicodeString			fPrinterName;
	int					fJobID;
	int					fTimeout;
	int					fInterval;

	void __fastcall	OnThreadTerminate(TObject* Sender);
	void					fCreateThread(const UnicodeString &PrinterName, int JobID, int Timeout, int Interval);
	TJobTrackerEvent	fOnPrintJobStatus;
};
//---------------------------------------------------------------------------
// You could use this class to handle the potential multitude of trackers.
//
//	Usage Example: GetPrintJobStatus
//
//	TPrintJobsTracker PrintJobsTracker(&OnPrintJobStatus);
//
//	WinPrinters::GetPrintJobStatus(Printers[ListBox1->ItemIndex].PrinterName, JobID);
//	PrintJobsTracker.StartTrackingJob(Printers[ListBox1->ItemIndex].PrinterName, JobID, 5000);
//
//	void __fastcall TForm1::OnPrintJobStatus(TPrintJobTracker *Sender, const UnicodeString &PrinterName,
//			int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking)
//	{
//		UnicodeString Caption;
//
//		switch (Status)
//		{
//			case jsUnknown:	Caption = "Job " + IntToStr(JobID) + ": Unknown";		break;
//			case jsNotFound:	Caption = "Job " + IntToStr(JobID) + ": Not Found";	break;
//			case jsWaiting:	Caption = "Job " + IntToStr(JobID) + ": Waiting";		break;
//			case jsPrinting:	Caption = "Job " + IntToStr(JobID) + ": Printing";		break;
//			case jsError:		Caption = "Job " + IntToStr(JobID) + ": Error";			break;
//			case jsComplete:	Caption = "Job " + IntToStr(JobID) + ": Complete";		break;
//		}
//		if (TimedOut)
//		{
//			if (Application->MessageBox(("Timed Out: " + PrinterName + " " + Caption).c_str(), "Continue Tracking?", MB_YESNO + MB_ICONQUESTION) == ID_YES)
//			{
//				ContinueTracking = true;
//			}
//		}
//		else
//		{
//			ShowMessage(PrinterName + " " + Caption);
//		}
//	}
//---------------------------------------------------------------------------
class TPrintJobsTracker
{
public:
	TPrintJobsTracker(TJobTrackerEvent OnPrintJobStatus);
	~TPrintJobsTracker();

	void StartTrackingJob(const UnicodeString &PrinterName, int JobID, int Timeout=0);
	TPrintJobStatus CurrentStatus(const UnicodeString &PrinterName, int JobID);
private:
	typedef std::set<TPrintJobTracker *> TPrintJobSet;
	TPrintJobSet fJobs;

	TJobTrackerEvent fOnPrintJobStatus;
	
	void __fastcall fInternalOnStatus(TPrintJobTracker *Sender, const UnicodeString &PrinterName,
			int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking);
};
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
class TPrinterMonitorThread;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TPrinterOnlineEvent)(const UnicodeString &PrinterName, bool Online);
//---------------------------------------------------------------------------
class TPrinterMonitor
{
public:
	TPrinterMonitor(TPrinterOnlineEvent OnPrinterOnlineChange, int Interval);
	~TPrinterMonitor();

	void	AddPrinter(const UnicodeString &PrinterName);
	void	RemovePrinter(const UnicodeString &PrinterName);

	bool	Start();
	void	Stop();
private:

	TPrinterMonitorThread *fPrinterMonitorThread;
	int	fInterval;
	bool	fStarted;

	void	fCreateThread();
	void	fKillThread();
	void __fastcall fOnThreadTerminate(TObject* Sender);

	TPrinterOnlineEvent fOnPrinterOnlineChange;

	std::set<UnicodeString> fOnlinePrinters;
	std::set<UnicodeString> fOfflinePrinters;
};
//---------------------------------------------------------------------------
}	// namespace WindowsPrinters
//---------------------------------------------------------------------------
#endif
