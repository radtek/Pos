using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class FixedListCSV : CsvMulti {

        public FixedListCSV(string basePath) {
            FULLFILEPATH = basePath + "FixedList.txt";
            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {
             XmlNodeList discounts = null;
             try {
                 discounts = doc.DocumentElement.SelectNodes("/ListCalculated/Calculated");


                 int counter = 0;
                 foreach (XmlNode discountNode in discounts) {

                     csvData.Add(new List<string>());

                     csvData[counter].Add(SummaFixedListCommon.GetFixedListId(discountNode));
                     csvData[counter].Add(SummaFixedListCommon.GetName(discountNode));
                     csvData[counter].Add(SummaCommon.GetStoreId(doc));

                     counter++;
                     csvLineData.Add(new StringBuilder());
                 }
             }
             catch (Exception e) {
                 SummaException se = new SummaException("Problem processing fixed list", e);
                 se.Data = doc.OuterXml;
                 throw se;
             }
        }
    }
}
