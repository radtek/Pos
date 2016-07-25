using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace SummaCL
{
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

			#region internal sanity check
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

			#endregion

			try {

				BuildCSVLines();

				FileMode createMode;
				if (AppendToFile) {
					createMode = FileMode.Append;
				}
				else {
					createMode = FileMode.Create;
				}

				// only write file if there is data
				if (csvLineData.Count > 0) {
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
				else 
                {
                    Summa.SingletonLoggerStatic.LogWarning(string.Format("No data to write to file ({0}), skipping.", FULLFILEPATH));

					//Summa.LOGGER.Debug(string.Format("No data to write to file ({0}), skipping.", FULLFILEPATH));
				}
			}
			catch (IOException ioe) {
				string message = "Unable to write to " + FULLFILEPATH;

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

					if (csvData[i][j].Length > fieldLengths[j]) 
                    {
						line.Append(csvData[i][j].Substring(0, fieldLengths[j]));

                        Summa.SingletonLoggerStatic.Log("Field #" + j + " on Line #" + i + " was truncated in " + FULLFILEPATH);
						//Summa.LOGGER.Debug("Field #" + j + " on Line #" + i +  " was truncated in " + FULLFILEPATH);
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
