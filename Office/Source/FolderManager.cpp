//---------------------------------------------------------------------------


#pragma hdrstop

#include "FolderManager.h"

//#include <Winnetwk.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

TFolderManager::TFolderManager()
{
	FRemoteHostManager = new TRemoteHostManager();

	SetAndValidate( LocalHost(), ExtractFilePath( Application->ExeName ), "", "" );
}
//---------------------------------------------------------------------------
TFolderManager::~TFolderManager()
{
	delete FRemoteHostManager;
}
//---------------------------------------------------------------------------
void TFolderManager::SetAndValidate( AnsiString inHostName, AnsiString inFolderName, AnsiString inUserName, AnsiString inPassword )
{
	HostName   = inHostName;
	FolderName = inFolderName;
	UserName   = inUserName;
	Password   = inPassword;

	Validate();
}
//---------------------------------------------------------------------------
void TFolderManager::Validate()
{
	if( IsLocalHost( FHostName ) )
	{
		ValidateLocal();
	}
	else
	{
		ValidateRemote();
	}
}
//---------------------------------------------------------------------------
void TFolderManager::ValidateLocal()
{
	TFolderManagerError tmpError = CheckLocalFolder();

	SetLastError( tmpError );
}
//---------------------------------------------------------------------------
void TFolderManager::ValidateRemote()
{
	TFolderManagerError tmpError = fmNO_ERROR;

	if( ConnectToRemoteHost() )
	{
		tmpError = CheckRemoteFolder();
	}
	else
	{
		tmpError = fmFAILED_TO_CONNECT_TO_REMOTE_HOST;
	}

	SetLastError( tmpError );
}
//---------------------------------------------------------------------------
void TFolderManager::writeHostName( AnsiString inHostName )
{
	FHostName = ResolveHostName( inHostName );
}
//---------------------------------------------------------------------------
void TFolderManager::writeFolderName( AnsiString inFolderName )
{
	FFolderName = inFolderName;
}
//---------------------------------------------------------------------------
void TFolderManager::writeUserName( AnsiString inUserName )
{
	FUserName = inUserName;
}
//---------------------------------------------------------------------------
void TFolderManager::writePassword( AnsiString inPassword )
{
	FPassword = inPassword;
}
//---------------------------------------------------------------------------
AnsiString TFolderManager::readFullPath()
{
	return ( FHostName == LocalHost() ) ? readLocalFullPath() : readRemoteFullPath();
}
//---------------------------------------------------------------------------
AnsiString TFolderManager::readLocalFullPath()
{
	return FFolderName;
}
//---------------------------------------------------------------------------
AnsiString TFolderManager::readRemoteFullPath()
{
	return "\\\\" + FHostName + "\\" + FFolderName;
}
//---------------------------------------------------------------------------
AnsiString TFolderManager::ResolveHostName( AnsiString inHostName )
{
	return FRemoteHostManager->ResolveHostName( inHostName );
}
//---------------------------------------------------------------------------
bool TFolderManager::IsLocalHost( AnsiString inHostName )
{
	return FRemoteHostManager->IsLocalHost( inHostName );
}
//---------------------------------------------------------------------------
AnsiString TFolderManager::LocalHost()
{
	return FRemoteHostManager->LocalHost();
}
//---------------------------------------------------------------------------
bool TFolderManager::ConnectToRemoteHost()
{
	FRemoteHostManager->SetAndValidate( FHostName, FUserName, FPassword );

	return FRemoteHostManager->LastError == rhmNO_ERROR;
}
//---------------------------------------------------------------------------
TFolderManagerError TFolderManager::CheckLocalFolder()
{
	return DirectoryExists( readFullPath() ) ? fmNO_ERROR : fmLOCAL_FOLDER_NOT_FOUND;
}
//---------------------------------------------------------------------------
TFolderManagerError TFolderManager::CheckRemoteFolder()
{
	//return fmNO_ERROR;

	//AnsiString fullPath = readFullPath();
	//return DirectoryExists( fullPath ) ? fmNO_ERROR : fmREMOTE_FOLDER_NOT_FOUND;

	return DirectoryExists( readFullPath() ) ? fmNO_ERROR : fmREMOTE_FOLDER_NOT_FOUND;
}
//---------------------------------------------------------------------------
void TFolderManager::SetLastError( TFolderManagerError inError)
{
	FLastError = inError;

	switch( FLastError )
	{
	   case fmNO_ERROR                        : FLastErrorMsg = "No error"; break;
	   case fmFAILED_TO_CONNECT_TO_REMOTE_HOST: FLastErrorMsg = "Failed to connect to remote host"; break;
	   case fmREMOTE_FOLDER_NOT_FOUND         : FLastErrorMsg = "Remote folder not found";  break;
	   case fmLOCAL_FOLDER_NOT_FOUND          : FLastErrorMsg = "Local folder not found"; break;

	   default: FLastErrorMsg = "Unknown error"; break;
	}
}
//---------------------------------------------------------------------------

