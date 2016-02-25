namespace KanColleSimGUI
{
    partial class MainForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
			this.MenuStrip = new System.Windows.Forms.MenuStrip();
			this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileOpenMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileSaveSMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileSaveAMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.FileCloseMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.ヘルプHToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.VerInfoMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.TabControl = new System.Windows.Forms.TabControl();
			this.WeaponTab = new System.Windows.Forms.TabPage();
			this.WPlusButton = new System.Windows.Forms.Button();
			this.WReloadButton = new System.Windows.Forms.Button();
			this.WMinusButton = new System.Windows.Forms.Button();
			this.WUndoButton = new System.Windows.Forms.Button();
			this.WSaveButton = new System.Windows.Forms.Button();
			this.WDefenseTextBox = new System.Windows.Forms.TextBox();
			this.label12 = new System.Windows.Forms.Label();
			this.WRangeComboBox = new System.Windows.Forms.ComboBox();
			this.label11 = new System.Windows.Forms.Label();
			this.WEvadeTextBox = new System.Windows.Forms.TextBox();
			this.label10 = new System.Windows.Forms.Label();
			this.WHitTextBox = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.WSearchTextBox = new System.Windows.Forms.TextBox();
			this.label8 = new System.Windows.Forms.Label();
			this.WAntiSubTextBox = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.WAntiAirTextBox = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.WBombTextBox = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.WTorpedoTextBox = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.WAttackTextBox = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.WTypeComboBox = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.WNameTextBox = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.WeaponListBox = new System.Windows.Forms.ListBox();
			this.KammusuTab = new System.Windows.Forms.TabPage();
			this.KAir4TextBox = new System.Windows.Forms.TextBox();
			this.KAir3TextBox = new System.Windows.Forms.TextBox();
			this.KAir2TextBox = new System.Windows.Forms.TextBox();
			this.KAir1TextBox = new System.Windows.Forms.TextBox();
			this.KPlusButton = new System.Windows.Forms.Button();
			this.KReloadButton = new System.Windows.Forms.Button();
			this.KMinusButton = new System.Windows.Forms.Button();
			this.KLuckTextBox = new System.Windows.Forms.TextBox();
			this.KSlotsComboBox = new System.Windows.Forms.ComboBox();
			this.label26 = new System.Windows.Forms.Label();
			this.label27 = new System.Windows.Forms.Label();
			this.label25 = new System.Windows.Forms.Label();
			this.KSpeedComboBox = new System.Windows.Forms.ComboBox();
			this.KRangeComboBox = new System.Windows.Forms.ComboBox();
			this.KMaxHPTextBox = new System.Windows.Forms.TextBox();
			this.label14 = new System.Windows.Forms.Label();
			this.KLevelTextBox = new System.Windows.Forms.TextBox();
			this.KUndoButton = new System.Windows.Forms.Button();
			this.KSaveButton = new System.Windows.Forms.Button();
			this.KDefenseTextBox = new System.Windows.Forms.TextBox();
			this.label13 = new System.Windows.Forms.Label();
			this.KEvadeTextBox = new System.Windows.Forms.TextBox();
			this.label15 = new System.Windows.Forms.Label();
			this.label16 = new System.Windows.Forms.Label();
			this.KSearchTextBox = new System.Windows.Forms.TextBox();
			this.label17 = new System.Windows.Forms.Label();
			this.KAntiSubTextBox = new System.Windows.Forms.TextBox();
			this.label18 = new System.Windows.Forms.Label();
			this.KAntiAirTextBox = new System.Windows.Forms.TextBox();
			this.label19 = new System.Windows.Forms.Label();
			this.label20 = new System.Windows.Forms.Label();
			this.KTorpedoTextBox = new System.Windows.Forms.TextBox();
			this.label21 = new System.Windows.Forms.Label();
			this.KAttackTextBox = new System.Windows.Forms.TextBox();
			this.label22 = new System.Windows.Forms.Label();
			this.KKindComboBox = new System.Windows.Forms.ComboBox();
			this.label23 = new System.Windows.Forms.Label();
			this.KNameTextBox = new System.Windows.Forms.TextBox();
			this.label24 = new System.Windows.Forms.Label();
			this.KammusuListBox = new System.Windows.Forms.ListBox();
			this.FleetsTab = new System.Windows.Forms.TabPage();
			this.FAllSearchTextBox = new System.Windows.Forms.TextBox();
			this.label43 = new System.Windows.Forms.Label();
			this.FAllAntiAirTextBox = new System.Windows.Forms.TextBox();
			this.label42 = new System.Windows.Forms.Label();
			this.FUndoButton = new System.Windows.Forms.Button();
			this.FUpperButton = new System.Windows.Forms.Button();
			this.FLowerButton = new System.Windows.Forms.Button();
			this.label41 = new System.Windows.Forms.Label();
			this.FNameTextBox = new System.Windows.Forms.TextBox();
			this.label40 = new System.Windows.Forms.Label();
			this.FHQLevelTextBox = new System.Windows.Forms.TextBox();
			this.FMinusButton = new System.Windows.Forms.Button();
			this.FPlusButton = new System.Windows.Forms.Button();
			this.FResetButton = new System.Windows.Forms.Button();
			this.label36 = new System.Windows.Forms.Label();
			this.FAmmoTextBox = new System.Windows.Forms.TextBox();
			this.label35 = new System.Windows.Forms.Label();
			this.FcondTextBox = new System.Windows.Forms.TextBox();
			this.FHPTextBox = new System.Windows.Forms.TextBox();
			this.label34 = new System.Windows.Forms.Label();
			this.FAir4TextBox = new System.Windows.Forms.TextBox();
			this.FAir3TextBox = new System.Windows.Forms.TextBox();
			this.FAir2TextBox = new System.Windows.Forms.TextBox();
			this.FAir1TextBox = new System.Windows.Forms.TextBox();
			this.label33 = new System.Windows.Forms.Label();
			this.label32 = new System.Windows.Forms.Label();
			this.label31 = new System.Windows.Forms.Label();
			this.label30 = new System.Windows.Forms.Label();
			this.label29 = new System.Windows.Forms.Label();
			this.label28 = new System.Windows.Forms.Label();
			this.FSlot4ComboBox = new System.Windows.Forms.ComboBox();
			this.FleetsListBox = new System.Windows.Forms.ListBox();
			this.FSlot3ComboBox = new System.Windows.Forms.ComboBox();
			this.FSlot2ComboBox = new System.Windows.Forms.ComboBox();
			this.FKammusuComboBox = new System.Windows.Forms.ComboBox();
			this.FSlot1ComboBox = new System.Windows.Forms.ComboBox();
			this.SimulationTab = new System.Windows.Forms.TabPage();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.SwapCheckBox = new System.Windows.Forms.CheckBox();
			this.BattleCountComboBox = new System.Windows.Forms.ComboBox();
			this.label39 = new System.Windows.Forms.Label();
			this.For2ComboBox = new System.Windows.Forms.ComboBox();
			this.For1ComboBox = new System.Windows.Forms.ComboBox();
			this.label38 = new System.Windows.Forms.Label();
			this.label37 = new System.Windows.Forms.Label();
			this.CalcButton = new System.Windows.Forms.Button();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.OutputTextBox = new System.Windows.Forms.TextBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.Read2Button = new System.Windows.Forms.Button();
			this.Fleets2ListBox = new System.Windows.Forms.ListBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.Read1Button = new System.Windows.Forms.Button();
			this.Fleets1ListBox = new System.Windows.Forms.ListBox();
			this.MapModeTab = new System.Windows.Forms.TabPage();
			this.groupBox8 = new System.Windows.Forms.GroupBox();
			this.OutputTextBox2 = new System.Windows.Forms.TextBox();
			this.groupBox7 = new System.Windows.Forms.GroupBox();
			this.BattleCountComboBox2 = new System.Windows.Forms.ComboBox();
			this.label44 = new System.Windows.Forms.Label();
			this.For3ComboBox = new System.Windows.Forms.ComboBox();
			this.label46 = new System.Windows.Forms.Label();
			this.CalcButton2 = new System.Windows.Forms.Button();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.Read4Button = new System.Windows.Forms.Button();
			this.PatternListBox = new System.Windows.Forms.ListBox();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.Read3Button = new System.Windows.Forms.Button();
			this.Fleets3ListBox = new System.Windows.Forms.ListBox();
			this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.MenuStrip.SuspendLayout();
			this.TabControl.SuspendLayout();
			this.WeaponTab.SuspendLayout();
			this.KammusuTab.SuspendLayout();
			this.FleetsTab.SuspendLayout();
			this.SimulationTab.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.MapModeTab.SuspendLayout();
			this.groupBox8.SuspendLayout();
			this.groupBox7.SuspendLayout();
			this.groupBox6.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.SuspendLayout();
			// 
			// MenuStrip
			// 
			this.MenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu,
            this.ヘルプHToolStripMenuItem});
			this.MenuStrip.Location = new System.Drawing.Point(0, 0);
			this.MenuStrip.Name = "MenuStrip";
			this.MenuStrip.Size = new System.Drawing.Size(434, 24);
			this.MenuStrip.TabIndex = 0;
			this.MenuStrip.Text = "menuStrip1";
			// 
			// FileMenu
			// 
			this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileOpenMenu,
            this.FileSaveSMenu,
            this.FileSaveAMenu,
            this.toolStripSeparator1,
            this.FileCloseMenu});
			this.FileMenu.Name = "FileMenu";
			this.FileMenu.Size = new System.Drawing.Size(66, 20);
			this.FileMenu.Text = "ファイル(&F)";
			// 
			// FileOpenMenu
			// 
			this.FileOpenMenu.Name = "FileOpenMenu";
			this.FileOpenMenu.Size = new System.Drawing.Size(186, 22);
			this.FileOpenMenu.Text = "開く(&O)...";
			this.FileOpenMenu.Click += new System.EventHandler(this.FileOpenMenu_Click);
			// 
			// FileSaveSMenu
			// 
			this.FileSaveSMenu.Name = "FileSaveSMenu";
			this.FileSaveSMenu.Size = new System.Drawing.Size(186, 22);
			this.FileSaveSMenu.Text = "上書き保存(&S)";
			this.FileSaveSMenu.Click += new System.EventHandler(this.FileSaveSMenu_Click);
			// 
			// FileSaveAMenu
			// 
			this.FileSaveAMenu.Name = "FileSaveAMenu";
			this.FileSaveAMenu.Size = new System.Drawing.Size(186, 22);
			this.FileSaveAMenu.Text = "名前を付けて保存(&A)...";
			this.FileSaveAMenu.Click += new System.EventHandler(this.FileSaveAMenu_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(183, 6);
			// 
			// FileCloseMenu
			// 
			this.FileCloseMenu.Name = "FileCloseMenu";
			this.FileCloseMenu.Size = new System.Drawing.Size(186, 22);
			this.FileCloseMenu.Text = "終了(&X)";
			this.FileCloseMenu.Click += new System.EventHandler(this.FileCloseMenu_Click);
			// 
			// ヘルプHToolStripMenuItem
			// 
			this.ヘルプHToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.VerInfoMenu});
			this.ヘルプHToolStripMenuItem.Name = "ヘルプHToolStripMenuItem";
			this.ヘルプHToolStripMenuItem.Size = new System.Drawing.Size(65, 20);
			this.ヘルプHToolStripMenuItem.Text = "ヘルプ(&H)";
			// 
			// VerInfoMenu
			// 
			this.VerInfoMenu.Name = "VerInfoMenu";
			this.VerInfoMenu.Size = new System.Drawing.Size(158, 22);
			this.VerInfoMenu.Text = "バージョン情報(&A)";
			this.VerInfoMenu.Click += new System.EventHandler(this.VerInfoMenu_Click);
			// 
			// TabControl
			// 
			this.TabControl.Controls.Add(this.WeaponTab);
			this.TabControl.Controls.Add(this.KammusuTab);
			this.TabControl.Controls.Add(this.FleetsTab);
			this.TabControl.Controls.Add(this.SimulationTab);
			this.TabControl.Controls.Add(this.MapModeTab);
			this.TabControl.Location = new System.Drawing.Point(12, 38);
			this.TabControl.Name = "TabControl";
			this.TabControl.SelectedIndex = 0;
			this.TabControl.Size = new System.Drawing.Size(410, 362);
			this.TabControl.TabIndex = 1;
			// 
			// WeaponTab
			// 
			this.WeaponTab.Controls.Add(this.WPlusButton);
			this.WeaponTab.Controls.Add(this.WReloadButton);
			this.WeaponTab.Controls.Add(this.WMinusButton);
			this.WeaponTab.Controls.Add(this.WUndoButton);
			this.WeaponTab.Controls.Add(this.WSaveButton);
			this.WeaponTab.Controls.Add(this.WDefenseTextBox);
			this.WeaponTab.Controls.Add(this.label12);
			this.WeaponTab.Controls.Add(this.WRangeComboBox);
			this.WeaponTab.Controls.Add(this.label11);
			this.WeaponTab.Controls.Add(this.WEvadeTextBox);
			this.WeaponTab.Controls.Add(this.label10);
			this.WeaponTab.Controls.Add(this.WHitTextBox);
			this.WeaponTab.Controls.Add(this.label9);
			this.WeaponTab.Controls.Add(this.WSearchTextBox);
			this.WeaponTab.Controls.Add(this.label8);
			this.WeaponTab.Controls.Add(this.WAntiSubTextBox);
			this.WeaponTab.Controls.Add(this.label7);
			this.WeaponTab.Controls.Add(this.WAntiAirTextBox);
			this.WeaponTab.Controls.Add(this.label6);
			this.WeaponTab.Controls.Add(this.WBombTextBox);
			this.WeaponTab.Controls.Add(this.label5);
			this.WeaponTab.Controls.Add(this.WTorpedoTextBox);
			this.WeaponTab.Controls.Add(this.label4);
			this.WeaponTab.Controls.Add(this.WAttackTextBox);
			this.WeaponTab.Controls.Add(this.label3);
			this.WeaponTab.Controls.Add(this.WTypeComboBox);
			this.WeaponTab.Controls.Add(this.label2);
			this.WeaponTab.Controls.Add(this.WNameTextBox);
			this.WeaponTab.Controls.Add(this.label1);
			this.WeaponTab.Controls.Add(this.WeaponListBox);
			this.WeaponTab.Location = new System.Drawing.Point(4, 22);
			this.WeaponTab.Name = "WeaponTab";
			this.WeaponTab.Padding = new System.Windows.Forms.Padding(3);
			this.WeaponTab.Size = new System.Drawing.Size(402, 336);
			this.WeaponTab.TabIndex = 0;
			this.WeaponTab.Text = "装備データ";
			this.WeaponTab.UseVisualStyleBackColor = true;
			// 
			// WPlusButton
			// 
			this.WPlusButton.Location = new System.Drawing.Point(307, 181);
			this.WPlusButton.Name = "WPlusButton";
			this.WPlusButton.Size = new System.Drawing.Size(75, 23);
			this.WPlusButton.TabIndex = 23;
			this.WPlusButton.Text = "追加";
			this.WPlusButton.UseVisualStyleBackColor = true;
			this.WPlusButton.Click += new System.EventHandler(this.WPlusButton_Click);
			// 
			// WReloadButton
			// 
			this.WReloadButton.Location = new System.Drawing.Point(307, 268);
			this.WReloadButton.Name = "WReloadButton";
			this.WReloadButton.Size = new System.Drawing.Size(75, 23);
			this.WReloadButton.TabIndex = 26;
			this.WReloadButton.Text = "再読み込み";
			this.WReloadButton.UseVisualStyleBackColor = true;
			this.WReloadButton.Click += new System.EventHandler(this.WReloadButton_Click);
			// 
			// WMinusButton
			// 
			this.WMinusButton.Location = new System.Drawing.Point(307, 210);
			this.WMinusButton.Name = "WMinusButton";
			this.WMinusButton.Size = new System.Drawing.Size(75, 23);
			this.WMinusButton.TabIndex = 24;
			this.WMinusButton.Text = "削除";
			this.WMinusButton.UseVisualStyleBackColor = true;
			this.WMinusButton.Click += new System.EventHandler(this.WMinusButton_Click);
			// 
			// WUndoButton
			// 
			this.WUndoButton.Location = new System.Drawing.Point(307, 239);
			this.WUndoButton.Name = "WUndoButton";
			this.WUndoButton.Size = new System.Drawing.Size(75, 23);
			this.WUndoButton.TabIndex = 25;
			this.WUndoButton.Text = "元に戻す";
			this.WUndoButton.UseVisualStyleBackColor = true;
			this.WUndoButton.Click += new System.EventHandler(this.WUndoButton_Click);
			// 
			// WSaveButton
			// 
			this.WSaveButton.Location = new System.Drawing.Point(307, 297);
			this.WSaveButton.Name = "WSaveButton";
			this.WSaveButton.Size = new System.Drawing.Size(75, 23);
			this.WSaveButton.TabIndex = 27;
			this.WSaveButton.Text = "上書き保存";
			this.WSaveButton.UseVisualStyleBackColor = true;
			this.WSaveButton.Click += new System.EventHandler(this.WSaveButton_Click);
			// 
			// WDefenseTextBox
			// 
			this.WDefenseTextBox.Location = new System.Drawing.Point(269, 92);
			this.WDefenseTextBox.Name = "WDefenseTextBox";
			this.WDefenseTextBox.Size = new System.Drawing.Size(39, 19);
			this.WDefenseTextBox.TabIndex = 22;
			this.WDefenseTextBox.Text = "0";
			// 
			// label12
			// 
			this.label12.AutoSize = true;
			this.label12.Location = new System.Drawing.Point(234, 95);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(29, 12);
			this.label12.TabIndex = 23;
			this.label12.Text = "装甲";
			// 
			// WRangeComboBox
			// 
			this.WRangeComboBox.FormattingEnabled = true;
			this.WRangeComboBox.Items.AddRange(new object[] {
            "無",
            "短",
            "中",
            "長",
            "超長"});
			this.WRangeComboBox.Location = new System.Drawing.Point(269, 66);
			this.WRangeComboBox.Name = "WRangeComboBox";
			this.WRangeComboBox.Size = new System.Drawing.Size(91, 20);
			this.WRangeComboBox.TabIndex = 21;
			this.WRangeComboBox.Text = "(射程を選択)";
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Location = new System.Drawing.Point(234, 70);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(29, 12);
			this.label11.TabIndex = 21;
			this.label11.Text = "射程";
			// 
			// WEvadeTextBox
			// 
			this.WEvadeTextBox.Location = new System.Drawing.Point(189, 242);
			this.WEvadeTextBox.Name = "WEvadeTextBox";
			this.WEvadeTextBox.Size = new System.Drawing.Size(39, 19);
			this.WEvadeTextBox.TabIndex = 20;
			this.WEvadeTextBox.Text = "0";
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(148, 245);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(29, 12);
			this.label10.TabIndex = 19;
			this.label10.Text = "回避";
			// 
			// WHitTextBox
			// 
			this.WHitTextBox.Location = new System.Drawing.Point(189, 217);
			this.WHitTextBox.Name = "WHitTextBox";
			this.WHitTextBox.Size = new System.Drawing.Size(39, 19);
			this.WHitTextBox.TabIndex = 18;
			this.WHitTextBox.Text = "0";
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(148, 220);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(29, 12);
			this.label9.TabIndex = 17;
			this.label9.Text = "命中";
			// 
			// WSearchTextBox
			// 
			this.WSearchTextBox.Location = new System.Drawing.Point(189, 192);
			this.WSearchTextBox.Name = "WSearchTextBox";
			this.WSearchTextBox.Size = new System.Drawing.Size(39, 19);
			this.WSearchTextBox.TabIndex = 16;
			this.WSearchTextBox.Text = "0";
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(148, 195);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(29, 12);
			this.label8.TabIndex = 15;
			this.label8.Text = "索敵";
			// 
			// WAntiSubTextBox
			// 
			this.WAntiSubTextBox.Location = new System.Drawing.Point(189, 167);
			this.WAntiSubTextBox.Name = "WAntiSubTextBox";
			this.WAntiSubTextBox.Size = new System.Drawing.Size(39, 19);
			this.WAntiSubTextBox.TabIndex = 14;
			this.WAntiSubTextBox.Text = "0";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(148, 170);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(29, 12);
			this.label7.TabIndex = 13;
			this.label7.Text = "対潜";
			// 
			// WAntiAirTextBox
			// 
			this.WAntiAirTextBox.Location = new System.Drawing.Point(189, 142);
			this.WAntiAirTextBox.Name = "WAntiAirTextBox";
			this.WAntiAirTextBox.Size = new System.Drawing.Size(39, 19);
			this.WAntiAirTextBox.TabIndex = 12;
			this.WAntiAirTextBox.Text = "0";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(148, 145);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(29, 12);
			this.label6.TabIndex = 11;
			this.label6.Text = "対空";
			// 
			// WBombTextBox
			// 
			this.WBombTextBox.Location = new System.Drawing.Point(189, 117);
			this.WBombTextBox.Name = "WBombTextBox";
			this.WBombTextBox.Size = new System.Drawing.Size(39, 19);
			this.WBombTextBox.TabIndex = 10;
			this.WBombTextBox.Text = "0";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(148, 120);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(29, 12);
			this.label5.TabIndex = 9;
			this.label5.Text = "爆装";
			// 
			// WTorpedoTextBox
			// 
			this.WTorpedoTextBox.Location = new System.Drawing.Point(189, 92);
			this.WTorpedoTextBox.Name = "WTorpedoTextBox";
			this.WTorpedoTextBox.Size = new System.Drawing.Size(39, 19);
			this.WTorpedoTextBox.TabIndex = 8;
			this.WTorpedoTextBox.Text = "0";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(148, 95);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(29, 12);
			this.label4.TabIndex = 7;
			this.label4.Text = "雷装";
			// 
			// WAttackTextBox
			// 
			this.WAttackTextBox.Location = new System.Drawing.Point(189, 67);
			this.WAttackTextBox.Name = "WAttackTextBox";
			this.WAttackTextBox.Size = new System.Drawing.Size(39, 19);
			this.WAttackTextBox.TabIndex = 6;
			this.WAttackTextBox.Text = "0";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(148, 70);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 12);
			this.label3.TabIndex = 5;
			this.label3.Text = "火力";
			// 
			// WTypeComboBox
			// 
			this.WTypeComboBox.FormattingEnabled = true;
			this.WTypeComboBox.Items.AddRange(new object[] {
            "主砲",
            "副砲",
            "魚雷",
            "特殊潜航艇",
            "艦上戦闘機",
            "艦上爆撃機(爆戦)",
            "艦上爆撃機",
            "水上爆撃機",
            "艦上攻撃機",
            "艦上偵察機",
            "艦上偵察機(彩雲)",
            "水上偵察機",
            "夜間偵察機",
            "対潜哨戒機",
            "オートジャイロ",
            "小型電探",
            "大型電探",
            "対空機銃",
            "高射装置",
            "爆雷",
            "ソナー",
            "探照灯",
            "照明弾",
            "水上艦要員",
            "その他"});
			this.WTypeComboBox.Location = new System.Drawing.Point(189, 41);
			this.WTypeComboBox.Name = "WTypeComboBox";
			this.WTypeComboBox.Size = new System.Drawing.Size(121, 20);
			this.WTypeComboBox.TabIndex = 4;
			this.WTypeComboBox.Text = "(種別を選択)";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(148, 45);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(29, 12);
			this.label2.TabIndex = 3;
			this.label2.Text = "種別";
			// 
			// WNameTextBox
			// 
			this.WNameTextBox.Location = new System.Drawing.Point(189, 16);
			this.WNameTextBox.Name = "WNameTextBox";
			this.WNameTextBox.Size = new System.Drawing.Size(121, 19);
			this.WNameTextBox.TabIndex = 2;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(148, 20);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(29, 12);
			this.label1.TabIndex = 1;
			this.label1.Text = "名前";
			// 
			// WeaponListBox
			// 
			this.WeaponListBox.FormattingEnabled = true;
			this.WeaponListBox.ItemHeight = 12;
			this.WeaponListBox.Location = new System.Drawing.Point(16, 16);
			this.WeaponListBox.Name = "WeaponListBox";
			this.WeaponListBox.Size = new System.Drawing.Size(120, 304);
			this.WeaponListBox.TabIndex = 0;
			this.WeaponListBox.SelectedIndexChanged += new System.EventHandler(this.WeaponListBox_SelectedIndexChanged);
			// 
			// KammusuTab
			// 
			this.KammusuTab.Controls.Add(this.KAir4TextBox);
			this.KammusuTab.Controls.Add(this.KAir3TextBox);
			this.KammusuTab.Controls.Add(this.KAir2TextBox);
			this.KammusuTab.Controls.Add(this.KAir1TextBox);
			this.KammusuTab.Controls.Add(this.KPlusButton);
			this.KammusuTab.Controls.Add(this.KReloadButton);
			this.KammusuTab.Controls.Add(this.KMinusButton);
			this.KammusuTab.Controls.Add(this.KLuckTextBox);
			this.KammusuTab.Controls.Add(this.KSlotsComboBox);
			this.KammusuTab.Controls.Add(this.label26);
			this.KammusuTab.Controls.Add(this.label27);
			this.KammusuTab.Controls.Add(this.label25);
			this.KammusuTab.Controls.Add(this.KSpeedComboBox);
			this.KammusuTab.Controls.Add(this.KRangeComboBox);
			this.KammusuTab.Controls.Add(this.KMaxHPTextBox);
			this.KammusuTab.Controls.Add(this.label14);
			this.KammusuTab.Controls.Add(this.KLevelTextBox);
			this.KammusuTab.Controls.Add(this.KUndoButton);
			this.KammusuTab.Controls.Add(this.KSaveButton);
			this.KammusuTab.Controls.Add(this.KDefenseTextBox);
			this.KammusuTab.Controls.Add(this.label13);
			this.KammusuTab.Controls.Add(this.KEvadeTextBox);
			this.KammusuTab.Controls.Add(this.label15);
			this.KammusuTab.Controls.Add(this.label16);
			this.KammusuTab.Controls.Add(this.KSearchTextBox);
			this.KammusuTab.Controls.Add(this.label17);
			this.KammusuTab.Controls.Add(this.KAntiSubTextBox);
			this.KammusuTab.Controls.Add(this.label18);
			this.KammusuTab.Controls.Add(this.KAntiAirTextBox);
			this.KammusuTab.Controls.Add(this.label19);
			this.KammusuTab.Controls.Add(this.label20);
			this.KammusuTab.Controls.Add(this.KTorpedoTextBox);
			this.KammusuTab.Controls.Add(this.label21);
			this.KammusuTab.Controls.Add(this.KAttackTextBox);
			this.KammusuTab.Controls.Add(this.label22);
			this.KammusuTab.Controls.Add(this.KKindComboBox);
			this.KammusuTab.Controls.Add(this.label23);
			this.KammusuTab.Controls.Add(this.KNameTextBox);
			this.KammusuTab.Controls.Add(this.label24);
			this.KammusuTab.Controls.Add(this.KammusuListBox);
			this.KammusuTab.Location = new System.Drawing.Point(4, 22);
			this.KammusuTab.Name = "KammusuTab";
			this.KammusuTab.Padding = new System.Windows.Forms.Padding(3);
			this.KammusuTab.Size = new System.Drawing.Size(402, 336);
			this.KammusuTab.TabIndex = 1;
			this.KammusuTab.Text = "艦娘データ";
			this.KammusuTab.UseVisualStyleBackColor = true;
			// 
			// KAir4TextBox
			// 
			this.KAir4TextBox.Location = new System.Drawing.Point(329, 67);
			this.KAir4TextBox.Name = "KAir4TextBox";
			this.KAir4TextBox.Size = new System.Drawing.Size(39, 19);
			this.KAir4TextBox.TabIndex = 8;
			this.KAir4TextBox.Text = "0";
			// 
			// KAir3TextBox
			// 
			this.KAir3TextBox.Location = new System.Drawing.Point(284, 67);
			this.KAir3TextBox.Name = "KAir3TextBox";
			this.KAir3TextBox.Size = new System.Drawing.Size(39, 19);
			this.KAir3TextBox.TabIndex = 7;
			this.KAir3TextBox.Text = "0";
			// 
			// KAir2TextBox
			// 
			this.KAir2TextBox.Location = new System.Drawing.Point(239, 67);
			this.KAir2TextBox.Name = "KAir2TextBox";
			this.KAir2TextBox.Size = new System.Drawing.Size(39, 19);
			this.KAir2TextBox.TabIndex = 6;
			this.KAir2TextBox.Text = "0";
			// 
			// KAir1TextBox
			// 
			this.KAir1TextBox.Location = new System.Drawing.Point(194, 67);
			this.KAir1TextBox.Name = "KAir1TextBox";
			this.KAir1TextBox.Size = new System.Drawing.Size(39, 19);
			this.KAir1TextBox.TabIndex = 5;
			this.KAir1TextBox.Text = "0";
			// 
			// KPlusButton
			// 
			this.KPlusButton.Location = new System.Drawing.Point(146, 263);
			this.KPlusButton.Name = "KPlusButton";
			this.KPlusButton.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.KPlusButton.Size = new System.Drawing.Size(75, 23);
			this.KPlusButton.TabIndex = 21;
			this.KPlusButton.Text = "追加";
			this.KPlusButton.UseVisualStyleBackColor = true;
			this.KPlusButton.Click += new System.EventHandler(this.KPlusButton_Click);
			// 
			// KReloadButton
			// 
			this.KReloadButton.Location = new System.Drawing.Point(227, 292);
			this.KReloadButton.Name = "KReloadButton";
			this.KReloadButton.Size = new System.Drawing.Size(75, 23);
			this.KReloadButton.TabIndex = 24;
			this.KReloadButton.Text = "再読み込み";
			this.KReloadButton.UseVisualStyleBackColor = true;
			this.KReloadButton.Click += new System.EventHandler(this.KReloadButton_Click);
			// 
			// KMinusButton
			// 
			this.KMinusButton.Location = new System.Drawing.Point(227, 263);
			this.KMinusButton.Name = "KMinusButton";
			this.KMinusButton.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.KMinusButton.Size = new System.Drawing.Size(75, 23);
			this.KMinusButton.TabIndex = 22;
			this.KMinusButton.Text = "削除";
			this.KMinusButton.UseVisualStyleBackColor = true;
			this.KMinusButton.Click += new System.EventHandler(this.KMinusButton_Click);
			// 
			// KLuckTextBox
			// 
			this.KLuckTextBox.Location = new System.Drawing.Point(345, 168);
			this.KLuckTextBox.Name = "KLuckTextBox";
			this.KLuckTextBox.Size = new System.Drawing.Size(39, 19);
			this.KLuckTextBox.TabIndex = 20;
			this.KLuckTextBox.Text = "0";
			// 
			// KSlotsComboBox
			// 
			this.KSlotsComboBox.FormattingEnabled = true;
			this.KSlotsComboBox.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3",
            "4"});
			this.KSlotsComboBox.Location = new System.Drawing.Point(194, 41);
			this.KSlotsComboBox.Name = "KSlotsComboBox";
			this.KSlotsComboBox.Size = new System.Drawing.Size(82, 20);
			this.KSlotsComboBox.TabIndex = 4;
			this.KSlotsComboBox.Text = "(スロット数)";
			this.KSlotsComboBox.SelectedIndexChanged += new System.EventHandler(this.KSlotsComboBox_SelectedIndexChanged);
			// 
			// label26
			// 
			this.label26.AutoSize = true;
			this.label26.Location = new System.Drawing.Point(310, 171);
			this.label26.Name = "label26";
			this.label26.Size = new System.Drawing.Size(17, 12);
			this.label26.TabIndex = 57;
			this.label26.Text = "運";
			this.label26.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// label27
			// 
			this.label27.AutoSize = true;
			this.label27.Location = new System.Drawing.Point(150, 44);
			this.label27.Name = "label27";
			this.label27.Size = new System.Drawing.Size(38, 12);
			this.label27.TabIndex = 58;
			this.label27.Text = "スロット";
			// 
			// label25
			// 
			this.label25.AutoSize = true;
			this.label25.Location = new System.Drawing.Point(150, 196);
			this.label25.Name = "label25";
			this.label25.Size = new System.Drawing.Size(29, 12);
			this.label25.TabIndex = 56;
			this.label25.Text = "速力";
			// 
			// KSpeedComboBox
			// 
			this.KSpeedComboBox.FormattingEnabled = true;
			this.KSpeedComboBox.Items.AddRange(new object[] {
            "低速",
            "高速"});
			this.KSpeedComboBox.Location = new System.Drawing.Point(194, 193);
			this.KSpeedComboBox.Name = "KSpeedComboBox";
			this.KSpeedComboBox.Size = new System.Drawing.Size(91, 20);
			this.KSpeedComboBox.TabIndex = 17;
			this.KSpeedComboBox.Text = "(速力を選択)";
			// 
			// KRangeComboBox
			// 
			this.KRangeComboBox.FormattingEnabled = true;
			this.KRangeComboBox.Items.AddRange(new object[] {
            "無",
            "短",
            "中",
            "長",
            "超長"});
			this.KRangeComboBox.Location = new System.Drawing.Point(194, 219);
			this.KRangeComboBox.Name = "KRangeComboBox";
			this.KRangeComboBox.Size = new System.Drawing.Size(91, 20);
			this.KRangeComboBox.TabIndex = 19;
			this.KRangeComboBox.Text = "(射程を選択)";
			// 
			// KMaxHPTextBox
			// 
			this.KMaxHPTextBox.Location = new System.Drawing.Point(185, 118);
			this.KMaxHPTextBox.Name = "KMaxHPTextBox";
			this.KMaxHPTextBox.Size = new System.Drawing.Size(39, 19);
			this.KMaxHPTextBox.TabIndex = 10;
			this.KMaxHPTextBox.Text = "0";
			// 
			// label14
			// 
			this.label14.AutoSize = true;
			this.label14.Location = new System.Drawing.Point(150, 222);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(29, 12);
			this.label14.TabIndex = 47;
			this.label14.Text = "射程";
			// 
			// KLevelTextBox
			// 
			this.KLevelTextBox.Location = new System.Drawing.Point(299, 16);
			this.KLevelTextBox.Name = "KLevelTextBox";
			this.KLevelTextBox.Size = new System.Drawing.Size(39, 19);
			this.KLevelTextBox.TabIndex = 3;
			this.KLevelTextBox.Text = "1";
			// 
			// KUndoButton
			// 
			this.KUndoButton.Location = new System.Drawing.Point(308, 263);
			this.KUndoButton.Name = "KUndoButton";
			this.KUndoButton.Size = new System.Drawing.Size(75, 23);
			this.KUndoButton.TabIndex = 23;
			this.KUndoButton.Text = "元に戻す";
			this.KUndoButton.UseVisualStyleBackColor = true;
			this.KUndoButton.Click += new System.EventHandler(this.KUndoButton_Click);
			// 
			// KSaveButton
			// 
			this.KSaveButton.Location = new System.Drawing.Point(308, 292);
			this.KSaveButton.Name = "KSaveButton";
			this.KSaveButton.Size = new System.Drawing.Size(75, 23);
			this.KSaveButton.TabIndex = 25;
			this.KSaveButton.Text = "上書き保存";
			this.KSaveButton.UseVisualStyleBackColor = true;
			this.KSaveButton.Click += new System.EventHandler(this.KSaveButton_Click);
			// 
			// KDefenseTextBox
			// 
			this.KDefenseTextBox.Location = new System.Drawing.Point(185, 143);
			this.KDefenseTextBox.Name = "KDefenseTextBox";
			this.KDefenseTextBox.Size = new System.Drawing.Size(39, 19);
			this.KDefenseTextBox.TabIndex = 12;
			this.KDefenseTextBox.Text = "0";
			// 
			// label13
			// 
			this.label13.AutoSize = true;
			this.label13.Location = new System.Drawing.Point(150, 146);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(29, 12);
			this.label13.TabIndex = 49;
			this.label13.Text = "装甲";
			// 
			// KEvadeTextBox
			// 
			this.KEvadeTextBox.Location = new System.Drawing.Point(185, 168);
			this.KEvadeTextBox.Name = "KEvadeTextBox";
			this.KEvadeTextBox.Size = new System.Drawing.Size(39, 19);
			this.KEvadeTextBox.TabIndex = 14;
			this.KEvadeTextBox.Text = "0";
			// 
			// label15
			// 
			this.label15.AutoSize = true;
			this.label15.Location = new System.Drawing.Point(150, 171);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(29, 12);
			this.label15.TabIndex = 45;
			this.label15.Text = "回避";
			// 
			// label16
			// 
			this.label16.AutoSize = true;
			this.label16.Location = new System.Drawing.Point(150, 95);
			this.label16.Name = "label16";
			this.label16.Size = new System.Drawing.Size(29, 12);
			this.label16.TabIndex = 43;
			this.label16.Text = "艦種";
			// 
			// KSearchTextBox
			// 
			this.KSearchTextBox.Location = new System.Drawing.Point(345, 143);
			this.KSearchTextBox.Name = "KSearchTextBox";
			this.KSearchTextBox.Size = new System.Drawing.Size(39, 19);
			this.KSearchTextBox.TabIndex = 18;
			this.KSearchTextBox.Text = "0";
			// 
			// label17
			// 
			this.label17.AutoSize = true;
			this.label17.Location = new System.Drawing.Point(310, 147);
			this.label17.Name = "label17";
			this.label17.Size = new System.Drawing.Size(29, 12);
			this.label17.TabIndex = 41;
			this.label17.Text = "索敵";
			// 
			// KAntiSubTextBox
			// 
			this.KAntiSubTextBox.Location = new System.Drawing.Point(345, 118);
			this.KAntiSubTextBox.Name = "KAntiSubTextBox";
			this.KAntiSubTextBox.Size = new System.Drawing.Size(39, 19);
			this.KAntiSubTextBox.TabIndex = 16;
			this.KAntiSubTextBox.Text = "0";
			// 
			// label18
			// 
			this.label18.AutoSize = true;
			this.label18.Location = new System.Drawing.Point(310, 121);
			this.label18.Name = "label18";
			this.label18.Size = new System.Drawing.Size(29, 12);
			this.label18.TabIndex = 39;
			this.label18.Text = "対潜";
			// 
			// KAntiAirTextBox
			// 
			this.KAntiAirTextBox.Location = new System.Drawing.Point(265, 168);
			this.KAntiAirTextBox.Name = "KAntiAirTextBox";
			this.KAntiAirTextBox.Size = new System.Drawing.Size(39, 19);
			this.KAntiAirTextBox.TabIndex = 15;
			this.KAntiAirTextBox.Text = "0";
			// 
			// label19
			// 
			this.label19.AutoSize = true;
			this.label19.Location = new System.Drawing.Point(230, 171);
			this.label19.Name = "label19";
			this.label19.Size = new System.Drawing.Size(29, 12);
			this.label19.TabIndex = 37;
			this.label19.Text = "対空";
			// 
			// label20
			// 
			this.label20.AutoSize = true;
			this.label20.Location = new System.Drawing.Point(150, 121);
			this.label20.Name = "label20";
			this.label20.Size = new System.Drawing.Size(29, 12);
			this.label20.TabIndex = 35;
			this.label20.Text = "耐久";
			// 
			// KTorpedoTextBox
			// 
			this.KTorpedoTextBox.Location = new System.Drawing.Point(265, 143);
			this.KTorpedoTextBox.Name = "KTorpedoTextBox";
			this.KTorpedoTextBox.Size = new System.Drawing.Size(39, 19);
			this.KTorpedoTextBox.TabIndex = 13;
			this.KTorpedoTextBox.Text = "0";
			// 
			// label21
			// 
			this.label21.AutoSize = true;
			this.label21.Location = new System.Drawing.Point(230, 146);
			this.label21.Name = "label21";
			this.label21.Size = new System.Drawing.Size(29, 12);
			this.label21.TabIndex = 33;
			this.label21.Text = "雷装";
			// 
			// KAttackTextBox
			// 
			this.KAttackTextBox.Location = new System.Drawing.Point(265, 118);
			this.KAttackTextBox.Name = "KAttackTextBox";
			this.KAttackTextBox.Size = new System.Drawing.Size(39, 19);
			this.KAttackTextBox.TabIndex = 11;
			this.KAttackTextBox.Text = "0";
			// 
			// label22
			// 
			this.label22.AutoSize = true;
			this.label22.Location = new System.Drawing.Point(230, 121);
			this.label22.Name = "label22";
			this.label22.Size = new System.Drawing.Size(29, 12);
			this.label22.TabIndex = 31;
			this.label22.Text = "火力";
			// 
			// KKindComboBox
			// 
			this.KKindComboBox.FormattingEnabled = true;
			this.KKindComboBox.Items.AddRange(new object[] {
            "戦艦",
            "航空戦艦",
            "正規空母",
            "装甲空母",
            "軽空母",
            "水上機母艦",
            "重巡洋艦",
            "航空巡洋艦",
            "軽巡洋艦",
            "重雷装巡洋艦",
            "駆逐艦",
            "潜水艦",
            "潜水空母",
            "輸送艦",
            "陸上棲姫",
            "護衛要塞",
            "練習巡洋艦",
            "その他"});
			this.KKindComboBox.Location = new System.Drawing.Point(194, 92);
			this.KKindComboBox.Name = "KKindComboBox";
			this.KKindComboBox.Size = new System.Drawing.Size(121, 20);
			this.KKindComboBox.TabIndex = 9;
			this.KKindComboBox.Text = "(艦種を選択)";
			// 
			// label23
			// 
			this.label23.AutoSize = true;
			this.label23.Location = new System.Drawing.Point(274, 19);
			this.label23.Name = "label23";
			this.label23.Size = new System.Drawing.Size(19, 12);
			this.label23.TabIndex = 29;
			this.label23.Text = "Lv.";
			// 
			// KNameTextBox
			// 
			this.KNameTextBox.Location = new System.Drawing.Point(194, 16);
			this.KNameTextBox.Name = "KNameTextBox";
			this.KNameTextBox.Size = new System.Drawing.Size(74, 19);
			this.KNameTextBox.TabIndex = 2;
			// 
			// label24
			// 
			this.label24.AutoSize = true;
			this.label24.Location = new System.Drawing.Point(150, 19);
			this.label24.Name = "label24";
			this.label24.Size = new System.Drawing.Size(29, 12);
			this.label24.TabIndex = 27;
			this.label24.Text = "艦名";
			// 
			// KammusuListBox
			// 
			this.KammusuListBox.FormattingEnabled = true;
			this.KammusuListBox.ItemHeight = 12;
			this.KammusuListBox.Location = new System.Drawing.Point(16, 16);
			this.KammusuListBox.Name = "KammusuListBox";
			this.KammusuListBox.Size = new System.Drawing.Size(120, 304);
			this.KammusuListBox.TabIndex = 1;
			this.KammusuListBox.SelectedIndexChanged += new System.EventHandler(this.KammusuListBox_SelectedIndexChanged);
			// 
			// FleetsTab
			// 
			this.FleetsTab.Controls.Add(this.FAllSearchTextBox);
			this.FleetsTab.Controls.Add(this.label43);
			this.FleetsTab.Controls.Add(this.FAllAntiAirTextBox);
			this.FleetsTab.Controls.Add(this.label42);
			this.FleetsTab.Controls.Add(this.FUndoButton);
			this.FleetsTab.Controls.Add(this.FUpperButton);
			this.FleetsTab.Controls.Add(this.FLowerButton);
			this.FleetsTab.Controls.Add(this.label41);
			this.FleetsTab.Controls.Add(this.FNameTextBox);
			this.FleetsTab.Controls.Add(this.label40);
			this.FleetsTab.Controls.Add(this.FHQLevelTextBox);
			this.FleetsTab.Controls.Add(this.FMinusButton);
			this.FleetsTab.Controls.Add(this.FPlusButton);
			this.FleetsTab.Controls.Add(this.FResetButton);
			this.FleetsTab.Controls.Add(this.label36);
			this.FleetsTab.Controls.Add(this.FAmmoTextBox);
			this.FleetsTab.Controls.Add(this.label35);
			this.FleetsTab.Controls.Add(this.FcondTextBox);
			this.FleetsTab.Controls.Add(this.FHPTextBox);
			this.FleetsTab.Controls.Add(this.label34);
			this.FleetsTab.Controls.Add(this.FAir4TextBox);
			this.FleetsTab.Controls.Add(this.FAir3TextBox);
			this.FleetsTab.Controls.Add(this.FAir2TextBox);
			this.FleetsTab.Controls.Add(this.FAir1TextBox);
			this.FleetsTab.Controls.Add(this.label33);
			this.FleetsTab.Controls.Add(this.label32);
			this.FleetsTab.Controls.Add(this.label31);
			this.FleetsTab.Controls.Add(this.label30);
			this.FleetsTab.Controls.Add(this.label29);
			this.FleetsTab.Controls.Add(this.label28);
			this.FleetsTab.Controls.Add(this.FSlot4ComboBox);
			this.FleetsTab.Controls.Add(this.FleetsListBox);
			this.FleetsTab.Controls.Add(this.FSlot3ComboBox);
			this.FleetsTab.Controls.Add(this.FSlot2ComboBox);
			this.FleetsTab.Controls.Add(this.FKammusuComboBox);
			this.FleetsTab.Controls.Add(this.FSlot1ComboBox);
			this.FleetsTab.Location = new System.Drawing.Point(4, 22);
			this.FleetsTab.Name = "FleetsTab";
			this.FleetsTab.Size = new System.Drawing.Size(402, 336);
			this.FleetsTab.TabIndex = 2;
			this.FleetsTab.Text = "艦隊データ";
			this.FleetsTab.UseVisualStyleBackColor = true;
			// 
			// FAllSearchTextBox
			// 
			this.FAllSearchTextBox.Enabled = false;
			this.FAllSearchTextBox.Location = new System.Drawing.Point(199, 271);
			this.FAllSearchTextBox.Name = "FAllSearchTextBox";
			this.FAllSearchTextBox.Size = new System.Drawing.Size(47, 19);
			this.FAllSearchTextBox.TabIndex = 74;
			// 
			// label43
			// 
			this.label43.AutoSize = true;
			this.label43.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label43.Location = new System.Drawing.Point(149, 274);
			this.label43.Name = "label43";
			this.label43.Size = new System.Drawing.Size(44, 12);
			this.label43.TabIndex = 73;
			this.label43.Text = "索敵値";
			// 
			// FAllAntiAirTextBox
			// 
			this.FAllAntiAirTextBox.Enabled = false;
			this.FAllAntiAirTextBox.Location = new System.Drawing.Point(199, 246);
			this.FAllAntiAirTextBox.Name = "FAllAntiAirTextBox";
			this.FAllAntiAirTextBox.Size = new System.Drawing.Size(47, 19);
			this.FAllAntiAirTextBox.TabIndex = 72;
			// 
			// label42
			// 
			this.label42.AutoSize = true;
			this.label42.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label42.Location = new System.Drawing.Point(149, 249);
			this.label42.Name = "label42";
			this.label42.Size = new System.Drawing.Size(44, 12);
			this.label42.TabIndex = 70;
			this.label42.Text = "制空値";
			// 
			// FUndoButton
			// 
			this.FUndoButton.Location = new System.Drawing.Point(311, 206);
			this.FUndoButton.Name = "FUndoButton";
			this.FUndoButton.Size = new System.Drawing.Size(75, 23);
			this.FUndoButton.TabIndex = 16;
			this.FUndoButton.Text = "元に戻す";
			this.FUndoButton.UseVisualStyleBackColor = true;
			this.FUndoButton.Click += new System.EventHandler(this.FUndoButton_Click);
			// 
			// FUpperButton
			// 
			this.FUpperButton.Location = new System.Drawing.Point(311, 235);
			this.FUpperButton.Name = "FUpperButton";
			this.FUpperButton.Size = new System.Drawing.Size(35, 23);
			this.FUpperButton.TabIndex = 17;
			this.FUpperButton.Text = "↑";
			this.FUpperButton.UseVisualStyleBackColor = true;
			this.FUpperButton.Click += new System.EventHandler(this.FUpperButton_Click);
			// 
			// FLowerButton
			// 
			this.FLowerButton.Location = new System.Drawing.Point(351, 235);
			this.FLowerButton.Name = "FLowerButton";
			this.FLowerButton.Size = new System.Drawing.Size(35, 23);
			this.FLowerButton.TabIndex = 18;
			this.FLowerButton.Text = "↓";
			this.FLowerButton.UseVisualStyleBackColor = true;
			this.FLowerButton.Click += new System.EventHandler(this.FLowerButton_Click);
			// 
			// label41
			// 
			this.label41.AutoSize = true;
			this.label41.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label41.Location = new System.Drawing.Point(149, 199);
			this.label41.Name = "label41";
			this.label41.Size = new System.Drawing.Size(44, 12);
			this.label41.TabIndex = 66;
			this.label41.Text = "艦隊名";
			// 
			// FNameTextBox
			// 
			this.FNameTextBox.Location = new System.Drawing.Point(199, 196);
			this.FNameTextBox.Name = "FNameTextBox";
			this.FNameTextBox.Size = new System.Drawing.Size(86, 19);
			this.FNameTextBox.TabIndex = 14;
			this.FNameTextBox.Text = "第1艦隊";
			// 
			// label40
			// 
			this.label40.AutoSize = true;
			this.label40.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label40.Location = new System.Drawing.Point(149, 224);
			this.label40.Name = "label40";
			this.label40.Size = new System.Drawing.Size(76, 12);
			this.label40.TabIndex = 64;
			this.label40.Text = "司令部レベル";
			// 
			// FHQLevelTextBox
			// 
			this.FHQLevelTextBox.Location = new System.Drawing.Point(231, 221);
			this.FHQLevelTextBox.Name = "FHQLevelTextBox";
			this.FHQLevelTextBox.Size = new System.Drawing.Size(39, 19);
			this.FHQLevelTextBox.TabIndex = 15;
			this.FHQLevelTextBox.Text = "100";
			// 
			// FMinusButton
			// 
			this.FMinusButton.Location = new System.Drawing.Point(311, 293);
			this.FMinusButton.Name = "FMinusButton";
			this.FMinusButton.Size = new System.Drawing.Size(75, 23);
			this.FMinusButton.TabIndex = 20;
			this.FMinusButton.Text = "削除";
			this.FMinusButton.UseVisualStyleBackColor = true;
			this.FMinusButton.Click += new System.EventHandler(this.FMinusButton_Click);
			// 
			// FPlusButton
			// 
			this.FPlusButton.Location = new System.Drawing.Point(311, 264);
			this.FPlusButton.Name = "FPlusButton";
			this.FPlusButton.Size = new System.Drawing.Size(75, 23);
			this.FPlusButton.TabIndex = 19;
			this.FPlusButton.Text = "追加・更新";
			this.FPlusButton.UseVisualStyleBackColor = true;
			this.FPlusButton.Click += new System.EventHandler(this.FPlusButton_Click);
			// 
			// FResetButton
			// 
			this.FResetButton.Location = new System.Drawing.Point(230, 293);
			this.FResetButton.Name = "FResetButton";
			this.FResetButton.Size = new System.Drawing.Size(75, 23);
			this.FResetButton.TabIndex = 21;
			this.FResetButton.Text = "リセット";
			this.FResetButton.UseVisualStyleBackColor = true;
			this.FResetButton.Click += new System.EventHandler(this.FResetButton_Click);
			// 
			// label36
			// 
			this.label36.AutoSize = true;
			this.label36.Location = new System.Drawing.Point(149, 174);
			this.label36.Name = "label36";
			this.label36.Size = new System.Drawing.Size(81, 12);
			this.label36.TabIndex = 59;
			this.label36.Text = "残り弾薬量(％)";
			// 
			// FAmmoTextBox
			// 
			this.FAmmoTextBox.Location = new System.Drawing.Point(236, 171);
			this.FAmmoTextBox.Name = "FAmmoTextBox";
			this.FAmmoTextBox.Size = new System.Drawing.Size(39, 19);
			this.FAmmoTextBox.TabIndex = 13;
			this.FAmmoTextBox.Text = "100";
			// 
			// label35
			// 
			this.label35.AutoSize = true;
			this.label35.Location = new System.Drawing.Point(244, 149);
			this.label35.Name = "label35";
			this.label35.Size = new System.Drawing.Size(41, 12);
			this.label35.TabIndex = 57;
			this.label35.Text = "cond値";
			// 
			// FcondTextBox
			// 
			this.FcondTextBox.Location = new System.Drawing.Point(291, 146);
			this.FcondTextBox.Name = "FcondTextBox";
			this.FcondTextBox.Size = new System.Drawing.Size(39, 19);
			this.FcondTextBox.TabIndex = 12;
			this.FcondTextBox.Text = "49";
			// 
			// FHPTextBox
			// 
			this.FHPTextBox.Location = new System.Drawing.Point(199, 146);
			this.FHPTextBox.Name = "FHPTextBox";
			this.FHPTextBox.Size = new System.Drawing.Size(39, 19);
			this.FHPTextBox.TabIndex = 11;
			this.FHPTextBox.Text = "0";
			// 
			// label34
			// 
			this.label34.AutoSize = true;
			this.label34.Location = new System.Drawing.Point(149, 149);
			this.label34.Name = "label34";
			this.label34.Size = new System.Drawing.Size(49, 12);
			this.label34.TabIndex = 46;
			this.label34.Text = "残り耐久";
			// 
			// FAir4TextBox
			// 
			this.FAir4TextBox.Location = new System.Drawing.Point(326, 121);
			this.FAir4TextBox.Name = "FAir4TextBox";
			this.FAir4TextBox.Size = new System.Drawing.Size(40, 19);
			this.FAir4TextBox.TabIndex = 10;
			this.FAir4TextBox.Text = "0";
			// 
			// FAir3TextBox
			// 
			this.FAir3TextBox.Location = new System.Drawing.Point(326, 95);
			this.FAir3TextBox.Name = "FAir3TextBox";
			this.FAir3TextBox.Size = new System.Drawing.Size(40, 19);
			this.FAir3TextBox.TabIndex = 8;
			this.FAir3TextBox.Text = "0";
			// 
			// FAir2TextBox
			// 
			this.FAir2TextBox.Location = new System.Drawing.Point(326, 69);
			this.FAir2TextBox.Name = "FAir2TextBox";
			this.FAir2TextBox.Size = new System.Drawing.Size(40, 19);
			this.FAir2TextBox.TabIndex = 6;
			this.FAir2TextBox.Text = "0";
			// 
			// FAir1TextBox
			// 
			this.FAir1TextBox.Location = new System.Drawing.Point(326, 43);
			this.FAir1TextBox.Name = "FAir1TextBox";
			this.FAir1TextBox.Size = new System.Drawing.Size(40, 19);
			this.FAir1TextBox.TabIndex = 4;
			this.FAir1TextBox.Text = "0";
			// 
			// label33
			// 
			this.label33.AutoSize = true;
			this.label33.Location = new System.Drawing.Point(323, 20);
			this.label33.Name = "label33";
			this.label33.Size = new System.Drawing.Size(53, 12);
			this.label33.TabIndex = 41;
			this.label33.Text = "艦載機数";
			// 
			// label32
			// 
			this.label32.AutoSize = true;
			this.label32.Location = new System.Drawing.Point(149, 123);
			this.label32.Name = "label32";
			this.label32.Size = new System.Drawing.Size(44, 12);
			this.label32.TabIndex = 40;
			this.label32.Text = "スロット4";
			// 
			// label31
			// 
			this.label31.AutoSize = true;
			this.label31.Location = new System.Drawing.Point(149, 97);
			this.label31.Name = "label31";
			this.label31.Size = new System.Drawing.Size(44, 12);
			this.label31.TabIndex = 39;
			this.label31.Text = "スロット3";
			// 
			// label30
			// 
			this.label30.AutoSize = true;
			this.label30.Location = new System.Drawing.Point(149, 71);
			this.label30.Name = "label30";
			this.label30.Size = new System.Drawing.Size(44, 12);
			this.label30.TabIndex = 38;
			this.label30.Text = "スロット2";
			// 
			// label29
			// 
			this.label29.AutoSize = true;
			this.label29.Location = new System.Drawing.Point(149, 45);
			this.label29.Name = "label29";
			this.label29.Size = new System.Drawing.Size(44, 12);
			this.label29.TabIndex = 37;
			this.label29.Text = "スロット1";
			// 
			// label28
			// 
			this.label28.AutoSize = true;
			this.label28.Location = new System.Drawing.Point(149, 20);
			this.label28.Name = "label28";
			this.label28.Size = new System.Drawing.Size(29, 12);
			this.label28.TabIndex = 36;
			this.label28.Text = "艦娘";
			// 
			// FSlot4ComboBox
			// 
			this.FSlot4ComboBox.FormattingEnabled = true;
			this.FSlot4ComboBox.Location = new System.Drawing.Point(199, 120);
			this.FSlot4ComboBox.Name = "FSlot4ComboBox";
			this.FSlot4ComboBox.Size = new System.Drawing.Size(121, 20);
			this.FSlot4ComboBox.TabIndex = 9;
			this.FSlot4ComboBox.Text = "(装備を選択)";
			// 
			// FleetsListBox
			// 
			this.FleetsListBox.FormattingEnabled = true;
			this.FleetsListBox.ItemHeight = 12;
			this.FleetsListBox.Location = new System.Drawing.Point(16, 16);
			this.FleetsListBox.Name = "FleetsListBox";
			this.FleetsListBox.Size = new System.Drawing.Size(120, 304);
			this.FleetsListBox.TabIndex = 1;
			this.FleetsListBox.SelectedIndexChanged += new System.EventHandler(this.FleetsListBox_SelectedIndexChanged);
			// 
			// FSlot3ComboBox
			// 
			this.FSlot3ComboBox.FormattingEnabled = true;
			this.FSlot3ComboBox.Location = new System.Drawing.Point(199, 94);
			this.FSlot3ComboBox.Name = "FSlot3ComboBox";
			this.FSlot3ComboBox.Size = new System.Drawing.Size(121, 20);
			this.FSlot3ComboBox.TabIndex = 7;
			this.FSlot3ComboBox.Text = "(装備を選択)";
			this.FSlot3ComboBox.SelectedIndexChanged += new System.EventHandler(this.FSlot3ComboBox_SelectedIndexChanged);
			// 
			// FSlot2ComboBox
			// 
			this.FSlot2ComboBox.FormattingEnabled = true;
			this.FSlot2ComboBox.Location = new System.Drawing.Point(199, 68);
			this.FSlot2ComboBox.Name = "FSlot2ComboBox";
			this.FSlot2ComboBox.Size = new System.Drawing.Size(121, 20);
			this.FSlot2ComboBox.TabIndex = 5;
			this.FSlot2ComboBox.Text = "(装備を選択)";
			this.FSlot2ComboBox.SelectedIndexChanged += new System.EventHandler(this.FSlot2ComboBox_SelectedIndexChanged);
			// 
			// FKammusuComboBox
			// 
			this.FKammusuComboBox.FormattingEnabled = true;
			this.FKammusuComboBox.Location = new System.Drawing.Point(184, 16);
			this.FKammusuComboBox.Name = "FKammusuComboBox";
			this.FKammusuComboBox.Size = new System.Drawing.Size(91, 20);
			this.FKammusuComboBox.TabIndex = 2;
			this.FKammusuComboBox.Text = "(艦娘を選択)";
			this.FKammusuComboBox.SelectedIndexChanged += new System.EventHandler(this.FKammusuComboBox_SelectedIndexChanged);
			// 
			// FSlot1ComboBox
			// 
			this.FSlot1ComboBox.FormattingEnabled = true;
			this.FSlot1ComboBox.Location = new System.Drawing.Point(199, 42);
			this.FSlot1ComboBox.Name = "FSlot1ComboBox";
			this.FSlot1ComboBox.Size = new System.Drawing.Size(121, 20);
			this.FSlot1ComboBox.TabIndex = 3;
			this.FSlot1ComboBox.Text = "(装備を選択)";
			this.FSlot1ComboBox.SelectedIndexChanged += new System.EventHandler(this.FSlot1ComboBox_SelectedIndexChanged);
			// 
			// SimulationTab
			// 
			this.SimulationTab.Controls.Add(this.groupBox3);
			this.SimulationTab.Controls.Add(this.CalcButton);
			this.SimulationTab.Controls.Add(this.groupBox4);
			this.SimulationTab.Controls.Add(this.groupBox2);
			this.SimulationTab.Controls.Add(this.groupBox1);
			this.SimulationTab.Location = new System.Drawing.Point(4, 22);
			this.SimulationTab.Name = "SimulationTab";
			this.SimulationTab.Size = new System.Drawing.Size(402, 336);
			this.SimulationTab.TabIndex = 3;
			this.SimulationTab.Text = "戦闘シミュレーション";
			this.SimulationTab.UseVisualStyleBackColor = true;
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.SwapCheckBox);
			this.groupBox3.Controls.Add(this.BattleCountComboBox);
			this.groupBox3.Controls.Add(this.label39);
			this.groupBox3.Controls.Add(this.For2ComboBox);
			this.groupBox3.Controls.Add(this.For1ComboBox);
			this.groupBox3.Controls.Add(this.label38);
			this.groupBox3.Controls.Add(this.label37);
			this.groupBox3.Location = new System.Drawing.Point(129, 20);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(256, 131);
			this.groupBox3.TabIndex = 4;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "シミュレーション設定";
			// 
			// SwapCheckBox
			// 
			this.SwapCheckBox.AutoSize = true;
			this.SwapCheckBox.Location = new System.Drawing.Point(65, 100);
			this.SwapCheckBox.Name = "SwapCheckBox";
			this.SwapCheckBox.Size = new System.Drawing.Size(92, 16);
			this.SwapCheckBox.TabIndex = 36;
			this.SwapCheckBox.Text = "先後入れ替え";
			this.SwapCheckBox.UseVisualStyleBackColor = true;
			// 
			// BattleCountComboBox
			// 
			this.BattleCountComboBox.FormattingEnabled = true;
			this.BattleCountComboBox.Items.AddRange(new object[] {
            "1",
            "10",
            "100",
            "1000",
            "10000",
            "100000"});
			this.BattleCountComboBox.Location = new System.Drawing.Point(65, 74);
			this.BattleCountComboBox.Name = "BattleCountComboBox";
			this.BattleCountComboBox.Size = new System.Drawing.Size(91, 20);
			this.BattleCountComboBox.TabIndex = 6;
			this.BattleCountComboBox.Text = "(回数を選択)";
			// 
			// label39
			// 
			this.label39.AutoSize = true;
			this.label39.Location = new System.Drawing.Point(6, 78);
			this.label39.Name = "label39";
			this.label39.Size = new System.Drawing.Size(53, 12);
			this.label39.TabIndex = 35;
			this.label39.Text = "試行回数";
			// 
			// For2ComboBox
			// 
			this.For2ComboBox.FormattingEnabled = true;
			this.For2ComboBox.Items.AddRange(new object[] {
            "単縦陣",
            "複縦陣",
            "輪形陣",
            "梯形陣",
            "単横陣"});
			this.For2ComboBox.Location = new System.Drawing.Point(65, 49);
			this.For2ComboBox.Name = "For2ComboBox";
			this.For2ComboBox.Size = new System.Drawing.Size(91, 20);
			this.For2ComboBox.TabIndex = 5;
			this.For2ComboBox.Text = "(陣形を選択)";
			// 
			// For1ComboBox
			// 
			this.For1ComboBox.FormattingEnabled = true;
			this.For1ComboBox.Items.AddRange(new object[] {
            "単縦陣",
            "複縦陣",
            "輪形陣",
            "梯形陣",
            "単横陣"});
			this.For1ComboBox.Location = new System.Drawing.Point(65, 24);
			this.For1ComboBox.Name = "For1ComboBox";
			this.For1ComboBox.Size = new System.Drawing.Size(91, 20);
			this.For1ComboBox.TabIndex = 4;
			this.For1ComboBox.Text = "(陣形を選択)";
			// 
			// label38
			// 
			this.label38.AutoSize = true;
			this.label38.Location = new System.Drawing.Point(6, 52);
			this.label38.Name = "label38";
			this.label38.Size = new System.Drawing.Size(53, 12);
			this.label38.TabIndex = 1;
			this.label38.Text = "後攻陣形";
			// 
			// label37
			// 
			this.label37.AutoSize = true;
			this.label37.Location = new System.Drawing.Point(6, 27);
			this.label37.Name = "label37";
			this.label37.Size = new System.Drawing.Size(53, 12);
			this.label37.TabIndex = 0;
			this.label37.Text = "先攻陣形";
			// 
			// CalcButton
			// 
			this.CalcButton.Location = new System.Drawing.Point(25, 294);
			this.CalcButton.Name = "CalcButton";
			this.CalcButton.Size = new System.Drawing.Size(91, 23);
			this.CalcButton.TabIndex = 8;
			this.CalcButton.Text = "計算開始";
			this.CalcButton.UseVisualStyleBackColor = true;
			this.CalcButton.Click += new System.EventHandler(this.CalcButton_Click);
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.OutputTextBox);
			this.groupBox4.Location = new System.Drawing.Point(129, 157);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(256, 160);
			this.groupBox4.TabIndex = 3;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "シミュレーション結果";
			// 
			// OutputTextBox
			// 
			this.OutputTextBox.Location = new System.Drawing.Point(8, 18);
			this.OutputTextBox.Multiline = true;
			this.OutputTextBox.Name = "OutputTextBox";
			this.OutputTextBox.ReadOnly = true;
			this.OutputTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.OutputTextBox.Size = new System.Drawing.Size(242, 136);
			this.OutputTextBox.TabIndex = 35;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.Read2Button);
			this.groupBox2.Controls.Add(this.Fleets2ListBox);
			this.groupBox2.Location = new System.Drawing.Point(19, 157);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(104, 131);
			this.groupBox2.TabIndex = 2;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "第2艦隊(後攻)";
			// 
			// Read2Button
			// 
			this.Read2Button.Location = new System.Drawing.Point(6, 100);
			this.Read2Button.Name = "Read2Button";
			this.Read2Button.Size = new System.Drawing.Size(91, 23);
			this.Read2Button.TabIndex = 3;
			this.Read2Button.Text = "読み込み";
			this.Read2Button.UseVisualStyleBackColor = true;
			this.Read2Button.Click += new System.EventHandler(this.Read2Button_Click);
			// 
			// Fleets2ListBox
			// 
			this.Fleets2ListBox.FormattingEnabled = true;
			this.Fleets2ListBox.ItemHeight = 12;
			this.Fleets2ListBox.Location = new System.Drawing.Point(6, 18);
			this.Fleets2ListBox.Name = "Fleets2ListBox";
			this.Fleets2ListBox.Size = new System.Drawing.Size(91, 76);
			this.Fleets2ListBox.TabIndex = 2;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.Read1Button);
			this.groupBox1.Controls.Add(this.Fleets1ListBox);
			this.groupBox1.Location = new System.Drawing.Point(19, 20);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(104, 131);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "第1艦隊(先攻)";
			// 
			// Read1Button
			// 
			this.Read1Button.Location = new System.Drawing.Point(6, 100);
			this.Read1Button.Name = "Read1Button";
			this.Read1Button.Size = new System.Drawing.Size(91, 23);
			this.Read1Button.TabIndex = 1;
			this.Read1Button.Text = "読み込み";
			this.Read1Button.UseVisualStyleBackColor = true;
			this.Read1Button.Click += new System.EventHandler(this.Read1Button_Click);
			// 
			// Fleets1ListBox
			// 
			this.Fleets1ListBox.FormattingEnabled = true;
			this.Fleets1ListBox.ItemHeight = 12;
			this.Fleets1ListBox.Location = new System.Drawing.Point(6, 18);
			this.Fleets1ListBox.Name = "Fleets1ListBox";
			this.Fleets1ListBox.Size = new System.Drawing.Size(91, 76);
			this.Fleets1ListBox.TabIndex = 0;
			// 
			// MapModeTab
			// 
			this.MapModeTab.Controls.Add(this.groupBox8);
			this.MapModeTab.Controls.Add(this.groupBox7);
			this.MapModeTab.Controls.Add(this.CalcButton2);
			this.MapModeTab.Controls.Add(this.groupBox6);
			this.MapModeTab.Controls.Add(this.groupBox5);
			this.MapModeTab.Location = new System.Drawing.Point(4, 22);
			this.MapModeTab.Name = "MapModeTab";
			this.MapModeTab.Padding = new System.Windows.Forms.Padding(3);
			this.MapModeTab.Size = new System.Drawing.Size(402, 336);
			this.MapModeTab.TabIndex = 4;
			this.MapModeTab.Text = "海域シミュレーション";
			this.MapModeTab.UseVisualStyleBackColor = true;
			// 
			// groupBox8
			// 
			this.groupBox8.Controls.Add(this.OutputTextBox2);
			this.groupBox8.Location = new System.Drawing.Point(129, 120);
			this.groupBox8.Name = "groupBox8";
			this.groupBox8.Size = new System.Drawing.Size(256, 197);
			this.groupBox8.TabIndex = 11;
			this.groupBox8.TabStop = false;
			this.groupBox8.Text = "シミュレーション結果";
			// 
			// OutputTextBox2
			// 
			this.OutputTextBox2.Location = new System.Drawing.Point(8, 18);
			this.OutputTextBox2.Multiline = true;
			this.OutputTextBox2.Name = "OutputTextBox2";
			this.OutputTextBox2.ReadOnly = true;
			this.OutputTextBox2.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.OutputTextBox2.Size = new System.Drawing.Size(242, 173);
			this.OutputTextBox2.TabIndex = 35;
			// 
			// groupBox7
			// 
			this.groupBox7.Controls.Add(this.BattleCountComboBox2);
			this.groupBox7.Controls.Add(this.label44);
			this.groupBox7.Controls.Add(this.For3ComboBox);
			this.groupBox7.Controls.Add(this.label46);
			this.groupBox7.Location = new System.Drawing.Point(129, 20);
			this.groupBox7.Name = "groupBox7";
			this.groupBox7.Size = new System.Drawing.Size(256, 94);
			this.groupBox7.TabIndex = 10;
			this.groupBox7.TabStop = false;
			this.groupBox7.Text = "シミュレーション設定";
			// 
			// BattleCountComboBox2
			// 
			this.BattleCountComboBox2.FormattingEnabled = true;
			this.BattleCountComboBox2.Items.AddRange(new object[] {
            "1",
            "10",
            "100",
            "1000",
            "10000",
            "100000"});
			this.BattleCountComboBox2.Location = new System.Drawing.Point(89, 49);
			this.BattleCountComboBox2.Name = "BattleCountComboBox2";
			this.BattleCountComboBox2.Size = new System.Drawing.Size(91, 20);
			this.BattleCountComboBox2.TabIndex = 6;
			this.BattleCountComboBox2.Text = "(回数を選択)";
			// 
			// label44
			// 
			this.label44.AutoSize = true;
			this.label44.Location = new System.Drawing.Point(30, 52);
			this.label44.Name = "label44";
			this.label44.Size = new System.Drawing.Size(53, 12);
			this.label44.TabIndex = 35;
			this.label44.Text = "試行回数";
			// 
			// For3ComboBox
			// 
			this.For3ComboBox.FormattingEnabled = true;
			this.For3ComboBox.Items.AddRange(new object[] {
            "単縦陣",
            "複縦陣",
            "輪形陣",
            "梯形陣",
            "単横陣"});
			this.For3ComboBox.Location = new System.Drawing.Point(89, 24);
			this.For3ComboBox.Name = "For3ComboBox";
			this.For3ComboBox.Size = new System.Drawing.Size(91, 20);
			this.For3ComboBox.TabIndex = 4;
			this.For3ComboBox.Text = "(陣形を選択)";
			// 
			// label46
			// 
			this.label46.AutoSize = true;
			this.label46.Location = new System.Drawing.Point(6, 27);
			this.label46.Name = "label46";
			this.label46.Size = new System.Drawing.Size(77, 12);
			this.label46.TabIndex = 0;
			this.label46.Text = "開幕夜戦陣形";
			// 
			// CalcButton2
			// 
			this.CalcButton2.Location = new System.Drawing.Point(25, 294);
			this.CalcButton2.Name = "CalcButton2";
			this.CalcButton2.Size = new System.Drawing.Size(91, 23);
			this.CalcButton2.TabIndex = 9;
			this.CalcButton2.Text = "計算開始";
			this.CalcButton2.UseVisualStyleBackColor = true;
			this.CalcButton2.Click += new System.EventHandler(this.CalcButton2_Click);
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.Read4Button);
			this.groupBox6.Controls.Add(this.PatternListBox);
			this.groupBox6.Location = new System.Drawing.Point(19, 157);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(104, 131);
			this.groupBox6.TabIndex = 3;
			this.groupBox6.TabStop = false;
			this.groupBox6.Text = "出撃する海域";
			// 
			// Read4Button
			// 
			this.Read4Button.Location = new System.Drawing.Point(6, 100);
			this.Read4Button.Name = "Read4Button";
			this.Read4Button.Size = new System.Drawing.Size(91, 23);
			this.Read4Button.TabIndex = 3;
			this.Read4Button.Text = "読み込み";
			this.Read4Button.UseVisualStyleBackColor = true;
			this.Read4Button.Click += new System.EventHandler(this.Read4Button_Click);
			// 
			// PatternListBox
			// 
			this.PatternListBox.FormattingEnabled = true;
			this.PatternListBox.ItemHeight = 12;
			this.PatternListBox.Location = new System.Drawing.Point(6, 18);
			this.PatternListBox.Name = "PatternListBox";
			this.PatternListBox.Size = new System.Drawing.Size(91, 76);
			this.PatternListBox.TabIndex = 2;
			// 
			// groupBox5
			// 
			this.groupBox5.Controls.Add(this.Read3Button);
			this.groupBox5.Controls.Add(this.Fleets3ListBox);
			this.groupBox5.Location = new System.Drawing.Point(19, 20);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(104, 131);
			this.groupBox5.TabIndex = 2;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "出撃する艦隊";
			// 
			// Read3Button
			// 
			this.Read3Button.Location = new System.Drawing.Point(6, 100);
			this.Read3Button.Name = "Read3Button";
			this.Read3Button.Size = new System.Drawing.Size(91, 23);
			this.Read3Button.TabIndex = 1;
			this.Read3Button.Text = "読み込み";
			this.Read3Button.UseVisualStyleBackColor = true;
			this.Read3Button.Click += new System.EventHandler(this.Read3Button_Click);
			// 
			// Fleets3ListBox
			// 
			this.Fleets3ListBox.FormattingEnabled = true;
			this.Fleets3ListBox.ItemHeight = 12;
			this.Fleets3ListBox.Location = new System.Drawing.Point(6, 18);
			this.Fleets3ListBox.Name = "Fleets3ListBox";
			this.Fleets3ListBox.Size = new System.Drawing.Size(91, 76);
			this.Fleets3ListBox.TabIndex = 0;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(434, 412);
			this.Controls.Add(this.TabControl);
			this.Controls.Add(this.MenuStrip);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MainMenuStrip = this.MenuStrip;
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.Text = "艦これシミュレータ";
			this.MenuStrip.ResumeLayout(false);
			this.MenuStrip.PerformLayout();
			this.TabControl.ResumeLayout(false);
			this.WeaponTab.ResumeLayout(false);
			this.WeaponTab.PerformLayout();
			this.KammusuTab.ResumeLayout(false);
			this.KammusuTab.PerformLayout();
			this.FleetsTab.ResumeLayout(false);
			this.FleetsTab.PerformLayout();
			this.SimulationTab.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.MapModeTab.ResumeLayout(false);
			this.groupBox8.ResumeLayout(false);
			this.groupBox8.PerformLayout();
			this.groupBox7.ResumeLayout(false);
			this.groupBox7.PerformLayout();
			this.groupBox6.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MenuStrip;
        private System.Windows.Forms.TabControl TabControl;
        private System.Windows.Forms.TabPage WeaponTab;
        private System.Windows.Forms.TabPage KammusuTab;
        private System.Windows.Forms.ToolStripMenuItem FileMenu;
        private System.Windows.Forms.ToolStripMenuItem FileOpenMenu;
        private System.Windows.Forms.ToolStripMenuItem FileSaveSMenu;
        private System.Windows.Forms.ToolStripMenuItem FileSaveAMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem FileCloseMenu;
        private System.Windows.Forms.TabPage FleetsTab;
        private System.Windows.Forms.TabPage SimulationTab;
        private System.Windows.Forms.ListBox WeaponListBox;
        private System.Windows.Forms.TextBox WDefenseTextBox;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox WRangeComboBox;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox WEvadeTextBox;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox WHitTextBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox WSearchTextBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox WAntiSubTextBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox WAntiAirTextBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox WBombTextBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox WTorpedoTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox WAttackTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox WTypeComboBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox WNameTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button WSaveButton;
        private System.Windows.Forms.Button WUndoButton;
        private System.Windows.Forms.ListBox KammusuListBox;
        private System.Windows.Forms.Button KUndoButton;
        private System.Windows.Forms.Button KSaveButton;
        private System.Windows.Forms.TextBox KDefenseTextBox;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox KRangeComboBox;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox KEvadeTextBox;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox KSearchTextBox;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox KAntiSubTextBox;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox KAntiAirTextBox;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox KTorpedoTextBox;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox KAttackTextBox;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.ComboBox KKindComboBox;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox KNameTextBox;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox KLevelTextBox;
        private System.Windows.Forms.TextBox KMaxHPTextBox;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.ComboBox KSpeedComboBox;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.TextBox KLuckTextBox;
        private System.Windows.Forms.ComboBox KSlotsComboBox;
        private System.Windows.Forms.ListBox FleetsListBox;
        private System.Windows.Forms.ComboBox FSlot4ComboBox;
        private System.Windows.Forms.ComboBox FSlot3ComboBox;
        private System.Windows.Forms.ComboBox FSlot2ComboBox;
        private System.Windows.Forms.ComboBox FKammusuComboBox;
        private System.Windows.Forms.ComboBox FSlot1ComboBox;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.TextBox FAir4TextBox;
        private System.Windows.Forms.TextBox FAir3TextBox;
        private System.Windows.Forms.TextBox FAir2TextBox;
        private System.Windows.Forms.TextBox FAir1TextBox;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TextBox FHPTextBox;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.TextBox FAmmoTextBox;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.TextBox FcondTextBox;
        private System.Windows.Forms.Button FMinusButton;
        private System.Windows.Forms.Button FPlusButton;
        private System.Windows.Forms.Button FResetButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button Read2Button;
        private System.Windows.Forms.ListBox Fleets2ListBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button Read1Button;
        private System.Windows.Forms.ListBox Fleets1ListBox;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button CalcButton;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox For2ComboBox;
        private System.Windows.Forms.ComboBox For1ComboBox;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.TextBox OutputTextBox;
        private System.Windows.Forms.ToolStripMenuItem ヘルプHToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem VerInfoMenu;
		private System.Windows.Forms.ComboBox BattleCountComboBox;
		private System.Windows.Forms.Label label40;
		private System.Windows.Forms.TextBox FHQLevelTextBox;
		private System.Windows.Forms.Label label41;
		private System.Windows.Forms.TextBox FNameTextBox;
		private System.Windows.Forms.Button FUpperButton;
		private System.Windows.Forms.Button FLowerButton;
		private System.Windows.Forms.Button WMinusButton;
		private System.Windows.Forms.Button KMinusButton;
		private System.Windows.Forms.Button WReloadButton;
		private System.Windows.Forms.Button KReloadButton;
		private System.Windows.Forms.Button FUndoButton;
		private System.Windows.Forms.Button WPlusButton;
		private System.Windows.Forms.Button KPlusButton;
		private System.Windows.Forms.TextBox KAir3TextBox;
		private System.Windows.Forms.TextBox KAir2TextBox;
		private System.Windows.Forms.TextBox KAir1TextBox;
		private System.Windows.Forms.TextBox KAir4TextBox;
		private System.Windows.Forms.Label label42;
		private System.Windows.Forms.TextBox FAllSearchTextBox;
		private System.Windows.Forms.Label label43;
		private System.Windows.Forms.TextBox FAllAntiAirTextBox;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		private System.Windows.Forms.CheckBox SwapCheckBox;
        private System.Windows.Forms.TabPage MapModeTab;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.TextBox OutputTextBox2;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.ComboBox BattleCountComboBox2;
        private System.Windows.Forms.Label label44;
        private System.Windows.Forms.ComboBox For3ComboBox;
        private System.Windows.Forms.Label label46;
        private System.Windows.Forms.Button CalcButton2;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button Read4Button;
        private System.Windows.Forms.ListBox PatternListBox;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button Read3Button;
        private System.Windows.Forms.ListBox Fleets3ListBox;
    }
}

