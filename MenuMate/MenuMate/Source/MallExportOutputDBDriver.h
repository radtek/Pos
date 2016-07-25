//---------------------------------------------------------------------------

#ifndef MallExportOutputDBDriverH
#define MallExportOutputDBDriverH

#include "MallExportIOutput.h"
#include "ManagerVariable.h"
#include <fstream>
#include <string>
#define SMMALL 1
#define ROBINSONMALL 2
#define AYALAMALL 3
#define POWERPLANTMALL 4
//---------------------------------------------------------------------------
class TMallExportOutputDBDriver : public TMallExportIOutput
{
public:
    void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI);
    void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus);
    void CopyOutputContent(AnsiString FileURI, AnsiString TempText);
    void SaveVariableInt(vmVariables vmVar, int CompName);

    std::fstream OutputFile;
    std::fstream TempFile;

    std::string line;
    void GenerateDBF(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus);
    void GenerateTXT(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus);

    __fastcall TMallExportOutputDBDriver();
};
#endif
