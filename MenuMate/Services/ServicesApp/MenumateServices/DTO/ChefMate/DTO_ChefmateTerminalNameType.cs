using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.ChefMate.DTO
{
    [DataContract]
    public enum DTO_ChefmateTerminalNameType
    {
        [EnumMember]
        COMPUTERNAME = 0,

        [EnumMember]
        USERADDEDNAME
    }
}
