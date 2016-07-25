//---------------------------------------------------------------------------

#ifndef MallExportIInputH
#define MallExportIInputH

#include <Classes.hpp>
#include <map>
//---------------------------------------------------------------------------
class TMallExportIInput
{
public:
    virtual void ReadFromDB(UnicodeString DataQuery, std::map<UnicodeString, UnicodeString> &DataRead)=0;
};
#endif
