//---------------------------------------------------------------------------


#pragma hdrstop

#include "DocketLogs.h"
#include "MMLogging.h"
#include "ItemComplete.h"
#include <Forms.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)

void TDocketLogs::SaveLogs(std::auto_ptr<TStringList> List)
{
    try
    {
        UnicodeString fileName = GetFileName();
        std::auto_ptr<TStringList> oldList(new TStringList);
        if (FileExists(fileName) )
          oldList->LoadFromFile(fileName);
        for(int index = 0; index < List->Count; index++)
        {
            AnsiString value = List->operator [](index);
            oldList->Add(value);
        }
        oldList->Add("===============================================================================================");
        oldList->SaveToFile(fileName );
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
}
//---------------------------------------------------------------------------
UnicodeString TDocketLogs::GetFileName()
{
    UnicodeString directoryName = "";
    UnicodeString fileName = "";
    try
    {
        directoryName = ExtractFilePath(Application->ExeName) + "/Logs";
        if (!DirectoryExists(directoryName))
            CreateDir(directoryName);
        directoryName = directoryName + "/DocketLogs";
        if (!DirectoryExists(directoryName))
            CreateDir(directoryName);
        AnsiString name = "DocketLogs " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
        fileName = directoryName + "/" + name;
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return fileName;
}
//---------------------------------------------------------------------------
std::auto_ptr<TStringList> TDocketLogs::AddItemsToList(TList *Orders)
{
    std::auto_ptr<TStringList> OrderList(new TStringList);
    try
    {
        OrderList->Add("Time :- " + Now().FormatString("hh:mm:ss tt"));
        for(int index = 0; index < Orders->Count; index++)
        {
            TItemComplete* ic = (TItemComplete*)Orders->Items[index];
            UnicodeString value = "";
            value += "ItemName: " + ic->Item + "        " + "Menu Name: " + ic->MenuName + "        "  +
                     "Course Kitchen Name: " + ic->CourseKitchenName + "        " +
                     "Course Name :"+ ic->Course + "        " +
                     "QTY: " + ic->GetQty() + "        " + "Table No: " + ic->TableNo + "        " +
                     "Seat No: " + ic->SeatNo + "        " "Tab Name: " + ic->TabName + "        " +
                     "Tab Type: " + ic->TabType;
            OrderList->Add(value);
            for(int indexSub = 0; indexSub < ic->SubOrders->Count; indexSub++)
            {
               TItemComplete* sub = (TItemComplete*)ic->SubOrders->Items[indexSub];
                UnicodeString valueSub = "";
                valueSub += "ItemName: " + sub->Item + "        " + "Menu Name: " + sub->MenuName + "        "  +
                         "Course Kitchen Name: " + sub->CourseKitchenName + "        " +
                         "Course Name :"+ sub->Course + "        " +
                         "QTY: " + sub->GetQty() + "        " + "Table No: " + sub->TableNo + "        " +
                         "Seat No: " + sub->SeatNo + "        " "Tab Name: " + sub->TabName + "        " +
                         "Tab Type: " + sub->TabType;
                OrderList->Add(valueSub);
            }
        }
    }
    catch(Exception &Ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Ex.Message);
    }
    return OrderList;
}
//---------------------------------------------------------------------------
