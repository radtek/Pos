using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Enum containing all the types of terminals that ChefMate can be.
    /// </summary>
    [DataContract]
    public enum ChefMateTerminalType
    {
        [EnumMember]
        KITCHEN,
        [EnumMember]
        RUNNER
    };

    /// <summary>
    /// Data Transfer Object for the terminal struct used by ChefMate to store terminals in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateTerminal
    {
        #region PUBLIC
        [DataMember]
        public int TerminalKey{ get; set; }

        [DataMember]
        public string TerminalName{ get; set; }

        [DataMember]
        public string TerminalIpAddress{ get; set; }
        #endregion
    }
}
