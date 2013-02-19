using System;
using System.Collections.Generic;
using System.Text;
using MGEgui.INI;

namespace MGEgui.Localization{

    public static class DefaultLocalization {

        public const String Language = "English (default)";

        public const String _Strings = ".Strings";
        #region Statics_Strings
        public static Dictionary<String, String> __Strings = new Dictionary<String, String> {
            {"Error", 
                "Error"},
            {"Warning",
                "Warning"},
            {"Message",
                "Message"},
            {"Close",
                "&Close"},
            {"MGEguiRunning",
                "MGEgui.exe is already running."},
            {"MWRunning",
                "Morrowind appears to be currently running.\n" +
                "Please quit the game before executing MGEgui.exe"},
            {"NotMWDir",
                "MGE must be installed to the Morrowind directory."},
            {"MGEMissing",
                "One or more of MGE's files appear to be missing. Please reinstall."},
            {"MWIncompat",
                "Your version of Morrowind is not compatible with MGE.\n" +
                "MGE requires a fully patched copy of Bloodmoon (i.e. Morrowind version 1.6.1820)."},
            {"MWCorrupt",
                "Morrowind.exe appears to be corrupt.\n" +
                "MGE is unable to determine Morrowind's version."},
            {"DSound",
                "You appear to have part of an old version of MGE installed.\n" +
                "You need to remove dsound.dll from your Morrowind directory for this version of MGE to function."},
            {"MWRegistry",
                "Unable to find Morrowind registry keys. Please run the game before installing MGE."},
            {"MgeAccess",
                "Unable to write to registry. Please run MGE as administrator."},
            {"Translation",
                ""}
        };
        #endregion

        public const String MainForm_Strings = "MainForm.Strings";
        #region MainForm_Strings
        public static Dictionary<String, String> _MainForm_Strings = new Dictionary<String, String> {
            {"About",
                "About"},
            {"ImportSet",
                "Import settings"},
            {"RegScrRes",
                "Unable to read screen resolution from registry."},
            {"RegWinMode",
                "Unable to read window mode from registry."},
            {"ImpOldSet",
                "Old graphics save file was found.\n\nDo you want to import settings?"},
            {"ErrImpSet",
                "Error importing old graphics save file.\n\n{0}"},
            {"SetTooOld",
                "The graphics save file is too old to be imported."},
            {"SetTooNew",
                "The graphics save file is newer than this version of MGE."},
            {"OldSetImp",
                "Old graphics save file was imported successfully.\n\n'settings' file was renamed to 'old-settings'."},
            {"ErrLdInp",
                "Error loading input save file.\n\n{0}"},
            {"InpTooOld",
                "The input save file is too old to import."},
            {"InpTooNew",
                "The input save file is newer than this version of MGE."},
            {"OldInpImp",
                "Old input save file was imported successfully."},
            {"MacrTrigErr",
                "An error occurred while loading macros or triggers. Some data may have been lost."},
            {"RegNotWrit",
                "Could not write Morrowind registry key.\n" +
                "MGEgui needs to be launched as Administrator."},
            {"MWIniNotInMGE",
                "Settings file does not contain ini data."},
            {"GraphCaps",
                "Graphics capabilities"},
            {"ViewCaps",
                "Max fullscreen antialiasing level: {0}x\n" +
                "Max windowed antialiasing level: {1}x\n" +
                "Max anisotropic filtering level: {2}x\n\n" +
                "Graphics drivers often report their capabilities incorrectly.\n" +
                "If you think your graphics card only supports lower levels of AA,\n" +
                "then you will need to reduce it by hand if you use best quality mode."},
            {"CreatedBy",
                "Created by"},
            {"FurtherDevel",
                "Further development"},
            {"DLVertexFog",
                "Distant land is not compatible with vertex fog when using some graphics cards.\n" +
                "If the distant land does not appear in game, or appears untextured, try switching back to pixel fog."},
            {"TooHighAA",
                "Your selected antialiasing level is not supported by your graphics card."},
            {"TooHighAF",
                "Your selected anisotropic filtering level is not supported by your graphics card."},
            {"DSMissing",
                "The distant statics files are missing.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them."},
            {"DLLackSM2",
                "Your graphics card lacks the Shader Model 2.0 support required to use Distant Land."},
            {"NoDLOrOld",
                "Distant Land files have not been created or are from an older version of MGE.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them."},
            {"DLDiffer",
                "Distant land files have not been created or are from a different version of MGE.\n" +
                "Use the 'Distant land file creation wizard' on the Tools tab to create them."},
            {"ResetAsk",
                "Reset Settings?"},
            {"AskReset",
                "Are you sure you want to reset all main {0}settings to default values?"},
            {"ResetSaved",
                "and other saved "},
            {"DLDelOldCorrupt",
                "Distant land files already exist, but are either incomplete, corrupt, or from an older version of MGE and must be deleted to continue.\n" +
                "Do you wish to continue?"},
            {"DLDelDifferent",
                "Distant land files already exist, but are from a different version of MGE and must be deleted to continue.\n" +
                "Do you wish to continue?"},
        };
        #endregion

        public const String MainForm_ToolTips = "MainForm.ToolTips";
        #region MainForm_ToolTips
        public static Dictionary<string, string[]> _MainForm_ToolTips = new Dictionary<string, string[]> {
        /* Main */
            {"DisableMGE", new string [] {"cbDisableMGE",
                "Disables hooking of DirectX input and 3D interface by Morrowind Graphics Extender.\n" +
                "Note that this doesn't disable MGE's internal version of MWSE, so you don't need to run its external version."}},
            {"ClearINI", new string [] {"cbClearINI",
                "Checking this will cause reinitialization of main settings file after resetting settings.\n" +
                "Note that all comments and deprecated settings added to that file will be lost."}},
            {"ResetSettings", new string [] {"bResetSettings",
                "Resets MGE settings back to defaults."}},
            {"Quality", new string [] {"bQuality",
                "Sets graphics options for the best visual quality."}},
            {"Help", new string [] {"bHelp",
                "Opens the help file."}},
            {"Import", new string [] {"bImport",
                "Import an MGE settings file.\n" +
                "To import Morrowind.ini as well, ensure that 'Import Morrowind.ini' is checked."}},
            {"Export", new string [] {"bExport",
                "Export your MGE settings and Morrowind.ini."}},
            {"ImportIni", new string [] {"cbImportIni",
                "If this is checked, Morrowind.ini will be imported along with MGE settings."}},
            {"ViewCaps", new string [] {"bViewCaps",
                "Displays your graphics card's maximum supported antialiasing and anisotropic filtering levels."}},
            {"About", new string [] {"bAbout",
                "Shows information about program."}},
            {"UILanguage", new string [] {"lUILanguage", "cmbUILanguage",
                "Sets translation language for user interface of MGEgui."}},
            {"UILangAuto", new string [] {"cbUILangAuto",
                "Checking this, enables autodetection of operating system's selected language,\n" +
                "for choosing automatically appropriate translation for user interface of MGEgui."}},
            {"TipReadSpd", new string [] {"lTipReadSpd", "cmbTipReadSpd", "lTipRSpdUnit",
                "This changes the fadeout time of MGEgui tooltips for selected reading speed (in characters per second).\n" +
                "For example, if you select 10 chars/sec. and some tooltip's text is 50 characters long (including spaces and new line characters),\n" +
                "that tooltip will be displayed for 5 seconds. Note that any tooltip can't be displayed for longer than about 33 seconds.\n" +
                "Timeout of tooltips for tab panel selection doesn't depend on its text length and isn't affected by this setting."}},
        /* Global graphics */
            {"RefreshRate", new string [] {"cmbRefreshRate", "lRefreshRate",
                "Sets the monitor refresh rate for fullscreen mode.\n" +
                "This has no effect in windowed mode."}},
            {"AntiAlias", new string [] {"cmbAntiAlias", "lAntiAlias",
                "Sets the antialiasing level.\n" +
                "If you set this above what your graphics card supports, you will get a render creation error when starting up Morrowind.\n" +
                "If you set this to anything other than none, you will not be able to create a lockable backbuffer."}},
            {"ZBuffer", new string [] {"cmbZBuffer", "lZBuffer",
                "Sets the z-buffer format.\n" +
                "If you set this to something that your graphics card doesn't support, you will get a render creation error when starting up Morrowind.\n" +
                "Using D16 or D16L will cause graphical artifacts while playing Morrowind."}},
            {"VWait", new string [] {"cmbVWait", "lVWait",
                "Sets the vertical refresh setting.\n" +
                "Setting this to 1, 2, 3, or 4 will cause an FPS hit, but will remove tearing artifacts.\n" +
                "In DirectX 9, which is only currently supported by MGE, 'Default (1)' is equivalent to '1'.\n" +
                "Setting it to more than 1 in windowed mode will result in using 'Default (1)' setting."}},
            {"CalcRefresh", new string [] {"bCalcRefresh",
                "Finds the valid refresh rates for Morrowind's currently selected resolution.\n" +
                "Valid refresh rates depend on resolution, adapter, and whether 16 bit colour mode is enabled.\n" +
                "Changes committed by MGEgui make an auto-search of valid refresh rates."}},
            {"Windowed", new string [] {"cbWindowed",
                "Check this to run Morrowind in a window instead of fullscreen."}},
            {"Resolution", new string [] {"tbResolution", "lResolution",
                "This is Morrowind's current screen resolution."}},
            {"CalcResolution", new string [] {"bCalcResolution",
                "Click this to change Morrowind's resolution.\n" +
                "In windowed mode you can use any resolution you like.\n" +
                "Non 4:3 aspect ratios may not work correctly. These are marked with an '*'."}},
            {"Aspect", new string [] {"tbAspect", "lAspect",
                "This is selected screen resolution calculated aspect ratio."}},
        /* Render state */
            {"ScaleFilter", new string [] {"cmbScaleFilter", "lScaleFilter",
                "Sets the min and mag texture filters.\n" +
                "If you set a filter not supported by your card, Morrowind will automatically use the next best available filter.\n" +
                "Do not set this to 'point' if you are using Morrowind's pixel-shaded water."}},
            {"MipFilter", new string [] {"cmbMipFilter", "lMipFilter",
                "Sets the mip texture filter.\n" +
                "If you set a filter not supported by your card, Morrowind will automatically use the next best available filter."}},
            {"AnisoLevel", new string [] {"cmbAnisoLevel", "lAnisoLevel",
                "Sets the anisotropic filtering level.\n" +
                "This will only have an effect if you use anisotropic filtering for either scale or mip filters.\n" +
                "If you set this to above what your card supports, it will automatically drop down to the highest supported level."}},
            {"LOD", new string [] {"udLOD", "lLOD",
                "Sets the mipmap level of detail bias. Valid range is between -12 and 12.\n" +
                "Negative values increase detail, positive values reduce it.\n" +
                "Very low values will cause aliasing of textures in the background."}},
            {"FogMode", new string [] {"cmbFogMode", "lFogMode",
                "Sets the type of fog that Morrowind uses.\n" +
                "Using vertex fog on some ATI cards may cause everything except the sky to become completely fogged."}},
            {"Fog", new string [] {"cbFog",
                "Uncheck this to disable fogging in Morrowind."}},
            {"FPSCounter", new string [] {"cbFPSCounter",
                "Enables MGE's FPS counter.\n" +
                "This is unrelated to Morrowind's FPS counter, and can be used instead of or as well as it.\n" +
                "Display messages must be checked for this to work."}},
            {"Zoom", new string [] {"udZoom", "lZoom",
                "Sets the initial level of zoom.\n" +
                "Changing this will not enable zoom. You must bind the 'toggle zoom' function to a key in the macro editor."}},
            {"Rotation", new string [] {"udRotation", "lRotation",
                "Sets the initial screen rotation clockwise from normal in degrees.\n" +
                "Rotation can be changed in 90 degree steps by using up/down buttons on the control."}},
            {"AAFix", new string [] {"cbAAFix",
                "Fixes the monochrome backgrounds you get when loading or saving when using antialiasing."}},
            {"DisplayMessages", new string [] {"cbDisplayMessages",
                "Allows MGE to draw text to the screen.\n" +
                "As well as the FPS counter, MGE will also alert you when various graphics options get changed."}},
            {"HWShader", new string [] {"cbHWShader",
                "Enables fullscreen pixel shaders.\n" +
                "Some shaders will make it difficult to view menus, so it may be better to use\n" +
                "the macro editor's 'toggle HW shader' function."}},
            {"DisableShadersInMenu", new string [] {"cbDisableShadersInMenu",
                "Disables fullscreen pixel shaders in menu.\n" +
                "Some shaders might decrease framerate in menu mode and cause input lag.\n" +
                "Checking this will disable shaders in loading screens and menus."}},
            {"SShotFormat", new string [] {"cmbSShotFormat", "lSShotFormat",
                "Sets the format in which MGE will save screenshots.\n" +
                "This does not change the format in which Morrowind saves screenshots."}},
            {"SShotName", new string [] {"lSShotName", "tbSShotName",
                "Every saved MGE screenshot will start its file name with this prefix."}},
            {"SShotDir", new string [] {"lSShotDir", "tbSShotDir",
                "This is output directory where taken screenshots will be saved."}},
            {"SShotNum", new string [] {"lSShotNum", "udSShotNum",
                "Minimum number of characters for screenshot number.\n" +
                "Setting it to more than 1 will result in leading zeros if screenshot number use less characters than set here,\n" +
                "e.g.: if it's set to 3 and first screenshot is saved to output directory, the resulting screenshot number is '001'."}},
            {"SShotDirBrowse", new string [] {"bSShotDirBrowse",
                "Use it for selecting output directory where you want the screenshots to be saved."}},
            {"SShotDirClear", new string [] {"bSShotDirClear",
                "Changes selected output directory for screenshots to default.\n" +
                "Screenshots will be saved to Morrowind installation directory."}},
            {"FOV", new string [] {"udFOV", "lFOV",
                "Adjusts the in-game horizontal field of view (FOV)."}},
            /*{"MsgsLanguage", new string [] {"lMsgsLanguage", "cmbMsgsLanguage",
                "NEEDS A DESCRIPTION."}},/**/
            {"MsgsTime", new string [] {"lMsgsTime", "udMsgsTime", "lMsgsTimeUnit",
                "Changes the maximum length of time for which MGE status messages will be displayed on screen in the game.\n" +
                "Units used for this setting are miliseconds, so to make MGE messages displayed for 5 seconds, you need to set it to 5000."}},
        /* Input */
            {"SkipMovie", new string [] {"cbSkipMovie",
                "Skips the two opening movies."}},
            {"Console", new string [] {"cbConsole",
                "Disables the console."}},
            {"AltCombat", new string [] {"cbAltCombat",
                "Allows you to use Daggerfall style combat controls.\n" +
                "You must bind the 'toggle alt combat' function to a key in the macro editor to use this."}},
            {"LagFix", new string [] {"cbLagFix",
                "Attempts to fix input lag.\n" +
                "This will cause a small graphical artifact in the top left corner of the screen, and may permanently mark your minimap.\n" +
                "Turning on MGE's FPS counter is usually sufficient to remove input lag caused by AA or shadows."}},
            {"MacroEd", new string [] {"bMacroEd",
                "Opens up the macro editor, allowing you to bind functions and macros to keyboard keys and mouse buttons."}},
            {"Remapper", new string [] {"bRemapper",
                "Opens up the keyboard remapper, with which you can remap any key on your keyboard."}},
        /* Misc */
            {"ZoomEffectsHUD", new string [] {"cbZoomEffectsHUD",
                "Check this if you want zoom and screen rotation to affect the HUD."}},
            {"Idle", new string [] {"cbIdle",
                "Check this to force Morrowind to give up control of the cpu once a frame."}},
            {"IdleTime", new string [] {"udIdle", "lIdle",
                "Controls how long Morrowind gives up the cpu. Valid range is 0 to 1000.\n" +
                "Value of 0 causes Morrowind to resume immediately if no other tasks are waiting."}},
            {"MaxView", new string [] {"udMaxView", "lMaxView",
                "Changes the maximum view distance that MGE will set.\n" +
                "This setting only affects the 'keep FPS constant' setting.\n" +
                "Morrowind default is 7168."}},
            {"MinView", new string [] {"udMinView", "lMinView",
                "Changes the minimum view distance that MGE will set.\n" +
                "This setting only affects the 'keep FPS constant' setting.\n" +
                "Morrowind default is 2500."}},
            {"SetEffectVars", new string [] {"cbSetEffectVars",
                "If this box is unchecked, the sinvar, linvar, tickcount, eyevec, sunvec and suncol variables will be unavailable to shaders.\n" +
                "Unchecking this will give a performance boost, especially if you run many simultaneous shaders."}},
            {"FPSConst", new string [] {"cbFPSConst",
                "Check this to allow MGE to modify view distance in order to keep a constant FPS.\n" +
                "MGE will only modify view distance. No other graphics options will be changed."}},
            {"FPSDes", new string [] {"udFPSDes", "lFPSDes",
                "The FPS MGE should try to obtain."}},
            {"FPSTol", new string [] {"udFPSTol", "lFPSTol",
                "The amount that your FPS needs to be above or below your desired amount before MGE modifies framerate."}},
            {"BindAI", new string [] {"cbBindAI",
                "Checking this box will bind the AI distance to your view distance.\n" +
                "Increasing view distance with MGE will automatically increase AI distance."}},
            {"HDR", new string [] {"cbHDR",
                "Check this box to supply HDR information to shaders.\n" +
                "Note that checking this box by itself will not enable HDR.\n" +
                "You also need to add a HDR shader to the active shader chain.\n" +
                "Checking this box may cause an FPS hit."}},
            {"HDRTime", new string [] {"udHDR", "lHDR",
                "This box controls how fast the HDR reacts to brightness changes.\n" +
                "The lower time is set, the reaction is faster."}},
            {"Hook", new string [] {"cbHook",
                "Enables MGE texture hooking. Required for mods which attach effects to existing Morrowind objects.\n" +
                "Checking this option may increase loading times."}},
            {"AspectZoom", new string [] {"cbAspectZoom",
                "Checking this will enable corrections of aspect ratio while zooming."}},
            {"DisableMWSE", new string [] {"cbDisableMWSE",
                "Checking this option disables MGE's internal version of MWSE functions.\n" +
                "This allows you to run MGE with an external version of MWSE.\n" +
                "Note that some MGE mods may require MGE's internal version of MWSE,\n" +
                "and will not work with external versions."}},
            {"DisableMWE", new string [] {"cbDisableMWE",
                "Checking this option disables MGE's internal version of MWE."}},
            {"ShaderDepth", new string [] {"cbShaderDepth",
                "Checking this will supply hardware shaders with screen depth information.\n" +
                "Checking this box may cause an FPS hit."}},
            {"Cam3rdPrsn", new string [] {"cbCam3rdPrsn",
                "Check this to customize parameters of 3rd person player camera.\n" +
                "Parameters set by this option will override the game defaults on start of Morrowind.\n" +
                "This option doesn't need to be set, to change the position of camera, by either a macro\n" +
                "function, or by a script. Scripts can switch off overriding of camera parameters."}},
            {"Cam3rdX", new string [] {"lCam3rdX", "udCam3rdX",
                "This sets the horizontal offset of 3rd person camera from player character's eyes.\n" +
                "Negative values move the camera to the left, and positive to the right."}},
            {"Cam3rdY", new string [] {"lCam3rdY", "udCam3rdY",
                "This sets the offset of 3rd person camera from player character's eyes along the direction of sight.\n" +
                "More negative values move the camera farther back, and less negative values move it forth, closer to player character.\n" +
                "Only negative values are used, because this setting, as also corresponding macro functions, allow only placing the 3rd\n" +
                "person camera behind the character. To set the camera ahead of character's eyes, you need to create a script doing it."}},
            {"Cam3rdZ", new string [] {"lCam3rdZ", "udCam3rdZ",
                "This sets the vertical offset of 3rd person camera from player character's eyes.\n" +
                "Negative values move the camera down, and positive values move the camera up."}},
        /* Morrowind.ini */
            {"Screenshots", new string [] {"cbScreenshots",
                "The 'Screen Shot Enable' line.\n" +
                "Check this to enable Morrowind's inbuilt screenshot function."}},
            {"YesToAll", new string [] {"cbYesToAll",
                "The 'AllowYesToAll' line.\n" +
                "Check this to add an additional 'yes to all' option when clicking through multiple warning messages."}},
            {"HQShadows", new string [] {"cbHQShadows",
                "The 'High Detail Shadows' line.\n" +
                "Check this to enable much better quality shadows.\n" +
                "This will cause a huge FPS hit."}},
            {"ThreadLoad", new string [] {"cbThreadLoad",
                "The 'DontThreadLoad' line.\n" +
                "Check this to allow Morrowind to load new cells in a separate thread."}},
            {"ShowFPS", new string [] {"cbShowFPS",
                "The 'Show FPS' line.\n" +
                "Check this to enable Morrowind's inbuilt FPS counter."}},
            {"Audio", new string [] {"cbAudio",
                "The 'Disable Audio' line.\n" +
                "Check this to disable most of Morrowind's audio. Music is unaffected by this setting."}},
            {"Subtitles", new string [] {"cbSubtitles",
                "The 'Subtitles' line.\n" +
                "Check this to display subtitles where normally there would just be a voice over."}},
            {"HitFader", new string [] {"cbHitFader",
                "The 'ShowHitFader' line.\n" +
                "Uncheck this to disable the red hit fader that appears when you take damage."}},
            {"MultipleCS", new string [] {"cbMultipleCS",
                "The 'AllowMultipleEditors' line.\n" +
                "Check this to allow multiple copies of the construction set to run simultaneously."}},
        /* Tools */
            {"ShaderEd", new string [] {"bShaderEd",
                "A shader editor for working on fullscreen shaders."}},
            {"HookGen", new string [] {"bHookGen",
                "Generates fake texture files which will be intercepted by MGE when Morrowind attempts to load them."}},
            {"DistantLandWizard", new string [] {"bDistantLandWizard",
                "This will launch the process that creates the files that distant land needs to work.\n" +
                "You cannot enable distant land without running it."}},
        /* Distant Land */
            {"DLDistantLand", new string [] {"cbDLDistantLand",
                "Check this box to enable the distant land feature.\n" +
                "This renders landscape and objects beyond Morrowind's normal drawing distance.\n" +
                "Note that Morrowind's pixel/vertex shader water will be disabled if you enable this,\n" +
                "and instead of it Distant Land's water shader will be used.\n" +
                "Morrowind's pixel shader will be re-enabled by MGEgui if you disable MGE or Distant Land."}},
            {"DLDrawDist", new string [] {"udDLDrawDist", "lDLDrawDist",
                "This is the maximum distance in cells that distant land will draw anything.\n" +
                "Anything beyond this distance will be cut off."}},
            {"DLAutoDist", new string [] {"cbDLAutoDist",
                "This will automatically set other than selected cutoff distances, depending on the selected base.\n" +
                "This way you don't have to think about things like fog, distant land draw distance or static distances, unless you really want to."}},
            {"DLAutoByDrawDist", new string [] {"rbDLAutoByDrawDist",
                "This will automatically set other cutoff distances based on your Draw Distance setting."}},
            {"DLAutoByAFogEnd", new string [] {"rbDLAutoByAFogEnd",
                "This will automatically set other cutoff distances based on your Above Water Fog End setting."}},
            /*{"DLBlur", new string [] {"udDLBlur", "lDLBlur",
                "This controls how many blur passes will be applied to far away objects.\n" +
                "Set higher for more blurring and set to zero to disable.\n" +
                "Note that this will disable AA in the areas rendered by distant land."}},/* REMOVED */
            {"DLDistantStatics", new string [] {"cbDLDistantStatics",
                "This enables the drawing of far off objects such as trees and buildings."}},
            {"DLNearSize", new string [] {"tbDLNearSize",
                "This reminds you what minimum size you selected while generating distant statics.\n" +
                "To change it you will need to generate statics again."}},
            {"DLDistNear", new string [] {"udDLDistNear",
                "This is the distance in cells where you will stop seeing 'small size' statics."}},
            {"DLSizeFar", new string [] {"udDLSizeFar",
                "This is the minimum size in Morrowind units of an object that will be considered 'medium sized'."}},
            {"DLDistFar", new string [] {"udDLDistFar",
                "This is the distance in cells where you will stop seeing 'medium size' statics."}},
            {"DLSizeVeryFar", new string [] {"udDLSizeVeryFar",
                "This is the minimum size in Morrowind units of an object that will be considered 'large sized'."}},
            {"DLDistVeryFar", new string [] {"udDLDistVeryFar",
                "This is the distance in cells where you will stop seeing 'large size' statics."}},
            {"DLFogAStart", new string [] {"udDLFogAStart",
                "This sets the distance in cells where fog will begin to affect objects above water in clear weather."}},
            {"DLFogAEnd", new string [] {"udDLFogAEnd",
                "This sets the distance in cells where objects will be completely hidden by fog above water in clear weather."}},
            {"DLFogBStart", new string [] {"udDLFogBStart",
                "This sets the distance in cells where fog will begin to affect objects below water."}},
            {"DLFogBEnd", new string [] {"udDLFogBEnd",
                "This sets the distance in cells where objects will be completely hidden by fog below water."}},
            {"DLFogIStart", new string [] {"udDLFogIStart",
                "This sets the distance in cells where fog will begin to affect objects in interiors with generated Distant Statics."}},
            {"DLFogIEnd", new string [] {"udDLFogIEnd",
                "This sets the distance in cells where objects will be completely hidden by fog in interiors with generated Distant Statics."}},
            {"DLReflLand", new string [] {"cbDLReflLand",
                "This determines whether the landscape drawn by distant land will be reflected in the water."}},
            {"DLReflNStatics", new string [] {"cbDLReflNStatics",
                "Enabling this will make nearby objects reflect in the water.\n" +
                "Note that this is expensive and may lower your frame rate."}},
            {"DLReflFStatics", new string [] {"cbDLReflFStatics",
                "Enabling this will make objects that are a bit further away reflect in the water.\n" +
                "Note that this is expensive and may lower your frame rate."}},
            {"DLWthr", new string [] {"bDLWthr",
                "Distant Land settings for all Morrowind weather types.\n" +
                "You can set there fog and wind ratios for each weather."}},
            {"DLOptions", new string [] {"bDLOptions",
                "Allows additional Distant Land options to be customized."}},
            {"DLSkyRefl", new string [] {"cbDLSkyRefl",
                "This determines whether the sky will be reflected in the water."}},
            {"DLDynRipples", new string [] {"cbDLDynRipples",
                "When checking this, rain drops and the player will create ripples on the water surface."}},
            {"DLSM20", new string [] {"rbDLSM20",
                "This sets Shader Model 2.0 as a default shader for Distant Land.\n" +
                "Note that this setting doesn't force the use of selected Shader Model for Distant Land.\n" +
                "It only suggests the maximum shader model, which should be used for Distant Land, Distant Statics, or Water.\n" +
                "Support for Shader Model 2.0 may be removed, if any part of Distant Land requires use of higher shaders in the future."}},
            {"DLSM30", new string [] {"rbDLSM30",
                "This sets Shader Model 3.0 as a default shader for Distant Land.\n" +
                "Note that this setting doesn't force the use of selected Shader Model for Distant Land.\n" +
                "It only suggests the maximum shader model, which should be used for Distant Land, Distant Statics or Water."}},
            {"DLReflBlur", new string [] {"cbDLReflBlur",
                "This determines whether reflections in the water should be blurred.\n" +
                "Note that setting this option might cause an FPS hit in game."}},
            {"DLFogExp", new string [] {"cbDLFogExp",
                "This enables an alternative exponential fog formula for Distant Land.\n" +
                "When using exponential fog, minimum value of Distant Land draw distance will be\n" +
                "Exponential Distance Multiplier times more than above water fog end distance.\n" +
                "Note that setting this option might cause an FPS hit in game."}},
            {"DLFogExpMul", new string [] {"lDLFogExpMul", "udDLFogExpMul",
                "This adjusts the exponential fog distance multiplier, which is used for multiplying draw distance,\n" +
                "to make distant land don't pop in and out too early, when exponential fog is used.\n" +
                "Note that high values will cause high FPS hit, and low values may cause distant land cutoff before fog end.\n" +
                "The most safe and the default setting, that will ensure exponential fog end and distant land drawing end\n" +
                "in about the same place, is 4.0."}},
            {"DLWtrCaust", new string [] {"udDLWtrCaust", "lDLWtrCaust",
                "This sets the percentage of water caustic lighting intensity.\n" +
                "Setting it to 0 will disable caustics. The default value is 50.\n" +
                "Note that this feature isn't available when using Shader Model 2.0 Distant Land."}},
            {"DLWtrWave", new string [] {"udDLWtrWave", "lDLWtrWave",
                "This sets the height of water waves.\n" +
                "Graphics card with vertex texture fetch support is required for this feature.\n" +
                "Setting it to 0 will disable waves. Suggested value for wave height is 30-50.\n" +
                "Note that this feature isn't available when using Shader Model 2.0 Distant Land."}},
        };
        #endregion
    }
}
