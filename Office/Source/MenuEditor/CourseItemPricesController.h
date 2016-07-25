//---------------------------------------------------------------------------
#ifndef CourseItemPricesControllerH
#define CourseItemPricesControllerH
//---------------------------------------------------------------------------
#include "ICourseItemPricesForm.h"
#include "ICourseItemPricesController.h"
#include <memory>
//---------------------------------------------------------------------------
class TItemSizeNode;
class TItemNode;
//---------------------------------------------------------------------------
namespace MenuEditor
{
//---------------------------------------------------------------------------
class CourseItemPricesController : public ICourseItemPricesController
{
public:

	CourseItemPricesController(ICourseItemPricesForm* form);
	virtual ~CourseItemPricesController();

	virtual void SetCurrentPrices(std::vector<std::pair<TItemSizeNode*, TItemNode*> > prices);

	virtual TStrings* GetRoundingStrategies();

	virtual void Calculate(std::vector<ItemSize>& itemSizes, double adjustBy, TObject* roundingStrategy);

	virtual void SaveMenuPrices(std::vector<ItemSize>& itemSizes);

private:
	ICourseItemPricesForm* form;
	std::vector<IRoundingStrategy*> roundingStrategies;
	std::auto_ptr<TStringList> roundingStrategiesAsTStrings;
	void BuildRoundingStrategies();
	void DeleteRoundingStrategies();
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
