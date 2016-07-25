using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MenumateServices.ChefMate.DTO;
using System.Runtime.Serialization;

namespace MenumateServices.ChefMate.DTO
{
    [DataContract]
    public class DTO_ChefMateTerminalNamesResponse : DTO_ChefMateResponse
    {
        public DTO_ChefMateTerminalNamesResponse()
        {
            AssignedTerminalName = new DTO_ChefmateTerminalName();
            AvailableTerminalNames = new List<DTO_ChefmateTerminalName>();
        }

        [DataMember]
        public DTO_ChefmateTerminalName AssignedTerminalName { get; set; }

        [DataMember]
        public List<DTO_ChefmateTerminalName> AvailableTerminalNames { get; set; }
    }
}
