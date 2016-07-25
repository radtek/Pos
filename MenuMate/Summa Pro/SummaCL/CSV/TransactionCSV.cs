using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;
//using SummaCL.Global;

namespace SummaCL
{

    /// <summary>
    /// Appends a single entry to the Transactions.txt csv file.
    /// </summary>
    public class TransactionCSV : CsvMulti {

        public TransactionCSV(string basePath)
        {
            //FULLFILEPATH = basePath + "Transactions.txt";
            FULLFILEPATH = basePath;

            AppendToFile = true;
            SEPERATOR = ",";

            //fieldLengths = new int[] { 16, 10, 10, 10, 10, 20, 20, 10, 100, 100, 100, 10 };
            //shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false, false, false, false, false };
            
            fieldLengths = new int[] { 10, 10, 10, 100, 10, 10, 10, 10, 20, 20, 10, 100, 100, 100, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        /// <summary>
        /// Deconstruct xml document and populate internal data fields
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc) 
        {
            try 
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.TransactionXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.Transactions, SummaTransactionCommon.GetStoreId(doc));

                csvData.Add(new List<string>());
                csvLineData.Add(new StringBuilder());

                csvData[0].Add(SummaTransactionCommon.GetTransactionID(doc));
                csvData[0].Add(SummaTransactionCommon.GetTransactionType(doc));
                csvData[0].Add(SummaTransactionCommon.GetTerminalID(doc));

                csvData[0].Add(SummaTransactionCommon.GetCustomerId(doc));

                //string id = SummaTransactionCommon.GetCustomerId(doc);
                //CheckCustomerID.Check(id);
                //csvData[0].Add(id);

                #region Old Specs
                //csvData[0].Add(SummaTransactionCommon.GetSalesTotal(doc));
                #endregion

                #region Specs v1.2
                csvData[0].Add(GetSalesTotalXTax(doc));      // Sales Total Excluding Tax
                csvData[0].Add(GetSalesTotalTaxAmount(doc)); // Sales Total Tax Amount
                
                csvData[0].Add(GetDiscountXTax(doc));      // Discount Excluding Tax
                csvData[0].Add(GetDiscountTaxAmount(doc)); // Discount Tax Amount
                #endregion
               
                csvData[0].Add(SummaTransactionCommon.GetDateStamp(doc));
                csvData[0].Add(SummaTransactionCommon.GetPromotionId(doc));
                csvData[0].Add(SummaTransactionCommon.GetOperatorId(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra1(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra2(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra3(doc));
                csvData[0].Add(SummaTransactionCommon.GetStoreId(doc));
            }
            catch (Exception e) 
            {
                SummaException se = new SummaException("Problem processing transaction", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        string GetSalesTotalXTax(XmlDocument inDoc)
        {
            string transactionType = SummaTransactionCommon.GetTransactionType(inDoc);
            double totalxTax = Convert.ToDouble(SummaTransactionCommon.GetSalesTotalXTax(inDoc));

            switch (transactionType)
            {
                case @"1": return string.Format("{0:#.00}", Math.Abs(totalxTax));      // Purchase
                case @"2": return string.Format("{0:#.00}", Math.Abs(totalxTax) * -1); // Refund

                default: return string.Format("{0:#.00}", Math.Abs(totalxTax));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        string GetSalesTotalTaxAmount(XmlDocument inDoc)
        {
            string transactionType = SummaTransactionCommon.GetTransactionType(inDoc);
            double totalTaxAmount  = Convert.ToDouble(SummaTransactionCommon.GetSalesTotalTaxAmount(inDoc));

            switch (transactionType)
            {
                case @"1": return string.Format("{0:#.00}", Math.Abs(totalTaxAmount));      // Purchase
                case @"2": return string.Format("{0:#.00}", Math.Abs(totalTaxAmount) * -1); // Refund

                default: return string.Format("{0:#.00}", Math.Abs(totalTaxAmount));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        string GetDiscountXTax(XmlDocument inDoc)
        {
            string transactionType = SummaTransactionCommon.GetTransactionType(inDoc);
            double discountXTax = Convert.ToDouble(SummaTransactionCommon.GetDiscountXTax(inDoc));

            switch (transactionType)
            {
                case @"1": return string.Format("{0:#.00}", Math.Abs(discountXTax));      // Purchase
                case @"2": return string.Format("{0:#.00}", Math.Abs(discountXTax) * -1); // Refund

                default: return string.Format("{0:#.00}", Math.Abs(discountXTax));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <returns></returns>
        string GetDiscountTaxAmount(XmlDocument inDoc)
        {
            string transactionType = SummaTransactionCommon.GetTransactionType(inDoc);
            double discountXTax = Convert.ToDouble(SummaTransactionCommon.GetDiscountTaxAmount(inDoc));

            switch (transactionType)
            {
                case @"1": return string.Format("{0:#.00}", Math.Abs(discountXTax));      // Purchase
                case @"2": return string.Format("{0:#.00}", Math.Abs(discountXTax) * -1); // Refund

                default: return string.Format("{0:#.00}", Math.Abs(discountXTax));
            }
        }
    }
}
