#ifndef FormMembershipExporterH
#define FormMembershipExporterH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <ComCtrls.hpp>
#include <map>
#include <memory>
#include <fstream>

#define REG_KEY_MENUMATE_DATABASE "SOFTWARE\\IQWorks\\Menumate\\Database"

class TfrmMemberExporter : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *pbExportProgress;
	TButton *btnExport;
	TEdit *edMenumateDatabasePath;
	TLabel *lblPathToMenumateDatabase;
	void __fastcall initialize(TObject *Sender);
	void __fastcall export_membership_data(TObject *Sender);

private:	// User declarations
	UnicodeString get_database_path_from_registry();
	bool          get_output_csv_path(UnicodeString &destination);
   int           get_string_value_from_registry(UnicodeString &destination,
                                                const char *value_name,
	                                             HKEY key);
   void          handle_error(unsigned int error_code);
	void          retrieve_and_write_membership_data(ofstream &stream,
	                                                 TIBTransaction &tr,
                                                    TIBSQL &qr);
	void          setup_database_link(std::auto_ptr<TIBDatabase> &db,
                                     std::auto_ptr<TIBTransaction> &tr,
	                                  std::auto_ptr<TIBSQL> &qr);
	void          write_member_record(ofstream &stream, TIBSQL &qr);

	std::map<unsigned int, UnicodeString> error_map;

public:		// User declarations
	__fastcall TfrmMemberExporter(TComponent* Owner);
};

extern PACKAGE TfrmMemberExporter *frmMemberExporter;

#endif
