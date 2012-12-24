namespace MGEgui {
    partial class DLRipplesForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose (bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose ();
            }
            base.Dispose (disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent () {
            this.bOK = new System.Windows.Forms.Button ();
            this.bCancel = new System.Windows.Forms.Button ();
            this.cmbPlayerTex = new System.Windows.Forms.ComboBox ();
            this.gbPlayer = new System.Windows.Forms.GroupBox ();
            this.lPlayerWorld = new System.Windows.Forms.Label ();
            this.lPlayerTex = new System.Windows.Forms.Label ();
            this.udPlayerWorld = new System.Windows.Forms.NumericUpDown ();
            this.gbRain = new System.Windows.Forms.GroupBox ();
            this.lRainTex = new System.Windows.Forms.Label ();
            this.cmbRainTex = new System.Windows.Forms.ComboBox ();
            this.gbPlayer.SuspendLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.udPlayerWorld)).BeginInit ();
            this.gbRain.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // bOK
            // 
            this.bOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.bOK.Location = new System.Drawing.Point (45, 121);
            this.bOK.Name = "bOK";
            this.bOK.Size = new System.Drawing.Size (75, 23);
            this.bOK.TabIndex = 0;
            this.bOK.Text = "&OK";
            this.bOK.UseVisualStyleBackColor = true;
            this.bOK.Click += new System.EventHandler (this.bOK_Click);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.bCancel.Location = new System.Drawing.Point (167, 121);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size (75, 23);
            this.bCancel.TabIndex = 1;
            this.bCancel.Text = "&Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            // 
            // cmbPlayerTex
            // 
            this.cmbPlayerTex.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmbPlayerTex.Items.AddRange (new object [] {
            "128",
            "256",
            "512",
            "1024",
            "2048",
            "4096"});
            this.cmbPlayerTex.Location = new System.Drawing.Point (87, 18);
            this.cmbPlayerTex.Name = "cmbPlayerTex";
            this.cmbPlayerTex.Size = new System.Drawing.Size (48, 21);
            this.cmbPlayerTex.TabIndex = 0;
            this.cmbPlayerTex.Text = "512";
            // 
            // gbPlayer
            // 
            this.gbPlayer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbPlayer.Controls.Add (this.lPlayerWorld);
            this.gbPlayer.Controls.Add (this.lPlayerTex);
            this.gbPlayer.Controls.Add (this.udPlayerWorld);
            this.gbPlayer.Controls.Add (this.cmbPlayerTex);
            this.gbPlayer.Location = new System.Drawing.Point (12, 12);
            this.gbPlayer.Name = "gbPlayer";
            this.gbPlayer.Size = new System.Drawing.Size (263, 46);
            this.gbPlayer.TabIndex = 2;
            this.gbPlayer.TabStop = false;
            this.gbPlayer.Text = "Player";
            // 
            // lPlayerWorld
            // 
            this.lPlayerWorld.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.lPlayerWorld.AutoSize = true;
            this.lPlayerWorld.Location = new System.Drawing.Point (147, 21);
            this.lPlayerWorld.Name = "lPlayerWorld";
            this.lPlayerWorld.Size = new System.Drawing.Size (56, 13);
            this.lPlayerWorld.TabIndex = 0;
            this.lPlayerWorld.Text = "World size";
            // 
            // lPlayerTex
            // 
            this.lPlayerTex.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lPlayerTex.AutoSize = true;
            this.lPlayerTex.Location = new System.Drawing.Point (17, 21);
            this.lPlayerTex.Name = "lPlayerTex";
            this.lPlayerTex.Size = new System.Drawing.Size (64, 13);
            this.lPlayerTex.TabIndex = 0;
            this.lPlayerTex.Text = "Texture size";
            // 
            // udPlayerWorld
            // 
            this.udPlayerWorld.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.udPlayerWorld.DecimalPlaces = 2;
            this.udPlayerWorld.Increment = new decimal (new int [] {
            25,
            0,
            0,
            131072});
            this.udPlayerWorld.Location = new System.Drawing.Point (209, 19);
            this.udPlayerWorld.Maximum = new decimal (new int [] {
            5,
            0,
            0,
            0});
            this.udPlayerWorld.Minimum = new decimal (new int [] {
            25,
            0,
            0,
            131072});
            this.udPlayerWorld.Name = "udPlayerWorld";
            this.udPlayerWorld.Size = new System.Drawing.Size (48, 20);
            this.udPlayerWorld.TabIndex = 1;
            this.udPlayerWorld.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udPlayerWorld.Value = new decimal (new int [] {
            25,
            0,
            0,
            65536});
            // 
            // gbRain
            // 
            this.gbRain.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbRain.Controls.Add (this.lRainTex);
            this.gbRain.Controls.Add (this.cmbRainTex);
            this.gbRain.Location = new System.Drawing.Point (12, 64);
            this.gbRain.Name = "gbRain";
            this.gbRain.Size = new System.Drawing.Size (263, 46);
            this.gbRain.TabIndex = 3;
            this.gbRain.TabStop = false;
            this.gbRain.Text = "Rain";
            // 
            // lRainTex
            // 
            this.lRainTex.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lRainTex.AutoSize = true;
            this.lRainTex.Location = new System.Drawing.Point (17, 21);
            this.lRainTex.Name = "lRainTex";
            this.lRainTex.Size = new System.Drawing.Size (64, 13);
            this.lRainTex.TabIndex = 0;
            this.lRainTex.Text = "Texture size";
            // 
            // cmbRainTex
            // 
            this.cmbRainTex.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cmbRainTex.Items.AddRange (new object [] {
            "128",
            "256",
            "512",
            "1024",
            "2048",
            "4096"});
            this.cmbRainTex.Location = new System.Drawing.Point (87, 18);
            this.cmbRainTex.Name = "cmbRainTex";
            this.cmbRainTex.Size = new System.Drawing.Size (48, 21);
            this.cmbRainTex.TabIndex = 0;
            this.cmbRainTex.Text = "512";
            // 
            // DLRipplesForm
            // 
            this.Icon = Properties.Resources.AppIcon;
            this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size (287, 156);
            this.Controls.Add (this.gbRain);
            this.Controls.Add (this.gbPlayer);
            this.Controls.Add (this.bCancel);
            this.Controls.Add (this.bOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "DLRipplesForm";
            this.Text = "Dynamic Ripples settings";
            this.gbPlayer.ResumeLayout (false);
            this.gbPlayer.PerformLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.udPlayerWorld)).EndInit ();
            this.gbRain.ResumeLayout (false);
            this.gbRain.PerformLayout ();
            this.ResumeLayout (false);

        }

        #endregion

        private System.Windows.Forms.Button bOK;
        private System.Windows.Forms.Button bCancel;
        public System.Windows.Forms.ComboBox cmbPlayerTex;
        private System.Windows.Forms.GroupBox gbPlayer;
        public System.Windows.Forms.NumericUpDown udPlayerWorld;
        private System.Windows.Forms.Label lPlayerWorld;
        private System.Windows.Forms.Label lPlayerTex;
        private System.Windows.Forms.GroupBox gbRain;
        private System.Windows.Forms.Label lRainTex;
        public System.Windows.Forms.ComboBox cmbRainTex;
    }
}