using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    public class FreeListCSV : CsvMulti {

        public FreeListCSV(string basePath) {
            //FULLFILEPATH = basePath + "FreeList.txt";
            FULLFILEPATH = basePath;

            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) 
        {
            XmlNodeList fixedNodes = null;
            try 
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.ListDiscountsXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.FreeList, SummaCommon.GetStoreId(doc));

                fixedNodes = doc.DocumentElement.SelectNodes("/ListDiscounts/Discount");

                int counter = 0;
                foreach (XmlNode fNode in fixedNodes) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaFreeListCommon.GetFreeListId(fNode));
                    csvData[counter].Add(SummaFreeListCommon.GetName(fNode));
                    csvData[counter].Add(SummaCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > fixedNodes.Count))
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
                SummaException se = new SummaException("Problem processing free list", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
