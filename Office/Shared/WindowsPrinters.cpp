//---------------------------------------------------------------------------
#pragma hdrstop

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "WindowsPrinters.h"
//---------------------------------------------------------------------------
// Testing
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace WinPrinters
{
//---------------------------------------------------------------------------
//#define min(a, b)  (((a) < (b)) ? (a) : (b))
//---------------------------------------------------------------------------
TWinPrinter::TWinPrinter(LPPRINTER_INFO_2 lppi2)
{
	if (AnsiString(lppi2->pServerName) == "" && AnsiString(lppi2->pShareName) == "" && AnsiString(lppi2->pPortName).SubString(1, 2) == "\\\\")
	{
		// [Auto XXXXX on YYYYY] type printer
		AnsiString ServerName		= lppi2->pPortName;
		ServerName						= ServerName.SubString(3, ServerName.Length() - 2);
		if (ServerName.AnsiPos("\\") != 0)
		{
			*this = TWinPrinter(lppi2->pPortName);
		}
	}
	else if (AnsiString(lppi2->pServerName) == "" && AnsiString(lppi2->pShareName) != "")
	{
		// Local printer
		DWORD Size = MAX_COMPUTERNAME_LENGTH;
		char ComputerName[MAX_COMPUTERNAME_LENGTH+1] = {0};
		if (GetComputerName(ComputerName, &Size))
		{
			ServerName			= "\\\\" + AnsiString(ComputerName);
			PrinterName			= lppi2->pPrinterName;
			ConnectionString	= lppi2->pShareName;
		}
	}
	else if (AnsiString(lppi2->pServerName) != "" && AnsiString(lppi2->pShareName) != "")
	{
		// Remote shared printer
		DWORD Size = MAX_COMPUTERNAME_LENGTH;
		char ComputerName[MAX_COMPUTERNAME_LENGTH+1] = {0};
		if (GetComputerName(ComputerName, &Size))
		{
			ServerName			= lppi2->pServerName;
			PrinterName			= lppi2->pShareName;
			ConnectionString	= lppi2->pPrinterName;
		}
	}
}
//---------------------------------------------------------------------------
TWinPrinter::TWinPrinter(const AnsiString &connectionString) : ConnectionString(connectionString)
{
	if (ConnectionString.SubString(1, 2) == "\\\\")
	{
		ServerName = ConnectionString.SubString(3, ConnectionString.Length() - 2);
		int SlashPos = ServerName.AnsiPos("\\");
		if (SlashPos)
		{
			PrinterName = ServerName.SubString(SlashPos + 1, ServerName.Length() - SlashPos);
		}
		ServerName = "\\\\" + ServerName.SubString(1, ServerName.Length() - PrinterName.Length() - 1);
	}
}
//---------------------------------------------------------------------------
// This class is basically a handle to the printer.
// It will automatically release the handle on destruction.
// Don't use new if you want it to automatically release on exit of scope!
//---------------------------------------------------------------------------
class TPrinterDetails
{
public:
	TPrinterDetails(const TWinPrinter &Printer);
	~TPrinterDetails();

	bool					GetDetails(HANDLE &Handle, DWORD &Status);
	bool					GetDetails(const PRINTER_INFO_2 * &lppi2);
private:
	HANDLE				fHandle;
	PRINTER_INFO_2		*fPrinterInfo;
//	DWORD		fStatus;
};
//---------------------------------------------------------------------------
// Internal functions.
//---------------------------------------------------------------------------
bool fEnumPrintJobs(HANDLE hPrinter, TWinPrintJobs &WinPrintJobs);
bool fPrinterOnline(HANDLE hPrinter, DWORD PrinterStatus);
WinPrinters::TPrintJobStatus fGetJobStatus(HANDLE hPrinter, int JobID);
//---------------------------------------------------------------------------
//}	// namespace WinPrinters
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
int WinPrinters::EnumPrinters(TWinPrinters &WinPrinters)
{
	WinPrinters.clear();
	DWORD dwNeeded, dwNumPrinters;

	::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &dwNeeded, &dwNumPrinters);

	unsigned char *buffer = new unsigned char[dwNeeded];
	try
	{
		setmem(buffer, dwNeeded, 0);
		::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, buffer, dwNeeded, &dwNeeded, &dwNumPrinters);

		LPPRINTER_INFO_2 lppi2;

		for (int index=0; index<(int)dwNumPrinters; index++)
		{
			lppi2 = (LPPRINTER_INFO_2)(buffer + index * sizeof(PRINTER_INFO_2));

			TWinPrinter WinPrinter(lppi2);

			if (WinPrinter.ServerName			!= "" &&
				 WinPrinter.PrinterName			!= "" &&
				 WinPrinter.ConnectionString	!= "")
			{
				WinPrinters.push_back(WinPrinter);
			}
		}
	}
	__finally
	{
		delete [] buffer;
	}
	return WinPrinters.size();
}
//---------------------------------------------------------------------------
int WinPrinters::EnumNetworkPrinters(TWinPrinters &WinPrinters)
{
	WinPrinters.clear();
	DWORD dwNeeded, dwNumPrinters;
	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_NETWORK | PRINTER_ENUM_REMOTE;

	::EnumPrinters(flags, NULL, 1, NULL, 0, &dwNeeded, &dwNumPrinters);

	unsigned char *buffer = new unsigned char[dwNeeded];
	try
	{
		setmem(buffer, dwNeeded, 0);
		::EnumPrinters(flags, NULL, 1, buffer, dwNeeded, &dwNeeded, &dwNumPrinters);

		LPPRINTER_INFO_1 lppi1;

		for (int index=0; index<(int)dwNumPrinters; index++)
		{
			lppi1 = (LPPRINTER_INFO_1)(buffer + index * sizeof(PRINTER_INFO_1));

			if (lppi1->Flags & PRINTER_ENUM_ICON8)
			{
			AnsiString fullName = lppi1->pName;
//			if (fullName.AnsiPos("\\\\") == 1)
//			{
				TWinPrinter WinPrinter;
				AnsiString serverName = fullName.SubString(3, fullName.Length() - 2);
				if (serverName.AnsiPos("\\") != 0)
				{
					WinPrinter.ServerName	= "\\\\" + serverName.SubString(1, serverName.AnsiPos("\\") - 1);
					WinPrinter.PrinterName	= serverName.SubString(WinPrinter.ServerName.Length(), serverName.Length() - WinPrinter.ServerName.Length() + 1);

					if (WinPrinter.ServerName	!= "" &&
						 WinPrinter.PrinterName	!= "")
					{
						TPrinterDetails PrinterDetails(TWinPrinter(WinPrinter.ServerName + "\\" + WinPrinter.PrinterName));
						LPPRINTER_INFO_2 lppi2 = NULL;
						if (PrinterDetails.GetDetails(lppi2))
						{
							TWinPrinter WinPrinter(lppi2);
							if (WinPrinter.ServerName			!= "" &&
								 WinPrinter.PrinterName			!= "" &&
								 WinPrinter.ConnectionString	!= "")
							{
								WinPrinters.push_back(WinPrinter);
							}
						}
					}
				}
			}
		}
	}
	__finally
	{
		delete [] buffer;
	}
	return WinPrinters.size();
}
//---------------------------------------------------------------------------
/*int WinPrinters::EnumNetworkPrinters(TWinPrinters &WinPrinters)
{
	WinPrinters.clear();
	DWORD dwNeeded, dwNumPrinters;

	::EnumPrinters(PRINTER_ENUM_NETWORK | PRINTER_ENUM_REMOTE, NULL, 1, NULL, 0, &dwNeeded, &dwNumPrinters);

	unsigned char *buffer = new unsigned char[dwNeeded];
	try
	{
		setmem(buffer, dwNeeded, 0);
		::EnumPrinters(PRINTER_ENUM_NETWORK | PRINTER_ENUM_REMOTE, NULL, 1, buffer, dwNeeded, &dwNeeded, &dwNumPrinters);

		LPPRINTER_INFO_1 lppi1;

		for (int index=0; index<(int)dwNumPrinters; index++)
		{
			lppi1 = (LPPRINTER_INFO_1)(buffer + index * sizeof(PRINTER_INFO_1));

			AnsiString fullName = lppi1->pName;
			if (fullName.AnsiPos("\\\\") == 1)
			{
				TWinPrinter WinPrinter;
				AnsiString serverName = fullName.SubString(3, fullName.Length() - 2);
				if (serverName.AnsiPos("\\") != 0)
				{
					WinPrinter.ServerName	= "\\\\" + serverName.SubString(1, serverName.AnsiPos("\\") - 1);
					WinPrinter.PrinterName	= serverName.SubString(WinPrinter.ServerName.Length(), serverName.Length() - WinPrinter.ServerName.Length() + 1);

					if (WinPrinter.ServerName	!= "" &&
						 WinPrinter.PrinterName	!= "")
					{
						TWinPrinter winPrinter = WinPrinter;
						winPrinter.ConnectionString = winPrinter.ServerName + "\\" + winPrinter.PrinterName;
						TPrinterDetails PrinterDetails(winPrinter);
						//if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
						{
							//WinPrinters.push_back(WinPrinter);
						}
					}
				}
			}
		}
	}
	__finally
	{
		delete [] buffer;
	}
	return WinPrinters.size();
} */
//---------------------------------------------------------------------------
bool WinPrinters::PrinterOnline(const TWinPrinter &Printer)
{
	HANDLE	PrinterHandle;
	DWORD		PrinterStatus;

	TPrinterDetails PrinterDetails(Printer);
	if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	{
		return fPrinterOnline(PrinterHandle, PrinterStatus);
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool WinPrinters::EnumPrintJobs(const TWinPrinter &Printer, TWinPrintJobs &WinPrintJobs)
{
	WinPrintJobs.clear();

	HANDLE	hPrinter;
	DWORD		Status;

	TPrinterDetails PrinterDetails(Printer);
	if (PrinterDetails.GetDetails(hPrinter, Status))
	{
		return fEnumPrintJobs(hPrinter, WinPrintJobs);
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
WinPrinters::TPrintJobStatus WinPrinters::GetPrintJobStatus(const TWinPrinter &Printer, int JobID)
{
	HANDLE	PrinterHandle;
	DWORD		PrinterStatus;

	TPrinterDetails PrinterDetails(Printer);
	if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	{
		return fGetJobStatus(PrinterHandle, JobID);
	}
	else
	{
		return jsError;
	}
}
//---------------------------------------------------------------------------
bool WinPrinters::PrintRaw(const TWinPrinter &Printer, AnsiString DocName, AnsiString PrintData,
		int &JobID, bool PrintIfOffline)
{
	bool			Success = false;

	HANDLE		PrinterHandle;
	DWORD			PrinterStatus;
	// Get a handle to the printer
	TPrinterDetails PrinterDetails(Printer);
	if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	{
		// Check that the printer isn't paused or there are stuck jobs (unless you don't want to)
		if (PrintIfOffline || fPrinterOnline(PrinterHandle, PrinterStatus))
		{
			// Print in raw mode
			DOC_INFO_1 DocInfo = {DocName.c_str(), NULL, "RAW"};
			// Usual stuff
			JobID = StartDocPrinter(PrinterHandle, 1, (LPBYTE)&DocInfo);
			if (JobID)
			{
				if (StartPagePrinter(PrinterHandle))
				{
					DWORD BytesWritten;
					if (WritePrinter(PrinterHandle, PrintData.c_str(), PrintData.Length(), &BytesWritten))
					{
						Success = true;
					}
					EndPagePrinter(PrinterHandle);
				}
				EndDocPrinter(PrinterHandle);
			}
		}
	}
	// Return true if sucessfully added to spooler. Use GetJobStatus to see if succesful.
	return Success;
}
//---------------------------------------------------------------------------
// Internal functions
//---------------------------------------------------------------------------
bool WinPrinters::fEnumPrintJobs(HANDLE hPrinter, TWinPrintJobs &WinPrintJobs)
{
	DWORD		cByteNeeded;
	DWORD		nReturned=0;

	if (!EnumJobs(hPrinter, 0, 0xFFFFFFFF, 2, NULL, 0, &cByteNeeded, &nReturned))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) return false;
	}
	JOB_INFO_2 *pJobStorage = (JOB_INFO_2 *)new unsigned char[cByteNeeded];

	bool Success = false;
	try
	{
		ZeroMemory(pJobStorage, cByteNeeded);

		DWORD cByteUsed;
		if (EnumJobs(hPrinter, 0, 0xFFFFFFFF, 2, (LPBYTE)pJobStorage, cByteNeeded, &cByteUsed, &nReturned))
		{
			Success = true;
			for (DWORD i=0; i<nReturned; i++)
			{
				FILETIME FileTime1, FileTime2;
				SystemTimeToFileTime(&pJobStorage[i].Submitted, &FileTime1);
				FileTimeToLocalFileTime(&FileTime1, &FileTime2);
				SYSTEMTIME SystemTime;
				FileTimeToSystemTime(&FileTime2, &SystemTime);

				TWinPrintJob WinPrintJob;

				WinPrintJob.DocumentName	= pJobStorage[i].pDocument;
				WinPrintJob.JobID				= pJobStorage[i].JobId;
				WinPrintJob.Submitted		= SystemTimeToDateTime(SystemTime);
//				if (!pJobStorage[i].pStatus)
					WinPrintJob.Status		= pJobStorage[i].Status;
//				else
//					WinPrintJob.Status		= Dunno???;

				WinPrintJobs.push_back(WinPrintJob);
			}                                          
		}
	}
	__finally
	{
		delete [] (unsigned char *)pJobStorage;
	}
	return Success;
}
//---------------------------------------------------------------------------
bool WinPrinters::fPrinterOnline(HANDLE hPrinter, DWORD PrinterStatus)
{
	bool Online=true;

	if (PrinterStatus & (
			PRINTER_STATUS_BUSY					|
			PRINTER_STATUS_DOOR_OPEN			|
			PRINTER_STATUS_ERROR					|
			PRINTER_STATUS_NO_TONER				|
			PRINTER_STATUS_NOT_AVAILABLE		|
			PRINTER_STATUS_OFFLINE				|
			PRINTER_STATUS_OUT_OF_MEMORY		|
			PRINTER_STATUS_OUTPUT_BIN_FULL	|
			PRINTER_STATUS_PAGE_PUNT			|
			PRINTER_STATUS_PAPER_JAM			|
			PRINTER_STATUS_PAPER_OUT			|
			PRINTER_STATUS_PAPER_PROBLEM		|
			PRINTER_STATUS_PAUSED				|
			PRINTER_STATUS_PENDING_DELETION	|
			PRINTER_STATUS_SERVER_UNKNOWN		|
			PRINTER_STATUS_USER_INTERVENTION))
	{
		Online = false;
	}
	else
	{
		TWinPrintJobs WinPrintJobs;
		if (fEnumPrintJobs(hPrinter, WinPrintJobs))
		{
			for (unsigned i=0; i<WinPrintJobs.size(); i++)
			{
				if (WinPrintJobs[i].Status & JOB_STATUS_PRINTING)
				{
					if (WinPrintJobs[i].Status & (
							JOB_STATUS_BLOCKED_DEVQ	|
							JOB_STATUS_ERROR			|
							JOB_STATUS_OFFLINE		|
							JOB_STATUS_PAPEROUT		|
							JOB_STATUS_PAUSED			|
							JOB_STATUS_USER_INTERVENTION))
					{
						Online = false;
						break;
					}
				}
			}
		}
		else
		{
			Online = false;
		}
	}
	return Online;
}
//---------------------------------------------------------------------------
WinPrinters::TPrintJobStatus WinPrinters::fGetJobStatus(HANDLE hPrinter, int JobID)
{
	TPrintJobStatus Result = jsNotFound;

	DWORD	cbNeeded=0;
	if (!GetJob(hPrinter, JobID, 2, NULL, 0, &cbNeeded))
	{
		DWORD Error = GetLastError();
		if (Error == RPC_S_SERVER_UNAVAILABLE)
		{
			return jsUnknown;
		}
		else if (Error != ERROR_INSUFFICIENT_BUFFER)
		{
			return Result;
		}
	}
	JOB_INFO_2 *pJobStorage = (JOB_INFO_2 *)new unsigned char[cbNeeded];
	try
	{
		if (GetJob(hPrinter, JobID, 2, (LPBYTE)pJobStorage, cbNeeded, &cbNeeded))
		{
			if (pJobStorage->Status & (
					JOB_STATUS_BLOCKED_DEVQ	|
					JOB_STATUS_ERROR			|
					JOB_STATUS_OFFLINE		|
					JOB_STATUS_PAPEROUT		|
					JOB_STATUS_PAUSED			|
					JOB_STATUS_USER_INTERVENTION))
			{
				Result = jsError;
			}
			else if (pJobStorage->Status & (JOB_STATUS_PRINTING | JOB_STATUS_SPOOLING))
			{
				Result = jsPrinting;
			}
			else if (pJobStorage->Status == 0)
			{
				Result = jsWaiting;
			}
			else// if (pJobStorage->Status & (JOB_STATUS_COMPLETE | JOB_STATUS_PRINTED))
			{
				Result = jsComplete;
			}
		}
	}
	__finally
	{
		delete [] pJobStorage;
	}
	return Result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
WinPrinters::TPrinterDetails::TPrinterDetails(const TWinPrinter &Printer) : fHandle(NULL)//, fStatus(0)
{
	fPrinterInfo = NULL;
//	PRINTER_INFO_2		*pPrinterInfo = NULL;
	DWORD					cByteNeeded;
	DWORD					cByteUsed;

	if (OpenPrinter(Printer.ConnectionString.c_str(), &fHandle, NULL) && fHandle)
	{
		if (!GetPrinter(fHandle, 2, NULL, 0, &cByteNeeded) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			ClosePrinter(fHandle);
			fHandle = NULL;
		}
		else
		{
			fPrinterInfo = (PRINTER_INFO_2 *)new unsigned char[cByteNeeded];
			try
			{
				ZeroMemory(fPrinterInfo, cByteNeeded);

				if (GetPrinter(fHandle, 2, (LPSTR)fPrinterInfo, cByteNeeded, &cByteUsed))
				{
//					fStatus = pPrinterInfo->Status;
				}
				else
				{
					ClosePrinter(fHandle);
					fHandle = NULL;
				}
			}
			catch (Exception &E)
			{
				delete [] (unsigned char *)fPrinterInfo;
				fPrinterInfo = NULL;
				throw;
			}
		}
	}
	else
	{
		if (fHandle)
		{
			ClosePrinter(fHandle);
			fHandle = NULL;
		}
	}
}
//---------------------------------------------------------------------------
WinPrinters::TPrinterDetails::~TPrinterDetails()
{
	delete [] (unsigned char *)fPrinterInfo;

	if (fHandle)
	{
		ClosePrinter(fHandle);
		fHandle = NULL;
	}
}
//---------------------------------------------------------------------------
bool WinPrinters::TPrinterDetails::GetDetails(HANDLE &Handle, DWORD &Status)
{
	if (!fHandle)
	{
		return false;
	}
	else
	{
		Handle = fHandle;
		Status = fPrinterInfo->Status;//fStatus;

		return true;
	}
}
//---------------------------------------------------------------------------
bool WinPrinters::TPrinterDetails::GetDetails(const PRINTER_INFO_2 * &lppi2)
{
	if (!fHandle)
	{
		return false;
	}
	else
	{
		lppi2 = fPrinterInfo;
		return true;
	}
}
//---------------------------------------------------------------------------
class TJobTrackerThread : public TThread
{
public:
	TJobTrackerThread(const TWinPrinter &Printer, int JobID, int Timeout, int Interval=250);

	TPrintJobStatus	Status;
	bool					TimedOut;
private:
	void __fastcall	Execute();
	TPrinterDetails	PrinterDetails;

	DWORD					fTimeout;
	int					fJobID;
	int					fInterval;
};
//---------------------------------------------------------------------------
TJobTrackerThread::TJobTrackerThread(const TWinPrinter &Printer, int JobID, int Timeout, int Interval) :
		PrinterDetails(Printer),
		fTimeout(Timeout),
		fJobID(JobID),
		fInterval(Interval),
		TThread(true),
		Status(jsUnknown),
		TimedOut(false)
{
}
//---------------------------------------------------------------------------
void __fastcall TJobTrackerThread::Execute()
{
	HANDLE	PrinterHandle;
	DWORD		PrinterStatus;

	DWORD StartTime = GetTickCount();

	if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	{
		while (!Terminated)
		{
			if (fTimeout != 0 && GetTickCount() >= StartTime + fTimeout)
			{
				Terminate();
				TimedOut = true;
			}
			else
			{
				Status = fGetJobStatus(PrinterHandle, fJobID);
				if (Status != jsWaiting && Status != jsPrinting)
				{
					Terminate();
				}
				else
				{
					unsigned long TickCount = GetTickCount();
					while (!Terminated && GetTickCount() < (TickCount + fInterval))
					{
						Sleep(std::min(fInterval, 500));
//						Sleep(0);
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPrintJobTracker* TPrintJobTracker::Create(const TWinPrinter &Printer, int JobID, TJobTrackerEvent OnJobStatus, int Timeout)
{
	return new TPrintJobTracker(Printer, JobID, OnJobStatus, Timeout);
}
//---------------------------------------------------------------------------
TPrintJobTracker::TPrintJobTracker(const TWinPrinter &Printer, int JobID, TJobTrackerEvent OnPrintJobStatus, int Timeout, int Interval) :
		fDeleting(false),
		JobTrackerThread(NULL),
		fOnPrintJobStatus(OnPrintJobStatus),
		fPrinter(Printer),
		fJobID(JobID),
		fInterval(Interval),
		fTimeout(Timeout)
{
	fCreateThread(fPrinter, fJobID, fTimeout, fInterval);
}
//---------------------------------------------------------------------------
void TPrintJobTracker::fCreateThread(const TWinPrinter &Printer, int JobID, int Timeout, int Interval)
{
	JobTrackerThread = new TJobTrackerThread(Printer, JobID, Timeout, Interval);

	JobTrackerThread->Priority				= tpIdle;
	JobTrackerThread->FreeOnTerminate	= true;
	JobTrackerThread->OnTerminate			= OnThreadTerminate;
	JobTrackerThread->Resume();
}
//---------------------------------------------------------------------------
TPrintJobTracker::~TPrintJobTracker()
{
	try
	{
		if (JobTrackerThread)
		{
			fDeleting = true;
			JobTrackerThread->Terminate();

			unsigned long TickCount = GetTickCount();
			while (JobTrackerThread && GetTickCount() < (TickCount + 10000))
			{
				Application->ProcessMessages();
				Sleep(0);
			}
			if (JobTrackerThread)
			{
				TerminateThread((HANDLE)JobTrackerThread->Handle, 0);
			}
		}
	}
	catch (...)
	{
		// Don't want any pointless exceptions in destructor!
	}
}
//---------------------------------------------------------------------------
TPrintJobStatus TPrintJobTracker::CurrentStatus()
{
	if (JobTrackerThread)
	{
		return JobTrackerThread->Status;
	}
	else
	{
		return jsUnknown;
	}
}
//---------------------------------------------------------------------------
int TPrintJobTracker::JobID()
{
	return fJobID;
}
//---------------------------------------------------------------------------
void __fastcall TPrintJobTracker::OnThreadTerminate(TObject* Sender)
{
	bool ContinueTracking = false;
	if (fOnPrintJobStatus && !fDeleting)
	{
		fOnPrintJobStatus(this, fPrinter, fJobID, JobTrackerThread->TimedOut, JobTrackerThread->Status, ContinueTracking);
	}
	if (JobTrackerThread->TimedOut && ContinueTracking)
	{
		fCreateThread(fPrinter, fJobID, fTimeout, fInterval);
	}
	else
	{
		JobTrackerThread = NULL;
		if (!fDeleting)
		{
			delete this;
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPrintJobsTracker::TPrintJobsTracker(TJobTrackerEvent OnPrintJobStatus) :
		fOnPrintJobStatus(OnPrintJobStatus)
{
}
//---------------------------------------------------------------------------
TPrintJobsTracker::~TPrintJobsTracker()
{
	int Size = fJobs.size();
	Size = Size;
	TPrintJobSet::iterator iJobs;
	while (fJobs.begin() != fJobs.end())
	{
		delete *fJobs.begin();
		fJobs.erase(*fJobs.begin());
	}
}
//---------------------------------------------------------------------------
void TPrintJobsTracker::StartTrackingJob(const TWinPrinter &Printer, int JobID, int Timeout)
{
	TPrintJobTracker *PrintJobTracker = TPrintJobTracker::Create(Printer, JobID, fInternalOnStatus, Timeout);
	fJobs.insert(PrintJobTracker);
}
//---------------------------------------------------------------------------
void __fastcall TPrintJobsTracker::fInternalOnStatus(TPrintJobTracker *Sender, const TWinPrinter &Printer,
			int JobID, bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking)
{
	if (fOnPrintJobStatus)
	{
		try
		{
			fOnPrintJobStatus(Sender, Printer, JobID, TimedOut, Status, ContinueTracking);
		}
		__finally
		{
			if (!ContinueTracking)
			{
				fJobs.erase(Sender);
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TPrinterMonitorThread : public TThread
{
public:
	TPrinterMonitorThread(std::set<AnsiString> &OnlinePrinters, std::set<AnsiString> &OfflinePrinters,
			TPrinterOnlineEvent PrinterOfflineEvent, int Interval=250);

private:
	void __fastcall		Execute();
	int						fInterval;
	std::set<AnsiString>	fOnlinePrinters;
	std::set<AnsiString>	fOfflinePrinters;
	TPrinterOnlineEvent	fPrinterOnlineEvent;
	AnsiString				fCurrentPrinter;

	void __fastcall		CallOfflineEvent();
	void __fastcall		CallOnlineEvent();
};
//---------------------------------------------------------------------------
TPrinterMonitorThread::TPrinterMonitorThread(std::set<AnsiString> &OnlinePrinters, std::set<AnsiString> &OfflinePrinters,
	TPrinterOnlineEvent PrinterOnlineEvent, int Interval) :
		fInterval(Interval),
		fOnlinePrinters(OnlinePrinters),
		fOfflinePrinters(OfflinePrinters),
		fPrinterOnlineEvent(PrinterOnlineEvent),
		TThread(true)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrinterMonitorThread::Execute()
{
	const AnsiString PrintData("\0\0", 1);
	while (!Terminated)
	{
		// Wait a bit before going thru.
		unsigned long TickCount = GetTickCount();
		while (!Terminated && GetTickCount() < (TickCount + fInterval))
		{
			Sleep(200);
		}
		// Check if any online printers are still online
		std::set<AnsiString>::iterator iOnlinePrinters = fOnlinePrinters.begin();

		while (!Terminated && iOnlinePrinters != fOnlinePrinters.end())
		{
			fCurrentPrinter = *iOnlinePrinters;
			++iOnlinePrinters;	// Do now as this printer could be moved

			TWinPrintJobs WinPrintJobs;
			EnumPrintJobs(fCurrentPrinter, WinPrintJobs);
			if (WinPrintJobs.size() == 0)
			{
				int JobID;
				if (!PrintRaw(fCurrentPrinter, "PrinterTest", PrintData, JobID, false))
				{
					Synchronize(CallOfflineEvent);
				}
			}
			else if (!PrinterOnline(fCurrentPrinter))
			{
				Synchronize(CallOfflineEvent);
			}
		}
		// Check if any offline printers are back online
		std::set<AnsiString>::iterator iOfflinePrinters = fOfflinePrinters.begin();

		while (!Terminated && iOfflinePrinters != fOfflinePrinters.end())
		{
			fCurrentPrinter = *iOfflinePrinters;
			++iOfflinePrinters;	// Do now as this printer could be moved

			int JobID;
			if (PrintRaw(fCurrentPrinter, "PrinterTest", PrintData, JobID, false))
			{
				Sleep(std::min(1000, fInterval));
				TPrintJobStatus Status = GetPrintJobStatus(fCurrentPrinter, JobID);
				if (Status != jsError)
				{
					Synchronize(CallOnlineEvent);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TPrinterMonitorThread::CallOfflineEvent()
{
	if (fPrinterOnlineEvent && !Terminated)
	{
		fPrinterOnlineEvent(fCurrentPrinter, false);
		fOnlinePrinters.erase(fCurrentPrinter);
		fOfflinePrinters.insert(fCurrentPrinter);
	}
}
//---------------------------------------------------------------------------
void __fastcall TPrinterMonitorThread::CallOnlineEvent()
{
	if (fPrinterOnlineEvent && !Terminated)
	{
		fPrinterOnlineEvent(fCurrentPrinter, true);
		fOfflinePrinters.erase(fCurrentPrinter);
		fOnlinePrinters.insert(fCurrentPrinter);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TPrinterMonitor::TPrinterMonitor(TPrinterOnlineEvent OnPrinterOnlineChange, int Interval) :
		fOnPrinterOnlineChange(OnPrinterOnlineChange),
		fInterval(Interval),
		fStarted(false),
		fPrinterMonitorThread(NULL)
{
}
//---------------------------------------------------------------------------
TPrinterMonitor::~TPrinterMonitor()
{
	fKillThread();
}
//---------------------------------------------------------------------------
bool TPrinterMonitor::fStart()
{
	if (!fStarted && (fOnlinePrinters.size() > 0 || fOfflinePrinters.size() > 0))
	{
		fCreateThread();
		fStarted = true;
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
bool TPrinterMonitor::Monitoring()
{
	return fStarted;
}
//---------------------------------------------------------------------------
void TPrinterMonitor::fStop()
{
	fKillThread();
	fStarted = false;
}
//---------------------------------------------------------------------------
void TPrinterMonitor::fKillThread()
{
	if (fPrinterMonitorThread && fStarted)
	{
		fPrinterMonitorThread->Terminate();
		Application->ProcessMessages();
		fPrinterMonitorThread->WaitFor();
		delete fPrinterMonitorThread;
		fPrinterMonitorThread = NULL;
/*
		unsigned long TickCount = GetTickCount();
		while (fPrinterMonitorThread && GetTickCount() < (TickCount + 10000))
		{
			Application->ProcessMessages();
			Sleep(0);
		}
		if (fPrinterMonitorThread)
		{
			TerminateThread((HANDLE)fPrinterMonitorThread->Handle, 0);
		}*/
	}
}
//---------------------------------------------------------------------------
void TPrinterMonitor::AddPrinter(const TWinPrinter &Printer)
{
	fStop();
	fOnlinePrinters.insert(Printer.ConnectionString);
	fStart();
}
//---------------------------------------------------------------------------
void TPrinterMonitor::RemovePrinter(const TWinPrinter &Printer)
{
	fStop();
	fOnlinePrinters.erase(Printer.ConnectionString);
	if (fOnlinePrinters.size() > 0) fStart();
}
//---------------------------------------------------------------------------
void TPrinterMonitor::Clear()
{
	fStop();
	fOnlinePrinters.clear();
}
//---------------------------------------------------------------------------
void TPrinterMonitor::fCreateThread()
{
	if (!fPrinterMonitorThread)
	{
		fPrinterMonitorThread = new TPrinterMonitorThread(fOnlinePrinters, fOfflinePrinters, fOnPrinterOnlineChange, fInterval);

		fPrinterMonitorThread->Priority			= tpIdle;
//		fPrinterMonitorThread->FreeOnTerminate	= true;
//		fPrinterMonitorThread->OnTerminate		= fOnThreadTerminate;
		fPrinterMonitorThread->Resume();
	}
}
//---------------------------------------------------------------------------
void __fastcall TPrinterMonitor::fOnThreadTerminate(TObject* Sender)
{
	fPrinterMonitorThread = NULL;
}
//---------------------------------------------------------------------------
}	// namespace WinPrinters
//---------------------------------------------------------------------------
/*Bills were $44 + $50
I bought 5 dvds in packet $7.50,
half the spindle $14,
3 dvd+rw $6
groceries $23

50 + 30 - (14 + 6 + 23)*/
