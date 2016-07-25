using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace MenumateServices.WebMate.DTO
{
    public class DTO_WebOrderDelivery
    {
        #region PUBLIC
        [DataMember]
        public string Recipient { get; set; }

        [DataMember]
        public string StreetNo { get; set; }

        [DataMember]
        public string StreetName { get; set; }

        [DataMember]
        public string Suburb { get; set; }

        [DataMember]
        public string City { get; set; }

        [DataMember]
        public string Country { get; set; }

        [DataMember]
        public string Note { get; set; }

        [DataMember]
        public string Phone { get; set; }
        #endregion    
    }
}
