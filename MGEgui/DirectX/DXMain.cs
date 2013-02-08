using System;
using Point=System.Drawing.Point;
using Control=System.Windows.Forms.Control;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace MGEgui.DirectX {
    static class DXMain {
        public struct MGECaps {
            public int MaxWindowedAA;
            public int MaxFullscreenAA;
            public int MaxAF;
            public bool SupportsSM1;
            public bool SupportsSM2;
            public bool SupportsSM3;
            public int MaxTexSize;
            public int MaxPrimitives;
            public int MaxIndicies;
        }
        private static Format _format;
        private static Caps caps;
        private static int adapter;

        public static MGECaps mCaps = new MGECaps();
        public static readonly System.Drawing.Rectangle Rect = new System.Drawing.Rectangle(0, 0, 1024, 1024);
        public static int Adapter { get { return adapter; } }
        public static Format format { get { return _format; } private set { _format=value; } }

        private static Device _device;
        public static Device device { get { return _device; } private set { _device=value; } }
        private static Surface _backbuffer;
        public static Surface BackBuffer { get { return _backbuffer; } private set { _backbuffer=value; } }

        private static readonly PresentParameters devParams;

        static DXMain() {
            try {
                format = Manager.Adapters[adapter].CurrentDisplayMode.Format;
            } catch {
                format = Format.X8R8G8B8;
            }

            devParams=new PresentParameters();
            devParams.BackBufferCount=1;
            devParams.BackBufferFormat=format;
            devParams.BackBufferHeight=1024;
            devParams.BackBufferWidth=1024;
            devParams.EnableAutoDepthStencil=false;
            devParams.MultiSample=MultiSampleType.None;
            devParams.SwapEffect=SwapEffect.Discard;
            devParams.Windowed=true;
            devParams.PresentationInterval = PresentInterval.One;
        }

        public static void GetDeviceCaps() {
            Device.IsUsingEventHandlers=false;

            adapter=(int)Microsoft.Win32.Registry.GetValue(Statics.reg_key_bethesda.Name + "\\" + Statics.reg_morrowind, "Adapter", 0);
            if(Manager.Adapters.Count <= adapter) throw new ApplicationException("Morrowind is set up to use an adapter which could not be found on your system");

            for(int i=2;i<=16;i++) {
                if(Manager.CheckDeviceMultiSampleType(adapter, DeviceType.Hardware, Format.X8R8G8B8, false, (MultiSampleType)i))
                    mCaps.MaxFullscreenAA=i;
                if(Manager.CheckDeviceMultiSampleType(adapter, DeviceType.Hardware, Format.X8R8G8B8, true, (MultiSampleType)i))
                    mCaps.MaxWindowedAA=i;
            }

            caps = Manager.GetDeviceCaps(adapter, DeviceType.Hardware);

            mCaps.MaxAF=caps.MaxAnisotropy;
            mCaps.SupportsSM1=(caps.VertexShaderVersion>=new Version(1,1))&&(caps.PixelShaderVersion>=new Version(1,4));
            mCaps.SupportsSM2=(caps.VertexShaderVersion.Major>=2)&&(caps.PixelShaderVersion.Major>=2);
            mCaps.SupportsSM3=(caps.VertexShaderVersion.Major>=3)&&(caps.PixelShaderVersion.Major>=3);
            mCaps.MaxTexSize=Math.Min(caps.MaxTextureHeight, caps.MaxTextureWidth);
            mCaps.MaxPrimitives=caps.MaxPrimitiveCount;
            mCaps.MaxIndicies=caps.MaxVertexIndex;
        }

        public static int[] GetRefreshRates(int width, int height) {
            System.Collections.Generic.List<int> rates=new System.Collections.Generic.List<int>();
            foreach(DisplayMode ds in Manager.Adapters[adapter].SupportedDisplayModes) {
                if(ds.Format==Format.X8R8G8B8 && ds.Width==width && ds.Height==height) rates.Add(ds.RefreshRate);
            }
            return rates.ToArray();
        }

        public static Point[] GetResolutions() {
            System.Collections.Generic.List<Point> resolutions=new System.Collections.Generic.List<Point>();
            foreach(DisplayMode ds in Manager.Adapters[adapter].SupportedDisplayModes) {
                if(ds.Format==Format.X8R8G8B8 && !resolutions.Contains(new Point(ds.Width, ds.Height)))
                    resolutions.Add(new Point(ds.Width, ds.Height));
            }
            return resolutions.ToArray();
        }

        public static void CreateDevice(Control window) {
            CloseDevice();

            device = new Device(adapter, DeviceType.Hardware, window, CreateFlags.FpuPreserve|CreateFlags.MultiThreaded|CreateFlags.HardwareVertexProcessing, devParams);

            //BackBuffer=device.GetBackBuffer(0, 0, BackBufferType.Mono);
            BackBuffer=device.GetRenderTarget(0);

            device.RenderState.Lighting = false;
            device.RenderState.ZBufferEnable = false;
            device.RenderState.StencilEnable = false;
            device.RenderState.AmbientColor = int.MinValue;
        }

        public static void CloseDevice() {
            BackBuffer=null;
            if(device!=null) {
                device.Dispose();
                device=null;
            }
        }

        public static bool CheckAALevel(int level, bool windowed) {
            return Manager.CheckDeviceMultiSampleType(adapter, DeviceType.Hardware, Format.X8R8G8B8, windowed, (MultiSampleType)level);
        }
    }
}