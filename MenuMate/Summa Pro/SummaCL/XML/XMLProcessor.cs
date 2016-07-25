using System;
using System.IO;
using System.Net.Sockets;
using System.Xml;
using System.Xml.XPath;
using NLog;

using LogFileManager;
using ConfigFileManager;

namespace SummaCL
{
	public class XMLProcessor 
    {
        private FileLogger _singletonLogger = FileLogger.Instance;
		//private static Logger logger = LogManager.GetLogger("Summa");

		private NetworkStream NetStream;

		public XMLProcessor(NetworkStream inNetStream) 
        {
			NetStream = inNetStream;
		}

		/// <summary>Starts communication with client.</summary>
		public void Process(string Transaction, string outputFilePath, string outputFilePathIsAccessDoor, bool doorAccessFlag)
        {
			StreamWriter streamWriter = new StreamWriter(NetStream);
			try 
            {
				// All writes will be done immediately and not cached:
				streamWriter.AutoFlush = true;

				// Start loop and handle commands:
			    // Convert to an xml object.
				XmlDocument xmlData = new XmlDocument();
                xmlData.LoadXml(Transaction);

                // USED TO DEBUG. Save the xmal file reveived from Menumate
                //xmlData.Save(string.Format(@"{0}XMLData{1}.xml", @".\archive\", Transaction.GetHashCode()));

                string messageId = SummaCommon.GetMessageId(xmlData);

				if (xmlData.FirstChild.Name == "xml") 
                {
					if (xmlData.DocumentElement.Name == "Request") 
                    {
						if (xmlData.DocumentElement.Attributes.GetNamedItem("Type").Value == "Status") 
                        {
							// Send the xml responce.
							XmlDocument xmlResponse = BuildResponse(messageId, "ACK", "");
                            string hResponse = ResponseWithHeader(xmlResponse.OuterXml);
							streamWriter.WriteLine(hResponse);

							// Process the xml object.
                            _singletonLogger.Log("XML Response : " + hResponse);
						}
						else 
                        {
                            streamWriter.WriteLine(ResponseWithHeader(BuildResponse("1", "NAK", "Unknown Request Type").OuterXml));

                            _singletonLogger.LogError(FileLogger.LogErrorLevel.InternalXMLError,
                                                      FileLogger.LogErrorLevelFile.RequestXMLFile,
                                                      @"Error occured: Not an xml Document", Transaction);

                            _singletonLogger.LogWarning(@"Summa failed processing a Menumate XML file.  See the log file for details");
						}
					}
					else 
                    {
						// create instance of summa interface
                        IPOSMessage summa = new Summa(outputFilePath, outputFilePathIsAccessDoor, doorAccessFlag);

						try 
                        {
							summa.Process(xmlData);
							streamWriter.WriteLine(ResponseWithHeader(BuildResponse(messageId, "ACK", "").OuterXml));

                            _singletonLogger.Log("Transaction OK, id = " + messageId);
						}
						catch (SummaException se) 
                        {
							// fail
							streamWriter.WriteLine(ResponseWithHeader(BuildResponse(messageId, "NAK", se.Message).OuterXml));

                            //_singletonLogger.LogError("Summa returned an error, " + se.ToString());
                            //_singletonLogger.LogException(@"Failed to process an XML file", se);

                            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                            // Save the document to a file and auto-indent the output.
                            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                            SaveXML.SaveInFolder(@"XML Failed", xmlData);
						}
						catch (XPathException xpe) 
                        {    
							// fail
							streamWriter.WriteLine(ResponseWithHeader(BuildResponse(messageId, "NAK", xpe.Message).OuterXml));
                            
                            //_singletonLogger.LogError("Summa returned an error, " + xpe.ToString());
                            //_singletonLogger.LogException(@"Failed to process an XML file", xpe);

                            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                            // Save the document to a file and auto-indent the output.
                            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                            SaveXML.SaveInFolder(@"XML Failed", xmlData);
						}
					}
				}
				else 
                {
					string message = "Not an XML Document";
					streamWriter.WriteLine(ResponseWithHeader(BuildResponse("1", "NAK", message).OuterXml));

                    _singletonLogger.LogError(FileLogger.LogErrorLevel.InternalXMLError,
                                              FileLogger.LogErrorLevelFile.UnknownXMLFile,
                                              "Not an XML Document: ", Transaction);

                    _singletonLogger.LogWarning(@"Summa failed processing a Menumate XML file.  See the log file for details");


					//logger.Error("Error occured: " + message);

                    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                    // Save the document to a file and auto-indent the output.
                    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                    SaveXML.SaveInFolder(@"XML Failed", xmlData);
				}
			}
			catch (Exception ex) 
            {
				// Generate a negitive xml responce here.
				streamWriter.WriteLine(ResponseWithHeader(BuildResponse("1", "NAK", ex.Message).OuterXml));

                _singletonLogger.LogError(FileLogger.LogErrorLevel.InternalXMLError,
                                          FileLogger.LogErrorLevelFile.UnknownXMLFile,
                                          string.Format(@"An Exception occured: {0}", ex.Message), 
                                          Transaction);

                _singletonLogger.LogWarning(@"Summa failed processing a Menumate XML file.  See the log file for details");

                //_singletonLogger.LogError(@"Raw Data Received: " + Transaction);

				//logger.Error("An Exception occured: " + ex.ToString());
				//logger.Error("Raw Data Received: " + Transaction);

                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                // Save Transaction string.
                //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
                SaveXML.SaveInFolder(@"XML Failed", Transaction);
			}
			finally 
            {
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

		public XmlDocument BuildResponse(string id, string status, string message) 
        {
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

		/// <summary>
		/// Build a response with a 10 character header to send to POS.
		/// </summary>
		/// <param name="id">The transaction id</param>
		/// <param name="status">The status of the response, ACK or NAK</param>
		/// <param name="message">Additional messages</param>
		/// <returns>An XML document</returns>
        /// 
        public string ResponseWithHeader(string inResponse)
        {
            string header = string.Format("{0:0000000000}", inResponse.Length);

            return header + inResponse;
        }
	}
}
