using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SummaCL
{
	public class FixedListPatrons : CsvMulti {

		public FixedListPatrons(string basePath) 
        {
			//FULLFILEPATH = basePath + "PatronTypes.txt";
            FULLFILEPATH = basePath;

			fieldLengths = new int[] { 10, 20, 10 };
			shouldQuoteField = new bool[] { false, true, false };
			csvData = new List<List<string>>();
			csvLineData = new List<StringBuilder>();
		}

		public override void SetupData(XmlDocument doc) 
        {
            FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.ListPatronsTypes, SummaCommon.GetStoreId(doc));

            XmlNodeList patrons = null;
			try
            {
				patrons = doc.DocumentElement.SelectNodes("/ListPatronTypes/PatronType");
				int c = 0;
				foreach (XmlNode patronNode in patrons) {
					csvData.Add(new List<string>());

					csvData[c].Add(GetPatronId(patronNode));
					csvData[c].Add(GetPatronName(patronNode));
					csvData[c].Add(GetStoreId(doc));
					c++;
					csvLineData.Add(new StringBuilder());
				}
			}
			catch (Exception e) {
				SummaException se = new SummaException("Problem processing patron types", e);
				se.Data = doc.OuterXml;
				throw se;
			}
		}

		private string GetStoreId(XmlDocument doc) {
			return doc.DocumentElement.Attributes["SiteID"].Value;
		}

		private string GetPatronName(XmlNode node) {
			return node.Attributes["Name"].Value;
		}

		public static string GetPatronId(XmlNode node) {
			StringBuilder b = new StringBuilder();
			b.Append(node.Attributes["id"].Value);
			byte[] bytes = BitConverter.GetBytes(b.ToString().GetHashCode());
			UInt64 scarry = BitConverter.ToUInt32(bytes, 0);
			return scarry.ToString();
		}
	}
}
