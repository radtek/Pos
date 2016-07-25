using System.Runtime.Serialization;

namespace Menumate.WinServices.Implementation.Contracts.DTO
{
    public enum ReservationViews { Occupancy, Session, Day };

    [DataContract]
    public class DTOReserver
    {

        private int _id = 0;

        private string _guestName;
        private string _guestPhone;
        private string _guestEmail;

        public DTOReserver()
        {
            _id = 0;
        }

        #region Reserver Details

        [DataMember]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [DataMember]
        public string GuestName
        {
            get { return _guestName; }
            set { _guestName = value; }
        }

        [DataMember]
        public string GuestPhone
        {
            get { return _guestPhone; }
            set { _guestPhone = value; }
        }

        [DataMember]
        public string GuestEmail
        {
            get { return _guestEmail; }
            set { _guestEmail = value; }
        }

        #endregion

        public override string ToString()
        {
            return this.GuestName;
        }

        public override bool Equals(object obj)
        {
            DTOReserver other = obj as DTOReserver;
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
