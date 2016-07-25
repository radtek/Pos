using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class PaymentMethodTotalsCSV : CsvMulti {

        public PaymentMethodTotalsCSV(string basePath) {
            FULLFILEPATH = basePath + "TransPaymentTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 100, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) {
            XmlNodeList paymentMethodTotals = null;
            try {
                paymentMethodTotals = doc.DocumentElement.SelectNodes("/TotalsPayments/TotalPayment");

                int counter = 0;
                foreach (XmlNode paymentTotalNode in paymentMethodTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaPaymentMethodTotalsCommon.GetPaymentId(paymentTotalNode));
                    csvData[counter].Add(SummaPaymentMethodTotalsCommon.GetTransactionCount(paymentTotalNode));
                    csvData[counter].Add(SummaPaymentMethodTotalsCommon.GetTotal(paymentTotalNode));
                    csvData[counter].Add(SummaPaymentMethodTotalsCommon.GetTimestamp(paymentTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing payment method totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
