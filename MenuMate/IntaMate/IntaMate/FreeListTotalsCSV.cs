using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class FreeListTotalsCSV : CsvMulti {

        public FreeListTotalsCSV(string basePath) {
            FULLFILEPATH = basePath + "FreeListTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList freeListTotals = null;
            try {
                freeListTotals = doc.DocumentElement.SelectNodes("/TotalsDiscounts/Discount");

                int counter = 0;
                foreach (XmlNode freeListTotalNode in freeListTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaFreeListTotals.GetId(freeListTotalNode));
                    csvData[counter].Add(SummaFreeListTotals.GetCount(freeListTotalNode));
                    csvData[counter].Add(SummaFreeListTotals.GetTotalSales(freeListTotalNode));
                    csvData[counter].Add(SummaFreeListTotals.GetTimestamp(freeListTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing free list totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
