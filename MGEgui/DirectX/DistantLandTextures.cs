using System;
using System.Windows.Forms;
using Microsoft.DirectX.Direct3D;
using TexCache=System.Collections.Generic.Dictionary<string, Microsoft.DirectX.Direct3D.Texture>;
using MGEgui.DistantLand;

namespace MGEgui.DirectX {
    public class LTEX {
        private static readonly TexCache texCache=new TexCache();

        public static void ReleaseCache() {
            foreach(Texture t in texCache.Values) { try { if(!t.Disposed) t.Dispose(); } catch { } }
            texCache.Clear();
        }

        private string filePath;
        public string FilePath {
            set { filePath=value.ToLower(); }
            get { return filePath; }
        }
        public int index;
        public Texture tex;

        public void LoadTexture() {
            if(texCache.ContainsKey(filePath)) {
                tex=texCache[filePath];
            } else {
                try {
                    byte[] data=MGEgui.DistantLand.BSA.GetTexture(filePath);
                    if (data.Length>6 && data[1] == 0 && (data[2] == 2 || data[2] == 10)) {
                        data[5] = data[6] = 0;
                    }
                    tex=TextureLoader.FromStream(DXMain.device, new System.IO.MemoryStream(data), 0, 0, 0, Usage.None, Format.Unknown, Pool.Managed, Filter.Triangle|Filter.Dither, Filter.Box, 0);
                    //tex.GenerateMipSubLevels();
                    //TextureLoader.FilterTexture(tex, 0, Filter.Box);
                    texCache[filePath]=tex;
                } catch {
                    tex=null;
                    throw;
                }
            }
        }
    }

    class StaticTexCreator {
        private readonly System.Collections.Generic.List<string> texCache;
        private readonly int div;

        public StaticTexCreator(int skip) {
            div=(1<<skip);
            texCache=new System.Collections.Generic.List<string>();
        }

        public void Dispose() {
            texCache.Clear();
        }

        public byte[] LoadTexture(string path) {
            if(texCache.Contains(path)) return null;
            texCache.Add(path);

            byte[] data=MGEgui.DistantLand.BSA.GetTexture(path);
            if (data == null) {
                if (MessageBox.Show("Missing texture: '" + path + "'\n"
                + "Continuing may result in texture glitches in game.\n"
                + "Do you wish to continue?", "Warning", MessageBoxButtons.YesNo) == DialogResult.Yes) return null;
            }
            if (data.Length>6 && data[1] == 0 && (data[2] == 2 || data[2] == 10)) {
                data[5] = data[6] = 0;
            }
            System.IO.MemoryStream ms=new System.IO.MemoryStream(data);
            path=System.IO.Path.ChangeExtension(path, ".dds");
            
            Texture t=TextureLoader.FromStream(DXMain.device, ms, 0, 0, 0, Usage.None, Format.Unknown, Pool.Scratch, Filter.Triangle|Filter.Dither, Filter.Box, 0);
            SurfaceDescription sd=t.GetLevelDescription(0);
            t.Dispose();

            Format format;
            if(sd.Format==Format.Dxt1||sd.Format==Format.X8R8G8B8) format=Format.Dxt1;
            else format=Format.Dxt3;

            ms.Position=0;
            //System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(System.IO.Path.Combine(@"data files\distantland\statics\textures\", path)));
            t=TextureLoader.FromStream(DXMain.device, ms, sd.Width/div, sd.Height/div, 0, Usage.None, format, Pool.Scratch, Filter.Triangle|Filter.Dither, Filter.Box, 0);
            //TextureLoader.Save(System.IO.Path.Combine(@"data files\distantland\statics\textures\", path), ImageFileFormat.Dds, t);
            Microsoft.DirectX.GraphicsStream gs = TextureLoader.SaveToStream(ImageFileFormat.Dds, t);
            byte[] lqTexture = new byte[gs.Length];
            if(lqTexture.Length != gs.Read(lqTexture, 0, lqTexture.Length)) lqTexture = null;
            gs.Close();
            t.Dispose();
            ms.Close();
            return lqTexture;
        }
    }

    class TextureBank
    {
        public LTEX t1, t2, t3, t4;
        public VertexBuffer wBuffer;

        public struct WeightVertex
        {
            public byte w1, w2, w3, w4;

            public const VertexFormats Format = VertexFormats.Diffuse;
            public const int Stride = 4;
        };


        public TextureBank() {
            wBuffer = new VertexBuffer(typeof(WeightVertex), 4225, DXMain.device, Usage.WriteOnly, WeightVertex.Format, Pool.Managed);
        }

        public void SetSingleTexture( LTEX tex1 ) {
            //There this will create a default bank which can be used for default land which has one texture
            t1 = tex1;

            WeightVertex[] WeightData = (WeightVertex[])wBuffer.Lock(0, LockFlags.None);
            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    //Write values
                    WeightData[i].w1 = 255;
                    WeightData[i].w2 = 0;
                    WeightData[i].w3 = 0;
                    WeightData[i].w4 = 0;
                }
            }
            wBuffer.Unlock();
        }

        private WeightVertex SampleWeightData(ref WeightVertex[] array, int x, int y) {

            //Ensure that x and y do not escape the bounds of the array.
            if (x < 0) { x = 0; }
            if (y < 0) { y = 0; }
            if (x > 64) { x = 64; }
            if (y > 64) { y = 64; }

            //Return the value at the constrained location
            return array[y * 65 + x];
        }
        public void CalcWeights(LAND cell) {

            WeightVertex[] WeightData = new WeightVertex[65*65];

            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = x * 65 + y;

                    //Figure out which texture is used here
                    int cell_x = cell.xpos;
                    int cell_y = cell.ypos;
                    int tex_x = (int)Math.Floor(((float)y - 1.0f) / 4.0f);//-2.0f
                    int tex_y = (int)Math.Floor(((float)x-3.0f) / 4.0f);//-2.0f

                    DistantLandForm.ModCell( ref cell_x, ref tex_x );
                    DistantLandForm.ModCell( ref cell_y, ref tex_y );

                    LTEX tmp = DistantLandForm.GetTex(cell_x, cell_y, tex_x, tex_y);
                    string tex_index = tmp.FilePath;

                    //Write values
                    if (t1 != null && t1.FilePath == tex_index) {
                        WeightData[i].w1 = 255;
                        continue;
                    } else {
                        WeightData[i].w1 = 0;
                    }

                    if (t2 != null && t2.FilePath == tex_index) {
                        WeightData[i].w2 = 255;
                        continue;
                    } else {
                        WeightData[i].w2 = 0;
                    }

                    if (t3 != null && t3.FilePath == tex_index) {
                        WeightData[i].w3 = 255;
                        continue;
                    } else {
                        WeightData[i].w3 = 0;
                    }

                    if (t4 != null && t4.FilePath == tex_index) {
                        WeightData[i].w4 = 255;
                        continue;
                    } else {
                        WeightData[i].w4 = 0;
                    }
                }
            }

            //Blur the weights as we transfer them so the transitions aren't quite so blocky and horrible.
            
            ////Normal Gaussian
            //float bf1 = 0.00081723f;
            //float bf2 = 0.02804153f;
            //float bf3 = 0.23392642f;
            //float bf4 = 0.47442968f;

            //Reduced center influence
            float bf1 = 0.1f;
            float bf2 = 0.15f;
            float bf3 = 0.2f;
            float bf4 = 0.1f;
            
            //Horizontal Pass
            WeightVertex[] FirstPassWD = new WeightVertex[65 * 65];
            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    if (x == 0 || x == 64 || y == 0 || y == 64){
                        //We're at the edge, so just copy the value (don't want to interfere with the way the edges of cells look
                        FirstPassWD[i] = WeightData[i];
                        continue;
                    }

                    //We're not at the edge, so add some influence from the surrounding weights
                    //Additional incides
                    WeightVertex wv0, wv1, wv2, wv3, wv4, wv5, wv6;

                    wv0 = SampleWeightData(ref WeightData, x - 3, y);
                    wv1 = SampleWeightData(ref WeightData, x - 2, y);
                    wv2 = SampleWeightData(ref WeightData, x - 1, y);
                    wv3 = SampleWeightData(ref WeightData, x, y);
                    wv4 = SampleWeightData(ref WeightData, x + 1, y);
                    wv5 = SampleWeightData(ref WeightData, x + 2, y);
                    wv6 = SampleWeightData(ref WeightData, x + 2, y);

                    float value;
                    value = 0.0f;
                    value += (float)wv0.w1 * bf1;
                    value += (float)wv1.w1 * bf2;
                    value += (float)wv2.w1 * bf3;
                    value += (float)wv3.w1 * bf4;
                    value += (float)wv4.w1 * bf3;
                    value += (float)wv5.w1 * bf2;
                    value += (float)wv6.w1 * bf1;
                    FirstPassWD[i].w1 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w2 * bf1;
                    value += (float)wv1.w2 * bf2;
                    value += (float)wv2.w2 * bf3;
                    value += (float)wv3.w2 * bf4;
                    value += (float)wv4.w2 * bf3;
                    value += (float)wv5.w2 * bf2;
                    value += (float)wv6.w2 * bf1;
                    FirstPassWD[i].w2 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w3 * bf1;
                    value += (float)wv1.w3 * bf2;
                    value += (float)wv2.w3 * bf3;
                    value += (float)wv3.w3 * bf4;
                    value += (float)wv4.w3 * bf3;
                    value += (float)wv5.w3 * bf2;
                    value += (float)wv6.w3 * bf1;
                    FirstPassWD[i].w3 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w4 * bf1;
                    value += (float)wv1.w4 * bf2;
                    value += (float)wv2.w4 * bf3;
                    value += (float)wv3.w4 * bf4;
                    value += (float)wv4.w4 * bf3;
                    value += (float)wv5.w4 * bf2;
                    value += (float)wv6.w4 * bf1;
                    FirstPassWD[i].w4 = (byte)value;
                }
            }

            //Vertical pass - writes to final vertex buffer
            WeightVertex[] FinalWeightData = (WeightVertex[])wBuffer.Lock(0, LockFlags.None);
            //Blur the weights as we transfer them so the transitions aren't quite so blocky and horrible.
            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    if (x == 0 || x == 64 || y == 0 || y == 64){
                        //We're at the edge, so just copy the value (don't want to interfere with the way the edges of cells look
                        FinalWeightData[i] = WeightData[i];
                        continue;
                    }

                    //We're not at the edge, so add some influence from the surrounding weights
                    //Additional incides
                    WeightVertex wv0, wv1, wv2, wv3, wv4, wv5, wv6;
                    wv0 = SampleWeightData(ref FirstPassWD, x, y - 2);
                    wv1 = SampleWeightData(ref FirstPassWD, x, y - 2);
                    wv2 = SampleWeightData(ref FirstPassWD, x, y - 1);
                    wv3 = SampleWeightData(ref FirstPassWD, x, y);
                    wv4 = SampleWeightData(ref FirstPassWD, x, y + 1);
                    wv5 = SampleWeightData(ref FirstPassWD, x, y + 2);
                    wv6 = SampleWeightData(ref FirstPassWD, x, y - 2);

                    float value;
                    value = 0.0f;
                    value += (float)wv0.w1 * bf1;
                    value += (float)wv1.w1 * bf2;
                    value += (float)wv2.w1 * bf3;
                    value += (float)wv3.w1 * bf4;
                    value += (float)wv4.w1 * bf3;
                    value += (float)wv5.w1 * bf2;
                    value += (float)wv6.w1 * bf1;
                    FinalWeightData[i].w1 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w2 * bf1;
                    value += (float)wv1.w2 * bf2;
                    value += (float)wv2.w2 * bf3;
                    value += (float)wv3.w2 * bf4;
                    value += (float)wv4.w2 * bf3;
                    value += (float)wv5.w2 * bf2;
                    value += (float)wv6.w2 * bf1;
                    FinalWeightData[i].w2 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w3 * bf1;
                    value += (float)wv1.w3 * bf2;
                    value += (float)wv2.w3 * bf3;
                    value += (float)wv3.w3 * bf4;
                    value += (float)wv4.w3 * bf3;
                    value += (float)wv5.w3 * bf2;
                    value += (float)wv6.w3 * bf1;
                    FinalWeightData[i].w3 = (byte)value;

                    value = 0.0f;
                    value += (float)wv0.w4 * bf1;
                    value += (float)wv1.w4 * bf2;
                    value += (float)wv2.w4 * bf3;
                    value += (float)wv3.w4 * bf4;
                    value += (float)wv4.w4 * bf3;
                    value += (float)wv5.w4 * bf2;
                    value += (float)wv6.w4 * bf1;
                    FinalWeightData[i].w4 = (byte)value;
                }
            }

            wBuffer.Unlock();
        }

        ~TextureBank() {
            wBuffer.Dispose();
        }

    };

    class CellTexCreator {
        private struct CellVertex {
            public float x, y, z, w;
            public float u, v;

            public const VertexFormats Format = VertexFormats.Position | VertexFormats.Texture1;
            public const int Stride = 24;
        };

        private struct NormalColorVertex {
            public float nx, ny, nz;
            public byte b, g, r, a;

            public const VertexFormats Format = VertexFormats.Normal | VertexFormats.Diffuse;
            public const int Stride = 16;
        };

        VertexElement[] Elements = new VertexElement[] {
            //Stream 0 - position and texture coordinates
            new VertexElement(0, 0, DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.Position, 0),
            new VertexElement(0, 16, DeclarationType.Float2, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),

            //Stream 1 - normals and vertex colors
            new VertexElement(1, 0, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0),
            new VertexElement(1, 12, DeclarationType.Color, DeclarationMethod.Default, DeclarationUsage.Color, 0),

            ///Stream 2 - Texture weights
            new VertexElement(2, 0, DeclarationType.Color, DeclarationMethod.Default, DeclarationUsage.Color, 1),
                                    
            VertexElement.VertexDeclarationEnd 
        };

        VertexElement[] NormalElements = new VertexElement[] {
            //Stream 0 - position and texture coordinates
            new VertexElement(0, 0, DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.Position, 0),
            new VertexElement(0, 16, DeclarationType.Float2, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),

            //Stream 1 - normals and vertex colors
            new VertexElement(1, 0, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0),
            new VertexElement(1, 12, DeclarationType.Color, DeclarationMethod.Default, DeclarationUsage.Color, 0),
                                    
            VertexElement.VertexDeclarationEnd 
        };

        private const string EffectPath=@"data files\shaders\distantland\CellTexBlend.fx";
        private VertexBuffer vBuffer;
        private VertexBuffer colorBuffer;
        private System.Collections.Generic.List<TextureBank> texBanks;
        private IndexBuffer iBuffer;
        private Texture RenderTargetTex;
        private Texture CompressedTex;
        private Surface RenderTarget;
        private Effect effect;
        //private bool vcolor;

        private EffectHandle m1h;
        private EffectHandle t1h;
        private EffectHandle t2h;
        private EffectHandle t3h;
        private EffectHandle t4h;

        public CellTexCreator(int Res) {
            texBanks = new System.Collections.Generic.List<TextureBank>();
            //Create basic vertex buffer that can bue used for all cells which has positions and texture coordinates
            vBuffer = new VertexBuffer(typeof(CellVertex), 4225, DXMain.device, Usage.WriteOnly, CellVertex.Format, Pool.Managed);
            CellVertex[] CellData = (CellVertex[])vBuffer.Lock(0, LockFlags.None);
            //vBuffers=new VertexBuffer[64, 64];
            float mult = (float)(Res / 64);
            for(int y=0;y<=64;y++) {
                for(int x=0;x<=64;x++) {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    //Write values
                    CellData[i].x = ((float)x / 64.0f) * 2.0f - 1.0f;
                    CellData[i].y = ((float)y / 64.0f) * 2.0f - 1.0f;
                    CellData[i].z = 0.5f;
                    CellData[i].w = 1.0f;
                    CellData[i].u = (float)x / 16.0f;
                    CellData[i].v = (float)y / 16.0f;
                }
            }
            vBuffer.Unlock();

            //Create triangle strip index buffer
            //Size is 2r + 2rc + 2(r-1) where r is rows and c is colums (squares, not vertices)
            iBuffer = new IndexBuffer(typeof(Int16), 8446, DXMain.device, Usage.WriteOnly, Pool.Managed);
            Int16[] iBuf = (Int16[])iBuffer.Lock(0, LockFlags.None);
            int idx = 0;
            for(int y=0;y<64;y++) {
                //If this is is a continuation strip, we need to add two extra vertices to create degenerat triangles
                //and get us back to the left side
                if ( y > 0 ) {
                    iBuf[idx] = (Int16)(y * 65 + (63+1));
                    iBuf[idx + 1] = (Int16)(y * 65 + 0);
                    idx += 2;
                }

                //Start the row off with a vertex in the lower left corner of the square
                iBuf[idx] = (Int16)(y * 65 + 0);
                ++idx;

                for(int x=0;x<64;x++) {
                    //Add the top left and bottom right vertex of each square
                    iBuf[idx] = (Int16)((y+1) * 65 + x);
                    iBuf[idx + 1] = (Int16)(y * 65 + (x+1));

                    idx += 2;
                }

                //End the row with the top right vertex
                iBuf[idx] = (Int16)((y+1) * 65 + (63+1));
                ++idx;
            }

            iBuffer.Unlock();

            //Create the buffers that will contain different information during each render
            colorBuffer = new VertexBuffer(typeof(NormalColorVertex), 4225, DXMain.device, Usage.WriteOnly, NormalColorVertex.Format, Pool.Managed);

            ResetColorsAndNormals();

            RenderTargetTex=new Texture(DXMain.device, Res, Res, 0, Usage.RenderTarget, Format.X8R8G8B8, Pool.Default);
            CompressedTex=new Texture(DXMain.device, Res, Res, 0, Usage.None, Format.Dxt1, Pool.SystemMemory);
            RenderTarget=RenderTargetTex.GetSurfaceLevel(0);
            effect=Effect.FromFile(DXMain.device, EffectPath, null, null, ShaderFlags.NotCloneable, null);

            m1h = effect.GetParameter(null, "transform");
            t1h = effect.GetParameter(null, "t1");
            t2h = effect.GetParameter(null, "t2");
            t3h = effect.GetParameter(null, "t3");
            t4h = effect.GetParameter(null, "t4");
            //if(!vColor) {
            //    ColorValue white=ColorValue.FromColor(System.Drawing.Color.White);
            //    effect.SetValue("c1", white);
            //    effect.SetValue("c2", white);
            //    effect.SetValue("c3", white);
            //    effect.SetValue("c4", white);
            //    vcolor=false;
            //} else {
            //    c1h=effect.GetParameter(null, "c1");
            //    c2h=effect.GetParameter(null, "c2");
            //    c3h=effect.GetParameter(null, "c3");
            //    c4h=effect.GetParameter(null, "c4");
            //    vcolor=true;
            //}
        }

        public void ResetColorsAndNormals() {

            //By default, the normal will be up and the color will be white
            NormalColorVertex[] ColorNormalData = (NormalColorVertex[])colorBuffer.Lock(0, LockFlags.None);
            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    //Write values
                    ColorNormalData[i].r = 255;
                    ColorNormalData[i].g = 255;
                    ColorNormalData[i].b = 255;
                    ColorNormalData[i].a = 255;

                    ColorNormalData[i].nx = 0.0f;
                    ColorNormalData[i].ny = 0.0f;
                    ColorNormalData[i].nz = 1.0f;
                }
            }
            colorBuffer.Unlock();
        }

        public void SetDefaultCell(LTEX tex) {
            ResetColorsAndNormals();
            texBanks.Clear();
            TextureBank tb = new TextureBank();
            tb.SetSingleTexture(tex);
            texBanks.Add(tb);
        }

        public void SetCell( LAND cell ) {
        
            //Write the new colors and normals into the color buffer
            NormalColorVertex[] ColorNormalData = (NormalColorVertex[])colorBuffer.Lock(0, LockFlags.None);
            for (int y = 0; y <= 64; y++)
            {
                for (int x = 0; x <= 64; x++)
                {
                    //Figure out which index to use
                    int i = y * 65 + x;

                    //Write values
                    ColorNormalData[i].r = cell.Color[x, y].r;
                    ColorNormalData[i].g = cell.Color[x, y].g;
                    ColorNormalData[i].b = cell.Color[x, y].b;
                    ColorNormalData[i].a = 255;

                    ColorNormalData[i].nx = cell.Normals[x, y].X;
                    ColorNormalData[i].ny = cell.Normals[x, y].Y;
                    ColorNormalData[i].nz = cell.Normals[x, y].Z;
                }
            }
            colorBuffer.Unlock();

            //Dispose of any current texture banks
            texBanks.Clear();
            
            //Group the unique textures in this cell in fours
            
            //Find all te uniqe textures in this cell
            System.Collections.Generic.Dictionary<string, LTEX> tex_dict = new System.Collections.Generic.Dictionary<string, LTEX>();
            for (int x = 0; x <= 64; ++x)  {
                for (int y = 0; y <= 64; ++y) {
                    int cell_x = cell.xpos;
                    int cell_y = cell.ypos;
                    int tex_x = (int)Math.Floor(((float)y - 1.0f) / 4.0f);//-2.0f
                    int tex_y = (int)Math.Floor(((float)x - 3.0f) / 4.0f);//-2.0f

                    DistantLandForm.ModCell(ref cell_x, ref tex_x);
                    DistantLandForm.ModCell(ref cell_y, ref tex_y);

                    LTEX tmp = DistantLandForm.GetTex(cell_x, cell_y, tex_x, tex_y);
                    string idx = tmp.FilePath;
                    tex_dict[idx] = tmp;
                }
            }
 
            //Create one bank for each group of 4 textures
            int index = 0;
            TextureBank tb = new TextureBank();
            foreach (LTEX tex in tex_dict.Values) {
                switch (index) {
                    case 0:
                        tb.t1 = tex;
                        ++index;
                        break;
                    case 1:
                        tb.t2 = tex;
                        ++index;
                        break;
                    case 2:
                        tb.t3 = tex;
                        ++index;
                        break;
                    case 3:
                        tb.t4 = tex;
                        texBanks.Add(tb);
                        tb = new TextureBank();
                        index = 0;
                        break;
                }
            }

            if (index != 0) {
                texBanks.Add(tb);
            }

            if ( texBanks.Count > 1 ) {
                int blah = 4;
                int blu = 7;
                int blablu = blah + blu;
            }

            //Calculate weights for all banks
            foreach (TextureBank bank in texBanks) {
                bank.CalcWeights(cell);
            }
        }

        public void Dispose()
        {
            //DXMain.device.SetRenderTarget(0, DXMain.BackBuffer);
            vBuffer.Dispose();
            colorBuffer.Dispose();
            texBanks.Clear();
            RenderTarget.Dispose();
            RenderTargetTex.Dispose();
            CompressedTex.Dispose();
            effect.Dispose();
        }

        public void Begin() {
            DXMain.device.RenderState.CullMode=Cull.CounterClockwise;
            //DXMain.device.RenderState.CullMode=Cull.None;
            DXMain.device.RenderState.Clipping=true;
            DXMain.device.VertexFormat=CellVertex.Format;

            DXMain.device.SetStreamSource(0, vBuffer, 0);
            DXMain.device.SetStreamSource(1, colorBuffer, 0);
            VertexDeclaration decl = new VertexDeclaration(DXMain.device, Elements);
            //DXMain.device.RenderState.FillMode = FillMode.WireFrame;
            DXMain.device.Indices = iBuffer;
            DXMain.device.VertexDeclaration = decl;

            /*DXMain.device.BeginScene();
            effect.Begin(FX.None);
            effect.BeginPass(0);*/
        }

        public void BeginNormalMap()
        {
            DXMain.device.RenderState.CullMode = Cull.CounterClockwise;
            //DXMain.device.RenderState.CullMode=Cull.None;
            DXMain.device.RenderState.Clipping = true;
            DXMain.device.VertexFormat = CellVertex.Format;

            DXMain.device.SetStreamSource(0, vBuffer, 0);
            DXMain.device.SetStreamSource(1, colorBuffer, 0);
            VertexDeclaration decl = new VertexDeclaration(DXMain.device, NormalElements);
            //DXMain.device.RenderState.FillMode = FillMode.WireFrame;
            DXMain.device.Indices = iBuffer;
            DXMain.device.VertexDeclaration = decl;

            /*DXMain.device.BeginScene();
            effect.Begin(FX.None);
            effect.BeginPass(1);*/
        }

        /*public void BeginTexture() {
            Begin();
            if ( DXMain.device.GetRenderTarget(0) != RenderTarget ) {
                DXMain.device.SetRenderTarget(0, RenderTarget);
            }
        }*/


        /*public void RenderTexture() {
            DXMain.device.Clear(ClearFlags.Target, 0, 0.0f, 0);
            Render(0.0f, 0.0f, 1.0f, 1.0f);
        }*/

        public void Render( float pos_x, float pos_y, float scale_x, float scale_y ) {
            

            Microsoft.DirectX.Matrix mat = Microsoft.DirectX.Matrix.Identity;
            mat.M41 = pos_x;
            mat.M42 = pos_y;
            mat.M11 = scale_x;
            mat.M22 = scale_y;

            effect.SetValue(m1h, mat);

            foreach (TextureBank bank in texBanks) {
                effect.SetValue(t1h, bank.t1.tex);
                if (bank.t2 != null) {
                    effect.SetValue(t2h, bank.t2.tex);
                } else {
                    effect.SetValue(t2h, bank.t1.tex);
                }
                if (bank.t3 != null) {
                    effect.SetValue(t3h, bank.t3.tex);
                } else {
                    effect.SetValue(t3h, bank.t1.tex);
                }
                if (bank.t4 != null) {
                    effect.SetValue(t4h, bank.t4.tex);
                } else {
                    effect.SetValue(t4h, bank.t1.tex);
                }
                
                effect.CommitChanges();
                DXMain.device.SetStreamSource(2, bank.wBuffer, 0);
                DXMain.device.BeginScene();
                effect.Begin(FX.None);
                effect.BeginPass(0);
                DXMain.device.DrawIndexedPrimitives(PrimitiveType.TriangleStrip, 0, 0, 4225, 0, 8444);
                effect.EndPass();
                effect.End();
                DXMain.device.EndScene();
            }

            DXMain.device.BeginScene();
            effect.Begin(FX.None);
            effect.BeginPass(2);
            DXMain.device.DrawIndexedPrimitives(PrimitiveType.TriangleStrip, 0, 0, 4225, 0, 8444);
            effect.EndPass();
            effect.End();
            DXMain.device.EndScene();
        }



        public void RenderNormalMap(float pos_x, float pos_y, float scale_x, float scale_y)
        {
            

            Microsoft.DirectX.Matrix mat = Microsoft.DirectX.Matrix.Identity;
            mat.M41 = pos_x;
            mat.M42 = pos_y;
            mat.M11 = scale_x;
            mat.M22 = scale_y;

            effect.SetValue(m1h, mat);
            
            effect.CommitChanges();
            DXMain.device.BeginScene();
            effect.Begin(FX.None);
            effect.BeginPass(1);
            DXMain.device.DrawIndexedPrimitives(PrimitiveType.TriangleStrip, 0, 0, 4225, 0, 8444);
            effect.EndPass();
            effect.End();
            DXMain.device.EndScene();
        }

        public void End()
        {
            /*effect.EndPass();
            effect.End();
            DXMain.device.EndScene();*/
        }

        public void EndNormalMap() {
            End();
        }

        /*public void EndTexture(string path) {

            End();

            Surface s1 = CompressedTex.GetSurfaceLevel(0);
            SurfaceLoader.FromSurface(s1, RenderTarget, Filter.None, 0);
            for (int i = 1; i < CompressedTex.LevelCount; i++) {
                Surface s2 = CompressedTex.GetSurfaceLevel(i);
                SurfaceLoader.FromSurface(s2, s1, Filter.Linear, 0);
                s1 = s2;
            }
            TextureLoader.Save(path, ImageFileFormat.Dds, CompressedTex);
        }*/
    }

    class WorldTexCreator
    {
        private const string DefaultTex = @"data files\distantland\default.dds";

        private Texture CompressedTex;
        private Texture UncompressedTex;
        private Texture RenderTargetTex;
        private Surface RenderTarget;
        //private Surface DefaultSurf;

        //private readonly System.Drawing.Rectangle cellRect;
        //private Filter surfFilter;

        //private readonly int[] PixelOffsetsX;
        //private readonly int[] PixelOffsetsY;
        //private readonly int ArrayOffsetX;
        //private readonly int ArrayOffsetY;

        private int MapMinX, MapMaxX, MapMinY, MapMaxY, MapSpanX, MapSpanY;
        public float x_scale, y_scale, x_spacing, y_spacing;

        public WorldTexCreator(int Res, int map_min_x, int map_max_x, int map_min_y, int map_max_y)
        {
            //float frac=(float)Res/(float)Cells;
            //cellRect=new System.Drawing.Rectangle(0, 0, CellRes, CellRes);

            //if(filter) surfFilter=Filter.Triangle|Filter.Dither; else surfFilter=Filter.Point;

            RenderTargetTex = new Texture(DXMain.device, Res, Res, 0, Usage.RenderTarget, Format.X8R8G8B8, Pool.Default);
            CompressedTex = new Texture(DXMain.device, Res, Res, 0, Usage.None, Format.Dxt1, Pool.SystemMemory);
            UncompressedTex = new Texture(DXMain.device, Res, Res, 0, Usage.None, Format.X8R8G8B8, Pool.SystemMemory);
            RenderTarget = RenderTargetTex.GetSurfaceLevel(0);

            //DefaultSurf=DXMain.device.CreateOffscreenPlainSurface(CellRes, CellRes, Format.X8R8G8B8, Pool.Scratch);
            //SurfaceLoader.FromFile(DefaultSurf, DefaultTex, Filter.None, 0);

            MapMinX = map_min_x;
            MapMaxX = map_max_x;
            MapMinY = map_min_y;
            MapMaxY = map_max_y;

            MapSpanX = MapMaxX - MapMinX + 1;
            MapSpanY = MapMaxY - MapMinY + 1;

            x_scale = 1.0f / (float)MapSpanX;
            y_scale = 1.0f / (float)MapSpanY;
            x_spacing = x_scale * 2.0f;
            y_spacing = y_scale * 2.0f;

            ////Calculate the texture offsets for each cell that we are rendering
            //int CellSpanX = CellMaxX - CellMinX + 1;
            //int CellSpanY = CellMaxY - CellMinY + 1;

            //float frac_x = (float)Res / (float)CellSpanX;
            //float frac_y = (float)Res / (float)CellSpanY;

            //ArrayOffsetX = 0 - CellMinX;
            //ArrayOffsetY = 0 - CellMinY;

            //PixelOffsetsX = new int[CellSpanX + 1];
            //PixelOffsetsY = new int[CellSpanY + 1];

            //for (int i = 0; i <= CellMaxX + ArrayOffsetX; ++i) {
            //    PixelOffsetsX[i] = (int)(i * frac_x);
            //}
            //for ( int i = 0; i <= CellMaxY + ArrayOffsetY; ++i ) {
            //    PixelOffsetsY[i] = (int)(i*frac_y);
            //}

            ////Add an extra offset at the end that is exactly on the edge of the final texture
            //PixelOffsetsX[CellSpanX] = Res-1;
            //PixelOffsetsY[CellSpanY] = Res-1;
        }

        public void Begin()
        {
            if (DXMain.device.GetRenderTarget(0) != RenderTarget)
            {
                DXMain.device.SetRenderTarget(0, RenderTarget);
            }
            DXMain.device.Clear(ClearFlags.Target, 0, 0.0f, 0);
        }

        public void FinishCompressed(string path, bool twoStep)
        {
            if(twoStep) {
                Surface tmp=UncompressedTex.GetSurfaceLevel(0);
                SurfaceLoader.FromSurface(tmp, RenderTarget, Filter.None, 0);

                Surface output = CompressedTex.GetSurfaceLevel(0);
                SurfaceLoader.FromSurface(output, tmp, Filter.None, 0);
                tmp.Dispose();

                TextureLoader.FilterTexture(CompressedTex, 0, Filter.Box);
                TextureLoader.Save(path, ImageFileFormat.Dds, CompressedTex);
                output.Dispose();
            } else {
                Surface output = CompressedTex.GetSurfaceLevel(0);
                SurfaceLoader.FromSurface(output, RenderTarget, Filter.None, 0);
                TextureLoader.FilterTexture(CompressedTex, 0, Filter.Box);
                TextureLoader.Save(path, ImageFileFormat.Dds, CompressedTex);
                output.Dispose();
            }
        }

        public void FinishUncompressed(string path)
        {
            Surface output = UncompressedTex.GetSurfaceLevel(0);
            SurfaceLoader.FromSurface(output, RenderTarget, Filter.None, 0);
            TextureLoader.FilterTexture(UncompressedTex, 0, Filter.Box);
            TextureLoader.Save(path, ImageFileFormat.Tga, UncompressedTex);
            output.Dispose();
        }

        public void Dispose()
        {
            RenderTarget.Dispose();
            CompressedTex.Dispose();
            UncompressedTex.Dispose();
            RenderTargetTex.Dispose();
        }
    };
}
