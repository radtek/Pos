using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    /// <summary>
    /// Appends a single entry to the Version.txt csv file. 
    /// </summary>
    public class VersionCsv : CsvMulti
    {
        public VersionCsv(string basePath)
        {
            FULLFILEPATH = basePath;

            AppendToFile = true;
            SEPERATOR = ",";
            
            fieldLengths = new int[] { 100, 100 };
            shouldQuoteField = new bool[] { true, true };

            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        /// <summary>
        /// Deconstruct xml document and populate internal data fields
        /// </summary>
        /// <param name="doc"></param>
        public override void SetupData(XmlDocument doc)
        {
            try
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.VersionXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.Version, SummaCommon.GetStoreId(doc));

                csvData.Add(new List<string>());
                csvLineData.Add(new StringBuilder());

                csvData[0].Add(SummaCommon.GetPOSVersion(doc));
                csvData[0].Add(SummaCommon.GetInterfaceVersion(doc));
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem processing version", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
