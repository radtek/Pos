using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;

namespace IntaMate {
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
