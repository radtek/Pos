//---------------------------------------------------------------------------

#ifndef MallExportInterfaceH
#define MallExportInterfaceH
//---------------------------------------------------------------------------
class TMallExportInterface
{
    public:
    virtual bool PushToDatabase() = 0;
    virtual bool Export() = 0;
}
#endif
