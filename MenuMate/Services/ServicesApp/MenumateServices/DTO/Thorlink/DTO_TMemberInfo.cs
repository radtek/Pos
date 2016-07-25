using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.Thorlink
{
    [DataContract]
    public class DTO_TMemberInfo
    {

        [DataMember]
        public string responseDescription
        {
            get;
            set;
        }

        [DataMember]
        public string responseCode
        {
            get;
            set;
        }

        [DataMember]
        public string inquiryResponse
        {
            get;
            set;
        }

        [DataMember]
        public string traderId
        {
            get;
            set;
        }
        [DataMember]
        public string maxPurchaseTransaction
        {
            get;
            set;
        }

        [DataMember]
        public string maxCreditTransaction
        {
            get;
            set;
        }

        [DataMember]
        public string enrollmentDate
        {
            get;
            set;
        }

        [DataMember]
        public string enrollmentTraderId
        {
            get;
            set;
        }

        [DataMember]
        public string enrollmentTraderName
        {
            get;
            set;
        }

        [DataMember]
        public string mailingPreferenceId
        {
            get;
            set;
        }

        [DataMember]
        public string mailingPreferenceName
        {
            get;
            set;
        }

        [DataMember]
        public string traderRankId
        {
            get;
            set;
        }

        [DataMember]
        public string traderRankName
        {
            get;
            set;
        }

        [DataMember]
        public string traderStatusId
        {
            get;
            set;
        }

        [DataMember]
        public string traderStatusName
        {
            get;
            set;
        }

        [DataMember]
        public string traderTypeId
        {
            get;
            set;
        }

        [DataMember]
        public string traderTypeName
        {
            get;
            set;
        }

        [DataMember]
        public string firstName
        {
            get;
            set;
        }

        [DataMember]
        public string lastName
        {
            get;
            set;
        }

        [DataMember]
        public string emailAddress
        {
            get;
            set;
        }

        [DataMember]
        public string mobileNumber
        {
            get;
            set;
        }

        [DataMember]
        public string birthDate
        {
            get;
            set;
        }

        [DataMember]
        public string genderId
        {
            get;
            set;
        }

        [DataMember]
        public string genderName
        {
            get;
            set;
        }

        [DataMember]
        public string address
        {
            get;
            set;
        }

        [DataMember]
        public string city
        {
            get;
            set;
        }

        [DataMember]
        public string suburb
        {
            get;
            set;
        }

        [DataMember]
        public string state
        {
            get;
            set;
        }

        [DataMember]
        public string countryId
        {
            get;
            set;
        }

        [DataMember]
        public string country
        {
            get;
            set;
        }

        [DataMember]
        public string postalCode
        {
            get;
            set;
        }

        [DataMember]
        public double loyaltyBalance
        {
            get;
            set;
        }

        [DataMember]
        public double loyaltyBalanceMin
        {
            get;
            set;
        }

        [DataMember]
        public double loyaltyBalanceMax
        {
            get;
            set;
        }

        [DataMember]
        public double creditBalance
        {
            get;
            set;
        }

        [DataMember]
        public double creditBalanceMin
        {
            get;
            set;
        }

        [DataMember]
        public double creditBalanceMax
        {
            get;
            set;
        }

        [DataMember]
        public double voucherBalance
        {
            get;
            set;
        }

        [DataMember]
        public double voucherBalanceMin
        {
            get;
            set;
        }

        [DataMember]
        public double voucherBalanceMax
        {
            get;
            set;
        }

/*        [DataMember]
        public string id
        {
            get;
            set;
        }

        [DataMember]
        public int type
        {
            get;
            set;
        }

        [DataMember]
        public string code
        {
            get;
            set;
        }

        [DataMember]
        public string pluCode
        {
            get;
            set;
        }

        [DataMember]
        public string securityCode
        {
            get;
            set;
        }

        [DataMember]
        public string name
        {
            get;
            set;
        }

        [DataMember]
        public double value
        {
            get;
            set;
        }

        [DataMember]
        public int active
        {
            get;
            set;
        }

        [DataMember]
        public string merchantName
        {
            get;
            set;
        }

        [DataMember]
        public string imageUrl
        {
            get;
            set;
        }

        [DataMember]
        public string startDate
        {
            get;
            set;
        }

        [DataMember]
        public string endDate
        {
            get;
            set;
        }

        [DataMember]
        public string expiryDate
        {
            get;
            set;
        }

        [DataMember]
        public int availableInStore
        {
            get;
            set;
        } */

        [DataMember]
        public List<DTO_VoucherDetails> voucherDetailsOfThor
        {
            get;
            set;
        }

    }
}
