using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.XPath;

namespace IntaMate {

    /// <summary>
    /// Appends a single entry to the Transactions.txt csv file.
    /// </summary>
    public class TransactionCSV : CsvMulti {

        public TransactionCSV(string basePath) {
            FULLFILEPATH = basePath + "Transactions.txt";
            AppendToFile = true;
            SEPERATOR = ",";
            fieldLengths = new int[] { 10, 10, 10, 100, 10, 20, 20, 10, 100, 100, 100, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false, false, false, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        /// <summary>
        /// Deconstruct xml document and populate internal data fields
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc) {

            try {
                csvData.Add(new List<string>());
                csvLineData.Add(new StringBuilder());

                csvData[0].Add(SummaTransactionCommon.GetTransactionId(doc));
                csvData[0].Add(SummaTransactionCommon.GetTransactionType(doc));
                csvData[0].Add(SummaTransactionCommon.GetTerminalID(doc));
                csvData[0].Add(SummaTransactionCommon.GetCustomerId(doc));
                csvData[0].Add(SummaTransactionCommon.GetSalesTotal(doc));
                csvData[0].Add(SummaTransactionCommon.GetDateStamp(doc));
                csvData[0].Add(SummaTransactionCommon.GetPromotionId(doc));
                csvData[0].Add(SummaTransactionCommon.GetOperatorId(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra1(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra2(doc));
                csvData[0].Add(SummaTransactionCommon.GetExtra3(doc));
                csvData[0].Add(SummaTransactionCommon.GetStoreId(doc));
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing transaction", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
