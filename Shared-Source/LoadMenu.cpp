//---------------------------------------------------------------------------


#pragma hdrstop

#include "LoadMenu.h"
#include <system.hpp>
#include "tinyxml.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

const AnsiString VERSION = "6.00";

TLoadMenu::TLoadMenu( AnsiString inFileName )
{
	FFileName = inFileName;

	_xmlMenuDoc = createXMLMenuDoc( FFileName.c_str() );

	if( _xmlMenuDoc != NULL )
	{
		loadMainElements();

		if( MenuVersion.Trim() != VERSION )
		{
			AnsiString msg = "Invalid menu version: " + MenuVersion.Trim() + ". " +
			"Make sure the xml file contains the menu version: " + VERSION;

			throw ExceptionLoadMenu( msg, lmInvalidMenuVersion );
		}
	}
}
//---------------------------------------------------------------------------

TLoadMenu::~TLoadMenu()
{
	delete _xmlMenuDoc;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::
// Public
//::::::::::::::::::::::::::::::::::::

__int32 TLoadMenu::SizeCount()
{
	return childCount( _sizesElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::SizeAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, WideString& outKitchenName,
WideString& outHandheldName, WideString& outReceiptName, bool& outWeighed,
__int32& outSizeID, __int32& outPalmID )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _sizesElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey          = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outDescription  = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outKitchenName  = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outHandheldName = convertUTF8CharToWideString( itPtr->Attribute( "handheldName" ) );
					outReceiptName  = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outWeighed      = AnsiString( itPtr->Attribute( "weighed" ) ).UpperCase() == "TRUE";
					outSizeID       = StrToInt( AnsiString( itPtr->Attribute( "sizeID" ) ) );
					outPalmID       = StrToInt( AnsiString( itPtr->Attribute( "palmID" ) ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::CategoryGroupCount()
{
	return childCount( _categoryGroupsElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::CategoryGroupAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, bool& outGetDeleted )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _categoryGroupsElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey          = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outDescription  = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outGetDeleted   = AnsiString( itPtr->Attribute( "deleted" ) ).UpperCase() == "TRUE";

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::CategoryCount( __int32 inCategoryGroupID )
{
	TiXmlElement *categoryGroup = ( TiXmlElement* )inCategoryGroupID;

	return childCount( categoryGroup );
}
//---------------------------------------------------------------------------

bool TLoadMenu::CategoryAtIndex( __int32 inIndex, __int32 inCategoryGroupHandle, __int32& outKey, WideString& outDescription, WideString& outGLCode, bool& outGetDeleted )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *_categoryGroupElem = ( TiXmlElement* )inCategoryGroupHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = _categoryGroupElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey          = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outDescription  = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outGetDeleted   = AnsiString( itPtr->Attribute( "deleted" ) ).UpperCase() == "TRUE";
					outGLCode       = convertUTF8CharToWideString( itPtr->Attribute( "glcode" ) );
					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::AllTaxProfilesCount()
{
	return childCount( _allTaxProfilesElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::TaxProfileAtIndex( __int32 inIndex,  __int32& outKey, WideString& outName, Currency& outRate,
__int32& outType, __int32& outPriority )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _allTaxProfilesElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey      = StrToInt(   AnsiString( itPtr->Attribute( "key" ) ) );
					outName     = convertUTF8CharToWideString( itPtr->Attribute( "name" ) );
					outRate     = StrToCurr(  AnsiString( itPtr->Attribute( "rate" ) ) );
					outType     = StrToInt(   AnsiString( itPtr->Attribute( "type" ) ) );
					outPriority = StrToInt(   AnsiString( itPtr->Attribute( "priority" ) ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ServingCourseCount()
{
	return childCount( _servingCoursesElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ServingCourseAtIndex( __int32 inIndex, __int32& outKey, WideString& outLongDescription, WideString& outKitchenName,
bool& outEnabled, bool& outDeleted, bool& outSelectable, TColor& outColor,__int32& displayOrder )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _servingCoursesElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey             = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outLongDescription = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outKitchenName     = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outEnabled         = AnsiString( itPtr->Attribute( "enabled" ) ).UpperCase() == "TRUE";
					outDeleted         = AnsiString( itPtr->Attribute( "deleted" ) ).UpperCase() == "TRUE";
					outSelectable      = AnsiString( itPtr->Attribute( "selectable" ) ).UpperCase() == "TRUE";
					outColor           = StrToInt( AnsiString( itPtr->Attribute( "color" ) ) );
					displayOrder       = StrToInt( AnsiString( itPtr->Attribute( "displayOrder" ) ) );
					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ThirdPartyCodeCount()
{
	return childCount( _TPCsElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ThirdPartyCodeAtIndex( __int32 inIndex, __int32& outKey, WideString& outCode, WideString& outDescription, bool& outVisible,
__int32& outCodeType )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _TPCsElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey         = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outCode        = convertUTF8CharToWideString( itPtr->Attribute( "code" ) );
					outDescription = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outVisible     = StrToInt(itPtr->Attribute( "visible" )) == 1 ? true : false;
					outCodeType    = StrToInt( AnsiString( itPtr->Attribute( "codeType" ) ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::CourseCount()
{
	return childCount( _coursesElem );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::CourseAtIndex( __int32 inIndex, __int32& outKey, WideString& outDescription, WideString& outKitchenName,
WideString& outHandheldName, WideString& outReceiptName, __int32& outLocation, bool& outEnabled,
__int32& outServingCourseKey, WideString& outServiceCourse, bool& outNoDefaultServingCourse )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			__int32       it    = 0;
			TiXmlElement *itPtr = _coursesElem->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey                    = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outDescription            = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outKitchenName            = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outHandheldName           = convertUTF8CharToWideString( itPtr->Attribute( "handheldName" ) );
					outReceiptName            = convertUTF8CharToWideString( itPtr->Attribute( "receiptName" ) );
					outLocation               = StrToInt( AnsiString( itPtr->Attribute( "location" ) ) );
					outEnabled                = AnsiString( itPtr->Attribute( "enabled" ) ).UpperCase() == "TRUE";
					outServingCourseKey       = StrToInt( AnsiString( itPtr->Attribute( "servingCourseKey" ) ) );
					outServiceCourse          = convertUTF8CharToWideString( itPtr->Attribute( "servingCourseName" ) );
					outNoDefaultServingCourse = AnsiString( itPtr->Attribute( "noDefaultServingCourse" ) ).UpperCase() == "TRUE";

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemCount( __int32 inCourseHandle )
{
	TiXmlElement *courseElem = ( TiXmlElement* )inCourseHandle;

	return childCount( loadElement( "Items", courseElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemAtIndex( __int32 inIndex, __int32 inCourseHandle, __int32& outKey, WideString& outLongDescription,
WideString& outKitchenName, WideString& outHandheldName, WideString& outReceiptName,
TColor& outColor, bool& outDisplaySizes, bool& outEnabled, bool& outItemOnlySide,
bool& outPrintUnderlined,  bool& outPrintBold, TColor& outPrintColor, __int32& outPrintFont,
bool& outPrintDoubleWidth, bool& outPrintDoubleHeight )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::
   //AnsiString msg;
	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *courseElem = ( TiXmlElement* )inCourseHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "Items", courseElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey               = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outLongDescription   = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
                    
					outKitchenName       = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outHandheldName      = convertUTF8CharToWideString( itPtr->Attribute( "handheldName" ) );
					outReceiptName       = convertUTF8CharToWideString( itPtr->Attribute( "receiptName" ) );
					outColor             = StrToInt(AnsiString( itPtr->Attribute( "color" ) ) ); //AnsiString( itPtr->Attribute( "color" ) ) );
					outDisplaySizes      = AnsiString( itPtr->Attribute( "displaySizes" ) ).UpperCase() == "TRUE";
					outEnabled           = AnsiString( itPtr->Attribute( "enabled" ) ).UpperCase() == "TRUE";
					outItemOnlySide      = AnsiString( itPtr->Attribute( "itemOnlySide" ) ).UpperCase() == "TRUE";
					outPrintUnderlined   = AnsiString( itPtr->Attribute( "printUnderlined" ) ).UpperCase() == "TRUE";
					outPrintBold         = AnsiString( itPtr->Attribute( "printBold" ) ).UpperCase() == "TRUE";
					outPrintColor        = StrToInt( AnsiString( itPtr->Attribute( "printColor" ) ) );
					outPrintFont         = StrToInt( AnsiString( itPtr->Attribute( "printFont" ) ) );
					outPrintDoubleWidth  = AnsiString( itPtr->Attribute( "printDoubleWidth" ) ).UpperCase() == "TRUE";
					outPrintDoubleHeight = AnsiString( itPtr->Attribute( "printDoubleHeight" ) ).UpperCase() == "TRUE";

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}

	}
	catch(... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::OptionCount( __int32 inCourseHandle )
{
	TiXmlElement *courseElem = ( TiXmlElement* )inCourseHandle;

	return childCount( loadElement( "Options", courseElem ) );
}
//---------------------------------------------------------------------------

void TLoadMenu::DisassembleOptionMask( const __int32 inFlags,
int& outGroupNumber,
int& outMaxSelect,
int& outPlusOption,
int& outMinusOption,
int& outOptionsGroupSkip )
const
{
	outGroupNumber      = inFlags  & 0x000f;
	//   outMaxSelect        = (inFlags & 0x03e0) >> 5;
	//   outPlusOption       = (inFlags & 0x0400) > 0;
	//   outMinusOption      = (inFlags & 0x0800) > 0;
	//   outOptionsGroupSkip = (inFlags & 0x1000) > 0;

	outMaxSelect        = (inFlags & 0x10);
	outPlusOption       = (inFlags & 0x20);
	outMinusOption      = (inFlags & 0x40);
	outOptionsGroupSkip = (inFlags & 0x80);
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::OptionAtIndex( __int32 inIndex, __int32 inCourseHandle, __int32& outKey, WideString& outLongDescription,
WideString& outKitchenName, WideString& outHandheldName, WideString& outReceiptName,
__int32& outCondimentsMask, __int32& outFlags, bool& outEnabled, bool& outPrintUnderlined,
bool& outPrintBold, __int32& outPrintColor, __int32& outPrintFont,
bool& outPrintDoubleWidth, bool& outPrintDoubleHeight )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *courseElem = ( TiXmlElement* )inCourseHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "Options", courseElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey               = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outLongDescription   = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outKitchenName       = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outHandheldName      = convertUTF8CharToWideString( itPtr->Attribute( "handheldName" ) );
					outReceiptName       = convertUTF8CharToWideString( itPtr->Attribute( "receiptName" ) );
					outCondimentsMask    = StrToInt( AnsiString( itPtr->Attribute( "condimentsMask" ) ) );
					outFlags             = StrToInt(AnsiString( itPtr->Attribute( "flags" ) ) );
					outEnabled           = AnsiString( itPtr->Attribute( "enabled" ) ).UpperCase() == "TRUE";
					outPrintUnderlined   = AnsiString( itPtr->Attribute( "printUnderlined" ) ).UpperCase() == "TRUE";
					outPrintBold         = AnsiString( itPtr->Attribute( "printBold" ) ).UpperCase() == "TRUE";
					outPrintColor        = StrToInt( AnsiString( itPtr->Attribute( "printColor" ) ) );
					outPrintFont         = StrToInt( AnsiString( itPtr->Attribute( "printFont" ) ) );
					outPrintDoubleWidth  = AnsiString( itPtr->Attribute( "printDoubleWidth" ) ).UpperCase() == "TRUE";
					outPrintDoubleHeight = AnsiString( itPtr->Attribute( "printDoubleHeight" ) ).UpperCase() == "TRUE";

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ForcedSideCount( __int32 inItemHandle )
{
	TiXmlElement *itemElem = ( TiXmlElement* )inItemHandle;

	return childCount( loadElement( "ForcedSides", itemElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ForcedSideAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey, __int32& outItemKey,
WideString& outLongDescription,
__int32& outGroupNumber, __int32& outMaxSelect, bool& outSideGroupSkip )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemElem = ( TiXmlElement* )inItemHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "ForcedSides", itemElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey             = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outItemKey         = StrToInt( AnsiString( itPtr->Attribute( "itemFKey" ) ) );
					outLongDescription = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outGroupNumber     = StrToInt( AnsiString( itPtr->Attribute( "groupNumber" ) ) );
					outMaxSelect       = StrToInt( AnsiString( itPtr->Attribute( "maxSelect" ) ) );
					outSideGroupSkip   = AnsiString( itPtr->Attribute( "sideGroupSkip" ) ).UpperCase() == "TRUE";

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ForcedOptionCount( __int32 inItemID )
{
	TiXmlElement *itemElem = ( TiXmlElement* )inItemID;

	return childCount( loadElement( "ForcedOptions", itemElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ForcedOptionAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey,
__int32& outOptionFKey, WideString& outDescription,
__int32& outGroupNumber )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemElem = ( TiXmlElement* )inItemHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "ForcedOptions", itemElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey         = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outOptionFKey  = StrToInt( AnsiString( itPtr->Attribute( "optionFKey" ) ) );
					outDescription = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outGroupNumber = StrToInt( AnsiString( itPtr->Attribute( "groupNumber" ) ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemSizeCount( __int32 inItemHandle )
{
	TiXmlElement *itemElem = ( TiXmlElement* )inItemHandle;

	return childCount( loadElement( "ItemSizes", itemElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemSizeAtIndex( __int32 inIndex, __int32 inItemHandle, __int32& outKey, __int32& outSizeFKey, WideString& outLongDescription,
									WideString& outSizeKitchenName, bool& outFree, Currency& outPrice, Currency& outMaxRetailPrice, Currency& outSpecialPrice,
									double& outGST, Currency& outCost, double& outCostGST, double& outPointsPercent, __int32& outMemberPurchaseCount,
									double& outMemberPurchaseDiscount, __int32& outLocationPurchaseCount, double& outLocationPurchaseDiscount, bool& outNoRecipe,
									bool& outIsTaxExempt, bool& outHasServiceCharge, __int32& outSetMenuMask, bool& outAvailableAsStandard, WideString& outBarcode,
									bool& outEnabled, __int32& outCategoryKey, WideString& outCategory, __int32& outThirdPartyCodeKey, double& outTareWeight,
									__int32& outPLU, double &outAvailableQuantity, double &outDefaultQuantity, double &outWarningQuantity,
									bool &outDisableWhenCountReachesZero, bool &outCanBePaidForUsingPoints, int &outDefaultPatronCount,
                                    Currency& outPriceForPoints)
{
	__int32 result = 0;

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemElem = ( TiXmlElement* )inItemHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "ItemSizes", itemElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey                      = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outSizeFKey                 = StrToInt( AnsiString( itPtr->Attribute( "sizeFKey" ) ) );
					outLongDescription          = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );
					outSizeKitchenName          = convertUTF8CharToWideString( itPtr->Attribute( "kitchenName" ) );
					outFree                     = AnsiString( itPtr->Attribute( "free" ) ).UpperCase() == "TRUE";
					outPrice                    = StrToCurr(  AnsiString( itPtr->Attribute( "price" ) ) );
					outMaxRetailPrice = itPtr->Attribute( "maxRetailPrice" ) == NULL ? 0.0 : StrToCurr(AnsiString(itPtr->Attribute( "maxRetailPrice" )));
					outSpecialPrice             = StrToCurr(  AnsiString( itPtr->Attribute( "specialPrice" ) ) );
					outGST                      = StrToFloat( AnsiString( itPtr->Attribute( "gst" ) ) );
					outCost                     = StrToCurr(  AnsiString( itPtr->Attribute( "cost" ) ) );
					outCostGST                  = StrToCurr(  AnsiString( itPtr->Attribute( "costGST" ) ) );
					outPointsPercent            = StrToFloat( AnsiString( itPtr->Attribute( "pointsPercent" ) ) );
					outMemberPurchaseCount      = StrToFloat( AnsiString( itPtr->Attribute( "memberPurchaseCount" ) ) );
					outMemberPurchaseDiscount   = StrToFloat( AnsiString( itPtr->Attribute( "memberPurchaseDiscount" ) ) );
					outLocationPurchaseCount    = StrToFloat( AnsiString( itPtr->Attribute( "locationPurchaseCount" ) ) );
					outLocationPurchaseDiscount = StrToFloat( AnsiString( itPtr->Attribute( "locationPurchaseDiscount" ) ) );
					outNoRecipe                 = AnsiString( itPtr->Attribute( "noRecipe" ) ).UpperCase() == "TRUE";
					outSetMenuMask              = StrToInt( AnsiString( itPtr->Attribute( "setMenuMask" ) ) );
					outAvailableAsStandard      = AnsiString( itPtr->Attribute( "availableAsStandard" ) ).UpperCase() == "TRUE";
					outBarcode                  = convertUTF8CharToWideString( itPtr->Attribute( "barcode" ) );
					outEnabled                  = AnsiString( itPtr->Attribute( "enabled" ) ).UpperCase() == "TRUE";
					outCategoryKey              = StrToInt(   itPtr->Attribute( "categoryKey" ) );
					outCategory                 = convertUTF8CharToWideString( itPtr->Attribute( "category" ) );
					outThirdPartyCodeKey        = StrToInt( AnsiString( itPtr->Attribute( "thirdPartyCodeKey" ) ) );
					outTareWeight               = StrToFloat( AnsiString( itPtr->Attribute( "tareWeight" ) ) );
					outPLU                      = StrToInt( AnsiString( itPtr->Attribute( "PLU" ) ) );
					outAvailableQuantity = StrToFloat(itPtr->Attribute("availableQuantity"));
					outDefaultQuantity = StrToFloat(itPtr->Attribute("defaultQuantity"));
					outWarningQuantity = StrToFloat(itPtr->Attribute("warningQuantity"));
					outDisableWhenCountReachesZero = AnsiString(itPtr->Attribute("disableWhenCountReachesZero")).UpperCase() == "TRUE";
					outCanBePaidForUsingPoints = AnsiString(itPtr->Attribute("canBePaidForUsingPoints")).UpperCase() == "TRUE";
					outDefaultPatronCount = StrToIntDef(AnsiString(itPtr->Attribute("defaultPatronCount")).Trim(), 0);
                    outPriceForPoints = itPtr->Attribute( "priceforpoints" ) == NULL ? 0.0 : StrToCurr(AnsiString(itPtr->Attribute( "priceforpoints" ))); // changes for read null value from xml

					result = ( __int32 )itPtr;
					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
////-----------------------------------------------------------

 __int32 TLoadMenu::PriceLevelCount( __int32 inItemSizeHandle )
{
	TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

      __int32 count=    childCount( loadElement( "PriceLevels", itemSizeElem ) );

      if (count !=NULL)
        return count;
      else
        return 0;

  //	return childCount( loadElement( "PriceLevels", itemSizeElem ) );
}
///--------------------------------------------------
__int32 TLoadMenu::ItemSizePriceLevelAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey,Currency& Price,AnsiString& Name )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "PriceLevels", itemSizeElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey         = StrToInt( AnsiString( itPtr->Attribute( "PriceLevelKey" ) ) );
				   //	Price = AnsiString( itPtr->Attribute( "Price" ) );
                   	Price = StrToCurr( itPtr->Attribute( "Price" )) ;

                    Name = AnsiString( itPtr->Attribute( "Name" ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::BCategoryCount( __int32 inItemSizeHandle )
{
	TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

	return childCount( loadElement( "BreakdownCategories", itemSizeElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::BCategoryAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey, WideString& outDescription )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "BreakdownCategories", itemSizeElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey         = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outDescription = convertUTF8CharToWideString( itPtr->Attribute( "description" ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemSizeReceipeCount( __int32 inItemSizeHandle )
{
	TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

	return childCount( loadElement( "Receipes", itemSizeElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemSizeReceipeAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey, WideString& outStockCode,
double& outQuantity, WideString& outLocation )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "Receipes", itemSizeElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey           = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );
					outStockCode     = convertUTF8CharToWideString( itPtr->Attribute( "stockCode" ) );
					outQuantity      = StrToFloat( itPtr->Attribute( "quantity" ) );
					outLocation      = convertUTF8CharToWideString( itPtr->Attribute( "location" ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::TaxProfileCount( __int32 inItemSizeHandle )
{
	TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

	return childCount( loadElement( "TaxProfiles", itemSizeElem ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::ItemSizeTaxProfileAtIndex( __int32 inIndex, __int32 inItemSizeHandle, __int32& outKey )
{
	__int32 result = 0;

	//:::::::::::::::::::::::::::::::::::::

	try
	{
		if( inIndex >= 0 )
		{
			TiXmlElement *itemSizeElem = ( TiXmlElement* )inItemSizeHandle;

			__int32       it    = 0;
			TiXmlElement *itPtr = loadElement( "TaxProfiles", itemSizeElem )->FirstChildElement();

			while( ( itPtr != NULL ) && ( it <= inIndex ) )
			{
				if( it == inIndex )
				{
					outKey = StrToInt( AnsiString( itPtr->Attribute( "key" ) ) );

					result = ( __int32 )itPtr;

					break;
				}

				itPtr = itPtr->NextSiblingElement();
				it++;
			}
		}
	}
	catch( ... )
	{
	}

	//:::::::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::
// Protected
//::::::::::::::::::::::::::::::::::::

AnsiString TLoadMenu::GetMenuVersion()
{
	return AnsiString( _rootElem->Attribute( "version" ) );
}
//---------------------------------------------------------------------------

AnsiString TLoadMenu::GetMenuName()
{
	return AnsiString( _rootElem->Attribute( "name" ) );
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::GetMenuKey()
{
	return StrToInt( AnsiString( _rootElem->Attribute( "key" ) ) );
}
//---------------------------------------------------------------------------

Menu::TMenuType TLoadMenu::GetMenuType()
{
	return ( Menu::TMenuType )( StrToInt( AnsiString( _rootElem->Attribute( "type" ) ) ) );
}
//---------------------------------------------------------------------------

bool TLoadMenu::GetAvailableOnPalm()
{
	return AnsiString( _rootElem->Attribute( "availableOnPalm" ) ).UpperCase() == "TRUE";
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::GetLastGeneratedID() const
{
	return StrToInt( _rootElem->Attribute( "nextAvailableKey" ) );
}

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::
// Private
//::::::::::::::::::::::::::::::::::::

TiXmlDocument* TLoadMenu::createXMLMenuDoc( AnsiString inFileName )
{
	TiXmlDocument* result = new TiXmlDocument();

	//::::::::::::::::::::::::::::::

	try
	{
		result->LoadFile( inFileName.c_str() );
	}
	catch( ... )
	{
		delete result;
		result = NULL;
	}
	//::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
void TLoadMenu::loadMainElements()
{
	try
	{
		_rootElem           = loadRootElement(           _xmlMenuDoc );
		_sizesElem          = loadSizesElement(          _rootElem );
		_categoryGroupsElem = loadCategoryGroupsElement( _rootElem );
		_allTaxProfilesElem = loadAllTaxProfilesElement( _rootElem );
		_servingCoursesElem = loadServiceCoursesElement( _rootElem );
		_coursesElem        = loadCoursesElement(        _rootElem );
		_TPCsElem           = loadTPCsElement(           _rootElem );
	}
	catch( Exception &e )
	{
		throw ExceptionLoadMenu( e.Message, lmElementNotFound );
	}
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadRootElement( TiXmlDocument* inXMLMenuDoc )
{
	return inXMLMenuDoc->RootElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadSizesElement( TiXmlElement* inRootElement )
{
	return loadElement( "Sizes", inRootElement );

	//return inRootElement->FirstChildElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadCategoryGroupsElement( TiXmlElement* inRootElement )
{
	return loadElement( "CategoryGroups", inRootElement );

	//return inRootElement->FirstChildElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadAllTaxProfilesElement( TiXmlElement* inRootElement )
{
	return loadElement( "AllTaxProfiles", inRootElement );

	//return inRootElement->FirstChildElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadServiceCoursesElement( TiXmlElement* inRootElement )
{
	return loadElement( "ServiceCourses", inRootElement );

	//return inRootElement->FirstChildElement()->NextSiblingElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadCoursesElement( TiXmlElement* inRootElement )
{
	return loadElement( "Courses", inRootElement );

	//return inRootElement->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadTPCsElement( TiXmlElement* inRootElement )
{
	return loadElement( "ThirdPartyCodes", inRootElement );

	//return inRootElement->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------
TiXmlElement* TLoadMenu::loadElement( AnsiString inElemName, TiXmlElement* inParentElement )
{
	try
	{
		TiXmlElement *element = inParentElement->FirstChildElement();

		while( element != NULL )
		{
			if( AnsiString( element->Value() ) == inElemName )
			{
				return element;
			}
			element = element->NextSiblingElement();
		}
	}
	catch( ... )
	{
		throw Exception( "Invalid menu version: " + VERSION + " Element not found: " + "root" );
	}

	//throw Exception( "Invalid menu version: " + VERSION + ". Element not found: " + inElemName );

    return 0;
	//return inRootElement->FirstChildElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
}
//---------------------------------------------------------------------------

__int32 TLoadMenu::childCount( TiXmlElement* inElem )
{
	__int32 result = 0;

	//::::::::::::::::::::::::::::::::

	try
	{
		if(inElem)
		{
			TiXmlElement *child = inElem->FirstChildElement();

			while( child != NULL )
			{
				result++;
				child = child->NextSiblingElement();
			}
		}
	}
	catch( ... )
	{
	}

	//::::::::::::::::::::::::::::::::

	return result;
}
//---------------------------------------------------------------------------
WideString TLoadMenu::convertUTF8CharToWideString( const char* pchars )
{
	WideString result = "";
#ifdef MenuMate
    result = UnicodeString( pchars );
#endif

#ifdef Office
	result = Trim(WideString( pchars ));
#endif
	return result;
}
//---------------------------------------------------------------------------

WideString TLoadMenu::UTF8ToWideString(AnsiString inString)
{
	int BufferSize   = MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest    = new wchar_t[BufferSize];
	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest,BufferSize);
	delete [] dest;
	return RetVal;
}

