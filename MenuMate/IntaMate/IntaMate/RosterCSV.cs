using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace IntaMate {
    public class RosterCSV : CsvMulti {

        public RosterCSV(string basePath) {
            FULLFILEPATH = basePath + "Operators.txt";
            fieldLengths = new int[] { 10, 16, 16, 16, 16, 1, 20, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {

            XmlNodeList inAndOuts = null;
            try {
                inAndOuts = doc.DocumentElement.SelectNodes("/ListRoster/TimeClock");

                int counter = 0;
                foreach (XmlNode inAndOutNode in inAndOuts) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(GetEmployeeId(inAndOutNode));
                    csvData[counter].Add(GetStartHour1(inAndOutNode));
                    csvData[counter].Add(GetEndHour1(inAndOutNode));
                    csvData[counter].Add(GetStartHour2(inAndOutNode));
                    csvData[counter].Add(GetEndHour2(inAndOutNode));
                    csvData[counter].Add(GetBreak(inAndOutNode));
                    csvData[counter].Add(GetRosterDate(inAndOutNode));
                    csvData[counter].Add(GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing roster", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }

        private string GetStoreId(XmlDocument doc) {
            return doc.DocumentElement.Attributes["SiteID"].Value;
        }

        private string GetRosterDate(XmlNode inAndOut) {
            return string.Empty;
        }

        private string GetBreak(XmlNode inAndOut) {
            return string.Empty;
        }

        private string GetEndHour2(XmlNode inAndOut) {
            return string.Empty;
        }

        private string GetStartHour2(XmlNode inAndOut) {
            return string.Empty;
        }

        private string GetEndHour1(XmlNode inAndOut) {
            return inAndOut.Attributes["EndTime"].Value;
        }

        private string GetStartHour1(XmlNode inAndOut) {
            return inAndOut.Attributes["StartTime"].Value;
        }

        private string GetEmployeeId(XmlNode inAndOut) {
            return inAndOut.Attributes["XmlID"].Value;
        }
    }
}
