using System;
using System.Runtime.Serialization;
using System.Text;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    [DataContract]
    public class DTOSession
    {
        private int _id;
        private String _name;
        private TimeSpan _start;
        private TimeSpan _end;
        private bool _isInSession;

        public DTOSession()
        {

        }

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [DataMember]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [DataMember]
        public TimeSpan StartTime
        {
            get { return _start; }
            set { _start = value; }
        }

        [DataMember]
        public TimeSpan EndTime
        {
            get { return _end; }
            set { _end = value; }
        }

        [DataMember]
        public bool IsInSession
        {
            get { return _isInSession; }
            set { _isInSession = value; }
        }

        public override bool Equals(object obj)
        {
            DTOSession other = obj as DTOSession;
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
    }
}
