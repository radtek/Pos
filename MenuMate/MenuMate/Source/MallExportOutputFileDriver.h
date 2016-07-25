//---------------------------------------------------------------------------

#ifndef MallExportOutputFileDriverH
#define MallExportOutputFileDriverH

#include "MallExportIOutput.h"
#include <fstream>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
class TMallExportOutputFileDriver : public TMallExportIOutput
{
public:
    void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI);
    void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus);

    std::fstream OutputFile;

    __fastcall TMallExportOutputFileDriver();
};
#endif
