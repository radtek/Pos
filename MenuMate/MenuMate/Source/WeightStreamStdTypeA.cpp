// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WeightStreamStdTypeA.h"
#include "Stream.h"
#include "DeviceRealTerminal.h"
#include <vector>
#include <iterator>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

std::vector<double> TWeightStreamStdTypeA::RecentWeightsList;

TWeightStreamStdTypeA::TWeightStreamStdTypeA(unsigned char *Buffer, int BufferSize) : Stream(new TMemoryStream)
{
    Stream->Write(Buffer, BufferSize);
    Stream->Position = 0;
}

TWeightStreamStdTypeA* TWeightStreamStdTypeA::Create(unsigned char *Buffer, int BufferSize)
{
    return new TWeightStreamStdTypeA(Buffer, BufferSize);
}

void TWeightStreamStdTypeA::GetWeight(TWeight &Weight)
{
    Stream->Position = 0;
    UnicodeString UnformattedWeightStr = "";
    StreamRead(Stream.get(), UnformattedWeightStr);


    TStringList* logList = new TStringList();
    if(FileExists(ExtractFilePath(Application->ExeName)+ "WeightLog.txt"))
    {
        logList->LoadFromFile(ExtractFilePath(Application->ExeName)+ "WeightLog.txt");
    }
    logList->Add(UnformattedWeightStr);
    logList->SaveToFile(ExtractFilePath(Application->ExeName)+ "WeightLog.txt");
    delete logList;

    UnicodeString WeightStr = UnformattedWeightStr.SubString(UnformattedWeightStr.Pos("\r")+1,7);
    double dblWeight = StrToFloatDef(WeightStr.Trim(), -1);
    if (dblWeight == -1)
    {
		Weight.SetWeight_Invalid(dblWeight);
    }
    else
    {
        RecentWeightsList.insert(RecentWeightsList.begin(), dblWeight);
        if (RecentWeightsList.size() > 3)
        {
            RecentWeightsList.pop_back();
        }
        Weight.SetWeightIn_Kg(dblWeight);
    }
}

void TWeightStreamStdTypeA::GetStable(bool &Stable)
{
    Stable = false;
    if (RecentWeightsList.size() >= 2)
    {
        double MatchVaule = RecentWeightsList[0];
        boolean AllMatch = true;
        for (std::vector<double>::iterator ptrQue = RecentWeightsList.begin(); ptrQue != RecentWeightsList.end(); ptrQue++)
        {
        	if(MatchVaule != *ptrQue)
            {
            	AllMatch = false;
            }
        }
        Stable = AllMatch;
    }
}

UnicodeString TWeightStreamStdTypeA::GetFormattedString(UnicodeString unFormattedString)
{
   TStringList* logList = new TStringList();
   if(FileExists(ExtractFilePath(Application->ExeName)+ "WeightLog.txt"))
    {
        logList->LoadFromFile(ExtractFilePath(Application->ExeName)+ "WeightLog.txt");
    }
   logList->Add(unFormattedString);
   logList->SaveToFile(ExtractFilePath(Application->ExeName)+ "WeightLog.txt");
   delete logList;

   UnicodeString orgString = unFormattedString;
   UnicodeString WeightStr = "-1";
   //char* charArray = orgString.t_str();
   int pos = unFormattedString.Pos("\r");
   /*for(int i = 0; i < unFormattedString.Length() ; i++)
   {
      if(charArray[i] == '\r')
      {
        pos = i;
        break;
      }
   }*/
   if(pos + 3 != unFormattedString.Length())
      WeightStr = unFormattedString.SubString(pos + 3, 7);
   return WeightStr;
}
