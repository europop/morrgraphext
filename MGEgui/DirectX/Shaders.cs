using System;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Collections.Generic;

namespace MGEgui.DirectX {

    public static class Shaders {
        private static Texture thisFrame;
        private static Texture lastFrame;
        private static Texture lastShader;
        private static Texture lastPass;
        private static Texture depthFrame;

        private static Surface thisFrameSurface;
        private static Surface lastFrameSurface;
        private static Surface lastShaderSurface;
        private static Surface lastPassSurface;

        private static Vector4 SinVar;
        private static Vector4 LinVar;

        

        private static VertexBuffer vBuffer;

        private struct vertex {
            float x, y, z, w;
            float u1, v1, u2, v2, u3, v3, u4, v4, u5, v5, u6, v6;

            public vertex(float _x, float _y, float _z, float _w, float _u, float _v) {
                x=_x; y=_y; z=_z; w=_w;
                u1=_u; v1=_v;
                u2=_u; v2=_v;
                u3=_u; v3=_v;
                u4=_u; v4=_v;
                u5=_u; v5=_v;
                u6=_u; v6=_v;
            }

            public const VertexFormats format=VertexFormats.Transformed|VertexFormats.Texture6;
            public const int StrideSize=64;
        }

        private struct shader {
            public EffectHandle ehSinVar;
            public EffectHandle ehLinVar;
            public EffectHandle ehTicks;

            public Effect effect;
        }

        private static readonly List<shader> effects=new List<shader>();

        private static void Reset() {
            foreach(shader e in effects) e.effect.Dispose();
            effects.Clear();
            if(thisFrameSurface!=null) {
                thisFrameSurface.Dispose();
                thisFrameSurface=null;
            }
            if(lastFrameSurface!=null) {
                lastFrameSurface.Dispose();
                lastFrameSurface=null;
            }
            if(lastShaderSurface!=null) {
                lastShaderSurface.Dispose();
                lastShaderSurface=null;
            }
            if(lastPassSurface!=null) {
                lastPassSurface.Dispose();
                lastPassSurface=null;
            }
            if(thisFrame!=null) {
                thisFrame.Dispose();
                thisFrame=null;
            }
            if(lastFrame!=null) {
                lastFrame.Dispose();
                lastFrame=null;
            }
            if(lastShader!=null) {
                lastShader.Dispose();
                lastShader=null;
            }
            if(lastPass!=null) {
                lastPass.Dispose();
                lastPass=null;
			}
			if (depthFrame != null)
			{
				depthFrame.Dispose();
				depthFrame = null;
			}
            if(vBuffer!=null) {
                vBuffer.Dispose();
                vBuffer=null;
            }
            GC.Collect();
        }

        private static shader LoadShaderTextures(Effect effect) {
            try { effect.SetValue("thisframe", thisFrame); } catch { } //throws an exception if this parameter doesn't exist :(
            try { effect.SetValue("lastframe", lastFrame); } catch { }
            try { effect.SetValue("lastpass", lastPass); } catch { }
            try { effect.SetValue("lastshader", lastShader); } catch { }
            try { effect.SetValue("depthframe", depthFrame); } catch { }
            try { effect.SetValue("rcpres", new Vector4(1.0f/1024.0f, 1.0f/1024.0f, 0, 1)); }
            catch { }
			try { effect.SetValue("HDR", new Vector4(0.5f, 0.5f, 0.5f, 0.5f)); } catch { } // pk edit

            int count=1;
            while(true) {
                string texpath;
                try { texpath=effect.GetValueString("texname"+count.ToString()); } catch { break; }
                if(texpath!=null) {
                    try {
                        Texture tex=TextureLoader.FromFile(DXMain.device, Statics.runDir + "\\data files\\textures\\"+texpath);
                        effect.SetValue("tex"+count.ToString(), tex);
                    } catch { }
                } else break;
                count++;
            }

            shader s=new shader();
            try {
                s.ehSinVar=effect.GetParameter(null, "sinvar");
            } catch {
                s.ehSinVar=null;
            }
            try {
                s.ehLinVar=effect.GetParameter(null, "linvar");
            } catch {
                s.ehLinVar=null;
            }
            try {
                s.ehTicks=effect.GetParameter(null, "tickcount");
            } catch {
                s.ehTicks=null;
            }
            s.effect=effect;

            return s;
        }

        private static string Setup(string pic1, string pic2, string pic3) {
            try {
                vBuffer=new VertexBuffer(typeof(vertex), 4, DXMain.device, Usage.WriteOnly, vertex.format, Pool.Default);
                vertex[] verts=(vertex[])vBuffer.Lock(0, LockFlags.None);
                verts[0]=new vertex(-000.5f, -000.5f, 0, 1, 0, 0);
                verts[1]=new vertex(-000.5f, +1023.5f, 0, 1, 0, 1);
                verts[2]=new vertex(+1023.5f, -000.5f, 0, 1, 1, 0);
                verts[3]=new vertex(+1023.5f, +1023.5f, 0, 1, 1, 1);
                vBuffer.Unlock();
            } catch {
                return "Unable to create vertex buffer";
            }

            try {
                DXMain.device.SetStreamSource(0, vBuffer, 0, 64);
                DXMain.device.VertexFormat=vertex.format;
                DXMain.device.Indices=null;
                DXMain.device.RenderState.CullMode=Cull.None;
            } catch {
                return "Unable to set device state";
            }

            try {
                thisFrame=TextureLoader.FromFile(DXMain.device, pic1==null?Statics.runDir+"\\mge3\\preview.bmp":pic1,
                    1024, 1024, 1, Usage.RenderTarget, DXMain.format, Pool.Default, Filter.Linear, Filter.Linear, 0);
                lastFrame = TextureLoader.FromFile(DXMain.device, pic2 == null ? Statics.runDir + "\\mge3\\preview.bmp" : pic2,
                    1024, 1024, 1, Usage.RenderTarget, DXMain.format, Pool.Default, Filter.Linear, Filter.Linear, 0);
				//lastPass = new Texture(DXMain.device, 1024, 1024, 1, Usage.RenderTarget, DXMain.format, Pool.Default);
                lastPass = new Texture(DXMain.device, 1024, 1024, 1, Usage.RenderTarget, Format.A8R8G8B8, Pool.Default);
                lastShader = TextureLoader.FromFile(DXMain.device, pic1 == null ? Statics.runDir + "\\mge3\\preview.bmp" : pic1,
                                    1024, 1024, 1, Usage.RenderTarget, DXMain.format, Pool.Default, Filter.Linear, Filter.Linear, 0);
                depthFrame = TextureLoader.FromFile(DXMain.device, pic3 == null ? Statics.runDir + "\\mge3\\depth.bmp" : pic3,
                    1024, 1024, 1, Usage.RenderTarget, DXMain.format, Pool.Default, Filter.Linear, Filter.Linear, 0);
                

                if(thisFrame==null||lastFrame==null||lastPass==null||lastShader==null||depthFrame==null) throw new ApplicationException();

                thisFrameSurface = thisFrame.GetSurfaceLevel(0);
                lastFrameSurface = lastFrame.GetSurfaceLevel(0);
                lastPassSurface = lastPass.GetSurfaceLevel(0);
                lastShaderSurface = lastShader.GetSurfaceLevel(0);
            } catch {
                return "Unable to create the required textures";
            }

            return null;
            
        }

        internal static string CompileShader(bool Render, string pic1, string pic2, string pic3, string shaderpath) {
            Reset();

            Effect effect;
            string errors = null;
            try {
                effect=Effect.FromFile(DXMain.device, shaderpath, null, null, ShaderFlags.NotCloneable, null, out errors);
                if(effect==null) throw new ApplicationException();
            } catch {
                if(errors!=null) return errors.Replace("\n",Environment.NewLine);
                else return "Effect load failed";
            }

            try {
                effect.Technique="T0";
                effect.ValidateTechnique("T0");
            } catch {
                return "Error setting effect technique, or shader validation error";
            }
            if(!Render) {
                effect.Dispose();
                return null;
            }

            string s=Setup(pic1, pic2, pic3);
            if(s!=null) return s;

            effects.Add(LoadShaderTextures(effect));
            return null;
        }

        internal static string PreviewShaderChain(string pic1, string pic2, string pic3) {
            Reset();
            string s=Setup(pic1, pic2, pic3);
            if(s!=null) return s;

            byte[] bytes = System.IO.File.ReadAllBytes(Statics.runDir + "\\" + Statics.fn_shadsav);
            int count=0;
            while(count<bytes.Length) {
                string path=Statics.runDir + "\\";
                while(bytes[count++]!=0) path+=(char)bytes[count-1];

                Effect effect;

                try {
                    effect=Effect.FromFile(DXMain.device, path, null, null, ShaderFlags.NotCloneable, null);
                    if(effect==null) throw new ApplicationException();
                } catch {
                    return "Shader '"+path+"' failed to load";
                }

                try {
                    effect.Technique="T0";
                    effect.ValidateTechnique("T0");
                } catch {
                    return "Shader '"+path+"' failed validation";
                }

                effects.Add(LoadShaderTextures(effect));
            }

            return null;
        }

        internal static void EndShader() {
            Reset();
        }

        internal static void RenderFrame(bool UseShader, decimal HDR_val) {
            DXMain.device.Clear(ClearFlags.Target, 255, 0, 0);
            if (UseShader) {
                for (int i = 0; i < 6; i++) DXMain.device.SetTexture(i, lastShader);
                long ticks=DateTime.Now.Ticks;
                float TimePassed=((float)(ticks%1000))/1000.0f;
                LinVar.X=TimePassed;
                LinVar.Y=TimePassed;
                LinVar.Z=TimePassed;
                LinVar.W=1-TimePassed;
                //need to mod to stop it loosing resolution after a few hours. Makes it jerk a bit every minute or so.
                TimePassed=(float)Math.Sin((float)((ticks%60001))*(0.00025f*Math.PI));   //One loop every 8 seconds
                SinVar.X=TimePassed;
                SinVar.Y=TimePassed;
                SinVar.Z=TimePassed;
                SinVar.W=(float)Math.Cos((float)((ticks%60001))*(0.00025f*Math.PI));
                
                DXMain.device.BeginScene();
                for(int i=0;i<effects.Count;i++) {
                    if(effects[i].ehSinVar!=null) effects[i].effect.SetValue(effects[i].ehSinVar, SinVar);
                    if(effects[i].ehLinVar!=null) effects[i].effect.SetValue(effects[i].ehLinVar, LinVar);
                    if(effects[i].ehTicks!=null) effects[i].effect.SetValue(effects[i].ehTicks, (int)(ticks&int.MaxValue));

					// pk edits
					try { effects[i].effect.SetValue("HDR", new Vector4((float)HDR_val, (float)HDR_val, (float)HDR_val, (float)HDR_val)); }
					catch { }
					// end

                    if (i > 0) {
                        DXMain.device.StretchRectangle(lastPassSurface, DXMain.Rect, lastShaderSurface, DXMain.Rect, TextureFilter.None);
                    } else {
                        DXMain.device.StretchRectangle(thisFrameSurface, DXMain.Rect, lastShaderSurface, DXMain.Rect, TextureFilter.None);
                    }
                    int passes = effects[i].effect.Begin(FX.None);
                    for(int pass = 0;pass < passes;pass++) {
                        effects[i].effect.BeginPass(pass);
                        DXMain.device.DrawPrimitives(PrimitiveType.TriangleStrip, 0, 2);
                        DXMain.device.StretchRectangle(DXMain.BackBuffer, DXMain.Rect, lastPassSurface, DXMain.Rect, TextureFilter.None);
                        effects[i].effect.EndPass();
                    }
                    effects[i].effect.End();
                }
                DXMain.device.EndScene();
                DXMain.device.StretchRectangle(DXMain.BackBuffer, DXMain.Rect, lastPassSurface, DXMain.Rect, TextureFilter.None);
            } else {
                for (int i = 0; i < 6; i++) DXMain.device.SetTexture(i, thisFrame);
                DXMain.device.BeginScene();
                DXMain.device.DrawPrimitives(PrimitiveType.TriangleStrip, 0, 2);
                DXMain.device.EndScene();
            }
            DXMain.device.Present(/*window*/);
        }
    }
}
