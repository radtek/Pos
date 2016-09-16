using System;
using System.IO;
using System.Text;
using System.Xml;
using MenumateServices.Properties;
using MenumateUtils.Encrypt;
using MenumateUtils.Helpers;
using SalesForceCommunication.Domain;
using XMLManager;


namespace MenumateServices.DTO.SalesForce
{
    public class SalesForceCredentialFactory
    {
        private const string ConfigurationFileName = "LoyaltyMateCredentials.info";

        public SalesForceCredential Create()
        {

            try
            {
                var configurationFileUrl = Helper.Instance.SystemFileURI(ConfigurationFileName);

                if (File.Exists(configurationFileUrl))
                {
                    byte[] decryptedFileBytes = null;
                    MMEncrypt.Instance.Decrypt(configurationFileUrl, EncryptAttributes.Instance.Password, ref decryptedFileBytes);

                    var xmlFileContent = Encoding.Default.GetString(decryptedFileBytes);

                    if (xmlFileContent.IndexOf('<') == -1)
                        throw new Exception("Credentials file missing the xml start tag");

                    int startIndex = xmlFileContent.IndexOf('<');
                    int length = xmlFileContent.LastIndexOf('>') + 1 - startIndex;
                    xmlFileContent = xmlFileContent.Substring(startIndex, length);

                    var xmlDocument = new XmlDocument();
                    xmlDocument.LoadXml(xmlFileContent);

                    var salesForceCredential = new SalesForceCredential
                    {
                        UserName = "menumate@menumate.com",
                        Password = "Loyaltymate@1358",//"Menu1203mate@SF",
                        SecurityToken = "oLbdKUyN9j6k9T15ocIx4ZZz"// "JAznA5075w7OdVZG8UTTztUyt"
                    };

                    return salesForceCredential;
                }
                //return null;
            }
            catch (Exception exception)
            {
                ServiceLogger.LogException("Exception in UpdateSalesForceVersion", exception);
            }
            return null;
        }

        private string LoadAttribute(XmlDocument xmlDocument, string attribute)
        {
            try
            {
                XmlNode salesforceNode = XMLDocManager.GetNode(xmlDocument, @"salesforce");
                return XMLDocManager.GetAttribute(salesforceNode, attribute, @"");
            }
            catch (Exception exception)
            {
                throw new Exception("There was an error retrieving query from sales force.") { Source = exception.Source };
            }
        }
    }
}