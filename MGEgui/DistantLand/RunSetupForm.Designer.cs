namespace MGEgui.DistantLand
{
    partial class RunSetupForm
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
            this.bRun = new System.Windows.Forms.Button();
            this.bSetup = new System.Windows.Forms.Button();
            this.cbTex = new System.Windows.Forms.CheckBox();
            this.cbMesh = new System.Windows.Forms.CheckBox();
            this.cbMeshAuto = new System.Windows.Forms.CheckBox();
            this.cbStat = new System.Windows.Forms.CheckBox();
            this.lOr = new System.Windows.Forms.Label();
            this.cbDebug = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // bRun
            // 
            this.bRun.Location = new System.Drawing.Point(12, 104);
            this.bRun.Name = "bRun";
            this.bRun.Size = new System.Drawing.Size(268, 23);
            this.bRun.TabIndex = 0;
            this.bRun.Text = "Run above steps using saved / default settings";
            this.bRun.UseVisualStyleBackColor = true;
            this.bRun.Click += new System.EventHandler(this.bRun_Click);
            // 
            // bSetup
            // 
            this.bSetup.Location = new System.Drawing.Point(12, 150);
            this.bSetup.Name = "bSetup";
            this.bSetup.Size = new System.Drawing.Size(268, 23);
            this.bSetup.TabIndex = 1;
            this.bSetup.Text = "Re-run Distant Land Configuration Setup >>>";
            this.bSetup.UseVisualStyleBackColor = true;
            this.bSetup.Click += new System.EventHandler(this.bSetup_Click);
            // 
            // cbTex
            // 
            this.cbTex.AutoSize = true;
            this.cbTex.Checked = true;
            this.cbTex.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbTex.Location = new System.Drawing.Point(12, 12);
            this.cbTex.Name = "cbTex";
            this.cbTex.Size = new System.Drawing.Size(166, 17);
            this.cbTex.TabIndex = 2;
            this.cbTex.Text = "Create distant land textures";
            this.cbTex.UseVisualStyleBackColor = true;
            // 
            // cbMesh
            // 
            this.cbMesh.AutoSize = true;
            this.cbMesh.Checked = true;
            this.cbMesh.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbMesh.Location = new System.Drawing.Point(12, 35);
            this.cbMesh.Name = "cbMesh";
            this.cbMesh.Size = new System.Drawing.Size(163, 17);
            this.cbMesh.TabIndex = 3;
            this.cbMesh.Text = "Create distant land meshes";
            this.cbMesh.UseVisualStyleBackColor = true;
            this.cbMesh.CheckedChanged += new System.EventHandler(this.cbMesh_CheckedChanged);
            // 
            // cbMeshAuto
            // 
            this.cbMeshAuto.AutoSize = true;
            this.cbMeshAuto.Checked = true;
            this.cbMeshAuto.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbMeshAuto.Location = new System.Drawing.Point(31, 58);
            this.cbMeshAuto.Name = "cbMeshAuto";
            this.cbMeshAuto.Size = new System.Drawing.Size(226, 17);
            this.cbMeshAuto.TabIndex = 4;
            this.cbMeshAuto.Text = "Use auto-calculated world mesh details";
            this.cbMeshAuto.UseVisualStyleBackColor = true;
            // 
            // cbStat
            // 
            this.cbStat.AutoSize = true;
            this.cbStat.Checked = true;
            this.cbStat.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbStat.Location = new System.Drawing.Point(12, 81);
            this.cbStat.Name = "cbStat";
            this.cbStat.Size = new System.Drawing.Size(131, 17);
            this.cbStat.TabIndex = 5;
            this.cbStat.Text = "Create distant statics";
            this.cbStat.UseVisualStyleBackColor = true;
            // 
            // lOr
            // 
            this.lOr.Location = new System.Drawing.Point(9, 130);
            this.lOr.Name = "lOr";
            this.lOr.Size = new System.Drawing.Size(271, 17);
            this.lOr.TabIndex = 6;
            this.lOr.Text = "or";
            this.lOr.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // cbDebug
            // 
            this.cbDebug.AutoSize = true;
            this.cbDebug.ForeColor = System.Drawing.Color.Red;
            this.cbDebug.Location = new System.Drawing.Point(12, 130);
            this.cbDebug.Name = "cbDebug";
            this.cbDebug.Size = new System.Drawing.Size(61, 17);
            this.cbDebug.TabIndex = 7;
            this.cbDebug.Text = "DEBUG";
            this.cbDebug.UseVisualStyleBackColor = true;
            this.cbDebug.Visible = false;
            // 
            // RunSetupForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 185);
            this.Controls.Add(this.cbDebug);
            this.Controls.Add(this.lOr);
            this.Controls.Add(this.cbStat);
            this.Controls.Add(this.cbMeshAuto);
            this.Controls.Add(this.cbMesh);
            this.Controls.Add(this.cbTex);
            this.Controls.Add(this.bSetup);
            this.Controls.Add(this.bRun);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.HelpButton = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RunSetupForm";
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Choose data creation method";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bRun;
        private System.Windows.Forms.Button bSetup;
        private System.Windows.Forms.CheckBox cbTex;
        private System.Windows.Forms.CheckBox cbMesh;
        private System.Windows.Forms.CheckBox cbMeshAuto;
        private System.Windows.Forms.CheckBox cbStat;
        private System.Windows.Forms.Label lOr;
        private System.Windows.Forms.CheckBox cbDebug;
    }
}