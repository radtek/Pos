// ---------------------------------------------------------------------------

#pragma hdrstop

#include "IMManager.h"
#include "POS_XMLBase.h"
#include "MMLogging.h"
#include "POS_XMLTransaction.h"
#include "PaymentTransaction.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TIMManager::TIMManager() {
	IM_Cache = NULL;
	POSID = 0;
	Enabled = false;
	Registered = false;
	OffLine = true;
	IP = "";
	Port = 0;
	ReadTimeOut = defaultReadTimeOut;
}

TIMManager::~TIMManager() {
	if (IM_Cache) {
		delete IM_Cache;
	}
}

void TIMManager::Initialise(AnsiString inIP, int inPort, int inPosID,
	int inReadTimeOut) {
	if (Registered) {
		IP = inIP;
		Port = inPort;
		ReadTimeOut = inReadTimeOut;
		POSID = inPosID;
		if (IP != "" && Port != 0) {
			if (IM_Cache) {
				IM_Cache->Terminate();
				IM_Cache->WaitFor();
				delete IM_Cache;
			}
			IM_Cache = new TIM_Transport_Cache(IP, Port, ReadTimeOut);
			Enabled = true;
			IM_Cache->Resume();
		}
		else {
			if (IM_Cache) {
				IM_Cache->Terminate();
				IM_Cache->WaitFor();
				delete IM_Cache;
			}
			IM_Cache = NULL;
			Enabled = false;
		}
		Sleep(1000);
		Status();
	}
}

void TIMManager::Status() {
	if (IM_Cache != NULL) {
		OffLine = IM_Cache->IM_TCPIP->OffLine;
		LastErrorMessage = IM_Cache->IM_TCPIP->ConnectionStatus;
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG, (OffLine ? "Interface Offline " : "Interface OnLine ") + LastErrorMessage);
	}
	else {
		Enabled = false;
		OffLine = true;
      TManagerLogs::Instance().Add(__FUNC__,INTAMATEINTERFACELOG,"Interface Offline No IM_Cache ");
	}
}

void TIMManager::Export(TPaymentTransaction &PaymentTransaction) {
	if (Enabled)
    {
        std::auto_ptr<TPOS_XMLTransaction>POSXML(new TPOS_XMLTransaction(PaymentTransaction.InvoiceNumber));
        POSXML->Build(PaymentTransaction);
        Export(*POSXML.get());
    }
}

void TIMManager::Export(TPOS_XMLBase &Data) {
	LastErrorMessage = "";
	if (Enabled) {
		if (Data.Cache) {
			if (!DirectoryExists(ExtractFilePath(Application->ExeName)
					+ "IntaMate\\Cache")) {
				ForceDirectories(ExtractFilePath(Application->ExeName)
					+ "IntaMate\\Cache");
			}
			UnicodeString FileName = Data.IntaMateID + _T(".xml");
			Data.Doc.SaveFile
			(UnicodeString(ExtractFilePath(Application->ExeName)
					+ "IntaMate\\Cache\\" + FileName).c_str());
		}
		else {
			std::auto_ptr<TIM_Transport_TCPIP>IM_TCPIP(new TIM_Transport_TCPIP);
			IM_TCPIP->Process(IP, Port, Data);
		}
	}
	else {
		Data.ResultText = "IntaMate Disabled.";
		LastErrorMessage = Data.ResultText;
	}
}
