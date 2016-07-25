using System;
using System.Globalization;
using System.Text;
using System.Xml;
using System.Xml.XPath;

using LogFileManager;

namespace SummaCL
{

    public static class SummaCommon {

        public static string GetMessageId(XmlDocument document) {
            string rv;
            if (document.DocumentElement.Attributes["id"] == null) {
                rv = String.Empty;
            }
            else {
                rv = document.DocumentElement.Attributes["id"].Value;
            }

            return rv;
        }

        public static string GetStoreId(XmlDocument doc) 
        {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetPOSVersion(XmlDocument doc)
        {
            return doc.DocumentElement.Attributes["POSVersion"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetInterfaceVersion(XmlDocument doc)
        {
            return doc.DocumentElement.Attributes["InterfaceVersion"].Value;
        }

        public static string GetTillId(XmlDocument doc) 
        {
            return doc.DocumentElement.Attributes["TerminalId"].Value;
        }

		private static string DATE_FORMAT_STRING = "yyyyMMdd";
		private static string TIME_FORMAT_STRING = "HHmmss";

		public static string GetSummaFormattedTimestampForCurrentDateTime() {
			StringBuilder b = new StringBuilder(DATE_FORMAT_STRING);
			b.Append(TIME_FORMAT_STRING);
			return DateTime.Now.ToString(b.ToString());
		}

        public static string GetSummaFormattedTimestamp(string dateString, string timeString) {

            // date and times are sent from menumate in dd/mm/yyy hh:mm:ss format, so 
            // force the conversion to be in a compatible format i.e., nz regardless of
            // the actual settings on the local machine
            DateTime date = DateTime.Parse(dateString, CultureInfo.CreateSpecificCulture("en-NZ"));
            DateTime time = DateTime.Parse(timeString, CultureInfo.CreateSpecificCulture("en-NZ"));

			StringBuilder timestampTemp = new StringBuilder(date.ToString(DATE_FORMAT_STRING));
			timestampTemp.Append(time.ToString(TIME_FORMAT_STRING));

            return timestampTemp.ToString();
        }

        public static string StringToUnsignedHashValue(string p)
        {
            byte[] bytes = BitConverter.GetBytes(p.GetHashCode());
            UInt64 result = BitConverter.ToUInt32(bytes, 0);

            return result.ToString();
        }
    }

    /// <summary>
    /// Implements xml data lookups for Summa
    /// </summary>
    public static class SummaTransactionCommon 
    {
        public static string GetTransactionPaymentID(XmlNode inPaymentNode)
        {
            XmlNode transactionNode = null;

            try
            {
                transactionNode = inPaymentNode.SelectSingleNode("/Transaction");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            return IDGenerator.GetTransactionDetailID(transactionNode.Attributes["id"].Value, inPaymentNode.Attributes["Name"].Value);
        }

        /// <summary>
        /// Builds Transaction Id from summa site Id and Receipt Number, or returns 9999999999.
        /// NOTE: Does not use the ID attribute of the Transaction element!
        /// Id is len 10 now.
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public static string GetTransactionID(XmlDocument document) 
        {
            string result = document.DocumentElement.Attributes["id"].Value;
    
            if (result.Length > 10) 
            {
                Summa.SingletonLoggerStatic.Log(string.Format("Txn Id will be truncated: {0}", result));
            }

            return result;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public static string GetTerminalID(XmlDocument document) {

            XmlNode terminalNode = null;
            try {
                terminalNode = document.DocumentElement.SelectSingleNode("/Transaction/Terminal");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return terminalNode.Attributes["TerminalId"].Value;
        }

        /// <summary>
        /// Customer Id formatted as XXXXYYYYYY where XXXX is the menumate site id
        /// for the customer (not the site id for the transaction), followed by the 
        /// menumate member number.
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetCustomerId(XmlDocument doc) 
        {

            XmlNode membershipNode = null;
            StringBuilder customerId = null;

            try
            {
                membershipNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt/Member");
                if (membershipNode != null) 
                {
                    customerId = new StringBuilder(membershipNode.Attributes["MemNumber"].Value);

                    /*
                    int siteid = Int32.Parse(membershipNode.Attributes["SiteID"].Value);
                    customerId = new StringBuilder(string.Format("{0:D4}", siteid));
                    customerId.Append(SummaCommon.StringToUnsignedHashValue(membershipNode.Attributes["MemNumber"].Value));
                     */
                }
            }
            catch (XPathException xpe) 
            {
                customerId = null;
                // logging!
            }

            if (customerId == null) 
            {
                customerId = new StringBuilder();
            }

            //...............................

            return customerId.ToString();
        }

        #region Old Specs
        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetSalesTotal(XmlDocument doc) {
            XmlNode receiptNode = null;
            try {
               receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return receiptNode.Attributes["TotalPayment"].Value;
        }
        #endregion

        #region Specs  v1.2
        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetSalesTotalXTax(XmlDocument doc)
        {
            XmlNode receiptNode = null;
            try 
            {
               receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) 
            {
                throw xpe;
            }

            return receiptNode.Attributes["TotalXTax"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetSalesTotalTaxAmount(XmlDocument doc)
        {
            XmlNode receiptNode = null;
            try 
            {
               receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) 
            {
                throw xpe;
            }

            return receiptNode.Attributes["TotalTax"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetDiscountXTax(XmlDocument doc)
        {
            XmlNode receiptNode = null;
            try
            {
               receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            return Convert.ToString(Math.Abs(Convert.ToDecimal(receiptNode.Attributes["AdjustmentXTax"].Value))); 

            //return receiptNode.Attributes["AdjustmentXTax"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetDiscountTaxAmount(XmlDocument doc)
        {
            XmlNode receiptNode = null;
            try {
               receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return receiptNode.Attributes["AdjustmentTax"].Value;
        }
        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetDateStamp(XmlDocument doc) {
            XmlNode receiptNode = null;
            try {
                receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return SummaCommon.GetSummaFormattedTimestamp(receiptNode.Attributes["Date"].Value,
                receiptNode.Attributes["Time"].Value);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetPromotionId(XmlDocument doc) {
            return String.Empty;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetOperatorId(XmlDocument doc) {
            XmlNode senderNode = null;
            try {
                senderNode = doc.DocumentElement.SelectSingleNode("/Transaction/Sender");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return senderNode.Attributes["XmlID"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetExtra1(XmlDocument doc) {
            return String.Empty;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetExtra2(XmlDocument doc) {
            return String.Empty;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetExtra3(XmlDocument doc) {
            return String.Empty;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetStoreId(XmlDocument doc) {
            XmlNode terminalNode = null;
            try {
                terminalNode = doc.DocumentElement.SelectSingleNode("/Transaction/Terminal");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return Summa.GetSummaSiteId(terminalNode.Attributes["SiteID"].Value);
        }

        internal static string GetTransactionType(XmlDocument doc) 
        {
            XmlNode receiptNode = null;
            try
            {
                receiptNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            string transactionTypeStr = receiptNode.Attributes["TransactionType"].Value;
            switch (transactionTypeStr.ToUpper())
            {
                case @"PURCHASE": return @"1";
                case @"REFUND": return @"2";

                default: return @"1";
            }

        }
    }

    /// <summary>
    /// 
    /// </summary>
    public static class SummaTransactionProductCommon {

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetProductId(XmlNode node) {
            if (node.Attributes["Code"].Value == String.Empty) {
                StringBuilder b = new StringBuilder();
                b.Append(node.Attributes["Menu"].Value);
                b.Append(node.Attributes["Course"].Value);
                b.Append(node.Attributes["Item"].Value);
                b.Append(node.Attributes["Size"].Value);

                return SummaCommon.StringToUnsignedHashValue(b.ToString());
            }
            else {
                return node.Attributes["Code"].Value;
            }
        }

        internal static string GetSummaNetXTax(XmlNode productNode)
        {
            XmlNode pricingNode = productNode.SelectSingleNode("Pricing");

            return pricingNode.Attributes["SummaNetXTax"].Value;
        }

        internal static string GetQuantity(XmlNode productNode) 
        {
            return productNode.Attributes["Qty"].Value;
        }

        #region Old Specs
        internal static string GetUnitPrice(XmlNode productNode) {
            XmlNode pricingNode = null;
            try {
                pricingNode = productNode.SelectSingleNode("Pricing");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return pricingNode.Attributes["Total"].Value;
        }

        internal static string GetDiscount(XmlNode productNode) {
            XmlNode priceChanges = null;
            try {
                priceChanges = productNode.SelectSingleNode("Pricing");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            return priceChanges.Attributes["AdjustmentTotal"].Value;
        }
        #endregion

        #region Specs v1.20
        // GST = Tax, in this context

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetTransactionDetailID(XmlNode inPrdNode)
        {
            string result;

            //..................................................................

            try
            {
                result = inPrdNode.Attributes["TransNo"].Value;
            }
            catch(Exception)
            {
                result = @"";
            }

/*
            Boolean transNoEmpty = false;

            try
            {
                if (inPrdNode.Attributes["TransNo"].Value.Trim() != @"")
                {
                    transNoEmpty = true;
                    result = inPrdNode.Attributes["TransNo"].Value;
                }
                else
                {
                    XmlNode transactionNode = inPrdNode.SelectSingleNode("/Transaction");

                    result = IDGenerator.GetTransactionDetailID(transactionNode.Attributes["id"].Value, inPrdNode.Attributes["Item"].Value);
                }
            }
            catch (Exception e)
            {
                if (transNoEmpty)
                {
                    throw e;
                }
                else
                {
                    try
                    {
                        XmlNode transactionNode = inPrdNode.SelectSingleNode("/Transaction");

                        result = IDGenerator.GetTransactionDetailID(transactionNode.Attributes["id"].Value, inPrdNode.Attributes["Item"].Value);
                    }
                    catch (XPathException xpe1)
                    {
                        throw xpe1;
                    }
                }
            }
*/

/*
            catch (XPathException xpe)
            {
                //throw xpe;

                if (transNoEmpty)
                {
                    throw xpe;
                }
                else
                {
                    try
                    {
                        XmlNode transactionNode = inPrdNode.SelectSingleNode("/Transaction");

                        result = IDGenerator.GetTransactionDetailID(transactionNode.Attributes["id"].Value, inPrdNode.Attributes["Item"].Value);
                    }
                    catch (XPathException xpe1)
                    {
                        throw xpe1;
                    }
                }
            }
*/

            //...........................................................

            return result;
        }
      
        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetUnitPriceXGST(XmlNode productNode) 
        {
            XmlNode pricingNode = null;

            try
            {
                pricingNode = productNode.SelectSingleNode(@"Pricing");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            return UnitPrice.CalcAsString(pricingNode.Attributes["SummaNet"].Value, productNode.Attributes["Qty"].Value, pricingNode.Attributes["SummaNetTax"].Value);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetGSTAmount(XmlNode productNode) 
        {
            XmlNode pricingNode = null;

            try
            {
                pricingNode = productNode.SelectSingleNode(@"Pricing");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            //.....................................................................

            string result = @"";

            string cTmpTax = UnitPrice.CalcTaxAsString(pricingNode.Attributes["GSTContent"].Value, productNode.Attributes["Qty"].Value);

            if (Convert.ToDecimal(cTmpTax) > 0)
            {
                if (Convert.ToDecimal(pricingNode.Attributes["SummaNetXTax"].Value) > 0) // NORMAL SALE
                {
                    result = cTmpTax;
                }
                else // SALE OF ITEM WITH A NEGATIVE PRICE
                {
                    result = string.Format(@"-{0}", cTmpTax);
                }
            }
            else // REFUND
            {
                result = cTmpTax;
            }

            return result;

            //return pricingNode.Attributes["GSTContent"].Value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetDiscountXGST(XmlNode productNode) 
        {
            XmlNode pricingNode = null;

            try
            {
                pricingNode = productNode.SelectSingleNode(@"Pricing");
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }

            return Convert.ToString(Math.Abs(Convert.ToDecimal(pricingNode.Attributes["Discount"].Value))); 

            //return pricingNode.Attributes["Discount"].Value;
        }
 
        /// <summary>
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetDiscountGSTAmount(XmlNode productNode) 
        {
            return "0";
            /*
                      XmlNode pricingNode = null;

                       try
                       {
                           //pricingNode = productNode.SelectSingleNode(@"/Transaction/Receipt/Product/Pricing");
                           pricingNode = productNode.SelectSingleNode(@"Pricing");
                       }
                       catch (XPathException xpe)
                       {
                           throw xpe;
                       }

                       return pricingNode.Attributes["AdjustmentGSTTotal"].Value;
           */
        }
        #endregion

        internal static string GetPromotionId(XmlNode productNode) {
            return String.Empty;
        }

        internal static string GetBarcode(XmlNode productNode) {
            return String.Empty;
        }
    }

    internal static class SummaPaymentMethodsCommon {

        /// <summary>
        /// Get the Name as the Id.
        /// </summary>
        /// <param name="paymentNode"></param>
        /// <returns></returns>
        internal static string GetPaymentId(XmlNode paymentNode) 
        {
            string code = paymentNode.Attributes["Code"].Value;

            if (code.Trim() == @"")
            {
                return SummaCommon.StringToUnsignedHashValue(paymentNode.Attributes["Name"].Value);
            }
            else
            {
                return code;
            }
        }

        internal static string GetCardNumber(XmlNode paymentNode) {
            return String.Empty;
        }

        internal static string GetSalesValue(XmlNode paymentNode)
        {
            return paymentNode.Attributes["Total"].Value;
        }
    }

    internal static class SummaPaymentTypeCommon 
    {

        /// <summary>
        /// Return the Name as the Id converted to a uint.
        /// </summary>
        /// <param name="paymentTypeNode"></param>
        /// <returns></returns>
        internal static string GetPaymentTypeId(XmlNode paymentTypeNode) 
        {
            string pID = paymentTypeNode.Attributes["Code"].Value;

            if (pID == @"")
            {
                return SummaCommon.StringToUnsignedHashValue(paymentTypeNode.Attributes["Name"].Value);
            }
            else
            {
                return pID;
            }

            //return SummaCommon.StringToUnsignedHashValue(paymentTypeNode.Attributes["Name"].Value);            
        }

        internal static string GetPaymentTypeName(XmlNode paymentTypeNode) 
        {
            return paymentTypeNode.Attributes["Name"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) 
        {
            Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.TransactionXMLFile;

            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaGroupsCommon 
    {
        internal static string GetGroupId(XmlNode groupNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(groupNode.Attributes["GroupID"].Value);
        }

        internal static string GetGroupName(XmlNode groupNode) 
        {
            return groupNode.Attributes["GroupName"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) 
        {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class NewSummaGroupsCommon
    {
        internal static string GetGroupId(XmlNode groupNode)
        {
            return IDGenerator.GetNLenID(groupNode.Attributes["GroupID"].Value, 10);
        }

        internal static string GetGroupName(XmlNode groupNode)
        {
            return groupNode.Attributes["GroupName"].Value;
        }

        internal static string GetStatus(XmlNode groupNode)
        {
            return groupNode.Attributes["Status"].Value;
        }

        internal static string GetStoreId(XmlDocument doc)
        {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }
    
    internal static class SummaOperatorsCommon
    {
        internal static string GetOperatorId(XmlNode operatorNode) {
            return operatorNode.Attributes["XmlID"].Value;
        }

        internal static string GetOperatorName(XmlNode operatorNode) {
            return operatorNode.Attributes["Name"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaDepartmentsCommon 
    {
        internal static string GetDepartmentId(XmlNode departmentNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(departmentNode.Attributes["Name"].Value);
        }

        internal static string GetDepartmentName(XmlNode departmentNode) {
            return departmentNode.Attributes["Name"].Value;
        }

        internal static string GetDepartmentGroupId(XmlNode departmentNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(departmentNode.Attributes["GroupID"].Value);
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaProductsListCommon {

        /// <summary>
        /// 
        /// </summary>
        /// <param name="productNode"></param>
        /// <returns></returns>
        internal static string GetProductId(XmlNode menuNode, XmlNode courseNode, XmlNode productNode, XmlNode sizeNode) 
        {
            if (sizeNode.Attributes["Code"].Value == String.Empty) 
            {
                StringBuilder b = new StringBuilder();
                b.Append(menuNode.Attributes["Name"].Value);
                b.Append(courseNode.Attributes["Name"].Value);
                b.Append(productNode.Attributes["Name"].Value);
                b.Append(sizeNode.Attributes["Name"].Value);

                byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
                UInt64 scarry = BitConverter.ToUInt32(bytes, 0);

                return scarry.ToString();
            }
            else
            {
                return sizeNode.Attributes["Code"].Value;
            }
        }


        /// <summary>
        /// Return the product name and its size concat., but only if the size is not 'Default'.
        /// </summary>
        /// <param name="menuNode"></param>
        /// <param name="courseNode"></param>
        /// <param name="productNode"></param>
        /// <param name="sizeNode"></param>
        /// <returns></returns>
        internal static string GetProductName(XmlNode menuNode, XmlNode courseNode, 
                XmlNode productNode, XmlNode sizeNode) {
            StringBuilder b = new StringBuilder();

            b.Append(productNode.Attributes["Name"].Value);
            if (sizeNode.Attributes["Name"].Value != "Default") {
                b.Append(" ").Append(sizeNode.Attributes["Name"].Value);
            }
            return b.ToString();
        }

        internal static string GetProductBarcode(XmlNode productNode) {
            return string.Empty;
        }

        internal static string GetProductPrice1(XmlNode sizeNode) {
            return sizeNode.Attributes["Price"].Value;
        }

        internal static string GetProductPrice2(XmlNode sizeNode) {
            return sizeNode.Attributes["SpecialPrice"].Value;
        }

        internal static string GetProductGroupId(XmlNode sizeNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(sizeNode.Attributes["GroupID"].Value);
        }

        internal static string GetProductDepartment(XmlNode sizeNode) {
            return sizeNode.Attributes["CategoryName"].Value;
        }

        internal static string GetProductDepartmentId(XmlNode sizeNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(sizeNode.Attributes["CategoryName"].Value);
        }

        internal static string GetProductCost(XmlNode sizeNode) {
            return sizeNode.Attributes["Cost"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaFreeListCommon 
    {
        internal static string GetFreeListId(XmlNode discountNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(discountNode.Attributes["id"].Value);
        }

        internal static string GetName(XmlNode discountNode) {
            return discountNode.Attributes["Name"].Value;
        }
    }

    internal static class SummaFixedListCommon 
    {
        internal static string GetFixedListId(XmlNode discountNode) 
        {
            return discountNode.Attributes["id"].Value;

            //return SummaCommon.StringToUnsignedHashValue(discountNode.Attributes["id"].Value);
        }

        internal static string GetName(XmlNode discountNode) {
            return discountNode.Attributes["Name"].Value;
        }
    }

    internal static class SummaPaymentMethodTotalsCommon {

        internal static string GetPaymentId(XmlNode paymentTotalNode) {
            StringBuilder b = new StringBuilder();
            b.Append(paymentTotalNode.Attributes["Name"].Value);

            byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
            UInt64 scarry = BitConverter.ToUInt32(bytes, 0);

            return scarry.ToString();
        }

        internal static string GetTransactionCount(XmlNode paymentTotalNode) {
            return paymentTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotal(XmlNode paymentTotalNode) {
            return paymentTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode paymentTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(paymentTotalNode.Attributes["Date"].Value,
                paymentTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaDepartmentTotals {

        internal static string GetDepartmentId(XmlNode departmentTotalNode) {
            return SummaCommon.StringToUnsignedHashValue(departmentTotalNode.Attributes["Name"].Value);
        }

        internal static string GetQtySold(XmlNode departmentTotalNode) {
            return departmentTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotalSales(XmlNode departmentTotalNode) {
            return departmentTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode departmentTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(departmentTotalNode.Attributes["Date"].Value,
                departmentTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaGroupTotals {
        internal static string GetGroupId(XmlNode groupTotalNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(groupTotalNode.Attributes["Name"].Value);
        }

        internal static string GetQtySold(XmlNode groupTotalNode) {
            return groupTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotalSales(XmlNode groupTotalNode) {
            return groupTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode groupTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(groupTotalNode.Attributes["Date"].Value,
                groupTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaOperatorTotals 
    {
        internal static string GetOperatorId(XmlNode operatorTotalNode) {

            //string tempOpId = operatorTotalNode.Attributes["PayrollID"].Value;
            //string rv = null;

            //if (tempOpId.Equals("0")) {
            //    rv = operatorTotalNode.Attributes["Name"].Value;
            //}
            //else {
            //    rv = tempOpId;
            //}

            return operatorTotalNode.Attributes["XmlID"].Value;
        }

        internal static string GetQtySold(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Qty"].Value;
        }

        #region Old Specs
        internal static string GetTotalSales(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["Total"].Value;
        }
        #endregion

        #region Specs v1.2
         internal static string GetTotalSalesXTax(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["TotalExcGST"].Value;
        }

         internal static string GetTotalTaxAmount(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["GSTContent"].Value;
        }
        #endregion

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaFreeListTotals 
    {
        internal static string GetId(XmlNode operatorTotalNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(operatorTotalNode.Attributes["id"].Value);
        }

        internal static string GetCount(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotalSales(XmlNode operatorTotalNode) 
        {
            return Convert.ToString(Math.Abs(Convert.ToDecimal(operatorTotalNode.Attributes["Total"].Value))); 

            //return operatorTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaFixedListTotals 
    {
        internal static string GetId(XmlNode operatorTotalNode) 
        {
            return SummaCommon.StringToUnsignedHashValue(operatorTotalNode.Attributes["id"].Value);
        }

        internal static string GetCount(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Qty"].Value;
        }

        #region Ols Specs
        internal static string GetTotalSales(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["Total"].Value;
        }
        #endregion

        #region Specs v1.2
        internal static string GetSalesTotalXTax(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["TotalExcGST"].Value;
        }

        internal static string GetSalesTotalTaxAmount(XmlNode operatorTotalNode) 
        {
            return operatorTotalNode.Attributes["GSTContent"].Value;
        }
        #endregion

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
        
    }

    internal static class SummaProductTotalsCommon 
    {
        internal static string GetProductId(XmlNode productTotalNode) {
            if (productTotalNode.Attributes["Code"].Value == String.Empty) {
                StringBuilder b = new StringBuilder();
                b.Append(productTotalNode.Attributes["Menu"].Value);
                b.Append(productTotalNode.Attributes["Course"].Value);
                b.Append(productTotalNode.Attributes["Item"].Value);
                b.Append(productTotalNode.Attributes["Size"].Value);

                byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
                UInt64 result = BitConverter.ToUInt32(bytes, 0);

                return result.ToString();
            }
            else {
                return productTotalNode.Attributes["Code"].Value;
            }
        }

        internal static string GetQty(XmlNode productTotalNode) {
            return productTotalNode.Attributes["Qty"].Value;
        }

        #region Old Specs
        internal static string GetTotalSales(XmlNode productTotalNode) 
        {
            return productTotalNode.Attributes["Total"].Value;
        }
        #endregion

        #region Specs v1.2
        internal static string GetTotalSalesXTax(XmlNode productTotalNode) 
        {
            return productTotalNode.Attributes["TotalExcGST"].Value;
        }

        internal static string GetTotalTaxAmount(XmlNode productTotalNode) 
        {
            return productTotalNode.Attributes["GSTContent"].Value;
        }
        #endregion

        internal static string GetTimestamp(XmlNode productTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(productTotalNode.Attributes["Date"].Value,
                productTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaHourlyTotalsCommon 
    {

        internal static string GetHourId(XmlNode hourlyTotalNode) {
            return hourlyTotalNode.Attributes["id"].Value;
        }

        internal static string GetTransQty(XmlNode hourlyTotalNode) {
            return hourlyTotalNode.Attributes["Qty"].Value;
        }

        #region Old Specs
        internal static string GetHourlyTotal(XmlNode hourlyTotalNode) 
        {
            return hourlyTotalNode.Attributes["Total"].Value;
        }
        #endregion

        #region Specs v1.2
        internal static string GetHourlySalesXTax(XmlNode hourlyTotalNode) 
        {
            return hourlyTotalNode.Attributes["TotalExcGST"].Value;
        }

        internal static string GetTotalTaxAmount(XmlNode hourlyTotalNode) 
        {
            return hourlyTotalNode.Attributes["GSTContent"].Value;
        }
        #endregion

        internal static string GetDateTime(XmlNode node) {
            return node.Attributes["Date"].Value;
        }
    }
}
