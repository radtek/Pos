using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.ChefMate.DTO
{
    /// <summary>
    /// Enum containing all the response codes to be provided to ChefMate.
    /// </summary>
    [DataContract]
    public enum ResponseCode
    {
        [EnumMember]
        Successful,
        [EnumMember]
        FirebirdError,
        [EnumMember]
        KeyGeneratorError,
        [EnumMember]
        ScalerFailureError,
        [EnumMember]
        UnknownError
    };

    /// <summary>
    /// Base Data Transfer Object for responses to ChefMate database requests.
    /// </summary>
    [DataContract]
    [KnownType(typeof(DTO_ChefMateBooleanResponse))]
    [KnownType(typeof(DTO_ChefMateDataTableResponse))]
    [KnownType(typeof(DTO_ChefMateDateTimeResponse))]
    [KnownType(typeof(DTO_ChefMateIntegerListResponse))]
    [KnownType(typeof(DTO_ChefMateIntegerResponse))]
    [KnownType(typeof(DTO_ChefMateOrderGroupResponse))]
    [KnownType(typeof(DTO_ChefMateOrderItemOptionResponse))]
    [KnownType(typeof(DTO_ChefMateOrderItemResponse))]
    [KnownType(typeof(DTO_ChefMateOrderItemSideResponse))]
    [KnownType(typeof(DTO_ChefMateOrderResponse))]
    [KnownType(typeof(DTO_ChefMateStringResponse))]
    [KnownType(typeof(DTO_ChefMateModelKeysResponse))]
    public class DTO_ChefMateResponse
    {
        #region PUBLIC
        [DataMember]
        public bool Successful { get; set; }

        [DataMember]
        public string Message { get; set; }

        [DataMember]
        public string Description { get; set; }

        [DataMember]
        public ResponseCode Response { get; set; }
        #endregion
    }
}
