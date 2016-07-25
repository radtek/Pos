using System;
using System.Collections.Generic;
using Castle.ActiveRecord;

namespace Menumate.WinServices.Implementation.Contracts.BusinessObjects
{
    [ActiveRecord("Guests")]
    public class Guest : ActiveRecordBase<Guest>
    {
        private String _name;
        private String _phone;
        private String _email;
        private int _id;

        public Guest()
        {
        }

        public Guest(String name, String phone, String email)
        {
            _name = name;
            _phone = phone;
            _email = email;
        }

        [PrimaryKey]
        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        [Property]
        public String Name
        {
            get { return _name; }
            set { _name = value; }
        }

        [Property]
        public String Phone
        {
            get { return _phone; }
            set { _phone = value; }
        }

        [Property]
        public String Email
        {
            get { return _email; }
            set { _email = value; }
        }

        public override string ToString()
        {
            return _name;
        }

        public override bool Equals(object obj)
        {
            bool areEqual = false;
            Guest g = obj as Guest;
            if (g != null)
            {
                if (this.Id == g.Id)
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

    public class GuestComparer : IEqualityComparer<Guest>
    {
        public bool Equals(Guest x, Guest y)
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

        public int GetHashCode(Guest obj)
        {
            return obj.Name.GetHashCode();
        }
    }
}
