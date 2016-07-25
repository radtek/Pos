using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    public class DepartmentsCSV : CsvMulti {

        public DepartmentsCSV(string basePath) {
            //FULLFILEPATH = basePath + "Departments.txt";
            FULLFILEPATH = basePath;

            fieldLengths = new int[] { 10, 100, 10, 10 };
            shouldQuoteField = new bool[] { false, true, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
            XmlNodeList departments = null;
            try 
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.ListCategoriesXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.SubCategories, SummaCommon.GetStoreId(doc));

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

/*
                    if ((counter == 1) || (counter > departments.Count))
                    {
                        csvLineData.Add(new StringBuilder());
                    }
                    else
                    {
                        csvLineData.Add(new StringBuilder(","));
                    }
*/
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
