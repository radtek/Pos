using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{

    /// <summary>
    /// Appends a single entry to the Transactions.txt csv file.
    /// </summary>
    public class TransactionCheckCSV : CsvMulti
    {

        public TransactionCheckCSV(string basePath)
        {
            //FULLFILEPATH = basePath + "Transactions.txt";
            FULLFILEPATH = basePath;

            AppendToFile = true;
            SEPERATOR = ",";

            fieldLengths = new int[] { 10, 10, 10, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        /// <summary>
        /// Deconstruct xml document and populate internal data fields
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc)
        {
            FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.TransactionCheck, SummaTransactionCommon.GetStoreId(doc));

            try
            {
                csvData.Add(new List<string>());
                csvLineData.Add(new StringBuilder());

                csvData[0].Add(SummaTransactionCommon.GetTransactionID(doc));
                csvData[0].Add(SummaTransactionCommon.GetSalesTotalXTax(doc));  // Sales Total Excluding Tax

                //XmlNode productNode = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt/Product");

                XmlNodeList products = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Product");

                decimal summaNetXTax = 0;
                decimal quantity = 0; 
                decimal unitPrice = 0;

                foreach (XmlNode productNode in products)
                {
                    summaNetXTax += Convert.ToDecimal(SummaTransactionProductCommon.GetSummaNetXTax(productNode));
                    quantity     += Convert.ToDecimal(SummaTransactionProductCommon.GetQuantity(productNode));
                    unitPrice    += Convert.ToDecimal(SummaTransactionProductCommon.GetUnitPriceXGST(productNode));
                }

                csvData[0].Add(string.Format("{0:#.00}", Math.Abs(summaNetXTax)));
                csvData[0].Add(string.Format("{0:#.00}", Math.Abs(quantity)));
                csvData[0].Add(string.Format("{0:#.00}", Math.Abs(unitPrice)));
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem processing transaction", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
