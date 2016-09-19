//---------------------------------------------------------------------------


#pragma hdrstop

#include "ExportCSV.h"
#include "DeviceRealTerminal.h"
#include "Analysis.h"
#include "PaymentTransaction.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//--------------------------------------------------------------------------
void TExportCSV::PostDateToCSV()
{
    UnicodeString CSVPath = TGlobalSettings::Instance().CSVPath + "\\";

    //Create File Name file will be created
    CreateFilename(CSVPath);

    //Create CSV Header
    CreateHeaderFormat(CSVPath,DataToWrite);

    //Use Federal Mall's Function of printing Header
    TMallExportOutputDBDriver  exportCSV;
    exportCSV.GenerateTXT(DataToWrite, CSVPath, "ZTXTHEADER");

    //Clear DataToWrite Vector
    DataToWrite.erase (DataToWrite.begin(),DataToWrite.end());

    //Load all data into vector
    LoadDataFromDB(DataToWrite);

    //calling existing mall function for insert Data into CSV
    exportCSV.GenerateTXT(DataToWrite, CSVPath, "ZTXTDATA");


    if(TGlobalSettings::Instance().CSVExportIP != "" && (TGlobalSettings::Instance().CSVExportIP).LowerCase() != "localhost" && TGlobalSettings::Instance().CSVExportIP != "127.0.0.1")
    {
        TCSVExportClient *csvClient = new TCSVExportClient();
            csvClient->SendPointDetailsToCSVExport(CSVPath);
    }
}

//------------------------------------------------------------------------
void TExportCSV::CreateFilename(UnicodeString &CSVPath)
{
    try
    {
        UnicodeString Format     = ".csv";
        UnicodeString TerminalName = TDeviceRealTerminal::Instance().ID.Name;
        TSearchRec sr;
	    int iAttributes = faAnyFile;
        CSVPath = CSVPath + "Accounting Export" + "\\";

        //Check if directory not exist than create it.
        if (!DirectoryExists(CSVPath))
        {
            CreateDir(CSVPath);
        }

        //Check any csv file exist on the path specified
        if( FindFirst( CSVPath + "*.CSV", iAttributes, sr ) == 0 )
	    {
            do
            {
                if (sr.Attr & iAttributes)
                {
                    MoveCSVFile( CSVPath + sr.Name, CSVPath + "Archive CSV\\" );
                }
            }
            while(FindNext(sr) == 0);
        }

        //CSV will be created with Following Name
        CSVPath = CSVPath + TerminalName + Now().FormatString("ddmmyyyyhhnnss") + Format;
    }
    catch( Exception& exc )
    {
    }
}
//---------------------------------------------------------------------------------------
void TExportCSV::CreateHeaderFormat(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite)
{
    UnicodeString Store = "";
    UnicodeString Format = ",";
    std::ofstream CreateColumn;

    //Creating CSV header
    if(!CreateColumn.is_open())
    {
            Store = "Customer ID,Invoice/CM #,Apply to Invoice Number,Credit Memo,Date,Ship By" + Format +
                    "Quote,Quote #,Quote Good Thru Date,DropShip,Ship To Name,Ship to Address-Line One" + Format +
                    "Ship to Address-Line Two,Ship to City,Ship to State,Ship to Zipcode" + Format +
                    "Ship to Country,Customer PO,Ship Via,Ship Date,Date Due,Discount Amount" + Format +
                    "Discount Date,Displayed Terms,Sales Representative ID,	Accounts Receivable Account" + Format +
                    "Sales Tax ID,Invoice Note,Note Prints After Line Items" + Format +
                    "Statement Note,Stmt Note Prints Before Ref,Internal Note,Beginning Balance Transaction" + Format +
                    "Number of Distributions,Invoice/CM Distribution,Apply to Invoice Distribution" + Format +
                    "Apply To Sales Order,Quantity,SO/Proposal Number,item ID,SO/Proposal Distribution" + Format +
                    "Description,G/L Account,Unit Price,Tax Type,UPC / SKU,Weight,Amount,Job ID" + Format +
                    "Sales Tax Agency ID,Transaction Period,Transaction Number,Return Authorization" + Format +
                    "Voided by Transaction,Recur Number" + "\n";
              DataToWrite.push_back(Store.t_str());

    }
    CreateColumn.close();
}
//------------------------------------------------------------------------------------------
void TExportCSV::LoadDataFromDB(std::vector<UnicodeString> &dataToWrite)
{
    UnicodeString Format = ",";
    UnicodeString OutputValue = "";
    UnicodeString masterSlaveCondition = "";
    bool isMasterTerminal;
    static int invoiceDistribution = 1;
    int oldArcBillKey, newArcBillKey, invoiceCMDistribution, noOfTransaction;
    Currency qty;
    TDateTime date;

     // check for master -slave terminal
    if (!TGlobalSettings::Instance().EnableDepositBagNum)
	{
		masterSlaveCondition = " AND  DAB.TERMINAL_NAME = :TERMINAL_NAME ";
	}

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();

    TIBSQL* query = DBTransaction.Query(DBTransaction.AddQuery());

    query->Close();
    query->SQL->Text =
             "SELECT                                      "
                "CONTACTS.MEMBER_NUMBER CUSTOMER_ID,     "
                "DAB.invoice_number,                     "
                "DAB.time_stamp ,                        "
                "SECURITY.USER_KEY SalesRepresentativeId,"
                "DAC.noOfItem NumberOfDistributions,     "
                "DAB.TOTAL FTOTAL ,                      "
                "Sum(da.Qty) Qty,                        "
                "ThirdPartyCodes.CODE,                 "
                "ACC.GL_CODE,                            "
                "ITEMSIZE.BARCODE UPC_SKU,               "
                "DAB.ARCBILL_KEY,                        "
                "da.ITEM_NAME, 							 "
                "da.SIZE_NAME,                           "
                "SIZES.WEIGHTED_SIZE,                    "
                "Cast(Sum(DA.Qty * DA.BASE_PRICE ) as Numeric(17,4)) priceExcl,                         "
                "Cast(Sum(COALESCE( abs(AOT.VAT) ,0)) as Numeric(17,4)) VAT,                                 "
                "Cast(Sum( COALESCE( abs(AOT.ServiceCharge),0) ) as Numeric(17,4)) ServiceCharge,            "
                "cast(Sum(COALESCE( abs(AOT.OtherServiceCharge),0)) as numeric(17, 4)) as ServiceChargeTax,  "
                "cast(Sum(COALESCE( abs(AOT.LocalTax),0)) as numeric(17, 4)) as LocalTax,  "
                "cast(Sum(COALESCE( abs(AOT.ProfitTax),0)) as numeric(17, 4)) as ProfitTax, "
                "cast(Sum(  COALESCE(DA.DISCOUNT,0))as numeric(17, 4)) AS Discount,                     "
                "coalesce(ROUND(Cast((cast(Sum( DA.QTY *DA.BASE_PRICE  )as Numeric(17,4))+Sum(COALESCE(abs(AOT.VAT),0))+ Sum(COALESCE( abs(AOT.ServiceCharge),0)) + "
                "Sum(COALESCE( abs(AOT.OtherServiceCharge),0)) + Sum(COALESCE( abs(AOT.LocalTax),0)) + Sum(COALESCE( abs(AOT.ProfitTax),0))+ Sum(COALESCE(DA.DISCOUNT_WITHOUT_TAX,0)))/Sum(da.Qty) as Numeric(17,4)),2),0) SalesIncl "
            "FROM DAYARCBILL DAB "
            "left JOIN DAYARCHIVE DA ON DAB.ARCBILL_KEY = DA.ARCBILL_KEY "
            "Left join (SELECT  a.ARCHIVE_KEY,sum(a.DISCOUNTED_VALUE) DISCOUNTED_VALUE,  a.DISCOUNT_GROUPNAME "
                           " FROM DAYARCORDERDISCOUNTS a                       "
                           " group by a.ARCHIVE_KEY ,a.DISCOUNT_GROUPNAME)     "
                           " DAOD on DA.ARCHIVE_KEY = DAOD.ARCHIVE_KEY         "
            "left join SECURITY on DAB.SECURITY_REF = SECURITY.SECURITY_REF     "
            "left join (SELECT count(*) noOfItem, DA.INVOICE_NUMBER            "
                      "FROM (select                                             "
                               "a.ARCBILL_KEY, a.ITEM_NAME,a.SIZE_NAME, DAB.INVOICE_NUMBER   "
                               "FROM DAYARCHIVE a                                            "
                               "inner join DAYARCBILL DAB on a.ARCBILL_KEY = DAB.ARCBILL_KEY "
                               "GROUP BY                                                     "
                               "1,2,3,4) DA                                                  "
                               "GROUP BY                                                     "
                               "DA.INVOICE_NUMBER                                            "
                               "ORDER BY 2)DAC on DAC.invoice_number = DAB.invoice_number    "
            "left join CONTACTS on DA.LOYALTY_KEY = CONTACTS.CONTACTS_KEY "
            "left join ARCCATEGORIES ACC on da.CATEGORY_KEY = ACC.CATEGORY_KEY              "
            "left join ITEMSIZE on DA.ITEM_ID = ITEMSIZE.ITEM_KEY and DA.SIZE_NAME = ITEMSIZE.SIZE_NAME "
            "left join SIZES on ITEMSIZE.SIZES_KEY = SIZES.SIZES_KEY                        "
            "Left Join ThirdPartyCodes On  "
				"ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
            "LEFT JOIN (                                                                 "
                                 "SELECT                                                     "
                                    "DAYARCORDERTAXES.ARCHIVE_KEY,                           "
                                    "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 0 THEN DAYARCORDERTAXES.TAX_VALUE END) AS VAT, 				"
                                    "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 2 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ServiceCharge,      "
                                    "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 3 THEN DAYARCORDERTAXES.TAX_VALUE END) AS OtherServiceCharge,  "
                                    "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 4 THEN DAYARCORDERTAXES.TAX_VALUE END) AS LocalTax, "
						            "MIN(CASE WHEN DAYARCORDERTAXES.TAX_TYPE = 5 THEN DAYARCORDERTAXES.TAX_VALUE END) AS ProfitTax "
                                "FROM (SELECT  a.ARCHIVE_KEY,a.TAX_TYPE,                                                                       "
                                    "Cast(Sum(a.TAX_VALUE ) as Numeric(17,4)) TAX_VALUE                                                      "
                                    "FROM DAYARCORDERTAXES a                                                                                 "
                                    "group by  a.TAX_TYPE  , a.ARCHIVE_KEY                                                                   "
                                    "order by 1 )  DAYARCORDERTAXES                                                                          "
                                    "GROUP BY DAYARCORDERTAXES.ARCHIVE_KEY )                                                                 "
                                    "AOT ON DA.ARCHIVE_KEY = AOT.ARCHIVE_KEY                                                                 "
                                "Where DA.ARCHIVE_KEY not in (Select     DAYARCHIVE.ARCHIVE_KEY from DAYARCHIVE left join SECURITY on  SECURITY.SECURITY_REF=DAYARCHIVE.SECURITY_REF "
                                                                "where  security.SECURITY_EVENT='CancelY' or security.SECURITY_EVENT='Cancel') and "
                                                                "((COALESCE(DAOD.DISCOUNT_GROUPNAME,0)<> 'Non-Chargeable' AND COALESCE(DAOD.DISCOUNT_GROUPNAME,0) <> 'Complimentary')) "
                                       "and SECURITY.SECURITY_EVENT NOT IN ('Credit', 'WriteOff', 'Discounted By','Price Adjust')  "
                                         + masterSlaveCondition +
            "GROUP BY DAB.invoice_number, DAB.time_stamp, DAOD.DISCOUNTED_VALUE,  da.QTY, da.ITEM_NAME,da.SIZE_NAME,DAB.TOTAL , DAB.ARCBILL_KEY,CONTACTS.MEMBER_NUMBER,     "
            "	DAC.noOfItem, SECURITY.USER_KEY,   "
            "	da.THIRDPARTYCODES_KEY,  "
            "	ACC.GL_CODE,             "
            "	ITEMSIZE.BARCODE, "
            "   SIZES.WEIGHTED_SIZE, "
            "   ThirdPartyCodes.CODE, "
            "   da.price "
            "ORDER BY DAB.ARCBILL_KEY ";
    if (!TGlobalSettings::Instance().EnableDepositBagNum)
    {
        query->ParamByName("TERMINAL_NAME")->AsString = TDeviceRealTerminal::Instance().ID.Name;
    }
    query->ExecQuery();
    noOfTransaction = query->FieldByName("ARCBILL_KEY")->AsInteger;
    int daysinMonth = Dateutils::DaysInMonth(Now());
    TDateTime dueDate = Dateutils::EncodeDateTime(YearOf(Now()),MonthOf(Now()),daysinMonth,
                                                            HourOfTheDay(Now()),MinuteOfTheHour(Now()),00,000);

    //initializing CSV fields
    while(!query->Eof)
    {
        Customer_ID                    =    query->FieldByName("CUSTOMER_ID")->AsString;
        Invoice_CM                     =    "REF#" + query->FieldByName("invoice_number")->AsString;
        Apply_to_Invoice_Number        =    "";
        Credit_Memo                    =    "FALSE";
        date                           =    query->FieldByName("time_stamp")->AsDateTime;
        Date                           =    date.FormatString("mm-dd-yyyy");
        Ship_By                        =    "";
        Quote                          =    "";
        Quote_                         =    "FALSE";
        Quote_Good_Thru_Date           =    "";
        DropShip                       =    "FALSE";
        Ship_To_Name                   =    "";
        Ship_to_Address_Line_One       =    "";
        Ship_to_Address_Line_Two       =    "";
        Ship_to_City                   =    "";
        Ship_to_State                  =    "";
        Ship_to_Zipcode                =    "";
        Ship_to_Country                =    "";
        Customer_PO                    =    "";
        Ship_Via                       =    "";
        Ship_Date                      =    "";
        Date_Due                       =    dueDate.FormatString("mm-dd-yyyy");
        Discount_Amount                =    query->FieldByName("Discount")->AsInteger;

        if(Discount_Amount != 0)
            Discount_Date                  =     date.FormatString("mm-dd-yyyy");

        Displayed_Terms                =    "Due at end of Month";
        Sales_Representative_ID        =    query->FieldByName("SalesRepresentativeId")->AsInteger;
        Accounts_Receivable_Account    =    "105-00";
        Sales_Tax_ID                   =    "";
        Invoice_Note                   =    "";
        Note_Prints_After_Line_Items   =    "FALSE";
        Statement_Note                 =    "";
        Stmt_Note_Prints_Before_Ref    =    "FALSE";
        Internal_Note                  =    "";
        Beginning_Balance_Transaction  =    "FALSE";
        Number_of_Distributions        =    query->FieldByName("NumberOfDistributions")->AsInteger;
        invoiceCMDistribution          =    query->FieldByName("NumberOfDistributions")->AsInteger;
        newArcBillKey                  =    query->FieldByName("ARCBILL_KEY")->AsInteger;

        if (invoiceCMDistribution > 1 && invoiceDistribution != invoiceCMDistribution &&
                                            oldArcBillKey == newArcBillKey)
        {
            invoiceDistribution++;
        }
        else
        {
            invoiceDistribution = 1;
            oldArcBillKey =  query->FieldByName("ARCBILL_KEY")->AsInteger;
            newArcBillKey =  query->FieldByName("ARCBILL_KEY")->AsInteger;
        }

        Invoice_CM_Distribution        =   invoiceDistribution;
        Apply_to_Invoice_Distribution  =    "";
        Apply_To_Sales_Order           =    "FALSE";

        SO_Proposal_Number             =    "";
        item_ID                        =    query->FieldByName("CODE")->AsString;
        SOProposal_Distribution        =    "";
        Description                    =    query->FieldByName("ITEM_NAME")->AsString;
        GL_Account                     =    query->FieldByName("GL_CODE")->AsString;
        Unit_Price                     =    query->FieldByName("SalesIncl")->AsCurrency;

        Tax_Type                       =    CombinedTaxTypeString(query->FieldByName("VAT")->AsCurrency,
                                                                query->FieldByName("ServiceCharge")->AsCurrency,
                                                                query->FieldByName("ServiceChargeTax")->AsCurrency,
                                                                query->FieldByName("LocalTax")->AsCurrency,
                                                                query->FieldByName("ProfitTax")->AsCurrency);
        UPC_SKU                        =    query->FieldByName("UPC_SKU")->AsString;
        qty                            =    query->FieldByName("Qty")->AsCurrency;
        if(query->FieldByName("WEIGHTED_SIZE")->AsString == 'T')
        {
            Weight                     =    qty;
            Quantity                   =    0;
        }
        else
        {
           Weight                      =    0;
           Quantity                    =    qty;
        }
        Amount                         =    (query->FieldByName("SalesIncl")->AsCurrency) > 0 ? (-1)*(query->FieldByName("SalesIncl")->AsCurrency)*qty : (query->FieldByName("FTOTAL")->AsCurrency);
        Job_ID                         =    "";
        Sales_Tax_Agency_ID            =    "";
        Transaction_Period             =    "44";
        Transaction_Number             =    abs(newArcBillKey - noOfTransaction + 1) ;
        Return_Authorization           =    "";
        Voided_by_Transaction          =    "";
        Recur_Number                   =    "0";

        //formatting single csv row , seperating all field by commas
        OutputValue = Customer_ID + Format + Invoice_CM + Format + Apply_to_Invoice_Number + Format + Credit_Memo + Format +
                    Date + Format + Ship_By + Format + Quote + Format + Quote_ + Format +
                    Quote_Good_Thru_Date + Format + DropShip + Format + Ship_To_Name + Format +
                    Ship_to_Address_Line_One + Format + Ship_to_Address_Line_Two + Format +
                    Ship_to_City + Format + Ship_to_State + Format + Ship_to_Zipcode + Format +
                    Ship_to_Country + Format + Customer_PO + Format + Ship_Via + Format +
                    Ship_Date + Format + Date_Due + Format + Discount_Amount + Format +
                    Discount_Date + Format + Displayed_Terms + Format + Sales_Representative_ID + Format +
                    Accounts_Receivable_Account + Format + Sales_Tax_ID + Format + Invoice_Note + Format +
                    Note_Prints_After_Line_Items + Format + Statement_Note + Format +
                    Stmt_Note_Prints_Before_Ref + Format + Internal_Note + Format +
                    Beginning_Balance_Transaction + Format + Number_of_Distributions + Format +
                    Invoice_CM_Distribution + Format + Apply_to_Invoice_Distribution + Format +
                    Apply_To_Sales_Order + Format + Quantity  + Format + SO_Proposal_Number + Format +
                    item_ID + Format + SOProposal_Distribution + Format + Description + Format +
                    GL_Account + Format + Unit_Price + Format + Tax_Type + Format + UPC_SKU + Format +
                    Weight + Format + Amount + Format + Job_ID + Format + Sales_Tax_Agency_ID + Format +
                    Transaction_Period + Format + Transaction_Number + Format + Return_Authorization + Format +
                    Voided_by_Transaction + Format + Recur_Number + "\n";

        dataToWrite.push_back(OutputValue.t_str());
        query->Next();
    }
    DBTransaction.Commit();
}

//-------------------------------------------------------------------------------------------
void TExportCSV::MoveCSVFile( AnsiString inFileName, AnsiString inDestPath )
{
    //check whether directory exist
	if( !DirectoryExists( inDestPath ) )
	{
		ForceDirectories( inDestPath );
	}

 	AnsiString destFileName = inDestPath + ExtractFileName( inFileName );

    //If file exists on destination than delete it.
	if( FileExists( destFileName ) )
	{
		DeleteFile( destFileName );
	}

    //delete file from source location.
	if( CopyFile( inFileName.c_str(), destFileName.c_str(), false ) != 0 )
	{
		if( FileExists( inFileName ) )
		{
			DeleteFile( inFileName );
		}
	}
}
//---------------------------------------------------------------------------------------------
UnicodeString TExportCSV::CombinedTaxTypeString(Currency vat, Currency serviceCharge, Currency serviceChargeTax, Currency localTax, Currency profitTax)
{
    UnicodeString taxType          =    "";

    if(vat !=0)
    {
        taxType = 0;
    }
    if(serviceCharge !=0)
    {
        if(taxType != "")
            taxType = taxType + "||" + 2;
        else
            taxType = 2;
    }
    if(serviceChargeTax !=0)
    {
        if(taxType != "")
            taxType = taxType + "||" + 3;
        else
            taxType = 3;
    }
    if(localTax !=0)
    {
        if(taxType != "")
            taxType = taxType + "||" + 4;
        else
            taxType = 4;
    }
    if(profitTax !=0)
    {
        if(taxType != "")
            taxType = taxType + "||" + 5;
        else
            taxType = 5;
    }
    return taxType;
}
