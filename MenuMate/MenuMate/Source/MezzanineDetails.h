//---------------------------------------------------------------------------

#ifndef MezzanineDetailsH
#define MezzanineDetailsH
//---------------------------------------------------------------------------

enum eTableMode {eNormalNode, eMizzanineMode};
enum eTableSelectionMode {eSelected, eDeSelected};

class TMezzanineTable
{
    public:
    eTableSelectionMode SelectionType;
    int FloorplanVer;
};
#endif
