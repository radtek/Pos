using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using System.IO;

namespace IntaMate {
    public class PaymentMethodTypesCSV: CsvMulti {

        public PaymentMethodTypesCSV(string basePath) {
            FULLFILEPATH = basePath + "PaymentTypes.txt";
            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList payments = null;
            try {
                payments = doc.DocumentElement.SelectNodes("/ListPaymentTypes/Payment");

                int counter = 0;
                foreach (XmlNode paymentTypeNode in payments) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaPaymentTypeCommon.GetPaymentTypeId(paymentTypeNode));
                    csvData[counter].Add(SummaPaymentTypeCommon.GetPaymentTypeName(paymentTypeNode));
                    csvData[counter].Add(SummaPaymentTypeCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing payment method types", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
