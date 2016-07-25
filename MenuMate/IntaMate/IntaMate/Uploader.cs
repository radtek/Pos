using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Net;

namespace IntaMate {
    public static class Uploader {
        private static int BUFFERLENGTH = 2048;

        public static void UploadFile(string filename, string serverip, string ftpuser, string ftppassword) {
            FileInfo fileInf = new FileInfo(filename);
            string uri = serverip + "/" + fileInf.Name;
            FtpWebRequest reqFTP;

            // Create FtpWebRequest object from the Uri provided
            reqFTP = (FtpWebRequest) FtpWebRequest.Create(new Uri(uri));

            reqFTP.UsePassive = false;

            // Provide the WebPermission Credintials
            reqFTP.Credentials = new NetworkCredential(ftpuser, ftppassword);

            // By default KeepAlive is true, where the control connection is not closed
            // after a command is executed.
            reqFTP.KeepAlive = false;

            // Specify the command to be executed.
            reqFTP.Method = WebRequestMethods.Ftp.AppendFile;

            // Specify the data transfer type.
            reqFTP.UseBinary = true;

            // Notify the server about the size of the uploaded file
            reqFTP.ContentLength = fileInf.Length;
            
            byte[] buff = new byte[BUFFERLENGTH];
            int contentLen;

            // Opens a file stream (System.IO.FileStream) to read the file to be uploaded
            using (FileStream fs = fileInf.OpenRead()) {

                // Stream to which the file to be upload is written
                using (Stream strm = reqFTP.GetRequestStream()) {

                    // Read from the file stream 2kb at a time
                    contentLen = fs.Read(buff, 0, BUFFERLENGTH);

                    // Till Stream content ends
                    while (contentLen != 0) {
                        // Write Content from the file stream to the FTP Upload Stream
                        strm.Write(buff, 0, contentLen);
                        contentLen = fs.Read(buff, 0, BUFFERLENGTH);
                    }
                }
            }
        }
    }
}
