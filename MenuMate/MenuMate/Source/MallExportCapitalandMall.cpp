//---------------------------------------------------------------------------


#pragma hdrstop

#include "MallExportCapitalandMall.h"
#include "MallExportDataManager.h"
#include "GlobalSettings.h"
#include "DeviceRealTerminal.h"
#include <wininet.h>
#include <dirent.h>
//---------------------------------------------------------------------------

#pragma comment(lib, "Wininet")
#pragma package(smart_init)


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC METHODS
TMallExportCapitalandMall::TMallExportCapitalandMall() : TMallExportIMall()
{
}
//---------------------------------------------------------------------------
_fastcall TMallExportCapitalandMall::~TMallExportCapitalandMall()
{
}
//---------------------------------------------------------------------------

// Override methods
AnsiString TMallExportCapitalandMall::CreateFileURI( AnsiString inBaseURL )
{
    FFileURI = inBaseURL;
    return FFileURI;
}
//---------------------------------------------------------------------------

// Delete this method if not needed
TExportResponse TMallExportCapitalandMall::ZExport()
{
    TExportResponse result;

    //::::::::::::::::::::::::::::::::::::::

    try
    {
        UnicodeString MachineID = TGlobalSettings::Instance().BranchCode;
        UnicodeString DocType = "";
        UnicodeString LocalPath = BasePath + "\\Capitaland\\";
        UnicodeString LocalPathFileName = "";
        UnicodeString LocalFileName = "";
        int FileSerialNumber = 0;
        int FCount = 0;

        UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
        UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
        UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
        UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;

        if(TGlobalSettings::Instance().FTPPath == "")
        {
            UserPath = "/";
        }

        const char * hostName = HostName.t_str();
        const char * userPath = UserPath.t_str();
        const char * userName = UserName.t_str();
        const char * userPassword = UserPassword.t_str();

        if(TGlobalSettings::Instance().CMFileSerialNumber == 1000)
        {
            TGlobalSettings::Instance().CMFileSerialNumber = 0;
            SaveIntVariable(vmCMFileSerialNumber, TGlobalSettings::Instance().CMFileSerialNumber);
        }
        else
        {
            FileSerialNumber = TGlobalSettings::Instance().CMFileSerialNumber;
        }

        DataRead.clear();
        DataToWrite.clear();

        DocType = "D";
        DataQuery = getCapitalandQuery();
        InputManager->ReadFromDB(DataQuery,DataRead);
        CreateFileName(LocalPath, LocalPathFileName, LocalFileName, DocType, MachineID, FileSerialNumber);
        CreateOutputFormatDaily(DataRead, DataToWrite, DocType);
        TMallExportDataManager::ExportFile(OutputManager, DataToWrite, LocalPathFileName);

        TGlobalSettings::Instance().CMFileSerialNumber += 1;
        SaveIntVariable(vmCMFileSerialNumber, TGlobalSettings::Instance().CMFileSerialNumber);

//        hostName = "vault.capitaland.com.sg";
//        userName = "1300084";
//        userPassword = "vcb7gf58";
//        hostName = "deskmate.menumate.com";
//        userName = "deskmate";
//        userPassword = "1202";
//        userPath = "Menumate/Beta/Version 6/Capitaland";

        FileSubmit(hostName, userName, userPassword, userPath, LocalPathFileName, LocalFileName, FCount);


        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }
    //::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// PROTECTED METHODS

// Virtual methods
void TMallExportCapitalandMall::initTimingExportResponse( TExportResponse& outResponse )
{
    // TODO: Initiate the response for Ayala

    setExportResponse(
       false,               // Succesful = false
       emrExportNotDoneYet, // Result = emrExportNotDoneYet
       "",                  // Message
       "",                  // Description
       outResponse );
}
//---------------------------------------------------------------------------

// This function is for getting query from the DB Table
UnicodeString TMallExportCapitalandMall::getCapitalandQuery()
{
    UnicodeString CapitalandQuery = "";
    CapitalandQuery = "SELECT * FROM CAPITALAND_MALLVIEW";

    return CapitalandQuery;
}
//---------------------------------------------------------------------------

void TMallExportCapitalandMall::CreateFileName(UnicodeString &LocalPath, UnicodeString &LocalPathFileName, UnicodeString &LocalFileName, UnicodeString DocType,
                                               UnicodeString MachineID, int FileSerialNumber)
{
    UnicodeString FileSerialNumberFormat = IntToStr(FileSerialNumber);
    UnicodeString Format = ".txt";

    if(FileSerialNumberFormat.Length() == 1)
    {
        FileSerialNumberFormat = "00" + FileSerialNumberFormat;
    }
    else if (FileSerialNumberFormat.Length() == 2)
    {
        FileSerialNumberFormat = "0" + FileSerialNumberFormat;
    }

    LocalPath = CheckDir(LocalPath);
    LocalPath = CreateYearlyFolder(LocalPath);

    LocalPathFileName = LocalPath + DocType + MachineID + "." + FileSerialNumberFormat + Format;
    LocalFileName = DocType + MachineID + "." + FileSerialNumberFormat + Format;
}
//---------------------------------------------------------------------------

// This function is for saving the integer global variable
void TMallExportCapitalandMall::SaveIntVariable(vmVariables vmVar, int CompName)
{
    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.StartTransaction();
    int GlobalProfileKey = TManagerVariable::Instance().GetProfile(DBTransaction,eSystemProfiles,"Globals");
    if(GlobalProfileKey == 0)
    {
        GlobalProfileKey = TManagerVariable::Instance().SetProfile(DBTransaction,eSystemProfiles,"Globals");
    }
    TManagerVariable::Instance().SetProfileInt(DBTransaction, GlobalProfileKey, vmVar, CompName);
    DBTransaction.Commit();
}
//---------------------------------------------------------------------------
// This function is for arranging the format of the output file
TExportResponse TMallExportCapitalandMall::CreateOutputFormatDaily(std::map<UnicodeString, UnicodeString> DataRead,
                                                                   std::vector<UnicodeString> &DataToWrite,
                                                                   UnicodeString DocType)
{
    TExportResponse result;

    try
    {
        int i = 0;
        UnicodeString OutputValue = "";
        UnicodeString Zeroes = "";

        OutputValue = DocType;
        DataToWrite.push_back(OutputValue.t_str());

        std::map<UnicodeString, UnicodeString>::iterator it;

        for(i=0;i<DataRead.size();i++)
        {
            switch(i)
            {
                case 0:
                    it = DataRead.find("MallCode");
                    break;
                case 1:
                    it = DataRead.find("TransactionDate");
                    break;
                case 2:
                    it = DataRead.find("TotalGrossSale");
                    break;
                default:
                    break;
            }

            if(i==1)
            {
                OutputValue = GetDate((it->second), DocType);
            }
            else if(i==2)
            {
                if(((it->second) == 0) || ((it->second) == ""))
                {
                    OutputValue = "0.00";
                }

                Zeroes = "";
                for(int j=0;j<(11-(it->second).Length());j++)
                {
                    Zeroes += "0";
                }

                OutputValue = Zeroes + (it->second);
            }
            else
            {
                OutputValue = (it->second);
            }

            DataToWrite.push_back(OutputValue.t_str());
        }

        setExportResponse(
           true,               // Succesful = false
           emrExportSuccesful, // Result = emrExportSuccesful
           "",                 // Message
           "",                 // Description
           result );
    }
    catch( Exception& exc )
    {
        setExportResponse(
           false,           // Succesful = false
           emrExportFailed, // Result = emrExportFailed
           "",              // Message
           "",              // Description
           result );
    }

    return result;
}
//---------------------------------------------------------------------------

// Get Date format
UnicodeString TMallExportCapitalandMall::GetDate(UnicodeString DateTime, UnicodeString DocType)
{
    AnsiString currentdate = "";

    try
    {
        TDateTime date(DateTime);
        if(DocType == "D")
        {
            currentdate = date.FormatString("yyyymmdd");
        }
        else
        {
            currentdate = date.FormatString("yyyymm");
        }
    }
    catch(Exception &ex)
    {
        //ignore the exception as this is invalid datetime string
    }

    return UnicodeString( currentdate );
}
//---------------------------------------------------------------------------

// This function is for checking the specified path
UnicodeString TMallExportCapitalandMall::CheckDir(UnicodeString Path)
{
    if(!DirectoryExists(Path))
    {
        Path = BasePath;
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
        Path = Path + "\\Capitaland\\";
        if(!DirectoryExists(Path))
        {
            CreateDir(Path);
        }
    }
    return Path;
}
//---------------------------------------------------------------------------

// This function is used to create yearly folder
UnicodeString TMallExportCapitalandMall::CreateYearlyFolder(UnicodeString Path)
{
    UnicodeString Year = Now().FormatString("yyyy");

    Path = Path + Year;
    if(!DirectoryExists(Path))
    {
        CreateDir(Path);
    }

    return Path + "\\";
}
//---------------------------------------------------------------------------

// This method is used to send the file from the local directory to the FTP server
// provided by the client
void TMallExportCapitalandMall::FileSubmit(const char * hostName, const char * userName,
                                           const char * userPassword, const char * userPath,
                                           UnicodeString LocalPathFileName, UnicodeString LocalFileName,
                                           int FCount)
{
    const char * pathFileName = LocalPathFileName.t_str();
    const char * fileName = LocalFileName.t_str();

    HINTERNET hInternet;
    HINTERNET hFtpSession;
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        if (FCount == 0)
        {
            MessageBox( "Connection to FTP Server failed. Please check internet connection", "Connection Failed!", MB_OK );
        }
    }
    else
    {
        hFtpSession = InternetConnect(hInternet, hostName, INTERNET_DEFAULT_FTP_PORT, userName, userPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
        if (hFtpSession == NULL)
        {
            if (FCount == 0)
            {
                MessageBox( "Cannot connect to FTP Server. Please try again!", "Connection to FTP Host Unsuccessful.", MB_OK );
            }
        }
        else
        {
            // Changing dicrectory is necessary
            FtpSetCurrentDirectory( hFtpSession, userPath );

            if (!FtpPutFile(hFtpSession, pathFileName, fileName, FTP_TRANSFER_TYPE_BINARY, 0))
            {
                if (FCount == 0)
                {
                    MessageBox( "File was not successfully uploaded!", "File Transfer Failed!", MB_OK );
                }
            }

            if (FCount == 0)
            {
                MessageBox( "File was sent successfully!", "File Transfer Success!", MB_OK );
            }
        }
        InternetCloseHandle(hFtpSession);
    }
}
//---------------------------------------------------------------------------

// This is used for resending the data that can be performed by the user in the settings
// This method generate all the export files that is located in the backup folder
void TMallExportCapitalandMall::GetReportsList(TStringList* reportsList, UnicodeString folder)
{
    UnicodeString LocalPath = "";
    std::vector<UnicodeString> files;

    if(folder == "")
    {
        LocalPath = BasePath + "\\Capitaland\\";
    }
    else
    {
        LocalPath = BasePath + "\\Capitaland\\" + folder + "\\";
    }

    DIR* dir = opendir(LocalPath.t_str());

    std::string getName;
    UnicodeString fileExt = "";

    while ( (entry = readdir(dir)) != NULL)
    {
        files.push_back(std::string(entry->d_name).c_str());
    }

    for (int i = 0;i < files.size();i++)
    {
        getName = files.at(i).t_str();
        if((files.at(i) == ".") || (files.at(i) == ".."))
        {
            continue;
        }
        else if(((files.at(i)).Length() > 2) && ((files.at(i)).Length() < 5))
        {
            reportsList->Add(files.at(i).t_str());
        }
        else
        {
            for (int i = getName.length() - 3 ; i<getName.length(); i++)
            {
                fileExt = fileExt + getName[i];
            }
            if((fileExt == "txt") || (fileExt == "TXT"))
            {
                reportsList->Add(files.at(i).t_str());
            }
            fileExt = "";
        }
    }
}
//---------------------------------------------------------------------------

// This method is used to resend the reports, it when the user select a file/multiple file
// the system will check if that files does exists in the backup data and then send it to
// the mall path if the path is available
void TMallExportCapitalandMall::ResendReports(TStringList* resendReports, UnicodeString folder, bool status)
{
    UnicodeString LocalPath = BasePath + "\\Capitaland\\" + folder + "\\";
    UnicodeString FTPFile = "";
    UnicodeString LocalPathFiles = "";
    UnicodeString HostName = TGlobalSettings::Instance().FTPServer;
    UnicodeString UserPath = TGlobalSettings::Instance().FTPPath;
    UnicodeString UserName = TGlobalSettings::Instance().FTPUserName;
    UnicodeString UserPassword = TGlobalSettings::Instance().FTPPassword;
    const char * hostName = HostName.t_str();
    const char * userPath = UserPath.t_str();
    const char * userName = UserName.t_str();
    const char * userPassword = UserPassword.t_str();

    if(status == false)
    {
        if(folder.Length() != 0)
        {
            for(int i=0; i<resendReports->Count;i++)
            {
                FTPFile = resendReports->Strings[i];
                LocalPathFiles = LocalPath + resendReports->Strings[i];

                FileSubmit(hostName, userName, userPassword, userPath, LocalPathFiles, FTPFile, i);
            }
        }
    }
}
//---------------------------------------------------------------------------

// This method checks the Local Path if it is already exists or not
// This is used in resend function
bool TMallExportCapitalandMall::CheckBackUpPath()
{
    UnicodeString LocalPath = BasePath + "\\Capitaland\\";

    return (DirectoryExists(LocalPath)) ? true : false;
}
//---------------------------------------------------------------------------

