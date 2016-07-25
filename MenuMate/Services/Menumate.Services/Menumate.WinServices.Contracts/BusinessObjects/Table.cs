using System;
using System.Collections.Generic;
using Castle.ActiveRecord;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{
    [ActiveRecord("Tables")]
    public class Table : ActiveRecordBase<Table>
    {
        #region Private Data
        private int _id;

        // business data
        private String _name;
        private int _maxGuests;

        // display data
        private double _x = 0;
        private double _y = 0;
        private double _width = 75;
        private double _height = 75;
        private double _rotationAngle = 0;

        private string _shape = "r";

        private Location _parentLocation;
        private int _number; // This is a unique number for a table in a location. it needs to be generated when the table is created.

        private IList<Reservation> _reservationsUsedBy = new List<Reservation>();
        #endregion

        public Table(String name, int maxGuests)
        {
            _name = name;
            _maxGuests = maxGuests;
        }

        public Table()
        {
        }

        [PrimaryKey]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [Property(ColumnType = "StringClob")]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [Property]
        public int MaxGuests
        {
            get { return _maxGuests; }
            set { _maxGuests = value; }
        }

        [Property]
        public double X
        {
            get { return _x; }
            set { _x = value; }
        }

        [Property]
        public double Y
        {
            get { return _y; }
            set { _y = value; }
        }

        [Property]
        public double Width
        {
            get { return _width; }
            set { _width = value; }
        }

        [Property]
        public double Height
        {
            get { return _height; }
            set { _height = value; }
        }

        [Property]
        public string Shape
        {
            get { return _shape; }
            set { _shape = value; }
        }

        [Property]
        public double RotationAngle
        {
            get { return _rotationAngle; }
            set { _rotationAngle = value; }
        }


        [BelongsTo(Cascade = CascadeEnum.SaveUpdate)]
        public Location ParentLocation
        {
            get { return _parentLocation; }
            set { _parentLocation = value; }
        }

        [Property]
        public int Number
        {
            get { return _number; }
            set { _number = value; }
        }

        /// <summary>
        /// This is the non-writable side of the relation. The reservation controlls
        /// it.
        /// </summary>
        [HasAndBelongsToMany(typeof(Reservation),
            Table = "ReservationTable", ColumnKey = "tableid", ColumnRef = "reservationid",
                Inverse = true, Lazy = true)]
        public IList<Reservation> ReservationsThatUseMe
        {
            get { return _reservationsUsedBy; }
            set { _reservationsUsedBy = value; }
        }

        /// <summary>
        /// Indicates, essentially, what color the table will be represented as. Produces a 'status'
        /// based on client terminal settings (date, filter etc)
        /// </summary>
        /// <param name="view"></param>
        /// <returns></returns>
        public string Status(Terminal term)
        {

            string s = null;
            // determine the table status here
            if (term == null)
            {
                s = "Red";		// default
            }
            else
            {
                if (term.TableViewFilter.Equals("occupancy"))
                {
                    bool found = false;
                    foreach (Reservation res in this.ReservationsThatUseMe)
                    {
                        DateTime compDateTime = DateTime.Now;
                        if ((compDateTime >= res.Arrival) && (compDateTime < (res.Arrival + res.Duration)))
                        {
                            s = "Red";
                            found = true;
                            break;	// bail as have found an active reservation on this table
                        }
                    }

                    // if no reservation was found then add the table with status open
                    if (!found)
                    {
                        s = "Green";
                    }
                }
                else if (term.TableViewFilter.Equals("day"))
                {
                    bool found = false;
                    foreach (Reservation res in this.ReservationsThatUseMe)
                    {
                        DateTime compDateTime = term.CurrentDate;
                        if (compDateTime.Date == res.Arrival.Date)
                        {
                            s = "Red";
                            found = true;
                            break;	// bail as have found an active reservation on this table for day
                        }
                    }

                    // if no reservation was found then add the table with status open
                    if (!found)
                    {
                        s = "Green";
                    }
                }
                else if (term.TableViewFilter.Equals("session"))
                {
                    bool found = false;
                    foreach (Reservation res in this.ReservationsThatUseMe)
                    {
                        DateTime sessionStart;
                        DateTime sessionEnd;
                        res.Session.GetStartAndEndAsDateTimes(term.CurrentDate, out sessionStart, out sessionEnd);

                        if (term.CurrentSession != null)
                        {
                            if (term.CurrentSession.Id == res.Session.Id)
                            {
                                // only check if the arrival falls in the session
                                if ((res.Arrival >= sessionStart) && (res.Arrival < sessionEnd))
                                {
                                    s = "Red";
                                    found = true;
                                    break;	// bail as have found an active reservation on this table for the given session
                                }
                            }
                        }
                    }

                    // if no reservation was found then add the table with status open
                    if (!found)
                    {
                        s = "Green";
                    }
                }
                else
                {
                    s = "Red";		// default
                }
            }
            return s;
        }

        public override string ToString()
        {
            return Name + " (" + MaxGuests.ToString() + ")";
        }

        public override bool Equals(object obj)
        {
            bool b = false;
            Table other = obj as Table;
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
    }

    public class TableComparer : IEqualityComparer<Table>
    {

        public bool Equals(Table x, Table y)
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

        public int GetHashCode(Table obj)
        {
            return obj.Name.GetHashCode();
        }
    }
}
