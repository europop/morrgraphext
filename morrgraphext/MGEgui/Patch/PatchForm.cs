using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MGEgui.Patching;
using MGEgui.INI;

namespace MGEgui {
    public partial class PatchForm : Form {
        public PatchForm(String[] languagesEng) {
            Languages = languagesEng;
            InitializeComponent();
        }
        private String[] Languages;
        private String patchSettings = "mge3/PatchSettings";
        private bool loaded = false;
        private void RemoveEmptyNodes(TreeNodeCollection nodes) {
            for (int i = 0; i < nodes.Count; i++) {
                RemoveEmptyNodes(nodes[i].Nodes);
                Patch patch = (Patch)nodes[i].Tag;
                if (nodes[i].Nodes.Count == 0 && (patch == null ? true : patch.Original == null && patch.Patched == null && patch.Attach == null)
                    || (patch == null ? false : patch.Removed)) nodes[i--].Remove();
            }
        }
        private void UpdateColour(TreeNodeCollection nodes, bool Checked) {
            foreach (TreeNode node in nodes) {
                node.ForeColor = Checked && node.Checked ? TreeView.DefaultForeColor : Color.Silver;
                UpdateColour(node.Nodes, Checked && node.Checked);
            }
        }
        private void ExpandNodes(TreeNodeCollection nodes) {
            foreach (TreeNode node in nodes) {
                Patch patch = (Patch)node.Tag;
                if (patch != null ? patch.Expanded : true) node.Expand();
                else node.Collapse();
                ExpandNodes(node.Nodes);
            }
        }
        private void SaveNodes(TreeNodeCollection nodes, List<INIFile.INIVariableDef> list) {
            foreach (TreeNode node in nodes) {
                Dictionary<Patch.Key, String> str = new Dictionary<Patch.Key, String> { { Patch.Key.Checked, node.Checked.ToString() }, { Patch.Key.Version, "3E33" } };
                if (!node.IsExpanded && node.Nodes.Count > 0) str.Add(Patch.Key.Expanded, node.IsExpanded.ToString());
                if (node.FullPath.LastIndexOf(node.Text) > 0) str.Add(Patch.Key.Parent, Patch.RebuildParentString(node.FullPath.Substring(0, node.FullPath.LastIndexOf(node.Text) - 1)));
                foreach (Patch.Key key in str.Keys)
                    list.Add(new INIFile.INIVariableDef(list.Count.ToString(), node.Text, Patch.Keys[key], INIFile.INIVariableType.Dictionary, str[key]));
                SaveNodes(node.Nodes, list);
            }
        }
        private void tvPatch_BeforeEditing(object sender, TreeViewCancelEventArgs e) { if (loaded) try { File.Delete(patchSettings); } catch { e.Cancel = true; return; } }
        private void tvPatch_AfterEditing(object sender, TreeViewEventArgs e) {
            if (!loaded) return;
            UpdateColour(tvPatch.Nodes, true);
            List<INIFile.INIVariableDef> guimcp = new List<INIFile.INIVariableDef> { INIFile.iniDefEmpty };
            SaveNodes(tvPatch.Nodes, guimcp);
            INIFile mcpFile = new INIFile(patchSettings, guimcp.ToArray(), true);
            mcpFile.reinitialize();
            mcpFile.save();
        }
        private void PatchForm_Load(object sender, EventArgs e) {
            tvPatch.Nodes.Clear();
            List<String> files = new List<String>(Directory.GetFiles("mge3", "*.mcp", SearchOption.AllDirectories));
            files.Add(patchSettings); // last of all
            foreach (String file in files) {
                INIFile mcpFile = new INIFile(file, new INIFile.INIVariableDef[] { INIFile.iniDefEmpty }, Encoding.Default);
                String[] sections = mcpFile.getSections();
                foreach (String Section in sections) {
                    Patch patch = new Patch(mcpFile, Section);
                    String[] parents = patch.Parent.Split(new String[] { Patch.SepInternal.ToString() }, StringSplitOptions.RemoveEmptyEntries);
                    TreeNodeCollection nodes = tvPatch.Nodes;
                    TreeNode[] exist;
                    for (int i = 0; i < parents.Length; i++) {
                        exist = nodes.Find(parents[i].ToLower(), false);
                        if (exist.Length > 0) nodes = exist[0].Nodes;
                        else {
                            TreeNode node = nodes.Add(parents[i].ToLower(), parents[i]);
                            node.Checked = true;
                            nodes = node.Nodes;
                        }
                    }
                    exist = nodes.Find(Section.ToLower(), false);
                    if (exist.Length > 0) {
                        Patch existing = (Patch)exist[0].Tag;
                        bool update = existing == null;
	                    if (!update) {
		                    if (patch.Version <= existing.Version) continue;
                            if (patch.Original == null && patch.Patched == null && patch.Attach == null) {
                                exist[0].Checked = existing.Checked = patch.Checked;
                                existing.Removed = patch.Removed || existing.Removed;
                                existing.Expanded = patch.Expanded;
                                continue;
                            }
		                    update = true;
	                    }
	                    if (update) {
                            exist[0].Checked = patch.Checked;
                            exist[0].Tag = patch;
                            continue;
	                    }
                    } else {
                        TreeNode node = nodes.Add(Section.ToLower(), Section);
                        node.Tag = patch;
                        node.Checked = patch.Checked;
                        node.ToolTipText = "";
                        foreach(String language in Languages)
                            if (node.ToolTipText == "")
                                foreach(Unit description in patch.Description)
                                    if (language == description.Name) { node.ToolTipText = description.Hex.Replace("\t", "\n"); break; }
                    }
                }
            }
            RemoveEmptyNodes(tvPatch.Nodes);
            UpdateColour(tvPatch.Nodes, true);
            ExpandNodes(tvPatch.Nodes);
            tvPatch.Sort();
            loaded = true;
        }
    }
}
