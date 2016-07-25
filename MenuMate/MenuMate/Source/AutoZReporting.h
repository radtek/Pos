#ifndef AutoZReportingH
#define AutoZReportingH

#include <vcl.h>
#include <vector>

#include "MM_DBCore.h"
#include "ManagerVariable.h"

using Database::TDBTransaction;
using std::vector;

enum TAutoZVariable {
	CheckTime,
	CheckOptions,
	VarsPerCheckTime,
	NumberOfCheckTimes = 4,
	VarSequenceSize = VarsPerCheckTime * NumberOfCheckTimes
};

enum TCheckTimeOption {
	Enabled = 1,
	PrintZReport = 2,
	EmailZReport = 4
};

enum TCheckTimeOptionShift {
	EnabledShift,
	PrintZReportShift,
	EmailZReportShift
};

struct TCheckTimeSettings {
	TCheckTimeSettings()
	{
		checkTime = 0;
		options = 0;
	}

	TCheckTimeSettings &operator=(const TCheckTimeSettings &);
	bool operator!=(const TCheckTimeSettings &);

	TDateTime checkTime;
	unsigned char options;
};

class TAutoZReporting {
public:
   TAutoZReporting();
	~TAutoZReporting();

	static TAutoZReporting &Instance();

	void DoZChecks();
	void ShowSetupForm();
	void UpdateSettings(const TCheckTimeSettings *);

	static const vmVariables varSequence[VarSequenceSize];
private:
	void BuildInitialConfiguration();
	void __fastcall CheckHeartbeatHandler(TObject *);
	void ReadSettingsFromDatabase();

	static TAutoZReporting *instance;

	int authenticateAsUserWithKey;
	TTimer *checkHeartbeat;
	TCheckTimeSettings *checkTimeSettings;
	int profileKey;
	TDBTransaction transaction;
};

#endif
