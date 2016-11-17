using System.Runtime.Serialization;

namespace MenumateServices.DTO.SmartLink
{
    [DataContract]
    public class DTO_SmartLinkTransactionResponse 
    {
        [DataMember]
        public bool Successful { get; set; }
        [DataMember]
        public string ErrorText { get; set; }
        [DataMember]
        public string TransactionResult { get; set; }
        [DataMember]
        public string ResultText { get; set; }
        [DataMember]
        public string Result { get; set; }
        [DataMember]
        public string AcquirerRef { get; set; }
        [DataMember]
        public bool TimeOut { get; set; }
    }
}