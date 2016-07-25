using System.Collections.Generic;
using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [XmlRoot(ElementName = "Items")]
    public class PosItems
    {
        [XmlElement(ElementName = "Item")]
        public List<PosItem> Items { get; set; }
    }

    [XmlRoot(ElementName = "Item")]
    public class PosItem
    {
        [XmlElement(ElementName = "PrintFormat")]
        public PrintFormat PrintFormat { get; set; }
        [XmlElement(ElementName = "Options")]
        public PosOptions Options { get; set; }
        [XmlElement(ElementName = "Sides")]
        public PosSides Sides { get; set; }
        [XmlAttribute(AttributeName = "key")]
        public string Key { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "kitchenName")]
        public string KitchenName { get; set; }
        [XmlAttribute(AttributeName = "orderItemKey")]
        public string OrderItemKey { get; set; }
        [XmlAttribute(AttributeName = "servingCourseDBKey")]
        public string ServingCoursePosKey { get; set; }
        [XmlAttribute(AttributeName = "courseDBKey")]
        public string CoursePosKey { get; set; }
        [XmlAttribute(AttributeName = "note")]
        public string Note { get; set; }
        [XmlAttribute(AttributeName = "canceled")]
        public string Canceled { get; set; }
        [XmlAttribute(AttributeName = "quantity")]
        public string Quantity { get; set; }
    }
}