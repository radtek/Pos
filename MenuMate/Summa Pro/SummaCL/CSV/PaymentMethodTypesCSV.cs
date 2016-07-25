using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

using LogFileManager;

namespace SummaCL
{
    public class PaymentMethodTypesCSV: CsvMulti {

        public PaymentMethodTypesCSV(string basePath) 
        {
            //FULLFILEPATH = basePath + "PaymentTypes.txt";
            FULLFILEPATH = basePath;

            fieldLengths = new int[] { 10, 100, 10 };
            shouldQuoteField = new bool[] { false, true, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(System.Xml.XmlDocument doc) 
        {
            try 
            {
                XmlNodeList payments = null;

                // In case GetStoreId fails
                Summa.ErrorLevelFile = FileLogger.LogErrorLevelFile.ListPaymentTypesXMLFile;
                Summa.ErrorStrDetail = doc.DocumentElement.Attributes["id"].Name;

                FULLFILEPATH += SummaFileName.GetName(SummaFileName.SummaFileType.PaymentMethodTypes, SummaCommon.GetStoreId(doc));

                payments = doc.DocumentElement.SelectNodes("/ListPaymentTypes/Payment");

                int counter = 0;
                foreach (XmlNode paymentTypeNode in payments) {

                    csvData.Add(new List<string>());

                    csvData[counter].Add(SummaPaymentTypeCommon.GetPaymentTypeId(paymentTypeNode));
                    csvData[counter].Add(SummaPaymentTypeCommon.GetPaymentTypeName(paymentTypeNode));
                    csvData[counter].Add(SummaPaymentTypeCommon.GetStoreId(doc));

                    counter++;
                    csvLineData.Add(new StringBuilder());

/*
                    if ((counter == 1 ) || (counter > payments.Count))
                    {
                        csvLineData.Add(new StringBuilder());
                    }
                    else
                    {
                        csvLineData.Add(new StringBuilder(","));
                    }
*/
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing payment method types", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
