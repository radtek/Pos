using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
    class HourlySalesCSV : CsvMulti 
    {

        public HourlySalesCSV(string basePath) 
        {
             //FULLFILEPATH = basePath + "HourlyTotals.txt";
            FULLFILEPATH = basePath;
            
            AppendToFile = true;
            //fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            //shouldQuoteField = new bool[] { false, false, false, false, false, false };
            fieldLengths = new int[] { 10, 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) 
        {
            FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.HourlyTotals, SummaCommon.GetStoreId(doc));

            XmlNodeList hourTotals = null;
            try 
            {
                hourTotals = doc.DocumentElement.SelectNodes("/TotalsHourlySales/Hour");

                int counter = 0;
                foreach (XmlNode hourTotalNode in hourTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaHourlyTotalsCommon.GetHourId(hourTotalNode));
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetTransQty(hourTotalNode));

                    #region Old Specs
                    //csvData[counter].Add(SummaHourlyTotalsCommon.GetHourlyTotal(hourTotalNode));
                    #endregion

                    #region Specs v1.2
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetHourlySalesXTax(hourTotalNode));
                    csvData[counter].Add(SummaHourlyTotalsCommon.GetTotalTaxAmount(hourTotalNode));
                    #endregion

                    csvData[counter].Add(SummaHourlyTotalsCommon.GetDateTime(hourTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > hourTotals.Count))
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
                SummaException se = new SummaException("Problem processing hourly sales", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
