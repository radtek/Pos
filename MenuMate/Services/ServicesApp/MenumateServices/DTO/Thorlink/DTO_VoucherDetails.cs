using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_VoucherDetails
    {
        [DataMember]
        public string id
        {
            get;
            set;
        }

        [DataMember]
        public int type
        {
            get;
            set;
        }

        [DataMember]
        public string code
        {
            get;
            set;
        }

        [DataMember]
        public string pluCode
        {
            get;
            set;
        }

        [DataMember]
        public string securityCode
        {
            get;
            set;
        }

        [DataMember]
        public string name
        {
            get;
            set;
        }

        [DataMember]
        public double value
        {
            get;
            set;
        }

        [DataMember]
        public int active
        {
            get;
            set;
        }

        [DataMember]
        public string merchantName
        {
            get;
            set;
        }

        [DataMember]
        public string imageUrl
        {
            get;
            set;
        }

        [DataMember]
        public string startDate
        {
            get;
            set;
        }

        [DataMember]
        public string endDate
        {
            get;
            set;
        }

        [DataMember]
        public string expiryDate
        {
            get;
            set;
        }

        [DataMember]
        public int availableInStore
        {
            get;
            set;
        }
    }
}
