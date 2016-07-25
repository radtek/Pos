#include <stdexcept>
#include <map>
#include <vcl.h>
#include <vector>
#include <windows.h>
#pragma hdrstop

#include "FormMembershipExporter.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

using std::auto_ptr;
using std::ofstream;
using std::map;
using std::vector;
using std::runtime_error;

/*
 * Unfortunately I couldn't find out what the symbolic constans for
 * Interbase's error codes. So, I have them here as constants. Kludgey,
 * I know but it's better than no error handling at all.
 */
enum interbase_error_code {
	iec_bad_credentials    = 0x14000098,
	iec_bad_host_path_pair = 0,
	iec_io_error           = 0x14000018,
	iec_table_doesnt_exist = 0x14000187,
	iec_unknown            = 0xdeadbeef
};

TfrmMemberExporter *frmMemberExporter;

__fastcall
TfrmMemberExporter::TfrmMemberExporter(TComponent *owner)
	: TForm(owner)
{
	error_map[iec_bad_credentials] = error_map[iec_table_doesnt_exist] =
		"The path you've given does not identify a Menumate database. "
	   "Check that your path is correct.";
   error_map[iec_io_error] =
	  error_map[iec_bad_host_path_pair] =
	    "The path you've given is invaild. Please check that the host:path pair"
	    " you've given is correct.";
	error_map[iec_unknown] =
     "I've encountered an error that I don't recognize. I have no advice for "
	  "you other than to check that your database isn't corrupted and try "
	  "again.";
}

#pragma warn -par

void __fastcall
TfrmMemberExporter::export_membership_data(TObject *sender)
{
	ofstream stream;

	try {
		UnicodeString            csv_path;
      auto_ptr<TIBDatabase>    db;
      auto_ptr<TIBTransaction> tr;
      auto_ptr<TIBSQL>         qr;

		if (!get_output_csv_path(csv_path)) {
			Application->MessageBox(L"You must give a path to export to.",
			                        L"Export error", MB_ICONERROR | MB_OK);
			return;
		} else if (stream.open(csv_path.c_str()), stream.fail()) {
			Application->MessageBox(L"Couldn't open the CSV file for writing."
			                         "Ensure you have the permissions to write "
			                         "at your target. If you're on a network, "
			                         "consult your administrator.",
			                        L"Export error", MB_ICONERROR | MB_OK);
			return;
		}

		btnExport->Enabled = false;
		pbExportProgress->Position = 0;

      setup_database_link(db, tr, qr);
		retrieve_and_write_membership_data(stream, *tr, *qr);

		Application->MessageBox(L"Export complete.", L"Export complete.",
		                        MB_ICONINFORMATION | MB_OK);
	} catch (EIBError &ibe) {
		handle_error(ibe.IBErrorCode);
	}

	if (stream.is_open())
		stream.close();

   btnExport->Enabled = true;
}

#pragma warm .par

void
TfrmMemberExporter::retrieve_and_write_membership_data(
  ofstream &stream,
  TIBTransaction &tr,
  TIBSQL &qr)
{
	try {
		int n_records;

		tr.StartTransaction();

		qr.SQL->Text = "select count(*) n_records from contacts;";
		qr.ExecQuery();
		n_records = qr.FieldByName("n_records")->AsInteger;

      tr.CommitRetaining();
		qr.Close();

      qr.SQL->Text =
         "select cm.company_name, "
         "       c.contact_id, "
         "       c.contact_type, "
         "       c.name, "
         "       c.title, "
         "       c.sex, "
         "       c.first_name, "
         "       c.middle_name, "
         "       c.last_name, "
         "       c.initials, "
         "       c.location_address, "
         "       c.mailing_address, "
         "       c.phone, "
         "       c.phone_extension, "
         "       c.mobile, "
         "       c.fax, "
         "       c.email, "
         "       c.total_spent, "
         "       c.pin, "
         "       c.prox_card_old, "
         "       c.access_level, "
         "       c.palm_auto_options, "
         "       c.palm_large_font, "
         "       c.palm_advanced_user, "
         "       c.palm_timeout, "
         "       c.tab_enalbed, "
         "       c.payroll_id, "
         "       c.start_time1, "
         "       c.start_time2, "
         "       c.start_time3, "
         "       c.start_time4, "
         "       c.start_time5, "
         "       c.start_time6, "
         "       c.dateofbirth, "
         "       c.next_order_number, "
         "       c.order_prefix, "
         "       c.order_suffix, "
         "       c.palm_screen_lock, "
         "       c.palm_classic_mode, "
         "       c.palm_sound_level, "
         "       c.palm_get_all_tables, "
         "       c.member_number, "
         "       c.site_id, "
         "       c.card_creation_date, "
         "       c.known_as, "
         "       c.last_modified, "
         "       c.note, "
         "       c.points_rules, "
         "       c.account_name, "
         "       c.account_id, "
         "       c.last_visit, "
         "       c.prox_card, "
         "       c.account_profile, "
         "       cc.swipe_card "
         "       from contacts c "
         "            left join companies cm on "
         "                 cm.company_key = c.company_key "
         "            left join contactcards cc on "
         "                 cc.contacts_key = c.contacts_key;";

      pbExportProgress->Max = n_records;
		for (qr.ExecQuery(); !qr.Eof; pbExportProgress->Position++, qr.Next())
			write_member_record(stream, qr);

		tr.Commit();
      qr.Close();

		stream.flush();
	} catch (...) {
		tr.Rollback();
		qr.Close();
		throw;
	}
}

UnicodeString
TfrmMemberExporter::get_database_path_from_registry()
{
	static const UnicodeString nill_str;

	HKEY hkMenumateDatabaseKey;
	UnicodeString db_ip;
	UnicodeString db_path;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_KEY_MENUMATE_DATABASE, 0, KEY_READ,
	                 &hkMenumateDatabaseKey) != ERROR_SUCCESS)
		return nill_str;
	else if (get_string_value_from_registry(db_ip, "InterbaseIP",
	                                        hkMenumateDatabaseKey) < 0
	         || get_string_value_from_registry(db_path, "DatabasePath",
                                              hkMenumateDatabaseKey) < 0)
		return nill_str;

	return db_ip + ":" + db_path;
}

bool
TfrmMemberExporter::get_output_csv_path(UnicodeString &destination)
{
	bool                  retcode;
	auto_ptr<TSaveDialog> sd(new TSaveDialog(this));

	sd->Filter = "Comma Seperated Values File (*.csv)|*.csv";
	sd->Options << ofOverwritePrompt << ofNoChangeDir << ofPathMustExist
	            << ofDontAddToRecent << ofNoReadOnlyReturn;
	sd->OptionsEx << ofExNoPlacesBar;

	retcode = sd->Execute();
	return destination = sd->FileName, retcode;
}

int
TfrmMemberExporter::get_string_value_from_registry(
  UnicodeString &destination,
  const char *value_name,
  HKEY key)
{
	vector<BYTE>  buffer;
	unsigned long datalen;
	unsigned long type;

	/* Try and determine the length of the string we're going to retrieve. */
	if (RegQueryValueEx(key, value_name, NULL, &type, NULL, &datalen)
	    != ERROR_SUCCESS)
		return -1;

	/* Is the value actually a string? */
	if (type != REG_SZ)
		return -1;

	/* Is the string empty? If so, signal a warning and empty the destination. */
	if (!datalen)
		return destination = "", 1;

	buffer.reserve(datalen);

	/* Get the data for real. */
	if (RegQueryValueEx(key, value_name, NULL, NULL, &buffer[0], &datalen)
	    != ERROR_SUCCESS)
		return -1;

	return destination = UnicodeString(reinterpret_cast<char *>(&buffer[0])), 0;
}

void
TfrmMemberExporter::handle_error(unsigned int error_code)
{
	map<unsigned int, UnicodeString>::const_iterator i =
	  error_map.find(error_code);

	error_code = i != error_map.end() ? error_code : iec_unknown;

	Application->MessageBox(error_map[error_code].w_str(), L"Export error",
	                        MB_ICONERROR | MB_OK);
}

#pragma warn -par

void __fastcall
TfrmMemberExporter::initialize(TObject *sender)
{
	edMenumateDatabasePath->Text = get_database_path_from_registry();
}

void
TfrmMemberExporter::setup_database_link(
	auto_ptr<TIBDatabase> &db,
   auto_ptr<TIBTransaction> &tr,
   auto_ptr<TIBSQL> &qr)
{
	db.reset(new TIBDatabase(this));
	tr.reset(new TIBTransaction(this));

	db->DefaultTransaction = tr.get();
	tr->DefaultDatabase    = db.get();

	db->LoginPrompt = false;
	db->DatabaseName = edMenumateDatabasePath->Text;
	db->DBParamByDPB[isc_dpb_user_name] = "sysdba";
	db->DBParamByDPB[isc_dpb_password]  = "masterkey";
	db->DBParamByDPB[isc_dpb_lc_ctype]  = "NONE";

	db->Open();

   qr.reset(new TIBSQL(this));
   qr->Transaction = tr.get();
}

#pragma warn .par

void
TfrmMemberExporter::write_member_record(ofstream &stream, TIBSQL &qr)
{
	int        i = 0;
	AnsiString m;

	/* You know Unicode's really a bitch ... */
	for (int j = qr.FieldCount - 1; i < j; i++) {
		m = qr.Fields[i]->AsString;
		stream << m.c_str() << ",";
	}

	stream << *qr.Fields[i]->AsString.c_str() << "\n";
}


