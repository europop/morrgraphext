using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MGEgui {
    public partial class DLRipplesForm : Form {
        public DLRipplesForm () {
            InitializeComponent ();
        }

        private void bOK_Click (object sender, EventArgs e) {
            Close ();
        }
    }
}
