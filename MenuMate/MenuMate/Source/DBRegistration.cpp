//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMLogging.h"
#include "DBRegistration.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TTerminalModel TDBRegistration::GetTerminalInfo(Database::TDBTransaction &dbTransaction)
{
    TTerminalModel terminalInfo;
    try
    {
//        terminalInfo.Name = TDeviceRealTerminal::Instance().ID.Name;
//        terminalInfo.Description = TDeviceRealTerminal::Instance().ID.Name;
//        terminalInfo.StaffName = TDeviceRealTerminal::Instance().User.Name;
////        terminalInfo.MacAdress = TDeviceRealTerminal::Instance();  to do
//        terminalInfo.ComputerName = TDeviceRealTerminal::Instance().ID.ComputerName;
////        terminalInfo.OperatingSystemName = TDeviceRealTerminal::Instance().OS.SoftwareVersion;
//        terminalInfo.MenumateVersion = TDeviceRealTerminal::Instance().OS.MMSoftwareVersion;
//        terminalInfo.SiteId = TGlobalSettings::Instance().SiteID;
//        terminalInfo.TerminalProfileId = TDeviceRealTerminal::Instance().ID.ProfileKey;
//        terminalInfo.LicenceSettingMappings = GetLicenseSettingMappingList(dbTransaction);
////        terminalInfo.
//        TGlobalSettings::Instance().SiteID;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return terminalInfo;
}
//-----------------------------------------------------------------------------------------------
std::list<TLicenceSettingMapping> TDBRegistration::GetLicenseSettingMappingList(Database::TDBTransaction &dbTransaction)
{
    std::list<TLicenceSettingMapping> licenseSettingMappingList;

    for(int i = eEftpos; i <= eOnlineOrdering; i++)
    {
        licenseSettingMappingList.push_back(GetLicenseSettingsMapping(dbTransaction, i));
    }
    return licenseSettingMappingList;
    return  licenseSettingMappingList;
}
//-------------------------------------------------------------------------------------------------
TLicenceSettingMapping TDBRegistration::GetLicenseSettingsMapping(Database::TDBTransaction &dbTransaction , int licenceType)
{
    TLicenceSettingMapping licenseSettingMapping;
    try
    {
        switch(licenceType)
        {
            case eEftpos:
                {
                    LoadEFTPosSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
            case eLoyalty:
                {
                    LoadLoyaltySettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
            case eAccounts:
                {
                    LoadAccountsSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
            case eTimeTracking:
                {
                    LoadTimeTrackingSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
//            case eChefmate:
//                {
//                    LoadChefmateSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
            case ePropertyManagement:
                {
                    LoadPropertyManagementSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
//            case eRoom:
//                {
//                    LoadeRoomSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
            case eFloorPlan:
                {
                    LoadFloorPlanSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
//            case ePosCashier:
//                {
//                    LoadPosCashierSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
            case ePosOrder:
                {
                    LoadPosOrderSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
//            case ePosHandHeld:
//                {
//                    LoadPosHandHeldSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
//            case eFiscal:
//                {
//                    LoadFiscalSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
            case eWebMat:
                {
                    LoadWebMatSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
//            case ePocketVoucher:
//                {
//                    LoadPocketVoucherSettingsForTerminal(dbTransaction, licenseSettingMapping);
//                }break;
            case eBarExchange:
                {
                    LoadBarExchangeSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
            case eRunRateBoard:
                {
                    LoadRunRateBoardSettingsForTerminal(dbTransaction, licenseSettingMapping);
                }break;
            case eOnlineOrdering:
                {
                    LoadOnlineOrderingSettingsForTerminal(dbTransaction, licenseSettingMapping);
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
    return licenseSettingMapping;
}
//---------------------------------------------------------------------
TLicenceSetting TDBRegistration::GetLicenseSettings(Database::TDBTransaction &dbTransaction, AnsiString name, AnsiString description, bool status, int subType)
{
    TLicenceSetting licenseSetting;
    try
    {
        licenseSetting.Name = name;
        licenseSetting.Description = description;
        licenseSetting.IsEnabledByDefault = status;
        licenseSetting.SettingType = subType;
       // licenseSetting.SettingSubTyp = 0;
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
    return licenseSetting;
}
//---------------------------------------------------------------------
void TDBRegistration::LoadEFTPosSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);

        if(TGlobalSettings::Instance().EnableEftPosANZ)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS ANZ", "EFTPOS ANZ", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosSyncro)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS Provenco", "EFTPOS Provenco", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosIngenico)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS INGENICO", "EFTPOS INGENICO", false, eEftpos);

        }
        else if(TGlobalSettings::Instance().EnableEftPosCadmus)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS CADMUS", "EFTPOS CADMUS", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosCadmusCronos)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS CADMUS CRONOS", "EFTPOS CADMUS CRONOS", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosIceLink)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS ICE LINK", "EFTPOS ICE LINK", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosDPS)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS DPS", "EFTPOS DPS", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosSmartPay)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS SMARTPAY", "EFTPOS SMARTPAY", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosSmartConnect)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS SMARTCONNECT", "EFTPOS SMARTCONNECT", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosAdyen)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS ADYEN", "EFTPOS ADYEN", false, eEftpos);
        }
        else if(TGlobalSettings::Instance().EnableEftPosPaymentSense)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "EFTPOS PAYMENT SENSE", "EFTPOS PAYMENT SENSE", false, eEftpos);
        }
    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}

}
//---------------------------------------------------------------------
void TDBRegistration::LoadLoyaltySettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);

        if(TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
//            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eLoyalty);
        }
        else if(TGlobalSettings::Instance().MembershipType == MembershipTypeERS && TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
//            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eLoyalty);

        }
        else if(TGlobalSettings::Instance().MembershipType == MembershipTypeEBet && TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
//            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eLoyalty);
        }
        else if(TGlobalSettings::Instance().MembershipType == MembershipTypeExternal && TGlobalSettings::Instance().LoyaltyMateEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
//            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eLoyalty);
        }
//        else if(TGlobalSettings::Instance().MembershipType == MembershipTypeMenuMate && TGlobalSettings::Instance().LoyaltyMateEnabled)
//        {
//                   licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;
//            licenseSettingMapping.Value = "";
//            licenseSettingMapping.Text = "";
//            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eLoyalty);
//        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);

        if(TGlobalSettings::Instance().IsXeroEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Xero", "Xero", false, eAccounts);
        }
        else if(TGlobalSettings::Instance().IsMYOBEnabled)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "MYOB", "MYOB", false, eAccounts);

        }
        else if(TGlobalSettings::Instance().IsEnabledPeachTree)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Peach Tree", "Peach Tree", false, eAccounts);

        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadTimeTrackingSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;
        licenseSettingMapping.IsEnabled = TManagerVariable::Instance().GetBool(dbTransaction,vmTrackSaleAndMakeTimes,false);
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Time Tracking", "Time Tracking", true, eTimeTracking);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
////---------------------------------------------------------------------
//void TDBRegistration::LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
//{
//    try
//    {
//        if(TGlobalSettings::Instance().IsXeroEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//        }
//        else if(TGlobalSettings::Instance().IsMYOBEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//        else if(TGlobalSettings::Instance().IsEnabledPeachTree)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//
//    }
//    catch(Exception &E)
//	{
//		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
//		throw;
//	}
//}
//---------------------------------------------------------------------
void TDBRegistration::LoadPropertyManagementSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);

        if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Phoenix)
        {
            licenseSettingMapping.LicenceSettingId = 1; //todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Phoenix", "Phoenix", false, ePropertyManagement);
        }
        else if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == SiHot)
        {
            licenseSettingMapping.LicenceSettingId = 1; //todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "SiHot", "SiHot", false, ePropertyManagement);

        }
        else if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Oracle)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Oracle", "Oracle", false, ePropertyManagement);

        }
        else if(TDeviceRealTerminal::Instance().BasePMS->Enabled && TGlobalSettings::Instance().PMSType == Mews)
        {
            licenseSettingMapping.LicenceSettingId = 1;//todo
            licenseSettingMapping.IsEnabled = true;
            licenseSettingMapping.Value = "";
            licenseSettingMapping.Text = "";
            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Mews", "Mews", false, ePropertyManagement);

        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
////---------------------------------------------------------------------
//void TDBRegistration::LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
//{
//    try
//    {
//        if(TGlobalSettings::Instance().IsXeroEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//        }
//        else if(TGlobalSettings::Instance().IsMYOBEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//        else if(TGlobalSettings::Instance().IsEnabledPeachTree)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//
//    }
//    catch(Exception &E)
//	{
//		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
//		throw;
//	}
//}
//---------------------------------------------------------------------
void TDBRegistration::LoadFloorPlanSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().ReservationsEnabled;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Floor Plan", "Floor Plan", false, eFloorPlan);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadPosOrderSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().EnableWaiterStation;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Waiter Station", "Waiter Station", false, ePosOrder);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadFiscalSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        if(TGlobalSettings::Instance().IsAustriaFiscalStorageEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Austria Fiscal", "Austria Fiscal", false, eFiscal);
//        }
//        else if(TGlobalSettings::Instance().UseItalyFiscalPrinter)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Italy Fiscal", "Italy Fiscal", false, eFiscal);
//
//        }
//        else if(TGlobalSettings::Instance().)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eFiscal);
//
//        }

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadWebMatSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().WebMateEnabled;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "WebMate", "WebMate", false, eWebMat);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
////---------------------------------------------------------------------
//void TDBRegistration::LoadAccountsSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
//{
//    try
//    {
//        if(TGlobalSettings::Instance().IsXeroEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//        }
//        else if(TGlobalSettings::Instance().IsMYOBEnabled)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//        else if(TGlobalSettings::Instance().IsEnabledPeachTree)
//        {
//            licenseSettingMapping.LicenceSettingId = 1;//todo
//            licenseSettingMapping.IsEnabled = true;//todo
//            licenseSettingMapping.Value = "";//todo
//            licenseSettingMapping.Text = "";//todo
////            licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "", "", false, eAccounts);
//
//        }
//
//    }
//    catch(Exception &E)
//	{
//		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
//		throw;
//	}
//}
//---------------------------------------------------------------------
void TDBRegistration::LoadBarExchangeSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().BarExchangeEnabled;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Bar Exchange", "Bar Exchange", false, eBarExchange);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadRunRateBoardSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().IsRunRateBoardEnabled;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "RunRate", "RunRate", false, eRunRateBoard);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}
//---------------------------------------------------------------------
void TDBRegistration::LoadOnlineOrderingSettingsForTerminal(Database::TDBTransaction &dbTransaction,TLicenceSettingMapping &licenseSettingMapping)
{
    try
    {
//        licenseSettingMapping.TerminalId = StrToInt(TDeviceRealTerminal::Instance().ID.TerminalID);
        licenseSettingMapping.LicenceSettingId = 1;//todo
        licenseSettingMapping.IsEnabled = TGlobalSettings::Instance().EnableOnlineOrdering;
        licenseSettingMapping.Value = "";
        licenseSettingMapping.Text = "";
        licenseSettingMapping.LicenceSettingSetting = GetLicenseSettings(dbTransaction, "Online Ordering", "Online Ordering", false, eOnlineOrdering);

    }
    catch(Exception &E)
	{
		TManagerLogs::Instance().Add(__FUNC__,EXCEPTIONLOG,E.Message);
		throw;
	}
}


