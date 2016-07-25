using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class OperatorsCSV : CsvMulti {

        public OperatorsCSV(string basePath) {
            FULLFILEPATH = basePath + "Operators.txt";
            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList operators = null;
            try {
                operators = doc.DocumentElement.SelectNodes("/ListStaff/Staff");

                int counter = 0;
                foreach (XmlNode operatorNode in operators) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaOperatorsCommon.GetOperatorId(operatorNode));
                    csvData[counter].Add(SummaOperatorsCommon.GetOperatorName(operatorNode));
                    csvData[counter].Add(SummaOperatorsCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing operators", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
