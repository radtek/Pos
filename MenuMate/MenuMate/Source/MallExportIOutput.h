//---------------------------------------------------------------------------

#ifndef MallExportIOutputH
#define MallExportIOutputH

#include <Classes.hpp>
#include <vector>
#include <map>
//---------------------------------------------------------------------------
class TMallExportIOutput
{
public:
    virtual void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI)=0;
    virtual void WriteToFile(std::vector<UnicodeString> DataToWrite, UnicodeString FileURI, UnicodeString FileStatus)=0;
};
#endif
