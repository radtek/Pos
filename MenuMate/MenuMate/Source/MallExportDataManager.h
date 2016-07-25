//---------------------------------------------------------------------------

#ifndef MallExportDataManagerH
#define MallExportDataManagerH

#include "MallExportIOutput.h"
#include <Classes.hpp>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
class TMallExportDataManager
{
public:

    static void ExportFile(TMallExportIOutput* IOutput, std::vector<UnicodeString> DataToWrite, UnicodeString FileURI);
    static void ExportFile(TMallExportIOutput* IOutput, std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus);

    __fastcall TMallExportDataManager();
};
#endif
