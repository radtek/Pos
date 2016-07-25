using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
    public class OperatorTotalsCSV : CsvMulti {

        public OperatorTotalsCSV(string basePath)
        {
             //FULLFILEPATH = basePath + "OperatorTotals.txt";
            FULLFILEPATH = basePath;
            AppendToFile = true;
//            fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
//            shouldQuoteField = new bool[] { false, false, false, false, false, false };
            fieldLengths = new int[] { 10, 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) 
        {
            FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.OperatorsTotals, SummaCommon.GetStoreId(doc));

            XmlNodeList operatorTotals = null;
            try {
                operatorTotals = doc.DocumentElement.SelectNodes("/TotalsStaff/Staff");

                int counter = 0;
                foreach (XmlNode operatorTotalNode in operatorTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaOperatorTotals.GetOperatorId(operatorTotalNode));
                    csvData[counter].Add(SummaOperatorTotals.GetQtySold(operatorTotalNode));

                    #region Old Specs
                    //csvData[counter].Add(SummaOperatorTotals.GetTotalSales(operatorTotalNode));
                    #endregion

                    #region Specs v1.2
                    csvData[counter].Add(SummaOperatorTotals.GetTotalSalesXTax(operatorTotalNode));
                    csvData[counter].Add(SummaOperatorTotals.GetTotalTaxAmount(operatorTotalNode));
                    #endregion

                    csvData[counter].Add(SummaOperatorTotals.GetTimestamp(operatorTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > operatorTotals.Count))
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
                SummaException se = new SummaException("Problem processing operator totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
