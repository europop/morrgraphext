using System.Windows.Forms;
using System;
using System.ComponentModel;

namespace MGEgui {
    partial class MainForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components=null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing&&(components!=null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            this.DudMenu = new System.Windows.Forms.ContextMenu();
            this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.OpenTangentMesh = new System.Windows.Forms.OpenFileDialog();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.cbDisableShadersInMenu = new System.Windows.Forms.CheckBox();
            this.cbDLSkyRefl = new System.Windows.Forms.CheckBox();
            this.tpTools = new System.Windows.Forms.TabPage();
            this.bPatchEd = new System.Windows.Forms.Button();
            this.bDistantLandWizard = new System.Windows.Forms.Button();
            this.bHookGen = new System.Windows.Forms.Button();
            this.bShaderEd = new System.Windows.Forms.Button();
            this.tpINI = new System.Windows.Forms.TabPage();
            this.gbMWGeneral = new System.Windows.Forms.GroupBox();
            this.cbScreenshots = new System.Windows.Forms.CheckBox();
            this.cbHitFader = new System.Windows.Forms.CheckBox();
            this.cbMultipleCS = new System.Windows.Forms.CheckBox();
            this.cbAudio = new System.Windows.Forms.CheckBox();
            this.cbHQShadows = new System.Windows.Forms.CheckBox();
            this.cbShowFPS = new System.Windows.Forms.CheckBox();
            this.cbThreadLoad = new System.Windows.Forms.CheckBox();
            this.cbYesToAll = new System.Windows.Forms.CheckBox();
            this.cbSubtitles = new System.Windows.Forms.CheckBox();
            this.lScreenshotStatus = new System.Windows.Forms.Label();
            this.tpMisc = new System.Windows.Forms.TabPage();
            this.cbDisableIntegration = new System.Windows.Forms.CheckBox();
            this.bPatch = new System.Windows.Forms.Button();
            this.cbDisablePatch = new System.Windows.Forms.CheckBox();
            this.bMWE = new System.Windows.Forms.Button();
            this.pCam3rdPrsn = new System.Windows.Forms.Panel();
            this.cbCam3rdPrsn = new System.Windows.Forms.CheckBox();
            this.gbCam3rdPrsn = new System.Windows.Forms.GroupBox();
            this.udCam3rdZ = new System.Windows.Forms.NumericUpDown();
            this.udCam3rdY = new System.Windows.Forms.NumericUpDown();
            this.udCam3rdX = new System.Windows.Forms.NumericUpDown();
            this.lCam3rdX = new System.Windows.Forms.Label();
            this.lCam3rdY = new System.Windows.Forms.Label();
            this.lCam3rdZ = new System.Windows.Forms.Label();
            this.pIdle = new System.Windows.Forms.Panel();
            this.cbIdle = new System.Windows.Forms.CheckBox();
            this.gbIdle = new System.Windows.Forms.GroupBox();
            this.udIdle = new System.Windows.Forms.NumericUpDown();
            this.lIdle = new System.Windows.Forms.Label();
            this.pFPSConst = new System.Windows.Forms.Panel();
            this.cbFPSConst = new System.Windows.Forms.CheckBox();
            this.gbFPSConst = new System.Windows.Forms.GroupBox();
            this.udFPSDes = new System.Windows.Forms.NumericUpDown();
            this.udFPSTol = new System.Windows.Forms.NumericUpDown();
            this.udMinView = new System.Windows.Forms.NumericUpDown();
            this.lFPSDes = new System.Windows.Forms.Label();
            this.udMaxView = new System.Windows.Forms.NumericUpDown();
            this.lMinView = new System.Windows.Forms.Label();
            this.lFPSTol = new System.Windows.Forms.Label();
            this.lMaxView = new System.Windows.Forms.Label();
            this.gbShaders = new System.Windows.Forms.GroupBox();
            this.cbHDR = new System.Windows.Forms.CheckBox();
            this.cbShaderDepth = new System.Windows.Forms.CheckBox();
            this.cbHook = new System.Windows.Forms.CheckBox();
            this.cbSetEffectVars = new System.Windows.Forms.CheckBox();
            this.gbHDR = new System.Windows.Forms.GroupBox();
            this.udHDR = new System.Windows.Forms.NumericUpDown();
            this.lHDR = new System.Windows.Forms.Label();
            this.cbDisableMWE = new System.Windows.Forms.CheckBox();
            this.cbDisableMWSE = new System.Windows.Forms.CheckBox();
            this.cbAspectZoom = new System.Windows.Forms.CheckBox();
            this.cbZoomEffectsHUD = new System.Windows.Forms.CheckBox();
            this.cbBindAI = new System.Windows.Forms.CheckBox();
            this.tpInput = new System.Windows.Forms.TabPage();
            this.gbInputTools = new System.Windows.Forms.GroupBox();
            this.bMacroEd = new System.Windows.Forms.Button();
            this.bRemapper = new System.Windows.Forms.Button();
            this.gbInputOpts = new System.Windows.Forms.GroupBox();
            this.cbSkipMovie = new System.Windows.Forms.CheckBox();
            this.cbConsole = new System.Windows.Forms.CheckBox();
            this.cbLagFix = new System.Windows.Forms.CheckBox();
            this.cbAltCombat = new System.Windows.Forms.CheckBox();
            this.tpRender = new System.Windows.Forms.TabPage();
            this.gbMsgs = new System.Windows.Forms.GroupBox();
            this.udMsgsTime = new System.Windows.Forms.NumericUpDown();
            this.cmbMsgsLocale = new System.Windows.Forms.ComboBox();
            this.lMsgsTime = new System.Windows.Forms.Label();
            this.lMsgsLanguage = new System.Windows.Forms.Label();
            this.lMsgsTimeUnit = new System.Windows.Forms.Label();
            this.gbFog = new System.Windows.Forms.GroupBox();
            this.cmbFogMode = new System.Windows.Forms.ComboBox();
            this.lFogMode = new System.Windows.Forms.Label();
            this.gbRendOpts = new System.Windows.Forms.GroupBox();
            this.cbFog = new System.Windows.Forms.CheckBox();
            this.cbDisplayMessages = new System.Windows.Forms.CheckBox();
            this.cbFPSCounter = new System.Windows.Forms.CheckBox();
            this.cbAAFix = new System.Windows.Forms.CheckBox();
            this.cbHWShader = new System.Windows.Forms.CheckBox();
            this.gbScreen = new System.Windows.Forms.GroupBox();
            this.udFOV = new System.Windows.Forms.NumericUpDown();
            this.udRotation = new System.Windows.Forms.NumericUpDown();
            this.udZoom = new System.Windows.Forms.NumericUpDown();
            this.lRotation = new System.Windows.Forms.Label();
            this.lZoom = new System.Windows.Forms.Label();
            this.lFOV = new System.Windows.Forms.Label();
            this.gbTexFilt = new System.Windows.Forms.GroupBox();
            this.cmbScaleFilter = new System.Windows.Forms.ComboBox();
            this.lScaleFilter = new System.Windows.Forms.Label();
            this.cmbAnisoLevel = new System.Windows.Forms.ComboBox();
            this.lLOD = new System.Windows.Forms.Label();
            this.udLOD = new System.Windows.Forms.NumericUpDown();
            this.lAnisoLevel = new System.Windows.Forms.Label();
            this.cmbMipFilter = new System.Windows.Forms.ComboBox();
            this.lMipFilter = new System.Windows.Forms.Label();
            this.lSShotFormat = new System.Windows.Forms.Label();
            this.cmbSShotFormat = new System.Windows.Forms.ComboBox();
            this.tpGlobal = new System.Windows.Forms.TabPage();
            this.gbSShot = new System.Windows.Forms.GroupBox();
            this.udSShotNum = new System.Windows.Forms.NumericUpDown();
            this.bSShotDirClear = new System.Windows.Forms.Button();
            this.bSShotDirBrowse = new System.Windows.Forms.Button();
            this.tbSShotName = new System.Windows.Forms.TextBox();
            this.tbSShotDir = new System.Windows.Forms.TextBox();
            this.lSShotName = new System.Windows.Forms.Label();
            this.lSShotDir = new System.Windows.Forms.Label();
            this.lSShotNum = new System.Windows.Forms.Label();
            this.gbScene = new System.Windows.Forms.GroupBox();
            this.cmbAntiAlias = new System.Windows.Forms.ComboBox();
            this.cmbVWait = new System.Windows.Forms.ComboBox();
            this.cmbZBuffer = new System.Windows.Forms.ComboBox();
            this.lZBuffer = new System.Windows.Forms.Label();
            this.lVWait = new System.Windows.Forms.Label();
            this.lAntiAlias = new System.Windows.Forms.Label();
            this.gbDisplay = new System.Windows.Forms.GroupBox();
            this.tbAspect = new System.Windows.Forms.TextBox();
            this.bCalcResolution = new System.Windows.Forms.Button();
            this.cbWindowed = new System.Windows.Forms.CheckBox();
            this.cmbRefreshRate = new System.Windows.Forms.ComboBox();
            this.bCalcRefresh = new System.Windows.Forms.Button();
            this.tbResolution = new System.Windows.Forms.TextBox();
            this.lResolution = new System.Windows.Forms.Label();
            this.lAspect = new System.Windows.Forms.Label();
            this.lRefreshRate = new System.Windows.Forms.Label();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tpMain = new System.Windows.Forms.TabPage();
            this.gbMainUI = new System.Windows.Forms.GroupBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.cmbUILanguage = new System.Windows.Forms.ComboBox();
            this.cbUILangAuto = new System.Windows.Forms.CheckBox();
            this.lUILanguage = new System.Windows.Forms.Label();
            this.cmbTipReadSpd = new System.Windows.Forms.ComboBox();
            this.lTipReadSpd = new System.Windows.Forms.Label();
            this.lTipRSpdUnit = new System.Windows.Forms.Label();
            this.cbDisableMGE = new System.Windows.Forms.CheckBox();
            this.gbMainInfo = new System.Windows.Forms.GroupBox();
            this.bViewCaps = new System.Windows.Forms.Button();
            this.bHelp = new System.Windows.Forms.Button();
            this.bAbout = new System.Windows.Forms.Button();
            this.gbMainSettings = new System.Windows.Forms.GroupBox();
            this.bResetSettings = new System.Windows.Forms.Button();
            this.bImport = new System.Windows.Forms.Button();
            this.bExport = new System.Windows.Forms.Button();
            this.cbImportIni = new System.Windows.Forms.CheckBox();
            this.cbClearINI = new System.Windows.Forms.CheckBox();
            this.gbMainQuality = new System.Windows.Forms.GroupBox();
            this.bQuality = new System.Windows.Forms.Button();
            this.tpDL = new System.Windows.Forms.TabPage();
            this.cbDLDistantLand = new System.Windows.Forms.CheckBox();
            this.gbDistantLand = new System.Windows.Forms.GroupBox();
            this.pDLStatics = new System.Windows.Forms.Panel();
            this.cbDLDistantStatics = new System.Windows.Forms.CheckBox();
            this.gbDLStatics = new System.Windows.Forms.GroupBox();
            this.cbDLDistantStaticsScript = new System.Windows.Forms.CheckBox();
            this.udDLDistVeryFar = new System.Windows.Forms.NumericUpDown();
            this.udDLDistFar = new System.Windows.Forms.NumericUpDown();
            this.udDLDistNear = new System.Windows.Forms.NumericUpDown();
            this.udDLSizeVeryFar = new System.Windows.Forms.NumericUpDown();
            this.udDLSizeFar = new System.Windows.Forms.NumericUpDown();
            this.tbDLNearSize = new System.Windows.Forms.TextBox();
            this.lDLVeryFar = new System.Windows.Forms.Label();
            this.lDLFar = new System.Windows.Forms.Label();
            this.lDLNear = new System.Windows.Forms.Label();
            this.lDLMinSize = new System.Windows.Forms.Label();
            this.lDLEndDist = new System.Windows.Forms.Label();
            this.cbDLAutoDist = new System.Windows.Forms.CheckBox();
            this.gbDLAutoDist = new System.Windows.Forms.GroupBox();
            this.rbDLAutoByAFogEnd = new System.Windows.Forms.RadioButton();
            this.rbDLAutoByDrawDist = new System.Windows.Forms.RadioButton();
            this.gbDLSM = new System.Windows.Forms.GroupBox();
            this.rbDLSM20 = new System.Windows.Forms.RadioButton();
            this.rbDLSM30 = new System.Windows.Forms.RadioButton();
            this.gbDLWater = new System.Windows.Forms.GroupBox();
            this.bDLDynRipples = new System.Windows.Forms.Button();
            this.gbDLWtrCaust = new System.Windows.Forms.GroupBox();
            this.lDLWtrCaust = new System.Windows.Forms.Label();
            this.udDLWtrCaust = new System.Windows.Forms.NumericUpDown();
            this.gbDLWtrWave = new System.Windows.Forms.GroupBox();
            this.lDLWtrWave = new System.Windows.Forms.Label();
            this.udDLWtrWave = new System.Windows.Forms.NumericUpDown();
            this.gbDLWtrRefl = new System.Windows.Forms.GroupBox();
            this.cbDLReflLand = new System.Windows.Forms.CheckBox();
            this.cbDLReflBlur = new System.Windows.Forms.CheckBox();
            this.cbDLReflNStatics = new System.Windows.Forms.CheckBox();
            this.cbDLReflFStatics = new System.Windows.Forms.CheckBox();
            this.lDLDynRipples = new System.Windows.Forms.Label();
            this.cbDLDynRipples = new System.Windows.Forms.CheckBox();
            this.bDLWthr = new System.Windows.Forms.Button();
            this.gbDLDrawDist = new System.Windows.Forms.GroupBox();
            this.lDLDrawDist = new System.Windows.Forms.Label();
            this.udDLDrawDist = new System.Windows.Forms.NumericUpDown();
            this.bDLOptions = new System.Windows.Forms.Button();
            this.gbDLFog = new System.Windows.Forms.GroupBox();
            this.lDLFogExpMul = new System.Windows.Forms.Label();
            this.udDLFogExpMul = new System.Windows.Forms.NumericUpDown();
            this.lDLFogI = new System.Windows.Forms.Label();
            this.udDLFogIEnd = new System.Windows.Forms.NumericUpDown();
            this.udDLFogIStart = new System.Windows.Forms.NumericUpDown();
            this.udDLFogBEnd = new System.Windows.Forms.NumericUpDown();
            this.cbDLFogExp = new System.Windows.Forms.CheckBox();
            this.udDLFogBStart = new System.Windows.Forms.NumericUpDown();
            this.udDLFogAEnd = new System.Windows.Forms.NumericUpDown();
            this.udDLFogAStart = new System.Windows.Forms.NumericUpDown();
            this.lDLFogB = new System.Windows.Forms.Label();
            this.lDLFogA = new System.Windows.Forms.Label();
            this.lDLFogStart = new System.Windows.Forms.Label();
            this.lDLFogEnd = new System.Windows.Forms.Label();
            this.SShotFolderDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.tpTools.SuspendLayout();
            this.tpINI.SuspendLayout();
            this.gbMWGeneral.SuspendLayout();
            this.tpMisc.SuspendLayout();
            this.pCam3rdPrsn.SuspendLayout();
            this.gbCam3rdPrsn.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdX)).BeginInit();
            this.pIdle.SuspendLayout();
            this.gbIdle.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udIdle)).BeginInit();
            this.pFPSConst.SuspendLayout();
            this.gbFPSConst.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udFPSDes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFPSTol)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxView)).BeginInit();
            this.gbShaders.SuspendLayout();
            this.gbHDR.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udHDR)).BeginInit();
            this.tpInput.SuspendLayout();
            this.gbInputTools.SuspendLayout();
            this.gbInputOpts.SuspendLayout();
            this.tpRender.SuspendLayout();
            this.gbMsgs.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udMsgsTime)).BeginInit();
            this.gbFog.SuspendLayout();
            this.gbRendOpts.SuspendLayout();
            this.gbScreen.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udFOV)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udRotation)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udZoom)).BeginInit();
            this.gbTexFilt.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udLOD)).BeginInit();
            this.tpGlobal.SuspendLayout();
            this.gbSShot.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udSShotNum)).BeginInit();
            this.gbScene.SuspendLayout();
            this.gbDisplay.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tpMain.SuspendLayout();
            this.gbMainUI.SuspendLayout();
            this.panel1.SuspendLayout();
            this.gbMainInfo.SuspendLayout();
            this.gbMainSettings.SuspendLayout();
            this.gbMainQuality.SuspendLayout();
            this.tpDL.SuspendLayout();
            this.gbDistantLand.SuspendLayout();
            this.pDLStatics.SuspendLayout();
            this.gbDLStatics.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistVeryFar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistFar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistNear)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLSizeVeryFar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLSizeFar)).BeginInit();
            this.gbDLAutoDist.SuspendLayout();
            this.gbDLSM.SuspendLayout();
            this.gbDLWater.SuspendLayout();
            this.gbDLWtrCaust.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLWtrCaust)).BeginInit();
            this.gbDLWtrWave.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLWtrWave)).BeginInit();
            this.gbDLWtrRefl.SuspendLayout();
            this.gbDLDrawDist.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDrawDist)).BeginInit();
            this.gbDLFog.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogExpMul)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogIEnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogIStart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogBEnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogBStart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogAEnd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogAStart)).BeginInit();
            this.SuspendLayout();
            // 
            // OpenFileDialog
            // 
            this.OpenFileDialog.DefaultExt = "mge";
            this.OpenFileDialog.Filter = "MGE exported settings (*.MGE)|*.MGE";
            this.OpenFileDialog.RestoreDirectory = true;
            this.OpenFileDialog.Title = "Select settings file to import";
            // 
            // SaveFileDialog
            // 
            this.SaveFileDialog.DefaultExt = "mge";
            this.SaveFileDialog.Filter = "MGE exported settings (*.MGE)|*.MGE";
            this.SaveFileDialog.RestoreDirectory = true;
            this.SaveFileDialog.Title = "Export settings";
            // 
            // OpenTangentMesh
            // 
            this.OpenTangentMesh.DefaultExt = "x";
            this.OpenTangentMesh.Filter = "Direct X meshes (*.x)|*.x";
            this.OpenTangentMesh.RestoreDirectory = true;
            this.OpenTangentMesh.Title = "Select mesh to generate tangent and binormal information for";
            // 
            // toolTip
            // 
            this.toolTip.AutomaticDelay = 3000;
            this.toolTip.AutoPopDelay = 30000;
            this.toolTip.InitialDelay = 500;
            this.toolTip.ReshowDelay = 100;
            // 
            // cbDisableShadersInMenu
            // 
            this.cbDisableShadersInMenu.AutoSize = true;
            this.cbDisableShadersInMenu.Location = new System.Drawing.Point(10, 124);
            this.cbDisableShadersInMenu.Name = "cbDisableShadersInMenu";
            this.cbDisableShadersInMenu.Size = new System.Drawing.Size(141, 17);
            this.cbDisableShadersInMenu.TabIndex = 4;
            this.cbDisableShadersInMenu.Text = "Disable shaders in menu";
            this.cbDisableShadersInMenu.UseVisualStyleBackColor = true;
            // 
            // cbDLSkyRefl
            // 
            this.cbDLSkyRefl.AutoSize = true;
            this.cbDLSkyRefl.Location = new System.Drawing.Point(11, 19);
            this.cbDLSkyRefl.Name = "cbDLSkyRefl";
            this.cbDLSkyRefl.Size = new System.Drawing.Size(44, 17);
            this.cbDLSkyRefl.TabIndex = 0;
            this.cbDLSkyRefl.Text = "S&ky";
            this.cbDLSkyRefl.UseVisualStyleBackColor = true;
            // 
            // tpTools
            // 
            this.tpTools.Controls.Add(this.bPatchEd);
            this.tpTools.Controls.Add(this.bDistantLandWizard);
            this.tpTools.Controls.Add(this.bHookGen);
            this.tpTools.Controls.Add(this.bShaderEd);
            this.tpTools.Location = new System.Drawing.Point(4, 22);
            this.tpTools.Name = "tpTools";
            this.tpTools.Size = new System.Drawing.Size(538, 353);
            this.tpTools.TabIndex = 6;
            this.tpTools.Text = "Tools";
            this.tpTools.ToolTipText = "Contains Shader editor, Texture hook generator,\nand Distant Land File Creation Wi" +
                "zard.";
            this.tpTools.UseVisualStyleBackColor = true;
            // 
            // bPatchEd
            // 
            this.bPatchEd.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bPatchEd.Location = new System.Drawing.Point(8, 6);
            this.bPatchEd.Name = "bPatchEd";
            this.bPatchEd.Size = new System.Drawing.Size(522, 25);
            this.bPatchEd.TabIndex = 0;
            this.bPatchEd.Text = "Patch editor";
            this.bPatchEd.UseVisualStyleBackColor = true;
            this.bPatchEd.Click += new System.EventHandler(this.bPatchEd_Click);
            // 
            // bDistantLandWizard
            // 
            this.bDistantLandWizard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bDistantLandWizard.Location = new System.Drawing.Point(8, 99);
            this.bDistantLandWizard.Name = "bDistantLandWizard";
            this.bDistantLandWizard.Size = new System.Drawing.Size(522, 25);
            this.bDistantLandWizard.TabIndex = 3;
            this.bDistantLandWizard.Text = "Distant land file creation wizard";
            this.bDistantLandWizard.Click += new System.EventHandler(this.bDistantLandWizard_Click);
            // 
            // bHookGen
            // 
            this.bHookGen.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bHookGen.Location = new System.Drawing.Point(8, 68);
            this.bHookGen.Name = "bHookGen";
            this.bHookGen.Size = new System.Drawing.Size(522, 25);
            this.bHookGen.TabIndex = 2;
            this.bHookGen.Text = "Texture hook generator";
            this.bHookGen.Click += new System.EventHandler(this.bHookGen_Click);
            // 
            // bShaderEd
            // 
            this.bShaderEd.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bShaderEd.Location = new System.Drawing.Point(8, 37);
            this.bShaderEd.Name = "bShaderEd";
            this.bShaderEd.Size = new System.Drawing.Size(522, 25);
            this.bShaderEd.TabIndex = 1;
            this.bShaderEd.Text = "Shader editor";
            this.bShaderEd.Click += new System.EventHandler(this.bShaderEd_Click);
            // 
            // tpINI
            // 
            this.tpINI.Controls.Add(this.gbMWGeneral);
            this.tpINI.Controls.Add(this.lScreenshotStatus);
            this.tpINI.Location = new System.Drawing.Point(4, 22);
            this.tpINI.Name = "tpINI";
            this.tpINI.Size = new System.Drawing.Size(538, 353);
            this.tpINI.TabIndex = 5;
            this.tpINI.Text = "Morrowind.ini";
            this.tpINI.ToolTipText = "Allows you to comfortably change several settings\nin the Morrowind.ini file from " +
                "within MGE.";
            this.tpINI.UseVisualStyleBackColor = true;
            // 
            // gbMWGeneral
            // 
            this.gbMWGeneral.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMWGeneral.Controls.Add(this.cbScreenshots);
            this.gbMWGeneral.Controls.Add(this.cbHitFader);
            this.gbMWGeneral.Controls.Add(this.cbMultipleCS);
            this.gbMWGeneral.Controls.Add(this.cbAudio);
            this.gbMWGeneral.Controls.Add(this.cbHQShadows);
            this.gbMWGeneral.Controls.Add(this.cbShowFPS);
            this.gbMWGeneral.Controls.Add(this.cbThreadLoad);
            this.gbMWGeneral.Controls.Add(this.cbYesToAll);
            this.gbMWGeneral.Controls.Add(this.cbSubtitles);
            this.gbMWGeneral.Location = new System.Drawing.Point(8, 3);
            this.gbMWGeneral.Name = "gbMWGeneral";
            this.gbMWGeneral.Size = new System.Drawing.Size(522, 140);
            this.gbMWGeneral.TabIndex = 0;
            this.gbMWGeneral.TabStop = false;
            this.gbMWGeneral.Text = "General settings";
            // 
            // cbScreenshots
            // 
            this.cbScreenshots.AutoSize = true;
            this.cbScreenshots.Enabled = false;
            this.cbScreenshots.Location = new System.Drawing.Point(13, 21);
            this.cbScreenshots.Name = "cbScreenshots";
            this.cbScreenshots.Size = new System.Drawing.Size(111, 17);
            this.cbScreenshots.TabIndex = 0;
            this.cbScreenshots.Text = "Allow screenshots";
            // 
            // cbHitFader
            // 
            this.cbHitFader.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbHitFader.AutoSize = true;
            this.cbHitFader.Checked = true;
            this.cbHitFader.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbHitFader.Location = new System.Drawing.Point(261, 90);
            this.cbHitFader.Name = "cbHitFader";
            this.cbHitFader.Size = new System.Drawing.Size(69, 17);
            this.cbHitFader.TabIndex = 8;
            this.cbHitFader.Text = "Hit Fader";
            // 
            // cbMultipleCS
            // 
            this.cbMultipleCS.AutoSize = true;
            this.cbMultipleCS.Location = new System.Drawing.Point(13, 113);
            this.cbMultipleCS.Name = "cbMultipleCS";
            this.cbMultipleCS.Size = new System.Drawing.Size(154, 17);
            this.cbMultipleCS.TabIndex = 4;
            this.cbMultipleCS.Text = "Allow multiple CS instances";
            // 
            // cbAudio
            // 
            this.cbAudio.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbAudio.AutoSize = true;
            this.cbAudio.Location = new System.Drawing.Point(261, 67);
            this.cbAudio.Name = "cbAudio";
            this.cbAudio.Size = new System.Drawing.Size(90, 17);
            this.cbAudio.TabIndex = 7;
            this.cbAudio.Text = "Disable audio";
            // 
            // cbHQShadows
            // 
            this.cbHQShadows.AutoSize = true;
            this.cbHQShadows.Location = new System.Drawing.Point(13, 44);
            this.cbHQShadows.Name = "cbHQShadows";
            this.cbHQShadows.Size = new System.Drawing.Size(121, 17);
            this.cbHQShadows.TabIndex = 1;
            this.cbHQShadows.Text = "High detail shadows";
            // 
            // cbShowFPS
            // 
            this.cbShowFPS.AutoSize = true;
            this.cbShowFPS.Location = new System.Drawing.Point(13, 67);
            this.cbShowFPS.Name = "cbShowFPS";
            this.cbShowFPS.Size = new System.Drawing.Size(76, 17);
            this.cbShowFPS.TabIndex = 2;
            this.cbShowFPS.Text = "Show FPS";
            // 
            // cbThreadLoad
            // 
            this.cbThreadLoad.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbThreadLoad.AutoSize = true;
            this.cbThreadLoad.Checked = true;
            this.cbThreadLoad.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbThreadLoad.Location = new System.Drawing.Point(261, 44);
            this.cbThreadLoad.Name = "cbThreadLoad";
            this.cbThreadLoad.Size = new System.Drawing.Size(97, 17);
            this.cbThreadLoad.TabIndex = 6;
            this.cbThreadLoad.Text = "Thread loading";
            // 
            // cbYesToAll
            // 
            this.cbYesToAll.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbYesToAll.AutoSize = true;
            this.cbYesToAll.Location = new System.Drawing.Point(261, 21);
            this.cbYesToAll.Name = "cbYesToAll";
            this.cbYesToAll.Size = new System.Drawing.Size(95, 17);
            this.cbYesToAll.TabIndex = 5;
            this.cbYesToAll.Text = "Allow yes to all";
            // 
            // cbSubtitles
            // 
            this.cbSubtitles.AutoSize = true;
            this.cbSubtitles.Location = new System.Drawing.Point(13, 90);
            this.cbSubtitles.Name = "cbSubtitles";
            this.cbSubtitles.Size = new System.Drawing.Size(66, 17);
            this.cbSubtitles.TabIndex = 3;
            this.cbSubtitles.Text = "Subtitles";
            // 
            // lScreenshotStatus
            // 
            this.lScreenshotStatus.AutoSize = true;
            this.lScreenshotStatus.Location = new System.Drawing.Point(8, 148);
            this.lScreenshotStatus.Name = "lScreenshotStatus";
            this.lScreenshotStatus.Size = new System.Drawing.Size(407, 26);
            this.lScreenshotStatus.TabIndex = 0;
            this.lScreenshotStatus.Text = "Morrowind\'s normal method of taking screenshots cannot be used with MGE\r\nIf you w" +
                "ish to takes screenshots, set up a \'take screenshot\' macro in the macro editor";
            this.lScreenshotStatus.Visible = false;
            // 
            // tpMisc
            // 
            this.tpMisc.Controls.Add(this.cbDisableIntegration);
            this.tpMisc.Controls.Add(this.bPatch);
            this.tpMisc.Controls.Add(this.cbDisablePatch);
            this.tpMisc.Controls.Add(this.bMWE);
            this.tpMisc.Controls.Add(this.pCam3rdPrsn);
            this.tpMisc.Controls.Add(this.pIdle);
            this.tpMisc.Controls.Add(this.pFPSConst);
            this.tpMisc.Controls.Add(this.gbShaders);
            this.tpMisc.Controls.Add(this.cbDisableMWE);
            this.tpMisc.Controls.Add(this.cbDisableMWSE);
            this.tpMisc.Controls.Add(this.cbAspectZoom);
            this.tpMisc.Controls.Add(this.cbZoomEffectsHUD);
            this.tpMisc.Controls.Add(this.cbBindAI);
            this.tpMisc.Location = new System.Drawing.Point(4, 22);
            this.tpMisc.Name = "tpMisc";
            this.tpMisc.Size = new System.Drawing.Size(538, 353);
            this.tpMisc.TabIndex = 4;
            this.tpMisc.Text = "Misc";
            this.tpMisc.ToolTipText = "Contains settings that did not fit well on any other tab,\nincluding the setting t" +
                "hat disables MWSE.";
            this.tpMisc.UseVisualStyleBackColor = true;
            // 
            // cbDisableIntegration
            // 
            this.cbDisableIntegration.AutoSize = true;
            this.cbDisableIntegration.Location = new System.Drawing.Point(14, 122);
            this.cbDisableIntegration.Name = "cbDisableIntegration";
            this.cbDisableIntegration.Size = new System.Drawing.Size(140, 17);
            this.cbDisableIntegration.TabIndex = 5;
            this.cbDisableIntegration.Text = "Disable other integration";
            this.cbDisableIntegration.UseVisualStyleBackColor = true;
            // 
            // bPatch
            // 
            this.bPatch.Location = new System.Drawing.Point(148, 143);
            this.bPatch.Name = "bPatch";
            this.bPatch.Size = new System.Drawing.Size(130, 25);
            this.bPatch.TabIndex = 7;
            this.bPatch.Text = "Patch settings...";
            this.bPatch.UseVisualStyleBackColor = true;
            this.bPatch.Click += new System.EventHandler(this.bPatch_Click);
            // 
            // cbDisablePatch
            // 
            this.cbDisablePatch.AutoSize = true;
            this.cbDisablePatch.Location = new System.Drawing.Point(14, 147);
            this.cbDisablePatch.Name = "cbDisablePatch";
            this.cbDisablePatch.Size = new System.Drawing.Size(118, 17);
            this.cbDisablePatch.TabIndex = 6;
            this.cbDisablePatch.Text = "Disable code patch";
            this.cbDisablePatch.UseVisualStyleBackColor = true;
            this.cbDisablePatch.CheckStateChanged += new System.EventHandler(this.cbDisablePatch_CheckStateChanged);
            // 
            // bMWE
            // 
            this.bMWE.Location = new System.Drawing.Point(148, 93);
            this.bMWE.Name = "bMWE";
            this.bMWE.Size = new System.Drawing.Size(130, 25);
            this.bMWE.TabIndex = 4;
            this.bMWE.Text = "MWE settings...";
            this.bMWE.Click += new System.EventHandler(this.bMWE_Click);
            // 
            // pCam3rdPrsn
            // 
            this.pCam3rdPrsn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.pCam3rdPrsn.Controls.Add(this.cbCam3rdPrsn);
            this.pCam3rdPrsn.Controls.Add(this.gbCam3rdPrsn);
            this.pCam3rdPrsn.Location = new System.Drawing.Point(286, 201);
            this.pCam3rdPrsn.Name = "pCam3rdPrsn";
            this.pCam3rdPrsn.Size = new System.Drawing.Size(244, 82);
            this.pCam3rdPrsn.TabIndex = 11;
            this.pCam3rdPrsn.TabStop = true;
            // 
            // cbCam3rdPrsn
            // 
            this.cbCam3rdPrsn.AutoSize = true;
            this.cbCam3rdPrsn.Location = new System.Drawing.Point(8, -2);
            this.cbCam3rdPrsn.Name = "cbCam3rdPrsn";
            this.cbCam3rdPrsn.Size = new System.Drawing.Size(165, 17);
            this.cbCam3rdPrsn.TabIndex = 0;
            this.cbCam3rdPrsn.Text = "Customize 3rd person camera";
            this.cbCam3rdPrsn.UseVisualStyleBackColor = true;
            this.cbCam3rdPrsn.CheckedChanged += new System.EventHandler(this.cbCam3rdPrsn_CheckedChanged);
            // 
            // gbCam3rdPrsn
            // 
            this.gbCam3rdPrsn.Controls.Add(this.udCam3rdZ);
            this.gbCam3rdPrsn.Controls.Add(this.udCam3rdY);
            this.gbCam3rdPrsn.Controls.Add(this.udCam3rdX);
            this.gbCam3rdPrsn.Controls.Add(this.lCam3rdX);
            this.gbCam3rdPrsn.Controls.Add(this.lCam3rdY);
            this.gbCam3rdPrsn.Controls.Add(this.lCam3rdZ);
            this.gbCam3rdPrsn.Enabled = false;
            this.gbCam3rdPrsn.Location = new System.Drawing.Point(0, 0);
            this.gbCam3rdPrsn.Name = "gbCam3rdPrsn";
            this.gbCam3rdPrsn.Size = new System.Drawing.Size(244, 82);
            this.gbCam3rdPrsn.TabIndex = 1;
            this.gbCam3rdPrsn.TabStop = false;
            // 
            // udCam3rdZ
            // 
            this.udCam3rdZ.DecimalPlaces = 1;
            this.udCam3rdZ.Location = new System.Drawing.Point(178, 48);
            this.udCam3rdZ.Maximum = new decimal(new int[] {
            125,
            0,
            0,
            0});
            this.udCam3rdZ.Minimum = new decimal(new int[] {
            125,
            0,
            0,
            -2147483648});
            this.udCam3rdZ.Name = "udCam3rdZ";
            this.udCam3rdZ.Size = new System.Drawing.Size(56, 20);
            this.udCam3rdZ.TabIndex = 2;
            this.udCam3rdZ.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udCam3rdZ.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            // 
            // udCam3rdY
            // 
            this.udCam3rdY.DecimalPlaces = 1;
            this.udCam3rdY.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udCam3rdY.Location = new System.Drawing.Point(90, 48);
            this.udCam3rdY.Maximum = new decimal(new int[] {
            25,
            0,
            0,
            -2147483648});
            this.udCam3rdY.Minimum = new decimal(new int[] {
            2500,
            0,
            0,
            -2147483648});
            this.udCam3rdY.Name = "udCam3rdY";
            this.udCam3rdY.Size = new System.Drawing.Size(64, 20);
            this.udCam3rdY.TabIndex = 1;
            this.udCam3rdY.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udCam3rdY.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udCam3rdY.Value = new decimal(new int[] {
            160,
            0,
            0,
            -2147483648});
            // 
            // udCam3rdX
            // 
            this.udCam3rdX.DecimalPlaces = 1;
            this.udCam3rdX.Location = new System.Drawing.Point(10, 48);
            this.udCam3rdX.Maximum = new decimal(new int[] {
            125,
            0,
            0,
            0});
            this.udCam3rdX.Minimum = new decimal(new int[] {
            125,
            0,
            0,
            -2147483648});
            this.udCam3rdX.Name = "udCam3rdX";
            this.udCam3rdX.Size = new System.Drawing.Size(56, 20);
            this.udCam3rdX.TabIndex = 0;
            this.udCam3rdX.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udCam3rdX.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            // 
            // lCam3rdX
            // 
            this.lCam3rdX.AutoSize = true;
            this.lCam3rdX.Location = new System.Drawing.Point(7, 32);
            this.lCam3rdX.Name = "lCam3rdX";
            this.lCam3rdX.Size = new System.Drawing.Size(43, 13);
            this.lCam3rdX.TabIndex = 3;
            this.lCam3rdX.Text = "X offset";
            // 
            // lCam3rdY
            // 
            this.lCam3rdY.AutoSize = true;
            this.lCam3rdY.Location = new System.Drawing.Point(87, 32);
            this.lCam3rdY.Name = "lCam3rdY";
            this.lCam3rdY.Size = new System.Drawing.Size(43, 13);
            this.lCam3rdY.TabIndex = 4;
            this.lCam3rdY.Text = "Y offset";
            // 
            // lCam3rdZ
            // 
            this.lCam3rdZ.AutoSize = true;
            this.lCam3rdZ.Location = new System.Drawing.Point(175, 32);
            this.lCam3rdZ.Name = "lCam3rdZ";
            this.lCam3rdZ.Size = new System.Drawing.Size(43, 13);
            this.lCam3rdZ.TabIndex = 5;
            this.lCam3rdZ.Text = "Z offset";
            // 
            // pIdle
            // 
            this.pIdle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.pIdle.Controls.Add(this.cbIdle);
            this.pIdle.Controls.Add(this.gbIdle);
            this.pIdle.Location = new System.Drawing.Point(286, 291);
            this.pIdle.Name = "pIdle";
            this.pIdle.Size = new System.Drawing.Size(244, 54);
            this.pIdle.TabIndex = 12;
            this.pIdle.TabStop = true;
            // 
            // cbIdle
            // 
            this.cbIdle.AutoSize = true;
            this.cbIdle.Location = new System.Drawing.Point(8, -2);
            this.cbIdle.Name = "cbIdle";
            this.cbIdle.Size = new System.Drawing.Size(67, 17);
            this.cbIdle.TabIndex = 0;
            this.cbIdle.Text = "CPU idle";
            this.cbIdle.CheckedChanged += new System.EventHandler(this.cbIdle_CheckedChanged);
            // 
            // gbIdle
            // 
            this.gbIdle.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbIdle.Controls.Add(this.udIdle);
            this.gbIdle.Controls.Add(this.lIdle);
            this.gbIdle.Enabled = false;
            this.gbIdle.Location = new System.Drawing.Point(0, 0);
            this.gbIdle.Name = "gbIdle";
            this.gbIdle.Size = new System.Drawing.Size(244, 54);
            this.gbIdle.TabIndex = 1;
            this.gbIdle.TabStop = false;
            // 
            // udIdle
            // 
            this.udIdle.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udIdle.Location = new System.Drawing.Point(10, 21);
            this.udIdle.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.udIdle.Name = "udIdle";
            this.udIdle.Size = new System.Drawing.Size(56, 20);
            this.udIdle.TabIndex = 0;
            this.udIdle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udIdle.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            // 
            // lIdle
            // 
            this.lIdle.AutoSize = true;
            this.lIdle.Location = new System.Drawing.Point(70, 24);
            this.lIdle.Name = "lIdle";
            this.lIdle.Size = new System.Drawing.Size(68, 13);
            this.lIdle.TabIndex = 0;
            this.lIdle.Text = "Idle time (ms)";
            // 
            // pFPSConst
            // 
            this.pFPSConst.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pFPSConst.Controls.Add(this.cbFPSConst);
            this.pFPSConst.Controls.Add(this.gbFPSConst);
            this.pFPSConst.Location = new System.Drawing.Point(286, 3);
            this.pFPSConst.Name = "pFPSConst";
            this.pFPSConst.Size = new System.Drawing.Size(244, 138);
            this.pFPSConst.TabIndex = 9;
            this.pFPSConst.TabStop = true;
            // 
            // cbFPSConst
            // 
            this.cbFPSConst.AutoSize = true;
            this.cbFPSConst.Location = new System.Drawing.Point(8, -2);
            this.cbFPSConst.Name = "cbFPSConst";
            this.cbFPSConst.Size = new System.Drawing.Size(118, 17);
            this.cbFPSConst.TabIndex = 0;
            this.cbFPSConst.Text = "Keep FPS constant";
            this.cbFPSConst.CheckedChanged += new System.EventHandler(this.cbFPSConst_CheckedChanged);
            // 
            // gbFPSConst
            // 
            this.gbFPSConst.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbFPSConst.Controls.Add(this.udFPSDes);
            this.gbFPSConst.Controls.Add(this.udFPSTol);
            this.gbFPSConst.Controls.Add(this.udMinView);
            this.gbFPSConst.Controls.Add(this.lFPSDes);
            this.gbFPSConst.Controls.Add(this.udMaxView);
            this.gbFPSConst.Controls.Add(this.lMinView);
            this.gbFPSConst.Controls.Add(this.lFPSTol);
            this.gbFPSConst.Controls.Add(this.lMaxView);
            this.gbFPSConst.Enabled = false;
            this.gbFPSConst.Location = new System.Drawing.Point(0, 0);
            this.gbFPSConst.Name = "gbFPSConst";
            this.gbFPSConst.Size = new System.Drawing.Size(244, 138);
            this.gbFPSConst.TabIndex = 1;
            this.gbFPSConst.TabStop = false;
            // 
            // udFPSDes
            // 
            this.udFPSDes.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udFPSDes.Location = new System.Drawing.Point(10, 23);
            this.udFPSDes.Maximum = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.udFPSDes.Name = "udFPSDes";
            this.udFPSDes.Size = new System.Drawing.Size(56, 20);
            this.udFPSDes.TabIndex = 0;
            this.udFPSDes.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udFPSDes.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udFPSDes.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // udFPSTol
            // 
            this.udFPSTol.Location = new System.Drawing.Point(10, 51);
            this.udFPSTol.Maximum = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.udFPSTol.Name = "udFPSTol";
            this.udFPSTol.Size = new System.Drawing.Size(56, 20);
            this.udFPSTol.TabIndex = 1;
            this.udFPSTol.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udFPSTol.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udFPSTol.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // udMinView
            // 
            this.udMinView.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.udMinView.Location = new System.Drawing.Point(10, 79);
            this.udMinView.Maximum = new decimal(new int[] {
            25000,
            0,
            0,
            0});
            this.udMinView.Minimum = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.udMinView.Name = "udMinView";
            this.udMinView.Size = new System.Drawing.Size(56, 20);
            this.udMinView.TabIndex = 2;
            this.udMinView.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udMinView.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udMinView.Value = new decimal(new int[] {
            2500,
            0,
            0,
            0});
            this.udMinView.ValueChanged += new System.EventHandler(this.udMinView_ValueChanged);
            // 
            // lFPSDes
            // 
            this.lFPSDes.AutoSize = true;
            this.lFPSDes.Location = new System.Drawing.Point(70, 26);
            this.lFPSDes.Name = "lFPSDes";
            this.lFPSDes.Size = new System.Drawing.Size(66, 13);
            this.lFPSDes.TabIndex = 0;
            this.lFPSDes.Text = "Desired FPS";
            // 
            // udMaxView
            // 
            this.udMaxView.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.udMaxView.Location = new System.Drawing.Point(10, 107);
            this.udMaxView.Maximum = new decimal(new int[] {
            25000,
            0,
            0,
            0});
            this.udMaxView.Minimum = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.udMaxView.Name = "udMaxView";
            this.udMaxView.Size = new System.Drawing.Size(56, 20);
            this.udMaxView.TabIndex = 3;
            this.udMaxView.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udMaxView.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udMaxView.Value = new decimal(new int[] {
            7168,
            0,
            0,
            0});
            this.udMaxView.ValueChanged += new System.EventHandler(this.udMaxView_ValueChanged);
            // 
            // lMinView
            // 
            this.lMinView.AutoSize = true;
            this.lMinView.Location = new System.Drawing.Point(70, 82);
            this.lMinView.Name = "lMinView";
            this.lMinView.Size = new System.Drawing.Size(92, 13);
            this.lMinView.TabIndex = 0;
            this.lMinView.Text = "Min view distance";
            // 
            // lFPSTol
            // 
            this.lFPSTol.AutoSize = true;
            this.lFPSTol.Location = new System.Drawing.Point(70, 54);
            this.lFPSTol.Name = "lFPSTol";
            this.lFPSTol.Size = new System.Drawing.Size(55, 13);
            this.lFPSTol.TabIndex = 0;
            this.lFPSTol.Text = "Tolerence";
            // 
            // lMaxView
            // 
            this.lMaxView.AutoSize = true;
            this.lMaxView.Location = new System.Drawing.Point(70, 110);
            this.lMaxView.Name = "lMaxView";
            this.lMaxView.Size = new System.Drawing.Size(95, 13);
            this.lMaxView.TabIndex = 0;
            this.lMaxView.Text = "Max view distance";
            // 
            // gbShaders
            // 
            this.gbShaders.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbShaders.Controls.Add(this.cbHDR);
            this.gbShaders.Controls.Add(this.cbShaderDepth);
            this.gbShaders.Controls.Add(this.cbHook);
            this.gbShaders.Controls.Add(this.cbSetEffectVars);
            this.gbShaders.Controls.Add(this.gbHDR);
            this.gbShaders.Location = new System.Drawing.Point(8, 201);
            this.gbShaders.Name = "gbShaders";
            this.gbShaders.Size = new System.Drawing.Size(272, 144);
            this.gbShaders.TabIndex = 8;
            this.gbShaders.TabStop = false;
            this.gbShaders.Text = "Shaders";
            // 
            // cbHDR
            // 
            this.cbHDR.AutoSize = true;
            this.cbHDR.Location = new System.Drawing.Point(14, 88);
            this.cbHDR.Name = "cbHDR";
            this.cbHDR.Size = new System.Drawing.Size(157, 17);
            this.cbHDR.TabIndex = 3;
            this.cbHDR.Text = "Supply HDR info to shaders";
            this.cbHDR.CheckedChanged += new System.EventHandler(this.cbHDR_CheckedChanged);
            // 
            // cbShaderDepth
            // 
            this.cbShaderDepth.AutoSize = true;
            this.cbShaderDepth.Location = new System.Drawing.Point(14, 65);
            this.cbShaderDepth.Name = "cbShaderDepth";
            this.cbShaderDepth.Size = new System.Drawing.Size(170, 17);
            this.cbShaderDepth.TabIndex = 2;
            this.cbShaderDepth.Text = "Supply shaders with depth info";
            this.cbShaderDepth.UseVisualStyleBackColor = true;
            // 
            // cbHook
            // 
            this.cbHook.AutoSize = true;
            this.cbHook.Location = new System.Drawing.Point(14, 19);
            this.cbHook.Name = "cbHook";
            this.cbHook.Size = new System.Drawing.Size(115, 17);
            this.cbHook.TabIndex = 0;
            this.cbHook.Text = "Hook texture loads";
            // 
            // cbSetEffectVars
            // 
            this.cbSetEffectVars.AutoSize = true;
            this.cbSetEffectVars.Location = new System.Drawing.Point(14, 42);
            this.cbSetEffectVars.Name = "cbSetEffectVars";
            this.cbSetEffectVars.Size = new System.Drawing.Size(141, 17);
            this.cbSetEffectVars.TabIndex = 1;
            this.cbSetEffectVars.Text = "Update shader variables";
            // 
            // gbHDR
            // 
            this.gbHDR.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbHDR.Controls.Add(this.udHDR);
            this.gbHDR.Controls.Add(this.lHDR);
            this.gbHDR.Enabled = false;
            this.gbHDR.Location = new System.Drawing.Point(6, 90);
            this.gbHDR.Name = "gbHDR";
            this.gbHDR.Size = new System.Drawing.Size(260, 48);
            this.gbHDR.TabIndex = 4;
            this.gbHDR.TabStop = false;
            // 
            // udHDR
            // 
            this.udHDR.DecimalPlaces = 2;
            this.udHDR.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udHDR.Location = new System.Drawing.Point(10, 21);
            this.udHDR.Maximum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.udHDR.Name = "udHDR";
            this.udHDR.Size = new System.Drawing.Size(56, 20);
            this.udHDR.TabIndex = 0;
            this.udHDR.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udHDR.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udHDR.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            // 
            // lHDR
            // 
            this.lHDR.AutoSize = true;
            this.lHDR.Location = new System.Drawing.Point(70, 24);
            this.lHDR.Name = "lHDR";
            this.lHDR.Size = new System.Drawing.Size(125, 13);
            this.lHDR.TabIndex = 0;
            this.lHDR.Text = "HDR reaction time (secs)";
            // 
            // cbDisableMWE
            // 
            this.cbDisableMWE.AutoSize = true;
            this.cbDisableMWE.Location = new System.Drawing.Point(14, 97);
            this.cbDisableMWE.Name = "cbDisableMWE";
            this.cbDisableMWE.Size = new System.Drawing.Size(128, 17);
            this.cbDisableMWE.TabIndex = 3;
            this.cbDisableMWE.Text = "Disable internal MWE";
            this.cbDisableMWE.CheckStateChanged += new System.EventHandler(this.cbDisableMWE_CheckStateChanged);
            // 
            // cbDisableMWSE
            // 
            this.cbDisableMWSE.AutoSize = true;
            this.cbDisableMWSE.Location = new System.Drawing.Point(14, 72);
            this.cbDisableMWSE.Name = "cbDisableMWSE";
            this.cbDisableMWSE.Size = new System.Drawing.Size(172, 17);
            this.cbDisableMWSE.TabIndex = 2;
            this.cbDisableMWSE.Text = "Disable internal MWSE version";
            // 
            // cbAspectZoom
            // 
            this.cbAspectZoom.AutoSize = true;
            this.cbAspectZoom.Location = new System.Drawing.Point(14, 47);
            this.cbAspectZoom.Name = "cbAspectZoom";
            this.cbAspectZoom.Size = new System.Drawing.Size(189, 17);
            this.cbAspectZoom.TabIndex = 1;
            this.cbAspectZoom.Text = "Correct aspect ratio when zooming";
            // 
            // cbZoomEffectsHUD
            // 
            this.cbZoomEffectsHUD.AutoSize = true;
            this.cbZoomEffectsHUD.Location = new System.Drawing.Point(14, 22);
            this.cbZoomEffectsHUD.Name = "cbZoomEffectsHUD";
            this.cbZoomEffectsHUD.Size = new System.Drawing.Size(172, 17);
            this.cbZoomEffectsHUD.TabIndex = 0;
            this.cbZoomEffectsHUD.Text = "Camera effects affect the HUD";
            // 
            // cbBindAI
            // 
            this.cbBindAI.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbBindAI.Location = new System.Drawing.Point(294, 147);
            this.cbBindAI.Name = "cbBindAI";
            this.cbBindAI.Size = new System.Drawing.Size(236, 19);
            this.cbBindAI.TabIndex = 10;
            this.cbBindAI.Text = "Bind AI distance to view distance";
            // 
            // tpInput
            // 
            this.tpInput.Controls.Add(this.gbInputTools);
            this.tpInput.Controls.Add(this.gbInputOpts);
            this.tpInput.Location = new System.Drawing.Point(4, 22);
            this.tpInput.Name = "tpInput";
            this.tpInput.Size = new System.Drawing.Size(538, 353);
            this.tpInput.TabIndex = 3;
            this.tpInput.Text = "Input";
            this.tpInput.ToolTipText = "Contains options affecting input, and also\nmacro editor and key remapper.";
            this.tpInput.UseVisualStyleBackColor = true;
            // 
            // gbInputTools
            // 
            this.gbInputTools.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gbInputTools.Controls.Add(this.bMacroEd);
            this.gbInputTools.Controls.Add(this.bRemapper);
            this.gbInputTools.Location = new System.Drawing.Point(286, 3);
            this.gbInputTools.Name = "gbInputTools";
            this.gbInputTools.Size = new System.Drawing.Size(244, 115);
            this.gbInputTools.TabIndex = 1;
            this.gbInputTools.TabStop = false;
            this.gbInputTools.Text = "Tools";
            // 
            // bMacroEd
            // 
            this.bMacroEd.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bMacroEd.Location = new System.Drawing.Point(6, 19);
            this.bMacroEd.Name = "bMacroEd";
            this.bMacroEd.Size = new System.Drawing.Size(232, 25);
            this.bMacroEd.TabIndex = 0;
            this.bMacroEd.Text = "Launch macro editor";
            this.bMacroEd.Click += new System.EventHandler(this.bMacroEd_Click);
            // 
            // bRemapper
            // 
            this.bRemapper.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bRemapper.Location = new System.Drawing.Point(6, 50);
            this.bRemapper.Name = "bRemapper";
            this.bRemapper.Size = new System.Drawing.Size(232, 25);
            this.bRemapper.TabIndex = 1;
            this.bRemapper.Text = "Launch remapper";
            this.bRemapper.Click += new System.EventHandler(this.bRemapper_Click);
            // 
            // gbInputOpts
            // 
            this.gbInputOpts.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbInputOpts.Controls.Add(this.cbSkipMovie);
            this.gbInputOpts.Controls.Add(this.cbConsole);
            this.gbInputOpts.Controls.Add(this.cbLagFix);
            this.gbInputOpts.Controls.Add(this.cbAltCombat);
            this.gbInputOpts.Location = new System.Drawing.Point(8, 3);
            this.gbInputOpts.Name = "gbInputOpts";
            this.gbInputOpts.Size = new System.Drawing.Size(272, 115);
            this.gbInputOpts.TabIndex = 0;
            this.gbInputOpts.TabStop = false;
            this.gbInputOpts.Text = "Options";
            // 
            // cbSkipMovie
            // 
            this.cbSkipMovie.AutoSize = true;
            this.cbSkipMovie.Location = new System.Drawing.Point(10, 19);
            this.cbSkipMovie.Name = "cbSkipMovie";
            this.cbSkipMovie.Size = new System.Drawing.Size(119, 17);
            this.cbSkipMovie.TabIndex = 0;
            this.cbSkipMovie.Text = "Skip opening movie";
            // 
            // cbConsole
            // 
            this.cbConsole.AutoSize = true;
            this.cbConsole.Location = new System.Drawing.Point(10, 42);
            this.cbConsole.Name = "cbConsole";
            this.cbConsole.Size = new System.Drawing.Size(101, 17);
            this.cbConsole.TabIndex = 1;
            this.cbConsole.Text = "Disable console";
            // 
            // cbLagFix
            // 
            this.cbLagFix.AutoSize = true;
            this.cbLagFix.Location = new System.Drawing.Point(10, 88);
            this.cbLagFix.Name = "cbLagFix";
            this.cbLagFix.Size = new System.Drawing.Size(80, 17);
            this.cbLagFix.TabIndex = 3;
            this.cbLagFix.Text = "Input lag fix";
            // 
            // cbAltCombat
            // 
            this.cbAltCombat.AutoSize = true;
            this.cbAltCombat.Location = new System.Drawing.Point(10, 65);
            this.cbAltCombat.Name = "cbAltCombat";
            this.cbAltCombat.Size = new System.Drawing.Size(173, 17);
            this.cbAltCombat.TabIndex = 2;
            this.cbAltCombat.Text = "Allow alternate combat controls";
            // 
            // tpRender
            // 
            this.tpRender.Controls.Add(this.gbMsgs);
            this.tpRender.Controls.Add(this.gbFog);
            this.tpRender.Controls.Add(this.gbRendOpts);
            this.tpRender.Controls.Add(this.gbScreen);
            this.tpRender.Controls.Add(this.gbTexFilt);
            this.tpRender.Location = new System.Drawing.Point(4, 22);
            this.tpRender.Name = "tpRender";
            this.tpRender.Size = new System.Drawing.Size(538, 353);
            this.tpRender.TabIndex = 2;
            this.tpRender.Text = "Render state";
            this.tpRender.ToolTipText = "Contains lots of settings to influence the way\nhow Morrowind renders its graphics" +
                ".";
            this.tpRender.UseVisualStyleBackColor = true;
            // 
            // gbMsgs
            // 
            this.gbMsgs.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbMsgs.Controls.Add(this.udMsgsTime);
            this.gbMsgs.Controls.Add(this.cmbMsgsLocale);
            this.gbMsgs.Controls.Add(this.lMsgsTime);
            this.gbMsgs.Controls.Add(this.lMsgsLanguage);
            this.gbMsgs.Controls.Add(this.lMsgsTimeUnit);
            this.gbMsgs.Location = new System.Drawing.Point(8, 268);
            this.gbMsgs.Name = "gbMsgs";
            this.gbMsgs.Size = new System.Drawing.Size(258, 77);
            this.gbMsgs.TabIndex = 4;
            this.gbMsgs.TabStop = false;
            this.gbMsgs.Text = "MGE status messages";
            // 
            // udMsgsTime
            // 
            this.udMsgsTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.udMsgsTime.Increment = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.udMsgsTime.Location = new System.Drawing.Point(161, 41);
            this.udMsgsTime.Maximum = new decimal(new int[] {
            50000,
            0,
            0,
            0});
            this.udMsgsTime.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.udMsgsTime.Name = "udMsgsTime";
            this.udMsgsTime.Size = new System.Drawing.Size(56, 20);
            this.udMsgsTime.TabIndex = 1;
            this.udMsgsTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udMsgsTime.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udMsgsTime.Value = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            // 
            // cmbMsgsLocale
            // 
            this.cmbMsgsLocale.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmbMsgsLocale.Enabled = false;
            this.cmbMsgsLocale.FormattingEnabled = true;
            this.cmbMsgsLocale.Items.AddRange(new object[] { MGEgui.Localization.LocalizationInterface.DefaultLanguage });
            this.cmbMsgsLocale.Location = new System.Drawing.Point(10, 41);
            this.cmbMsgsLocale.Name = "cmbMsgsLocale";
            this.cmbMsgsLocale.Size = new System.Drawing.Size(128, 21);
            this.cmbMsgsLocale.Sorted = true;
            this.cmbMsgsLocale.TabIndex = 0;
            this.cmbMsgsLocale.Text = MGEgui.Localization.LocalizationInterface.DefaultLanguage;
            this.cmbMsgsLocale.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lMsgsTime
            // 
            this.lMsgsTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lMsgsTime.AutoSize = true;
            this.lMsgsTime.Location = new System.Drawing.Point(158, 25);
            this.lMsgsTime.Name = "lMsgsTime";
            this.lMsgsTime.Size = new System.Drawing.Size(45, 13);
            this.lMsgsTime.TabIndex = 0;
            this.lMsgsTime.Text = "Timeout";
            // 
            // lMsgsLanguage
            // 
            this.lMsgsLanguage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lMsgsLanguage.AutoSize = true;
            this.lMsgsLanguage.Enabled = false;
            this.lMsgsLanguage.Location = new System.Drawing.Point(8, 25);
            this.lMsgsLanguage.Name = "lMsgsLanguage";
            this.lMsgsLanguage.Size = new System.Drawing.Size(110, 13);
            this.lMsgsLanguage.TabIndex = 0;
            this.lMsgsLanguage.Text = "Language localization";
            // 
            // lMsgsTimeUnit
            // 
            this.lMsgsTimeUnit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lMsgsTimeUnit.AutoSize = true;
            this.lMsgsTimeUnit.Location = new System.Drawing.Point(221, 43);
            this.lMsgsTimeUnit.Name = "lMsgsTimeUnit";
            this.lMsgsTimeUnit.Size = new System.Drawing.Size(20, 13);
            this.lMsgsTimeUnit.TabIndex = 0;
            this.lMsgsTimeUnit.Text = "ms";
            // 
            // gbFog
            // 
            this.gbFog.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.gbFog.Controls.Add(this.cmbFogMode);
            this.gbFog.Controls.Add(this.lFogMode);
            this.gbFog.Location = new System.Drawing.Point(8, 168);
            this.gbFog.Name = "gbFog";
            this.gbFog.Size = new System.Drawing.Size(258, 85);
            this.gbFog.TabIndex = 2;
            this.gbFog.TabStop = false;
            this.gbFog.Text = "Fog";
            // 
            // cmbFogMode
            // 
            this.cmbFogMode.Items.AddRange(new object[] {
            "Depth pixel",
            "Depth vertex",
            "Range vertex"});
            this.cmbFogMode.Location = new System.Drawing.Point(10, 36);
            this.cmbFogMode.Name = "cmbFogMode";
            this.cmbFogMode.Size = new System.Drawing.Size(128, 21);
            this.cmbFogMode.TabIndex = 0;
            this.cmbFogMode.Text = "Depth pixel";
            this.cmbFogMode.SelectedIndexChanged += new System.EventHandler(this.cmbFogMode_SelectedIndexChanged);
            this.cmbFogMode.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lFogMode
            // 
            this.lFogMode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lFogMode.AutoSize = true;
            this.lFogMode.Location = new System.Drawing.Point(142, 39);
            this.lFogMode.Name = "lFogMode";
            this.lFogMode.Size = new System.Drawing.Size(54, 13);
            this.lFogMode.TabIndex = 0;
            this.lFogMode.Text = "Fog mode";
            // 
            // gbRendOpts
            // 
            this.gbRendOpts.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.gbRendOpts.Controls.Add(this.cbDisableShadersInMenu);
            this.gbRendOpts.Controls.Add(this.cbFog);
            this.gbRendOpts.Controls.Add(this.cbDisplayMessages);
            this.gbRendOpts.Controls.Add(this.cbFPSCounter);
            this.gbRendOpts.Controls.Add(this.cbAAFix);
            this.gbRendOpts.Controls.Add(this.cbHWShader);
            this.gbRendOpts.Location = new System.Drawing.Point(272, 95);
            this.gbRendOpts.Name = "gbRendOpts";
            this.gbRendOpts.Size = new System.Drawing.Size(258, 250);
            this.gbRendOpts.TabIndex = 3;
            this.gbRendOpts.TabStop = false;
            this.gbRendOpts.Text = "Options";
            // 
            // cbFog
            // 
            this.cbFog.AutoSize = true;
            this.cbFog.Location = new System.Drawing.Point(10, 32);
            this.cbFog.Name = "cbFog";
            this.cbFog.Size = new System.Drawing.Size(77, 17);
            this.cbFog.TabIndex = 0;
            this.cbFog.Text = "Enable fog";
            // 
            // cbDisplayMessages
            // 
            this.cbDisplayMessages.AutoSize = true;
            this.cbDisplayMessages.Location = new System.Drawing.Point(10, 78);
            this.cbDisplayMessages.Name = "cbDisplayMessages";
            this.cbDisplayMessages.Size = new System.Drawing.Size(110, 17);
            this.cbDisplayMessages.TabIndex = 2;
            this.cbDisplayMessages.Text = "Display messages";
            this.cbDisplayMessages.CheckedChanged += new System.EventHandler(this.cbDisplayMessages_CheckedChanged);
            // 
            // cbFPSCounter
            // 
            this.cbFPSCounter.AutoSize = true;
            this.cbFPSCounter.Location = new System.Drawing.Point(10, 55);
            this.cbFPSCounter.Name = "cbFPSCounter";
            this.cbFPSCounter.Size = new System.Drawing.Size(85, 17);
            this.cbFPSCounter.TabIndex = 1;
            this.cbFPSCounter.Text = "FPS counter";
            // 
            // cbAAFix
            // 
            this.cbAAFix.AutoSize = true;
            this.cbAAFix.Location = new System.Drawing.Point(10, 147);
            this.cbAAFix.Name = "cbAAFix";
            this.cbAAFix.Size = new System.Drawing.Size(85, 17);
            this.cbAAFix.TabIndex = 5;
            this.cbAAFix.Text = "AA colour fix";
            // 
            // cbHWShader
            // 
            this.cbHWShader.AutoSize = true;
            this.cbHWShader.Location = new System.Drawing.Point(10, 101);
            this.cbHWShader.Name = "cbHWShader";
            this.cbHWShader.Size = new System.Drawing.Size(107, 17);
            this.cbHWShader.TabIndex = 3;
            this.cbHWShader.Text = "Hardware shader";
            // 
            // gbScreen
            // 
            this.gbScreen.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.gbScreen.Controls.Add(this.udFOV);
            this.gbScreen.Controls.Add(this.udRotation);
            this.gbScreen.Controls.Add(this.udZoom);
            this.gbScreen.Controls.Add(this.lRotation);
            this.gbScreen.Controls.Add(this.lZoom);
            this.gbScreen.Controls.Add(this.lFOV);
            this.gbScreen.Location = new System.Drawing.Point(8, 95);
            this.gbScreen.Name = "gbScreen";
            this.gbScreen.Size = new System.Drawing.Size(258, 67);
            this.gbScreen.TabIndex = 1;
            this.gbScreen.TabStop = false;
            this.gbScreen.Text = "Screen";
            // 
            // udFOV
            // 
            this.udFOV.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.udFOV.DecimalPlaces = 2;
            this.udFOV.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udFOV.Location = new System.Drawing.Point(184, 36);
            this.udFOV.Maximum = new decimal(new int[] {
            150,
            0,
            0,
            0});
            this.udFOV.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udFOV.Name = "udFOV";
            this.udFOV.Size = new System.Drawing.Size(64, 20);
            this.udFOV.TabIndex = 2;
            this.udFOV.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udFOV.Value = new decimal(new int[] {
            75,
            0,
            0,
            0});
            // 
            // udRotation
            // 
            this.udRotation.DecimalPlaces = 2;
            this.udRotation.Increment = new decimal(new int[] {
            90,
            0,
            0,
            0});
            this.udRotation.Location = new System.Drawing.Point(10, 36);
            this.udRotation.Maximum = new decimal(new int[] {
            360000,
            0,
            0,
            0});
            this.udRotation.Minimum = new decimal(new int[] {
            360000,
            0,
            0,
            -2147483648});
            this.udRotation.Name = "udRotation";
            this.udRotation.Size = new System.Drawing.Size(64, 20);
            this.udRotation.TabIndex = 0;
            this.udRotation.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udRotation.ValueChanged += new System.EventHandler(this.udRotation_ValueChanged);
            // 
            // udZoom
            // 
            this.udZoom.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.udZoom.DecimalPlaces = 2;
            this.udZoom.Increment = new decimal(new int[] {
            5,
            0,
            0,
            65536});
            this.udZoom.Location = new System.Drawing.Point(97, 36);
            this.udZoom.Maximum = new decimal(new int[] {
            15,
            0,
            0,
            0});
            this.udZoom.Minimum = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.udZoom.Name = "udZoom";
            this.udZoom.Size = new System.Drawing.Size(64, 20);
            this.udZoom.TabIndex = 1;
            this.udZoom.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udZoom.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            // 
            // lRotation
            // 
            this.lRotation.AutoSize = true;
            this.lRotation.Location = new System.Drawing.Point(7, 22);
            this.lRotation.Name = "lRotation";
            this.lRotation.Size = new System.Drawing.Size(47, 13);
            this.lRotation.TabIndex = 0;
            this.lRotation.Text = "Rotation";
            // 
            // lZoom
            // 
            this.lZoom.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lZoom.AutoSize = true;
            this.lZoom.Location = new System.Drawing.Point(94, 22);
            this.lZoom.Name = "lZoom";
            this.lZoom.Size = new System.Drawing.Size(59, 13);
            this.lZoom.TabIndex = 0;
            this.lZoom.Text = "Initial zoom";
            // 
            // lFOV
            // 
            this.lFOV.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lFOV.AutoSize = true;
            this.lFOV.Location = new System.Drawing.Point(181, 22);
            this.lFOV.Name = "lFOV";
            this.lFOV.Size = new System.Drawing.Size(56, 13);
            this.lFOV.TabIndex = 0;
            this.lFOV.Text = "Horz. FOV";
            // 
            // gbTexFilt
            // 
            this.gbTexFilt.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbTexFilt.Controls.Add(this.cmbScaleFilter);
            this.gbTexFilt.Controls.Add(this.lScaleFilter);
            this.gbTexFilt.Controls.Add(this.cmbAnisoLevel);
            this.gbTexFilt.Controls.Add(this.lLOD);
            this.gbTexFilt.Controls.Add(this.udLOD);
            this.gbTexFilt.Controls.Add(this.lAnisoLevel);
            this.gbTexFilt.Controls.Add(this.cmbMipFilter);
            this.gbTexFilt.Controls.Add(this.lMipFilter);
            this.gbTexFilt.Location = new System.Drawing.Point(8, 3);
            this.gbTexFilt.Name = "gbTexFilt";
            this.gbTexFilt.Size = new System.Drawing.Size(522, 77);
            this.gbTexFilt.TabIndex = 0;
            this.gbTexFilt.TabStop = false;
            this.gbTexFilt.Text = "Texture filters";
            // 
            // cmbScaleFilter
            // 
            this.cmbScaleFilter.Items.AddRange(new object[] {
            "Point",
            "Linear",
            "Anisotropic",
            "Pyramidal quad",
            "Gaussian quad"});
            this.cmbScaleFilter.Location = new System.Drawing.Point(10, 21);
            this.cmbScaleFilter.Name = "cmbScaleFilter";
            this.cmbScaleFilter.Size = new System.Drawing.Size(128, 21);
            this.cmbScaleFilter.TabIndex = 0;
            this.cmbScaleFilter.Text = "Point";
            this.cmbScaleFilter.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lScaleFilter
            // 
            this.lScaleFilter.AutoSize = true;
            this.lScaleFilter.Location = new System.Drawing.Point(142, 24);
            this.lScaleFilter.Name = "lScaleFilter";
            this.lScaleFilter.Size = new System.Drawing.Size(64, 13);
            this.lScaleFilter.TabIndex = 0;
            this.lScaleFilter.Text = "Scaling filter";
            // 
            // cmbAnisoLevel
            // 
            this.cmbAnisoLevel.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cmbAnisoLevel.Items.AddRange(new object[] {
            "Off",
            "2x",
            "4x",
            "8x",
            "16x"});
            this.cmbAnisoLevel.Location = new System.Drawing.Point(273, 21);
            this.cmbAnisoLevel.Name = "cmbAnisoLevel";
            this.cmbAnisoLevel.Size = new System.Drawing.Size(48, 21);
            this.cmbAnisoLevel.TabIndex = 1;
            this.cmbAnisoLevel.Text = "Off";
            this.cmbAnisoLevel.SelectedIndexChanged += new System.EventHandler(this.cmbAnsiLevel_SelectedIndexChanged);
            this.cmbAnisoLevel.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lLOD
            // 
            this.lLOD.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lLOD.AutoSize = true;
            this.lLOD.Location = new System.Drawing.Point(325, 50);
            this.lLOD.Name = "lLOD";
            this.lLOD.Size = new System.Drawing.Size(91, 13);
            this.lLOD.TabIndex = 0;
            this.lLOD.Text = "Mipmap LOD bias";
            // 
            // udLOD
            // 
            this.udLOD.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.udLOD.Location = new System.Drawing.Point(273, 47);
            this.udLOD.Maximum = new decimal(new int[] {
            12,
            0,
            0,
            0});
            this.udLOD.Minimum = new decimal(new int[] {
            12,
            0,
            0,
            -2147483648});
            this.udLOD.Name = "udLOD";
            this.udLOD.Size = new System.Drawing.Size(48, 20);
            this.udLOD.TabIndex = 3;
            this.udLOD.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // lAnisoLevel
            // 
            this.lAnisoLevel.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lAnisoLevel.AutoSize = true;
            this.lAnisoLevel.Location = new System.Drawing.Point(325, 24);
            this.lAnisoLevel.Name = "lAnisoLevel";
            this.lAnisoLevel.Size = new System.Drawing.Size(120, 13);
            this.lAnisoLevel.TabIndex = 0;
            this.lAnisoLevel.Text = "Anisotropic filtering level";
            // 
            // cmbMipFilter
            // 
            this.cmbMipFilter.Items.AddRange(new object[] {
            "None",
            "Point",
            "Linear"});
            this.cmbMipFilter.Location = new System.Drawing.Point(10, 47);
            this.cmbMipFilter.Name = "cmbMipFilter";
            this.cmbMipFilter.Size = new System.Drawing.Size(128, 21);
            this.cmbMipFilter.TabIndex = 2;
            this.cmbMipFilter.Text = "Point";
            this.cmbMipFilter.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lMipFilter
            // 
            this.lMipFilter.AutoSize = true;
            this.lMipFilter.Location = new System.Drawing.Point(142, 50);
            this.lMipFilter.Name = "lMipFilter";
            this.lMipFilter.Size = new System.Drawing.Size(66, 13);
            this.lMipFilter.TabIndex = 0;
            this.lMipFilter.Text = "Mipmap filter";
            // 
            // lSShotFormat
            // 
            this.lSShotFormat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lSShotFormat.Location = new System.Drawing.Point(248, 21);
            this.lSShotFormat.Name = "lSShotFormat";
            this.lSShotFormat.Size = new System.Drawing.Size(60, 21);
            this.lSShotFormat.TabIndex = 0;
            this.lSShotFormat.Text = "Format";
            this.lSShotFormat.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // cmbSShotFormat
            // 
            this.cmbSShotFormat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbSShotFormat.Items.AddRange(new object[] {
            "BMP",
            "JPEG",
            "DDS",
            "PNG",
            "TGA"});
            this.cmbSShotFormat.Location = new System.Drawing.Point(310, 21);
            this.cmbSShotFormat.Name = "cmbSShotFormat";
            this.cmbSShotFormat.Size = new System.Drawing.Size(56, 21);
            this.cmbSShotFormat.TabIndex = 1;
            this.cmbSShotFormat.Text = "BMP";
            // 
            // tpGlobal
            // 
            this.tpGlobal.Controls.Add(this.gbSShot);
            this.tpGlobal.Controls.Add(this.gbScene);
            this.tpGlobal.Controls.Add(this.gbDisplay);
            this.tpGlobal.Location = new System.Drawing.Point(4, 22);
            this.tpGlobal.Name = "tpGlobal";
            this.tpGlobal.Size = new System.Drawing.Size(538, 353);
            this.tpGlobal.TabIndex = 1;
            this.tpGlobal.Text = "Global graphics";
            this.tpGlobal.ToolTipText = "Contains general graphics settings like screen size,\nrefresh rate, anti-aliasing," +
                " buffer type.";
            this.tpGlobal.UseVisualStyleBackColor = true;
            // 
            // gbSShot
            // 
            this.gbSShot.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbSShot.Controls.Add(this.udSShotNum);
            this.gbSShot.Controls.Add(this.bSShotDirClear);
            this.gbSShot.Controls.Add(this.bSShotDirBrowse);
            this.gbSShot.Controls.Add(this.tbSShotName);
            this.gbSShot.Controls.Add(this.tbSShotDir);
            this.gbSShot.Controls.Add(this.cmbSShotFormat);
            this.gbSShot.Controls.Add(this.lSShotName);
            this.gbSShot.Controls.Add(this.lSShotDir);
            this.gbSShot.Controls.Add(this.lSShotFormat);
            this.gbSShot.Controls.Add(this.lSShotNum);
            this.gbSShot.Location = new System.Drawing.Point(8, 263);
            this.gbSShot.Name = "gbSShot";
            this.gbSShot.Size = new System.Drawing.Size(522, 82);
            this.gbSShot.TabIndex = 2;
            this.gbSShot.TabStop = false;
            this.gbSShot.Text = "Screenshots";
            // 
            // udSShotNum
            // 
            this.udSShotNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.udSShotNum.Location = new System.Drawing.Point(484, 21);
            this.udSShotNum.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udSShotNum.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udSShotNum.Name = "udSShotNum";
            this.udSShotNum.Size = new System.Drawing.Size(32, 20);
            this.udSShotNum.TabIndex = 2;
            this.udSShotNum.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udSShotNum.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // bSShotDirClear
            // 
            this.bSShotDirClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bSShotDirClear.Location = new System.Drawing.Point(446, 51);
            this.bSShotDirClear.Name = "bSShotDirClear";
            this.bSShotDirClear.Size = new System.Drawing.Size(70, 22);
            this.bSShotDirClear.TabIndex = 5;
            this.bSShotDirClear.Text = "Clear";
            this.bSShotDirClear.UseVisualStyleBackColor = true;
            this.bSShotDirClear.Click += new System.EventHandler(this.bSSDirClear_Click);
            // 
            // bSShotDirBrowse
            // 
            this.bSShotDirBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bSShotDirBrowse.Location = new System.Drawing.Point(372, 51);
            this.bSShotDirBrowse.Name = "bSShotDirBrowse";
            this.bSShotDirBrowse.Size = new System.Drawing.Size(70, 22);
            this.bSShotDirBrowse.TabIndex = 4;
            this.bSShotDirBrowse.Text = "Browse";
            this.bSShotDirBrowse.UseVisualStyleBackColor = true;
            this.bSShotDirBrowse.Click += new System.EventHandler(this.bSShotDirBrowse_Click);
            // 
            // tbSShotName
            // 
            this.tbSShotName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSShotName.Location = new System.Drawing.Point(110, 21);
            this.tbSShotName.Name = "tbSShotName";
            this.tbSShotName.Size = new System.Drawing.Size(136, 20);
            this.tbSShotName.TabIndex = 0;
            this.tbSShotName.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.fileNameFilter_KeyPress);
            // 
            // tbSShotDir
            // 
            this.tbSShotDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSShotDir.Location = new System.Drawing.Point(110, 52);
            this.tbSShotDir.Name = "tbSShotDir";
            this.tbSShotDir.ReadOnly = true;
            this.tbSShotDir.Size = new System.Drawing.Size(256, 20);
            this.tbSShotDir.TabIndex = 3;
            this.tbSShotDir.Text = "- Morrowind folder -";
            this.tbSShotDir.TextAlignChanged += new System.EventHandler(this.tbSSDir_TextAlignChanged);
            // 
            // lSShotName
            // 
            this.lSShotName.Location = new System.Drawing.Point(4, 21);
            this.lSShotName.Name = "lSShotName";
            this.lSShotName.Size = new System.Drawing.Size(104, 21);
            this.lSShotName.TabIndex = 0;
            this.lSShotName.Text = "File name prefix";
            this.lSShotName.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lSShotDir
            // 
            this.lSShotDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lSShotDir.Location = new System.Drawing.Point(4, 51);
            this.lSShotDir.Name = "lSShotDir";
            this.lSShotDir.Size = new System.Drawing.Size(104, 21);
            this.lSShotDir.TabIndex = 0;
            this.lSShotDir.Text = "Output directory";
            this.lSShotDir.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lSShotNum
            // 
            this.lSShotNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lSShotNum.Location = new System.Drawing.Point(372, 21);
            this.lSShotNum.Name = "lSShotNum";
            this.lSShotNum.Size = new System.Drawing.Size(110, 21);
            this.lSShotNum.TabIndex = 0;
            this.lSShotNum.Text = "Num. chars min";
            this.lSShotNum.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // gbScene
            // 
            this.gbScene.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.gbScene.Controls.Add(this.cmbAntiAlias);
            this.gbScene.Controls.Add(this.cmbVWait);
            this.gbScene.Controls.Add(this.cmbZBuffer);
            this.gbScene.Controls.Add(this.lZBuffer);
            this.gbScene.Controls.Add(this.lVWait);
            this.gbScene.Controls.Add(this.lAntiAlias);
            this.gbScene.Location = new System.Drawing.Point(8, 141);
            this.gbScene.Name = "gbScene";
            this.gbScene.Size = new System.Drawing.Size(522, 64);
            this.gbScene.TabIndex = 1;
            this.gbScene.TabStop = false;
            this.gbScene.Text = "Scene";
            // 
            // cmbAntiAlias
            // 
            this.cmbAntiAlias.ContextMenu = this.DudMenu;
            this.cmbAntiAlias.Items.AddRange(new object[] {
            "None",
            "2x",
            "3x",
            "4x",
            "5x",
            "6x",
            "7x",
            "8x",
            "9x",
            "10x",
            "11x",
            "12x",
            "13x",
            "14x",
            "15x",
            "16x"});
            this.cmbAntiAlias.Location = new System.Drawing.Point(6, 34);
            this.cmbAntiAlias.Name = "cmbAntiAlias";
            this.cmbAntiAlias.Size = new System.Drawing.Size(128, 21);
            this.cmbAntiAlias.TabIndex = 0;
            this.cmbAntiAlias.Text = "None";
            this.cmbAntiAlias.SelectedIndexChanged += new System.EventHandler(this.cmbAlias_SelectedIndexChanged);
            this.cmbAntiAlias.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // cmbVWait
            // 
            this.cmbVWait.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbVWait.Items.AddRange(new object[] {
            "Immediate",
            "1",
            "2",
            "3",
            "4",
            "Default (1)"});
            this.cmbVWait.Location = new System.Drawing.Point(388, 34);
            this.cmbVWait.Name = "cmbVWait";
            this.cmbVWait.Size = new System.Drawing.Size(128, 21);
            this.cmbVWait.TabIndex = 2;
            this.cmbVWait.Text = "Immediate";
            this.cmbVWait.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // cmbZBuffer
            // 
            this.cmbZBuffer.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cmbZBuffer.ContextMenu = this.DudMenu;
            this.cmbZBuffer.Items.AddRange(new object[] {
            "D24S8",
            "D24X8",
            "D32",
            "D16",
            "D16L",
            "D32FL",
            "INTZ",
            "RAWZ",
            "DF16",
            "DF24"});
            this.cmbZBuffer.Location = new System.Drawing.Point(197, 34);
            this.cmbZBuffer.Name = "cmbZBuffer";
            this.cmbZBuffer.Size = new System.Drawing.Size(128, 21);
            this.cmbZBuffer.TabIndex = 1;
            this.cmbZBuffer.Text = "D24S8";
            this.cmbZBuffer.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lZBuffer
            // 
            this.lZBuffer.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.lZBuffer.AutoSize = true;
            this.lZBuffer.Location = new System.Drawing.Point(194, 18);
            this.lZBuffer.Name = "lZBuffer";
            this.lZBuffer.Size = new System.Drawing.Size(76, 13);
            this.lZBuffer.TabIndex = 0;
            this.lZBuffer.Text = "Z-buffer format";
            // 
            // lVWait
            // 
            this.lVWait.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lVWait.AutoSize = true;
            this.lVWait.Location = new System.Drawing.Point(385, 18);
            this.lVWait.Name = "lVWait";
            this.lVWait.Size = new System.Drawing.Size(70, 13);
            this.lVWait.TabIndex = 0;
            this.lVWait.Text = "VWait setting";
            // 
            // lAntiAlias
            // 
            this.lAntiAlias.AutoSize = true;
            this.lAntiAlias.Location = new System.Drawing.Point(3, 18);
            this.lAntiAlias.Name = "lAntiAlias";
            this.lAntiAlias.Size = new System.Drawing.Size(85, 13);
            this.lAntiAlias.TabIndex = 0;
            this.lAntiAlias.Text = "Antialiasing level";
            // 
            // gbDisplay
            // 
            this.gbDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbDisplay.Controls.Add(this.tbAspect);
            this.gbDisplay.Controls.Add(this.bCalcResolution);
            this.gbDisplay.Controls.Add(this.cbWindowed);
            this.gbDisplay.Controls.Add(this.cmbRefreshRate);
            this.gbDisplay.Controls.Add(this.bCalcRefresh);
            this.gbDisplay.Controls.Add(this.tbResolution);
            this.gbDisplay.Controls.Add(this.lResolution);
            this.gbDisplay.Controls.Add(this.lAspect);
            this.gbDisplay.Controls.Add(this.lRefreshRate);
            this.gbDisplay.Location = new System.Drawing.Point(8, 3);
            this.gbDisplay.Name = "gbDisplay";
            this.gbDisplay.Size = new System.Drawing.Size(522, 80);
            this.gbDisplay.TabIndex = 0;
            this.gbDisplay.TabStop = false;
            this.gbDisplay.Text = "Display";
            // 
            // tbAspect
            // 
            this.tbAspect.Location = new System.Drawing.Point(182, 22);
            this.tbAspect.Name = "tbAspect";
            this.tbAspect.ReadOnly = true;
            this.tbAspect.Size = new System.Drawing.Size(36, 20);
            this.tbAspect.TabIndex = 2;
            this.tbAspect.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // bCalcResolution
            // 
            this.bCalcResolution.Location = new System.Drawing.Point(6, 49);
            this.bCalcResolution.Name = "bCalcResolution";
            this.bCalcResolution.Size = new System.Drawing.Size(170, 23);
            this.bCalcResolution.TabIndex = 1;
            this.bCalcResolution.Text = "Change screen resolution";
            this.bCalcResolution.Click += new System.EventHandler(this.bCalcResolution_Click);
            // 
            // cbWindowed
            // 
            this.cbWindowed.AutoSize = true;
            this.cbWindowed.Location = new System.Drawing.Point(182, 53);
            this.cbWindowed.Name = "cbWindowed";
            this.cbWindowed.Size = new System.Drawing.Size(106, 17);
            this.cbWindowed.TabIndex = 3;
            this.cbWindowed.Text = "Windowed mode";
            this.cbWindowed.CheckedChanged += new System.EventHandler(this.cbWindowed_CheckedChanged);
            // 
            // cmbRefreshRate
            // 
            this.cmbRefreshRate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbRefreshRate.ItemHeight = 13;
            this.cmbRefreshRate.Items.AddRange(new object[] {
            "Default"});
            this.cmbRefreshRate.Location = new System.Drawing.Point(444, 21);
            this.cmbRefreshRate.Name = "cmbRefreshRate";
            this.cmbRefreshRate.Size = new System.Drawing.Size(72, 21);
            this.cmbRefreshRate.TabIndex = 3;
            this.cmbRefreshRate.Text = "Default";
            // 
            // bCalcRefresh
            // 
            this.bCalcRefresh.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bCalcRefresh.Location = new System.Drawing.Point(316, 49);
            this.bCalcRefresh.Name = "bCalcRefresh";
            this.bCalcRefresh.Size = new System.Drawing.Size(200, 23);
            this.bCalcRefresh.TabIndex = 5;
            this.bCalcRefresh.Text = "Find valid refresh rates";
            this.bCalcRefresh.Click += new System.EventHandler(this.bCalcRefresh_Click);
            // 
            // tbResolution
            // 
            this.tbResolution.Location = new System.Drawing.Point(96, 22);
            this.tbResolution.Name = "tbResolution";
            this.tbResolution.ReadOnly = true;
            this.tbResolution.Size = new System.Drawing.Size(80, 20);
            this.tbResolution.TabIndex = 0;
            // 
            // lResolution
            // 
            this.lResolution.Location = new System.Drawing.Point(10, 21);
            this.lResolution.Name = "lResolution";
            this.lResolution.Size = new System.Drawing.Size(83, 21);
            this.lResolution.TabIndex = 0;
            this.lResolution.Text = "Resolution";
            this.lResolution.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lAspect
            // 
            this.lAspect.AutoSize = true;
            this.lAspect.Location = new System.Drawing.Point(221, 24);
            this.lAspect.Name = "lAspect";
            this.lAspect.Size = new System.Drawing.Size(63, 13);
            this.lAspect.TabIndex = 0;
            this.lAspect.Text = "Aspect ratio";
            this.lAspect.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lRefreshRate
            // 
            this.lRefreshRate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lRefreshRate.Location = new System.Drawing.Point(316, 21);
            this.lRefreshRate.Name = "lRefreshRate";
            this.lRefreshRate.Size = new System.Drawing.Size(126, 21);
            this.lRefreshRate.TabIndex = 0;
            this.lRefreshRate.Text = "Refresh rate";
            this.lRefreshRate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tpMain);
            this.tabControl.Controls.Add(this.tpGlobal);
            this.tabControl.Controls.Add(this.tpRender);
            this.tabControl.Controls.Add(this.tpInput);
            this.tabControl.Controls.Add(this.tpMisc);
            this.tabControl.Controls.Add(this.tpINI);
            this.tabControl.Controls.Add(this.tpTools);
            this.tabControl.Controls.Add(this.tpDL);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.ShowToolTips = true;
            this.tabControl.Size = new System.Drawing.Size(546, 379);
            this.tabControl.TabIndex = 0;
            // 
            // tpMain
            // 
            this.tpMain.Controls.Add(this.gbMainUI);
            this.tpMain.Controls.Add(this.cbDisableMGE);
            this.tpMain.Controls.Add(this.gbMainInfo);
            this.tpMain.Controls.Add(this.gbMainSettings);
            this.tpMain.Controls.Add(this.gbMainQuality);
            this.tpMain.Location = new System.Drawing.Point(4, 22);
            this.tpMain.Name = "tpMain";
            this.tpMain.Size = new System.Drawing.Size(538, 353);
            this.tpMain.TabIndex = 0;
            this.tpMain.Text = "Main";
            this.tpMain.ToolTipText = "Contains general settings, and shortcuts\r\nto change many settings with a single c" +
                "lick.";
            this.tpMain.UseVisualStyleBackColor = true;
            // 
            // gbMainUI
            // 
            this.gbMainUI.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMainUI.Controls.Add(this.panel1);
            this.gbMainUI.Controls.Add(this.cmbTipReadSpd);
            this.gbMainUI.Controls.Add(this.lTipReadSpd);
            this.gbMainUI.Controls.Add(this.lTipRSpdUnit);
            this.gbMainUI.Location = new System.Drawing.Point(8, 167);
            this.gbMainUI.Name = "gbMainUI";
            this.gbMainUI.Size = new System.Drawing.Size(522, 76);
            this.gbMainUI.TabIndex = 2;
            this.gbMainUI.TabStop = false;
            this.gbMainUI.Text = "User Interface";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.cmbUILanguage);
            this.panel1.Controls.Add(this.cbUILangAuto);
            this.panel1.Controls.Add(this.lUILanguage);
            this.panel1.Location = new System.Drawing.Point(8, 25);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(156, 37);
            this.panel1.TabIndex = 0;
            // 
            // cmbUILanguage
            // 
            this.cmbUILanguage.FormattingEnabled = true;
            this.cmbUILanguage.Items.AddRange(new object[] { MGEgui.Localization.LocalizationInterface.DefaultLanguage });
            this.cmbUILanguage.Location = new System.Drawing.Point(0, 16);
            this.cmbUILanguage.Name = "cmbUILanguage";
            this.cmbUILanguage.Size = new System.Drawing.Size(156, 21);
            this.cmbUILanguage.TabIndex = 1;
            this.cmbUILanguage.Text = MGEgui.Localization.LocalizationInterface.DefaultLanguage;
            this.cmbUILanguage.SelectedIndexChanged += new System.EventHandler(this.cmbUILanguage_SelectedIndexChanged);
            this.cmbUILanguage.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // cbUILangAuto
            // 
            this.cbUILangAuto.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbUILangAuto.AutoSize = true;
            this.cbUILangAuto.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbUILangAuto.Location = new System.Drawing.Point(78, -1);
            this.cbUILangAuto.Name = "cbUILangAuto";
            this.cbUILangAuto.Size = new System.Drawing.Size(78, 17);
            this.cbUILangAuto.TabIndex = 0;
            this.cbUILangAuto.Text = "Autodetect";
            this.cbUILangAuto.UseVisualStyleBackColor = true;
            this.cbUILangAuto.CheckedChanged += new System.EventHandler(this.cbUILangAuto_CheckedChanged);
            // 
            // lUILanguage
            // 
            this.lUILanguage.AutoSize = true;
            this.lUILanguage.Location = new System.Drawing.Point(-2, 0);
            this.lUILanguage.Name = "lUILanguage";
            this.lUILanguage.Size = new System.Drawing.Size(55, 13);
            this.lUILanguage.TabIndex = 0;
            this.lUILanguage.Text = "Language";
            // 
            // cmbTipReadSpd
            // 
            this.cmbTipReadSpd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbTipReadSpd.FormattingEnabled = true;
            this.cmbTipReadSpd.Items.AddRange(new object[] {
            "5",
            "6.25",
            "7.5",
            "8.75",
            "10",
            "12.5",
            "15",
            "17.5",
            "20",
            "25",
            "30",
            "35",
            "40",
            "50",
            "60",
            "70"});
            this.cmbTipReadSpd.Location = new System.Drawing.Point(358, 41);
            this.cmbTipReadSpd.Name = "cmbTipReadSpd";
            this.cmbTipReadSpd.Size = new System.Drawing.Size(48, 21);
            this.cmbTipReadSpd.TabIndex = 2;
            this.cmbTipReadSpd.Text = "15";
            this.cmbTipReadSpd.SelectedIndexChanged += new System.EventHandler(this.cmbMainTipReadSpd_SelectedIndexChanged);
            this.cmbTipReadSpd.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKey);
            // 
            // lTipReadSpd
            // 
            this.lTipReadSpd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lTipReadSpd.AutoSize = true;
            this.lTipReadSpd.Location = new System.Drawing.Point(355, 25);
            this.lTipReadSpd.Name = "lTipReadSpd";
            this.lTipReadSpd.Size = new System.Drawing.Size(109, 13);
            this.lTipReadSpd.TabIndex = 0;
            this.lTipReadSpd.Text = "Tooltip reading speed";
            // 
            // lTipRSpdUnit
            // 
            this.lTipRSpdUnit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lTipRSpdUnit.AutoSize = true;
            this.lTipRSpdUnit.Location = new System.Drawing.Point(409, 44);
            this.lTipRSpdUnit.Name = "lTipRSpdUnit";
            this.lTipRSpdUnit.Size = new System.Drawing.Size(58, 13);
            this.lTipRSpdUnit.TabIndex = 0;
            this.lTipRSpdUnit.Text = "chars/sec.";
            // 
            // cbDisableMGE
            // 
            this.cbDisableMGE.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cbDisableMGE.AutoSize = true;
            this.cbDisableMGE.Location = new System.Drawing.Point(10, 331);
            this.cbDisableMGE.Name = "cbDisableMGE";
            this.cbDisableMGE.Size = new System.Drawing.Size(128, 17);
            this.cbDisableMGE.TabIndex = 4;
            this.cbDisableMGE.Text = "Disable MGE in game";
            this.cbDisableMGE.UseVisualStyleBackColor = true;
            this.cbDisableMGE.CheckedChanged += new System.EventHandler(this.cbDisableMGE_CheckedChanged);
            // 
            // gbMainInfo
            // 
            this.gbMainInfo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMainInfo.Controls.Add(this.bViewCaps);
            this.gbMainInfo.Controls.Add(this.bHelp);
            this.gbMainInfo.Controls.Add(this.bAbout);
            this.gbMainInfo.Location = new System.Drawing.Point(8, 249);
            this.gbMainInfo.Name = "gbMainInfo";
            this.gbMainInfo.Size = new System.Drawing.Size(522, 76);
            this.gbMainInfo.TabIndex = 3;
            this.gbMainInfo.TabStop = false;
            this.gbMainInfo.Text = "Information";
            // 
            // bViewCaps
            // 
            this.bViewCaps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bViewCaps.Location = new System.Drawing.Point(6, 44);
            this.bViewCaps.Name = "bViewCaps";
            this.bViewCaps.Size = new System.Drawing.Size(160, 25);
            this.bViewCaps.TabIndex = 0;
            this.bViewCaps.Text = "View max AA and AF";
            this.bViewCaps.Click += new System.EventHandler(this.bViewCaps_Click);
            // 
            // bHelp
            // 
            this.bHelp.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.bHelp.Location = new System.Drawing.Point(181, 44);
            this.bHelp.Name = "bHelp";
            this.bHelp.Size = new System.Drawing.Size(160, 25);
            this.bHelp.TabIndex = 1;
            this.bHelp.Text = "Help";
            this.bHelp.Click += new System.EventHandler(this.bHelp_Click);
            // 
            // bAbout
            // 
            this.bAbout.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bAbout.Location = new System.Drawing.Point(356, 44);
            this.bAbout.Name = "bAbout";
            this.bAbout.Size = new System.Drawing.Size(160, 25);
            this.bAbout.TabIndex = 2;
            this.bAbout.Text = "About";
            this.bAbout.Click += new System.EventHandler(this.bAbout_Click);
            // 
            // gbMainSettings
            // 
            this.gbMainSettings.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMainSettings.Controls.Add(this.bResetSettings);
            this.gbMainSettings.Controls.Add(this.bImport);
            this.gbMainSettings.Controls.Add(this.bExport);
            this.gbMainSettings.Controls.Add(this.cbImportIni);
            this.gbMainSettings.Controls.Add(this.cbClearINI);
            this.gbMainSettings.Location = new System.Drawing.Point(8, 85);
            this.gbMainSettings.Name = "gbMainSettings";
            this.gbMainSettings.Size = new System.Drawing.Size(522, 76);
            this.gbMainSettings.TabIndex = 1;
            this.gbMainSettings.TabStop = false;
            this.gbMainSettings.Text = "Settings";
            // 
            // bResetSettings
            // 
            this.bResetSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bResetSettings.Location = new System.Drawing.Point(6, 44);
            this.bResetSettings.Name = "bResetSettings";
            this.bResetSettings.Size = new System.Drawing.Size(160, 25);
            this.bResetSettings.TabIndex = 1;
            this.bResetSettings.Text = "Reset settings";
            this.bResetSettings.Click += new System.EventHandler(this.bResetSettings_Click);
            // 
            // bImport
            // 
            this.bImport.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.bImport.Location = new System.Drawing.Point(181, 44);
            this.bImport.Name = "bImport";
            this.bImport.Size = new System.Drawing.Size(160, 25);
            this.bImport.TabIndex = 3;
            this.bImport.Text = "Import settings";
            this.bImport.Click += new System.EventHandler(this.bImport_Click);
            // 
            // bExport
            // 
            this.bExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bExport.Location = new System.Drawing.Point(356, 44);
            this.bExport.Name = "bExport";
            this.bExport.Size = new System.Drawing.Size(160, 25);
            this.bExport.TabIndex = 4;
            this.bExport.Text = "Export settings";
            this.bExport.Click += new System.EventHandler(this.bExport_Click);
            // 
            // cbImportIni
            // 
            this.cbImportIni.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.cbImportIni.AutoSize = true;
            this.cbImportIni.Location = new System.Drawing.Point(183, 24);
            this.cbImportIni.Name = "cbImportIni";
            this.cbImportIni.Size = new System.Drawing.Size(120, 17);
            this.cbImportIni.TabIndex = 2;
            this.cbImportIni.Text = "Import Morrowind.ini";
            // 
            // cbClearINI
            // 
            this.cbClearINI.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cbClearINI.AutoSize = true;
            this.cbClearINI.Location = new System.Drawing.Point(8, 24);
            this.cbClearINI.Name = "cbClearINI";
            this.cbClearINI.Size = new System.Drawing.Size(121, 17);
            this.cbClearINI.TabIndex = 0;
            this.cbClearINI.Text = "Clear saved settings";
            this.cbClearINI.UseVisualStyleBackColor = true;
            // 
            // gbMainQuality
            // 
            this.gbMainQuality.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMainQuality.Controls.Add(this.bQuality);
            this.gbMainQuality.Location = new System.Drawing.Point(8, 3);
            this.gbMainQuality.Name = "gbMainQuality";
            this.gbMainQuality.Size = new System.Drawing.Size(522, 76);
            this.gbMainQuality.TabIndex = 0;
            this.gbMainQuality.TabStop = false;
            this.gbMainQuality.Text = "Quality";
            // 
            // bQuality
            // 
            this.bQuality.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bQuality.Location = new System.Drawing.Point(356, 44);
            this.bQuality.Name = "bQuality";
            this.bQuality.Size = new System.Drawing.Size(160, 25);
            this.bQuality.TabIndex = 0;
            this.bQuality.Text = "Best quality";
            this.bQuality.Click += new System.EventHandler(this.bQuality_Click);
            // 
            // tpDL
            // 
            this.tpDL.Controls.Add(this.cbDLDistantLand);
            this.tpDL.Controls.Add(this.gbDistantLand);
            this.tpDL.Location = new System.Drawing.Point(4, 22);
            this.tpDL.Name = "tpDL";
            this.tpDL.Size = new System.Drawing.Size(538, 353);
            this.tpDL.TabIndex = 7;
            this.tpDL.Text = "Distant Land";
            this.tpDL.ToolTipText = "This tab controls whether and how distant land,\ndistant statics, animated grass, " +
                "reflective water\nare rendered, and also contains weather settings\nfor distant la" +
                "nd wind speed and fog range.";
            this.tpDL.UseVisualStyleBackColor = true;
            // 
            // cbDLDistantLand
            // 
            this.cbDLDistantLand.AutoSize = true;
            this.cbDLDistantLand.Location = new System.Drawing.Point(16, 1);
            this.cbDLDistantLand.Name = "cbDLDistantLand";
            this.cbDLDistantLand.Size = new System.Drawing.Size(108, 17);
            this.cbDLDistantLand.TabIndex = 0;
            this.cbDLDistantLand.Text = "Use Distant &Land";
            this.cbDLDistantLand.UseVisualStyleBackColor = true;
            this.cbDLDistantLand.CheckedChanged += new System.EventHandler(this.cbDistantLand_CheckedChanged);
            // 
            // gbDistantLand
            // 
            this.gbDistantLand.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbDistantLand.Controls.Add(this.pDLStatics);
            this.gbDistantLand.Controls.Add(this.cbDLAutoDist);
            this.gbDistantLand.Controls.Add(this.gbDLAutoDist);
            this.gbDistantLand.Controls.Add(this.gbDLSM);
            this.gbDistantLand.Controls.Add(this.gbDLWater);
            this.gbDistantLand.Controls.Add(this.bDLWthr);
            this.gbDistantLand.Controls.Add(this.gbDLDrawDist);
            this.gbDistantLand.Controls.Add(this.bDLOptions);
            this.gbDistantLand.Controls.Add(this.gbDLFog);
            this.gbDistantLand.Enabled = false;
            this.gbDistantLand.Location = new System.Drawing.Point(8, 3);
            this.gbDistantLand.Name = "gbDistantLand";
            this.gbDistantLand.Size = new System.Drawing.Size(522, 342);
            this.gbDistantLand.TabIndex = 1;
            this.gbDistantLand.TabStop = false;
            // 
            // pDLStatics
            // 
            this.pDLStatics.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pDLStatics.Controls.Add(this.cbDLDistantStatics);
            this.pDLStatics.Controls.Add(this.gbDLStatics);
            this.pDLStatics.Location = new System.Drawing.Point(278, 19);
            this.pDLStatics.Name = "pDLStatics";
            this.pDLStatics.Size = new System.Drawing.Size(238, 124);
            this.pDLStatics.TabIndex = 5;
            this.pDLStatics.TabStop = true;
            // 
            // cbDLDistantStatics
            // 
            this.cbDLDistantStatics.AutoSize = true;
            this.cbDLDistantStatics.Location = new System.Drawing.Point(8, -2);
            this.cbDLDistantStatics.Name = "cbDLDistantStatics";
            this.cbDLDistantStatics.Size = new System.Drawing.Size(116, 17);
            this.cbDLDistantStatics.TabIndex = 0;
            this.cbDLDistantStatics.Text = "Use Distant &Statics";
            this.cbDLDistantStatics.UseVisualStyleBackColor = true;
            this.cbDLDistantStatics.CheckedChanged += new System.EventHandler(this.cbDistantStatics_CheckedChanged);
            // 
            // gbDLStatics
            // 
            this.gbDLStatics.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbDLStatics.Controls.Add(this.cbDLDistantStaticsScript);
            this.gbDLStatics.Controls.Add(this.udDLDistVeryFar);
            this.gbDLStatics.Controls.Add(this.udDLDistFar);
            this.gbDLStatics.Controls.Add(this.udDLDistNear);
            this.gbDLStatics.Controls.Add(this.udDLSizeVeryFar);
            this.gbDLStatics.Controls.Add(this.udDLSizeFar);
            this.gbDLStatics.Controls.Add(this.tbDLNearSize);
            this.gbDLStatics.Controls.Add(this.lDLVeryFar);
            this.gbDLStatics.Controls.Add(this.lDLFar);
            this.gbDLStatics.Controls.Add(this.lDLNear);
            this.gbDLStatics.Controls.Add(this.lDLMinSize);
            this.gbDLStatics.Controls.Add(this.lDLEndDist);
            this.gbDLStatics.Enabled = false;
            this.gbDLStatics.Location = new System.Drawing.Point(0, 0);
            this.gbDLStatics.Name = "gbDLStatics";
            this.gbDLStatics.Size = new System.Drawing.Size(238, 124);
            this.gbDLStatics.TabIndex = 1;
            this.gbDLStatics.TabStop = false;
            // 
            // cbDLDistantStaticsScript
            // 
            this.cbDLDistantStaticsScript.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbDLDistantStaticsScript.Enabled = false;
            this.cbDLDistantStaticsScript.Location = new System.Drawing.Point(20, 102);
            this.cbDLDistantStaticsScript.Name = "cbDLDistantStaticsScript";
            this.cbDLDistantStaticsScript.Size = new System.Drawing.Size(212, 18);
            this.cbDLDistantStaticsScript.TabIndex = 6;
            this.cbDLDistantStaticsScript.Text = "Reserved for future\r\n\r\nAllow script controlling";
            this.cbDLDistantStaticsScript.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbDLDistantStaticsScript.UseVisualStyleBackColor = true;
            this.cbDLDistantStaticsScript.Visible = false;
            // 
            // udDLDistVeryFar
            // 
            this.udDLDistVeryFar.DecimalPlaces = 1;
            this.udDLDistVeryFar.Location = new System.Drawing.Point(182, 78);
            this.udDLDistVeryFar.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.udDLDistVeryFar.Name = "udDLDistVeryFar";
            this.udDLDistVeryFar.Size = new System.Drawing.Size(50, 20);
            this.udDLDistVeryFar.TabIndex = 5;
            this.udDLDistVeryFar.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLDistVeryFar.Value = new decimal(new int[] {
            12,
            0,
            0,
            0});
            this.udDLDistVeryFar.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLDistFar
            // 
            this.udDLDistFar.DecimalPlaces = 1;
            this.udDLDistFar.Location = new System.Drawing.Point(182, 54);
            this.udDLDistFar.Maximum = new decimal(new int[] {
            2999,
            0,
            0,
            65536});
            this.udDLDistFar.Name = "udDLDistFar";
            this.udDLDistFar.Size = new System.Drawing.Size(50, 20);
            this.udDLDistFar.TabIndex = 3;
            this.udDLDistFar.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLDistFar.Value = new decimal(new int[] {
            6,
            0,
            0,
            0});
            this.udDLDistFar.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLDistNear
            // 
            this.udDLDistNear.DecimalPlaces = 1;
            this.udDLDistNear.Location = new System.Drawing.Point(182, 30);
            this.udDLDistNear.Maximum = new decimal(new int[] {
            2998,
            0,
            0,
            65536});
            this.udDLDistNear.Name = "udDLDistNear";
            this.udDLDistNear.Size = new System.Drawing.Size(50, 20);
            this.udDLDistNear.TabIndex = 1;
            this.udDLDistNear.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLDistNear.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.udDLDistNear.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLSizeVeryFar
            // 
            this.udDLSizeVeryFar.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.udDLSizeVeryFar.Location = new System.Drawing.Point(124, 78);
            this.udDLSizeVeryFar.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.udDLSizeVeryFar.Name = "udDLSizeVeryFar";
            this.udDLSizeVeryFar.Size = new System.Drawing.Size(50, 20);
            this.udDLSizeVeryFar.TabIndex = 4;
            this.udDLSizeVeryFar.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLSizeVeryFar.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udDLSizeVeryFar.Value = new decimal(new int[] {
            800,
            0,
            0,
            0});
            this.udDLSizeVeryFar.ValueChanged += new System.EventHandler(this.ValidateDistances);
            // 
            // udDLSizeFar
            // 
            this.udDLSizeFar.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.udDLSizeFar.Location = new System.Drawing.Point(124, 54);
            this.udDLSizeFar.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.udDLSizeFar.Name = "udDLSizeFar";
            this.udDLSizeFar.Size = new System.Drawing.Size(50, 20);
            this.udDLSizeFar.TabIndex = 2;
            this.udDLSizeFar.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLSizeFar.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udDLSizeFar.Value = new decimal(new int[] {
            600,
            0,
            0,
            0});
            this.udDLSizeFar.ValueChanged += new System.EventHandler(this.ValidateDistances);
            // 
            // tbDLNearSize
            // 
            this.tbDLNearSize.Location = new System.Drawing.Point(124, 30);
            this.tbDLNearSize.MaxLength = 3;
            this.tbDLNearSize.Name = "tbDLNearSize";
            this.tbDLNearSize.ReadOnly = true;
            this.tbDLNearSize.Size = new System.Drawing.Size(50, 20);
            this.tbDLNearSize.TabIndex = 0;
            this.tbDLNearSize.Text = "400";
            this.tbDLNearSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // lDLVeryFar
            // 
            this.lDLVeryFar.Location = new System.Drawing.Point(20, 78);
            this.lDLVeryFar.Name = "lDLVeryFar";
            this.lDLVeryFar.Size = new System.Drawing.Size(102, 19);
            this.lDLVeryFar.TabIndex = 0;
            this.lDLVeryFar.Text = "Very Far";
            this.lDLVeryFar.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lDLFar
            // 
            this.lDLFar.Location = new System.Drawing.Point(20, 54);
            this.lDLFar.Name = "lDLFar";
            this.lDLFar.Size = new System.Drawing.Size(102, 19);
            this.lDLFar.TabIndex = 0;
            this.lDLFar.Text = "Far";
            this.lDLFar.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lDLNear
            // 
            this.lDLNear.Location = new System.Drawing.Point(20, 30);
            this.lDLNear.Name = "lDLNear";
            this.lDLNear.Size = new System.Drawing.Size(102, 19);
            this.lDLNear.TabIndex = 0;
            this.lDLNear.Text = "Near";
            this.lDLNear.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lDLMinSize
            // 
            this.lDLMinSize.AutoSize = true;
            this.lDLMinSize.Location = new System.Drawing.Point(121, 16);
            this.lDLMinSize.Name = "lDLMinSize";
            this.lDLMinSize.Size = new System.Drawing.Size(47, 13);
            this.lDLMinSize.TabIndex = 0;
            this.lDLMinSize.Text = "Min Size";
            // 
            // lDLEndDist
            // 
            this.lDLEndDist.AutoSize = true;
            this.lDLEndDist.Location = new System.Drawing.Point(179, 16);
            this.lDLEndDist.Name = "lDLEndDist";
            this.lDLEndDist.Size = new System.Drawing.Size(47, 13);
            this.lDLEndDist.TabIndex = 0;
            this.lDLEndDist.Text = "End Dist";
            // 
            // cbDLAutoDist
            // 
            this.cbDLAutoDist.AutoSize = true;
            this.cbDLAutoDist.Location = new System.Drawing.Point(14, 67);
            this.cbDLAutoDist.Name = "cbDLAutoDist";
            this.cbDLAutoDist.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cbDLAutoDist.Size = new System.Drawing.Size(140, 17);
            this.cbDLAutoDist.TabIndex = 2;
            this.cbDLAutoDist.Text = "&Auto set other distances";
            this.cbDLAutoDist.UseVisualStyleBackColor = true;
            this.cbDLAutoDist.CheckedChanged += new System.EventHandler(this.cbAutoSetDistances_CheckedChanged);
            // 
            // gbDLAutoDist
            // 
            this.gbDLAutoDist.Controls.Add(this.rbDLAutoByAFogEnd);
            this.gbDLAutoDist.Controls.Add(this.rbDLAutoByDrawDist);
            this.gbDLAutoDist.Enabled = false;
            this.gbDLAutoDist.Location = new System.Drawing.Point(6, 69);
            this.gbDLAutoDist.Name = "gbDLAutoDist";
            this.gbDLAutoDist.Size = new System.Drawing.Size(266, 74);
            this.gbDLAutoDist.TabIndex = 3;
            this.gbDLAutoDist.TabStop = false;
            // 
            // rbDLAutoByAFogEnd
            // 
            this.rbDLAutoByAFogEnd.AutoSize = true;
            this.rbDLAutoByAFogEnd.Location = new System.Drawing.Point(13, 44);
            this.rbDLAutoByAFogEnd.Name = "rbDLAutoByAFogEnd";
            this.rbDLAutoByAFogEnd.Size = new System.Drawing.Size(146, 17);
            this.rbDLAutoByAFogEnd.TabIndex = 1;
            this.rbDLAutoByAFogEnd.TabStop = true;
            this.rbDLAutoByAFogEnd.Text = "By Above Water Fo&g End";
            this.rbDLAutoByAFogEnd.UseVisualStyleBackColor = true;
            this.rbDLAutoByAFogEnd.CheckedChanged += new System.EventHandler(this.rbDLAutoDist_CheckedChanged);
            // 
            // rbDLAutoByDrawDist
            // 
            this.rbDLAutoByDrawDist.AutoSize = true;
            this.rbDLAutoByDrawDist.Checked = true;
            this.rbDLAutoByDrawDist.Location = new System.Drawing.Point(13, 21);
            this.rbDLAutoByDrawDist.Name = "rbDLAutoByDrawDist";
            this.rbDLAutoByDrawDist.Size = new System.Drawing.Size(110, 17);
            this.rbDLAutoByDrawDist.TabIndex = 0;
            this.rbDLAutoByDrawDist.TabStop = true;
            this.rbDLAutoByDrawDist.Text = "By D&raw Distance";
            this.rbDLAutoByDrawDist.UseVisualStyleBackColor = true;
            this.rbDLAutoByDrawDist.CheckedChanged += new System.EventHandler(this.rbDLAutoDist_CheckedChanged);
            // 
            // gbDLSM
            // 
            this.gbDLSM.Controls.Add(this.rbDLSM20);
            this.gbDLSM.Controls.Add(this.rbDLSM30);
            this.gbDLSM.Location = new System.Drawing.Point(142, 19);
            this.gbDLSM.Name = "gbDLSM";
            this.gbDLSM.Size = new System.Drawing.Size(130, 44);
            this.gbDLSM.TabIndex = 1;
            this.gbDLSM.TabStop = false;
            this.gbDLSM.Text = "Shader &Model";
            // 
            // rbDLSM20
            // 
            this.rbDLSM20.AutoSize = true;
            this.rbDLSM20.Checked = true;
            this.rbDLSM20.Location = new System.Drawing.Point(11, 18);
            this.rbDLSM20.Name = "rbDLSM20";
            this.rbDLSM20.Size = new System.Drawing.Size(40, 17);
            this.rbDLSM20.TabIndex = 0;
            this.rbDLSM20.TabStop = true;
            this.rbDLSM20.Text = "&2.0";
            this.rbDLSM20.UseVisualStyleBackColor = true;
            this.rbDLSM20.CheckedChanged += new System.EventHandler(this.rbDLSM_CheckedChanged);
            // 
            // rbDLSM30
            // 
            this.rbDLSM30.AutoSize = true;
            this.rbDLSM30.Location = new System.Drawing.Point(72, 18);
            this.rbDLSM30.Name = "rbDLSM30";
            this.rbDLSM30.Size = new System.Drawing.Size(40, 17);
            this.rbDLSM30.TabIndex = 1;
            this.rbDLSM30.Text = "&3.0";
            this.rbDLSM30.UseVisualStyleBackColor = true;
            this.rbDLSM30.CheckedChanged += new System.EventHandler(this.rbDLSM_CheckedChanged);
            // 
            // gbDLWater
            // 
            this.gbDLWater.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.gbDLWater.Controls.Add(this.bDLDynRipples);
            this.gbDLWater.Controls.Add(this.gbDLWtrCaust);
            this.gbDLWater.Controls.Add(this.gbDLWtrWave);
            this.gbDLWater.Controls.Add(this.gbDLWtrRefl);
            this.gbDLWater.Controls.Add(this.lDLDynRipples);
            this.gbDLWater.Controls.Add(this.cbDLDynRipples);
            this.gbDLWater.Location = new System.Drawing.Point(278, 149);
            this.gbDLWater.Name = "gbDLWater";
            this.gbDLWater.Size = new System.Drawing.Size(238, 187);
            this.gbDLWater.TabIndex = 9;
            this.gbDLWater.TabStop = false;
            this.gbDLWater.Text = "&Water";
            // 
            // bDLDynRipples
            // 
            this.bDLDynRipples.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bDLDynRipples.Enabled = false;
            this.bDLDynRipples.Location = new System.Drawing.Point(144, 109);
            this.bDLDynRipples.Name = "bDLDynRipples";
            this.bDLDynRipples.Size = new System.Drawing.Size(88, 24);
            this.bDLDynRipples.TabIndex = 2;
            this.bDLDynRipples.Text = "Settings...";
            this.bDLDynRipples.UseVisualStyleBackColor = true;
            this.bDLDynRipples.Visible = false;
            this.bDLDynRipples.Click += new System.EventHandler(this.bDLDynRipples_Click);
            // 
            // gbDLWtrCaust
            // 
            this.gbDLWtrCaust.Controls.Add(this.lDLWtrCaust);
            this.gbDLWtrCaust.Controls.Add(this.udDLWtrCaust);
            this.gbDLWtrCaust.Location = new System.Drawing.Point(122, 137);
            this.gbDLWtrCaust.Name = "gbDLWtrCaust";
            this.gbDLWtrCaust.Size = new System.Drawing.Size(110, 44);
            this.gbDLWtrCaust.TabIndex = 4;
            this.gbDLWtrCaust.TabStop = false;
            this.gbDLWtrCaust.Text = "&Caustic intensity";
            // 
            // lDLWtrCaust
            // 
            this.lDLWtrCaust.Location = new System.Drawing.Point(50, 15);
            this.lDLWtrCaust.Name = "lDLWtrCaust";
            this.lDLWtrCaust.Size = new System.Drawing.Size(49, 19);
            this.lDLWtrCaust.TabIndex = 0;
            this.lDLWtrCaust.Text = "%";
            this.lDLWtrCaust.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // udDLWtrCaust
            // 
            this.udDLWtrCaust.Location = new System.Drawing.Point(6, 15);
            this.udDLWtrCaust.Name = "udDLWtrCaust";
            this.udDLWtrCaust.Size = new System.Drawing.Size(42, 20);
            this.udDLWtrCaust.TabIndex = 0;
            this.udDLWtrCaust.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLWtrCaust.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udDLWtrCaust.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // gbDLWtrWave
            // 
            this.gbDLWtrWave.Controls.Add(this.lDLWtrWave);
            this.gbDLWtrWave.Controls.Add(this.udDLWtrWave);
            this.gbDLWtrWave.Enabled = false;
            this.gbDLWtrWave.Location = new System.Drawing.Point(6, 137);
            this.gbDLWtrWave.Name = "gbDLWtrWave";
            this.gbDLWtrWave.Size = new System.Drawing.Size(110, 44);
            this.gbDLWtrWave.TabIndex = 3;
            this.gbDLWtrWave.TabStop = false;
            this.gbDLWtrWave.Text = "Height of wa&ves";
            // 
            // lDLWtrWave
            // 
            this.lDLWtrWave.Location = new System.Drawing.Point(50, 15);
            this.lDLWtrWave.Name = "lDLWtrWave";
            this.lDLWtrWave.Size = new System.Drawing.Size(49, 19);
            this.lDLWtrWave.TabIndex = 1;
            this.lDLWtrWave.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // udDLWtrWave
            // 
            this.udDLWtrWave.Location = new System.Drawing.Point(6, 15);
            this.udDLWtrWave.Maximum = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.udDLWtrWave.Name = "udDLWtrWave";
            this.udDLWtrWave.Size = new System.Drawing.Size(42, 20);
            this.udDLWtrWave.TabIndex = 0;
            this.udDLWtrWave.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLWtrWave.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            // 
            // gbDLWtrRefl
            // 
            this.gbDLWtrRefl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.gbDLWtrRefl.Controls.Add(this.cbDLSkyRefl);
            this.gbDLWtrRefl.Controls.Add(this.cbDLReflLand);
            this.gbDLWtrRefl.Controls.Add(this.cbDLReflBlur);
            this.gbDLWtrRefl.Controls.Add(this.cbDLReflNStatics);
            this.gbDLWtrRefl.Controls.Add(this.cbDLReflFStatics);
            this.gbDLWtrRefl.Location = new System.Drawing.Point(6, 19);
            this.gbDLWtrRefl.Name = "gbDLWtrRefl";
            this.gbDLWtrRefl.Size = new System.Drawing.Size(226, 89);
            this.gbDLWtrRefl.TabIndex = 0;
            this.gbDLWtrRefl.TabStop = false;
            this.gbDLWtrRefl.Text = "Reflections";
            // 
            // cbDLReflLand
            // 
            this.cbDLReflLand.AutoSize = true;
            this.cbDLReflLand.Location = new System.Drawing.Point(11, 42);
            this.cbDLReflLand.Name = "cbDLReflLand";
            this.cbDLReflLand.Size = new System.Drawing.Size(79, 17);
            this.cbDLReflLand.TabIndex = 2;
            this.cbDLReflLand.Text = "Landsca&pe";
            this.cbDLReflLand.UseVisualStyleBackColor = true;
            this.cbDLReflLand.CheckedChanged += new System.EventHandler(this.cbDLReflectiveWater_CheckedChanged);
            // 
            // cbDLReflBlur
            // 
            this.cbDLReflBlur.AutoSize = true;
            this.cbDLReflBlur.Enabled = false;
            this.cbDLReflBlur.Location = new System.Drawing.Point(11, 65);
            this.cbDLReflBlur.Name = "cbDLReflBlur";
            this.cbDLReflBlur.Size = new System.Drawing.Size(95, 17);
            this.cbDLReflBlur.TabIndex = 4;
            this.cbDLReflBlur.Text = "&Blur reflections";
            this.cbDLReflBlur.UseVisualStyleBackColor = true;
            // 
            // cbDLReflNStatics
            // 
            this.cbDLReflNStatics.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbDLReflNStatics.AutoSize = true;
            this.cbDLReflNStatics.Enabled = false;
            this.cbDLReflNStatics.Location = new System.Drawing.Point(116, 19);
            this.cbDLReflNStatics.Name = "cbDLReflNStatics";
            this.cbDLReflNStatics.Size = new System.Drawing.Size(95, 17);
            this.cbDLReflNStatics.TabIndex = 1;
            this.cbDLReflNStatics.Text = "&Nearby Statics";
            this.cbDLReflNStatics.UseVisualStyleBackColor = true;
            this.cbDLReflNStatics.CheckedChanged += new System.EventHandler(this.cbDLReflectNStatics_CheckedChanged);
            // 
            // cbDLReflFStatics
            // 
            this.cbDLReflFStatics.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.cbDLReflFStatics.AutoSize = true;
            this.cbDLReflFStatics.Enabled = false;
            this.cbDLReflFStatics.Location = new System.Drawing.Point(116, 42);
            this.cbDLReflFStatics.Name = "cbDLReflFStatics";
            this.cbDLReflFStatics.Size = new System.Drawing.Size(94, 17);
            this.cbDLReflFStatics.TabIndex = 3;
            this.cbDLReflFStatics.Text = "D&istant Statics";
            this.cbDLReflFStatics.UseVisualStyleBackColor = true;
            // 
            // lDLDynRipples
            // 
            this.lDLDynRipples.Enabled = false;
            this.lDLDynRipples.Location = new System.Drawing.Point(132, 110);
            this.lDLDynRipples.Margin = new System.Windows.Forms.Padding(0);
            this.lDLDynRipples.Name = "lDLDynRipples";
            this.lDLDynRipples.Size = new System.Drawing.Size(14, 19);
            this.lDLDynRipples.TabIndex = 0;
            this.lDLDynRipples.Text = "«";
            this.lDLDynRipples.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lDLDynRipples.Visible = false;
            // 
            // cbDLDynRipples
            // 
            this.cbDLDynRipples.AutoSize = true;
            this.cbDLDynRipples.Location = new System.Drawing.Point(8, 114);
            this.cbDLDynRipples.Name = "cbDLDynRipples";
            this.cbDLDynRipples.Size = new System.Drawing.Size(105, 17);
            this.cbDLDynRipples.TabIndex = 1;
            this.cbDLDynRipples.Text = "D&ynamic Ripples";
            this.cbDLDynRipples.UseVisualStyleBackColor = true;
            this.cbDLDynRipples.CheckedChanged += new System.EventHandler(this.cbDLDynRipples_CheckedChanged);
            // 
            // bDLWthr
            // 
            this.bDLWthr.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bDLWthr.Location = new System.Drawing.Point(142, 311);
            this.bDLWthr.Name = "bDLWthr";
            this.bDLWthr.Size = new System.Drawing.Size(130, 25);
            this.bDLWthr.TabIndex = 8;
            this.bDLWthr.Text = "Weat&her Settings...";
            this.bDLWthr.UseVisualStyleBackColor = true;
            this.bDLWthr.Click += new System.EventHandler(this.bDLWthr_Click);
            // 
            // gbDLDrawDist
            // 
            this.gbDLDrawDist.Controls.Add(this.lDLDrawDist);
            this.gbDLDrawDist.Controls.Add(this.udDLDrawDist);
            this.gbDLDrawDist.Location = new System.Drawing.Point(6, 19);
            this.gbDLDrawDist.Name = "gbDLDrawDist";
            this.gbDLDrawDist.Size = new System.Drawing.Size(130, 44);
            this.gbDLDrawDist.TabIndex = 0;
            this.gbDLDrawDist.TabStop = false;
            this.gbDLDrawDist.Text = "&Draw Distance";
            // 
            // lDLDrawDist
            // 
            this.lDLDrawDist.Location = new System.Drawing.Point(58, 15);
            this.lDLDrawDist.Name = "lDLDrawDist";
            this.lDLDrawDist.Size = new System.Drawing.Size(61, 19);
            this.lDLDrawDist.TabIndex = 0;
            this.lDLDrawDist.Text = "cells";
            this.lDLDrawDist.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // udDLDrawDist
            // 
            this.udDLDrawDist.DecimalPlaces = 1;
            this.udDLDrawDist.Location = new System.Drawing.Point(6, 15);
            this.udDLDrawDist.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.udDLDrawDist.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udDLDrawDist.Name = "udDLDrawDist";
            this.udDLDrawDist.Size = new System.Drawing.Size(50, 20);
            this.udDLDrawDist.TabIndex = 0;
            this.udDLDrawDist.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLDrawDist.UpDownAlign = System.Windows.Forms.LeftRightAlignment.Left;
            this.udDLDrawDist.Value = new decimal(new int[] {
            13,
            0,
            0,
            0});
            this.udDLDrawDist.ValueChanged += new System.EventHandler(this.udDLDrawDist_ValueChanged);
            // 
            // bDLOptions
            // 
            this.bDLOptions.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bDLOptions.Location = new System.Drawing.Point(6, 311);
            this.bDLOptions.Name = "bDLOptions";
            this.bDLOptions.Size = new System.Drawing.Size(130, 25);
            this.bDLOptions.TabIndex = 7;
            this.bDLOptions.Text = "&Options...";
            this.bDLOptions.UseVisualStyleBackColor = true;
            this.bDLOptions.Click += new System.EventHandler(this.bDLOptions_Click);
            // 
            // gbDLFog
            // 
            this.gbDLFog.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbDLFog.Controls.Add(this.lDLFogExpMul);
            this.gbDLFog.Controls.Add(this.udDLFogExpMul);
            this.gbDLFog.Controls.Add(this.lDLFogI);
            this.gbDLFog.Controls.Add(this.udDLFogIEnd);
            this.gbDLFog.Controls.Add(this.udDLFogIStart);
            this.gbDLFog.Controls.Add(this.udDLFogBEnd);
            this.gbDLFog.Controls.Add(this.cbDLFogExp);
            this.gbDLFog.Controls.Add(this.udDLFogBStart);
            this.gbDLFog.Controls.Add(this.udDLFogAEnd);
            this.gbDLFog.Controls.Add(this.udDLFogAStart);
            this.gbDLFog.Controls.Add(this.lDLFogB);
            this.gbDLFog.Controls.Add(this.lDLFogA);
            this.gbDLFog.Controls.Add(this.lDLFogStart);
            this.gbDLFog.Controls.Add(this.lDLFogEnd);
            this.gbDLFog.Location = new System.Drawing.Point(6, 149);
            this.gbDLFog.Name = "gbDLFog";
            this.gbDLFog.Size = new System.Drawing.Size(266, 156);
            this.gbDLFog.TabIndex = 6;
            this.gbDLFog.TabStop = false;
            this.gbDLFog.Text = "&Fog";
            // 
            // lDLFogExpMul
            // 
            this.lDLFogExpMul.Enabled = false;
            this.lDLFogExpMul.Location = new System.Drawing.Point(8, 127);
            this.lDLFogExpMul.Name = "lDLFogExpMul";
            this.lDLFogExpMul.Size = new System.Drawing.Size(199, 19);
            this.lDLFogExpMul.TabIndex = 8;
            this.lDLFogExpMul.Text = "Exponential Distance Multiplier";
            this.lDLFogExpMul.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // udDLFogExpMul
            // 
            this.udDLFogExpMul.DecimalPlaces = 2;
            this.udDLFogExpMul.Enabled = false;
            this.udDLFogExpMul.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udDLFogExpMul.Location = new System.Drawing.Point(210, 127);
            this.udDLFogExpMul.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udDLFogExpMul.Minimum = new decimal(new int[] {
            25,
            0,
            0,
            65536});
            this.udDLFogExpMul.Name = "udDLFogExpMul";
            this.udDLFogExpMul.Size = new System.Drawing.Size(50, 20);
            this.udDLFogExpMul.TabIndex = 7;
            this.udDLFogExpMul.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogExpMul.Value = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.udDLFogExpMul.ValueChanged += new System.EventHandler(this.udDLFogExpMul_ValueChanged);
            // 
            // lDLFogI
            // 
            this.lDLFogI.Location = new System.Drawing.Point(8, 78);
            this.lDLFogI.Name = "lDLFogI";
            this.lDLFogI.Size = new System.Drawing.Size(142, 19);
            this.lDLFogI.TabIndex = 0;
            this.lDLFogI.Text = "Distant Interiors Fog";
            this.lDLFogI.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // udDLFogIEnd
            // 
            this.udDLFogIEnd.DecimalPlaces = 2;
            this.udDLFogIEnd.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udDLFogIEnd.Location = new System.Drawing.Point(210, 78);
            this.udDLFogIEnd.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.udDLFogIEnd.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            131072});
            this.udDLFogIEnd.Name = "udDLFogIEnd";
            this.udDLFogIEnd.Size = new System.Drawing.Size(50, 20);
            this.udDLFogIEnd.TabIndex = 5;
            this.udDLFogIEnd.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogIEnd.Value = new decimal(new int[] {
            9,
            0,
            0,
            65536});
            this.udDLFogIEnd.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLFogIStart
            // 
            this.udDLFogIStart.DecimalPlaces = 2;
            this.udDLFogIStart.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udDLFogIStart.Location = new System.Drawing.Point(152, 78);
            this.udDLFogIStart.Maximum = new decimal(new int[] {
            990,
            0,
            0,
            131072});
            this.udDLFogIStart.Minimum = new decimal(new int[] {
            90,
            0,
            0,
            -2147352576});
            this.udDLFogIStart.Name = "udDLFogIStart";
            this.udDLFogIStart.Size = new System.Drawing.Size(50, 20);
            this.udDLFogIStart.TabIndex = 4;
            this.udDLFogIStart.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogIStart.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLFogBEnd
            // 
            this.udDLFogBEnd.DecimalPlaces = 1;
            this.udDLFogBEnd.Location = new System.Drawing.Point(210, 54);
            this.udDLFogBEnd.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.udDLFogBEnd.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udDLFogBEnd.Name = "udDLFogBEnd";
            this.udDLFogBEnd.Size = new System.Drawing.Size(50, 20);
            this.udDLFogBEnd.TabIndex = 3;
            this.udDLFogBEnd.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogBEnd.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.udDLFogBEnd.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // cbDLFogExp
            // 
            this.cbDLFogExp.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbDLFogExp.Enabled = false;
            this.cbDLFogExp.Location = new System.Drawing.Point(8, 104);
            this.cbDLFogExp.Name = "cbDLFogExp";
            this.cbDLFogExp.Size = new System.Drawing.Size(252, 18);
            this.cbDLFogExp.TabIndex = 6;
            this.cbDLFogExp.Text = "Use e&xponential fog";
            this.cbDLFogExp.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbDLFogExp.UseVisualStyleBackColor = true;
            this.cbDLFogExp.CheckedChanged += new System.EventHandler(this.cbDLFogExp_CheckedChanged);
            // 
            // udDLFogBStart
            // 
            this.udDLFogBStart.DecimalPlaces = 1;
            this.udDLFogBStart.Location = new System.Drawing.Point(152, 54);
            this.udDLFogBStart.Maximum = new decimal(new int[] {
            2999,
            0,
            0,
            65536});
            this.udDLFogBStart.Minimum = new decimal(new int[] {
            999,
            0,
            0,
            -2147418112});
            this.udDLFogBStart.Name = "udDLFogBStart";
            this.udDLFogBStart.Size = new System.Drawing.Size(50, 20);
            this.udDLFogBStart.TabIndex = 2;
            this.udDLFogBStart.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogBStart.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // udDLFogAEnd
            // 
            this.udDLFogAEnd.DecimalPlaces = 1;
            this.udDLFogAEnd.Location = new System.Drawing.Point(210, 30);
            this.udDLFogAEnd.Maximum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.udDLFogAEnd.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.udDLFogAEnd.Name = "udDLFogAEnd";
            this.udDLFogAEnd.Size = new System.Drawing.Size(50, 20);
            this.udDLFogAEnd.TabIndex = 1;
            this.udDLFogAEnd.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogAEnd.Value = new decimal(new int[] {
            13,
            0,
            0,
            0});
            this.udDLFogAEnd.ValueChanged += new System.EventHandler(this.udDLFogAEnd_ValueChanged);
            // 
            // udDLFogAStart
            // 
            this.udDLFogAStart.DecimalPlaces = 1;
            this.udDLFogAStart.Location = new System.Drawing.Point(152, 30);
            this.udDLFogAStart.Maximum = new decimal(new int[] {
            2999,
            0,
            0,
            65536});
            this.udDLFogAStart.Name = "udDLFogAStart";
            this.udDLFogAStart.Size = new System.Drawing.Size(50, 20);
            this.udDLFogAStart.TabIndex = 0;
            this.udDLFogAStart.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.udDLFogAStart.Value = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.udDLFogAStart.ValueChanged += new System.EventHandler(this.ValidateDLSettings);
            // 
            // lDLFogB
            // 
            this.lDLFogB.Location = new System.Drawing.Point(6, 54);
            this.lDLFogB.Name = "lDLFogB";
            this.lDLFogB.Size = new System.Drawing.Size(144, 19);
            this.lDLFogB.TabIndex = 0;
            this.lDLFogB.Text = "Below Water Fog";
            this.lDLFogB.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lDLFogA
            // 
            this.lDLFogA.Location = new System.Drawing.Point(8, 30);
            this.lDLFogA.Name = "lDLFogA";
            this.lDLFogA.Size = new System.Drawing.Size(142, 19);
            this.lDLFogA.TabIndex = 0;
            this.lDLFogA.Text = "Above Water Fog";
            this.lDLFogA.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lDLFogStart
            // 
            this.lDLFogStart.AutoSize = true;
            this.lDLFogStart.Location = new System.Drawing.Point(149, 16);
            this.lDLFogStart.Name = "lDLFogStart";
            this.lDLFogStart.Size = new System.Drawing.Size(29, 13);
            this.lDLFogStart.TabIndex = 0;
            this.lDLFogStart.Text = "Start";
            // 
            // lDLFogEnd
            // 
            this.lDLFogEnd.AutoSize = true;
            this.lDLFogEnd.Location = new System.Drawing.Point(207, 16);
            this.lDLFogEnd.Name = "lDLFogEnd";
            this.lDLFogEnd.Size = new System.Drawing.Size(26, 13);
            this.lDLFogEnd.TabIndex = 0;
            this.lDLFogEnd.Text = "End";
            // 
            // SShotFolderDialog
            // 
            this.SShotFolderDialog.Description = "Select output folder for screenshots taken in game:";
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(546, 379);
            this.Controls.Add(this.tabControl);
            this.Icon = global::MGEgui.Properties.Resources.AppIcon;
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(554, 409);
            this.Name = "MainForm";
            this.Text = "     ";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_FormClosing);
            this.Resize += new System.EventHandler(this.MainForm_Resize);
            this.tpTools.ResumeLayout(false);
            this.tpINI.ResumeLayout(false);
            this.tpINI.PerformLayout();
            this.gbMWGeneral.ResumeLayout(false);
            this.gbMWGeneral.PerformLayout();
            this.tpMisc.ResumeLayout(false);
            this.tpMisc.PerformLayout();
            this.pCam3rdPrsn.ResumeLayout(false);
            this.pCam3rdPrsn.PerformLayout();
            this.gbCam3rdPrsn.ResumeLayout(false);
            this.gbCam3rdPrsn.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udCam3rdX)).EndInit();
            this.pIdle.ResumeLayout(false);
            this.pIdle.PerformLayout();
            this.gbIdle.ResumeLayout(false);
            this.gbIdle.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udIdle)).EndInit();
            this.pFPSConst.ResumeLayout(false);
            this.pFPSConst.PerformLayout();
            this.gbFPSConst.ResumeLayout(false);
            this.gbFPSConst.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udFPSDes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFPSTol)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMinView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udMaxView)).EndInit();
            this.gbShaders.ResumeLayout(false);
            this.gbShaders.PerformLayout();
            this.gbHDR.ResumeLayout(false);
            this.gbHDR.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udHDR)).EndInit();
            this.tpInput.ResumeLayout(false);
            this.gbInputTools.ResumeLayout(false);
            this.gbInputOpts.ResumeLayout(false);
            this.gbInputOpts.PerformLayout();
            this.tpRender.ResumeLayout(false);
            this.gbMsgs.ResumeLayout(false);
            this.gbMsgs.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udMsgsTime)).EndInit();
            this.gbFog.ResumeLayout(false);
            this.gbFog.PerformLayout();
            this.gbRendOpts.ResumeLayout(false);
            this.gbRendOpts.PerformLayout();
            this.gbScreen.ResumeLayout(false);
            this.gbScreen.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udFOV)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udRotation)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udZoom)).EndInit();
            this.gbTexFilt.ResumeLayout(false);
            this.gbTexFilt.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udLOD)).EndInit();
            this.tpGlobal.ResumeLayout(false);
            this.gbSShot.ResumeLayout(false);
            this.gbSShot.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udSShotNum)).EndInit();
            this.gbScene.ResumeLayout(false);
            this.gbScene.PerformLayout();
            this.gbDisplay.ResumeLayout(false);
            this.gbDisplay.PerformLayout();
            this.tabControl.ResumeLayout(false);
            this.tpMain.ResumeLayout(false);
            this.tpMain.PerformLayout();
            this.gbMainUI.ResumeLayout(false);
            this.gbMainUI.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.gbMainInfo.ResumeLayout(false);
            this.gbMainSettings.ResumeLayout(false);
            this.gbMainSettings.PerformLayout();
            this.gbMainQuality.ResumeLayout(false);
            this.tpDL.ResumeLayout(false);
            this.tpDL.PerformLayout();
            this.gbDistantLand.ResumeLayout(false);
            this.gbDistantLand.PerformLayout();
            this.pDLStatics.ResumeLayout(false);
            this.pDLStatics.PerformLayout();
            this.gbDLStatics.ResumeLayout(false);
            this.gbDLStatics.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistVeryFar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistFar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLDistNear)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLSizeVeryFar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLSizeFar)).EndInit();
            this.gbDLAutoDist.ResumeLayout(false);
            this.gbDLAutoDist.PerformLayout();
            this.gbDLSM.ResumeLayout(false);
            this.gbDLSM.PerformLayout();
            this.gbDLWater.ResumeLayout(false);
            this.gbDLWater.PerformLayout();
            this.gbDLWtrCaust.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.udDLWtrCaust)).EndInit();
            this.gbDLWtrWave.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.udDLWtrWave)).EndInit();
            this.gbDLWtrRefl.ResumeLayout(false);
            this.gbDLWtrRefl.PerformLayout();
            this.gbDLDrawDist.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.udDLDrawDist)).EndInit();
            this.gbDLFog.ResumeLayout(false);
            this.gbDLFog.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogExpMul)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogIEnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogIStart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogBEnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogBStart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogAEnd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDLFogAStart)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        public Label lDLVeryFar;
        public Label lDLFar;
        public Label lDLNear;
        public Label lDLMinSize;
        public Label lDLEndDist;
        public CheckBox cbDLDistantStatics;
        public CheckBox cbDLDistantLand;
        public TextBox tbDLNearSize;
        public TabControl tabControl;
        public ContextMenu DudMenu;
        public OpenFileDialog OpenFileDialog;
        public SaveFileDialog SaveFileDialog;
        public OpenFileDialog OpenTangentMesh;
        public ToolTip toolTip;
        public TabPage tpTools;
        public Button bHookGen;
        public Button bShaderEd;
        public TabPage tpINI;
        public CheckBox cbMultipleCS;
        public CheckBox cbHitFader;
        public CheckBox cbSubtitles;
        public CheckBox cbAudio;
        public CheckBox cbShowFPS;
        public Label lScreenshotStatus;
        public CheckBox cbScreenshots;
        public CheckBox cbHQShadows;
        public CheckBox cbYesToAll;
        public CheckBox cbThreadLoad;
        public TabPage tpMisc;
        public CheckBox cbZoomEffectsHUD;
        public Label lHDR;
        public CheckBox cbHDR;
        public CheckBox cbHook;
        public CheckBox cbSetEffectVars;
        public Label lMinView;
        public Label lMaxView;
        public CheckBox cbBindAI;
        public CheckBox cbFPSConst;
        public Label lFPSTol;
        public Label lFPSDes;
        public TabPage tpInput;
        public Button bRemapper;
        public CheckBox cbAltCombat;
        public CheckBox cbSkipMovie;
        public CheckBox cbConsole;
        public CheckBox cbLagFix;
        public Button bMacroEd;
        public TabPage tpRender;
        public Label lLOD;
        public ComboBox cmbScaleFilter;
        public Label lSShotFormat;
        public CheckBox cbHWShader;
        public CheckBox cbFog;
        public ComboBox cmbSShotFormat;
        public CheckBox cbAAFix;
        public Label lScaleFilter;
        public ComboBox cmbMipFilter;
        public Label lMipFilter;
        public ComboBox cmbAnisoLevel;
        public Label lAnisoLevel;
        public Label lRotation;
        public ComboBox cmbFogMode;
        public Label lZoom;
        public Label lFogMode;
        public CheckBox cbFPSCounter;
        public CheckBox cbDisplayMessages;
        public TabPage tpGlobal;
        public TextBox tbResolution;
        public Button bCalcResolution;
        public Label lResolution;
        public CheckBox cbWindowed;
        public Button bCalcRefresh;
        public Label lRefreshRate;
        public ComboBox cmbRefreshRate;
        public ComboBox cmbAntiAlias;
        public Label lAntiAlias;
        public ComboBox cmbZBuffer;
        public Label lZBuffer;
        public ComboBox cmbVWait;
        public Label lVWait;
        public CheckBox cbAspectZoom;
        public CheckBox cbDisableMWSE;
        public TabPage tpDL;
        public CheckBox cbDLReflFStatics;
        public CheckBox cbDLReflNStatics;
        public CheckBox cbDLReflLand;
        public GroupBox gbDistantLand;
        public GroupBox gbDLWtrRefl;
        public GroupBox gbDLStatics;
        public CheckBox cbDLAutoDist;
        public GroupBox gbDLFog;
        public Label lDLFogB;
        public Label lDLFogA;
        public Label lDLFogStart;
        public Label lDLFogEnd;
        public NumericUpDown udDLDrawDist;
        public NumericUpDown udZoom;
        public NumericUpDown udLOD;
        public NumericUpDown udRotation;
        public NumericUpDown udFPSDes;
        public NumericUpDown udHDR;
        public NumericUpDown udMinView;
        public NumericUpDown udMaxView;
        public NumericUpDown udFPSTol;
        public NumericUpDown udDLSizeVeryFar;
        public NumericUpDown udDLSizeFar;
        public NumericUpDown udDLDistVeryFar;
        public NumericUpDown udDLDistFar;
        public NumericUpDown udDLDistNear;
        public NumericUpDown udDLFogAStart;
        public NumericUpDown udDLFogAEnd;
        public NumericUpDown udDLFogBEnd;
        public NumericUpDown udDLFogBStart;
        public Button bDLWthr;
        public GroupBox gbTexFilt;
        public GroupBox gbRendOpts;
        public GroupBox gbScreen;
        public NumericUpDown udFOV;
        public Label lFOV;
        public GroupBox gbShaders;
        public GroupBox gbFPSConst;
        public GroupBox gbHDR;
        public CheckBox cbIdle;
        public GroupBox gbIdle;
        public NumericUpDown udIdle;
        public Label lIdle;
        public Button bDLOptions;
        public TabPage tpMain;
        public Button bResetSettings;
        public Button bAbout;
        public Button bViewCaps;
        public CheckBox cbImportIni;
        public Button bExport;
        public Button bImport;
        public Button bHelp;
        public Button bQuality;
        public GroupBox gbMainInfo;
        public GroupBox gbMainSettings;
        public GroupBox gbMainQuality;
        public Button bDistantLandWizard;
        public GroupBox gbDLDrawDist;
        public Label lDLDrawDist;
        public CheckBox cbClearINI;
        public GroupBox gbMWGeneral;
        public GroupBox gbDisplay;
        public GroupBox gbScene;
        public TextBox tbAspect;
        public Label lAspect;
        public GroupBox gbSShot;
        public Label lSShotDir;
        public TextBox tbSShotDir;
        public Label lSShotNum;
        public NumericUpDown udSShotNum;
        public Button bSShotDirClear;
        public Button bSShotDirBrowse;
        public TextBox tbSShotName;
        public Label lSShotName;
        public FolderBrowserDialog SShotFolderDialog;
        public NumericUpDown udDLFogIEnd;
        public NumericUpDown udDLFogIStart;
        public Label lDLFogI;
		public CheckBox cbDLSkyRefl;
        public CheckBox cbShaderDepth;
        public CheckBox cbDLDynRipples;
        private CheckBox cbDisableMGE;
        private GroupBox gbFog;
        private GroupBox gbInputOpts;
        private GroupBox gbInputTools;
        private GroupBox gbDLWater;
        private RadioButton rbDLSM30;
        private RadioButton rbDLSM20;
        private CheckBox cbDLReflBlur;
        private GroupBox gbDLWtrCaust;
        public NumericUpDown udDLWtrCaust;
        private GroupBox gbDLWtrWave;
        public NumericUpDown udDLWtrWave;
        private GroupBox gbDLSM;
        public Label lDLWtrCaust;
        public Label lDLWtrWave;
        public NumericUpDown udDLFogExpMul;
        public Label lDLFogExpMul;
        public CheckBox cbDLFogExp;
        private GroupBox gbDLAutoDist;
        private RadioButton rbDLAutoByAFogEnd;
        private RadioButton rbDLAutoByDrawDist;
        private CheckBox cbDLDistantStaticsScript;
        private GroupBox gbMainUI;
        private ComboBox cmbUILanguage;
        private Label lUILanguage;
        private Label lTipReadSpd;
        private Label lTipRSpdUnit;
        public ComboBox cmbTipReadSpd;
        public Button bDLDynRipples;
        private GroupBox gbMsgs;
        private ComboBox cmbMsgsLocale;
        private Label lMsgsLanguage;
        public NumericUpDown udMsgsTime;
        private Label lMsgsTimeUnit;
        private Label lMsgsTime;
        private Label lDLDynRipples;
        private Panel pDLStatics;
        private Panel pFPSConst;
        private Panel pIdle;
        private CheckBox cbUILangAuto;
        private Panel panel1;
        private Panel pCam3rdPrsn;
        private CheckBox cbCam3rdPrsn;
        private GroupBox gbCam3rdPrsn;
        public NumericUpDown udCam3rdZ;
        public NumericUpDown udCam3rdY;
        public NumericUpDown udCam3rdX;
        private Label lCam3rdX;
        private Label lCam3rdY;
        private Label lCam3rdZ;
        private CheckBox cbDisableShadersInMenu;
        public CheckBox cbDisableMWE;
        public Button bMWE;
        public Button bPatchEd;
        private Button bPatch;
        private CheckBox cbDisablePatch;
        private CheckBox cbDisableIntegration;
    }
}