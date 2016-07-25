using System.Collections.Generic;
using System.Xml;

namespace FileMate.Core.Xml {
    public class CopyGroupDTO {


        private List<CopyOperationDTO> _operations = new List<CopyOperationDTO>();

        public List<CopyOperationDTO> Operations {
            get { return _operations; }
        }

        public CopyGroupDTO() {
        }

        public CopyGroupDTO(XmlDocument doc) {

            XmlNodeList operationNodeList = Helper.GetCopyOperationNodes(doc);

            foreach (XmlNode opNode in operationNodeList) {
                CopyOperationDTO copyOp = new CopyOperationDTO(opNode);
                _operations.Add(copyOp);
            }
        }

        public XmlDocument AsXml() {

            XmlDocument fileMateSettingsDoc = new XmlDocument();
            // write out the xml header

            fileMateSettingsDoc.AppendChild(fileMateSettingsDoc.CreateNode(XmlNodeType.XmlDeclaration, "", ""));

            // write out the root node and copygroup node
            XmlNode rootNode = fileMateSettingsDoc.CreateNode(XmlNodeType.Element, "FileMate", "");

            XmlNode copyGroupNode = fileMateSettingsDoc.CreateNode(XmlNodeType.Element, "CopyGroup", "");

            fileMateSettingsDoc.AppendChild(rootNode);
            rootNode.AppendChild(copyGroupNode);

            foreach (CopyOperationDTO co in Operations) {
                // create operation node and append it to copy group node
                copyGroupNode.AppendChild(co.AsXml(fileMateSettingsDoc));
            }

            return fileMateSettingsDoc;
        }
    }
}


