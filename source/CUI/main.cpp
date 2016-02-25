/* ---------------------------- */
/*  艦これシミュレータ Ver.0.2  */
/* ---------------------------- */

/*
   最終更新日時：2014/12/04 03:17
   製作者：◆RoVKHyEYUtVB
   コマンドライン引数：
     「KanColleSim 試行回数 艦隊データ(先攻) 艦隊データ(後攻) 陣形(先攻) 陣形(後攻)」
     ・試行回数……シミュレートする回数(自然数)。「1」を指定すると、戦闘の途中経過も表示されるが、
                   2以上を指定すると途中経過表示が無くなり、代わりに複数回のシミュレートにおける
                   艦娘の残り耐久(平均)と、戦闘結果(完全勝利S～敗北D)の割合が表示される。
     ・艦隊データ……サンプルをfleet1.txtとfleet2.txtに示した。先攻はその名の通り、
                     相手に先に攻撃する方になる。仕様上、先攻の方が有利ではある。
     ・陣形……単縦陣・複縦陣・輪形陣・梯形陣・単横陣から1つ選択。
     なお、コマンドライン引数は5つとも全て指定する必要がある。
   呼び出し例：
     「KanColleSim 10000 fleet1.txt fleet2.txt 単縦陣 単縦陣」
   注意：
   ・kammusu.txtとweapon.txtは毎回必ず読み込まれるので注意
   ・入力にエラーがある場合その旨をエラーメッセージとして知らせる
   ・勿論kammusu.txtとweapon.txt、および艦隊データは自由に編集できるが
   くれぐれも文法ミスがないように！
*/

#include "header.h"
#include <chrono>

int main(const int argc, const char *argv[]) {
	/* 初期設定 */
	try{
		if(argc < 5) throw "引数が不正です.";
		// 試行回数
		stringstream sin;
		sin << argv[1];
		int BattleCount;
		sin >> BattleCount;
		if(BattleCount < 1) throw "試行回数は自然数のみ取ることができます.";
		// 艦隊データ
		fleets FleetsData[BattleSize];
		for(int i = 0; i < BattleSize; ++i){
			string FleetsDataName(argv[i + 2]);
			FleetsData[i].ReadData(FleetsDataName);
		}
		// 陣形
		for(int i = 0; i < BattleSize; ++i){
			string FormationString(argv[i + 4]);
			if(FormationString == "単縦陣"){
				FleetsData[i].Formation = FOR_TRAIL;
			}else if(FormationString == "複縦陣"){
				FleetsData[i].Formation = FOR_SUBTRAIL;
			}else if(FormationString == "輪形陣"){
				FleetsData[i].Formation = FOR_CIRCLE;
			}else if(FormationString == "梯形陣"){
				FleetsData[i].Formation = FOR_ECHELON;
			}else if(FormationString == "単横陣"){
				FleetsData[i].Formation = FOR_ABREAST;
			}else{
				throw "その陣形は存在しません";
			}
		}
		/* 第1→第2艦隊への攻撃 */
		if(BattleCount == 1) {
			// 1回のみ
			FleetsData[FriendSide].Simulate(FleetsData[EnemySide]);
		} else {
			// 複数回(統計モード)
			//準備
			fleets FleetsData_[BattleSize];
			vector< vector<int> > HPSum(BattleSize);
			vector< vector<int> > HeavyDamageCount(BattleSize);
			for(int i = 0; i < BattleSize; ++i) {
				HPSum[i].resize(FleetsData[i].Members, 0);
				HeavyDamageCount[i].resize(FleetsData[i].Members, 0);
			}
			//複数回試行して集計する
			vector<int> WinReason(WIN_Size, 0);
			for(int n = 0; n < BattleCount; ++n) {
				for(int i = 0; i < BattleSize; ++i) {
					FleetsData_[i] = FleetsData[i];
				}
				++WinReason[FleetsData_[FriendSide].Simulate(FleetsData_[EnemySide], false)];
				for(int i = 0; i < BattleSize; ++i) {
					for(int j = 0; j < FleetsData[i].Members; ++j) {
						if(FleetsData_[i].Kammusues[j].ShowDamage() >= HeavyDamage){
							HeavyDamageCount[i][j]++;
						}
						HPSum[i][j] += FleetsData_[i].Kammusues[j].HP;
					}
				}
			}
			//結果を表示する
			cout << "【結果表示】\n";
			for(int i = 0; i < BattleSize; ++i) {
				cout << "○" << Position[i] << "\n";
				for(int j = 0; j < FleetsData[i].Members; ++j) {
					FleetsData[i].Kammusues[j].HP = static_cast<int>(1.0 * HPSum[i][j] / BattleCount);
					cout << "　" << FleetsData[i].Kammusues[j].Label() << " "
						<< 1.0 * HPSum[i][j] / BattleCount << "/" << FleetsData[i].Kammusues[j].MaxHP
						<< "(" << DMString[FleetsData[i].Kammusues[j].ShowDamage()] << ") ";
					cout << "大破率：" << (100.0 * HeavyDamageCount[i][j] / BattleCount) << "％\n";
				}
			}
			double WinPer = 100.0 * (WinReason[WIN_SS] + WinReason[WIN_S] + WinReason[WIN_A] + WinReason[WIN_B]) / BattleCount;
			cout << "○勝利判定の分布(勝率：" << WinPer << "％)\n";
			for(int i = WIN_Size - 1; i >= 0; --i) {
				cout << "　" << WINString[i] << "：" << WinReason[i] << "回(" << 100.0 * WinReason[i] / BattleCount << "％)\n";
			}
		}
	}
	catch(char *e){
		cout << "エラー：" << e << std::endl;
	}
}
/*
	//装備 変数名           名前,                   種別,           火, 雷,爆,空,潜,索,中,回, 射程,        装甲
	weapon Gun_127_Multi   {"12.7cm連装砲",         Type_Gun,        2,  0, 0, 2, 0, 0, 0, 0, ShortRange,  0};
	weapon Gun_140_Single  {"14cm単装砲",           Type_Gun,        2,  0, 0, 0, 0, 0, 1, 0, MiddleRange, 0};
	weapon Gun_203_Multi   {"20.3cm連装砲",         Type_Gun,        8,  0, 0, 3, 0, 0, 0, 0, MiddleRange, 0};
	weapon Gun_356_Multi   {"35.6cm連装砲",         Type_Gun,       15,  0, 0, 4, 0, 0, 0, 0, LongRange,   0};
	weapon Gun_152_Single  {"15.2cm単装砲",         Type_SubGun,     2,  0, 0, 0, 0, 0, 1, 0, MiddleRange, 0};
	weapon Torpedo_61_3    {"61cm三連装魚雷",       Type_Torpedo,    0,  5, 0, 0, 0, 0, 0, 0, ShortRange,  0};
	weapon Torpedo_61_4_Oxy{"61cm四連装(酸素)魚雷", Type_Torpedo,    0, 10, 0, 0, 0, 0, 0, 0, ShortRange,  0};
	weapon SpecialSS_Kou   {"甲標的 甲",            Type_SpecialSS,  0, 12, 0, 0, 0, 0, 0, 0, NoneRange,   0};
	weapon WB_Zuiun        {"瑞雲",                 Type_WB,         0,  0, 4, 2, 4, 6, 1, 0, NoneRange,   0};
	weapon WS_Zero         {"零式水上偵察機",       Type_WS,         0,  0, 1, 1, 2, 5, 1, 0, NoneRange,   0};
	weapon PF_21           {"零式艦戦21型",         Type_PF,         0,  0, 0, 5, 0, 0, 0, 0, NoneRange,   0};
	weapon PBF_62          {"零式艦戦62型(爆戦)",   Type_PBF,        0,  0, 4, 4, 3, 0, 0, 0, NoneRange,   0};
	weapon PB_99           {"九九式艦爆",           Type_PB,         0,  0, 5, 0, 3, 0, 0, 0, NoneRange,   0};
	weapon PB_SuiSei       {"彗星",                 Type_PB,         0,  0, 8, 0, 3, 0, 0, 0, NoneRange,   0};
	weapon PA_97           {"九七式艦攻",           Type_PA,         0,  5, 0, 0, 4, 1, 0, 0, NoneRange,   0};
	weapon PA_Tenzan       {"天山",                 Type_PA,         0,  7, 0, 0, 3, 1, 0, 0, NoneRange,   0};
	weapon AAG_77_Single   {"7.7mm機銃",            Type_AAG,        0,  0, 0, 2, 0, 0, 0, 1, NoneRange,   0};
	//艦娘  変数名              艦名,     艦種,   Lv, 耐, 火, 装, 雷, 回, 空, 潜, 速力,      索, 射程,        運, ス,艦1,艦2,艦3,艦4
	kammusu Fubuki_Fubuki_0    ("吹雪",   SC_DD,   1, 15, 10,  5, 27, 40, 10, 20, HighSpeed,  5, ShortRange,  17, 2);
	kammusu Kuma_Oi_1          ("大井改", SC_CLT, 10, 32,  8, 14, 80, 37, 13, 28, HighSpeed, 11, MiddleRange, 10, 2);
	kammusu Mogami_Mogami_1    ("最上改", SC_CAV, 10, 50, 28, 39, 23, 38, 24,  0, HighSpeed, 25, MiddleRange, 10, 4,  5,  6,  5,  3);
	kammusu Akagi_Akagi_0      ("赤城",   SC_CV,   1, 69,  0, 28,  0, 28, 32,  0, HighSpeed, 44, ShortRange,  12, 4, 18, 18, 27, 10);
	kammusu Ise_Ise_1          ("伊勢改", SC_BBV, 10, 77, 63, 75,  0, 36, 45,  0, LowSpeed,  22, LongRange,   30, 4, 11, 11, 11, 14);
	kammusu E_E401_0           ("伊401",  SC_SSV,  1, 20,  2,  5, 36, 13,  0,  0, LowSpeed,  15, ShortRange,  20, 1,  3);
	kammusu Kongoh_Kongoh_0    ("金剛",   SC_BB,   1, 63, 63, 52,  0, 30, 24,  0, HighSpeed, 13, LongRange,   12, 3,  3,  3,  3);
	kammusu Taihoh_Taihoh_0    ("大鳳",   SC_ACV,  1, 67,  0, 40,  0, 33, 42,  0, HighSpeed, 47, ShortRange,   2, 4, 18, 18, 18,  7);
	kammusu HohShoh_HohShoh_0  ("鳳翔",   SC_CVL,  1, 30,  0, 15,  0, 24, 10,  0, LowSpeed,  32, ShortRange,  20, 2,  8, 11);
	kammusu Chitose_Chitose_2  ("千歳甲", SC_AV,  12, 42, 10, 22, 18, 29, 19,  0, HighSpeed, 34, ShortRange,  10, 3, 12,  6,  6);
	kammusu Hurutaka_Hurutaka_0("古鷹",   SC_CA,   1, 36, 30, 25, 12, 33, 16,  0, HighSpeed, 10, MiddleRange, 10, 3,  2,  2,  2);
	kammusu Tenryuu_Tenryuu_0  ("天龍",   SC_CL,   1, 23, 11,  7, 18, 35,  8, 18, HighSpeed,  7, MiddleRange, 17, 2);
	//艦隊 変数名
	fleets First_Fleets(10), Second_Fleets(10);
	//改装
	Fubuki_Fubuki_0.Weapons[0] = Gun_127_Multi;
	Fubuki_Fubuki_0.Weapons[1] = Torpedo_61_3;
	Kuma_Oi_1.Weapons[0] = Torpedo_61_4_Oxy;
	Kuma_Oi_1.Weapons[1] = Torpedo_61_4_Oxy;
	Mogami_Mogami_1.Weapons[0] = Gun_203_Multi;
	Mogami_Mogami_1.Weapons[1] = WB_Zuiun;
	Mogami_Mogami_1.Weapons[2] = WS_Zero;
	Akagi_Akagi_0.Weapons[0] = PF_21;
	Akagi_Akagi_0.Weapons[1] = PB_99;
	Akagi_Akagi_0.Weapons[2] = PA_97;
	Ise_Ise_1.Weapons[0] = Gun_356_Multi;
	Ise_Ise_1.Weapons[1] = WB_Zuiun;
	Ise_Ise_1.Weapons[2] = Gun_356_Multi;
	Kongoh_Kongoh_0.Weapons[0] = Gun_356_Multi;
	Kongoh_Kongoh_0.Weapons[1] = Gun_152_Single;
	Kongoh_Kongoh_0.Weapons[2] = AAG_77_Single;
	Taihoh_Taihoh_0.Weapons[0] = PBF_62;
	Taihoh_Taihoh_0.Weapons[1] = PB_SuiSei;
	Taihoh_Taihoh_0.Weapons[2] = PA_Tenzan;
	HohShoh_HohShoh_0.Weapons[0] = PB_99;
	Chitose_Chitose_2.Weapons[0] = WS_Zero;
	Chitose_Chitose_2.Weapons[1] = SpecialSS_Kou;
	Chitose_Chitose_2.Weapons[2] = SpecialSS_Kou;
	Hurutaka_Hurutaka_0.Weapons[0] = Gun_203_Multi;
	Hurutaka_Hurutaka_0.Weapons[1] = AAG_77_Single;
	Tenryuu_Tenryuu_0.Weapons[0] = Gun_140_Single;
	Tenryuu_Tenryuu_0.Weapons[1] = AAG_77_Single;
	//編成
	First_Fleets.SetKammusu(Fubuki_Fubuki_0);
	First_Fleets.SetKammusu(Kuma_Oi_1);
	First_Fleets.SetKammusu(Mogami_Mogami_1);
	First_Fleets.SetKammusu(Akagi_Akagi_0);
	First_Fleets.SetKammusu(Ise_Ise_1);
	First_Fleets.SetKammusu(E_E401_0);
	Second_Fleets.SetKammusu(Kongoh_Kongoh_0);
	Second_Fleets.SetKammusu(Taihoh_Taihoh_0);
	Second_Fleets.SetKammusu(HohShoh_HohShoh_0);
	Second_Fleets.SetKammusu(Chitose_Chitose_2);
	Second_Fleets.SetKammusu(Hurutaka_Hurutaka_0);
	Second_Fleets.SetKammusu(Tenryuu_Tenryuu_0);
}*/
