using System;
using System.Xml.Serialization;

namespace Chefmate.Core.PosModel
{
    [Serializable]
    public class PosGroup
    {
        [XmlAttribute(AttributeName = "key")]
        public string Key { get; set; }
        [XmlAttribute(AttributeName = "name")]
        public string Name { get; set; }
        [XmlAttribute(AttributeName = "kitchenName")]
        public string KitchenName { get; set; }
        [XmlAttribute(AttributeName = "displayOrder")]
        public string DisplayOrder { get; set; }
    }
}