using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
	public class PatronTotals : CsvMulti {

		public PatronTotals(string basePath) 
        {
           //FULLFILEPATH = basePath + "PatronTotals.txt";
            FULLFILEPATH = basePath;

            fieldLengths = new int[] { 10, 10, 10 };
			shouldQuoteField = new bool[] { false, false, false };
			csvData = new List<List<string>>();
			csvLineData = new List<StringBuilder>();
			
			AppendToFile = true;
			
		}

		public override void SetupData(XmlDocument doc) 
        {
			XmlNodeList patronTotals = null;
			try
            {
                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.PatronCountsTotals, SummaCommon.GetStoreId(doc));

				patronTotals = doc.DocumentElement.SelectNodes("/TotalsPatron/PatronType");
				int c = 0;
				foreach (XmlNode patronTotalNode in patronTotals) {
					csvData.Add(new List<string>());

					csvData[c].Add(GetPatronId(patronTotalNode));
					csvData[c].Add(GetPatronCount(patronTotalNode));
					csvData[c].Add(GetTimestamp(patronTotalNode));
					csvData[c].Add(GetTillId(doc));
					csvData[c].Add(doc.DocumentElement.Attributes["SiteID"].Value);

					c++;
					csvLineData.Add(new StringBuilder());
				}
			}
			catch (Exception e) {
				SummaException se = new SummaException("Problem processing patron totals", e);
				se.Data = doc.OuterXml;
				throw se;
			}
		}

		private string GetTimestamp(XmlNode node) {
			return SummaCommon.GetSummaFormattedTimestamp(node.Attributes["Date"].Value,
				node.Attributes["Time"].Value);
		}

		private string GetTillId(XmlDocument doc) {
			return doc.DocumentElement.Attributes["TerminalId"].Value;
		}

		private string GetPatronId(XmlNode patronTotalNode) {
			StringBuilder b = new StringBuilder();
			b.Append(patronTotalNode.Attributes["id"].Value);
			byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
			UInt64 scarry = BitConverter.ToUInt32(bytes, 0);
			return scarry.ToString();
		}

		private string GetPatronCount(XmlNode patronTotalNode) {
			return patronTotalNode.Attributes["Qty"].Value;
		}
	}
}
