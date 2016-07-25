// ---------------------------------------------------------------------------

#ifndef OnlineManagerH
#define OnlineManagerH

#include <memory>
#include "VerticalSelect.h"
#include "ftp.h"

// ---------------------------------------------------------------------------
enum TftpControl
{
	ftpDir, ftpFile, ftpQuit, ftpUnknown
};

class TOnlineManager
{
private:
	bool ftpConnect(AnsiString Password);
	void ftpDisconnect();
	void ftpGetCurrentDir(TfrmVerticalSelect *SelectionForm);
	bool ftpGetFile(AnsiString File);
	TftpControl ftpSelectDirOrFile();
	void ftpChangeDir(AnsiString Dir);

	void __fastcall ftpStatus(System::TObject* ASender, const TIdStatus AStatus, const System::UnicodeString AStatusText);
	void __fastcall ftpWork(System::TObject* ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	void __fastcall ftpGetWorkEnd(TObject *Sender, TWorkMode AWorkMode);
	std::auto_ptr <TfrmFTP> frmFTP;

public:
	TOnlineManager();
	virtual ~TOnlineManager();
	AnsiString GetUpdateFile(AnsiString Password);
	std::auto_ptr <TIdFTP> ftp;

	TftpControl SelectionType;
	AnsiString Selection;
	AnsiString SelectionFileSize;
};
#endif
