//---------------------------------------------------------------------------

#ifndef DBMallsH
#define DBMallsH
//---------------------------------------------------------------------------
#endif

class TDBMalls
{
    public:
    void UpdateZ_Key();
    void SelectDataFromMallExportSettings();
    virtual void InsertDataIntoMallExportSettings() = 0;
    virtual void InsertDataIntoMallExportTransaction() = 0;
};
