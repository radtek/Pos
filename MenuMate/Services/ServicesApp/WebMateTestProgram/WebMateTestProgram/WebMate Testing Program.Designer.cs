namespace WebMateTestProgram
{
    partial class WebMateTestingProgram
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
            this.SendButton = new System.Windows.Forms.Button();
            this.StoreNameTextBox = new System.Windows.Forms.TextBox();
            this.StoreNameLabel = new System.Windows.Forms.Label();
            this.RespondCheckBox = new System.Windows.Forms.CheckBox();
            this.PickupTimeDateTimePicker = new System.Windows.Forms.DateTimePicker();
            this.PickupTimeLabel = new System.Windows.Forms.Label();
            this.CoversLabel = new System.Windows.Forms.Label();
            this.CoversNumeric = new System.Windows.Forms.NumericUpDown();
            this.CustomerNameLabel = new System.Windows.Forms.Label();
            this.CustomerNameTextBox = new System.Windows.Forms.TextBox();
            this.MemberNumberLabel = new System.Windows.Forms.Label();
            this.MemberNumberTextBox = new System.Windows.Forms.TextBox();
            this.PaymentRequiredCheckBox = new System.Windows.Forms.CheckBox();
            this.ItemsButton = new System.Windows.Forms.Button();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.CommentsTextBox = new System.Windows.Forms.TextBox();
            this.CommentsLabel = new System.Windows.Forms.Label();
            this.RecipientTextBox = new System.Windows.Forms.TextBox();
            this.RecipientLabel = new System.Windows.Forms.Label();
            this.StreetNumberTextBox = new System.Windows.Forms.TextBox();
            this.StreetNameTextBox = new System.Windows.Forms.TextBox();
            this.SuburbTextBox = new System.Windows.Forms.TextBox();
            this.CityTextBox = new System.Windows.Forms.TextBox();
            this.CountryTextBox = new System.Windows.Forms.TextBox();
            this.PhoneNumberTextBox = new System.Windows.Forms.TextBox();
            this.StreetNumberLabel = new System.Windows.Forms.Label();
            this.StreetNameLabel = new System.Windows.Forms.Label();
            this.SuburbLabel = new System.Windows.Forms.Label();
            this.CityLabel = new System.Windows.Forms.Label();
            this.CountryLabel = new System.Windows.Forms.Label();
            this.PhoneNumberLabel = new System.Windows.Forms.Label();
            this.SiteNameLabel = new System.Windows.Forms.Label();
            this.SiteNameBox = new System.Windows.Forms.TextBox();
            this.SiteIDLabel = new System.Windows.Forms.Label();
            this.SiteIDBox = new System.Windows.Forms.TextBox();
            this.CheckOrderStatusButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.CoversNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // SendButton
            // 
            this.SendButton.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SendButton.Location = new System.Drawing.Point(368, 621);
            this.SendButton.Name = "SendButton";
            this.SendButton.Size = new System.Drawing.Size(122, 44);
            this.SendButton.TabIndex = 10;
            this.SendButton.Text = "Send Order";
            this.SendButton.UseVisualStyleBackColor = true;
            this.SendButton.Click += new System.EventHandler(this.SendButton_Click);
            // 
            // StoreNameTextBox
            // 
            this.StoreNameTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StoreNameTextBox.Location = new System.Drawing.Point(170, 87);
            this.StoreNameTextBox.Name = "StoreNameTextBox";
            this.StoreNameTextBox.Size = new System.Drawing.Size(320, 30);
            this.StoreNameTextBox.TabIndex = 1;
            this.StoreNameTextBox.Text = "Test Store";
            // 
            // StoreNameLabel
            // 
            this.StoreNameLabel.AutoSize = true;
            this.StoreNameLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StoreNameLabel.Location = new System.Drawing.Point(11, 90);
            this.StoreNameLabel.Name = "StoreNameLabel";
            this.StoreNameLabel.Size = new System.Drawing.Size(109, 23);
            this.StoreNameLabel.TabIndex = 2;
            this.StoreNameLabel.Text = "Store Name";
            // 
            // RespondCheckBox
            // 
            this.RespondCheckBox.AutoSize = true;
            this.RespondCheckBox.Checked = true;
            this.RespondCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.RespondCheckBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RespondCheckBox.Location = new System.Drawing.Point(11, 555);
            this.RespondCheckBox.Name = "RespondCheckBox";
            this.RespondCheckBox.Size = new System.Drawing.Size(102, 27);
            this.RespondCheckBox.TabIndex = 7;
            this.RespondCheckBox.Text = "Respond";
            this.RespondCheckBox.UseVisualStyleBackColor = true;
            // 
            // PickupTimeDateTimePicker
            // 
            this.PickupTimeDateTimePicker.CustomFormat = "yyyy-MM-dd hh:mm:ss";
            this.PickupTimeDateTimePicker.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PickupTimeDateTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.PickupTimeDateTimePicker.Location = new System.Drawing.Point(170, 123);
            this.PickupTimeDateTimePicker.Name = "PickupTimeDateTimePicker";
            this.PickupTimeDateTimePicker.Size = new System.Drawing.Size(320, 30);
            this.PickupTimeDateTimePicker.TabIndex = 2;
            // 
            // PickupTimeLabel
            // 
            this.PickupTimeLabel.AutoSize = true;
            this.PickupTimeLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PickupTimeLabel.Location = new System.Drawing.Point(11, 126);
            this.PickupTimeLabel.Name = "PickupTimeLabel";
            this.PickupTimeLabel.Size = new System.Drawing.Size(111, 23);
            this.PickupTimeLabel.TabIndex = 2;
            this.PickupTimeLabel.Text = "Pickup Time";
            // 
            // CoversLabel
            // 
            this.CoversLabel.AutoSize = true;
            this.CoversLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CoversLabel.Location = new System.Drawing.Point(11, 162);
            this.CoversLabel.Name = "CoversLabel";
            this.CoversLabel.Size = new System.Drawing.Size(65, 23);
            this.CoversLabel.TabIndex = 5;
            this.CoversLabel.Text = "Covers";
            // 
            // CoversNumeric
            // 
            this.CoversNumeric.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CoversNumeric.Location = new System.Drawing.Point(170, 159);
            this.CoversNumeric.Name = "CoversNumeric";
            this.CoversNumeric.Size = new System.Drawing.Size(320, 30);
            this.CoversNumeric.TabIndex = 3;
            this.CoversNumeric.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // CustomerNameLabel
            // 
            this.CustomerNameLabel.AutoSize = true;
            this.CustomerNameLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CustomerNameLabel.Location = new System.Drawing.Point(11, 198);
            this.CustomerNameLabel.Name = "CustomerNameLabel";
            this.CustomerNameLabel.Size = new System.Drawing.Size(144, 23);
            this.CustomerNameLabel.TabIndex = 2;
            this.CustomerNameLabel.Text = "Customer Name";
            // 
            // CustomerNameTextBox
            // 
            this.CustomerNameTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CustomerNameTextBox.Location = new System.Drawing.Point(170, 195);
            this.CustomerNameTextBox.Name = "CustomerNameTextBox";
            this.CustomerNameTextBox.Size = new System.Drawing.Size(320, 30);
            this.CustomerNameTextBox.TabIndex = 4;
            this.CustomerNameTextBox.Text = "Test Customer Name";
            // 
            // MemberNumberLabel
            // 
            this.MemberNumberLabel.AutoSize = true;
            this.MemberNumberLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MemberNumberLabel.Location = new System.Drawing.Point(11, 234);
            this.MemberNumberLabel.Name = "MemberNumberLabel";
            this.MemberNumberLabel.Size = new System.Drawing.Size(153, 23);
            this.MemberNumberLabel.TabIndex = 2;
            this.MemberNumberLabel.Text = "Member Number";
            // 
            // MemberNumberTextBox
            // 
            this.MemberNumberTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MemberNumberTextBox.Location = new System.Drawing.Point(170, 231);
            this.MemberNumberTextBox.Name = "MemberNumberTextBox";
            this.MemberNumberTextBox.Size = new System.Drawing.Size(320, 30);
            this.MemberNumberTextBox.TabIndex = 5;
            this.MemberNumberTextBox.Text = "1";
            // 
            // PaymentRequiredCheckBox
            // 
            this.PaymentRequiredCheckBox.AutoSize = true;
            this.PaymentRequiredCheckBox.Checked = true;
            this.PaymentRequiredCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.PaymentRequiredCheckBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PaymentRequiredCheckBox.Location = new System.Drawing.Point(11, 588);
            this.PaymentRequiredCheckBox.Name = "PaymentRequiredCheckBox";
            this.PaymentRequiredCheckBox.Size = new System.Drawing.Size(183, 27);
            this.PaymentRequiredCheckBox.TabIndex = 8;
            this.PaymentRequiredCheckBox.Text = "Payment Required";
            this.PaymentRequiredCheckBox.UseVisualStyleBackColor = true;
            // 
            // ItemsButton
            // 
            this.ItemsButton.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ItemsButton.Location = new System.Drawing.Point(11, 621);
            this.ItemsButton.Name = "ItemsButton";
            this.ItemsButton.Size = new System.Drawing.Size(122, 44);
            this.ItemsButton.TabIndex = 9;
            this.ItemsButton.Text = "Items";
            this.ItemsButton.UseVisualStyleBackColor = true;
            this.ItemsButton.Click += new System.EventHandler(this.ItemsButton_Click);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.numericUpDown1.Location = new System.Drawing.Point(-175, -297);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(400, 30);
            this.numericUpDown1.TabIndex = 6;
            // 
            // CommentsTextBox
            // 
            this.CommentsTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CommentsTextBox.Location = new System.Drawing.Point(170, 267);
            this.CommentsTextBox.Name = "CommentsTextBox";
            this.CommentsTextBox.Size = new System.Drawing.Size(320, 30);
            this.CommentsTextBox.TabIndex = 6;
            this.CommentsTextBox.Text = "Test Comment";
            // 
            // CommentsLabel
            // 
            this.CommentsLabel.AutoSize = true;
            this.CommentsLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CommentsLabel.Location = new System.Drawing.Point(11, 270);
            this.CommentsLabel.Name = "CommentsLabel";
            this.CommentsLabel.Size = new System.Drawing.Size(98, 23);
            this.CommentsLabel.TabIndex = 2;
            this.CommentsLabel.Text = "Comments";
            // 
            // RecipientTextBox
            // 
            this.RecipientTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RecipientTextBox.Location = new System.Drawing.Point(170, 303);
            this.RecipientTextBox.Name = "RecipientTextBox";
            this.RecipientTextBox.Size = new System.Drawing.Size(320, 30);
            this.RecipientTextBox.TabIndex = 6;
            this.RecipientTextBox.Text = "Test Recipient";
            // 
            // RecipientLabel
            // 
            this.RecipientLabel.AutoSize = true;
            this.RecipientLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RecipientLabel.Location = new System.Drawing.Point(11, 306);
            this.RecipientLabel.Name = "RecipientLabel";
            this.RecipientLabel.Size = new System.Drawing.Size(87, 23);
            this.RecipientLabel.TabIndex = 2;
            this.RecipientLabel.Text = "Recipient";
            // 
            // StreetNumberTextBox
            // 
            this.StreetNumberTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StreetNumberTextBox.Location = new System.Drawing.Point(170, 339);
            this.StreetNumberTextBox.Name = "StreetNumberTextBox";
            this.StreetNumberTextBox.Size = new System.Drawing.Size(320, 30);
            this.StreetNumberTextBox.TabIndex = 6;
            this.StreetNumberTextBox.Text = "1";
            // 
            // StreetNameTextBox
            // 
            this.StreetNameTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StreetNameTextBox.Location = new System.Drawing.Point(170, 375);
            this.StreetNameTextBox.Name = "StreetNameTextBox";
            this.StreetNameTextBox.Size = new System.Drawing.Size(320, 30);
            this.StreetNameTextBox.TabIndex = 6;
            this.StreetNameTextBox.Text = "Street Name";
            // 
            // SuburbTextBox
            // 
            this.SuburbTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SuburbTextBox.Location = new System.Drawing.Point(170, 411);
            this.SuburbTextBox.Name = "SuburbTextBox";
            this.SuburbTextBox.Size = new System.Drawing.Size(320, 30);
            this.SuburbTextBox.TabIndex = 6;
            this.SuburbTextBox.Text = "Sydenham";
            // 
            // CityTextBox
            // 
            this.CityTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CityTextBox.Location = new System.Drawing.Point(170, 447);
            this.CityTextBox.Name = "CityTextBox";
            this.CityTextBox.Size = new System.Drawing.Size(320, 30);
            this.CityTextBox.TabIndex = 6;
            this.CityTextBox.Text = "Christchurch";
            // 
            // CountryTextBox
            // 
            this.CountryTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CountryTextBox.Location = new System.Drawing.Point(170, 483);
            this.CountryTextBox.Name = "CountryTextBox";
            this.CountryTextBox.Size = new System.Drawing.Size(320, 30);
            this.CountryTextBox.TabIndex = 6;
            this.CountryTextBox.Text = "NZ";
            // 
            // PhoneNumberTextBox
            // 
            this.PhoneNumberTextBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PhoneNumberTextBox.Location = new System.Drawing.Point(170, 519);
            this.PhoneNumberTextBox.Name = "PhoneNumberTextBox";
            this.PhoneNumberTextBox.Size = new System.Drawing.Size(320, 30);
            this.PhoneNumberTextBox.TabIndex = 6;
            this.PhoneNumberTextBox.Text = "0800 800800";
            // 
            // StreetNumberLabel
            // 
            this.StreetNumberLabel.AutoSize = true;
            this.StreetNumberLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StreetNumberLabel.Location = new System.Drawing.Point(11, 342);
            this.StreetNumberLabel.Name = "StreetNumberLabel";
            this.StreetNumberLabel.Size = new System.Drawing.Size(134, 23);
            this.StreetNumberLabel.TabIndex = 2;
            this.StreetNumberLabel.Text = "Street Number";
            // 
            // StreetNameLabel
            // 
            this.StreetNameLabel.AutoSize = true;
            this.StreetNameLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StreetNameLabel.Location = new System.Drawing.Point(11, 378);
            this.StreetNameLabel.Name = "StreetNameLabel";
            this.StreetNameLabel.Size = new System.Drawing.Size(115, 23);
            this.StreetNameLabel.TabIndex = 2;
            this.StreetNameLabel.Text = "Street Name";
            // 
            // SuburbLabel
            // 
            this.SuburbLabel.AutoSize = true;
            this.SuburbLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SuburbLabel.Location = new System.Drawing.Point(11, 414);
            this.SuburbLabel.Name = "SuburbLabel";
            this.SuburbLabel.Size = new System.Drawing.Size(72, 23);
            this.SuburbLabel.TabIndex = 2;
            this.SuburbLabel.Text = "Suburb";
            // 
            // CityLabel
            // 
            this.CityLabel.AutoSize = true;
            this.CityLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CityLabel.Location = new System.Drawing.Point(11, 450);
            this.CityLabel.Name = "CityLabel";
            this.CityLabel.Size = new System.Drawing.Size(40, 23);
            this.CityLabel.TabIndex = 2;
            this.CityLabel.Text = "City";
            // 
            // CountryLabel
            // 
            this.CountryLabel.AutoSize = true;
            this.CountryLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CountryLabel.Location = new System.Drawing.Point(11, 486);
            this.CountryLabel.Name = "CountryLabel";
            this.CountryLabel.Size = new System.Drawing.Size(75, 23);
            this.CountryLabel.TabIndex = 2;
            this.CountryLabel.Text = "Country";
            // 
            // PhoneNumberLabel
            // 
            this.PhoneNumberLabel.AutoSize = true;
            this.PhoneNumberLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PhoneNumberLabel.Location = new System.Drawing.Point(11, 522);
            this.PhoneNumberLabel.Name = "PhoneNumberLabel";
            this.PhoneNumberLabel.Size = new System.Drawing.Size(136, 23);
            this.PhoneNumberLabel.TabIndex = 2;
            this.PhoneNumberLabel.Text = "Phone Number";
            // 
            // SiteNameLabel
            // 
            this.SiteNameLabel.AutoSize = true;
            this.SiteNameLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SiteNameLabel.Location = new System.Drawing.Point(11, 54);
            this.SiteNameLabel.Name = "SiteNameLabel";
            this.SiteNameLabel.Size = new System.Drawing.Size(96, 23);
            this.SiteNameLabel.TabIndex = 12;
            this.SiteNameLabel.Text = "Site Name";
            // 
            // SiteNameBox
            // 
            this.SiteNameBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SiteNameBox.Location = new System.Drawing.Point(170, 51);
            this.SiteNameBox.Name = "SiteNameBox";
            this.SiteNameBox.Size = new System.Drawing.Size(320, 30);
            this.SiteNameBox.TabIndex = 11;
            this.SiteNameBox.Text = "Site Name";
            // 
            // SiteIDLabel
            // 
            this.SiteIDLabel.AutoSize = true;
            this.SiteIDLabel.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SiteIDLabel.Location = new System.Drawing.Point(11, 18);
            this.SiteIDLabel.Name = "SiteIDLabel";
            this.SiteIDLabel.Size = new System.Drawing.Size(67, 23);
            this.SiteIDLabel.TabIndex = 14;
            this.SiteIDLabel.Text = "Site ID";
            // 
            // SiteIDBox
            // 
            this.SiteIDBox.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SiteIDBox.Location = new System.Drawing.Point(170, 15);
            this.SiteIDBox.Name = "SiteIDBox";
            this.SiteIDBox.Size = new System.Drawing.Size(320, 30);
            this.SiteIDBox.TabIndex = 13;
            this.SiteIDBox.Text = "Site ID";
            // 
            // CheckOrderStatusButton
            // 
            this.CheckOrderStatusButton.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CheckOrderStatusButton.Location = new System.Drawing.Point(139, 621);
            this.CheckOrderStatusButton.Name = "CheckOrderStatusButton";
            this.CheckOrderStatusButton.Size = new System.Drawing.Size(122, 44);
            this.CheckOrderStatusButton.TabIndex = 15;
            this.CheckOrderStatusButton.Text = "Check Order Status";
            this.CheckOrderStatusButton.UseVisualStyleBackColor = true;
            this.CheckOrderStatusButton.Click += new System.EventHandler(this.CheckOrderStatusButton_Click);
            // 
            // WebMateTestingProgram
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(513, 677);
            this.Controls.Add(this.CheckOrderStatusButton);
            this.Controls.Add(this.SiteIDLabel);
            this.Controls.Add(this.SiteIDBox);
            this.Controls.Add(this.SiteNameLabel);
            this.Controls.Add(this.SiteNameBox);
            this.Controls.Add(this.ItemsButton);
            this.Controls.Add(this.SendButton);
            this.Controls.Add(this.numericUpDown1);
            this.Controls.Add(this.CoversNumeric);
            this.Controls.Add(this.CoversLabel);
            this.Controls.Add(this.PickupTimeDateTimePicker);
            this.Controls.Add(this.PaymentRequiredCheckBox);
            this.Controls.Add(this.RespondCheckBox);
            this.Controls.Add(this.PickupTimeLabel);
            this.Controls.Add(this.PhoneNumberLabel);
            this.Controls.Add(this.CountryLabel);
            this.Controls.Add(this.CityLabel);
            this.Controls.Add(this.SuburbLabel);
            this.Controls.Add(this.StreetNameLabel);
            this.Controls.Add(this.StreetNumberLabel);
            this.Controls.Add(this.RecipientLabel);
            this.Controls.Add(this.CommentsLabel);
            this.Controls.Add(this.MemberNumberLabel);
            this.Controls.Add(this.CustomerNameLabel);
            this.Controls.Add(this.StoreNameLabel);
            this.Controls.Add(this.PhoneNumberTextBox);
            this.Controls.Add(this.CountryTextBox);
            this.Controls.Add(this.CityTextBox);
            this.Controls.Add(this.SuburbTextBox);
            this.Controls.Add(this.StreetNameTextBox);
            this.Controls.Add(this.StreetNumberTextBox);
            this.Controls.Add(this.RecipientTextBox);
            this.Controls.Add(this.CommentsTextBox);
            this.Controls.Add(this.MemberNumberTextBox);
            this.Controls.Add(this.CustomerNameTextBox);
            this.Controls.Add(this.StoreNameTextBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(519, 389);
            this.Name = "WebMateTestingProgram";
            this.ShowIcon = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "WebMate Testing Program";
            ((System.ComponentModel.ISupportInitialize)(this.CoversNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SendButton;
        private System.Windows.Forms.TextBox StoreNameTextBox;
        private System.Windows.Forms.Label StoreNameLabel;
        private System.Windows.Forms.CheckBox RespondCheckBox;
        private System.Windows.Forms.DateTimePicker PickupTimeDateTimePicker;
        private System.Windows.Forms.Label PickupTimeLabel;
        private System.Windows.Forms.Label CoversLabel;
        private System.Windows.Forms.NumericUpDown CoversNumeric;
        private System.Windows.Forms.Label CustomerNameLabel;
        private System.Windows.Forms.TextBox CustomerNameTextBox;
        private System.Windows.Forms.Label MemberNumberLabel;
        private System.Windows.Forms.TextBox MemberNumberTextBox;
        private System.Windows.Forms.CheckBox PaymentRequiredCheckBox;
        private System.Windows.Forms.Button ItemsButton;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.TextBox CommentsTextBox;
        private System.Windows.Forms.Label CommentsLabel;
        private System.Windows.Forms.TextBox RecipientTextBox;
        private System.Windows.Forms.Label RecipientLabel;
        private System.Windows.Forms.TextBox StreetNumberTextBox;
        private System.Windows.Forms.TextBox StreetNameTextBox;
        private System.Windows.Forms.TextBox SuburbTextBox;
        private System.Windows.Forms.TextBox CityTextBox;
        private System.Windows.Forms.TextBox CountryTextBox;
        private System.Windows.Forms.TextBox PhoneNumberTextBox;
        private System.Windows.Forms.Label StreetNumberLabel;
        private System.Windows.Forms.Label StreetNameLabel;
        private System.Windows.Forms.Label SuburbLabel;
        private System.Windows.Forms.Label CityLabel;
        private System.Windows.Forms.Label CountryLabel;
        private System.Windows.Forms.Label PhoneNumberLabel;
        private System.Windows.Forms.Label SiteNameLabel;
        private System.Windows.Forms.TextBox SiteNameBox;
        private System.Windows.Forms.Label SiteIDLabel;
        private System.Windows.Forms.TextBox SiteIDBox;
        private System.Windows.Forms.Button CheckOrderStatusButton;
    }
}

