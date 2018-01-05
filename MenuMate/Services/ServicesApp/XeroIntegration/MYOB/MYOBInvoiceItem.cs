using System.Xml.Serialization;

namespace AccountingIntegration.MYOB
{
    [XmlRoot(ElementName = "Item")]
    public class Item
    {
        [XmlAttribute(AttributeName = "GLCode")]
        public string GLCode { get; set; }
        [XmlAttribute(AttributeName = "Description")]
        public string Description { get; set; }
        [XmlAttribute(AttributeName = "UnitAmount")]
        public string UnitAmount { get; set; }
        [XmlAttribute(AttributeName = "TaxName")]
        public string TaxStatus { get; set; }
        [XmlAttribute(AttributeName = "TaxAmount")]
        public string TaxAmount { get; set; }
    }
}
