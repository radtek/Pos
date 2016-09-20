using System.Collections.Generic;
using Chefmate.Database.Parser;

namespace ChefMate.Database.Parser
{
    public class ParserFactory
    {
        private Dictionary<string, VersionParser> _versionParsers;
        public ParserFactory()
        {
            InitializeParsers();

        }

        private void InitializeParsers()
        {
            _versionParsers = new Dictionary<string, VersionParser>();
            _versionParsers.Add("1.0", new ChefmateParserVersion1_0());
            _versionParsers.Add("1.1", new ChefmateParserVersion1_1());
            _versionParsers.Add("1.2", new ChefmateParserVersion1_2());
        }

        public VersionParser GetVersionParser(string version)
        {
            if (_versionParsers.ContainsKey(version))
                return _versionParsers[version];
            return null;
        }
    }
}