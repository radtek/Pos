using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{

    /// <summary>
    /// Appends entries to the TransactionDetails.txt, one for each product.
    /// </summary>
    public class TransactionDetailCSV : CsvMulti 
    {
        int _lineNumber = 1;

        public TransactionDetailCSV(string basePath) 
        {
            //FULLFILEPATH = basePath + "TransactionDetails.txt";
            FULLFILEPATH = basePath;

            AppendToFile = true;

            //fieldLengths = new int[] { 16, 10, 10, 10, 10, 10, 100, 10 };
            //shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false };
            fieldLengths = new int[] { 10, 10, 10, 10, 10, 10, 10, 10, 10, 100, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false, false, false, false };
            
            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();

            resetLineNumber();
        }

        /// <summary>
        /// Setup internal data values for each product.
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc) 
        {
            XmlNodeList products = null;
            try 
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.TransactionXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.TransactionDetails, SummaTransactionCommon.GetStoreId(doc));

                products = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Product");

                int counter = 0;
                foreach (XmlNode productNode in products) {

                    csvData.Add(new List<string>());

                    #region Specs v1.2
                    csvData[counter].Add(GetTransactionDetailID(doc, productNode));
                    #endregion

                    csvData[counter].Add(SummaTransactionCommon.GetTransactionID(doc));
                    csvData[counter].Add(SummaTransactionProductCommon.GetProductId(productNode));
                    csvData[counter].Add(GetQty(doc, productNode));

                    #region Old Specs
                    //csvData[counter].Add(SummaTransactionProductCommon.GetUnitPrice(productNode));
                    //csvData[counter].Add(SummaTransactionProductCommon.GetDiscount(productNode));
                    #endregion

                    #region Specs v1.2
                    // GST = Tax, in this context
                    csvData[counter].Add(GetUnitPriceXGST(productNode));
                    csvData[counter].Add(GetGSTAmount(productNode));

                    csvData[counter].Add(SummaTransactionProductCommon.GetDiscountXGST(productNode));
                    csvData[counter].Add(SummaTransactionProductCommon.GetDiscountGSTAmount(productNode));
                    #endregion

                    csvData[counter].Add(SummaTransactionProductCommon.GetPromotionId(productNode));
                    csvData[counter].Add(SummaTransactionProductCommon.GetBarcode(productNode));
                    csvData[counter].Add(SummaTransactionCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing transaction detail", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <param name="inProductNode"></param>
        /// <returns></returns>
        string GetTransactionDetailID(XmlDocument inDoc, XmlNode inProductNode)
        {
            return Convert.ToString(getCurrentLineNumber());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        int getCurrentLineNumber()
        {
            return ++_lineNumber;
        }

        /// <summary>
        /// 
        /// </summary>
        public void resetLineNumber()
        {
            _lineNumber = 0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDoc"></param>
        /// <param name="inProductNode"></param>
        /// <returns></returns>
        string GetQty(XmlDocument inDoc, XmlNode inProductNode)
        {
            string transactionType = SummaTransactionCommon.GetTransactionType(inDoc);
            double qty = Convert.ToDouble(SummaTransactionProductCommon.GetQuantity(inProductNode));

            switch (transactionType)
            {
                case @"1": return string.Format("{0:#.00}", Math.Abs(qty));      // Purchase
                case @"2": return string.Format("{0:#.00}", Math.Abs(qty) * -1); // Refund

                default: return string.Format("{0:#.00}", Math.Abs(qty));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProductNode"></param>
        /// <returns></returns>
        string GetUnitPriceXGST(XmlNode inProductNode)
        {
            double unitPriceXTax = Convert.ToDouble(SummaTransactionProductCommon.GetUnitPriceXGST(inProductNode));

            return string.Format("{0:#.00}", Math.Abs(unitPriceXTax));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProductNode"></param>
        /// <returns></returns>
        string GetGSTAmount(XmlNode inProductNode)
        {
            double gstAmount = Convert.ToDouble(SummaTransactionProductCommon.GetGSTAmount(inProductNode));

            return string.Format("{0:#.00}", Math.Abs(gstAmount));
        }
     }
}
