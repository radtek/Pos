//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMessageFromFileManager.h"

#include "ManagerNet.h"
#include "NetMessageMsg.h"
#include "DeviceRealTerminal.h"

#include "MMMessageBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TMMessageFromFile::TMMessageFromFile() : TObject()
{
	FileName = "";
	Text     = "";
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

TMMessageFromFileManager::TMMessageFromFileManager()
{
	_messageList = new TObjectList();

	FFolder = ExtractFilePath( Application->ExeName );
}
//...........................................................................

__fastcall TMMessageFromFileManager::~TMMessageFromFileManager()
{
	delete _messageList;
}
//...........................................................................

//...........................................................................

void TMMessageFromFileManager::writeFolder( UnicodeString inFolder )
{
	FFolder = CheckFolder( inFolder );
}
//...........................................................................

bool TMMessageFromFileManager::readMessagesInQueue()
{
   return CheckMessagesInQueue();
}
//...........................................................................

UnicodeString TMMessageFromFileManager::CheckFolder( UnicodeString inFolder )
{
	UnicodeString result = inFolder;

	//...........................................

	try
	{
		if( !DirectoryExists( result ) )
		{
			CreateDir( result );
		}
	}
	catch(...)
	{
		result = ExtractFilePath( Application->ExeName );
	}

	//...........................................

	return result;
}
//...........................................................................

bool TMMessageFromFileManager::CheckMessagesInQueue( )
{
	if( _messageList->Count == 0 )
	{
		TSearchRec sr;
		int iAttributes = faAnyFile;

		if( FindFirst( FFolder + "\\*.csv", iAttributes, sr ) == 0 )
		{
			if( sr.Attr & iAttributes )
			{
                if( UpperCase(sr.Name) != "MEMBERS.CSV")   // prevent displaying members.csv file's content
    				_messageList->Add( ( TObject * )CreateMessageFromFile( sr ) );
			}
			while( FindNext( sr ) == 0 )
			{
			   if( sr.Attr & iAttributes )
			   {
                    if( UpperCase(sr.Name) != "MEMBERS.CSV" ) // prevent displaying members.csv file's content
    					_messageList->Add( ( TObject * )CreateMessageFromFile( sr ) );
			   }
            }
		}
		FindClose(sr);

		QuickSort( _messageList, 0, _messageList->Count - 1 );
	}

	return _messageList->Count > 0;
}
//...........................................................................

TMMessageFromFile* TMMessageFromFileManager::CreateMessageFromFile( TSearchRec inSearchRec )
{
	 TMMessageFromFile* result = new TMMessageFromFile();

	 //.............................................

	 int date = inSearchRec.Time;
	 UnicodeString fileName = FFolder + "\\"  + inSearchRec.Name;

	 result->FileName = fileName;
	 result->Text     = TextInFile( fileName );
	 result->Date     = date;

	 //.............................................

	 return result;
}
//...........................................................................

void TMMessageFromFileManager::InterChange( TMMessageFromFile *a, TMMessageFromFile *b )
{
	TMMessageFromFile *tmp;

	UnicodeString fileName = a->FileName;
	UnicodeString text     = a->Text;
	int date               = a->Date;

	a->FileName = b->FileName;
	a->Text     = b->Text;
	a->Date     = b->Date;

	b->FileName = fileName;
	b->Text     = text;
	b->Date     = date;
}
//...........................................................................

void TMMessageFromFileManager::QuickSort( TObjectList *list,int left,int right )
{
	if( left < right )
	{
		int i = left, j = right + 1;
		TMMessageFromFile *pivot = ( TMMessageFromFile* )list->Items[left];

		do
		{
			do
			{
				i++;
			}
			while( ( ( TMMessageFromFile* )list->Items[i] )->Date < pivot->Date );

			do
			{
				j--;
			}
			while( ( ( TMMessageFromFile* )list->Items[j] )->Date > pivot->Date );

			if( i < j )
            {
				InterChange( ( TMMessageFromFile* )list->Items[i], ( TMMessageFromFile* )list->Items[j] );
            }
		}
		while( i < j );

		InterChange( ( TMMessageFromFile* )list->Items[left], ( TMMessageFromFile* )list->Items[j] );

		QuickSort( list, left, j-1 );
		QuickSort( list, j+1, right );
	}
}
//...........................................................................

UnicodeString TMMessageFromFileManager::TextInFile( UnicodeString inFileName )
{
	TStringList *stringList = new TStringList();
	stringList->LoadFromFile( inFileName );

	UnicodeString result = stringList->DelimitedText;

	delete stringList;

	return result;
}
//---------------------------------------------------------------------------

void TMMessageFromFileManager::ShowMessage()
{
	try
	{
		TMMessageFromFile* msgFromFile = ( TMMessageFromFile * )_messageList->Items[0];

		TNetMessageMsg *ReqMessage = new TNetMessageMsg;
		ReqMessage->Device = TDeviceRealTerminal::Instance().ID.Name;
		ReqMessage->User = TDeviceRealTerminal::Instance().User.Name;
		ReqMessage->Message = msgFromFile->Text;
		try
		{
			TDeviceRealTerminal::Instance().ManagerNet->SendToAll(ReqMessage);

			MessageBox( msgFromFile->Text, "Message", MB_OK );

			DeleteFile( msgFromFile->FileName );
			_messageList->Remove( ( TObject* )msgFromFile );
		}
		__finally
		{
			delete ReqMessage;
		}
	}
	catch( ... )
	{

    }
}
//---------------------------------------------------------------------------

