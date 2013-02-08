using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using System.IO;

namespace MGEgui.DistantLand {

    public partial class DirectoriesForm : Form {

        private List<string> dirs;
        private string datafiles;

        public DirectoriesForm(string datafiles, List<string> dirs) {
            InitializeComponent();
            Statics.Localizations.Apply(this);

            this.folderAdd.SelectedPath = Statics.runDir + "\\Data files";
            this.datafiles = datafiles;
            this.dirs = dirs;
            for (int i = 0; i < dirs.Count; i++ )
                if (String.Compare(Path.GetFullPath(dirs[i]), Path.GetFullPath(datafiles), true, Statics.Culture) == 0)
                    dirs.RemoveAt(i--);
            lbDirectories.Items.AddRange(dirs.ToArray());
        }

        private void bClear_Click(object sender, EventArgs e) {
            lbDirectories.Items.Clear();
            lbDirectories.Items.Insert(0, Statics.fn_statics);
        }

        private void bAdd_Click(object sender, EventArgs e) {
            if (folderAdd.ShowDialog() != DialogResult.OK) return;
            if (String.Compare(folderAdd.SelectedPath, Path.GetFullPath(Statics.fn_statics), true, Statics.Culture) == 0 ||
                String.Compare(folderAdd.SelectedPath, Path.GetFullPath(datafiles), true, Statics.Culture) == 0) return;
            foreach (String dirname in lbDirectories.Items)
                if (String.Compare(folderAdd.SelectedPath, Path.GetFullPath(dirname), true, Statics.Culture) == 0) return;
            String[] items = new String[lbDirectories.Items.Count];
            lbDirectories.Items.CopyTo(items, 0);
            lbDirectories.Items.Clear();
            List<String> dirslist = new List<String>(items);
            dirslist.Add(folderAdd.SelectedPath);
            for (int i = 0; i < dirslist.Count; i++)
                if (String.Compare(Path.GetFullPath(dirslist[i]), Path.GetFullPath(Statics.fn_statics), true, Statics.Culture) == 0)
                    dirslist.RemoveAt(i--);
            dirslist.Sort();
            dirslist.Insert(0, Statics.fn_statics);
            lbDirectories.Items.AddRange(dirslist.ToArray());
        }

        private void bRemove_Click(object sender, EventArgs e) {
            int index = lbDirectories.SelectedIndex;
            if (index != -1) {
                if (String.Compare(Path.GetFullPath((String)lbDirectories.Items[index]), Path.GetFullPath(Statics.fn_statics), true, Statics.Culture) == 0)
                    MessageBox.Show("You can't remove the preset path.", "Warning");
                else lbDirectories.Items.RemoveAt(index);
            }
        }

        private void bSave_Click(object sender, EventArgs e) {
            dirs.Clear();
            foreach (string s in lbDirectories.Items) dirs.Add(s);
            Close();
        }

    }

}
