using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    [DataContract]
    public class DTO_ChefMateModelKeysResponse : DTO_ChefMateResponse
    {
        /// <summary>
        /// Data Transfer Object for responses to ChefMate database requests that return model keys.
        /// </summary>
        #region PUBLIC
        [DataMember]
        public int OrderKey { get; set; }

        [DataMember]
        public int CourseKey { get; set; }

        [DataMember]
        public int ServingCourseKey { get; set; }
        #endregion
    }
}
