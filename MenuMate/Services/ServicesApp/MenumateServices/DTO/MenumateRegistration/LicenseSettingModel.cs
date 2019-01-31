using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.MenumateRegistration
{
    [DataContract]
    public class LicenceSettingModel
    {
        [DataMember]
        public string SettingType { get; set; }
        [DataMember]
        public string SettingSubType { get; set; }
        [DataMember]
        public bool IsActive { get; set; }
    }
}
