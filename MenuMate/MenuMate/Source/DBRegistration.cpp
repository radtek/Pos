//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMLogging.h"
#include "DBRegistration.h"
#include "ManagerPhysicalPrinter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TTerminalModel TDBRegistration::GetTerminalInfo(Database::TDBTransaction &dbTransaction)
{
    TTerminalModel terminalInfo;
    try
    {
        terminalInfo.SiteCode             = TGlobalSettings::Instance().SiteID;
        terminalInfo.SyndicateCode        = GetSyndCode(dbTransaction);
        terminalInfo.TerminalName         = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.TerminalDescription  = TDeviceRealTerminal::Instance().ID.Name;
        terminalInfo.StaffName            = TDeviceRealTerminal::Instance().User.Name;
        terminalInfo.MacAdress            = GetMACaddress();
        terminalInfo.ComputerName         = TDeviceRealTerminal::Instance().ID.ComputerName;
        terminalInfo.OperatingSystemName  = GetOperatingSystemName();
        terminalInfo.MenumateVersion      = TDeviceRealTerminal::Instance().OS.MMSoftwareVersion;
        terminalInfo.LicenceSettingsModel = GetLicenseSettingsModelList(dbTransaction);
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return terminalInfo;
}
//-----------------------------------------------------------------------------------------------
std::list<TLicenceSettingModel> TDBRegistration::GetLicenseSettingsModelList(Database::TDBTransaction &dbTransaction)
{
    std::list<TLicenceSettingModel> licenceSettingModelList;
    try
    {
        for(int settingType = eEftpos; settingType <= eOnlineOrdering; settingType++)
        {
            LoadLicenseSettingsModelList(dbTransaction, settingType, licenceSettingModelList);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return licenceSettingModelList;
}
//-------------------------------------------------------------------------------------------------
void TDBRegistration::LoadLicenseSettingsModelList(Database::TDBTransaction &dbTransaction , int licenceType, std::list<TLicenceSettingModel> &licenceSettingModelList)
{
    try
    {
        switch(licenceType)
        {
            case eEftpos:
                {
                    LoadEFTPosSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eLoyalty:
                {
                    LoadLoyaltySettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eAccounts:
                {
                    LoadAccountsSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eTimeTracking:
                {
                    LoadTimeTrackingSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eChefmate:
                {
                    LoadChefmateSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case ePropertyManagement:
                {
                    LoadPropertyManagementSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eRoom:
                {
                    LoadRoomSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eFloorPlan:
                {
                    LoadFloorPlanSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case ePosCashier:
                {
                    LoadPosCashierSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case ePosOrder:
                {
                    LoadPosOrderSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case ePosHandHeld:
                {
                    LoadPosHandHeldSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eFiscal:
                {
                    LoadFiscalSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eWebMat:
                {
                    LoadWebMatSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case ePocketVoucher:
                {
                    LoadPocketVoucherSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eBarExchange:
                {
                    LoadBarExchangeSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eRunRateBoard:
                {
                    LoadRunRateBoardSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            case eOnlineOrdering:
                {
                    LoadOnlineOrderingSettingsForTerminal(dbTransaction, licenceSettingModelList, licenceType);
                }break;
            default:
            {
            }

        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadEFTPosSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int eftposSubType = eEftpostNZ; eftposSubType <= eEFTPOSPaymentSense; eftposSubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = eftposSubType;
            licenceSettingModel.IsActive          = GetEFTPosSetting(eftposSubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}
//---------------------------------------------------------------------
void TDBRegistration::LoadLoyaltySettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int loyaltySubType = eMenumateLoyaltyLocal; loyaltySubType <= eCasinoExternalMembership; loyaltySubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = loyaltySubType;
            licenceSettingModel.IsActive          = GetLoyaltySetting(loyaltySubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int accountSubType = eXero; accountSubType <= ePeachtree; accountSubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = accountSubType;
            licenceSettingModel.IsActive          = GetAccountSetting(accountSubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadTimeTrackingSettingsForTerminal(Database::TDBTransaction &dbTransaction,  std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TManagerVariable::Instance().GetBool(dbTransaction,vmTrackSaleAndMakeTimes,false);

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadChefmateSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = GetChefmateSetting(dbTransaction);

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPropertyManagementSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int propertySubType = eMotelMate; propertySubType <= eMews; propertySubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = propertySubType;
            licenceSettingModel.IsActive          = GetPropertyManagementSetting(propertySubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadRoomSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int roomSubType = eStrait; roomSubType <= eNewBook; roomSubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = roomSubType;
            licenceSettingModel.IsActive          = GetRoomSetting(roomSubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadFloorPlanSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().ReservationsEnabled;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPosCashierSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = (!TGlobalSettings::Instance().EnableWaiterStation) ? true : false;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPosOrderSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().EnableWaiterStation;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPosHandHeldSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = GetPosHandHeldSetting(dbTransaction);

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadFiscalSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        for(int  fiscalSubType = ePOSPlus; fiscalSubType <= eAustriaPrinter; fiscalSubType++)
        {
            TLicenceSettingModel licenceSettingModel;

            licenceSettingModel.SettingType       = licenceType;
            licenceSettingModel.SettingSubType    = fiscalSubType;
            licenceSettingModel.IsActive          = GetFiscalSetting(fiscalSubType);

            licenceSettingModelList.push_back(licenceSettingModel);
        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadWebMatSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().WebMateEnabled;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPocketVoucherSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = GetPocketVoucherSetting(dbTransaction);

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadBarExchangeSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().BarExchangeEnabled;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadRunRateBoardSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().IsRunRateBoardEnabled;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadOnlineOrderingSettingsForTerminal(Database::TDBTransaction &dbTransaction, std::list<TLicenceSettingModel> &licenceSettingModelList, int licenceType)
{
    try
    {
        TLicenceSettingModel licenceSettingModel;

        licenceSettingModel.SettingType       = licenceType;
        licenceSettingModel.SettingSubType    = "0";
        licenceSettingModel.IsActive          = TGlobalSettings::Instance().EnableOnlineOrdering;

        licenceSettingModelList.push_back(licenceSettingModel);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetEFTPosSetting(int eftPosSubType)
{
    bool status;
    switch(eftPosSubType)
    {
        case eEftpostNZ:
            {
                status = TGlobalSettings::Instance().EnableEftPosANZ;
            }break;
        case eProvenco:
            {
                status = TGlobalSettings::Instance().EnableEftPosSyncro;
            }break;
        case eIngenicoAndPCEFTPOSAustralia:
            {
                status = TGlobalSettings::Instance().EnableEftPosIngenico;
            }break;
        case eCadmusKeylinkOneWay:
            {
                status = TGlobalSettings::Instance().EnableEftPosCadmus;
            }break;
        case eCadmusCronos:
            {
                status = TGlobalSettings::Instance().EnableEftPosCadmusCronos;
            }break;
        case eICELinkICE5000Hyperlcom:
            {
                status = TGlobalSettings::Instance().EnableEftPosIceLink;
            }break;
        case eDPSPaymentExpress:
            {
                status = TGlobalSettings::Instance().EnableEftPosDPS;
            }break;
        case eEFTPOSSmartpay:
            {
                status = TGlobalSettings::Instance().EnableEftPosSmartPay;
            }break;
        case eEFTPOSSmartConnect:
                    {
                status = TGlobalSettings::Instance().EnableEftPosSmartConnect;
            }break;
        case eEFTPOSAdyen:
            {
                status = TGlobalSettings::Instance().EnableEftPosAdyen;
            }break;
        case eEFTPOSPaymentSense:
                    {
                status = TGlobalSettings::Instance().EnableEftPosPaymentSense;
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetLoyaltySetting(int loyaltySubType)
{
    bool status;
    switch(loyaltySubType)
    {
        case eMenumateLoyaltyLocal:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && !TGlobalSettings::Instance().LoyaltyMateEnabled);
            }break;
        case eMenumateLoyaltyLocalSubscription:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && TGlobalSettings::Instance().UseMemberSubs);
            }break;
        case eMenumateLoyaltyMateWeb:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && TGlobalSettings::Instance().LoyaltyMateEnabled);
            }break;
        case eMenuMateClubMembership:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeERS);
            }break;
        case eEBetGamingMembership:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeEBet);
            }break;
        case eCasinoExternalMembership:
            {
                status = (TGlobalSettings::Instance().MembershipType == MembershipTypeExternal);
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetAccountSetting(int accountSubType)
{
    bool status;
    switch(accountSubType)
    {
        case eXero:
            {
                status = TGlobalSettings::Instance().IsXeroEnabled;
            }break;
        case eMYOB:
            {
                status = TGlobalSettings::Instance().IsMYOBEnabled;
            }break;
        case ePeachtree:
            {
                status = TGlobalSettings::Instance().IsEnabledPeachTree;
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetChefmateSetting(Database::TDBTransaction &dbTransaction)
{
	TManagerPhysicalPrinter printerManager;
 	std::auto_ptr<TStringList>serverNameList(new TStringList);
	printerManager.GetPrinterServerList(dbTransaction, serverNameList.get(), ptChefMate_Printer);

	return serverNameList->Count > 0;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetPropertyManagementSetting(int propertySubType)
{
    bool status;
    switch(propertySubType)
    {
        case eMotelMate:
            {
                status = (TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Phoenix);
            }break;
        case eSihot:
            {
                status = (TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot);
            }break;
        case eOracle:
            {
                status = (TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Oracle);
            }break;
        case eMews:
            {
                status = (TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Mews);
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetRoomSetting(int roomSubType)
{
    bool status;
    switch(roomSubType)
    {
        case eStrait:
            {
                status = (TGlobalSettings::Instance().NewBook == 1) ? true : false ;
            }break;
        case eNewBook:
            {
                status = (TGlobalSettings::Instance().NewBook == 2) ? true : false ;
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}

//-----------------------------------------------------------------------
bool TDBRegistration::GetPosHandHeldSetting(Database::TDBTransaction &dbTransaction)
{
    bool status = false;
    try
    {
        TIBSQL *IBInternalQuery= dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text =  "SELECT * FROM DEVICES WHERE DEVICE_TYPE = :POS_HAND_HELD ";

        IBInternalQuery->ParamByName("POS_HAND_HELD")->AsInteger = 2;

        IBInternalQuery->ExecQuery();

        if(IBInternalQuery->RecordCount > 0)
            status = true;

    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        throw;
    }


	return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetFiscalSetting(int fiscalSubType)
{
    bool status;
    switch(fiscalSubType)
    {
        case ePOSPlus:
            {
                status = TGlobalSettings::Instance().IsFiscalStorageEnabled;
            }break;
        case eFiscalPrinter:
            {
                status = TGlobalSettings::Instance().UseItalyFiscalPrinter;
            }break;
        case eAustriaPrinter:
            {
                status = TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled;
            }break;
        default:
            {
                status = false;
            }
    }

    return status;
}
//-----------------------------------------------------------------------
bool TDBRegistration::GetPocketVoucherSetting(Database::TDBTransaction &dbTransaction)
{
    bool status = false;
    AnsiString URL = TManagerVariable::Instance().GetStr(dbTransaction, vmPocketVoucherURL,
     TDeviceRealTerminal::Instance().PocketVouchers->URL);
    try
    {
        TIBSQL *IBInternalQuery= dbTransaction.Query(dbTransaction.AddQuery());
        IBInternalQuery->Close();

        IBInternalQuery->SQL->Text =  "SELECT PROPERTIES FROM PAYMENTTYPES ";


        IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
        {
            if(IBInternalQuery->FieldByName("PROPERTIES")->AsString.Pos("-28-"))
            {
                status = true;
                break;
            }
        }
    }
    catch(Exception &ex)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,ex.Message);
        throw;
    }


	return (status && URL != "");
}
//---------------------------------------------------------------------------
AnsiString TDBRegistration::GetSyndCode(Database::TDBTransaction &dbTransaction)
{
    AnsiString syndicateCode = "";

    TDeviceRealTerminal::Instance().RegisterTransaction(dbTransaction);

    try
    {
        TManagerSyndCode ManagerSyndicateCode;
        ManagerSyndicateCode.Initialise(dbTransaction);
        TSyndCode currentSyndicateCode = ManagerSyndicateCode.GetCommunicationSyndCode();
        syndicateCode = currentSyndicateCode.GetSyndCode();
    }
    catch( Exception& exc )
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,exc.Message);
		throw;
    }
    return syndicateCode;
}
//---------------------------------------------------------------------------
void TDBRegistration::UpdateIsCloudSyncRequiredFlag(bool status)
{
    Database::TDBTransaction tr(TDeviceRealTerminal::Instance().DBControl);
    tr.StartTransaction();
    try
    {
        TGlobalSettings::Instance().IsCloudSyncRequired = status;
        TManagerVariable::Instance().SetDeviceBool(tr,vmIsCloudSyncRequired,TGlobalSettings::Instance().IsCloudSyncRequired);
        tr.Commit();

    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        tr.Rollback();
    }
}
//---------------------------------------------------------------------------
void TDBRegistration::UpdateIsRegistrationVerifiedFlag(Database::TDBTransaction &dbTransaction, bool status)
{
    try
    {
        TGlobalSettings::Instance().IsRegistrationVerified = status;
        TManagerVariable::Instance().SetDeviceBool(dbTransaction,vmIsRegistrationVerified,TGlobalSettings::Instance().IsRegistrationVerified);

    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        throw;
    }
}
//---------------------------------------------------------------------
AnsiString TDBRegistration::GetMACaddress()
{
    AnsiString retVal;
    try
    {
        char mac_address[18];

        UUID uuid;
        UuidCreateSequential( &uuid );    // Ask OS to create UUID

        // Bytes 2 through 7 inclusive are MAC address
        sprintf(mac_address,"%02X:%02X:%02X:%02X:%02X:%02X",uuid.Data4[2],uuid.Data4[3],uuid.Data4[4],uuid.Data4[5],uuid.Data4[6],uuid.Data4[7]);

        retVal = mac_address;
    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
        throw;
    }

  return  retVal;

}
//----------------------------------------------------------------------
AnsiString TDBRegistration::GetOperatingSystemName()
{
    AnsiString retValue = "";
    try
    {
        OSVERSIONINFOEX info;
        ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
        info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        GetVersionEx((LPOSVERSIONINFO)&info);//info requires typecasting

        double version;

        version = info.dwMajorVersion + (info.dwMinorVersion / 10.0);

        if(version == 5)
            retValue = "Windows 2000";
        else if(version == 5.1)
            retValue = "Windows XP";
        else if(version == 5.2 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows XP Professional x64 Edition";
        else if(version == 5.2 && (GetSystemMetrics(SM_SERVERR2) == 0))
            retValue = "Windows Server 2003";
        else if(version == 5.2 && (info.wSuiteMask & VER_SUITE_WH_SERVER))
            retValue = "Windows Home Server";
        else if(version == 5.2 && (GetSystemMetrics(SM_SERVERR2) != 0))
            retValue = "Windows Server 2003 R2";
        else if(version == 6.0 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows Vista";
        else if(version == 6.0 && (info.wProductType != VER_NT_WORKSTATION))
            retValue = "Windows Server 2008";
        else if(version == 6.1 && (info.wProductType != VER_NT_WORKSTATION))
            retValue = "Windows Server 2008 R2";
        else if(version == 6.1 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows 7";
        else if(version == 6.2 && (info.wProductType != VER_NT_WORKSTATION))
            retValue = "Windows Server 2012";
        else if(version == 6.2 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows 8";
        else if(version == 6.3 && (info.wProductType != VER_NT_WORKSTATION))
            retValue = "Windows Server 2012 R2";
        else if(version == 6.3 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows 8.1";
        else if(version == 10.0 && (info.wProductType != VER_NT_WORKSTATION))
            retValue = "Windows Server 2016";
        else if(version == 10.0 && (info.wProductType == VER_NT_WORKSTATION))
            retValue = "Windows 10";

    }
    catch(Exception &Exc)
    {
        TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,Exc.Message);
    }

    return retValue;
}

