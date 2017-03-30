#include <vcl.h>
#pragma hdrstop

#ifdef madExcept
#include "madExcept.hpp"
#endif

#include "Requests.h"
#include "MMRegistry.h"
#include "Printing.h"
#include "CSV.h"

#include "ContactStaff.h"
#include "Membership.h"
#include "MembershipERS.h"

#include "SystemConfig.h"
#include "Locations.h"
#include "MMTouchNumpad.h"
#include "ListSecurityRefContainer.h"

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "Processing.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <IBDatabaseInfo.hpp>
#include <IBServices.hpp>
#include "Stream.h"
// #include <IdTime.hpp>
#include <IdSNTP.hpp>
#include "MMMessageBox.h"
#include "ManagerVariable.h"
#include "Version.h"
#include "Stream.h"
#include "SimpleTCP.h"
#include "ManagerNet.h"

#include <StrUtils.hpp>
//#include "StringTableRes.h"
#include "StringTableVariables.h"
#include "OpSys.h"

#ifdef  PalmMate
#include "Palm.h"
#endif

#pragma package(smart_init)
// ---------------------------------------------------------------------------
char const soh = '[';
char const eoh = ']';
char const sep = '|';

char const soo = '{';
char const eoo = '}';

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam);

// ---------------------------------------------------------------------------
TReqLogin::TReqLogin(TDeviceImage *Requester) : TRequest(Requester)
{
   UserID = 0;
   UsersPIN = "";
   ErrorMsg = "";
   Error = proA_Ok;
}
// ---------------------------------------------------------------------------



