//---------------------------------------------------------------------------
#ifndef ZedCollectionH
#define ZedCollectionH
//---------------------------------------------------------------------------
#include "Zed.h"
#include "ISink.h"
#include <map>
//---------------------------------------------------------------------------
namespace Cashup
{
//---------------------------------------------------------------------------
class ZedCollection
{
public:
	virtual ~ZedCollection();

	void BeginLoad(const System::TDateTime& startDateTimeFilter,
		const System::TDateTime& endDateTimeFilter, ISink<Zed>& zedSink);

	Zed* GetZed(int zedId) const;

private:
	void Clear();
	void CreateZ(
		const int id,
		const System::TDateTime& createdDateTime,
		const System::AnsiString& terminalName,
		const AnsiString& cashier,
		const bool reconciled,
		const Currency& variance);

	std::map<int, Zed*> _zeds;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
