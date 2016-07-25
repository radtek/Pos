using System;
using System.Runtime.Serialization;
using System.Text;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    [DataContract]
    public class DTOReservable
    {
        private int _id;
        private string _name;
        private int _maxGuests;
        private bool _available;
        private int _currentInterestLevel;

        private string _tableColor;

        private DTOLocation _parentLocation;

        private int _number;

        #region table plan data
        private double _x;
        private double _y;
        private double _width;
        private double _height;
        private double _rotationAngle;
        private string _shape;

        #endregion

        [DataMember]
        public double X
        {
            get { return _x; }
            set { _x = value; }
        }

        [DataMember]
        public double Y
        {
            get { return _y; }
            set { _y = value; }
        }

        [DataMember]
        public double Width
        {
            get { return _width; }
            set { _width = value; }
        }

        [DataMember]
        public double Height
        {
            get { return _height; }
            set { _height = value; }
        }

        [DataMember]
        public double RotationAngle
        {
            get { return _rotationAngle; }
            set { _rotationAngle = value; }
        }

        [DataMember]
        public string Shape
        {
            get { return _shape; }
            set { _shape = value; }
        }

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [DataMember]
        public string TableColor
        {
            get { return _tableColor; }
            set { _tableColor = value; }
        }

        [DataMember]
        public DTOLocation ParentLocation
        {
            get { return _parentLocation; }
            set { _parentLocation = value; }
        }

        [DataMember]
        public int Number
        {
            get { return _number; }
            set { _number = value; }
        }

        public DTOReservable()
        {

        }

        [DataMember]
        public String Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [DataMember]
        public int MaxGuests
        {
            get { return _maxGuests; }
            set { _maxGuests = value; }
        }

        [DataMember]
        public bool Available
        {
            get { return _available; }
            set { _available = value; }
        }



        [DataMember]
        public int CurrentInterestLevel
        {
            get { return _currentInterestLevel; }
            set { _currentInterestLevel = value; }
        }

        public override string ToString()
        {
            StringBuilder b = new StringBuilder(this.Name);
            if (this.CurrentInterestLevel > 1)
            {
                b.Append(" *");
            }
            return b.ToString();
        }

        public override bool Equals(object obj)
        {
            bool b = false;
            DTOReservable other = obj as DTOReservable;
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
