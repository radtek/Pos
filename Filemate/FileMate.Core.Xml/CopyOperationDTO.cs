using System.Collections.Generic;
using System.Xml;

namespace FileMate.Core.Xml {
    public class CopyOperationDTO {

        private string _name;

        public string Name {
            get { return _name; }
            set { _name = value; }
        }
        private bool _deleteSource;

        public bool DeleteSource {
            get { return _deleteSource; }
            set { _deleteSource = value; }
        }
        private bool _calledFromExternal;

        public bool CalledFromExternal {
            get { return _calledFromExternal; }
            set { _calledFromExternal = value; }
        }
        private string _sourceUrl;

        public string SourceUrl {
            get { return _sourceUrl; }
            set { _sourceUrl = value; }
        }
        private string _sourceUname;

        public string SourceUname {
            get { return _sourceUname; }
            set { _sourceUname = value; }
        }
        private string _sourcePwd;

        public string SourcePwd {
            get { return _sourcePwd; }
            set { _sourcePwd = value; }
        }
        private List<FileMateTargetDTO> _targets = new List<FileMateTargetDTO>();

        public List<FileMateTargetDTO> Targets {
            get { return _targets; }
        }
        private string _schedule;

        public string Schedule {
            get { return _schedule; }
            set { _schedule = value; }
        }
        private string _pattern;

        public string Pattern {
            get { return _pattern; }
            set { _pattern = value; }
        }

        private bool _enabled;

        public bool Enabled
        {
            get { return _enabled; }
            set { _enabled = value; }
        }

        public CopyOperationDTO() { }

        public CopyOperationDTO(XmlNode opNode) {

            _name = Helper.GetCopyOperationName(opNode);
            _deleteSource = Helper.GetDeleteSourceValueForOperation(opNode);
            _calledFromExternal = Helper.GetCalledFromForOperation(opNode);
            _enabled = Helper.GetCopyOperationEnabled(opNode);
            

            _sourceUrl = Helper.GetSourceUrl(Helper.GetSourceNodeForOperation(opNode));

            XmlNode credentialsNode = Helper.GetSourceCredentialsNode(opNode);
            if (credentialsNode != null) {
                Helper.GetSourceUsernameAndPassword(opNode, out _sourceUname, out _sourcePwd);
            }

            XmlNodeList targetNodeList = Helper.GetTargetsForOperation(opNode);
            foreach (XmlNode targetNode in targetNodeList) {
                FileMateTargetDTO target = new FileMateTargetDTO(targetNode);
                _targets.Add(target);
            }

            _schedule = Helper.GetScheduleForOperation(opNode);
            _pattern = Helper.GetFilePatternForOperation(opNode);

        }

        public XmlNode AsXml(XmlDocument doc) {
            XmlNode opNode = doc.CreateNode(XmlNodeType.Element, "CopyOperation", "");

            XmlAttribute opName = doc.CreateAttribute("name");
            opName.InnerText = Name;
            XmlAttribute opDeleteSource = doc.CreateAttribute("deleteSource");
            opDeleteSource.InnerText = DeleteSource.ToString().ToLower();
            XmlAttribute opCalledExternal = doc.CreateAttribute("calledFromExternal");
            opCalledExternal.InnerText = CalledFromExternal.ToString().ToLower();
            XmlAttribute opEnabled = doc.CreateAttribute("enabled");
            opEnabled.InnerText = Enabled ? "true" : "false";

            opNode.Attributes.Append(opName);
            opNode.Attributes.Append(opDeleteSource);
            opNode.Attributes.Append(opCalledExternal);
            opNode.Attributes.Append(opEnabled);

            // source
            XmlAttribute sourceName = doc.CreateAttribute("name");
            XmlNode sourceNode = doc.CreateNode(XmlNodeType.Element, "source", "");
            XmlAttribute sourceUrl = doc.CreateAttribute("url");
            sourceUrl.InnerText = Helper.FormatUrl(SourceUrl);
            sourceNode.Attributes.Append(sourceUrl);
            sourceName.InnerText = "source...";
            sourceNode.Attributes.Append(sourceName);


            if (SourceUname != string.Empty && SourceUname != null &
                SourcePwd != string.Empty && SourcePwd != null) {

                XmlNode sourceCredNode = doc.CreateNode(XmlNodeType.Element, "credentials", "");

                XmlAttribute srcUName = doc.CreateAttribute("username");
                srcUName.InnerText = SourceUname;
                sourceCredNode.Attributes.Append(srcUName);

                XmlAttribute srcPwd = doc.CreateAttribute("password");
                srcPwd.InnerText = SourcePwd;
                sourceCredNode.Attributes.Append(srcPwd);

                sourceNode.AppendChild(sourceCredNode);
            }

            opNode.AppendChild(sourceNode);

            // targets
            XmlNode targetsNode = doc.CreateNode(XmlNodeType.Element, "targets", "");
            foreach (FileMateTargetDTO targetDto in Targets) {
                targetsNode.AppendChild(targetDto.AsXml(doc));
            }

            opNode.AppendChild(targetsNode);

            // schedule
            XmlNode scheduleNode = doc.CreateNode(XmlNodeType.Element, "schedule", "");
            scheduleNode.InnerText = Schedule;
            opNode.AppendChild(scheduleNode);

            // pattern
            XmlNode patternNode = doc.CreateNode(XmlNodeType.Element, "pattern", "");
            patternNode.InnerText = Pattern;
            opNode.AppendChild(patternNode);

            return opNode;
        }
    }
}
