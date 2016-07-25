using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using NLog;

using LogFileManager;
//using SummaCL.Global;

namespace SummaCL
{
    public class CustomersCSV : CsvMulti {

        private FileLogger _singletonLogger = FileLogger.Instance;

        public CustomersCSV(string basePath) 
        {
            //FULLFILEPATH = basePath + "Customers.txt";
            FULLFILEPATH = basePath;

            fieldLengths = new int[] { 10, 100, 100, 100, 100, 100, 1, 20, 20, 100, 10, 1, 100, 100, 
                100, 50, 50, 50, 100, 100, 
                100, 50, 50, 50, 100, 100, 100, 100, 100, 100, 10, 10, 10, 10, 100, 100, 100, 10, 
                2, 5, 10, 10, 10, 10, 10, 10, 10, 
                10, 100, 100, 20, 30, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 4000, 20, 
                20, 1, 1, 10};

            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false, 
                false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, 
                false, false, false, false, false, false, false, false, false, false, false, false, 
                false, false, false, false, false, 
                false, false, false, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();

            AppendToFile = true;
        }
        public override void SetupData(XmlDocument doc)
        {
            XmlNodeList customers = null;
            try 
            {
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.UpdateMemberXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.Customers, SummaCommon.GetStoreId(doc));

                customers = doc.DocumentElement.SelectNodes("/UpdateMember/Member");

                int counter = 0;
                foreach (XmlNode customerNode in customers) 
                {
                    csvData.Add(new List<string>());

                    csvData[counter].Add(GetCustomerId(customerNode));

                    //string id = GetCustomerId(customerNode);
                    //CheckCustomerID.Check(id);
                    //csvData[counter].Add(id);

                    csvData[counter].Add(GetCustomerTitle(customerNode));
                    csvData[counter].Add(GetFirstName(customerNode));
                    csvData[counter].Add(GetMiddleName(customerNode));
                    csvData[counter].Add(GetLastName(customerNode));
                    csvData[counter].Add(GetNickName(customerNode));
                    csvData[counter].Add(GetGender(customerNode));
                    csvData[counter].Add(GetCustomerDOB(customerNode));
                    csvData[counter].Add(GetEnrolmentDate(customerNode));
                    csvData[counter].Add(GetCustomerCode(customerNode));
                    csvData[counter].Add(GetExternalCustomerId(customerNode));
                    csvData[counter].Add(GetActiveStatus(customerNode));
                    csvData[counter].Add(GetAddress1(customerNode));
                    csvData[counter].Add(GetAddress2(customerNode));
                    csvData[counter].Add(GetSuburb(customerNode));
                    csvData[counter].Add(GetCity(customerNode));
                    csvData[counter].Add(GetState(customerNode));
                    csvData[counter].Add(GetCountry(customerNode));
                    csvData[counter].Add(GetPostalAddress1(customerNode));
                    csvData[counter].Add(GetPostalAddress2(customerNode));
                    csvData[counter].Add(GetPostalAddressSuburb(customerNode));
                    csvData[counter].Add(GetPostalAddressCity(customerNode));
                    csvData[counter].Add(GetPostalAddressState(customerNode));
                    csvData[counter].Add(GetPostalAddressCountry(customerNode));
                    csvData[counter].Add(PostalCode(customerNode));
                    csvData[counter].Add(GetPhoneMobile(customerNode));
                    csvData[counter].Add(GetPhoneWork(customerNode));
                    csvData[counter].Add(GetPhoneHome(customerNode));
                    csvData[counter].Add(GetFax(customerNode));
                    csvData[counter].Add(GetEmail(customerNode));
                    csvData[counter].Add(GetBankAccount1(customerNode));
                    csvData[counter].Add(GetBankAccount2(customerNode));
                    csvData[counter].Add(GetBankAccount3(customerNode));
                    csvData[counter].Add(GetBankAccount4(customerNode));
                    csvData[counter].Add(GetWebsite(customerNode));
                    csvData[counter].Add(GetCompany(customerNode));
                    csvData[counter].Add(GetJobTitle(customerNode));
                    csvData[counter].Add(GetEthnicityId(customerNode));
                    csvData[counter].Add(GetMarriedStatus(customerNode));
                    csvData[counter].Add(GetNumberOfChildren(customerNode));
                    csvData[counter].Add(GetRankingId(customerNode));
                    csvData[counter].Add(GetIncomeId(customerNode));
                    csvData[counter].Add(GetOccupationId(customerNode));
                    csvData[counter].Add(GetCustomerGroupId(customerNode));
                    csvData[counter].Add(GetCurrentBalance(customerNode));
                    csvData[counter].Add(GetRewardBalance(customerNode));
                    csvData[counter].Add(GetGiftBalance(customerNode));
                    csvData[counter].Add(GetVoucherBalance(customerNode));
                    csvData[counter].Add(GetLogin(customerNode));
                    csvData[counter].Add(GetPassword(customerNode));
                    csvData[counter].Add(GetLastModified(customerNode));
                    csvData[counter].Add(GetIPAddress(customerNode));
                    csvData[counter].Add(GetExtra1(customerNode));
                    csvData[counter].Add(GetExtra2(customerNode));
                    csvData[counter].Add(GetExtra3(customerNode));
                    csvData[counter].Add(GetExtra4(customerNode));
                    csvData[counter].Add(GetExtra5(customerNode));
                    csvData[counter].Add(GetExtra6(customerNode));
                    csvData[counter].Add(GetExtra7(customerNode));
                    csvData[counter].Add(GetExtra8(customerNode));
                    csvData[counter].Add(GetExtra9(customerNode));
                    csvData[counter].Add(GetExtra10(customerNode));
                    csvData[counter].Add(GetComments(customerNode));
                    csvData[counter].Add(GetCardId(customerNode));
                    csvData[counter].Add(GetCardExpiryDate(customerNode));
                    csvData[counter].Add(GetCardActiveStatus(customerNode));
                    csvData[counter].Add(GetCardPrintStatus(customerNode));

                    csvData[counter].Add(GetStoreId(doc.DocumentElement));
                    //csvData[counter].Add(GetStoreId(customerNode));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > customers.Count))
                    {
                        csvLineData.Add(new StringBuilder());
                    }
                    else
                    {
                        csvLineData.Add(new StringBuilder(","));
                    }
*/
                }
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem processing customer", e);
                se.Data = doc.OuterXml;
                throw se;
            }

        }

        private string GetStoreId(XmlNode customerNode) 
        {
            return Summa.GetSummaSiteId(customerNode.Attributes["SiteID"].Value);
        }

        private string GetCardPrintStatus(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCardActiveStatus(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCardExpiryDate(XmlNode customerNode) {
            return string.Empty;
        }

		/// <summary>
		/// Returns the FIRST card in the cards collection associated with this
		/// customer.
		/// </summary>
		/// <param name="customerNode"></param>
		/// <returns></returns>
        private string GetCardId(XmlNode customerNode) {
            XmlNodeList cards = null;
            try {
                cards = customerNode.SelectNodes("/MagCards/MagCard");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            string cardId = null;
            if (cards.Count > 0) {
                cardId = cards[0].Attributes["Card"].Value;
            }
            else {
                cardId = string.Empty;
            }

            return cardId;
        }

        private string GetComments(XmlNode customerNode) {
			// Removed 07/02/2011 to prevent cr/lf characters from being exported to Summa.
            //return customerNode.Attributes["Note"].Value;
			return string.Empty;
        }

        private string GetExtra10(XmlNode customerNode) {
            return string.Empty;
        }
        private string GetExtra9(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra8(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra7(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra6(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra5(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra4(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra3(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra2(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetExtra1(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetIPAddress(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetLastModified(XmlNode customerNode) {
            return customerNode.Attributes["LastMod"].Value;
        }

        private string GetPassword(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetLogin(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetVoucherBalance(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetGiftBalance(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetRewardBalance(XmlNode customerNode) {
            return customerNode.Attributes["PointsTotal"].Value;
        }

        private string GetCurrentBalance(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCustomerGroupId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetOccupationId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetIncomeId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetRankingId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetNumberOfChildren(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetMarriedStatus(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetEthnicityId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetJobTitle(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCompany(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetWebsite(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetBankAccount4(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetBankAccount3(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetBankAccount2(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetBankAccount1(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetEmail(XmlNode customerNode) {
            return customerNode.Attributes["Email"].Value;
        }

        private string GetFax(XmlNode customerNode) {
            return customerNode.Attributes["Fax"].Value;
        }

        private string GetPhoneHome(XmlNode customerNode) {
            return customerNode.Attributes["Phone"].Value;
        }

        private string GetPhoneWork(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPhoneMobile(XmlNode customerNode) {
            return customerNode.Attributes["Mobile"].Value;
        }

        private string PostalCode(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddressCountry(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddressState(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddressCity(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddressSuburb(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddress2(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetPostalAddress1(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCountry(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetState(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCity(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetSuburb(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetAddress2(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetAddress1(XmlNode customerNode) {
            return string.Empty;
        }

        /// <summary>
        /// Menumate customer types;
        /// 2 = 
        /// 3 = 
        /// 4 = 
        /// 5 = 
        /// </summary>
        /// <param name="customerNode"></param>
        /// <returns></returns>
        private string GetActiveStatus(XmlNode customerNode) {
            string mmType = customerNode.Attributes["Type"].Value;
            string summaType = null;
            if (mmType.Equals("2") || mmType.Equals("4")) {
                summaType = "1";
            }
            else if (mmType.Equals("3") || mmType.Equals("5")) {
                summaType = "0";
            }
            return summaType;
        }

        private string GetExternalCustomerId(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCustomerCode(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetEnrolmentDate(XmlNode customerNode) {
            return string.Empty;
        }

        private string GetCustomerDOB(XmlNode customerNode) {
            string dateString = null;
            DateTime customerDob = DateTime.MinValue;
            string returnString = string.Empty;

            try {
                dateString = customerNode.Attributes["DOB"].Value;
                if (string.IsNullOrEmpty(dateString)) 
                {
                    Summa.SingletonLoggerStatic.Log("No DOB supplied");
                    //Summa.LOGGER.Log(LogLevel.Debug, "No DOB supplied");
                }
                else {
                    customerDob = DateTime.Parse(dateString);
                    returnString = customerDob.ToString("yyyyMMdd");
                }
            }
            catch (FormatException fe) 
            {
                _singletonLogger.LogError(FileLogger.LogErrorLevel.InternalXMLError, FileLogger.LogErrorLevelFile.UpdateMemberXMLFile,
                                          string.Format("Unable to convert date;[{0}]. Returning empty field!{1}{2}", dateString, Environment.NewLine, fe.ToString()),
                                          @"");
                //Summa.LOGGER.Log(LogLevel.Error, string.Format("Unable to convert date;[{0}]. Returning empty field!{1}{2}",
                //    dateString, Environment.NewLine, fe.ToString()));
            }
            return returnString;
        }

        private string GetNickName(XmlNode customerNode) {
            return customerNode.Attributes["KnownAs"].Value;
        }

        private string GetGender(XmlNode customerNode) {
            string gender = customerNode.Attributes["Sex"].Value;
            string summaGender = null;
            if (gender.Equals("Male")) {
                summaGender = "1";
            }
            else {
                summaGender = "0";
            }
            return summaGender;
        }

        private string GetLastName(XmlNode customerNode) {
            string fullName = customerNode.Attributes["Name"].Value;
            string[] nameParts = fullName.Split(new char[] { ' ' });
            string lastName = null;

            if (nameParts.Length == 1) {
                lastName = "";
            }
            else {
                lastName = nameParts[nameParts.Length - 1];
            }
            return lastName;
        }

        private string GetMiddleName(XmlNode customerNode) {
            string fullName = customerNode.Attributes["Name"].Value;
            string[] nameParts = fullName.Split(new char[] { ' ' });
            string middleName = null;

            if (nameParts.Length < 3) {
                middleName = "";
            }
            else {
                middleName = nameParts[1];
            }
            return middleName;
        }

        private string GetFirstName(XmlNode customerNode) {
            string fullName = customerNode.Attributes["Name"].Value;
            string[] nameParts = fullName.Split(new char[] { ' ' });
            return nameParts[0];
        }

        private string GetCustomerTitle(XmlNode customerNode) {
            return customerNode.Attributes["Title"].Value;
        }

        /// <summary>
        /// Customer Id formatted as XXXXYYYYYY where XXXX is the menumate site id
        /// for the customer (not the site id for the transaction), followed by the 
        /// menumate member number.
        /// </summary>
        /// <param name="customerNode"></param>
        /// <returns></returns>
        private string GetCustomerId(XmlNode customerNode) 
        {
            /*
            int siteid = Int32.Parse(customerNode.Attributes["SiteID"].Value);
            StringBuilder customerId = new StringBuilder(string.Format("{0:D4}", siteid));
            customerId.Append(SummaCommon.StringToUnsignedHashValue(customerNode.Attributes["MemNumber"].Value));
            
            return customerId.ToString();
            */

            return customerNode.Attributes["MemNumber"].Value;       
        }
    }
}
