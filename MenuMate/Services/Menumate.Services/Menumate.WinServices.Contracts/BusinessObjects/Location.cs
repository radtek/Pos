using System;
using System.Collections.Generic;
using System.Linq;
using Castle.ActiveRecord;
using Castle.ActiveRecord.Queries;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects 
{

	[ActiveRecord("Locations")]
	public class Location : ActiveRecordBase<Location>
    {


		#region private data

		private int _id;
		private String _name;
		private TimeSpan _defaultDuration;
		private double _planWidth;
		private double _planHeight;
		private IList<Table> _tableList = new List<Table>();
		private IList<Session> _sessions = new List<Session>();
		private IList<Reservation> _reservations = new List<Reservation>();

        private Byte[] _backgroundImg = null;

		#endregion

		#region ctors

		public Location() { }
		public Location(String name) 
        {

			this.Name = name;
			this.DefaultDuration = new TimeSpan(2, 0, 0);
		}

		#endregion

		#region Public Properties

		[PrimaryKey]
		public int Id {
			get { return _id; }
			set { _id = value; }
		}

		[Property(ColumnType = "StringClob")]
		public String Name {
			get { return _name; }
			set { _name = value; }
		}

		[Property]
		public TimeSpan DefaultDuration {
			get { return _defaultDuration; }
			set { _defaultDuration = value; }
		}

		[HasMany(Cascade = ManyRelationCascadeEnum.SaveUpdate)]
		public IList<Session> Sessions {
			get { return _sessions; }
			set { _sessions = value; }
		}

		[HasMany(Cascade = ManyRelationCascadeEnum.SaveUpdate, Lazy = true)]
		public IList<Table> Tables {
			get { return _tableList; }
			set { _tableList = value; }
		}

		[HasMany(Cascade = ManyRelationCascadeEnum.SaveUpdate, Lazy = true)]
		public IList<Reservation> Reservations {
			get { return _reservations; }
			set { _reservations = value; }
		}

        [Property]
        public double PlanWidth {
            get { return _planWidth; }
            set { _planWidth = value; }
        }

        [Property]
        public double PlanHeight {
            get { return _planHeight; }
            set { _planHeight = value; }
        }

        [Property]
        public Byte[] BackgroundImg {
            get { return _backgroundImg; }
            set { _backgroundImg = value; }
        }
		#endregion


		/// <summary>
		/// Adds to locations collection of reservations, saves location's state to database.
		/// </summary>
		/// <param name="newReservation"></param>
		public void AddReservation(Reservation newReservation) 
        {
			if (newReservation.ReservedList.Count == 0)
            {
				throw new ReservationException("No tables.");
			}

			if (newReservation.GuestCount < 1) 
            {
				throw new ReservationException("Guest count cannot be less than one");
			}

			foreach (Table table in newReservation.ReservedList)
            {
				IList<Reservation> reservationsByTable = table.ReservationsThatUseMe;
				Reservability availability = this.GetReservability(reservationsByTable, table, newReservation.Arrival, null, newReservation.Duration);
				if (!availability.FullyAvailable)
                {
					throw new ReservationException(string.Format("Reservation not added because {0} is not available.", table.ToString()));
				}
			}

			newReservation.ParentLocation = this;
			newReservation.Save();
			Reservations.Add(newReservation);

			// cascade
			this.Save();
		}


		/// <summary>
		/// Add a table to the location. Saves to database.
		/// </summary>
		/// <param name="reservable"></param>
		public void AddTable(Table reservable) 
        {
			if (Tables.Contains<Table>(reservable, new TableComparer())) 
            {
				throw new ReservableAlreadyExistsException();
			}
			else
            {
				reservable.ParentLocation = this;
				reservable.Save();

				Tables.Add(reservable);
				this.Save();    //cascade
			}
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="table"></param>
		public void RemoveTable(Table table) 
        {
			if (Tables.Contains<Table>(table)) 
            {
				if (table.ReservationsThatUseMe.Count == 0) 
                {
					Tables.Remove(table);
					table.Delete();
				}
				else
                {
					throw new InvalidOperationException("Reservations still exists for this table; cannot delete it.");
				}
			}
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="t"></param>
		/// <param name="name"></param>
		/// <param name="maxguests"></param>
		public void UpdateTable(Table t, string name, int maxguests, double x, double y, double w, double h, string shape, int number) 
        {
            // validation!

            //if (!(t.Name.Equals(name)) && Tables.Contains<Table>(t)) {
            //    throw new ReservableAlreadyExistsException("Cannot have two tables with same name.");
            //}
            //else {

				t.Name = name;
				t.MaxGuests = maxguests;
				t.X = x;
				t.Y = y;
				t.Width = w;
				t.Height = h;
                t.Shape = shape;
                t.Number = number;


				t.Save();
            //}
		}

		/// <summary>
		/// Add a session to the collection of sessions for a location. Saves to database.
		/// </summary>
		/// <param name="session"></param>
		public void AddSession(Session session) 
        {
			if (Sessions.Contains(session, new SessionComparer()))
            {
				throw new SessionAlreadyExistsException();
			}
			else 
            {
				Sessions.Add(session);
				this.Save();
			}
		}

		public void DeleteSession(Session session) 
        {

			// find out if any reservations are using this session
			SimpleQuery<Reservation> q = new SimpleQuery<Reservation>(@"from Reservation res where res.Session.id = :session_id");

			q.SetParameter("session_id", session.Id);
			Reservation[] reservationList = q.Execute();

			if (reservationList.Length == 0)
            {

				// remove the reference to the session from its list
				this.Sessions.Remove(session);

				// delete the session, will cause cascade
				session.Delete();
			}
			else
            {
				throw new ReservationException("Cannot delete session if reservations exists for that session.");
			}
		}

		public void CancelReservation(Reservation reservation) 
        {
			if (Reservations.Contains<Reservation>(reservation)) 
            {

				IList<Table> tables = reservation.ReservedList.ToList<Table>();
				foreach (Table t in tables)
                {
					reservation.RemoveReservable(t);

				}

				Reservations.Remove(reservation);

				reservation.Delete();
			}
		}

		/// <summary>
		/// Implies that real people have arrived to fill this reservation. Cannot seat
		/// guests if it is outside their reservation times (arrival and duration).
		/// </summary>
		/// <param name="res"></param>
		public void SeatReservation(Reservation res) 
        {
			DateTime seatedTime = DateTime.Now;
			if ((seatedTime >= res.Arrival) && (seatedTime < (res.Arrival + res.Duration)))
            {

				res.Seated = true;

				// cascade the changes
				this.Save();
			}
			else
            {
				string message = string.Format("Cannot seat a reservation outside of allocated time; {0} => {1}",
					res.Arrival, res.Arrival + res.Duration);
				throw new ReservationException(message);
			}
		}

		public void ReleaseReservation(Reservation reservation1, DateTime whenReleased)
        {
			reservation1.Release(whenReleased);

			// cascade the changes
			this.Save();
		}


		/// <summary>
		/// This value is based on the total number of seats for the given tables. This
		/// could potentially be alternate ways to calculate this i.e., staffing levels may
		/// effect how many guests can be handled.
		/// </summary>
		/// 
		public int MaxGuests 
        {
			get
            {
				int total = 0;

				foreach (Table r in Tables)
                {
					total += r.MaxGuests;
				}

				return total;
			}
		}

		public IList<Reservation> GetReservationsByReservable(Table table) 
        {
			return table.ReservationsThatUseMe;
		}

		///// <summary>
		///// Returns the number of reservable items available in the specified
		///// session for the supplied date/time.
		///// </summary>
		///// <param name="session"></param>
		///// <param name="dateTime"></param>
		///// <returns></returns>
		public int GetReservableAvailability(Session session, DateTime dateTime) 
        {
			return GetAvailability(session, dateTime, false, DefaultDuration);
		}

		public int GetReservableAvailability(Session session, DateTime dateTime, TimeSpan duration) 
        {
			return GetAvailability(session, dateTime, false, duration);
		}

		///// <summary>
		///// Returns the number of guest seatings available given the session and
		///// the date/time, using the default duration.
		///// </summary>
		///// <param name="session"></param>
		///// <param name="dateTime"></param>
		///// <returns></returns>
		public int GetGuestAvailability(Session session, DateTime dateTime) 
        {
			return GetAvailability(session, dateTime, true, DefaultDuration);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="session"></param>
		/// <param name="dateTime"></param>
		/// <param name="duration"></param>
		/// <returns></returns>
		public int GetGuestAvailability(Session session, DateTime dateTime, TimeSpan duration) 
        {
			return GetAvailability(session, dateTime, true, duration);
		}

		/// <summary>
		/// Returns a value that indicates either the number of reservable items available,
		/// or the number of guest seatings available.
		/// </summary>
		/// <param name="session"></param>
		/// <param name="dateTime"></param>
		/// <param name="guest"></param>
		/// <returns></returns>
		private int GetAvailability(Session session, DateTime dateTime, bool guest, TimeSpan duration)
        {
			int total = 0;

			IList<Table> reservables = GetReservables(session, dateTime, duration);
			foreach (Table reservable in reservables) 
            {
				Reservability reservability = GetReservability(reservable, dateTime, null, duration);

				if (reservability.FullyAvailable) 
                {
					if (guest) 
                    {
						total += reservable.MaxGuests;
					}
					else
                    {
						total++;
					}
				}
			}
			return total;
		}

		/// <summary>
		/// Returns a list of reservable items that are available for booking
		/// given the session and the date/time.
		/// </summary>
		/// <param name="session"></param>
		/// <param name="dateTime"></param>
		/// <returns></returns>
		public IList<Table> GetReservables(Session session, DateTime dateTime, TimeSpan duration)
        {
			IList<Table> reservables = new List<Table>();
			foreach (Table reservable in Tables)
            {
				if (GetReservability(reservable, dateTime, null, duration).FullyAvailable)
                {
					reservables.Add(reservable);
				}
			}

			return reservables;
		}

		///// <summary>
		///// Conveniance method to evaluate the reservability of a particular
		///// reservable item. Used when creating a reservation.
		///// </summary>
		///// <param name="reservable"></param>
		///// <param name="dateTime"></param>
		///// <returns></returns>
		public Reservability GetReservability(Table reservable, DateTime dateTime) 
        {
			return GetReservability(this.GetReservationsByReservable(reservable), reservable, dateTime, null, DefaultDuration);
		}

		///// <summary>
		///// Conveniance method to evaluate the reservability of a particular
		///// reservable item. Used when creating a reservation.
		///// </summary>
		///// <param name="reservable"></param>
		///// <param name="dateTime"></param>
		///// <returns></returns>
		public Reservability GetReservability(Table reservable, DateTime dateTime, TimeSpan duration)
        {
			return GetReservability(this.GetReservationsByReservable(reservable), reservable, dateTime, null, duration);
		}

		///// <summary>
		///// Conveniance method to evaluate the reservability of a particular
		///// reservable item where a reservation already exists and needs to be
		///// excluded.
		///// </summary>
		///// <param name="reservable"></param>
		///// <param name="dateTime"></param>
		///// <param name="excludeReservation"></param>
		///// <returns></returns>
		public Reservability GetReservability(Table reservable, DateTime dateTime, Reservation excludedReservation, TimeSpan duration) 
        {
			return GetReservability(this.GetReservationsByReservable(reservable),
			   reservable, dateTime, excludedReservation, duration);
		}

		/// <summary>
		/// Given a list of reservations and a reservable item return the availability
		/// for the given date/time. The reservable item has been reserved by the reservations
		/// in the list.
		/// </summary>
		/// <param name="reservations"></param>
		/// <param name="reservable"></param>
		/// <param name="dateTime"></param>
		/// <returns></returns>
		public Reservability GetReservability(IList<Reservation> reservations, Table reservable, DateTime dateTime, Reservation excludedReservation, TimeSpan newDuration) {

			Reservation prev = null;
			Reservation next = null;

			if (reservations != null) {
				foreach (Reservation reservation in reservations) 
                {
					if (Reservability.getDateTimeZeroed(reservation.Arrival) < Reservability.getDateTimeZeroed(dateTime)) 
                    {
						if (prev == null || ((Reservability.getDateTimeZeroed(prev.Arrival) + prev.Duration) < (Reservability.getDateTimeZeroed(reservation.Arrival) + reservation.Duration))) 
                        {
							if (excludedReservation == null || (!reservation.Equals(excludedReservation))) 
                            {
								prev = reservation;
							}
						}
					}
					else 
                    {
						if ((next == null || (Reservability.getDateTimeZeroed(next.Arrival) >
								Reservability.getDateTimeZeroed(reservation.Arrival))))
                        {
							if (excludedReservation == null || (!reservation.Equals(excludedReservation)))
                            {
								next = reservation;
							}
						}
					}
				}
			}
			return new Reservability(reservable, dateTime, newDuration, prev, next);
		}

		public override string ToString()
        {
			return _name;
		}

		public override bool Equals(object obj)
        {
			bool b = false;
			Location other = obj as Location;
			if (other != null) {
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

	}

	public class LocationComparer : IEqualityComparer<Location> 
    {
		public bool Equals(Location x, Location y)
        {
			if (x.Name.Equals(y.Name))
            {
				return true;
			}
			else
            {
				return false;
			}
		}

		public int GetHashCode(Location obj)
        {
			return obj.Name.GetHashCode();
		}
	}

}
