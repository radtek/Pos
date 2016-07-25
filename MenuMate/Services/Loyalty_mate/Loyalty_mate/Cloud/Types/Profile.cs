using System;

namespace Loyalty_mate.Cloud.Types
{

    [Serializable]
    public sealed class Profile
    {
        public
        Profile()
        {
        }

        public
        Profile(string name,
                string cell_phone,
                Nullable<DateTime> date_of_birth,
                string title, string first_name,
                string last_name, string address1,
                string address2,string city,
                string state, string zip_code,
                string country, string phone_number)
        {
            this.name = name;
            this.cell_phone = cell_phone;
            this.date_of_birth = date_of_birth;
            this.title = title;
            this.first_name = first_name;
            this.last_name = last_name;
            this.address1 = address1;
            this.address2 = address2;
            this.city = city;
            this.state = state;
            this.zip_code = zip_code;
            this.country = country;
            this.phone_number = phone_number;
        }

        public string name
        {
            get;
            set;
        }

        public string cell_phone
        {
            get;
            set;
        }

        public Nullable<DateTime> date_of_birth
        {
            get;
            set;
        }

        public string title
        {
            get;
            set;
        }
        public string first_name
        {
            get;
            set;
        }
        public string last_name
        {
            get;
            set;
        }
        public string address1
        {
            get;
            set;
        }
        public string address2
        {
            get;
            set;
        }
        public string city
        {
            get;
            set;
        }
        public string state
        {
            get;
            set;
        }
        public string zip_code
        {
            get;
            set;
        }
        public string country
        {
            get;
            set;
        }
        public string phone_number { get; set; }





    } /* Profile */

} /* Loyalty_mate.Cloud */
