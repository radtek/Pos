using System;
using System.Linq;
using System.Net;
using System.Net.Sockets;

namespace Chefmate.Infrastructure.Terminal
{
    public class ChefmateTerminal
    {
        private static ChefmateTerminal _terminal;
        private readonly string _ipAddress;

        public int TerminalDbKey { get; private set; }
        public string Name { get; internal set; }

        private ChefmateTerminal()
        {
            //ToDo: Change Required
            //Name = Settings.Default.TerminalName;
            if (Name == "")
            {
                Name = Environment.MachineName;
                SaveTerminalNameInSettings(Name);
            }
            _ipAddress = LocalIpAddress().ToString();
        }

        public static ChefmateTerminal Instance
        {
            get
            {
                Initalize();
                return _terminal;
            }
        }

        public static void Initalize()
        {
            if (_terminal == null)
            {
                _terminal = new ChefmateTerminal();

                _terminal.CheckDatabase();
            }

        }

        private void CheckDatabase()
        {
            LoadTerminalDbKey();
            //ToDo: Change Required
            if (TerminalDbKey == -1)
            {
                //TerminalDbKey = DbTerminal.AddTerminal(Name, _ipAddress);
                SaveTerminalDbKeyInSettings(TerminalDbKey);
            }
            else
            {
                //DbTerminal.SetTerminalIpAddress(_ipAddress, TerminalDbKey);
            }
        }

        private void LoadTerminalDbKey()
        {
            //ToDo: Change Required
            //TerminalDbKey = Settings.Default.TerminalKey;

            //if (TerminalDbKey == -1 || !DbTerminal.CheckTerminalInformation(TerminalDbKey, Environment.MachineName))
            //{
            //    TerminalDbKey = DbTerminal.GetTerminalKey(Name);
            //    SaveTerminalDbKeyInSettings(TerminalDbKey);
            //}
        }

        private void SaveTerminalDbKeyInSettings(int TerminalDbKey)
        {
            //ToDo: Change Required
            // Settings.Default.TerminalKey = TerminalDbKey;
            SaveTerminalSettings();
        }

        private void SaveTerminalNameInSettings(string inTerminalName)
        {
            //ToDo: Change Required
            //Settings.Default.TerminalName = inTerminalName;
            SaveTerminalSettings();
        }

        private void SaveTerminalSettings()
        {
            //ToDo: Change Required
            //Settings.Default.Save();
        }

        private IPAddress LocalIpAddress()
        {
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            return host.AddressList.FirstOrDefault(ip => ip.AddressFamily == AddressFamily.InterNetwork);
        }
    }
}