using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MenumateUtils.Encrypt;

namespace Loyalty_mate_encrypter
{
    public partial class Encrypter : Form
    {
        private string _credentialsFileFilter = "Credentials File (*.info)|*.info";
        private string _allFilesFilter = "All Files(*.*)|*.*";

        public Encrypter()
        {
            InitializeComponent();
        }

        private void btnDecryptFile_Click(object sender, EventArgs e)
        {
            if (validateDecryptFileInputs())
            {
                try
                {
                    MMEncrypt.Instance.Decrypt(
                        lblInputFile_Decrypt.Text,
                        lblOutputFile_Decrypt.Text,
                        EncryptAttributes.Instance.Password);

                    showMessage("File decrypted successfully");
                }
                catch (Exception ex)
                {
                    showExceptionMessage(ex);
                }
            }
            else
                showMessage("No file/ Invalid file selected for decryption");
        }        

        private void btnDecryptText_Click(object sender, EventArgs e)
        {
            if (validateDecryptTextInputs())
            {
                try
                {
                    string decryptedText = "";
                    MMEncrypt.Instance.Decrypt(txtInputText_Decrypt.Text, EncryptAttributes.Instance.Password, ref decryptedText);
                    txtOutputText_Decrypt.Text = decryptedText;
                }
                catch (Exception ex)
                {
                    showExceptionMessage(ex);
                }
            }
            else
                showMessage("Please enter encrypted text to decrypt");
        }

        private void btnInputFile_Decrypt_Click(object sender, EventArgs e)
        {
            lblInputFile_Decrypt.Text = getOpenSaveFileName("open", this._credentialsFileFilter);
        }

        private void btnOutputFile_Decrypt_Click(object sender, EventArgs e)
        {
            lblOutputFile_Decrypt.Text = getOpenSaveFileName("save", this._allFilesFilter);
        }

        private void btnInputFile_Encrypt_Click(object sender, EventArgs e)
        {
            lblInputFile_Encrypt.Text = getOpenSaveFileName("open",this._allFilesFilter);
        }

        private void btnOutputFile_Encrypt_Click(object sender, EventArgs e)
        {
            lblOutputFile_Encrypt.Text = getOpenSaveFileName("save",this._credentialsFileFilter);
        }

        private void btnEncryptFile_Click(object sender, EventArgs e)
        {
            if (validateEncryptFileInputs())
            {
                try
                {
                    MMEncrypt.Instance.Encrypt(
                        lblInputFile_Encrypt.Text,
                        lblOutputFile_Encrypt.Text,
                        EncryptAttributes.Instance.Password);

                    showMessage("File encrypted successfully");
                }
                catch (Exception ex)
                {
                    showExceptionMessage(ex);
                }
            }
            else
                showMessage("No file/ Invalid file selected for encryption");
        }        

        private void btnEncryptText_Click(object sender, EventArgs e)
        {
            if (validateEncryptTextInputs())
            {
                try
                {
                    string encryptedText = "";
                    MMEncrypt.Instance.Encrypt(txtInputText_Encrypt.Text, EncryptAttributes.Instance.Password, ref encryptedText);
                    txtOutputText_Encrypt.Text = encryptedText;
                }
                catch (Exception ex)
                {
                    showExceptionMessage(ex);
                }
            }
            else
                showMessage("Please enter some text to encrypt");
        }

        private void showExceptionMessage(Exception ex)
        {
            MessageBox.Show("Cannot perform the operation. An error occurred", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //TODO: Log the exceptions here
        }

        private void showMessage(string messageText)
        {
            MessageBox.Show(messageText);
        }

        private bool validateDecryptFileInputs()
        {
            return File.Exists(lblInputFile_Decrypt.Text);
        }

        private bool validateDecryptTextInputs()
        {
            return !string.IsNullOrEmpty(txtInputText_Decrypt.Text);
        }

        private bool validateEncryptFileInputs()
        {
            return File.Exists(lblInputFile_Encrypt.Text);
        }

        private bool validateEncryptTextInputs()
        {
            return !string.IsNullOrEmpty(txtInputText_Encrypt.Text);
        }

        private string getOpenSaveFileName(string mode,string filter)
        {
            string filename="";

            if (mode == "open")
            {
                dlgOpenFile.Filter = filter;
                dlgOpenFile.ShowDialog(this);
                filename = !string.IsNullOrEmpty(dlgOpenFile.FileName) ? dlgOpenFile.FileName : "";                    
            }
            else if (mode == "save")
            {
                dlgSaveFile.Filter = filter;
                dlgSaveFile.ShowDialog(this);
                filename = !string.IsNullOrEmpty(dlgSaveFile.FileName) ? dlgSaveFile.FileName : "";
            }

            return filename;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
