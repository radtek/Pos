//---------------------------------------------------------------------------
#ifndef WindowsPrintersH
#define WindowsPrintersH
//---------------------------------------------------------------------------
#include <system.hpp>
#include <vector>
#include <set>
//---------------------------------------------------------------------------
namespace WinPrinters
{
//---------------------------------------------------------------------------
// Structure returned by EnumPrinters.
// Then use PrinterName for most other fuctions.
//---------------------------------------------------------------------------
struct TWinPrinter
{
	TWinPrinter() {}
	TWinPrinter(LPPRINTER_INFO_2 lppi2);
	TWinPrinter(const AnsiString &connectionString);
	
	AnsiString		ServerName;
	AnsiString		PrinterName;
	AnsiString		ConnectionString;
};
typedef std::vector<TWinPrinter> TWinPrinters;
//---------------------------------------------------------------------------
struct TWinPrintJob
{
	AnsiString		DocumentName;
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
int EnumNetworkPrinters(TWinPrinters &WinPrinters); // Testing
//---------------------------------------------------------------------------
bool PrinterOnline(const TWinPrinter &Printer);
// First checkes to see if printer exists and isn't paused.
// Then checkes the job queue to see if there are any failed jobs. (These stall the printer)
//---------------------------------------------------------------------------
//	Usage Example: PrinterOnline
//
//	if (WinPrinters::PrinterOnline(Printers[ListBox1->ItemIndex].Printer))
//	{
//		Label1->Caption = "Status: Online";
//	}
//	else
//	{
//		Label1->Caption = "Status: Offline";
//	}
//---------------------------------------------------------------------------
bool EnumPrintJobs(const TWinPrinter &Printer, TWinPrintJobs &WinPrintJobs);
// Use if you need a list of jobs in the queue and their status.
//---------------------------------------------------------------------------
//	Usage Example: EnumPrintJobs
//
//	WinPrinters::TWinPrintJobs WinPrintJobs;
//	WinPrinters::EnumPrintJobs(Printers[ListBox1->ItemIndex].Printer, WinPrintJobs);
//	for (unsigned index=0; index<WinPrintJobs.size(); index++)
//	{
//		TListItem *ListItem = ListView1->Items->Add();
//		ListItem->Caption = WinPrintJobs[index].DocumentName;
//		ListItem->SubItems->Add(WinPrintJobs[index].JobID);
//
//		AnsiString Status;
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
TPrintJobStatus GetPrintJobStatus(const TWinPrinter &Printer, int JobID);
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
//		switch (WinPrinters::GetPrintJobStatus(Printers[ListBox1->ItemIndex].Printer, JobID))
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
bool PrintRaw(const TWinPrinter &Printer, AnsiString DocName, AnsiString PrintData,
				int &JobID, bool PrintIfOffline=false);
// Prints raw text (including control chars). Will set JobID so the job can be tracked.
// Returns whether the job is added to the spooler or not.
// Leave out last param to return false if an earlier failed job has stalled the queue.
//---------------------------------------------------------------------------
//	Usage Example: PrintRaw
//
//	if (!WinPrinters::PrintRaw(Printers[ListBox1->ItemIndex].Printer, "My Test Doc " + IntToStr(DocCount++), Printout, JobID))
//	{
//		ShowMessage("Printer's crapped out!");
//	}
//---------------------------------------------------------------------------
class TJobTrackerThread;
class TPrintJobTracker;
// ContinueTracking is ignored if the TimedOut == false
typedef void __fastcall (__closure *TJobTrackerEvent)(TPrintJobTracker *Sender,
		const TWinPrinter &Printer, int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking);
//---------------------------------------------------------------------------
class TPrintJobTracker
{
//---------------------------------------------------------------------------
// Will poll print queue looking at JobID.
//	Will give up after Timeout ms.
// Completes when job in error or job completed/gone
//---------------------------------------------------------------------------
public:
	static TPrintJobTracker* Create(const TWinPrinter &Printer, int JobID, TJobTrackerEvent OnPrintJobStatus, int Timeout=0);
	~TPrintJobTracker();

	int					JobID();
	TPrintJobStatus	CurrentStatus();
private:
	TPrintJobTracker(const TWinPrinter &Printer, int JobID, TJobTrackerEvent JobTrackerEvent, int Timeout, int Interval=250);
	TJobTrackerThread *JobTrackerThread;

	bool					fDeleting;
	
	TWinPrinter			fPrinter;
	int					fJobID;
	int					fTimeout;
	int					fInterval;

	void __fastcall	OnThreadTerminate(TObject* Sender);
	void					fCreateThread(const TWinPrinter &Printer, int JobID, int Timeout, int Interval);
	TJobTrackerEvent	fOnPrintJobStatus;
};
//---------------------------------------------------------------------------
// You could use this class to handle the potential multitude of trackers.
//
//	Usage Example: GetPrintJobStatus
//
//	TPrintJobsTracker PrintJobsTracker(&OnPrintJobStatus);
//
//	WinPrinters::GetPrintJobStatus(Printers[ListBox1->ItemIndex].Printer, JobID);
//	PrintJobsTracker.StartTrackingJob(Printers[ListBox1->ItemIndex].Printer, JobID, 5000);
//
//	void __fastcall TForm1::OnPrintJobStatus(TPrintJobTracker *Sender, const TWinPrinter &Printer,
//			int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking)
//	{
//		AnsiString Caption;
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
//			if (Application->MessageBox(("Timed Out: " + Printer.ConnectionString + " " + Caption).c_str(), "Continue Tracking?", MB_YESNO + MB_ICONQUESTION) == ID_YES)
//			{
//				ContinueTracking = true;
//			}
//		}
//		else
//		{
//			ShowMessage(Printer.ConnectionString + " " + Caption);
//		}
//	}
//---------------------------------------------------------------------------
class TPrintJobsTracker
{
public:
	TPrintJobsTracker(TJobTrackerEvent OnPrintJobStatus);
	~TPrintJobsTracker();

	void StartTrackingJob(const TWinPrinter &Printer, int JobID, int Timeout=0);
	TPrintJobStatus CurrentStatus(const TWinPrinter &Printer, int JobID);
private:
	typedef std::set<TPrintJobTracker *> TPrintJobSet;
	TPrintJobSet fJobs;

	TJobTrackerEvent fOnPrintJobStatus;
	
	void __fastcall fInternalOnStatus(TPrintJobTracker *Sender, const TWinPrinter &Printer,
			int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking);
};
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TPrinterOnlineEvent)(const TWinPrinter &Printer, bool Online);
//---------------------------------------------------------------------------
class TPrinterMonitor
{
public:
	TPrinterMonitor(TPrinterOnlineEvent OnPrinterOnlineChange, int Interval);
	~TPrinterMonitor();

	void	AddPrinter(const TWinPrinter &Printer);
	void	RemovePrinter(const TWinPrinter &Printer);
	void	Clear();

	bool	Monitoring();
private:

	class	TPrinterMonitorThread *fPrinterMonitorThread;
	int	fInterval;
	bool	fStarted;

	bool	fStart();
	void	fStop();

	void	fCreateThread();
	void	fKillThread();
	void __fastcall fOnThreadTerminate(TObject* Sender);

	TPrinterOnlineEvent fOnPrinterOnlineChange;

	std::set<AnsiString> fOnlinePrinters;
	std::set<AnsiString> fOfflinePrinters;
};
//---------------------------------------------------------------------------
}	// namespace WindowsPrinters
//---------------------------------------------------------------------------
#endif
