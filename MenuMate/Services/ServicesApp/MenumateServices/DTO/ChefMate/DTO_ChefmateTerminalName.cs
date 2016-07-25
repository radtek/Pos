using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.ChefMate.DTO
{
    [DataContract]
    public class DTO_ChefmateTerminalName
    {
        public DTO_ChefmateTerminalName()
        {
            Key = 0;
            Name = "";
            Type = DTO_ChefmateTerminalNameType.COMPUTERNAME;
        }

        [DataMember(Order=1)]
        public int Key { get; set; }

        [DataMember(Order = 2)]
        public string Name { get; set; }

        [DataMember(Order = 3)]
        public DTO_ChefmateTerminalNameType Type { get; set; }
    }
}
