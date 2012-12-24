using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Drawing.Design;
using System.Globalization;

namespace MGEgui.Patching {

    public class INIFile : MGEgui.INI.INIFile {
        public INIFile(string file, INIVariableDef[] varDefn, bool saveDef) : base(file, varDefn, saveDef) { }
        public INIFile(string file, INIVariableDef[] varDefn) : base(file, varDefn) { }
        public INIFile(string file, INIVariableDef[] varDefn, Encoding encoding, bool saveDef) : base(file, varDefn, encoding, saveDef) { }
        public INIFile(string file, INIVariableDef[] varDefn, Encoding encoding) : base(file, varDefn, encoding) { }
        public string[] getSections() {
            List<String> sections = new List<String>();
            foreach (INIFile.INILine line in iniContent) {
                String Section = line.entry.Trim();
                int start = Section.IndexOf("[") + 1;
                int length = Section.IndexOf("]") - start;
                if ((start == 1) && (length > 0)) Section = Section.Substring(start, length).Trim(); else continue;
                if (hasSection(Section)) sections.Add(Section);
            }
            return sections.ToArray();
        }
        public String getCommentAbove(String section, String key) {
            String section_lwr = section.ToLower();
            String[] lines = getSectList(section_lwr);
            String previous_line = "";
            foreach (String line in lines) if (line.StartsWith(key, StringComparison.OrdinalIgnoreCase)) break; else previous_line = line;
            String comment = "";
            foreach (INIFile.INILine line in iniContent) {
                if (line.section != section_lwr) continue;
                if (line.key == key) break;
                if(previous_line.Length > 0) {
                    if (line.entry == previous_line) previous_line = "";
                } else if (line.comment.Length > 0)
                    comment += line.comment.Substring(line.comment.IndexOf(INIFile.INIComment) + INIFile.INIComment.Length) + "\n";
            }
            return comment;
        }        
        public bool setCommentAbove(String section, String key, String comment) {
            String section_lwr = section.ToLower();
            for (int i = 0; i < iniContent.Count; i++) {
                if (iniContent[i].section != section_lwr) continue;
                if (iniContent[i].key == key) { iniContent.Insert(i, new INILine((INIFile.INIComment + comment).Trim())); return true; }
            }
            return false;
        }
    }

    [TypeConverter(typeof(UnitConverter))]
    public class Unit {
        public Unit(String name) { this.name = name; Hex = ""; Asm = ""; address = 0; }
        public Unit(UInt32 address) { this.address = address; Hex = ""; Asm = ""; name = ""; }
        public Unit(UInt32 address, String Hex, String Asm)
        { this.address = address; this.Hex = Hex; this.Asm = Asm; name = ""; }
        public Unit(String name, UInt32 address, String Hex, String Asm)
        { this.name = name; this.address = address; this.Hex = Hex; this.Asm = Asm; }
        public Unit Copy() { return new Unit(name, address, Hex, Asm); }
        public String Hex;
        public String Asm;
        private UInt32 address;
        public UInt32 Address {
            get { return address; }
            set { address = value; }
        }
        private String name;
        public String Name {
            get { return name; }
            set { name = value; }
        }
        public override string ToString() {
            Patch patch = MGEgui.Statics.mf.PatchEditor.GetSelectedPatch();
            uint DefaultInstance = (patch != null ? patch.GetDefaultInstance() : 0);
            if (Address < Patch.Default_Instance_Exe) Address += DefaultInstance;
            else if (DefaultInstance == Patch.Default_Instance_Exe) {
                if (Address >= Patch.Default_Instance_Dll) Address -= (Patch.Default_Instance_Dll - Patch.Default_Instance_Exe);
            } else if (Address < Patch.Default_Instance_Dll) Address += (Patch.Default_Instance_Dll - Patch.Default_Instance_Exe);
            return Name == "" ? String.Format("{0:X8}", Address) : Name;
        }
    }

    public class PropertyGridParentEditor : UITypeEditor {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context) {
	        return UITypeEditorEditStyle.Modal;
        }
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value) {
            String Parent = value==null ? "" : (String)value;
            String fileName = MGEgui.Statics.mf.PatchEditor.SelectExecutableFile("Open executable file"); 
            if (fileName != null) {
                String directory = Application.StartupPath + "\\";
                if(fileName.StartsWith(directory)) {
                    String ParentBranch = Patch.GetParentBranch(Parent);
                    Parent = fileName.Substring(directory.Length) + (ParentBranch.Length > 0 ? "\\" + ParentBranch : "");
                    MGEgui.Statics.mf.PatchEditor.AddBinaryFile(Patch.GetParentFile(Parent));
                }
            }
            return Parent;
        }
    }

    public class PropertyGridNumericUpDownEditor : UITypeEditor {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context) {
	        return UITypeEditorEditStyle.DropDown;
        }
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value) {
            Decimal Value = Convert.ToDecimal(value);
            NumericUpDown numericUpDown = new NumericUpDown();
            numericUpDown.DecimalPlaces = 2;
            numericUpDown.Increment = 0.01M;
            numericUpDown.Minimum = 1;
            numericUpDown.Maximum = 1000;
            numericUpDown.Value = Value < numericUpDown.Maximum ? Value < 1 ? numericUpDown.Minimum : Value : numericUpDown.Maximum;
            ((IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService))).DropDownControl(numericUpDown);
            return (float)numericUpDown.Value;
        }
    }
    public class UnitArrayEditor : UITypeEditor {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context) {
	        return UITypeEditorEditStyle.None;
        }
    }

    public class Patch {
        public const uint Default_Instance_Exe = 0x00400000;
        public const uint Default_Instance_Dll = 0x10000000;
        public Patch() { }
        public Patch(String path) { Parent = path; }
        public Patch(INIFile mcpFile, String section) {
            Dictionary<String, String> keys = mcpFile.getSectionKeys(section);
            foreach (String sKey in keys.Keys) {
                String value = mcpFile.getKeyString(section, sKey).Trim();
                bool oKey = sKey.StartsWith(Keys[Key.Original], StringComparison.OrdinalIgnoreCase);
                bool pKey = sKey.StartsWith(Keys[Key.Patch], StringComparison.OrdinalIgnoreCase);
                bool aKey = sKey.StartsWith(Keys[Key.Attach], StringComparison.OrdinalIgnoreCase);
                bool dKey = sKey.StartsWith(Keys[Key.Description], StringComparison.OrdinalIgnoreCase);
                if (aKey || oKey || pKey || dKey) {
                    value = value.Replace("\t", "\n");
                    String comment = mcpFile.getCommentAbove(section, sKey);
                    Unit[] units = oKey ? Original : pKey ? Patched : aKey ? Attach : Description;
                    List<Unit> array = units != null ? new List<Unit>(units) : new List<Unit>();
                    String tag = sKey.Substring(Keys[oKey ? Key.Original : pKey ? Key.Patch : aKey ? Key.Attach : Key.Description].Length).Trim();
                    array.Add(aKey || dKey ? new Unit(tag, 0, value, comment) : new Unit(Convert.ToUInt32(tag, 16), value, comment));
                    if (oKey) Original = array.ToArray(); else if (pKey) Patched = array.ToArray(); else if (aKey) Attach = array.ToArray();
                    else Description = array.ToArray();
                    continue;
                }
                foreach (Key key in Keys.Keys) {
                    if (sKey.ToLower() == Keys[key].ToLower()) {
                        NumberFormatInfo provider = new NumberFormatInfo();
                        provider.NumberDecimalSeparator = ".";
                        switch (key) {
                            case Key.Checked: { try { Checked = Convert.ToBoolean(value.ToLower()); } catch { } break; }
                            case Key.Parent: { Parent = value; break; }
                            case Key.Version: { try { version = Math.Abs((float)Convert.ToDouble(value, provider)); } catch { } break; }
                            case Key.FileVersion: { try { fileVersion = Math.Abs(Convert.ToInt32(value)); } catch { } break; }
                            case Key.Author: { Author = value; break; }
                            case Key.Removed: { try { Removed = Convert.ToBoolean(value.ToLower()); } catch { } break; }
                        }
                        break;
                    }
                }
            }
        }
        public enum Key { None, Checked, Parent, Version, FileVersion, Original, Patch, Attach, Affected, Author, Description, Removed }
        public static Dictionary<Key, String> Keys = new Dictionary<Key, String> {
            { Key.Checked, "Checked" }, { Key.Parent, "Parent" }, { Key.Version, "Version" }, { Key.FileVersion, "FileVersion" },
            { Key.Original, "Original" }, { Key.Patch, "Patch" }, { Key.Attach, "Attach" }, { Key.Affected, "Affected" },
            { Key.Author, "Author" }, { Key.Description, "Description" }, { Key.Removed, "Removed" }
        };
        private Unit[] CopyArray(Unit[] array) {
            if (array == null) return array;
            List<Unit> copy = new List<Unit>();
            for (int i = 0; i < array.Length; i++) { copy.Add(array[i].Copy()); }
            return copy.ToArray();
        }
        public Patch Copy() {
            Patch copy = (Patch)this.MemberwiseClone();
            copy.Description = CopyArray(copy.Description);
            copy.Original = CopyArray(copy.Original);
            copy.Patched = CopyArray(copy.Patched);
            copy.Attach = CopyArray(copy.Attach);
            return copy;
        }
        private static String GetParentFile(String ParentString, String End) {
            String ParentFile = "";
            int EndIndex = 0;
            while ((EndIndex = ParentString.IndexOf(End, EndIndex, StringComparison.OrdinalIgnoreCase) + End.Length) > End.Length) {
                if (EndIndex == ParentString.Length ? true : ParentString[EndIndex] == '\\') {
                    ParentFile = ParentString.Substring(0, EndIndex);
                    break;
                }
            }
            return ParentFile;
        }
        public static String GetParentFile(String ParentString) {
            String ParentExe = GetParentFile(ParentString, ".exe");
            String ParentDll = GetParentFile(ParentString, ".dll");
            return ParentExe.Length <= 0 ? ParentDll : ParentDll.Length <= 0 ? ParentExe : (ParentExe.Length < ParentDll.Length ? ParentExe : ParentDll);
        }
        public String GetParentFile() {
            return GetParentFile(Parent);
        }
        public static String GetParentBranch(String ParentString) {
            String ParentFile = GetParentFile(ParentString);
            return ParentFile.Length > 0 && ParentString.Length > ParentFile.Length ? ParentString.Substring(ParentFile.Length + 1) : "";
        }
        public String GetParentBranch() {
            return GetParentBranch(Parent);
        }
        public uint GetDefaultInstance() {
            return GetParentFile().EndsWith(".exe", StringComparison.OrdinalIgnoreCase) ? Patch.Default_Instance_Exe : Patch.Default_Instance_Dll;
        }
        private const String general = "General";
        private const String patching = "Patching";
        private const String secondary = "Secondary";
        //  General
        private bool check = true;
        [Category(general)]
        [DisplayName("Checked")]
        [DefaultValue(true)]
        public bool Checked {
            get { return check; }
            set { check = value; }
        }
        private String parent = "";
        [Category(general)]
        [DisplayName("Parent")]
        [Editor(typeof(PropertyGridParentEditor), typeof(UITypeEditor))]
        public String Parent {
            get { return parent; }
            set { parent = value;
                parent = parent.Replace("/", "\\");
                Char[] llegalchar = new Char[] { '\t', '\n', '\r', '=' }; // And may be '-', '+', '&', '|', '^', '>', '<', '?', '(', ')', '[', ']', '*', ':', '\"' 
                while (parent.IndexOfAny(llegalchar) >= 0) parent = parent.Remove(parent.IndexOfAny(llegalchar), 1); 
                while (parent.IndexOf("\\ ") >= 0) parent = parent.Replace("\\ ", "\\");
                while (parent.IndexOf(" \\") >= 0) parent = parent.Replace(" \\", "\\");
                while (parent.IndexOf("\\\\") >= 0) parent = parent.Replace("\\\\", "\\");
            }
        }
        private float version = 1F;
        [Category(general)]
        [DisplayName("Version")]
        [DefaultValue(1F)]
        [Editor(typeof(PropertyGridNumericUpDownEditor), typeof(UITypeEditor))]
        public float Version {
            get { return version; }
            set { version = value < 1000 ? value < 1 ? 1 : value : 1000; }
        }
        private int fileVersion = 1820;
        [Category(general)]
        [DisplayName("FileVersion")]
        [DefaultValue(1820)]
        public int FileVersion {
            get { return fileVersion; }
            set { fileVersion = value < 10000 ? value < 0 ? 0 : value : 10000; }
        }
        //  Patching
        private Unit[] original;
        [Category(patching)]
        [Editor(typeof(UnitArrayEditor), typeof(UITypeEditor))]
        [TypeConverter(typeof(UnitArrayTypeConverter))]
        public Unit[] Original {
            get { return original; }
            set { original = value; }
        }
        private Unit[] patch;
        [Category(patching)]
        [DisplayName("Patch")]
        [Editor(typeof(UnitArrayEditor), typeof(UITypeEditor))]
        [TypeConverter(typeof(UnitArrayTypeConverter))]
        public Unit[] Patched {
            get { return patch; }
            set { patch = value; }
        }
        private Unit[] attach;
        [Category(patching)]
        [Editor(typeof(UnitArrayEditor), typeof(UITypeEditor))]
        [TypeConverter(typeof(UnitArrayTypeConverter))]
        public Unit[] Attach {
            get { return attach; }
            set { attach = value; }
        }
        //  Secondary
        private String author = "";
        [Category(secondary)]
        [DisplayName("Author")]
        public String Author {
            get { return author; }
            set { author = value; }
        }
        public Unit[] Description = new Unit[] { };
        [Category(secondary)]
        [DisplayName("Description")]
        [TypeConverter(typeof(DescriptionConverter))]
        public String description {
            get {   return Description.Length > 0 ? Description[Description.Length - 1].Name : ""; }
            set {   List<Unit> units = new List<Unit>();
                    Unit last = new Unit(value);
                    foreach (Unit unit in Description) if (unit.Name == value) last = unit; else units.Add(unit);
                    units.Add(last);
                    Description = units.ToArray();
                }
        }
        private bool removed = false;
        [Category(secondary)]
        [DisplayName("Removed")]
        [DefaultValue(false)]
        public bool Removed {
            get { return removed; }
            set { removed = value; }
        }
    }

    public class DescriptionConverter : StringConverter {
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context) { return true; }
        public override bool GetStandardValuesExclusive(ITypeDescriptorContext   context) { return true; }
        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context) {
            List<String> languages = new List<String>();
            foreach (CultureInfo info in CultureInfo.GetCultures(CultureTypes.NeutralCultures)) {
                if ((info.EnglishName.Length > 0) && (info.EnglishName.IndexOf(" ") < 0)) languages.Add(info.EnglishName);
            }
            languages.Sort();
            languages.Remove(CultureInfo.CurrentCulture.Parent.EnglishName);
            languages.Insert(0, CultureInfo.CurrentCulture.Parent.EnglishName);
            languages.Remove("English");
            languages.Insert(0, "English");
            languages.Insert(0, "");
            List<String> top = new List<String>();
            foreach (Unit unit in (Unit[])((Patch)context.Instance).Description) if (unit.Name != "" && unit.Hex.Trim() != "") top.Add(unit.Name);
            top.Sort();
            for (int i = top.Count - 1; i >= 0; i--) { languages.Remove(top[i]); languages.Insert(0, top[i]); }
            return new StandardValuesCollection(languages);
        }
    }

    public class UnitConverter : TypeConverter {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType) {
            if (sourceType == typeof(string)) { return true; } else { return base.CanConvertFrom(context, sourceType); }
        }
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType) {
            if (destinationType == typeof(string)) { return true; } else { return base.CanConvertTo(context, destinationType); }
        }
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value) {
            if (value is string) {
                String label = context.PropertyDescriptor.Name;
                Unit property = ((Unit[])context.Instance)[label.Length - label.Trim().Length - 1];
                if (label.Trim() == Patch.Keys[Patch.Key.Attach]) {
                    if (!System.Text.RegularExpressions.Regex.IsMatch((String)value, "^[a-zA-Z0-9_]+$")) throw new System.ArgumentException();
                    property.Name = ((String)value).Trim();
                }
                else property.Address = uint.Parse((string)value, NumberStyles.HexNumber, culture);
                return property;
            } else { return base.ConvertFrom(context, culture, value); }
        }
        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType) {
            if (destinationType == typeof(string)) { return ((Unit)value).ToString(); }
            else return base.ConvertTo(value, destinationType);
        }
        public override object CreateInstance(ITypeDescriptorContext context, System.Collections.IDictionary propertyValues) {
            return new Unit((uint)propertyValues["Address"]);
        }
        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes) {
            PropertyDescriptorCollection collection;
            collection = TypeDescriptor.GetProperties(typeof(Unit), attributes);
            return collection.Sort(new String[] { "Address" });
        }
    }

    public class UnitArrayTypeConverter : TypeConverter {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType) {
            if (sourceType == typeof(string)) { return true; } else { return base.CanConvertFrom(context, sourceType); }
        }        
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value) {
            String label = context.PropertyDescriptor.Name.Trim();
            Patch patch = (Patch)context.Instance;
            Unit[] array = label != Patch.Keys[Patch.Key.Original] ? label != Patch.Keys[Patch.Key.Attach] ? patch.Patched : patch.Attach : patch.Original;
            List<Unit> values = new List<Unit>();
            if (array != null) values.AddRange(array);
            if (label == Patch.Keys[Patch.Key.Attach]) {
                if (!System.Text.RegularExpressions.Regex.IsMatch((String)value, "^[a-zA-Z0-9_]+$")) throw new System.ArgumentException();
                if (values.Find(delegate(Unit unit) { return unit.Name == (String)value; }) == null) values.Add(new Unit(((String)value).Trim()));
            } else {
                UInt32 address = uint.Parse((string)value, NumberStyles.HexNumber, culture);
                if (values.Find(delegate(Unit unit) { return unit.Address == address; }) == null) values.Add(new Unit(address));
            }
            values.Sort(delegate(Unit u1, Unit u2) { return label == Patch.Keys[Patch.Key.Attach] ? u1.Name.CompareTo(u2.Name) : u1.Address.CompareTo(u2.Address); });
            if (value is string) return values.ToArray(); else return array;
        }
        private class UnitPropertyDescriptor : TypeConverter.SimplePropertyDescriptor {
            private int index;
            public UnitPropertyDescriptor(Type arrayType, Type itemType, string name, int index) : base(arrayType, name, itemType) {
                this.index = index;
            }
            public override object GetValue(object component) {
                if (component is Unit[]) {
                    Unit[] array = (Unit[])component;
                    if (array.Length > this.index) return array[index];
                }
                return null;
            }
            public override void SetValue(object component, object value) {
                if (component is Unit[]) {
                    Unit[] array = (Unit[])component;
                    if (array.Length > this.index) array[index] = (Unit)value;
                }
            }
        }        
        public override object CreateInstance(ITypeDescriptorContext context, System.Collections.IDictionary propertyValues) {
            String label = context.PropertyDescriptor.Name.Trim();
            Patch patch = (Patch)context.Instance;
            Unit[] array = label != Patch.Keys[Patch.Key.Original] ? label != Patch.Keys[Patch.Key.Attach] ? patch.Patched : patch.Attach : patch.Original;
            foreach (String updated in propertyValues.Keys) {
                array[updated.Length - updated.Trim().Length - 1] = (Unit)propertyValues[updated];
            }
            return array;
        }
        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes) {
            PropertyDescriptor[] props = null;
            if (value is Unit[]) {
                props = new PropertyDescriptor[((Unit[])value).Length];
                Patch patch = (Patch)context.Instance;
                String text = value != patch.Original ? value != patch.Attach ? Patch.Keys[Patch.Key.Patch] : Patch.Keys[Patch.Key.Attach] : Patch.Keys[Patch.Key.Original];
                for (int i = 0; i < props.Length; i++) {
                    props[i] = new UnitPropertyDescriptor(typeof(Unit[]), typeof(Unit), text += " ", i);
                }
            }
            return new PropertyDescriptorCollection(props);
        }
        public override bool GetCreateInstanceSupported(ITypeDescriptorContext context) { return true; }
        public override bool GetPropertiesSupported(ITypeDescriptorContext context) { return true; }
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType) {
            if (destinationType == typeof(string)) { return true; } else { return base.CanConvertTo(context, destinationType); }
        }
        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType) {
            return "";
        }
    }
}