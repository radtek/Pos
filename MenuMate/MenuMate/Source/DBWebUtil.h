// ---------------------------------------------------------------------------

#ifndef DBWebUtilH
#define DBWebUtilH

#include "MM_DBCore.h"
#include "XMLOrdermm.h"
#include "MMLogging.h"
#include "ItemComplete.h"
#include "ItemCompleteSub.h"
#include "WebOrderContainer.h"
#include "Membership.h"
#include "ChitNumberController.h"

// ---------------------------------------------------------------------------
enum WebOrderStatus
{
	ewosReceived = 0, ewosProcessed, ewosBilled
};

class TDBWebUtil
{
private:
	static int CreateWebOrder(Database::TDBTransaction &DBTransaction);
	static int GetOrderKey(Database::TDBTransaction &DBTransaction, UnicodeString GUID);

public:
	TDBWebUtil();
	~TDBWebUtil();

	static void UpdateDBVersion_5_2(Database::TDBControl &DBControl);
	static void TearDownDBVersion_5_2(Database::TDBControl &DBControl);

	static int LoadItemSizeKeyFromThridPartyCode(Database::TDBTransaction &DBTransaction, UnicodeString PLU);
	static bool CheckItemCompleteEnabled(Database::TDBTransaction &DBTransaction, int SizeKey);
	static bool LoadItemComplete(Database::TDBTransaction &DBTransaction, TItemComplete *Order, int SizeKey);
	static bool LoadItemComplete(Database::TDBTransaction &DBTransaction, TItemCompleteSub *Order, int SizeKey);
	static void LoadBreakDownCategories(Database::TDBTransaction &DBTransaction, TListCategoryContainer *CategoryContainer, int SizeKey);
	static void LoadSalesRecipes(Database::TDBTransaction &DBTransaction, TListRecipeContainer *RecipeContainer, int SizeKey, double OrderQty);

	static bool LoadItemOption(Database::TDBTransaction &DBTransaction, TItemOption &Option);
	static int CreateWebOrderFromXML(Database::TDBTransaction &DBTransaction, _di_IXMLORDERType &WebOrder);

	static void SetOrderGUID(Database::TDBTransaction &DBTransaction, int WebOrderKey, UnicodeString GUID);
	static void SetTabKey(Database::TDBTransaction &DBTransaction, int WebOrderKey, int TabKey);
	static void SetWebOrderStatus(Database::TDBTransaction &DBTransaction, int WebOrderKey, WebOrderStatus Status);
	static void SetWebOrderTimeKey(Database::TDBTransaction &DBTransaction, int WebOrderKey, int TimeKey);
	static void SetWebOrderContactKey(Database::TDBTransaction &DBTransaction, int WebOrderKey, int ContactsKey);

	static UnicodeString GetOrderGUID(Database::TDBTransaction &DBTransaction, int WebOrderKey);
	static bool GetWebOrderExists(Database::TDBTransaction &DBTransaction, int WebOrderKey);
	static int GetTabKeyByWebOrderKey(Database::TDBTransaction &DBTransaction, int WebOrderKey);
	static int GetWebOrderKeyByTabKey(Database::TDBTransaction &DBTransaction, int TabKey);
	static int GetKeyByOrderGUID(Database::TDBTransaction &DBTransaction, UnicodeString GUID);

	static bool WebOrdersPending(Database::TDBTransaction &DBTransaction);
	static void LoadWebOrders(Database::TDBTransaction &DBTransaction, TWebOrderContainer &WebOrderContainer);
	static void LoadWebOrder(Database::TDBTransaction &DBTransaction, int WebOrderKey, TWebOrder &WebOrder);
	static int FindOrCreateWebOrder(Database::TDBTransaction &DBTransaction, int WebKey);

	static void DeleteWebOrder(Database::TDBTransaction &DBTransaction, int WebOrderKey);
	static void getWebOrderDetials(Database::TDBTransaction &DBTransaction, int WebOrderKey, TStringList &Detials);
	static void GetTabs(Database::TDBTransaction &DBTransaction, TStringList * TabList, WebOrderStatus Status);
	static void GetUnpaidTabs(Database::TDBTransaction &, TStringList *);
	static Currency __fastcall ProcessPrePay(_di_IXMLACCOUNTType &OrderAccount);
	static Currency GetWebOrderPrePaid(Database::TDBTransaction &DBTransaction, int WebOrderKey);

	// UnicodeString GetPartyName(Database::TDBTransaction &DBTransaction, int WebOrderKey);
	// void SetPartyName(Database::TDBTransaction &DBTransaction,int WebOrderKey,UnicodeString PartyName);
    static void TDBWebUtil::ProcessWebOrderData(Database::TDBTransaction &DBTransaction,int WebKey,_di_IXMLORDERType &WebOrder);
    static void TDBWebUtil::ProcessWebOrderElement(Database::TDBTransaction &DBTransaction,int WebKey,
	UnicodeString Category, UnicodeString Element, UnicodeString Value );
    static void TDBWebUtil::getWebOrderExtraData(Database::TDBTransaction &DBTransaction,int WebKey,UnicodeString Category, TStringList *Strings);

    static int GetWebOrderCovers(Database::TDBTransaction &DBTransaction, int inWebOrderKey );
    static TDateTime getWebOrderDeliveryTime(Database::TDBTransaction &DBTransaction,int WebKey, TDateTime delivery_time);
    static void TDBWebUtil::getWebOrderContactDetails(Database::TDBTransaction &DBTransaction, UnicodeString Email, TStringList *Strings);
    static TMMContactInfo autoAddWebMembers(Database::TDBTransaction &DBTransaction, TStringList *WebOrderDetails, TStringList *WebMemberAddress, int web_key);
    static TMMContactInfo checkAppliedChit(TChitNumber WebOrderChitNumber, TMMContactInfo webMemberInfo, TStringList *WebMemberAddress);
    static void TDBWebUtil::getWebOrderData(Database::TDBTransaction &DBTransaction, TStringList *webDeliveryDetails, TStringList *webPaymentDetials, TStringList *webComments,  int web_key);
    static void InitializeChit(int web_key, TChitNumber &WebOrderChitNumber);//,TChitNumberController &ChitNumberController);
    static void AssignChitNumber(Database::TDBTransaction &DBTransaction, TChitNumber &chit_number, TChitNumber &WebOrderChitNumber);//, TChitNumberController &ChitNumberController);
    static TMMContactInfo LoadMemberDetails(Database::TDBTransaction &DBTransaction, int webKey);
    static UnicodeString LoadPaymentStatus(Database::TDBTransaction &DBTransaction, int webKey);
};
#endif
