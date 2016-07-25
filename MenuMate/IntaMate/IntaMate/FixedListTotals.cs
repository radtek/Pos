using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class FixedListTotals : CsvMulti {

        public FixedListTotals(string basePath) {
            FULLFILEPATH = basePath + "FixedListTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList fixedListTotals = null;
            try {
                fixedListTotals = doc.DocumentElement.SelectNodes("/TotalsCalculated/Calculated");

                int counter = 0;
                foreach (XmlNode fixedListTotalNode in fixedListTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaFixedListTotals.GetId(fixedListTotalNode));
                    csvData[counter].Add(SummaFixedListTotals.GetCount(fixedListTotalNode));
                    csvData[counter].Add(SummaFixedListTotals.GetTotalSales(fixedListTotalNode));
                    csvData[counter].Add(SummaFixedListTotals.GetTimestamp(fixedListTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing fixed list totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
