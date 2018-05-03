using System;
using System.Runtime.Serialization;

namespace AdyenIntegration.Domain
{
    [DataContract]
    public class OutputText
    {
        [DataMember]
        public string Text { get; set; }
        [DataMember]
        public int CharacterSet { get; set; }
        [DataMember]
        public string Font { get; set; }
        [DataMember]
        public int StartRow { get; set; }
        [DataMember]
        public int StartColumn { get; set; }
        [DataMember]
        public string Color { get; set; }
        [DataMember]
        public string CharacterWidth { get; set; }
        [DataMember]
        public string CharacterHeight { get; set; }
        [DataMember]
        public string CharacterStyle { get; set; }
        [DataMember]
        public string Alignment { get; set; }
        [DataMember]
        public string EndOfLineFlag { get; set; }
    }
}
