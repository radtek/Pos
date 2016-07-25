using Chefmate.Communication.EventArguments;

namespace ChefMate.Communication.Receiver
{
    public abstract class StreamConverter
    {
        protected IStreamReceiver StreamReceiver;
        protected abstract void OnDataReceived(object sender, StreamReceiverEventArgs streamArgs);
        protected abstract void OpenReceiver();
        public bool Stopped
        {
            get { return StreamReceiver.Stopped; }
        }
        public StreamConverter()
        {
            OpenReceiver();
        }
        ~StreamConverter()
        {
            if (!StreamReceiver.Stopped)
            {
                CloseReceiver();
            }
        }
        public void Start()
        {
            StreamReceiver.StreamReceived += OnDataReceived;
            StreamReceiver.Start();
        }
        public void Stop()
        {
            StreamReceiver.Stop();
            StreamReceiver.StreamReceived -= OnDataReceived;
        }
        private void CloseReceiver()
        {
            this.Stop();
        }
    }
}
