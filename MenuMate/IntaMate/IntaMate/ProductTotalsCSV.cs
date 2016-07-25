using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.XPath;
using System.Xml;

namespace IntaMate {
    public class ProductTotalsCSV : CsvMulti {

        public ProductTotalsCSV(string basePath) {
            FULLFILEPATH = basePath + "ProductTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) {
            XmlNodeList productTotals = null;
            try {
                productTotals = doc.DocumentElement.SelectNodes("/TotalsProducts/Product");

                int counter = 0;
                foreach (XmlNode productTotalNode in productTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaProductTotalsCommon.GetProductId(productTotalNode));
                    csvData[counter].Add(SummaProductTotalsCommon.GetQty(productTotalNode));
                    csvData[counter].Add(SummaProductTotalsCommon.GetTotalSales(productTotalNode));
                    csvData[counter].Add(SummaProductTotalsCommon.GetTimestamp(productTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing product totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
