using System;
using System.Runtime.Serialization;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    [DataContract]
    public class DTOReservability
    {

        private DTOReservation _previousReservation;
        private DTOReservation _nextReservation;
        private DateTime _inspectionDateTime;
        private TimeSpan _duration;
        private DTOReservable _table;
        private bool _fullyAvailable;

        protected int _id;

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }


        public DTOReservability()
        {

        }

        [DataMember]
        public DateTime InspectionDateTime
        {
            get { return _inspectionDateTime; }
            set { _inspectionDateTime = value; }
        }

        [DataMember]
        public TimeSpan Duration
        {
            get { return _duration; }
            set { _duration = value; }
        }

        [DataMember]
        public DTOReservation PreviousReservation
        {
            get { return _previousReservation; }
            set { _previousReservation = value; }
        }

        [DataMember]
        public DTOReservation NextReservation
        {
            get { return _nextReservation; }
            set { _nextReservation = value; }
        }

        [DataMember]
        public bool FullyAvailable
        {
            get { return _fullyAvailable; }
            set { _fullyAvailable = value; }
        }
    }
}
