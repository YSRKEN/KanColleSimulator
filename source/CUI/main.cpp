/* ---------------------------- */
/*  艦これシミュレータ Ver.0.6  */
/* ---------------------------- */

/*
   最終更新日時：2016/02/10
   製作者：@YSRKEN
   コマンドライン引数：
     「KanColleSim 試行回数 艦隊データ(先攻) 艦隊データ(後攻) 陣形(先攻) 陣形(後攻)」
     ・試行回数……シミュレートする回数(自然数)。「1」を指定すると、戦闘の途中経過も表示されるが、
                   2以上を指定すると途中経過表示が無くなり、代わりに複数回のシミュレートにおける
                   艦娘の残り耐久(平均)と、戦闘結果(完全勝利S～敗北D)の割合が表示される。
     ・艦隊データ……サンプルをfleet1.txt～fleet3.txtに示した。先攻はその名の通り、
                     相手に先に攻撃する方になる。仕様上、先攻の方が有利ではある。
                     ちなみに後攻に限り、「*.map」を読み込むとマップモードとなる。
     ・陣形……単縦陣・複縦陣・輪形陣・梯形陣・単横陣から1つ選択。マップモードだと後攻は無視され、
               自陣形は対潜マスは単横陣・開幕夜戦は指定・通常は単縦陣となる。
     なお、コマンドライン引数は5つとも全て指定する必要がある。
   呼び出し例：
     「KanColleSim 10000 fleet1.txt fleet2.txt 単縦陣 単縦陣」
     「KanColleSim 10000 fleet1.txt 1-5_high.map 梯形陣 ほげ」
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
		string FleetsDataName[2];
		for(int i = 0; i < BattleSize; ++i){
			FleetsDataName[i] = argv[i + 2];
		}
		if (FleetsDataName[1].substr(FleetsDataName[1].size() - 3, 3) != "map") {	// 通常モード
			fleets FleetsData[BattleSize];
			for (int i = 0; i < BattleSize; ++i) {
				FleetsData[i].ReadData(FleetsDataName[i]);
			}
			// 陣形
			for (int i = 0; i < BattleSize; ++i) {
				string FormationString(argv[i + 4]);
				if (FormationString == "単縦陣") {
					FleetsData[i].Formation = FOR_TRAIL;
				}
				else if (FormationString == "複縦陣") {
					FleetsData[i].Formation = FOR_SUBTRAIL;
				}
				else if (FormationString == "輪形陣") {
					FleetsData[i].Formation = FOR_CIRCLE;
				}
				else if (FormationString == "梯形陣") {
					FleetsData[i].Formation = FOR_ECHELON;
				}
				else if (FormationString == "単横陣") {
					FleetsData[i].Formation = FOR_ABREAST;
				}
				else {
					throw "その陣形は存在しません";
				}
			}
			/* 第1→第2艦隊への攻撃 */
			if (BattleCount == 1) {
				// 1回のみ
				FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, kModeDN);
			}
			else {
				// 複数回(統計モード)
				//準備
				fleets FleetsData_[BattleSize];
				vector< vector<int> > HPSum(BattleSize);
				vector< vector<int> > HeavyDamageCount(BattleSize);
				vector< vector<int> > LostCount(BattleSize);
				for (int i = 0; i < BattleSize; ++i) {
					HPSum[i].resize(FleetsData[i].Members, 0);
					HeavyDamageCount[i].resize(FleetsData[i].Members, 0);
					LostCount[i].resize(FleetsData[i].Members, 0);
				}
				//複数回試行して集計する
				vector<int> WinReason(WIN_Size, 0);
				for (int n = 0; n < BattleCount; ++n) {
					for (int i = 0; i < BattleSize; ++i) {
						FleetsData_[i] = FleetsData[i];
					}
					++WinReason[FleetsData_[FriendSide].Simulate(FleetsData_[EnemySide], false, kModeDN)];
					for (int i = 0; i < BattleSize; ++i) {
						for (int j = 0; j < FleetsData[i].Members; ++j) {
							if (FleetsData_[i].Kammusues[j].ShowDamage() == HeavyDamage) {
								HeavyDamageCount[i][j]++;
							}
							if (FleetsData_[i].Kammusues[j].ShowDamage() == Lost) {
								LostCount[i][j]++;
							}
							HPSum[i][j] += FleetsData_[i].Kammusues[j].HP;
						}
					}
				}
				//結果を表示する
				cout << "【結果表示】\n";
				for (int i = 0; i < BattleSize; ++i) {
					cout << "○" << Position[i] << "\n";
					for (int j = 0; j < FleetsData[i].Members; ++j) {
						FleetsData[i].Kammusues[j].HP = static_cast<int>(1.0 * HPSum[i][j] / BattleCount);
						cout << "　" << FleetsData[i].Kammusues[j].Label() << " "
							<< 1.0 * HPSum[i][j] / BattleCount << "/" << FleetsData[i].Kammusues[j].MaxHP
							<< "(" << DMString[FleetsData[i].Kammusues[j].ShowDamage()] << ") ";
						cout << "大破率：" << (100.0 * HeavyDamageCount[i][j] / BattleCount) << "％ ";
						cout << "撃沈率：" << (100.0 * LostCount[i][j] / BattleCount) << "％\n";
					}
				}
				double WinPer = 100.0 * (WinReason[WIN_SS] + WinReason[WIN_S] + WinReason[WIN_A] + WinReason[WIN_B]) / BattleCount;
				cout << "○勝利判定の分布(勝率：" << WinPer << "％)\n";
				for (int i = WIN_Size - 1; i >= 0; --i) {
					cout << "　" << WINString[i] << "：" << WinReason[i] << "回(" << 100.0 * WinReason[i] / BattleCount << "％)\n";
				}
			}
		}
		else {	// マップモード
			// 自艦隊の情報を読み込む
			fleets MyFleetsData;
			MyFleetsData.ReadData(FleetsDataName[0]);
			// マップの情報を読み込む
			vector<vector<fleets>> MapData;
			ReadMapData(MapData, FleetsDataName[1]);
			// 夜戦用陣形を読み込む
			string temp(argv[4]);
			FORMATION night_formation;
			if (temp == "単縦陣") {
				night_formation = FOR_TRAIL;
			}
			else if (temp == "複縦陣") {
				night_formation = FOR_SUBTRAIL;
			}
			else if (temp == "輪形陣") {
				night_formation = FOR_CIRCLE;
			}
			else if (temp == "梯形陣") {
				night_formation = FOR_ECHELON;
			}
			else if (temp == "単横陣") {
				night_formation = FOR_ABREAST;
			}
			else {
				throw "その陣形は存在しません";
			}
			// 戦闘をシミュレートする
			fleets FleetsData[BattleSize];
			if (BattleCount == 1) {
				// 1回のみ
				FleetsData[0] = MyFleetsData;
				for (rsize_t i = 0; i < MapData.size(); ++i) {
					cout << "《" << (i + 1) << "戦目》\n";
					// 1戦毎にランダムに編成を選び、あてがう
					FleetsData[1] = MapData[i][RandInt(MapData[i].size())];
					// 自陣形を敵編成から判断する
					FleetsData[FriendSide].Formation = FOR_TRAIL;
						// 敵旗艦が潜水艦ならば単横陣
					if (FleetsData[EnemySide].Kammusues[0].isSubmarine()) {
						FleetsData[FriendSide].Formation = FOR_ABREAST;
					}
					if(i != MapData.size() - 1){
						// 道中
						FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, kModeD);
						// 誰も大破していないかを調べる
						if (FleetsData[FriendSide].hasHeavyDamage()) {
							cout << "●道中撤退！●\n";
							break;
						}
					}else {
						// ボス
						FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, kModeDN);
					}
				}
				cout << "●終了！●\n";
			}else {
				// 複数回行う
				//準備
				fleets FleetsData[BattleSize];
				rsize_t areas = MapData.size(), members = MyFleetsData.Members;
				vector<vector<int>> WinReason(areas, vector<int>(WIN_Size, 0));
				vector<vector<int>> LossReason(areas - 1, vector<int>(members, 0));
				//複数回試行して集計する
				for (int n = 0; n < BattleCount; ++n) {
					FleetsData[0] = MyFleetsData;
					for (rsize_t i = 0; i < areas; ++i) {
						// 1戦毎にランダムに編成を選び、あてがう
						FleetsData[1] = MapData[i][RandInt(MapData[i].size())];
						// 自陣形を敵編成から判断する
						FleetsData[FriendSide].Formation = FOR_TRAIL;
						// 敵旗艦が潜水艦ならば単横陣
						if (FleetsData[EnemySide].Kammusues[0].isSubmarine()) {
							FleetsData[FriendSide].Formation = FOR_ABREAST;
						}
						// シミュレート(勝利判定をカウント)
						if (i != MapData.size() - 1) {
							// 道中
							++WinReason[i][FleetsData[FriendSide].Simulate(FleetsData[EnemySide], false, kModeD)];
						}else {
							// ボス
							++WinReason[i][FleetsData[FriendSide].Simulate(FleetsData[EnemySide], false, kModeDN)];
						}
						// 誰も大破していないかを調べる
						if (i != areas - 1) {
							bool loss_flg = false;
							for (rsize_t j = 0; j < members; ++j) {
								if (FleetsData[FriendSide].Kammusues[j].ShowDamage() >= HeavyDamage) {
									++LossReason[i][j];
									loss_flg = true;
								}
							}
							if (loss_flg) {
								break;
							}
						}
					}
				}
				// 結果を出力する
				cout << "【結果表示】\n";
				vector<int> sum(areas, 0);
				for (rsize_t i = 0; i < areas; ++i) {
					for (auto &it : WinReason[i]) {
						sum[i] += it;
					}
				}
				for (rsize_t i = 0; i < areas; ++i) {
					cout << "《" << (i + 1) << "戦目》到達率：" << (100.0 * sum[i] / BattleCount) << "％\n";
					if (i != areas - 1) {
						cout << "○撤退要因(撤退率：" << (100.0 * (sum[i] - sum[i + 1]) / sum[i]) << "％)\n";
						for (rsize_t j = 0; j < members; ++j) {
							cout << "　" << FleetsData[0].Kammusues[j].Label() << "：" << LossReason[i][j] << "回(" << (100.0 * LossReason[i][j] / sum[i]) << "％)\n";
						}
					}
					double WinPer = 100.0 * (WinReason[i][WIN_SS] + WinReason[i][WIN_S] + WinReason[i][WIN_A] + WinReason[i][WIN_B]) / sum[i];
					cout << "○勝利判定の分布(勝率：" << WinPer << "％)\n";
					for (int j = WIN_Size - 1; j >= 0; --j) {
						cout << "　" << WINString[j] << "：" << WinReason[i][j] << "回(" << (100.0 * WinReason[i][j] / sum[i]) << "％)\n";
					}
				}
			}
		}
	}
	catch(char *e){
		cout << "エラー：" << e << std::endl;
	}
}
