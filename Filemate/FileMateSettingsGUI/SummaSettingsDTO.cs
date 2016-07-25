
namespace FileMateSettingsGUI {
    public class SummaSettingsDTO {

        private string _exportPath;

        public string ExportPath {
            get { return _exportPath; }
            set { _exportPath = value; }
        }
        private string _port;

        public string Port {
            get { return _port; }
            set { _port = value; }
        }

        private bool _enabled;

        public bool Enabled
        {
            get { return _enabled; }
            set { _enabled = value; }
        }

        public SummaSettingsDTO() {

        }
    }
}
