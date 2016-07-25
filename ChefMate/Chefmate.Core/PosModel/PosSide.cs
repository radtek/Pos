using System.Collections.Generic;
using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [XmlRoot(ElementName = "Sides")]
    public class PosSides
    {
        [XmlElement(ElementName = "Side")]
        public List<PosSide> Sides { get; set; }
    }

    [XmlRoot(ElementName = "Side")]
    public class PosSide
    {
        [XmlElement(ElementName = "PrintFormat")]
        public PrintFormat PrintFormat { get; set; }
        [XmlElement(ElementName = "Options")]
        public PosOption Options { get; set; }
        [XmlAttribute(AttributeName = "key")]
        public string Key { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "kitchenName")]
        public string KitchenName { get; set; }
    }
}