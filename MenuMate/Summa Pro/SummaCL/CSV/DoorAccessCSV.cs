using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;
using System.IO;

namespace SummaCL
{
    public class DoorAccessCSV : CsvMulti
    {

        private string _basePath;

        public DoorAccessCSV(string basePath)
        {

            _basePath = basePath;

            AppendToFile = true;
            SEPERATOR = ",";
            fieldLengths = new int[] { 16, 25, 25, 25, 25, 100, 100 };
            shouldQuoteField = new bool[] { false, false, false, false, false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc)
        {
            try
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.TransactionXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;


                // build unique filename
                FULLFILEPATH = GenerateCGFileName(doc);

                XmlNodeList products = null;
                XmlNodeList cards = null;

                products = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Product");
                cards = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Member/MagCards/MagCard");
                int counter = 0;

                foreach (XmlNode productNode in products)
                {
                    foreach (XmlNode cardNode in cards)
                    {
                        csvData.Add(new List<string>());
                        csvData[counter].Add(SummaTransactionCommon.GetTransactionID(doc));	// txn id
                        csvData[counter].Add(SummaTransactionCommon.GetCustomerId(doc));	// customer id

                        csvData[counter].Add(GetTag(cardNode));	// card tag
                        csvData[counter].Add(GetMemberNumber(doc));	// member number

                        csvData[counter].Add(SummaTransactionProductCommon.GetProductId(productNode)); // product id
                        //csvData[counter].Add(GetProxCard(doc));	// proc card

                        csvData[counter].Add(GetProductName(productNode)); // product name
                        csvData[counter].Add(SummaTransactionCommon.GetDateStamp(doc)); // Date Stamp

                        //csvData[counter].Add(GetProxCard(doc));	// prox card

                        counter++;

                        csvLineData.Add(new StringBuilder());
                    }
                }
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem processing transaction detail", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }

        private string GetTag(XmlNode cardNode)
        {
            return cardNode.Attributes["Card"].Value;
        }

        private string GetProductName(XmlNode productNode)
        {
            StringBuilder b = new StringBuilder();

            b.Append(productNode.Attributes["Item"].Value);
            string sizeValue = productNode.Attributes["Size"].Value;
            if (!sizeValue.Equals("Default"))
            {
                b.Append(sizeValue);
            }
            return b.ToString();
        }

        private string GetMemberNumber(XmlDocument doc)
        {
            XmlNode member = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt/Member");

            return member.Attributes["MemNumber"].Value;
        }

        private string GetProxCard(XmlDocument doc)
        {
            XmlNode member = doc.DocumentElement.SelectSingleNode("/Transaction/Receipt/Member");

            return member.Attributes["ProxCard"].Value;
        }

        private string GenerateFileName(XmlDocument doc)
        {
            StringBuilder b = new StringBuilder(_basePath);
            b.AppendFormat(@"\ACCESS_{0}_{1}.csv", DateTime.Now.Ticks, SummaTransactionCommon.GetTerminalID(doc));
            return b.ToString();
        }

        private string GenerateCGFileName(XmlDocument doc) // Calendar Girl
        {
            StringBuilder b = new StringBuilder(_basePath);
            b.AppendFormat(@"{0}_pos1girlproximitytag{1}.csv", SummaTransactionCommon.GetDateStamp(doc), GetMemberNumber(doc));
            return b.ToString();
        }

    }
}
