//---------------------------------------------------------------------------


#pragma hdrstop

#include "SiHotInterface.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TSiHotInterface::TSiHotInterface()
{
    initSIClient();
}
//---------------------------------------------------------------------------
void TSiHotInterface::initSIClient()
{
    bool useWSDL = false;
    AnsiString siHotURL = AnsiString("http://localhost:8742/MenumateServices/SiHotService/");

    siHotClient = GetISiHotIntegrationWebService(
                            useWSDL, siHotURL, NULL );
}
//---------------------------------------------------------------------------
TRoomResponse TSiHotInterface::SendRoomRequest(TRoomRequest _roomRequest, int timeOut)
{
    TRoomResponse roomResponse;
    try
    {
        RoomDetails* roomDetails = new RoomDetails();
        RoomRequest *roomRequest = new RoomRequest();
        roomRequest->IPAddress = _roomRequest.IPAddress;
        roomRequest->PortNumber = _roomRequest.PortNumber;
        roomRequest->TransactionNumber = _roomRequest.TransactionNumber;
        roomRequest->RoomNumber = _roomRequest.RoomNumber;
        CoInitialize(NULL);
        // Send Request and get Response
        timeOut = timeOut * 1000;
        roomDetails = siHotClient->GetRoomDetails(roomRequest,timeOut);
        ConvertSiHotRoomResponse(roomDetails, roomResponse);
        return roomResponse;
    }
    catch(Exception & E)
    {
        AnsiString directoryName = ExtractFilePath(Application->ExeName) + "/Menumate Services";
        if (!DirectoryExists(directoryName))
            CreateDir(directoryName);
        directoryName = directoryName + "/Sihot Post Logs";
        if (!DirectoryExists(directoryName))
            CreateDir(directoryName);
        AnsiString name = "SiHotPosts " + Now().CurrentDate().FormatString("DDMMMYYYY")+ ".txt";
        AnsiString fileName =  directoryName + "/" + name;
        std::auto_ptr<TStringList> List(new TStringList);
        if (FileExists(fileName) )
          List->LoadFromFile(fileName);
        List->Add("Exception Occurred while communication:- " + E.Message);
        List->Add("=================================================================");
        List->SaveToFile(fileName);
        return roomResponse;
	}
}
//---------------------------------------------------------------------------
void TSiHotInterface::ConvertSiHotRoomResponse(RoomDetails* _roomDetails, TRoomResponse &roomResponse)
{
    roomResponse.TransactionNumber = _roomDetails->TransNo;
    roomResponse.IsSuccessful = _roomDetails->IsSuccessful;
    roomResponse.ResponseMessage = _roomDetails->ResponseMessage;
    TGuestDetails guestDetails;
    if(_roomDetails->GuestDetailsList.get_length() > 0)
    {
        for(int i=0; i < _roomDetails->GuestDetailsList.get_length(); i++)
        {
            guestDetails.FirstName = _roomDetails->GuestDetailsList[i]->FirstName;
            guestDetails.LastName  = _roomDetails->GuestDetailsList[i]->LastName;
            guestDetails.Limit  = _roomDetails->GuestDetailsList[i]->Limit;
            guestDetails.AccountActive  = _roomDetails->GuestDetailsList[i]->AccountActive;
            guestDetails.AccountNumber  = _roomDetails->GuestDetailsList[i]->AccountNo;
            guestDetails.RoomBedNumber = _roomDetails->GuestDetailsList[i]->RoomBedNo;
            roomResponse.GuestsInformation.push_back(guestDetails);
        }
    }
}
//---------------------------------------------------------------------------
TRoomChargeResponse TSiHotInterface::SendRoomChargePost(TRoomCharge _roomCharge, int timeOut)
{
    TRoomChargeResponse roomChargeResponse;
    roomChargeResponse.IsSuccessful = false;
    RoomChargeResponse *roomResponse = new RoomChargeResponse;
    roomResponse->IsSuccessful = false;
    // Post room charge and get Response
    RoomChargeDetails *roomChargeDetails = new RoomChargeDetails;
    try
    {
        roomChargeDetails->TransNo = _roomCharge.TransactionNumber;
        roomChargeDetails->AccountNumber = _roomCharge.AccountNumber;
        roomChargeDetails->CashNo = _roomCharge.CashNo;
        roomChargeDetails->CoverType = "";
        roomChargeDetails->Covers = "";
        roomChargeDetails->Date = "";
        roomChargeDetails->IPAddress = _roomCharge.IPAddress;
        roomChargeDetails->PortNumber = _roomCharge.PortNumber;
        roomChargeDetails->LinkID = "";
        roomChargeDetails->LinkText = "";
        roomChargeDetails->Shift = "";
        roomChargeDetails->Time = "";
        ArrayOfSiHotService arrayofSiHotServices;
        ArrayOfSiHotPayment arrayofSiHotPayments;
        for(std::vector<TSiHotService>::iterator i = _roomCharge.SiHotServices.begin();
        i != _roomCharge.SiHotServices.end() ; ++i)
        {
            SiHotService *siHotService = GetServices(i);
            arrayofSiHotServices.Length = (arrayofSiHotServices.Length + 1);
            arrayofSiHotServices[arrayofSiHotServices.Length - 1] = siHotService;
        }
        roomChargeDetails->ItemList = arrayofSiHotServices;
        for(std::vector<TSiHotPayments>::iterator i = _roomCharge.SiHotPayments.begin();
        i != _roomCharge.SiHotPayments.end() ; ++i)
        {
            SiHotPayment *siHotPayment = GetPayment(i);
            arrayofSiHotPayments.Length = (arrayofSiHotPayments.Length + 1);
            arrayofSiHotPayments[arrayofSiHotPayments.Length - 1] = siHotPayment;
        }
        roomChargeDetails->PaymentList = arrayofSiHotPayments;
        roomChargeDetails->Total = _roomCharge.Total;
        CoInitialize(NULL);
        timeOut = timeOut * 1000;
        roomResponse = siHotClient->PostRoomCharge(roomChargeDetails,timeOut);
        roomChargeResponse.IsSuccessful = roomResponse->IsSuccessful;
        roomChargeResponse.ResponseMessage = roomResponse->Response;
        return roomChargeResponse;
    }
    catch(Exception & E)
    {
       return roomChargeResponse;
	}
}
//---------------------------------------------------------------------------
SiHotService* TSiHotInterface::GetServices(std::vector<TSiHotService>::iterator it)
{
    SiHotService *service = new SiHotService;
    service->SuperCategory = it->SuperCategory;
    service->SuperCategory_Desc = it->SuperCategory_Desc;
    service->Amount = it->Amount;
    service->ArticleCategory = it->ArticleCategory;
    service->ArticleCategory_Desc = it->ArticleCategory_Desc;
    service->ArticleNo = it->ArticleNo;
    service->ArticleNo_Desc = it->ArticleNo_Desc;
    service->Billno = it->Billno;
    service->Cashier = it->Cashier;
    service->Cashno = it->Cashno;
    service->MiddleCategory = it->MiddleCategory;
    service->MiddleCategory_Desc = it->MiddleCategory_Desc;
    service->PricePerUnit = it->PricePerUnit;
    service->PriceTotal = it->PriceTotal;
    service->Source = it->Source;
    service->VATPercentage = it->VATPercentage;
    return service;
}
//---------------------------------------------------------------------------
SiHotPayment* TSiHotInterface::GetPayment(std::vector<TSiHotPayments>::iterator it)
{
    SiHotPayment *payment = new SiHotPayment;
    payment->Amount = it->Amount;
    payment->Billno = it->Billno;
    payment->Cashier = it->Cashier;
    payment->Cashno = it->Cashno;
    payment->Description = it->Description;
    payment->Source = it->Source;
    payment->Type = it->Type;
    return payment;
}
//---------------------------------------------------------------------------
bool TSiHotInterface::ValidateIPAddressPort(AnsiString address,int port,int transno)
{
    try
    {
        CoInitialize(NULL);
        bool retValue = siHotClient->ValidateCreadentials(address,port,transno);
        return retValue;
    }
    catch(Exception & E)
    {
       return false;
	}
}
//---------------------------------------------------------------------------
