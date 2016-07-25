//---------------------------------------------------------------------------

#include <memory>
#include <stdio.h>
#include <vcl.h>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>


#pragma hdrstop

//---------------------------------------------------------------------------
#pragma argsused

void CreateDirStructure();
void ListDir(TIdFTP *ftp,TStringList *AFiles);


int main(int argc, char* argv[])
{
	CreateDirStructure();
	return 0;
}
//---------------------------------------------------------------------------
void Log(AnsiString Text)
{
	printf(AnsiString(Text + "\n\r").c_str());
}

AnsiString MMGetEnvironmentVariable(AnsiString Variable)
{
	char Buffer[255];
	int Size=0;
	ZeroMemory(&Buffer,sizeof(Buffer));
	Size = GetEnvironmentVariable(Variable.c_str(),	// address of environment variable name
											Buffer,	// address of buffer for variable value
											sizeof(Buffer) 	// size of buffer, in characters
										  );

	return AnsiString(Buffer,Size);
}

void ListDir(TIdFTP *ftp,TStringList *AFiles)
{
	try
	{
		AFiles->Clear();
		ftp->List(AFiles,"", false);
	}
	catch(Exception &E)
	{
		Log(E.Message);
	}
}

void FTPCreateDir(TIdFTP *ftp,AnsiString ADir)
{
	try
	{
		ftp->MakeDir(ADir);
		Log("Created " + ADir + " Directory");
	}
	catch(Exception &E)
	{
		Log(E.Message);
	}
}

void FTPUpload(TIdFTP *ftp,AnsiString AFile)
{
	try
	{
		TSearchRec File;
		if(FindFirst(AFile, faAnyFile, File) == 0)
		{
			do
			{
				std::auto_ptr<TFileStream> file(new TFileStream(ExtractFilePath(AFile) + File.Name,fmOpenRead));
				ftp->Put(file.get() ,File.Name,false);
				if(ftp->SupportsVerification)
				{
					Log("Verifing " + File.Name);
					if(ftp->VerifyFile(file.get() ,File.Name,false))
					{
						Log(File.Name + "[Ok]");
					}
					else
					{
						Log(File.Name + "[Fail]");
               }
				}
			} while(FindNext(File) == 0);
		}
		FindClose(File);
		Log("Uploaded " + AFile);
	}
	catch(Exception &E)
	{
		Log(E.Message);
	}
}

void CreateDirStructure()
{
	try
	{
		AnsiString TagLabel = MMGetEnvironmentVariable("TAGLABEL");
		AnsiString VerLabel = MMGetEnvironmentVariable("VERLABEL");
		AnsiString Version = MMGetEnvironmentVariable("MMVERSION");
		AnsiString BaseDir = MMGetEnvironmentVariable("BASEDIR");

		AnsiString VersionMajor = "";

		if(VerLabel == "" || Version == "" || TagLabel =="")
		{
			printf("Tag/Version Info Unavailable.. Exiting");
			return;
		}

		int FirstSectionEnd = Version.Pos(".")-1;
		AnsiString MajorVersion = Version.SubString(1,FirstSectionEnd);
		AnsiString RestOfStr = Version.SubString(Version.Pos(".")+1,Version.Length());
		int SecondSectionEnd = RestOfStr.Pos(".")-1;
		if(SecondSectionEnd <= 0) SecondSectionEnd = RestOfStr.Length();
		AnsiString MinorVersion = RestOfStr.SubString(1,SecondSectionEnd);
		VersionMajor = MajorVersion + "." + MinorVersion;

		Log("Connecting to.. ftp.menumate.com");
		std::auto_ptr<TIdFTP> ftp(new TIdFTP(NULL));
		ftp->Host = "ftp.menumate.com";
		ftp->Passive = false;
		ftp->TransferType = ftBinary;
		ftp->Username = "beta";
		ftp->Password = "dab0mb";
		ftp->AutoLogin = true;

		ftp->Connect();
		Log("Connected");

		ftp->TransferMode(dmStream);

		std::auto_ptr<TStringList> AFiles(new TStringList);

		ListDir(ftp.get(),AFiles.get());

		if(AFiles->IndexOf("beta") == -1)
		{
			FTPCreateDir(ftp.get(),"beta");
			Log("Creating Directory beta");
		}

		ftp->ChangeDir("beta");

		ListDir(ftp.get(),AFiles.get());

		ListDir(ftp.get(),AFiles.get());

		if(AFiles->IndexOf(VerLabel) == -1)
		{
			FTPCreateDir(ftp.get(),VerLabel);
			Log("Creating Directory " + VerLabel);
		}
		ftp->ChangeDir(VerLabel);


		ListDir(ftp.get(),AFiles.get());

		/* Theres a bug in the indy component that means after doing an list
		it gets stuck in acsii mode, this fixes it. */
		ftp->Disconnect();
		ftp->TransferType = ftBinary;
		ftp->Connect();
		ftp->TransferType = ftBinary;
		ftp->ChangeDir("beta");
		ftp->ChangeDir(VerLabel);

		FTPUpload(ftp.get(),BaseDir+"\\Patch\\MMUpdate*.*");

		FTPUpload(ftp.get(),BaseDir+"\\Patch\\MMSetup*.*");

		ftp->Quit();
		Log("FTP Quit");
	}
	catch(Exception &E)
	{
		Log(E.Message);
	}
}
