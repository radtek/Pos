//---------------------------------------------------------------------------

#ifndef ISaveOrderH
#define ISaveOrderH

//---------------------------------------------------------------------------

#include <vcl.h>

#include "ChefMateOrderGenerator.h"

class ISaveOrder
{
	public:
		virtual ~ISaveOrder() {}

		// Open a Complete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		virtual HORDER OpenCompleteOrder( __int32    inDBKey,
										  AnsiString inServerName,
										  __int32    inOrderNumber,
										  AnsiString inChitValue,
										  AnsiString inTableTabName,
										  AnsiString inOrderType,
										  AnsiString inCustomerName,
										  AnsiString inPartyName,
										  AnsiString inPatronCount,
										  AnsiString inSaleStartTime,
										  AnsiString inDeliveryTime,
                                          AnsiString phone,
                                          AnsiString email,
                                          AnsiString address,
                                          AnsiString paymentStatus ) = 0;

		// Opens an Incomplete Order
		// Order's components must be added afterwards
		// Once all the components SaveOrder() must be called
		virtual HORDER OpenIncompleteOrder( __int32    inDBKey,
											AnsiString inServerName,
											__int32    inOrderNumber,
											AnsiString inChitValue,
											AnsiString inTableTabName,
											AnsiString inOrderType,
											AnsiString inCustomerName,
										    AnsiString inPartyName ) = 0;

		// Opens a Cancel Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		virtual HORDER OpenCancelOrder( __int32 inDBKey,
										__int32 inOrderNumber,
                                          AnsiString inTableTabName ) = 0;

		// Opens a Credit Order
		// The items to be canceled must be added afterwards
		// Once all the items are added SaveOrder() must be called
		virtual HORDER OpenCreditOrder() = 0;

		// Opens a Transfer Order
		// SaveOrder() must be called straight away
		virtual HORDER OpenTransferOrder( __int32    inDBKey,
										  __int32    inOrderNumber,
										  AnsiString inTableTabName,
                                          AnsiString inServerName,
                                          AnsiString inPatronCount,
                                          AnsiString inSaleStartTime,
                                          AnsiString inSouceTableName) = 0;

		// Adds a ServingCourse to an Order
		virtual HSCOURSE AddServingCourse( __int32    inDBKey,
										   AnsiString inName,
								           AnsiString inKitchenName,
										   __int32 	  inDisplayOrder ) = 0;

		// Adds a Course to an Order
		virtual HCOURSE AddCourse( __int32    inDBKey,
								   AnsiString inName,
								   AnsiString inKitchenName,
								   __int32    inServingCourseKey,
								   __int32 	  inDisplayOrder ) = 0;

		// Adds a Item to an Order
		virtual HITEM AddItem( __int32    inDBKey,
							   __int32    inOrderItemDBKey,
							   AnsiString inName,
							   AnsiString inKitchenName,
							   __int32    inServingCourseKey,
							   __int32    inCourseKey,
							   AnsiString inNote ) = 0;

		// Adds a Item to cancel to an Order
		// THIS FUNCTION IS TO BE USED FOR CANCEL ORDERS ONLY
		// If inCaceled is false then inQty is ignored
		virtual HITEM AddItemToCancel( __int32    inOrderItemKey,
									   AnsiString inItemName,
									   bool       inCanceled = true,
									   __int32    inQty      = 1 ) = 0;

		// Adds a Item's Print Format to an Order
		virtual HPFORMAT AddItemPrintingFormat( HITEM   inItemHandle,
												__int32 inDBKey,
												bool	inUnderline,
												bool	inBold,
												TColor  inColor,
												bool	inLargeFont,
												bool	inDoubleWidth,
												bool	inDoubleHeight ) = 0;

		// Adds a Item's Option to an Order
		virtual HOPTION AddItemOption( HITEM      inItemHandle,
									   __int32    inDBKey,
									   AnsiString inName,
									   AnsiString inKitchenName,
									   bool	      inIsPlus ) = 0;

		// Adds a Item's Option Print Format to an Order
		virtual HPFORMAT AddItemOptionPrintingFormat( HOPTION inOptionHandle,
													  __int32 inDBKey,
													  bool	  inUnderline,
													  bool	  inBold,
													  TColor  inColor,
													  bool	  inLargeFont,
													  bool	  inDoubleWidth,
													  bool	  inDoubleHeight ) = 0;

		// Adds a Item's Side to an Order
		virtual HSIDE AddItemSide( HITEM      inItemHandle,
								   __int32    inOrderSideKey,
								   AnsiString inName,
								   AnsiString inKitchenName ) = 0;

		virtual HSIDE AddItemSideToCancel( HITEM      inItemHandle,
										   __int32    inOrderSideKey,
										   AnsiString inSideName ) = 0;

		// Adds a Option's Print Format to an Order
		virtual HPFORMAT AddSidePrintingFormat( HSIDE      inSideHandle,
												__int32    inDBKey,
												bool	   inUnderline,
												bool	   inBold,
												TColor	   inColor,
												bool	   inLargeFont,
												bool	   inDoubleWidth,
												bool	   inDoubleHeight ) = 0;

		// Adds a Side's Option to an Order
		virtual HOPTION AddSideOption( HSIDE      inSideHandle,
									   __int32    inDBKey,
									   AnsiString inName,
									   AnsiString inKitchenName,
									   bool	      inIsPlus ) = 0;

		// Adds a Option's Print Format to an Order
		virtual HPFORMAT AddSideOptionPrintingFormat( HOPTION    inOptionHandle,
													  __int32    inDBKey,
													  bool	  	 inUnderline,
													  bool		 inBold,
													  TColor	 inColor,
													  bool	     inLargeFont,
													  bool		 inDoubleWidth,
													  bool		 inDoubleHeight ) = 0;

		// Commit an Order
		virtual OERROR CommitOrder( HostnameLIST inHostNameList ) = 0;
};

//----------------------------------------------------------------------------

#endif


