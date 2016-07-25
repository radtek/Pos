using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [XmlRoot(ElementName = "PrintFormat")]
    public class PrintFormat
    {
        [XmlAttribute(AttributeName = "key")]
        public string Key { get; set; }
        [XmlAttribute(AttributeName = "underline")]
        public string Underline { get; set; }
        [XmlAttribute(AttributeName = "bold")]
        public string Bold { get; set; }
        [XmlAttribute(AttributeName = "color")]
        public string Color { get; set; }
        [XmlAttribute(AttributeName = "largeFont")]
        public string LargeFont { get; set; }
        [XmlAttribute(AttributeName = "doubleWidth")]
        public string DoubleWidth { get; set; }
        [XmlAttribute(AttributeName = "doubleHeight")]
        public string DoubleHeight { get; set; }
    }
}