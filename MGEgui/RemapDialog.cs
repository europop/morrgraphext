using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace MGEgui {
    public class RemapDialog : Form {
        #region Form designer gunk
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            this.bCancel = new System.Windows.Forms.Button();
            this.bClear = new System.Windows.Forms.Button();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // bCancel
            // 
            this.bCancel.Location = new System.Drawing.Point(12, 12);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(60, 23);
            this.bCancel.TabIndex = 0;
            this.bCancel.Text = "Cancel";
            //this.bCancel.UseVisualStyleBackColor = true;
            this.bCancel.Click += new System.EventHandler(this.bCancel_Click);
            // 
            // bClear
            // 
            this.bClear.Location = new System.Drawing.Point(78, 12);
            this.bClear.Name = "bClear";
            this.bClear.Size = new System.Drawing.Size(60, 23);
            this.bClear.TabIndex = 1;
            this.bClear.Text = "Clear";
            //this.bClear.UseVisualStyleBackColor = true;
            this.bClear.Click += new System.EventHandler(this.bClear_Click);
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // RemapDialog
            // 
            //this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            //this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Icon = Properties.Resources.AppIcon;
            this.ClientSize = new System.Drawing.Size(154, 47);
            this.ControlBox = false;
            this.Controls.Add(this.bClear);
            this.Controls.Add(this.bCancel);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RemapDialog";
            this.ShowInTaskbar = false;
            this.Text = "Please hit a key";
            this.TopMost = true;
            this.Closing += new CancelEventHandler(this.RemapDialog_FormClosing);
            this.Load += new System.EventHandler(this.RemapDialog_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private Button bCancel;
        #endregion
        private Button bClear;
        private Timer timer;
        
        public byte result;
        private bool Initilized=false;

        public RemapDialog() {
            InitializeComponent();
            DialogResult=DialogResult.Cancel;
        }

        private void bCancel_Click(object sender, EventArgs e) {
            DialogResult=DialogResult.Cancel;
            Close();
        }

        private void bClear_Click(object sender, EventArgs e) {
            DialogResult=DialogResult.OK;
            result=0;
            Close();
        }

        private void timer_Tick(object sender, EventArgs e) {
            if(!Initilized) return;
            byte b=NativeMethods.RemapperPressed();
            if(b!=0) {
                result=b;
                DialogResult=DialogResult.OK;
                Close();
            }
        }

        private void RemapDialog_FormClosing(object sender, CancelEventArgs e) {
            timer.Stop();
            NativeMethods.RemapperEnd();
        }

        private void RemapDialog_Load(object sender, EventArgs e) {
            if(NativeMethods.RemapperInit(Handle)!=0) {
                MessageBox.Show("Error creating direct input device", "Error");
            } else {
                Initilized=true;
            }
        }
    }
}
