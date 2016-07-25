using System;
using System.Runtime.Serialization;
using System.Text;


namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    [DataContract]
    public class DTOReservation
    {
        private int _id = 0;

        private DateTime _arrival;
        private DateTime _released = DateTime.MinValue;
        private TimeSpan _duration = TimeSpan.MinValue;
        private int _guestCount;
        private bool _hasCorrectSeating;

        private DTOReserver _reserverDto = null;

        private DTOReservable[] _reservables = null;

        private DTOSession _session;
        private DTOLocation _location;

        private String _partyName;

        private String _note;

        private bool _seated = false;

        public DTOReservation()
        {
            _reserverDto = new DTOReserver();
        }

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        #region Reservation Details

        [DataMember]
        public bool HasCorrectSeating
        {
            get { return _hasCorrectSeating; }
            set { _hasCorrectSeating = value; }
        }

        [DataMember]
        public DateTime Arrival
        {
            get { return _arrival; }
            set { _arrival = value; }
        }

        [DataMember]
        public DateTime Released
        {
            get { return _released; }
            set { _released = value; }
        }

        [DataMember]
        public TimeSpan Duration
        {
            get { return _duration; }
            set { _duration = value; }
        }

        [DataMember]
        public int GuestCount
        {
            get { return _guestCount; }
            set { _guestCount = value; }
        }



        [DataMember]
        public DTOReservable[] Reservables
        {
            get { return _reservables; }
            set { _reservables = value; }
        }

        [DataMember]
        public DTOLocation Location
        {
            get { return _location; }
            set { _location = value; }
        }

        [DataMember]
        public DTOSession Session
        {
            get { return _session; }
            set { _session = value; }
        }

        [DataMember]
        public String PartyName
        {
            get { return _partyName; }
            set { _partyName = value; }
        }

        [DataMember]
        public String Note
        {
            get { return _note; }
            set { _note = value; }
        }

        [DataMember]
        public bool Seated
        {
            get { return _seated; }
            set { _seated = value; }
        }

        [DataMember]
        public DTOReserver Reserver
        {
            get { return _reserverDto; }
            set { _reserverDto = value; }
        }


        #endregion

        public override string ToString()
        {
            StringBuilder b = new StringBuilder();
            b.Append(this.Arrival.ToShortDateString()).Append(" ").Append(this.Arrival.ToShortTimeString())
                .Append(" ").Append(this.Reserver.GuestName);

            if (this.Reservables != null && this.Reservables.Length > 0)
            {
                b.Append(" ");
                foreach (DTOReservable table in this.Reservables)
                {
                    b.Append(table.Name).Append(" ");
                }
            }

            if (this.Seated)
            {
                b.Append("(seated)");
            }

            return b.ToString();
        }

        public override bool Equals(object obj)
        {
            DTOReservation other = obj as DTOReservation;
            if (other == null)
            {
                return false;
            }

            return this.Id == other.Id;
        }

        public override int GetHashCode()
        {
            return this.Id;
        }
    }
}
