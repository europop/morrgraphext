using System;
using CancelEventArgs = System.ComponentModel.CancelEventArgs;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using RegistryKey = Microsoft.Win32.RegistryKey;
using Registry = Microsoft.Win32.Registry;
using System.IO;
using MGEgui.DirectX;
using MGEgui.INI;
using MGEgui.Localization;

namespace MGEgui {

    public partial class MainForm : Form {

        private bool Save = true;

        private bool loading = true;

        private string clearedSSDir;

        System.Drawing.Point gbMainSettingsLocation;
        System.Drawing.Point gbMainUILocation;

        public MainForm (LocalizationInterface.Localization Language, bool AutoLang) {
            InitializeComponent ();
            //Attach dud context menus to combo boxes
            cmbAntiAlias.ContextMenu = DudMenu;
            cmbAnisoLevel.ContextMenu = DudMenu;
            cmbFogMode.ContextMenu = DudMenu;
            cmbMipFilter.ContextMenu = DudMenu;
            cmbScaleFilter.ContextMenu = DudMenu;
            cmbVWait.ContextMenu = DudMenu;
            cmbZBuffer.ContextMenu = DudMenu;
            cmbSShotFormat.ContextMenu = DudMenu;
            cmbTipReadSpd.ContextMenu = DudMenu;
            cmbMsgsLocale.ContextMenu = DudMenu;
            cmbUILanguage.ContextMenu = DudMenu;
            // Save default tabpage tooltips
            foreach (TabPage page in tabControl.TabPages) tabpage_tooltips.Add (page, page.ToolTipText);
            //Set initial directories
            this.OpenFileDialog.InitialDirectory = this.SaveFileDialog.InitialDirectory = Statics.runDir + "\\MGE3";
            this.OpenTangentMesh.InitialDirectory = Statics.runDir + "\\Data files\\MGE meshes";
            //Title
            Text = "Morrowind Graphics Extender (" + Statics.versionString + ")";
            // Set default tooltip text reading speed
            try {
                Statics.tipReadSpeed = (int)(1000 / double.Parse (cmbTipReadSpd.Text));
            } catch {
                Statics.tipReadSpeed = (int)(1000 / double.Parse (cmbTipReadSpd.Text.Replace(".", System.Globalization.NumberFormatInfo.CurrentInfo.NumberDecimalSeparator)));
            }
            // set a handler for each control which sets tooltip popup timeout dependent on tooltip text length
            Control [] controls = Statics.GetChildControls (this);
            foreach (Control control in controls) {
                if (!(control is ComboBox || control is TextBox || control is NumericUpDown) || control.Name == "tbSShotDir") default_text.Add (control, control.Text);
                control.MouseHover += new EventHandler (TooltipTimeoutHandler);
            }
            //Save control text
            clearedSSDir = tbSShotDir.Text;
            //Get available UI localizations and set UI language
            cmbUILanguage.Items.AddRange (Statics.Localizations.Languages);
            if (AutoLang) cbUILangAuto.Checked = true;
            else if (Language != null) cmbUILanguage.SelectedIndex = cmbUILanguage.FindStringExact (Language.Language);
            SetTooltips ();
            //Store default locations
            gbMainSettingsLocation = gbMainSettings.Location;
            gbMainUILocation = gbMainUI.Location;
            gbMainUILocation.Y -= this.Size.Height;
            //Load settings
            LoadSettings ();
            try {
                RegistryKey key = Registry.LocalMachine.OpenSubKey (Statics.reg_mw);
                uint width, height;
                try {
                    width = (uint)(int)key.GetValue ("Screen Width");
                    height = (uint)(int)key.GetValue ("Screen Height");
                } catch {
                    MessageBox.Show (strings ["RegScrRes"].text, Statics.strings ["Error"].text);
                    width = 640;
                    height = 480;
                }
                tbResolution.Text = width.ToString () + " x " + height.ToString ();
                CalcAspectRatio ((int)width, (int)height);
                try {
                    cbWindowed.Checked = !Convert.ToBoolean (((byte [])key.GetValue ("Fullscreen")) [0]);
                } catch {
                    MessageBox.Show (strings ["RegWinMode"].text, Statics.strings ["Error"].text);
                    cbWindowed.Checked = false;
                }
                key.Close ();
            } catch { }
        }

        public Dictionary<Control, string> default_text = new Dictionary<Control, string> ();

        #region strings
        public static Dictionary<string, Str> strings = new Dictionary<string, Str> {
            {"About", new Str (
                "About")},
            {"ImportSet", new Str (
                "Import settings")},
            {"RegScrRes", new Str (
                "Unable to read screen resolution from registry.")},
            {"RegWinMode", new Str (
                "Unable to read window mode from registry.")},
            {"ImpOldSet", new Str (
                "Old graphics save file was found.\n\nDo you want to import settings?")},
            {"ErrImpSet", new Str (
                "Error importing old graphics save file.\n\n{0}")},
            {"SetTooOld", new Str (
                "The graphics save file is too old to be imported.")},
            {"SetTooNew", new Str (
                "The graphics save file is newer than this version of MGE.")},
            {"OldSetImp", new Str (
                "Old graphics save file was imported successfully.\n\n'settings' file was renamed to 'old-settings'.")},
            {"ErrLdInp", new Str (
                "Error loading input save file.\n\n{0}")},
            {"InpTooOld", new Str (
                "The input save file is too old to import.")},
            {"InpTooNew", new Str (
                "The input save file is newer than this version of MGE.")},
            {"OldInpImp", new Str (
                "Old input save file was imported successfully.")},
            {"MacrTrigErr", new Str (
                "An error occurred while loading macros or triggers. Some data may have been lost.")},
            {"RegNotWrit", new Str (
                "Could not write Morrowind registry key.\n" +
                "MGEgui needs to be launched as Administrator.")},
            {"MWIniNotInMGE", new Str (
                "Settings file does not contain ini data.")},
            {"GraphCaps", new Str (
                "Graphics capabilities")},
            {"ViewCaps", new Str (
                "Max fullscreen antialiasing level: {0}x\n" +
                "Max windowed antialiasing level: {1}x\n" +
                "Max anisotropic filtering level: {2}x\n\n" +
                "Graphics drivers often report their capabilities incorrectly.\n" +
                "If you think your graphics card only supports lower levels of AA,\n" +
                "then you will need to reduce it by hand if you use best quality mode.")},
            {"CreatedBy", new Str (
                "Created by")},
            {"FurtherDevel", new Str (
                "Further development")},
            {"DLVertexFog", new Str (
                "Distant land is not compatible with vertex fog when using some graphics cards.\n" +
                "If the distant land does not appear in game, or appears untextured, try switching back to pixel fog.")},
            {"TooHighAA", new Str (
                "Your selected antialiasing level is not supported by your graphics card.")},
            {"TooHighAF", new Str (
                "Your selected anisotropic filtering level is not supported by your graphics card.")},
            {"DSMissing", new Str (
                "The distant statics files are missing.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them.")},
            {"DLLackSM2", new Str (
                "Your graphics card lacks the Shader Model 2.0 support required to use Distant Land.")},
            {"NoDLOrOld", new Str (
                "Distant Land files have not been created or are from an older version of MGE.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them.")},
            {"DLDiffer", new Str (
                "Distant land files have not been created or are from a different version of MGE.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them.")},
            {"ResetAsk", new Str (
                "Reset Settings?")},
            {"AskReset", new Str (
                "Are you sure you want to reset all main {0}settings to default values?")},
            {"ResetSaved", new Str (
                "and other saved ")},
            {"DLDelOldCorrupt", new Str (
                "Distant land files already exist, but are either incomplete, corrupt, or from an older version of MGE and must be deleted to continue.\n" +
                "Do you wish to continue?")},
            {"DLDelDifferent", new Str (
                "Distant land files already exist, but are from a different version of MGE and must be deleted to continue.\n" +
                "Do you wish to continue?")},
        };
        #endregion

        #region tooltip_messages
        public Dictionary<TabPage, string> tabpage_tooltips = new Dictionary<TabPage, string> ();
        public Dictionary<string, Tip> tooltip_messages = new Dictionary<string, Tip> {
        /* Main */
            {"DisableMGE", new Tip (new string [] {"cbDisableMGE"},
                "Disables hooking of DirectX input and 3D interface by Morrowind Graphics Extender.\n" +
                "Note that this doesn't disable MGE's internal version of MWSE, so you don't need to run its external version.")},
            {"ClearINI", new Tip (new string [] {"cbClearINI"},
                "Checking this will cause reinitialization of main settings file after resetting settings.\n" +
                "Note that all comments and deprecated settings added to that file will be lost.")},
            {"ResetSettings", new Tip (new string [] {"bResetSettings"},
                "Resets MGE settings back to defaults.")},
            {"Quality", new Tip (new string [] {"bQuality"},
                "Sets graphics options for the best visual quality.")},
            {"Help", new Tip (new string [] {"bHelp"},
                "Opens the help file.")},
            {"Import", new Tip (new string [] {"bImport"},
                "Import an MGE settings file.\n" +
                "To import Morrowind.ini as well, ensure that 'Import Morrowind.ini' is checked.")},
            {"Export", new Tip (new string [] {"bExport"},
                "Export your MGE settings and Morrowind.ini.")},
            {"ImportIni", new Tip (new string [] {"cbImportIni"},
                "If this is checked, Morrowind.ini will be imported along with MGE settings.")},
            {"ViewCaps", new Tip (new string [] {"bViewCaps"},
                "Displays your graphics card's maximum supported antialiasing and anisotropic filtering levels.")},
            {"About", new Tip (new string [] {"bAbout"},
                "Shows information about program.")},
            {"UILanguage", new Tip (new string [] {"lUILanguage", "cmbUILanguage"},
                "Sets translation language for user interface of MGEgui.")},
            {"UILangAuto", new Tip (new string [] {"cbUILangAuto"},
                "Checking this, enables autodetection of operating system's selected language,\n" +
                "for choosing automatically appropriate translation for user interface of MGEgui.")},
            {"TipReadSpd", new Tip (new string [] {"lTipReadSpd", "cmbTipReadSpd", "lTipRSpdUnit"},
                "This changes the fadeout time of MGEgui tooltips for selected reading speed (in characters per second).\n" +
                "For example, if you select 10 chars/sec. and some tooltip's text is 50 characters long (including spaces and new line characters),\n" +
                "that tooltip will be displayed for 5 seconds. Note that any tooltip can't be displayed for longer than about 33 seconds.\n" +
                "Timeout of tooltips for tab panel selection doesn't depend on its text length and isn't affected by this setting.")},
        /* Global graphics */
            {"RefreshRate", new Tip (new string [] {"cmbRefreshRate", "lRefreshRate"},
                "Sets the monitor refresh rate for fullscreen mode.\n" +
                "This has no effect in windowed mode.")},
            {"AntiAlias", new Tip (new string [] {"cmbAntiAlias", "lAntiAlias"},
                "Sets the antialiasing level.\n" +
                "If you set this above what your graphics card supports, you will get a render creation error when starting up Morrowind.\n" +
                "If you set this to anything other than none, you will not be able to create a lockable backbuffer.")},
            {"ZBuffer", new Tip (new string [] {"cmbZBuffer", "lZBuffer"},
                "Sets the z-buffer format.\n" +
                "If you set this to something that your graphics card doesn't support, you will get a render creation error when starting up Morrowind.\n" +
                "Using D16 or D16L will cause graphical artifacts while playing Morrowind.")},
            {"VWait", new Tip (new string [] {"cmbVWait", "lVWait"},
                "Sets the vertical refresh setting.\n" +
                "Setting this to 1, 2, 3, or 4 will cause an FPS hit, but will remove tearing artifacts.\n" +
                "In DirectX 9, which is only currently supported by MGE, 'Default (1)' is equivalent to '1'.\n" +
                "Setting it to more than 1 in windowed mode will result in using 'Default (1)' setting.")},
            {"CalcRefresh", new Tip (new string [] {"bCalcRefresh"},
                "Finds the valid refresh rates for Morrowind's currently selected resolution.\n" +
                "Valid refresh rates depend on resolution, adapter, and whether 16 bit colour mode is enabled.\n" +
                "Changes committed by MGEgui make an auto-search of valid refresh rates.")},
            {"Windowed", new Tip (new string [] {"cbWindowed"},
                "Check this to run Morrowind in a window instead of fullscreen.")},
            {"Resolution", new Tip (new string [] {"tbResolution", "lResolution"},
                "This is Morrowind's current screen resolution.")},
            {"CalcResolution", new Tip (new string [] {"bCalcResolution"},
                "Click this to change Morrowind's resolution.\n" +
                "In windowed mode you can use any resolution you like.\n" +
                "Non 4:3 aspect ratios may not work correctly. These are marked with an '*'.")},
            {"Aspect", new Tip (new string [] {"tbAspect", "lAspect"},
                "This is selected screen resolution calculated aspect ratio.")},
        /* Render state */
            {"ScaleFilter", new Tip (new string [] {"cmbScaleFilter", "lScaleFilter"},
                "Sets the min and mag texture filters.\n" +
                "If you set a filter not supported by your card, Morrowind will automatically use the next best available filter.\n" +
                "Do not set this to 'point' if you are using Morrowind's pixel-shaded water.")},
            {"MipFilter", new Tip (new string [] {"cmbMipFilter", "lMipFilter"},
                "Sets the mip texture filter.\n" +
                "If you set a filter not supported by your card, Morrowind will automatically use the next best available filter.")},
            {"AnisoLevel", new Tip (new string [] {"cmbAnisoLevel", "lAnisoLevel"},
                "Sets the anisotropic filtering level.\n" +
                "This will only have an effect if you use anisotropic filtering for either scale or mip filters.\n" +
                "If you set this to above what your card supports, it will automatically drop down to the highest supported level.")},
            {"LOD", new Tip (new string [] {"udLOD", "lLOD"},
                "Sets the mipmap level of detail bias. Valid range is between -12 and 12.\n" +
                "Negative values increase detail, positive values reduce it.\n" +
                "Very low values will cause aliasing of textures in the background.")},
            {"FogMode", new Tip (new string [] {"cmbFogMode", "lFogMode"},
                "Sets the type of fog that Morrowind uses.\n" +
                "Using vertex fog on some ATI cards may cause everything except the sky to become completely fogged.")},
            {"Fog", new Tip (new string [] {"cbFog"},
                "Uncheck this to disable fogging in Morrowind.")},
            {"FPSCounter", new Tip (new string [] {"cbFPSCounter"},
                "Enables MGE's FPS counter.\n" +
                "This is unrelated to Morrowind's FPS counter, and can be used instead of or as well as it.\n" +
                "Display messages must be checked for this to work.")},
            {"Zoom", new Tip (new string [] {"udZoom", "lZoom"},
                "Sets the initial level of zoom.\n" +
                "Changing this will not enable zoom. You must bind the 'toggle zoom' function to a key in the macro editor.")},
            {"Rotation", new Tip (new string [] {"udRotation", "lRotation"},
                "Sets the initial screen rotation clockwise from normal in degrees.\n" +
                "Rotation can be changed in 90 degree steps by using up/down buttons on the control.")},
            {"AAFix", new Tip (new string [] {"cbAAFix"},
                "Fixes the monochrome backgrounds you get when loading or saving when using antialiasing.")},
            {"DisplayMessages", new Tip (new string [] {"cbDisplayMessages"},
                "Allows MGE to draw text to the screen.\n" +
                "As well as the FPS counter, MGE will also alert you when various graphics options get changed.")},
            {"HWShader", new Tip (new string [] {"cbHWShader"},
                "Enables fullscreen pixel shaders.\n" +
                "Some shaders will make it difficult to view menus, so it may be better to use\n" +
                "the macro editor's 'toggle HW shader' function.")},
            {"DisableShadersInMenu", new Tip (new string [] {"cbDisableShadersInMenu"},
                "Disables fullscreen pixel shaders in menu.\n" +
                "Some shaders might decrease framerate in menu mode and cause input lag.\n" +
                "Checking this will disable shaders in loading screens and menus.")},
            {"SShotFormat", new Tip (new string [] {"cmbSShotFormat", "lSShotFormat"},
                "Sets the format in which MGE will save screenshots.\n" +
                "This does not change the format in which Morrowind saves screenshots.")},
            {"SShotName", new Tip (new string [] {"lSShotName", "tbSShotName"},
                "Every saved MGE screenshot will start its file name with this prefix.")},
            {"SShotDir", new Tip (new string [] {"lSShotDir", "tbSShotDir"},
                "This is output directory where taken screenshots will be saved.")},
            {"SShotNum", new Tip (new string [] {"lSShotNum", "udSShotNum"},
                "Minimum number of characters for screenshot number.\n" +
                "Setting it to more than 1 will result in leading zeros if screenshot number use less characters than set here,\n" +
                "e.g.: if it's set to 3 and first screenshot is saved to output directory, the resulting screenshot number is '001'.")},
            {"SShotDirBrowse", new Tip (new string [] {"bSShotDirBrowse"},
                "Use it for selecting output directory where you want the screenshots to be saved.")},
            {"SShotDirClear", new Tip (new string [] {"bSShotDirClear"},
                "Changes selected output directory for screenshots to default.\n" +
                "Screenshots will be saved to Morrowind installation directory.")},
            {"FOV", new Tip (new string [] {"udFOV", "lFOV"},
                "Adjusts the in-game horizontal field of view (FOV).")},
            /*{"MsgsLanguage", new Tip (new string [] {"lMsgsLanguage", "cmbMsgsLanguage"},
                "NEEDS A DESCRIPTION.")},/**/
            {"MsgsTime", new Tip (new string [] {"lMsgsTime", "udMsgsTime", "lMsgsTimeUnit"},
                "Changes the maximum length of time for which MGE status messages will be displayed on screen in the game.\n" +
                "Units used for this setting are miliseconds, so to make MGE messages displayed for 5 seconds, you need to set it to 5000.")},
        /* Input */
            {"SkipMovie", new Tip (new string [] {"cbSkipMovie"},
                "Skips the two opening movies.")},
            {"Console", new Tip (new string [] {"cbConsole"},
                "Disables the console.")},
            {"AltCombat", new Tip (new string [] {"cbAltCombat"},
                "Allows you to use Daggerfall style combat controls.\n" +
                "You must bind the 'toggle alt combat' function to a key in the macro editor to use this.")},
            {"LagFix", new Tip (new string [] {"cbLagFix"},
                "Attempts to fix input lag.\n" +
                "This will cause a small graphical artifact in the top left corner of the screen, and may permanently mark your minimap.\n" +
                "Turning on MGE's FPS counter is usually sufficient to remove input lag caused by AA or shadows.")},
            {"MacroEd", new Tip (new string [] {"bMacroEd"},
                "Opens up the macro editor, allowing you to bind functions and macros to keyboard keys and mouse buttons.")},
            {"Remapper", new Tip (new string [] {"bRemapper"},
                "Opens up the keyboard remapper, with which you can remap any key on your keyboard.")},
        /* Misc */
            {"ZoomEffectsHUD", new Tip (new string [] {"cbZoomEffectsHUD"},
                "Check this if you want zoom and screen rotation to affect the HUD.")},
            {"Idle", new Tip (new string [] {"cbIdle"},
                "Check this to force Morrowind to give up control of the cpu once a frame.")},
            {"IdleTime", new Tip (new string [] {"udIdle", "lIdle"},
                "Controls how long Morrowind gives up the cpu. Valid range is 0 to 1000.\n" +
                "Value of 0 causes Morrowind to resume immediately if no other tasks are waiting.")},
            {"MaxView", new Tip (new string [] {"udMaxView", "lMaxView"},
                "Changes the maximum view distance that MGE will set.\n" +
                "This setting only affects the 'keep FPS constant' setting.\n" +
                "Morrowind default is 7168.")},
            {"MinView", new Tip (new string [] {"udMinView", "lMinView"},
                "Changes the minimum view distance that MGE will set.\n" +
                "This setting only affects the 'keep FPS constant' setting.\n" +
                "Morrowind default is 2500.")},
            {"SetEffectVars", new Tip (new string [] {"cbSetEffectVars"},
                "If this box is unchecked, the sinvar, linvar, tickcount, eyevec, sunvec and suncol variables will be unavailable to shaders.\n" +
                "Unchecking this will give a performance boost, especially if you run many simultaneous shaders.")},
            {"FPSConst", new Tip (new string [] {"cbFPSConst"},
                "Check this to allow MGE to modify view distance in order to keep a constant FPS.\n" +
                "MGE will only modify view distance. No other graphics options will be changed.")},
            {"FPSDes", new Tip (new string [] {"udFPSDes", "lFPSDes"},
                "The FPS MGE should try to obtain.")},
            {"FPSTol", new Tip (new string [] {"udFPSTol", "lFPSTol"},
                "The amount that your FPS needs to be above or below your desired amount before MGE modifies framerate.")},
            {"BindAI", new Tip (new string [] {"cbBindAI"},
                "Checking this box will bind the AI distance to your view distance.\n" +
                "Increasing view distance with MGE will automatically increase AI distance.")},
            {"HDR", new Tip (new string [] {"cbHDR"},
                "Check this box to supply HDR information to shaders.\n" +
                "Note that checking this box by itself will not enable HDR.\n" +
                "You also need to add a HDR shader to the active shader chain.\n" +
                "Checking this box may cause an FPS hit.")},
            {"HDRTime", new Tip (new string [] {"udHDR", "lHDR"},
                "This box controls how fast the HDR reacts to brightness changes.\n" +
                "The lower time is set, the reaction is faster.")},
            {"Hook", new Tip (new string [] {"cbHook"},
                "Enables MGE texture hooking. Required for mods which attach effects to existing Morrowind objects.\n" +
                "Checking this option may increase loading times.")},
            {"AspectZoom", new Tip (new string [] {"cbAspectZoom"},
                "Checking this will enable corrections of aspect ratio while zooming.")},
            {"DisableMWSE", new Tip (new string [] {"cbDisableMWSE"},
                "Checking this option disables MGE's internal version of MWSE functions.\n" +
                "This allows you to run MGE with an external version of MWSE.\n" +
                "Note that some MGE mods may require MGE's internal version of MWSE,\n" +
                "and will not work with external versions.")},
            {"DisableMWE", new Tip (new string [] {"cbDisableMWE"},
                "Checking this option disables MGE's internal version of MWE.")},
            {"ShaderDepth", new Tip (new string [] {"cbShaderDepth"},
                "Checking this will supply hardware shaders with screen depth information.\n" +
                "Checking this box may cause an FPS hit.")},
            {"Cam3rdPrsn", new Tip (new string [] {"cbCam3rdPrsn"},
                "Check this to customize parameters of 3rd person player camera.\n" +
                "Parameters set by this option will override the game defaults on start of Morrowind.\n" +
                "This option doesn't need to be set, to change the position of camera, by either a macro\n" +
                "function, or by a script. Scripts can switch off overriding of camera parameters.")},
            {"Cam3rdX", new Tip (new string [] {"lCam3rdX", "udCam3rdX"},
                "This sets the horizontal offset of 3rd person camera from player character's eyes.\n" +
                "Negative values move the camera to the left, and positive to the right.")},
            {"Cam3rdY", new Tip (new string [] {"lCam3rdY", "udCam3rdY"},
                "This sets the offset of 3rd person camera from player character's eyes along the direction of sight.\n" +
                "More negative values move the camera farther back, and less negative values move it forth, closer to player character.\n" +
                "Only negative values are used, because this setting, as also corresponding macro functions, allow only placing the 3rd\n" +
                "person camera behind the character. To set the camera ahead of character's eyes, you need to create a script doing it.")},
            {"Cam3rdZ", new Tip (new string [] {"lCam3rdZ", "udCam3rdZ"},
                "This sets the vertical offset of 3rd person camera from player character's eyes.\n" +
                "Negative values move the camera down, and positive values move the camera up.")},
        /* Morrowind.ini */
            {"Screenshots", new Tip (new string [] {"cbScreenshots"},
                "The 'Screen Shot Enable' line.\n" +
                "Check this to enable Morrowind's inbuilt screenshot function.")},
            {"YesToAll", new Tip (new string [] {"cbYesToAll"},
                "The 'AllowYesToAll' line.\n" +
                "Check this to add an additional 'yes to all' option when clicking through multiple warning messages.")},
            {"HQShadows", new Tip (new string [] {"cbHQShadows"},
                "The 'High Detail Shadows' line.\n" +
                "Check this to enable much better quality shadows.\n" +
                "This will cause a huge FPS hit.")},
            {"ThreadLoad", new Tip (new string [] {"cbThreadLoad"},
                "The 'DontThreadLoad' line.\n" +
                "Check this to allow Morrowind to load new cells in a separate thread.")},
            {"ShowFPS", new Tip (new string [] {"cbShowFPS"},
                "The 'Show FPS' line.\n" +
                "Check this to enable Morrowind's inbuilt FPS counter.")},
            {"Audio", new Tip (new string [] {"cbAudio"},
                "The 'Disable Audio' line.\n" +
                "Check this to disable most of Morrowind's audio. Music is unaffected by this setting.")},
            {"Subtitles", new Tip (new string [] {"cbSubtitles"},
                "The 'Subtitles' line.\n" +
                "Check this to display subtitles where normally there would just be a voice over.")},
            {"HitFader", new Tip (new string [] {"cbHitFader"},
                "The 'ShowHitFader' line.\n" +
                "Uncheck this to disable the red hit fader that appears when you take damage.")},
            {"MultipleCS", new Tip (new string [] {"cbMultipleCS"},
                "The 'AllowMultipleEditors' line.\n" +
                "Check this to allow multiple copies of the construction set to run simultaneously.")},
        /* Tools */
            {"ShaderEd", new Tip (new string [] {"bShaderEd"},
                "A shader editor for working on fullscreen shaders.")},
            {"HookGen", new Tip (new string [] {"bHookGen"},
                "Generates fake texture files which will be intercepted by MGE when Morrowind attempts to load them.")},
            {"DistantLandWizard", new Tip (new string [] {"bDistantLandWizard"},
                "This will launch the process that creates the files that distant land needs to work.\n" +
                "You cannot enable distant land without running it.")},
        /* Distant Land */
            {"DLDistantLand", new Tip (new string [] {"cbDLDistantLand"},
                "Check this box to enable the distant land feature.\n" +
                "This renders landscape and objects beyond Morrowind's normal drawing distance.\n" +
                "Note that Morrowind's pixel/vertex shader water will be disabled if you enable this,\n" +
                "and instead of it Distant Land's water shader will be used.\n" +
                "Morrowind's pixel shader will be re-enabled by MGEgui if you disable MGE or Distant Land.")},
            {"DLDrawDist", new Tip (new string [] {"udDLDrawDist", "lDLDrawDist"},
                "This is the maximum distance in cells that distant land will draw anything.\n" +
                "Anything beyond this distance will be cut off.")},
            {"DLAutoDist", new Tip (new string [] {"cbDLAutoDist"},
                "This will automatically set other than selected cutoff distances, depending on the selected base.\n" +
                "This way you don't have to think about things like fog, distant land draw distance or static distances, unless you really want to.")},
            {"DLAutoByDrawDist", new Tip (new string [] {"rbDLAutoByDrawDist"},
                "This will automatically set other cutoff distances based on your Draw Distance setting.")},
            {"DLAutoByAFogEnd", new Tip (new string [] {"rbDLAutoByAFogEnd"},
                "This will automatically set other cutoff distances based on your Above Water Fog End setting.")},
            /*{"DLBlur", new Tip (new string [] {"udDLBlur", "lDLBlur"},
                "This controls how many blur passes will be applied to far away objects.\n" +
                "Set higher for more blurring and set to zero to disable.\n" +
                "Note that this will disable AA in the areas rendered by distant land.")},/* REMOVED */
            {"DLDistantStatics", new Tip (new string [] {"cbDLDistantStatics"},
                "This enables the drawing of far off objects such as trees and buildings.")},
            {"DLNearSize", new Tip (new string [] {"tbDLNearSize"},
                "This reminds you what minimum size you selected while generating distant statics.\n" +
                "To change it you will need to generate statics again.")},
            {"DLDistNear", new Tip (new string [] {"udDLDistNear"},
                "This is the distance in cells where you will stop seeing 'small size' statics.")},
            {"DLSizeFar", new Tip (new string [] {"udDLSizeFar"},
                "This is the minimum size in Morrowind units of an object that will be considered 'medium sized'.")},
            {"DLDistFar", new Tip (new string [] {"udDLDistFar"},
                "This is the distance in cells where you will stop seeing 'medium size' statics.")},
            {"DLSizeVeryFar", new Tip (new string [] {"udDLSizeVeryFar"},
                "This is the minimum size in Morrowind units of an object that will be considered 'large sized'.")},
            {"DLDistVeryFar", new Tip (new string [] {"udDLDistVeryFar"},
                "This is the distance in cells where you will stop seeing 'large size' statics.")},
            {"DLFogAStart", new Tip (new string [] {"udDLFogAStart"},
                "This sets the distance in cells where fog will begin to affect objects above water in clear weather.")},
            {"DLFogAEnd", new Tip (new string [] {"udDLFogAEnd"},
                "This sets the distance in cells where objects will be completely hidden by fog above water in clear weather.")},
            {"DLFogBStart", new Tip (new string [] {"udDLFogBStart"},
                "This sets the distance in cells where fog will begin to affect objects below water.")},
            {"DLFogBEnd", new Tip (new string [] {"udDLFogBEnd"},
                "This sets the distance in cells where objects will be completely hidden by fog below water.")},
            {"DLFogIStart", new Tip (new string [] {"udDLFogIStart"},
                "This sets the distance in cells where fog will begin to affect objects in interiors with generated Distant Statics.")},
            {"DLFogIEnd", new Tip (new string [] {"udDLFogIEnd"},
                "This sets the distance in cells where objects will be completely hidden by fog in interiors with generated Distant Statics.")},
            {"DLReflLand", new Tip (new string [] {"cbDLReflLand"},
                "This determines whether the landscape drawn by distant land will be reflected in the water.")},
            {"DLReflNStatics", new Tip (new string [] {"cbDLReflNStatics"},
                "Enabling this will make nearby objects reflect in the water.\n" +
                "Note that this is expensive and may lower your frame rate.")},
            {"DLReflFStatics", new Tip (new string [] {"cbDLReflFStatics"},
                "Enabling this will make objects that are a bit further away reflect in the water.\n" +
                "Note that this is expensive and may lower your frame rate.")},
            {"DLWthr", new Tip (new string [] {"bDLWthr"},
                "Distant Land settings for all Morrowind weather types.\n" +
                "You can set there fog and wind ratios for each weather.")},
            {"DLOptions", new Tip (new string [] {"bDLOptions"},
                "Allows additional Distant Land options to be customized.")},
            {"DLSkyRefl", new Tip (new string [] {"cbDLSkyRefl"},
                "This determines whether the sky will be reflected in the water.")},
            {"DLDynRipples", new Tip (new string [] {"cbDLDynRipples"},
                "When checking this, rain drops and the player will create ripples on the water surface.")},
            {"DLSM20", new Tip (new string [] {"rbDLSM20"},
                "This sets Shader Model 2.0 as a default shader for Distant Land.\n" +
                "Note that this setting doesn't force the use of selected Shader Model for Distant Land.\n" +
                "It only suggests the maximum shader model, which should be used for Distant Land, Distant Statics, or Water.\n" +
                "Support for Shader Model 2.0 may be removed, if any part of Distant Land requires use of higher shaders in the future.")},
            {"DLSM30", new Tip (new string [] {"rbDLSM30"},
                "This sets Shader Model 3.0 as a default shader for Distant Land.\n" +
                "Note that this setting doesn't force the use of selected Shader Model for Distant Land.\n" +
                "It only suggests the maximum shader model, which should be used for Distant Land, Distant Statics or Water.")},
            {"DLReflBlur", new Tip (new string [] {"cbDLReflBlur"},
                "This determines whether reflections in the water should be blurred.\n" +
                "Note that setting this option might cause an FPS hit in game.")},
            {"DLFogExp", new Tip (new string [] {"cbDLFogExp"},
                "This enables an alternative exponential fog formula for Distant Land.\n" +
                "When using exponential fog, minimum value of Distant Land draw distance will be\n" +
                "Exponential Distance Multiplier times more than above water fog end distance.\n" +
                "Note that setting this option might cause an FPS hit in game.")},
            {"DLFogExpMul", new Tip (new string [] {"lDLFogExpMul", "udDLFogExpMul"},
                "This adjusts the exponential fog distance multiplier, which is used for multiplying draw distance,\n" +
                "to make distant land don't pop in and out too early, when exponential fog is used.\n" +
                "Note that high values will cause high FPS hit, and low values may cause distant land cutoff before fog end.\n" +
                "The most safe and the default setting, that will ensure exponential fog end and distant land drawing end\n" +
                "in about the same place, is 4.0.")},
            {"DLWtrCaust", new Tip (new string [] {"udDLWtrCaust", "lDLWtrCaust"},
                "This sets the percentage of water caustic lighting intensity.\n" +
                "Setting it to 0 will disable caustics. The default value is 50.\n" +
                "Note that this feature isn't available when using Shader Model 2.0 Distant Land.")},
            {"DLWtrWave", new Tip (new string [] {"udDLWtrWave", "lDLWtrWave"},
                "This sets the height of water waves.\n" +
                "Graphics card with vertex texture fetch support is required for this feature.\n" +
                "Setting it to 0 will disable waves. Suggested value for wave height is 30-50.\n" +
                "Note that this feature isn't available when using Shader Model 2.0 Distant Land.")},
        };
        #endregion

        private void SetTooltips () {
            foreach (KeyValuePair<string,Tip> tip in tooltip_messages) {
                foreach (string controlName in tip.Value.controls) {
                    Control [] controls = this.Controls.Find (controlName, true);
                    foreach (Control control in controls) toolTip.SetToolTip (control, tip.Value.tip);
                }
            }
        }

        private const string smwiniGeneral = "General";

        #region morrowind_ini_variable_definitions
        private static INIFile.INIVariableDef mwiniSSEnable = new INIFile.INIVariableDef ("SSEnable", smwiniGeneral, "Screen Shot Enable", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniNoThread = new INIFile.INIVariableDef ("NoThread", smwiniGeneral, "DontThreadLoad", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniYesToAll = new INIFile.INIVariableDef ("YesToAll", smwiniGeneral, "allowyestoall", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniHDShadow = new INIFile.INIVariableDef ("HDShadow", smwiniGeneral, "High Detail Shadows", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniShowFPS = new INIFile.INIVariableDef ("ShowFPS", smwiniGeneral, "Show FPS", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniAudioOff = new INIFile.INIVariableDef ("AudioOff", smwiniGeneral, "Disable Audio", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniSubtitles = new INIFile.INIVariableDef ("Subtitles", smwiniGeneral, "Subtitles", INIFile.INIBoolType.Number, "0");
        private static INIFile.INIVariableDef mwiniHitFader = new INIFile.INIVariableDef ("HitFader", smwiniGeneral, "ShowHitFader", INIFile.INIBoolType.Number, "1");
        private static INIFile.INIVariableDef mwiniMultiCS = new INIFile.INIVariableDef ("MultiCS", smwiniGeneral, "AllowMultipleEditors", INIFile.INIBoolType.Number, "0");
        #endregion

        private static INIFile.INIVariableDef [] mwSettings = new INIFile.INIVariableDef [] {
            mwiniSSEnable, mwiniNoThread, mwiniYesToAll, mwiniHDShadow,
            mwiniShowFPS, mwiniAudioOff, mwiniSubtitles, mwiniHitFader,
            mwiniMultiCS
        };

        private void LoadMWINI () {
            INIFile mwini = new INIFile (Statics.fn_mwini, mwSettings, System.Text.Encoding.Default);
            cbScreenshots.Checked = (mwini.getKeyValue ("SSEnable") == 1);
            cbThreadLoad.Checked = (mwini.getKeyValue ("NoThread") != 1);
            cbYesToAll.Checked = (mwini.getKeyValue ("YesToAll") == 1);
            cbHQShadows.Checked = (mwini.getKeyValue ("HDShadow") == 1);
            cbShowFPS.Checked = (mwini.getKeyValue ("ShowFPS") == 1);
            cbAudio.Checked = (mwini.getKeyValue ("AudioOff") == 1);
            cbSubtitles.Checked = (mwini.getKeyValue ("Subtitles") == 1);
            cbHitFader.Checked = (mwini.getKeyValue ("HitFader") == 1);
            cbMultipleCS.Checked = (mwini.getKeyValue ("MultiCS") == 1);
        }

        private static Dictionary<string, double> antiAliasDict = new Dictionary<string, double> {
            {"None", 0},
            {"2x", 1},
            {"3x", 2},
            {"4x", 3},
            {"5x", 4},
            {"6x", 5},
            {"7x", 6},
            {"8x", 7},
            {"9x", 8},
            {"10x", 9},
            {"11x", 10},
            {"12x", 11},
            {"13x", 12},
            {"14x", 13},
            {"15x", 14},
            {"16x", 15}
        };

        private static Dictionary<string, double> zBufferDict = new Dictionary<string, double> {
            {"D24S8", 0},
            {"D24X8", 1},
            {"D32", 2},
            {"D16", 3},
            {"D16L", 4},
            {"D32FL", 5},
            {"INTZ", 6},
            {"RAWZ", 7},
            {"DF16", 8},
            {"DF24", 9}
        };

        private static Dictionary<string, double> vWaitDict = new Dictionary<string, double> {
            {"Immediate", 0},
            {"1", 1},
            {"2", 2},
            {"3", 3},
            {"4", 4},
            {"Default", 5}
        };

        private static Dictionary<string, double> refreshDict = new Dictionary<string, double> {
            {"Default", 0}
        };

        private static Dictionary<string, double> ssFormatDict = new Dictionary<string, double> {
            {"BMP", 0},
            {"JPEG", 1},
            {"DDS", 2},
            {"PNG", 3},
            {"TGA", 4}
        };

        private static Dictionary<string, double> scaleFilterDict = new Dictionary<string, double> {
            {"Point", 0},
            {"Linear", 1},
            {"Anisotropic", 2},
            {"Pyramidal quad", 3},
            {"Gaussian quad", 4}
        };

        private static Dictionary<string, double> mipFilterDict = new Dictionary<string, double> {
            {"None", 0},
            {"Point", 1},
            {"Linear", 2}
        };

        private static Dictionary<string, double> anisoLevelDict = new Dictionary<string, double> {
            {"Off", 0},
            {"2x", 1},
            {"4x", 2},
            {"8x", 3},
            {"16x", 4}
        };

        private static Dictionary<string, double> fogModeDict = new Dictionary<string, double> {
            {"Depth pixel", 0},
            {"Depth vertex", 1},
            {"Range vertex", 2}
        };

        private static Dictionary<string, double> shaderModelDict = new Dictionary<string, double> {
            {"2.0", 2},
            {"3.0", 3}
        };

        private static Dictionary<string, double> autoDistDict = new Dictionary<string, double> {
            {"By Draw Distance", 1},
            {"By Above Water Fog End", 2}
        };

        private static Dictionary<string, double> tipSpeedDict = new Dictionary<string, double> {
            {"5cps", 0},
            {"6.25cps", 1},
            {"7.5cps", 2},
            {"8.75cps", 3},
            {"10cps", 4},
            {"12.5cps", 5},
            {"15cps", 6},
            {"17.5cps", 7},
            {"20cps", 8},
            {"25cps", 9},
            {"30cps", 10},
            {"35cps", 11},
            {"40cps", 12},
            {"50cps", 13},
            {"60cps", 14},
            {"70cps", 15}
        };

        private const string siniMain = "Main";
        private const string siniGlobGraph = "Global Graphics";
        private const string siniRendState = "Render State";
        private const string siniInput = "Input";
        private const string siniMisc = "Misc";
        private const string siniDL = "Distant Land";

        #region mge_ini_variable_definitions
        // Main
        private static INIFile.INIVariableDef iniVersion = new INIFile.INIVariableDef ("Version", siniMain, "Version", INIFile.INIVariableType.String, Statics.versionString);
        private static INIFile.INIVariableDef iniIipSpeed = new INIFile.INIVariableDef ("IipSpeed", siniMain, "Tooltip Reading Speed", INIFile.INIVariableType.Dictionary, "15cps", tipSpeedDict);
        public static INIFile.INIVariableDef iniLanguage = new INIFile.INIVariableDef ("Language", siniMain, "GUI Language", INIFile.INIVariableType.String, "English (default)");
        public static INIFile.INIVariableDef iniAutoLang = new INIFile.INIVariableDef ("AutoLang", siniMain, "Language Autodetection", INIFile.INIBoolType.Text, "True");
        // Global Graphics
        private static INIFile.INIVariableDef iniAntiAlias = new INIFile.INIVariableDef ("AntiAlias", siniGlobGraph, "Antialiasing Level", INIFile.INIVariableType.Dictionary, "None", antiAliasDict);
        private static INIFile.INIVariableDef iniZBuffer = new INIFile.INIVariableDef ("ZBuffer", siniGlobGraph, "Z-Buffer Format", INIFile.INIVariableType.Dictionary, "D24S8", zBufferDict);
        private static INIFile.INIVariableDef iniVWait = new INIFile.INIVariableDef ("VWait", siniGlobGraph, "VWait", INIFile.INIVariableType.Dictionary, "Default", vWaitDict);
        private static INIFile.INIVariableDef iniRefresh = new INIFile.INIVariableDef ("Refresh", siniGlobGraph, "Refresh Rate", INIFile.INIVariableType.Byte, "Default", refreshDict, 0, 240);
        // Render State
        private static INIFile.INIVariableDef iniScaleFilt = new INIFile.INIVariableDef ("ScaleFilt", siniRendState, "Scaling Filter", INIFile.INIVariableType.Dictionary, "Linear", scaleFilterDict);
        private static INIFile.INIVariableDef iniAnisoLvl = new INIFile.INIVariableDef ("AnisoLvl", siniRendState, "Anisotropic Filtering Level", INIFile.INIVariableType.Dictionary, "Off", anisoLevelDict);
        private static INIFile.INIVariableDef iniMipFilt = new INIFile.INIVariableDef ("MipFilt", siniRendState, "Mipmap Filter", INIFile.INIVariableType.Dictionary, "Linear", mipFilterDict);
        private static INIFile.INIVariableDef iniLODBias = new INIFile.INIVariableDef ("LODBias", siniRendState, "Mipmap LOD Bias", INIFile.INIVariableType.SByte, "0", -12, 12);
        private static INIFile.INIVariableDef iniScrRot = new INIFile.INIVariableDef ("ScrRot", siniRendState, "Screen Rotation", INIFile.INIVariableType.Single, "0", 0, 360, 2);
        private static INIFile.INIVariableDef iniInitZoom = new INIFile.INIVariableDef ("InitZoom", siniRendState, "Initial Zoom", INIFile.INIVariableType.Single, "3", 3, 15, 2);
        private static INIFile.INIVariableDef iniFogMode = new INIFile.INIVariableDef ("FogMode", siniRendState, "Fog Mode", INIFile.INIVariableType.Dictionary, "Depth pixel", fogModeDict);
        private static INIFile.INIVariableDef iniUseFog = new INIFile.INIVariableDef ("UseFog", siniRendState, "Enable Fog", INIFile.INIBoolType.Text, "True");
        private static INIFile.INIVariableDef iniFPSCount = new INIFile.INIVariableDef ("FPSCount", siniRendState, "MGE FPS Counter", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniMessages = new INIFile.INIVariableDef ("Messages", siniRendState, "MGE Messages", INIFile.INIBoolType.OnOff, "On");
        private static INIFile.INIVariableDef iniMsgTime = new INIFile.INIVariableDef ("MsgTime", siniRendState, "MGE Messages Timeout", INIFile.INIVariableType.UInt16, "5000", 500, 50000);
        private static INIFile.INIVariableDef iniHWShader = new INIFile.INIVariableDef ("HWShader", siniRendState, "Hardware Shader", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniDisableShadersInMenu = new INIFile.INIVariableDef("DisableShadersInMenu", siniRendState, "Disable Shaders In Menu", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniAAFix = new INIFile.INIVariableDef ("AAFix", siniRendState, "AA Colour Fix", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniFOV = new INIFile.INIVariableDef ("FOV", siniRendState, "Horizontal Screen FOV", INIFile.INIVariableType.Single, "75", 5, 150, 2);
        private static INIFile.INIVariableDef iniSSFormat = new INIFile.INIVariableDef ("SSFormat", siniRendState, "Screenshot Format", INIFile.INIVariableType.Dictionary, "BMP", ssFormatDict);
        private static INIFile.INIVariableDef iniSSName = new INIFile.INIVariableDef ("SSName", siniRendState, "Screenshot Name Prefix", INIFile.INIVariableType.String, "MGE Screenshot ");
        private static INIFile.INIVariableDef iniSSDir = new INIFile.INIVariableDef ("SSDir", siniRendState, "Screenshot Output Directory", INIFile.INIVariableType.String, "");
        private static INIFile.INIVariableDef iniSSNum = new INIFile.INIVariableDef ("SSNum", siniRendState, "Screenshot Number Min Lenght", INIFile.INIVariableType.Byte, "1", 1, 5);
        // Input
        private static INIFile.INIVariableDef iniLagFix = new INIFile.INIVariableDef ("LagFix", siniInput, "Input Lag Fix", INIFile.INIBoolType.OnOff, "Off");
        // Misc
        private static INIFile.INIVariableDef iniAffectUI = new INIFile.INIVariableDef ("AffectUI", siniMisc, "Camera Effects Affect The HUD", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniAspectZoom = new INIFile.INIVariableDef ("AspectZoom", siniMisc, "Correct Aspect Ratio When Zooming", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDisableMWSE = new INIFile.INIVariableDef ("DisableMWSE", siniMisc, "Internal MWSE Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDisableMWE = new INIFile.INIVariableDef ("DisableMWE", siniMisc, "Internal MWE Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDisableIntegration = new INIFile.INIVariableDef("DisableIntegration", siniMisc, "Other Integration Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDisablePatch = new INIFile.INIVariableDef("DisablePatch", siniMisc, "Patch Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniHookTex = new INIFile.INIVariableDef ("HookTex", siniMisc, "Use Texture Hooks", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniShaderVars = new INIFile.INIVariableDef ("ShaderVars", siniMisc, "Update Shader Vars", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniUseHDR = new INIFile.INIVariableDef ("UseHDR", siniMisc, "Use HDR", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniHDRTime = new INIFile.INIVariableDef ("HDRTime", siniMisc, "HDR Reaction Time", INIFile.INIVariableType.Single, "2", 0.01, 30, 2);
        private static INIFile.INIVariableDef iniKeepFPS = new INIFile.INIVariableDef ("KeepFPS", siniMisc, "Keep FPS", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniFPSDesired = new INIFile.INIVariableDef ("FPSDesired", siniMisc, "Desired FPS", INIFile.INIVariableType.Byte, "30", 1, 240);
        private static INIFile.INIVariableDef iniFPSToler = new INIFile.INIVariableDef ("FPSToler", siniMisc, "FPS Tolerance", INIFile.INIVariableType.Byte, "5", 0, 250);
        private static INIFile.INIVariableDef iniMinMWView = new INIFile.INIVariableDef ("MinMWView", siniMisc, "Min View Distance", INIFile.INIVariableType.UInt16, "2500", 50, 25000);
        private static INIFile.INIVariableDef iniMaxMWView = new INIFile.INIVariableDef ("MaxMWView", siniMisc, "Max View Distance", INIFile.INIVariableType.UInt16, "7168", 50, 25000);
        private static INIFile.INIVariableDef iniAIToMWView = new INIFile.INIVariableDef ("AIToMWView", siniMisc, "Bind AI To View Distance", INIFile.INIBoolType.OnOff, "On");
        private static INIFile.INIVariableDef iniCPUIdle = new INIFile.INIVariableDef ("CPUIdle", siniMisc, "Force CPU Idle", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniIdleTime = new INIFile.INIVariableDef ("IdleTime", siniMisc, "Idle Time", INIFile.INIVariableType.UInt16, "0", 0, 1000);
        private static INIFile.INIVariableDef iniShaderDepth = new INIFile.INIVariableDef ("ShaderDepth", siniMisc, "Update Shader Depth", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDisableMGE = new INIFile.INIVariableDef ("DisableMGE", siniMisc, "MGE Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniCam3rdCustom = new INIFile.INIVariableDef ("Cam3rdCustom", siniMisc, "Customize 3rd Person Camera", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniCam3rdX = new INIFile.INIVariableDef ("Cam3rdX", siniMisc, "Initial 3rd Person Camera X", INIFile.INIVariableType.Single, "0", -125, 125, 1);
        private static INIFile.INIVariableDef iniCam3rdY = new INIFile.INIVariableDef ("Cam3rdY", siniMisc, "Initial 3rd Person Camera Y", INIFile.INIVariableType.Single, "-160", -2500, -25, 1);
        private static INIFile.INIVariableDef iniCam3rdZ = new INIFile.INIVariableDef ("Cam3rdZ", siniMisc, "Initial 3rd Person Camera Z", INIFile.INIVariableType.Single, "0", -125, 125, 1);
        // Distant Land
        private static INIFile.INIVariableDef iniDistLand = new INIFile.INIVariableDef ("DistLand", siniDL, "Distant Land", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniDrawDist = new INIFile.INIVariableDef ("DrawDist", siniDL, "Draw Distance", INIFile.INIVariableType.Single, "13", 1, 300, 1);
        private static INIFile.INIVariableDef iniAutoDist = new INIFile.INIVariableDef ("AutoDist", siniDL, "Auto Distances", INIFile.INIBoolType.Text, "True");
        private static INIFile.INIVariableDef iniAutoDistBy = new INIFile.INIVariableDef ("AutoDistBy", siniDL, "Auto Distances Choice", INIFile.INIVariableType.Dictionary, "By Draw Distance", autoDistDict);
        private static INIFile.INIVariableDef iniDLNotInt = new INIFile.INIVariableDef ("DLNotInt", siniDL, "Disable In Interiors", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDistStat = new INIFile.INIVariableDef ("DistStat", siniDL, "Distant Statics", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniSizeFar = new INIFile.INIVariableDef ("SizeFar", siniDL, "Far Static Min Size", INIFile.INIVariableType.UInt16, "600", 0, 9999);
        private static INIFile.INIVariableDef iniSizeVFar = new INIFile.INIVariableDef ("SizeVFar", siniDL, "Very Far Static Min Size", INIFile.INIVariableType.UInt16, "800", 0, 9999);
        private static INIFile.INIVariableDef iniEndNear = new INIFile.INIVariableDef ("EndNear", siniDL, "Near Statics End", INIFile.INIVariableType.Single, "3", 2, 299.8, 1);
        private static INIFile.INIVariableDef iniEndFar = new INIFile.INIVariableDef ("EndFar", siniDL, "Far Statics End", INIFile.INIVariableType.Single, "6", 2.1, 299.9, 1);
        private static INIFile.INIVariableDef iniEndVFar = new INIFile.INIVariableDef ("EndVFar", siniDL, "Very Far Statics End", INIFile.INIVariableType.Single, "12", 2.2, 300, 1);
        private static INIFile.INIVariableDef iniAboveBeg = new INIFile.INIVariableDef ("AboveBeg", siniDL, "Above Water Fog Start", INIFile.INIVariableType.Single, "2", 0, 299.9, 1);
        private static INIFile.INIVariableDef iniAboveEnd = new INIFile.INIVariableDef ("AboveEnd", siniDL, "Above Water Fog End", INIFile.INIVariableType.Single, "13", 0.1, 300, 1);
        private static INIFile.INIVariableDef iniBelowBeg = new INIFile.INIVariableDef ("BelowBeg", siniDL, "Below Water Fog Start", INIFile.INIVariableType.Single, "0", -99.9, 299.9, 1);
        private static INIFile.INIVariableDef iniBelowEnd = new INIFile.INIVariableDef ("BelowEnd", siniDL, "Below Water Fog End", INIFile.INIVariableType.Single, "3", 0.1, 300, 1);
        private static INIFile.INIVariableDef iniInterBeg = new INIFile.INIVariableDef ("InterBeg", siniDL, "Interior Fog Start", INIFile.INIVariableType.Single, "0", -0.9, 9.9, 2);
        private static INIFile.INIVariableDef iniInterEnd = new INIFile.INIVariableDef ("InterEnd", siniDL, "Interior Fog End", INIFile.INIVariableType.Single, "0.9", 0.1, 10, 2);
        //private static INIFile.INIVariableDef iniDistBlur = new INIFile.INIVariableDef ("DistBlur", siniDL, "Distant Blur Strength", INIFile.INIVariableType.Byte, "0", 0, 255);
        private static INIFile.INIVariableDef iniReflLand = new INIFile.INIVariableDef ("ReflLand", siniDL, "Water Reflects Land", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniReflNear = new INIFile.INIVariableDef ("ReflNear", siniDL, "Water Reflects Near Statics", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniReflFar = new INIFile.INIVariableDef ("ReflFar", siniDL, "Water Reflects Far Statics", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDLInitOff = new INIFile.INIVariableDef ("DLInitOff", siniDL, "Distant Land Initially Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDLMWBlendOff = new INIFile.INIVariableDef ("DLMWBlendOff", siniDL, "MW Blending Initially Disabled", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniSkyRefl = new INIFile.INIVariableDef ("SkyRefl", siniDL, "Enable Sky Reflections", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDynRipples = new INIFile.INIVariableDef ("DynRipples", siniDL, "Dynamic Ripples", INIFile.INIBoolType.Text, "False");
        private static INIFile.INIVariableDef iniDLShader = new INIFile.INIVariableDef ("DLShader", siniDL, "Shader Model", INIFile.INIVariableType.Dictionary, "2.0", shaderModelDict);
        private static INIFile.INIVariableDef iniReflBlur = new INIFile.INIVariableDef ("ReflBlur", siniDL, "Blur Water Reflections", INIFile.INIBoolType.OnOff, "Off");
        private static INIFile.INIVariableDef iniExpFog = new INIFile.INIVariableDef ("ExpFog", siniDL, "Use Exponential Fog", INIFile.INIBoolType.YesNo, "No");
        private static INIFile.INIVariableDef iniDLExpMul = new INIFile.INIVariableDef ("DLExpMul", siniDL, "Exponential Distance Multiplier", INIFile.INIVariableType.Single, "4.0", 2.5, 5.0, 2);
        private static INIFile.INIVariableDef iniWaveHght = new INIFile.INIVariableDef ("WaveHght", siniDL, "Water Wave Height", INIFile.INIVariableType.Byte, "50", 0, 250);
        private static INIFile.INIVariableDef iniCaustics = new INIFile.INIVariableDef ("Caustics", siniDL, "Water Caustics Intensity", INIFile.INIVariableType.Byte, "50", 0, 100);
        #endregion

        private static INIFile.INIVariableDef [] iniSettings = {
            INIFile.iniDefEmpty,
            // Main
            iniVersion, iniIipSpeed, iniLanguage, iniAutoLang,
            // Misc
            iniDisableMGE, iniDisableMWSE, iniDisableMWE, iniDisableIntegration,
            iniDisablePatch, iniAffectUI, iniAspectZoom, iniHookTex,
            iniShaderVars, iniUseHDR, iniHDRTime, iniKeepFPS,
            iniFPSDesired, iniFPSToler, iniMinMWView, iniMaxMWView,
            iniAIToMWView, iniCPUIdle, iniIdleTime, iniShaderDepth,
            iniCam3rdCustom, iniCam3rdX, iniCam3rdY, iniCam3rdZ,
            // Global graphics
            iniAntiAlias, iniZBuffer, iniVWait, iniRefresh,
            // Render state
            iniScaleFilt, iniAnisoLvl, iniMipFilt, iniLODBias,
            iniScrRot, iniInitZoom, iniFOV, iniFogMode,
            iniUseFog, iniFPSCount, iniMessages, iniMsgTime,
            iniHWShader, iniDisableShadersInMenu, iniAAFix, iniSSFormat,
            iniSSName, iniSSDir, iniSSNum,
            // Input
            iniLagFix,
            // Distant Land
            iniDistLand, iniDLInitOff, iniDLNotInt, iniDrawDist,
            iniDistStat, iniDLMWBlendOff, iniAutoDist, iniAutoDistBy,
            iniEndNear, iniEndFar, iniEndVFar, iniSizeFar,
            iniSizeVFar, iniReflLand, iniReflNear, iniReflFar,
            iniAboveBeg, iniAboveEnd, iniBelowBeg, iniBelowEnd,
            iniInterBeg, iniInterEnd, iniSkyRefl, iniDynRipples,
            iniDLShader, iniReflBlur, iniExpFog, iniDLExpMul,
            iniWaveHght, iniCaustics
        };

        private Dictionary<string, bool> DLOptions = new Dictionary<string, bool> {
            {"DLNotInt", false},
            {"DLInitOff", false},
            {"DLMWBlendOff", false}
        };

        private void LoadGraphicsSettings () {
            LoadGraphicsSettings (false, true);
        }

        private void LoadGraphicsSettings (bool reset) {
            LoadGraphicsSettings (reset, false);
        }

        private void LoadGraphicsSettings (bool reset, bool save) {
            INIFile iniFile = new INIFile (reset ? Statics.fn_nul : Statics.iniFileName, iniSettings, true);
            if (reset) iniFile.fileName = Statics.iniFileName;
            else bCalcRefresh_Click (null, null);
            if (save) {
                iniFile.initialize ();
                iniFile.save ();
            }
            loading = true;
            //Main
            cmbTipReadSpd.SelectedIndex = (int)iniFile.getKeyValue ("IipSpeed");
            //Global Graphics
            cmbAntiAlias.SelectedIndex = (int)iniFile.getKeyValue ("AntiAlias");
            cmbZBuffer.SelectedIndex = (int)iniFile.getKeyValue ("ZBuffer");
            cmbVWait.SelectedIndex = (int)iniFile.getKeyValue ("VWait");
            cmbRefreshRate.SelectedIndex = cmbRefreshRate.FindStringExact (iniFile.getKeyValue ("Refresh").ToString ());
            if (cmbRefreshRate.SelectedIndex == -1) cmbRefreshRate.SelectedIndex = 0;
            //Render State
            cmbScaleFilter.SelectedIndex = (int)iniFile.getKeyValue ("ScaleFilt");
            cmbMipFilter.SelectedIndex = (int)iniFile.getKeyValue ("MipFilt");
            cmbAnisoLevel.SelectedIndex = (int)iniFile.getKeyValue ("AnisoLvl");
            udLOD.Value = (int)iniFile.getKeyValue ("LODBias");
            cmbFogMode.SelectedIndex = (int)iniFile.getKeyValue ("FogMode");
            udZoom.Value = (decimal)iniFile.getKeyValue ("InitZoom");
            udRotation.Value = (decimal)iniFile.getKeyValue ("ScrRot");
            udFOV.Value = (decimal)iniFile.getKeyValue ("FOV");
            cbFog.Checked = (iniFile.getKeyValue ("UseFog") == 1);
            cbFPSCounter.Checked = (iniFile.getKeyValue ("FPSCount") == 1);
            cbDisplayMessages.Checked = (iniFile.getKeyValue ("Messages") == 1);
            udMsgsTime.Value = (decimal)iniFile.getKeyValue ("MsgTime");
            cbHWShader.Checked = (iniFile.getKeyValue ("HWShader") == 1);
            cbDisableShadersInMenu.Checked = (iniFile.getKeyValue("DisableShadersInMenu") == 1);
            cbAAFix.Checked = (iniFile.getKeyValue ("AAFix") == 1);
            cmbSShotFormat.SelectedIndex = (int)iniFile.getKeyValue ("SSFormat");
            tbSShotDir.Text = iniFile.getKeyString ("SSDir");
            if (tbSShotDir.Text.Length == 0) bSSDirClear_Click (null, null);
            tbSShotName.Text = iniFile.getKeyString ("SSName");
            udSShotNum.Value = (decimal)iniFile.getKeyValue ("SSNum");
            //Input
            cbLagFix.Checked = (iniFile.getKeyValue ("LagFix") == 1);
            //Misc
            cbDisableMWSE.Checked = (iniFile.getKeyValue ("DisableMWSE") == 1);
            cbDisableMWE.CheckState = (iniFile.getKeyValue("DisableMWE") == 1) ? CheckState.Checked : File.Exists(Statics.fn_mwedll) ? CheckState.Unchecked : CheckState.Indeterminate;
            cbDisableIntegration.Checked = (iniFile.getKeyValue("DisableIntegration") == 1);
            cbDisablePatch.Checked = (iniFile.getKeyValue("DisablePatch") == 1);
            cbZoomEffectsHUD.Checked = (iniFile.getKeyValue ("AffectUI") == 1);
            cbAspectZoom.Checked = (iniFile.getKeyValue ("AspectZoom") == 1);
            cbHook.Checked = (iniFile.getKeyValue ("HookTex") == 1);
            cbSetEffectVars.Checked = (iniFile.getKeyValue ("ShaderVars") == 1);
            cbHDR.Checked = (iniFile.getKeyValue ("UseHDR") == 1);
            udHDR.Value = (decimal)iniFile.getKeyValue ("HDRTime");
            cbFPSConst.Checked = (iniFile.getKeyValue ("KeepFPS") == 1);
            udFPSDes.Value = (int)iniFile.getKeyValue ("FPSDesired");
            udFPSTol.Value = (int)iniFile.getKeyValue ("FPSToler");
            udMaxView.Value = (int)iniFile.getKeyValue ("MaxMWView");
            udMinView.Value = (int)iniFile.getKeyValue ("MinMWView");
            cbBindAI.Checked = (iniFile.getKeyValue ("AIToMWView") == 1);
            cbIdle.Checked = (iniFile.getKeyValue ("CPUIdle") == 1);
            udIdle.Value = (int)iniFile.getKeyValue ("IdleTime");
            cbShaderDepth.Checked = (iniFile.getKeyValue ("ShaderDepth") == 1);
            cbCam3rdPrsn.Checked = (iniFile.getKeyValue ("Cam3rdCustom") == 1);
            udCam3rdX.Value = (decimal)iniFile.getKeyValue ("Cam3rdX");
            udCam3rdY.Value = (decimal)iniFile.getKeyValue ("Cam3rdY");
            udCam3rdZ.Value = (decimal)iniFile.getKeyValue ("Cam3rdZ");
            //Distant Land
            cbDLDistantLand.Checked = (iniFile.getKeyValue ("DistLand") == 1);
            List<string> list = new List<string> (DLOptions.Keys);
            foreach (string key in list) DLOptions [key] = (iniFile.getKeyValue (key) == 1);
            cbDLDistantStatics.Checked = (iniFile.getKeyValue ("DistStat") == 1);
            udDLDrawDist.Value = (decimal)iniFile.getKeyValue ("DrawDist");
            //udDLBlur.Value = (int)iniFile.getKeyValue ("DistBlur");   //removed
            udDLDistNear.Value = (decimal)iniFile.getKeyValue ("EndNear");
            udDLDistFar.Value = (decimal)iniFile.getKeyValue ("EndFar");
            udDLDistVeryFar.Value = (int)iniFile.getKeyValue ("EndVFar");
            udDLSizeFar.Value = (decimal)iniFile.getKeyValue ("SizeFar");
            udDLSizeVeryFar.Value = (decimal)iniFile.getKeyValue ("SizeVFar");
            cbDLReflLand.Checked = (iniFile.getKeyValue ("ReflLand") == 1);
            cbDLReflNStatics.Checked = (iniFile.getKeyValue ("ReflNear") == 1);
            cbDLReflFStatics.Checked = (iniFile.getKeyValue ("ReflFar") == 1);
            udDLFogAStart.Value = (decimal)iniFile.getKeyValue ("AboveBeg");
            udDLFogAEnd.Value = (decimal)iniFile.getKeyValue ("AboveEnd");
            udDLFogBStart.Value = (decimal)iniFile.getKeyValue ("BelowBeg");
            udDLFogBEnd.Value = (decimal)iniFile.getKeyValue ("BelowEnd");
            udDLFogIStart.Value = (decimal)iniFile.getKeyValue ("InterBeg");
            udDLFogIEnd.Value = (decimal)iniFile.getKeyValue ("InterEnd");
            cbDLSkyRefl.Checked = (iniFile.getKeyValue ("SkyRefl") == 1);
            cbDLDynRipples.Checked = (iniFile.getKeyValue ("DynRipples") == 1);
            rbDLSM30.Checked = (iniFile.getKeyValue ("DLShader") == 3);
            cbDLReflBlur.Checked = (iniFile.getKeyValue ("ReflBlur") == 1);
            udDLFogExpMul.Value = (decimal)iniFile.getKeyValue ("DLExpMul");
            cbDLFogExp.Checked = (iniFile.getKeyValue ("ExpFog") == 1);
            udDLWtrWave.Value = (decimal)iniFile.getKeyValue ("WaveHght");
            udDLWtrCaust.Value = (decimal)iniFile.getKeyValue ("Caustics");
            int autoDistBy = (int)iniFile.getKeyValue ("AutoDistBy");
            rbDLAutoByDrawDist.Checked = (autoDistBy == 1);
            rbDLAutoByAFogEnd.Checked = (autoDistBy == 2);
            cbDLAutoDist.Checked = (iniFile.getKeyValue ("AutoDist") == 1);
            //MGE disabled
            cbDisableMGE.Checked = (iniFile.getKeyValue ("DisableMGE") == 1);
            loading = false;
        }

        private void SaveGraphicsSettings () {
            INIFile iniFile = new INIFile (Statics.iniFileName, iniSettings, true);
            //Main
            iniFile.setKey ("Version", Statics.versionString);
            iniFile.setKey ("IipSpeed", cmbTipReadSpd.SelectedIndex);
            iniFile.setKey ("Language", cmbUILanguage.Text);
            iniFile.setKey ("AutoLang", cbUILangAuto.Checked);
            //MGE disabled
            iniFile.setKey ("DisableMGE", cbDisableMGE.Checked);
            //Global Graphics
            iniFile.setKey ("AntiAlias", cmbAntiAlias.SelectedIndex);
            iniFile.setKey ("ZBuffer", cmbZBuffer.SelectedIndex);
            iniFile.setKey ("VWait", cmbVWait.SelectedIndex);
            iniFile.setKey ("Refresh", cmbRefreshRate.Text);
            //Render State
            iniFile.setKey ("ScaleFilt", cmbScaleFilter.SelectedIndex);
            iniFile.setKey ("MipFilt", cmbMipFilter.SelectedIndex);
            iniFile.setKey ("AnisoLvl", cmbAnisoLevel.SelectedIndex);
            iniFile.setKey ("LODBias", (double)udLOD.Value);
            iniFile.setKey ("FogMode", cmbFogMode.SelectedIndex);
            iniFile.setKey ("InitZoom", (double)udZoom.Value);
            iniFile.setKey ("ScrRot", (double)udRotation.Value);
            iniFile.setKey ("FOV", (double)udFOV.Value);
            iniFile.setKey ("UseFog", cbFog.Checked);
            iniFile.setKey ("FPSCount", cbFPSCounter.Checked);
            iniFile.setKey ("Messages", cbDisplayMessages.Checked);
            iniFile.setKey ("MsgTime", (double)udMsgsTime.Value);
            iniFile.setKey ("HWShader", cbHWShader.Checked);
            iniFile.setKey("DisableShadersInMenu", cbDisableShadersInMenu.Checked);
            iniFile.setKey ("AAFix", cbAAFix.Checked);
            iniFile.setKey ("SSFormat", cmbSShotFormat.SelectedIndex);
            if (tbSShotDir.TextAlign == HorizontalAlignment.Left) iniFile.setKey ("SSDir", tbSShotDir.Text);
            else iniFile.setKey ("SSDir", "");
            iniFile.setKey ("SSName", tbSShotName.Text);
            iniFile.setKey ("SSNum", (double)udSShotNum.Value);
            //Input
            iniFile.setKey ("LagFix", cbLagFix.Checked);
            //Misc
            iniFile.setKey ("DisableMWSE", cbDisableMWSE.Checked);
            iniFile.setKey ("DisableMWE", cbDisableMWE.CheckState == CheckState.Checked);
            iniFile.setKey ("DisableIntegration", cbDisableIntegration.Checked);
            iniFile.setKey ("DisablePatch", cbDisablePatch.Checked);
            iniFile.setKey ("AffectUI", cbZoomEffectsHUD.Checked);
            iniFile.setKey ("AspectZoom", cbAspectZoom.Checked);
            iniFile.setKey ("HookTex", cbHook.Checked);
            iniFile.setKey ("ShaderVars", cbSetEffectVars.Checked);
            iniFile.setKey ("ShaderDepth", cbShaderDepth.Checked);
            iniFile.setKey ("UseHDR", cbHDR.Checked);
            iniFile.setKey ("HDRTime", (double)udHDR.Value);
            iniFile.setKey ("KeepFPS", cbFPSConst.Checked);
            iniFile.setKey ("FPSDesired", (double)udFPSDes.Value);
            iniFile.setKey ("FPSToler", (double)udFPSTol.Value);
            iniFile.setKey ("MaxMWView", (double)udMaxView.Value);
            iniFile.setKey ("MinMWView", (double)udMinView.Value);
            iniFile.setKey ("AIToMWView", cbBindAI.Checked);
            iniFile.setKey ("CPUIdle", cbIdle.Checked);
            iniFile.setKey ("IdleTime", (double)udIdle.Value);
            iniFile.setKey ("Cam3rdCustom", cbCam3rdPrsn.Checked);
            iniFile.setKey ("Cam3rdX", (double)udCam3rdX.Value);
            iniFile.setKey ("Cam3rdY", (double)udCam3rdY.Value);
            iniFile.setKey ("Cam3rdZ", (double)udCam3rdZ.Value);
            //Distant Land
            iniFile.setKey ("DistLand", cbDLDistantLand.Checked);
            iniFile.setKey ("DistStat", cbDLDistantStatics.Checked);
            iniFile.setKey ("DLShader", rbDLSM30.Checked ? 3 : 2);
            foreach (KeyValuePair<string, bool> v in DLOptions) iniFile.setKey (v.Key, v.Value);
            iniFile.setKey ("DrawDist", (double)udDLDrawDist.Value);
            iniFile.setKey ("AutoDist", cbDLAutoDist.Checked);
            int autoDistBy;
            if (rbDLAutoByAFogEnd.Checked) autoDistBy = 2;
            else autoDistBy = 1;
            iniFile.setKey ("AutoDistBy", autoDistBy);
            //iniFile.setKey ("DistBlur", (double)udDLBlur.Value);  //removed
            iniFile.setKey ("EndNear", (double)udDLDistNear.Value);
            iniFile.setKey ("EndFar", (double)udDLDistFar.Value);
            iniFile.setKey ("EndVFar", (double)udDLDistVeryFar.Value);
            iniFile.setKey ("SizeFar", (double)udDLSizeFar.Value);
            iniFile.setKey ("SizeVFar", (double)udDLSizeVeryFar.Value);
            iniFile.setKey ("ReflLand", cbDLReflLand.Checked);
            iniFile.setKey ("ReflNear", cbDLReflNStatics.Checked);
            iniFile.setKey ("ReflFar", cbDLReflFStatics.Checked);
            iniFile.setKey ("SkyRefl", cbDLSkyRefl.Checked);
            iniFile.setKey ("ReflBlur", cbDLReflBlur.Checked);
            iniFile.setKey ("DynRipples", cbDLDynRipples.Checked);
            iniFile.setKey ("WaveHght", (double)udDLWtrWave.Value);
            iniFile.setKey ("Caustics", (double)udDLWtrCaust.Value);
            iniFile.setKey ("AboveBeg", (double)udDLFogAStart.Value);
            iniFile.setKey ("AboveEnd", (double)udDLFogAEnd.Value);
            iniFile.setKey ("BelowBeg", (double)udDLFogBStart.Value);
            iniFile.setKey ("BelowEnd", (double)udDLFogBEnd.Value);
            iniFile.setKey ("InterBeg", (double)udDLFogIStart.Value);
            iniFile.setKey ("InterEnd", (double)udDLFogIEnd.Value);
            iniFile.setKey ("ExpFog", cbDLFogExp.Checked);
            iniFile.setKey ("DLExpMul", (double)udDLFogExpMul.Value);
            iniFile.save ();
            try {
                RegistryKey key = Registry.LocalMachine.OpenSubKey (Statics.reg_mw, true);
                if (key != null) {
                    key.SetValue ("Pixelshader", new byte [] { Convert.ToByte (!gbDistantLand.Enabled) });
                    key.Close ();
                }
            } catch { }
        }

        private decimal udValue (NumericUpDown target, double value) {
            return ((decimal)value < target.Minimum ? target.Minimum : ((decimal)value > target.Maximum ? target.Maximum : (decimal)value));
        }

        private void ImportGraphicsSaveFile () {
            BinaryReader br;
            br = new BinaryReader (File.OpenRead (Statics.fn_settings));
            byte version = br.ReadByte ();
            if (version < 41) throw new Exception (strings ["SetTooOld"].text);
            if (version > Statics.SaveVersion) throw new Exception (strings ["SetTooNew"].text);
            int i;
            loading = true;
            cbBindAI.Checked = br.ReadBoolean ();
            switch (i = br.ReadByte ()) {
                case 0: cmbAntiAlias.SelectedIndex = 0; break;
                default: cmbAntiAlias.SelectedIndex = i - 1; break;
            }
            switch (br.ReadByte ()) {
                case 75: cmbZBuffer.SelectedIndex = 0; break;
                case 77: cmbZBuffer.SelectedIndex = 1; break;
                case 71: cmbZBuffer.SelectedIndex = 2; break;
                case 80: cmbZBuffer.SelectedIndex = 3; break;
                case 70: cmbZBuffer.SelectedIndex = 4; break;
                case 82: cmbZBuffer.SelectedIndex = 5; break;
                case 1: cmbZBuffer.SelectedIndex = 6; break;
                case 2: cmbZBuffer.SelectedIndex = 7; break;
                case 3: cmbZBuffer.SelectedIndex = 8; break;
                case 4: cmbZBuffer.SelectedIndex = 9; break;
                default: cmbZBuffer.SelectedIndex = 0; break;
            }
            switch (br.ReadByte ()) {
                case 3: cmbVWait.SelectedIndex = 0; break;
                case 1: cmbVWait.SelectedIndex = 1; break;
                case 2: cmbVWait.SelectedIndex = 2; break;
                case 4: cmbVWait.SelectedIndex = 3; break;
                case 8: cmbVWait.SelectedIndex = 4; break;
                default: cmbVWait.SelectedIndex = 5; break;
            }
            switch (i = br.ReadByte ()) {
                case 0: cmbRefreshRate.SelectedIndex = 0; break;
                default:
                    cmbRefreshRate.SelectedIndex = cmbRefreshRate.FindStringExact (i.ToString ());
                    if (cmbRefreshRate.SelectedIndex == -1) cmbRefreshRate.SelectedIndex = 0;
                    break;
            }
            cbFPSCounter.Checked = br.ReadBoolean ();
            cbLagFix.Checked = br.ReadBoolean ();
            cbAAFix.Checked = br.ReadBoolean ();
            cbDisplayMessages.Checked = br.ReadBoolean ();
            cbIdle.Checked = br.ReadBoolean ();
            udIdle.Value = udValue (udIdle, br.ReadByte ());
            cbFog.Checked = br.ReadBoolean ();
            cmbSShotFormat.SelectedIndex = br.ReadByte ();
            cmbScaleFilter.SelectedIndex = br.ReadByte () - 1;
            cmbMipFilter.SelectedIndex = br.ReadByte ();
            switch (br.ReadByte ()) {
                case 1: cmbAnisoLevel.SelectedIndex = 0; break;
                case 2: cmbAnisoLevel.SelectedIndex = 1; break;
                case 4: cmbAnisoLevel.SelectedIndex = 2; break;
                case 8: cmbAnisoLevel.SelectedIndex = 3; break;
                case 16: cmbAnisoLevel.SelectedIndex = 4; break;
            }
            udLOD.Value = udValue (udLOD, br.ReadSByte ());
            cmbFogMode.SelectedIndex = br.ReadByte ();
            br.ReadByte ();  //Fog pixel mode
            br.ReadByte ();  //Fog vertex mode
            br.ReadByte ();  //Ranged fog
            udZoom.Value = udValue (udZoom, br.ReadByte ());
            udRotation.Value = udValue (udRotation, br.ReadByte () * 90);
            cbHWShader.Checked = br.ReadBoolean ();
            cbZoomEffectsHUD.Checked = br.ReadBoolean ();
            cbFPSConst.Checked = br.ReadBoolean ();
            udFPSDes.Value = udValue (udFPSDes, br.ReadByte ());
            udFPSTol.Value = udValue (udFPSTol, br.ReadByte ());
            udMinView.Value = udValue (udMinView, br.ReadByte () * 100);
            udMaxView.Value = udValue (udMaxView, br.ReadByte () * 100);
            cbSetEffectVars.Checked = br.ReadBoolean ();
            cbHook.Checked = br.ReadBoolean ();
            cbDLDistantLand.Checked = br.ReadBoolean ();
            cbDLDistantStatics.Checked = br.ReadBoolean ();
            if (version >= 43) br.ReadByte ();
            if (version >= 42) {
                cbDLReflLand.Checked = br.ReadBoolean ();
                cbDLReflNStatics.Checked = br.ReadBoolean ();
                cbDLReflFStatics.Checked = br.ReadBoolean ();
            }
            br.ReadBoolean (); // Used to be SM3 water
            udDLDistNear.Value = udValue (udDLDistNear, br.ReadByte ());
            udDLFogAEnd.Value = udValue (udDLFogAEnd, br.ReadByte ());
            cbHDR.Checked = br.ReadBoolean ();
            udHDR.Value = udValue (udHDR, (10.0 / br.ReadByte ()));
            cbAspectZoom.Checked = br.ReadBoolean ();
            if (version >= 44) {
                udDLFogAStart.Value = udValue (udDLFogAStart, br.ReadByte ());
                udDLFogBStart.Value = udValue (udDLFogBStart, br.ReadByte ());
                udDLFogBEnd.Value = udValue (udDLFogBEnd, br.ReadByte ());
                udDLDrawDist.Value = udValue (udDLDrawDist, br.ReadByte ());
                udDLDistFar.Value = udValue (udDLDistFar, br.ReadByte ());
                udDLDistVeryFar.Value = udValue (udDLDistVeryFar, br.ReadByte ());
                udDLSizeFar.Value = udValue (udDLSizeFar, br.ReadByte () * 10);
                udDLSizeVeryFar.Value = udValue (udDLSizeVeryFar, br.ReadByte () * 10);
                cbDLAutoDist.Checked = br.ReadBoolean ();
            }
            if (version >= 46) {
                tbDLNearSize.Text = (br.ReadByte () * 10).ToString ();
            }
            if (version >= 47) {
                br.ReadByte (); // Distant blur
                //udDLBlur.Value = udValue (udDLBlur, br.ReadByte ());  //removed
            }
            loading = false;
            br.Close ();
            SaveGraphicsSettings ();
            if (File.Exists (Statics.fn_oldsettings)) File.Delete (Statics.fn_oldsettings);
            File.Move (Statics.fn_settings, Statics.fn_oldsettings);
            MessageBox.Show (strings ["OldSetImp"].text, Statics.strings ["Message"].text, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void LoadInputSaveFile () {
            byte version;
            if (!File.Exists (Statics.fn_didata)) return;
            BinaryReader br = new BinaryReader (File.OpenRead (Statics.fn_didata));
            version = br.ReadByte ();
            if (version < 39) throw new Exception (strings ["InpTooOld"].text);
            if (version > Statics.SaveVersion) throw new Exception (strings ["InpTooNew"].text);
            cbSkipMovie.Checked = br.ReadBoolean ();
            cbConsole.Checked = br.ReadBoolean ();
            cbAltCombat.Checked = br.ReadBoolean ();
            try { UnserializeMacroSaves (); } catch {
                MessageBox.Show (strings ["MacrTrigErr"].text, Statics.strings ["Error"].text);
                for (int i = 0; i < 266; i++) {
                    Statics.Macros [i].Type = (MacroType)br.ReadByte ();
                    switch (Statics.Macros [i].Type) {
                        case MacroType.Console1:
                        case MacroType.Console2:
                            Statics.Macros [i].Console.Length = br.ReadByte ();
                            for (int j = 0; j < 266; j++) {
                                Statics.Macros [i].Console.KeyCodes [j].Code = br.ReadByte ();
                            }
                            for (int j = 0; j < 266; j++) {
                                Statics.Macros [i].Console.KeyCodes [j].Down = br.ReadBoolean ();
                            }
                            break;
                        case MacroType.Press1:
                        case MacroType.Press2:
                        case MacroType.Unpress:
                        case MacroType.Hammer1:
                        case MacroType.Hammer2:
                        case MacroType.Unhammer:
                        case MacroType.AHammer1:
                        case MacroType.AHammer2:
                        case MacroType.AUnhammer:
                            for (int j = 0; j < 266; j++) {
                                Statics.Macros [i].Press.KeyStates [j] = br.ReadBoolean ();
                            }
                            for (int j = 0; j < 267; j++) {
                                br.ReadByte ();
                            }
                            break;
                        case MacroType.BeginTimer:
                        case MacroType.EndTimer:
                            Statics.Macros [i].Timer.TimerID = br.ReadByte ();
                            for (int j = 0; j < 266 * 2; j++) {
                                br.ReadByte ();
                            }
                            break;
                        case MacroType.Graphics:
                            Statics.Macros [i].Graphics.function = br.ReadByte ();
                            for (int j = 0; j < 266 * 2; j++) {
                                br.ReadByte ();
                            }
                            break;
                        default:
                            for (int j = 0; j < 266 * 2 + 1; j++) {
                                br.ReadByte ();
                            }
                            break;
                    }
                }
                for (int i = 0; i < 4; i++) {
                    Statics.Triggers [i].TimeInterval = br.ReadUInt32 (); ;
                    Statics.Triggers [i].Active = br.ReadBoolean ();
                    for (int j = 0; j < Statics.MACROS; j++) {
                        Statics.Triggers [i].data.KeyStates [j] = br.ReadBoolean ();
                    }
                    br.ReadByte ();  //Needed for memory alignment reasons
                }
            }
            br.Close ();
            if (version < Statics.SaveVersion) {
                MessageBox.Show (strings ["OldInpImp"].text, Statics.strings ["Message"].text);
            }
        }

        private void UnserializeMacroSaves () {
            FileStream fs;
            fs = File.OpenRead (Statics.fn_macro);
            Statics.Macros = (Macro [])Statics.formatter.Deserialize (fs);
            fs.Close ();
            fs = File.OpenRead (Statics.fn_triger);
            Statics.Triggers = (Trigger [])Statics.formatter.Deserialize (fs);
            fs.Close ();
        }

        private void SaveMWINI () {
            INIFile mwini = new INIFile (Statics.fn_mwini, mwSettings, System.Text.Encoding.Default, true);
            mwini.setKey ("SSEnable", cbScreenshots.Checked);
            mwini.setKey ("NoThread", !cbThreadLoad.Checked);
            mwini.setKey ("YesToAll", cbYesToAll.Checked);
            mwini.setKey ("HDShadow", cbHQShadows.Checked);
            mwini.setKey ("ShowFPS", cbShowFPS.Checked);
            mwini.setKey ("AudioOff", cbAudio.Checked);
            mwini.setKey ("Subtitles", cbSubtitles.Checked);
            mwini.setKey ("HitFader", cbHitFader.Checked);
            mwini.setKey ("MultiCS", cbMultipleCS.Checked);
            mwini.save ();
        }

        private void SaveSettings () {
            SaveMWINI ();
            SaveGraphicsSettings ();
            //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            //XX Input - see input.h for layout of save file XX
            //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            FileStream fs;
            fs = File.Open (Statics.fn_macro, FileMode.Create);
            Statics.formatter.Serialize (fs, Statics.Macros);
            fs.Close ();
            fs = File.Open (Statics.fn_triger, FileMode.Create);
            Statics.formatter.Serialize (fs, Statics.Triggers);
            fs.Close ();
            BinaryWriter bw = new BinaryWriter (File.Open (Statics.fn_didata, FileMode.Create));
            bw.Write (Statics.SaveVersion);
            bw.Write (cbSkipMovie.Checked);
            bw.Write (cbConsole.Checked);
            bw.Write (cbAltCombat.Checked);
            for (int i = 0; i < Statics.MACROS; i++) {
                bw.Write ((byte)Statics.Macros [i].Type);
                switch (Statics.Macros [i].Type) {
                    case MacroType.Console1:
                    case MacroType.Console2:
                        bw.Write (Statics.Macros [i].Console.Length);
                        for (int j = 0; j < Statics.MACROS; j++) {
                            bw.Write (Statics.Macros [i].Console.KeyCodes [j].Code);
                        }
                        for (int j = 0; j < Statics.MACROS; j++) {
                            bw.Write ((byte)(Convert.ToByte (Statics.Macros [i].Console.KeyCodes [j].Down) * 0x80));
                        }
                        break;
                    case MacroType.Press1:
                    case MacroType.Press2:
                    case MacroType.Unpress:
                    case MacroType.Hammer1:
                    case MacroType.Hammer2:
                    case MacroType.Unhammer:
                    case MacroType.AHammer1:
                    case MacroType.AHammer2:
                    case MacroType.AUnhammer:
                        for (int j = 0; j < Statics.MACROS; j++) {
                            bw.Write ((byte)(Convert.ToByte (Statics.Macros [i].Press.KeyStates [j]) * 0x80));
                        }
                        for (int j = 0; j < Statics.MACROS + 1; j++) {
                            bw.Write ((byte)0);
                        }
                        break;
                    case MacroType.BeginTimer:
                    case MacroType.EndTimer:
                        bw.Write ((byte)Statics.Macros [i].Timer.TimerID);
                        for (int j = 0; j < Statics.MACROS * 2; j++) {
                            bw.Write ((byte)0);
                        }
                        break;
                    case MacroType.Graphics:
                        bw.Write ((byte)Statics.Macros [i].Graphics.function);
                        for (int j = 0; j < Statics.MACROS * 2; j++) {
                            bw.Write ((byte)0);
                        }
                        break;
                    default:
                        for (int j = 0; j < Statics.MACROS * 2 + 1; j++) {
                            bw.Write ((byte)0);
                        }
                        break;
                }
            }
            for (int i = 0; i < Statics.TRIGGERS; i++) {
                bw.Write (Statics.Triggers [i].TimeInterval * 1000);
                bw.Write (Statics.Triggers [i].Active);
                for (int j = 0; j < Statics.MACROS; j++) {
                    bw.Write ((byte)(Convert.ToByte (Statics.Triggers [i].data.KeyStates [j]) * 0x80));
                }
                bw.Write ((byte)0);  //Needed for memory alignment reasons
            }
            bw.Close ();
            bool CreateRemap = false;
            for (int i = 0; i < 256; i++) {
                if (Statics.Remapper [i] != 0) {
                    CreateRemap = true;
                    break;
                }
            }
            if (CreateRemap) {
                bw = new BinaryWriter (File.Open (Statics.fn_remap, FileMode.Create));
                bw.Write (Statics.Remapper);
                bw.Close ();
            } else {
                File.Delete (Statics.fn_remap);
            }
        }

        private void LoadSettings () {
            LoadGraphicsSettings (false);
            if (File.Exists (Statics.fn_mwse)) {
                cbDisableMWSE.Checked = true;
                File.Delete (Statics.fn_mwse);
            }
            LoadMWINI ();
            if (!cbDisableMGE.Enabled) cbScreenshots.Checked = false;
            if (File.Exists (Statics.fn_settings) && MessageBox.Show (strings ["ImpOldSet"].text, strings ["ImportSet"].text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes) {
                try {
                    ImportGraphicsSaveFile ();
                } catch (Exception ex) {
                    loading = false;
                    MessageBox.Show (String.Format (strings ["ErrImpSet"].text, ex.Message), Statics.strings ["Error"].text);
                }
            }
            if (File.Exists (Statics.fn_didata)) try {
                    LoadInputSaveFile ();
                } catch (Exception ex) {
                    loading = false;
                    MessageBox.Show (String.Format (strings ["ErrLdInp"].text, ex.Message), Statics.strings ["Error"].text);
                }
            if (File.Exists (Statics.fn_remap)) {
                FileStream fs = File.OpenRead (Statics.fn_remap);
                fs.Read (Statics.Remapper, 0, 256);
                fs.Close ();
            }
            if (File.Exists (Statics.fn_dlver) && File.Exists (Statics.fn_usagedata)) {
                byte [] bytes = File.ReadAllBytes (Statics.fn_dlver);
                if (bytes.Length == 1 && bytes [0] == (byte)Statics.DistantLandVersion) {
                    BinaryReader br = new BinaryReader (File.OpenRead (Statics.fn_usagedata));
                    /*br.BaseStream.Position = 4;
                    long size = br.ReadInt32 () * 32 + 8;
                    if (br.BaseStream.Length == size) {
                        br.Close ();
                        if (MessageBox.Show ("Statics usage file uses old format, that doesn't contain \"Minimum static size\" saved.\nDo you want to update this file and save \"Minimum static size\" to it?\n\nIt will be still compatible with older versions of MGE.", "Update Statics usage data", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes) {
                            BinaryWriter bw = new BinaryWriter (File.OpenWrite (Statics.fn_usagedata));
                            bw.BaseStream.Position = size;
                            bw.Write ((int)0);
                            bw.Write (Convert.ToSingle (tbDLNearSize.Text));
                            bw.Close ();
                            MessageBox.Show ("Statics usage file was updated to new format,\nand is still compatible with older versions of MGE.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                    } else {/**/
                    br.BaseStream.Position = br.BaseStream.Length - 4;
                    tbDLNearSize.Text = br.ReadSingle ().ToString ();
                    br.Close ();
                    //}
                }
            }
            ValidateDistances (null, null);
            udMinView_ValueChanged (null, null);
            udMaxView_ValueChanged (null, null);
            if (!(this.rbDLSM30.Enabled = DXMain.mCaps.SupportsSM3)) {
                this.rbDLSM20.Checked = true;
            }
        }

        private void IgnoreKey (object sender, KeyPressEventArgs e) {
            e.Handled = true;
        }

        private void bMacroEd_Click (object sender, EventArgs e) {
            (new MacroEditorForm ()).ShowDialog ();
        }

        private void MainForm_FormClosing (object sender, CancelEventArgs e) {
            if (Save) SaveSettings ();
        }

        private void bQuality_Click (object sender, EventArgs e) {
            int MaxAA = cbWindowed.Checked ? DXMain.mCaps.MaxWindowedAA : DXMain.mCaps.MaxFullscreenAA;
            if (MaxAA == 0) cmbAntiAlias.SelectedIndex = 0; else cmbAntiAlias.SelectedIndex = MaxAA - 1;
            cmbMipFilter.SelectedIndex = 2;
            cmbScaleFilter.SelectedIndex = 2;
            switch (DXMain.mCaps.MaxAF) {
                case 2:
                    cmbAnisoLevel.SelectedIndex = 1;
                    break;
                case 4:
                    cmbAnisoLevel.SelectedIndex = 2;
                    break;
                case 8:
                    cmbAnisoLevel.SelectedIndex = 3;
                    break;
                case 16:
                    cmbAnisoLevel.SelectedIndex = 4;
                    break;
                default:
                    cmbMipFilter.SelectedIndex = 1;
                    cmbScaleFilter.SelectedIndex = 1;
                    cmbAnisoLevel.SelectedIndex = 0;
                    break;
            }
            cmbZBuffer.SelectedIndex = 0;
            if (MaxAA > 0) cbAAFix.Checked = true; else cbAAFix.Checked = false;
            udLOD.Value = -1;
        }

        private void bHelp_Click (object sender, EventArgs e) {
            try {
                System.Diagnostics.Process p = System.Diagnostics.Process.Start (@"http://morrgraphext.wiki.sourceforge.net/");
                p.Close ();
            } catch { }
        }

        private void bShaderEd_Click (object sender, EventArgs e) {
            ShaderEditorForm ed = new ShaderEditorForm ();
            ed.ShowDialog ();
        }

        private void cbWindowed_CheckedChanged (object sender, EventArgs e) {
            cmbRefreshRate.Enabled = !cbWindowed.Checked && !cbDisableMGE.Checked;
            lRefreshRate.Enabled = !cbWindowed.Checked && !cbDisableMGE.Checked;
            bCalcRefresh.Enabled = !cbWindowed.Checked && !cbDisableMGE.Checked;
            RegistryKey key = null;
            try {
                key = Registry.LocalMachine.OpenSubKey (Statics.reg_mw, true);
            } catch {
                MessageBox.Show (strings ["RegNotWrit"].text, Statics.strings ["Error"].text);
            }
            if (key != null) {
                key.SetValue ("Fullscreen", new byte [] { Convert.ToByte (!cbWindowed.Checked) });
                key.Close ();
            }
        }

        private void cbFPSConst_CheckedChanged (object sender, EventArgs e) {
            gbFPSConst.Enabled = cbFPSConst.Checked;
        }

        private void bExport_Click (object sender, EventArgs e) {
            if (SaveFileDialog.ShowDialog () == DialogResult.OK) {
                BinaryWriter bw = new BinaryWriter (File.Open (SaveFileDialog.FileName, FileMode.Create));
                SaveSettings ();
                byte [] b;
                b = File.ReadAllBytes (Statics.fn_didata);
                bw.Write (b.Length);
                bw.Write (b);
                bw.Write (0); // used to be 'MGE3/settings' file
                b = File.ReadAllBytes (Statics.fn_macro);
                bw.Write (b.Length);
                bw.Write (b);
                b = File.ReadAllBytes (Statics.fn_triger);
                bw.Write (b.Length);
                bw.Write (b);
                b = File.ReadAllBytes (Statics.fn_mwini);
                bw.Write (b.Length);
                bw.Write (b);
                b = File.ReadAllBytes (Statics.iniFileName);
                bw.Write (b.Length);
                bw.Write (b);
                bw.Close ();
            }
        }

        private void bImport_Click (object sender, EventArgs e) {
            if (OpenFileDialog.ShowDialog () == DialogResult.OK) {
                BinaryReader br = new BinaryReader (File.OpenRead (OpenFileDialog.FileName));
                FileStream fs;
                byte [] b;
                long eof = br.BaseStream.Length;
                int len;
                len = br.ReadInt32 ();
                b = br.ReadBytes (len);
                fs = File.Open (Statics.fn_didata, FileMode.Create);
                fs.Write (b, 0, len);
                fs.Close ();
                len = br.ReadInt32 ();
                if (len > 0) {
                    b = br.ReadBytes (len);
                    fs = File.Open (Statics.fn_settings, FileMode.Create);
                    fs.Write (b, 0, len);
                    fs.Close ();
                }
                len = br.ReadInt32 ();
                b = br.ReadBytes (len);
                fs = File.Open (Statics.fn_macro, FileMode.Create);
                fs.Write (b, 0, len);
                fs.Close ();
                len = br.ReadInt32 ();
                b = br.ReadBytes (len);
                fs = File.Open (Statics.fn_triger, FileMode.Create);
                fs.Write (b, 0, len);
                fs.Close ();
                if (cbImportIni.Checked) {
                    if (br.BaseStream.Position < eof) {
                        MessageBox.Show (strings ["MWIniNotInMGE"].text, Statics.strings ["Message"].text);
                    } else {
                        len = br.ReadInt32 ();
                        b = br.ReadBytes (len);
                        fs = File.Open (Statics.fn_mwini, FileMode.Create);
                        fs.Write (b, 0, len);
                        fs.Close ();
                    }
                } else {
                    if (br.BaseStream.Position < eof) {
                        len = br.ReadInt32 ();
                        b = br.ReadBytes (len);
                    }
                }
                if (br.BaseStream.Position < eof) {
                    len = br.ReadInt32 ();
                    b = br.ReadBytes (len);
                    fs = File.Open (Statics.iniFileName, FileMode.Create);
                    fs.Write (b, 0, len);
                    fs.Close ();
                }
                br.Close ();
                LoadSettings ();
            }
        }

        private void bViewCaps_Click (object sender, EventArgs e) {
            MessageBox.Show (String.Format (strings ["ViewCaps"].text, DXMain.mCaps.MaxFullscreenAA.ToString (),
                DXMain.mCaps.MaxWindowedAA.ToString (), DXMain.mCaps.MaxAF.ToString ()), strings ["GraphCaps"].text);
        }

        private void cbDisplayMessages_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDisplayMessages.Checked;
            cbFPSCounter.Checked &= status;
            cbFPSCounter.Enabled = status;
            gbMsgs.Enabled = status;
        }

        private void bCalcRefresh_Click(object sender, EventArgs e)
        {
            int width, height;
            RegistryKey key = Registry.LocalMachine.OpenSubKey(Statics.reg_mw, true);
            try
            {
                width = (int)key.GetValue("Screen Width");
                height = (int)key.GetValue("Screen Height");
            }
            catch (NullReferenceException)
            {
                key.SetValue("Screen Width", 640);
                key.SetValue("Screen Height", 480);
                key.SetValue("Fullscreen", new byte[] { Convert.ToByte(1) });
                width = 640;
                height = 480;
            }   //mendres: Crashed, when Morrowind Launcher didn't run before.
            key.Close();
            cmbRefreshRate.Items.Clear();
            cmbRefreshRate.Items.Add("Default");
            cmbRefreshRate.Text = "Default";
            foreach (int i in DirectX.DXMain.GetRefreshRates(width, height))
            {
                cmbRefreshRate.Items.Add(i.ToString());
            }
        }

        private void bCalcResolution_Click (object sender, EventArgs e) {
            System.Drawing.Point p;
            if (ResolutionForm.ShowDialog (out p, cbWindowed.Checked)) {
                tbResolution.Text = p.X.ToString () + " x " + p.Y.ToString ();
                string refrRate = cmbRefreshRate.Text;
                bCalcRefresh_Click (null, null);
                cmbRefreshRate.SelectedIndex = cmbRefreshRate.FindStringExact (refrRate);
                if (cmbRefreshRate.SelectedIndex == -1) cmbRefreshRate.SelectedIndex = 0;
                CalcAspectRatio (p.X, p.Y);
                if (((float)p.Y / (float)p.X) < 0.75) cbAspectZoom.Checked = true;
            }
        }

        private void cbIdle_CheckedChanged (object sender, EventArgs e) {
            gbIdle.Enabled = cbIdle.Checked;
        }

        private void cbHDR_CheckedChanged (object sender, EventArgs e) {
            gbHDR.Enabled = cbHDR.Checked;
        }

        private class AboutForm : Form {
            private System.Timers.Timer OpacityTimer;
            private System.ComponentModel.IContainer components = null;
            delegate void SetOpacityCallback (double opacity);
            public AboutForm (string message, string title, string close) {
                this.SuspendLayout ();
                // bClose
                System.Windows.Forms.Button bClose = new System.Windows.Forms.Button ();
                bClose.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
                bClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
                bClose.Location = new System.Drawing.Point (194, 143);
                bClose.Size = new System.Drawing.Size (128, 25);
                bClose.TabIndex = 0;
                bClose.UseVisualStyleBackColor = true;
                bClose.Text = close;
                // lBody
                System.Windows.Forms.Label lBody = new System.Windows.Forms.Label ();
                lBody.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
                lBody.AutoEllipsis = true;
                lBody.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
                lBody.Location = new System.Drawing.Point (116, 9);
                lBody.Size = new System.Drawing.Size (292, 131);
                lBody.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
                lBody.UseMnemonic = false;
                lBody.Text = message;
                // this
                this.AcceptButton = bClose;
                this.AutoScaleDimensions = new System.Drawing.SizeF (96F, 96F);
                this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
                this.AutoSize = true;
                this.CancelButton = bClose;
                this.ClientSize = new System.Drawing.Size (420, 180);
                this.Controls.Add (lBody);
                this.Controls.Add (bClose);
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
                this.MaximizeBox = false;
                this.MinimizeBox = false;
                this.Opacity = 0;
                this.ShowIcon = false;
                this.ShowInTaskbar = false;
                this.StartPosition = FormStartPosition.CenterScreen;
                this.Text = title;
                this.ResumeLayout (false);
                // alphaTimer
                this.OpacityTimer = new System.Timers.Timer ();
                this.OpacityTimer.Elapsed += new System.Timers.ElapsedEventHandler (IncreaseOpacityEvent);
                this.OpacityTimer.Interval = 25;
                this.OpacityTimer.Start ();
            }
            protected override void Dispose (bool disposing) {
                if (disposing && (components != null)) {
                    components.Dispose ();
                }
                base.Dispose (disposing);
            }
            protected override void OnPaint (System.Windows.Forms.PaintEventArgs e) {
                Graphics graphics = e.Graphics;
                Icon ic = new Icon (Properties.Resources.AppIcon, 96, 96);
                graphics.DrawIcon (ic, 12, 24);
                graphics.Dispose ();
            }
            private void SetOpacity (double opacity) {
                if (this.InvokeRequired) {
                    SetOpacityCallback d = new SetOpacityCallback (SetOpacity);
                    this.Invoke (d, new object [] { opacity });
                } else {
                    this.Opacity = opacity;
                }
            }
            private void IncreaseOpacityEvent (object source, System.Timers.ElapsedEventArgs e) {
                if (this.Opacity < 0.9) this.SetOpacity (this.Opacity + 0.03);
                else OpacityTimer.Elapsed -= IncreaseOpacityEvent;
            }
        }

        private void bAbout_Click (object sender, EventArgs e) {
            string s = String.Format ("Morrowind Graphics Extender\n{0}\n\n{1} Timeslip (www.timeslipped.co.uk)\n\n{2}: LizTail, Krzymar & Phal", Statics.versionString, strings ["CreatedBy"].text, strings ["FurtherDevel"].text);
            if (cmbUILanguage.SelectedIndex > 0) {
                string ttn = Statics.strings ["Translation"].text;
                string ttr = Statics.Localizations [cmbUILanguage.Text].Translator;
                if (ttn != "" && ttr != "") s += String.Format ("\n\n{0}: {1}", ttn, ttr);
            }
            (new AboutForm (s, strings ["About"].text, Statics.strings ["Close"].text)).ShowDialog (this);
        }

        private void bRemapper_Click (object sender, EventArgs e) {
            (new KeyRemapper ()).ShowDialog ();
        }

        private void bHookGen_Click (object sender, EventArgs e) {
            (new HookCreator ()).ShowDialog ();
        }

        private void cmbFogMode_SelectedIndexChanged (object sender, EventArgs e) {
            if (cmbFogMode.SelectedIndex != 0 && cbDLDistantLand.Checked) {
                MessageBox.Show (strings ["DLVertexFog"].text, Statics.strings ["Warning"].text);
            }
        }

        private void cmbAlias_SelectedIndexChanged (object sender, EventArgs e) {
            if (cmbAntiAlias.SelectedIndex == 0) return;
            if (!DXMain.CheckAALevel (cmbAntiAlias.SelectedIndex + 1, cbWindowed.Checked)) {
                MessageBox.Show (strings ["TooHighAA"].text, Statics.strings ["Error"].text);
                cmbAntiAlias.SelectedIndex = 0;
            }
        }

        private void cmbAnsiLevel_SelectedIndexChanged (object sender, EventArgs e) {
            if (cmbAnisoLevel.SelectedIndex == 0) return;
            if (1 << cmbAnisoLevel.SelectedIndex > DXMain.mCaps.MaxAF) {
                MessageBox.Show (strings ["TooHighAF"].text, Statics.strings ["Error"].text);
                cmbAnisoLevel.SelectedIndex = 0;
            }
        }

        private void cbDLReflectNStatics_CheckedChanged (object sender, EventArgs e) {
            cbDLReflFStatics.Enabled = cbDLDistantLand.Checked && cbDLReflLand.Checked && cbDLDistantStatics.Checked && cbDLReflNStatics.Checked;
        }

        private void cbDLReflectiveWater_CheckedChanged (object sender, EventArgs e) {
            //if (cbDLReflLand.Checked && !DXMain.mCaps.SupportsSM2) {
            //    cbDLReflLand.Checked = false;
            //    MessageBox.Show ("You graphics card lacks the required shader model 2.0 support required to use reflective water.", Statics.strings ["Error"].text);
            //    return;
            //}
            //cbDLSkyRefl.Enabled = cbDLReflLand.Checked;
            cbDLReflNStatics.Enabled = cbDLDistantLand.Checked && cbDLReflLand.Checked && cbDLDistantStatics.Checked;
            cbDLReflFStatics.Enabled = cbDLDistantLand.Checked && cbDLReflLand.Checked && cbDLDistantStatics.Checked && cbDLReflNStatics.Checked;
        }

        private void cbDistantStatics_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDLDistantStatics.Checked;
            if (cbDLDistantStatics.CheckState != CheckState.Indeterminate) {
                if (status && (!File.Exists(Statics.fn_usagedata) || !File.Exists(Statics.fn_statmesh))) {
                    cbDLDistantStatics.Checked = false;
                    if (!loading) MessageBox.Show(strings["DSMissing"].text, Statics.strings["Message"].text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                    cbDLDistantStatics.CheckState = CheckState.Indeterminate;
                    return;
                }
            }
            gbDLStatics.Enabled = status;
            lDLFogI.Enabled = udDLFogIStart.Enabled = udDLFogIEnd.Enabled = status && !cbDLAutoDist.Checked && !DLOptions ["DLNotInt"];
            cbDLReflNStatics.Enabled = status && cbDLReflLand.Checked && cbDLDistantStatics.Checked;
            cbDLReflFStatics.Enabled = status && cbDLReflLand.Checked && cbDLDistantStatics.Checked && cbDLReflNStatics.Checked;
        }

        private void cbDistantLand_CheckedChanged (object sender, EventArgs e) {
            if (cbDLDistantLand.Checked) {
                if (!DXMain.mCaps.SupportsSM2) {
                    cbDLDistantLand.Checked = false;
                    MessageBox.Show (strings ["DLLackSM2"].text, Statics.strings ["Error"].text);
                    return;
                }
                if (cbDLDistantLand.CheckState != CheckState.Indeterminate) {
                    if (!File.Exists (Statics.fn_dlver) || !File.Exists (Statics.fn_world) || !File.Exists (Statics.fn_worldds) || !File.Exists (Statics.fn_worldn)) {
                        cbDLDistantLand.Checked = false;
                        if (!loading) MessageBox.Show (strings["NoDLOrOld"].text, Statics.strings["Message"].text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                        cbDLDistantLand.CheckState = CheckState.Indeterminate;    
                        return;
                    }
                    byte [] bytes = File.ReadAllBytes (Statics.fn_dlver);
                    if (bytes.Length != 1 || bytes [0] != Statics.DistantLandVersion) {
                        cbDLDistantLand.Checked = false;
                        MessageBox.Show (strings ["DLDiffer"].text, Statics.strings ["Error"].text);
                        return;
                    }
                }
                gbDistantLand.Enabled = true;
                cmbZBuffer.Enabled = false; lZBuffer.Enabled = false;
                cmbZBuffer.SelectedIndex = 0;
            } else {
                gbDistantLand.Enabled = false;
                cmbZBuffer.Enabled = true; lZBuffer.Enabled = true;
            }
            cbDLReflNStatics.Enabled = cbDLDistantLand.Checked && cbDLReflLand.Checked && cbDLDistantStatics.Checked;
            cbDLReflFStatics.Enabled = cbDLReflNStatics.Enabled && cbDLReflNStatics.Checked;
        }

        enum AutoDistance { none, byDrawDist, byAFogEnd };

        private AutoDistance autoDistances = AutoDistance.none;

        //Corrects any distant land settings that cause known problems
        private void ValidateDLSettings (object sender, EventArgs e) {
            if (loading) return;
            if (autoDistances == AutoDistance.none) ValidateDistances (sender, e);
            else if (sender == udDLFogAStart) ValidateDistances (udDLFogAEnd, e);
        }

        //Corrects any distant land settings that cause known problems
        private void ValidateDistances (object sender, EventArgs e) {
            if (loading) return;
            loading = true;
            int near_size, far_size, v_far_size;
            near_size = Convert.ToInt32 (tbDLNearSize.Text);
            far_size = (int)udDLSizeFar.Value;
            v_far_size = (int)udDLSizeVeryFar.Value;
            if (far_size < near_size) far_size = near_size;
            if (sender == udDLSizeVeryFar && (v_far_size >= near_size && v_far_size < far_size)) far_size = v_far_size;
            if (v_far_size < far_size) v_far_size = far_size;
            udDLSizeFar.Value = far_size;
            udDLSizeVeryFar.Value = v_far_size;
            double draw_dist, fog_start_above, fog_end_above, fog_start_below, fog_end_below, fog_start_int, fog_end_int, near_dist, far_dist, v_far_dist;
            draw_dist = (double)(udDLDrawDist.Value * (cbDLFogExp.Checked ? 1.0M / udDLFogExpMul.Value : 1.0M));
            fog_start_above = (double)udDLFogAStart.Value;
            fog_end_above = (double)udDLFogAEnd.Value;
            fog_start_below = (double)udDLFogBStart.Value;
            fog_end_below = (double)udDLFogBEnd.Value;
            fog_start_int = (double)udDLFogIStart.Value;
            fog_end_int = (double)udDLFogIEnd.Value;
            near_dist = (double)udDLDistNear.Value;
            far_dist = (double)udDLDistFar.Value;
            v_far_dist = (double)udDLDistVeryFar.Value;
            if (draw_dist < fog_end_above || sender == udDLFogAEnd) {
                if (draw_dist < fog_end_above) {
                    if (sender == udDLFogAEnd) draw_dist = fog_end_above;
                    else fog_end_above = draw_dist;
                }
                if (fog_start_above >= fog_end_above) fog_start_above = fog_end_above - 0.1;
            }
            //Prevent fog end from being less than or equal to fog start
            if (fog_start_above < 0) fog_start_above = 0;
            if (fog_end_above <= fog_start_above) fog_end_above = fog_start_above + 0.1;
            if (draw_dist < fog_end_below || sender == udDLFogBEnd) {
                if (draw_dist < fog_end_below) fog_end_below = draw_dist;
                if (fog_start_below >= fog_end_below) fog_start_below = fog_end_below - 0.1;
            }
            if (fog_start_below < -99.9) fog_start_below = -99.9;
            if (fog_end_below <= fog_start_below) fog_end_below = fog_start_below + 0.1;

            if (fog_end_int < 1.0 && fog_end_int > draw_dist) {
                fog_end_int = draw_dist;
                sender = null;
            }
            if (fog_start_int >= fog_end_int && sender != udDLFogIStart) fog_start_int = fog_end_int - 0.1;
            if (fog_end_int <= fog_start_int) fog_end_int = fog_start_int + 0.1;

            if (sender == udDLDistFar && (near_dist >= far_dist)) near_dist = far_dist - 0.1;
            if (sender == udDLDistVeryFar && (far_dist >= v_far_dist)) {
                far_dist = v_far_dist - 0.1;
                if (near_dist >= far_dist) near_dist = far_dist - 0.1;
            }
            //Prevent the near static distance from ever being less than Morrowind's draw distance
            if (near_dist < 2) near_dist = 2;
            //Prevent any distant static distance from being less than or equal to the one before it.
            if (far_dist <= near_dist) far_dist = near_dist + 0.1;
            if (v_far_dist <= far_dist) v_far_dist = far_dist + 0.1;
            udDLFogAStart.Value = (decimal)fog_start_above;
            udDLFogAEnd.Value = (decimal)fog_end_above;
            udDLFogBStart.Value = (decimal)fog_start_below;
            udDLFogBEnd.Value = (decimal)fog_end_below;
            udDLFogIStart.Value = (decimal)fog_start_int;
            udDLFogIEnd.Value = (decimal)fog_end_int;
            udDLDistNear.Value = (decimal)near_dist;
            udDLDistFar.Value = (decimal)far_dist;
            udDLDistVeryFar.Value = (decimal)v_far_dist;
            if (cbDLFogExp.Checked) {
                draw_dist *= (double)udDLFogExpMul.Value;
                if (draw_dist > (double)udDLDrawDist.Maximum) draw_dist = (double)udDLDrawDist.Maximum;
            }
            udDLDrawDist.Value = (decimal)draw_dist;
            loading = false;
        }

        private void AutoSetDistances (object sender, EventArgs e) {
            double draw_distance, dist_root;
            loading = true;
            if (autoDistances == AutoDistance.byAFogEnd) {
                decimal distance = udDLFogAEnd.Value;
                if (cbDLFogExp.Checked) distance *= udDLFogExpMul.Value;
                if (distance < 1.0M) distance = 1.0M;
                else if (distance > 300.0M) distance = 300.0M;
                udDLDrawDist.Value = distance;
            }
            draw_distance = (double)(udDLDrawDist.Value * (cbDLFogExp.Checked ? 1.0M / udDLFogExpMul.Value : 1.0M));
            dist_root = Math.Sqrt (draw_distance);
            //udDLFogAStart.Value = (decimal)(draw_distance * 0.18);
            if (autoDistances != AutoDistance.byAFogEnd) {
                udDLFogAStart.Value = (decimal)(draw_distance * (cbDLFogExp.Checked ? 0.15 : 0.05));
                udDLFogAEnd.Value = (decimal)draw_distance;
            }
            udDLFogBStart.Value = (decimal)(Math.Max (dist_root * -0.45, -99.9));
            udDLFogBEnd.Value = (decimal)(dist_root < 0.25 ? 0.1 : dist_root * 0.45);
            udDLFogIStart.Value = 0.0M;
            udDLFogIEnd.Value = 0.9M;
            udDLDistNear.Value = (decimal)(draw_distance * 0.2) * (cbDLFogExp.Checked ? 0.75M * udDLFogExpMul.Value : 1.0M);
            //udDLDistFar.Value = (decimal)(draw_distance * 0.5);
            udDLDistVeryFar.Value = (decimal)(draw_distance * 0.9) * (cbDLFogExp.Checked ? 0.75M * udDLFogExpMul.Value : 1.0M);
            udDLDistFar.Value = (udDLDistNear.Value + udDLDistVeryFar.Value) * 0.5M;
            //Make sure improper values have not been generated
            loading = false;
            ValidateDistances (sender, e);
        }

        private void udRotation_ValueChanged (object sender, EventArgs e) {
            if (udRotation.Value < 0) udRotation.Value = udRotation.Value + 360;
            if (udRotation.Value >= 360) udRotation.Value = udRotation.Value - 360;
        }

        private void udMinView_ValueChanged (object sender, EventArgs e) {
            if (loading) return;
            if (udMinView.Value > udMaxView.Value) udMaxView.Value = udMinView.Value;
        }

        private void udMaxView_ValueChanged (object sender, EventArgs e) {
            if (loading) return;
            if (udMaxView.Value < udMinView.Value) udMinView.Value = udMaxView.Value;
        }

        private void bResetSettings_Click (object sender, EventArgs e) {
            bool delete = cbClearINI.Checked;
            DialogResult res = MessageBox.Show (String.Format (strings ["AskReset"].text, (delete ? strings ["ResetSaved"].text : "")), strings ["ResetAsk"].text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2);
            if (res == DialogResult.Yes) {
                LoadGraphicsSettings (true, delete);
                INIFile iniFile = new INIFile (Statics.iniFileName, DLWeatherForm.iniWeatherSettings, true);
                iniFile.reinitialize ();
                iniFile.save ();
                cbSkipMovie.Checked = false;
                cbConsole.Checked = false;
                cbClearINI.Checked = false;
                cbImportIni.Checked = false;
                cbAltCombat.Checked = false;
            }
        }

        private void bDLWthr_Click (object sender, EventArgs e) {
            (new DLWeatherForm ()).ShowDialog ();
        }

        private void bDLOptions_Click (object sender, EventArgs e) {
            (new DLOptionsForm (DLOptions)).ShowDialog ();
        }

        private void bDistantLandWizard_Click (object sender, EventArgs e) {
            if (!DXMain.mCaps.SupportsSM2) {
                MessageBox.Show (strings ["DLLackSM2"].text, Statics.strings ["Error"].text);
                return;
            }
            bool exists = false;
            if (Directory.Exists (Statics.fn_dl)) {
                if (!File.Exists (Statics.fn_dlver) || !File.Exists (Statics.fn_world) || !File.Exists (Statics.fn_worldds) || !File.Exists (Statics.fn_worldn)) {
                    if (MessageBox.Show (strings ["DLDelOldCorrupt"].text, Statics.strings ["Warning"].text, MessageBoxButtons.YesNo) != DialogResult.Yes) return;
                    Directory.Delete (Statics.fn_dl, true);
                } else {
                    byte [] b = File.ReadAllBytes (Statics.fn_dlver);
                    if (b.Length != 1 || b [0] != Statics.DistantLandVersion) {
                        if (MessageBox.Show (strings ["DLDelDifferent"].text, Statics.strings ["Warning"].text, MessageBoxButtons.YesNo) != DialogResult.Yes) return;
                        Directory.Delete (Statics.fn_dl, true);
                    } else exists = true;
                }
            }
            (new DistantLand.DistantLandForm (exists)).ShowDialog ();
        }

        private void CalcAspectRatio (int x, int y) {
            int dx = x;
            int dy = y;
            while (dx != dy) {
                if (dx > dy) dx = dx - dy;
                else dy = dy - dx;
            }
            tbAspect.Text = (x / dx).ToString () + ":" + (y / dx).ToString ();
        }

        private void tbSSDir_TextAlignChanged (object sender, EventArgs e) {
            bSShotDirClear.Enabled = (tbSShotDir.TextAlign == HorizontalAlignment.Left);
        }

        private void bSSDirClear_Click (object sender, EventArgs e) {
            tbSShotDir.TextAlign = HorizontalAlignment.Center;
            tbSShotDir.Text = clearedSSDir;
        }

        private void bSShotDirBrowse_Click (object sender, EventArgs e) {
            if (tbSShotDir.TextAlign == HorizontalAlignment.Left) SShotFolderDialog.SelectedPath = tbSShotDir.Text;
            else SShotFolderDialog.SelectedPath = Statics.runDir;
            if (SShotFolderDialog.ShowDialog () == DialogResult.OK) {
                tbSShotDir.TextAlign = HorizontalAlignment.Left;
                tbSShotDir.Text = SShotFolderDialog.SelectedPath;
            }
        }

        private void fileNameFilter_KeyPress (object sender, KeyPressEventArgs e) {
            switch (e.KeyChar) {
                case '*':
                case '?':
                case '"':
                case '/':
                case '\\':
                case ':':
                case '|':
                case '>':
                case '<':
                    e.Handled = true;
                    break;
            }
        }

        private void cbDisableMGE_CheckedChanged (object sender, EventArgs e) {
            bool status = !this.cbDisableMGE.Checked;
            this.gbMainQuality.Enabled = status;
            this.gbMainSettings.Enabled = status;
            this.cmbRefreshRate.Enabled = status && !cbWindowed.Checked;
            this.lRefreshRate.Enabled = status && !cbWindowed.Checked;
            this.bCalcRefresh.Enabled = status && !cbWindowed.Checked;
            this.gbScene.Enabled = status;
            this.gbSShot.Enabled = status;
            this.gbTexFilt.Enabled = status;
            this.gbScreen.Enabled = status;
            this.gbFog.Enabled = status;
            this.gbRendOpts.Enabled = status;
            this.gbMsgs.Enabled = status;
            this.gbInputOpts.Enabled = status;
            this.gbInputTools.Enabled = status;
            this.gbShaders.Enabled = status;
            this.cbFPSConst.Enabled = status;
            this.gbFPSConst.Enabled = status && this.cbFPSConst.Checked;
            this.cbIdle.Enabled = status;
            this.gbIdle.Enabled = status && this.cbIdle.Checked;
            this.cbZoomEffectsHUD.Enabled = status;
            this.cbAspectZoom.Enabled = status;
            this.cbBindAI.Enabled = status;
            this.bShaderEd.Enabled = status;
            this.bHookGen.Enabled = status;
            this.bDistantLandWizard.Enabled = status;
            this.cbDLDistantLand.Enabled = status;
            this.gbDistantLand.Enabled = status && this.cbDLDistantLand.Checked;
            this.cbScreenshots.Checked &= !status;
            this.cbScreenshots.Enabled = !status;
        }

        private void rbDLSM_CheckedChanged (object sender, EventArgs e) {
            bool status = this.rbDLSM30.Checked;
            this.cbDLReflBlur.Enabled = status;
            this.cbDLFogExp.Enabled = status;
            this.gbDLWtrWave.Enabled = status;
            //this.gbDLWtrCaust.Enabled = status;
            this.lDLFogExpMul.Enabled = status && this.cbDLFogExp.Checked;
            this.udDLFogExpMul.Enabled = status && this.cbDLFogExp.Checked;
            /**/
            if (!status) {
                this.cbDLFogExp.Checked = false;    // should this be reset?
                if (this.cbDLAutoDist.Checked) AutoSetDistances (sender, e);
            }/**/
        }

        private void udDLDrawDist_ValueChanged (object sender, EventArgs e) {
            if (loading) return;
            if (autoDistances == AutoDistance.byDrawDist) AutoSetDistances (sender, e);
            else ValidateDLSettings (sender, e);
        }

        private void udDLFogAEnd_ValueChanged (object sender, EventArgs e) {
            if (loading) return;
            if (autoDistances == AutoDistance.byAFogEnd) {
                AutoSetDistances (sender, e);
                ValidateDistances (null, null);
            } else ValidateDLSettings (sender, e);
        }

        private void cbAutoSetDistances_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDLAutoDist.Checked;
            gbDLAutoDist.Enabled = status;
            gbDLDrawDist.Enabled = !status || !rbDLAutoByAFogEnd.Checked;
            udDLFogAStart.Enabled = !status || !rbDLAutoByDrawDist.Checked;
            udDLFogAEnd.Enabled = !status || !rbDLAutoByDrawDist.Checked;
            udDLFogBStart.Enabled = !status;
            udDLFogBEnd.Enabled = !status;
            lDLFogI.Enabled = udDLFogIStart.Enabled = udDLFogIEnd.Enabled = !status && cbDLDistantStatics.Checked && !DLOptions ["DLNotInt"];
            udDLDistNear.Enabled = !status;
            udDLDistFar.Enabled = !status;
            udDLDistVeryFar.Enabled = !status;
            if (status) {
                if (rbDLAutoByDrawDist.Checked) autoDistances = AutoDistance.byDrawDist;
                else autoDistances = AutoDistance.byAFogEnd;
                if (!loading) AutoSetDistances (sender, e);
            } else autoDistances = AutoDistance.none;
        }

        private void cbDLFogExp_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDLFogExp.Checked;
            lDLFogExpMul.Enabled = status;
            udDLFogExpMul.Enabled = status;
            if (loading) return;
            decimal draw_dist = udDLDrawDist.Value;
            if (status) {
                draw_dist *= udDLFogExpMul.Value;
                if (draw_dist > udDLDrawDist.Maximum) draw_dist = udDLDrawDist.Maximum;
            } else {
                draw_dist /= udDLFogExpMul.Value;
                if (draw_dist < udDLDrawDist.Minimum) draw_dist = udDLDrawDist.Minimum;
            }
            udDLDrawDist.Value = draw_dist;
            if (cbDLAutoDist.Checked) AutoSetDistances (sender, e);
        }

        private void udDLFogExpMul_ValueChanged (object sender, EventArgs e) {
            if (loading) return;
            if (cbDLAutoDist.Checked) AutoSetDistances (sender, e);
            else ValidateDistances (sender, e);
        }

        private void rbDLAutoDist_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDLAutoDist.Checked;
            gbDLDrawDist.Enabled = !status || !rbDLAutoByAFogEnd.Checked;
            udDLFogAStart.Enabled = !status || !rbDLAutoByDrawDist.Checked;
            udDLFogAEnd.Enabled = !status || !rbDLAutoByDrawDist.Checked;
            if (status) {
                if (rbDLAutoByDrawDist.Checked) autoDistances = AutoDistance.byDrawDist;
                else autoDistances = AutoDistance.byAFogEnd;
                if (!loading) AutoSetDistances (sender, e);
            } else autoDistances = AutoDistance.none;
        }

        private void TooltipTimeoutHandler (object sender, EventArgs e) {
            Control control = (sender as Control);
            int delay = toolTip.GetToolTip (control).Length * Statics.tipReadSpeed;
            toolTip.AutoPopDelay = (delay < 32768 ? delay : 32767);
        }

        private void cmbMainTipReadSpd_SelectedIndexChanged (object sender, EventArgs e) {
            try {
                Statics.tipReadSpeed = (int)(1000 / double.Parse (cmbTipReadSpd.Text));
            } catch {
                Statics.tipReadSpeed = (int)(1000 / double.Parse (cmbTipReadSpd.Text.Replace(".", System.Globalization.NumberFormatInfo.CurrentInfo.NumberDecimalSeparator)));
            }
        }

        private void cbDLDynRipples_CheckedChanged (object sender, EventArgs e) {
            bool status = cbDLDynRipples.Checked;
            bDLDynRipples.Enabled = status;
            lDLDynRipples.Enabled = status;
        }

        private void bDLDynRipples_Click (object sender, EventArgs e) {
            (new DLRipplesForm ()).ShowDialog ();
        }

        private void MainForm_Resize (object sender, EventArgs e) {
            int y = (this.Size.Height - this.MinimumSize.Height) / 3;
            gbMainSettings.Location = new System.Drawing.Point (gbMainSettingsLocation.X, gbMainSettingsLocation.Y + y);
            gbMainUI.Location = new System.Drawing.Point (gbMainUILocation.X, this.Size.Height + gbMainUILocation.Y - y);
        }

        private void cmbUILanguage_SelectedIndexChanged (object sender, EventArgs e) {
            foreach (KeyValuePair<Control, string> text in default_text) text.Key.Text = text.Value;
            foreach (KeyValuePair<TabPage, string> text in tabpage_tooltips) text.Key.ToolTipText = text.Value;
            foreach (KeyValuePair<string, Tip> text in tooltip_messages) text.Value.reset ();
            foreach (KeyValuePair<string, Str> text in Statics.strings) text.Value.reset ();
            foreach (KeyValuePair<string, Str> text in strings) text.Value.reset ();
            string s = tbSShotDir.Text;
            Statics.Localizations.ApplyStrings ("", Statics.strings, cmbUILanguage.Text);
            Statics.Localizations.Apply (this, cmbUILanguage.Text);
            clearedSSDir = tbSShotDir.Text;
            if (tbSShotDir.TextAlign != HorizontalAlignment.Center) tbSShotDir.Text = s;
            SetTooltips ();
        }

        private void cbUILangAuto_CheckedChanged (object sender, EventArgs e) {
            bool status = cbUILangAuto.Checked;
            cmbUILanguage.Enabled = !status;
            if (status) {
                int index = cmbUILanguage.FindStringExact (LocalizationInterface.SysLang);
                if (index == -1) {
                    index = cmbUILanguage.FindStringExact (LocalizationInterface.SysLangRev);
                    if (index == -1) {
                        index = cmbUILanguage.FindString (LocalizationInterface.SysLangOrg);
                        if (index == -1) {
                            index = cmbUILanguage.FindString (LocalizationInterface.SysLangEng);
                        }
                    }
                }
                if (index != -1) cmbUILanguage.SelectedIndex = index;
            }
        }

        private void cbCam3rdPrsn_CheckedChanged (object sender, EventArgs e) {
            gbCam3rdPrsn.Enabled = cbCam3rdPrsn.Checked;
        }

        private void cbDisableMWE_CheckStateChanged(object sender, EventArgs e) {
            switch (cbDisableMWE.CheckState) {
                case CheckState.Checked:        bMWE.Enabled = false;   break;
                case CheckState.Indeterminate:  bMWE.Enabled = true;    break;
                case CheckState.Unchecked: {
                    if (File.Exists(Statics.fn_mwedll))
                        bMWE.Enabled = true;
                    else
                        if (bMWE.Enabled)
                            cbDisableMWE.CheckState = CheckState.Checked;
                        else {
                            cbDisableMWE.CheckState = CheckState.Indeterminate;
                            MessageBox.Show("Copy the latest version of 'Morrowind Enhanced.dll' to 'mge3' folder to use MWE.", "MWE requires Morrowind Enhanced.dll", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                    break;
                }
            }
        }

        private void cbDisablePatch_CheckStateChanged(object sender, EventArgs e) {
            bPatch.Enabled = cbDisablePatch.CheckState != CheckState.Checked;
        }

        private void bMWE_Click(object sender, EventArgs e) {
            (new MWEForm()).ShowDialog();
        }

        public PatchEditorForm PatchEditor;
        private void bPatchEd_Click(object sender, EventArgs e) {
            String currentDirectory = Environment.CurrentDirectory;
            PatchEditor = new PatchEditorForm();
            PatchEditor.ShowDialog();
            PatchEditor = null;
            Environment.CurrentDirectory = currentDirectory;
        }

        private void bPatch_Click(object sender, EventArgs e) {
            int start = cmbUILanguage.Text.IndexOf("(") + 1;
            int length = cmbUILanguage.Text.IndexOf(")") - start;
            String language = start > 0 && length > 0 ? cmbUILanguage.Text.Substring(start, length) : "English";
            language = cmbUILanguage.Text == "English (default)" ? "English" : language;
            (new PatchForm(cbUILangAuto.Checked ? LocalizationInterface.SysLangEng : language)).ShowDialog();
        }

    }

}