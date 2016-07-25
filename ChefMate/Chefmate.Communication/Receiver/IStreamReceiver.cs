using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using Chefmate.Communication.EventArguments;

namespace ChefMate.Communication.Receiver
{
    public delegate void StreamReceivedEventHandler(object sender, StreamReceiverEventArgs streamArgs);

    public interface IStreamReceiver
    {
        #region EVENTS

        event StreamReceivedEventHandler StreamReceived;

        #endregion

        #region PROPERTIES

        bool Stopped
        {
            get;
        }

        #endregion

        #region PUBLIC
        
        void Start();
        void Stop();

        #endregion
    }
}
