using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    public class DepartmentTotalsCSV : CsvMulti 
    {

        public DepartmentTotalsCSV(string basePath) 
        {
            //FULLFILEPATH = basePath + "DepartmentTotals.txt";
            FULLFILEPATH = basePath;

            AppendToFile = true;
            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) 
        {
            XmlNodeList departmentTotals = null;
            try 
            {
                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.CategoriesTotals, SummaCommon.GetStoreId(doc));

                // In case GetStoreId fails
                //Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.UpdateMemberXMLFile;
                //Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                departmentTotals = doc.DocumentElement.SelectNodes("/TotalsCategories/Category");

                int counter = 0;
                foreach (XmlNode departmentTotalNode in departmentTotals) 
                {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaDepartmentTotals.GetDepartmentId(departmentTotalNode));
                    csvData[counter].Add(SummaDepartmentTotals.GetQtySold(departmentTotalNode));
                    csvData[counter].Add(SummaDepartmentTotals.GetTotalSales(departmentTotalNode));
                    csvData[counter].Add(SummaDepartmentTotals.GetTimestamp(departmentTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > departmentTotals.Count))
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
                SummaException se = new SummaException("Problem processing department totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
