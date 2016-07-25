using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.XPath;
using System.Xml;

namespace IntaMate {
    public class DepartmentsCSV : CsvMulti {

        public DepartmentsCSV(string basePath) {
            FULLFILEPATH = basePath + "Departments.txt";
            fieldLengths = new int[] { 10, 100, 10, 10 };
            shouldQuoteField = new bool[] { false, true, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList departments = null;
            try {
                departments = doc.DocumentElement.SelectNodes("/ListCategories/Category");

                int counter = 0;
                foreach (XmlNode categoryNode in departments) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaDepartmentsCommon.GetDepartmentId(categoryNode));
                    csvData[counter].Add(SummaDepartmentsCommon.GetDepartmentName(categoryNode));
                    csvData[counter].Add(SummaDepartmentsCommon.GetDepartmentGroupId(categoryNode));
                    csvData[counter].Add(SummaDepartmentsCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing departments", e);
                se.Data = doc.OuterXml;
                throw se;
            }

        }
    }
}
