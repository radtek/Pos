// ---------------------------------------------------------------------------

#pragma hdrstop

#include "WebMate.h"

#include <IdContext.hpp>
#include <Forms.hpp>
#include <XMLDoc.hpp>
#include "XMLConst.h"
#include "tinyxml.h"
#include "DBWebUtil.h"
#include "DBOrder.h"
#include "DBMenu.h"
#include "DBSaleTimes.h"
#include "DBContacts.h"
#include "GlobalSettings.h"
#include <SysUtils.hpp>
#include "DBGroups.h"
#include "WebProcUtil.h"
#include "ItemCompleteSub.h"
#include "DBTab.h"
#include <IOUtils.hpp>
#include "DBSecurity.h"
#include "enumMenu.h"
#include "XMLPrintJobmm.h"
#include "ChefmateClient.h"
#include "MMMessageBox.h"

#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdContext"
#pragma link "IdCustomTCPServer"
#pragma link "IdTCPServer"

TWebMate::TWebMate() : fTCPServer(new TIdTCPServer(NULL))
{
}

TWebMate::~TWebMate()
{
}

void TWebMate::Initialise(bool inUserEnabled, UnicodeString inBasePath, UnicodeString inDBIP, UnicodeString inDBPath, int inWebPort)
{
	try
	{
		DBIP = inDBIP;
		DBPath = inDBPath;

		BasePath = inBasePath;
		WebDir = BasePath + "Import\\WebOrders";
		CacheDir = WebDir + "\\Cache";
		FailedDir = WebDir + "\\Failed"; ;
		ProcessedDir = WebDir + "\\Processed"; ;

		if (!DirectoryExists(WebDir))
		{
			ForceDirectories(WebDir);
		}

		if (!DirectoryExists(CacheDir))
		{
			ForceDirectories(CacheDir);
		}

		if (!DirectoryExists(FailedDir))
		{
			ForceDirectories(FailedDir);
		}

		if (!DirectoryExists(ProcessedDir))
		{
			ForceDirectories(ProcessedDir);
		}

		Enabled = false;
		if (inUserEnabled)
		{
			if (DBIP != "" && DBPath != "")
			{
				fTCPServer->DefaultPort = inWebPort;
				fTCPServer->OnExecute = OnExecute;
				fTCPServer->OnConnect = OnConnect;
				fTCPServer->OnDisconnect = OnDisconnect;
				fTCPServer->Active = true;
				Enabled = true;
				TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Enabled.");
			}
			else
			{
				TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Disabled. No DB Path or IP Address");
			}
		}
		else
		{
			fTCPServer->Active = false;
		}
	}
	catch(Exception & E)
	{
		Enabled = false;
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Initialise Failed.");
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "WebMate Initialise Failed.");
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

void __fastcall TWebMate::OnConnect(TIdContext *AContext)
{
	UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
	TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Client Connected." + PeerIP);
}

void __fastcall TWebMate::OnDisconnect(TIdContext *AContext)
{
	UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
	TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Client Disconnected." + PeerIP);
}

void __fastcall TWebMate::OnExecute(TIdContext *AContext)
{
	try
	{
		UnicodeString PeerIP = AContext->Connection->Socket->Binding->PeerIP;
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebMate Client Executing." + PeerIP);

		AnsiString Data = "";
		AContext->Connection->IOHandler->CheckForDataOnSource(100);
		AContext->Connection->IOHandler->ReadTimeout = 2000;

		TByteDynArray Incomming;
		AContext->Connection->IOHandler->ReadBytes(Incomming, 10, false);
		Data = AnsiString((char*) & Incomming[0], 10);
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Read Size: " + Data);
		int DataLength = StrToIntDef(Data, 0);
		Data = "";
		if (DataLength > 0)
		{
			Data = AContext->Connection->IOHandler->ReadString(DataLength);
		}
		else
		{
			AContext->Connection->IOHandler->ReadBytes(Incomming, -1, true);
			Data = AnsiString((char*) & Incomming[0], Incomming.Length);
		}

		// TEncoding Encoding = TEncoding.ASCII();
		/* bool WaitedLongEnough = false;
		while (!AContext->Connection->IOHandler->InputBufferIsEmpty() || !WaitedLongEnough)
		{
		Data += AContext->Connection->IOHandler->ReadLn(TEncoding::ASCII);
		if(AContext->Connection->IOHandler->InputBufferIsEmpty() && AContext->Connection->IOHandler->CheckForDataOnSource(100))
		{
		WaitedLongEnough = true;
		}
		} */

		if (Data != "")
		{
			AnsiString FileName = CacheDir + "\\" + Now().FormatString("yyyy-mm-dd - hh-nn-ss") + "--" + "WO-IN.XML";
			if (DirectoryExists(ExtractFilePath(FileName)))
			{
				// Reload and remove MobieToGo's Header's addby by there cURL.
				// POST /Mobi2Go/api/1.2/tests/unit_tests/pos/menumate/test HTTP/1.1
				// Host: localhost
				// Accept: */*
				// Content-type: text/xml
				// Content-length: 843
				std::auto_ptr<TMemoryStream>Stream(new TMemoryStream);
				Stream->Write(Data.c_str(), Data.Length());
				Stream->SaveToFile(FileName);

				// Strip any Data before the XML.
				int StartIndex = Data.Pos("<?xml version") - 1;
				int Length = Stream->Size - StartIndex;
				Stream->Position = StartIndex;
				std::auto_ptr<TMemoryStream>FileStream(new TMemoryStream());
				FileStream->CopyFrom(Stream.get(), Length);
				FileStream->SaveToFile(FileName);
			}

			WebMsgType Type = GetMsgType(Data);
			switch(Type)
			{
			case WebMsgOrder:
				ProcessWebMsgOrder(AContext, FileName);
				break;
			case WebMsgCallAway:
				ProcessWebMsgCallAway(AContext, FileName);
				break;
			case WebMsgReceipt:
				ProcessWebMsgReceipt(AContext, FileName);
				break;
			case WebMsgPrintJob:
				ProcessWebMsgPrintJob(AContext, FileName);
				break;
			default:
				{
					AnsiString ErrMsg = "MenuMate Error Unknow Web Message";
					Data = BuildXMLWebOrdersResponce("Unknown", xmlResultErr, ErrMsg);
					AContext->Connection->IOHandler->WriteLn(Data);

					TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
					if (DirectoryExists(FailedDir))
					{
						TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
					}
					DeleteFile(FileName);
				}
			}
		}
		AContext->Connection->Disconnect();
	}
	catch(Exception & E)
	{
		AContext->Connection->Disconnect();
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}

WebMsgType TWebMate::GetMsgType(AnsiString Data)
{
	if (Data.Pos("<ORDER GUID") != 0)
	{
		return WebMsgOrder;
	}
	else if (Data.Pos("<CALLAWAY>") != 0)
	{
		return WebMsgCallAway;
	}
	else if (Data.Pos("<RECEIPT>") != 0)
	{
		return WebMsgReceipt;
	}
	else if (Data.Pos("<PRINTJOB") != 0)
	{
		return WebMsgPrintJob;
	}
	return WebMsgUnknown;
}

void TWebMate::ProcessWebMsgOrder(TIdContext *AContext, UnicodeString FileName)
{
	Database::TDBControl DBControl;
	try
	{
		DBControl.Disconnect();
		DBControl.Init(Database::TDBSettings(DBIP, DBPath, false));
		DBControl.Connect();

		try
		{
			CoInitialize(NULL);
			_di_IXMLORDERType WebOrder = LoadORDER(FileName);
			if (WebOrder != NULL)
			{
				TDateTime OrderReceived = Now();
				AnsiString CacheFileNameINID = OrderReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + WebOrder->GUID + "-" + "WO-IN.XML";
				AnsiString CacheFileNameOUTID = OrderReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + WebOrder->GUID + "-" + "WO-OUT.XML";

				if (DirectoryExists(CacheDir))
				{
					TFile::Copy(FileName, ExtractFilePath(FileName) + CacheFileNameINID);
					if (CacheFileNameINID != ExtractFileName(FileName))
					{
						DeleteFile(FileName);
						FileName = CacheDir + "\\" + CacheFileNameINID;
					}
				}

				AnsiString ErrMsg = "Ok";
				XMLStatusCodes Status = ProcessWebOrders(DBControl, WebOrder, ErrMsg);
				AnsiString Data = BuildXMLWebOrdersResponce(WebOrder->GUID, Status, ErrMsg);

				std::auto_ptr<TMemoryStream>RespStream(new TMemoryStream);
				RespStream->Write(Data.c_str(), Data.Length());

				switch(Status)
				{
				case xmlResultOK:
				case xmlResultWarn:
					{
						TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebOrders Accepted " + WebOrder->GUID);
						if (DirectoryExists(ProcessedDir))
						{
							TFile::Copy(FileName, ProcessedDir + "\\" + ExtractFileName(FileName));
							RespStream->SaveToFile(ProcessedDir + "\\" + CacheFileNameOUTID);
						}
						DeleteFile(FileName);
						PostMessage(Application->Handle, WEBORDER_STATUSCHANGED, (UINT)0, WEBORDER_RECEIVED);
					}break;
				case xmlResultErr:
					{
						if (DirectoryExists(FailedDir))
						{
							TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
							RespStream->SaveToFile(FailedDir + "\\" + CacheFileNameOUTID);
						}
						DeleteFile(FileName);
						TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "WebOrders Rejected " + ErrMsg + " " + WebOrder->GUID);
						PostMessage(Application->Handle, WEBORDER_STATUSCHANGED, (UINT)0, WEBORDER_REJECTED);
					}
				}

				if (StrToBoolDef(WebOrder->Get_RESPOND(), false))
				{
					AContext->Connection->IOHandler->WriteLn(Data);
				}
			}
			else
			{
				AnsiString ErrMsg = "MenuMate Unable to Load WebOrder";
				AnsiString Data = BuildXMLWebOrdersResponce(WebOrder->GUID, xmlResultErr, ErrMsg);
				AContext->Connection->IOHandler->WriteLn(Data);

				TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
				TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
				if (DirectoryExists(FailedDir))
				{
					TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
				}
				DeleteFile(FileName);
			}
		}
		catch(Exception & E)
		{
			AnsiString ErrMsg = "MenuMate Error Processing WebOrder " + E.Message;
			AnsiString Data = BuildXMLWebOrdersResponce("Unknown", xmlResultErr, ErrMsg);
			AContext->Connection->IOHandler->WriteLn(Data);

			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
			if (DirectoryExists(FailedDir))
			{
				TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
			}
			DeleteFile(FileName);
		}
		DBControl.Disconnect();
	}
	catch(Exception & E)
	{
		AnsiString ErrMsg = "MenuMate Web Interface unable to connect to DB";
		AnsiString Data = BuildXMLWebOrdersResponce("Unknown", xmlResultErr, ErrMsg);
		AContext->Connection->IOHandler->WriteLn(Data);

		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
		if (DirectoryExists(FailedDir))
		{
			TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
		}
		DeleteFile(FileName);
	}
}

void TWebMate::ProcessWebMsgCallAway(TIdContext *AContext, UnicodeString FileName)
{
	Database::TDBControl DBControl;
	try
	{
		DBControl.Disconnect();
		DBControl.Init(Database::TDBSettings(DBIP, DBPath, false));
		DBControl.Connect();

		try
		{
			CoInitialize(NULL);
			XMLCallAways::_di_IXMLCALLAWAYType WebCallAway = XMLCallAways::LoadCALLAWAY(FileName);
			if (WebCallAway != NULL)
			{
				TDateTime TimeReceived = Now();
				AnsiString CacheFileNameINID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-CallAway-" + "WO-IN.XML";
				AnsiString CacheFileNameOUTID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-CallAway-" + "WO-OUT.XML";

				if (DirectoryExists(CacheDir))
				{
					TFile::Copy(FileName, ExtractFilePath(FileName) + CacheFileNameINID);
					if (CacheFileNameINID != ExtractFileName(FileName))
					{
						DeleteFile(FileName);
						FileName = CacheDir + "\\" + CacheFileNameINID;
					}
				}

				// Load up the Account info
				XMLCallAways::_di_IXMLACCOUNTType Account = WebCallAway->Get_ACCOUNT();
				XMLCallAways::_di_IXMLACCOUNTIDENTType AccIdent = Account->Get_ACCOUNTIDENT();
				if (AccIdent != NULL)
				{
					switch(AccIdent->Get_TYPE())
					{
					case 0: // Tables
						{
							int TableNo = AccIdent->TABLE;
							std::auto_ptr<TStringList>CourseList(new TStringList);
							XMLCallAways::_di_IXMLSERVINGCOURSETypeList ServingCourses = Account->Get_SERVINGCOURSE();
							for (int a = 0; a < ServingCourses->Count; a++)
							{
								XMLCallAways::_di_IXMLSERVINGCOURSEType ServingCourse = ServingCourses->Get_Item(a);
								CourseList->Add(ServingCourse->Get_NAME());
							}
							Database::TDBTransaction DBTransaction(DBControl);
							DBTransaction.StartTransaction();
							TWebProcUtil::ProcessCallAway(DBTransaction, TableNo, CourseList.get());
							DBTransaction.Commit();

							AnsiString Data = BuildXMLWebOrdersResponce("CallAway", xmlResultOK, "Ok");
							std::auto_ptr<TMemoryStream>RespStream(new TMemoryStream);
							RespStream->Write(Data.c_str(), Data.Length());
							AContext->Connection->IOHandler->WriteLn(Data);

							TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "CallAway Processed ");
							if (DirectoryExists(ProcessedDir))
							{
								TFile::Copy(FileName, ProcessedDir + "\\" + ExtractFileName(FileName));
								RespStream->SaveToFile(ProcessedDir + "\\" + CacheFileNameOUTID);
							}
							DeleteFile(FileName);
						}break;
					case 1: // Tabs
					case 2: // Members
					case 3: // Rooms
					case 4: // Web
					case 5: // Staff
					default:
						{
							AnsiString ErrMsg = "CallAway is Tables Only";
							AnsiString Data = BuildXMLWebOrdersResponce("CallAway", xmlResultErr, ErrMsg);
							AContext->Connection->IOHandler->WriteLn(Data);

							TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
							TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
							DeleteFile(FileName);
						}break;
					}
				}
			}
		}
		catch(Exception & E)
		{
			AnsiString ErrMsg = "MenuMate Error Processing CallAway " + E.Message;
			AnsiString Data = BuildXMLWebOrdersResponce("CallAway", xmlResultErr, ErrMsg);
			AContext->Connection->IOHandler->WriteLn(Data);

			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
			if (DirectoryExists(FailedDir))
			{
				TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
			}
			DeleteFile(FileName);
		}
		DBControl.Disconnect();
	}
	catch(Exception & E)
	{
		AnsiString ErrMsg = "MenuMate Web Interface unable to connect to DB";
		AnsiString Data = BuildXMLWebOrdersResponce("CallAway", xmlResultErr, ErrMsg);
		AContext->Connection->IOHandler->WriteLn(Data);

		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
		if (DirectoryExists(FailedDir))
		{
			TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
		}
		DeleteFile(FileName);
	}
	AContext->Connection->Disconnect();
}

void TWebMate::ProcessWebMsgReceipt(TIdContext *AContext, UnicodeString FileName)
{
	Database::TDBControl DBControl;
	try
	{
		DBControl.Disconnect();
		DBControl.Init(Database::TDBSettings(DBIP, DBPath, false));
		DBControl.Connect();

		try
		{
			CoInitialize(NULL);
			XMLReceipt::_di_IXMLRECEIPTType WebRecipt = XMLReceipt::LoadRECEIPT(FileName);
			if (WebRecipt != NULL)
			{
				TDateTime TimeReceived = Now();
				AnsiString CacheFileNameINID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-Receipt-" + "WO-IN.XML";
				AnsiString CacheFileNameOUTID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-Receipt-" + "WO-OUT.XML";

				if (DirectoryExists(CacheDir))
				{
					TFile::Copy(FileName, ExtractFilePath(FileName) + CacheFileNameINID);
					if (CacheFileNameINID != ExtractFileName(FileName))
					{
						DeleteFile(FileName);
						FileName = CacheDir + "\\" + CacheFileNameINID;
					}
				}

				// Load up the Account info
				XMLReceipt::_di_IXMLACCOUNTIDENTType AccIdent = WebRecipt->Get_ACCOUNTIDENT();
				if (AccIdent != NULL)
				{
					Database::TDBTransaction DBTransaction(DBControl);
					DBTransaction.StartTransaction();
					TMMTabType TabType = TabNone;
					int Primary = 0;
					switch(AccIdent->Get_TYPE())
					{
					case 0: // Tables
						{
							TabType = TabTableSeat;
							Primary = AccIdent->TABLE;
						}break;
					case 1: // Tabs
						TabType = TabNormal;
						Primary = AccIdent->TAB;
						break;
					case 2: // Members
						TabType = TabMember;
						Primary = AccIdent->MEMBER;
						break;
					case 3: // Rooms
						TabType = TabRoom;
						Primary = AccIdent->ROOM;
						break;
					case 4: // Web
						TabType = TabWeb;
						break;
					case 5: // Staff
						TabType = TabStaff;
						Primary = AccIdent->STAFF;
						break;
					default:
						{

						}break;
					}

					if (Primary != 0)
					{
						TWebProcUtil::ProcessReceipt(DBTransaction, TabType, Primary, 0);

						AnsiString Data = BuildXMLWebOrdersResponce("Receipt", xmlResultOK, "Ok");
						std::auto_ptr<TMemoryStream>RespStream(new TMemoryStream);
						RespStream->Write(Data.c_str(), Data.Length());
						AContext->Connection->IOHandler->WriteLn(Data);

						TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Receipt Processed ");
						if (DirectoryExists(ProcessedDir))
						{
							TFile::Copy(FileName, ProcessedDir + "\\" + ExtractFileName(FileName));
							RespStream->SaveToFile(ProcessedDir + "\\" + CacheFileNameOUTID);
						}
					}
					else
					{
						AnsiString ErrMsg = "Receipt is Account Type Not Found";
						AnsiString Data = BuildXMLWebOrdersResponce("Receipt", xmlResultErr, ErrMsg);
						AContext->Connection->IOHandler->WriteLn(Data);

						if (DirectoryExists(FailedDir))
						{
							TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
						}

						TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
						TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
					}
					DeleteFile(FileName);
					DBTransaction.Commit();
				}
			}
		}
		catch(Exception & E)
		{
			AnsiString ErrMsg = "MenuMate Error Processing Receipt " + E.Message;
			AnsiString Data = BuildXMLWebOrdersResponce("Receipt", xmlResultErr, ErrMsg);
			AContext->Connection->IOHandler->WriteLn(Data);

			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
			if (DirectoryExists(FailedDir))
			{
				TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
			}
			DeleteFile(FileName);
		}
		DBControl.Disconnect();
	}
	catch(Exception & E)
	{
		AnsiString ErrMsg = "MenuMate Web Interface unable to connect to DB";
		AnsiString Data = BuildXMLWebOrdersResponce("Receipt", xmlResultErr, ErrMsg);
		AContext->Connection->IOHandler->WriteLn(Data);

		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
		if (DirectoryExists(FailedDir))
		{
			TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
		}
		DeleteFile(FileName);
	}
	AContext->Connection->Disconnect();
}

void TWebMate::ProcessWebMsgPrintJob(TIdContext *AContext, UnicodeString FileName)
{
	Database::TDBControl DBControl;
	try
	{
		DBControl.Disconnect();
		DBControl.Init(Database::TDBSettings(DBIP, DBPath, false));
		DBControl.Connect();

		try
		{
			CoInitialize(NULL);
			XMLPrintJob::_di_IXMLPRINTJOBType PrintJob = XMLPrintJob::LoadPRINTJOB(FileName);
			if (PrintJob != NULL)
			{
				TDateTime TimeReceived = Now();
				UnicodeString TransNumber = PrintJob->Get_TRANSNO();
				AnsiString CacheFileNameINID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + TransNumber + "-PrintJob-" + "WO-IN.XML";
				AnsiString CacheFileNameOUTID = TimeReceived.FormatString("yyyy-mm-dd - hh-nn-ss") + "-" + TransNumber + "-PrintJob-" + "WO-OUT.XML";

				if (DirectoryExists(CacheDir))
				{
					TFile::Copy(FileName, ExtractFilePath(FileName) + CacheFileNameINID);
					if (CacheFileNameINID != ExtractFileName(FileName))
					{
						DeleteFile(FileName);
						FileName = CacheDir + "\\" + CacheFileNameINID;
					}
				}

				UnicodeString Name = PrintJob->Get_NAME();
                UnicodeString DeviceName = PrintJob->Get_DEVICENAME();

				std::vector<UnicodeString>GUIDS;
				for (int i = 0; i < PrintJob->GetCount(); i++)
				{
					UnicodeString OrderGUILD = PrintJob->Get_OrderGUID(i);
					GUIDS.push_back(OrderGUILD);
				}

				if (GUIDS.size() > 0)
				{
					Database::TDBTransaction DBTransaction(DBControl);
					DBTransaction.StartTransaction();

					TWebProcUtil::ProcessPrintJob(DBTransaction, TransNumber,DeviceName, GUIDS);

					AnsiString Data = BuildXMLWebOrdersResponce("PrintJob", xmlResultOK, "Ok");
					std::auto_ptr<TMemoryStream>RespStream(new TMemoryStream);
					RespStream->Write(Data.c_str(), Data.Length());
					AContext->Connection->IOHandler->WriteLn(Data);

					TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "PrintJob Processed ");
					if (DirectoryExists(ProcessedDir))
					{
						TFile::Copy(FileName, ProcessedDir + "\\" + ExtractFileName(FileName));
						RespStream->SaveToFile(ProcessedDir + "\\" + CacheFileNameOUTID);
					}
					DBTransaction.Commit();
				}
				else
				{
					AnsiString ErrMsg = "Empty Print Job";
					AnsiString Data = BuildXMLWebOrdersResponce("PrintJob", xmlResultOK, ErrMsg);
					AContext->Connection->IOHandler->WriteLn(Data);

					if (DirectoryExists(FailedDir))
					{
						TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
					}
					TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
				}
				DeleteFile(FileName);
			}
		}
		catch(Exception & E)
		{
			AnsiString ErrMsg = "MenuMate Error Processing PrintJob " + E.Message;
			AnsiString Data = BuildXMLWebOrdersResponce("PrintJob", xmlResultErr, ErrMsg);
			AContext->Connection->IOHandler->WriteLn(Data);

			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
			TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
			if (DirectoryExists(FailedDir))
			{
				TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
			}
			DeleteFile(FileName);
		}
		DBControl.Disconnect();
	}
	catch(Exception & E)
	{
		AnsiString ErrMsg = "MenuMate Web Interface unable to connect to DB";
		AnsiString Data = BuildXMLWebOrdersResponce("Receipt", xmlResultErr, ErrMsg);
		AContext->Connection->IOHandler->WriteLn(Data);

		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, ErrMsg);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, ErrMsg);
		if (DirectoryExists(FailedDir))
		{
			TFile::Copy(FileName, FailedDir + "\\" + ExtractFileName(FileName));
		}
		DeleteFile(FileName);
	}
	AContext->Connection->Disconnect();
}

AnsiString __fastcall TWebMate::BuildXMLWebOrdersResponce(UnicodeString GUID, XMLStatusCodes Status, AnsiString ErrMsg)
{
	AnsiString RetVal = "";
	TiXmlDocument Doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration(_T("1.0"), _T("UTF-8"), _T(""));
	Doc.LinkEndChild(decl);

	TiXmlElement * xmlOrderStatus = new TiXmlElement(xmlEleOrdersResult);
	xmlOrderStatus->SetAttribute(xmlAttrOrderID, AnsiString(GUID).c_str());

	switch(Status)
	{
	case xmlResultErr:
		xmlOrderStatus->SetAttribute(xmlAttrStatus, 0);
		break;
	case xmlResultWarn:
		xmlOrderStatus->SetAttribute(xmlAttrStatus, 128);
		break;
	case xmlResultOK:
		xmlOrderStatus->SetAttribute(xmlAttrStatus, 255);
		break;
	}
	xmlOrderStatus->SetAttribute(xmlMesg, ErrMsg.c_str());
	Doc.LinkEndChild(xmlOrderStatus);

	TiXmlPrinter printer;
	printer.SetIndent(_T("\t"));
	Doc.Accept(&printer);
	RetVal = printer.CStr();
	return RetVal;
}

XMLStatusCodes __fastcall TWebMate::ProcessWebOrders(Database::TDBControl &DBControl, _di_IXMLORDERType &WebOrder, AnsiString &ErrMsg)
{
	XMLStatusCodes RetVal = xmlResultOK;
	std::auto_ptr<TList>CompleteOrderList(new TList());
	try
	{
		if (WebOrder->GUID == "")
		{
			throw Exception("WebOrder ID Required.", xmlResultErr);
		}
		// Begin DB Transaction.
		Database::TDBTransaction DBTransaction(DBControl);
		DBTransaction.StartTransaction();
		if (TDBWebUtil::GetKeyByOrderGUID(DBTransaction, WebOrder->Get_GUID()) != 0)
		{ // Check for Duplication
			throw Exception("Duplicate Order Detected", xmlResultWarn);
		}

		// Check for Duplications
		bool AllDups = true;

		int CurrentTimeKey = 0;
		// Get the Order Type
		_di_IXMLACCOUNTType Account = WebOrder->Get_ACCOUNT();
		eTransactionType TransType = eWeb;
		int DefaultCovers = 1;
        int WebKey = 0;
        std::auto_ptr<TList>OrderList(new TList());

        // Pull the covers out of the XML.
        // Used if no Cover Value set on Table etc.
        if (Account->HasAttribute("COVERS"))
            DefaultCovers = Account->Get_COVERS();

        TransType = eWeb;
        // ORDER_NAME Depricated use Account Name attribute instead.
        if (Account->Get_NAME() != "")
        {
            WebOrder->Set_ORDER_NAME(Account->Get_NAME());
        }
        else if (WebOrder->Get_ORDER_NAME() != "")
        {
            WebOrder->Set_ORDER_NAME(WebOrder->Get_ORDER_NAME());
        }

        WebKey = TDBWebUtil::CreateWebOrderFromXML(DBTransaction, WebOrder);

        TMMContactInfo Member;
        Member.MembershipNumber = Account->Get_MEMBER_NUMBER();
        ProcessMembership(DBTransaction, Member);
        if (Member.Valid())
        {
            TDBWebUtil::SetWebOrderContactKey(DBTransaction, WebKey, Member.ContactKey);
        }

        _di_IXMLPRODUCTTypeList ProductList = Account->Get_PRODUCT();
        for (int j = 0; j < ProductList->Count; j++)
        {
            _di_IXMLPRODUCTType Product = ProductList->Get_Item(j);
            AddProduct(DBTransaction, WebKey, WebOrder->Get_GUID(), Account, OrderList.get(), Product, Member);
        }

        if (CurrentTimeKey == 0)
        {
            CurrentTimeKey = TDBSaleTimes::OpenSaleStartTime(DBTransaction, CurrentTimeKey);
            TDBSaleTimes::CloseSaleStartTime(DBTransaction, CurrentTimeKey);
            TDBWebUtil::SetWebOrderTimeKey(DBTransaction, WebKey, CurrentTimeKey);
        }

        int SecRefNumber = TDBSecurity::GetNextSecurityRef(DBTransaction);
        // Double Check Serving courses.
        // Assign Membership.
        // Assign Security Ref.
        TServingCourse DefaultWebServingCourse = TDBMenu::LoadDefaultServingCourse(DBTransaction);
        for (int o = 0; o < OrderList->Count; o++)
        {
            TItemComplete *Order = (TItemComplete*)OrderList->Items[o];
            // Complete Order Security.
            Order->Security->SetSecurityRefNumber(SecRefNumber);
            ApplyMembership(DBTransaction, Member, Order);
            if (Order->ServingCourse.ServingCourseKey < 1)
            {
                Order->ServingCourse = DefaultWebServingCourse;
                TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Order has no Serving Course");
            }
            for (int j = 0; j < Order->SubOrders->Count; j++)
            {
                TItemCompleteSub *SubOrder = Order->SubOrders->SubOrderGet(j);
                if (SubOrder)
                {
                    SubOrder->Security->SetSecurityRefNumber(SecRefNumber);
                }
            }
        }

        // Place the Orders in the DB.
        TDBOrder::ProcessOrders(DBTransaction, OrderList.get());

        for (int o = 0; o < OrderList->Count; o++)
        {
            TItemComplete *Order = (TItemComplete*)OrderList->Items[o];
            if (Order->DuplicateDetected)
            {
                ErrMsg = "MenuMate Warning Duplicate Order Detected";
                RetVal = xmlResultWarn;
            }
            else
            {
                AllDups = false;
            }
        }

        if (AllDups)
        {
            // Remove the Weborder.
            TDBWebUtil::DeleteWebOrder(DBTransaction, WebKey);
            ErrMsg = "MenuMate Warning Order Already Received";
            RetVal = xmlResultWarn;
        }

        CompleteOrderList->Assign(OrderList.get(), laOr);

        if (!AllDups)
        {
            TMMContactInfo Staff;
            // Find the Staff Member
            _di_IXMLFROMType From = WebOrder->Get_FROM();
            if (From != Null)
            {
                int ContactKey = StrToIntDef(From->Get_STAFF_ID(), -1);
                if (ContactKey != -1)
                {
                    TDBContacts::GetContactDetails(DBTransaction, ContactKey, Staff);
                    if (Staff.ContactKey == 0)
                    {
                        Staff.Name = From->Get_STAFF_NAME();
                    }
                }
                else
                {
                    Staff.Name = From->Get_STAFF_NAME();
                }
            }
            if(TGlobalSettings::Instance().AutoAcceptWebOrders)
            {
	            TWebProcUtil::ProcessWebOrder(DBTransaction, TransType, DefaultCovers, Staff, OrderList.get(), Member, CurrentTimeKey, WebKey);
            	TDBWebUtil::SetWebOrderStatus(DBTransaction, WebKey, ewosProcessed);
            }
        }

		DBTransaction.Commit();

		for (int o = 0; o < CompleteOrderList->Count; o++)
		{
			TItemComplete *Order = (TItemComplete*)CompleteOrderList->Items[o];
			delete Order;
		}

	}
	catch(Exception & E)
	{
		RetVal = E.HelpContext;
		ErrMsg = "MenuMate Error Contact MenuMate support Quote: " + E.Message;
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		// Clean up any orders.
		for (int o = 0; o < CompleteOrderList->Count; o++)
		{
			TItemComplete *Order = (TItemComplete*)CompleteOrderList->Items[o];
			delete Order;
		}
	}
	return RetVal;
}

__fastcall TWebMate::ApplyMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo Member, TItemComplete *Order)
{
	Order->Loyalty_Key = Member.ContactKey;
	// Included for Heppy our with members only.
	// If its happy hour now adding members means we need
	// to recalculate the price.

	// Order->ResetPrice();

	// Calculate Members Freebie rewards.
	// Rewards do not cascade though sides, which is the normal discount functionality.
	/*
	if (!Order->IsFreebe && ManagerFreebie->IsFree(DBTransaction, Member, Order->Item, Order->Size,Order->MemberFreeSaleCount))
	{
	Order->IsFreebe = true;
	Order->FreeSource = eFreeLocation;

	if (Order->Security->GetSecurityRefNumber() == 0)
	{
	Order->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
	}
	TSecurityReference *SecRef = new TSecurityReference;
	SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
	SecRef->Event = SecurityTypes[secLocationDiscount];
	SecRef->From = CurrToStrF(Order->TotalPriceAdjustment(), ffCurrency, 2);

	TDiscount RewardDiscount;
	RewardDiscount.DiscountKey = -1;
	RewardDiscount.Priority = -1;
	RewardDiscount.Description = "Membership Reward";
	RewardDiscount.Name = "Member Reward";
	RewardDiscount.Source = dsMMMembershipReward;
	RewardDiscount.Mode = DiscModePercent;
	RewardDiscount.PercentAmount = Order->MemberFreeSaleDiscount;
	RewardDiscount.Rounding = MIN_CURRENCY_VALUE;
	Order->DiscountAdd(RewardDiscount);

	SecRef->To = CurrToStrF(Order->TotalPriceAdjustment(), ffCurrency, 2);
	SecRef->Note = "Discount " + FloatToStr(Order->LocationFreeSaleDiscount) + "%";
	SecRef->TimeStamp = Now();
	Order->Security->SecurityAdd(SecRef);
	} // If its a Members freebe then

	// Sort out the Sides.
	for (int j = 0; j < Order->SubOrders->Count; j++)
	{
	TItemMinorComplete *CurrentSubOrder = (TItemMinorComplete*)Order->SubOrders->Items[j];
	CurrentSubOrder->Loyalty_Key =  Member.ContactKey;

	// Included for Heppy our with members only.
	// If its happy hour now adding members means we need
	// to recalculate the price.
	CurrentSubOrder->ResetPrice();
	// Calculate Members Freebie rewards.
	if (!CurrentSubOrder->IsFreebe && SeatOrders[SeatsToApply[iSeat]]->Orders->AppliedMembership.ContactKey != 0 &&
	ManagerFreebie->IsFree(DBTransaction, &SeatOrders[SeatsToApply[iSeat]]->Orders->AppliedMembership,
	CurrentSubOrder->Item, CurrentSubOrder->Size, CurrentSubOrder->MemberFreeSaleCount))
	{
	// TODO : Tidy this up is a massive dupliation of code... either orders should should the membership or vis versa but it needs to be handled better than this!!!
	CurrentSubOrder->IsFreebe = true;
	CurrentSubOrder->FreeSource = eFreeMember;

	if (CurrentSubOrder->Security->GetSecurityRefNumber() == 0)
	{
	CurrentSubOrder->Security->SetSecurityRefNumber(TDBSecurity::GetNextSecurityRef(DBTransaction));
	}
	// Add Security.
	TSecurityReference *SecRef = new TSecurityReference;
	SecRef->UserKey = TDeviceRealTerminal::Instance().User.ContactKey;
	SecRef->Event = SecurityTypes[secMemberDiscount];
	SecRef->From = CurrToStrF(CurrentSubOrder->TotalPriceAdjustment(), ffCurrency, 2);

	TDiscount RewardDiscount;
	RewardDiscount.DiscountKey = -1;
	RewardDiscount.Priority = -1;
	RewardDiscount.Description = "Membership Reward";
	RewardDiscount.Name = "Member Reward";
	RewardDiscount.Source = dsMMMembershipReward;
	RewardDiscount.Mode = DiscModePercent;
	RewardDiscount.PercentAmount = CurrentSubOrder->MemberFreeSaleDiscount;
	RewardDiscount.Rounding = MIN_CURRENCY_VALUE;
	CurrentSubOrder->DiscountAdd(RewardDiscount);

	SecRef->To = CurrToStrF(CurrentSubOrder->TotalPriceAdjustment(), ffCurrency, 2);
	SecRef->Note = "Discount " + FloatToStr(CurrentSubOrder->MemberFreeSaleDiscount) + "%";
	SecRef->TimeStamp = Now();
	CurrentSubOrder->Security->SecurityAdd(SecRef);
	}
	}
	 */

	/*
	// Apply Member Specific Discounts.
	for (std::set <int> ::iterator ptrDiscountKey = Member.AutoAppliedDiscounts.begin();
	ptrDiscountKey != Member.AutoAppliedDiscounts.end(); ptrDiscountKey++)
	{
	ApplyDiscount(DBTransaction, *ptrDiscountKey, SeatOrders[SeatsToApply[iSeat]]->Orders->List, dsMMMembership);
	} */
}

__fastcall TWebMate::ProcessMembership(Database::TDBTransaction &DBTransaction, TMMContactInfo &Member)
{
	try
	{
		bool MemberFound = false;
		if (Member.ContactKey != 0 && TDBContacts::GetContactExists(DBTransaction, Member.ContactKey))
		{
			// Load the Member.
			TDBContacts::GetContactDetails(DBTransaction, Member.ContactKey, Member);
			MemberFound = true;
			// If you cant find the member then no membership.
		}
		else if (Member.MembershipNumber != "")
		{
			// Get the Member
			int MemberKey = TDBContacts::GetContactByMemberNumberSiteID(DBTransaction, Member.MembershipNumber, Member.SiteID);
			if (MemberKey != 0 && TDBContacts::GetContactExists(DBTransaction, MemberKey))
			{
				TDBContacts::GetContactDetails(DBTransaction, MemberKey, Member);
				MemberFound = true;
			}
		}
		else if(Member.EMail != "")
		{
			Member.ContactKey = TDBContacts::GetContactByEmail(DBTransaction, Member.EMail);
			if (Member.ContactKey != 0 && TDBContacts::GetContactExists(DBTransaction, Member.ContactKey))
			{
				TDBContacts::GetContactDetails(DBTransaction, Member.ContactKey, Member);
				MemberFound = true;
			}
		}
		else if(Member.Name != "" && Member.Phone != "")
        {
            Member.ContactKey = TDBContacts::GetContactByNamePhone(DBTransaction, Member.Name, Member.Phone);
            if (Member.ContactKey != 0 && TDBContacts::GetContactExists(DBTransaction, Member.ContactKey))
            {
                TDBContacts::GetContactDetails(DBTransaction, Member.ContactKey, Member);
                MemberFound = true;
            }
        }

		if (MemberFound && TGlobalSettings::Instance().AutoAddWebMembers)
		{
			Member.ContactKey = 0;
			Member.AccessLevel = 0;
			Member.LastModified = Now();
			TDBContacts::SetContactDetails(DBTransaction, Member.ContactKey, eMember, Member);
			TDBGroups::SetContactGroup(DBTransaction, Member.ContactKey, "Web Member");
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, E.Message);
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

__fastcall TWebMate::AddProduct(Database::TDBTransaction &DBTransaction, int WebKey, UnicodeString WebGUID, _di_IXMLACCOUNTType Account, TList *OrderList,
	_di_IXMLPRODUCTType Product, TMMContactInfo Member, int SetMenuGroup)
{
	UnicodeString PLU = Product->PRODUCT_PLU;
	int SizeKey = TDBWebUtil::LoadItemSizeKeyFromThridPartyCode(DBTransaction, PLU);

	_di_IXMLPRODUCTIDENTType Productident = NULL;
	if (Product->ChildNodes->FindNode("PRODUCTIDENT") != NULL)
	{
		Productident = Product->Get_PRODUCTIDENT();
		if (SizeKey == 0 && Productident->ChildNodes->FindNode("SIZEKEY") != NULL)
		{
			SizeKey = StrToInt(Productident->SIZEKEY);
		}
	}

	if (SizeKey == 0)
	{
		throw Exception("Item Not Found, Check Product PLU", xmlResultErr);
	}

	if (!TDBWebUtil::CheckItemCompleteEnabled(DBTransaction, SizeKey))
	{ // Check for Duplication
		throw Exception("Item Ordered is Disabled or Missing in Menu.", xmlResultErr);
	}

	TItemComplete *Order = new TItemComplete();
	if (TDBWebUtil::LoadItemComplete(DBTransaction, Order, SizeKey))
	{
		OrderList->Add(Order);

		Order->TabName = Account->Get_NAME();
		Order->TransNo = Product->Get_GUID();
		Order->Terminal = "WebMate";
		Order->OrderedLocation = "WebMate";

		if (Product->Get_TYPE().LowerCase() == "parent")
		{
			Order->SetMenuGroup = TDBOrder::GetNextSetMenuGroupNumber(DBTransaction);
			if (TST_PROMO_MASTER(Order->SetMenuMask)) // Is a master.
			{
				Order->SetMenuMaster = true;
			}
		}
		else
		{
			Order->SetMenuGroup = SetMenuGroup;
		}

		if (Member.Valid())
		{
			Order->PartyName = Member.Name;
			Order->Loyalty_Key = Member.ContactKey;
		}
		else
		{
			Order->PartyName = Order->TabName;
		}

        Order->WebKey = WebKey;
        Order->TabType = TabWeb;
        Order->WebID = WebGUID;

		SetPrice(Product, Order);
		TDBWebUtil::LoadSalesRecipes(DBTransaction, Order->SalesRecipesToApply, SizeKey, Order->GetQty());
		// Process Options.
		_di_IXMLOPTIONTypeList OptionList = Product->Get_OPTION();
		for (int j = 0; j < OptionList->Count; j++)
		{
			_di_IXMLOPTIONType WebOption = OptionList->Get_Item(j);
			TItemOption *Option = new TItemOption();
			Option->OptionKey = WebOption->Get_Key();
			if (!TDBWebUtil::LoadItemOption(DBTransaction, *Option))
			{
				Option->Name = WebOption->Get_Name();
				Option->KitchenName = Option->Name;
			}
			Option->Owner = Order->OptionsSelected;
			Order->OptionsSelected->OptionAdd(Option);
		}

		// Notes
		Order->Note = Product->Get_PRODUCT_COMMENT();
		// Fix Serving Course.
		int ServingCourseKey = 0;
		if (Productident != NULL)
		{
			ServingCourseKey = StrToInt(Productident->Get_SERVINGCOURSEKEY());
			if (ServingCourseKey != 0)
			{
				Order->ServingCourse = TDBMenu::GetServingCourseFromDB(DBTransaction, ServingCourseKey);
			}
		}

		if (Order->ServingCourse.ServingCourseKey < 1)
		{
			Order->ServingCourse = TDBMenu::LoadDefaultServingCourse(DBTransaction);
		}

		_di_IXMLPRODUCTTypeList ProductList = Product->Get_PRODUCT();
		for (int j = 0; j < ProductList->Count; j++)
		{
			_di_IXMLPRODUCTType SideProduct = ProductList->Get_Item(j);
			UnicodeString PLU = SideProduct->PRODUCT_PLU;
			if (SideProduct->Get_TYPE().LowerCase() == "child")
			{
				AddProduct(DBTransaction, WebKey, WebGUID, Account, OrderList, SideProduct, Member, Order->SetMenuGroup);
			}
			else
			{
				AddSides(DBTransaction, Account, Order, SideProduct, Member);
			}
		}
	}
	else
	{
		delete Order;
		throw Exception("Loading Item. More than One Item or no Item Found.", xmlResultErr);
	}
}

__fastcall TWebMate::AddSides(Database::TDBTransaction &DBTransaction, _di_IXMLACCOUNTType Account, TItemComplete *MasterOrder, _di_IXMLPRODUCTType SideProduct,
	TMMContactInfo Member)
{
	UnicodeString PLU = SideProduct->PRODUCT_PLU;
	int SizeKey = TDBWebUtil::LoadItemSizeKeyFromThridPartyCode(DBTransaction, PLU);

	_di_IXMLPRODUCTIDENTType Productident = NULL;
	if (SideProduct->ChildNodes->FindNode("PRODUCTIDENT") != NULL)
	{
		Productident = SideProduct->Get_PRODUCTIDENT();
		if (SizeKey == 0 && Productident->ChildNodes->FindNode("SIZEKEY") != NULL)
		{
			SizeKey = StrToInt(Productident->SIZEKEY);
		}
	}

	if (SizeKey == 0)
	{
		throw Exception("Side Not Found, Check Product PLU", xmlResultErr);
	}

	if (!TDBWebUtil::CheckItemCompleteEnabled(DBTransaction, SizeKey))
	{ // Check for Duplication
		throw Exception("Side Ordered is Disabled or Missing in Menu.", xmlResultErr);
	}

	TItemCompleteSub *Order = new TItemCompleteSub();
	if (TDBWebUtil::LoadItemComplete(DBTransaction, Order, SizeKey))
	{
		MasterOrder->SubOrders->SubOrderAdd(Order);
		Order->OrderType = MasterOrder->OrderType;
		Order->TransNo = SideProduct->Get_GUID();

		if (Member.Valid())
		{
			Order->Loyalty_Key = Member.ContactKey;
		}
		SetPrice(SideProduct, Order);

		// Fix Serving Course.
		int ServingCourseKey = 0;
		if (Productident != NULL)
		{
			ServingCourseKey = StrToInt(Productident->Get_SERVINGCOURSEKEY());
			if (ServingCourseKey != 0)
			{
				Order->ServingCourse = TDBMenu::GetServingCourseFromDB(DBTransaction, ServingCourseKey);
			}
		}

		if (Order->ServingCourse.ServingCourseKey < 1)
		{
			Order->ServingCourse = MasterOrder->ServingCourse;
		}
	}
	else
	{
		delete Order;
		throw Exception("Loading Item. More than One Item or no Item Found.", xmlResultErr);
	}
}

void __fastcall TWebMate::SetPrice(_di_IXMLPRODUCTType Product, TItemMinorComplete *Order)
{
	// Fix Pricing.
	if (Product->GetChildNodes()->FindNode("PRICELEVEL") != NULL)
	{
		switch(Product->Get_PRICELEVEL())
		{
		case -1
			:
			{
				Currency MiscPrice = StrToCurr(Product->Get_PRODUCT_CHARGE_PRICE());
				Order->SetPriceLevelCustom(MiscPrice);
				Order->SetQty(Product->Get_QTY());
			}break;
		case 0:
			Order->SetPriceLevel0();
			Order->SetQty(Product->Get_QTY());
			break;
		case 1:
			Order->SetPriceLevel1();
			Order->SetQty(Product->Get_QTY());
			break;
		}
	}
	else if (Product->GetChildNodes()->FindNode("PRODUCT_CHARGE_PRICE") != NULL)
	{
		Currency MiscPrice = StrToCurr(Product->Get_PRODUCT_CHARGE_PRICE());
		Order->SetPriceLevelCustom(MiscPrice / Currency(Product->Get_QTY()));
		Order->SetQty(Product->Get_QTY());
	}
	else if (Product->GetChildNodes()->FindNode("PRODUCT_BASE_PRICE") != NULL)
	{
		Currency MiscPrice = StrToCurr(Product->Get_PRODUCT_BASE_PRICE());
		Order->SetPriceLevelCustom(MiscPrice);
		Order->SetQty(Product->Get_QTY());
	}
	else
	{
		throw Exception("Order has no Pricing Info", xmlResultErr);
	}
}
