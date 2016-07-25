using System;
using System.Runtime.Serialization;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    [DataContract]
    public class DTOLocation
    {
        private int _id;
        private string _name;
        private DTOReservable[] _reservableList;
        private DTOSession[] _sessions;
        private TimeSpan _defaultDuration;

        private Byte[] _backgroundImg;


        private double _width;

        [DataMember]
        public double Width
        {
            get { return _width; }
            set { _width = value; }
        }
        private double _height;

        [DataMember]
        public double Height
        {
            get { return _height; }
            set { _height = value; }
        }

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        public DTOLocation()
        {
        }

        [DataMember]
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [DataMember]
        public DTOReservable[] ReservableList
        {
            get { return _reservableList; }
            set { _reservableList = value; }
        }

        [DataMember]
        public DTOSession[] Sessions
        {
            get { return _sessions; }
            set { _sessions = value; }
        }

        [DataMember]
        public TimeSpan DefaultDuration
        {
            get { return _defaultDuration; }
            set { _defaultDuration = value; }
        }

        [DataMember]
        public Byte[] BackgroundImg
        {
            get { return _backgroundImg; }
            set { _backgroundImg = value; }
        }


        // TODO: check other dto/business objects for sensible hashcode/equals implementations

        public override bool Equals(object obj)
        {
            DTOLocation other = obj as DTOLocation;
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

        public override string ToString()
        {
            return this.Name;
        }
    }
}
