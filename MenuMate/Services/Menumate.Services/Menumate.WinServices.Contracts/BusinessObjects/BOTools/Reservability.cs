using System;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{
    public class Reservability
    {
        private readonly Table _reservable;
        private readonly DateTime _inspectionDateTime;
        private readonly TimeSpan _duration;
        private readonly Reservation _previousReservation;
        private readonly Reservation _nextReservation;

        internal Reservability(Table reservable, DateTime inspectionDateTime, TimeSpan duration,
            Reservation previousReservation, Reservation nextReservation)
        {
            _reservable = reservable;
            _inspectionDateTime = Reservability.getDateTimeZeroed(inspectionDateTime);
            _duration = duration;
            _previousReservation = previousReservation;
            _nextReservation = nextReservation;
        }

        public Table Reservable
        {
            get { return _reservable; }
        }

        public DateTime InspectionDateTime
        {
            get { return _inspectionDateTime; }
        }

        public TimeSpan Duration
        {
            get { return _duration; }
        }

        public Reservation PreviousReservation
        {
            get { return _previousReservation; }
        }

        public Reservation NextReservation
        {
            get { return _nextReservation; }
        }

        public bool FullyAvailable
        {
            get
            {
                DateTime previousReservationArrival = DateTime.MinValue;
                DateTime nextReservationArrival = DateTime.MinValue;

                if (this.PreviousReservation != null)
                {
                    previousReservationArrival = getDateTimeZeroed(this.PreviousReservation.Arrival);
                }

                if (this.NextReservation != null)
                {
                    nextReservationArrival = getDateTimeZeroed(this.NextReservation.Arrival);
                }

                if (this.PreviousReservation == null ||
                    (previousReservationArrival + this.PreviousReservation.Duration <= this.InspectionDateTime))
                {
                    if (this.NextReservation == null ||
                        (nextReservationArrival >= this.InspectionDateTime + this.Duration))
                    {
                        return true;
                    }
                }
                return false;
            }
        }

        public static DateTime getDateTimeZeroed(DateTime d)
        {
            return new DateTime(d.Year,
                    d.Month,
                    d.Day,
                    d.Hour,
                    d.Minute, 0, 0);
        }
    }
}
