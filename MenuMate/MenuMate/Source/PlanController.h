#ifndef PlanControllerH
#define PlanControllerH

#include <winsock2.h>
#include <vector>
#include <iterator>
#include <map>

#include "PosIntegration.h"
#include "MM_DBCore.h"
#include "PlanControllerObjects.h"

namespace TablePlan {
	class PlanController {

	private:

		TPoint _currentMouseLocation;
		int _idxCurrentPlan;
         _di_IPosIntegration _client;

      	TRect BackGndRect;

		bool _connected;
      	void ClearView();
		double ScaleFactor;
        void SetColorAsPerNoServiceTime(Database::TDBTransaction &DBTransaction,int TableNumber) ;

	protected:

	public:
		PlanController();
		~PlanController();
        TImage *image;
      // Location ID , Location.
	   std::map<int,DTOLocation*> locations;
       std::map<int,TPlanLocation> locations123;

		void SetView(TImage *v);
		void DrawCurrentPlan(Database::TDBTransaction &DBTransaction);

		AnsiString GetCurrentPlanName();
		DTOReservable *GetTable(int LocationID , int TableNo);
//        TPlanReservable GetTable(int LocationID, int TableNo);
		DTOReservable *GetCurrentTable();
//        TPlanReservable GetCurrentTable();
		AnsiString GetTableDesc();

		int PlanCount();
        int CurrentID();
        void FirstPlan();
        void SetLocation(int LocationIndex);

		std::vector<DTOLocation*> getLocations();
//        std::vector<TPlanLocation> getLocations();
		TPoint CurrentMouseLocation();

		void UpdateMousePos(int, int);

		bool IsInitOk();

		bool Init();
//        std::vector<TPlanReservable> tableList;

	};
}
#endif /* #ifndef PlanControllerH */
