//---------------------------------------------------------------------------

#ifndef ExportDataH
#define ExportDataH
//---------------------------------------------------------------------------
#endif

class TExportData
{
    public:
    void SelectDataFromDB();
    void WriteDatainToMap();
    virtual void CreateFileHeaders() = 0;
    virtual void WriteDatainToFile() = 0;
    virtual void ExportFile() = 0;
};
