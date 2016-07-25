#pragma hdrstop

#include "DeviceRealTerminal.h"
#include "MagicMemoriesSfService.h"

#pragma package(smart_init)

#include <cassert>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <DateUtils.hpp>

#include "Enum.h"
#include "MagicMemoriesSfBinding.h"

namespace SfIntegration {

using std::auto_ptr;
using std::list;
using std::vector;

/*
 * See "Enum.h"; it defines the predefined staff types. The
 * staff type is determined by a contact's access-level.
 *
 * Anything that is not one of the predefined types is considered
 * a custom type. We use this definition to set the number of
 * staff titles which, at the time of writing, are as follows:
 *
 * "Administrator", "Custom", "Power User" and "Sales."
 *
 * It's a pain but this will have to be manually kept in sync
 * with Enum.h.
 */
#define ADMIN_TITLE      "Administrator"
#define CUSTOM_TITLE     "Custom"
#define POWER_USER_TITLE "Power User"
#define SALES_TITLE      "Sales"

/*
 * Make sure this (Nil_string) is defined before you include any of the
 * MagicMemoriesSf(Bridge|Data)*.cpp files are included as they all rely on it.
 *
 * This is defined as it's clearer to say "I want to represent a dead string"
 * if someone sees "Nil_string" rather than just ""; It gets rid of the
 * "Why is he returning an empty string?!?!" question.
 *
 * Everyone's used to NULL being used for signalling error conditions.
 */
static const UnicodeString Nil_string("");

/*
 * It's a hacky way to achieve some organization given the constraint that
 * _Sf_bridge cannot be defined "openly."
 */
#include "MagicMemoriesSfBridge.cpp"
#include "MagicMemoriesSfDataObject.cpp"
#include "MagicMemoriesSfDataObjectDay.cpp"
#include "MagicMemoriesSfDataObjectDeposit.cpp"
#include "MagicMemoriesSfDataObjectInventoryItem.cpp"
#include "MagicMemoriesSfDataObjectKpiType.cpp"
#include "MagicMemoriesSfDataObjectKpiValue.cpp"
#include "MagicMemoriesSfDataObjectPaymentType.cpp"
#include "MagicMemoriesSfDataObjectProduct.cpp"
#include "MagicMemoriesSfDataObjectDepositDetail.cpp"
#include "MagicMemoriesSfDataObjectSite.cpp"
#include "MagicMemoriesSfDataObjectStaffTitle.cpp"
#include "MagicMemoriesSfDataObjectStaff.cpp"
#include "MagicMemoriesSfDataObjectStaffTime.cpp"
#include "MagicMemoriesSfDataObjectTransaction.cpp"

Sf_svc_iface::Sf_svc_iface(Sf_svc_iface_params &parameters)
  : drt(TDeviceRealTerminal::Instance()),
    tr(drt.DBControl),
    sb(new _Sf_bridge(parameters.notification_receiver,
                      parameters.password + parameters.security_token,
                      parameters.username)),
    site_id(parameters.site),
    parameters(parameters)
{
   _Sf_day::set_cache(&day_cache);
	_Sf_deposit::set_cache(&deposit_cache);
   _Sf_kpi_type::set_cache(&kpi_type_cache);
   _Sf_payment_type::set_cache(&payment_type_cache);
   _Sf_product::set_cache(&product_cache);
   _Sf_site::set_cache(&site_cache);
   _Sf_staff::set_cache(&staff_cache);
   _Sf_staff_title::set_cache(&staff_title_cache);

   push_initialize();
}

Sf_svc_iface::~Sf_svc_iface()
{
   sb->logout();
}

void
Sf_svc_iface::push_blind_balances(int z_key)
{
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text = "select payment payment_type, "
                   "       blind_balance counted_value, "
                   "       system_balance calculated_value, "
                   "       depositbag_id deposit_bag_id "
	                "       from blindbalance "
                   "where z_key = :z_key "
                   "      and depositbag_id <> '';";

   tr.StartTransaction();

   qr->ParamByName("z_key")->AsInteger = z_key;
   for (qr->ExecQuery(); !qr->Eof; qr->Next()) {
      (UnicodeString)_Sf_deposit_detail(
        *sb,
        qr->FieldByName("counted_value")->AsDouble,
        qr->FieldByName("calculated_value")->AsDouble,
        _Sf_deposit(*sb,
                    qr->FieldByName("deposit_bag_id")->AsString,
                    _Sf_day(*sb, z_range.second, site_id)),
        _Sf_payment_type(*sb, qr->FieldByName("payment_type")->AsString));
   }

   qr->Close();
   tr.Commit();
}

void
Sf_svc_iface::push_inventory_items()
{
   auto_ptr<TIBDatabase>    db(new TIBDatabase(NULL));
   auto_ptr<TIBTransaction> tr(new TIBTransaction(NULL));
   auto_ptr<TIBSQL>         qr(new TIBSQL(NULL));

	const UnicodeString day_id = _Sf_day(*sb, Now(), site_id);

	qr->Database    = db.get();
   qr->Transaction = tr.get();

   tr->AddDatabase(db.get());

   db->DBParamByDPB[isc_dpb_user_name] = "sysdba";
   db->DBParamByDPB[isc_dpb_password]  = "masterkey";
   db->DatabaseName = parameters.stock_database_path;
	db->LoginPrompt = false;

	db->Open();

   qr->SQL->Text = "select si.description name, "
                   "       si.code, "
                   "       sl.on_hand quantity, "
                   "       sl.average_cost, "
                   "       sl.latest_cost "
                   "       from stock si "
                   "            inner join stocklocation sl on "
                   "                  sl.stock_key = si.stock_key ";
	                "       where si.deleted = 'F';";
   tr->StartTransaction();

   for (qr->ExecQuery(); !qr->Eof; qr->Next())
		(UnicodeString)_Sf_inventory_item(
        *sb,
        qr->FieldByName("average_cost")->AsCurrency,
        qr->FieldByName("latest_cost")->AsCurrency,
        qr->FieldByName("quantity")->AsDouble,
        day_id,
        qr->FieldByName("name")->AsString,
        qr->FieldByName("code")->AsString);

   qr->Close();
   tr->Commit();
	db->Close();
}

void
Sf_svc_iface::push_data()
{
   int zed_key = get_last_zed_info(z_range);

   /* Ordered from (what I see to be) most to least important. */
   if (parameters.upload_transactions)
      push_transactions();
   if (parameters.upload_staff_hours)
      push_staff_hours();
   if (parameters.upload_transaction_kpis)
      push_transaction_kpis();
   if (parameters.upload_pax)
      push_pax_kpis(zed_key);
	if (parameters.upload_stock)
		push_inventory_items();
   if (parameters.upload_printer_counts)
      push_printer_kpis(zed_key);

	push_blind_balances(zed_key);
}

const UnicodeString
Sf_svc_iface::access_level_to_staff_title(int access_level)
{
   UnicodeString title;

   switch (access_level)   {
   case mmAdmin:
      title = ADMIN_TITLE;
      break;
   case mmPowerUser:
      title = POWER_USER_TITLE;
      break;
   case mmSales:
      title = SALES_TITLE;
      break;
   default:
      title = CUSTOM_TITLE;
      break;
   };

   /* Most of the time the IDs of these titles will be cached. */
   return _Sf_staff_title(*sb, 0.0, 0.0, site_id, title);
}

void
Sf_svc_iface::add_line_item_from_query(
  TIBSQL *qr,
  _Sf_transaction *transaction)
{
   transaction->add_line_item(
     _Sf_product(*sb,
                 qr->FieldByName("item_name")->AsString,
                 qr->FieldByName("plu")->AsFloat,
                 site_id,
                 qr->FieldByName("size_name")->AsString),
     qr->FieldByName("price")->AsFloat,
     qr->FieldByName("qty")->AsFloat);
}

_Sf_transaction *
Sf_svc_iface::create_transaction_from_query_row(
  TIBSQL *qr,
  int &invoice_number)
{
   return new _Sf_transaction(
     *sb,
     _Sf_day(*sb,
             qr->FieldByName("time_stamp")->AsDate,
             site_id),
     invoice_number =
       qr->FieldByName("invoice_number")->AsInteger,
     _Sf_staff(*sb,
               qr->FieldByName("staff_name")->AsString,
               site_id,
               access_level_to_staff_title(
                 qr->FieldByName("access_level")->AsInteger)),
      qr->FieldByName("time_stamp")->AsDate);
}

void
Sf_svc_iface::build_kpis_from_query(TIBSQL *qr)
{
   if (qr->ExecQuery(), qr->Eof)
      return;

   for (UnicodeString m, n; !qr->Eof; qr->Next()) {
      m = qr->FieldByName("type")->AsString;
      n = m.SubString(1, 1).UpperCase()
          + m.SubString(2, m.Length() - 1).LowerCase();

      (const UnicodeString)_Sf_kpi_value(
        *sb,
        _Sf_day(*sb, qr->FieldByName("date")->AsDate, site_id),
        _Sf_kpi_type(*sb, n),
        qr->FieldByName("value")->AsFloat);
   }
}

int
Sf_svc_iface::get_last_zed_info(datetime_range &destination)
{
   TIBSQL         *qr = tr.Query(tr.AddQuery());
   int            key;

   qr->SQL->Text = "select first 2 "
                   "       z_key, "
                   "       time_stamp "
                   "       from zeds "
                   "       order by time_stamp desc;";

   tr.StartTransaction();

   qr->ExecQuery();
   assert(qr->RecordCount);
   
   key = qr->FieldByName("z_key")->AsInteger;
   destination.second = qr->FieldByName("time_stamp")->AsDateTime;
   if (qr->Next(), !qr->Eof)
      destination.first = qr->FieldByName("time_stamp")->AsDateTime;

   qr->Close();
   return tr.Commit(), key;
}

/*
 * Note that this function only uploads the PAX entered at the time of Z.
 * It does /not/ upload updates to previously entered PAX values. "Update" PAX
 * entries do not have a Z key set. The reason we don't upload them here is
 * there is no real way to filter out those that have already been sent.
 *
 * The function "update_pax_kpi" is used to update an existing PAX in
 * Salesforce.
 */
void
Sf_svc_iface::push_pax_kpis(int z_key)
{
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text = "select date_from, "
                   "       date_to, "
                   "       pax_number "
                   "       from paxcount "
                   "       where z_key = :z_key;";

   tr.StartTransaction();
   qr->ParamByName("z_key")->AsInteger = z_key;

   if (qr->ExecQuery(), !qr->Eof) {
      TDateTime     df;
      TDateTime     dt;
      UnicodeString ktid = _Sf_kpi_type(*sb, "Pax");

      for (double an; !qr->Eof; qr->Next()) {
         an = (qr->FieldByName("pax_number")->AsFloat
               / DaysBetween(df = qr->FieldByName("date_from")->AsDateTime,
                             dt = qr->FieldByName("date_to")->AsDateTime));
         for ( ; df < dt; df++)
            (const UnicodeString)_Sf_kpi_value(*sb, _Sf_day(*sb, df, site_id),
                                               ktid, an);
      }
   }

   qr->Close();
   tr.Commit();
}

void
Sf_svc_iface::push_initialize()
{
   sb->login();
   site_id = _Sf_site(*sb, site_id);
}

void
Sf_svc_iface::push_printer_kpis(int z_key)
{
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text = "select printer_name \"type\", "
                  "        current_date \"date\", "
                  "        copies \"value\" "
                  "        from printerreadings "
                  "        where z_key = :z_key "
                  "              and copies <> 0;";

   tr.StartTransaction();
   qr->ParamByName("z_key")->AsInteger = z_key;

   build_kpis_from_query(qr);

   qr->Close();
   tr.Commit();
}

void
Sf_svc_iface::push_staff_hours()
{
   TIBSQL        *qr = tr.Query(tr.AddQuery());
   UnicodeString id;

   qr->SQL->Text = "select distinct "
                   "       c.name, "
                   "       c.access_level, "
                   "       c.hourly_rate, "
                   "       ct.breaks, "
                   "       ct.totalhours, "
                   "       ct.login_datetime, "
                   "       ct.logout_datetime "
                   "       from contacts c "
                   "            inner join contacttime ct on "
                   "                  ct.contacts_key = c.contacts_key "
                   "       where ct.breaks is not null "
                   "             and ct.totalhours is not null "
                   "             and ct.login_datetime is not null "
                   "             and ct.logout_datetime is not null "
                   "             and ct.login_datetime >= :last_z "
                   "             and ct.logout_datetime <= :curr_z;";

   tr.StartTransaction();

   qr->ParamByName("last_z")->AsDateTime = z_range.first;
   qr->ParamByName("curr_z")->AsDateTime = z_range.second;

   for (qr->ExecQuery(); !qr->Eof; qr->Next())
      id = _Sf_staff_time(*sb,
                          MinuteOf(qr->FieldByName("breaks")->AsDateTime),
	                       qr->FieldByName("hourly_rate")->AsDouble,
                          qr->FieldByName("totalhours")->AsDouble,
                          qr->FieldByName("login_datetime")->AsDateTime,
                          qr->FieldByName("logout_datetime")->AsDateTime,
                          _Sf_day(*sb, Now(), site_id),
                          _Sf_staff(
                             *sb,
                             qr->FieldByName("name")->AsString,
                             site_id,
                             access_level_to_staff_title(
                               qr->FieldByName("access_level")->AsInteger)));

   qr->Close();
   tr.Commit();
}

void
Sf_svc_iface::push_transaction_kpis()
{
   TIBSQL *qr = tr.Query(tr.AddQuery());

   qr->SQL->Text =
     "select tpc.code \"type\", "
     "       current_date \"date\", "
     "       count(*) \"value\" "
     "       from archive a "
     "            inner join thirdpartycodes tpc on "
     "                  tpc.thirdpartycodes_key = a.thirdpartycodes_key "
     "       where a.terminal_name = :terminal_name "
     "             and time_stamp between :last_z and :curr_z "
     "             and tpc.code in ('PACK', 'PRINT') "
     "       group by tpc.code;";

   tr.StartTransaction();
   qr->ParamByName("last_z")->AsDateTime = z_range.first;
   qr->ParamByName("curr_z")->AsDateTime = z_range.second;
   qr->ParamByName("terminal_name")->AsString = drt.ID.Name;

   build_kpis_from_query(qr);

   qr->Close();
   tr.Commit();
}

void
Sf_svc_iface::push_transactions()
{
   TIBSQL                    *qr = tr.Query(tr.AddQuery());
   int                       last_invoice_number = 0;
   auto_ptr<_Sf_transaction> transaction;

   qr->SQL->Text =
     "select ab.time_stamp, "
     "       ab.invoice_number, "
     "       ab.staff_name, "
     "       c.access_level, "
     "       a.item_name, "
     "       a.size_name, "
     "       a.plu, "
     "       a.price, "
     "       sum(a.qty) qty "
     "       from arcbill ab "
     "            inner join contacts c on "
     "                  c.name = ab.staff_name "
     "            inner join archive a on "
     "                  a.arcbill_key = ab.arcbill_key "
     "       where ab.terminal_name = :terminal_name "
     "             and ab.time_stamp between :last_z and :curr_z"
     "       group by ab.time_stamp, "
     "                ab.invoice_number, "
     "                ab.staff_name, "
     "                c.access_level, "
     "                a.item_name, "
     "                a.size_name, "
     "                a.plu, "
     "                a.price;";

   tr.StartTransaction();

   qr->ParamByName("last_z")->AsDateTime = z_range.first;
   qr->ParamByName("curr_z")->AsDateTime = z_range.second;
   qr->ParamByName("terminal_name")->AsString = drt.ID.Name;
   if (qr->ExecQuery(), qr->Eof) {
      qr->Close();
      tr.Commit();
      return;
   }

   /* Create the first Transaction's initial Line Item. */
   transaction.reset(
     create_transaction_from_query_row(qr, last_invoice_number));
   add_line_item_from_query(qr, transaction.get());

   for (qr->Next() ; !qr->Eof; qr->Next()) {
      if (qr->FieldByName("invoice_number")->AsInteger
          != last_invoice_number) {
         /* Trigger the creation of the Transaction on Salesforce's side. */
         (const UnicodeString)*transaction;
         transaction.reset(
           create_transaction_from_query_row(qr, last_invoice_number));
      }
      add_line_item_from_query(qr, transaction.get());
   }

   /*
    * The final result row will not be created as we never get to
    * compare it against a previous and, different, invoice number before the
    * loop breaks; we need to trigger it here.
    */
   (const UnicodeString)*transaction;

   qr->Close();
   tr.Commit();
}

/*
 * This function's used to update PAX values. Keep in mind, however, that this
 * function will create NEW values in Salesforce if a previous entry with the
 * same "date_from" cannot be found.
 *
 * The same rule applies when updating the values for the PAX KPIs within the
 * "PAX Date Range."
 */

void
Sf_svc_iface::update_pax_kpi(
  TDateTime date_from,
  TDateTime date_to,
  double pax_count)
{
   UnicodeString ktid = _Sf_kpi_type(*sb, "Pax");
   double an = pax_count / DaysBetween(date_from, date_to);

   while (date_from < date_to)
      (UnicodeString)_Sf_kpi_value(*sb, _Sf_day(*sb, date_from++, site_id),
                                   ktid, an);
}

}

