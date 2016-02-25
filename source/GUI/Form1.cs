/* 残り作業：
 * ・メニューボタンの反映
 * ・KanColleSimとの連携(ファイルを投げる機能)
 * ・Readmeの作成(例外処理の次に大変かも)
 * ・例外処理(一番面倒かも。ファイル読み込み・保存・数値等データ編集・シミュ実行など多岐に渡るから)
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace KanColleSimGUI {
	public partial class MainForm : Form {
		/* 内部変数(実質グローバル変数) */
		public string SoftName = "艦これシミュレータカッコカリ";
		public const int MaxSlots = 4, MaxKammusues = 6;	//スロット最大数・1艦隊の艦娘最大数
		public const bool USE_NEW_SEARCH_METHOD = true;		//2-5式索敵計算を最新版にするか
		//開いているファイル名
		public string FilePath = "", FilePath1 = "", FilePath2 = "";
		//装備・艦娘一覧
		public DataTable WeaponData = new DataTable();
		public DataTable KammusuData = new DataTable();
		//艦隊データ
		List<KammusuNow> FleetsData = new List<KammusuNow>();
		//コンボボックス用リスト
		public Dictionary<string, int> TypeList   = new Dictionary<string, int>();	//種別
		public Dictionary<string, int> WRangeList = new Dictionary<string, int>();	//射程(装備)
		public Dictionary<string, int> KRangeList = new Dictionary<string, int>();	//射程(艦娘)
		public Dictionary<string, int> KindList   = new Dictionary<string, int>();	//艦種
		public Dictionary<string, int> SpeedList  = new Dictionary<string, int>();	//速力

		/* プログラム起動時の動作 */
		public MainForm() {
			InitializeComponent();
			try{
				InitializeData();
				ReadWeaponData();
				ReadKammusuData();
				DrawWeaponData();
				DrawKammusuData();
			}catch(Exception ex) {
				MessageBox.Show(ex.Message, SoftName, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/* 「装備データ」タブ */
		private void WPlusButton_Click(object sender, EventArgs e) {
			/* 追加(項目を新規作成する) */
			if (WTypeComboBox.SelectedIndex == -1) return;
			if (WRangeComboBox.SelectedIndex == -1) return;
			DataRow dr = WeaponData.NewRow();
			dr["No"] = WeaponData.Rows.Count + 1;
			dr["名前"] = WNameTextBox.Text;
			dr["種別"] = WTypeComboBox.Text;
			dr["火力"] = WAttackTextBox.Text;
			dr["雷装"] = WTorpedoTextBox.Text;
			dr["爆装"] = WBombTextBox.Text;
			dr["対空"] = WAntiAirTextBox.Text;
			dr["対潜"] = WAntiSubTextBox.Text;
			dr["索敵"] = WSearchTextBox.Text;
			dr["命中"] = WHitTextBox.Text;
			dr["回避"] = WEvadeTextBox.Text;
			dr["射程"] = WRangeComboBox.Text;
			dr["装甲"] = WDefenseTextBox.Text;
			WeaponData.Rows.Add(dr);
			DrawWeaponData();
		}
		private void WMinusButton_Click(object sender, EventArgs e) {
			/* 削除(当該項目を削除する) */
			if (WeaponListBox.SelectedIndex != -1) {
				WeaponData.Rows.RemoveAt(WeaponListBox.SelectedIndex);
				DrawWeaponData();
			}
		}
		private void WUndoButton_Click(object sender, EventArgs e) {
			/* 元に戻す(変更を反映せずに戻す) */
			WeaponListBox_SelectedIndexChanged(sender, e);
		}
		private void WReloadButton_Click(object sender, EventArgs e) {
			/* 再読み込み(ファイルからロードし直す) */
			ReadWeaponData();
			DrawWeaponData();
			//入力欄も初期化しておく
			WNameTextBox.Text = "";
			WTypeComboBox.SelectedIndex = -1;
			WTypeComboBox.Text = "(種別を選択)";
			WAttackTextBox.Text = "0";
			WTorpedoTextBox.Text = "0";
			WBombTextBox.Text = "0";
			WAntiAirTextBox.Text = "0";
			WAntiSubTextBox.Text = "0";
			WSearchTextBox.Text = "0";
			WEvadeTextBox.Text = "0";
			WHitTextBox.Text = "0";
			WRangeComboBox.SelectedIndex = -1;
			WRangeComboBox.Text = "(射程を選択)";
			WDefenseTextBox.Text = "0";
		}
		private void WSaveButton_Click(object sender, EventArgs e) {
			/* 上書き保存(変更を反映する) */
			//変更を反映
			if (WeaponListBox.SelectedIndex != -1) {
				int i = WeaponListBox.SelectedIndex;
				DataRow[] dr = WeaponData.Select();
				dr[i]["名前"] = WNameTextBox.Text;		//名前
				dr[i]["火力"] = WAttackTextBox.Text;	//火力
				dr[i]["雷装"] = WTorpedoTextBox.Text;	//雷装
				dr[i]["爆装"] = WBombTextBox.Text;		//爆装
				dr[i]["対空"] = WAntiAirTextBox.Text;	//対空
				dr[i]["対潜"] = WAntiSubTextBox.Text;	//対潜
				dr[i]["索敵"] = WSearchTextBox.Text;	//索敵
				dr[i]["命中"] = WHitTextBox.Text;		//命中
				dr[i]["回避"] = WEvadeTextBox.Text;		//回避
				dr[i]["装甲"] = WDefenseTextBox.Text;	//装甲
				//種別
				dr[i]["種別"] = TypeList.FirstOrDefault(x => x.Value == WTypeComboBox.SelectedIndex).Key;
				//種別
				dr[i]["射程"] = WRangeList.FirstOrDefault(x => x.Value == WRangeComboBox.SelectedIndex).Key;
			}
			//書き込み準備
			StringBuilder WeaponDataCSV = new StringBuilder();
			//1行目
			foreach(DataColumn dc in WeaponData.Columns){
				if(WeaponData.Columns.IndexOf(dc) != 0){
					WeaponDataCSV.Append(",");
				}
				WeaponDataCSV.Append(dc.ColumnName);
			}
			WeaponDataCSV.Append("\n");
			//2行目以降
			foreach (DataRow dr in WeaponData.Rows) {
				for (int i = 0; i < dr.ItemArray.Length; ++i) {
					if (i == 0) {
						WeaponDataCSV.Append((WeaponData.Rows.IndexOf(dr) + 1).ToString());
					} else {
						WeaponDataCSV.Append(",");
						WeaponDataCSV.Append(dr.ItemArray[i].ToString());
					}
				}
				WeaponDataCSV.Append("\n");
			}
			//保存
			string csvDir = System.IO.Directory.GetCurrentDirectory();
			string csvFileName = "weapon.csv";
			StreamWriter sw = new StreamWriter(csvDir + "\\" + csvFileName, false, Encoding.GetEncoding("shift_jis"));
			sw.Write(WeaponDataCSV);
			sw.Close();
			//反映
			WReloadButton_Click(sender, e);
		}
		private void WeaponListBox_SelectedIndexChanged(object sender, EventArgs e) {
			/* 左欄をクリックした際の処理 */
			if (WeaponListBox.SelectedIndex != -1){
				int i = WeaponListBox.SelectedIndex;
				DataRow[] dr = WeaponData.Select();
				WNameTextBox.Text    = dr[i]["名前"].ToString();	//名前
				WAttackTextBox.Text  = dr[i]["火力"].ToString();	//火力
				WTorpedoTextBox.Text = dr[i]["雷装"].ToString();	//雷装
				WBombTextBox.Text    = dr[i]["爆装"].ToString();	//爆装
				WAntiAirTextBox.Text = dr[i]["対空"].ToString();	//対空
				WAntiSubTextBox.Text = dr[i]["対潜"].ToString();	//対潜
				WSearchTextBox.Text  = dr[i]["索敵"].ToString();	//索敵
				WHitTextBox.Text     = dr[i]["命中"].ToString();	//命中
				WEvadeTextBox.Text   = dr[i]["回避"].ToString();	//回避
				WDefenseTextBox.Text = dr[i]["装甲"].ToString();	//装甲
				//種別
				if(TypeList.ContainsKey((string)dr[i]["種別"])){
					WTypeComboBox.SelectedIndex = TypeList[(string)dr[i]["種別"]];
				}else{
					WTypeComboBox.SelectedIndex = TypeList["その他"];
				}
				//射程
				WRangeComboBox.SelectedIndex = WRangeList[(string)dr[i]["射程"]];
			}
		}

		/* 「艦娘データ」タブ */
		private void KPlusButton_Click(object sender, EventArgs e) {
			/* 追加(項目を新規作成する) */
			if (KKindComboBox.SelectedIndex == -1) return;
			if (KSpeedComboBox.SelectedIndex == -1) return;
			if (KRangeComboBox.SelectedIndex == -1) return;
			if (KSlotsComboBox.SelectedIndex == -1) return;
			DataRow dr = KammusuData.NewRow();
			dr["No"]   = KammusuData.Rows.Count + 1;
			dr["艦名"] = KNameTextBox.Text;
			dr["艦種"] = KKindComboBox.Text;
			dr["Lv"]   = KLevelTextBox.Text;
			dr["耐久"] = KMaxHPTextBox.Text;
			dr["火力"] = KAttackTextBox.Text;
			dr["装甲"] = KDefenseTextBox.Text;
			dr["雷装"] = KTorpedoTextBox.Text;
			dr["回避"] = KEvadeTextBox.Text;
			dr["対空"] = KAntiAirTextBox.Text;
			dr["対潜"] = KAntiSubTextBox.Text;
			dr["速力"] = KSpeedComboBox.Text;
			dr["索敵"] = KSearchTextBox.Text;
			dr["射程"] = KRangeComboBox.Text;
			dr["運"]   = KLuckTextBox.Text;
			dr["スロット数"] = KSlotsComboBox.Text;
			dr["艦1"] = KAir1TextBox.Text;
			dr["艦2"] = KAir2TextBox.Text;
			dr["艦3"] = KAir3TextBox.Text;
			dr["艦4"] = KAir4TextBox.Text;
			KammusuData.Rows.Add(dr);
			DrawKammusuData();
		}
		private void KMinusButton_Click(object sender, EventArgs e) {
			/* 削除(当該項目を削除する) */
			if (KammusuListBox.SelectedIndex != -1) {
				KammusuData.Rows.RemoveAt(KammusuListBox.SelectedIndex);
				DrawKammusuData();
			}
		}
		private void KUndoButton_Click(object sender, EventArgs e) {
			/* 元に戻す(変更を反映せずに戻す) */
			KammusuListBox_SelectedIndexChanged(sender, e);
		}
		private void KReloadButton_Click(object sender, EventArgs e) {
			/* 再読み込み(ファイルからロードし直す) */
			ReadKammusuData();
			DrawKammusuData();
			//入力欄も初期化しておく
			KNameTextBox.Text = "";
			KLevelTextBox.Text = "";
			KSlotsComboBox.SelectedIndex = -1;
			KSlotsComboBox.Text = "(スロット数)";
			KAir1TextBox.Text = "";
			KAir2TextBox.Text = "";
			KAir3TextBox.Text = "";
			KAir4TextBox.Text = "";
			KKindComboBox.SelectedIndex = -1;
			KKindComboBox.Text = "(艦種を選択)";
			KMaxHPTextBox.Text = "";
			KAttackTextBox.Text = "";
			KDefenseTextBox.Text = "";
			KTorpedoTextBox.Text = "";
			KEvadeTextBox.Text = "";
			KAntiAirTextBox.Text = "";
			KSpeedComboBox.SelectedIndex = -1;
			KSpeedComboBox.Text = "(速力を選択)";
			KAntiSubTextBox.Text = "";
			KRangeComboBox.SelectedIndex = -1;
			KRangeComboBox.Text = "(射程を選択)";
			KSearchTextBox.Text = "";
			KLuckTextBox.Text = "";
		}
		private void KSaveButton_Click(object sender, EventArgs e) {
			/* 上書き保存(変更を反映する) */
			// 変更を反映
			if (KammusuListBox.SelectedIndex != -1) {
				int i = KammusuListBox.SelectedIndex;
				DataRow[] dr = KammusuData.Select();
				dr[i]["艦名"] = KNameTextBox.Text;		//艦名
				dr[i]["Lv"]   = KLevelTextBox.Text;		//Lv
				dr[i]["耐久"] = KMaxHPTextBox.Text;		//耐久
				dr[i]["火力"] = KAttackTextBox.Text;	//火力
				dr[i]["装甲"] = KDefenseTextBox.Text;	//装甲
				dr[i]["雷装"] = KTorpedoTextBox.Text;	//雷装
				dr[i]["回避"] = KEvadeTextBox.Text;		//回避
				dr[i]["対空"] = KAntiAirTextBox.Text;	//対空
				dr[i]["対潜"] = KAntiSubTextBox.Text;	//対潜
				dr[i]["索敵"] = KSearchTextBox.Text;	//索敵
				dr[i]["運"]   = KLuckTextBox.Text;		//運
				//スロット数
				dr[i]["スロット数"] = KSlotsComboBox.SelectedIndex;
				dr[i]["艦1"] = KAir1TextBox.Text;
				dr[i]["艦2"] = KAir2TextBox.Text;
				dr[i]["艦3"] = KAir3TextBox.Text;
				dr[i]["艦4"] = KAir4TextBox.Text;
				//艦種
				dr[i]["艦種"] = KindList.FirstOrDefault(x => x.Value == KKindComboBox.SelectedIndex).Key;
				//速力
				dr[i]["速力"] = SpeedList.FirstOrDefault(x => x.Value == KSpeedComboBox.SelectedIndex).Key;
				//射程
				dr[i]["射程"] = KRangeList.FirstOrDefault(x => x.Value == KRangeComboBox.SelectedIndex).Key;
			}
			// 書き込み準備
			StringBuilder KammusuDataCSV = new StringBuilder();
			//スロット数による微調整
			DataRow[] dr_ = KammusuData.Select();
			for (int i = 0; i < dr_.Length; ++i) {
				int Slots = int.Parse(dr_[i]["スロット数"].ToString());
				for (int k = 0; k < MaxSlots; ++k) {
					if (k >= Slots) {
						//無効化する
						switch (k) {
							case 0:
								dr_[i]["艦1"] = "0";
								break;
							case 1:
								dr_[i]["艦2"] = "0";
								break;
							case 2:
								dr_[i]["艦3"] = "0";
								break;
							case 3:
								dr_[i]["艦4"] = "0";
								break;
						}
					}
				}
			}
			//1行目
			foreach (DataColumn dc in KammusuData.Columns) {
				if (KammusuData.Columns.IndexOf(dc) != 0) {
					KammusuDataCSV.Append(",");
				}
				KammusuDataCSV.Append(dc.ColumnName);
			}
			KammusuDataCSV.Append("\n");
			//2行目以降
			foreach (DataRow dr in KammusuData.Rows) {
				for (int i = 0; i < dr.ItemArray.Length; ++i) {
					if (i == 0) {
						KammusuDataCSV.Append((KammusuData.Rows.IndexOf(dr) + 1).ToString());
					} else {
						KammusuDataCSV.Append(",");
						KammusuDataCSV.Append(dr.ItemArray[i].ToString());
					}
				}
				KammusuDataCSV.Append("\n");
			}
			//保存
			string csvDir = System.IO.Directory.GetCurrentDirectory();
			string csvFileName = "kammusu.csv";
			StreamWriter sw = new StreamWriter(csvDir + "\\" + csvFileName, false, Encoding.GetEncoding("shift_jis"));
			sw.Write(KammusuDataCSV);
			sw.Close();
			//反映
			KReloadButton_Click(sender, e);
		}
		private void KammusuListBox_SelectedIndexChanged(object sender, EventArgs e) {
			/* 左欄をクリックした際の処理 */
			if (KammusuListBox.SelectedIndex != -1)
			{
				int i = KammusuListBox.SelectedIndex;
				DataRow[] dr = KammusuData.Select();
				KNameTextBox.Text    = dr[i]["艦名"].ToString();	//艦名
				KLevelTextBox.Text   = dr[i]["Lv"].ToString();		//Lv
				KMaxHPTextBox.Text   = dr[i]["耐久"].ToString();	//耐久
				KAttackTextBox.Text  = dr[i]["火力"].ToString();	//火力
				KDefenseTextBox.Text = dr[i]["装甲"].ToString();	//装甲
				KTorpedoTextBox.Text = dr[i]["雷装"].ToString();	//雷装
				KEvadeTextBox.Text   = dr[i]["回避"].ToString();	//回避
				KAntiAirTextBox.Text = dr[i]["対空"].ToString();	//対空
				KAntiSubTextBox.Text = dr[i]["対潜"].ToString();	//対潜
				KSearchTextBox.Text  = dr[i]["索敵"].ToString();	//索敵
				KLuckTextBox.Text    = dr[i]["運"].ToString();		//運
				//スロット数
				KSlotsComboBox.SelectedIndex = int.Parse(dr[i]["スロット数"].ToString());
				KSlotsComboBox_SelectedIndexChanged(sender, e);
				//艦種
				if (KindList.ContainsKey((string)dr[i]["艦種"])){
					KKindComboBox.SelectedIndex = KindList[(string)dr[i]["艦種"]];
				}else{
					KKindComboBox.SelectedIndex = KindList["その他"];
				}
				//速力
				KSpeedComboBox.SelectedIndex = SpeedList[(string)dr[i]["速力"]];
				//射程
				KRangeComboBox.SelectedIndex = KRangeList[(string)dr[i]["射程"]];
			}
		}
		private void KSlotsComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			/* スロット数を変更した時の処理 */
			if (KammusuListBox.SelectedIndex != -1) {
				int i = KammusuListBox.SelectedIndex;
				DataRow[] dr = KammusuData.Select();
				//スロット関係
				int Slots = KSlotsComboBox.SelectedIndex;
				for (int k = 0; k < MaxSlots; ++k) {
					if (k < Slots) {
						//適当に初期化する
						switch (k) {
							case 0:
								KAir1TextBox.Enabled = true;
								KAir1TextBox.Text = dr[i]["艦1"].ToString();
								break;
							case 1:
								KAir2TextBox.Enabled = true;
								KAir2TextBox.Text = dr[i]["艦2"].ToString();
								break;
							case 2:
								KAir3TextBox.Enabled = true;
								KAir3TextBox.Text = dr[i]["艦3"].ToString();
								break;
							case 3:
								KAir4TextBox.Enabled = true;
								KAir4TextBox.Text = dr[i]["艦4"].ToString();
								break;
						}
					}
					else {
						//無効化する
						switch (k) {
							case 0:
								KAir1TextBox.Enabled = false;
								KAir1TextBox.Text = "0";
								break;
							case 1:
								KAir2TextBox.Enabled = false;
								KAir2TextBox.Text = "0";
								break;
							case 2:
								KAir3TextBox.Enabled = false;
								KAir3TextBox.Text = "0";
								break;
							case 3:
								KAir4TextBox.Enabled = false;
								KAir4TextBox.Text = "0";
								break;
						}
					}
				}
			}
		}

		/* 「艦隊データ」タブ */
		private void FResetButton_Click(object sender, EventArgs e) {
			/* リセット(全消去) */
			FleetsData.Clear();
			FleetsListBox.Items.Clear();
			FKammusuComboBox.SelectedIndex = -1;
			FKammusuComboBox.Text = "(艦娘を選択)";
			FSlot1ComboBox.SelectedIndex = -1;
			FSlot1ComboBox.Text = "(装備を選択)";
			FAir1TextBox.Text = "0";
			FSlot2ComboBox.SelectedIndex = -1;
			FSlot2ComboBox.Text = "(装備を選択)";
			FAir2TextBox.Text = "0";
			FSlot3ComboBox.SelectedIndex = -1;
			FSlot3ComboBox.Text = "(装備を選択)";
			FAir3TextBox.Text = "0";
			FSlot4ComboBox.SelectedIndex = -1;
			FSlot4ComboBox.Text = "(装備を選択)";
			FAir4TextBox.Text = "0";
			FHPTextBox.Text = "0";
			FcondTextBox.Text = "49";
			FAmmoTextBox.Text = "100";
			FNameTextBox.Text = "第1艦隊";
			FHQLevelTextBox.Text = "100";
			FAllAntiAirTextBox.Text = "";
			FAllSearchTextBox.Text = "";
		}
		private void FUndoButton_Click(object sender, EventArgs e) {
			/* 元に戻す(変更を反映せずに戻す) */
			FKammusuComboBox_SelectedIndexChanged(sender, e);
		}
		private void FPlusButton_Click(object sender, EventArgs e) {
			/* 追加(末尾に追加) */
			//追加できないかをチェックする
			if (FKammusuComboBox.SelectedIndex == -1) return;
			if ((FSlot1ComboBox.Enabled) && (FSlot1ComboBox.SelectedIndex == -1)) return;
			if ((FSlot2ComboBox.Enabled) && (FSlot2ComboBox.SelectedIndex == -1)) return;
			if ((FSlot3ComboBox.Enabled) && (FSlot3ComboBox.SelectedIndex == -1)) return;
			if ((FSlot4ComboBox.Enabled) && (FSlot4ComboBox.SelectedIndex == -1)) return;
			bool hasSameKammusu = false;
			int I = 0;
			for (int i = 0; i < FleetsData.Count; ++i) {
				if (FleetsData[i].Number == FKammusuComboBox.SelectedIndex + 1) {
					hasSameKammusu = true;
					I = i;
					break;
				}
			}
			if (hasSameKammusu) {
				//単なる更新のみを行う
				FleetsListBox.SelectedIndex = I;
				int i = FleetsListBox.SelectedIndex;
				FleetsData[i].Number = FKammusuComboBox.SelectedIndex + 1;
				FleetsData[i].Slots = int.Parse(KammusuData.Rows[FleetsData[i].Number - 1]["スロット数"].ToString());
				if (FleetsData[i].Slots > 0) {
					FleetsData[i].Weapons[0] = FSlot1ComboBox.SelectedIndex;
					FleetsData[i].Airs[0] = int.Parse(FAir1TextBox.Text);
					if (FleetsData[i].Weapons[0] == 0) FleetsData[i].Airs[0] = 0;
				}
				if (FleetsData[i].Slots > 1) {
					FleetsData[i].Weapons[1] = FSlot2ComboBox.SelectedIndex;
					FleetsData[i].Airs[1] = int.Parse(FAir2TextBox.Text);
					if (FleetsData[i].Weapons[1] == 0) FleetsData[i].Airs[1] = 0;
				}
				if (FleetsData[i].Slots > 2) {
					FleetsData[i].Weapons[2] = FSlot3ComboBox.SelectedIndex;
					FleetsData[i].Airs[2] = int.Parse(FAir3TextBox.Text);
					if (FleetsData[i].Weapons[2] == 0) FleetsData[i].Airs[2] = 0;
				}
				if (FleetsData[i].Slots > 3) {
					FleetsData[i].Weapons[3] = FSlot4ComboBox.SelectedIndex;
					FleetsData[i].Airs[3] = int.Parse(FAir4TextBox.Text);
					if (FleetsData[i].Weapons[3] == 0) FleetsData[i].Airs[3] = 0;
				}
				FleetsData[i].HP = int.Parse(FHPTextBox.Text);
				FleetsData[i].cond = int.Parse(FcondTextBox.Text);
				FleetsData[i].Ammo = int.Parse(FAmmoTextBox.Text);
				CalcAllAntiAir();
				CalcAllSearch();
				return;
			}
			if (FleetsData.Count == MaxKammusues) return;
			//問題はないので追加する
			KammusuNow NewKammusu = new KammusuNow();
			NewKammusu.Number = FKammusuComboBox.SelectedIndex + 1;
			NewKammusu.Slots = int.Parse(KammusuData.Rows[NewKammusu.Number - 1]["スロット数"].ToString());
			if (NewKammusu.Slots > 0) {
				NewKammusu.Weapons[0] = FSlot1ComboBox.SelectedIndex;
				NewKammusu.Airs[0] = int.Parse(FAir1TextBox.Text);
				if (NewKammusu.Weapons[0] == 0) NewKammusu.Airs[0] = 0;
			}
			if (NewKammusu.Slots > 1) {
				NewKammusu.Weapons[1] = FSlot2ComboBox.SelectedIndex;
				NewKammusu.Airs[1] = int.Parse(FAir2TextBox.Text);
				if (NewKammusu.Weapons[1] == 0) NewKammusu.Airs[1] = 0;
			}
			if (NewKammusu.Slots > 2) {
				NewKammusu.Weapons[2] = FSlot3ComboBox.SelectedIndex;
				NewKammusu.Airs[2] = int.Parse(FAir3TextBox.Text);
				if (NewKammusu.Weapons[2] == 0) NewKammusu.Airs[2] = 0;
			}
			if (NewKammusu.Slots > 3) {
				NewKammusu.Weapons[3] = FSlot4ComboBox.SelectedIndex;
				NewKammusu.Airs[3] = int.Parse(FAir4TextBox.Text);
				if (NewKammusu.Weapons[3] == 0) NewKammusu.Airs[3] = 0;
			}
			NewKammusu.HP = int.Parse(FHPTextBox.Text);
			NewKammusu.cond = int.Parse(FcondTextBox.Text);
			NewKammusu.Ammo = int.Parse(FAmmoTextBox.Text);
			FleetsData.Add(NewKammusu);
			DrawFleetsData();
			FleetsListBox.SelectedIndex = FleetsData.Count - 1;
		}
		private void FMinusButton_Click(object sender, EventArgs e) {
			/* 削除(選択を削除) */
			if (FleetsListBox.SelectedIndex != -1) {
				FleetsData.RemoveAt(FleetsListBox.SelectedIndex);
				DrawFleetsData();
			}
		}
		private void FUpperButton_Click(object sender, EventArgs e) {
			/* ↑(1つ上に上げる) */
			int i = FleetsListBox.SelectedIndex;
			if (i == -1) return;
			if (i == 0) return;
			if (FleetsData.Count == 1) return;
			KammusuNow temp = FleetsData[i];
			FleetsData[i] = FleetsData[i - 1];
			FleetsData[i - 1] = temp;
			DrawFleetsData();
			FleetsListBox.SelectedIndex = i - 1;
		}
		private void FLowerButton_Click(object sender, EventArgs e) {
			/* ↓(1つ下に下げる) */
			int i = FleetsListBox.SelectedIndex;
			if (i == -1) return;
			if (i == FleetsData.Count - 1) return;
			if (FleetsData.Count == 1) return;
			KammusuNow temp = FleetsData[i];
			FleetsData[i] = FleetsData[i + 1];
			FleetsData[i + 1] = temp;
			DrawFleetsData();
			FleetsListBox.SelectedIndex = i + 1;
		}
		private void FleetsListBox_SelectedIndexChanged(object sender, EventArgs e) {
			/* 左欄をクリックした際の処理 */
			if (FleetsListBox.SelectedIndex != -1) {
				int i = FleetsListBox.SelectedIndex;
				FKammusuComboBox.SelectedIndex = FleetsData[i].Number - 1;
				FSlot1ComboBox.SelectedIndex = FleetsData[i].Weapons[0];
				FAir1TextBox.Text = FleetsData[i].Airs[0].ToString();
				FSlot2ComboBox.SelectedIndex = FleetsData[i].Weapons[1];
				FAir2TextBox.Text = FleetsData[i].Airs[1].ToString();
				FSlot3ComboBox.SelectedIndex = FleetsData[i].Weapons[2];
				FAir3TextBox.Text = FleetsData[i].Airs[2].ToString();
				FSlot4ComboBox.SelectedIndex = FleetsData[i].Weapons[3];
				FAir4TextBox.Text = FleetsData[i].Airs[3].ToString();
				FHPTextBox.Text = FleetsData[i].HP.ToString();
				FcondTextBox.Text = FleetsData[i].cond.ToString();
				FAmmoTextBox.Text = FleetsData[i].Ammo.ToString();
			}
		}
		private void FKammusuComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			/* 艦娘部分を変更した際の処理 */
			if (FKammusuComboBox.SelectedIndex != -1) {
				int i = FKammusuComboBox.SelectedIndex;
				DataRow[] dr = KammusuData.Select();
				//スロット関係
				int Slots = int.Parse(dr[i]["スロット数"].ToString());
				for (int k = 0; k < MaxSlots; ++k) {
					if (k < Slots) {
						//適当に初期化する
						switch (k) {
							case 0:
								FSlot1ComboBox.Enabled = true;
								FAir1TextBox.Enabled = true;
								FAir1TextBox.Text = dr[i]["艦1"].ToString();
								break;
							case 1:
								FSlot2ComboBox.Enabled = true;
								FAir2TextBox.Enabled = true;
								FAir2TextBox.Text = dr[i]["艦2"].ToString();
								break;
							case 2:
								FSlot3ComboBox.Enabled = true;
								FAir3TextBox.Enabled = true;
								FAir3TextBox.Text = dr[i]["艦3"].ToString();
								break;
							case 3:
								FSlot4ComboBox.Enabled = true;
								FAir4TextBox.Enabled = true;
								FAir4TextBox.Text = dr[i]["艦4"].ToString();
								break;
						}
					}
					else {
						//無効化する
						switch (k) {
							case 0:
								FSlot1ComboBox.Enabled = false;
								FAir1TextBox.Enabled = false;
								break;
							case 1:
								FSlot2ComboBox.Enabled = false;
								FAir2TextBox.Enabled = false;
								break;
							case 2:
								FSlot3ComboBox.Enabled = false;
								FAir3TextBox.Enabled = false;
								break;
							case 3:
								FSlot4ComboBox.Enabled = false;
								FAir4TextBox.Enabled = false;
								break;
						}
					}
				}
				//その他
				FHPTextBox.Text = dr[i]["耐久"].ToString();
				FcondTextBox.Text = "49";
				FAmmoTextBox.Text = "100";
			}
		}
		private void FSlot1ComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			if (FSlot1ComboBox.SelectedIndex == 0) {
				//<未装備>向けの調整
				FSlot2ComboBox.SelectedIndex = 0;
				FSlot3ComboBox.SelectedIndex = 0;
				FSlot4ComboBox.SelectedIndex = 0;
			}
		}
		private void FSlot2ComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			if (FSlot2ComboBox.SelectedIndex == 0) {
				//<未装備>向けの調整
				FSlot3ComboBox.SelectedIndex = 0;
				FSlot4ComboBox.SelectedIndex = 0;
			}
		}
		private void FSlot3ComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			if (FSlot3ComboBox.SelectedIndex == 0) {
				//<未装備>向けの調整
				FSlot4ComboBox.SelectedIndex = 0;
			}
		}

		/* 「戦闘シミュレーション」タブ */
		private void Read1Button_Click(object sender, EventArgs e) {
			/* 第1艦隊を読み込み */
			//テキストとして読み込む
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.Filter = "艦隊データ(*.txt)|*.txt|すべてのファイル(*.*)|*.*";
			if (ofd.ShowDialog() != DialogResult.OK) return;
			FilePath1 = ofd.FileName;
			string[] ReadLines = File.ReadAllLines(FilePath1, System.Text.Encoding.GetEncoding("Shift_JIS"));
			//データとして取り込む
			int Step = 0;
			for (int i = 0; i < ReadLines.Length; ++i) {
				Debug.WriteLine(Step.ToString() + " " + ReadLines[i]);
				if (ReadLines[i] == "") continue;
				if (ReadLines[i].Substring(0, 1) == "#") continue;
				//司令部レベル
				if (Step == 0) {
					++Step;
					continue;
				}
				//メンバー
				if (Step == 1) {
					string[] Numbers = ReadLines[i].Split(',');
					Fleets1ListBox.Items.Clear();
					for (int j = 0; j <  Numbers.Length; ++j) {
						Fleets1ListBox.Items.Add(KammusuData.Rows[int.Parse(Numbers[j]) - 1]["艦名"]);
					}
					break;
				}
			}
			Fleets1ListBox.Refresh();
		}
		private void Read2Button_Click(object sender, EventArgs e) {
			/* 第2艦隊を読み込み */
			//テキストとして読み込む
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.Filter = "艦隊データ(*.txt)|*.txt|すべてのファイル(*.*)|*.*";
			if (ofd.ShowDialog() != DialogResult.OK) return;
			FilePath2 = ofd.FileName;
			string[] ReadLines = File.ReadAllLines(FilePath2, System.Text.Encoding.GetEncoding("Shift_JIS"));
			//データとして取り込む
			int Step = 0;
			for (int i = 0; i < ReadLines.Length; ++i) {
				Debug.WriteLine(Step.ToString() + " " + ReadLines[i]);
				if (ReadLines[i] == "") continue;
				if (ReadLines[i].Substring(0, 1) == "#") continue;
				//司令部レベル
				if (Step == 0) {
					++Step;
					continue;
				}
				//メンバー
				if (Step == 1) {
					string[] Numbers = ReadLines[i].Split(',');
					Fleets2ListBox.Items.Clear();
					for (int j = 0; j < Numbers.Length; ++j) {
						Fleets2ListBox.Items.Add(KammusuData.Rows[int.Parse(Numbers[j]) - 1]["艦名"]);
					}
					break;
				}
			}
			Fleets2ListBox.Refresh();
		}
		private void CalcButton_Click(object sender, EventArgs e) {
			/* 計算開始 */
			if (For1ComboBox.SelectedIndex == -1) return;
			if (For1ComboBox.SelectedIndex == -1) return;
			if (BattleCountComboBox.SelectedIndex == -1) return;
			OutputTextBox.Text = "";
			//コマンドラインを設定
			string CommandLine = "";
			CommandLine += BattleCountComboBox.Text;
			if (SwapCheckBox.Checked) {
				CommandLine += " \"" + FilePath2 + "\" \"" + FilePath1;
				CommandLine += "\" " + For2ComboBox.Text;
				CommandLine += " " + For1ComboBox.Text;
			}
			else {
				CommandLine += " \"" + FilePath1 + "\" \"" + FilePath2;
				CommandLine += "\" " + For1ComboBox.Text;
				CommandLine += " " + For2ComboBox.Text;
			}
			//外部exeファイルで実行し、標準出力を取り込む
			ProcessStartInfo psInfo = new ProcessStartInfo();
			psInfo.FileName = @System.IO.Directory.GetCurrentDirectory() + @"\KanColleSim.exe";	//実行するファイル
            System.Console.WriteLine(CommandLine);
            System.Console.WriteLine(psInfo.FileName);

            psInfo.CreateNoWindow = true;			//コンソール・ウィンドウを開かない
			psInfo.UseShellExecute = false;			//シェル機能を使用しない
			psInfo.Arguments = CommandLine;			//コマンドライン引数を設定
			psInfo.RedirectStandardOutput = true;	//標準出力を取り込むようにする
			Process p = Process.Start(psInfo);
			string Output = p.StandardOutput.ReadToEnd();
			Output = Output.Replace("\t\t\n", "\n");
			OutputTextBox.Text = Output;
			OutputTextBox.Focus();
		}

		/* メニューボタン */
		private void FileOpenMenu_Click(object sender, EventArgs e) {
			/* 艦隊を開く */
			//テキストとして読み込む
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.Filter = "艦隊データ(*.txt)|*.txt|すべてのファイル(*.*)|*.*";
			if (ofd.ShowDialog() != DialogResult.OK) return;
			FilePath = ofd.FileName;
			string[] ReadLines = File.ReadAllLines(FilePath, System.Text.Encoding.GetEncoding("Shift_JIS"));
			//データとして取り込む
			int Step = 0, Members = 0;
			for (int i = 0; i < ReadLines.Length; ++i) {
				Debug.WriteLine(Step.ToString() + " " + ReadLines[i]);
				if (ReadLines[i] == "") continue;
				if (ReadLines[i].Substring(0, 1) == "#") {
					//艦隊名
					if (ReadLines[i].Length >= 5) {
						if (ReadLines[i].Substring(1, 4) == "艦隊名：") {
							FNameTextBox.Text = ReadLines[i].Substring(5);
						}
					}
					continue;
				}
				//司令部レベル
				if (Step == 0) {
					FHQLevelTextBox.Text = ReadLines[i];
					++Step;
					continue;
				}
				//メンバー
				if (Step == 1) {
					string[] Numbers = ReadLines[i].Split(',');
					FleetsData.Clear();
					Members = Numbers.Length;
					for (int j = 0; j < Members; ++j) {
						KammusuNow temp = new KammusuNow();
						FleetsData.Add(temp);
					}
					for (int j = 0; j < Members; ++j) {
						FleetsData[j].Number = int.Parse(Numbers[j]);
					}
					++Step;
					continue;
				}
				//装備
				if ((Step >= 2) && (Step < 2 + Members)) {
					string[] Weapons = ReadLines[i].Split(',');
					for (int j = 0; j < MaxSlots; ++j) {
						FleetsData[Step - 2].Weapons[j] = 0;
					}
					int slots = int.Parse(KammusuData.Rows[FleetsData[Step - 2].Number -1]["スロット数"].ToString());
					FleetsData[Step - 2].Slots = slots;
					if (slots > Weapons.Length) slots = Weapons.Length;
					for (int j = 0; j < slots; ++j) {
						if (int.Parse(Weapons[j]) > 0) {
							FleetsData[Step - 2].Weapons[j] = int.Parse(Weapons[j]);
						}
					}
					++Step;
					continue;
				}
				//状態
				if ((Step >= 2 + Members) && (Step < 2 + Members * 2)) {
					string[] Info = ReadLines[i].Split(',');
					int k = Step - 2 - Members;
					int n = FleetsData[k].Number - 1;
					DataRow dr = KammusuData.Rows[n];
					FleetsData[k].HP      = int.Parse(Info[0]);
					if (FleetsData[k].HP     <= 0) FleetsData[k].HP      = int.Parse(dr["耐久"].ToString());
					FleetsData[k].cond    = int.Parse(Info[1]);
					if (FleetsData[k].cond    < 0) FleetsData[k].cond    = 49;
					FleetsData[k].Ammo    = int.Parse(Info[2]); 
					if (FleetsData[k].Ammo    < 0) FleetsData[k].Ammo    = 100;
					FleetsData[k].Airs[0] = int.Parse(Info[3]);
					if (FleetsData[k].Airs[0] < 0) FleetsData[k].Airs[0] = int.Parse(dr["艦1"].ToString());
					FleetsData[k].Airs[1] = int.Parse(Info[4]);
					if (FleetsData[k].Airs[1] < 0) FleetsData[k].Airs[1] = int.Parse(dr["艦2"].ToString());
					FleetsData[k].Airs[2] = int.Parse(Info[5]);
					if (FleetsData[k].Airs[2] < 0) FleetsData[k].Airs[2] = int.Parse(dr["艦3"].ToString());
					FleetsData[k].Airs[3] = int.Parse(Info[6]);
					if (FleetsData[k].Airs[3] < 0) FleetsData[k].Airs[3] = int.Parse(dr["艦4"].ToString());
					++Step;
				}
			}
			//画面に反映する
			DrawFleetsData();
		}
		private void FileSaveAMenu_Click(object sender, EventArgs e) {
			/* 艦隊を名前を付けて保存 */
			if(FleetsData.Count == 0) return;
			if(saveFileDialog.ShowDialog() != DialogResult.OK) return;
			FilePath = saveFileDialog.FileName;
			//保存準備
			String SaveData = MakeSaveData();
			//保存
			StreamWriter sw = new StreamWriter(FilePath, false, Encoding.GetEncoding("shift_jis"));
			sw.Write(SaveData);
			sw.Close();
		}
		private void FileSaveSMenu_Click(object sender, EventArgs e) {
			/* 艦隊を上書き保存 */
			if (FilePath == "") {
				FileSaveAMenu_Click(sender, e);
				return;
			}
			//保存準備
			String SaveData = MakeSaveData();
			//保存
			StreamWriter sw = new StreamWriter(FilePath, false, Encoding.GetEncoding("shift_jis"));
			sw.Write(SaveData);
			sw.Close();
		}
		private void FileCloseMenu_Click(object sender, EventArgs e) {
			/* ソフトウェアの終了 */
			this.Close();
		}
		private void VerInfoMenu_Click(object sender, EventArgs e) {
			/* バージョン情報表示 */
			string VerInfo = "";
			VerInfo += SoftName + "\n";
			VerInfo += "バージョン：0.5\n";
			VerInfo += "作成者：◆RoVKHyEYUtVB\n";
			VerInfo += "最終更新日時：2014/12/13 12:38\n";
			VerInfo += "連絡先：ysr.hsp@gmail.com";
			MessageBox.Show(VerInfo, SoftName);
		}

		/* 処理用関数 */
		private void InitializeData() {
			/* 起動時に変数を初期化する */
			for (int i = 0; i < WTypeComboBox.Items.Count; ++i){
				TypeList[WTypeComboBox.Items[i].ToString()] = i;
			}
			for (int i = 0; i < WRangeComboBox.Items.Count; ++i){
				WRangeList[WRangeComboBox.Items[i].ToString()] = i;
			}
			for (int i = 0; i < KKindComboBox.Items.Count; ++i){
				KindList[KKindComboBox.Items[i].ToString()]  = i;
			}
			for (int i = 0; i < KSpeedComboBox.Items.Count; ++i){
				SpeedList[KSpeedComboBox.Items[i].ToString()] = i;
			}
			for (int i = 0; i < KRangeComboBox.Items.Count; ++i) {
				KRangeList[KRangeComboBox.Items[i].ToString()] = i;
			}
		}
		private void ReadWeaponData() {
			/* 装備データを読み込む */
			string csvDir = System.IO.Directory.GetCurrentDirectory(); 
			string csvFileName = "weapon.csv";
			string conString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + csvDir + ";Extended Properties=\"text;HDR=Yes;FMT=Delimited\"";
			System.Data.OleDb.OleDbConnection con = new System.Data.OleDb.OleDbConnection(conString);
			string commText = "SELECT * FROM [" + csvFileName + "]";
			System.Data.OleDb.OleDbDataAdapter da = new System.Data.OleDb.OleDbDataAdapter(commText, con);
			WeaponData.Clear();
			da.Fill(WeaponData);
		}
		private void ReadKammusuData() {
			/* 艦娘データを読み込む */
			string csvDir = System.IO.Directory.GetCurrentDirectory();
			string csvFileName = "kammusu.csv";
			string conString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + csvDir + ";Extended Properties=\"text;HDR=Yes;FMT=Delimited\"";
			System.Data.OleDb.OleDbConnection con = new System.Data.OleDb.OleDbConnection(conString);
			string commText = "SELECT * FROM [" + csvFileName + "]";
			System.Data.OleDb.OleDbDataAdapter da = new System.Data.OleDb.OleDbDataAdapter(commText, con);
			KammusuData.Clear();
			da.Fill(KammusuData);
		}
		private void DrawWeaponData() {
			WeaponListBox.Items.Clear();
			FSlot1ComboBox.Items.Clear();
			FSlot2ComboBox.Items.Clear();
			FSlot3ComboBox.Items.Clear();
			FSlot4ComboBox.Items.Clear();
			FSlot1ComboBox.Items.Add("<未装備>");
			FSlot2ComboBox.Items.Add("<未装備>");
			FSlot3ComboBox.Items.Add("<未装備>");
			FSlot4ComboBox.Items.Add("<未装備>");
			/* 装備データを画面に反映させる */
			DataRow[] dr = WeaponData.Select();
			for (int i = 0; i < dr.Length; ++i) {
				WeaponListBox.Items.Add(dr[i]["名前"]);
				FSlot1ComboBox.Items.Add(dr[i]["名前"]);
				FSlot2ComboBox.Items.Add(dr[i]["名前"]);
				FSlot3ComboBox.Items.Add(dr[i]["名前"]);
				FSlot4ComboBox.Items.Add(dr[i]["名前"]);
			}
			WeaponListBox.Refresh();
			FSlot1ComboBox.Refresh();
			FSlot2ComboBox.Refresh();
			FSlot3ComboBox.Refresh();
			FSlot4ComboBox.Refresh();
		}
		private void DrawKammusuData() {
			KammusuListBox.Items.Clear();
			FKammusuComboBox.Items.Clear();
			/* 艦娘データを画面に反映させる */
			DataRow[] dr = KammusuData.Select();
			for (int i = 0; i < dr.Length; ++i){
				KammusuListBox.Items.Add(dr[i]["艦名"].ToString() + "(Lv" + dr[i]["Lv"].ToString() + ")");
				FKammusuComboBox.Items.Add(dr[i]["艦名"].ToString() + "(Lv" + dr[i]["Lv"].ToString() + ")");
			}
			KammusuListBox.Refresh();
			FKammusuComboBox.Refresh();
		}
		private void DrawFleetsData() {
			FleetsListBox.Items.Clear();
			/* 艦隊データを画面に反映させる */
			DataRow[] dr = KammusuData.Select();
			for (int i = 0; i < FleetsData.Count; ++i) {
				FleetsListBox.Items.Add(KammusuData.Rows[FleetsData[i].Number - 1]["艦名"].ToString() + "(Lv" + dr[FleetsData[i].Number - 1]["Lv"].ToString() + ")");
			}
			FleetsListBox.Refresh();
			CalcAllAntiAir();
			CalcAllSearch();
		}
		private void CalcAllAntiAir() {
			int AAASum = 0;
			for(int i = 0; i < FleetsData.Count; ++i) {
				for(int j = 0; j < FleetsData[i].Slots; ++j) {
					int weapon = FleetsData[i].Weapons[j];
					if(weapon >= 1){
						string type = WeaponData.Rows[weapon - 1]["種別"].ToString();
						if((type == "艦上戦闘機")
						|| (type == "艦上爆撃機(爆戦)")
						|| (type == "艦上爆撃機")
						|| (type == "水上爆撃機")
						|| (type == "艦上攻撃機")) {
							AAASum += (int)(int.Parse(WeaponData.Rows[weapon - 1]["対空"].ToString()) * Math.Sqrt(FleetsData[i].Airs[j]));
						}
					}
				}
			}
			FAllAntiAirTextBox.Text = AAASum.ToString();
			return;
		}
		private int RoundUp5(int n) {
			if(n % 5 == 0) {
				return n;
			} else {
				return (n / 5 + 1) * 5;
			}
		}
		private void CalcAllSearch() {
			double Search = 0.0;
			if(USE_NEW_SEARCH_METHOD) {
				// 2-5式(秋)
				// http://ch.nicovideo.jp/biikame/blomaga/ar663428
				for(int i = 0; i < FleetsData.Count; ++i) {
					Search += Math.Sqrt(int.Parse(KammusuData.Rows[FleetsData[i].Number - 1]["索敵"].ToString())) * 1.6841056;
					for(int j = 0; j < FleetsData[i].Slots; ++j){
						if(FleetsData[i].Weapons[j] >= 1) {
							string type = WeaponData.Rows[FleetsData[i].Weapons[j] - 1]["種別"].ToString();
							int search = int.Parse(WeaponData.Rows[FleetsData[i].Weapons[j] - 1]["索敵"].ToString());
							if(type == "艦上爆撃機") {
								Search += search * 1.0376255;
							}else if(type == "水上爆撃機"){
								Search += search * 1.7787282;
							}else if(type == "艦上攻撃機"){
								Search += search * 1.3677954;
							}else if(type == "艦上偵察機"){
								Search += search * 1.6592780;
							}else if(type == "水上偵察機"){
								Search += search * 2.0000000;
							}else if(type == "小型電探"){
								Search += search * 1.0045358;
							}else if(type == "大型電探"){
								Search += search * 0.9906638;
							}else if(type == "探照灯"){
								Search += search * 0.9067950;
							}
						}
					}
				}
				Search += RoundUp5(int.Parse(FHQLevelTextBox.Text)) * (-0.6142467);
			} else {
				double AllKammusuSearch = 0.0;
				for(int i = 0; i < FleetsData.Count; ++i) {
					AllKammusuSearch += int.Parse(KammusuData.Rows[FleetsData[i].Number - 1]["索敵"].ToString());
					for(int j = 0; j < FleetsData[i].Slots; ++j) {
						if(FleetsData[i].Weapons[j] >= 1) {
							string type = WeaponData.Rows[FleetsData[i].Weapons[j] - 1]["種別"].ToString();
							int search = int.Parse(WeaponData.Rows[FleetsData[i].Weapons[j] - 1]["索敵"].ToString());
							if(type == "水上爆撃機") {
								Search += search * 2.0;
							} else if(type == "艦上偵察機") {
								Search += search * 2.0;
							} else if(type == "水上偵察機") {
								Search += search * 2.0;
							} else if(type == "小型電探") {
								Search += search * 1.0;
							} else if(type == "大型電探") {
								Search += search * 1.0;
							}
						}
					}
				}
				Search += Math.Sqrt(AllKammusuSearch);
			}
			FAllSearchTextBox.Text = Search.ToString("F2");	//小数第2位を四捨五入
			return;
		}
		private string MakeSaveData() {
			/* セーブデータを作成する */
			StringBuilder SaveData = new StringBuilder();
			SaveData.Append("#艦隊名：" + FNameTextBox.Text + "\n");
			SaveData.Append("#司令部レベル\n" + FHQLevelTextBox.Text + "\n");
			SaveData.Append("#メンバー(カンマ区切り)\n");
			for (int i = 0; i < FleetsData.Count; ++i) {
				if (i != 0) SaveData.Append(",");
				SaveData.Append(FleetsData[i].Number.ToString());
			}
			SaveData.Append("\n");
			SaveData.Append("#装備(カンマ区切り。無い場合は-1とする)\n");
			for (int i = 0; i < FleetsData.Count; ++i) {
				if (FleetsData[i].Slots == 0) SaveData.Append("-1");
				if (FleetsData[i].Weapons[0] == 0) SaveData.Append("-1");
				for (int j = 0; j < FleetsData[i].Slots; ++j) {
					if (FleetsData[i].Weapons[j] == 0) break;
					if (j != 0) SaveData.Append(",");
					SaveData.Append(FleetsData[i].Weapons[j].ToString());
				}
				SaveData.Append("\n");
			}
			SaveData.Append("#状態(残り耐久・cond値・残り弾薬％・1～4スロ目の残り艦載機数)\n");
			SaveData.Append("#それぞれ、「-1」とすると自動的に最大耐久・49・100・最大艦載機数に初期化される\n");
			for (int i = 0; i < FleetsData.Count; ++i) {
				SaveData.Append(FleetsData[i].HP.ToString()   + ",");
				SaveData.Append(FleetsData[i].cond.ToString() + ",");
				SaveData.Append(FleetsData[i].Ammo.ToString());
				for (int j = 0; j < MaxSlots; ++j) {
					SaveData.Append("," + FleetsData[i].Airs[j].ToString());
				}
				SaveData.Append("\n");
			}
			return SaveData.ToString();
		}

	}
	/* 保存用データ */
	public class KammusuNow {
		public int Number;
		public int Slots;
		public int[] Weapons = new int[MainForm.MaxSlots];
		public int[] Airs = new int[MainForm.MaxSlots];
		public int HP;
		public int cond;
		public int Ammo;
		public KammusuNow() {
			Number = 0;
			Slots = 0;
			for (int i = 0; i < MainForm.MaxSlots; ++i) {
				Weapons[i] = 0;
				Airs[i] = 0;
			}
			HP = 0;
			cond = 49;
			Ammo = 100;
		}
	}
}
