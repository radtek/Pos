using System;
using System.Collections.Generic;
using System.Text;
using Castle.ActiveRecord;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{
    [ActiveRecord("Sessions")]
    public class Session : ActiveRecordBase<Session>
    {
        protected String _name;
        protected TimeSpan _start;
        protected TimeSpan _end;

        private Location _location;

        private int _id;

        public Session() { }

        public Session(String name, TimeSpan start, TimeSpan end)
        {
            _name = name;
            _start = start;
            _end = end;
        }

        [PrimaryKey]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [Property(ColumnType = "StringClob")]
        public String Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [BelongsTo(Cascade = CascadeEnum.SaveUpdate)]
        public Location ParentLocation
        {

            get { return _location; }
            set { _location = value; }
        }

        public bool IsInSession(DateTime dateTime)
        {
            DateTime sessionStartTime;
            DateTime sessionEndTime;

            GetStartAndEndAsDateTimes(dateTime, out sessionStartTime, out sessionEndTime);
            bool b = (dateTime >= sessionStartTime && dateTime <= sessionEndTime);
            return b;
        }

        [Property]
        public TimeSpan StartTime
        {
            get { return _start; }
            set { _start = value; }
        }

        [Property]
        public TimeSpan EndTime
        {
            get { return _end; }
            set { _end = value; }
        }

        public TimeSpan OutOfSessionBy(DateTime dateTime)
        {
            DateTime sessionStartTime;
            DateTime sessionEndTime;

            GetStartAndEndAsDateTimes(dateTime, out sessionStartTime, out sessionEndTime);

            if (dateTime < sessionStartTime)
            {
                return sessionStartTime - dateTime;
            }
            else if (dateTime > sessionEndTime)
            {
                return dateTime - sessionEndTime;
            }
            else
            {
                return TimeSpan.Zero;
            }
        }

        public void GetStartAndEndAsDateTimes(DateTime dateTime, out DateTime start, out DateTime end)
        {
            start = dateTime.Date + _start;

            if (start - dateTime > TimeSpan.FromHours(12))
            {
                start -= TimeSpan.FromDays(1);
            }

            end = start + (_end - _start);

            if (start >= end)
            {
                end += TimeSpan.FromDays(1);
            }
        }

        public override string ToString()
        {
            DateTime startDateTime;
            DateTime endDateTime;

            GetStartAndEndAsDateTimes(DateTime.Now, out startDateTime, out endDateTime);

            if (startDateTime.TimeOfDay.Equals(endDateTime.TimeOfDay))
            {
                return new StringBuilder().Append(Name).Append(" (All Day)").ToString();
            }
            else
            {
                return new StringBuilder().Append(Name).Append(" (").Append(startDateTime.TimeOfDay).Append(" - ")
                    .Append(endDateTime.TimeOfDay).Append(")").ToString();
            }
        }

        public override bool Equals(object obj)
        {
            bool areEqual = false;
            Session otherSession = obj as Session;
            if (otherSession != null)
            {
                if (this.Id == otherSession.Id)
                {
                    areEqual = true;
                }
            }
            return areEqual;
        }

        public override int GetHashCode()
        {
            return this.Id;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class SessionComparer : IEqualityComparer<Session>
    {
        public bool Equals(Session x, Session y)
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

        public int GetHashCode(Session obj)
        {
            return obj.Name.GetHashCode();
        }
    }

}
