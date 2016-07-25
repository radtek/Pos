using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using System.Globalization;

namespace IntaMate {

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

        public static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }

        public static string GetTillId(XmlDocument doc) {
            return doc.DocumentElement.Attributes["TerminalId"].Value;
        }

        public static string GetSummaFormattedTimestamp(string dateString, string timeString) {

            // date and times are sent from menumate in dd/mm/yyy hh:mm:ss format, so 
            // force the conversion to be in a compatible format i.e., nz regardless of
            // the actual settings on the local machine
            DateTime date = DateTime.Parse(dateString, CultureInfo.CreateSpecificCulture("en-NZ"));
            DateTime time = DateTime.Parse(timeString, CultureInfo.CreateSpecificCulture("en-NZ"));

            StringBuilder timestampTemp = new StringBuilder(date.ToString("yyyyMMdd"));
            timestampTemp.Append(time.ToString("HHmmss"));

            return timestampTemp.ToString();
        }

        public static string StringToUnsignedHashValue(string p) {
            byte[] bytes = BitConverter.GetBytes(p.GetHashCode());
            UInt64 result = BitConverter.ToUInt32(bytes, 0);

            return result.ToString();
        }
    }

    /// <summary>
    /// Implements xml data lookups for Summa
    /// </summary>
    public static class SummaTransactionCommon {



        /// <summary>
        /// Builds Transaction Id from Site Id and Receipt Number, or returns 999999999.
        /// NOTE: Does not use the ID attribute of the Transaction element!
        /// </summary>
        /// <param name="document"></param>
        /// <returns></returns>
        public static string GetTransactionId(XmlDocument document) {
            XmlNode receiptNode;
            string rv;

            try {
                receiptNode = document.DocumentElement.SelectSingleNode("/Transaction/Receipt");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            if (receiptNode == null) {
                rv = "9999999999";
            }
            else {
                rv = SummaTransactionCommon.GetStoreId(document) + receiptNode.Attributes["ReceiptNumber"].Value;
            }

            return rv;
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
        /// 
        /// </summary>
        /// <param name="doc"></param>
        /// <returns></returns>
        public static string GetCustomerId(XmlDocument doc) {

            XmlNode membershipNode = null;
            string customerId = string.Empty;

            try {
                membershipNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt/Membership");
                if (membershipNode != null) {
                    customerId = membershipNode.Attributes["MemberID"].Value;
                }
            }
            catch (XPathException xpe) {
                customerId = String.Empty;
                // logging!
            }

            return customerId;
        }

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

        internal static string GetTransactionType(XmlDocument doc) {
            return "1";
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

        internal static string GetQuantity(XmlNode productNode) {
            return productNode.Attributes["Qty"].Value;
        }

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
        internal static string GetPaymentId(XmlNode paymentNode) {
            return SummaCommon.StringToUnsignedHashValue(paymentNode.Attributes["Name"].Value);
        }

        internal static string GetCardNumber(XmlNode paymentNode) {
            return String.Empty;
        }

        internal static string GetSalesValue(XmlNode paymentNode) {
            return paymentNode.Attributes["Total"].Value;
        }
    }

    internal static class SummaPaymentTypeCommon {

        /// <summary>
        /// Return the Name as the Id converted to a uint.
        /// </summary>
        /// <param name="paymentTypeNode"></param>
        /// <returns></returns>
        internal static string GetPaymentTypeId(XmlNode paymentTypeNode) {

            return SummaCommon.StringToUnsignedHashValue(paymentTypeNode.Attributes["Name"].Value);
        }

        internal static string GetPaymentTypeName(XmlNode paymentTypeNode) {
            return paymentTypeNode.Attributes["Name"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaGroupsCommon {
        internal static string GetGroupId(XmlNode groupNode) {
            return SummaCommon.StringToUnsignedHashValue(groupNode.Attributes["Name"].Value);
        }


        internal static string GetGroupName(XmlNode groupNode) {
            return groupNode.Attributes["Name"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaOperatorsCommon {
        internal static string GetOperatorId(XmlNode operatorNode) {

            //string tempOpId = 
            //string rv = null;

            //if (tempOpId.Equals("0")) {
            //    rv = operatorNode.Attributes["Name"].Value;
            //}
            //else {
            //    rv = tempOpId;
            //}

            return operatorNode.Attributes["XmlID"].Value;
        }

        internal static string GetOperatorName(XmlNode operatorNode) {
            return operatorNode.Attributes["Name"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaDepartmentsCommon {
        internal static string GetDepartmentId(XmlNode departmentNode) {
            return SummaCommon.StringToUnsignedHashValue(departmentNode.Attributes["Name"].Value);
        }

        internal static string GetDepartmentName(XmlNode departmentNode) {
            return departmentNode.Attributes["Name"].Value;
        }

        internal static string GetDepartmentGroupId(XmlNode departmentNode) {
            return SummaCommon.StringToUnsignedHashValue(departmentNode.Attributes["GroupName"].Value);
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
        internal static string GetProductId(XmlNode menuNode, XmlNode courseNode, XmlNode productNode, XmlNode sizeNode) {
            if (sizeNode.Attributes["Code"].Value == String.Empty) {
                StringBuilder b = new StringBuilder();
                b.Append(menuNode.Attributes["Name"].Value);
                b.Append(courseNode.Attributes["Name"].Value);
                b.Append(productNode.Attributes["Name"].Value);
                b.Append(sizeNode.Attributes["Name"].Value);

                byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
                UInt64 scarry = BitConverter.ToUInt32(bytes, 0);

                return scarry.ToString();
            }
            else {
                return sizeNode.Attributes["Code"].Value;
            }
        }

        internal static string GetProductName(XmlNode menuNode, XmlNode courseNode, XmlNode productNode, XmlNode sizeNode) {
            StringBuilder b = new StringBuilder();
            b.Append(menuNode.Attributes["Name"].Value);
            b.Append(" ").Append(courseNode.Attributes["Name"].Value);
            b.Append(" ").Append(productNode.Attributes["Name"].Value);
            b.Append(" ").Append(sizeNode.Attributes["Name"].Value);

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

        internal static string GetProductGroupId(XmlNode sizeNode) {
            return SummaCommon.StringToUnsignedHashValue(sizeNode.Attributes["GroupName"].Value);
        }

        internal static string GetProductDepartment(XmlNode sizeNode) {
            return sizeNode.Attributes["CategoryName"].Value;
        }

        internal static string GetProductDepartmentId(XmlNode sizeNode) {
            return SummaCommon.StringToUnsignedHashValue(sizeNode.Attributes["CategoryName"].Value);
        }

        internal static string GetProductCost(XmlNode sizeNode) {
            return sizeNode.Attributes["Cost"].Value;
        }

        internal static string GetStoreId(XmlDocument doc) {
            return Summa.GetSummaSiteId(doc.DocumentElement.Attributes["SiteID"].Value);
        }
    }

    internal static class SummaFreeListCommon {
        internal static string GetFreeListId(XmlNode discountNode) {
            return discountNode.Attributes["id"].Value;
        }

        internal static string GetName(XmlNode discountNode) {
            return discountNode.Attributes["Name"].Value;
        }
    }

    internal static class SummaFixedListCommon {
        internal static string GetFixedListId(XmlNode discountNode) {
            return discountNode.Attributes["id"].Value;
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
        internal static string GetGroupId(XmlNode groupTotalNode) {
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

    internal static class SummaOperatorTotals {
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

        internal static string GetTotalSales(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaFreeListTotals {
        internal static string GetId(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["id"].Value;
        }

        internal static string GetCount(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotalSales(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaFixedListTotals {
        internal static string GetId(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["id"].Value;
        }

        internal static string GetCount(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetTotalSales(XmlNode operatorTotalNode) {
            return operatorTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode operatorTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(operatorTotalNode.Attributes["Date"].Value,
                operatorTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaProductTotalsCommon {
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

        internal static string GetTotalSales(XmlNode productTotalNode) {
            return productTotalNode.Attributes["Total"].Value;
        }

        internal static string GetTimestamp(XmlNode productTotalNode) {
            return SummaCommon.GetSummaFormattedTimestamp(productTotalNode.Attributes["Date"].Value,
                productTotalNode.Attributes["Time"].Value);
        }
    }

    internal static class SummaHourlyTotalsCommon {

        internal static string GetHourId(XmlNode hourlyTotalNode) {
            return hourlyTotalNode.Attributes["id"].Value;
        }

        internal static string GetTransQty(XmlNode hourlyTotalNode) {
            return hourlyTotalNode.Attributes["Qty"].Value;
        }

        internal static string GetHourlyTotal(XmlNode hourlyTotalNode) {
            return hourlyTotalNode.Attributes["Total"].Value;
        }

        internal static string GetDateTime(XmlNode node) {
            return node.Attributes["Date"].Value;
        }
    }
}
