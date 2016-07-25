using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.XPath;
using NLog;

using System.Text;
using System.IO;

using LogFileManager;
using ConfigFileManager;
//using SummaCL.Global;

namespace SummaCL
{
    public class Summa : IPOSMessage
    {
/*
        public static Logger LOGGER = LogManager.GetLogger("Summa");
        public static Logger FAILED_TX_LOGGER = LogManager.GetLogger("SummaFailedTx");
        public static Logger RAW_XML = LogManager.GetLogger("RawXML");
*/
        private static FileLogger.LogErrorLevel _errorLevel = FileLogger.LogErrorLevel.None;
        private static FileLogger.LogErrorLevelFile _errorLevelFile = FileLogger.LogErrorLevelFile.None;
        private static string _errorStr = @"";
        private static string _errorStrDetail = @"";

        public static  FileLogger SingletonLoggerStatic  = FileLogger.Instance;
        private        FileLogger _singletonLogger        = FileLogger.Instance;
        public static ConfigurationFileManager SingletonConfigFile = ConfigurationFileManager.Instance;
        private ConfigurationFileManager _singletonConfigFile = ConfigurationFileManager.Instance;

        private string _outputFilePath;
        private string _outputFilePathIsAccessDoor;
        private bool _isDoorAccess = false;

        private static Dictionary<string, string> _summaSiteIdMap = null;

        public Summa(string outputFilePath, string outputFilePathIsAccessDoor, bool doorAccessFlag)
        {
            _outputFilePath = outputFilePath;
            _outputFilePathIsAccessDoor = outputFilePathIsAccessDoor;
            _isDoorAccess = doorAccessFlag;


            XmlDocument doc = null;
            try
            {
                doc = new XmlDocument();
                
                //XmlTextReader reader = new XmlTextReader(Application.StartupPath + System.IO.Path.DirectorySeparatorChar + @"summaIdMap.xml");
                //XmlTextReader reader = new XmlTextReader( StartupPath.text + System.IO.Path.DirectorySeparatorChar + @"summaIdMap.xml" );

                XmlTextReader reader = new XmlTextReader(_singletonConfigFile.ConfigPath);
                
                reader.Read();
                doc.Load(reader);
                reader.Close();

                Summa.BuildSummaIdMap(doc);
            }
            catch (System.Xml.XmlException xe)
            {
                _errorLevel = FileLogger.LogErrorLevel.SiteIDMapFailed;
                _errorLevelFile = FileLogger.LogErrorLevelFile.ConfigFile;
              
                _errorStr = @"Error loading Summa site id map file (problem with XML format?)";
                _errorStrDetail = @"";

                _singletonLogger.LogException(_errorLevel, _errorLevelFile, _errorStr, xe);
            }
            catch (Exception e)
            {
                _errorLevel = FileLogger.LogErrorLevel.SiteIDMapFailed;
                _errorLevelFile = FileLogger.LogErrorLevelFile.ConfigFile;

                _errorStr = @"Error loading Summa site id map file";

                _singletonLogger.LogException(_errorLevel, _errorLevelFile, _errorStr, e);
            }
        }

        //:::::::::::::::::::::::::::::::::::::::::

        public static FileLogger.LogErrorLevel ErrorLevel
        {
            get
            {
                return _errorLevel;
            }
            set
            {
                _errorLevel = value;
            }
        }

        //..........................................

        public static FileLogger.LogErrorLevelFile ErrorLevelFile
        {
            get
            {
                return _errorLevelFile;
            }
            set
            {
                _errorLevelFile = value;
            }
        }

        //.........................................

        public static string ErrorStrDetail
        {
            get
            {
                return _errorStrDetail;
            }
            set
            {
                _errorStrDetail = value;
            }
        }

        //:::::::::::::::::::::::::::::::::::::::::

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mmSiteId"></param>
        /// <returns></returns>
        public static string GetSummaSiteId(string mmSiteId)
        {

            string summaSiteId;

            if (_summaSiteIdMap != null)
            {
                if (_summaSiteIdMap.ContainsKey(mmSiteId))
                {
                    summaSiteId = _summaSiteIdMap[mmSiteId];
                }
                else
                {
                    summaSiteId = mmSiteId;

                    _errorLevel = FileLogger.LogErrorLevel.InvalidSiteID;
                    _errorStr = string.Format("Menumate site id not found, returning original value: {0}", mmSiteId);
                    
                    //_singletonLoggerStatic.LogError(_errorLevel, _errorLevelFile, _errorStr, _errorStrDetail);

                    Exception e = new Exception(_errorStr);
                    throw e;
                }
            }
            else
            {
                summaSiteId = mmSiteId;

                _errorLevel = FileLogger.LogErrorLevel.InvalidSiteID;
                _errorStr = string.Format("Summa site id map has not been initialised (missing id map file?), returning original value: {0}", mmSiteId);

                //_singletonLoggerStatic.LogError(_errorLevel, _errorLevelFile, _errorStr, _errorStrDetail);

                Exception e = new Exception(_errorStr);
                throw e;
            }
            return summaSiteId;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="data"></param>
        private static void BuildSummaIdMap(XmlDocument data)
        {

            // TODO: detect if this has been previously loaded and dont do it again.
            _summaSiteIdMap = new Dictionary<string, string>();

            XmlNodeList entries = null;
            try
            {
                entries = data.DocumentElement.SelectNodes("./SummaSiteIdMap/SummaIdMapEntry");

                foreach (XmlNode entry in entries)
                {
                    _summaSiteIdMap.Add(entry.Attributes["siteId"].Value, entry.Attributes["summaSiteId"].Value);
                }
            }
            catch (XPathException xpe)
            {
                throw xpe;
            }
        }

        void IPOSMessage.Process(XmlDocument document)
        {
            try
            {
                _errorLevel = FileLogger.LogErrorLevel.InternalXMLError;

                if( _isDoorAccess )
                {
                    ProcessDoorAccess(document);
                }
                else
                {
                    ProcessSummaBI(document);
                }
            }
            catch (SummaException se)
            {
                _singletonLogger.LogError(_errorLevel, _errorLevelFile, string.Format(@"{0} | {1}", _errorStr, se.StackTrace), se.Data);
                _singletonLogger.LogWarning(@"Summa failed processing a Menumate XML file.  See the log file for details");
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="data"></param>
        void ProcessDoorAccess(XmlDocument inDocument)
        {
            string documentType = inDocument.DocumentElement.Name;

            switch(documentType)
            {
                case "Transaction":
                    CsvMulti dacsv = new DoorAccessCSV(_outputFilePathIsAccessDoor);
                    dacsv.SetupData(inDocument);
                    dacsv.WriteDataToCSV();

                    //......................

                    exportDoorAccess(dacsv);
                break;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="data"></param>
        void ProcessSummaBI(XmlDocument inDocument)
        {
            string documentType = inDocument.DocumentElement.Name;

            //CheckCustomerID.Clear();

            switch (documentType)
            {
                #region Version

                case "Version":
                    CsvMulti vcsv = new VersionCsv(_outputFilePath);
                    vcsv.SetupData(inDocument);
                    vcsv.WriteDataToCSV();
                 break;

                #endregion

                #region Transaction Files

                case "Transaction":
                    CsvMulti tcsv = new TransactionCSV(_outputFilePath);
                    tcsv.SetupData(inDocument);
                    tcsv.WriteDataToCSV();

                    CsvMulti tdcsv = new TransactionDetailCSV(_outputFilePath);
                    tdcsv.SetupData(inDocument);
                    tdcsv.WriteDataToCSV();

                    CsvMulti tpmcsv = new TransactionPaymentMethodsCSV(_outputFilePath);
                    tpmcsv.SetupData(inDocument);
                    tpmcsv.WriteDataToCSV();
                break;

                #endregion

                #region List Files

                case "ListPaymentTypes":
                    CsvMulti lptcsv = new PaymentMethodTypesCSV(_outputFilePath);
                    lptcsv.SetupData(inDocument);
                    lptcsv.WriteDataToCSV();
                    break;

                case "CategoryGroups":
                    CsvMulti gcsv = new GroupsCSV(_outputFilePath);
                    gcsv.SetupData(inDocument);
                    gcsv.WriteDataToCSV();
                    break;

                case "ListStaff":
                    CsvMulti ocsv = new OperatorsCSV(_outputFilePath);
                    ocsv.SetupData(inDocument);
                    ocsv.WriteDataToCSV();
                    break;

                case "ListCategories":
                    CsvMulti ccsv = new DepartmentsCSV(_outputFilePath);
                    ccsv.SetupData(inDocument);
                    ccsv.WriteDataToCSV();
                    break;

                case "ListProductTypes":
                    CsvMulti pcsv = new ProductsCSV(_outputFilePath);
                    pcsv.SetupData(inDocument);
                    pcsv.WriteDataToCSV();
                    break;

                case "ListDiscounts":
                    CsvMulti dcsv = new FreeListCSV(_outputFilePath);
                    dcsv.SetupData(inDocument);
                    dcsv.WriteDataToCSV();
                    break;

                case "ListCalculated":
                    CsvMulti listCalcCsv = new FixedListCSV(_outputFilePath);
                    listCalcCsv.SetupData(inDocument);
                    listCalcCsv.WriteDataToCSV();
                    break;

                case "ListPatronTypes":
                    //CsvMulti listPatronTypes = new FixedListPatrons(_outputFilePath);
                    //listPatronTypes.SetupData(document);
                    //listPatronTypes.WriteDataToCSV();
                    break;

                #endregion

                #region Totals Files

                case "TotalsPayments":
                    //CsvMulti tpcsv = new PaymentMethodTotalsCSV(_outputFilePath);
                    //tpcsv.SetupData(document);
                    //tpcsv.WriteDataToCSV();
                    break;

                case "TotalsStaff":
                    //CsvMulti tscsv = new OperatorTotalsCSV(_outputFilePath);
                    //tscsv.SetupData(document);
                    //tscsv.WriteDataToCSV();
                    break;

                case "TotalsGroups":
                    //CsvMulti tgcsv = new GroupTotalsCSV(_outputFilePath);
                    //tgcsv.SetupData(document);
                    //tgcsv.WriteDataToCSV();
                    break;

                case "TotalsProducts":
                    //CsvMulti tprdcsv = new ProductTotalsCSV(_outputFilePath);
                    //tprdcsv.SetupData(document);
                    //tprdcsv.WriteDataToCSV();
                    break;

                case "TotalsCategories":
                    //CsvMulti tccsv = new DepartmentTotalsCSV(_outputFilePath);
                    //tccsv.SetupData(document);
                    //tccsv.WriteDataToCSV();
                    break;

                case "TotalsDiscounts":
                    CsvMulti fltcsv = new FreeListTotalsCSV(_outputFilePath);
                    fltcsv.SetupData(inDocument);
                    fltcsv.WriteDataToCSV();
                    break;

                case "TotalsCalculated":
                    //CsvMulti fixedListCsv = new FixedListTotals(_outputFilePath);
                    //fixedListCsv.SetupData(document);
                    //fixedListCsv.WriteDataToCSV();
                    break;

                case "TotalsHourlySales":
                    //CsvMulti hourlySalesCsv = new HourlySalesCSV(_outputFilePath);
                    //hourlySalesCsv.SetupData(document);
                    //hourlySalesCsv.WriteDataToCSV();
                    break;

                case "TotalsPatron":
                    //CsvMulti patronTotals = new PatronTotals(_outputFilePath);
                    //patronTotals.SetupData(document);
                    //patronTotals.WriteDataToCSV();
                    break;
                #endregion

                #region Membership
                case "UpdateMember":
                    CsvMulti membersListCsv = new CustomersCSV(_outputFilePath);
                    membersListCsv.SetupData(inDocument);
                    membersListCsv.WriteDataToCSV();
                    break;

                case "ListMemberGroups":
                    CsvMulti groupsListCsv = new SummaGroupsCSV(_outputFilePath);
                    groupsListCsv.SetupData(inDocument);
                    groupsListCsv.WriteDataToCSV();
                    break;
                #endregion

                #region Roster

                case "ListRoster":
                    CsvMulti rosterCsv = new RosterCSV(_outputFilePath);
                    rosterCsv.SetupData(inDocument);
                    rosterCsv.WriteDataToCSV();
                    break;

                #endregion

                default:
                    {
                        _errorLevel = FileLogger.LogErrorLevel.InternalXMLError;
                        _errorLevelFile = FileLogger.LogErrorLevelFile.None;

                        _errorStr = @"Unknown Summa message type: " + documentType;
                        _errorStrDetail = @"";

                        _singletonLogger.LogError(_errorLevel, _errorLevelFile, _errorStr, _errorStrDetail);
                        _singletonLogger.LogWarning(@"Summa failed processing a Menumate XML file.  See the log file for details");

                        throw new SummaException(_errorStr);
                    }
            }
        }

        void exportDoorAccess(CsvMulti inCSV)
        {
            StringBuilder target = new StringBuilder(@"");
            string source = @"";

            try
            {
                string folder = System.IO.Path.GetDirectoryName(inCSV.FullFilePath);

                string[] filenames = System.IO.Directory.GetFiles(folder);

                foreach (string filename in filenames)
                {
                    source = filename;

                    target.Append(_singletonConfigFile.IsDoorAccessTargetFolder);
                    target.AppendFormat("{0}", System.IO.Path.GetFileName(source));

                    if (!File.Exists(target.ToString()))
                    {
                        System.IO.File.Copy(source, target.ToString());

                        _singletonLogger.Log(string.Format(@"File {0} has been copied to {1}", System.IO.Path.GetFileName(source), Summa.SingletonConfigFile.IsDoorAccessTargetFolder));
                    }

                    target.Clear();
                }
            }
            catch (Exception e)
            {
                _singletonLogger.LogWarning(string.Format(@"Failed to copy {0} to {1}. Message: {2}", System.IO.Path.GetFileName(source), Summa.SingletonConfigFile.IsDoorAccessTargetFolder, e.Message));
            }
        }
    }
}
