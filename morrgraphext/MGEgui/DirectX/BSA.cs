//TODO: When reading in the BSAs, ignore anything that isn't a texture
using System;
using System.Collections.Generic;
using System.IO;

namespace MGEgui.DistantLand {
    struct BSAEntry : IComparable<BSAEntry> {
        public readonly BinaryReader bsafile;
        public readonly string entryname;
        public readonly int offset;
        public readonly int size;

        public int CompareTo(BSAEntry entry) { return string.Compare(entryname, entry.entryname); }

        public BSAEntry(BinaryReader file, string ename, int _offset, int _size) {
            bsafile=file;
            entryname=ename.ToLower();
            offset=_offset;
            size=_size;
        }

        public static bool operator==(BSAEntry a, BSAEntry b) {
            return a.entryname==b.entryname;
        }
        public static bool operator==(BSAEntry a, string b) {
            return a.entryname==b.ToLower();
        }
        public static bool operator!=(BSAEntry a, BSAEntry b) {
            return a.entryname!=b.entryname;
        }
        public static bool operator!=(BSAEntry a, string b) {
            return a.entryname!=b.ToLower(); ;
        }
        public override bool Equals(object obj) {
            if(obj is string) return this==(string)obj;
            if(obj is BSAEntry) return this==(BSAEntry)obj;
            return false;
        }
        public override int GetHashCode() {
            return entryname.GetHashCode();
        }
    }

    static class BSA {
        //Should be a hash dictionary. Sorted anyway, so little difference
        private static readonly List<BSAEntry> entries=new List<BSAEntry>(16384);
        private static readonly List<BinaryReader> files=new List<BinaryReader>();

        public static void InitBSAs() {
            if(entries.Count>0) return; //Already been init-ed
            string[] bsas=Directory.GetFiles("data files", "*.bsa");
            foreach(string s in bsas) {
                BinaryReader br=new BinaryReader(File.OpenRead(s));
                br.BaseStream.Position+=4;
                int hashoffset=br.ReadInt32();
                int numfiles=br.ReadInt32();
                for(int i=0;i<numfiles;i++) {
                    br.BaseStream.Position=12+i*8;
                    int size=br.ReadInt32();
                    int offset=br.ReadInt32()+12+hashoffset+numfiles*8;
                    br.BaseStream.Position=12+numfiles*8+i*4;
                    br.BaseStream.Position=br.ReadInt32()+12+numfiles*12;
                    string name="";
                    while(true) {
                        byte b=br.ReadByte();
                        if(b==0) break;
                        name+=(char)b;
                    }
                    entries.Add(new BSAEntry(br, "data files\\"+name, offset, size));
                }
                files.Add(br);
            }
            entries.Sort();
        }

        public static byte[] GetTexture(string name) {
            name=name.ToLower();

            int index;
            if (Path.IsPathRooted(name)) {
                throw new ArgumentException("Something tried to load a texture using an absolute path.");
            }
            
            name = name.ToLower();

            if (name[0] != '\\') {
                name = "\\" + name;
            }
            string dds_name = Path.ChangeExtension(name, ".dds");

            List<string> search_paths = new List<string>();
            search_paths.Add("data files\\textures" + dds_name);
            search_paths.Add("data files" + dds_name);
            search_paths.Add("data files\\textures" + name);
            search_paths.Add("data files" + name);

            //Search file system
            foreach (string file_path in search_paths) {
                if (File.Exists(file_path)) {
                    return File.ReadAllBytes(file_path);
                }
            }

            //Search BSA files
            foreach (string file_path in search_paths) {
                index = entries.BinarySearch(new BSAEntry(null, file_path, 0, 0));
                if (index < 0) continue;
                entries[index].bsafile.BaseStream.Position = entries[index].offset;
                byte[] result = entries[index].bsafile.ReadBytes(entries[index].size);
                return result;
            }

            //If we get here, the file wasn't found anywhere.
            return null;
        }

        public static byte[] GetNif(string name) {
            name=name.ToLower();
            if(Path.IsPathRooted(name)) {
                throw new ArgumentException("Something tried to load a nif using an absolute path.");
            }

            string path=Path.Combine(@"data files\mge meshes\distantland\", name);
            if(File.Exists(path)) return File.ReadAllBytes(path);
            path=Path.Combine(@"data files\meshes\", name);
            if(File.Exists(path)) return File.ReadAllBytes(path);

            int index = entries.BinarySearch(new BSAEntry(null, path, 0, 0));
            if(index < 0) throw new ArgumentException("No nif matching the specified filename was found");
            entries[index].bsafile.BaseStream.Position = entries[index].offset;
            byte[] data = entries[index].bsafile.ReadBytes(entries[index].size);
            return data;
        }

        public static void CloseFiles() {
            foreach(BinaryReader br in files) br.Close();
            files.Clear();
            entries.Clear();
        }
    }

    public class ArchiveBSA
    {
        private class NameAndData {
            public NameAndData(UInt64 hash, String name, byte[] data) {
                Hash = hash;
                Name = name;
                Data = data;
            }
            public UInt64 Hash;
            public String Name;
            public byte[] Data;
        }
        public void Clear() { Items.Clear(); }
        private SortedList<UInt64, NameAndData> Items = new SortedList<UInt64, NameAndData>();
        public static UInt64 GetHash(String name) {
            UInt64 hash;
            String str = name.ToLower();
	        uint len = (uint)str.Length;

	        //Use GhostWheel's code to hash the string
	        uint l = (len>>1);
	        uint sum, off, temp, i, n;

	        for(sum = off = i = 0; i < l; i++) {
		        sum ^= (((uint)(str[(int)i])) << (int)(off&0x1F));
		        off += 8;
	        }
	        hash = sum;

	        for(sum = off = 0; i < len; i++) {
                temp = (((uint)(str[(int)i])) << (int)(off & 0x1F));
		        sum ^= temp;
		        n = temp & 0x1F;
                sum = (sum << (int)(32 - n)) | (sum >> (int)n);  // binary "rotate right"
		        off += 8;
	        }
	        hash |= ((UInt64)sum)<<32;
            return hash;
        }
        public int GetItemsCount() { return Items.Count; }
        public void AddFile(String name, byte[] data) {
            if (name == null || data == null) return;
            if (name.Length == 0 || data.Length == 0) return;
            UInt64 hash = GetHash(name);
            UInt64 hashR = ((hash & 0x00000000FFFFFFFF) << 32) | ((hash & 0xFFFFFFFF00000000) >> 32);
            if (!Items.ContainsKey(hashR)) Items.Add(hashR, new NameAndData(hash, name, data));
        }
        public bool Save(String filename) {
            StreamWriter sw;
            try {
                sw = new StreamWriter(filename, false, System.Text.Encoding.Default);
            } catch {
                sw = null;
            }
            if (sw == null) return false;
            BinaryWriter bw = new BinaryWriter(sw.BaseStream);
            int Position = 0;
            int[] NameOffsets = new int[Items.Count];
            for (int i = 0; i < Items.Count; i++) {
                NameOffsets[i] = Position;
                Position += Items.Values[i].Name.Length + 1;
            }

            bw.Write((uint)0x100);
            bw.Write(Position + 3 * sizeof(uint) * Items.Count);
            bw.Write(Items.Count);

            Position = 0;
            for (int i = 0; i < Items.Count; i++) {
                bw.Write(Items.Values[i].Data.Length);
                bw.Write(Position);
                Position += Items.Values[i].Data.Length;
            }
            for (int i = 0; i < Items.Count; i++) {
                bw.Write(NameOffsets[i]);
            }
            for (int i = 0; i < Items.Count; i++) {
                bw.Write(Items.Values[i].Name.ToCharArray());
                bw.Write((byte)0);
            }
            for (int i = 0; i < Items.Count; i++) {
                bw.Write(Items.Values[i].Hash);
            }
            for (int i = 0; i < Items.Count; i++) {
                bw.Write(Items.Values[i].Data);
            }
            sw.Close();
            bw.Close();
            return true;
        }
    }
}
