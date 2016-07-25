using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    class SummaGroupsCSV : CsvMulti
    {
        public SummaGroupsCSV(string basePath)
        {
            //FULLFILEPATH = basePath + "Groups.txt";
            FULLFILEPATH = basePath;

            SEPERATOR = ",";

            fieldLengths = new int[] { 10, 100, 1, 10 };
            shouldQuoteField = new bool[] { false, true, false, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc)
        {
            XmlNodeList groups = null;
            try
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.ListMemberGroupsXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.CustomerRanking, NewSummaGroupsCommon.GetStoreId(doc));

                groups = doc.DocumentElement.SelectNodes("/ListMemberGroups/Group");

                int counter = 0;
                foreach (XmlNode groupNode in groups)
                {
                    csvData.Add(new List<string>());

                    csvData[counter].Add(NewSummaGroupsCommon.GetGroupId(groupNode));
                    csvData[counter].Add(NewSummaGroupsCommon.GetGroupName(groupNode));
                    csvData[counter].Add(NewSummaGroupsCommon.GetStatus(groupNode));
                    csvData[counter].Add(NewSummaGroupsCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem processing groups", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
