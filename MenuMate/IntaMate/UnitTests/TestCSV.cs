using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IntaMate;
using System.IO;

namespace UnitTests {
    public class TestCSV : CsvMulti {

        public TestCSV() {
            FULLFILEPATH = Path.GetTempPath() + "Test.txt";
            fieldLengths = new int[] { 10, 5 };
            shouldQuoteField = new bool[] { true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) {

            csvData.Add(new List<string>());
            csvData.Add(new List<string>());
            csvData.Add(new List<string>());

            csvLineData.Add(new StringBuilder());
            csvLineData.Add(new StringBuilder());
            csvLineData.Add(new StringBuilder());

            // first field too long and quoted, second field too long
            csvData[0].Add("1234567890123");
            csvData[0].Add("abcdefg");

            // first field exact length and quoted, second exact length
            csvData[1].Add("1234567890");
            csvData[1].Add("abcde");

            // first field less than max. length and quoted, second less than max. length
            csvData[2].Add("123");
            csvData[2].Add("abc");
        }
    }
}
