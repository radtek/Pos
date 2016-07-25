using System;
using Chefmate.Communication.EventArguments;
using Chefmate.Core.Model;
using Chefmate.Infrastructure.Controller;
using Chefmate.Logger;
using ChefMate.Communication.Receiver;

namespace Chefmate.Infrastructure.Builder
{
    public class Builder
    {
        public delegate void DatabaseOrderReceived(DbOrderReceivedEventArgs eventArgs);
        public event DatabaseOrderReceived DatabaseOrderReceivedEvent;
        private static Builder _instance = null;
        private static readonly object SyncRoot = new Object();
        StreamConverterChefmate _chefmateStreamConverter;
        private Builder()
        {
            InitBuilder();
        }
        public static Builder Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (SyncRoot)
                    {
                        if (_instance == null)
                            _instance = new Builder();
                    }
                }

                return _instance;
            }
        }
        public bool Stopped { get { return _chefmateStreamConverter.Stopped; } }
        public void Start()
        {
            _chefmateStreamConverter.Start();
        }
        public void Stop()
        {
            _chefmateStreamConverter.Stop();
        }
        public void BuildOrderFromDatabase(DbOrderReceivedEventArgs eventArgs)
        {
            try
            {
                switch (eventArgs.MessageType)
                {
                    case DbMessageType.Order:
                        BuildOrderFromOrderKey(eventArgs.Key);
                        break;
                    case DbMessageType.Group:
                        BuildOrderFromOrderGroupKey(eventArgs.Key);
                        break;
                    case DbMessageType.Item:
                        BuildOrderFromOrderItemKey(eventArgs.Key);
                        break;
                    default:
                        break;
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogEvent("BuildOrderFromDatabase", ex.Message);
            }

        }
        private void InitBuilder()
        {
            _chefmateStreamConverter = new StreamConverterChefmate();
            _chefmateStreamConverter.DbOrderReceived += DatabaseOrderReceivedHandler;
        }
        private void DatabaseOrderReceivedHandler(object sender, DbOrderReceivedEventArgs e)
        {
            if (DatabaseOrderReceivedEvent != null)
                DatabaseOrderReceivedEvent(e);
        }
        private void BuildOrderFromOrderKey(int inOrderKey)
        {
            var order = DatabaseOrderBuilder.Instance.BuildOrderFromOrderKey(inOrderKey);
            ChefmateController.Instance.OrderArrived(order);
        }
        private void BuildOrderFromOrderGroupKey(int inOrderGroupKey)
        {
            Order order = DatabaseOrderBuilder.Instance.BuildOrderFromOrderGroupKey(inOrderGroupKey);
            ChefmateController.Instance.OrderArrived(order);
        }
        private void BuildOrderFromOrderItemKey(int inOrderItemKey)
        {
            Order order = DatabaseOrderBuilder.Instance.BuildOrderFromOrderItemKey(inOrderItemKey);
            ChefmateController.Instance.OrderArrived(order);
        }
    }
}
