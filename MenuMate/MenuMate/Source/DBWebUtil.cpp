// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBWebUtil.h"
#include "WebOrder.h"
#include "ItemOption.h"
#include <algorithm>
#include "DB_5_2.h"

#include "ItemSizeCategory.h"
#include "ItemRecipe.h"
#include "DeviceRealTerminal.h"
#include "MMContactInfo.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

TDBWebUtil::TDBWebUtil()
{

}

TDBWebUtil::~TDBWebUtil()
{

}

void TDBWebUtil::UpdateDBVersion_5_2(Database::TDBControl &DBControl)
{
	DB_5_2::Up_5_2(DBControl);
}

void TDBWebUtil::TearDownDBVersion_5_2(Database::TDBControl &DBControl)
{
	DB_5_2::Down_5_2(DBControl);
}

int TDBWebUtil::LoadItemSizeKeyFromThridPartyCode(Database::TDBTransaction &DBTransaction, UnicodeString PLU)
{
	int RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text =
      	"SELECT "
      		"ITEMSIZE_KEY "
         "FROM "
         	"ITEMSIZE left join THIRDPARTYCODES on ITEMSIZE.THIRDPARTYCODES_KEY = THIRDPARTYCODES.THIRDPARTYCODES_KEY "
         "WHERE "
         	"THIRDPARTYCODES.CODE = :PLU";
		IBInternalQuery->ParamByName("PLU")->AsString = PLU;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount == 1)
		{
			RetVal = IBInternalQuery->FieldByName("ITEMSIZE_KEY")->AsInteger;
		}
		else if (IBInternalQuery->RecordCount > 1)
		{ // Unable to find product
         UnicodeString ErrMsg = "Web PLU Code not unique";
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG,ErrMsg);
		}
		else
		{
         UnicodeString ErrMsg = "Web PLU Code not Found";
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG,ErrMsg);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TDBWebUtil::CreateWebOrder(Database::TDBTransaction &DBTransaction)
{
   int WebKey = 0;
	try
   {
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_WEBORDERS_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		WebKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO WEBORDERS ("
          "WEBORDER_KEY,"
          "GUID,"
          "NAME,"
          "STORE_NAME,"
          "ORDER_DATE,"
          "EXPECTED_DATE,"
          "TAB_KEY,"
          "TIME_KEY,"
          "STATUS,"
          "PREPAID, "
          "CONTACTS_KEY)"
          " VALUES ("
			 ":WEBORDER_KEY,"
          ":GUID,"
          ":NAME,"
          ":STORE_NAME,"
          ":ORDER_DATE,"
          ":EXPECTED_DATE,"
          ":TAB_KEY,"
          ":TIME_KEY,"
          ":STATUS,"
          ":PREPAID, "
          ":CONTACTS_KEY);";

		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebKey;
		IBInternalQuery->ParamByName("STORE_NAME")->AsString = "";
		IBInternalQuery->ParamByName("GUID")->AsString = "";
		IBInternalQuery->ParamByName("NAME")->AsString = "";
		IBInternalQuery->ParamByName("ORDER_DATE")->AsDateTime = Now();
		IBInternalQuery->ParamByName("EXPECTED_DATE")->AsDateTime = Now();
		IBInternalQuery->ParamByName("STATUS")->AsInteger = ewosReceived;
		IBInternalQuery->ParamByName("PREPAID")->AsCurrency = 0;
		IBInternalQuery->ParamByName("TAB_KEY")->Clear();
		IBInternalQuery->ParamByName("TIME_KEY")->Clear();
		IBInternalQuery->ParamByName("CONTACTS_KEY")->Clear();
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return WebKey;
}

int TDBWebUtil::CreateWebOrderFromXML(Database::TDBTransaction &DBTransaction, _di_IXMLORDERType &WebOrder)
{

	int WebKey = GetOrderKey(DBTransaction,WebOrder->Get_GUID());

   if (WebKey != 0) {
      return WebKey;
   }

	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_WEBORDERS_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		WebKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO WEBORDERS ("
          "WEBORDER_KEY,"
          "GUID,"
          "NAME,"
          "STORE_NAME,"
          "ORDER_DATE,"
          "EXPECTED_DATE,"
          "TAB_KEY,"
          "TIME_KEY,"
          "STATUS,"
          "PREPAID,"
          "CONTACTS_KEY)"
          " VALUES ("
			 ":WEBORDER_KEY,"
          ":GUID,"
          ":NAME,"
          ":STORE_NAME,"
          ":ORDER_DATE,"
          ":EXPECTED_DATE,"
          ":TAB_KEY,"
          ":TIME_KEY,"
          ":STATUS,"
          ":PREPAID,"
          ":CONTACTS_KEY);";

		// TFormatSettings Settings = GetLocaleFormatSettings();
		TFormatSettings Settings;
		GetLocaleFormatSettings(10955, Settings);
		Settings.ShortDateFormat = "yyyy-MM-dd";
		Settings.ShortTimeFormat = "hh:mm:ss";
		Settings.LongDateFormat = "yyyy-MM-dd";
		Settings.LongTimeFormat = "hh:mm:ss";
		Settings.TimeSeparator = wchar_t(':');
		Settings.DateSeparator = wchar_t('-');

		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebKey;
		IBInternalQuery->ParamByName("STORE_NAME")->AsString = WebOrder->Get_STORE_NAME().SubString(1,50);
		IBInternalQuery->ParamByName("GUID")->AsString = WebOrder->Get_GUID().SubString(1,50);
		IBInternalQuery->ParamByName("NAME")->AsString = WebOrder->Get_ORDER_NAME().SubString(1,50);
		IBInternalQuery->ParamByName("ORDER_DATE")->AsDateTime = StrToDateTime(WebOrder->Get_ORDER_DATE(), Settings);
		IBInternalQuery->ParamByName("EXPECTED_DATE")->AsDateTime = StrToDateTime(WebOrder->Get_EXPECTED_DATE(), Settings);
		IBInternalQuery->ParamByName("STATUS")->AsInteger = ewosReceived;
		_di_IXMLACCOUNTType OrderAccount = WebOrder->ACCOUNT;
  		IBInternalQuery->ParamByName("PREPAID")->AsCurrency = ProcessPrePay(OrderAccount);
		IBInternalQuery->ParamByName("CONTACTS_KEY")->Clear();
		IBInternalQuery->ParamByName("TAB_KEY")->Clear();
		IBInternalQuery->ParamByName("TIME_KEY")->Clear();
		IBInternalQuery->ExecQuery();

        ProcessWebOrderData(DBTransaction,WebKey,WebOrder);
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return WebKey;
}

void TDBWebUtil::ProcessWebOrderData(Database::TDBTransaction &DBTransaction,int WebKey,_di_IXMLORDERType &WebOrder)
{
	try
	{
        UnicodeString category = "";
        UnicodeString element = "";
        UnicodeString value = "";
        _di_IXMLACCOUNTType Account = WebOrder->ACCOUNT;
        _di_IXMLDELEVERYType DeliveryInfo = Account->Get_DELEVERY();
        if(DeliveryInfo)
        {
            category = DeliveryInfo->NodeName;
         	_di_IXMLNodeList List = DeliveryInfo->GetChildNodes();

            for(int i = 0; i < List->Count; i++)
            {
                _di_IXMLNode Node = List->Get(i);
                element = Node->NodeName;
                value = Node->Text;
                ProcessWebOrderElement(DBTransaction,WebKey,category,element,value);

            }
        }

        _di_IXMLPAYMENTSType Payments = Account->Get_PAYMENTS();
        if(Payments)
        {
            _di_IXMLPAYMENT_INFOType PaymentInfo = Payments->Get_PAYMENT_INFO();
            if(PaymentInfo)
        	{
            	category = PaymentInfo->NodeName;
         		_di_IXMLNodeList List = PaymentInfo->GetChildNodes();
            	for(int i = 0; i < List->Count; i++)
            	{
                	_di_IXMLNode Node = List->Get(i);
                	element = Node->NodeName;
                	value = Node->Text;
                	ProcessWebOrderElement(DBTransaction,WebKey,category,element,value);
            	}
            }
        }

        _di_IXMLCOMMENTSType Comments = Account->Get_COMMENTS();
        if(Comments)
        {
            category = Comments->NodeName;
            _di_IXMLNodeList List = Comments->GetChildNodes();
            for(int i = 0; i < List->Count; i++)
            {
                _di_IXMLNode Node = List->Get(i);
                element = Node->NodeName;
                value = Node->Text;
                ProcessWebOrderElement(DBTransaction,WebKey,category,element,value);
            }
        }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::ProcessWebOrderElement(Database::TDBTransaction &DBTransaction,int WebKey,
	UnicodeString Category, UnicodeString Element, UnicodeString Value )
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GEN_ID(GEN_WEBDATA_KEY, 1) FROM RDB$DATABASE";
		IBInternalQuery->ExecQuery();
		int WebDataKey = IBInternalQuery->Fields[0]->AsInteger;

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "INSERT INTO WEBDATA ("
          "WEBDATA_KEY,"
          "WEBORDER_KEY,"
          "CATEGORY,"
          "ELEMENT,"
          "DATA)"
          " VALUES ("
          ":WEBDATA_KEY,"
          ":WEBORDER_KEY,"
          ":CATEGORY,"
          ":ELEMENT,"
          ":DATA);";

		IBInternalQuery->ParamByName("WEBDATA_KEY")->AsInteger = WebDataKey;
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebKey;
		IBInternalQuery->ParamByName("CATEGORY")->AsString = Category.SubString(1,60);
		IBInternalQuery->ParamByName("ELEMENT")->AsString = Element.SubString(1,60);
		IBInternalQuery->ParamByName("DATA")->AsString = Value.SubString(1,60);
		IBInternalQuery->ExecQuery();
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::getWebOrderExtraData(Database::TDBTransaction &DBTransaction,int WebKey,UnicodeString Category, TStringList *Strings)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "SELECT * FROM WEBDATA WHERE WEBORDER_KEY = :WEBORDER_KEY AND CATEGORY = :CATEGORY "
             " ORDER BY WEBDATA_KEY";

		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebKey;
		IBInternalQuery->ParamByName("CATEGORY")->AsString = Category;
		IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
		    Strings->Add(IBInternalQuery->FieldByName("DATA")->AsString);
		}
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

int TDBWebUtil::GetWebOrderCovers(Database::TDBTransaction &DBTransaction, int inWebOrderKey )
{
    return 1;
}

int TDBWebUtil::FindOrCreateWebOrder(Database::TDBTransaction &DBTransaction,int WebKey)
{
   int RetVal = 0;
	try
	{
		if (GetWebOrderExists(DBTransaction,WebKey))
      {
      	RetVal = WebKey;
   	}
      else
      {
			RetVal = CreateWebOrder(DBTransaction);
      }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return RetVal;
}

bool TDBWebUtil::CheckItemCompleteEnabled(Database::TDBTransaction &DBTransaction, int SizeKey)
{
	bool RetVal = false;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "Select Item.Enabled From ItemSize Left Join Item on Item.Item_Key = ItemSize.Item_Key "
			 "Where ITEMSIZE.ITEMSIZE_KEY = :ITEMSIZE_KEY Order By Item.IAO";
		IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = SizeKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = (IBInternalQuery->FieldByName("Enabled")->AsString == "T");
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TDBWebUtil::LoadItemComplete(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int SizeKey)
{
	bool Retval = false;
	try
	{
		const UnicodeString MenuSQL =
  "Select "
  "   Menu.Menu_Key, Menu.Menu_Name, Menu.Menu_Type, "
  "   Course.Course_Key, Course.Course_ID, Course.Course_Name, Course.Course_Kitchen_Name, Course.No_Default_Serving_Course, "
  "   Course.ServingCourses_Key, "
  "   ServingCourses.ServingCourse_Name, ServingCourses.SERVINGCOURSE_KITCHEN_NAME, "
  "   Item.Item_Key, Item.Item_Name, Item.IAO, Item.Item_Kitchen_Name, Item.Enabled, Item.Item_Category, Item.Item_ID, "
  "   Item.Print_Chit, Item.Display_Sizes, Item.Print_Underlined, Item.Print_Bold, Item.Print_Colour, Item.Print_Font, "
  "   Item.Print_Double_Width, Item.Print_Double_Height, "
  "   ItemSize.ItemSize_Key, ItemSize.Size_ID, ItemSize.Size_Name, ItemSize.Size_Kitchen_Name, ItemSize.Price, "
  "   ItemSize.Special_Price, ItemSize.GST_Percent, ItemSize.Cost, ItemSize.Cost_GST_Percent, ItemSize.Free, "
  "   ItemSize.Points_Percent, ItemSize.Setmenu_Mask, ItemSize.Mem_Sale_Count, ItemSize.Mem_Discount_Percent, "
  "   ItemSize.Loc_Sale_Count, ItemSize.Loc_Discount_Percent, ItemSize.PLU, ArcCategories.Category, ArcCategories.Category_Key, "
  "   CategoryGroups.Name Category_Group_Name, CategoryGroups.CategoryGroups_Key, ItemSize.ThirdPartyCodes_Key, "
  "   ThirdPartyCodes.Code ThirdPartyCode "
  "From ItemSize "
  "   Left Join ThirdPartyCodes On ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
  "   Left Join Item on Item.Item_Key = ItemSize.Item_Key "
  "   Left Join Course on  Course.Course_Key = Item.Course_Key "
  "   Left Join ServingCourses On Course.ServingCourses_Key = ServingCourses.ServingCourses_Key "
  "   Left Join Menu on Menu.Menu_Key =  Course.Menu_Key "
  "   Left Join ArcCategories on  ItemSize.Category_Key = ArcCategories.Category_Key "
  "   Left Join CategoryGroups on CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key "
  "Where "
  "	ItemSize.ITEMSIZE_KEY = :ITEMSIZE_KEY "
  "Order By "
  "   Menu.Menu_Name, "
  "   Course.CAO, "
  "   Item.IAO, "
  "   ItemSize.ISAO ";

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = MenuSQL;
		IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = SizeKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount == 1)
		{ // If you find more than one product load them all?
      	Retval = true;
			// Read Menu Stuff
			Order->MenuKey = IBInternalQuery->FieldByName("Menu_Key")->AsInteger;
			Order->MenuName = IBInternalQuery->FieldByName("Menu_Name")->AsString;
			Order->ItemType = static_cast <TItemType> (IBInternalQuery->FieldByName("Menu_Type")->AsInteger);
			Order->OrderType = NormalOrder;
			Order->Course = IBInternalQuery->FieldByName("Course_Name")->AsString;
			Order->CourseKitchenName = IBInternalQuery->FieldByName("Course_Kitchen_Name")->AsString;

			if (Order->CourseKitchenName == "")
				Order->CourseKitchenName = Order->Course;

			Order->ServingCourse.Name = IBInternalQuery->FieldByName("ServingCourse_Name")->AsString;
			Order->ServingCourse.KitchenName = IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString;
			Order->ServingCourse.ServingCourseKey = IBInternalQuery->FieldByName("ServingCourses_Key")->AsInteger;

			if (Order->ServingCourse.KitchenName == "")
				Order->ServingCourse.KitchenName = Order->ServingCourse.Name;

			Order->ItemKey = IBInternalQuery->FieldByName("Item_Key")->AsInteger;
			Order->ItemAppearanceOrder = IBInternalQuery->FieldByName("IAO")->AsInteger;
			Order->Item = IBInternalQuery->FieldByName("Item_Name")->AsString;
			Order->ItemKitchenName = IBInternalQuery->FieldByName("Item_Kitchen_Name")->AsString;
			Order->OriginalItemKey = IBInternalQuery->FieldByName("Item_Key")->AsInteger;

			if (Order->ItemKitchenName == "")
				Order->ItemKitchenName = Order->Item;

			Order->Item_ID = IBInternalQuery->FieldByName("Item_ID")->AsInteger;
			Order->PrintChitNumber = (IBInternalQuery->FieldByName("Print_Chit")->AsString == "T");
			Order->FontInfo.Underlined = (IBInternalQuery->FieldByName("Print_Underlined")->AsString == "T");
			Order->FontInfo.Bold = (IBInternalQuery->FieldByName("Print_Bold")->AsString == "T");
			Order->FontInfo.Colour = (IBInternalQuery->FieldByName("Print_Colour")->AsInteger == 0) ? fcBlack : fcRed;
			Order->FontInfo.Font = (IBInternalQuery->FieldByName("Print_Font")->AsInteger == 0) ? ftFontA : ftFontB;
			Order->FontInfo.Width = (IBInternalQuery->FieldByName("Print_Double_Width")->AsString == "T") ? fsDoubleSize : fsNormalSize;
			Order->FontInfo.Height = (IBInternalQuery->FieldByName("Print_Double_Height")->AsString == "T") ? fsDoubleSize : fsNormalSize;

			Order->Size = IBInternalQuery->FieldByName("Size_Name")->AsString;
			Order->SizeKitchenName = IBInternalQuery->FieldByName("Size_Kitchen_Name")->AsString;
			if (Order->SizeKitchenName == "")
				Order->SizeKitchenName = Order->Size;

			Order->PriceLevel0 = IBInternalQuery->FieldByName("Price")->AsCurrency;
			Order->PriceLevel1 = IBInternalQuery->FieldByName("Special_Price")->AsCurrency;
			//Order->GSTPercent = IBInternalQuery->FieldByName("GST_Percent")->AsDouble;
            Order->TaxProfiles.push_back(TaxProfile("GST_Percent", IBInternalQuery->FieldByName("GST_Percent")->AsCurrency, SalesTax, 1,0));
			Order->Cost = IBInternalQuery->FieldByName("Cost")->AsCurrency;
			Order->CostGSTPercent = IBInternalQuery->FieldByName("Cost_GST_Percent")->AsDouble;
			Order->PointsPercent = IBInternalQuery->FieldByName("Points_Percent")->AsDouble;
			Order->Categories->FinancialCategory = IBInternalQuery->FieldByName("Category")->AsString;
			Order->Categories->FinancialCategoryKey = IBInternalQuery->FieldByName("Category_Key")->AsInteger;
			Order->Categories->FinancialCategoryGroup = IBInternalQuery->FieldByName("Category_Group_Name")->AsString;

			Order->SetMenuMask = IBInternalQuery->FieldByName("Setmenu_Mask")->AsInteger;
			Order->MemberFreeSaleCount = IBInternalQuery->FieldByName("Mem_Sale_Count")->AsInteger;
			Order->MemberFreeSaleDiscount = IBInternalQuery->FieldByName("Mem_Discount_Percent")->AsDouble;
			Order->LocationFreeSaleCount = IBInternalQuery->FieldByName("Loc_Sale_Count")->AsInteger;
			Order->LocationFreeSaleDiscount = IBInternalQuery->FieldByName("Loc_Discount_Percent")->AsDouble;

			Order->ThirdPartyCode = IBInternalQuery->FieldByName("ThirdPartyCode")->AsString;
			Order->ThirdPartyKey = IBInternalQuery->FieldByName("ThirdPartyCodes_Key")->AsInteger;
			Order->PLU = IBInternalQuery->FieldByName("PLU")->AsInteger;

         LoadBreakDownCategories(DBTransaction,Order->Categories,SizeKey);
		}
		else if (IBInternalQuery->RecordCount > 1)
		{ // Unable to find product
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web PLU Code not unique");
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web PLU Code not Found");
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return Retval;
}

void TDBWebUtil::LoadBreakDownCategories(Database::TDBTransaction &DBTransaction,TListCategoryContainer *CategoryContainer,int SizeKey)
{
	try
	{
      const UnicodeString SizeCategoriesSQL =
         "Select "
            "ItemSizeCategory.ItemSize_Key,"
            "ArcCategories.Category_Key, "
            "ArcCategories.Category "
         "From "
            "ItemSizeCategory "
            "Left Join ArcCategories On "
               "ArcCategories.Category_Key = ItemSizeCategory.Category_Key "
         "Where "
            "ItemSizeCategory.ItemSize_Key = :ITEMSIZE_KEY "
         "Order By "
            "ArcCategories.Category";

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = SizeCategoriesSQL;
		IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = SizeKey;
		IBInternalQuery->ExecQuery();

		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
		   TItemSizeCategory *Cat = new TItemSizeCategory();
			Cat->Category = IBInternalQuery->FieldByName("Category")->AsString;
			Cat->CategoryKey = IBInternalQuery->FieldByName("Category_Key")->AsInteger;
			CategoryContainer->CategoryAdd(Cat);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::LoadSalesRecipes(Database::TDBTransaction &DBTransaction,TListRecipeContainer *RecipeContainer,int SizeKey, double OrderQty)
{
try
	{
      const UnicodeString SizeCategoriesSQL =
         "Select "
         "Recipe.Recipe_Key,"
         "Recipe.Stock_Code,"
         "Recipe.Qty,"
         "StockCosts.Cost_GST_Percent,"
         "StockCosts.Cost Stock_Unit_Cost,"
         "Recipe.Stock_Location "
         "From "
         " Recipe "
            "Left Join StockCosts On "
               "StockCosts.Stock_Code = Recipe.Stock_Code and StockCosts.Location = Recipe.Stock_Location "
         "Where "
         " Recipe.ITEMSIZE_KEY = :ITEMSIZE_KEY";

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = SizeCategoriesSQL;
		IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = SizeKey;
		IBInternalQuery->ExecQuery();

		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
         TItemRecipe *RecipeItem = new TItemRecipe;
         RecipeItem->StockCode 		= IBInternalQuery->FieldByName("Stock_Code")->AsString;
         RecipeItem->StockLocation 	= IBInternalQuery->FieldByName("Stock_Location")->AsString;
         RecipeItem->Qty 				= IBInternalQuery->FieldByName("Qty")->AsDouble * OrderQty;
         RecipeItem->Cost 				= IBInternalQuery->FieldByName("Stock_Unit_Cost")->AsCurrency * RecipeItem->Qty;
         RecipeItem->CostGSTPercent = IBInternalQuery->FieldByName("Cost_GST_Percent")->AsCurrency;
         RecipeContainer->RecipeAdd(RecipeItem);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}


bool TDBWebUtil::LoadItemComplete(Database::TDBTransaction &DBTransaction, TItemCompleteSub *Order, int SizeKey)
{
	bool Retval = false;
	try
	{
		const UnicodeString MenuSQL =
  "Select "
  "   Menu.Menu_Key, Menu.Menu_Name, Menu.Menu_Type, "
  "   Course.Course_Key, Course.Course_ID, Course.Course_Name, Course.Course_Kitchen_Name, Course.No_Default_Serving_Course, "
  "   Course.ServingCourses_Key, "
  "   ServingCourses.ServingCourse_Name, ServingCourses.SERVINGCOURSE_KITCHEN_NAME, "
  "   Item.Item_Key, Item.Item_Name, Item.IAO, Item.Item_Kitchen_Name, Item.Enabled, Item.Item_Category, Item.Item_ID, "
  "   Item.Print_Chit, Item.Display_Sizes, Item.Print_Underlined, Item.Print_Bold, Item.Print_Colour, Item.Print_Font, "
  "   Item.Print_Double_Width, Item.Print_Double_Height, "
  "   ItemSize.ItemSize_Key, ItemSize.Size_ID, ItemSize.Size_Name, ItemSize.Size_Kitchen_Name, ItemSize.Price, "
  "   ItemSize.Special_Price, ItemSize.GST_Percent, ItemSize.Cost, ItemSize.Cost_GST_Percent, ItemSize.Free, "
  "   ItemSize.Points_Percent, ItemSize.Setmenu_Mask, ItemSize.Mem_Sale_Count, ItemSize.Mem_Discount_Percent, "
  "   ItemSize.Loc_Sale_Count, ItemSize.Loc_Discount_Percent,  ItemSize.PLU, ArcCategories.Category, ArcCategories.Category_Key, "
  "   CategoryGroups.Name Category_Group_Name, CategoryGroups.CategoryGroups_Key, ItemSize.ThirdPartyCodes_Key, "
  "   ThirdPartyCodes.Code ThirdPartyCode "
  "From ItemSize "
  "   Left Join ThirdPartyCodes On ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdPartyCodes_Key "
  "   Left Join Item on Item.Item_Key = ItemSize.Item_Key "
  "   Left Join Course on  Course.Course_Key = Item.Course_Key "
  "   Left Join ServingCourses On Course.ServingCourses_Key = ServingCourses.ServingCourses_Key "
  "   Left Join Menu on Menu.Menu_Key =  Course.Menu_Key "
  "   Left Join ArcCategories on  ItemSize.Category_Key = ArcCategories.Category_Key "
  "   Left Join CategoryGroups on CategoryGroups.CategoryGroups_Key = ArcCategories.CategoryGroups_Key "
  "Where "
  "	ItemSize.ITEMSIZE_KEY = :ITEMSIZE_KEY "
  "Order By "
  "   Menu.Menu_Name, "
  "   Course.CAO, "
  "   Item.IAO, "
  "   ItemSize.ISAO ";

		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = MenuSQL;
		IBInternalQuery->ParamByName("ITEMSIZE_KEY")->AsInteger = SizeKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount == 1)
		{ // If you find more than one product load them all?
      	Retval = true;
			// Read Menu Stuff
			Order->MenuName = IBInternalQuery->FieldByName("Menu_Name")->AsString;
			Order->OrderType = NormalOrder;
			Order->Course = IBInternalQuery->FieldByName("Course_Name")->AsString;
			Order->CourseKitchenName = IBInternalQuery->FieldByName("Course_Kitchen_Name")->AsString;

			if (Order->CourseKitchenName == "")
				Order->CourseKitchenName = Order->Course;

			Order->ServingCourse.Name = IBInternalQuery->FieldByName("ServingCourse_Name")->AsString;
			Order->ServingCourse.KitchenName = IBInternalQuery->FieldByName("SERVINGCOURSE_KITCHEN_NAME")->AsString;
			Order->ServingCourse.ServingCourseKey = IBInternalQuery->FieldByName("ServingCourses_Key")->AsInteger;

			if (Order->ServingCourse.KitchenName == "")
				Order->ServingCourse.KitchenName = Order->ServingCourse.Name;

			Order->ItemKey = IBInternalQuery->FieldByName("Item_Key")->AsInteger;
			Order->ItemAppearanceOrder = IBInternalQuery->FieldByName("IAO")->AsInteger;
			Order->Item = IBInternalQuery->FieldByName("Item_Name")->AsString;
			Order->ItemKitchenName = IBInternalQuery->FieldByName("Item_Kitchen_Name")->AsString;

			if (Order->ItemKitchenName == "")
				Order->ItemKitchenName = Order->Item;

			Order->Item_ID = IBInternalQuery->FieldByName("Item_ID")->AsInteger;
			Order->FontInfo.Underlined = (IBInternalQuery->FieldByName("Print_Underlined")->AsString == "T");
			Order->FontInfo.Bold = (IBInternalQuery->FieldByName("Print_Bold")->AsString == "T");
			Order->FontInfo.Colour = (IBInternalQuery->FieldByName("Print_Colour")->AsInteger == 0) ? fcBlack : fcRed;
			Order->FontInfo.Font = (IBInternalQuery->FieldByName("Print_Font")->AsInteger == 0) ? ftFontA : ftFontB;
			Order->FontInfo.Width = (IBInternalQuery->FieldByName("Print_Double_Width")->AsString == "T") ? fsDoubleSize : fsNormalSize;
			Order->FontInfo.Height = (IBInternalQuery->FieldByName("Print_Double_Height")->AsString == "T") ? fsDoubleSize : fsNormalSize;

			Order->Size = IBInternalQuery->FieldByName("Size_Name")->AsString;
			Order->SizeKitchenName = UTF8ToUnicodeString((AnsiString)IBInternalQuery->FieldByName("Size_Kitchen_Name")->AsString);
			if (Order->SizeKitchenName == "")
				Order->SizeKitchenName = Order->Size;

			Order->PriceLevel0 = IBInternalQuery->FieldByName("Price")->AsCurrency;
			Order->PriceLevel1 = IBInternalQuery->FieldByName("Special_Price")->AsCurrency;
			//Order->GSTPercent = IBInternalQuery->FieldByName("GST_Percent")->AsDouble;
            Order->TaxProfiles.push_back(TaxProfile("GST_Percent", IBInternalQuery->FieldByName("GST_Percent")->AsCurrency, SalesTax, 1,0));
			Order->Cost = IBInternalQuery->FieldByName("Cost")->AsCurrency;
			Order->CostGSTPercent = IBInternalQuery->FieldByName("Cost_GST_Percent")->AsDouble;
			Order->PointsPercent = IBInternalQuery->FieldByName("Points_Percent")->AsDouble;
			Order->Categories->FinancialCategory = IBInternalQuery->FieldByName("Category")->AsString;
			Order->Categories->FinancialCategoryKey = IBInternalQuery->FieldByName("Category_Key")->AsInteger;
			Order->Categories->FinancialCategoryGroup = IBInternalQuery->FieldByName("Category_Group_Name")->AsString;

			Order->SetMenuMask = IBInternalQuery->FieldByName("Setmenu_Mask")->AsInteger;
			Order->MemberFreeSaleCount = IBInternalQuery->FieldByName("Mem_Sale_Count")->AsInteger;
			Order->MemberFreeSaleDiscount = IBInternalQuery->FieldByName("Mem_Discount_Percent")->AsDouble;
			Order->LocationFreeSaleCount = IBInternalQuery->FieldByName("Loc_Sale_Count")->AsInteger;
			Order->LocationFreeSaleDiscount = IBInternalQuery->FieldByName("Loc_Discount_Percent")->AsDouble;

			Order->ThirdPartyCode = IBInternalQuery->FieldByName("ThirdPartyCode")->AsString;
			Order->ThirdPartyKey = IBInternalQuery->FieldByName("ThirdPartyCodes_Key")->AsInteger;
			Order->PLU = IBInternalQuery->FieldByName("PLU")->AsInteger;

         LoadBreakDownCategories(DBTransaction,Order->Categories,SizeKey);
		}
		else if (IBInternalQuery->RecordCount > 1)
		{ // Unable to find product
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web PLU Code not unique");
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web PLU Code not Found");
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return Retval;
}

bool TDBWebUtil::LoadItemOption(Database::TDBTransaction &DBTransaction, TItemOption &Option)
{
   bool Retval = false;
	try
	{

		const UnicodeString OptionSQL = "Select * from OPTIONS where OPTIONS_KEY = :OPTIONS_KEY ";
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());
		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = OptionSQL;
		IBInternalQuery->ParamByName("OPTIONS_KEY")->AsInteger = Option.OptionKey;
		IBInternalQuery->ExecQuery();
		if (IBInternalQuery->RecordCount == 1)
		{ // If you find more than one product load them all?
      	Retval = true;
         Option.Name = IBInternalQuery->FieldByName("Option_Name")->AsString;
         Option.KitchenName = IBInternalQuery->FieldByName("Option_Kitchen_Name")->AsString;
         if(Option.KitchenName == "")
         {
				Option.KitchenName = Option.Name;
         }
         Option.OptionID = IBInternalQuery->FieldByName("Option_ID")->AsInteger;
         Option.ForcedMask = IBInternalQuery->FieldByName("Forced_Mask")->AsInteger;
         Option.Flags = IBInternalQuery->FieldByName("Flags")->AsInteger;
         Option.GroupNumber = IBInternalQuery->FieldByName("Flags")->AsInteger & 0x0F;
         Option.DisallowMuliSelect = IBInternalQuery->FieldByName("Flags")->AsInteger & 0x10;
         Option.PlusOption = IBInternalQuery->FieldByName("Flags")->AsInteger & 0x20;
         Option.MinusOption = IBInternalQuery->FieldByName("Flags")->AsInteger & 0x40;
         Option.AllowSkip = IBInternalQuery->FieldByName("Flags")->AsInteger & 0x80;
         Option.FontInfo.Underlined = (IBInternalQuery->FieldByName("Print_Underlined")->AsString == "T");
         Option.FontInfo.Bold = (IBInternalQuery->FieldByName("Print_Bold")->AsString == "T");
         Option.FontInfo.Colour = (IBInternalQuery->FieldByName("Print_Colour")->AsInteger == 0) ? fcBlack : fcRed;;
         Option.FontInfo.Font = IBInternalQuery->FieldByName("Print_Font")->AsInteger;
         Option.FontInfo.Width = (IBInternalQuery->FieldByName("Print_Double_Width")->AsString == "T") ? fsDoubleSize : fsNormalSize;
         Option.FontInfo.Height = (IBInternalQuery->FieldByName("Print_Double_Height")->AsString == "T") ? fsDoubleSize : fsNormalSize;
		}
		else if (IBInternalQuery->RecordCount > 1)
		{ // Unable to find product
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web Option PLU Code not unique");
		}
		else
		{
			TManagerLogs::Instance().Add(__FUNC__, WEBMATELOG, "Web Option PLU Code not Found");
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
   return Retval;
}

int TDBWebUtil::GetWebOrderKeyByTabKey(Database::TDBTransaction &DBTransaction, int TabKey)
{
	int RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT WEBORDER_KEY FROM WEBORDERS WHERE TAB_KEY = :TAB_KEY";
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("WEBORDER_KEY")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TDBWebUtil::GetTabKeyByWebOrderKey(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	int RetVal = 0;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT TAB_KEY FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TDBWebUtil::SetTabKey(Database::TDBTransaction &DBTransaction,int WebOrderKey,int TabKey)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET TAB_KEY = :TAB_KEY WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("TAB_KEY")->AsInteger = TabKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::SetWebOrderStatus(Database::TDBTransaction &DBTransaction,int WebOrderKey,WebOrderStatus Status)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET STATUS = :STATUS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("STATUS")->AsInteger = Status;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::SetWebOrderTimeKey(Database::TDBTransaction &DBTransaction,int WebOrderKey,int TimeKey)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET TIME_KEY = :TIME_KEY WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("TIME_KEY")->AsInteger = TimeKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::SetWebOrderContactKey(Database::TDBTransaction &DBTransaction,int WebOrderKey,int ContactsKey)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET CONTACTS_KEY = :CONTACTS_KEY WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("CONTACTS_KEY")->AsInteger = ContactsKey;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}


bool TDBWebUtil::GetWebOrderExists(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	bool RetVal = false;
   if(WebOrderKey == 0 ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT WEBORDER_KEY FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}
/*
UnicodeString TDBWebUtil::GetPartyName(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	UnicodeString RetVal = "";
   if(WebOrderKey == 0 ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT PARTY_NAME FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("PARTY_NAME")->AsString;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TDBWebUtil::SetPartyName(Database::TDBTransaction &DBTransaction,int WebOrderKey,UnicodeString PartyName)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET PARTY_NAME = :PARTY_NAME WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("PARTY_NAME")->AsString = PartyName;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}*/

void TDBWebUtil::SetOrderGUID(Database::TDBTransaction &DBTransaction,int WebOrderKey,UnicodeString GUID)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "UPDATE WEBORDERS SET GUID = :GUID WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ParamByName("GUID")->AsString = GUID;
		IBInternalQuery->ExecQuery();
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

UnicodeString TDBWebUtil::GetOrderGUID(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	UnicodeString RetVal = "";
   if(WebOrderKey == 0 ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT GUID FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("GUID")->AsString;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TDBWebUtil::GetOrderKey(Database::TDBTransaction &DBTransaction, UnicodeString GUID)
{
	int RetVal = 0;
   if(GUID == "" ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT WEBORDER_KEY FROM WEBORDERS WHERE GUID = :GUID";
		IBInternalQuery->ParamByName("GUID")->AsString = GUID;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("WEBORDER_KEY")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

int TDBWebUtil::GetKeyByOrderGUID(Database::TDBTransaction &DBTransaction, UnicodeString GUID)
{
	int RetVal = 0;
   if(GUID == "" ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT WEBORDER_KEY FROM WEBORDERS WHERE GUID = :GUID";
		IBInternalQuery->ParamByName("GUID")->AsString = GUID;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("WEBORDER_KEY")->AsInteger;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

bool TDBWebUtil::WebOrdersPending(Database::TDBTransaction &DBTransaction)
{
	bool RetVal = false;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT * FROM WEBORDERS WHERE STATUS = :STATUS";
		IBInternalQuery->ParamByName("STATUS")->AsInteger = ewosReceived;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = true;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

void TDBWebUtil::LoadWebOrders(Database::TDBTransaction &DBTransaction, TWebOrderContainer &WebOrderContainer)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT * FROM WEBORDERS WHERE STATUS = :STATUS ORDER BY ORDER_DATE DESC";
		IBInternalQuery->ParamByName("STATUS")->AsInteger = ewosReceived;
		IBInternalQuery->ExecQuery();

		for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
         TWebOrder WebOrder;
         WebOrder.WebKey 			= IBInternalQuery->FieldByName("WEBORDER_KEY")->AsInteger;
         WebOrder.TabKey 			= IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
         WebOrder.ContactKey		= IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
         WebOrder.TimeKey			= IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
         WebOrder.GUID 				= IBInternalQuery->FieldByName("GUID")->AsString;
         WebOrder.StoreName 		= IBInternalQuery->FieldByName("STORE_NAME")->AsString;
         WebOrder.DateOrdered 	= IBInternalQuery->FieldByName("ORDER_DATE")->AsDateTime;
         WebOrder.DateExpected 	= IBInternalQuery->FieldByName("EXPECTED_DATE")->AsDateTime;
			WebOrderContainer.Next.push(WebOrder);
		}

      if(WebOrderContainer.hasnext())
      {
      	WebOrderContainer.Current = WebOrderContainer.Next.top();
      	WebOrderContainer.Next.pop();
      }
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::LoadWebOrder(Database::TDBTransaction &DBTransaction, int WebOrderKey, TWebOrder &WebOrder)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT * FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY ";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount != 0)
		{
         TWebOrder WebOrder;
         WebOrder.WebKey 			= IBInternalQuery->FieldByName("WEBORDER_KEY")->AsInteger;
         WebOrder.TabKey 			= IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
         WebOrder.TimeKey			= IBInternalQuery->FieldByName("TIME_KEY")->AsInteger;
         WebOrder.ContactKey		= IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
         WebOrder.GUID	 			= IBInternalQuery->FieldByName("GUID")->AsString;
         WebOrder.StoreName 		= IBInternalQuery->FieldByName("STORE_NAME")->AsString;
         WebOrder.DateOrdered 	= IBInternalQuery->FieldByName("ORDER_DATE")->AsDateTime;
         WebOrder.DateExpected 	= IBInternalQuery->FieldByName("EXPECTED_DATE")->AsDateTime;
         WebOrder.PrePaid			= IBInternalQuery->FieldByName("PREPAID")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void TDBWebUtil::DeleteWebOrder(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	TWebOrder WebOrder;
	LoadWebOrder(DBTransaction, WebOrderKey, WebOrder);

	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "DELETE FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY ";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ExecQuery();

	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}

}

void TDBWebUtil::getWebOrderDetials(Database::TDBTransaction &DBTransaction, int WebOrderKey, TStringList &Detials)
{
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT * FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY ";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if(IBInternalQuery->RecordCount != 0)
		{
			Detials.Add("Web Order Info");
			Detials.Add("Store Name : " + IBInternalQuery->FieldByName("STORE_NAME")->AsString);
			Detials.Add("PrePaid : " + CurrToStrF(IBInternalQuery->FieldByName("PREPAID")->AsCurrency, ffCurrency, CurrencyDecimals));
			Detials.Add("ID : " + IBInternalQuery->FieldByName("GUID")->AsString);
			Detials.Add("Ordered : " + IBInternalQuery->FieldByName("ORDER_DATE")->AsDateTime.FormatString("dd-mm-yyyy HH:MM:SS"));
			Detials.Add("Required : " + IBInternalQuery->FieldByName("EXPECTED_DATE")->AsDateTime.FormatString("dd-mm-yyyy HH:MM:SS"));
            Detials.Add(IBInternalQuery->FieldByName("NAME")->AsString);
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

void
TDBWebUtil::GetUnpaidTabs(Database::TDBTransaction &tr, TStringList *tl)
{
	try {
		TIBSQL *qr = tr.Query(tr.AddQuery());

		qr->SQL->Text = "select tab_key, "
		                "       name "
		                "       from weborders "
		                "       where tab_key is not null "
		                "       order by order_date desc;";

		qr->ExecQuery();
		for (int i; !qr->Eof; qr->Next()) {
			i = tl->Add(qr->FieldByName("name")->AsString);
			tl->Objects[i] =
			  reinterpret_cast<TObject *>(qr->FieldByName("tab_key")->AsInteger);
		}

		qr->Close();
	} catch (Exception &e) {
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, e.Message);
		throw;
	}
}

void TDBWebUtil::GetTabs(Database::TDBTransaction &DBTransaction,TStringList * TabList,WebOrderStatus Status)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

      TabList->Clear();
      IBInternalQuery->SQL->Text =
         "SELECT "
            "WEBORDERS.TAB_KEY,WEBORDERS.NAME "
         "FROM "
            "WEBORDERS "
			"WHERE STATUS = :STATUS ORDER BY ORDER_DATE DESC";
		IBInternalQuery->ParamByName("STATUS")->AsInteger = Status;
      IBInternalQuery->ExecQuery();
      while (!IBInternalQuery->Eof)
      {
         int Index = TabList->Add(IBInternalQuery->FieldByName("NAME")->AsString);
         // Little bit of pointer abuse but we just want to store the int somewhere.
         TabList->Objects[Index] = (TObject *)IBInternalQuery->FieldByName("TAB_KEY")->AsInteger;
         IBInternalQuery->Next();
      }
	}
	catch(Exception &Err)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, Err.Message);
		throw;
	}
}

Currency __fastcall TDBWebUtil::ProcessPrePay(_di_IXMLACCOUNTType &OrderAccount)
{
   Currency TotalPayment = 0;
   if(OrderAccount != NULL)
   {
      _di_IXMLPAYMENTSType Payments =  OrderAccount->Get_PAYMENTS();
      _di_IXMLPAYMENTTypeList Paymentlist = Payments->Get_PAYMENT();
      for(int j = 0; j < Paymentlist->GetCount(); j++ )
      {
         _di_IXMLPAYMENTType Payment = Paymentlist->Get_Item(j);
         Currency PayValue = StrToCurrDef(Payment->Get_VALUE(),0);
         TotalPayment += PayValue;
      }
   }
   return TotalPayment;
}

Currency TDBWebUtil::GetWebOrderPrePaid(Database::TDBTransaction &DBTransaction, int WebOrderKey)
{
	Currency RetVal = 0;
   if(WebOrderKey == 0 ) return RetVal;
	try
	{
		Database::TcpIBSQL IBInternalQuery(new TIBSQL(NULL));
		DBTransaction.RegisterQuery(IBInternalQuery);

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Text = "SELECT PREPAID FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY";
		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = WebOrderKey;
		IBInternalQuery->ExecQuery();

		if (IBInternalQuery->RecordCount)
		{
			RetVal = IBInternalQuery->FieldByName("PREPAID")->AsCurrency;
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
	return RetVal;
}

TDateTime TDBWebUtil::getWebOrderDeliveryTime(Database::TDBTransaction &DBTransaction,int WebKey, TDateTime delivery_time)
{
	try
	{
        delivery_time = 0;
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "SELECT * FROM WEBORDERS WHERE WEBORDER_KEY = :WEBORDER_KEY ";

		IBInternalQuery->ParamByName("WEBORDER_KEY")->AsInteger = WebKey;
		IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
		    delivery_time = IBInternalQuery->FieldByName("EXPECTED_DATE")->AsDateTime;
		}
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
    return delivery_time;
}

void TDBWebUtil::getWebOrderContactDetails(Database::TDBTransaction &DBTransaction, UnicodeString Email, TStringList *Strings)
{
	try
	{
		TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

		IBInternalQuery->Close();
		IBInternalQuery->SQL->Clear();
		IBInternalQuery->SQL->Text =
			 "SELECT * FROM CONTACTS a WHERE a.EMAIL = :EMAIL "
             " ORDER BY EMAIL";

		IBInternalQuery->ParamByName("EMAIL")->AsString = Email;
		IBInternalQuery->ExecQuery();

        for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
		{
            int key = IBInternalQuery->FieldByName("CONTACTS_KEY")->AsInteger;
		    Strings->AddObject(IBInternalQuery->FieldByName("EMAIL")->AsString, (TObject *)key);
		}
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
		throw;
	}
}

TMMContactInfo TDBWebUtil::autoAddWebMembers(Database::TDBTransaction &DBTransaction, TStringList *WebOrderDetails, TStringList *WebMemberAddress, int web_key)
{
    try
    {
        TMMContactInfo webMemberInfo;
        eMemberSource MemberSource;
        AnsiString locationAddress = "";

        std::auto_ptr<TStringList>WebEmailDetials(new TStringList);
        TDBWebUtil::getWebOrderExtraData(DBTransaction, web_key, "EMAIL", WebEmailDetials.get());
        std::auto_ptr<TStringList>WebMemberDetials(new TStringList);

        int web_contactkey = 0;
        webMemberInfo.Name = WebOrderDetails->Strings[6];
        webMemberInfo.EMail = WebEmailDetials->Strings[0];
        for(int i = 0; i < 5; i++)
        {
           locationAddress += WebMemberAddress->Strings[i] + " ";
        }
        webMemberInfo.LocationAddress = locationAddress;
        webMemberInfo.Phone = WebMemberAddress->Strings[7];
        webMemberInfo.Points.PointsRules << eprNeverEarnsPoints;
        webMemberInfo.MemberType = 1;
        webMemberInfo.SiteID = TGlobalSettings::Instance().SiteID;
        TDBWebUtil::getWebOrderContactDetails(DBTransaction, WebEmailDetials->Strings[0], WebMemberDetials.get());
        if(WebMemberDetials->Count == 0)
        {
          if(TGlobalSettings::Instance().AutoAddWebMembers)
          {
               TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->SetContactDetails(DBTransaction, webMemberInfo.ContactKey, webMemberInfo);
               TDBWebUtil::getWebOrderContactDetails(DBTransaction, WebEmailDetials->Strings[0], WebMemberDetials.get());
               web_contactkey = (int)WebMemberDetials->Objects[0];
           }
        }
        else
        {
           TDBWebUtil::getWebOrderContactDetails(DBTransaction, WebEmailDetials->Strings[0], WebMemberDetials.get());
           if(WebMemberDetials->Count > 0)
           {
             web_contactkey = (int)WebMemberDetials->Objects[0];
           }
        }
        TDeviceRealTerminal::Instance().ManagerMembership->MembershipSystem->GetContactDetails(DBTransaction, web_contactkey, webMemberInfo);
        webMemberInfo.LocationAddress = locationAddress;
        return webMemberInfo;
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}
}

TMMContactInfo TDBWebUtil::checkAppliedChit(TChitNumber WebOrderChitNumber, TMMContactInfo webMemberInfo, TStringList *WebMemberAddress)
{
    try
    {
       eMemberSource MemberSource;
       AnsiString locationAddress = "";
       for(int i = 0; i < 5; i++)
       {
          locationAddress += WebMemberAddress->Strings[i] + " ";
       }
       if(WebOrderChitNumber.Valid())
       {
          if(WebOrderChitNumber.IsCaptureCustomerDetails && WebOrderChitNumber.PromptForPickUpDeliveryTime)
          {
               if(locationAddress != webMemberInfo.LocationAddress)
               {
                   webMemberInfo.LocationAddress = locationAddress;
               }
          }
          else
          {
             webMemberInfo.Clear();
          }
       }
       return webMemberInfo;
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}
}

void TDBWebUtil::getWebOrderData(Database::TDBTransaction &DBTransaction, TStringList *webDeliveryDetails, TStringList *webPaymentDetials, TStringList *webComments,  int web_key)
{
    try
    {
        getWebOrderExtraData(DBTransaction, web_key, "DELIVERY", webDeliveryDetails);
    	getWebOrderExtraData(DBTransaction, web_key, "COMMENTS", webComments);
        getWebOrderExtraData(DBTransaction, web_key, "PAYMENT_INFO", webPaymentDetials);
    }
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}
}

TChitNumber TDBWebUtil::InitializeChit(int web_key, TChitNumber &WebOrderChitNumber)//, TChitNumberController &ChitNumberController)
{
    try
    {
        ChitResult result;
        TChitNumber ChitNumber;
        Database::TDBTransaction transaction(TDeviceRealTerminal::Instance().DBControl);
        transaction.StartTransaction();
        AnsiString orderType ="";
        std::auto_ptr<TStringList>webOrderType(new TStringList);
        TDBWebUtil::getWebOrderExtraData(transaction, web_key, "ORDERTYPE", webOrderType.get());

        if(webOrderType->Count > 0)
        {
           orderType = webOrderType->Strings[0];
        }
        TManagerChitNumber::Instance().LoadAll(transaction);

        for (TManagerChitNumber::Instance().First(false); !TManagerChitNumber::Instance().Eof(); TManagerChitNumber::Instance().Next(false))
        {
             TChitNumber chit_number = TManagerChitNumber::Instance().ChitNumber();
             if(orderType == "Pickup" && chit_number.OnlinePickupOrder)
             {
                 AssignChitNumber(transaction, chit_number, WebOrderChitNumber);
                 break;
             }
             if(orderType == "Delivery" && chit_number.OnlineDeliveryOrder)
             {
                 AssignChitNumber(transaction, chit_number, WebOrderChitNumber);
                 break;
             }
        }
        transaction.Commit();
      return ChitNumber;
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}
}

void TDBWebUtil::AssignChitNumber(Database::TDBTransaction &DBTransaction, TChitNumber &chit_number, TChitNumber &WebOrderChitNumber)//, TChitNumberController &ChitNumberController)
{
   TChitNumberController ChitNumberController(DBTransaction);
   ChitResult Result = ChitNumberController.GetChitNumber(false, chit_number);
   if (Result == ChitOk)
   {
         WebOrderChitNumber = chit_number;
   }
}
//---------------------------------------------------------------------------------------------------------
TMMContactInfo TDBWebUtil::LoadMemberDetails(Database::TDBTransaction &DBTransaction, int webKey)
{
    try
    {
        if(webKey != 0)
        {
            UnicodeString Address = "";
            TMMContactInfo memberInfo;
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            AnsiString selectQuery = "SELECT a.WEBORDER_KEY, a.CATEGORY, a.ELEMENT, a.DATA "
                                     "FROM WEBDATA a "
                                     "WHERE a.WEBORDER_KEY = :WEBORDER_KEY and a.ELEMENT ";
            IBInternalQuery->SQL->Text = selectQuery +
                                            "in ('Recipient','StreetNo','StreetName','City','Country')";

            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();

            for(;!IBInternalQuery->Eof;IBInternalQuery->Next())
            {
                 Address += IBInternalQuery->FieldByName("DATA")->AsString;
            }

            memberInfo.MailingAddress = Address;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text = selectQuery +  " = 'OrderType'";
            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();
            AnsiString OrderType = IBInternalQuery->FieldByName("DATA")->AsString;

            if(OrderType == "Pick up")
                memberInfo.MailingAddress = "";

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text = selectQuery +  " = 'Email'";
            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();
            memberInfo.EMail = IBInternalQuery->FieldByName("DATA")->AsString;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text = "SELECT a.NAME FROM WEBORDERS a where a.WEBORDER_KEY = :WEBORDER_KEY ";
            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();
            memberInfo.Name = IBInternalQuery->FieldByName("NAME")->AsString;

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text = selectQuery +  " = 'Phone'";
            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();
            memberInfo.Phone = IBInternalQuery->FieldByName("DATA")->AsString;

            return memberInfo;
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}

}//-----------------------------------------------------------------------------------------------
UnicodeString TDBWebUtil::LoadPaymentStatus(Database::TDBTransaction &DBTransaction, int webKey)
{
    try
    {
        if(webKey != 0)
        {
            Currency prepaidAmount = 0.0;
            UnicodeString paidStatus = "";
            TMMContactInfo memberInfo;
            TIBSQL *IBInternalQuery = DBTransaction.Query(DBTransaction.AddQuery());

            IBInternalQuery->Close();
            IBInternalQuery->SQL->Clear();
            IBInternalQuery->SQL->Text = "SELECT a.PREPAID "
                                         "FROM WEBORDERS a "
                                         "WHERE a.WEBORDER_KEY = :WEBORDER_KEY ";

            IBInternalQuery->ParamByName("WEBORDER_KEY")->AsString = webKey;
            IBInternalQuery->ExecQuery();

            if(IBInternalQuery->RecordCount)
            {
                prepaidAmount = IBInternalQuery->FieldByName("PREPAID")->AsCurrency;
            }

            if(prepaidAmount > 0)
            {
                paidStatus = "PAID";
            }
            else
            {
                paidStatus = "TO BE PAID FOR";
            }
            return paidStatus;
        }
    }
    catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
        throw;
	}

}





