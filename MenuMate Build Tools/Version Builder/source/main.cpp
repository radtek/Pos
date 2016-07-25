//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop

#include <StrUtils.hpp>
#include <SysUtils.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma argsused

void UpdateScript(AnsiString Version,AnsiString VerLabel,AnsiString Path,AnsiString File)
{
	if(FileExists(File))
	{
		TStringList *Data = new TStringList;
		try
		{
			Data->LoadFromFile(File);
			for (int i = 0; i < Data->Count ; i++)
			{
				AnsiString Line = Data->Strings[i];
				if(Line.Pos("OutputBaseFilename=") != 0)
				{
					Version = AnsiReplaceStr(Version, ".","-");
					Data->Strings[i] = "OutputBaseFilename=MMSetup " + VerLabel + Version;
				}

				if(Line.Pos("AppVerName=") != 0)
				{
					Version = AnsiReplaceStr(Version, "-",".");
					Data->Strings[i] = "AppVerName=MenuMate " + VerLabel + Version;
				}

				if(Line.Pos("AppVersion=") != 0)
				{
					Version = AnsiReplaceStr(Version, "-",".");
					Data->Strings[i] = "AppVersion=" + Version;
				}
			}
			Data->SaveToFile(File);
		}
		__finally
		{
			delete Data;
		}
	}
}

AnsiString ExtractVersion(AnsiString VersionFile)
{
	AnsiString Version = "Unknown Version";
	if(FileExists(VersionFile))
	{
		TStringList *Data = new TStringList;
		try
		{
			Data->LoadFromFile(VersionFile);
			for (int i = 0; i < Data->Count ; i++)
			{
				AnsiString Line = Data->Strings[i];
				if(Line.Pos(" FILEVER") != 0)
				{
					Version = Line.SubString( Line.Pos("FILEVER") + 7,Line.Length() );
					Version = Version.Trim();
					Version = AnsiReplaceStr(Version, ",","-");
				}
			}
		}
		__finally
		{
			delete Data;
		}
	}
	return Version;
}

void UpdateRar(AnsiString Version,AnsiString VerLabel,AnsiString Path,AnsiString File)
{
	if(FileExists(File))
	{
		const AnsiString RarStart = "\"C:\\Program Files\\WinRar\\Rar.exe\" a -o+ -m5 -sfx -ep -ep1 \"MenuMate" + VerLabel +"Patch ";
		const AnsiString RarEnd = "\" @" + ExtractFilePath(Path) + "CompressionList.txt";

		TStringList *Data = new TStringList;
		try
		{
			Data->LoadFromFile(File);
			for (int i = 0; i < Data->Count ; i++)
			{
				AnsiString Line = Data->Strings[i];
				if(Line.Pos("Rar.exe") != 0)
				{
					Data->Strings[i] = RarStart + Version + RarEnd;
				}
			}
			Data->SaveToFile(File);
		}
		__finally
		{
			delete Data;
		}
	}
}

void UpdateEnviroment(AnsiString Version,AnsiString VerLabel,AnsiString Path,AnsiString File)
{
	if(FileExists(File))
	{
		TStringList *Data = new TStringList;
		try
		{
			Data->LoadFromFile(File);
			bool Found = false;
			for (int i = 0; i < Data->Count ; i++)
			{
				AnsiString Line = Data->Strings[i];
				if(Line.Pos(VerLabel) != 0)
				{
					Data->Strings[i] = "SET " + VerLabel +"="+ Version;
					Found = true;
				}
			}
			if(!Found)
			{
				Data->Add("SET " + VerLabel +"="+ Version);
			}
			Data->SaveToFile(File);
		}
		__finally
		{
			delete Data;
		}
	}
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


int main(int argc, char* argv[])
{
	// Files to adjust
	AnsiString Version = "Unknown Version";
	AnsiString TAGLabel = MMGetEnvironmentVariable("TAGLABEL");
	//if(VerLabel == "") VerLabel = "Unknown";
	// Retrive the Version Number
	AnsiString Path = ExtractFilePath(argv[0]);
	Version = ExtractVersion(Path + "\\"+ TAGLabel + "\\MenuMate\\Version\\VersionRes.h");

	AnsiString MMVersionLabel = "MMVERSION";

	UpdateEnviroment(Version,MMVersionLabel,Path,Path + "Enviro.bat");
/*	// Set Patch File Name.
	UpdateRar(Version,VerLabel,Path,Path + "BuildPatch.bat");
	UpdateScript(Version,VerLabel,Path,Path + "MMInstallScript.iss");
	UpdateScript(Version,VerLabel,Path,Path + "MMUpdateScript.iss");*/

	return 0;
}
//---------------------------------------------------------------------------


