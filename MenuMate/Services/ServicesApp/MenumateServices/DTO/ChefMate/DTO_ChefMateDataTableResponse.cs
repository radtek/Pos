using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Data Transfer Object for responses to ChefMate database requests that return data tables.
    /// </summary>
    [DataContract]
    public class DTO_ChefMateDataTableResponse : DTO_ChefMateResponse
    {
        #region PUBLIC
        [DataMember]
        public DataTable ReturnValue { get; set; }
        #endregion
    }
}
