using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using NLog;
using System.Windows.Forms;
using System.Xml.XPath;

namespace IntaMate {
    public class Summa : IPOSMessage {

        public static Logger logger = LogManager.GetLogger("IntaMateLog");
        public static Logger failedTxLogger = LogManager.GetLogger("SummaFailedTx");
        private string _outputFilePath;

        private static Dictionary<string, string> _summaSiteIdMap = null;

        public Summa(string outputFilePath) {
            _outputFilePath = outputFilePath;


        }

        public static string GetSummaSiteId(string mmSiteId) {

            if (_summaSiteIdMap == null) {
                XmlDocument doc = null;
                try {
                    doc = new XmlDocument();
                    XmlTextReader reader = new XmlTextReader(Application.StartupPath + System.IO.Path.DirectorySeparatorChar + @"summaIdMap.xml");
                    reader.Read();
                    doc.Load(reader);

                    Summa.BuildSummaIdMap(doc);

                }
                catch (Exception e) {
                    Console.WriteLine("Error loading Summa site id map file: {0}", e.ToString());
                    //throw;
                }
            }

            string summaSiteId;
            try {
                if (_summaSiteIdMap != null) {
                    summaSiteId = _summaSiteIdMap[mmSiteId];
                }
                else {
                    summaSiteId = mmSiteId;
                    logger.Log(LogLevel.Error, "Summa site id map has not been initialised (missing id map file?), returning original value");
                }
            }
            catch (KeyNotFoundException knfe) {
                summaSiteId = mmSiteId;
                logger.LogException(LogLevel.Error, "Menumate site id not found, returning original value", knfe);
            }

            return summaSiteId;
        }

        private static void BuildSummaIdMap(XmlDocument data) {


            _summaSiteIdMap = new Dictionary<string, string>();

            XmlNodeList entries = null;
            try {
                entries = data.DocumentElement.SelectNodes("/SummaSiteIdMap/SummaIdMapEntry");
            }
            catch (XPathException xpe) {
                throw xpe;
            }

            foreach (XmlNode entry in entries) {
                _summaSiteIdMap.Add(entry.Attributes["siteId"].Value,
                    entry.Attributes["summaSiteId"].Value);
            }

        }

        void IPOSMessage.Process(XmlDocument document) {

            string documentType = document.DocumentElement.Name;

            try {
                switch (documentType) {

                    #region Transaction Files

                    case "Transaction":
                        CsvMulti tcsv = new TransactionCSV(_outputFilePath);
                        tcsv.SetupData(document);
                        tcsv.WriteDataToCSV();

                        CsvMulti tdcsv = new TransactionDetailCSV(_outputFilePath);
                        tdcsv.SetupData(document);
                        tdcsv.WriteDataToCSV();

                        CsvMulti tpmcsv = new TransactionPaymentMethodsCSV(_outputFilePath);
                        tpmcsv.SetupData(document);
                        tpmcsv.WriteDataToCSV();
                        break;

                    #endregion

                    #region List Files

                    case "ListPaymentTypes":
                        CsvMulti lptcsv = new PaymentMethodTypesCSV(_outputFilePath);
                        lptcsv.SetupData(document);
                        lptcsv.WriteDataToCSV();
                        break;

                    case "ListGroups":
                        CsvMulti gcsv = new GroupsCSV(_outputFilePath);
                        gcsv.SetupData(document);
                        gcsv.WriteDataToCSV();
                        break;

                    case "ListStaff":
                        CsvMulti ocsv = new OperatorsCSV(_outputFilePath);
                        ocsv.SetupData(document);
                        ocsv.WriteDataToCSV();
                        break;

                    case "ListCategories":
                        CsvMulti ccsv = new DepartmentsCSV(_outputFilePath);
                        ccsv.SetupData(document);
                        ccsv.WriteDataToCSV();
                        break;

                    case "ListProductTypes":
                        CsvMulti pcsv = new ProductsCSV(_outputFilePath);
                        pcsv.SetupData(document);
                        pcsv.WriteDataToCSV();
                        break;

                    case "ListDiscounts":
                        CsvMulti dcsv = new FreeListCSV(_outputFilePath);
                        dcsv.SetupData(document);
                        dcsv.WriteDataToCSV();
                        break;

                    case "ListCalculated":
                        CsvMulti listCalcCsv = new FixedListCSV(_outputFilePath);
                        listCalcCsv.SetupData(document);
                        listCalcCsv.WriteDataToCSV();
                        break;

                    #endregion

                    #region Totals Files

                    case "TotalsPayments":
                        CsvMulti tpcsv = new PaymentMethodTotalsCSV(_outputFilePath);
                        tpcsv.SetupData(document);
                        tpcsv.WriteDataToCSV();
                        break;

                    case "TotalsStaff":
                        CsvMulti tscsv = new OperatorTotalsCSV(_outputFilePath);
                        tscsv.SetupData(document);
                        tscsv.WriteDataToCSV();
                        break;

                    case "TotalsGroups":
                        CsvMulti tgcsv = new GroupTotalsCSV(_outputFilePath);
                        tgcsv.SetupData(document);
                        tgcsv.WriteDataToCSV();
                        break;

                    case "TotalsProducts":
                        CsvMulti tprdcsv = new ProductTotalsCSV(_outputFilePath);
                        tprdcsv.SetupData(document);
                        tprdcsv.WriteDataToCSV();
                        break;

                    case "TotalsCategories":
                        CsvMulti tccsv = new DepartmentTotalsCSV(_outputFilePath);
                        tccsv.SetupData(document);
                        tccsv.WriteDataToCSV();
                        break;

                    case "TotalsDiscounts":
                        CsvMulti fltcsv = new FreeListTotalsCSV(_outputFilePath);
                        fltcsv.SetupData(document);
                        fltcsv.WriteDataToCSV();
                        break;

                    case "TotalsCalculated":
                        CsvMulti fixedListCsv = new FixedListTotals(_outputFilePath);
                        fixedListCsv.SetupData(document);
                        fixedListCsv.WriteDataToCSV();
                        break;

                    case "TotalsHourlySales":
                        CsvMulti hourlySalesCsv = new HourlySalesCSV(_outputFilePath);
                        hourlySalesCsv.SetupData(document);
                        hourlySalesCsv.WriteDataToCSV();
                        break;
                    #endregion

                    #region Membership

                    case "ListMembers":
                        CsvMulti membersListCsv = new CustomersCSV(_outputFilePath);
                        membersListCsv.SetupData(document);
                        membersListCsv.WriteDataToCSV();
                        break;
                    #endregion

                    #region Roster

                    case "ListRoster":
                        CsvMulti rosterCsv = new RosterCSV(_outputFilePath);
                        rosterCsv.SetupData(document);
                        rosterCsv.WriteDataToCSV();
                        break;

                    #endregion

                    default:
                        throw new SummaException("Unknown Summa message type: " + documentType);
                }
            }
            catch (SummaException) {
                throw;
            }
        }
    }
}
