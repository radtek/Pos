//---------------------------------------------------------------------------

#ifndef FolderManagerH
#define FolderManagerH
//---------------------------------------------------------------------------

#include <vcl.h>
#include "RemoteHostManager.h"

//---------------------------------------------------------------------------

enum TFolderManagerError { fmNO_ERROR, fmFAILED_TO_CONNECT_TO_REMOTE_HOST, fmREMOTE_FOLDER_NOT_FOUND, fmLOCAL_FOLDER_NOT_FOUND };

class TFolderManager
{
	public:
		static TFolderManager& Instance()
		{
			static TFolderManager singleton;
			return singleton;
		}

		TFolderManager();
		~TFolderManager();

		void SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword );
		void Validate();

		__property AnsiString HostName   = { read = FHostName,   write = writeHostName   };
		__property AnsiString FolderName = { read = FFolderName, write = writeFolderName };
		__property AnsiString FullPath   = { read = readFullPath };

		// Username and Password are ignored if the HostName is local (localhost or 127.0.0.1)
		__property AnsiString UserName   = { read = FUserName, write = writeUserName };
		__property AnsiString Password   = { read = FPassword, write = writePassword };

		__property TFolderManagerError LastError    = { read = FLastError    };
		__property AnsiString          LastErrorMsg = { read = FLastErrorMsg };

	protected:
		TRemoteHostManager *FRemoteHostManager;

		AnsiString FHostName;
		AnsiString FFolderName;

		AnsiString FUserName;
		AnsiString FPassword;

		TFolderManagerError FLastError;
		AnsiString          FLastErrorMsg;

		void ValidateLocal();
		void ValidateRemote();

		void writeHostName(   AnsiString inHostName   );
		void writeFolderName( AnsiString inFolderName );

		void writeUserName( AnsiString inUserName );
		void writePassword( AnsiString inPassword );

		AnsiString readFullPath();
		AnsiString readLocalFullPath();
		AnsiString readRemoteFullPath();

		AnsiString ResolveHostName( AnsiString inHostName );

		bool IsLocalHost( AnsiString inHostName );
		AnsiString LocalHost();

		bool ConnectToRemoteHost();

		TFolderManagerError CheckLocalFolder();
		TFolderManagerError CheckRemoteFolder();

		void SetLastError( TFolderManagerError inError);
};

//---------------------------------------------------------------------------
#endif
