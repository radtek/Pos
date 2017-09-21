#ifndef PlanControllerH
#define PlanControllerH

#include <winsock2.h>
#include <vector>
#include <iterator>
#include <map>

#include "PosIntegration.h"
#include "MM_DBCore.h"

namespace TablePlan
{
class PlanController
{
private:
    TPoint _currentMouseLocation;
    int _idxCurrentPlan;
    TRect BackGndRect;
    bool _connected;
    void ClearView();
    void SetColorAsPerNoServiceTime(Database::TDBTransaction &DBTransaction,int TableNumber) ;
public:
    PlanController();
    ~PlanController();
    TImage *image;
    std::map<int,DTOLocation*> locations;
    void SetView(TImage *v);
    void DrawCurrentPlan(Database::TDBTransaction &DBTransaction, bool isNormalArea = true);
    AnsiString GetCurrentPlanName();
    DTOReservable *GetTable(int LocationID , int TableNo);
    DTOReservable *GetCurrentTable();
    AnsiString GetTableDesc();
    int PlanCount();
    int CurrentID();
    void FirstPlan();
    void SetLocation(int LocationIndex);
    std::vector<DTOLocation*> getLocations();
    TPoint CurrentMouseLocation();
    void UpdateMousePos(int, int);
    bool IsInitOk();
    bool Init();
    _di_IPosIntegration _client;
    double ScaleFactor;
};
}
#endif /* #ifndef PlanControllerH */
