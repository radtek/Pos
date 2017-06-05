//---------------------------------------------------------------------------

#ifndef ExportCSVH
#define ExportCSVH
//---------------------------------------------------------------------------
#endif

#include <Dateutils.hpp>
#include <vector>
#include <map>
#include <Forms.hpp>
#include "MallExportOutputDBDriver.h"
#include "CSVExportClient.h"

class TExportCSV
{
    private:
        UnicodeString Customer_ID;
        UnicodeString Invoice_CM;
        UnicodeString Apply_to_Invoice_Number;
        UnicodeString Credit_Memo;
        UnicodeString Date;
        UnicodeString Ship_By;
        UnicodeString Quote;
        UnicodeString Quote_;
        UnicodeString Quote_Good_Thru_Date;
        UnicodeString DropShip;
        UnicodeString Ship_To_Name;
        UnicodeString Ship_to_Address_Line_One;
        UnicodeString Ship_to_Address_Line_Two;
        UnicodeString Ship_to_City;
        UnicodeString Ship_to_State;
        UnicodeString Ship_to_Zipcode;
        UnicodeString Ship_to_Country;
        UnicodeString Customer_PO;
        UnicodeString Ship_Via;
        UnicodeString Ship_Date;
        UnicodeString Date_Due;
        UnicodeString Discount_Amount;
        UnicodeString Discount_Date;
        UnicodeString Displayed_Terms;
        UnicodeString Sales_Representative_ID;
        UnicodeString Accounts_Receivable_Account;
        UnicodeString Sales_Tax_ID;
        UnicodeString Invoice_Note;
        UnicodeString Note_Prints_After_Line_Items;
        UnicodeString Statement_Note;
        UnicodeString Stmt_Note_Prints_Before_Ref;
        UnicodeString Internal_Note;
        UnicodeString Beginning_Balance_Transaction;
        UnicodeString Number_of_Distributions;
        UnicodeString Invoice_CM_Distribution;
        UnicodeString Apply_to_Invoice_Distribution;
        UnicodeString Apply_To_Sales_Order;
        UnicodeString Quantity;
        UnicodeString SO_Proposal_Number;
        UnicodeString item_ID;
        UnicodeString SOProposal_Distribution;
        UnicodeString Description;
        UnicodeString GL_Account;
        UnicodeString Unit_Price;
        UnicodeString Tax_Type;
        UnicodeString UPC_SKU;
        UnicodeString Weight;
        UnicodeString Amount;
        UnicodeString Job_ID;
        UnicodeString Sales_Tax_Agency_ID;
        UnicodeString Transaction_Period;
        UnicodeString Transaction_Number;
        UnicodeString Return_Authorization;
        UnicodeString Voided_by_Transaction;
        UnicodeString Recur_Number;
        void CreateFilename(UnicodeString &MallPath);
        void CreateHeaderFormat(UnicodeString MallPathFileName, std::vector<UnicodeString> &DataToWrite);
        std::vector<UnicodeString> DataToWrite;
        void LoadDataFromDB(std::vector<UnicodeString> &DataToWrite);
        UnicodeString GetTaxType(Currency vat, Currency serviceCharge, Currency serviceChargeTax, Currency localTax, Currency profitTax);

   public:
        void PostDateToCSV();
        void MoveCSVFile( AnsiString inFileName, AnsiString inDestPath );
};
