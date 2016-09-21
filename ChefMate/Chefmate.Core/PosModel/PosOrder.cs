using System.Collections.Generic;
using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [XmlRoot(ElementName = "Order")]
    public class PosOrder
    {
        [XmlArray(ElementName = "ServingCourses")]
        public List<ServingCourse> ServingCourses { get; set; }
        [XmlArray(ElementName = "Courses")]
        public List<Course> Courses { get; set; }
        [XmlElement(ElementName = "Items")]
        public PosItems Items { get; set; }
        [XmlAttribute(AttributeName = "action")]
        public string Action { get; set; }
        [XmlAttribute(AttributeName = "type")]
        public string ActionType { get; set; }
        [XmlAttribute(AttributeName = "dbKey")]
        public string DbKey { get; set; }
        [XmlAttribute(AttributeName = "serverName")]
        public string ServerName { get; set; }
        [XmlAttribute(AttributeName = "orderNumber")]
        public string OrderNumber { get; set; }
        [XmlAttribute(AttributeName = "chitValue")]
        public string ChitValue { get; set; }
        [XmlAttribute(AttributeName = "tableTabName")]
        public string TableTabName { get; set; }
        [XmlAttribute(AttributeName = "orderType")]
        public string OrderType { get; set; }
        [XmlAttribute(AttributeName = "customerName")]
        public string CustomerName { get; set; }
        [XmlAttribute(AttributeName = "partyName")]
        public string PartyName { get; set; }
        [XmlAttribute(AttributeName = "patronCount")]
        public string PatronCount { get; set; }
        [XmlAttribute(AttributeName = "saleStartTime")]
        public string SaleStartTime { get; set; }
        [XmlAttribute(AttributeName = "saleFinishTime")]
        public string SaleFinishTime { get; set; }
        [XmlAttribute(AttributeName = "deliveryTime")]
        public string DeliveryTime { get; set; }
        [XmlAttribute(AttributeName = "sourceTableName")]
        public string SourceTableName { get; set; }
        [XmlAttribute(AttributeName = "customerPhone")]
        public string CustomerPhone { get; set; }
        [XmlAttribute(AttributeName = "customerEmail")]
        public string CustomerEmail { get; set; }
        [XmlAttribute(AttributeName = "customerAddress")]
        public string CustomerAddress{ get; set; }
        [XmlAttribute(AttributeName = "paymentStatus")]
        public string PaymentStatus { get; set; }
        
    }
}