namespace MGEgui
{
    partial class PatchEditorForm
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
            this.components = new System.ComponentModel.Container();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.tsmiFile = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiNew = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.tsmiSave = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tsmiEexit = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiUndo = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiRedo = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.tsmiCut = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.tsmiSelect = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiTools = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiPatchByFile = new System.Windows.Forms.ToolStripMenuItem();
            this.customizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tsmiHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.contentsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.indexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.bNew = new System.Windows.Forms.ToolStripButton();
            this.bOpen = new System.Windows.Forms.ToolStripButton();
            this.bSave = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.bUndo = new System.Windows.Forms.ToolStripButton();
            this.bRedo = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.bCut = new System.Windows.Forms.ToolStripButton();
            this.bCopy = new System.Windows.Forms.ToolStripButton();
            this.bPaste = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.bAsm = new System.Windows.Forms.ToolStripButton();
            this.bHex = new System.Windows.Forms.ToolStripButton();
            this.splitContainer = new System.Windows.Forms.SplitContainer();
            this.bAddSection = new System.Windows.Forms.Button();
            this.tbNewSection = new System.Windows.Forms.TextBox();
            this.lbSections = new System.Windows.Forms.ListBox();
            this.splitContainerRight = new System.Windows.Forms.SplitContainer();
            this.panelBorder = new System.Windows.Forms.Panel();
            this.propertyGrid = new System.Windows.Forms.PropertyGrid();
            this.splitContainerEditor = new System.Windows.Forms.SplitContainer();
            this.richTextBoxView = new System.Windows.Forms.RichTextBox();
            this.richTextBoxEditor = new System.Windows.Forms.RichTextBox();
            this.openPatch = new System.Windows.Forms.OpenFileDialog();
            this.savePatch = new System.Windows.Forms.SaveFileDialog();
            this.contextStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.splitContainer.Panel1.SuspendLayout();
            this.splitContainer.Panel2.SuspendLayout();
            this.splitContainer.SuspendLayout();
            this.splitContainerRight.Panel1.SuspendLayout();
            this.splitContainerRight.Panel2.SuspendLayout();
            this.splitContainerRight.SuspendLayout();
            this.panelBorder.SuspendLayout();
            this.splitContainerEditor.Panel1.SuspendLayout();
            this.splitContainerEditor.Panel2.SuspendLayout();
            this.splitContainerEditor.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.BackColor = System.Drawing.SystemColors.Menu;
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiFile,
            this.tsmiEdit,
            this.tsmiTools,
            this.tsmiHelp});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStrip.Size = new System.Drawing.Size(632, 24);
            this.menuStrip.TabIndex = 0;
            // 
            // tsmiFile
            // 
            this.tsmiFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiNew,
            this.tsmiOpen,
            this.toolStripSeparator,
            this.tsmiSave,
            this.tsmiSaveAs,
            this.toolStripSeparator1,
            this.tsmiEexit});
            this.tsmiFile.Name = "tsmiFile";
            this.tsmiFile.Size = new System.Drawing.Size(35, 20);
            this.tsmiFile.Text = "&File";
            // 
            // tsmiNew
            // 
            this.tsmiNew.Image = global::MGEgui.Properties.Resources._new;
            this.tsmiNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiNew.Name = "tsmiNew";
            this.tsmiNew.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.tsmiNew.Size = new System.Drawing.Size(192, 22);
            this.tsmiNew.Text = "&New";
            this.tsmiNew.Click += new System.EventHandler(this.bNew_Click);
            // 
            // tsmiOpen
            // 
            this.tsmiOpen.Image = global::MGEgui.Properties.Resources.open;
            this.tsmiOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiOpen.Name = "tsmiOpen";
            this.tsmiOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.tsmiOpen.Size = new System.Drawing.Size(192, 22);
            this.tsmiOpen.Text = "&Open";
            this.tsmiOpen.Click += new System.EventHandler(this.bOpen_Click);
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(189, 6);
            // 
            // tsmiSave
            // 
            this.tsmiSave.Enabled = false;
            this.tsmiSave.Image = global::MGEgui.Properties.Resources.save;
            this.tsmiSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiSave.Name = "tsmiSave";
            this.tsmiSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.tsmiSave.Size = new System.Drawing.Size(192, 22);
            this.tsmiSave.Text = "&Save";
            this.tsmiSave.Click += new System.EventHandler(this.bSave_Click);
            // 
            // tsmiSaveAs
            // 
            this.tsmiSaveAs.Image = global::MGEgui.Properties.Resources.saveas;
            this.tsmiSaveAs.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiSaveAs.Name = "tsmiSaveAs";
            this.tsmiSaveAs.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.S)));
            this.tsmiSaveAs.Size = new System.Drawing.Size(192, 22);
            this.tsmiSaveAs.Text = "Save &As";
            this.tsmiSaveAs.Click += new System.EventHandler(this.tsmiSaveAs_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(189, 6);
            // 
            // tsmiEexit
            // 
            this.tsmiEexit.Name = "tsmiEexit";
            this.tsmiEexit.Size = new System.Drawing.Size(192, 22);
            this.tsmiEexit.Text = "E&xit";
            this.tsmiEexit.Click += new System.EventHandler(this.tsmiEexit_Click);
            // 
            // tsmiEdit
            // 
            this.tsmiEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiUndo,
            this.tsmiRedo,
            this.toolStripSeparator3,
            this.tsmiCut,
            this.tsmiCopy,
            this.tsmiPaste,
            this.toolStripSeparator4,
            this.tsmiSelect});
            this.tsmiEdit.Name = "tsmiEdit";
            this.tsmiEdit.Size = new System.Drawing.Size(37, 20);
            this.tsmiEdit.Text = "&Edit";
            // 
            // tsmiUndo
            // 
            this.tsmiUndo.Image = global::MGEgui.Properties.Resources.undo;
            this.tsmiUndo.Name = "tsmiUndo";
            this.tsmiUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.tsmiUndo.Size = new System.Drawing.Size(167, 22);
            this.tsmiUndo.Text = "&Undo";
            this.tsmiUndo.Click += new System.EventHandler(this.tsmiUndoRedo_Click);
            // 
            // tsmiRedo
            // 
            this.tsmiRedo.Image = global::MGEgui.Properties.Resources.redo;
            this.tsmiRedo.Name = "tsmiRedo";
            this.tsmiRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.tsmiRedo.Size = new System.Drawing.Size(167, 22);
            this.tsmiRedo.Text = "&Redo";
            this.tsmiRedo.Click += new System.EventHandler(this.tsmiUndoRedo_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(164, 6);
            // 
            // tsmiCut
            // 
            this.tsmiCut.Enabled = false;
            this.tsmiCut.Image = global::MGEgui.Properties.Resources.cut;
            this.tsmiCut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiCut.Name = "tsmiCut";
            this.tsmiCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.tsmiCut.Size = new System.Drawing.Size(167, 22);
            this.tsmiCut.Text = "Cu&t";
            this.tsmiCut.Click += new System.EventHandler(this.bCut_Click);
            // 
            // tsmiCopy
            // 
            this.tsmiCopy.Enabled = false;
            this.tsmiCopy.Image = global::MGEgui.Properties.Resources.copy;
            this.tsmiCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiCopy.Name = "tsmiCopy";
            this.tsmiCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.tsmiCopy.Size = new System.Drawing.Size(167, 22);
            this.tsmiCopy.Text = "&Copy";
            this.tsmiCopy.Click += new System.EventHandler(this.bCopy_Click);
            // 
            // tsmiPaste
            // 
            this.tsmiPaste.Enabled = false;
            this.tsmiPaste.Image = global::MGEgui.Properties.Resources.paste;
            this.tsmiPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsmiPaste.Name = "tsmiPaste";
            this.tsmiPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.tsmiPaste.Size = new System.Drawing.Size(167, 22);
            this.tsmiPaste.Text = "&Paste";
            this.tsmiPaste.Click += new System.EventHandler(this.bPaste_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(164, 6);
            // 
            // tsmiSelect
            // 
            this.tsmiSelect.Enabled = false;
            this.tsmiSelect.Name = "tsmiSelect";
            this.tsmiSelect.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.tsmiSelect.Size = new System.Drawing.Size(167, 22);
            this.tsmiSelect.Text = "Select &All";
            this.tsmiSelect.Click += new System.EventHandler(this.tsmiSelect_Click);
            // 
            // tsmiTools
            // 
            this.tsmiTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiPatchByFile,
            this.customizeToolStripMenuItem,
            this.optionsToolStripMenuItem});
            this.tsmiTools.Name = "tsmiTools";
            this.tsmiTools.Size = new System.Drawing.Size(44, 20);
            this.tsmiTools.Text = "&Tools";
            // 
            // tsmiPatchByFile
            // 
            this.tsmiPatchByFile.Name = "tsmiPatchByFile";
            this.tsmiPatchByFile.Size = new System.Drawing.Size(227, 22);
            this.tsmiPatchByFile.Text = "Create Patch by difference...";
            this.tsmiPatchByFile.Click += new System.EventHandler(this.tsmiPatchByFile_Click);
            // 
            // customizeToolStripMenuItem
            // 
            this.customizeToolStripMenuItem.Enabled = false;
            this.customizeToolStripMenuItem.Name = "customizeToolStripMenuItem";
            this.customizeToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
            this.customizeToolStripMenuItem.Text = "&Customize";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Enabled = false;
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(227, 22);
            this.optionsToolStripMenuItem.Text = "&Options";
            // 
            // tsmiHelp
            // 
            this.tsmiHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contentsToolStripMenuItem,
            this.indexToolStripMenuItem,
            this.searchToolStripMenuItem,
            this.toolStripSeparator5,
            this.aboutToolStripMenuItem});
            this.tsmiHelp.Enabled = false;
            this.tsmiHelp.Name = "tsmiHelp";
            this.tsmiHelp.Size = new System.Drawing.Size(40, 20);
            this.tsmiHelp.Text = "&Help";
            // 
            // contentsToolStripMenuItem
            // 
            this.contentsToolStripMenuItem.Name = "contentsToolStripMenuItem";
            this.contentsToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.contentsToolStripMenuItem.Text = "&Contents";
            // 
            // indexToolStripMenuItem
            // 
            this.indexToolStripMenuItem.Name = "indexToolStripMenuItem";
            this.indexToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.indexToolStripMenuItem.Text = "&Index";
            // 
            // searchToolStripMenuItem
            // 
            this.searchToolStripMenuItem.Name = "searchToolStripMenuItem";
            this.searchToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.searchToolStripMenuItem.Text = "&Search";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(126, 6);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.aboutToolStripMenuItem.Text = "&About...";
            // 
            // toolStrip
            // 
            this.toolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.bNew,
            this.bOpen,
            this.bSave,
            this.toolStripSeparator2,
            this.bUndo,
            this.bRedo,
            this.toolStripSeparator6,
            this.bCut,
            this.bCopy,
            this.bPaste,
            this.toolStripSeparator7});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip.Size = new System.Drawing.Size(632, 25);
            this.toolStrip.TabIndex = 1;
            // 
            // bNew
            // 
            this.bNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bNew.Image = global::MGEgui.Properties.Resources._new;
            this.bNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bNew.Name = "bNew";
            this.bNew.Size = new System.Drawing.Size(23, 22);
            this.bNew.Text = "&New";
            this.bNew.Click += new System.EventHandler(this.bNew_Click);
            // 
            // bOpen
            // 
            this.bOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bOpen.Image = global::MGEgui.Properties.Resources.open;
            this.bOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bOpen.Name = "bOpen";
            this.bOpen.Size = new System.Drawing.Size(23, 22);
            this.bOpen.Text = "&Open";
            this.bOpen.Click += new System.EventHandler(this.bOpen_Click);
            // 
            // bSave
            // 
            this.bSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bSave.Enabled = false;
            this.bSave.Image = global::MGEgui.Properties.Resources.save;
            this.bSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bSave.Name = "bSave";
            this.bSave.Size = new System.Drawing.Size(23, 22);
            this.bSave.Text = "&Save";
            this.bSave.Click += new System.EventHandler(this.bSave_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // bUndo
            // 
            this.bUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bUndo.Image = global::MGEgui.Properties.Resources.undo;
            this.bUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bUndo.Name = "bUndo";
            this.bUndo.Size = new System.Drawing.Size(23, 22);
            this.bUndo.Text = "&Undo";
            this.bUndo.Click += new System.EventHandler(this.tsmiUndoRedo_Click);
            // 
            // bRedo
            // 
            this.bRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bRedo.Image = global::MGEgui.Properties.Resources.redo;
            this.bRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bRedo.Name = "bRedo";
            this.bRedo.Size = new System.Drawing.Size(23, 22);
            this.bRedo.Text = "&Redo";
            this.bRedo.Click += new System.EventHandler(this.tsmiUndoRedo_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 25);
            // 
            // bCut
            // 
            this.bCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bCut.Enabled = false;
            this.bCut.Image = global::MGEgui.Properties.Resources.cut;
            this.bCut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bCut.Name = "bCut";
            this.bCut.Size = new System.Drawing.Size(23, 22);
            this.bCut.Text = "C&ut";
            this.bCut.Click += new System.EventHandler(this.bCut_Click);
            // 
            // bCopy
            // 
            this.bCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bCopy.Enabled = false;
            this.bCopy.Image = global::MGEgui.Properties.Resources.copy;
            this.bCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bCopy.Name = "bCopy";
            this.bCopy.Size = new System.Drawing.Size(23, 22);
            this.bCopy.Text = "&Copy";
            this.bCopy.Click += new System.EventHandler(this.bCopy_Click);
            // 
            // bPaste
            // 
            this.bPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bPaste.Enabled = false;
            this.bPaste.Image = global::MGEgui.Properties.Resources.paste;
            this.bPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bPaste.Name = "bPaste";
            this.bPaste.Size = new System.Drawing.Size(23, 22);
            this.bPaste.Text = "&Paste";
            this.bPaste.Click += new System.EventHandler(this.bPaste_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // bAsm
            // 
            this.bAsm.Checked = true;
            this.bAsm.CheckState = System.Windows.Forms.CheckState.Checked;
            this.bAsm.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bAsm.Image = global::MGEgui.Properties.Resources.asm;
            this.bAsm.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bAsm.Name = "bAsm";
            this.bAsm.Size = new System.Drawing.Size(23, 22);
            this.bAsm.Text = "Assembler";
            this.bAsm.Click += new System.EventHandler(this.bAsm_Click);
            // 
            // bHex
            // 
            this.bHex.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.bHex.Image = global::MGEgui.Properties.Resources.hex;
            this.bHex.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.bHex.Name = "bHex";
            this.bHex.Size = new System.Drawing.Size(23, 22);
            this.bHex.Text = "HEX view";
            this.bHex.Click += new System.EventHandler(this.bHex_Click);
            // 
            // splitContainer
            // 
            this.splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer.Location = new System.Drawing.Point(0, 49);
            this.splitContainer.Name = "splitContainer";
            // 
            // splitContainer.Panel1
            // 
            this.splitContainer.Panel1.Controls.Add(this.bAddSection);
            this.splitContainer.Panel1.Controls.Add(this.tbNewSection);
            this.splitContainer.Panel1.Controls.Add(this.lbSections);
            this.splitContainer.Panel1MinSize = 112;
            // 
            // splitContainer.Panel2
            // 
            this.splitContainer.Panel2.Controls.Add(this.splitContainerRight);
            this.splitContainer.Panel2MinSize = 120;
            this.splitContainer.Size = new System.Drawing.Size(632, 284);
            this.splitContainer.SplitterDistance = 160;
            this.splitContainer.TabIndex = 2;
            // 
            // bAddSection
            // 
            this.bAddSection.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bAddSection.Location = new System.Drawing.Point(84, 5);
            this.bAddSection.Name = "bAddSection";
            this.bAddSection.Size = new System.Drawing.Size(75, 20);
            this.bAddSection.TabIndex = 2;
            this.bAddSection.Text = "Add";
            this.bAddSection.UseVisualStyleBackColor = true;
            this.bAddSection.Click += new System.EventHandler(this.bAddSection_Click);
            // 
            // tbNewSection
            // 
            this.tbNewSection.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbNewSection.Location = new System.Drawing.Point(3, 5);
            this.tbNewSection.Name = "tbNewSection";
            this.tbNewSection.Size = new System.Drawing.Size(76, 20);
            this.tbNewSection.TabIndex = 0;
            this.tbNewSection.Tag = null;
            this.tbNewSection.TextChanged += new System.EventHandler(this.tbNewSection_TextChanged);
            this.tbNewSection.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbNewSection_KeyPress);
            // 
            // lbSections
            // 
            this.lbSections.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lbSections.FormattingEnabled = true;
            this.lbSections.IntegralHeight = false;
            this.lbSections.Location = new System.Drawing.Point(3, 30);
            this.lbSections.Name = "lbSections";
            this.lbSections.Size = new System.Drawing.Size(157, 251);
            this.lbSections.Sorted = true;
            this.lbSections.TabIndex = 2;
            this.lbSections.SelectedIndexChanged += new System.EventHandler(this.lbSections_SelectedIndexChanged);
            this.lbSections.DoubleClick += new System.EventHandler(this.lbSections_DoubleClick);
            this.lbSections.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbSections_KeyDown);
            // 
            // splitContainerRight
            // 
            this.splitContainerRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerRight.Location = new System.Drawing.Point(0, 0);
            this.splitContainerRight.Name = "splitContainerRight";
            this.splitContainerRight.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerRight.Panel1
            // 
            this.splitContainerRight.Panel1.Controls.Add(this.panelBorder);
            // 
            // splitContainerRight.Panel2
            // 
            this.splitContainerRight.Panel2.Controls.Add(this.splitContainerEditor);
            this.splitContainerRight.Size = new System.Drawing.Size(468, 284);
            this.splitContainerRight.SplitterDistance = 140;
            this.splitContainerRight.TabIndex = 0;
            // 
            // panelBorder
            // 
            this.panelBorder.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelBorder.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panelBorder.Controls.Add(this.propertyGrid);
            this.panelBorder.Location = new System.Drawing.Point(0, 5);
            this.panelBorder.Name = "panelBorder";
            this.panelBorder.Size = new System.Drawing.Size(465, 135);
            this.panelBorder.TabIndex = 0;
            // 
            // propertyGrid
            // 
            this.propertyGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGrid.HelpVisible = false;
            this.propertyGrid.Location = new System.Drawing.Point(-1, -1);
            this.propertyGrid.Name = "propertyGrid";
            this.propertyGrid.Size = new System.Drawing.Size(463, 133);
            this.propertyGrid.TabIndex = 0;
            this.propertyGrid.SelectedGridItemChanged += new System.Windows.Forms.SelectedGridItemChangedEventHandler(this.propertyGrid_SelectedGridItemChanged);
            this.propertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGrid_PropertyValueChanged);
            // 
            // splitContainerEditor
            // 
            this.splitContainerEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerEditor.Location = new System.Drawing.Point(0, 0);
            this.splitContainerEditor.Name = "splitContainerEditor";
            // 
            // splitContainerEditor.Panel1
            // 
            this.splitContainerEditor.Panel1.Controls.Add(this.richTextBoxView);
            // 
            // splitContainerEditor.Panel2
            // 
            this.splitContainerEditor.Panel2.Controls.Add(this.richTextBoxEditor);
            this.splitContainerEditor.Size = new System.Drawing.Size(468, 140);
            this.splitContainerEditor.SplitterDistance = 296;
            this.splitContainerEditor.TabIndex = 1;
            // 
            // richTextBoxView
            // 
            this.richTextBoxView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.richTextBoxView.BackColor = System.Drawing.SystemColors.Window;
            this.richTextBoxView.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxView.HideSelection = false;
            this.richTextBoxView.Location = new System.Drawing.Point(0, 0);
            this.richTextBoxView.Name = "richTextBoxView";
            this.richTextBoxView.ReadOnly = true;
            this.richTextBoxView.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Horizontal;
            this.richTextBoxView.Size = new System.Drawing.Size(296, 137);
            this.richTextBoxView.TabIndex = 0;
            this.richTextBoxView.Text = "";
            this.richTextBoxView.WordWrap = false;
            this.richTextBoxView.VScroll += new System.EventHandler(this.richTextBoxView_VScroll);
            this.richTextBoxView.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.richTextBoxView_MouseWheel);
            this.richTextBoxView.Resize += new System.EventHandler(this.richTextBoxView_Resize);
            this.richTextBoxView.Enter += new System.EventHandler(this.richTextBoxView_Enter);
            this.richTextBoxView.Leave += new System.EventHandler(this.richTextBoxView_Leave);
            // 
            // richTextBoxEditor
            // 
            this.richTextBoxEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.richTextBoxEditor.Enabled = false;
            this.richTextBoxEditor.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.richTextBoxEditor.Location = new System.Drawing.Point(0, 0);
            this.richTextBoxEditor.Name = "richTextBoxEditor";
            this.richTextBoxEditor.Size = new System.Drawing.Size(165, 137);
            this.richTextBoxEditor.TabIndex = 0;
            this.richTextBoxEditor.Text = "";
            this.richTextBoxEditor.WordWrap = false;
            this.richTextBoxEditor.Enter += new System.EventHandler(this.richTextBoxEditor_Enter);
            this.richTextBoxEditor.Leave += new System.EventHandler(this.richTextBoxEditor_Leave);
            this.richTextBoxEditor.MouseDown += new System.Windows.Forms.MouseEventHandler(this.richTextBox_MouseDown);
            this.richTextBoxEditor.TextChanged += new System.EventHandler(this.richTextBox_TextChanged);
            // 
            // openPatch
            // 
            this.openPatch.DefaultExt = "mcp";
            this.openPatch.Filter = "MGE patch file (mcp)|*.mcp";
            this.openPatch.InitialDirectory = "mge3\\";
            this.openPatch.RestoreDirectory = true;
            this.openPatch.Title = "Open patch file";
            // 
            // savePatch
            // 
            this.savePatch.DefaultExt = "mcp";
            this.savePatch.Filter = "MGE patch file (mcp)|*.mcp";
            this.savePatch.InitialDirectory = "mge3\\";
            this.savePatch.RestoreDirectory = true;
            this.savePatch.Tag = "New Patch.mcp";
            this.savePatch.Title = "Save patch";
            // 
            // contextStrip
            // 
            this.contextStrip.Name = "contextStrip";
            this.contextStrip.Size = new System.Drawing.Size(61, 4);
            this.contextStrip.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.contextStrip_ItemClicked);
            // 
            // PatchEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(632, 333);
            this.Controls.Add(this.splitContainer);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.menuStrip);
            this.Icon = global::MGEgui.Properties.Resources.AppIcon;
            this.MainMenuStrip = this.menuStrip;
            this.MinimumSize = new System.Drawing.Size(400, 300);
            this.Name = "PatchEditorForm";
            this.Tag = "";
            this.Text = "PatchEditor";
            this.Load += new System.EventHandler(this.PatchEditorForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PatchEditorForm_FormClosing);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.splitContainer.Panel1.ResumeLayout(false);
            this.splitContainer.Panel1.PerformLayout();
            this.splitContainer.Panel2.ResumeLayout(false);
            this.splitContainer.ResumeLayout(false);
            this.splitContainerRight.Panel1.ResumeLayout(false);
            this.splitContainerRight.Panel2.ResumeLayout(false);
            this.splitContainerRight.ResumeLayout(false);
            this.panelBorder.ResumeLayout(false);
            this.splitContainerEditor.Panel1.ResumeLayout(false);
            this.splitContainerEditor.Panel2.ResumeLayout(false);
            this.splitContainerEditor.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem tsmiFile;
        private System.Windows.Forms.ToolStripMenuItem tsmiNew;
        private System.Windows.Forms.ToolStripMenuItem tsmiOpen;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem tsmiSave;
        private System.Windows.Forms.ToolStripMenuItem tsmiSaveAs;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem tsmiEexit;
        private System.Windows.Forms.ToolStripMenuItem tsmiEdit;
        private System.Windows.Forms.ToolStripMenuItem tsmiUndo;
        private System.Windows.Forms.ToolStripMenuItem tsmiRedo;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem tsmiCut;
        private System.Windows.Forms.ToolStripMenuItem tsmiCopy;
        private System.Windows.Forms.ToolStripMenuItem tsmiPaste;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem tsmiSelect;
        private System.Windows.Forms.ToolStripMenuItem tsmiTools;
        private System.Windows.Forms.ToolStripMenuItem customizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tsmiHelp;
        private System.Windows.Forms.ToolStripMenuItem contentsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem indexToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem searchToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton bNew;
        private System.Windows.Forms.ToolStripButton bOpen;
        private System.Windows.Forms.ToolStripButton bSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripButton bCut;
        private System.Windows.Forms.ToolStripButton bAsm;
        private System.Windows.Forms.ToolStripButton bHex;
        private System.Windows.Forms.ToolStripButton bCopy;
        private System.Windows.Forms.ToolStripButton bPaste;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.SplitContainer splitContainer;
        private System.Windows.Forms.TextBox tbNewSection;
        private System.Windows.Forms.Button bAddSection;
        private System.Windows.Forms.ListBox lbSections;
        private System.Windows.Forms.SplitContainer splitContainerRight;
        private System.Windows.Forms.PropertyGrid propertyGrid;
        private System.Windows.Forms.RichTextBox richTextBoxEditor;
        private System.Windows.Forms.Panel panelBorder;
        private System.Windows.Forms.OpenFileDialog openPatch;
        private System.Windows.Forms.SaveFileDialog savePatch;
        private System.Windows.Forms.ContextMenuStrip contextStrip;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton bUndo;
        private System.Windows.Forms.ToolStripButton bRedo;
        private System.Windows.Forms.SplitContainer splitContainerEditor;
        private System.Windows.Forms.RichTextBox richTextBoxView;
        private System.Windows.Forms.ToolStripMenuItem tsmiPatchByFile;

    }
}