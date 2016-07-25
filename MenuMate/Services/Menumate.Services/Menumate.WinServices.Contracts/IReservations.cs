using System;
using System.ServiceModel;

using Menumate.WinServices.Implementation.Contracts.DTO;

namespace Menumate.WinServices.Implementation.Contracts
{
    [ServiceContract(SessionMode = SessionMode.Required, CallbackContract = typeof(IReservationsCallback))]
    public interface IReservations
    {
        [OperationContract]
        OperationResult Logon(string terminalName);

        [OperationContract]
        OperationResult LogOff();

        [OperationContract]
        DTOLocation GetCurrentLocation();

        [OperationContract]
        DTOSession GetCurrentSession();

        [OperationContract]
        DateTime GetCurrentDate();

        [OperationContract]
        OperationResult SetCurrentLocation(int locationId);

        [OperationContract]
        OperationResult SetCurrentSession(int sessionId);

        [OperationContract]
        OperationResult AddLocation(DTOLocation locationDTO);

        [OperationContract]
        OperationResult AddReservable(DTOReservable tableDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult AddReservation(DTOReservation reservationDTO);

        [OperationContract]
        OperationResult AddReserver(DTOReserver guestDTO);

        [OperationContract]
        OperationResult AddSession(DTOSession sessionDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult CancelReservation(DTOReservation resDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult DeleteLocation(DTOLocation locationDTO);

        [OperationContract]
        OperationResult DeleteReservable(DTOReservable tableDTO, DTOLocation locationDTO);

        [OperationContract]
        OperationResult DeleteSession(DTOSession sessionToDeleteDTO, DTOLocation locationDTO);

        [OperationContract]
        DTOLocation[] GetAllLocations();

        [OperationContract]
        DTOSession[] GetSessionsForLocation(int locationId);

        [OperationContract]
        DTOReservable[] GetTablesForLocation(int locationId);

        [OperationContract]
        DTOReservation[] GetAllReservationsByLocation(int locationId);

        [OperationContract]
        DTOReservation[] GetReservationsForDay(DateTime day, int locationId, int sessionId);

        [OperationContract]
        DTOReservable[] GetTableAvailabilityForLocation(int locationId, DateTime inspectionTime,
            DTOReservation excludedReservation, TimeSpan duration);

        [OperationContract]
        bool DoesReservationIntersect(DTOReservable table);

        [OperationContract]
        DTOReservability GetReservability(int locationId, DTOReservation[] reservations, DTOReservable table,
            DateTime inspectionDateTime,
            DTOReservation excludedReservation, TimeSpan duration);

        [OperationContract]
        OperationResult GetReservationStatus(DTOReservation reservation, bool excludeSelf);

        [OperationContract]
        DTOReservation[] GetReservationsByReservable(DTOReservable reservableDTO, DTOLocation locationDTO);

        [OperationContract]
        DTOReserver[] GetReserversByName(string guestName);

        [OperationContract]
        byte[] GetBackgroundImgDataForLocation(int locationid);

        [OperationContract]
        DTOReservation[] SearchForReservationsByGuestName(string searchString);

        [OperationContract]
        OperationResult ReleaseReservation(DTOReservation reservation, DateTime releaseDateTime);

        [OperationContract]
        OperationResult SeatReservation(DTOReservation reservation);

        [OperationContract]
        OperationResult SetCurrentDate(DateTime newDate);

        [OperationContract]
        OperationResult IncreaseInterestLevel(int tableid);

        [OperationContract]
        OperationResult DecreaseInterestLevel(int tableid);

        [OperationContract]
        OperationResult DecreaseInterestLevelForReservation(DTOReservation res);

        [OperationContract]
        OperationResult UpdateDefaultDuration(DTOLocation location);

        [OperationContract]
        OperationResult UpdateLocation(DTOLocation location);

        [OperationContract]
        OperationResult UpdateSession(DTOSession session, DTOLocation location);

        [OperationContract]
        OperationResult UpdateReservable(DTOReservable reservable);

        [OperationContract]
        OperationResult UpdateGuest(int guestId, string name, string phone, string email);

        [OperationContract]
        OperationResult UpdateReservation(DTOReservation updatedReservation);

        [OperationContract]
        DTOReserver[] GetAllGuests();

        [OperationContract]
        OperationResult LocateReservation(int reservationId);

        [OperationContract]
        DTOReservation[] SetView(int locationid, int sessionid, DateTime dateToView);

        [OperationContract]
        OperationResult CycleCurrentLocation();

        [OperationContract]
        OperationResult SetTableViewFilter(ReservationViews view);
    }
}
