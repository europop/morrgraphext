using System;
using System.Collections.Generic;
using System.Text;

namespace MGEgui {
    public class Assembler {
        public enum DisassembleMode {
            /// <summary>
            /// Quickly determines size of the command only. Use this mode if you want to walk through the code. In this mode, treat all members of disasm as undefined.
            /// </summary>
            Disasm_Size,               // Determine command size only
            Disasm_Data,               // Determine size and analysis data
            Reserved,
            Disasm_File,               // Disassembly, no symbols
            /// <summary>
            /// Full disassembly.
            /// </summary>
            Disasm_Code                // Full disassembly
        }

        ////////////////////////////////////////////////  Assembler  ///////////////////////////////////////////////

        private static String AssemblePreparedLine(String line, uint address, uint attempt, uint constsize, out int length) {
            StringBuilder assembledString = new StringBuilder(256);
            StringBuilder errorString = new StringBuilder(256);
            length = NativeMethods.AssembleInstruction(line, address, attempt, constsize, assembledString, errorString);
            return assembledString.ToString().Trim();
        }

        public static String[] AssembleLine(String line, uint address) {
            List<String> HexVariants = new List<String>();
            for (uint attempt = 0; attempt < 3; attempt++) {
                for (uint constsize = 0; constsize < 4; constsize++) {
                    int length;
                    String hex = AssemblePreparedLine(line, address, attempt, constsize, out length);
                    if (length <= 0 || hex.Length == 0) continue;
                    if (!HexVariants.Contains(hex)) HexVariants.Add(hex);
                }
            }
            if (HexVariants.Count > 0) {
                String first = HexVariants[0];
                HexVariants.Sort(delegate(String a1, String a2) { return a1.Length > a2.Length ? 1 : a1.Length < a2.Length ? -1 : 0; });
                if (HexVariants[0].Length == first.Length) {    // Because of additional sorting by alphabet under Framework 2.0
                    HexVariants.Remove(first);
                    HexVariants.Insert(0, first);
                }
            }
            return HexVariants.ToArray();
        }

        public static String AssembleAsm(String asm, uint address, String[] attachedNames) {
            String hex = "";
            uint position = address;
            String[] lines = asm.Split(new Char[] { '\n' });
            foreach (String line in lines) {
                String instruction = "";
                String asmline = PrepareStringToAssembling(line);
                String[] addresses = GetLongAddresses(asmline, attachedNames);
                if (addresses.Length > 0) {
                    asmline = asmline.Replace(addresses[0], "0A0000000");
                    if (addresses.Length > 1) asmline = asmline.Replace(addresses[1], "0B0000000");
                }
                String[] variants = Assembler.AssembleLine(asmline, position);
                if (variants.Length > 0) {
                    String hexline = variants[0];
                    uint instrlength = (uint)hexline.Replace(" ", "").Length / 2;                    
                    if (addresses.Length > 0) {
                        String sA_dir = GetJumpAddress(0xA0000000, 0, 0);
                        String sA_jmp = GetJumpAddress(0xA0000000, position, instrlength);
                        if (hexline.IndexOf(sA_dir) > 0) {
                            hexline = hexline.Replace(sA_dir, "(" + addresses[0] + ")");
                        } else if(hexline.IndexOf(sA_jmp) > 0) {
                            hexline = hexline.Replace(sA_jmp, "[" + addresses[0] + "]");
                        }
                        if (addresses.Length > 1) {
                            String sB_dir = GetJumpAddress(0xB0000000, 0, 0);
                            String sB_jmp = GetJumpAddress(0xB0000000, position, instrlength);
                            if (hexline.IndexOf(sB_dir) > 0) {
                                hexline = hexline.Replace(sB_dir, "(" + addresses[1] + ")");
                            } else if(hexline.IndexOf(sB_jmp) > 0) {
                                hexline = hexline.Replace(sB_jmp, "[" + addresses[1] + "]");
                            }
                        }
                    }
                    instruction = hexline;
                    position += instrlength;
                } else {
                    //variants = Assembler.AssembleLine(PrepareStringToAssembling(line), position);
                    String[] args = asmline.Split(new Char[] {' ', '\t', ','}, StringSplitOptions.RemoveEmptyEntries);
                    if (args.Length > 0) {
                        if (args[0].ToLower() == "db") {
                            for (int i = 1; i < args.Length; i++)
                                if (args[i].Length == 2) instruction += (instruction.Length > 0 ? " " : "") + args[i]; else { instruction = ""; break; }
                        }
                    }
                }
                hex += (hex.Length > 0 ? "\n" : "") + instruction;
            }
            return hex;
        }

        //////////////////////////////////////////////  Disassembler  //////////////////////////////////////////////

        public static String DisassembleLine(byte[] source, uint index, uint address, DisassembleMode disassembleMode, out int length) {
            StringBuilder disassembledString = new StringBuilder(256);
            length = NativeMethods.DisassembleInstruction(source, source.Length, index, address, disassembleMode, disassembledString);
            return disassembledString.ToString().Trim();
        }
        public static String DisassembleLine(byte[] source, uint address, out int length) {
            return DisassembleLine(source, 0, address, DisassembleMode.Disasm_Code, out length);
        }
        public static String DisassembleLine(String sourceHex, uint address, out int length) {
            List<Byte> binary = new List<Byte>();
            String source = sourceHex.Replace(" ", "");
            for (int i = 0; i < source.Length; i += 2)  binary.Add(Convert.ToByte(source.Substring(i, 2), 16));
            return DisassembleLine(binary.ToArray(), address, out length);
        }
        public static String DisassembleLine(String sourceHex, uint address) {
            int length;
            return DisassembleLine(sourceHex, address, out length);
        }

        public static String DisassembleHex(String hex, uint address) {
            String asm = "";
            String replacedcode = ReplaceBrackets(hex, 0x000000A0);
            int length;
            uint position = address;
            while (replacedcode.Length > 0) {
                String nulled = DisassembleLine(replacedcode, position, out length);
                String s = String.Format("{0:X8}", 0xA0000000 + position + length);
                
                int index = nulled.IndexOf(s);
                if (index > 0)
                {
                    String instr = GetHexSubstring(hex, (int)(position - address), length);
                    instr = GetBracketsSubstring(instr);
                    nulled = nulled.Replace(s, instr);
                }
                index = nulled.IndexOf("A0000000");
                if (index > 0)
                {
                    String instr = GetHexSubstring(hex, (int)(position - address), length);
                    instr = GetBracketsSubstring(instr);
                    nulled = nulled.Replace("A0000000", instr);
                }

                asm += (asm.Length > 0 ? "\n" : "") + nulled;
                
                position += (uint)length;
                replacedcode = replacedcode.Substring(length * 2);
                
            }
            return asm;
        }

        public static SortedList<uint, String> DisassembleRegion(byte[] bFile, uint instance, uint desiredStart, int rows) {
            SortedList<uint, String> region = new SortedList<uint, String>();
            if (instance <= desiredStart && bFile.Length + instance > desiredStart ) {
                uint address = desiredStart - (desiredStart % 0x10); // The beginning of HEX line.
                while(instance < address) {
                    uint lastfourth = System.BitConverter.ToUInt32(bFile, (int)(address - instance - 4));
                    if (lastfourth == 0x90909090 || lastfourth == 0xCCCCCCCC)
                        break;
                    else address -= 0x10;
                }
                int length;
                while (address < desiredStart) {
                    DisassembleLine(bFile, address-instance, address, DisassembleMode.Disasm_Size, out length);
                    if (length <= 0) return region;
                    address += (uint)length;
                }
                if (address > desiredStart) {
                    region.Add(desiredStart, ToByteSequence(bFile, desiredStart - instance, (int)(address - desiredStart)));
                }
                while (region.Count < rows) {
                    String instruction = DisassembleLine(bFile, address - instance, address, DisassembleMode.Disasm_Code, out length);
                    if (length <= 0) { region.Clear(); return region; }
                    region.Add(address, instruction);
                    address += (uint)length;
                }
            }
            return region;
        }

        ////////////////////////////////////////////////  Accessory  ///////////////////////////////////////////////

        private static String PrepareStringToAssembling(String line) {
            String asmline = line.Trim().Replace(" sub_", " ").Replace(" loc_", " ").Replace(" offset ", " ").Replace(" unk_", " ");
            int ind;
            if ((ind = asmline.IndexOf("off_")) > 0) {
                asmline = asmline.Replace("off_", "[");
                if (asmline.IndexOf(",") > ind) asmline = asmline.Replace(", ", "], ");
                else asmline += "]";
            }
            if ((ind = asmline.IndexOf("dword_")) > 0) {
                asmline = asmline.Replace("dword_", "[");
                if (asmline.IndexOf(",") > ind) asmline = asmline.Replace(", ", "], ");
                else asmline += "]";
            }
            if ((ind = asmline.IndexOf("word_")) > 0) {
                asmline = asmline.Replace("word_", "[");
                if (asmline.IndexOf(",") > ind) asmline = asmline.Replace(", ", "], ");
                else asmline += "]";
            }
            if ((ind = asmline.IndexOf("byte_")) > 0) {
                asmline = asmline.Replace("byte_", "[");
                if (asmline.IndexOf(",") > ind) asmline = asmline.Replace(", ", "], ");
                else asmline += "]";
            }
            String[] lines = line.Split(new Char[] { ' ', '\t', ',' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = lines.Length - 1; 0 < i; i--) {    // Addition '0' at the start of HEX values, that start with letter.
                String added = TryConvert(asmline, lines, i);
                if (added != "") asmline = added;
                else if (lines[i].StartsWith("[") && lines[i].EndsWith("]")) {
                    String original = lines[i];
                    lines[i] = lines[i].Substring(1, lines[i].Length - 2);
                    added = TryConvert(asmline, lines, i);
                    if (added != "") asmline = added;
                    else if(lines[i].IndexOf("+") > 0 || lines[i].IndexOf("-") > 0 || lines[i].IndexOf("*") > 0) {
                        String[] sub_lines = lines[i].Split(new Char[] { '+', '-', '*' }, StringSplitOptions.RemoveEmptyEntries);
                        for (int sub_i = sub_lines.Length - 1; 0 <= sub_i; sub_i--) {
                            String sub_added = TryConvert(lines[i], sub_lines, sub_i);
                            if (sub_added != "") lines[i] = sub_added;
                        }
                        asmline = asmline.Replace(original, "[" + lines[i] + "]");
                    }
                }
            }
            return asmline;
        }

        private static String TryConvert(String line, String[] lines, int i) {
            String asmline = line;
            uint address = 0;
            try {
                address = Convert.ToUInt32(lines[i], 16);
                if (Char.IsLetter(lines[i][0])) {
                    int start = 0;
                    for (int index = 0; index < i; index++) {
                        start = asmline.IndexOf(lines[index], start);
                    }
                    start = asmline.IndexOf(lines[i], start);
                    asmline = asmline.Remove(start, lines[i].Length);
                    asmline = asmline.Insert(start, "0" + lines[i]);
                }
            } catch { asmline = ""; }
            return asmline;
        }

        private static String GetBracketsSubstring(String hexSubstring) {
            String substring = hexSubstring.Replace(" ", "").Replace("\t", "").Replace("\n", "");;
            int indexO, indexC = 0;
            if ((indexO = substring.IndexOfAny(new Char[] { '[', '(' }, indexC)) >= 0) {
                if ((indexC = substring.IndexOfAny(new Char[] { ']', ')' }, indexO)) >= 0) substring = substring.Substring(indexO + 1, indexC - indexO - 1);
                else substring = "";
            } else substring = "";
            return substring;
        }

        private static String GetHexSubstring(String hex, int start, int length) {
            String hexcode = hex.Replace(" ", "").Replace("\t", "").Replace("\n", "");
            int start_i = 0, position = 0;
            bool addrsegment = false;
            for (int i = 0; i < hexcode.Length; i++) {
                if (addrsegment) {
                    if (hexcode[i] == ']' || hexcode[i] == ')') {
                        addrsegment = false;
                        if (position == (start + length) * 2) { hexcode = hexcode.Substring(start_i, i - start_i + 1); break; }
                    }
                } else {
                    if (position == start * 2) start_i = i;
                    if (position == (start + length) * 2) { hexcode = hexcode.Substring(start_i, i - start_i + 1); break; }
                    if (hexcode[i] == '[' || hexcode[i] == '(') {
                        position += 8;
                        addrsegment = true;
                    } else { position++; if (position == (start + length) * 2) { hexcode = hexcode.Substring(start_i, i - start_i + 1); break; } }
                }
            }
            return hexcode;
        }

        private static String ReplaceBrackets(String hex, uint pattern) {
            String hexcode = hex.Replace(" ", "").Replace("\t", "").Replace("\n", "");
            int open, close;
            while ((open = hexcode.IndexOf("[")) >= 0) {
                if ((close = hexcode.IndexOf("]")) < 0) return "";
                hexcode = hexcode.Replace(hexcode.Substring(open, close - open + 1), String.Format("{0:X8}", pattern));
            }
            while ((open = hexcode.IndexOf("(")) >= 0) {
                if ((close = hexcode.IndexOf(")")) < 0) return "";
                hexcode = hexcode.Replace(hexcode.Substring(open, close - open + 1), String.Format("{0:X8}", pattern));
            }
            return hexcode;
        }

        private static String ToByteSequence(byte[] code, uint index, int length) {
            String instruction = "";
            if(index + length < code.Length && length > 0) {
                for (int i = 0; i < length; i++) instruction += (instruction.Length > 0 ? ", " : "") + String.Format("{0:X2}", code[(int)index + i]);
                instruction = "db      " + instruction;
            }
            return instruction;
        }

        private static String GetJumpAddress(uint label, uint position, uint length) {
            String address = String.Format("{0:X8}", label - position - length);
            return address.Substring(6, 2) + " " + address.Substring(4, 2) + " " + address.Substring(2, 2) + " " + address.Substring(0, 2);
        }

        private static String[] GetLongAddresses(String asm, String[] attachedNames) {
            String[] lines = asm.Split(new Char[] {' ', '\t', ','}, StringSplitOptions.RemoveEmptyEntries);
            List<String> attached = new List<String>(attachedNames);
            List<String> addresses = new List<String>();
            for (int i=0; i<lines.Length; i++) {
                if (attached.Contains(lines[i])) addresses.Add(lines[i]);
                else {
                    uint address = 0;
                    try {
                        address = Convert.ToUInt32(lines[i], 16);
                    } catch {}
                    if ((i > 0 ? lines[i - 1].ToLower() != "short" : false) && address >= 0x100) addresses.Add(lines[i]);
                    else if (lines[i].StartsWith("[") && lines[i].EndsWith("]")) {
                        lines[i] = lines[i].Substring(1, lines[i].Length - 2);
                        address = 0;
                        try {
                            address = Convert.ToUInt32(lines[i], 16);
                        } catch {}
                        if (address >= 0x100) addresses.Add(lines[i]);
                    }
                }
            }
            return addresses.ToArray();
        }
    }
}
