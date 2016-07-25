using System.Runtime.Serialization;

namespace Menumate.WinServices.Implementation.Contracts
{
    [DataContract]
    public class OperationResult
    {
        public OperationResult()
        {
            Success = false;

            ObjectAffected = 0;
            Message = string.Empty;
            LongMessage = string.Empty;
        }

        [DataMember]
        public bool Success;

        /// <summary>
        /// The unique ID of the object affected (can be zero).
        /// </summary>
        [DataMember]
        public int ObjectAffected;

        [DataMember]
        public string Message;

        [DataMember]
        public string LongMessage;
    }
}
