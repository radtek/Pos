namespace Loyalty_mate_encrypter
{
    partial class Encrypter
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnInputFile_Encrypt = new System.Windows.Forms.Button();
            this.lblInputFile_Encrypt = new System.Windows.Forms.Label();
            this.btnOutputFile_Encrypt = new System.Windows.Forms.Button();
            this.lblOutputFile_Encrypt = new System.Windows.Forms.Label();
            this.btnEncryptFile = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.txtInputText_Encrypt = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtOutputText_Encrypt = new System.Windows.Forms.TextBox();
            this.btnEncryptText = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnDecryptText = new System.Windows.Forms.Button();
            this.txtOutputText_Decrypt = new System.Windows.Forms.TextBox();
            this.txtInputText_Decrypt = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnDecryptFile = new System.Windows.Forms.Button();
            this.lblOutputFile_Decrypt = new System.Windows.Forms.Label();
            this.lblInputFile_Decrypt = new System.Windows.Forms.Label();
            this.btnOutputFile_Decrypt = new System.Windows.Forms.Button();
            this.btnInputFile_Decrypt = new System.Windows.Forms.Button();
            this.dlgOpenFile = new System.Windows.Forms.OpenFileDialog();
            this.dlgSaveFile = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(673, 24);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(673, 344);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(665, 318);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Encrypt";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.panel2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(665, 318);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Decrypt";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(659, 312);
            this.panel1.TabIndex = 0;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.groupBox3);
            this.panel2.Controls.Add(this.groupBox4);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(3, 3);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(659, 312);
            this.panel2.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.btnEncryptFile);
            this.groupBox1.Controls.Add(this.lblOutputFile_Encrypt);
            this.groupBox1.Controls.Add(this.lblInputFile_Encrypt);
            this.groupBox1.Controls.Add(this.btnOutputFile_Encrypt);
            this.groupBox1.Controls.Add(this.btnInputFile_Encrypt);
            this.groupBox1.Location = new System.Drawing.Point(6, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(648, 142);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Files";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.btnEncryptText);
            this.groupBox2.Controls.Add(this.txtOutputText_Encrypt);
            this.groupBox2.Controls.Add(this.txtInputText_Encrypt);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(6, 152);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(648, 157);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Text";
            // 
            // btnInputFile_Encrypt
            // 
            this.btnInputFile_Encrypt.Location = new System.Drawing.Point(7, 20);
            this.btnInputFile_Encrypt.Name = "btnInputFile_Encrypt";
            this.btnInputFile_Encrypt.Size = new System.Drawing.Size(102, 23);
            this.btnInputFile_Encrypt.TabIndex = 0;
            this.btnInputFile_Encrypt.Text = "Select Input File";
            this.btnInputFile_Encrypt.UseVisualStyleBackColor = true;
            this.btnInputFile_Encrypt.Click += new System.EventHandler(this.btnInputFile_Encrypt_Click);
            // 
            // lblInputFile_Encrypt
            // 
            this.lblInputFile_Encrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblInputFile_Encrypt.AutoSize = true;
            this.lblInputFile_Encrypt.Location = new System.Drawing.Point(115, 25);
            this.lblInputFile_Encrypt.Name = "lblInputFile_Encrypt";
            this.lblInputFile_Encrypt.Size = new System.Drawing.Size(0, 13);
            this.lblInputFile_Encrypt.TabIndex = 1;
            // 
            // btnOutputFile_Encrypt
            // 
            this.btnOutputFile_Encrypt.Location = new System.Drawing.Point(7, 49);
            this.btnOutputFile_Encrypt.Name = "btnOutputFile_Encrypt";
            this.btnOutputFile_Encrypt.Size = new System.Drawing.Size(102, 23);
            this.btnOutputFile_Encrypt.TabIndex = 0;
            this.btnOutputFile_Encrypt.Text = "Select Output File";
            this.btnOutputFile_Encrypt.UseVisualStyleBackColor = true;
            this.btnOutputFile_Encrypt.Click += new System.EventHandler(this.btnOutputFile_Encrypt_Click);
            // 
            // lblOutputFile_Encrypt
            // 
            this.lblOutputFile_Encrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblOutputFile_Encrypt.AutoSize = true;
            this.lblOutputFile_Encrypt.Location = new System.Drawing.Point(115, 54);
            this.lblOutputFile_Encrypt.Name = "lblOutputFile_Encrypt";
            this.lblOutputFile_Encrypt.Size = new System.Drawing.Size(0, 13);
            this.lblOutputFile_Encrypt.TabIndex = 1;
            // 
            // btnEncryptFile
            // 
            this.btnEncryptFile.Location = new System.Drawing.Point(7, 113);
            this.btnEncryptFile.Name = "btnEncryptFile";
            this.btnEncryptFile.Size = new System.Drawing.Size(75, 23);
            this.btnEncryptFile.TabIndex = 2;
            this.btnEncryptFile.Text = "Encrypt File";
            this.btnEncryptFile.UseVisualStyleBackColor = true;
            this.btnEncryptFile.Click += new System.EventHandler(this.btnEncryptFile_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Input text";
            // 
            // txtInputText_Encrypt
            // 
            this.txtInputText_Encrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtInputText_Encrypt.Location = new System.Drawing.Point(69, 17);
            this.txtInputText_Encrypt.Multiline = true;
            this.txtInputText_Encrypt.Name = "txtInputText_Encrypt";
            this.txtInputText_Encrypt.Size = new System.Drawing.Size(570, 45);
            this.txtInputText_Encrypt.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 86);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Output text";
            // 
            // txtOutputText_Encrypt
            // 
            this.txtOutputText_Encrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutputText_Encrypt.Location = new System.Drawing.Point(69, 68);
            this.txtOutputText_Encrypt.Multiline = true;
            this.txtOutputText_Encrypt.Name = "txtOutputText_Encrypt";
            this.txtOutputText_Encrypt.Size = new System.Drawing.Size(570, 45);
            this.txtOutputText_Encrypt.TabIndex = 2;
            // 
            // btnEncryptText
            // 
            this.btnEncryptText.Location = new System.Drawing.Point(7, 128);
            this.btnEncryptText.Name = "btnEncryptText";
            this.btnEncryptText.Size = new System.Drawing.Size(75, 23);
            this.btnEncryptText.TabIndex = 3;
            this.btnEncryptText.Text = "Encrypt Text";
            this.btnEncryptText.UseVisualStyleBackColor = true;
            this.btnEncryptText.Click += new System.EventHandler(this.btnEncryptText_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.btnDecryptText);
            this.groupBox3.Controls.Add(this.txtOutputText_Decrypt);
            this.groupBox3.Controls.Add(this.txtInputText_Decrypt);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(5, 152);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(648, 157);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Text";
            // 
            // btnDecryptText
            // 
            this.btnDecryptText.Location = new System.Drawing.Point(7, 128);
            this.btnDecryptText.Name = "btnDecryptText";
            this.btnDecryptText.Size = new System.Drawing.Size(75, 23);
            this.btnDecryptText.TabIndex = 3;
            this.btnDecryptText.Text = "Decrypt Text";
            this.btnDecryptText.UseVisualStyleBackColor = true;
            this.btnDecryptText.Click += new System.EventHandler(this.btnDecryptText_Click);
            // 
            // txtOutputText_Decrypt
            // 
            this.txtOutputText_Decrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutputText_Decrypt.Location = new System.Drawing.Point(69, 68);
            this.txtOutputText_Decrypt.Multiline = true;
            this.txtOutputText_Decrypt.Name = "txtOutputText_Decrypt";
            this.txtOutputText_Decrypt.Size = new System.Drawing.Size(570, 45);
            this.txtOutputText_Decrypt.TabIndex = 2;
            // 
            // txtInputText_Decrypt
            // 
            this.txtInputText_Decrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.txtInputText_Decrypt.Location = new System.Drawing.Point(69, 17);
            this.txtInputText_Decrypt.Multiline = true;
            this.txtInputText_Decrypt.Name = "txtInputText_Decrypt";
            this.txtInputText_Decrypt.Size = new System.Drawing.Size(570, 45);
            this.txtInputText_Decrypt.TabIndex = 1;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 86);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Output text";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Input text";
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.btnDecryptFile);
            this.groupBox4.Controls.Add(this.lblOutputFile_Decrypt);
            this.groupBox4.Controls.Add(this.lblInputFile_Decrypt);
            this.groupBox4.Controls.Add(this.btnOutputFile_Decrypt);
            this.groupBox4.Controls.Add(this.btnInputFile_Decrypt);
            this.groupBox4.Location = new System.Drawing.Point(5, 4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(648, 142);
            this.groupBox4.TabIndex = 1;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Files";
            // 
            // btnDecryptFile
            // 
            this.btnDecryptFile.Location = new System.Drawing.Point(7, 113);
            this.btnDecryptFile.Name = "btnDecryptFile";
            this.btnDecryptFile.Size = new System.Drawing.Size(75, 23);
            this.btnDecryptFile.TabIndex = 2;
            this.btnDecryptFile.Text = "Decrypt File";
            this.btnDecryptFile.UseVisualStyleBackColor = true;
            this.btnDecryptFile.Click += new System.EventHandler(this.btnDecryptFile_Click);
            // 
            // lblOutputFile_Decrypt
            // 
            this.lblOutputFile_Decrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblOutputFile_Decrypt.AutoSize = true;
            this.lblOutputFile_Decrypt.Location = new System.Drawing.Point(115, 54);
            this.lblOutputFile_Decrypt.Name = "lblOutputFile_Decrypt";
            this.lblOutputFile_Decrypt.Size = new System.Drawing.Size(0, 13);
            this.lblOutputFile_Decrypt.TabIndex = 1;
            // 
            // lblInputFile_Decrypt
            // 
            this.lblInputFile_Decrypt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lblInputFile_Decrypt.AutoSize = true;
            this.lblInputFile_Decrypt.Location = new System.Drawing.Point(115, 25);
            this.lblInputFile_Decrypt.Name = "lblInputFile_Decrypt";
            this.lblInputFile_Decrypt.Size = new System.Drawing.Size(0, 13);
            this.lblInputFile_Decrypt.TabIndex = 1;
            // 
            // btnOutputFile_Decrypt
            // 
            this.btnOutputFile_Decrypt.Location = new System.Drawing.Point(7, 49);
            this.btnOutputFile_Decrypt.Name = "btnOutputFile_Decrypt";
            this.btnOutputFile_Decrypt.Size = new System.Drawing.Size(102, 23);
            this.btnOutputFile_Decrypt.TabIndex = 0;
            this.btnOutputFile_Decrypt.Text = "Select Output File";
            this.btnOutputFile_Decrypt.UseVisualStyleBackColor = true;
            this.btnOutputFile_Decrypt.Click += new System.EventHandler(this.btnOutputFile_Decrypt_Click);
            // 
            // btnInputFile_Decrypt
            // 
            this.btnInputFile_Decrypt.Location = new System.Drawing.Point(7, 20);
            this.btnInputFile_Decrypt.Name = "btnInputFile_Decrypt";
            this.btnInputFile_Decrypt.Size = new System.Drawing.Size(102, 23);
            this.btnInputFile_Decrypt.TabIndex = 0;
            this.btnInputFile_Decrypt.Text = "Select Input File";
            this.btnInputFile_Decrypt.UseVisualStyleBackColor = true;
            this.btnInputFile_Decrypt.Click += new System.EventHandler(this.btnInputFile_Decrypt_Click);
            // 
            // Encrypter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(673, 383);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.MaximizeBox = false;
            this.Name = "Encrypter";
            this.Text = "Encrypter";
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnEncryptText;
        private System.Windows.Forms.TextBox txtOutputText_Encrypt;
        private System.Windows.Forms.TextBox txtInputText_Encrypt;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEncryptFile;
        private System.Windows.Forms.Label lblOutputFile_Encrypt;
        private System.Windows.Forms.Label lblInputFile_Encrypt;
        private System.Windows.Forms.Button btnOutputFile_Encrypt;
        private System.Windows.Forms.Button btnInputFile_Encrypt;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnDecryptText;
        private System.Windows.Forms.TextBox txtOutputText_Decrypt;
        private System.Windows.Forms.TextBox txtInputText_Decrypt;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btnDecryptFile;
        private System.Windows.Forms.Label lblOutputFile_Decrypt;
        private System.Windows.Forms.Label lblInputFile_Decrypt;
        private System.Windows.Forms.Button btnOutputFile_Decrypt;
        private System.Windows.Forms.Button btnInputFile_Decrypt;
        private System.Windows.Forms.OpenFileDialog dlgOpenFile;
        private System.Windows.Forms.SaveFileDialog dlgSaveFile;
    }
}