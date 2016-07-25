/*
 * This file is an interface (or bridge) to the Salesforce object store for
 * Magic Memories. It's purpose is to store the transactions performed in a
 * single day. that is to say all transactions that have occured between the
 * previous and current zed time, into the Salesforce object store.
 *
 * The user-visible interface (where the user is other code within the P.O.S)
 * is only to create the object and submit transaction information.
 *
 * The input of the class, which are determined and pulled internally by the
 * class, is the local Menumate database. The output of the class is a value
 * specifying whether the push was successful and in the case the push is not
 * successful the result represents a code representing the type of error.
 * The function optionally returns a string which can be displayed to the
 * operator of the P.O.S.
 */
#ifndef MagicMemoriesSfServiceH
#define MagicMemoriesSfServiceH

#include <System.hpp>
#include <list>
#include <map>
#include <string>

#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include "MM_DBCore.h"

namespace SfIntegration {

using std::list;
using std::map;
using std::pair;
using std::string;

using Database::TDBTransaction;

enum Sf_result {
   COULDNT_CONNECT,
   COULDNT_CREATE_OBJECT,
   INVALID_AUTH_CREDENTIALS,
   SUCCESS
};

class _Sf_bridge;

typedef map<TDate, UnicodeString>         date_map;
typedef pair<TDateTime, TDateTime>        datetime_range;
typedef map<UnicodeString, UnicodeString> string_map;

enum Sf_data_object_type {
   DAY,
   DEPOSIT,
   DEPOSIT_DETAIL,
   INVENTORY_ITEM,
   KPI_TYPE,
   KPI_VALUE,
   LINE_ITEM,
   NIL,
   PRODUCT,
   SITE,
   STAFF,
   STAFF_TIME,
   STAFF_TITLE,
   TRANSACTION,
   SF_DATA_OBJECT_TYPES
};

const UnicodeString Sf_data_object_types[SF_DATA_OBJECT_TYPES] = {
   "Day",
   "Deposit Detail",
   "Deposit",
   "Inventory Item",
   "KPI Value",
   "KPI",
   "Line Item",
   "Nil",
   "Product",
   "Site",
   "Staff Time",
   "Staff Title",
   "Staff",
   "Transaction"
};

enum Sf_notification {
   AUTHENTICATING,
   AUTHENTICATION_FAILURE,
   AUTHENTICATION_SUCCESS,
   UPLOADING,
   UPLOAD_FAILURE,
   UPLOAD_SUCCESS
};

class _Sf_transaction;

class Sf_notification_receiver {
public:
   Sf_notification_receiver() { };
   virtual ~Sf_notification_receiver() { };

   virtual void push_notification(
     Sf_notification notification_type,
     Sf_data_object_type type = Sf_data_object_type::NIL) = 0;
};

struct Sf_svc_iface_params {
   Sf_svc_iface_params()
     : notification_receiver(0x0)
   {
      upload_pax = upload_printer_counts =
        upload_staff_hours = upload_stock =
		    upload_transaction_kpis = upload_transactions = false;
   }

   Sf_notification_receiver *notification_receiver;

   UnicodeString password;
   UnicodeString security_token;
   UnicodeString site;
   UnicodeString stock_database_path;
   UnicodeString username;

	bool upload_stock;
   bool upload_pax;
   bool upload_printer_counts;
   bool upload_staff_hours;
   bool upload_transaction_kpis;
   bool upload_transactions;
};

class Sf_svc_iface {
public:
   Sf_svc_iface(Sf_svc_iface_params &parameters);
   ~Sf_svc_iface();

   void push_data();
   void update_pax_kpi(TDateTime date_from, TDateTime date_to,
                       double pax_count);

private:
   Sf_svc_iface(const Sf_svc_iface &);
   Sf_svc_iface &operator=(const Sf_svc_iface &);

   const UnicodeString access_level_to_staff_title(int access_level);

   void  add_line_item_from_query(TIBSQL *qr, _Sf_transaction *transaction);
   void  build_kpis_from_query(TIBSQL *qr);

   _Sf_transaction *create_transaction_from_query_row(TIBSQL *qr,
                                                      int &invoice_number);

   int get_last_zed_info(datetime_range &destination);

   void push_initialize();

   void push_blind_balances(int z_key);
   void push_inventory_items();
   void push_pax_kpis(int z_key);
   void push_printer_kpis(int z_key);
   void push_staff_hours();
   void push_transactions();
   void push_transaction_kpis();

   TDeviceRealTerminal &drt;
   TDBTransaction      tr;
   _Sf_bridge          *sb;

   date_map   day_cache;
	string_map deposit_cache;
   string_map kpi_type_cache;
	string_map payment_type_cache;
   string_map product_cache;
   string_map site_cache;
   string_map staff_cache;
   string_map staff_title_cache;

   Sf_svc_iface_params parameters;

   UnicodeString  site_id;
   datetime_range z_range;
};

};

#endif
