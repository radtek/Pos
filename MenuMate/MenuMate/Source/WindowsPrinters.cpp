// ---------------------------------------------------------------------------
#pragma hdrstop

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Printers.hpp>
#include <Graphics.hpp>
#include <Sysutils.hpp>
#include <memory>
#include "WindowsPrinters.h"
#include "MMLogging.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
namespace WinPrinters
{
   // ---------------------------------------------------------------------------
#define min(a, b)  (((a) < (b)) ? (a) : (b))

   // ---------------------------------------------------------------------------
   // This class is basically a handle to the printer.
   // It will automatically release the handle on destruction.
   // Don't use new if you want it to automatically release on exit of scope!
   // ---------------------------------------------------------------------------
   class TPrinterDetails
   {
   public:
	  TPrinterDetails(const UnicodeString &PrinterName);
	  ~TPrinterDetails();

	  bool GetDetails(HANDLE &Handle, DWORD &Status);

   private:
	  HANDLE fHandle;
	  DWORD fStatus;
   };

   // ---------------------------------------------------------------------------
   // Internal functions.
   // ---------------------------------------------------------------------------
   bool fEnumPrintJobs(HANDLE hPrinter, TWinPrintJobs & WinPrintJobs);
   bool fPrinterOnline(HANDLE hPrinter, DWORD PrinterStatus);
   WinPrinters::TPrintJobStatus fGetJobStatus(HANDLE hPrinter, int JobID);

   // ---------------------------------------------------------------------------
   // }	// namespace WinPrinters
   // ---------------------------------------------------------------------------
   // Public functions
   // ---------------------------------------------------------------------------
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

		 for (int index = 0; index < (int)dwNumPrinters; index++)
		 {
			lppi2 = (LPPRINTER_INFO_2)(buffer + index*sizeof(PRINTER_INFO_2));

			TWinPrinter WinPrinter;

			WinPrinter.ServerName = lppi2->pServerName;
			WinPrinter.PrinterName = lppi2->pPrinterName;
			WinPrinter.ShareName = lppi2->pShareName;
			WinPrinter.PortName = lppi2->pPortName;
			WinPrinter.DriverName = lppi2->pDriverName;
			WinPrinter.Comment = lppi2->pComment;
			WinPrinter.Location = lppi2->pLocation;

			WinPrinters.push_back(WinPrinter);
		 }
	  }
	  __finally
	  {
		 delete[]buffer;
	  }
	  return WinPrinters.size();
   }

   // ---------------------------------------------------------------------------
   bool WinPrinters::PrinterOnline(const UnicodeString &PrinterName)
   {
	  HANDLE PrinterHandle;
	  DWORD PrinterStatus;

	  TPrinterDetails PrinterDetails(PrinterName);
	  if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	  {
		 return fPrinterOnline(PrinterHandle, PrinterStatus);
	  }
	  else
	  {
		 return false;
	  }
   }

   // ---------------------------------------------------------------------------
   bool WinPrinters::EnumPrintJobs(const UnicodeString &PrinterName, TWinPrintJobs &WinPrintJobs)
   {
	  WinPrintJobs.clear();

	  HANDLE hPrinter;
	  DWORD Status;

	  TPrinterDetails PrinterDetails(PrinterName);
	  if (PrinterDetails.GetDetails(hPrinter, Status))
	  {
		 return fEnumPrintJobs(hPrinter, WinPrintJobs);
	  }
	  else
	  {
		 return false;
	  }
   }

   // ---------------------------------------------------------------------------
   WinPrinters::TPrintJobStatus WinPrinters::GetPrintJobStatus(const UnicodeString &PrinterName, int JobID)
   {
	  HANDLE PrinterHandle;
	  DWORD PrinterStatus;

	  TPrinterDetails PrinterDetails(PrinterName);
	  if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	  {
		 return fGetJobStatus(PrinterHandle, JobID);
	  }
	  else
	  {
		 return jsError;
	  }
   }

   // ---------------------------------------------------------------------------

bool WinPrinters::PrintGraphics(const UnicodeString &PrinterName, UnicodeString DocName, UnicodeString PrintData, int &JobID,
	  TGraphicPrinterSettings &Settings, bool PrintIfOffline)
   {

	  bool Success = false;
	  try
	  {
		 HANDLE PrinterHandle;
		 DWORD PrinterStatus;
		 // Get a handle to the printer
		 TPrinterDetails PrinterDetails(PrinterName);
		 if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
		 {
			// Check that the printer isn't paused or there are stuck jobs (unless you don't want to)
			if (PrintIfOffline || fPrinterOnline(PrinterHandle, PrinterStatus))
			{
			   TPrinter *Printer = ::Printer();
			   Printer->PrinterIndex = Printer->Printers->IndexOf(PrinterName);
			   // begin the printing
			   Printer->BeginDoc();
			   DrawBackGround(Printer, Settings);
			   DrawReceiptHeader(Printer, Settings);

			   std::auto_ptr <TStringList> PrinterStringList(new TStringList);
			   PrinterStringList->Text = PrintData;

			   Printer->Canvas->Font->Color = clBlack;
			   Printer->Canvas->Font->Name = Settings.GraphicFontName;
			   Printer->Canvas->Font->Size = Settings.GraphicFontSize;
			   Printer->Canvas->Font->Style = TFontStyles();
			   Printer->Canvas->Brush->Color = clBlack;
			   Printer->Canvas->Brush->Style = bsClear;

			   int yOffSet = 0;
			   int PageCount = 1;
			   int CurrentPage = 0;
			   int MaxPageCount = PrinterStringList->Count / Settings.GraphicReceiptMaxLines;
			   if (MaxPageCount == 0)
				  MaxPageCount = 1;
			   if (PrinterStringList->Count % Settings.GraphicReceiptMaxLines != 0)
				  MaxPageCount++;
			   UnicodeString PageCountTextTemplate = "Page %x/" + IntToStr(MaxPageCount);

			   for (int i = 0; i < PrinterStringList->Count; i++, yOffSet++)
			   {
				  Printer->Canvas->TextOut(Settings.GraphicReceiptX,
					 Settings.GraphicReceiptY + (yOffSet * Printer->Canvas->TextHeight(PrinterStringList->Strings[i])),
					 PrinterStringList->Strings[i]);

				  if (CurrentPage != PageCount)
				  {
					 UnicodeString PageCountText = StringReplace(PageCountTextTemplate, "%x", IntToStr(PageCount),
						TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
					 int PageNoX = Settings.GraphicPageCountX;
					 int PageNoY = Settings.GraphicPageCountY;
					 if (PageNoX == -1)
						PageNoX = Printer->PageWidth - Printer->Canvas->TextWidth(PageCountText);
					 if (PageNoY == -1)
						PageNoY = Printer->PageHeight - Printer->Canvas->TextHeight(PageCountText);

					 Printer->Canvas->TextOut(PageNoX, PageNoY, PageCountText);
					 CurrentPage = PageCount;
				  }

				  if ((i != 0) && (i % Settings.GraphicReceiptMaxLines == 0))
				  {
					 yOffSet = 0;
					 Printer->NewPage();
					 DrawBackGround(Printer, Settings);
					 DrawReceiptHeader(Printer, Settings);
					 PageCount++;
				  }
			   }

			   Printer->EndDoc();
			   Success = true;
			}
		 }
	  }
	  catch(Exception & E)
	  {
		 Success = false;
	  }

	  // Return true if sucessfully added to spooler. Use GetJobStatus to see if succesful.
	  return Success;
}


bool WinPrinters::PrintRaw(const UnicodeString &PrinterName, UnicodeString DocName, AnsiString PrintData, int &JobID,
	  bool PrintIfOffline)
   {

	  bool Success = false;

	  HANDLE PrinterHandle;
	  DWORD PrinterStatus;
	  // Get a handle to the printer
	  TPrinterDetails PrinterDetails(PrinterName);
	  if (PrinterDetails.GetDetails(PrinterHandle, PrinterStatus))
	  {
		 // Check that the printer isn't paused or there are stuck jobs (unless you don't want to)
		 if (PrintIfOffline || fPrinterOnline(PrinterHandle, PrinterStatus))
		 {
			// Print in raw mode
            if(DocName == "")
            {
                DocName = "MenuMate Docket";
            }
			DOC_INFO_1 DocInfo =
			{
			   DocName.t_str(), NULL, NULL
			};
			// DOC_INFO_1 DocInfo = {DocName.c_str(), NULL, NULL};
			// Usual stuff
			JobID = StartDocPrinter(PrinterHandle, 1, (LPBYTE) & DocInfo);
			if (JobID)
			{
			   if (StartPagePrinter(PrinterHandle))
			   {
				  DWORD BytesWritten;
				  if (WritePrinter(PrinterHandle, PrintData.c_str(), PrintData.Length(), &BytesWritten))
				  {
					 Success = true;
                     PrintData="";
				  }
                  else
                  {
                      TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped WritePrinter Failed");
                  }
				  EndPagePrinter(PrinterHandle);
			   }
               else
               {
                   TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped StartPagePrinter Failed");
               }
			   EndDocPrinter(PrinterHandle);
			}
            else
            {
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped StartDocPrinter Failed");
            }
		 }
         else
         {
	         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped Printer Offline");
         }
	  }
      else
      {
          TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped Unable to get printer detials");
      }
	  // Return true if sucessfully added to spooler. Use GetJobStatus to see if succesful.
	  return Success;
   }



void WinPrinters::DrawReceiptHeader(TPrinter *Printer, TGraphicPrinterSettings &Settings)
   {
	  if (Settings.GraphicHeaderFile != "")
	  {
		 Graphics::TBitmap *CanvasHeader = new Graphics::TBitmap();
		 UnicodeString Path = ExtractFilePath(Application->ExeName) + Settings.GraphicHeaderFile;
		 if (FileExists(Path))
		 {
			CanvasHeader->LoadFromFile(Path);
			// create a memory dc for the image
			HDC h_dc = CanvasHeader->Canvas->Handle;
			int bmp_w = CanvasHeader->Width;
			int bmp_h = CanvasHeader->Height;
			// Creaet a compatable Device Context with the canvas.
			HDC h_mem_dc = ::CreateCompatibleDC(h_dc);
			HBITMAP h_mem_bmp = ::CreateCompatibleBitmap(h_dc, bmp_w, bmp_h);
			HBITMAP h_old_bmp = (HBITMAP)::SelectObject(h_mem_dc, h_mem_bmp);

			// fix up bad video drivers
			bool is_pal_dev = false;
			LOGPALETTE *pal;
			HPALETTE h_pal, h_old_pal;

			if (::GetDeviceCaps(CanvasHeader->Canvas->Handle, RASTERCAPS) & RC_PALETTE)
			{
			   pal = static_cast <LOGPALETTE*> (malloc(sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256)));
			   memset(pal, 0, sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256));
			   pal->palVersion = 0x300;
			   pal->palNumEntries = ::GetSystemPaletteEntries(CanvasHeader->Canvas->Handle, 0, 256, pal->palPalEntry);
			   if (pal->palNumEntries != 0)
			   {
				  h_pal = ::CreatePalette(pal);
				  h_old_pal = ::SelectPalette(h_mem_dc, h_pal, false);
				  is_pal_dev = true;
			   }
			   else
			   {
				  free(pal);
			   }
			}

			// copy the image on to the memory dc
			::BitBlt(h_mem_dc, 0, 0, bmp_w, bmp_h, h_dc, 0, 0, SRCCOPY);

			if (is_pal_dev)
			{
			   ::SelectPalette(h_mem_dc, h_old_pal, false);
			   ::DeleteObject(h_pal);
			}

			// delete the mem dc
  //			::SelectObject(h_mem_dc, h_old_bmp);
			::DeleteDC(h_mem_dc);

			// get memory for a BITMAPIFO Structure
			HANDLE h_bmp_info = ::GlobalAlloc(GHND, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256));
			BITMAPINFO* bmp_info = static_cast <BITMAPINFO*> (::GlobalLock(h_bmp_info));
			// Set up the structure
			memset(bmp_info, NULL, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 255));
			bmp_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmp_info->bmiHeader.biPlanes = 1;
			bmp_info->bmiHeader.biBitCount = 8;
			bmp_info->bmiHeader.biWidth = bmp_w;
			bmp_info->bmiHeader.biHeight = bmp_h;
			bmp_info->bmiHeader.biCompression = BI_RGB;

			// find out how much memory for the bits
			::GetDIBits(h_dc, h_mem_bmp, 0, bmp_h, NULL, bmp_info, DIB_RGB_COLORS);

			// Allocate memory for the bits
			HANDLE h_bits = GlobalAlloc(GHND, bmp_info->bmiHeader.biSizeImage);
			void *bits = ::GlobalLock(h_bits);

			// this time get the bits
			::GetDIBits(h_dc, h_mem_bmp, 0, bmp_h, bits, bmp_info, DIB_RGB_COLORS);

			// fix up for bad video driver
			if (is_pal_dev)
			{
			   for (int i = 0; i < pal->palNumEntries; i++)
			   {
				  bmp_info->bmiColors[i].rgbRed = pal->palPalEntry[i].peRed;
				  bmp_info->bmiColors[i].rgbGreen = pal->palPalEntry[i].peGreen;
				  bmp_info->bmiColors[i].rgbBlue = pal->palPalEntry[i].peBlue;
			   }
			   free(pal);
			}

			// fix up for print with palette
			is_pal_dev = false;
			if (::GetDeviceCaps(h_dc, RASTERCAPS) & RC_PALETTE)
			{
			   pal = static_cast <LOGPALETTE*> (malloc(sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256)));
			   memset(pal, 0, sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256));
			   pal->palVersion = 0x300;
			   pal->palNumEntries = 256;
			   for (int i = 0; pal->palNumEntries; i++)
			   {
				  pal->palPalEntry[i].peRed = bmp_info->bmiColors[i].rgbRed;
				  pal->palPalEntry[i].peGreen = bmp_info->bmiColors[i].rgbGreen;
				  pal->palPalEntry[i].peBlue = bmp_info->bmiColors[i].rgbBlue;
			   }
			   h_pal = CreatePalette(pal);
			   free(pal);
			   h_old_pal = SelectPalette(Printer->Canvas->Handle, h_pal, false);
			   is_pal_dev = true;
			}

			// send the bits to the printer
			if (Settings.GraphicHeaderWidth == 0)
			   Settings.GraphicHeaderWidth = (Printer->PageWidth - Settings.GraphicHeaderX);
			if (Settings.GraphicHeaderHeight == 0)
			   Settings.GraphicHeaderHeight = (Printer->PageHeight - Settings.GraphicHeaderY);

			// send the bits to the printer
			StretchDIBits(Printer->Canvas->Handle, Settings.GraphicHeaderX, Settings.GraphicHeaderY, Settings.GraphicHeaderWidth,
			   Settings.GraphicHeaderHeight, 0, 0, bmp_w, bmp_h, bits, bmp_info, DIB_RGB_COLORS, SRCCOPY);
			// clean up
			::DeleteObject(h_mem_bmp);
			if (is_pal_dev)
			{
			   ::SelectObject(Printer->Canvas->Handle, h_old_pal);
			   ::DeleteObject(h_pal);
			}
			::GlobalUnlock(bits);
			::GlobalFree(h_bits);
			::GlobalUnlock(bmp_info);
			::GlobalFree(h_bmp_info);
		 }
	  }
   }

   void WinPrinters::DrawBackGround(TPrinter *Printer, TGraphicPrinterSettings &Settings)
   {
	  if (Settings.GraphicBackGroundFile != "")
	  {
		 Graphics::TBitmap *CanvasBackGround = new Graphics::TBitmap();
		 UnicodeString Path = ExtractFilePath(Application->ExeName) + Settings.GraphicBackGroundFile;
		 if (FileExists(Path))
		 {
			CanvasBackGround->LoadFromFile(Path);
			// create a memory dc for the image
			HDC h_dc = CanvasBackGround->Canvas->Handle;
			int bmp_w = CanvasBackGround->Width;
			int bmp_h = CanvasBackGround->Height;
			// Creaet a compatable Device Context with the canvas.
			HDC h_mem_dc = ::CreateCompatibleDC(h_dc);
			HBITMAP h_mem_bmp = ::CreateCompatibleBitmap(h_dc, bmp_w, bmp_h);
			HBITMAP h_old_bmp = (HBITMAP)::SelectObject(h_mem_dc, h_mem_bmp);

			// fix up bad video drivers
			bool is_pal_dev = false;
			LOGPALETTE *pal;
			HPALETTE h_pal, h_old_pal;

			if (::GetDeviceCaps(CanvasBackGround->Canvas->Handle, RASTERCAPS) & RC_PALETTE)
			{
			   pal = static_cast <LOGPALETTE*> (malloc(sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256)));
			   memset(pal, 0, sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256));
			   pal->palVersion = 0x300;
			   pal->palNumEntries = ::GetSystemPaletteEntries(CanvasBackGround->Canvas->Handle, 0, 256, pal->palPalEntry);
			   if (pal->palNumEntries != 0)
			   {
				  h_pal = ::CreatePalette(pal);
				  h_old_pal = ::SelectPalette(h_mem_dc, h_pal, false);
				  is_pal_dev = true;
			   }
			   else
			   {
				  free(pal);
			   }
			}

			// copy the image on to the memory dc
			::BitBlt(h_mem_dc, 0, 0, bmp_w, bmp_h, h_dc, 0, 0, SRCCOPY);

			if (is_pal_dev)
			{
			   ::SelectPalette(h_mem_dc, h_old_pal, false);
			   ::DeleteObject(h_pal);
			}

			// delete the mem dc
   //			::SelectObject(h_mem_dc, h_old_bmp);
			::DeleteDC(h_mem_dc);

			// get memory for a BITMAPIFO Structure
			HANDLE h_bmp_info = ::GlobalAlloc(GHND, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256));
			BITMAPINFO* bmp_info = static_cast <BITMAPINFO*> (::GlobalLock(h_bmp_info));
			// Set up the structure
			memset(bmp_info, NULL, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 255));
			bmp_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmp_info->bmiHeader.biPlanes = 1;
			bmp_info->bmiHeader.biBitCount = 8;
			bmp_info->bmiHeader.biWidth = bmp_w;
			bmp_info->bmiHeader.biHeight = bmp_h;
			bmp_info->bmiHeader.biCompression = BI_RGB;

			// find out how much memory for the bits
			::GetDIBits(h_dc, h_mem_bmp, 0, bmp_h, NULL, bmp_info, DIB_RGB_COLORS);

			// Allocate memory for the bits
			HANDLE h_bits = GlobalAlloc(GHND, bmp_info->bmiHeader.biSizeImage);
			void *bits = ::GlobalLock(h_bits);

			// this time get the bits
			::GetDIBits(h_dc, h_mem_bmp, 0, bmp_h, bits, bmp_info, DIB_RGB_COLORS);

			// fix up for bad video driver
			if (is_pal_dev)
			{
			   for (int i = 0; i < pal->palNumEntries; i++)
			   {
				  bmp_info->bmiColors[i].rgbRed = pal->palPalEntry[i].peRed;
				  bmp_info->bmiColors[i].rgbGreen = pal->palPalEntry[i].peGreen;
				  bmp_info->bmiColors[i].rgbBlue = pal->palPalEntry[i].peBlue;
			   }
			   free(pal);
			}

			// fix up for print with palette
			is_pal_dev = false;
			if (::GetDeviceCaps(h_dc, RASTERCAPS) & RC_PALETTE)
			{
			   pal = static_cast <LOGPALETTE*> (malloc(sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256)));
			   memset(pal, 0, sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256));
			   pal->palVersion = 0x300;
			   pal->palNumEntries = 256;
			   for (int i = 0; pal->palNumEntries; i++)
			   {
				  pal->palPalEntry[i].peRed = bmp_info->bmiColors[i].rgbRed;
				  pal->palPalEntry[i].peGreen = bmp_info->bmiColors[i].rgbGreen;
				  pal->palPalEntry[i].peBlue = bmp_info->bmiColors[i].rgbBlue;
			   }
			   h_pal = CreatePalette(pal);
			   free(pal);
			   h_old_pal = SelectPalette(Printer->Canvas->Handle, h_pal, false);
			   is_pal_dev = true;
			}

			// send the bits to the printer
			if (Settings.GraphicBackGroundWidth == 0)
			   Settings.GraphicBackGroundWidth = (Printer->PageWidth - Settings.GraphicBackGroundX);
			if (Settings.GraphicBackGroundHeight == 0)
			   Settings.GraphicBackGroundHeight = (Printer->PageHeight - Settings.GraphicBackGroundY);

			StretchDIBits(Printer->Canvas->Handle, Settings.GraphicBackGroundX, Settings.GraphicBackGroundY,
			   Settings.GraphicBackGroundWidth, Settings.GraphicBackGroundHeight, 0, 0, bmp_w, bmp_h, bits, bmp_info,
			   DIB_RGB_COLORS, SRCCOPY);
			// clean up
			::DeleteObject(h_mem_bmp);
			if (is_pal_dev)
			{
			   ::SelectObject(Printer->Canvas->Handle, h_old_pal);
			   ::DeleteObject(h_pal);
			}
			::GlobalUnlock(bits);
			::GlobalFree(h_bits);
			::GlobalUnlock(bmp_info);
			::GlobalFree(h_bmp_info);
		 }
	  }
   }

   // ---------------------------------------------------------------------------
   // Internal functions
   // ---------------------------------------------------------------------------
   bool WinPrinters::fEnumPrintJobs(HANDLE hPrinter, TWinPrintJobs &WinPrintJobs)
   {
	  DWORD cByteNeeded;
	  DWORD nReturned = 0;

	  if (!EnumJobs(hPrinter, 0, 0xFFFFFFFF, 2, NULL, 0, &cByteNeeded, &nReturned))
	  {
      	 __int64 Error  = GetLastError();
		 if (Error != ERROR_INSUFFICIENT_BUFFER)
         {
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Prep For Enum Jobs Print Jobs Failed :  Win Error Code : " + IntToStr(Error));
			return false;
         }
	  }
	  JOB_INFO_2 *pJobStorage = (JOB_INFO_2*)new unsigned char[cByteNeeded];

	  DWORD cByteUsed = 0;

	  bool Success = false;
	  try
	  {
		 ZeroMemory(pJobStorage, cByteNeeded);

		 if (EnumJobs(hPrinter, 0, nReturned, 2, (LPBYTE)pJobStorage, cByteNeeded, &cByteUsed, &nReturned))
		 {
			Success = true;
			for (DWORD i = 0; i < nReturned; i++)
			{
			   FILETIME FileTime1, FileTime2;
			   SystemTimeToFileTime(&pJobStorage[i].Submitted, &FileTime1);
			   FileTimeToLocalFileTime(&FileTime1, &FileTime2);
			   SYSTEMTIME SystemTime;
			   FileTimeToSystemTime(&FileTime2, &SystemTime);

			   TWinPrintJob WinPrintJob;

			   WinPrintJob.DocumentName = pJobStorage[i].pDocument;
			   WinPrintJob.JobID = pJobStorage[i].JobId;
			   WinPrintJob.Submitted = SystemTimeToDateTime(SystemTime);
			   // if (!pJobStorage[i].pStatus)
			   WinPrintJob.Status = pJobStorage[i].Status;
			   // else
			   // WinPrintJob.Status		= Dunno???;

			   WinPrintJobs.push_back(WinPrintJob);
			}
		 }
         else
         {
      		__int64 Error  = GetLastError();
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Enum Jobs Print Jobs Failed : Win Error Code : " + IntToStr((__int64)Error));
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Enum Jobs Print Jobs Failed : Bytes Needed : " + IntToStr((__int64)cByteNeeded));
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Enum Jobs Print Jobs Failed : Bytes Used : " + IntToStr((__int64)cByteUsed));
            TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Enum Jobs Print Jobs Failed : Job Structures Returned : " + IntToStr((__int64)nReturned));
         }
	  }
	  __finally
	  {
		 delete[](unsigned char*)pJobStorage;
	  }
	  return Success;
   }

   // ---------------------------------------------------------------------------
   bool WinPrinters::fPrinterOnline(HANDLE hPrinter, DWORD PrinterStatus)
   {
	  bool Online = true;

	  if (PrinterStatus & (PRINTER_STATUS_BUSY | PRINTER_STATUS_DOOR_OPEN | PRINTER_STATUS_ERROR | PRINTER_STATUS_NO_TONER |
			PRINTER_STATUS_NOT_AVAILABLE | PRINTER_STATUS_OFFLINE | PRINTER_STATUS_OUT_OF_MEMORY | PRINTER_STATUS_OUTPUT_BIN_FULL |
			PRINTER_STATUS_PAGE_PUNT | PRINTER_STATUS_PAPER_JAM | PRINTER_STATUS_PAPER_OUT | PRINTER_STATUS_PAPER_PROBLEM |
			PRINTER_STATUS_PAUSED | PRINTER_STATUS_PENDING_DELETION | PRINTER_STATUS_SERVER_UNKNOWN | PRINTER_STATUS_USER_INTERVENTION))
	  {
		 Online = false;

         if(PrinterStatus & PRINTER_STATUS_BUSY)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_BUSY");
         }
         else if(PrinterStatus & PRINTER_STATUS_DOOR_OPEN)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_DOOR_OPEN");
         }
         else if(PrinterStatus & PRINTER_STATUS_ERROR)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_ERROR");
         }
         else if(PrinterStatus & PRINTER_STATUS_NO_TONER)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_NO_TONER");
         }
         else if(PrinterStatus & PRINTER_STATUS_NOT_AVAILABLE)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_NOT_AVAILABLE");
         }
         else if(PrinterStatus & PRINTER_STATUS_OFFLINE)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_OFFLINE");
         }
         else if(PrinterStatus & PRINTER_STATUS_OUT_OF_MEMORY)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_OUT_OF_MEMORY");
         }
         else if(PrinterStatus & PRINTER_STATUS_OUTPUT_BIN_FULL)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_OUTPUT_BIN_FULL");
         }
         else if(PrinterStatus & PRINTER_STATUS_PAGE_PUNT)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PAGE_PUNT");
         }
         else if(PrinterStatus & PRINTER_STATUS_PAPER_JAM)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PAPER_JAM");
         }
         else if(PrinterStatus & PRINTER_STATUS_PAPER_OUT)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PAPER_OUT");
         }
         else if(PrinterStatus & PRINTER_STATUS_PAPER_PROBLEM)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PAPER_PROBLEM");
         }
         else if(PrinterStatus & PRINTER_STATUS_PAUSED)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PAUSED");
         }
         else if(PrinterStatus & PRINTER_STATUS_PENDING_DELETION)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_PENDING_DELETION");
         }
         else if(PrinterStatus & PRINTER_STATUS_SERVER_UNKNOWN)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_SERVER_UNKNOWN");
         }
         else if(PrinterStatus & PRINTER_STATUS_USER_INTERVENTION)
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped PRINTER_STATUS_USER_INTERVENTION");
         }
         else
         {
             TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped Offline for unknown reason");
         }
	  }
	  else
	  {
		 TWinPrintJobs WinPrintJobs;
		 if (fEnumPrintJobs(hPrinter, WinPrintJobs))
		 {
			for (unsigned i = 0; i < WinPrintJobs.size(); i++)
			{
			   if (WinPrintJobs[i].Status & JOB_STATUS_PRINTING)
			   {
				  if (WinPrintJobs[i].Status & (JOB_STATUS_BLOCKED_DEVQ | JOB_STATUS_ERROR | JOB_STATUS_OFFLINE | JOB_STATUS_PAPEROUT |
						JOB_STATUS_PAUSED | JOB_STATUS_USER_INTERVENTION))
				  {
					 Online = false;
                     if(WinPrintJobs[i].Status & JOB_STATUS_BLOCKED_DEVQ)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_BLOCKED_DEVQ");
                     }
                     else if(WinPrintJobs[i].Status & JOB_STATUS_ERROR)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_ERROR");
                     }
                     else if(WinPrintJobs[i].Status & JOB_STATUS_OFFLINE)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_OFFLINE");
                     }
                     else if(WinPrintJobs[i].Status & JOB_STATUS_PAPEROUT)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_PAPEROUT");
                     }
                     else if(WinPrintJobs[i].Status & JOB_STATUS_PAUSED)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_PAUSED");
                     }
                     else if(WinPrintJobs[i].Status & JOB_STATUS_USER_INTERVENTION)
                     {
                         TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Dropped JOB_STATUS_USER_INTERVENTION");
                     }
                     else
                     {
                        TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Bad Status for unknown reason");
                     }
				  }
			   }
			}
		 }
		 else
		 {
			Online = false;
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Print Job Enumeration Failed");
		 }
	  }
	  return Online;
   }

   // ---------------------------------------------------------------------------
   WinPrinters::TPrintJobStatus WinPrinters::fGetJobStatus(HANDLE hPrinter, int JobID)
   {
	  TPrintJobStatus Result = jsNotFound;

	  DWORD cbNeeded = 0;
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
	  JOB_INFO_2 *pJobStorage = (JOB_INFO_2*)new unsigned char[cbNeeded];
	  try
	  {
		 if (GetJob(hPrinter, JobID, 2, (LPBYTE)pJobStorage, cbNeeded, &cbNeeded))
		 {
			if (pJobStorage->Status & (JOB_STATUS_BLOCKED_DEVQ | JOB_STATUS_ERROR | JOB_STATUS_OFFLINE | JOB_STATUS_PAPEROUT |
				  JOB_STATUS_PAUSED | JOB_STATUS_USER_INTERVENTION))
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
			else // if (pJobStorage->Status & (JOB_STATUS_COMPLETE | JOB_STATUS_PRINTED))
			{
			   Result = jsComplete;
			}
		 }
	  }
	  __finally
	  {
		 delete[]pJobStorage;
	  }
	  return Result;
   }

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   WinPrinters::TPrinterDetails::TPrinterDetails(const UnicodeString &PrinterName) : fHandle(NULL), fStatus(0)
   {
	  PRINTER_INFO_2 *pPrinterInfo = NULL;
	  DWORD cByteNeeded;
	  DWORD cByteUsed;

	  if (OpenPrinterW(PrinterName.w_str(), &fHandle, NULL) && fHandle)
	  {
		 if (!GetPrinter(fHandle, 2, NULL, 0, &cByteNeeded) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		 {
			ClosePrinter(fHandle);
			fHandle = NULL;
		 }
		 else
		 {
			pPrinterInfo = (PRINTER_INFO_2*)new unsigned char[cByteNeeded];
			try
			{
			   ZeroMemory(pPrinterInfo, cByteNeeded);

			   if (GetPrinter(fHandle, 2, (LPSTR)pPrinterInfo, cByteNeeded, &cByteUsed))
			   {
				  fStatus = pPrinterInfo->Status;
			   }
			   else
			   {
				  ClosePrinter(fHandle);
				  fHandle = NULL;
			   }
			}
			__finally
			{
			   delete[](unsigned char*)pPrinterInfo;
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

   // ---------------------------------------------------------------------------
   WinPrinters::TPrinterDetails::~TPrinterDetails()
   {
	  if (fHandle)
	  {
		 ClosePrinter(fHandle);
		 fHandle = NULL;
	  }
   }

   // ---------------------------------------------------------------------------
   bool WinPrinters::TPrinterDetails::GetDetails(HANDLE &Handle, DWORD &Status)
   {
	  if (!fHandle)
	  {
		 return false;
	  }
	  else
	  {
		 Handle = fHandle;
		 Status = fStatus;

		 return true;
	  }
   }

   // ---------------------------------------------------------------------------
   class TJobTrackerThread : public TThread
   {
   public:
	  TJobTrackerThread(const UnicodeString &PrinterName, int JobID, int Timeout, int Interval = 250);

	  TPrintJobStatus Status;
	  bool TimedOut;

   private:
	  void __fastcall Execute();

	  TPrinterDetails PrinterDetails;

	  DWORD fTimeout;
	  int fJobID;
	  int fInterval;
   };

   // ---------------------------------------------------------------------------
   TJobTrackerThread::TJobTrackerThread(const UnicodeString &PrinterName, int JobID, int Timeout, int Interval) : PrinterDetails
	   (PrinterName), fTimeout(Timeout), fJobID(JobID), fInterval(Interval), TThread(true), Status(jsUnknown), TimedOut(false)
   {
   }

   // ---------------------------------------------------------------------------
   void __fastcall TJobTrackerThread::Execute()
   {
	  HANDLE PrinterHandle;
	  DWORD PrinterStatus;

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
					 Sleep(0);
				  }
			   }
			}
		 }
	  }
   }

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   TPrintJobTracker* TPrintJobTracker::Create(const UnicodeString &PrinterName, int JobID, TJobTrackerEvent OnJobStatus, int Timeout)
   {
	  return new TPrintJobTracker(PrinterName, JobID, OnJobStatus, Timeout);
   }

   // ---------------------------------------------------------------------------
   TPrintJobTracker::TPrintJobTracker(const UnicodeString &PrinterName, int JobID, TJobTrackerEvent OnPrintJobStatus, int Timeout,
	  int Interval) : fDeleting(false), JobTrackerThread(NULL), fOnPrintJobStatus(OnPrintJobStatus), fPrinterName(PrinterName),
   fJobID(JobID), fInterval(Interval), fTimeout(Timeout)
   {
	  fCreateThread(fPrinterName, fJobID, fTimeout, fInterval);
   }

   // ---------------------------------------------------------------------------
   void TPrintJobTracker::fCreateThread(const UnicodeString &PrinterName, int JobID, int Timeout, int Interval)
   {
	  JobTrackerThread = new TJobTrackerThread(PrinterName, JobID, Timeout, Interval);

	  JobTrackerThread->FreeOnTerminate = true;
	  JobTrackerThread->OnTerminate = OnThreadTerminate;
	  JobTrackerThread->Resume();
   }

   // ---------------------------------------------------------------------------
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
	  catch(...)
	  {
		 // Don't want any pointless exceptions in destructor!
	  }
   }

   // ---------------------------------------------------------------------------
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

   // ---------------------------------------------------------------------------
   int TPrintJobTracker::JobID()
   {
	  return fJobID;
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrintJobTracker::OnThreadTerminate(TObject* Sender)
   {
	  bool ContinueTracking = false;
	  if (fOnPrintJobStatus && !fDeleting)
	  {
		 fOnPrintJobStatus(this, fPrinterName, fJobID, JobTrackerThread->TimedOut, JobTrackerThread->Status, ContinueTracking);
	  }
	  if (JobTrackerThread->TimedOut && ContinueTracking)
	  {
		 fCreateThread(fPrinterName, fJobID, fTimeout, fInterval);
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

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   TPrintJobsTracker::TPrintJobsTracker(TJobTrackerEvent OnPrintJobStatus) : fOnPrintJobStatus(OnPrintJobStatus)
   {
   }

   // ---------------------------------------------------------------------------
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

   // ---------------------------------------------------------------------------
   void TPrintJobsTracker::StartTrackingJob(const UnicodeString &PrinterName, int JobID, int Timeout)
   {
	  TPrintJobTracker *PrintJobTracker = TPrintJobTracker::Create(PrinterName, JobID, fInternalOnStatus, Timeout);
	  fJobs.insert(PrintJobTracker);
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrintJobsTracker::fInternalOnStatus(TPrintJobTracker *Sender, const UnicodeString &PrinterName, int JobID,
	  bool TimedOut, TPrintJobStatus Status, bool &ContinueTracking)
   {
	  if (fOnPrintJobStatus)
	  {
		 try
		 {
			fOnPrintJobStatus(Sender, PrinterName, JobID, TimedOut, Status, ContinueTracking);
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

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   class TPrinterMonitorThread : public TThread
   {
   public:
	  TPrinterMonitorThread(std::set <UnicodeString> &OnlinePrinters, std::set <UnicodeString> &OfflinePrinters,
		 TPrinterOnlineEvent PrinterOfflineEvent, int Interval = 250);

   private:
	  void __fastcall Execute();

	  int fInterval;

	  std::set <UnicodeString> fOnlinePrinters;
	  std::set <UnicodeString> fOfflinePrinters;

	  TPrinterOnlineEvent fPrinterOnlineEvent;
	  UnicodeString fCurrentPrinter;

	  void __fastcall CallOfflineEvent();
	  void __fastcall CallOnlineEvent();
   };

   // ---------------------------------------------------------------------------
   TPrinterMonitorThread::TPrinterMonitorThread(std::set <UnicodeString> &OnlinePrinters, std::set <UnicodeString> &OfflinePrinters,
	  TPrinterOnlineEvent PrinterOnlineEvent, int Interval) : fInterval(Interval), fOnlinePrinters(OnlinePrinters),
   fOfflinePrinters(OfflinePrinters), fPrinterOnlineEvent(PrinterOnlineEvent), TThread(true)
   {
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrinterMonitorThread::Execute()
   {
	  const UnicodeString PrintData("\0\0", 1);
	  while (!Terminated)
	  {
		 // Check if any online printers are still online
		 {
			std::set <UnicodeString> ::iterator iOnlinePrinters = fOnlinePrinters.begin();

			while (!Terminated && iOnlinePrinters != fOnlinePrinters.end())
			{
			   fCurrentPrinter = *iOnlinePrinters;
			   ++iOnlinePrinters; // Do now as this printer could be moved

			   int JobID;
			   if (!PrintRaw(fCurrentPrinter, "PrinterTest", PrintData, JobID, false))
			   {
				  Synchronize(CallOfflineEvent);
			   }
			   Sleep(0);
			}
		 }
		 // Check if any offline printers are back online
		 std::set <UnicodeString> ::iterator iOfflinePrinters = fOfflinePrinters.begin();

		 while (!Terminated && iOfflinePrinters != fOfflinePrinters.end())
		 {
			fCurrentPrinter = *iOfflinePrinters;
			++iOfflinePrinters; // Do now as this printer could be moved

			int JobID;
			if (PrintRaw(fCurrentPrinter, "PrinterTest", PrintData, JobID, false))
			{
			   Sleep(min(1000, fInterval));
			   TPrintJobStatus Status = GetPrintJobStatus(fCurrentPrinter, JobID);
			   if (Status != jsError)
			   {
				  Synchronize(CallOnlineEvent);
			   }
			}
			Sleep(0);
		 }
		 // Wait a bit before going thru again
		 unsigned long TickCount = GetTickCount();
		 while (!Terminated && GetTickCount() < (TickCount + fInterval))
		 {
			Sleep(0);
		 }
	  }
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrinterMonitorThread::CallOfflineEvent()
   {
	  if (fPrinterOnlineEvent)
	  {
		 fPrinterOnlineEvent(fCurrentPrinter, false);
		 fOnlinePrinters.erase(fCurrentPrinter);
		 fOfflinePrinters.insert(fCurrentPrinter);
	  }
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrinterMonitorThread::CallOnlineEvent()
   {
	  if (fPrinterOnlineEvent)
	  {
		 fPrinterOnlineEvent(fCurrentPrinter, true);
		 fOfflinePrinters.erase(fCurrentPrinter);
		 fOnlinePrinters.insert(fCurrentPrinter);
	  }
   }

   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   // ---------------------------------------------------------------------------
   TPrinterMonitor::TPrinterMonitor(TPrinterOnlineEvent OnPrinterOnlineChange, int Interval) : fOnPrinterOnlineChange
	   (OnPrinterOnlineChange), fInterval(Interval), fStarted(false), fPrinterMonitorThread(NULL)
   {
   }

   // ---------------------------------------------------------------------------
   TPrinterMonitor::~TPrinterMonitor()
   {
	  fKillThread();
   }

   // ---------------------------------------------------------------------------
   bool TPrinterMonitor::Start()
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

   // ---------------------------------------------------------------------------
   void TPrinterMonitor::Stop()
   {
	  fKillThread();
	  fStarted = false;
   }

   // ---------------------------------------------------------------------------
   void TPrinterMonitor::fKillThread()
   {
	  if (fPrinterMonitorThread)
	  {
		 fPrinterMonitorThread->Terminate();

		 unsigned long TickCount = GetTickCount();
		 while (fPrinterMonitorThread && GetTickCount() < (TickCount + 10000))
		 {
			Application->ProcessMessages();
			Sleep(0);
		 }
		 if (fPrinterMonitorThread)
		 {
			TerminateThread((HANDLE)fPrinterMonitorThread->Handle, 0);
		 }
	  }
   }

   // ---------------------------------------------------------------------------
   void TPrinterMonitor::AddPrinter(const UnicodeString &PrinterName)
   {
	  bool Running = fStarted;
	  Stop();
	  fOnlinePrinters.insert(PrinterName);
	  if (Running)
		 Start();
   }

   // ---------------------------------------------------------------------------
   void TPrinterMonitor::fCreateThread()
   {
	  if (!fPrinterMonitorThread)
	  {
		 fPrinterMonitorThread = new TPrinterMonitorThread(fOnlinePrinters, fOfflinePrinters, fOnPrinterOnlineChange, fInterval);

		 fPrinterMonitorThread->FreeOnTerminate = true;
		 fPrinterMonitorThread->OnTerminate = fOnThreadTerminate;
		 fPrinterMonitorThread->Resume();
	  }
   }

   // ---------------------------------------------------------------------------
   void __fastcall TPrinterMonitor::fOnThreadTerminate(TObject* Sender)
   {
	  fPrinterMonitorThread = NULL;
   }
   // ---------------------------------------------------------------------------
} // namespace WinPrinters
// ---------------------------------------------------------------------------
