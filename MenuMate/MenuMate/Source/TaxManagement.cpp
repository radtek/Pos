#pragma hdrstop

#include "TaxManagement.h"

#pragma package(smart_init)

const TaxManagement TaxManagement::instance_;

TaxManagement::TaxManagement()
{
}

TaxManagement::~TaxManagement()
{
}

Currency
TaxManagement::Price_excluding_tax(
  Currency tax_inclusive_price,
  double tax_percent)
const
{
	Currency tax_exclusive_price = tax_inclusive_price;

	tax_exclusive_price = (tax_inclusive_price * 100) / (tax_percent + 100);

	return tax_exclusive_price;
}

Currency
TaxManagement::Tax_content(
  Currency tax_inclusive_price,
  double tax_percent)
const
{
	/* The check for a zero tax percent is done in Price_excluding tax. */
	const Currency tax_content =
	  tax_inclusive_price - Price_excluding_tax(tax_inclusive_price,
	                                            tax_percent);
	return tax_content;
}

const TaxManagement &
TaxManagement::Instance()
{
	return instance_;
}

