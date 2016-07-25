using System.Collections.Generic;
using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [XmlRoot(ElementName = "Options")]
    public class PosOptions
    {
        [XmlElement(ElementName = "Option")]
        public List<PosOption> Options { get; set; }
    }

    [XmlRoot(ElementName = "Option")]
    public class PosOption
    {
        [XmlElement(ElementName = "PrintFormat")]
        public PrintFormat PrintFormat { get; set; }
        [XmlAttribute(AttributeName = "key")]
        public string Key { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "kitchenName")]
        public string KitchenName { get; set; }
        [XmlAttribute(AttributeName = "isPlus")]
        public string IsPlus { get; set; }
    }
}