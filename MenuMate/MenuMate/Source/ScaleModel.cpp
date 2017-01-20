// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ScaleModel.h"
#include "ManagerVariable.h"
#include "MMLogging.h"
#include "WeightStreamQUQA.h"
#include "WeightStreamStdTypeA.h"
#include "WeightStreamTypeAveryFx.h"
#include "WeightStreamKenkoKK300.h"
#include <Forms.hpp>
#include "enum.h"
// ---------------------------------------------------------------------------

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif
#ifdef PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)

TScaleModel::TScaleModel()
{
    Port = new TComPort(NULL);
    Port->BaudRate = br9600;
    Port->DataBits = dbEight;
    Port->Parity->Bits = prNone;
    Port->StopBits = sbOneStopBit;
    Port->Parity->Check = false;
    Port->Parity->Replace = false;
    Port->Buffer->InputSize = 16384;
    Port->Buffer->OutputSize = 16384;
    Port->DiscardNull = false;
    Port->OnRxChar = GetData;
    Port->Timeouts->ReadInterval = 200;
    Port->Timeouts->ReadTotalMultiplier = 200;
    Port->Timeouts->WriteTotalMultiplier = 200;
    Port->Timeouts->ReadTotalConstant = 300;
    Port->FlowControl->ControlDTR = dtrEnable;
    Port->FlowControl->ControlRTS = rtsHandshake;
    Port->FlowControl->OutCTSFlow = true;
    Port->FlowControl->XonXoffOut = false;
    Port->FlowControl->XonXoffIn = false;
    CloseingPort = false;
    OpeningPort = false;
    Busy = false;
    PortNumber = -1;
    AsyncMode = true;
    StablilityCount = 0;
    Port->OnTxEmpty = PortTxEmpty;
    ScaleStreamType = TypeA_STDStream;
}

TScaleModel::~TScaleModel()
{
    Close();
    delete Port;
}

void TScaleModel::Initialise(Database::TDBTransaction &DBTransaction)
{
    int ProfileKey = TManagerVariable::Instance().GetInt(DBTransaction, vmScalesProfile, -1);
    if (ProfileKey == -1)
    {
	ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles, "Scales" + IntToStr(TManagerVariable::Instance().DeviceProfileKey));
	TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmScalesProfile, ProfileKey);
    }

    if (ProfileKey != 0)
    {
	ScaleStreamType = TypeA_STDStream;
	int IntScaleStreamType = (int)TypeA_STDStream;
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmScalesType, IntScaleStreamType);
	ScaleStreamType = IntScaleStreamType;

	PortNumber = -1;
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);

	AsyncMode = true;
	TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);

	int BaudRate = int(br2400);
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, BaudRate);
	Port->BaudRate = TBaudRate(BaudRate);

	int DataBits = int(dbSeven);
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, DataBits);
	Port->DataBits = TDataBits(DataBits);

	int StopBits = int(sbOneStopBit);
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, StopBits);
	Port->StopBits = TStopBits(StopBits);

	int ParityBits = int(prEven);
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, ParityBits);
	Port->Parity->Bits = TParityBits(ParityBits);

	int ControlRTS = int(rtsHandshake);
	TManagerVariable::Instance().GetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl, ControlRTS);
	Port->FlowControl->ControlRTS = TRTSFlowControl(ControlRTS);

	bool OutCTSFlow = true;
	TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, OutCTSFlow);
	Port->FlowControl->OutCTSFlow = OutCTSFlow;

	bool XonXoffOut = false;
	TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, XonXoffOut);
	Port->FlowControl->XonXoffOut = XonXoffOut;

	bool XonXoffIn = false;
	TManagerVariable::Instance().GetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, XonXoffIn);
	Port->FlowControl->XonXoffIn = XonXoffIn;
    }
}

void TScaleModel::SaveSettings(Database::TDBTransaction &DBTransaction)
{
    int ProfileKey = TManagerVariable::Instance().GetInt(DBTransaction, vmScalesProfile, -1);
    if (ProfileKey == -1)
    {
	ProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction, eEquipmentProfiles, "Scales" + IntToStr(TManagerVariable::Instance().DeviceProfileKey));
	TManagerVariable::Instance().SetDeviceInt(DBTransaction, vmScalesProfile, ProfileKey);
    }

    if (ProfileKey != 0)
    {
	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmScalesType, ScaleStreamType);

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialPort, PortNumber);

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialBaudRate, static_cast<int>(Port->BaudRate));

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialDataBits, static_cast<int>(Port->DataBits));

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialStopBits, static_cast<int>(Port->StopBits));

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialParityBits, static_cast<int>(Port->Parity->Bits));

	TManagerVariable::Instance().SetProfileInt(DBTransaction, ProfileKey, vmSerialRTSFlowControl, static_cast<int>(Port->FlowControl->ControlRTS));

	TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialOutCTSFlow, Port->FlowControl->OutCTSFlow);

	TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffOut, Port->FlowControl->XonXoffOut);

	TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialXonXoffIn, Port->FlowControl->XonXoffIn);

	TManagerVariable::Instance().SetProfileBool(DBTransaction, ProfileKey, vmSerialAsync, AsyncMode);
    }
}

bool TScaleModel::Open(int PortNum)
{
    bool Success = false;
    try
    {
	if (!OpeningPort)
	{
	    OpeningPort = true;
	    try
	    {
		if (PortNum > 0)
		{
		    if (Port->Connected && Port->Port != ("COM" + IntToStr(PortNum)))
		    { // Comports Changed.
			Port->Close();
			Port->Port = "COM" + IntToStr(PortNum);
			Port->Open();
			Success = true;
			PortNumber = PortNum;
		    }
		    else if (!Port->Connected)
		    {
			Port->Port = "COM" + IntToStr(PortNum);
			Port->Open();
			Success = true;
			PortNumber = PortNum;
		    }
		    else // Port already open and on the right commport.
		    {
			Success = true;
		    }
		}
		else
		{
		    if (Port->Connected)
		    {
			Port->Close();
		    }
		    Success = true;
		}
	    }
	    __finally
	    {
		OpeningPort = false;
	    }
	}
    }
    catch(Exception & E)
    {
	TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Port Number : " + IntToStr(PortNum));
	LastError = E.Message;
	Success = false;
    }
    return Success;
}

void TScaleModel::Close()
{
    try
    {
	if (CloseingPort)
	    return;
	CloseingPort = true;
	try
	{
	    if (writeAsyncPtrList.size() > 0)
	    {
		Port->AbortAllAsync();
		writeAsyncPtrList.clear();
	    }
	    Port->Close();
	}
	__finally
	{
	    CloseingPort = false;
	}
    }
    catch(Exception & E)
    {
	TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message + " Port Number : " + IntToStr(PortNumber));
    }
}

void __fastcall TScaleModel::GetData(TObject *Sender, int count)
{
    try
    {
		try
		{
			Port->ClearBuffer(false, true);
			if (OnStabilityChanged.EventCount() != 0 || OnWeightChanged.EventCount() != 0)
			{
				#define Max_Bytes 1000
				int CurrentTime, TimeOut;
				std::vector<BYTE>recvbuffer(Max_Bytes, 0xFF);
				UnicodeString temp = "";
				int BytesRead = 0;
				if (ScaleStreamType == TypeC_QUQA)
				{
					 BytesRead = Port->Read(&recvbuffer[0], Max_Bytes);
				}
				else if(ScaleStreamType == Type_AveryFxStream)
				{
					BytesRead = Port->Read(&recvbuffer[0], 48);
				}
                else if(ScaleStreamType == Type_KenkoKK300W)
				{
					BytesRead = Port->Read(&recvbuffer[0], 48);
				}
				else
				{
					BytesRead = Port->Read(&recvbuffer[0], 48);
				}

				TWeight NewWeight;
				bool NewStable;
				try
				{
					std::auto_ptr<IWeightStreamConverter>Convert(NULL);
					if (ScaleStreamType == TypeC_QUQA)
					{
						Convert.reset(TWeightStreamQUQA::Create(&recvbuffer[0], BytesRead));
					}
					else if(ScaleStreamType == Type_AveryFxStream)
					{
						Convert.reset(TWeightStreamTypeAveryFx::Create(&recvbuffer[0], BytesRead));
					}
                    else if(ScaleStreamType == Type_KenkoKK300W)
                    {
                        Convert.reset(TWeightStreamKenkoKK300::Create(&recvbuffer[0], BytesRead));
                    }
					else
					{
						Convert.reset(TWeightStreamStdTypeA::Create(&recvbuffer[0], BytesRead));
					}

					Convert->GetWeight(NewWeight);
                    if(TDeviceRealTerminal::Instance().Scales->Counter == 0)
    					Convert->GetStable(NewStable);

					if (NewStable != Stable)
					{
						// Go unstabile imediatly but dampen the stability a bit more,
						// the scales I have seem to report "stable" but then change the weight by 5g
						// on the next communcation.
						if (!NewStable)
						{
							Stable = NewStable;
							// Thread Safes the OnStabilityChanged.Occured() events.
							PostMessage(Application->Handle, WEIGHSCALES_STATUSCHANGED, NULL, WEIGHSCALES_STABILITYCHANGED);
						}
						else
						{
							if (StablilityCount > 1)
							{
							Stable = NewStable;
							StablilityCount = 0;
							// Thread Safes the OnStabilityChanged.Occured() events.
							PostMessage(Application->Handle, WEIGHSCALES_STATUSCHANGED, NULL, WEIGHSCALES_STABILITYCHANGED);
							}
							else
							{
							StablilityCount++;
							}
						}
					}

					// Update the Weight here.
					if (!Weight.IsEqual(NewWeight))
					{
						Weight.Assign(NewWeight);

						Stable = false;
						// Thread Safes the OnWeightChanged.Occured() events.
						PostMessage(Application->Handle, WEIGHSCALES_STATUSCHANGED, NULL, WEIGHSCALES_WEIGHTCHANGED);
					}
				}
				catch(TWSCException & E)
				{
					if (E.Type == twscOverLoaded)
					{
					PostMessage(Application->Handle, WEIGHSCALES_STATUSCHANGED, NULL, WEIGHSCALES_STABILITYCHANGED);
					}
					TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Scales Exception", E.Message);
				}
			}
			else
			{
				Stable = false;
				Weight.SetWeightIn_Kg(0);
			}
		}
		__finally
		{
			Port->ClearBuffer(true, false);
		}
    }
    catch(Exception & E)
    {
	TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, "Scales Exception", E.Message);
	Port->ClearBuffer(true, true);
    }
}

void __fastcall TScaleModel::SetData(AnsiString Data)
{
    if (Busy)
	return;
    Busy = true;
    try
    {
	if (Open(PortNumber))
	{
	    if (AsyncMode)
	    {
		if (writeAsyncPtrList.size() < 2000)
		{
		    PAsync ptr;
		    InitAsync(ptr);
		    writeAsyncPtrList.push_back(ptr);
		    Port->WriteStrAsync(Data, ptr);
		}
		else
		{
		    TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, "Scale Port Buffer Full.");
		}
	    }
	    else
	    {
		TMemoryStream *Stream = new TMemoryStream;
		try
		{
		    Stream->Position = 0;
		    Stream->Write(Data.c_str(), Data.Length());
		    Stream->Position = 0;
		    Port->Write((unsigned char*)(Stream->Memory), Stream->Size);
		}
		__finally
		{
		    delete Stream;
		}
	    }
	}
    }
    __finally
    {
	Busy = false;
    }
}

void __fastcall TScaleModel::PortTxEmpty(TObject *Sender)
{
    std::list<PAsync>::iterator i;
    for (i = writeAsyncPtrList.begin(); i != writeAsyncPtrList.end(); )
    {
	if (Port->IsAsyncCompleted(*i))
	{
	    DoneAsync(*i);
	    writeAsyncPtrList.erase(i++);
	}
	else
	{
	    ++i;
	}
    }
}

TComPort *TScaleModel::GetTComPort()
{
    return Port;
}
