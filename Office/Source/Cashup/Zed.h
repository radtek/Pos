//---------------------------------------------------------------------------
#ifndef ZedH
#define ZedH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <set>
#include "Payment.h"
//---------------------------------------------------------------------------
namespace Cashup
{
//---------------------------------------------------------------------------
class Zed
{
public:

//	Zed();

	Zed(const int id,
		const TDateTime& createdDateTime,
		const AnsiString& terminalName,
		const AnsiString& cashier,
		const bool reconciled,
		const Currency& totalVariance);

	bool operator == (const Zed& rhs) const;
	bool operator < (const Zed& rhs) const;

	int GetId() const;
	const TDateTime& GetCreated() const;
	const AnsiString& GetTerminalName() const;
	const AnsiString& GetCashier() const;
	bool HasBeenReconciled() const;
	const Currency& GetTotalVariance() const;

	std::set<Payment> GetPayments() const;
	void SavePaymentAdjustments(const std::set<Payment>& payments);

	AnsiString GetOriginalReport() const;


private:

	const int _id;
	const TDateTime _createdDateTime;
	const AnsiString _terminalName;
	const AnsiString _cashier;

	void GetPreviousZCreatedDateTime() const;
	mutable TDateTime _previousZCreatedDateTime;

	bool _reconciled;
	Currency _totalVariance;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
