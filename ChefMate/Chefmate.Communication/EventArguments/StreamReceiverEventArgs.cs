using System;
using System.IO;

namespace Chefmate.Communication.EventArguments
{
    public class StreamReceiverEventArgs : EventArgs
    {
        public StreamReceiverEventArgs(MemoryStream stream) : base()
        {
            _streamReceived = stream;
            _streamResponse = @"";
        }

        #region PROPERTIES

        public MemoryStream Stream
        {
            get { return _streamReceived; }
        }

        public string Response
        {
            get { return _streamResponse; }
            set { _streamResponse = value; }
        }

        #endregion

        #region PRIVATE

        MemoryStream _streamReceived;
        string _streamResponse;

        #endregion
    }
}
