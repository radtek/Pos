using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
    public class FixedListTotals : CsvMulti 
    {

        public FixedListTotals(string basePath)
        {
            //FULLFILEPATH = basePath + "FixedListTotals.txt";
            FULLFILEPATH = basePath;
            
            AppendToFile = true;
            //fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            //shouldQuoteField = new bool[] { false, false, false, false, false, false };
            fieldLengths = new int[] { 10, 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) 
        {
            FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.DailyTotals, SummaCommon.GetStoreId(doc));

            XmlNodeList fixedListTotals = null;
            try {
                fixedListTotals = doc.DocumentElement.SelectNodes("/TotalsCalculated/Calculated");

                int counter = 0;
                foreach (XmlNode fixedListTotalNode in fixedListTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaFixedListTotals.GetId(fixedListTotalNode));
                    csvData[counter].Add(SummaFixedListTotals.GetCount(fixedListTotalNode));

                    #region Old Specs
                    //csvData[counter].Add(SummaFixedListTotals.GetTotalSales(fixedListTotalNode));
                    #endregion

                    #region Specs v1.2

                    int tId = Convert.ToInt16(SummaFixedListTotals.GetId(fixedListTotalNode));

                    if (tId == 4) // Discount always positive
                    {
                        string total = SummaFixedListTotals.GetSalesTotalXTax(fixedListTotalNode);

                        csvData[counter].Add(Convert.ToString(Math.Abs(Convert.ToDecimal(total))));
                    }
                    else
                    {
                        csvData[counter].Add(SummaFixedListTotals.GetSalesTotalXTax(fixedListTotalNode));
                    }

                    csvData[counter].Add(SummaFixedListTotals.GetSalesTotalTaxAmount(fixedListTotalNode)); 

                    #endregion

                    csvData[counter].Add(SummaFixedListTotals.GetTimestamp(fixedListTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > fixedListTotals.Count))
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
                SummaException se = new SummaException("Problem processing fixed list totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
