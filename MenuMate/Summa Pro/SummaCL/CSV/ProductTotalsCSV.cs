using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
    public class ProductTotalsCSV : CsvMulti {

        public ProductTotalsCSV(string basePath) {
            //FULLFILEPATH = basePath + "ProductTotals.txt";
            FULLFILEPATH = basePath;
            AppendToFile = true;
            //fieldLengths = new int[] { 10, 10, 10, 20, 10, 10 };
            //shouldQuoteField = new bool[] { false, false, false, false, false, false };
            fieldLengths = new int[] { 10, 10, 10, 10, 20, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false, false };

            csvLineData = new List<StringBuilder>();
            csvData = new List<List<string>>();
        }

        public override void SetupData(XmlDocument doc) {
            XmlNodeList productTotals = null;
            try 
            {
                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.ProductTotals, SummaCommon.GetStoreId(doc));

                productTotals = doc.DocumentElement.SelectNodes("/TotalsProducts/Product");

                int counter = 0;
                foreach (XmlNode productTotalNode in productTotals) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaProductTotalsCommon.GetProductId(productTotalNode));
                    csvData[counter].Add(SummaProductTotalsCommon.GetQty(productTotalNode));

                    #region Old Specs
                    //  csvData[counter].Add(SummaProductTotalsCommon.GetTotalSales(productTotalNode));
                    #endregion

                    #region Specs v1.2
                    csvData[counter].Add(SummaProductTotalsCommon.GetTotalSalesXTax(productTotalNode));
                    csvData[counter].Add(SummaProductTotalsCommon.GetTotalTaxAmount(productTotalNode));
                    #endregion

                    csvData[counter].Add(SummaProductTotalsCommon.GetTimestamp(productTotalNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));
                    csvData[counter].Add(SummaCommon.GetTillId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > productTotals.Count))
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
                SummaException se = new SummaException("Problem processing product totals", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
