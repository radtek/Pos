using System;
using System.Collections.Generic;

namespace ChefMate.Database.Parser
{
    public class ChefmateParser
    {
        public delegate void ParserStatusDelegate(string status);

        public event ParserStatusDelegate StatusChangeEvent;
        private readonly ParserFactory _parserFactory;
        private List<string> _versionNumbers;
        private string _dbAddress;
        private string _dbPath;

        public ChefmateParser()
        {
            InitializeVersionNumbers();
            _parserFactory = new ParserFactory();
        }
        public ChefmateParser(string dbAddress, string dbPath)
            :this()
        {
            _dbAddress = dbAddress;
            _dbPath = dbPath;
        }
        public void RunParser()
        {
            DatabaseCore.Instance.InitializeDatabae(_dbAddress, _dbPath);
            SendStatus("Starting Database upgrade.....");
            foreach (var version in _versionNumbers)
            {
                var versionParser = _parserFactory.GetVersionParser(version);
                SendStatus("Checking Version " + version + ".....");
                SendStatus("Starting upgarde to version " + version);
                versionParser.RunParser();
                SendStatus("Upgarde to version " + version + " Successfully!");
            }
            SendStatus("Successfully upgraded Database!");
        }
        private void CreateDatabase()
        {
            DatabaseCore.Instance.CreateDatabase(_dbAddress, _dbPath);
        }
        public void CreateDatabaseAndrunParser()
        {
            SendStatus("Creating database.....");
            CreateDatabase();
            RunParser();
            SendStatus("Database Created");
        }
        private void InitializeVersionNumbers()
        {
            _versionNumbers = new List<string>();
            _versionNumbers.Add("1.0");
        }

        private void SendStatus(string inStatus)
        {
            if (StatusChangeEvent != null)
                StatusChangeEvent(inStatus);
        }
    }
}