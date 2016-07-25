//---------------------------------------------------------------------------


#pragma hdrstop

#include "PrintJob.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//------------------------------------------------------------------------------

TPrintJob::TPrintJob(PrintJobType inJobType ,TDeviceReal *Device, TConnectPrinter *Printer)
{
   JobType = inJobType;
   Header.Device = Device;
   Header.Connection = Printer;
   JobStream = new TMemoryStream;
   if(Device)
   {
   	JobReceivedFrom = Device->ID.Type;
   }
   else
   {
		JobReceivedFrom = devPC;
   }
   JobInProgress = false;
   ContainsData = false;
   DataAdded = false;
   PreFormatted = false;
}

//------------------------------------------------------------------------------

TPrintJob::~TPrintJob()
{
   delete JobStream;
}

//------------------------------------------------------------------------------

void TPrintJob::Open()
{
   unsigned char Open = pjOpen;
   if(!JobInProgress)
   {
      JobStream->Clear();
      JobInProgress = true;
   }
   JobStream->Write(&Open,1);//(const void *Buffer, int Count);
}

void TPrintJob::KickDraw()
{
   unsigned char Kick = pjKickDraw;
   JobStream->Write(&Kick,1);//(const void *Buffer, int Count);
}



//------------------------------------------------------------------------------

void TPrintJob::Close()
{
   unsigned char Close = pjClose;
   JobStream->Write(&Close,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
/*void TPrintJob::Add(UnicodeString Data)
{
   unsigned char Add = pjAdd;
   unsigned char Length = (unsigned char)Data.Length();
   JobStream->Write(&Add,1);//(const void *Buffer, int Count);
   JobStream->Write(&Length,1);// The Length of the String.
   JobStream->Write(Data.c_str(),Data.Length());//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::AddLines(UnicodeString Data)
{
   unsigned char AddLines = pjAddLines;
   unsigned char Length = (unsigned char)Data.Length();
   JobStream->Write(&AddLines,1);//(const void *Buffer, int Count);
   JobStream->Write(&Length,1);// The Length of the String.
   JobStream->Write(Data.c_str(),Data.Length());
}
//------------------------------------------------------------------------------
void TPrintJob::AddRightJustify(UnicodeString Col1, UnicodeString Col2)
{
   unsigned char AddRightJustify = pjAddRightJustify;
   int Length = Col1.Length();
   JobStream->Write(&AddRightJustify,1);//(const void *Buffer, int Count);
   JobStream->Write(&Length,sizeof(Length));// The Length of the String.
   JobStream->Write(Col1.c_str(),Col1.Length());
   Length = Col2.Length();
   JobStream->Write(&Length,sizeof(Length));// The Length of the String.
   JobStream->Write(Col2.c_str(),Col2.Length());
}

//------------------------------------------------------------------------------

void TPrintJob::AddCentered(UnicodeString Col1)
{
	unsigned char AddCentered = pjAddCenter;
   unsigned char Length = (unsigned char)Col1.Length();
   JobStream->Write(&AddCentered,1);//(const void *Buffer, int Count);
   JobStream->Write(&Length,1);// The Length of the String.
   JobStream->Write(Col1.c_str(),Col1.Length());
}

//------------------------------------------------------------------------------
void TPrintJob::AddLine(UnicodeString Data)
{
   unsigned char AddLine = pjAddLine;
   unsigned char Length = (unsigned char)Data.Length();
   JobStream->Write(&AddLine,1);//(const void *Buffer, int Count);
   JobStream->Write(&Length,1);// The Length of the String.
   JobStream->Write(Data.c_str(),Data.Length());
}
//------------------------------------------------------------------------------
void TPrintJob::Tab()
{
   unsigned char Tab = pjTab;
   JobStream->Write(&Tab,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::Cut(bool Full)
{
   unsigned char Cut = pjCutPartial;
   if(Full)
   {
      Cut = pjCutFull;
   }
   else
   {
      Cut = pjCutPartial;
   }
   JobStream->Write(&Cut,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::DrawLine()
{
   unsigned char DrawLine = pjDrawLine;
   JobStream->Write(&DrawLine,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::Reverse(unsigned char Lines)
{
   unsigned char Reverse = pjReverseFeed;
   JobStream->Write(&Reverse,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::NewLine()
{
   unsigned char NewLine = pjNewLine;
   JobStream->Write(&NewLine,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::Reset()
{
   unsigned char Reset = pjReset;
   JobStream->Write(&Reset,1);//(const void *Buffer, int Count);
}
//------------------------------------------------------------------------------
void TPrintJob::Font(bool inLarge,bool inBold,bool inUnderLined, bool inRed)
{
	//FIXUP	Can't print in bold on a windows printer currently.
	inBold = false;
   unsigned char Result = 0x00;
   unsigned char Font = pjTxtFont;
   if(inLarge)
   {
     Result |= pjfntLarge;
   }
   if(inBold)
   {
      Result |= pjfntBold;
   }
   if(inUnderLined)
   {
      Result |= pjfntUnderLined;
   }
   if(inRed)
   {
      Result |= pjfntRed;
   }
   JobStream->Write(&Font,1);//(const void *Buffer, int Count);
   JobStream->Write(&Result,1);//(const void *Buffer, int Count);
}
// Printer Specifics.

void TPrintJob::Clear()
{
   JobStream->Clear();
   JobInProgress = false;
   ContainsData = false;
   DataAdded = false;
}


void TMMPrinter::ProcessWindowsPrinter()
{

}
*/
