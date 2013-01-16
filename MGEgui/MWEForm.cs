using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace MGEgui
{
    public partial class MWEForm : Form
    {
        public MWEForm()
        {
            InitializeComponent();
        }

        private RegistryKey mwekey;

        [DllImport("user32.dll")]
        private static extern uint MapVirtualKey(uint uCode, uint uMapType);

        [DllImport("user32.dll")]
        private static extern short GetAsyncKeyState(Keys vKey);

        private void MWEForm_Load(object sender, EventArgs e)
        {
            mwekey = Statics.reg_key_bethesda.OpenSubKey(Statics.reg_morrowind_enhanced, true);
            if (mwekey == null)
            {
                mwekey = Statics.reg_key_bethesda.CreateSubKey(Statics.reg_morrowind_enhanced);
            }
            foreach (Control trb in gbCombat.Controls)
            {
                if (trb is TrackBar)
                {
                    String name = ((TextBox)(((TrackBar)trb).Tag)).Tag.ToString();
                    try
                    {
                        ((TrackBar)trb).Value = (int)mwekey.GetValue(name);
                    }
                    catch
                    {
                        mwekey.SetValue(name, ((TrackBar)trb).Value, RegistryValueKind.DWord);
                    }
                }
            }
            foreach (Button b in new System.Collections.ArrayList{ bTrance, bRage, bBlock})
            {
                String name = b.Tag.ToString();
                int keyc = 0xFF;    // Code
                try { keyc = (int)mwekey.GetValue(name + " Code"); }
                catch { mwekey.SetValue(name + " Code", keyc, RegistryValueKind.DWord); }
                int keym = 0;       // Mouse
                try { keym = (int)mwekey.GetValue(name + " Mouse"); }
                catch { mwekey.SetValue(name + " Mouse", keym, RegistryValueKind.DWord); }
                if (keym == 1)
                    b.Text = "M" + Convert.ToString(keyc + 1);
                else if ((keym == 0) && (0x01 < keyc) && (keyc < 0xFF))
                    SetMWEButtonText(b, keyc);
            }
        }

        private void trackBars_ValueChanged(object sender, EventArgs e)
        {
            if (sender is TrackBar)
            {
                mwekey.SetValue(((TextBox)((TrackBar)sender).Tag).Tag.ToString(), ((TrackBar)sender).Value, RegistryValueKind.DWord);
                float ftrbv = ((TrackBar)sender).Value;
                if (((TrackBar)sender) == trbTrance)
                {
                    ftrbv = ftrbv / 10.0F;
                }
                else if (((TrackBar)sender) != trbRage)
                {
                    ftrbv = ftrbv / 20.0F;
                }
                ((TextBox)((TrackBar)sender).Tag).Text = Convert.ToString(ftrbv);
            }
        }

        private void bButtons_Click(object sender, EventArgs e)
        {
            bHitKey.Tag = sender;
            bHitKey.Visible = true;
            this.Controls.SetChildIndex(bHitKey, 0);
            bHitKey.Focus();
        }

        private void bHitKey_Leave(object sender, EventArgs e)
        {
            bHitKey.Tag = null;
            bHitKey.Visible = false;
        }

        private void bHitKey_MouseDown(object sender, MouseEventArgs e)
        {
            int key;
            switch (e.Button)
            {
                case MouseButtons.Left: key = 0; break;
                case MouseButtons.Right: key = 1; break;
                case MouseButtons.Middle: key = 2; break;
                case MouseButtons.XButton1: key = 3; break;
                case MouseButtons.XButton2: key = 4; break;
                default: key = -1; break;
            }
            if (key >= 0)
            {
                String name = ((Button)((Button)sender).Tag).Tag.ToString();
                mwekey.SetValue(name + " Code", key, RegistryValueKind.DWord);
                mwekey.SetValue(name + " Mouse", 1, RegistryValueKind.DWord);
                ((Button)((Button)sender).Tag).Text = "M" + Convert.ToString(key+1);
                bHitKey_Leave(null, null);
            }
        }

        private void bHitKey_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            int scancode = (int)MapVirtualKey((uint)e.KeyValue, 0);
            switch (e.KeyCode)
            {
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
            if ((0x01 < scancode) && (scancode < 0xFF))
            {
                String name = ((Button)((Button)sender).Tag).Tag.ToString();
                mwekey.SetValue(name + " Code", scancode, RegistryValueKind.DWord);
                mwekey.SetValue(name + " Mouse", 0, RegistryValueKind.DWord);
                SetMWEButtonText(((Button)((Button)sender).Tag), scancode);
                if (e.KeyCode == Keys.Enter) trbTrance.Focus();
            }
            bHitKey_Leave(null, null);
        }

        private void SetMWEButtonText(Button sender, int scancode)
        {
            int key = (int)MapVirtualKey((uint)scancode, 3);
            String st = (new KeysConverter()).ConvertToString(key);
            switch ((Keys)key)
            {   // Oem
                case Keys.Oemtilde:         st = "`"; break;
                case Keys.OemMinus:         st = "-"; break;
                case Keys.Oemplus:          st = "+"; break;
                case Keys.OemOpenBrackets:  st = "["; break;
                case Keys.Oem6:             st = "]"; break;
                case Keys.Oem1:             st = ";"; break;
                case Keys.Oem7:             st = "'"; break;
                case Keys.Oemcomma:         st = ","; break;
                case Keys.OemPeriod:        st = "."; break;
                case Keys.OemQuestion:      st = "/"; break;
                case Keys.Oem5:             st = "\\"; break;
                // Rename
                case Keys.Capital:          st = "Caps"; break;
                case Keys.LControlKey:      st = "L Ctrl"; break;
                case Keys.LMenu:            st = "L Alt"; break;
                case Keys.Multiply: st = "NL *"; break;
                case Keys.Subtract: st = "NL -"; break;
                case Keys.Add:      st = "NL +"; break;
                // Change
                case Keys.Delete:   st = "NL ."; break;
                case Keys.Insert:   st = "NL 0"; break;
                case Keys.End:      st = "NL 1"; break;
                case Keys.Down:     st = "NL 2"; break;
                case Keys.PageDown: st = "NL 3"; break;
                case Keys.Left:     st = "NL 4"; break;
                case Keys.Clear:    st = "NL 5"; break;
                case Keys.Right:    st = "NL 6"; break;
                case Keys.Home:     st = "NL 7"; break;
                case Keys.Up:       st = "NL 8"; break;
                case Keys.PageUp:   st = "NL 9"; break;
                // Extended
                case Keys.None:
                {
                    if (scancode < 0x80) { st = ""; break; }
                    key = (int)MapVirtualKey((uint)(scancode-0x80), 3);
                    st = (new KeysConverter()).ConvertToString(key);
                    switch ((Keys)key)
                    {
                        case Keys.Insert:   case Keys.Home: case Keys.PageUp:   case Keys.Up:   case Keys.Left:
                        case Keys.Delete:   case Keys.End:  case Keys.PageDown: case Keys.Down: case Keys.Right: break;
                        // Replace
                        case (Keys)0xF1: st = "L Win"; break;
                        case (Keys)0xEA: st = "R Win"; break;
                        case Keys.LMenu:        st = "R Alt"; break;
                        case Keys.LControlKey:  st = "R Ctrl"; break;
                        case Keys.EraseEof: st = "Appl"; break;
                        case Keys.NumLock:  st = "Pause"; break;
                        case Keys.Oem2:     st = "NL /"; break;
                        default: st = ""; break;
                    }
                    break;
                }
            }
            sender.Text = st;
        }
    }
}
