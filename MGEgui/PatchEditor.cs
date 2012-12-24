using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MGEgui.Patching;

namespace MGEgui {
    public partial class PatchEditorForm : Form {
        public PatchEditorForm() {
            InitializeComponent();
            (this.propertyGrid.Controls[3] as System.Windows.Forms.ToolStrip).Items.AddRange(new System.Windows.Forms.ToolStripItem[] { new System.Windows.Forms.ToolStripSeparator(), this.bAsm, this.bHex});
        }
        public Char[] illegal = new Char[] { '\\', '/', '\t', '\n', '\r', '[', ']', ';' };
        private int currentState = 0;
        private Dictionary<String, Byte[]> BinaryFiles = new Dictionary<String, Byte[]>();
        private Dictionary<String, Patch> currentSections = new Dictionary<String, Patch>();
        private List<Dictionary<String, Patch>> history = new List<Dictionary<String, Patch>>();

        private Dictionary<String, Patch> CopyDictionary(Dictionary<String, Patch> dictionary) {
            Dictionary<String, Patch> copy = new Dictionary<String,Patch>();
            foreach (String key in dictionary.Keys) { copy.Add(key, dictionary[key].Copy()); }
            return copy;
        }
        public Patch GetSelectedPatch() {
            return ((Patch)propertyGrid.SelectedObject);
        }
        public void AddBinaryFile(String fileName) {
            String name = fileName.ToLower();
            if (!BinaryFiles.ContainsKey(name)) {
                BinaryFiles.Add(name, new Byte[] { });
                if(File.Exists(fileName)) {
                    BinaryReader binReader = new BinaryReader(File.Open(fileName, FileMode.Open));
                    if(binReader != null) {
                        byte[] buffer = new byte[binReader.BaseStream.Length];
                        int count = binReader.Read(buffer, 0, buffer.Length);
                        binReader.Close();
                        BinaryFiles[name] = buffer;
                    }
                }
            }
        }
        private String[] GetAttachedNames(Patch section) {
            List<String> names = new List<String>();
            if (section != null) {
                foreach (Patch patch in currentSections.Values) {
                    if (section.Parent.StartsWith(patch.Parent)) {
                        if (patch.Attach != null) foreach (Unit attachment in patch.Attach) names.Add(attachment.Name);
                    }
                }
            }
            names.Sort();
            return names.ToArray();
        }
        private void EnableCutCopyPaste(bool value) {
            tsmiCut.Enabled = bCut.Enabled = tsmiCopy.Enabled = bCopy.Enabled = tsmiPaste.Enabled = bPaste.Enabled = tsmiSelect.Enabled = value;
        }
        private void DisableEditor() {
            richTextBoxView.Tag = null;
            splitContainerEditor.Panel1Collapsed = true;
            richTextBoxEditor.Tag = null; // before editing
            richTextBoxEditor.Text = "";
            richTextBoxEditor.Enabled = false;
        }
        private uint HexViewerColumns = 8;
        private uint HexViewerAddress = 0;
        private void FillHexViewer(uint address) {
            String name = GetSelectedPatch().GetParentFile().ToLower();
            uint instance = GetSelectedPatch().GetDefaultInstance();
            if (address < instance) { richTextBoxView.Clear(); return; }
            String view = "";
            String line = "";
            int SelectionStart = richTextBoxView.SelectionStart;
            int SelectionLength = richTextBoxView.SelectionLength;
            uint columns = (uint)((richTextBoxView.ClientRectangle.Width / (TextRenderer.MeasureText("00000000", richTextBoxView.Font).Width / 8f - 0.75f) - 10) / 24f);
            HexViewerColumns = columns = 8 * (columns > 0 ? columns : 1);
            int rows = (int)(richTextBoxView.ClientRectangle.Height / (float)TextRenderer.MeasureText("00000000", richTextBoxView.Font).Height);
            byte[] bFile = BinaryFiles[name];
            List<int> LLength = new List<int>();
            for (uint i = ((address - instance) / columns) * columns; i < bFile.Length; i++) {
                uint remainder = i % columns;
                if (remainder == 0) {
                    view += line;
                    LLength.Add(view.Length);
                    if (--rows < 0) { line = ""; break; }
                    line = String.Format("{0:X8}:", i + instance);
                }
                line += String.Format(" {0:X2}" + (++remainder == columns ? rows == 0 ? "" : "\n" : remainder % 8 == 0 ? " " : ""), bFile[i]);
            }
            view += line;
            LLength.Add(view.Length);
            richTextBoxView.Text = "";
            richTextBoxView.Text = view;
            for(int i=0; i<LLength.Count; i++) {
                richTextBoxView.Select(LLength[i], 9);
                richTextBoxView.SelectionColor = Color.Blue;
            }
        }
        private void richTextBoxView_MouseWheel(object sender, MouseEventArgs e) {
            if (richTextBoxView.Tag != null) {
                uint address = HexViewerAddress - (uint)(HexViewerColumns * (e.Delta / 40));
                address = (address < GetSelectedPatch().GetDefaultInstance() ? GetSelectedPatch().GetDefaultInstance() : address);
                if (HexViewerAddress != address) FillHexViewer(HexViewerAddress = address);
            }
        }
        private void HistoryUpdate() {
            if (currentState < history.Count) history.RemoveRange(currentState, history.Count - currentState);
            history.Add(CopyDictionary(currentSections));
            currentState = history.Count;
            tsmiUndo.Enabled = bUndo.Enabled = tsmiSave.Enabled = bSave.Enabled = currentState > 1;
            tsmiRedo.Enabled = bRedo.Enabled = false;
        }
        private void tsmiUndoRedo_Click(object sender, EventArgs e) {
            Object selected = lbSections.SelectedItem;
            if (((sender == tsmiUndo || sender == bUndo) && currentState > 1 ) || ((sender == tsmiRedo || sender == bRedo) && currentState < history.Count )) {
                if (sender == tsmiUndo || sender == bUndo) currentState--; else currentState++;
                tsmiUndo.Enabled = bUndo.Enabled = tsmiSave.Enabled = bSave.Enabled = currentState > 1;
                tsmiRedo.Enabled = bRedo.Enabled = currentState < history.Count;
                currentSections = CopyDictionary(history[currentState - 1]);
                lbSections.Items.Clear();
                foreach (String section in currentSections.Keys) { lbSections.Items.Add(section); }
                lbSections.SelectedItem = selected;
                if (lbSections.SelectedIndex < 0) { propertyGrid.SelectedObject = null; DisableEditor(); }
                if (propertyGrid.SelectedGridItem == null ? false : (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Attach]) || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Original])
                    || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Patch]) || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Description]))
                    propertyGrid_SelectedGridItemChanged(propertyGrid, new SelectedGridItemChangedEventArgs(propertyGrid.SelectedGridItem, propertyGrid.SelectedGridItem));
            }
        }
        private bool SaveFile(String fileName) {
            try { File.Delete(fileName); } catch { return false; }
            List<INIFile.INIVariableDef> defmcp = new List<INIFile.INIVariableDef>();
            List<String[]> comments = new List<String[]>(); //  0 - section, 1 - key, 2 - comment above
            defmcp.Add(INIFile.iniDefEmpty);
            foreach (String section in currentSections.Keys) {
                Patch patch = currentSections[section];
                if (patch.Checked != new Patch().Checked)
                    defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, Patch.Keys[Patch.Key.Checked], INIFile.INIVariableType.Dictionary, patch.Checked.ToString()));
                defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, Patch.Keys[Patch.Key.Version], INIFile.INIVariableType.Dictionary, patch.Version.ToString()));
                Dictionary<Patch.Key, String> str = new Dictionary<Patch.Key, String> { { Patch.Key.Parent, patch.Parent }, { Patch.Key.Author, patch.Author } };
                foreach (Patch.Key key in str.Keys) if (str[key].Trim().Length > 0)
                    defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, Patch.Keys[key], INIFile.INIVariableType.Dictionary, str[key].Trim()));
                if (patch.FileVersion != new Patch().FileVersion)
                    defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, Patch.Keys[Patch.Key.FileVersion], INIFile.INIVariableType.Dictionary, patch.FileVersion.ToString()));
                Dictionary<Patch.Key, Unit[]> array = new Dictionary<Patch.Key, Unit[]> { { Patch.Key.Original, patch.Original }, { Patch.Key.Patch, patch.Patched }, { Patch.Key.Attach, patch.Attach }, { Patch.Key.Description, patch.Description } };
                foreach (Patch.Key key in array.Keys) if (array[key] != null) foreach (Unit unit in array[key]) {
                    String[] asm_lines = unit.Asm.Split(new Char[] { '\n' });
                    String[] hex_lines = unit.Hex.Split(new Char[] { '\n' });
                    String hex = "";
                    int comment = 0;
                    foreach (String line in hex_lines) hex += (hex.Length > 0 ? "\t" : "") + ((comment = line.IndexOf(INIFile.INIComment)) < 0 ? line : line.Substring(0, comment)).TrimEnd(null);
                    if (hex.Trim().Length == 0) continue;
                    String variable = Patch.Keys[key] + " " + unit.ToString();
                    for (int i = 0; i < asm_lines.Length; i++) if (i == asm_lines.Length - 1 ? asm_lines[i].Trim().Length > 0 : true) comments.Add(new String[3] { section, variable, asm_lines[i] });
                    defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, variable, INIFile.INIVariableType.Dictionary, hex));
                }
                if (patch.Removed != new Patch().Removed)
                    defmcp.Add(new INIFile.INIVariableDef(defmcp.Count.ToString(), section, Patch.Keys[Patch.Key.Removed], INIFile.INIVariableType.Dictionary, patch.Removed.ToString()));
            }
            INIFile mcpFile = new INIFile(fileName, defmcp.ToArray(), true);
            mcpFile.initialize();
            foreach (String[] triplet in comments) mcpFile.setCommentAbove(triplet[0], triplet[1], triplet[2]);
            return mcpFile.save();
        }
        private bool SaveChanges(bool message) {
            RestoreButton();
            if (!bSave.Enabled) return true;
            bool dialog = savePatch.FileName == "";
            if (dialog) savePatch.FileName = (String)savePatch.Tag;
            DialogResult result = message ? MessageBox.Show("Save changes to " + savePatch.FileName, "Save changes?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning) : DialogResult.OK;
            if (result == DialogResult.Cancel) return false;
            if (result != DialogResult.No) {
                if (dialog ? savePatch.ShowDialog() == DialogResult.Cancel : false) { savePatch.FileName = ""; return false; }
                if (!SaveFile(savePatch.FileName)) return false;
            } else {
                bSave.Enabled = false;
                bNew_Click();   
            }
            currentState = 0;
            HistoryUpdate();
            return true;
        }
        private void tsmiSaveAs_Click(object sender, EventArgs e) {
            String fileName = savePatch.FileName;
            if (savePatch.ShowDialog() != DialogResult.Cancel) SaveFile(savePatch.FileName);
            savePatch.FileName = fileName;
        }
        public String SelectExecutableFile(String title) {
            String currentDirectory = Environment.CurrentDirectory;
            OpenFileDialog openParent = new OpenFileDialog();
            openParent.Title = title;
            openParent.Filter = "Executable files (EXE & DLL)|*.exe;*.dll";
            openParent.InitialDirectory = "mge3\\..";
            if (openParent.ShowDialog() != DialogResult.Cancel) {
                Environment.CurrentDirectory = currentDirectory;
                return openParent.FileName;
            }
            return null;
        }
        private void PatchEditorForm_Load(object sender, EventArgs e) {
            bNew_Click();
        }
        private void bNew_Click(object sender, EventArgs e) {
            bNew_Click();
        }
        private bool bNew_Click() {
            if (!SaveChanges(true)) return false;
            propertyGrid.SelectedObject = null;
            currentSections.Clear();
            lbSections.Items.Clear();
            currentState = 0;
            HistoryUpdate();
            savePatch.FileName = "";
            DisableEditor();
            RestoreButton();
            return true;
        }
        private void bSave_Click(object sender, EventArgs e) { SaveChanges(false); }
        private void PatchEditorForm_FormClosing(object sender, FormClosingEventArgs e) { 
            if (!SaveChanges(true)) e.Cancel = true;
        }
        private void tsmiEexit_Click(object sender, EventArgs e) { Close(); }
        private void bOpen_Click(object sender, EventArgs e) {
            RestoreButton();
            if (!SaveChanges(true)) return;
            if (openPatch.ShowDialog() != DialogResult.OK) return;
            INIFile mcpFile = new INIFile(openPatch.FileName, new INIFile.INIVariableDef[] { INIFile.iniDefEmpty });
            String[] sections = mcpFile.getSections();
            if (sections.Length > 0) {
                currentSections.Clear();
                lbSections.Items.Clear();
            }
            foreach (String section in sections) {
                currentSections.Add(section, new Patch(mcpFile, section));
                lbSections.Items.Add(section);
            }
            propertyGrid.SelectedObject = null;
            currentState = 0;
            HistoryUpdate();
            savePatch.FileName = openPatch.FileName;
            DisableEditor();
        }
        private void lbSections_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyData == Keys.Delete && lbSections.SelectedItem != null) {
                propertyGrid.SelectedObject = null;
                currentSections.Remove(lbSections.SelectedItem.ToString());
                lbSections.Items.Remove(lbSections.SelectedItem);
                HistoryUpdate();
            }
        }
        private void lbSections_SelectedIndexChanged(object sender, EventArgs e) {
            if (tbNewSection.Tag != null) {
                RestoreButton();
            }
            if (lbSections.SelectedItem != null) {
                propertyGrid.SelectedObject = currentSections[(String)lbSections.SelectedItem];
                AddBinaryFile(GetSelectedPatch().GetParentFile());
            }
            if (propertyGrid.SelectedGridItem == null ? false : (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Attach]) || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Original])
                || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Patch]) || (propertyGrid.SelectedGridItem.Label.Trim() == Patch.Keys[Patch.Key.Description]))
                propertyGrid_SelectedGridItemChanged(propertyGrid, new SelectedGridItemChangedEventArgs(propertyGrid.SelectedGridItem, propertyGrid.SelectedGridItem));
            else {
                DisableEditor();
                EnableCutCopyPaste(false);
            }
        }
        private void propertyGrid_SelectedGridItemChanged(object sender, SelectedGridItemChangedEventArgs e) {
            Patch patch = GetSelectedPatch();
            String label = e.NewSelection.Label;
            int index = label.Length - label.Trim().Length - 1;
            Unit[] array = label.Trim() != Patch.Keys[Patch.Key.Original] ? label.Trim() != Patch.Keys[Patch.Key.Attach] ? 
                           label.Trim() != Patch.Keys[Patch.Key.Patch] ? null : patch.Patched : patch.Attach : patch.Original;
            if (label.Trim() == Patch.Keys[Patch.Key.Description]) {
                array = patch.Description;
                String language = ((String)e.NewSelection.Value).Trim();
                if (language != "") for (int i = 0; i < array.Length; i++) if (array[i].Name == language) { index = i; break; }
            }
            if ((array != null) && (index >= 0)) {
                if (array == patch.Original || array == patch.Patched) {
                    splitContainerEditor.Panel1Collapsed = false;
                    String name = GetSelectedPatch().Parent.Split(new Char[] { '\\' })[0].ToLower();
                    richTextBoxView.Clear();
                    richTextBoxView.Tag = HexViewerAddress = array[index].Address;
                    FillHexViewer(HexViewerAddress);
                }
                else splitContainerEditor.Panel1Collapsed = true;
                if (label.Trim() == Patch.Keys[Patch.Key.Description]) bHex_Click(null, null);
                else if (richTextBoxEditor.Tag != array[index]) { if (array[index].Asm.Trim().Length > 0) bAsm_Click(null, null); else bHex_Click(null, null); }
                richTextBoxEditor.Tag = null; // before editing
                richTextBoxEditor.Text = bAsm.Checked ? array[index].Asm : array[index].Hex;
                richTextBoxEditor.Tag = array[index];
                richTextBoxEditor.Enabled = true;
                return;
            }
            DisableEditor();
            EnableCutCopyPaste(false);
        }
        private void richTextBox_TextChanged(object sender, EventArgs e) {
            if (richTextBoxEditor.Tag != null) {
                Unit unit = (Unit)richTextBoxEditor.Tag;
                if (bAsm.Checked) unit.Asm = richTextBoxEditor.Text; else unit.Hex = richTextBoxEditor.Text;
                HistoryUpdate();
            }
        }
        private void bAddSection_Click(object sender, EventArgs e) {
            String section = tbNewSection.Text.Trim();
            if (section.Length == 0) return;
            bool contains = false;  // Instead of non case sensitive lbSections.Items.Contains(section)
            foreach (String name in lbSections.Items) if (name.ToLower() == section.ToLower()) { contains = true; section = name; break; }
            if (!contains) {
                Patch patch;
                if (tbNewSection.Tag != null) {
                    patch = currentSections[(String)lbSections.SelectedItem];
                    currentSections.Remove(lbSections.SelectedItem.ToString());
                    lbSections.Items.Remove(lbSections.SelectedItem);
                    RestoreButton();
                } else patch = new Patch("Morrowind.exe\\Misc");
                lbSections.Items.Add(section);
                currentSections.Add(section, patch);
                HistoryUpdate();
            }
            lbSections.SelectedItem = section;
            tbNewSection.Text = "";
        }
        private void tbNewSection_KeyPress(object sender, KeyPressEventArgs e) {
            if (e.KeyChar == '\r') { bAddSection_Click(null, null); e.Handled = true; }
            foreach (Char symbol in illegal) { if (e.KeyChar == symbol) { e.Handled = true; break; }}
        }
        private void RestoreButton() {
            tbNewSection.Tag = null;
            tbNewSection.Text = "";
            bAddSection.Text = "Add";
            bAddSection.Font = Button.DefaultFont;
        }
        private void tbNewSection_TextChanged(object sender, EventArgs e) {
            if (tbNewSection.Tag != null && tbNewSection.Text == "") {
                RestoreButton();
            }
            int index;
            if ((index = tbNewSection.Text.IndexOfAny(illegal)) >= 0) tbNewSection.Text = tbNewSection.Text.Remove(index, 1);
        }
        private void propertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e) {            
            if ((e.ChangedItem.Label == Patch.Keys[Patch.Key.Attach]) || (e.ChangedItem.Label == Patch.Keys[Patch.Key.Original]) || (e.ChangedItem.Label == Patch.Keys[Patch.Key.Patch])) {
                List<Unit> units = e.OldValue == null ? new List<Unit>() : new List<Unit>((Unit[])e.OldValue);
                if (units.Count != ((Unit[])e.ChangedItem.Value).Length)
                    foreach(GridItem item in e.ChangedItem.GridItems)
                        if (units.Find(delegate(Unit unit) { return unit == item.Value; }) == null) {
                            e.ChangedItem.Expanded = true; propertyGrid.SelectedGridItem = item; break;
                        }
                if (((Unit[])e.ChangedItem.Value).Length != e.ChangedItem.GridItems.Count) {
                    Object selected = propertyGrid.SelectedObject;
                    propertyGrid.SelectedObject = null; // rebuilds collapsed indexes
                    propertyGrid.SelectedObject = selected;
                }
            }
            if (e.ChangedItem.Label == Patch.Keys[Patch.Key.Description]) propertyGrid_SelectedGridItemChanged(s, new SelectedGridItemChangedEventArgs(e.ChangedItem, e.ChangedItem));
            if (e.ChangedItem.Label == Patch.Keys[Patch.Key.Version]) if ((float)e.OldValue == (float)e.ChangedItem.Value) return;
            if (e.ChangedItem.Label == Patch.Keys[Patch.Key.Checked] || e.ChangedItem.Label == Patch.Keys[Patch.Key.Removed]) if ((bool)e.OldValue == (bool)e.ChangedItem.Value) return;
            HistoryUpdate();
        }

        private void richTextBox_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Right) {
                contextStrip.Items.Clear();
                Patch patch = GetSelectedPatch();
                if (patch != null ? patch.Description.Length > 0 ? patch.Description[0] != richTextBoxEditor.Tag : true : false) {
                    String[] names = GetAttachedNames(patch);   
                    foreach (String name in names) contextStrip.Items.Add(name);
                }
                contextStrip.Show(e.X + splitContainer.Panel2.Left + this.Left + this.Cursor.Size.Width/2, e.Y + menuStrip.Height + toolStrip.Height + splitContainerRight.Panel2.Top + this.Top);
            }
        }

        private void contextStrip_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {
            richTextBoxEditor.SelectedText = e.ClickedItem.Text;
        }

        private void bCut_Click(object sender, EventArgs e) {
            richTextBoxEditor.Cut();
        }

        private void bCopy_Click(object sender, EventArgs e) {
            if(bCut.Enabled) richTextBoxEditor.Copy();
            else richTextBoxView.Copy();
        }

        private void bPaste_Click(object sender, EventArgs e) {
            richTextBoxEditor.Paste();
        }

        private void tsmiSelect_Click(object sender, EventArgs e) {
            richTextBoxEditor.Focus();
            richTextBoxEditor.SelectAll();
        }

        private void richTextBoxView_Resize(object sender, EventArgs e)
        {
            if (richTextBoxView.Tag != null) FillHexViewer(HexViewerAddress);
           //richTextBoxView.Text = Math.Round((richTextBoxView.ClientRectangle.Width / (TextRenderer.MeasureText("00000000", richTextBoxView.Font).Width / 8f - 1f) - 10) / 24f).ToString();
        }

        private void richTextBoxView_VScroll(object sender, EventArgs e)
        {
            //richTextBoxView.Sc
        }

        private void bAsm_Click(object sender, EventArgs e) {
            if (bHex.CheckState != CheckState.Unchecked) {
                bAsm.CheckState = CheckState.Checked;
                bHex.CheckState = CheckState.Unchecked;
                propertyGrid_SelectedGridItemChanged(null, new SelectedGridItemChangedEventArgs(propertyGrid.SelectedGridItem, propertyGrid.SelectedGridItem));
            }
        }

        private void bHex_Click(object sender, EventArgs e) {
            if (bAsm.CheckState != CheckState.Unchecked) {
                bAsm.CheckState = CheckState.Unchecked;
                bHex.CheckState = CheckState.Checked;
                propertyGrid_SelectedGridItemChanged(null, new SelectedGridItemChangedEventArgs(propertyGrid.SelectedGridItem, propertyGrid.SelectedGridItem));
            }
        }

        private void tsmiPatchByFile_Click(object sender, EventArgs e) {
            if (!bNew_Click()) return;
            String original = SelectExecutableFile("Open Original file for comparison...");
            if (original == null) return;
            AddBinaryFile(original);
            this.Text = original;
            String patched;
            do {
                patched = SelectExecutableFile("Open Patched file for comparison...");
                if (patched == null) return;
            } while (patched == original);
            AddBinaryFile(patched);
            this.Text = patched;
            byte[] bOriginal = BinaryFiles[original.ToLower()];
            byte[] bPatched = BinaryFiles[patched.ToLower()];
            if(bOriginal.Length != bPatched.Length) {
                MessageBox.Show("Files for comparison must be equal in size!", Statics.strings["Warning"].text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.Text = "";
                return;
            }
            String section = patched.Substring(patched.LastIndexOf("\\") + 1).ToLower().Replace(".", "_");
            foreach (Char c in illegal) section = section.Replace(c, '_');
            Patch patch = new Patch(original.Substring(original.LastIndexOf("\\")+1) + "\\Difference");
            List<Unit> uOriginal = new List<Unit>();
            List<Unit> uPatch = new List<Unit>();
            uint address = 0;    // We must start after PE header
            for (uint i = 1; i < bOriginal.Length; i++ ) {
                if (bOriginal[i] == bPatched[i]) address = 0;
                else {
                    if (address == 0) {
                        address = i;
                        uOriginal.Add(new Unit(address));
                        uPatch.Add(new Unit(address));
                    }
                    uOriginal[uOriginal.Count - 1].Hex += String.Format("{0:X2} ", bOriginal[i]);
                    uPatch[uPatch.Count - 1].Hex += String.Format("{0:X2} ", bPatched[i]);
                }
            }
            patch.Original = uOriginal.ToArray();
            patch.Patched = uPatch.ToArray();
            if(uOriginal.Count > 0) {
                lbSections.Items.Add(section);
                currentSections.Add(section, patch);
                lbSections.SelectedIndex = lbSections.Items.Count - 1;
                HistoryUpdate();
            } else {
                MessageBox.Show("Selected files are identical.", Statics.strings["Message"].text, MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            BinaryFiles.Remove(original.ToLower());
            BinaryFiles.Remove(patched.ToLower());
        }

        private void richTextBoxView_Enter(object sender, EventArgs e) {
            tsmiCopy.Enabled = bCopy.Enabled = true;
        }

        private void richTextBoxView_Leave(object sender, EventArgs e) {
            EnableCutCopyPaste(false);
        }

        private void richTextBoxEditor_Enter(object sender, EventArgs e) {
            EnableCutCopyPaste(true);
        }

        private void richTextBoxEditor_Leave(object sender, EventArgs e) {
            EnableCutCopyPaste(false);
        }

        private void lbSections_DoubleClick(object sender, EventArgs e) {
            if(lbSections.SelectedIndex >= 0) {
                tbNewSection.Text = (String)lbSections.SelectedItem;
                tbNewSection.Tag = lbSections.SelectedIndex;
                bAddSection.Text = "Edit";
                bAddSection.Font = new Font(Button.DefaultFont, FontStyle.Bold);
            }
        }
    }
}
