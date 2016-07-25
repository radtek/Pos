using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    public class GroupsCSV : CsvMulti {

        public GroupsCSV(string basePath)
        {
            //FULLFILEPATH = basePath + "Groups.txt";
            FULLFILEPATH = basePath;

            SEPERATOR = ",";

            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc) 
        {
            try
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.ListGroupsXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.Categories, SummaCommon.GetStoreId(doc));

                XmlNodeList groups = null;

                groups = doc.DocumentElement.SelectNodes("/CategoryGroups/Group");

                int counter = 0;
                foreach (XmlNode groupNode in groups) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaGroupsCommon.GetGroupId(groupNode));
                    csvData[counter].Add(SummaGroupsCommon.GetGroupName(groupNode));
                    csvData[counter].Add(SummaGroupsCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1) || (counter > groups.Count))
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
                SummaException se = new SummaException("Problem processing groups", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
