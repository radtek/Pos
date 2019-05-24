using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.MenumateOnlineOrdering
{
    public enum OOLoyaltyResponseCode
    {
        Successful = 0,
        AuthenticationFailed,
        CreateMemberFailed,
        UpdateMemberFailed,
        DeleteMemberFailed,
        GetMemberFailed,
        PostTransactionFailed,
        MemberNotExist,
        CompanySyncFailed,
        InvalidGiftVoucher,
        InvalidPocketVoucher,
        TransactionFailed,
        GetGiftCardFailed,
        GetPocketVoucherFailed,
        MenuSyncingFailed,
        TaxSettingSyncingFailed,
        UpdateOnlineOrderStatusFailed,
        PostOnlineOrderInvoiceInfoFailed,
        MultipleGUIDExist,
        GUIDNotFound,
        SendZedRequestNotificationFailed
    };
    [DataContract]
    public class OOLoyaltyResponse : OOServiceResponse
    {
        OOLoyaltyResponseCode _responseCode;

        internal OOLoyaltyResponse(bool inSuccesful, string inMessage, string inDescription, OOLoyaltyResponseCode inResponseCode)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
        }

        [DataMember]
        public OOLoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }
    }
}
