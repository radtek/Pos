//---------------------------------------------------------------------------
#include "Stream.h"
#include "System.hpp"

void StreamRead(TStream *Stream,UnicodeString &Value)
{
	byte input = NULL;
	RawByteString Data = "";
	do
	{
	  Stream->Read(&input,sizeof(input));
	  if(input != NULL)
	  {
		 Data += (char)(input);
	  }
	}while(input != NULL && Stream->Position < Stream->Size);
	Value = UTF8ToString(Data);
}

void StreamRead(TStream *Stream,AnsiString &Value)
{
	byte input = NULL;
	RawByteString Data = "";
	do
	{
	  Stream->Read(&input,sizeof(input));
	  if(input != NULL)
	  {
		 Data += (char)(input);
	  }
	}while(input != NULL && Stream->Position < Stream->Size);

	Value = Utf8ToAnsi(Data);
}


void StreamRead(TStream *Stream,TStringList *Strings)
{
	Strings->Clear();
	int NumberOfLines = 0;
	StreamRead(Stream,NumberOfLines);
	for (int i = 0; i < NumberOfLines ; i++)
	{
		UnicodeString MyString = "";
		StreamRead(Stream,MyString);
		Strings->Add(MyString);
	}
}

void StreamRead(TStream *Stream,std::set<byte> &Data)
{
   Data.empty();
   byte NumberOfBytes = 0;
   StreamRead(Stream,NumberOfBytes);
	for (int i = 0; i < NumberOfBytes ; i++)
	{
      byte Dat = 0;
      StreamRead(Stream,Dat);
		Data.insert(Dat);
   }
}

void StreamRead(TStream *Stream,TPointsRulesSet &Data)
{
   int PointsRules;
   StreamRead(Stream,PointsRules);
   TPointsRulesSetUtils().Expand(PointsRules,Data);
}

void StreamRead(TStream *Stream,int &Value)
{
   int Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}
void StreamRead(TStream *Stream,long &Value)
{
   long Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}
void StreamRead(TStream *Stream,unsigned long &Value)
{
   unsigned long Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}
void StreamRead(TStream *Stream,bool &Value)
{
   bool Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}
void StreamRead(TStream *Stream,char &Value)
{
	char Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}

void StreamRead(TStream *Stream,unsigned char &Value)
{
   unsigned char Temp = 0;
   Stream->Read(&Temp, sizeof(Temp));
   Value = Temp;
}
void StreamRead(TStream *Stream,TDateTime &Value)
{
   double DateTime;
   Stream->Read(&DateTime, sizeof(DateTime));
   Value = DateTime;
}

void StreamRead(TStream *Stream,Currency &Value)
{
   Currency Amount;
	Stream->Read(&Amount, sizeof(Amount));
   Value = Amount;
}
void StreamRead(TStream *Stream,double &Value)
{
   double Temp;
   Stream->Read(&Temp,sizeof(Temp));
   Value = Temp;
}

void StreamRead(TStream *Stream,unsigned short &Value)
{
   unsigned short Temp;
   Stream->Read(&Temp,sizeof(Temp));
   Value = Temp;
}

//---------------------------------------------------------------------------
void StreamWrite(TStream *Stream,UnicodeString &Value)
{
	char Nully = NULL;
	RawByteString Data = UTF8Encode(Value);
	Stream->Write(Data.c_str(),Data.Length());
	Stream->Write(&Nully,sizeof(Nully));
}

void StreamWrite(TStream *Stream,AnsiString &Value)
{
	char Nully = NULL;
	RawByteString Data = AnsiToUtf8(Value);
	Stream->Write(Data.c_str(),Data.Length());
	Stream->Write(&Nully,sizeof(Nully));
}

void StreamWrite(TStream *Stream,Currency &Value)
{
   Stream->Write(&Value, sizeof(Value));
}
void StreamWrite(TStream *Stream,int &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,long &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,unsigned long &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,bool &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,char &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,unsigned char &Value)
{
   Stream->Write(&Value, sizeof(Value));
}
void StreamWrite(TStream *Stream,TDateTime &Value)
{
   double DateTime = Value;
   Stream->Write(&DateTime, sizeof(DateTime));
}

void StreamWrite(TStream *Stream,double &Value)
{
   Stream->Write(&Value,sizeof(Value));
}
void StreamWrite(TStream *Stream,unsigned short &Value)
{
   Stream->Write(&Value,sizeof(Value));
}

void StreamWrite(TStream *Stream,TStringList *Strings)
{
   int NumberOfLines = Strings->Count;
   StreamWrite(Stream,NumberOfLines);
   for (int i = 0; i < Strings->Count ; i++)
   {
      UnicodeString Data = Strings->Strings[i];
		StreamWrite(Stream,Data);
   }
}

void StreamWrite(TStream *Stream,std::set<byte> &Data)
{
   byte NumberOfBytes = Data.size();
   StreamWrite(Stream,NumberOfBytes);
   for(std::set<byte>::iterator itData = Data.begin(); itData != Data.end(); std::advance(itData,1) )
   {
		StreamWrite(Stream,byte(*itData));
   }
}

void StreamWrite(TStream *Stream,TPointsRulesSet &Data)
{
   int RulesSet = TPointsRulesSetUtils().Compress(Data);
   StreamWrite(Stream,RulesSet);
}

