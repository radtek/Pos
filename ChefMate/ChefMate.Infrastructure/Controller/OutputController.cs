using System;
using Chefmate.Core.Enums;
using Chefmate.Core.Model;
using Chefmate.Database.DbModels;
using Chefmate.Infrastructure.Output;
using Chefmate.Logger;
using ChefMate.Communication.Tcp;

namespace Chefmate.Infrastructure.Controller
{
    public class OutputController
    {
        private static OutputController _instance;
        private static volatile object _syncRoot = new Object();
        private PrinterFormatter _formatter;
        private IPrinter _printer;
        private Core.Model.Terminal _outputTerminal;
        private OutputController()
        {
            Initialize();
        }
        public static OutputController Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_syncRoot)
                    {
                        if (_instance == null)
                            _instance = new OutputController();
                    }
                }
                return _instance;
            }
        }

        public void Initialize()
        {
            _printer = PrinterFactory.Instance.BuildPrinter();
            _formatter = new PrinterFormatter();
            if (ChefmateController.Instance.CurrentSettings.OutputTerminal > 0)
                _outputTerminal = DbTerminal.GetTerminal(ChefmateController.Instance.CurrentSettings.OutputTerminal);
        }
        public void OutputOrder(Order inOrder)
        {
            try
            {
                if (ChefmateController.Instance.CurrentSettings.OutputTerminal > 0)
                {
                    OutputOrderToTerminal(inOrder);
                }
                else if (ChefmateController.Instance.CurrentSettings.OutputTerminal < 0)
                {
                    OutputOrderToPrinter(inOrder);
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("OutputController.OutputOrder", ex.Message);
            }

        }
        public void OutputGroup(Group inGroup)
        {
            try
            {
                if (ChefmateController.Instance.CurrentSettings.OutputTerminal > 0)
                {
                    OutputGroupToTerminal(inGroup);
                }
                else if (ChefmateController.Instance.CurrentSettings.OutputTerminal < 0)
                {
                    OutputGroupToPrinter(inGroup);
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("OutputController.OutputGroup", ex.Message);
            }
        }
        public void OutputItem(Item inItem)
        {
            try
            {
                if (ChefmateController.Instance.CurrentSettings.OutputTerminal > 0)
                {
                    OutputItemToTerminal(inItem);
                }
                else if (ChefmateController.Instance.CurrentSettings.OutputTerminal < 0)
                {
                    OutputItemToPrinter(inItem);
                }
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("OutputController.OutputItem", ex.Message);
            }
        }

        #region TerminalOutput
        private void OutputOrderToTerminal(Order inOrder)
        {
            UpdateOrderTerminalKey(inOrder);
            SendOrderKey(inOrder.OrderKey);
        }
        private void OutputGroupToTerminal(Group inGroup)
        {
            UpdateGroupTerminalKey(inGroup);
            SendGroupKey(inGroup.OrderGroupKey);
        }
        private void OutputItemToTerminal(Item inItem)
        {
            UpdateItemTerminalKey(inItem);
            SendItemKey(inItem.OrderItemKey);
        }
        private void UpdateOrderTerminalKey(Order inOrder)
        {
            foreach (var item in inOrder.Items)
            {
                UpdateItemTerminalKey(item);
            }
        }
        private void UpdateGroupTerminalKey(Group inGroup)
        {
            foreach (var item in inGroup.Items)
            {
                UpdateItemTerminalKey(item);
            }
        }
        private void UpdateItemTerminalKey(Item inItem)
        {
            // set item status to normal
            DbOrderItem.UpdateOrderItemStatus(inItem.OrderItemKey, OrderStatus.Normal);

            // update terminalKey for the item
            DbOrderItem.UpdateOrderItemTerminalKey(inItem.OrderItemKey, _outputTerminal.TerminalId);
        }
        private void SendOrderKey(int orderKey)
        {
            TcpStreamMessage message = new TcpStreamMessage();
            message.SetIpAddressFromString(_outputTerminal.TerminalIpAddress);
            message.AddData(orderKey);
            message.MessageType = TcpStreamMessageType.Order;
            SendTcpMessage(message);
        }
        private void SendGroupKey(int orderGroupKey)
        {
            TcpStreamMessage message = new TcpStreamMessage();
            message.SetIpAddressFromString(_outputTerminal.TerminalIpAddress);
            message.AddData(orderGroupKey);
            message.MessageType = TcpStreamMessageType.Group;
            SendTcpMessage(message);
        }
        private void SendItemKey(int orderItemKey)
        {
            TcpStreamMessage message = new TcpStreamMessage();
            message.SetIpAddressFromString(_outputTerminal.TerminalIpAddress);
            message.AddData(orderItemKey);
            message.MessageType = TcpStreamMessageType.Item;
            SendTcpMessage(message);
        }
        private void SendTcpMessage(TcpStreamMessage message)
        {
            TcpStreamSender tcpStreamSender = new TcpStreamSender();
            tcpStreamSender.SendMessage(message);
        }

        #endregion

        #region PrinterlOutput
        private void OutputOrderToPrinter(Order inOrder)
        {
            DocketLayout docketLayout = _formatter.BuildDocketLayout(inOrder);
            _printer.Print(docketLayout);
        }
        private void OutputGroupToPrinter(Group inGroup)
        {
            DocketLayout docketLayout = _formatter.BuildDocketLayout(inGroup);
            _printer.Print(docketLayout);
        }
        private void OutputItemToPrinter(Item inItem)
        {
            DocketLayout docketLayout = _formatter.BuildDocketLayout(inItem);
            _printer.Print(docketLayout);
        }
        #endregion

    }
}