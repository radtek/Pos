using System;
using System.Collections.Generic;
using Castle.ActiveRecord;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{

    [ActiveRecord("Reservations")]
    public class Reservation : ActiveRecordBase<Reservation>
    {
        private int _id;
        private DateTime _arrival = new DateTime(1753, 1, 1, 12, 0, 0);

        // This is only the estimated duration
        private TimeSpan _duration = TimeSpan.MinValue;

        private bool _beenReleased = false;
        private DateTime _whenReleased = new DateTime(1753, 1, 1, 12, 0, 0);
        private int _guestCount = 0;

        private String _partyName;
        private String _note;
        private bool _seated = false;

        // associations
        private Guest _reserver = null;
        private IList<Table> _reservables = new List<Table>();
        private Session _session;
        private Location _location;


        public bool HasCorrectSeating
        {
            get
            {
                int s = 0;
                foreach (Table reservable in _reservables)
                {
                    s += reservable.MaxGuests;
                }
                return s >= _guestCount;
            }
        }

        public Reservation()
        {
        }

        public Reservation(Guest reserver, Session session, DateTime arrival, bool released, DateTime whenReleased, TimeSpan duration, int guestCount,
            string partyName, string note, bool seated)
        {

            _reserver = reserver;
            _session = session;
            _arrival = arrival;
            _beenReleased = released;
            _whenReleased = whenReleased;
            _duration = duration;
            _guestCount = guestCount;
            _partyName = partyName;
            _note = note;
            _seated = seated;
        }

        [PrimaryKey]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [BelongsTo(Cascade = CascadeEnum.SaveUpdate)]
        public Guest Reserver
        {
            get { return _reserver; }
            set { _reserver = value; }
        }

        [BelongsTo(Cascade = CascadeEnum.SaveUpdate)]
        public Session Session
        {
            get { return _session; }
            set { _session = value; }
        }

        [BelongsTo(Cascade = CascadeEnum.SaveUpdate)]
        public Location ParentLocation
        {
            get { return _location; }
            set { _location = value; }
        }

        [Property]
        public DateTime Arrival
        {
            get { return _arrival; }
            set { _arrival = value; }
        }

        [Property]
        public TimeSpan EstimatedDuration
        {
            get { return _duration; }
            set { _duration = value; }
        }

        [Property]
        public bool BeenReleased
        {
            get { return _beenReleased; }
            set { _beenReleased = value; }
        }

        [Property]
        public DateTime WhenReleased
        {
            get { return _whenReleased; }
            set { _whenReleased = value; }
        }

        [Property]
        public bool Seated
        {
            get { return _seated; }
            set { _seated = value; }
        }

        [Property(ColumnType = "StringClob")]
        public String PartyName { get { return _partyName; } set { _partyName = value; } }

        [Property(ColumnType = "StringClob")]
        public String Note { get { return _note; } set { _note = value; } }

        public TimeSpan Duration
        {
            get
            {

                if (this.BeenReleased)
                {
                    TimeSpan t = WhenReleased - Arrival;
                    return t;
                }
                else
                {
                    return _duration;
                }
            }
            set
            {
                // TODO: validation logic?
                _duration = value;
            }
        }

        [Property]
        public int GuestCount
        {
            get { return _guestCount; }
            set { _guestCount = value; }
        }

        /// <summary>
        /// This is the writable side of the relation.
        /// </summary>
        [HasAndBelongsToMany(typeof(Table),
            Table = "ReservationTable", ColumnKey = "reservationid", ColumnRef = "tableid",
                Cascade = ManyRelationCascadeEnum.All, Lazy = true)]
        public IList<Table> ReservedList
        {
            get { return _reservables; }
            set { _reservables = value; }
        }

        /// <summary>
        /// Adds the table to the reservations list of tables and saves the reservations state
        /// to the database.
        /// </summary>
        /// <param name="reservable"></param>
        public void AddReservable(Table reservable)
        {
            if (ReservedList.Contains(reservable))
            {
                throw new AlreadyReservedException("Already reserved: " + reservable.Name);
            }
            else
            {
                ReservedList.Add(reservable);
                this.Save();
            }
        }

        /// <summary>
        /// Removes a table from the reservations list of tables, saves.
        /// </summary>
        /// <param name="table"></param>
        public void RemoveReservable(Table table)
        {
            if (ReservedList.Contains(table))
            {
                ReservedList.Remove(table);
                this.Update();
            }
        }


        public override string ToString()
        {
            return _reserver.Name;
        }

        public bool IsComplete()
        {
            return
                Reserver != null &&
                Session != null &&
                Arrival != DateTime.MinValue &&
                Duration != TimeSpan.MinValue &&
                GuestCount != 0 &&
                ReservedList.Count != 0;
        }

        public override bool Equals(object obj)
        {


            bool b = false;
            Reservation other = obj as Reservation;
            if (other != null)
            {
                if (this.Id == other.Id)
                {
                    b = true;
                }
            }
            return b;
        }

        public override int GetHashCode()
        {
            return this.Id;
        }

        /// <summary>
        /// Release the reservables held by this reservation by way of changing
        /// the Duration to be shorter and therefore relinquising claim on them
        /// for that time period.
        /// </summary>
        public void Release(DateTime whenReleased)
        {
            if ((whenReleased > (_arrival + _duration)) || (whenReleased < _arrival))
            {
                // can't release a reservation after its has already passed
                // it estimated duration, or before arrival so do nothing
            }
            else
            {
                BeenReleased = true;
                WhenReleased = whenReleased;
                this.Update();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="g1"></param>
        public void AddGuest(Guest g1)
        {
            this.Reserver = g1;
        }
    }

    public class ReservationComparer : IEqualityComparer<Reservation>
    {
        #region IComparer<Reservation> Members

        public bool Equals(Reservation r1, Reservation r2)
        {
            return (r1.Arrival.Equals(r2.Arrival) &&
                (r1.GuestCount == r2.GuestCount));
        }

        public int GetHashCode(Reservation r)
        {
            return r.Arrival.GetHashCode() ^ r.GuestCount.GetHashCode();
        }

        #endregion
    }


}
