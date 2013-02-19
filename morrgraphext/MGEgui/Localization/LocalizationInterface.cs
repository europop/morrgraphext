using System;
using System.Reflection;
using System.Security;
using System.Collections.Generic;
using System.Globalization;
using System.Windows.Forms;
using System.Text;
using MGEgui.INI;

namespace MGEgui.Localization {

    public class LocalizationInterface {

        public class Localization {

            public string FileName;
            public string Language;
            public List<INIFile> langFiles = new List<INIFile>();
            public string Translator;

            private const string secLang = "## Language ##";
            private const string entLang = "lang";
            private const string entTtor = "ttor";

            private static INIFile.INIVariableDef [] langINI = new INIFile.INIVariableDef [] {
                INIFile.iniDefEmpty,
                new INIFile.INIVariableDef (entLang, secLang, "Name", INIFile.INIVariableType.String, ""),
                new INIFile.INIVariableDef (entTtor, secLang, "Translator", INIFile.INIVariableType.String, "")
            };

            public Localization (string fileName) {
                FileName = fileName;
                INIFile langFile = new INIFile(fileName, langINI);
                Language = langFile.getKeyString (entLang);
                Translator = langFile.getKeyString (entTtor);
                langFiles.Add(langFile);
            }

            /// <summary>
            /// Default Localization
            /// </summary>
            public Localization (Form form) {
                FileName = "";
                List<INIFile.INIVariableDef> lang = new List<INIFile.INIVariableDef>(langINI);
                lang.AddRange(AddVariableDef(DefaultLocalization._Strings, DefaultLocalization.__Strings));
                lang.AddRange(AddVariableDef(DefaultLocalization.MainForm_Strings, DefaultLocalization._MainForm_Strings));
                
                if(form != null) {
                    Dictionary<String, String> default_text = new Dictionary<String, String>();
                    Control [] controls = Statics.GetChildControls (form);
                    foreach (Control control in controls) {
                        if (!(control is ComboBox || control is TextBox || control is NumericUpDown) || control.Name == "tbSShotDir") if (!default_text.ContainsValue(control.Name)) default_text.Add(control.Name, control.Text);
                    }
                    lang.AddRange(AddVariableDef("MainForm.Text", default_text));
                }

                lang.AddRange(AddVariableDef(DefaultLocalization.MainForm_ToolTips, DefaultLocalization._MainForm_ToolTips));
                INIFile langFile = new INIFile(lang.ToArray());
                Language = DefaultLocalization.Language;
                Translator = "";
                langFiles.Add(langFile);
            }

            private List<INIFile.INIVariableDef> AddVariableDef(String section, Dictionary<string, string[]> toolTips) {
                int uniquename = 0;
                List<INIFile.INIVariableDef> lang = new List<INIFile.INIVariableDef>();
                lang.Add(new INIFile.INIVariableDef(section, "[" + section + "]"));
                foreach(String key in toolTips.Keys) {
                    lang.Add(new INIFile.INIVariableDef(section + uniquename++.ToString(), section, key, INIFile.INIVariableType.String, toolTips[key][1]));
                }
                return lang;
            }

            private List<INIFile.INIVariableDef> AddVariableDef(String section, Dictionary<String, String> strings) {
                int uniquename = 0;
                List<INIFile.INIVariableDef> lang = new List<INIFile.INIVariableDef>();
                lang.Add(new INIFile.INIVariableDef(section, "[" + section + "]"));
                foreach(String key in strings.Keys) {
                    lang.Add(new INIFile.INIVariableDef(section + uniquename++.ToString(), section, key, INIFile.INIVariableType.String, strings[key]));
                }
                return lang;
            }
        }

        private static List<CultureInfo> UserCultures = new List<CultureInfo>();
        public static CultureInfo[] UserLanguages {
            get {
                if(UserCultures.Count == 0) {
                    UserCultures.Add(CultureInfo.CurrentCulture);
                    foreach (InputLanguage inputlanguage in System.Windows.Forms.InputLanguage.InstalledInputLanguages)
                        if (!UserCultures.Contains(inputlanguage.Culture) && !inputlanguage.Culture.EnglishName.StartsWith(GetFirstInPair(DefaultLocalization.Language)))
                            UserCultures.Add(inputlanguage.Culture);
                }
                return UserCultures.ToArray();
            }
        }

        public static String GetFirstInPair(String pair) {
            String s = pair;
            int i = s.IndexOf('(');
            if (i > 0) s = (s.Substring(0, i)).Trim();
            return s;
        }
        public static String GetSecondInPair(String pair) {
            String s = pair;
            int i = s.IndexOf('(');
            int e = s.IndexOf(')');
            if (i++ > 0 && e > i) s = (s.Substring(i, e - i)).Trim();
            return s;
        }

        public static String SysLang(CultureInfo culture) {
            return (SysLangOrg(culture) + " (" + SysLangEng(culture) + ")");
        }
        public static String SysLang() {
            return SysLang(CultureInfo.CurrentCulture);
        }

        public static String SysLangRev(CultureInfo culture) {
            return (SysLangEng(culture) + " (" + SysLangOrg(culture) + ")");
        }
        public static String SysLangRev() {
            return SysLangRev(CultureInfo.CurrentCulture);
        }

        public static String SysLangEng(CultureInfo culture) {
            return GetFirstInPair(culture.EnglishName);
        }
        public static String SysLangEng() {
            return SysLangEng(CultureInfo.CurrentCulture);
        }

        public static String SysLangOrg(CultureInfo culture) {
            return GetFirstInPair(culture.NativeName);
        }
        public static String SysLangOrg() {
            return SysLangOrg(CultureInfo.CurrentCulture);
        }

        private Dictionary<string, Localization> localizations;
        private Localization currentLocalization;

        public LocalizationInterface () {
            localizations = new Dictionary<string, Localization> ();
        }

        public void Add (Localization localization) {
            AddLocalizations(localization.Language, localization);
        }

        private void AddLocalizations(String language, Localization localization) {
            if (localizations.ContainsKey(language)) {
                localizations[language].langFiles.AddRange(localization.langFiles);
            } else localizations.Add(language, localization);
        }

        public void Add (string fileName) {
            if (fileName != null && fileName != "") {
                Localization localization = new Localization (fileName);
                if (localization.Language != "") AddLocalizations(localization.Language, localization);
            }
        }

        public string [] Languages {
            get {
                List<string> list = new List<string> (localizations.Keys);
                list.Sort ();
                return list.ToArray ();
            }
        }

        public Localization this [string Language] {
            get {
                if (localizations.ContainsKey (Language)) return localizations [Language];
                else return null;
            }
            set {
                if (localizations.ContainsKey (Language)) localizations [Language] = value;
                else AddLocalizations(Language, value);
            }
        }

        public string Current {
            get {
                return currentLocalization.Language;
            }
            set {
                if(value != null && localizations.ContainsKey(value))
                    currentLocalization = localizations[value];
                else
                    currentLocalization = localizations[DefaultLocalization.Language];
            }
        }
        
        public int Count {
            get { return localizations.Count; }
        }

        public void ApplyStrings (string name, Dictionary<string, string> messages) {
            ApplyStrings(name, messages, localizations[DefaultLocalization.Language]);
            ApplyStrings(name, messages, currentLocalization);
        }

        public void ApplyStrings (string name, Dictionary<string, string> messages, Localization localization) {
            foreach (INIFile langFile in localization.langFiles) {
                Dictionary<string, string> dict;
                dict = langFile.getSectionKeys (name + ".Strings");
                foreach (KeyValuePair<string, string> entry in dict) {
                    messages [entry.Key] = entry.Value;
                }
            }
        }

        public void Apply (Form form) {
            Apply(form, localizations[DefaultLocalization.Language]);
            Apply(form, currentLocalization);
        }

        public void Apply (Form form, Localization localization) {
            FieldInfo messages_field;
            foreach (INIFile langFile in localization.langFiles) {
                Dictionary<string, string> dict;
                dict = langFile.getSectionKeys(form.Name + ".Text");
                foreach(KeyValuePair<string, string> entry in dict) {
                    Control[] controls = form.Controls.Find(entry.Key, true);
                    foreach(Control control in controls) {
                        if(!(control is ComboBox))
                            control.Text = entry.Value;
                    }
                }
            }

            messages_field = form.GetType().GetField("strings");
            Dictionary<string, string> messages = new Dictionary<string, string>();
            if(messages_field != null && messages_field.FieldType == messages.GetType()) {
                messages = messages_field.GetValue(form) as Dictionary<string, string>;
                ApplyStrings(form.Name, messages, localization);
            }

            messages_field = form.GetType().GetField("tooltip_messages");
            Dictionary<string, string[]> tips = new Dictionary<string, string[]>();
            if(messages_field != null && messages_field.FieldType == tips.GetType()) {
                tips = messages_field.GetValue(form) as Dictionary<string, string[]>;
                messages_field = form.GetType().GetField("toolTip");
                ToolTip toolTip = messages_field.GetValue(form) as ToolTip;
                foreach (INIFile langFile in localization.langFiles) {
                    Dictionary<string, string> dict;
                    dict = langFile.getSectionKeys(form.Name + ".ToolTips");
                    foreach (KeyValuePair<string, string> entry in dict) {
                        if (tips.ContainsKey(entry.Key)) {
                            foreach(string controlName in tips[entry.Key]) {
                                Control[] controls = form.Controls.Find(controlName, true);
                                foreach(Control control in controls)
                                    toolTip.SetToolTip(control, entry.Value);
                            }
                        }
                        else {
                            Control[] controls = form.Controls.Find(entry.Key, true);
                            foreach(Control control in controls) {
                                if(control is TabPage)
                                    (control as TabPage).ToolTipText = entry.Value;
                            }
                        }
                    }
                }
            }
        }

    }

}
