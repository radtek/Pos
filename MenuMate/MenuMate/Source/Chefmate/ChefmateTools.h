//---------------------------------------------------------------------------

#ifndef ChefmateToolsH
#define ChefmateToolsH

//---------------------------------------------------------------------------

#include <vcl.h>
#include "map.h"
#include "MM_DBCore.h"
#include "ip_utilities.h"
#include "SystemEvents.h"
#include "DeviceRealTerminal.h"

//---------------------------------------------------------------------------

typedef std::set<__int32>           TSetOfSCourse;
typedef std::set<__int32>::iterator TSetOfSCourseIterator;

typedef std::set<__int32>           TSetOfCourse;
typedef std::set<__int32>::iterator TSetOfCourseIterator;

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//   TChefmateServingCourse
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct TChefmateServingCourse
{
	__int32       key;
	UnicodeString Name;
	UnicodeString KitchenName;
	__int32 	  DisplayOrder;
};

//.............................................

class TChefmateServingCourseMap
{
protected:
	TIBDatabase *ibDatabase;

	// This map is not used since the ServingCourseKey field coming from
	// the POS is always 0. _c_scMap is used instead
	map<__int32, TChefmateServingCourse> _scMap;

	void _readServingCourseWithKey(
				__int32                 inSCKey,
				TChefmateServingCourse& outSC );
	void _readServingCourseFromDBWithKey(
				__int32                   inSCKey,
				Database::TDBTransaction* inDBTransaction,
				TChefmateServingCourse&   outSC );
	void _readServingCourseWithName(
				UnicodeString           inSCName,
				TChefmateServingCourse& outSC );
	void _readServingCourseFromDBWithName(
				UnicodeString             inSCName,
				Database::TDBTransaction* inDBTransaction,
				TChefmateServingCourse&   outSC );
	//......................................................

	map<__int32, TChefmateServingCourse> _c_scMap;

	void _readServingCourseWithCourseKey(
				__int32                 inCKey,
				TChefmateServingCourse& outSC );
	void _readServingCourseFromDBWithCourseKey(
				__int32                   inCKey,
				Database::TDBTransaction* inDBTransaction,
				TChefmateServingCourse&   outSC );

    void OnPOSMenuChanged(TSystemEvents *Sender);

public:
/*	static TChefmateServingCourseMap& Instance()
	{
		static TChefmateServingCourseMap singleton;
		return singleton;
	}*/
	TChefmateServingCourseMap();
	~TChefmateServingCourseMap();

	void SetDatabase( TIBDatabase *inIBDatabase );

	// This function is not used since the ServingCourseKey field coming from
	// the POS is always 0. addServingCourseWithCourseKey is used instead
	TChefmateServingCourse ServingCourseWithKey( __int32 inSCKey );
	TChefmateServingCourse ServingCourseWithName( UnicodeString inSCName );
	TChefmateServingCourse ServingCourseWithCourseKey( __int32 inCKey );

    void ClearChefmateServingCourseMap();

private:
};

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//   TChefmateCourse
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct TChefmateCourse
{
	UnicodeString Name;
	UnicodeString KitchenName;
	__int32       ServingCourseKey;
	__int32 	  DisplayOrder;
};

//.............................................

class TChefmateCourseMap
{
protected:
	TIBDatabase *ibDatabase;

	map<__int32, TChefmateCourse> _cMap;

	void _readCourseWithKey(
			__int32          inCKey,
			TChefmateCourse& outSC );

	void _readCourseFromDBWithKey(
			__int32                   inCKey,
			Database::TDBTransaction* inDBTransaction,
			TChefmateCourse&          outC );

public:
/*	static TChefmateCourseMap& Instance()
	{
		static TChefmateCourseMap singleton;
		return singleton;
	} */

	TChefmateCourseMap();
	~TChefmateCourseMap();

	void SetDatabase( TIBDatabase *inIBDatabase );
	TChefmateCourse CourseWithKey( __int32 inCKey );

private:
};

//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//   TChefmatePrintingFormat
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

struct TChefmatePrintingFormat
{
	bool   Underline;
	bool   Bold;
	TColor Color;
	bool   LargeFont;
	bool   DoubleWidth;
	bool   DoubleHeight;
};

//.............................................

class TChefmatePrintingFormatMap
{
protected:
	TIBDatabase *ibDatabase;

	map<__int32, TChefmatePrintingFormat> _iPFMap;  // Item's Print Format Map
	map<__int32, TChefmatePrintingFormat> _iOPFMap; // Item Option's Print Format Map
	map<__int32, TChefmatePrintingFormat> _iSPFMap; // Item Side's Print Format Map
	map<__int32, TChefmatePrintingFormat> _sOPFMap; // Side Option's Print Format Map

	//..............................................

	void _readItemPFWithKey(
			__int32                  inItemKey,
			TChefmatePrintingFormat& outPF );

	void _readItemPFFromDBWithKey(
			__int32                   inItemKey,
			Database::TDBTransaction* inDBTransaction,
			TChefmatePrintingFormat&  outPF );

	//..............................................

	void _readItemOptionPFWithKey(
			__int32                  inItemOptionKey,
			TChefmatePrintingFormat& outPF );

	void _readItemOptionPFFromDBWithKey(
			__int32                   inItemOptionKey,
			Database::TDBTransaction* inDBTransaction,
			TChefmatePrintingFormat&  outPF );

	//..............................................

	void _readItemSidePFWithKey(
			__int32                  inItemSideKey,
			TChefmatePrintingFormat& outPF );

	void _readItemSidePFFromDBWithKey(
			__int32                   inItemSideKey,
			Database::TDBTransaction* inDBTransaction,
			TChefmatePrintingFormat&  outPF );

	//..............................................

	void _readSideOptionPFWithKey(
			__int32                  inSideOptionKey,
			TChefmatePrintingFormat& outPF );

	void _readSideOptionPFFromDBWithKey(
			__int32                   inSideOptionKey,
			Database::TDBTransaction* inDBTransaction,
			TChefmatePrintingFormat&  outPF );

public:
	/*static TChefmatePrintingFormatMap& Instance()
	{
		static TChefmatePrintingFormatMap singleton;
		return singleton;
	} */
	TChefmatePrintingFormatMap();
	~TChefmatePrintingFormatMap();

	void SetDatabase( TIBDatabase *inIBDatabase );

	TChefmatePrintingFormat ItemPFormatWithKey(       __int32 inItemKey );
	TChefmatePrintingFormat ItemOptionPFormatWithKey( __int32 inItemOptionKey );
	TChefmatePrintingFormat ItemSidePFormatWithKey(   __int32 inItemSideKey );
	TChefmatePrintingFormat SideOptionPFormatWithKey(  __int32 inItemSideKey );

private:
};

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::
// TChefmateOrderNumberGenerator: This is a temporary solution for generating Order Numbers.
//:::::::::::::::::::::::::::::::::::::::::::::::::

#define _ORDER_NO_FILE_NAME "ChefmateOrderNo.TXT"

class TChefmateOrderNumberGenerator
{
public:
	static TChefmateOrderNumberGenerator& Instance()
	{
		static TChefmateOrderNumberGenerator singleton;
		return singleton;
	}

	~TChefmateOrderNumberGenerator();

	void InitOrderNumber(  UnicodeString inTerminalName );
	void ResetOrderNumber();
	void NextOrderNumber();
	unsigned __int32 CurrentOrderNumber();

private:
	UnicodeString     _terminalName;
	unsigned __int32  _orderNo;

	TChefmateOrderNumberGenerator();

	bool loadOrderNo( unsigned __int32& outOrderNo );
	void saveOrderNo( unsigned __int32  inOrderNo );

	unsigned __int32 buildOrderNo(
                        const ip_utilities::ip_address_t &inTerminalIPAddress,
						unsigned __int32 inOrderNoPrefix,
						unsigned __int32 inOrderNoCounter );

	unsigned __int32 strHashUINT( UnicodeString inStr );

    ip_utilities::ip_address_t terminal_ip_address_;
};

//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::
// TDBChefmate
//:::::::::::::::::::::::::::::::::::::::::::::::::

class TDBChefmate
{
public:
	static int  GetItemSizeKey( int inItemKey, UnicodeString inSizeName );
	static long GetItemCourseKey( int inItemKey );

	static UnicodeString GetItemName( int inItemKey );
	static UnicodeString GetItemKitchenName( int inItemKey );
};

//---------------------------------------------------------------------------

#endif
