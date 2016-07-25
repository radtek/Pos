using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using IntaMate;
using System.IO;

//string[] outputFiles = new string[] {
//     "PaymentTypes.txt", "Products.txt", "Departments.txt", "Groups.txt",
//    "Operators.txt", "FreeList.txt", "FixedList.txt", "Transactions.txt", "TransactionDetails.txt",
//    "TransactionPaymentMethods.txt", "TransPaymentTotals.txt", "ProductTotals.txt", "DepartmentTotals.txt",
//    "GroupTotals.txt", "OperatorTotals.txt", "FixedListTotals.txt", "FreeListTotals.txt", "HourlyTotals.txt",
//    "Customers.txt"
//    };

namespace SummaCLI {
    class Program {
        static void Main(string[] args) {

            // process command line
            if (args.Length > 0) {

                switch (args[0]) {

                    case "--process-failed":
                        if (args.Length == 2) {
                            ProcessFailed(args[1]);
                        }
                        else {
                            Console.WriteLine("No file specified, or too many arguments!");
                        }
                        break;

                    default:
                        Console.WriteLine("Command not recognised!");
                        break;
                }
            }
            else {
                Console.WriteLine("No command specified!");
            }
        }


        private static void ProcessFailed(string filename) {

            // look in the current directory for the file
            StringBuilder tmpFullFilePath = new StringBuilder(Environment.CurrentDirectory);
            tmpFullFilePath.Append(@"\");
            tmpFullFilePath.Append(filename);
            string fullFilePath = tmpFullFilePath.ToString();

            try {

                string line;
                StreamReader reader = new StreamReader(fullFilePath);
                while ((line = reader.ReadLine()) != null) {

                    // trim off the logging parts to get to the xml
                    StringBuilder tmpLine = new StringBuilder(line);
                    tmpLine.Remove(0, @"2010-02-15 10:00:49.7500|ERROR|Process|".Length);
                    tmpLine.Remove(tmpLine.Length - 1, 1);
                    line = tmpLine.ToString();

                    XmlDocument doc = new XmlDocument();
                    StringReader sr = new StringReader(line);
                    doc.Load(sr);

                    string transactionId = SummaCommon.GetMessageId(doc);
                    IPOSMessage summa = new Summa(Environment.CurrentDirectory + @"\");

                    try {
                        summa.Process(doc);
                        Console.WriteLine(BuildResponse(transactionId, "ACK", "").OuterXml);

                    }
                    catch (SummaException e) {
                        Console.WriteLine(BuildResponse(transactionId, "NAK", e.ToString()).OuterXml);
                        Console.WriteLine("Summa returned an error;\n" + e.Data);
                    }
                }
            }
            catch (Exception e) {
                Console.WriteLine(e.ToString());
            }

        }

        private static XmlDocument BuildResponse(string id, string status, string message) {
            XmlDocument xdoc2 = new XmlDocument();
            xdoc2.AppendChild(xdoc2.CreateXmlDeclaration("1.0", "UTF-8", "yes"));
            XmlWriter docWriter = xdoc2.CreateNavigator().AppendChild();
            docWriter.WriteStartElement("Response", "");
            docWriter.WriteAttributeString("id", "", id);
            docWriter.WriteAttributeString("Status", "", status);
            docWriter.WriteAttributeString("Message", "", message);
            docWriter.WriteEndElement();
            docWriter.Close();

            XmlWriter w2 = XmlWriter.Create(Console.Out);
            xdoc2.WriteTo(w2);
            Console.WriteLine();
            Console.WriteLine();
            w2.Flush();

            return xdoc2;
        }
    }
}
