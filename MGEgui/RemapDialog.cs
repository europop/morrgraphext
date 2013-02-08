using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices;


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
            this.bCancel = new System.Windows.Forms.Button();
            this.bClear = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // bCancel
            // 
            this.bCancel.Location = new System.Drawing.Point(12, 12);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(60, 23);
            this.bCancel.TabIndex = 0;
            this.bCancel.Text = "Cancel";
            this.bCancel.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.Button_PreviewKeyDown);
            this.bCancel.Click += new System.EventHandler(this.bCancel_Click);
            // 
            // bClear
            // 
            this.bClear.Location = new System.Drawing.Point(78, 12);
            this.bClear.Name = "bClear";
            this.bClear.Size = new System.Drawing.Size(60, 23);
            this.bClear.TabIndex = 1;
            this.bClear.Text = "Clear";
            this.bClear.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.Button_PreviewKeyDown);
            this.bClear.Click += new System.EventHandler(this.bClear_Click);
            // 
            // RemapDialog
            // 
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
            this.Load += new System.EventHandler(this.RemapDialog_Load);
            this.Closing += new CancelEventHandler(this.RemapDialog_FormClosing);
            this.ResumeLayout(false);
        }

        #endregion

        private Button bCancel;
        private Button bClear;
        #endregion

        [DllImport("user32.dll")]
        private static extern uint MapVirtualKey(uint uCode, uint uMapType);

        [DllImport("user32.dll")]
        private static extern short GetAsyncKeyState(Keys vKey);

        public static byte GetKeyScanCode(Keys key, int keyValue) {
            int scancode = (int)MapVirtualKey((uint)key, 0);
            switch (key) {
                case Keys.LWin:
                case Keys.RWin:
                case Keys.Apps:
                case Keys.Insert:
                case Keys.Delete:
                case Keys.Home:
                case Keys.End:
                case Keys.PageUp:
                case Keys.PageDown:
                case Keys.Up:
                case Keys.Down:
                case Keys.Left:
                case Keys.Right:
                case Keys.Divide:
                case Keys.VolumeUp:
                case Keys.VolumeDown:
                case Keys.VolumeMute:
                // Add here
                scancode += 0x80; break;  // Extended key (with KF_EXTENDED flag)

                case Keys.Pause: scancode = 0xC5; break;
                case Keys.ShiftKey:
                    if (Convert.ToBoolean(GetAsyncKeyState(Keys.LShiftKey))) scancode = 0x2a;
                    else if (Convert.ToBoolean(GetAsyncKeyState(Keys.RShiftKey))) scancode = 0x36; break;
                case Keys.ControlKey:
                    if (Convert.ToBoolean(GetAsyncKeyState(Keys.LControlKey))) scancode = 0x1d;
                    else if (Convert.ToBoolean(GetAsyncKeyState(Keys.RControlKey))) scancode = 0x9d; break;
                case Keys.Menu:
                    if (Convert.ToBoolean(GetAsyncKeyState(Keys.LMenu))) scancode = 0x38;
                    else if (Convert.ToBoolean(GetAsyncKeyState(Keys.RMenu))) scancode = 0xb8; break;
                default: break;
            }
            return (byte)((0x01 < scancode) && (scancode < 0xFF) ? scancode : 0);
        }

        public byte result;

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

        private void Button_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            result = GetKeyScanCode(e.KeyCode, e.KeyValue);
            DialogResult = DialogResult.OK;
            Close();
        }

        private void RemapDialog_FormClosing(object sender, CancelEventArgs e) {
        }

        private void RemapDialog_Load(object sender, EventArgs e) {
        }
    }
}
