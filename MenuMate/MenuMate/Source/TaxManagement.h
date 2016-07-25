#ifndef __h_tax_management__
#define __h_tax_management__

/* Required for Currency */
#include <System.hpp>

#include <vector>

class TaxManagement {
public:
	~TaxManagement();

	/*
	 * Worker/Utility functions.
	 *
	 * Note that these functions expect a scaled "tax_percent." That is to say
	 * that they expect 10.0 rather than 0.1 for 10%. This directly corresponds
	 * to the GSTPercent property of TItemMinor.
	 */

	Currency Price_excluding_tax(Currency tax_inclusive_price,
	                             double tax_percent) const;
	Currency Tax_content(Currency tax_inclusive_price,
	                     double tax_percent) const;

	/*
	 * The class doesn't posess any state of it's own. All data needed for
	 * calculation is provided explicitly.
	 */
	static const TaxManagement &Instance();

private:
	/* No one creates the class but the class itself. */
	TaxManagement();

	/*
	 * The class is a singleton - we don't want to allow the user to assign
	 * the class in any way. So... let's enforce it with the compiler!
    */
	TaxManagement(const TaxManagement &initializer);
	TaxManagement &operator = (const TaxManagement & rhs);

	static const TaxManagement instance_;
};

#endif /* ! __h_tax_management__ */
