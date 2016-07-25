using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using System.IO;

namespace IntaMate {
    public class TransactionPaymentMethodsCSV : CsvMulti {

        public TransactionPaymentMethodsCSV(string basePath) {
            FULLFILEPATH = basePath + "TransactionPaymentMethods.txt";
            AppendToFile = true;
            SEPERATOR = ",";
            fieldLengths = new int[] { 10, 10, 100, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc) {

            
            XmlNodeList methods = null;
            try {
                methods = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Payment");

                int counter = 0;
                foreach (XmlNode paymentNode in methods) {

                    csvData.Add(new List<string>());
                    csvData[counter].Add(SummaTransactionCommon.GetTransactionId(doc));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetPaymentId(paymentNode));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetCardNumber(paymentNode));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetSalesValue(paymentNode));
                    csvData[counter].Add(SummaTransactionCommon.GetStoreId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing transaction payment methods", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
