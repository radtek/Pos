using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Enum containing all the group types that ChefMate uses.
    /// </summary>
    [DataContract]
    public enum ChefMateGroupType
    {
        [EnumMember]
        COURSE,
        [EnumMember]
        SERVING_COURSE
    };

    /// <summary>
    /// Data Transfer Object for the order group struct used by ChefMate to store and retrieve order groups in the Database.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateOrderGroup
    {
        #region PUBLIC
        [DataMember]
        public int OrderGroupKey { get; set; }

        [DataMember]
        public int GroupPosKey { get; set; }

        [DataMember]
        public string GroupName { get; set; }

        [DataMember]
        public ChefMateGroupType GroupType { get; set; }

        [DataMember]
        public int GroupDisplayOrder { get; set; }

        [DataMember]
        public bool GroupCalledAway { get; set; }
        #endregion
    }
}
