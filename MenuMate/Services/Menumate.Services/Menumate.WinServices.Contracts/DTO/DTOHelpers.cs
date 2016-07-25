using Menumate.WinServices.Implementation.Contracts.BusinessObjects;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    public static class DTOHelpers
    {
        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="tableDTO"></param>
        /// <returns></returns>
        public static Table TableFromDTO(DTOReservable tableDTO)
        {
            // TODO: validation!

            Table t = new Table(tableDTO.Name, tableDTO.MaxGuests);
            t.Id = tableDTO.Id;

            t.X = tableDTO.X;
            t.Y = tableDTO.Y;
            t.Width = tableDTO.Width;
            t.Height = tableDTO.Height;

            t.RotationAngle = tableDTO.RotationAngle;
            t.Shape = tableDTO.Shape;

            t.Number = tableDTO.Number;

            return t;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="reservationDTO"></param>
        /// <returns></returns>
        public static Reservation ReservationFromDTO(DTOReservation reservationDTO)
        {

            // TODO: validation!

            Reservation reservation = new Reservation();

            reservation.Id = reservationDTO.Id;
            reservation.Arrival = reservationDTO.Arrival;
            reservation.GuestCount = reservationDTO.GuestCount;
            reservation.Duration = reservationDTO.Duration;
            reservation.Note = reservationDTO.Note;
            reservation.PartyName = reservationDTO.PartyName;

            foreach (DTOReservable tableDto in reservationDTO.Reservables)
            {
                reservation.ReservedList.Add(DTOHelpers.TableFromDTO(tableDto));
            }


            return reservation;
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// 
        /// </summary>
        /// <param name="guestDTO"></param>
        /// <returns></returns>
        public static Guest GuestFromDTO(DTOReserver guestDTO)
        {

            DTOHelpers.ValidateGuestDTO(guestDTO);

            Guest newGuest = new Guest(guestDTO.GuestName, guestDTO.GuestPhone, guestDTO.GuestEmail);
            return newGuest;
        }

        public static void ValidateGuestDTO(DTOReserver guestDTO)
        {
            if (guestDTO == null)
            {
                throw new InvalidDTOException("Invalid guest information supplied (null reference).");
            }
        }

        // ---------------------------------------------------------------------
        /// <summary>
        /// Does basic completness/sanity checks on reservation dto. Throws an
        /// InvalidDTOException when any validation check fails. This method must
        /// be used in a try/catch block!
        /// </summary>
        /// <param name="reservationDto"></param>
        public static void ValidateReservationDTO(DTOReservation reservationDto)
        {
            if (reservationDto == null)
            {
                throw new InvalidDTOException("Invalid reservation information supplied (null reference).");
            }

            if (reservationDto.Session == null)
            {
                throw new InvalidDTOException("No session set for this reservation.");
            }

            if (reservationDto.Location == null)
            {
                throw new InvalidDTOException("No parent location.");
            }

            if (reservationDto.Reservables == null)
            {
                throw new InvalidDTOException("No table information supplied.");
            }

            if (reservationDto.Reserver == null)
            {
                throw new InvalidDTOException("No guest information supplied.");
            }
        }

        /// <summary>
        /// Does basic completness/sanity checks on location dto. Throws an
        /// InvalidDTOException when any validation check fails. This method must
        /// be used in a try/catch block!
        /// </summary>
        /// <param name="locationDTO"></param>
        public static void ValidateLocationDTO(DTOLocation locationDTO)
        {
            if (locationDTO == null)
            {
                throw new InvalidDTOException("Invalid location informaiton supplied (null reference)");
            }
        }

        /// <summary>
        /// Does basic completness/sanity checks on reservable (table) dto. Throws an
        /// InvalidDTOException when any validation check fails. This method must
        /// be used in a try/catch block!
        /// </summary>
        /// <param name="tableDTO"></param>
        public static void ValidateReservableDTO(DTOReservable tableDTO)
        {
            if (tableDTO == null)
            {
                throw new InvalidDTOException("Invalid reservable informaiton supplied (null reference)");
            }
            if (tableDTO.Name == null)
            {
                throw new InvalidDTOException("Name of reservable was null");
            }
        }
    }
}
