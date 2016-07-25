//---------------------------------------------------------------------------


#pragma hdrstop

#include "WeightStreamQUQA.h"
#include "Stream.h"
#include <vector>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TWeightStreamQUQA::TWeightStreamQUQA(unsigned char *Buffer, int BufferSize) : Stream (new TMemoryStream)
{
   Stream->Write(Buffer,BufferSize);
   Stream->Position = 0;
}

TWeightStreamQUQA* TWeightStreamQUQA::Create(unsigned char *Buffer, int BufferSize)
{
	return new TWeightStreamQUQA(Buffer,BufferSize);
}

void TWeightStreamQUQA::GetWeight(TWeight &Weight)
{
   Stream->Position = 12;
   unsigned char Data;
   StreamRead(Stream.get(),Data);
   if(Data != 0x02)
   {
      throw TWSCException(twscPacketFormatError,"No STX in GetWeight");
   }
   StreamRead(Stream.get(),Data);

   if(Data != 'S' && Data != 'U')
   {
      throw TWSCException(twscDataError,"Unknown Stability Type S/U");
   }

   std::vector<BYTE> WeightBuffer(7,'0');
   Stream->Read(&WeightBuffer[0], WeightBuffer.size());

   AnsiString WeightStr = AnsiString((const char *)(&WeightBuffer[0]),(unsigned int)(WeightBuffer.size()));

   std::vector<BYTE> WeightUnit(2,' ');
   Stream->Read(&WeightUnit[0], WeightUnit.size());

   AnsiString WeightUnitStr = AnsiString((const char *)&WeightUnit[0],WeightUnit.size());

   StreamRead(Stream.get(),Data); // BCC.
   StreamRead(Stream.get(),Data);
   if(Data != 0x03)
   {
      throw TWSCException(twscPacketFormatError,"No ETX");
   }

   double dblWeight = StrToFloatDef(WeightStr,-1);
   if(dblWeight == -1)
   {
      Weight.SetWeight_Invalid(dblWeight);
      //throw TWSCException(twscDataError,AnsiString("Unable to Convert Weight " + WeightStr));
   }
   else
   {
      if(WeightUnitStr.UpperCase() == " G")
      {
         Weight.SetWeightIn_g(dblWeight);
      }
      else if(WeightUnitStr.UpperCase() == "KG")
      {
         Weight.SetWeightIn_Kg(dblWeight);
      }
      else if(WeightUnitStr.UpperCase() == "LB")
      {
         Weight.SetWeightIn_lb(dblWeight);
      }
      else
      {
         Weight.SetWeight_Invalid(dblWeight);
         //throw TWSCException(twscDataError,AnsiString("Unknown Weight Type " + WeightUnitStr));
      }
   }
}

void TWeightStreamQUQA::GetStable(bool &Stable)
{
   Stable = false;
   Stream->Position = 12;
   BYTE Data;
   StreamRead(Stream.get(),Data);
   if(Data != 0x02)
   {
      throw TWSCException(twscPacketFormatError,"No STX  in GetStable");
   }
   
   StreamRead(Stream.get(),Data);
/* if(Data != 'S' && Data != 'U')
   {
      throw TWSCException(twscDataError,"Unknown Stability Type S/U");
   }*/

   if(Data == 'S')
   {
      Stable = true;
   }
   else
   {
      Stable = false;
   }
}


