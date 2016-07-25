using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class GroupsCSV : CsvMulti {

        public GroupsCSV(string basePath) {
            FULLFILEPATH = basePath + "Groups.txt";
            SEPERATOR = ",";

            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc) {

            XmlNodeList groups = null;
            try {
                groups = doc.DocumentElement.SelectNodes("/ListGroups/Group");

                int counter = 0;
                foreach (XmlNode groupNode in groups) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaGroupsCommon.GetGroupId(groupNode));
                    csvData[counter].Add(SummaGroupsCommon.GetGroupName(groupNode));
                    csvData[counter].Add(SummaGroupsCommon.GetStoreId(doc));

                    counter++;

                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing groups", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
