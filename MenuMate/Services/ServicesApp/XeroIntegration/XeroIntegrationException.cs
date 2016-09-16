using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;

namespace AccountingIntegration
{
    [Serializable]
    public class XeroIntegrationException : Exception
    {

        private string _excData;

        public XeroIntegrationException()
        {
        }

        public XeroIntegrationException(string message)
            : base(message)
        {
        }

        public XeroIntegrationException(string message, Exception innerException)
            : base(message, innerException)
        {
        }

        protected XeroIntegrationException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }

        public string excData
        {
            get { return this._excData; }
            set { this._excData = value; }
        }
    }
}
