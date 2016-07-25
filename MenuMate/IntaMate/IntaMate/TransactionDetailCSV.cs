using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Xml;
using System.Xml.XPath;
using System.IO;

namespace IntaMate {

    /// <summary>
    /// Appends entries to the TransactionDetails.txt, one for each product.
    /// </summary>
    public class TransactionDetailCSV : CsvMulti {

        public TransactionDetailCSV(string basePath) {
            FULLFILEPATH = basePath + "TransactionDetails.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 10, 10, 10, 100, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false };
            
            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        /// <summary>
        /// Setup internal data values for each product.
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc) {

            XmlNodeList products = null;
            try {
                products = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Product");

                int counter = 0;
                foreach (XmlNode productNode in products) {

                    csvData.Add(new List<string>());
                    csvData[counter].Add(SummaTransactionCommon.GetTransactionId(doc));
                    csvData[counter].Add(SummaTransactionProductCommon.GetProductId(productNode));
                    csvData[counter].Add(SummaTransactionProductCommon.GetQuantity(productNode));
                    csvData[counter].Add(SummaTransactionProductCommon.GetUnitPrice(productNode));
                    csvData[counter].Add(SummaTransactionProductCommon.GetDiscount(productNode));
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
    }
}
