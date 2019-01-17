//---------------------------------------------------------------------------

#ifndef RegistrationEnumsH
#define RegistrationEnumsH
//---------------------------------------------------------------------------
enum eEFTPOS
{
eEftpostNZ = 1, eProvenco, eIngenicoAndPCEFTPOSAustralia, eCadmusKeylinkOneWay, eCadmusCronos, eICELinkICE5000Hyperlcom, eDPSPaymentExpress, eEFTPOSSmartpay,
eEFTPOSSmartConnect, eEFTPOSAdyen, eEFTPOSPaymentSense
};
//------------------------------------------------------------------------------
enum eLoyalty
{
eMenumateLoyaltyLocal = 1, eMenumateLoyaltyLocalSubscription, eMenumateLoyaltyMateWeb, eMenuMateClubMembership, eEBetGamingMembership, eCasinoExternalMembership
};
//------------------------------------------------------------------------------
enum eAccounting
{
eXero = 1, eMYOB, ePeachtree
};
//------------------------------------------------------------------------------
enum ePropertyManagement
{
eMotelMate = 1, eSihot, eOracle, eMews
};
//------------------------------------------------------------------------------
enum eRooms
{
eStrait = 1, eNewBook
};
//------------------------------------------------------------------------------
enum eFiscal
{
ePOSPlus = 1, eFiscalPrinter, eAustriaPrinter
};
#endif
