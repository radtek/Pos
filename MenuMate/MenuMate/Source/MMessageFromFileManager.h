//---------------------------------------------------------------------------

#ifndef MMessageFromFileManagerH
#define MMessageFromFileManagerH

#include <Classes.hpp>
#include <Forms.hpp>
#include <StrUtils.hpp>

//---------------------------------------------------------------------------

class TMMessageFromFile : TObject
{
public:		// User declarations
	TMMessageFromFile();

	UnicodeString FileName;
	UnicodeString Text;
	int           Date;   // File Date
};

//---------------------------------------------------------------------------

class TMMessageFromFileManager
{
protected:
	TObjectList *_messageList;

	UnicodeString FFolder;

	UnicodeString readFolder();
	void writeFolder( UnicodeString inFolder );

	bool readMessagesInQueue();

	UnicodeString CheckFolder( UnicodeString inFolder );
	bool CheckMessagesInQueue( );

	//TMMessageFromFile* CreateMessageFromFile( UnicodeString inFileName );
	TMMessageFromFile* CreateMessageFromFile( TSearchRec inSearchRec );
	void InterChange( TMMessageFromFile *a, TMMessageFromFile *b );
	void QuickSort( TObjectList *list,int left,int right );
	UnicodeString TextInFile( UnicodeString inFileName );

public:		// User declarations
	TMMessageFromFileManager();
	__fastcall ~TMMessageFromFileManager();

	static TMMessageFromFileManager& Instance()
	{
		static TMMessageFromFileManager singleton;
        return singleton;
    }
	__property UnicodeString Folder = {read = FFolder, write = writeFolder };
	__property bool MessagesInQueue = {read = readMessagesInQueue };

   void ShowMessage();
};

#endif
