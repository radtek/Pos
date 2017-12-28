//---------------------------------------------------------------------------


#pragma hdrstop

#include "FiscalDataUtility.h"
#include "MMMessageBox.h"
#include "CrcTable.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::CreateVersionRequest()
{
    UnicodeString retValue = "";
    retValue = "ver 0000/r/n";
    return retValue;
}
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::CreatePostRequest(PostType type)
{
    UnicodeString retValue = "";
    return retValue;
}
//---------------------------------------------------------------------------
bool TFiscalDataUtility::AnalyzeResponse(AnsiString inData, PostType type)
{
    bool retValue = false;
    switch(type)
    {
        case eFiscalVerNumber:
        {
            if(inData.Pos("0") == 1)
                retValue = true;
            break;
        }
        case eFiscalNormalReceipt:
        case eFiscalCopyReceipt:
        case eFiscalPracticeReceipt:
        case eFiscalProofReceipt:
        {
            retValue = AnalyzePostResponse(inData, type);
            break;
        }
        default:
            break;
    }
    return retValue;
}
//---------------------------------------------------------------------------
bool TFiscalDataUtility::AnalyzePostResponse(AnsiString inData, PostType type)
{
    bool retValue = false;
    try
    {
        inData = inData.Trim();
        AnsiString value = inData.SubString(0,1);
        PostType postType;
        int data = atoi(value.c_str());
        if(value == "0")
            retValue = true;
        else
        {
            AnsiString errorString = "";
            value = inData.SubString(0,2);
            bool checkFurther = true;
            errorString = "Error in sending data to POSPlus.\r";
            if(checkFurther && value.Pos("-3") != 0)
            {
                errorString += "Unknown command error";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("-2") != 0)
            {
                errorString += "CRC error.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("-1") != 0)
            {
                errorString += "Wrong length.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("1 ") != 0)
            {
                errorString += "Wrong number of arguments.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("2 ") != 0)
            {
                errorString += "Wrong date/time range.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("3 ") != 0)
            {
                errorString += "Wrong format of organisation number.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("4 ") != 0)
            {
                errorString += "Wrong format of cash register id (POS Id).";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("5 ") != 0)
            {
                errorString += "Wrong format of serial number (Receipt Number).";
                checkFurther = false;
            }
            if(checkFurther && (value.Pos("6 ") != 0 || value.Pos("7 ") != 0))
            {
                errorString += "Type of receipt not defined.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("8 ") != 0)
            {
                errorString += "Wrong format of return amount.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("9 ") != 0)
            {
                errorString += "Wrong format of sales amount.";
                checkFurther = false;
            }
            if(checkFurther && (value.Pos("10") != 0 || value.Pos("11") != 0 || value.Pos("12") != 0 || value.Pos("13") != 0))
            {
                errorString += "Wrong format of vat.";
                checkFurther = false;
            }
            if(checkFurther && (value.Pos("15") != 0 || value.Pos("16") != 0 || value.Pos("17") != 0 || value.Pos("18") != 0 || value.Pos("19") != 0))
            {
                errorString += "Internal error in the control unit.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("20") != 0)
            {
                errorString += "Power fail abort.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("21") != 0)
            {
                errorString += "Relationship between sales amount and return amount is wrong.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("22") != 0)
            {
                errorString += "Field is present after field CRC.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("23") != 0)
            {
                errorString += "Error in an internal counter.";
                checkFurther = false;
            }
            if(checkFurther && value.Pos("24") != 0)
            {
                errorString += "Internal log is full.";
                checkFurther = false;
            }
            MessageBox(errorString,"Error",MB_OK + MB_ICONERROR);
        }
    }
    catch(Exception &Exc)
    {
     TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::GetPOSPlusData(TPaymentTransaction &paymentTransaction)
{
    AnsiString fiscalData = "";
    fiscalData = PrepareDataForPOSPlus(paymentTransaction);
    return fiscalData;
}
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::PrepareDataForPOSPlus(TPaymentTransaction &paymentTransaction)
{
    AnsiString retValue = "";
    try
    {
        AnsiString dateTime = Now().CurrentDateTime().FormatString("YYYYMMDDhhmm");
        double amountValue = paymentTransaction.Money.PaymentAmount;
        amountValue = RoundTo(amountValue,-2);
        AnsiString amountStr = amountValue;
        amountStr = StringReplace(amountStr,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
        if(amountStr.Pos(",") == 0)
            amountStr += ",00";

        std::auto_ptr<TFiscalData> fiscalData(new TFiscalData());
        fiscalData->FiscalDateTime = dateTime; // Call code for DateTime as string
        fiscalData->FiscalRegisterID = "1234567890";
        //"1234567890"; // Register ID
        if(paymentTransaction.CreditTransaction)
        {
//            fiscalData->FiscalSerialNumber = paymentTransaction.RefundRefReceipt;
            fiscalData->FiscalSerialNumber = GetInvoiceNumber(paymentTransaction);
        }
        else
            fiscalData->FiscalSerialNumber = GetInvoiceNumber(paymentTransaction);
        fiscalData->FiscalReceiptType = "Normal";
        fiscalData->FiscalAmount = amountStr;
        fiscalData->FiscalVatRate1 = "";
        fiscalData->FiscalVat1 = "";
        fiscalData->FiscalVatRate2 = "";
        fiscalData->FiscalVat2 = "";
        fiscalData->FiscalVatRate3 = "";
        fiscalData->FiscalVat3 = "";
        fiscalData->FiscalVatRate4 = "";
        fiscalData->FiscalVat4 = "";
        ExtractVatDetails(paymentTransaction, fiscalData);
        fiscalData->FiscalCRCValue = "0x000";
        fiscalData->FIscalResponse = "";
        fiscalData->IsPostedToPosPlus = "F";
        retValue = ConvertDataToString(fiscalData, paymentTransaction.CreditTransaction);
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
    }
    return retValue;
}
////---------------------------------------------------------------------------
//AnsiString TFiscalDataUtility::ConvertDataToString(std::auto_ptr<TFiscalData> &fiscalData, TPaymentTransaction &paymentTransaction)
//{
//    AnsiString retValue = "";
//    bool isRefundReceipt = paymentTransaction.CreditTransaction;
//    try
//    {
//        retValue = ConvertDataToString(fiscalData, isRefundReceipt);
//    }
//    catch(Exception &Exc)
//    {
//        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
//    }
//    return retValue;
//}
//--------------------------------------------------------------------------
AnsiString TFiscalDataUtility::ConvertDataToString(std::auto_ptr<TFiscalData> fiscalData, bool isRefundReceipt)
{
    AnsiString retValue = "";
    try
    {
      retValue += "kd ";
      retValue += fiscalData->FiscalDateTime + " ";
      retValue += fiscalData->FiscalRegisterID + " ";
      retValue += TDeviceRealTerminal::Instance().ID.ComputerName + " ";
      retValue += fiscalData->FiscalSerialNumber + " ";
      retValue += fiscalData->FiscalReceiptType + " ";
      if(isRefundReceipt)
      {
          retValue += fiscalData->FiscalAmount + " ";
          if(fiscalData->FiscalAmount.Pos(",") == 0)
              retValue += ",00";
          if(fiscalData->FiscalAmount.Pos(",") != 0 &&
            (fiscalData->FiscalAmount.Length() - fiscalData->FiscalAmount.Pos(",")) == 1)
          {
              retValue += "0";
          }
          retValue += "0,00 ";
      }
      else
      {
          retValue += "0,00 ";
          retValue += fiscalData->FiscalAmount + " ";
          if(fiscalData->FiscalAmount.Pos(",") == 0)
              retValue += ",00";
          if(fiscalData->FiscalAmount.Pos(",") != 0 &&
            (fiscalData->FiscalAmount.Length() - fiscalData->FiscalAmount.Pos(",")) == 1)
          {
              retValue += "0";
          }
      }
      retValue += fiscalData->FiscalVatRate1 != "" ? fiscalData->FiscalVatRate1 : (AnsiString)"0,00";
      retValue += ";";
      if(!isRefundReceipt)
          retValue += fiscalData->FiscalVat1 != "" ? fiscalData->FiscalVat1 : (AnsiString)"0,00";
      else
          retValue += "0,00";

      retValue += " ";
      retValue += fiscalData->FiscalVatRate2 != "" ? fiscalData->FiscalVatRate2 : (AnsiString)"0,00";
      retValue += ";";
      if(!isRefundReceipt)
          retValue += fiscalData->FiscalVat2 != "" ? fiscalData->FiscalVat2 : (AnsiString)"0,00";
      else
          retValue += "0,00";

      retValue += " ";
      retValue += fiscalData->FiscalVatRate3 != "" ? fiscalData->FiscalVatRate3 : (AnsiString)"0,00";
      retValue += ";";
      if(!isRefundReceipt)
          retValue += fiscalData->FiscalVat3  != "" ? fiscalData->FiscalVat3 : (AnsiString)"0,00";
      else
          retValue += "0,00";

      retValue += " ";
      retValue += fiscalData->FiscalVatRate4 != "" ? fiscalData->FiscalVatRate4 : (AnsiString)"0,00";
      retValue += ";";
      if(!isRefundReceipt)
          retValue += fiscalData->FiscalVat4 != "" ? fiscalData->FiscalVat4 : (AnsiString)"0,00";
      else
          retValue += "0,00";

      retValue = StringReplace(retValue,UnicodeString("-"),UnicodeString(""), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
      retValue += " ";
      int CalcCRC = StreamCheckCRC(retValue);
      UnicodeString hex_token = IntToHex(CalcCRC, 2);
      retValue += hex_token;
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
    }
    return retValue;
}
//---------------------------------------------------------------------------
int TFiscalDataUtility::StreamCheckCRC(AnsiString value)
{
    std::string data = value.c_str();
    __int32 maxBufferSize = data.size();
    __int32 itPos = 0;
    char   *buffer = new char[maxBufferSize];
    data.copy( buffer, maxBufferSize, itPos );
    TBytes bytes = CreateByteArray( buffer, maxBufferSize);

    int crc = 0;
    for (int i = 0; i < bytes.Length; i++)
        crc = (crc << 8) ^ XMODEMCRCTable[((crc >> 8) ^ bytes[i]) & 0x00FF];
    return crc & 0xFFFF;
}
//----------------------------------------------------------------------------
TBytes TFiscalDataUtility::CreateByteArray( const char* inBuffer, __int32 inBufferSize )
{
	TBytes result;
	result.Length = inBufferSize;

	for( int i = 0; i < result.Length; i++ )
	{
	   result[i] = inBuffer[i];
	}
	return result;
}
//---------------------------------------------------------------------------
void TFiscalDataUtility::ExtractVatDetails(TPaymentTransaction paymentTransaction, std::auto_ptr<TFiscalData> &inFiscalData)
{
    try
    {
        TaxVector.clear();
        for(int itemIndex = 0; itemIndex < paymentTransaction.Orders->Count; itemIndex++)
        {
            TItemComplete  *itemComplete = (TItemComplete*)paymentTransaction.Orders->Items[itemIndex];
            ExtractVatDetailsOnItemBasis(itemComplete);
            for(int subItemIndex = 0; subItemIndex < itemComplete->SubOrders->Count; subItemIndex++)
            {
                TItemComplete *subItem = (TItemComplete*)itemComplete->SubOrders->Items[subItemIndex];
                ExtractVatDetailsOnItemBasis(subItem);
            }
            PopulateTaxDetails(inFiscalData);
        }
     }
     catch(Exception &Exc)
     {
         MessageBox("in catch of extract tax","Shivashu",MB_OK);
         TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
     }
}
//---------------------------------------------------------------------------
void TFiscalDataUtility::PopulateTaxDetails(std::auto_ptr<TFiscalData> &inFiscalData)
{
    if(TaxVector.size() > 0)
    {
        AnsiString percentage = "";
        AnsiString value = "";
        double percentageTax = 0;
        double valueTax = 0;
        percentageTax = TaxVector[0].Percentage;
        percentageTax = RoundTo(percentageTax,-2);
        percentage = percentageTax;
        percentage = StringReplace(percentage,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
        if(percentage.Pos(",") == 0)
            percentage += ",00";
        if(percentage.Pos(",") != 0 &&
           (percentage.Length() - percentage.Pos(",")) == 1)
        {
           percentage += "0";
        }

        valueTax = TaxVector[0].Value;
        valueTax = RoundTo(valueTax,-2);
        value = valueTax;
        value = StringReplace(value,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
        if(value.Pos(",") == 0)
            value += ",00";

        if(value.Pos(",") != 0 &&
           (value.Length() - value.Pos(",")) == 1)
        {
           value += "0";
        }
        inFiscalData->FiscalVatRate1 = percentage;
        inFiscalData->FiscalVat1 = value;
        if(TaxVector.size() > 1)
        {
            percentage = "";
            value = "";
            percentageTax = 0;
            valueTax = 0;
            percentageTax = TaxVector[1].Percentage;
            percentageTax = RoundTo(percentageTax,-2);
            percentage = percentageTax;
            percentage = StringReplace(percentage,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(percentage.Pos(",") == 0)
                percentage += ",00";
            if(percentage.Pos(",") != 0 &&
               (percentage.Length() - percentage.Pos(",")) == 1)
            {
               percentage += "0";
            }

            valueTax = TaxVector[1].Value;
            valueTax = RoundTo(valueTax,-2);
            value = valueTax;
            value = StringReplace(value,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(value.Pos(",") == 0)
                value += ",00";

            if(value.Pos(",") != 0 &&
               (value.Length() - value.Pos(",")) == 1)
            {
               value += "0";
            }
            inFiscalData->FiscalVatRate2 = percentage;
            inFiscalData->FiscalVat2 = value;
        }
        if(TaxVector.size() > 2)
        {
            percentage = "";
            value = "";
            percentageTax = 0;
            valueTax = 0;
            percentageTax = TaxVector[2].Percentage;
            percentageTax = RoundTo(percentageTax,-2);
            percentage = percentageTax;
            percentage = StringReplace(percentage,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(percentage.Pos(",") == 0)
                percentage += ",00";

            if(percentage.Pos(",") != 0 &&
               (percentage.Length() - percentage.Pos(",")) == 1)
            {
               percentage += "0";
            }
            valueTax = TaxVector[2].Value;
            valueTax = RoundTo(valueTax,-2);
            value = valueTax;
            value = StringReplace(value,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(value.Pos(",") == 0)
                value += ",00";

            if(value.Pos(",") != 0 &&
               (value.Length() - value.Pos(",")) == 1)
            {
               value += "0";
            }
            inFiscalData->FiscalVatRate3 = percentage;
            inFiscalData->FiscalVat3 = value;
        }
        if(TaxVector.size() > 3)
        {
            percentage = "";
            value = "";
            percentageTax = 0;
            valueTax = 0;
            percentageTax = TaxVector[3].Percentage;
            percentageTax = RoundTo(percentageTax,-2);
            percentage = percentageTax;
            percentage = StringReplace(percentage,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(percentage.Pos(",") == 0)
                percentage += ",00";

            if(percentage.Pos(",") != 0 &&
               (percentage.Length() - percentage.Pos(",")) == 1)
            {
               percentage += "0";
            }

            valueTax = TaxVector[3].Value;
            valueTax = RoundTo(valueTax,-2);
            value = valueTax;
            value = StringReplace(value,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
            if(value.Pos(",") == 0)
                value += ",00";

            if(value.Pos(",") != 0 &&
               (value.Length() - value.Pos(",")) == 1)
            {
               value += "0";
            }
            inFiscalData->FiscalVatRate4 = percentage;
            inFiscalData->FiscalVat4 = value;
        }
    }
}
//---------------------------------------------------------------------------
void TFiscalDataUtility::ExtractVatDetailsOnItemBasis(TItemComplete *itemComplete)
{
    try
    {
        for(int j = 0; j < itemComplete->BillCalcResult.Tax.size(); j++)
        {
            if(TaxVector.size() == 0)
            {
                TFiscalTax tax;
                tax.Name = itemComplete->BillCalcResult.Tax[j].Name;
                tax.Type = (int)itemComplete->BillCalcResult.Tax[j].TaxType;
                tax.Percentage = itemComplete->BillCalcResult.Tax[j].Percentage;
                tax.Value = itemComplete->BillCalcResult.Tax[j].Value;
                TaxVector.push_back(tax);
            }
            else
            {
                std::vector<TFiscalTax>::iterator taxIT = TaxVector.begin();
                int ptr = 0;
                bool existingTax = false;
                TFiscalTax tax;
                tax.Name = "";
                tax.Type = 0;
                tax.Percentage = 0;
                tax.Value = 0;
                for(;taxIT != TaxVector.end(); advance(taxIT,1))
                {
                    if(taxIT->Name != itemComplete->BillCalcResult.Tax[j].Name ||
                       taxIT->Percentage != itemComplete->BillCalcResult.Tax[j].Percentage ||
                       taxIT->Type != (int)itemComplete->BillCalcResult.Tax[j].TaxType)
                    {
                        tax.Name = itemComplete->BillCalcResult.Tax[j].Name;
                        tax.Type = (int)itemComplete->BillCalcResult.Tax[j].TaxType;
                        tax.Percentage = itemComplete->BillCalcResult.Tax[j].Percentage;
                        tax.Value = itemComplete->BillCalcResult.Tax[j].Value;
                    }
                    else
                    {
                        existingTax = true;
                        double taxValue = TaxVector[ptr].Value;
                        taxValue += (double)itemComplete->BillCalcResult.Tax[j].Value;
                        TaxVector[ptr].Value = taxValue;
                        break;
                    }
                    ptr++;
                }
                if(!existingTax)
                {
                    TaxVector.push_back(tax);
                }
            }
        }
     }
     catch(Exception &Exc)
     {
         TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, Exc.Message);
     }
}
//---------------------------------------------------------------------------
AnsiString TFiscalDataUtility::GetInvoiceNumber(TPaymentTransaction paymentTransaction)
{
    UnicodeString invoiceNumber = "";
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    try
    {
        TIBSQL *IBInternalQueryGenerator= DBTransaction.Query(DBTransaction.AddQuery());
        IBInternalQueryGenerator->Close();
        switch(paymentTransaction.TypeOfSale)
        {
           case 1:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERCOMP, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = "Comp " + IntToStr(number);
                break;
           }
           case 2:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBERNC, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = "NC "+ IntToStr(number);
                break;
           }
           default:
           {
                IBInternalQueryGenerator->SQL->Text = "SELECT GEN_ID(GEN_INVOICENUMBER, 0) FROM RDB$DATABASE ";
                IBInternalQueryGenerator->ExecQuery();
                int number = IBInternalQueryGenerator->Fields[0]->AsInteger + 1;
                invoiceNumber = IntToStr(number);
                break;
           }
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        DBTransaction.Rollback();
    }
    return invoiceNumber;
}
//---------------------------------------------------------------------------
TFiscalData::TFiscalData()
{
//    FiscalDataKey       = 0;
//    FiscalInvoiceNumber = "";
    FiscalDateTime		= "";
    FiscalRegisterID	= "";
    FiscalSerialNumber	= "";
    FiscalReceiptType	= "";
    FiscalAmount		= "";
    FiscalVatRate1		= "";
    FiscalVat1			= "";
    FiscalVatRate2		= "";
    FiscalVat2			= "";
    FiscalVatRate3		= "";
    FiscalVat3			= "";
    FiscalVatRate4		= "";
    FiscalVat4			= "";
    FiscalCRCValue		= "";
    FIscalResponse		= "";
    IsPostedToPosPlus	= "F";
}
//----------------------------------------------------------------------------
TFiscalData::~TFiscalData()
{
}
//----------------------------------------------------------------------------
TFiscalTax::TFiscalTax()
{
    Type = 0;
    Name = "";
    Percentage = 0;
    Value = 0;
}
//----------------------------------------------------------------------------
TFiscalTax::~TFiscalTax()
{
}
//----------------------------------------------------------------------------
AnsiString TFiscalDataUtility::GetPOSPlusData(AnsiString invoiceNumber)
{
    AnsiString retValue = GetBillData(invoiceNumber);
    return retValue;
}
//----------------------------------------------------------------------------
AnsiString TFiscalDataUtility::GetBillData(AnsiString invoiceNumber)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    std::auto_ptr<TFiscalData> fiscalData(new TFiscalData());
    bool isRefundReceipt = false;
    AnsiString retValue = "";
    try
    {
        TIBSQL *IBSelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBSelectQuery->Close();
        IBSelectQuery->SQL->Text =
            "Select a.TERMINAL_NAME, a.REFUND_REFRECEIPT,a.INVOICE_NUMBER,a.TOTAL, dt.TAX_NAME, dt.TAX_VALUE, dt.TAX_TYPE "
            "FROM DAYARCBILL a                  "
            "Left Join                          "
            "DAYARCHIVE da                      "
            "on a.ARCBILL_KEY = da.ARCBILL_KEY  "
            "LEFT JOIN                          "
            "DAYARCORDERTAXES dt                "
            "on da.ARCHIVE_KEY = dt.ARCHIVE_KEY "
            "WHERE a.INVOICE_NUMBER = :INVOICE_NUMBER" ;
//                        "UNION ALL"
//            "Select a.TERMINAL_NAME, a.REFUND_REFRECEIPT,a.INVOICE_NUMBER,a.TOTAL, dt.TAX_NAME, dt.TAX_VALUE, dt.TAX_TYPE "
//            "FROM ARCBILL a                  "
//            "Left Join                          "
//            "ARCHIVE da                      "
//            "on a.ARCBILL_KEY = da.ARCBILL_KEY  "
//            "LEFT JOIN                          "
//            "ARCORDERTAXES dt                "
//            "on da.ARCHIVE_KEY = dt.ARCHIVE_KEY "
//            "WHERE a.INVOICE_NUMBER = :INVOICE_NUMBER";
       IBSelectQuery->ParamByName("INVOICE_NUMBER")->AsString = invoiceNumber;
       IBSelectQuery->ExecQuery();
       if(IBSelectQuery->RecordCount > 0)
       {
          AnsiString amountValue = FloatToStrF(IBSelectQuery->FieldByName("TOTAL")->AsFloat,ffNumber,15,2);
          amountValue = StringReplace(amountValue,UnicodeString("."),UnicodeString(","), TReplaceFlags() << rfReplaceAll << rfIgnoreCase);
          fiscalData->FiscalAmount = amountValue;
          if(IBSelectQuery->FieldByName("REFUND_REFRECEIPT")->AsString != "")
              isRefundReceipt = true;
          fiscalData->FiscalSerialNumber = invoiceNumber;
          fiscalData->FiscalDateTime = Now().CurrentDateTime().FormatString("YYYYMMDDhhmm");
          fiscalData->FiscalReceiptType = "kopia";
          fiscalData->FiscalRegisterID =  "1234567890";
          //"1234567890";

          TaxVector.clear();
          for ( ; !IBSelectQuery->Eof; IBSelectQuery->Next())
          {
              TFiscalTax tax;
              tax.Type = IBSelectQuery->FieldByName("TAX_TYPE")->AsInteger;
              tax.Name = IBSelectQuery->FieldByName("TAX_NAME")->AsString;
              tax.Value = RoundTo(IBSelectQuery->FieldByName("TAX_VALUE")->AsFloat,-2);
              TaxVector.push_back(tax);
          }
          GetTaxPercentageFromDB(DBTransaction);
          PopulateTaxDetails(fiscalData);
          DBTransaction.Commit();
          retValue = ConvertDataToString(fiscalData,isRefundReceipt);
       }
    }
    catch(Exception &E)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
        DBTransaction.Rollback();
    }
    return retValue;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void TFiscalDataUtility::GetTaxPercentageFromDB(Database::TDBTransaction &DBTransaction)
{
    for(int index = 0; index < TaxVector.size(); index++)
    {
        TIBSQL *IBSelectQuery= DBTransaction.Query(DBTransaction.AddQuery());
        IBSelectQuery->Close();
        IBSelectQuery->SQL->Text = "SELECT t.RATE FROM TAXPROFILES t "
                                   "WHERE t.NAME = :NAME AND t.TYPE = :TYPE";
        IBSelectQuery->ParamByName("NAME")->AsString = TaxVector[index].Name;
        IBSelectQuery->ParamByName("TYPE")->AsInteger = TaxVector[index].Type;
        IBSelectQuery->ExecQuery();
        TaxVector[index].Percentage = FloatToStrF(IBSelectQuery->FieldByName("RATE")->AsFloat,ffNumber,15,2);
    }
}
//----------------------------------------------------------------------------
