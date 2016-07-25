using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;
using System.Xml;
using IntaMate;

namespace UnitTests {
    /// <summary>
    /// Summary description for CsvMultiUnitTest
    /// </summary>
    [TestClass]
    public class CsvMultiUnitTest {
        public CsvMultiUnitTest() {
            // delete the Test.txt file
            File.Delete(Path.GetTempPath() + "Test.txt");
        }

        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext {
            get {
                return testContextInstance;
            }
            set {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        //
        // You can use the following additional attributes as you write your tests:
        //
        // Use ClassInitialize to run code before running the first test in the class
        // [ClassInitialize()]
        // public static void MyClassInitialize(TestContext testContext) { }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        // [ClassCleanup()]
        // public static void MyClassCleanup() { }
        //
        // Use TestInitialize to run code before running each test 
        // [TestInitialize()]
        // public void MyTestInitialize() { }
        //
        // Use TestCleanup to run code after each test has run
        // [TestCleanup()]
        // public void MyTestCleanup() { }
        //
        #endregion


        /// <summary>
        /// Read in a process an xml file (transaction)
        /// Check that the time format is in 24hrs clock format
        /// </summary>
        [TestMethod]
        public void Test24HrDateAndTime() {

            // load xml document
            // transaction on the 29/12/2009 at 13:30:00 
            // for $3.50
            string xml_data = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Transaction id=\"563550-0001\"><XMLVersion Version=\"1.0\" /><ProductVersion Product=\"MenuMate\" Version=\"4.13\" /><Terminal TerminalId=\"1\" TerminalName=\"SLASH\" Location=\"Restaurant\" SiteID=\"-1\" /><Sender id=\"1\" XmlID=\"2\" Name=\"z-Admin\" PayrollID=\"0\" Initials=\"MM\" /><Destination Name=\"Unknown\" To=\"Unknown\" Folio=\"Unknown\" /><Receipt ReceiptNumber=\"563550\" TransactionType=\"Purchase\" Date=\"29/12/2009\" Time=\"13:30:00\" TotalPayment=\"3.50\" TotalProduct=\"3.50\" TotalDiscount=\"0.00\" TotalSurcharges=\"0.00\"><Payment id=\"\" Name=\"Cash\" Code=\"\" Total=\"3.50\" Pay=\"3.50\" CashOut=\"0.00\" Surcharge=\"0.00\" /><Product id=\"\" Menu=\"Restaurant\" Course=\"Dinner\" Item=\"Garlic Bread\" Size=\"Default\" Qty=\"1.00\" ServingCourse=\"No Serving Course\" Code=\"\"><PriceChanges /><Metrics Date=\"29/12/2009\" Time=\"13:30:00\" GroupName=\"Restaurant Total\" CategoryName=\"Dinner\" /><Pricing OrderType=\"0\" HappyHour=\"0\" Total=\"3.50\" AdjustmentTotal=\"0.00\" PriceLevel0=\"3.50\" PriceLevel1=\"3.50\" PointsRedeemed=\"0.00\" PointsPercent=\"5.00\" PointsEarned=\"0.00\" /><Source TableNo=\"0\" SeatNo=\"0\" RoomNo=\"0\" TabName=\"Sale\" Pax=\"1\" /></Product></Receipt></Transaction>";
            XmlDocument doc = new XmlDocument();
            doc.Load(new StringReader(xml_data));

            // process
            TransactionCSV TxnCsv = new TransactionCSV(string.Empty);
            TxnCsv.SetupData(doc);
            TxnCsv.WriteDataToCSV();

            // check output
            string line;
            string[] fields;

            using (TextReader tr = new StreamReader(TxnCsv.FullFilePath)) {
                // read the first line
                line = tr.ReadLine();
                fields = line.Split(',');

                // test that the time format is 24hr
                Assert.AreEqual(@"20091229133000", fields[5]);
            }
        }

        [TestMethod]
        public void TestCSVBasic() {
            TestCSV tcsv = new TestCSV();
            tcsv.SetupData(null);
            tcsv.WriteDataToCSV();

            string line;
            string[] fields;

            // open the Test.csv file and read it in
            using (TextReader tr = new StreamReader(Path.GetTempPath() + "Test.txt")) {

                // read the first line
                line = tr.ReadLine();
                fields = line.Split(',');

                // first field too long and quoted, second field too long
                Assert.AreEqual(@"""1234567890""", fields[0]);
                Assert.AreEqual("abcde", fields[1]);

                // read the second line
                line = tr.ReadLine();
                fields = line.Split(',');

                // first field exact length and quoted, second exact length
                Assert.AreEqual(@"""1234567890""", fields[0]);
                Assert.AreEqual("abcde", fields[1]);

                // read the third line
                line = tr.ReadLine();
                fields = line.Split(',');

                // first field exact length and quoted, second exact length
                Assert.AreEqual(@"""123""", fields[0]);
                Assert.AreEqual("abc", fields[1]);

            }

        }
    }
}
