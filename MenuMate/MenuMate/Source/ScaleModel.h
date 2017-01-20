// ---------------------------------------------------------------------------

#ifndef ScaleModelH
#define ScaleModelH

#include "SystemEvents.h"
#include "MM_DBCore.h"
#include "WeighScale.h"
#include <CPort.hpp>

#include <list>

// ---------------------------------------------------------------------------
enum TWSCScaleStreamType
{
    TypeA_STDStream,        // Wedderburn scales
    TypeC_QUQA,             // Towa scales
    Type_AveryFxStream,      // Avery Berkel FX Series
    Type_KenkoKK300W        //Kenko KK-300W
};

class TScaleModel : public TWeighScale
{
private:
    TComPort *Port;

    void __fastcall GetData(TObject *Sender, int Count);
    void __fastcall SetData(AnsiString Data);

    bool Busy;
    bool CloseingPort;
    bool OpeningPort;

    void __fastcall PortTxEmpty(TObject *Sender);

    int StablilityCount;

    std::list<PAsync>writeAsyncPtrList;

public:
    int PortNumber;
    TWSCScaleStreamType ScaleStreamType;
    bool AsyncMode;
    AnsiString LastData;
    AnsiString LastError;

    TScaleModel();
    virtual ~TScaleModel();

    void Initialise(Database::TDBTransaction &DBTransaction);
    void SaveSettings(Database::TDBTransaction &DBTransaction);

    bool Open(int PortNum);
    void Close();
    TComPort *GetTComPort();
    int Counter;
};
#endif
