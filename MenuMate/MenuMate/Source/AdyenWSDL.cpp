// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8745/MenumateServices/AdyenService/?wsdl
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?wsdl>0
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd0
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd2
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd3
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd5
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd8
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd4
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd6
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd7
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (7/05/2018 5:55:24 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "AdyenWSDL.h"
#endif



namespace NS__AdyenWSDL {

_di_IAdyenIntegrationWebService GetIAdyenIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8745/MenumateServices/AdyenService/?wsdl";
  static const char* defURL = "http://localhost:8745/MenumateServices/AdyenService/";
  static const char* defSvc = "AdyenIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceSalesForce";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  rio->HTTPWebNode->SendTimeout = 120000;
  rio->HTTPWebNode->ReceiveTimeout = 120000;
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_IAdyenIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall SaleTerminalData::~SaleTerminalData()
{
  delete FSaleProfile;
}

__fastcall PaymentRequest::~PaymentRequest()
{
  delete FPaymentData;
  delete FPaymentTransaction;
  delete FSaleData;
}

__fastcall PaymentTransaction::~PaymentTransaction()
{
  delete FAmountsReq;
}

__fastcall SaleData::~SaleData()
{
  delete FSaleTerminalData;
  delete FSaleToIssuerData;
  delete FSaleTransactionID;
}

__fastcall ReversalRequest::~ReversalRequest()
{
  delete FOriginalPOITransaction;
}

__fastcall OriginalPOITransaction::~OriginalPOITransaction()
{
  delete FPOITransactionID;
}

__fastcall POIData::~POIData()
{
  delete FPOITransactionID;
}

__fastcall PaymentResponse::~PaymentResponse()
{
  delete FPOIData;
  delete FPaymentResult;
  delete FResponse;
  delete FSaleData;
  for(int i=0; i<FPaymentReceipt.Length; i++)
    if (FPaymentReceipt[i])
      delete FPaymentReceipt[i];
}

__fastcall PaymentResult::~PaymentResult()
{
  delete FAmountsResp;
  delete FCapturedSignature;
  delete FCurrencyConversion;
  delete FInstalment;
  delete FPaymentAcquirerData;
  delete FPaymentInstrumentData;
}

__fastcall PaymentReceipt::~PaymentReceipt()
{
  delete FOutputContent;
}

__fastcall OutputContent::~OutputContent()
{
  delete FPredefinedContent;
  for(int i=0; i<FOutputText.Length; i++)
    if (FOutputText[i])
      delete FOutputText[i];
}

__fastcall CapturedSignature::~CapturedSignature()
{
  delete FAreaSize;
}

__fastcall CurrencyConversion::~CurrencyConversion()
{
  delete FConvertedAmount;
}

__fastcall PaymentAcquirerData::~PaymentAcquirerData()
{
  delete FAcquirerTransactionID;
}

__fastcall PaymentInstrumentData::~PaymentInstrumentData()
{
  delete FCardData;
  delete FCheckData;
  delete FMobileData;
  delete FStoredValueAccountID;
}

__fastcall CheckData::~CheckData()
{
  delete FTrackData;
}

__fastcall MobileData::~MobileData()
{
  delete FGeolocation;
  delete FSensitiveMobileData;
}

__fastcall CardData::~CardData()
{
  delete FPaymentToken;
  delete FSensitiveCardData;
  for(int i=0; i<FAllowedProduct.Length; i++)
    if (FAllowedProduct[i])
      delete FAllowedProduct[i];
  for(int i=0; i<FCustomerOrder.Length; i++)
    if (FCustomerOrder[i])
      delete FCustomerOrder[i];
}

__fastcall SensitiveCardData::~SensitiveCardData()
{
  delete FTrackData;
}

__fastcall Geolocation::~Geolocation()
{
  delete FGeographicCoordinates;
  delete FUTMCoordinates;
}

__fastcall DiagnosisResponse::~DiagnosisResponse()
{
  delete FPOIStatus;
  delete FResponse;
}

__fastcall LoginRequest::~LoginRequest()
{
  delete FSaleSoftware;
  delete FSaleTerminalData;
}

__fastcall Envelop::~Envelop()
{
  delete FSaleToPOIRequest;
}

__fastcall SaleToPOIRequest::~SaleToPOIRequest()
{
  delete FDiagnosisRequest;
  delete FLoginRequest;
  delete FLogoutRequest;
  delete FMessageHeader;
  delete FPaymentRequest;
  delete FReversalRequest;
}

__fastcall SaleToPOIResponse::~SaleToPOIResponse()
{
  delete FDiagnosisResponse;
  delete FEventNotification;
  delete FMessageHeader;
  delete FPaymentResponse;
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IAdyenIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IAdyenIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IAdyenIntegrationWebService), L"http://tempuri.org/IAdyenIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IAdyenIntegrationWebService), ioDocument);
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* SaleSoftware */
  RemClassRegistry()->RegisterXSClass(__classid(SaleSoftware), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleSoftware");
  /* SaleTerminalData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleTerminalData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleTerminalData");
  /* SaleProfile */
  RemClassRegistry()->RegisterXSClass(__classid(SaleProfile), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleProfile");
  /* PaymentRequest */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentRequest");
  /* PaymentData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentData");
  /* PaymentTransaction */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentTransaction), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentTransaction");
  /* SaleData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleData");
  /* AmountsReq */
  RemClassRegistry()->RegisterXSClass(__classid(AmountsReq), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AmountsReq");
  /* SaleToIssuerData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToIssuerData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleToIssuerData");
  /* SaleTransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(SaleTransactionID), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleTransactionID");
  /* ReversalRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ReversalRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ReversalRequest");
  /* OriginalPOITransaction */
  RemClassRegistry()->RegisterXSClass(__classid(OriginalPOITransaction), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OriginalPOITransaction");
  /* POITransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(POITransactionID), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"POITransactionID");
  /* Response */
  RemClassRegistry()->RegisterXSClass(__classid(Response), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Response");
  /* POIData */
  RemClassRegistry()->RegisterXSClass(__classid(POIData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"POIData");
  /* ArrayOfPaymentReceipt */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPaymentReceipt), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfPaymentReceipt");
  /* PaymentResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentResponse), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentResponse");
  /* PaymentResult */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentResult), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentResult");
  /* PaymentReceipt */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentReceipt), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentReceipt");
  /* ArrayOfOutputText */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOutputText), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfOutputText");
  /* OutputContent */
  RemClassRegistry()->RegisterXSClass(__classid(OutputContent), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OutputContent");
  /* PredefinedContent */
  RemClassRegistry()->RegisterXSClass(__classid(PredefinedContent), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PredefinedContent");
  /* OutputText */
  RemClassRegistry()->RegisterXSClass(__classid(OutputText), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OutputText");
  /* AmountsResp */
  RemClassRegistry()->RegisterXSClass(__classid(AmountsResp), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AmountsResp");
  /* CapturedSignature */
  RemClassRegistry()->RegisterXSClass(__classid(CapturedSignature), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CapturedSignature");
  /* CurrencyConversion */
  RemClassRegistry()->RegisterXSClass(__classid(CurrencyConversion), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CurrencyConversion");
  /* Instalment */
  RemClassRegistry()->RegisterXSClass(__classid(Instalment), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Instalment");
  /* PaymentAcquirerData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentAcquirerData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentAcquirerData");
  /* PaymentInstrumentData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentInstrumentData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentInstrumentData");
  /* AreaSize */
  RemClassRegistry()->RegisterXSClass(__classid(AreaSize), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AreaSize");
  /* ConvertedAmount */
  RemClassRegistry()->RegisterXSClass(__classid(ConvertedAmount), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ConvertedAmount");
  /* AcquirerTransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(AcquirerTransactionID), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AcquirerTransactionID");
  /* CheckData */
  RemClassRegistry()->RegisterXSClass(__classid(CheckData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CheckData");
  /* MobileData */
  RemClassRegistry()->RegisterXSClass(__classid(MobileData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"MobileData");
  /* StoredValueAccountID */
  RemClassRegistry()->RegisterXSClass(__classid(StoredValueAccountID), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"StoredValueAccountID");
  /* ArrayOfAllowedProduct */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfAllowedProduct), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfAllowedProduct");
  /* ArrayOfCustomerOrder */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCustomerOrder), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfCustomerOrder");
  /* CardData */
  RemClassRegistry()->RegisterXSClass(__classid(CardData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CardData");
  /* PaymentToken */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentToken), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentToken");
  /* SensitiveCardData */
  RemClassRegistry()->RegisterXSClass(__classid(SensitiveCardData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SensitiveCardData");
  /* AllowedProduct */
  RemClassRegistry()->RegisterXSClass(__classid(AllowedProduct), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AllowedProduct");
  /* CustomerOrder */
  RemClassRegistry()->RegisterXSClass(__classid(CustomerOrder), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CustomerOrder");
  /* TrackData */
  RemClassRegistry()->RegisterXSClass(__classid(TrackData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"TrackData");
  /* Geolocation */
  RemClassRegistry()->RegisterXSClass(__classid(Geolocation), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Geolocation");
  /* SensitiveMobileData */
  RemClassRegistry()->RegisterXSClass(__classid(SensitiveMobileData), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SensitiveMobileData");
  /* GeographicCoordinates */
  RemClassRegistry()->RegisterXSClass(__classid(GeographicCoordinates), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"GeographicCoordinates");
  /* UTMCoordinates */
  RemClassRegistry()->RegisterXSClass(__classid(UTMCoordinates), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"UTMCoordinates");
  /* SaleSoftware */
  RemClassRegistry()->RegisterXSClass(__classid(SaleSoftware2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleSoftware2", L"SaleSoftware");
  /* SaleTerminalData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleTerminalData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleTerminalData2", L"SaleTerminalData");
  /* SaleProfile */
  RemClassRegistry()->RegisterXSClass(__classid(SaleProfile2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleProfile2", L"SaleProfile");
  /* PaymentRequest */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentRequest2", L"PaymentRequest");
  /* PaymentData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentData2", L"PaymentData");
  /* PaymentTransaction */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentTransaction2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentTransaction2", L"PaymentTransaction");
  /* AmountsReq */
  RemClassRegistry()->RegisterXSClass(__classid(AmountsReq2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AmountsReq2", L"AmountsReq");
  /* SaleData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleData2", L"SaleData");
  /* SaleToIssuerData */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToIssuerData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleToIssuerData2", L"SaleToIssuerData");
  /* SaleTransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(SaleTransactionID2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SaleTransactionID2", L"SaleTransactionID");
  /* ReversalRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ReversalRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ReversalRequest2", L"ReversalRequest");
  /* OriginalPOITransaction */
  RemClassRegistry()->RegisterXSClass(__classid(OriginalPOITransaction2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OriginalPOITransaction2", L"OriginalPOITransaction");
  /* POITransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(POITransactionID2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"POITransactionID2", L"POITransactionID");
  /* Response */
  RemClassRegistry()->RegisterXSClass(__classid(Response2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Response2", L"Response");
  /* PaymentResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentResponse2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentResponse2", L"PaymentResponse");
  /* POIData */
  RemClassRegistry()->RegisterXSClass(__classid(POIData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"POIData2", L"POIData");
  /* PaymentReceipt */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentReceipt2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentReceipt2", L"PaymentReceipt");
  /* OutputContent */
  RemClassRegistry()->RegisterXSClass(__classid(OutputContent2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OutputContent2", L"OutputContent");
  /* OutputText */
  RemClassRegistry()->RegisterXSClass(__classid(OutputText2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"OutputText2", L"OutputText");
  /* PredefinedContent */
  RemClassRegistry()->RegisterXSClass(__classid(PredefinedContent2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PredefinedContent2", L"PredefinedContent");
  /* PaymentResult */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentResult2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentResult2", L"PaymentResult");
  /* AmountsResp */
  RemClassRegistry()->RegisterXSClass(__classid(AmountsResp2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AmountsResp2", L"AmountsResp");
  /* CapturedSignature */
  RemClassRegistry()->RegisterXSClass(__classid(CapturedSignature2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CapturedSignature2", L"CapturedSignature");
  /* AreaSize */
  RemClassRegistry()->RegisterXSClass(__classid(AreaSize2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AreaSize2", L"AreaSize");
  /* CurrencyConversion */
  RemClassRegistry()->RegisterXSClass(__classid(CurrencyConversion2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CurrencyConversion2", L"CurrencyConversion");
  /* ConvertedAmount */
  RemClassRegistry()->RegisterXSClass(__classid(ConvertedAmount2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ConvertedAmount2", L"ConvertedAmount");
  /* Instalment */
  RemClassRegistry()->RegisterXSClass(__classid(Instalment2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Instalment2", L"Instalment");
  /* PaymentAcquirerData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentAcquirerData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentAcquirerData2", L"PaymentAcquirerData");
  /* AcquirerTransactionID */
  RemClassRegistry()->RegisterXSClass(__classid(AcquirerTransactionID2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AcquirerTransactionID2", L"AcquirerTransactionID");
  /* PaymentInstrumentData */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentInstrumentData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentInstrumentData2", L"PaymentInstrumentData");
  /* CardData */
  RemClassRegistry()->RegisterXSClass(__classid(CardData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CardData2", L"CardData");
  /* AllowedProduct */
  RemClassRegistry()->RegisterXSClass(__classid(AllowedProduct2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"AllowedProduct2", L"AllowedProduct");
  /* CustomerOrder */
  RemClassRegistry()->RegisterXSClass(__classid(CustomerOrder2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CustomerOrder2", L"CustomerOrder");
  /* PaymentToken */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentToken2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"PaymentToken2", L"PaymentToken");
  /* SensitiveCardData */
  RemClassRegistry()->RegisterXSClass(__classid(SensitiveCardData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SensitiveCardData2", L"SensitiveCardData");
  /* TrackData */
  RemClassRegistry()->RegisterXSClass(__classid(TrackData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"TrackData2", L"TrackData");
  /* CheckData */
  RemClassRegistry()->RegisterXSClass(__classid(CheckData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"CheckData2", L"CheckData");
  /* MobileData */
  RemClassRegistry()->RegisterXSClass(__classid(MobileData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"MobileData2", L"MobileData");
  /* Geolocation */
  RemClassRegistry()->RegisterXSClass(__classid(Geolocation2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"Geolocation2", L"Geolocation");
  /* GeographicCoordinates */
  RemClassRegistry()->RegisterXSClass(__classid(GeographicCoordinates2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"GeographicCoordinates2", L"GeographicCoordinates");
  /* UTMCoordinates */
  RemClassRegistry()->RegisterXSClass(__classid(UTMCoordinates2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"UTMCoordinates2", L"UTMCoordinates");
  /* SensitiveMobileData */
  RemClassRegistry()->RegisterXSClass(__classid(SensitiveMobileData2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"SensitiveMobileData2", L"SensitiveMobileData");
  /* StoredValueAccountID */
  RemClassRegistry()->RegisterXSClass(__classid(StoredValueAccountID2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"StoredValueAccountID2", L"StoredValueAccountID");
  /* DiagnosisRequest */
  RemClassRegistry()->RegisterXSClass(__classid(DiagnosisRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"DiagnosisRequest");
  /* DiagnosisResponse */
  RemClassRegistry()->RegisterXSClass(__classid(DiagnosisResponse), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"DiagnosisResponse");
  /* POIStatus */
  RemClassRegistry()->RegisterXSClass(__classid(POIStatus), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"POIStatus");
  /* EventNotification */
  RemClassRegistry()->RegisterXSClass(__classid(EventNotification), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"EventNotification");
  /* DiagnosisRequest */
  RemClassRegistry()->RegisterXSClass(__classid(DiagnosisRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"DiagnosisRequest2", L"DiagnosisRequest");
  /* DiagnosisResponse */
  RemClassRegistry()->RegisterXSClass(__classid(DiagnosisResponse2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"DiagnosisResponse2", L"DiagnosisResponse");
  /* POIStatus */
  RemClassRegistry()->RegisterXSClass(__classid(POIStatus2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"POIStatus2", L"POIStatus");
  /* EventNotification */
  RemClassRegistry()->RegisterXSClass(__classid(EventNotification2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis", L"EventNotification2", L"EventNotification");
  /* LoginRequest */
  RemClassRegistry()->RegisterXSClass(__classid(LoginRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut", L"LoginRequest");
  /* LogoutRequest */
  RemClassRegistry()->RegisterXSClass(__classid(LogoutRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut", L"LogoutRequest");
  /* LoginRequest */
  RemClassRegistry()->RegisterXSClass(__classid(LoginRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut", L"LoginRequest2", L"LoginRequest");
  /* LogoutRequest */
  RemClassRegistry()->RegisterXSClass(__classid(LogoutRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut", L"LogoutRequest2", L"LogoutRequest");
  /* MessageHeader */
  RemClassRegistry()->RegisterXSClass(__classid(MessageHeader), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common", L"MessageHeader");
  /* ResourceDetails */
  RemClassRegistry()->RegisterXSClass(__classid(ResourceDetails), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common", L"ResourceDetails");
  /* MessageHeader */
  RemClassRegistry()->RegisterXSClass(__classid(MessageHeader2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common", L"MessageHeader2", L"MessageHeader");
  /* ResourceDetails */
  RemClassRegistry()->RegisterXSClass(__classid(ResourceDetails2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common", L"ResourceDetails2", L"ResourceDetails");
  /* Envelop */
  RemClassRegistry()->RegisterXSClass(__classid(Envelop), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop", L"Envelop");
  /* SaleToPOIRequest */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToPOIRequest), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop", L"SaleToPOIRequest");
  /* Envelop */
  RemClassRegistry()->RegisterXSClass(__classid(Envelop2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop", L"Envelop2", L"Envelop");
  /* SaleToPOIRequest */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToPOIRequest2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop", L"SaleToPOIRequest2", L"SaleToPOIRequest");
  /* SaleToPOIResponse */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToPOIResponse), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.ResponseEnvelop", L"SaleToPOIResponse");
  /* SaleToPOIResponse */
  RemClassRegistry()->RegisterXSClass(__classid(SaleToPOIResponse2), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.ResponseEnvelop", L"SaleToPOIResponse2", L"SaleToPOIResponse");
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* ArrayOfOutputText */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOutputText), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfOutputText");
  /* ArrayOfCustomerOrder */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCustomerOrder), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfCustomerOrder");
  /* ArrayOfPaymentReceipt */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPaymentReceipt), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfPaymentReceipt");
  /* ArrayOfAllowedProduct */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfAllowedProduct), L"http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain", L"ArrayOfAllowedProduct");
}
#pragma startup RegTypes 32

};     // NS__
