using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

using MenumateServices.DTO.LoyaltyMate;

namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IWCFServiceLoyaltyMate" in both code and config file together.
    [ServiceContract]
    public interface IWCFServiceLoyaltyMate
    {
        // Member Operations
        [OperationContract]
        LoyaltyMemberResponse SaveMember(string inSyndicateCode, MemberInfo inInfo);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByUniqueId(string inSyndicateCode, RequestInfo requestInfo);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByCardCode(string inSyndicateCode, RequestInfo requestInfo);

        [OperationContract]
        LoyaltyMemberResponse GetMemberByEmail(string inSyndicateCode, RequestInfo requestInfo);

        [OperationContract]
        LoyaltyResponse UpdateMemberCardCode(string inSyndicateCode, string uniqueId, string memberCardCode);

        [OperationContract]
        LoyaltyResponse PostTransaction(string inSyndicateCode, TransactionInfo transaction);

        [OperationContract]
        LoyaltyCompanyResponse GetCompanyInformation(string inSyndicateCode);

        [OperationContract]
        LoyaltyGiftCardResponse GetGiftCardBalance(string inSyndicateCode, RequestInfo requestInfo);

        [OperationContract]
        LoyaltyVoucherResponse GetPocketVoucherDetail(string inSyndicateCode, RequestInfo requestInfo);

        [OperationContract]
        VoucherTransactionResponse ProcessVoucherTransaction(string inSyndicateCode, VoucherTransactionInfo transaction);

        [OperationContract]
        LoyaltyResponse ReleaseVouchers(string inSyndicateCode, ReleasedVoucherInfo releasedVoucherInfo);
    }
}
