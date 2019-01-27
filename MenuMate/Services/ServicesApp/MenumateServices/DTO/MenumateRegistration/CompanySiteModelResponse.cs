using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class CompanySiteModelResponse
    {
        [DataMember]
        public string CompanyName { get; set; }
        [DataMember]
        public bool IsSuccessful { get; set; }
        [DataMember]
        public string Message { get; set; }       
    }
}
