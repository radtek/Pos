using System;
using System.Collections.Generic;
using System.Text;
using NLog;
using System.Net.Sockets;
using System.Xml;
using System.IO;

namespace IntaMate {
    public class XMLProcessor {
        private static Logger logger = LogManager.GetLogger("IntaMateLog");
        private NetworkStream NetStream;

        public XMLProcessor(NetworkStream inNetStream) {
            NetStream = inNetStream;
        }

        /// <summary>Starts communication with client.</summary>
        public void Process(string Transaction, string outputFilePath) {
            
            StreamWriter streamWriter = new StreamWriter(NetStream);
            try {
                // All writes will be done immediately and not cached:
                streamWriter.AutoFlush = true;

                // Start loop and handle commands:
                //logger.Debug("XML Received :" + Transaction);
                // Convert to an xml object.
                XmlDocument xmlData = new XmlDocument();

                // TODO exception handling!!
                xmlData.LoadXml(Transaction);

                string messageId = SummaCommon.GetMessageId(xmlData);

                //logger.Info("Creating output files in " + outputFilePath);

                if (xmlData.FirstChild.Name == "xml") {
                    if (xmlData.DocumentElement.Name == "Request") {
                        if (xmlData.DocumentElement.Attributes.GetNamedItem("Type").Value == "Status") {
                            // Send the xml responce.
                            XmlDocument xmlResponse = BuildResponse(messageId, "ACK", "");
                            streamWriter.WriteLine(xmlResponse.OuterXml);

                            // Process the xml object.
                            logger.Debug("XML Response : " + xmlResponse.OuterXml);
                        }
                        else {
                            streamWriter.WriteLine(BuildResponse("1", "NAK", "Unknown Request Type").OuterXml);
                            logger.Error("Error occured: Not an xml Document");
                        }
                    }
                    else {
                        // create instance of summa interface
                        IPOSMessage summa = new Summa(outputFilePath);
                        

                        try {
                            summa.Process(xmlData);
                            streamWriter.WriteLine(BuildResponse(messageId, "ACK", "").OuterXml);
                            logger.Info("Transaction OK, id = " + messageId);
                        }
                        catch (SummaException se) {
                            // fail
                            streamWriter.WriteLine(BuildResponse(messageId, "NAK", se.Message).OuterXml);
                            Summa.logger.Error("Summa returned an error, " + se.ToString());
                            Summa.failedTxLogger.Error(se.Data);
                        }
                    }
                }
                else {
                    string message = "Not an XML Document";
                    streamWriter.WriteLine(BuildResponse("1", "NAK", message).OuterXml);
                    logger.Error("Error occured: " + message);
                }
            }
            catch (Exception ex) {
                // Generate a negitive xml responce here.
                streamWriter.WriteLine(BuildResponse("1", "NAK", ex.Message).OuterXml);
                logger.Error("An Exception occured: " + ex.ToString());
                logger.Error("Raw Data Received: " + Transaction);
            }
            finally {
                streamWriter.Close();
                streamWriter.Dispose();
            }
        }

        /// <summary>
        /// Build an XML response to send to POS.
        /// </summary>
        /// <param name="id">The transaction id</param>
        /// <param name="status">The status of the response, ACK or NAK</param>
        /// <param name="message">Additional messages</param>
        /// <returns>An XML document</returns>

        private XmlDocument BuildResponse(string id, string status, string message) {
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
