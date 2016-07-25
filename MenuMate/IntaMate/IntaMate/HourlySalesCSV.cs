using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    class HourlySalesCSV : CsvMulti {

        public HourlySalesCSV(string basePath) {
            FULLFILEPATH = basePath + "HourlyTotals.txt";
            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) {
            XmlNodeList hourTotals = null;
            try {
                hourTotals = doc.DocumentElement.SelectNodes("/TotalsHourlySales/Hour");

                int counter = 0;
                foreach (XmlNode hourTotalNode in hourTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaHourlyTotalsCommon.GetHourId(hourTotalNode));
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetTransQty(hourTotalNode));
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetHourlyTotal(hourTotalNode));
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetDateTime(hourTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing hourly sales", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
