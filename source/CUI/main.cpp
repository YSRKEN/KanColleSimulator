/* ---------------------------- */
/*  艦これシミュレータ Ver.0.8  */
/* ---------------------------- */

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
				auto result = FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, kModeDN);
				cout << "MVP艦：" << FleetsData[FriendSide].Kammusues[get<1>(result)].Name << "\n";
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
				int kill_boss_count = 0;
				//複数回試行して集計する
				vector<int> WinReason(WIN_Size, 0);
				vector<int> MVPKammusu(MaxKanmusu, 0);
				auto start_time = std::chrono::system_clock::now();
				for (int n = 0; n < BattleCount; ++n) {
					for (int i = 0; i < BattleSize; ++i) {
						FleetsData_[i] = FleetsData[i];
					}
					auto result = FleetsData_[FriendSide].Simulate(FleetsData_[EnemySide], false, kModeDN);
					++WinReason[get<0>(result)];
					++MVPKammusu[get<1>(result)];
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
					if (FleetsData_[EnemySide].Kammusues[0].HP == 0) ++kill_boss_count;
				}
				auto end_time = std::chrono::system_clock::now();
				//結果を表示する
				cout << "【結果表示】\n";
				cout << "処理時間：" << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "[us]\n";
				for (int i = 0; i < BattleSize; ++i) {
					cout << "○" << Position[i] << "\n";
					for (int j = 0; j < FleetsData[i].Members; ++j) {
						FleetsData[i].Kammusues[j].HP = static_cast<int>(1.0 * HPSum[i][j] / BattleCount);
						cout << "　" << FleetsData[i].Kammusues[j].Label() << " "
							<< 1.0 * HPSum[i][j] / BattleCount << "/" << FleetsData[i].Kammusues[j].MaxHP
							<< "(" << DMString[FleetsData[i].Kammusues[j].ShowDamage()] << ") ";
						cout << "大破率：" << (100.0 * HeavyDamageCount[i][j] / BattleCount) << "％ ";
						cout << "撃沈率：" << (100.0 * LostCount[i][j] / BattleCount) << "％";
						if (i == FriendSide) {
							cout << " MVP率：" << (100.0 * MVPKammusu[j] / BattleCount) << "％\n";
						}
						else {
							cout << "\n";
						}
					}
				}
				double WinPer = 100.0 * (WinReason[WIN_SS] + WinReason[WIN_S] + WinReason[WIN_A] + WinReason[WIN_B]) / BattleCount;
				cout << "○勝利判定の分布(勝率：" << WinPer << "％)\n";
				for (int i = WIN_Size - 1; i >= 0; --i) {
					cout << "　" << WINString[i] << "：" << WinReason[i] << "回(" << 100.0 * WinReason[i] / BattleCount << "％)\n";
				}
				cout << "\n旗艦撃破：" << kill_boss_count << "回(" << (100.0 * kill_boss_count / BattleCount) << "％)\n";
			}
		}
		else {	// マップモード
			// 自艦隊の情報を読み込む
			fleets MyFleetsData;
			MyFleetsData.ReadData(FleetsDataName[0]);
			// マップの情報を読み込む
			vector<vector<fleets>> MapData;
			vector<vector<kSimulateMode>> MapSim;
			ReadMapData(MapData, MapSim, FleetsDataName[1]);
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
					rsize_t j = RandInt(MapData[i].size());
					FleetsData[1] = MapData[i][j];
					// 自陣形を敵編成から判断する
					FleetsData[FriendSide].Formation = FOR_TRAIL;
					//敵旗艦が潜水艦ならば単横陣
					if (FleetsData[EnemySide].Kammusues[0].isSubmarine()) {
						FleetsData[FriendSide].Formation = FOR_ABREAST;
					}
					//夜戦マスなら指定した陣形にする
					if (MapSim[i][j] == kModeN) {
						FleetsData[FriendSide].Formation = night_formation;
					}
					if(i != MapData.size() - 1){
						// 道中
						FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, MapSim[i][j]);
						// 誰も大破していないかを調べる
						if (FleetsData[FriendSide].hasHeavyDamage()) {
							cout << "●道中撤退！●\n";
							break;
						}
					}else {
						// ボス
						FleetsData[FriendSide].Simulate(FleetsData[EnemySide], true, MapSim[i][j]);
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
				int kill_boss_count = 0;
				//複数回試行して集計する
				auto start_time = std::chrono::system_clock::now();
				for (int n = 0; n < BattleCount; ++n) {
					FleetsData[0] = MyFleetsData;
					for (rsize_t i = 0; i < areas; ++i) {
						// 1戦毎にランダムに編成を選び、あてがう
						rsize_t j = RandInt(MapData[i].size());
						FleetsData[1] = MapData[i][j];
						// 自陣形を敵編成から判断する
						FleetsData[FriendSide].Formation = FOR_TRAIL;
						//敵旗艦が潜水艦ならば単横陣
						if (FleetsData[EnemySide].Kammusues[0].isSubmarine()) {
							FleetsData[FriendSide].Formation = FOR_ABREAST;
						}
						//夜戦マスなら指定した陣形にする
						if (MapSim[i][j] == kModeN) {
							FleetsData[FriendSide].Formation = night_formation;
						}
						// シミュレート(勝利判定をカウント)
						++WinReason[i][get<0>(FleetsData[FriendSide].Simulate(FleetsData[EnemySide], false, MapSim[i][j]))];
						// 誰も大破していないかを調べる
						if (i != areas - 1) {
							bool loss_flg = false;
							for (rsize_t j = 0; j < members; ++j) {
								if (FleetsData[FriendSide].Kammusues[j].ShowDamage() >= HeavyDamage) {
									++LossReason[i][j];
									loss_flg = true;
									break;
								}
							}
							if (loss_flg) {
								break;
							}
						}
						else {
							if (FleetsData[EnemySide].Kammusues[0].HP == 0) ++kill_boss_count;
						}
					}
					/*if (((n + 1) % 100) == 0) {
						cout << (n + 1) << "\t";
						cout << (WinReason[0][WIN_SS] + WinReason[0][WIN_S] + WinReason[0][WIN_A] + WinReason[0][WIN_B]) << "\t";
						cout << kill_boss_count << "\n";
					}*/
				}
				auto end_time = std::chrono::system_clock::now();
				// 結果を出力する
				cout << "【結果表示】\n";
				cout << "処理時間：" << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "[us]\n";
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
				cout << "\nボス旗艦撃破：" << kill_boss_count << "回(" << (100.0 * kill_boss_count / BattleCount) << "％)\n";
				/*rsize_t areas = MapData.size(), members = MyFleetsData.Members;
				vector<int> data; for (int i = 0; i < 6; ++i) data.push_back(i);
				vector<vector<int>> data2;
				do {
					data2.push_back(data);
				} while (next_permutation(data.begin(), data.end()));
				vector<rsize_t> win_count(720, 0), kill_boss_count(720, 0);
				auto start_time = std::chrono::system_clock::now();
				#pragma omp parallel for
				for (int x = 0; x < 720; ++x) {
					fleets MyFleetsData2 = MyFleetsData;
					for (int i = 0; i < 6; ++i) {
						MyFleetsData2.Kammusues[i] = MyFleetsData.Kammusues[data2[x][i]];
					}
					fleets FleetsData[BattleSize];
					for (int n = 0; n < BattleCount; ++n) {
						FleetsData[0] = MyFleetsData2;
						for (rsize_t i = 0; i < areas; ++i) {
							rsize_t j = RandInt(MapData[i].size());
							FleetsData[1] = MapData[i][j];
							FleetsData[FriendSide].Formation = FOR_TRAIL;
							if (FleetsData[EnemySide].Kammusues[0].isSubmarine()) {
								FleetsData[FriendSide].Formation = FOR_ABREAST;
							}
							if (MapSim[i][j] == kModeN) {
								FleetsData[FriendSide].Formation = night_formation;
							}
							// シミュレート(勝利判定をカウント)
							WIN reason = get<0>(FleetsData[FriendSide].Simulate(FleetsData[EnemySide], false, MapSim[i][j]));
							if (reason >= WIN_B) ++win_count[x];
							// 誰も大破していないかを調べる
							if (i != areas - 1) {
								bool loss_flg = false;
								for (rsize_t j = 0; j < members; ++j) {
									if (FleetsData[FriendSide].Kammusues[j].ShowDamage() >= HeavyDamage) {
										loss_flg = true;
										break;
									}
								}
								if (loss_flg) {
									break;
								}
							}
							else {
								if (FleetsData[EnemySide].Kammusues[0].HP == 0) ++kill_boss_count[x];
							}
						}
					}
				}
				auto end_time = std::chrono::system_clock::now();
				for (int x = 0; x < 720; ++x) {
					for (int i = 0; i < 6; ++i) {
						cout << MyFleetsData.Kammusues[data2[x][i]].Name.substr(0, 2);
					}
					cout << "," << win_count[x] << "," << kill_boss_count[x] << "\n";
				}
				cout << "処理時間：" << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "[us]\n";*/
			}
		}
	}
	catch(char *e){
		cout << "エラー：" << e << std::endl;
	}
}
