namespace MGEgui
{
    partial class PatchForm
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
            this.tvPatch = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // tvPatch
            // 
            this.tvPatch.CheckBoxes = true;
            this.tvPatch.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvPatch.Location = new System.Drawing.Point(0, 0);
            this.tvPatch.Name = "tvPatch";
            this.tvPatch.ShowNodeToolTips = true;
            this.tvPatch.Size = new System.Drawing.Size(392, 273);
            this.tvPatch.TabIndex = 0;
            this.tvPatch.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.tvPatch_AfterCheck);
            this.tvPatch.BeforeCheck += new System.Windows.Forms.TreeViewCancelEventHandler(this.tvPatch_BeforeCheck);
            // 
            // PatchForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(392, 273);
            this.Controls.Add(this.tvPatch);
            this.Icon = global::MGEgui.Properties.Resources.AppIcon;
            this.Name = "PatchForm";
            this.Text = "Patch manager";
            this.Load += new System.EventHandler(this.PatchForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView tvPatch;

    }
}