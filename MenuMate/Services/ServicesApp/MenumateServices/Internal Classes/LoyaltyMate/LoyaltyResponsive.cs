using System.Collections.Generic;
using MenumateServices.DTO.LoyaltyMate;

namespace MenumateServices.LoyaltyMate
{
    public class LoyaltyResponsive
    {
        protected LoyaltyResponse CreateResponseNoError()
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful);
        }

        protected LoyaltyResponse CreateResponseError(string inMessage, string inDescription, LoyaltyResponseCode inResponseCode)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode);
        }

        //Member Response
        protected LoyaltyMemberResponse CreateMemberResponseNoError(MemberInfo inMemberInfo)
        {
            return CreateMemberResponse(true, "", "", LoyaltyResponseCode.Successful, inMemberInfo);
        }

        protected LoyaltyMemberResponse CreateMemberResponseError(string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, MemberInfo inMemberInfo)
        {
            return CreateMemberResponse(false, inMessage, inDescription, inResponseCode, inMemberInfo);
        }

        //Company Response
        protected LoyaltyCompanyResponse CreateCompanyResponseNoError(CompanyInfo inCompanyInfo)
        {
            return CreateCompanyResponse(true, "", "", LoyaltyResponseCode.Successful, inCompanyInfo);
        }

        protected LoyaltyCompanyResponse CreateCompanyResponseError(string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, CompanyInfo inCompanyInfo)
        {
            return CreateCompanyResponse(false, inMessage, inDescription, inResponseCode, inCompanyInfo);
        }

        //GiftCard Response
        protected LoyaltyGiftCardResponse CreateGiftCardResponseNoError(GiftCardInfo giftCardInfo)
        {
            return CreateGiftCardResponse(true, "", "", LoyaltyResponseCode.Successful, giftCardInfo);
        }

        protected LoyaltyGiftCardResponse CreateGiftCardResponseError(string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, GiftCardInfo giftCardInfo)
        {
            return CreateGiftCardResponse(false, inMessage, inDescription, inResponseCode, giftCardInfo);
        }

        //Voucher Response
        protected LoyaltyVoucherResponse CreateVoucherResponseNoError(VoucherInfo inVoucherInfo)
        {
            return CreateVoucherResponse(true, "", "", LoyaltyResponseCode.Successful, inVoucherInfo);
        }

        protected LoyaltyVoucherResponse CreateVoucherResponseError(string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, VoucherInfo inVoucherInfo)
        {
            return CreateVoucherResponse(false, inMessage, inDescription, inResponseCode, inVoucherInfo);
        }


        #region Private Methods

        private LoyaltyResponse CreateResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode)
        {
            return new LoyaltyResponse(inSuccesful, inMessage, inDescription, inResponseCode);
        }

        private LoyaltyMemberResponse CreateMemberResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, MemberInfo inMemberInfo)
        {
            return new LoyaltyMemberResponse(inSuccesful, inMessage, inDescription, inResponseCode, inMemberInfo);
        }

        private LoyaltyCompanyResponse CreateCompanyResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, CompanyInfo inCompanyInfo)
        {
            return new LoyaltyCompanyResponse(inSuccesful, inMessage, inDescription, inResponseCode, inCompanyInfo);
        }

        private LoyaltyGiftCardResponse CreateGiftCardResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, GiftCardInfo giftCardInfo)
        {
            return new LoyaltyGiftCardResponse(inSuccesful, inMessage, inDescription, inResponseCode, giftCardInfo);
        }

        private LoyaltyVoucherResponse CreateVoucherResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode, VoucherInfo inVoucherInfo)
        {
            return new LoyaltyVoucherResponse(inSuccesful, inMessage, inDescription, inResponseCode, inVoucherInfo);
        }
        
        #endregion
    }
}
