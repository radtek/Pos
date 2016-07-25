using System;
using System.Xml;
using System.IO;
using Chefmate.Communication.EventArguments;
using Chefmate.Logger;
using ChefMate.Communication.Tcp;
using XMLManager;

namespace ChefMate.Communication.Receiver
{
    public class StreamConverterPos : StreamConverter
    {
        const int PosIpPort = 56001;
        string _fileFolder;
        public string FileFolder
        {
            get { return _fileFolder; }
            set { _fileFolder = value; }
        }

        public StreamConverterPos() : base()
        {
        }
        protected override void OpenReceiver()
        {
            this.StreamReceiver = new TcpStreamReceiver(PosIpPort);
        }
        protected override void OnDataReceived(object sender, StreamReceiverEventArgs streamArgs)
        {
            MemoryStream stream = streamArgs.Stream;
            string response = SaveStream(stream);
            streamArgs.Response = response;
        }
        protected string SaveStream(MemoryStream inStream)
        {
            StreamReader reader = new StreamReader(inStream);

            inStream.Position = 0;
            string dataString = reader.ReadToEnd();
            string streamResponse = SaveDataString(dataString);

            return streamResponse;
        }
        protected string SaveDataString(string inDataString)
        {
            string streamResponse = "";

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            // Convert to an xml object.
            XmlDocument xmlOrderDoc = new XmlDocument();
            string orderNo = "";

            try
            {
                xmlOrderDoc.LoadXml(inDataString);
                orderNo = OrderNoFromXmlData(xmlOrderDoc);

                try
                {
                    SaveXmlOrderToFile(OrderUri(orderNo), xmlOrderDoc);

                    LogEvent(@"readStreamFromSocket()", @"Chefmate Receiver has read a data stream");
                    streamResponse = BuildResponse(orderNo, true, @"").OuterXml;
                }
                catch (Exception exc)
                {
                    LogError(@"saveDataStream()", exc.Message);
                    streamResponse = BuildResponse(orderNo, false, exc.Message).OuterXml;
                }
            }
            catch (Exception exc)
            {
               
                LogError(@"saveDataStream()", exc.Message);
                streamResponse = BuildResponse(@"Invalid order number", false, exc.Message).OuterXml;
            }

            return streamResponse;
        }
        private string OrderNoFromXmlData(XmlDocument inXmlOrder)
        {
            XmlNode orderNode = XMLDocManager.GetRoot(inXmlOrder);

            return orderNode.Attributes[@"orderNumber"].Value;
        }
        private void SaveXmlOrderToFile(string inFilename, XmlDocument inXmlOrderDoc)
        {
            XmlTextWriter writer = new XmlTextWriter(inFilename, null);
            writer.Formatting = Formatting.Indented;
            inXmlOrderDoc.Save(writer);
            writer.Close();
        }
        private string OrderUri(string inOrderNo)
        {
            return string.Format(@"{0}\{1} {2}.xml", _fileFolder, DateTime.Now.ToString(@"yyyy_MM_dd HH_mm_ss"), inOrderNo);
        }
        private XmlDocument BuildResponse(string inOrderNo, bool inSuccess, string inMsg)
        {
            XmlDocument result = new XmlDocument();

            //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            XMLDocManager.CreateXmlDeclaration(result, @"1.0", @"", @"");
            XmlElement orderResponse = XMLDocManager.CreateRoot(result, @"Order");
            XMLDocManager.AddAttribute(result, orderResponse, @"action", @"orderResponse");
            XMLDocManager.AddAttribute(result, orderResponse, @"orderNumber", @"0");
            XMLDocManager.AddAttribute(result, orderResponse, @"result", inSuccess ? "OK" : "FAILED");
            XMLDocManager.AddAttribute(result, orderResponse, @"message", inMsg);

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            return result;
        }
        void LogEvent(string inFuncName, string inEventDescription)
        {
            string functUri = string.Format(@"ChefMate.Communication.Receiver.StreamConverterPOS.{0}", inFuncName);
            ChefmateLogger.Instance.LogEvent(functUri, inEventDescription);
        }

        void LogError(string inFuncName, string inMsg)
        {
            string functUri = string.Format(@"ChefMate.Communication.Receiver.StreamConverterPOS.{0}", inFuncName);
            ChefmateLogger.Instance.LogEvent(functUri, inMsg);
        }
    }
}
