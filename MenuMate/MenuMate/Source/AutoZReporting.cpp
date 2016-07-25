#pragma hdrstop

#include <DateUtils.hpp>
#include <stdexcept>

#include "Analysis.h"
#include "AutoZReporting.h"
#include "AutoZReportingSetup.h"
#include "DeviceRealTerminal.h"

#pragma package(smart_init)

#define DevRealTermInstance TDeviceRealTerminal::Instance()
#define MgrVariableInstance TManagerVariable::Instance()

using std::runtime_error;

TAutoZReporting *TAutoZReporting::instance = NULL;

const vmVariables TAutoZReporting::varSequence[VarSequenceSize] = {
	vmAutoZCheck1Time, vmAutoZCheck1Options,
	vmAutoZCheck2Time, vmAutoZCheck2Options,
	vmAutoZCheck3Time, vmAutoZCheck3Options,
	vmAutoZCheck4Time, vmAutoZCheck4Options
};

TCheckTimeSettings &TCheckTimeSettings::operator=(
	const TCheckTimeSettings &rhs)
{
	if (this != &rhs) {
		checkTime = rhs.checkTime, options = rhs.options;
	}
	return *this;
}

bool TCheckTimeSettings::operator!=(const TCheckTimeSettings &rhs)
{
	return checkTime != rhs.checkTime || options != rhs.options;
}

TAutoZReporting::TAutoZReporting()
	: transaction(DevRealTermInstance.DBControl)
{
	if (instance)
		throw runtime_error("Auto Z-Reporting instance is already active.");
	instance = this;

	checkTimeSettings =
		new TCheckTimeSettings[TAutoZVariable::NumberOfCheckTimes];

	ReadSettingsFromDatabase();

	checkHeartbeat = new TTimer(NULL);
	checkHeartbeat->Interval = 30000;
	checkHeartbeat->OnTimer = this->CheckHeartbeatHandler;
   checkHeartbeat->Enabled = true;
}

TAutoZReporting::~TAutoZReporting()
{
	delete checkHeartbeat;
	delete[] checkTimeSettings;
}

void TAutoZReporting::BuildInitialConfiguration()
{
	transaction.StartTransaction();
   if ((profileKey = MgrVariableInstance.GetInt(transaction,
	                                             vmAutoZProfileKey, -1)) <= 0) {
		const vmVariables *j = varSequence + TAutoZVariable::VarSequenceSize;

		profileKey =
			MgrVariableInstance.SetProfile(transaction,
		                                  ProfileType::eTerminalProfiles,
		                                  "AutoZReporting"
		                                  + DevRealTermInstance.ID.Name);
		transaction.Commit();
		transaction.StartTransaction();

		MgrVariableInstance.SetDeviceInt(transaction,
		                                 vmVariables::vmAutoZProfileKey,
		                                 profileKey);
		for (const vmVariables *i = varSequence; i < j; ) {
			MgrVariableInstance.SetProfileTime(transaction, profileKey,
                                            i[TAutoZVariable::CheckTime],
			                                   0);
			MgrVariableInstance.SetProfileInt(transaction, profileKey,
                                           i[TAutoZVariable::CheckOptions],
			                                  0);
			i += TAutoZVariable::VarsPerCheckTime;
		}
	}
	transaction.Commit();
}

void __fastcall TAutoZReporting::CheckHeartbeatHandler(TObject *sender)
{
	;
}

void TAutoZReporting::DoZChecks()
{
   ;
}

TAutoZReporting &TAutoZReporting::Instance()
{
	if (!instance)
		new TAutoZReporting();
	return *instance;
}

void TAutoZReporting::ReadSettingsFromDatabase()
{
	const vmVariables  *i = varSequence;
	TCheckTimeSettings *m = checkTimeSettings;
	TCheckTimeSettings *n = m + TAutoZVariable::NumberOfCheckTimes;

	BuildInitialConfiguration();

	transaction.StartTransaction();
	for ( ; m < n; i += TAutoZVariable::VarsPerCheckTime, m++) {
		MgrVariableInstance.GetProfileTime(transaction, profileKey,
		                                   i[TAutoZVariable::CheckTime],
		                                   m->checkTime);
		MgrVariableInstance.GetProfileInt(transaction, profileKey,
		                                  i[TAutoZVariable::CheckOptions],
		                                  m->options);
	}
	transaction.Commit();
}

void TAutoZReporting::ShowSetupForm()
{
	checkHeartbeat->Enabled = false;

	auto_ptr<TfrmAutoZReportingSetup> setup(
		new TfrmAutoZReportingSetup(NULL, checkTimeSettings,
	                               &authenticateAsUserWithKey));
	setup->ShowModal();

	checkHeartbeat->Enabled = true;
}

void TAutoZReporting::UpdateSettings(const TCheckTimeSettings *k)
{
	TCheckTimeSettings *i = checkTimeSettings;
	TCheckTimeSettings *j = i + TAutoZVariable::NumberOfCheckTimes;
	const vmVariables *l = varSequence;

	transaction.StartTransaction();
	for ( ; i < j; i++, k++, l += TAutoZVariable::VarsPerCheckTime) {
		if (*i != *k) {
			*i = *k;
			MgrVariableInstance.SetProfileTime(transaction, profileKey,
			                                   l[TAutoZVariable::CheckTime],
			                                   i->checkTime);
			MgrVariableInstance.SetProfileInt(transaction, profileKey,
			                                  l[TAutoZVariable::CheckOptions],
			                                  i->options);
		}
	}
	transaction.Commit();
}

