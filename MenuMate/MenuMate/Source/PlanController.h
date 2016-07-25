#ifndef PlanControllerH
#define PlanControllerH

#include <winsock2.h>
#include <vector>
#include <iterator>
#include <map>

#include "PosIntegration.h"
#include "MM_DBCore.h"

namespace TablePlan {
	class PlanController {

	private:
		TImage *image;
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

      // Location ID , Location.
	   std::map<int,DTOLocation*> locations;

		void SetView(TImage *v);
		void DrawCurrentPlan(Database::TDBTransaction &DBTransaction);

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

	};
}
#endif /* #ifndef PlanControllerH */
