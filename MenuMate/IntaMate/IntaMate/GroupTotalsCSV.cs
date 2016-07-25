using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class GroupTotalsCSV : CsvMulti {

        public GroupTotalsCSV(string basePath) {
            FULLFILEPATH = basePath + "GroupTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList groupTotals = null;
            try {
                groupTotals = doc.DocumentElement.SelectNodes("/TotalsGroups/Group");

                int counter = 0;
                foreach (XmlNode groupTotalNode in groupTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaGroupTotals.GetGroupId(groupTotalNode));
                    csvData[counter].Add(SummaGroupTotals.GetQtySold(groupTotalNode));
                    csvData[counter].Add(SummaGroupTotals.GetTotalSales(groupTotalNode));
                    csvData[counter].Add(SummaGroupTotals.GetTimestamp(groupTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing group totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }

        }
    }
}
