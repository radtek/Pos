using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Xml;
using System.Xml.XPath;
using System.IO;

using System.Windows.Forms;
using NLog;

using Ionic.Zip;

namespace SummaCL
{
    //.............................................
    // This is a singleton class to set the Startup Path as the Application variable is not accessible in an Class Library 
    // neither is Application.StartupPath 
    // The Startup Path defaults to "", it must be set to an existing path before it's used. 
    //.............................................
    public sealed class StartupPath
    {
        public static string text = "";

        StartupPath() 
        { 
        }

        public static StartupPath Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly StartupPath instance = new StartupPath();
        }

        public static void Show() //It's just used for testing.  It's actually not needed
        {
            MessageBox.Show(text);
        }

    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to create a LogLevel.Debug Logger 
    // It is usefull in Development time only. 
    //.............................................
    public sealed class DebugLog
    {
        public static string text = "";
        private static Logger _logger = LogManager.GetLogger("Summa");

        DebugLog()
        {
        }

        public static DebugLog Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly DebugLog instance = new DebugLog();
        }

        public static void Log(string aLogLine)
        {
            _logger.Log(LogLevel.Debug, aLogLine);
        }

        public static void Error(string inMsg)
        {
            _logger.Error(inMsg);
        }
    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to create .csv Summa-style file names 
    //.............................................
    public sealed class SummaFileName
    {
        public enum SummaFileType
        {
            PaymentMethodTypes, Products, Categories, SubCategories, Operators, DailyTotalList, FreeList,

            Transactions, TransactionDetails, TransactionPaymentMethods, TransactionCheck,

            DailyTotals, FreeListTotals, HourlyTotals, OperatorsTotals, PaymentTotals, ProductTotals,
            CategoriesTotals, GroupTotals, PatronCountsTotals,

            Customers, CustomerRanking, ListPatronsTypes,

            Version
        };

        private static string FileExtention = ".txt";

        SummaFileName()
        {
        }

        public static SummaFileName Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly SummaFileName instance = new SummaFileName();
        }

        public static string GetZIPName(string inPath)
        {
            string[] filenames = System.IO.Directory.GetFiles(inPath);
            string siteID = @"00";

            foreach (string filename in filenames)
            {
                try
                {
                    if (filename.EndsWith(@".txt"))
                    {
                        string tmp = System.IO.Path.GetFileName(filename);

                        siteID = tmp.Substring(0, tmp.IndexOf("_"));
                        break;
                    }
                }
                catch(Exception)
                {
                }
            }

            return inPath + siteID + "_" + DateTimeToSummaFileNamePrefix(DateTime.Now) + "_" + "Upload" + ".zip";
        }

        public static string GetName(SummaFileType inFileType, string inStoreID)
        {
            switch (inFileType)
            {
                #region Lookup Files
                case SummaFileType.PaymentMethodTypes: return GetFileNamePrefix(inStoreID) + "PaymentType" + FileExtention;
                case SummaFileType.Products: return GetFileNamePrefix(inStoreID) + "Products" + FileExtention;
                case SummaFileType.Categories: return GetFileNamePrefix(inStoreID) + "Categories" + FileExtention;
                case SummaFileType.SubCategories: return GetFileNamePrefix(inStoreID) + "Subcategories" + FileExtention;
                case SummaFileType.Operators: return GetFileNamePrefix(inStoreID) + "Operators" + FileExtention;
                case SummaFileType.DailyTotalList: return GetFileNamePrefix(inStoreID) + "DailyTotalList" + FileExtention;
                case SummaFileType.FreeList: return GetFileNamePrefix(inStoreID) + "FreeList" + FileExtention;
                #endregion

                #region Sales (Transaction) Files
                case SummaFileType.Transactions: return GetFileNamePrefix(inStoreID) + "Transactions" + FileExtention;
                case SummaFileType.TransactionDetails: return GetFileNamePrefix(inStoreID) + "TransactionDetails" + FileExtention;
                case SummaFileType.TransactionPaymentMethods: return GetFileNamePrefix(inStoreID) + "TransactionPaymentMethods" + FileExtention;
                case SummaFileType.TransactionCheck: return "TransactionCheck" + FileExtention;
                //case SummaFileType.TransactionCheck: return GetFileNamePrefix(inStoreID) + "TransactionCheck" + FileExtention;
                #endregion

                #region Totals Files
                case SummaFileType.DailyTotals: return GetFileNamePrefix(inStoreID) + "DailyTotals" + FileExtention;
                case SummaFileType.FreeListTotals: return GetFileNamePrefix(inStoreID) + "FreeListTotals" + FileExtention;
                case SummaFileType.HourlyTotals: return GetFileNamePrefix(inStoreID) + "HourlyTotals" + FileExtention;
                case SummaFileType.OperatorsTotals: return GetFileNamePrefix(inStoreID) + "OperatorTotals" + FileExtention;
                case SummaFileType.PaymentTotals: return GetFileNamePrefix(inStoreID) + "PaymentTotals" + FileExtention;
                case SummaFileType.ProductTotals: return GetFileNamePrefix(inStoreID) + "ProductTotals" + FileExtention;
                case SummaFileType.CategoriesTotals: return GetFileNamePrefix(inStoreID) + "CategoriesTotals" + FileExtention;
                case SummaFileType.GroupTotals: return GetFileNamePrefix(inStoreID) + "GroupTotals" + FileExtention;
                case SummaFileType.PatronCountsTotals: return GetFileNamePrefix(inStoreID) + "PatronCountsTotals" + FileExtention;
                #endregion

                case SummaFileType.Customers: return GetFileNamePrefix(inStoreID) + "Customers" + FileExtention;
                case SummaFileType.CustomerRanking: return GetFileNamePrefix(inStoreID) + "CustomerRanking" + FileExtention;
                case SummaFileType.ListPatronsTypes: return GetFileNamePrefix(inStoreID) + "ListPatronsTypes" + FileExtention;

                case SummaFileType.Version: return GetFileNamePrefix(inStoreID) + "Version" + FileExtention;
            }

            #region Not supported Summa File Type

            SummaException se = new SummaException("Summa file type not supported");
            se.Data = "File type: " + Convert.ToString((int)inFileType);
            throw se;

            #endregion
        }

        private static string GetFileNamePrefix(string inStoreID)
        {
            return inStoreID + "_" + DateTimeToSummaFileNamePrefix(DateTime.Now) + "_";
        }

        private static string DateTimeToSummaFileNamePrefix(DateTime inDateTime)
        {
            return inDateTime.ToString("yyyyMMddHHmmss");
        }

    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to generate Summa item IDs (Hash Code) 
    //.............................................
    public sealed class IDGenerator
    {
        IDGenerator() 
        { 
        }

        public static IDGenerator Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly IDGenerator instance = new IDGenerator();
        }

        public static string GetNLenID(string inID, int inLen)
        {
            string buffer = inID;

            if (buffer.Length > 10)
            {
                return buffer.Substring(0, inLen);
            }
            else
            {
                return buffer;
            }
        }

        public static string GetTransactionDetailID(string inTransactionID, string inProductName)
        {
            string buffer = inTransactionID + inProductName + DateTime.Now.ToString("yyyyMMddHHmmssFFFFFFF");
            string hashCodeStr = Convert.ToString(Math.Abs(buffer.GetHashCode()));

            return GetNLenID(hashCodeStr, 10);
        }
    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to Calculate Unit Price
    //.............................................
    public sealed class UnitPrice
    {
        UnitPrice() 
        { 
        }

        public static UnitPrice Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly UnitPrice instance = new UnitPrice();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTotal"></param>
        /// <param name="inQty"></param>
        /// <returns></returns>
        public static string CalcAsString(string inTotal, string inQty)
        {
            string result = @"0.00";

            //....................................................

            decimal totalDecimal = Convert.ToDecimal(inTotal);
            decimal qtyDecimal = Convert.ToDecimal(inQty);

            if (qtyDecimal != 0)
            {
               if (qtyDecimal > 0)
               {
                   // NORMAL SALE AND SALE OF AN ITEM WITH A NEGATIVE PRICE
                   result = string.Format("{0:#.00}", totalDecimal / qtyDecimal);
               }
               else // REFUND!!!!!!
               {
                   result = string.Format("{0:#.00}", Math.Abs(totalDecimal / qtyDecimal));
               }
            }
            else // QUANTITY = 0
            {
                result = string.Format("{0:#.00}", totalDecimal);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTotal"></param>
        /// <param name="inQty"></param>
        /// <param name="inTax"></param>
        /// <returns></returns>
        public static string CalcAsString(string inPrice, string inQty, string inTax)
        {
            // result = (B/A) - (D/A) = Unit Price
            // A      = Quantity: inQty
            // B      = Net Price: inPrice
            // D      = Tx: inTax

            string result = @"0.00";

            //....................................................

            decimal A = Convert.ToDecimal(inQty);
            decimal B = Convert.ToDecimal(inPrice);
            decimal D = Convert.ToDecimal(inTax);

            decimal resultDecimal = (B / A) - (D / A);

            if (A != 0)
            {
                if (A > 0)
                {
                    // NORMAL SALE AND SALE OF AN ITEM WITH A NEGATIVE PRICE
                    result = string.Format("{0:#.00}", resultDecimal);
                }
                else // REFUND!!!!!!
                {
                    result = string.Format("{0:#.00}", Math.Abs(resultDecimal));
                }
            }
            else // QUANTITY = 0
            {
                result = string.Format("{0:#.00}", B);
            }

            //....................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTotal"></param>
        /// <param name="inQty"></param>
        /// <returns></returns>
        public static string CalcTaxAsString(string inTax, string inQty)
        {
            // result = (D/A) = Unit Tax
            // A      = Quantity: inQty
            // D      = Tx: inTax

            string result = @"0.00";

            //....................................................

            decimal A = Convert.ToDecimal(inQty);
            decimal D = Convert.ToDecimal(inTax);

            decimal resultDecimal = (D / A);

            if (A != 0)
            {
                result = string.Format("{0:#.00}", resultDecimal);
            }
            else // QUANTITY = 0
            {
                result = string.Format("{0:#.00}", D);
            }

            //....................................................

            return result;
        }
    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to create ZIP files contaning all the CSV files in a transaction
    //.............................................

    public sealed class SummaZIP
    {
        SummaZIP() 
        { 
        }

        public static SummaZIP Instance
        {
            get
            {
               return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly SummaZIP instance = new SummaZIP();
        }

        public static bool SaveWithFiles(List<string> inFiles, string inZipFileName)
        {
            try
            {
                bool result = false;

                using (ZipFile zip = new ZipFile())
                {
                    foreach (string filename in inFiles)
                    {
                        if(!filename.EndsWith(@".zip"))
                        {
                            zip.AddFile(filename, @"");
                        }
                    }

                    if(zip.Count > 0)
                    {
                        zip.Save(inZipFileName);

                        result = true;
                    }
                }

                return result;
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem creating Summa ZIP file. " + inZipFileName, e);

                throw e;
            }
        }

        public static bool SaveWithFolder(string inFolder, string inZipFileName)
        {
            try
            {
                // note: this does not recurse directories! 
                string[] filenames = System.IO.Directory.GetFiles(inFolder);

                List<string> fileList = new List<string>();

                foreach (string filename in filenames)
                {
                    //fileList.Add(System.IO.Path.GetFileName(filename));
                    fileList.Add(filename);
                }

                if (fileList.Count > 0)
                {
                    return SaveWithFiles(fileList, inZipFileName);
                }
                else
                {
                    DebugLog.Log(String.Format("No files found in folder {0}", inFolder));

                    return false;
                }
            }
            catch (Exception e)
            {
                SummaException se = new SummaException("Problem creating Summa ZIP file. Source folder not found ...", e);
                throw se;
            }
         }

        public static bool SaveAndClean(string inFolder, string inZipFileName)
        {
            string tmp = @"";

            try
            {
                 bool result = SaveWithFolder(inFolder, inZipFileName);

                //:::::::::::::::::::::::::::::::::::::

                if (result)
                {
                    string[] filenames = System.IO.Directory.GetFiles(inFolder);

                    foreach (string filename in filenames)
                    {
                        tmp = filename;

                        if (!tmp.EndsWith(@".zip"))
                        {
                            System.IO.File.Delete(tmp);
                        }
                    }
                }

                return result;
             }
            catch (Exception e)
            {
                SummaException se = new SummaException(string.Format(@"Problem deleting file: {0}  ...", tmp), e);
                throw se;

                //SummaException se = new SummaException("Problem deleting Summa .txt file ...", e);
                //throw se;
            }
        }
    }

    //:::::::::::::::::::::::::::::::::::::

    //.............................................
    // This is a singleton class to save XML files.  StartupPath.text must be set before using this class
    //.............................................

    public sealed class SaveXML
    {
        SaveXML()
        {
        }

        public static SaveXML Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly SaveXML instance = new SaveXML();
        }

        public static void SaveInFolder(string inFolderName, XmlDocument inDoc)
        {
            string xmlPath = String.Format(@"{0}\{1}", StartupPath.text, inFolderName);

            try
            {
                // If the directory doesn't exist, create it.
                if (!Directory.Exists(xmlPath))
                {
                    Directory.CreateDirectory(xmlPath);
                }
            }
            catch (Exception e)
            {
                DebugLog.Error("Creating XML file folder, " + e.ToString());
            }

            Random randomValue = new Random();
            string randomValueStr = Convert.ToString(randomValue.Next());

            XmlTextWriter writer = new XmlTextWriter(String.Format(@"{0}\{1}_{2}.xml", xmlPath, inDoc.DocumentElement.Name, Convert.ToString(randomValue.GetHashCode())), null);

            writer.Formatting = Formatting.Indented;
            inDoc.Save(writer);
        }

        public static void SaveInFolder(string inFolderName, string inString)
        {
            string filePath = String.Format(@"{0}\{1}", StartupPath.text, inFolderName);

            try
            {
                // If the directory doesn't exist, create it.
                if (!Directory.Exists(filePath))
                {
                    Directory.CreateDirectory(filePath);
                }
            }
            catch (Exception e)
            {
                DebugLog.Error("Creating XML file folder, " + e.ToString());
            }

            Random randomValue = new Random();
            string randomValueStr = Convert.ToString(randomValue.Next());

            //:::::::::::::::::::::::::::::::::

            Byte[] buffer = Encoding.UTF8.GetBytes(inString);
            FileStream wStream = File.Open(String.Format(@"{0}\MMTransaction_{1}.txt", filePath, Convert.ToString(randomValue.GetHashCode())), FileMode.CreateNew, FileAccess.Write);
            wStream.Write(buffer, 0, buffer.Length);
        }
    }

    //:::::::::::::::::::::::::::::::::::::
}
