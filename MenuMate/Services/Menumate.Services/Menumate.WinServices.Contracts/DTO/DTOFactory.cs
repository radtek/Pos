using System;
using System.Collections.Generic;
using System.Linq;

using Castle.ActiveRecord;

using Menumate.WinServices.Implementation;
using Menumate.WinServices.Implementation.Contracts.BusinessObjects;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    public static class DTOFactory
    {
        // ---------------------------------------------------------------------
        /// <summary>
        /// Builds a DTO with tables and session, but EXLUDES reservations, 
        /// this is because;
        /// 1 - A location has reference to a reservation, and a reservation
        /// has reference to a locaiton so we could easily build a never-ending nested
        /// DTO.
        /// 2 - A location may have a LARGE amount of reservations! So only get selected ones.
        /// 
        /// Also excludes any background image data; get this in a seperate call.
        /// 
        /// </summary>
        /// <param name="location"></param>
        /// <returns></returns>
        public static DTOLocation BuildLocationDTO(Location location)
        {
            DTOLocation locationDto = null;
            if (location != null)
            {

                locationDto = new DTOLocation();
                locationDto.Id = location.Id;
                locationDto.Name = location.Name;
                locationDto.DefaultDuration = location.DefaultDuration;
                locationDto.Width = location.PlanWidth;
                locationDto.Height = location.PlanHeight;


                IList<DTOSession> dtoSessions = new List<DTOSession>();
                foreach (Session s in location.Sessions)
                {
                    dtoSessions.Add(BuildSessionDTO(s));
                }
                locationDto.Sessions = dtoSessions.ToArray<DTOSession>();

                IList<DTOReservable> dtoReservables = new List<DTOReservable>();
                foreach (Table r in location.Tables)
                {
                    dtoReservables.Add(BuildReservableDTO(r, null));
                }
                locationDto.ReservableList = dtoReservables.ToArray<DTOReservable>();
            }
            return locationDto;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="r"></param>
        /// <returns></returns>
        public static DTOReservable BuildReservableDTO(Table r, Terminal t)
        {
            DTOReservable reservableDTO = new DTOReservable();
            if (r != null)
            {
                reservableDTO.Id = r.Id;
                reservableDTO.Name = r.Name;
                reservableDTO.MaxGuests = r.MaxGuests;

                // create a 'shallow' dto location
                DTOLocation pl = new DTOLocation();
                pl.Id = r.ParentLocation.Id;
                pl.Name = r.ParentLocation.Name;

                reservableDTO.ParentLocation = pl;

                // send back the current interest level
                if (r.Id != 0)
                {
                    reservableDTO.CurrentInterestLevel = ReservationsService.GetTableInterestLevel(r.Id);
                }

                // table plan data
                reservableDTO.X = r.X;
                reservableDTO.Y = r.Y;
                reservableDTO.Width = r.Width;
                reservableDTO.Height = r.Height;
                reservableDTO.RotationAngle = r.RotationAngle;
                reservableDTO.Shape = r.Shape;

                reservableDTO.TableColor = r.Status(t);

                reservableDTO.Number = r.Number;

            }
            return reservableDTO;
        }

        // ---------------------------------------------------------------------
        public static DTOSession BuildSessionDTO(Session s)
        {
            DTOSession sessionDTO = new DTOSession();
            if (s != null)
            {
                sessionDTO.Id = s.Id;
                sessionDTO.Name = s.Name;
                sessionDTO.StartTime = s.StartTime;
                sessionDTO.EndTime = s.EndTime;
            }
            return sessionDTO;
        }

        // ---------------------------------------------------------------------
        public static DTOReservation BuildReservationDTO(Reservation r)
        {
            DTOReservation reservationDTO = null;

            if (r != null)
            {

                reservationDTO = new DTOReservation();
                reservationDTO.Id = r.Id;

                reservationDTO.Reserver = DTOFactory.BuildReserverDTO(r.Reserver);
                reservationDTO.Session = DTOFactory.BuildSessionDTO(r.Session);

                IList<DTOReservable> dtoReservables = new List<DTOReservable>();
                foreach (Table res in r.ReservedList)
                {
                    dtoReservables.Add(BuildReservableDTO(res, null));
                }
                reservationDTO.Reservables = dtoReservables.ToArray<DTOReservable>();

                reservationDTO.GuestCount = r.GuestCount;
                reservationDTO.Arrival = r.Arrival;
                reservationDTO.Duration = r.Duration;
                reservationDTO.Note = r.Note;
                reservationDTO.PartyName = r.PartyName;
                reservationDTO.Released = r.WhenReleased;
                reservationDTO.Seated = r.Seated;
                reservationDTO.HasCorrectSeating = r.HasCorrectSeating;
                reservationDTO.Location = DTOFactory.BuildLocationDTO(r.ParentLocation);
            }
            return reservationDTO;

        }

        // ---------------------------------------------------------------------
        public static DTOReserver BuildReserverDTO(Guest iReserver)
        {

            DTOReserver reserverDTO = new DTOReserver();
            if (iReserver != null)
            {
                reserverDTO.Id = iReserver.Id;
                reserverDTO.GuestName = iReserver.Name;
                reserverDTO.GuestPhone = iReserver.Phone;
                reserverDTO.GuestEmail = iReserver.Email;
            }
            return reserverDTO;
        }

        // ---------------------------------------------------------------------
        internal static DTOReservability BuildReservabilityDTO(Reservability reservability)
        {
            DTOReservability dtoReservability = new DTOReservability();

            dtoReservability.Duration = reservability.Duration;
            dtoReservability.InspectionDateTime = reservability.InspectionDateTime;
            dtoReservability.NextReservation = DTOFactory.BuildReservationDTO(reservability.NextReservation);
            dtoReservability.PreviousReservation = DTOFactory.BuildReservationDTO(reservability.PreviousReservation);
            dtoReservability.FullyAvailable = reservability.FullyAvailable;

            return dtoReservability;
        }
    }
}
