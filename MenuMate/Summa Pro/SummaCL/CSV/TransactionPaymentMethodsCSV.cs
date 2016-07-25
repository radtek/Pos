using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using NLog;

using LogFileManager;

namespace SummaCL
{
    public class TransactionPaymentMethodsCSV : CsvMulti
    {
        public TransactionPaymentMethodsCSV(string basePath) 
        {
            //FULLFILEPATH = basePath + "TransactionPaymentMethods.txt";
            FULLFILEPATH = basePath;

            AppendToFile = true;
            SEPERATOR = ",";
            fieldLengths = new int[] { 10, 10, 10, 100, 10, 10 };
            shouldQuoteField = new bool[] { false, false, false, false, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc)
        {
            XmlNodeList methods = null;
            try 
            {
                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.TransactionXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.TransactionPaymentMethods, SummaTransactionCommon.GetStoreId(doc));

                methods = doc.DocumentElement.SelectNodes("/Transaction/Receipt/Payment");

                if (methods.Count == 0) 
                {
                    Summa.SingletonLoggerStatic.Log(string.Format(string.Format("No payment methods for txn # {0}",
                        SummaTransactionCommon.GetTransactionID(doc))));

                    //Summa.LOGGER.Log(LogLevel.Warn, string.Format("No payment methods for txn # {0}",
                    //    SummaTransactionCommon.GetTransactionID(doc)));
                }

                int counter = 0;
                foreach (XmlNode paymentNode in methods) {

                    csvData.Add(new List<string>());
                    csvData[counter].Add(SummaTransactionCommon.GetTransactionPaymentID(paymentNode));
                    csvData[counter].Add(SummaTransactionCommon.GetTransactionID(doc));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetPaymentId(paymentNode));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetCardNumber(paymentNode));
                    csvData[counter].Add(SummaPaymentMethodsCommon.GetSalesValue(paymentNode));
                    csvData[counter].Add(SummaTransactionCommon.GetStoreId(doc));

                    counter++;    
                    csvLineData.Add(new StringBuilder());
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing transaction payment methods", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
