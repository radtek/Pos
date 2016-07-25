using System;
using System.Runtime.Serialization;

namespace SummaCL
{
    [Serializable]
    public class SummaException : Exception {

        private string csvData;

        public SummaException() {
        }

        public SummaException(string message)
            : base(message) {
        }

        public SummaException(string message, Exception innerException)
            : base(message, innerException) {
        }

        protected SummaException(SerializationInfo info, StreamingContext context)
            : base(info, context) {
        }

        public string Data {
            get { return this.csvData; }
            set { this.csvData = value; }
        }
    }
}
