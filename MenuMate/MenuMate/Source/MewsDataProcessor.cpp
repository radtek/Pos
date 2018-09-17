//---------------------------------------------------------------------------


#pragma hdrstop

#include "MewsDataProcessor.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TMewsDataProcessor::TMewsDataProcessor()
{
}
//---------------------------------------------------------------------------
TMewsDataProcessor::~TMewsDataProcessor()
{
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateOutlets(std::vector<TOutlet> outlets)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Text =  "DELETE FROM OUTLETS";
        IBInternalQuery->ExecQuery();
        std::vector<TOutlet> ::iterator it = outlets.begin();
        for(;it != outlets.end();advance(it,1))
        {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text =
              "INSERT INTO OUTLETS "
              " ( UNIQUEID, NAME, ISACTIVE) "
              " VALUES "
              " (:UNIQUEID, :NAME, :ISACTIVE)";
          IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
          IBInternalQuery->ParamByName("NAME")->AsString = it->Name;
          if(it->IsActive)
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
          else
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";
          IBInternalQuery->ExecQuery();
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in OUTLETS",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateServices(std::vector<TServiceMews> services)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
          IBInternalQuery->Close();
          IBInternalQuery->ParamCheck = true;
          IBInternalQuery->SQL->Text =  "DELETE FROM SERVICES";
          IBInternalQuery->ExecQuery();
        std::vector<TServiceMews> ::iterator it = services.begin();
        for(;it != services.end();advance(it,1))
        {
          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text =
              "INSERT INTO SERVICES "
              " ( UNIQUEID, NAME, ISACTIVE, STARTTIME, ENDTIME) "
              " VALUES "
              " (:UNIQUEID, :NAME, :ISACTIVE, :STARTTIME, :ENDTIME)";
          IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
          IBInternalQuery->ParamByName("NAME")->AsString = it->Name;
          if(it->IsActive)
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
          else
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";

          // to be changed to actual values
          IBInternalQuery->ParamByName("STARTTIME")->AsDateTime = Now();
          IBInternalQuery->ParamByName("ENDTIME")->AsString = Now();

          IBInternalQuery->ExecQuery();
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in SERVICES",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateSpaces(TSpaceDetails spaces)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Text =  "DELETE FROM SPACES";
        IBInternalQuery->ExecQuery();
        std::vector<TSpace> ::iterator it = spaces.Spaces.begin();
        for(;it != spaces.Spaces.end();advance(it,1))
        {

          IBInternalQuery->Close();
          IBInternalQuery->SQL->Text =
              "INSERT INTO SPACES "
              " ( UNIQUEID, ISACTIVE, PARENTSPACEID, CATEGORYID, TYPE, NUMBER, FLOORNUMBER, BUILDINGNUMBER, STATE) "
              " VALUES "
              " (:UNIQUEID, :ISACTIVE, :PARENTSPACEID, :CATEGORYID, :TYPE, :NUMBER, :FLOORNUMBER, :BUILDINGNUMBER, :STATE)";
          IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;

          if(it->IsActive)
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
          else
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "F";

          // to be changed to actual values
          IBInternalQuery->ParamByName("PARENTSPACEID")->AsString = it->ParentSpaceId;
          IBInternalQuery->ParamByName("CATEGORYID")->AsString = it->CategoryId;
          IBInternalQuery->ParamByName("TYPE")->AsString = it->Type;
          IBInternalQuery->ParamByName("NUMBER")->AsString = it->Number;
          IBInternalQuery->ParamByName("FLOORNUMBER")->AsString = it->FloorNumber;
          IBInternalQuery->ParamByName("BUILDINGNUMBER")->AsString = it->BuildingNumber;
          IBInternalQuery->ParamByName("STATE")->AsString = it->State;

          IBInternalQuery->ExecQuery();
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in SPACES",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
void TMewsDataProcessor::UpdateCategories(std::vector<TAccountingCategory> categories)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->ParamCheck = true;
        IBInternalQuery->SQL->Text =  "DELETE FROM REVENUECODEDETAILS";
        IBInternalQuery->ExecQuery();
        std::vector<TAccountingCategory> ::iterator it = categories.begin();
        int i = 1;
        for(;it != categories.end();advance(it,1))
        {
            if(it->IsActive)
            {
              IBInternalQuery->Close();
              IBInternalQuery->SQL->Text =
                  "INSERT INTO REVENUECODEDETAILS "
                  " ( REVENUECODE, REVENUECODE_DESCRIPTION, ISDEFAULT_REVENUECODE, UNIQUEID, ISACTIVE, CODE, "
                  " EXTERNALCODE, LEDGERACCOUNTCODE, POSTINGACCOUNTCODE, COSTCENTRECODE, CLASSIFICATION) "
                  " VALUES "
                  " ( :REVENUECODE, :REVENUECODE_DESCRIPTION, :ISDEFAULT_REVENUECODE, :UNIQUEID, :ISACTIVE, "
                  " :CODE, :EXTERNALCODE, :LEDGERACCOUNTCODE, :POSTINGACCOUNTCODE, :COSTCENTRECODE, :CLASSIFICATION)";
              IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;

              IBInternalQuery->ParamByName("REVENUECODE")->AsInteger = i;
              IBInternalQuery->ParamByName("REVENUECODE_DESCRIPTION")->AsString = it->ExternalCode;
              IBInternalQuery->ParamByName("ISDEFAULT_REVENUECODE")->AsString = "F";
              IBInternalQuery->ParamByName("UNIQUEID")->AsString = it->Id;
              IBInternalQuery->ParamByName("ISACTIVE")->AsString = "T";
              IBInternalQuery->ParamByName("CODE")->AsString = it->Code;
              IBInternalQuery->ParamByName("EXTERNALCODE")->AsString = it->ExternalCode;
              IBInternalQuery->ParamByName("LEDGERACCOUNTCODE")->AsString = it->LedgerAccountCode;
              IBInternalQuery->ParamByName("POSTINGACCOUNTCODE")->AsString = it->PostingAccountCode;
              IBInternalQuery->ParamByName("CLASSIFICATION")->AsString = it->Classification;

              IBInternalQuery->ExecQuery();
              i++;
            }
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        MessageBox(ex.Message,"Exception in UpdateCategories",MB_OK);
        DBTransaction.Rollback();
    }
}
//---------------------------------------------------------------------------
std::vector<TOutlet> TMewsDataProcessor::GetOutlets()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TOutlet> outlets;
    outlets.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM OUTLETS";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)
        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           TOutlet outlet;
           outlet.Id = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
           if(IBInternalQuery->FieldByName("ISACTIVE")->AsString == "T")
               outlet.IsActive = true;
           else
               outlet.IsActive = false;
           outlet.Name = IBInternalQuery->FieldByName("NAME")->AsString;
           outlets.push_back(outlet);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
    }
    return outlets;
}
//---------------------------------------------------------------------------
std::vector<TServiceMews> TMewsDataProcessor::GetServices()
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
    std::vector<TServiceMews> services;
    services.clear();
    try
    {
        IBInternalQuery->Close();
        IBInternalQuery->SQL->Text =
          "SELECT * FROM SERVICES";
        IBInternalQuery->ExecQuery();
        //if(IBInternalQuery->RecordCount)

        for( ; !IBInternalQuery->Eof; IBInternalQuery->Next())
        {
           TServiceMews service;
           service.Id = IBInternalQuery->FieldByName("UNIQUEID")->AsString;
           if(IBInternalQuery->FieldByName("ISACTIVE")->AsString == "T")
               service.IsActive = true;
           else
               service.IsActive = false;
           service.Name = IBInternalQuery->FieldByName("NAME")->AsString;
           services.push_back(service);
        }
        DBTransaction.Commit();
    }
    catch(Exception &ex)
    {
        DBTransaction.Rollback();
    }
    return services;
}
//---------------------------------------------------------------------------
TCustomerSearch TMewsDataProcessor::CreateInquiryForRoomBySpace(UnicodeString queryString)
{
    TCustomerSearch customerSearch;
    customerSearch.SpaceId = queryString;
    return customerSearch;
}
//---------------------------------------------------------------------------
TCustomerSearch TMewsDataProcessor::CreateInquiryForRoomByName(UnicodeString queryString)
{
    TCustomerSearch customerSearch;
    customerSearch.Name = queryString;
    return customerSearch;
}
//---------------------------------------------------------------------------
