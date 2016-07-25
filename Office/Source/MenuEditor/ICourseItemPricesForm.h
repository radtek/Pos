//---------------------------------------------------------------------------
#ifndef ICourseItemPricesFormH
#define ICourseItemPricesFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "ICourseItemPricesController.h"
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
class ICourseItemPricesForm
{
public:

	virtual void SetController(ICourseItemPricesController* controller) = 0;

	virtual bool Show() = 0;

	virtual void Set(std::vector<ItemSize>& itemSizes) = 0;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif

