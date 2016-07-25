using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace IntaMate {
    public abstract class CsvMulti {

        protected static string FULLFILEPATH;

        

        protected static string SEPERATOR = ",";
        
        protected int[] fieldLengths;
        protected bool[] shouldQuoteField;

        protected List<List<string>> csvData;
        protected List<StringBuilder> csvLineData;

        protected bool AppendToFile = false;

        // must implememnt this
        public abstract void SetupData(XmlDocument doc);

        public string FullFilePath {
            get {
                return FULLFILEPATH;
            }
        }

        public void WriteDataToCSV() {
            // TODO investigate threading issues, this method must be thread safe!
            // exceptions will bubble up and be caught by Summa.cs

            // internal sanity check
            if (fieldLengths.Length != shouldQuoteField.Length) {
                throw new Exception("Internal error: fieldsLengths and sholdQuoteField arrays should be same length!");
            }

            if (csvData.Count > 0) {
                if (csvData[0] != null) {
                    if (csvData[0].Count != fieldLengths.Length) {
                        throw new Exception("Internal error: CSV data item count and fieldLengths data structures should be same length!");
                    }
                }
            }
        

            try {

                BuildCSVLines();

                FileMode createMode;
                if (AppendToFile) {
                    createMode = FileMode.Append;
                }
                else {
                    createMode = FileMode.Create;
                }

                using (FileStream csvFile = File.Open(FULLFILEPATH, createMode, FileAccess.Write, FileShare.None)) {
                    ASCIIEncoding asciiEncoder = new ASCIIEncoding();

                    foreach (StringBuilder b in csvLineData) {
                        byte[] bytesToWrite = asciiEncoder.GetBytes(b.ToString());
                        csvFile.Write(bytesToWrite, 0, bytesToWrite.Length);
                    }

                    csvFile.Flush();
                    csvFile.Close();
                }
            }
            catch (IOException ioe) {
                string message = "Unable to write to " + FULLFILEPATH;
                Summa.logger.ErrorException(message, ioe);
                SummaException se = new SummaException(message, ioe);

                StringBuilder allCsvData = new StringBuilder();
                foreach (StringBuilder sb in csvLineData) {
                    allCsvData.Append(sb);
                }
                se.Data = allCsvData.ToString();

                throw se;
            }
        }

        protected void BuildCSVLines() {
            for (int i = 0; i < csvData.Count; i++) {
                for (int j = 0; j < csvData[i].Count; j++) {

                    StringBuilder line = new StringBuilder();

                    if (shouldQuoteField[j]) {
                        line.Append(@"""");
                    }

                    if (csvData[i][j].Length > fieldLengths[j]) {
                        line.Append(csvData[i][j].Substring(0, fieldLengths[j]));
                        Summa.logger.Debug("Field #" + j + " on Line #" + i +  " was truncated in " + FULLFILEPATH);
                    }
                    else {
                        line.Append(csvData[i][j]);
                    }

                    if (shouldQuoteField[j]) {
                        line.Append(@"""");
                    }

                    csvLineData[i].Append(line.ToString());

                    if (j < (csvData[i].Count - 1)) {
                        AppendSeperator(i);
                    }
                    else {
                        AppendEOL(i);
                    }
                }
            }
        }

        private void AppendSeperator(int i) {
            this.csvLineData[i].Append(SEPERATOR);
        }

        private void AppendEOL(int i) {
            this.csvLineData[i].Append("\n");
        }
    }
}
