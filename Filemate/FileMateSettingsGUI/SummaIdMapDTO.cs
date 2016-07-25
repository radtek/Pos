using System;
using System.Collections.Generic;

namespace FileMateSettingsGUI {
    public class SummaIdMapDTO {

        private Dictionary<int, int> _idMap;

        public Dictionary<int, int> IdMap {
            get { return _idMap; }
        }

        public SummaIdMapDTO() {
            _idMap = new Dictionary<int, int>();
        }

        public void AddMapEntry(int siteid, int summaid) {
            try {
                _idMap.Add(siteid, summaid);
            }
            catch (ArgumentException ae) {
                new ErrorHandler(ae.ToString()).ShowDialog();
            }
        }

        public void RemoveMapEntry(int siteid) {

            try {
                _idMap.Remove(siteid);
            }
            catch (Exception e) {
                new ErrorHandler(e.ToString()).ShowDialog();
            }
        }
    }
}
