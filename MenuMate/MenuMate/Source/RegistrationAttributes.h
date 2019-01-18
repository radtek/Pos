//---------------------------------------------------------------------------

#ifndef RegistrationAttributesH
#define RegistrationAttributesH
#include <Classes.hpp>
#include <list>
//---------------------------------------------------------------------------
enum eLicenceType {eEftpos, eLoyalty, eAccounting, eTimeTracking, eChefmate, ePropertyManagement, eRooms, eFloorPlan, ePosCashier, ePosOrder,
                    ePosHandHeld, eFiscal, eWebMate, ePocketVoucher, eBarExchange, eRunRateBoard, eOnlineOrdering};

//License Sub Types
enum eEFTPOS { eEftpostNZ = 1, eProvenco, eIngenicoAndPCEFTPOSAustralia, eCadmusKeylinkOneWay, eCadmusCronos, eICELinkICE5000Hyperlcom,
                eDPSPaymentExpress, eEFTPOSSmartpay,eEFTPOSSmartConnect, eEFTPOSAdyen, eEFTPOSPaymentSense};

enum eLoyalty {eMenumateLoyaltyLocal = 1, eMenumateLoyaltyLocalSubscription, eMenumateLoyaltyMateWeb, eMenuMateClubMembership,
                eEBetGamingMembership, eCasinoExternalMembership };

enum eAccounting { eXero = 1, eMYOB, ePeachtree  };

enum ePropertyManagement { eMotelMate = 1, eSihot, eOracle, eMews };

enum eRooms { eStrait = 1, eNewBook };

enum eFiscal {  ePOSPlus = 1, eFiscalPrinter, eAustriaPrinter };

struct LicenceSetting
{
     AnsiString Name;
     AnsiString Description;
     bool IsEnabledByDefault;
     eLicenceType SettingType;
     int SettingSubType;
};

struct LicenceSettingMapping
{
     long TerminalId;
    // Terminal Terminal;
     long LicenceSettingId;
     LicenceSetting LicenceSettingSetting;
     bool IsEnabled;
     AnsiString Value;
     AnsiString Text;
};

struct Terminal
{
     AnsiString Name;
     AnsiString Description;
     AnsiString TerminalCode;
     AnsiString MacAdress;
     AnsiString ComputerName;
     AnsiString OperatingSystemName;
     AnsiString MenumateVersionSystemName;
     long SiteId;
    // Site Site;
     long TerminalProfileId;
     std::list<LicenceSettingMapping> LicenceSettingMappings;
};

struct Site
{
     AnsiString Name;
     AnsiString Description;
     long SiteCode;
     long CompanyId;
     std::list<Terminal> Terminals;
     long SiteProfileId;
};

#endif

