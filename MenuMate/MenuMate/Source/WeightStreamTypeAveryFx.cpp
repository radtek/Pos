// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WeightStreamTypeAveryFx.h"
#include "Stream.h"
#include <vector>
#include <iterator>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

std::vector<double> TWeightStreamTypeAveryFx::RecentWeightsList;

TWeightStreamTypeAveryFx::TWeightStreamTypeAveryFx(unsigned char *Buffer, int BufferSize) : Stream(new TMemoryStream)
{
    Stream->Write(Buffer, BufferSize);
    Stream->Position = 0;
}

TWeightStreamTypeAveryFx* TWeightStreamTypeAveryFx::Create(unsigned char *Buffer, int BufferSize)
{
    return new TWeightStreamTypeAveryFx(Buffer, BufferSize);
}

void TWeightStreamTypeAveryFx::GetWeight(TWeight &Weight)
{
    Stream->Position = 0;
    UnicodeString UnformattedWeightStr = "";
    StreamRead(Stream.get(), UnformattedWeightStr);

    /* using LastDelimiter here because it is the place we have the most recent reading of the weights */
    int lastEndOfTextPosition = UnformattedWeightStr.LastDelimiter("\x03");
    int weightStringLength = 7;
    int lastWeightStringStartIndex = lastEndOfTextPosition - weightStringLength;

    UnicodeString WeightStr = UnformattedWeightStr.SubString(lastWeightStringStartIndex,weightStringLength);
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
        Weight.SetWeightIn_g(dblWeight);
    }
}

void TWeightStreamTypeAveryFx::GetStable(bool &Stable)
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
