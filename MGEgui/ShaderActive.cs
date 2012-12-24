using System;
using System.Windows.Forms;
using System.IO;

namespace MGEgui {
    public class ShaderActive : Form {
        private Button bMoveUp;
        private ListBox lbSelected;
        private ListBox lbAvailable;
        private Button bMoveDown;
        private Button bSave;
        private Button bClear;
        private Button bCancel;
        #region FormDesignerGunk
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components=null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing&&(components!=null)) {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ShaderActive));
            this.bMoveUp = new System.Windows.Forms.Button();
            this.lbSelected = new System.Windows.Forms.ListBox();
            this.lbAvailable = new System.Windows.Forms.ListBox();
            this.bMoveDown = new System.Windows.Forms.Button();
            this.bSave = new System.Windows.Forms.Button();
            this.bClear = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // bMoveUp
            // 
            this.bMoveUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bMoveUp.Location = new System.Drawing.Point(204, 164);
            this.bMoveUp.Name = "bMoveUp";
            this.bMoveUp.Size = new System.Drawing.Size(75, 23);
            this.bMoveUp.TabIndex = 4;
            this.bMoveUp.Text = "Move up";
            this.bMoveUp.Click += new System.EventHandler(this.bMoveUp_Click);
            // 
            // lbSelected
            // 
            this.lbSelected.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lbSelected.Location = new System.Drawing.Point(12, 164);
            this.lbSelected.Name = "lbSelected";
            this.lbSelected.Size = new System.Drawing.Size(186, 95);
            this.lbSelected.TabIndex = 3;
            this.lbSelected.DoubleClick += new System.EventHandler(this.lbSelected_DoubleClick);
            // 
            // lbAvailable
            // 
            this.lbAvailable.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lbAvailable.Location = new System.Drawing.Point(12, 12);
            this.lbAvailable.Name = "lbAvailable";
            this.lbAvailable.Size = new System.Drawing.Size(186, 134);
            this.lbAvailable.TabIndex = 2;
            this.lbAvailable.DoubleClick += new System.EventHandler(this.lbAvailable_DoubleClick);
            // 
            // bMoveDown
            // 
            this.bMoveDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bMoveDown.Location = new System.Drawing.Point(204, 193);
            this.bMoveDown.Name = "bMoveDown";
            this.bMoveDown.Size = new System.Drawing.Size(75, 23);
            this.bMoveDown.TabIndex = 5;
            this.bMoveDown.Text = "Move down";
            this.bMoveDown.Click += new System.EventHandler(this.bMoveDown_Click);
            // 
            // bSave
            // 
            this.bSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bSave.Location = new System.Drawing.Point(204, 12);
            this.bSave.Name = "bSave";
            this.bSave.Size = new System.Drawing.Size(75, 23);
            this.bSave.TabIndex = 0;
            this.bSave.Text = "Save";
            this.bSave.Click += new System.EventHandler(this.bSave_Click);
            // 
            // bClear
            // 
            this.bClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bClear.Location = new System.Drawing.Point(204, 222);
            this.bClear.Name = "bClear";
            this.bClear.Size = new System.Drawing.Size(75, 23);
            this.bClear.TabIndex = 6;
            this.bClear.Text = "Clear";
            this.bClear.Click += new System.EventHandler(this.bClear_Click);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(204, 41);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(75, 23);
            this.bCancel.TabIndex = 1;
            this.bCancel.Text = "Cancel";
            this.bCancel.Click += new System.EventHandler(this.bCancel_Click);
            // 
            // ShaderActive
            // 
            this.Icon = Properties.Resources.AppIcon;
            this.AcceptButton = this.bSave;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(295, 273);
            this.Controls.Add(this.bCancel);
            this.Controls.Add(this.bClear);
            this.Controls.Add(this.bSave);
            this.Controls.Add(this.bMoveDown);
            this.Controls.Add(this.lbAvailable);
            this.Controls.Add(this.lbSelected);
            this.Controls.Add(this.bMoveUp);
            this.MinimumSize = new System.Drawing.Size(303, 303);
            this.Name = "ShaderActive";
            this.Text = "Set active shaders";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ShaderActive_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion
        #endregion
        public ShaderActive() {
            InitializeComponent();
            foreach (String s in Directory.GetFiles(Statics.runDir + "\\" + @"data files\shaders\default")) {
                if(s.EndsWith(".fx")) lbAvailable.Items.Add(Path.GetFileNameWithoutExtension(s));
            }
            if (File.Exists(Statics.runDir + "\\" + @"data files\shaders\default\active.sav")) {
                FileStream fs = File.OpenRead(Statics.runDir + "\\" + @"data files\shaders\default\active.sav");
                if(fs.Length>0) {
                    string s="";
                    int i;
                    while((i=fs.ReadByte())!=-1) {
                        if(i==0) {
                            lbSelected.Items.Add(Path.GetFileNameWithoutExtension(s));
                            s="";
                        } else {
                            s+=(char)(byte)i;
                        }
                    }
                }
                fs.Close();
            }
            DirectX.DXMain.CreateDevice(this);
        }

        private void lbAvailable_DoubleClick(object sender,EventArgs e) {
            if(lbSelected.Items.Count>=32) return;
            if(lbAvailable.SelectedIndex==-1) return;
            if (DirectX.Shaders.CompileShader(false, null, null, null, Statics.runDir + "\\" + "Data files\\shaders\\default\\" + (string)lbAvailable.SelectedItem + ".fx") != null) {
                MessageBox.Show("That shader does not currently compile.");
            } else {
                lbSelected.Items.Add(string.Copy((string)lbAvailable.SelectedItem));
            }
        }

        private void lbSelected_DoubleClick(object sender,EventArgs e) {
            if(lbSelected.SelectedIndex!=-1) lbSelected.Items.RemoveAt(lbSelected.SelectedIndex);
        }

        private void bCancel_Click(object sender,EventArgs e) {
            Close();
        }

        private void bSave_Click(object sender,EventArgs e) {
            if(lbSelected.Items.Count==0) {
                File.Delete(Statics.runDir + "\\" + @"Data files\shaders\default\active.sav");
            } else {
                BinaryWriter bw = new BinaryWriter(File.Open(Statics.runDir + "\\" + @"Data files\shaders\default\active.sav", FileMode.Create));
                foreach(string s in lbSelected.Items) {
                    foreach(char c in "data files\\shaders\\default\\") {
                        bw.Write((byte)c);
                    }
                    for(int i=0;i<s.Length;i++) bw.Write((byte)s[i]);
                    foreach(char c in ".fx") {
                        bw.Write((byte)c);
                    }
                    bw.Write((byte)0);
                }
                bw.Close();
            }
            Close();
        }

        private void bMoveUp_Click(object sender,EventArgs e) {
            if(lbSelected.SelectedIndex>0) {
                string s=(string)lbSelected.Items[lbSelected.SelectedIndex];
                lbSelected.Items[lbSelected.SelectedIndex]=lbSelected.Items[lbSelected.SelectedIndex-1];
                lbSelected.Items[lbSelected.SelectedIndex-1]=s;
                lbSelected.SelectedIndex--;
            }
        }

        private void bMoveDown_Click(object sender,EventArgs e) {
            if(lbSelected.SelectedIndex!=-1&&lbSelected.SelectedIndex!=lbSelected.Items.Count-1) {
                string s=(string)lbSelected.Items[lbSelected.SelectedIndex];
                lbSelected.Items[lbSelected.SelectedIndex]=lbSelected.Items[lbSelected.SelectedIndex+1];
                lbSelected.Items[lbSelected.SelectedIndex+1]=s;
                lbSelected.SelectedIndex++;
            }
        }

        private void bClear_Click(object sender,EventArgs e) {
            lbSelected.Items.Clear();
            File.Delete(Statics.runDir + "\\" + @"Data files\shaders\default\active.sav");
        }

        private void ShaderActive_FormClosing(object sender, FormClosingEventArgs e) {
            DirectX.DXMain.CloseDevice();
        }
    }
}
