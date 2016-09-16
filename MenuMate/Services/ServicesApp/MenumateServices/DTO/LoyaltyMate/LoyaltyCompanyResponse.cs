using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyCompanyResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        CompanyInfo _companyInfo;

        internal LoyaltyCompanyResponse(bool inSuccesful, string inMessage, string inDescription, LoyaltyResponseCode inResponseCode,
                        CompanyInfo inCompanyInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _companyInfo = CreateCompanyInfo(inCompanyInfo);
        }

         CompanyInfo CreateCompanyInfo(CompanyInfo inCompanyInfo)
        {
            var result = new CompanyInfo();
            result.Discounts = inCompanyInfo.Discounts;
            result.TierLevels = inCompanyInfo.TierLevels;
            result.HasGiftCardsAvailable = inCompanyInfo.HasGiftCardsAvailable;
            result.HasPocketVouchersAvailable = inCompanyInfo.HasPocketVouchersAvailable;
            return result;
        }

         [DataMember]
         public LoyaltyResponseCode ResponseCode
         {
             get { return _responseCode; }
             set { _responseCode = value; }
         }

         [DataMember]
         public CompanyInfo CompanyInfo
         {
             get { return _companyInfo; }
             set { _companyInfo = value; }
         }

    }
}
