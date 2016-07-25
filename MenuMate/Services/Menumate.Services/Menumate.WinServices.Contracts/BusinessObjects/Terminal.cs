using System;
using Castle.ActiveRecord;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{
    [ActiveRecord("Terminals")]
    public class Terminal : ActiveRecordBase<Terminal>
    {

        private int _id;

        public Terminal() { }

        public Terminal(string name)
        {
            this.Name = name;
            this.CurrentDate = DateTime.Today;
            this.TableViewFilter = "occupancy";
        }

        [PrimaryKey]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }
        private string _name;

        [Property]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
        private Location _currentLocation;

        [BelongsTo()]
        public Location CurrentLocation
        {
            get { return _currentLocation; }
            set { _currentLocation = value; }
        }
        private Session _currentSession;

        [BelongsTo()]
        public Session CurrentSession
        {
            get { return _currentSession; }
            set { _currentSession = value; }
        }
        private DateTime _currentDate;

        [Property]
        public DateTime CurrentDate
        {
            get { return _currentDate; }
            set { _currentDate = value; }
        }

        private string _tableViewFilter;

        [Property]
        public string TableViewFilter
        {
            get { return _tableViewFilter; }
            set { _tableViewFilter = value; }
        }

        public override bool Equals(object obj)
        {
            bool b = false;
            Terminal other = obj as Terminal;
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
}
