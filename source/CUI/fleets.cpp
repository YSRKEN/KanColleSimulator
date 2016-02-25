#include "header.h"

using std::fstream;

/* プロトタイプ宣言 */
//メイン
void ShowListStart(fleets**, const bool isShow = true);
void SearchPhase(fleets**, bool*, const bool isShow = true);
BP BPSelect(fleets**);
AIR_MAS AirWarPhase(fleets**, const bool*, double[], vector<size_t>&, const bool isShow = true);
void FirstTorpedoPhase(fleets**, const BP, const double[], vector<size_t>&, const bool isShow = true);
void FirePhase(fleets**, const BP, const AIR_MAS, const double[], vector<size_t>&, const bool isShow = true);
void FirePhase2(fleets**, const BP, const AIR_MAS, const double[], vector<size_t>&, const bool isShow = true);
void TorpedoPhase(fleets**, const BP, const double[], vector<size_t>&, const bool isShow = true);
void NightPhase(fleets**, const BP, const double[], vector<size_t>&, const bool, const bool isShow = true);
WIN ShowListEnd(fleets**, const bool, const BP, const bool isShow = true);
//補助
int RoundUp5(int);
bool isExit(fleets**);
bool isAllSubmarine1(fleets*);
bool CheckPercent(const double);
int AttackAction(fleets*, fleets*, const int, int&, const int, const BP, const double, const TURN,
                 const bool isShow = true, const double Multiple = 1.0, const bool isSpecialAttack = false);
TYPE ToType(const string);
int ToInt(const string);
RANGE ToRange(const string);
SC ToKind(const string);
SPEED ToSpeed(const string);

/* ---------------------- */
/* |  シミュレート関係  | */
/* | (メインとなる関数) | */
/* ---------------------- */

/* 戦闘をシミュレートする */
tuple<WIN, size_t> fleets::Simulate(fleets &Enemy, const bool isShow, const kSimulateMode simulate_mode) {
	fleets* Fleets[] = {this, &Enemy};
	bool is_night_game = false;	//夜戦に突入したらtrue
	vector<size_t> all_damage(this->Members,0);
	/* 0. 情報表示 */
	ShowListStart(Fleets, isShow);
	/* 1. 索敵フェイズ */
	bool isSearchSuccess[2];
	SearchPhase(Fleets, isSearchSuccess, isShow);
	/* 1.5. 艦隊行動の決定 */
	BP BattlePosition = BPSelect(Fleets);
	bool isShowBP = false;	//開幕雷撃までに終了した際に、交戦形態を表示するためのオプション
	if (simulate_mode != kModeN) {
		/* 2. 航空戦フェイズ */
		double AllAttackPlus[BattleSize];
		AIR_MAS AirWarResult = AirWarPhase(Fleets, isSearchSuccess, AllAttackPlus, all_damage, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 3. 開幕雷撃フェイズ */
		FirstTorpedoPhase(Fleets, BattlePosition, AllAttackPlus, all_damage, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 4. 交戦形態フェイズ */
		if (isShow) cout << "【交戦形態フェイズ】\n";
		if (isShow) cout << "　交戦形態：" << BPString[BattlePosition] << "\n";
		isShowBP = true;
		/* 5. 砲撃戦フェイズ */
		FirePhase(Fleets, BattlePosition, AirWarResult, AllAttackPlus, all_damage, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 5.5. 砲撃戦フェイズ(二巡目) */
		FirePhase2(Fleets, BattlePosition, AirWarResult, AllAttackPlus, all_damage, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 6. 雷撃戦フェイズ */
		TorpedoPhase(Fleets, BattlePosition, AllAttackPlus, all_damage, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 7. 夜戦フェイズ */
		if (simulate_mode == kModeDN) {
			is_night_game = true;
			NightPhase(Fleets, BattlePosition, AllAttackPlus, all_damage, false, isShow);
			if (isExit(Fleets)) goto Exit;
		}
	}
	else {
		/* 夜戦フェイズ2 */
		if (simulate_mode == kModeN) {
			double AllAttackPlus[BattleSize];
			AllAttackPlus[0] = AllAttackPlus[1] = 1.0;
			NightPhase(Fleets, BattlePosition, AllAttackPlus, all_damage, true, isShow);
			if (isExit(Fleets)) goto Exit;
		}
	}
Exit:
	/* X. 結果表示 */
	WIN WinReason = ShowListEnd(Fleets, isShowBP, BattlePosition, isShow);
	/*if (!isShow) {
		for (auto &it : all_damage) {
			cout << it << " ";
		}
		cout << "\n";
	}*/
	auto max_iter = std::max_element(all_damage.begin(), all_damage.end());
	size_t mvp_index = std::distance(all_damage.begin(), max_iter);

	/* 弾薬補正 */
	for (auto &it : this->Kammusues) {
		it.Ammo -= 20;
		if (it.Ammo < 0) it.Ammo = 0;
	}

	/* cond補正 */
	//艦隊全体に掛かるもの
	int cond_change = 0;
	switch(simulate_mode){
	case kModeD:
		cond_change -= 3;
		break;
	case kModeN:
		cond_change -= 2;
		break;
	case kModeDN:
		cond_change -= 3;
		if (is_night_game) cond_change -= 2;
		break;
	}
	switch (WinReason) {
	case WIN_SS:
		cond_change += 4;
		break;
	case WIN_S:
		cond_change += 4;
		break;
	case WIN_A:
		cond_change += 3;
		break;
	case WIN_B:
		cond_change += 2;
		break;
	case WIN_C:
		cond_change += 1;
		break;
	case WIN_D:
		break;
	}
	for (auto& it : Fleets[FriendSide]->Kammusues) {
		it.changeCond(cond_change);
	}
	//個別に適用されるもの
	Fleets[FriendSide]->Kammusues[0].changeCond(3);
	Fleets[FriendSide]->Kammusues[mvp_index].changeCond(10);

	if(isShow) cout << "\n";
	return tuple<WIN, int>(WinReason, mvp_index);
}

/* 情報表示 */
void ShowListStart(fleets **Fleets, const bool isShow) {
	if(isShow) cout << "【情報表示】\n";
	for(int i = 0; i < BattleSize; ++i) {
		if(isShow) cout << "○" << Position[i] << "\n";
		Fleets[i]->ShowList(isShow);
	}
	if(isShow) cout << "○陣形\n";
	if(isShow) cout << "　" << FORString[Fleets[FriendSide]->Formation] << "vs" << FORString[Fleets[EnemySide]->Formation] << "\n";
}

/* 索敵フェイズ */
void SearchPhase(fleets **Fleets, bool *isSearchSuccess, const bool isShow) {
	if(isShow) cout << "【索敵フェイズ】\n";
	for(int i = 0; i < BattleSize; ++i) {
		double SearchPower = Fleets[i]->CalcSearchPower();
		if(isShow) cout << "　" << Position[i] << "索敵値：" << SearchPower << "\n";
		//索敵の成功条件がよく分からないので実は適当だったりする
		if(SearchPower > 10.0) {
			if(isShow) cout << "　" << Position[i] << "策敵結果：成功\n";
			isSearchSuccess[i] = true;
		} else {
			if(isShow) cout << "　" << Position[i] << "策敵結果：失敗\n";
			isSearchSuccess[i] = false;
		}
	}
}

/* 交戦形態の決定 */
BP BPSelect(fleets **Fleets) {
	double p = Rand(mt);
	if(p < 0.45) {
		return BP_SAME;		//同航戦
	} else if(p < 0.75) {
		return BP_DIFF;		//反航戦
	} else if(p < 0.90) {
		return BP_T_PLUS;	//T字有利
	} else {
		// 彩雲がいた場合は反航戦、そうでない場合はT字不利
		if((Fleets[FriendSide]->hasSaiun()) || (Fleets[EnemySide]->hasSaiun())) {
			return BP_DIFF;
		} else {
			return BP_T_MINUS;
		}
	}
}

/* 航空戦フェイズ */
AIR_MAS AirWarPhase(fleets **Fleets, const bool *isSearchSuccess, double AllAttackPlus[], vector<size_t> &all_damage, const bool isShow) {
	if(isShow) cout << "【航空戦フェイズ】\n";
	/* 制空状態の決定 */
	if(isShow) cout << "○制空権の決定\n";
	// 制空値の計算
	int AntiAirScore[BattleSize];
	bool hasAir[BattleSize];
	double bonus_all[] = { 0,1,1,2,2,2,2,3 }, bonus_pf[] = { 0,0,2,5,9,14,14,22 }, bonus_wb[] = {0,0,1,1,1,3,3,6};
	for(int i = 0; i < BattleSize; ++i) {
		AntiAirScore[i] = 0;
		hasAir[i] = false;
		if(!isSearchSuccess[i]) continue;	//索敵失敗時は制空値0
		for(auto &it : Fleets[i]->Kammusues) {
			for(int j = 0; j < it.Slots; ++j) {
				weapon &weapon_temp = it.Weapons[j];
				if((weapon_temp.isAirWar()) && (it.Airs[j] != 0)) hasAir[i] = true;
				int level = 0;
				if (weapon_temp.level_ <= -100) {
					level = 7;
				}
				else if (weapon_temp.level_ <= -85) {
					level = 6;
				}
				else if (weapon_temp.level_ <= -70) {
					level = 5;
				}
				else if (weapon_temp.level_ <= -55) {
					level = 4;
				}
				else if (weapon_temp.level_ <= -40) {
					level = 3;
				}
				else if (weapon_temp.level_ <= -25) {
					level = 2;
				}
				else if (weapon_temp.level_ <= -10) {
					level = 1;
				}
				if(weapon_temp.isAirWar()) {
					double aas_temp = weapon_temp.AntiAir * sqrt(it.Airs[j]);
					switch (weapon_temp.Type) {
					case Type_PF:
						if (weapon_temp.level_ >= 0) {
							aas_temp += bonus_pf[weapon_temp.level_] + bonus_all[weapon_temp.level_];
						}
						else {
							aas_temp += bonus_pf[level] + sqrt(-weapon_temp.level_ / 10);
						}
						break;
					case Type_WB:
						if (weapon_temp.level_ >= 0) {
							aas_temp += bonus_wb[weapon_temp.level_] + bonus_all[weapon_temp.level_];
						}
						else {
							aas_temp += bonus_wb[level] + sqrt(-weapon_temp.level_ / 10);
						}
						break;
					default:
						if (weapon_temp.level_ >= 0) {
							aas_temp += bonus_all[weapon_temp.level_];
						}
						else {
							aas_temp += sqrt(-weapon_temp.level_ / 10);
						}
						break;
					}
					AntiAirScore[i] += static_cast<int>(aas_temp);
				}
				if (weapon_temp.level_ <= 0) weapon_temp.level_ = level;
			}
		}
		if(isShow) cout << "　" << Position[i] << "制空値：" << AntiAirScore[i] << "\n";
	}
	// 制空状態の確認
	AIR_MAS AirWarResult;
	if(((!hasAir[FriendSide]) && (!hasAir[EnemySide]))
	|| ((!isSearchSuccess[FriendSide]) && (!hasAir[EnemySide]))
	|| ((!isSearchSuccess[EnemySide]) && (!hasAir[FriendSide]))
	|| (isAllSubmarine1(Fleets[EnemySide]))){
		AirWarResult = AM_NORMAL;
	}else if(AntiAirScore[FriendSide] * 3 < AntiAirScore[EnemySide]){
		AirWarResult = AM_WORST;
	} else if(AntiAirScore[FriendSide] * 3 < AntiAirScore[EnemySide] * 2){
		AirWarResult = AM_BAD;
	} else if(AntiAirScore[FriendSide] * 2 < AntiAirScore[EnemySide] * 3){
		AirWarResult = AM_NORMAL;
	} else if(AntiAirScore[FriendSide] < AntiAirScore[EnemySide] * 3){
		AirWarResult = AM_GOOD;
	} else{
		AirWarResult = AM_BEST;
	}
	if(isShow) cout << "　結果：" << AMString[AirWarResult] << "\n";
	/* 触接判定 */
	// この判定方法が正確なのかは微妙だけど流石にしゃーないかと思われ
	if(isShow) cout << "○触接判定\n";
	for(int i = 0; i < BattleSize; ++i) {
		AllAttackPlus[i] = 1.0;
		if(isShow) cout << "　" << Position[i] << "触接：";
		// 味方・敵共に航空劣勢以上でないと発動しない
		if(((i == FriendSide) && ((AirWarResult == AM_WORST) || (!hasAir[i])))
		|| ((i == EnemySide) && ((AirWarResult == AM_BEST) || (!hasAir[i])))){
			if(isShow) cout << "不可能\n";
			continue;
		}
		// 触接の開始率を計算する
		double trailer_aircraft_per = 0.0;
		for (auto &it : Fleets[i]->Kammusues) {
			for (int j = 0; j < it.Slots; ++j) {
				switch (it.Weapons[j].Type) {
				case Type_WS:
				case Type_WSN:
				case Type_PS:
				case Type_PSS:
				case Type_DaiteiChan:
					trailer_aircraft_per += 0.04 * it.Weapons[j].Search * sqrt(it.MaxAirs[j]);
				default:
					break;
				}
			}
		}
		switch (AirWarResult) {
		case AM_BEST:
			break;
		case AM_GOOD:
			trailer_aircraft_per *= 0.6;
			break;
		case AM_NORMAL:
			trailer_aircraft_per *= 0.4;
			break;
		case AM_BAD:
			trailer_aircraft_per *= 0.2;
			break;
		case AM_WORST:
			trailer_aircraft_per *= 0;
			break;
		}
		if (trailer_aircraft_per <= Rand(mt)) {
			if (isShow) cout << "可能(失敗)\n";
			continue;
		}
		// 触接の選択率を計算する
		const double aap_plus[] = { 1.12, 1.12, 1.17, 1.20 };
		[&] {
			for (auto &it : Fleets[i]->Kammusues) {
				for (int j = 0; j < it.Slots; ++j) {
					switch (it.Weapons[j].Type) {
					case Type_WS:
					case Type_WSN:
					case Type_PS:
					case Type_PSS:
					case Type_PA:
					case Type_DaiteiChan:
						if (0.07 * it.Weapons[j].Search >= Rand(mt)) {
							AllAttackPlus[i] = aap_plus[it.Weapons[j].Hit];
							return;
						}
					default:
						break;
					}
				}
			}
		}();
		if(AllAttackPlus[i] != 1.0) {
			if(isShow) cout << "可能(成功、＋" << static_cast<int>((AllAttackPlus[i] - 1) * 100) << "％)\n";
		} else{
			if(isShow) cout << "可能(失敗)\n";
		}
	}
	/* 空中戦 */
	// 撃墜割合は一様乱数にしたけどこれでいいのだろうか？
	// と言うかさり気なく自軍と敵軍で落とされ方がかなり違うってのは
	// シミュ作者泣かせだよな……まあ適当にでっち上げたが
	if(isShow) cout << "○戦闘機による敵艦載機の攻撃・撃墜\n";
	double KilledAirsPer[BattleSize];
	switch(AirWarResult){
		case AM_WORST:
			KilledAirsPer[FriendSide] = 1.0 * (RandInt(150 - 65 + 1) + 65) / 256;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.1;
			break;
		case AM_BAD:
			KilledAirsPer[FriendSide] = 1.0 * (RandInt(105 - 45 + 1) + 45) / 256;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.4;
			break;
		case AM_NORMAL:
			KilledAirsPer[FriendSide] = 1.0 * (RandInt(75 - 30 + 1) + 30) / 256;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.6;
			break;
		case AM_GOOD:
			KilledAirsPer[FriendSide] = 1.0 * (RandInt(45 - 20 + 1) + 20) / 256;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.8;
			break;
		case AM_BEST:
			KilledAirsPer[FriendSide] = 1.0 * (RandInt(15-7+1) + 7) / 256;
			KilledAirsPer[EnemySide]  = Rand(mt) * 1.0;
			break;
	}
	for(int i = 0; i < BattleSize; ++i){
		if(isShow) cout << "　" << Position[i] << "：" << floor(KilledAirsPer[i] * 1000 + 0.5) / 10 << "％\n";
	}
	for(int i = 0; i < BattleSize; ++i) {
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			for(int j = 0; j < itKammusu->Slots; ++j) {
				if(itKammusu->Weapons[j].isAirWar()) {
					itKammusu->Airs[j] -= static_cast<int>(itKammusu->Airs[j] * KilledAirsPer[i]);
				}
			}
		}
	}
	/* 防空攻撃・対空放火 */
	if(isShow) cout << "○対空火砲による敵攻撃機・爆撃機の迎撃・撃墜\n";
	for (int i = 0; i < BattleSize; ++i) {
		//艦隊対空ボーナス値を決定
		int FleetsAntiAirBonus = 0;
		for (auto &it_k : Fleets[i]->Kammusues) {
			double AntiAirBouns = 0.0;
			for (auto &it_w : it_k.Weapons) {
				if (it_w.Name.find("高角砲") != string::npos || it_w.Name.find("高射装置") != string::npos) {
					AntiAirBouns += 0.35 * it_w.AntiAir;
				}
				else if (it_w.Type == Type_LargeS || it_w.Type == Type_SmallS) {
					AntiAirBouns += 0.4 * it_w.AntiAir;
				}
				else if (it_w.Name == "三式弾") {
					AntiAirBouns += 0.6 * it_w.AntiAir;
				}
				else {
					AntiAirBouns += 0.2 * it_w.AntiAir;
				}
			}
			FleetsAntiAirBonus += static_cast<int>(AntiAirBouns);
		}
		FleetsAntiAirBonus = static_cast<int>(2 * AntiAirBonusPer[5 * (Fleets[i]->Kammusues[0].is_kammusu_ ? 0 : 1) + Fleets[i]->Formation] * FleetsAntiAirBonus);
		if (isShow) cout << "　" << (i == FriendSide ? "自軍" : "敵軍") << "　対空ボーナス値：" << FleetsAntiAirBonus << "\n";
		//対空カットイン判定を行う
		size_t aac_bonus_type = 0;
		[&] {
			// まず、秋月型カットイン以外の判定を行う
			for (auto &it_k : Fleets[i]->Kammusues) {
				size_t aac_bonus_type_ = it_k.getAAC();
				if (aac_bonus_type_ >= 4) {
					//※発動率は推測値
					if (it_k.getAACPer(aac_bonus_type_) >= Rand(mt)) {
						if (isShow) cout << "　" << "対空カットイン：成功(" << it_k.Name << ", 第" << aac_bonus_type_ << "種)\n";
						aac_bonus_type = aac_bonus_type_;
						return;
					}
				}
			}
			// 次に秋月型カットインの判定を行う
			for (auto &it_k : Fleets[i]->Kammusues) {
				size_t aac_bonus_type_ = it_k.getAAC();
				if (aac_bonus_type_ >= 1 && aac_bonus_type_ <= 3) {
					if (it_k.getAACPer(aac_bonus_type_) >= Rand(mt)) {
						if (isShow) cout << "　" << "対空カットイン：成功(" << it_k.Name << ", 第" << aac_bonus_type_ << "種)\n";
						aac_bonus_type = aac_bonus_type_;
						break;
					}
				}
			}
		}();
		//迎撃！
		int AllKilledAirs = 0;
		int aac_bonus_add1[] = { 0,7,6,4,6,4,4,3,4,2,8,6,3,0,4,3,4,2 };
		for (auto &it_k : Fleets[BattleSize - i - 1]->Kammusues) {
			for (int j = 0; j < it_k.Slots; ++j) {
				if (!it_k.Weapons[j].isAirWar2()) continue;
				kammusu *InterceptKammusu = &(Fleets[i]->Kammusues[Fleets[i]->RandomKammsu()]);
				int KilledAirs = 0;
				//割合撃墜
				if (RandInt(2) == 1) KilledAirs += static_cast<int>(it_k.Airs[j]);
				//固定撃墜
				if (RandInt(2) == 1) {
					if (InterceptKammusu->is_kammusu_) {
						KilledAirs += static_cast<int>(0.1 * (InterceptKammusu->AllAntiAir() + FleetsAntiAirBonus));
					}
					else {
						KilledAirs += static_cast<int>((InterceptKammusu->AllAntiAir() + FleetsAntiAirBonus) / 10.6);
					}
					if (aac_bonus_type > 0) {
						KilledAirs += static_cast<int>(-1.1376 + 0.2341 * aac_bonus_add1[aac_bonus_type] + 0.0392 * InterceptKammusu->AllAntiAir() + 0.5);
					}
				}
				//対空カットイン成功時の固定ボーナス
				KilledAirs += aac_bonus_add1[aac_bonus_type];
				//艦娘限定ボーナス
				if (InterceptKammusu->is_kammusu_) KilledAirs += 1;
				//撃墜処理
				if (it_k.Airs[j] > KilledAirs) {
					AllKilledAirs += KilledAirs;
					it_k.Airs[j] -= KilledAirs;
				}
				else {
					AllKilledAirs += it_k.Airs[j];
					it_k.Airs[j] = 0;
				}
			}
		}
		if (isShow) cout << "　" << (i == FriendSide ? "自軍" : "敵軍") << "迎撃結果：" << AllKilledAirs << "機\n";
	}
	/* 開幕爆撃 */
	// 全艦同時なので、全滅判定はひと通り終わった後に行う
	// (キャップ前補正は無効なので、AttackAction関数の引数におけるBP_SAMEはダミー)
	if(isShow) cout << "○開幕爆撃\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		// 敵の中での水上艦だけをピックアップする
		vector<rsize_t> all_unsub;
		for (int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if (!Fleets[OtherSide]->Kammusues[j].isSubmarine()) all_unsub.push_back(j);
		}
		// 水上艦だけに開幕爆撃が行われる
		if (all_unsub.size() == 0) continue;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//ダメージ計算
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			for(int k = 0; k < MovedKammusu->Slots; ++k) {
				if((MovedKammusu->Airs[k] != 0) && (MovedKammusu->Weapons[k].isAirWar2())) {
					int Target = all_unsub[RandInt(all_unsub.size())];
					int BaseAttack;
					switch(MovedKammusu->Weapons[k].Type) {
					case Type_PB:
					case Type_PBF:
					case Type_WB:
						//爆撃は等倍ダメージ
						BaseAttack = static_cast<int>(MovedKammusu->Weapons[k].Bomb * sqrt(MovedKammusu->Airs[k]) + 25);
						break;
					case Type_PA:
						//雷撃は150％か80％かがランダムで決まる
						if(RandInt(2) == 0) {
							BaseAttack = static_cast<int>(1.5 * (MovedKammusu->Weapons[k].Torpedo * sqrt(MovedKammusu->Airs[k]) + 25));
						} else {
							BaseAttack = static_cast<int>(0.8 * (MovedKammusu->Weapons[k].Torpedo * sqrt(MovedKammusu->Airs[k]) + 25));
						}
						break;
					default:
						BaseAttack = 0;
						break;
					}
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BP_SAME, AllAttackPlus[i], TURN_AIR, isShow);
					if (i == 0) all_damage[j] += Damage;
					AllDamage[Target] += Damage;
					isTarget[Target] = true;
				}
			}
		}
		//ダメージ処理
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(isShow) cout << "　" << MovedKammusu->Label(OtherSide) << "に" << AllDamage[j] << "ダメージ！\n";
				if(MovedKammusu->HP > AllDamage[j]) {
					MovedKammusu->HP -= AllDamage[j];
					if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
				} else {
					// 自軍にのみ轟沈ストッパーを適用する
					// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
					if(OtherSide == 0){
						MovedKammusu->HP -= static_cast<int>(0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP));
						if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
						if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
					}else{
						MovedKammusu->HP = 0;
						if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
					}
				}
			}
		}
	}
	

	return AirWarResult;
}

/* 開幕雷撃フェイズ */
void FirstTorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], vector<size_t> &all_damage, const bool isShow) {
	if(isShow) cout << "【開幕雷撃フェイズ】\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//ダメージ計算
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j){
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			if((MovedKammusu->isFirstTorpedo()) && (MovedKammusu->ShowDamage() != Lost)){
				int Target = Fleets[OtherSide]->RandomKammsuWithoutSS(true);
				if(Target != -1){
					int BaseAttack = MovedKammusu->AllTorpedo() + 5;
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BattlePosition, AllAttackPlus[i], TURN_TOR_FIRST, isShow);
					if (i == 0) all_damage[j] += Damage;
					AllDamage[Target] += Damage;
					isTarget[Target] = true;
				}
			}
		}
		//ダメージ処理
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(MovedKammusu->ShowDamage() != Lost){
					if(isShow) cout << "　" << MovedKammusu->Label(OtherSide) << "に" << AllDamage[j] << "ダメージ！\n";
					if(MovedKammusu->HP > AllDamage[j]) {
						MovedKammusu->HP -= AllDamage[j];
						if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
					} else {
						// 自軍にのみ轟沈ストッパーを適用する
						// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
						if(OtherSide == 0){
							MovedKammusu->HP -= static_cast<int>(0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP));
							if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
							if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
						}else{
							MovedKammusu->HP = 0;
							if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
						}
					}
				}
			}
		}
	}
}

/* 砲撃戦フェイズ */
void FirePhase(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], vector<size_t> &all_damage, const bool isShow) {
	if(isShow) cout << "【砲撃戦フェイズ】\n";
	/* 攻撃順を策定 */
	vector< vector<int> >ShotList(BattleSize);
	for(int i = 0; i < BattleSize; ++i) {
		//行動可能な艦娘一覧を作成する
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			if(Fleets[i]->Kammusues[j].isMoveInGun()) {
				ShotList[i].push_back(j);
			}
		}
		if (ShotList[i].size() == 0) continue;
		//一覧をシャッフルする
		random_shuffle(ShotList[i].begin(), ShotList[i].end());
		//一覧を射程で安定ソートする
		//(これにより、同射程では順番がランダムになる)
		for(unsigned int j = 0; j < ShotList[i].size() - 1; ++j){
			for(unsigned int k = j + 1; k < ShotList[i].size(); ++k){
				if(Fleets[i]->Kammusues[ShotList[i][j]].MaxRange() < Fleets[i]->Kammusues[ShotList[i][k]].MaxRange()){
					int temp = ShotList[i][j];
					ShotList[i][j] = ShotList[i][k];
					ShotList[i][k] = temp;
				}
			}
		}
	}
	/* 攻撃開始 */
	for(unsigned int i = 0; i < MaxKanmusu; ++i){
		for(int j = 0; j < BattleSize; ++j){
			if(ShotList[j].size() <= i) continue;
			if(!Fleets[j]->Kammusues[ShotList[j][i]].isMoveInGun()) continue;
			// ダメージ計算
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[ShotList[j][i]]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSub()){
				// 対潜攻撃可能艦の場合、潜水艦系が残っている際に対潜攻撃を優先させる
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1){
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack){
				// 対潜攻撃でなければ、水上艦に向かって砲撃を行う
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS(Fleets[j]->Kammusues[ShotList[j][i]].hasBomb());
				if(Target != -1){
					BaseAttack = MovedKammusu->AllAttack();
					isNormalAttack = true;
				}
			}
			// ダメージ処理
			// 対潜攻撃の場合
			if(isAntiSubAttack){
				int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
					BattlePosition, AllAttackPlus[j], TURN_GUN, isShow);
				if (j == 0) all_damage[ShotList[j][i]] += Damage;
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
				} else {
					// 自軍にのみ轟沈ストッパーを適用する
					// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
					if(OtherSide == 0){
						TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//通常攻撃の場合
			if(isNormalAttack){
				// 攻撃の種類を判別
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				if(((i == 0) && (AirWarResult > AM_NORMAL)) || ((i == 1) && (AirWarResult < AM_NORMAL))){
					//偵察機が存在しないとそもそも弾着観測射撃しようがないし、大破も基本的に駄目
					if((MovedKammusu->isSearchAir()) && (MovedKammusu->ShowDamage() < HeavyDamage)) {
						//弾着観測射撃は複合時は発生確率が上昇する特性があるので、
						//とりあえず1つの可能性につき発動確率を30%とし、それに総索敵補正・
						//旗艦補正・確保時補正を加え、発動したら確率を等分するようにした
						vector<int> isAttackType(5, 0);		//vector<bool>は例によって避けた
						MovedKammusu->ShowAttackType(isAttackType);
						int SpecialPer = 0, SpecialKinds = 0;
						for(vector<int>::iterator itAT = isAttackType.begin(); itAT != isAttackType.end(); ++itAT){
							if(*itAT == 1){
								SpecialPer += 30;
								++SpecialKinds;
							}
						}
						if(SpecialPer != 0){
							//総索敵補正・旗艦補正・確保時補正を加える
							SpecialPer += static_cast<int>(Fleets[i]->CalcSearchPower() * 0.06);
							if(ShotList[j][i] == 0) SpecialPer += 10;
							if(AirWarResult == AM_BEST) SpecialPer += 10;
							//判定
							int p = static_cast<int>(Rand(mt) * 100);
							if(p < SpecialPer){
								int APKind = p * SpecialKinds / SpecialPer;
								int APKind_ = 0;
								for(unsigned int i = 0; i < isAttackType.size(); ++i){
									if(isAttackType[i]){
										if(APKind == APKind_){
											switch(i){
											case 0:
												//主主カットイン
												if(isShow) cout << "<<主主カットインが発生！>>\n";
												Multiple = 1.5;
												break;
											case 1:
												//主徹カットイン
												if(isShow) cout << "<<主徹カットインが発生！>>\n";
												Multiple = 1.3;
												break;
											case 2:
												//主電カットイン
												if(isShow) cout << "<<主電カットインが発生！>>\n";
												Multiple = 1.2;
												break;
											case 3:
												//主副カットイン
												if(isShow) cout << "<<主副カットインが発生！>>\n";
												Multiple = 1.1;
												break;
											case 4:
												//連撃
												if(isShow) cout << "<<連撃が発生！>>\n";
												AttackCount = 2;
												Multiple = 1.2;
												break;
											}
											isSpecialAttack = true;
											break;
										} else{
											++APKind_;
										}
									}
								}
							}
						}
					}
				}
				//攻撃処理
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_GUN, isShow, Multiple, isSpecialAttack);
					if (j == 0) all_damage[ShotList[j][i]] += Damage;
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					} else {
						// 自軍にのみ轟沈ストッパーを適用する
						// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
						if(OtherSide == 0){
							TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* 砲撃戦フェイズ(二巡目) */
void FirePhase2(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], vector<size_t> &all_damage, const bool isShow) {
	if(isShow) cout << "【砲撃戦フェイズ(二巡目)】\n";
	/* 最低1隻以上戦艦が存在するかを判定する */
	bool hasBB = false;
	for(int i = 0; i < BattleSize; ++i) {
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			if((itKammusu->Kind == SC_BB) || (itKammusu->Kind == SC_BBV)) {
				hasBB = true;
				break;
			}
		}
		if(hasBB) break;
	}
	if(!hasBB) return;
	/* 攻撃順を策定 */
	vector< vector<int> >ShotList(BattleSize);
	for(int i = 0; i < BattleSize; ++i) {
		//行動可能な艦娘一覧を作成する
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			if(Fleets[i]->Kammusues[j].isMoveInGun()) {
				ShotList[i].push_back(j);
			}
		}
	}
	/* 攻撃開始 */
	for(unsigned int i = 0; i < MaxKanmusu; ++i) {
		for(int j = 0; j < BattleSize; ++j) {
			if(ShotList[j].size() <= i) continue;
			if(!Fleets[j]->Kammusues[ShotList[j][i]].isMoveInGun()) continue;
			// ダメージ計算
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[ShotList[j][i]]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSub()) {
				// 対潜攻撃可能艦の場合、潜水艦系が残っている際に対潜攻撃を優先させる
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack) {
				// 対潜攻撃でなければ、水上艦に向かって砲撃を行う
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS(Fleets[j]->Kammusues[ShotList[j][i]].hasBomb());
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAttack();
					isNormalAttack = true;
				}
			}
			// ダメージ処理
			// 対潜攻撃の場合
			if(isAntiSubAttack){
				int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
					BattlePosition, AllAttackPlus[j], TURN_GUN, isShow);
				if (j == 0) all_damage[ShotList[j][i]] += Damage;
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
				} else {
					// 自軍にのみ轟沈ストッパーを適用する
					// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
					if(OtherSide == 0){
						TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//通常攻撃の場合
			if(isNormalAttack) {
				// 攻撃の種類を判別
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				if(((i == 0) && (AirWarResult > AM_NORMAL)) || ((i == 1) && (AirWarResult < AM_NORMAL))){
					//偵察機が存在しないとそもそも弾着観測射撃しようがないし、大破も基本的に駄目
					if((MovedKammusu->isSearchAir()) && (MovedKammusu->ShowDamage() < HeavyDamage)) {
						//弾着観測射撃は複合時は発生確率が上昇する特性があるので、
						//とりあえず1つの可能性につき発動確率を30%とし、それに総索敵補正・
						//旗艦補正・確保時補正を加え、発動したら確率を等分するようにした
						vector<int> isAttackType(5, 0);		//vector<bool>は例によって避けた
						MovedKammusu->ShowAttackType(isAttackType);
						int SpecialPer = 0, SpecialKinds = 0;
						for(vector<int>::iterator itAT = isAttackType.begin(); itAT != isAttackType.end(); ++itAT){
							if(*itAT == 1){
								SpecialPer += 30;
								++SpecialKinds;
							}
						}
						if(SpecialPer != 0){
							//総索敵補正・旗艦補正・確保時補正を加える
							SpecialPer += static_cast<int>(Fleets[i]->CalcSearchPower() * 0.06);
							if(ShotList[j][i] == 0) SpecialPer += 10;
							if(AirWarResult == AM_BEST) SpecialPer += 10;
							//判定
							int p = static_cast<int>(Rand(mt) * 100);
							if(p < SpecialPer){
								int APKind = p * SpecialKinds / SpecialPer;
								int APKind_ = 0;
								for(unsigned int i = 0; i < isAttackType.size(); ++i){
									if(isAttackType[i]){
										if(APKind == APKind_){
											switch(i){
											case 0:
												//主主カットイン
												if(isShow) cout << "<<主主カットインが発生！>>\n";
												Multiple = 1.5;
												break;
											case 1:
												//主徹カットイン
												if(isShow) cout << "<<主徹カットインが発生！>>\n";
												Multiple = 1.3;
												break;
											case 2:
												//主電カットイン
												if(isShow) cout << "<<主電カットインが発生！>>\n";
												Multiple = 1.2;
												break;
											case 3:
												//主副カットイン
												if(isShow) cout << "<<主副カットインが発生！>>\n";
												Multiple = 1.1;
												break;
											case 4:
												//連撃
												if(isShow) cout << "<<連撃が発生！>>\n";
												AttackCount = 2;
												Multiple = 1.2;
												break;
											}
											isSpecialAttack = true;
											break;
										} else{
											++APKind_;
										}
									}
								}
							}
						}
					}
				}
				//攻撃処理
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_GUN, isShow, Multiple, isSpecialAttack);
					if (j == 0) all_damage[ShotList[j][i]] += Damage;
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					} else {
						// 自軍にのみ轟沈ストッパーを適用する
						// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
						if(OtherSide == 0){
							TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* 雷撃戦フェイズ */
void TorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], vector<size_t> &all_damage, const bool isShow) {
	if(isShow) cout << "【雷撃戦フェイズ】\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//ダメージ計算
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			if((MovedKammusu->isTorpedo()) && (MovedKammusu->ShowDamage() < MiddleDamage)) {
				int Target = Fleets[OtherSide]->RandomKammsuWithoutSS(true);
				if(Target != -1) {
					int BaseAttack = MovedKammusu->AllTorpedo() + 5;
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BattlePosition, AllAttackPlus[i], TURN_TOR, isShow);
					if (i == 0) all_damage[j] += Damage;
					AllDamage[Target] += Damage;
					isTarget[Target] = true;
				}
			}
		}
		//ダメージ処理
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(MovedKammusu->ShowDamage() != Lost) {
					if(isShow) cout << "　" << MovedKammusu->Label(OtherSide) << "に" << AllDamage[j] << "ダメージ！\n";
					if(MovedKammusu->HP > AllDamage[j]) {
						MovedKammusu->HP -= AllDamage[j];
						if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
					} else {
						// 自軍にのみ轟沈ストッパーを適用する
						// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
						if(OtherSide == 0){
							MovedKammusu->HP -= static_cast<int>(0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP));
							if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
							if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
						}else{
							MovedKammusu->HP = 0;
							if(isShow) cout << "　　残り耐久：" << MovedKammusu->ShowHP() << "\n";
						}
					}
				}
			}
		}
	}
}

/* 夜戦フェイズ */
void NightPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], vector<size_t> &all_damage, const bool isNightGame, const bool isShow) {
	if(isShow) cout << "【夜戦フェイズ】\n";
	for(unsigned int i = 0; i < MaxKanmusu; ++i) {
		for(int j = 0; j < BattleSize; ++j) {
			if(Fleets[j]->Members <= static_cast<int>(i)) continue;
			if(!Fleets[j]->Kammusues[i].isMoveInNight()) continue;
			// ダメージ計算
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[i]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSubInNight()) {
				// 対潜攻撃可能艦の場合、潜水艦系が残っている際に対潜攻撃を優先させる
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack) {
				// 対潜攻撃でなければ、水上艦に向かって砲撃を行う
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS(Fleets[j]->Kammusues[j].hasBomb(), true);
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAttackInNight();
					isNormalAttack = true;
				}
			}
			// ダメージ処理
			//対潜攻撃の場合
			if(isAntiSubAttack) {
				int Damage;
				if (isNightGame) {
					// 開幕夜戦だと対潜は効く
					Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_GUN, isShow);
				}
				else {
					Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_NIGHT, isShow);
				}
				if (j == 0) all_damage[i] += Damage;
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
				} else {
					// 自軍にのみ轟沈ストッパーを適用する
					// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
					if(OtherSide == 0){
						TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//通常攻撃の場合
			if(isNormalAttack) {
				// 攻撃の種類を判別
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				AT AttackType = MovedKammusu->ShowAttackTypeInNight(AttackCount, Multiple, isSpecialAttack);
				if((AttackType == CutinAttackT) || (AttackType == CutinAttackGT) || (AttackType == CutinAttackG)){
					// カットイン確率を計算し、失敗した場合は通常攻撃に戻す
					int luck = MovedKammusu->Luck;
					//熟練見張員補正
					if (MovedKammusu->hasWatch()) luck += 5;
					//カットイン種別によっては運キャップを考慮する
					switch (AttackType) {
					case CutinAttackT:
						if (luck > 60) luck = 60;
						break;
					case CutinAttackGT:
						if (luck > 75) luck = 75;
						break;
					case CutinAttackG:
						if (luck > 55) luck = 55;
						break;
					default:
						luck = 0;
					}
					double CutinPer;
					if (AttackType == CutinAttackG) {
						CutinPer = sqrt(50 * luck);
					}else {
						CutinPer = sqrt(70 * luck);
					}
					//配置補正
					if (i == 0) CutinPer += 12.5;
					//損傷補正
					if(MovedKammusu->ShowDamage() == MiddleDamage) CutinPer += 20;
					//探照灯・照明弾補正
					if(Fleets[j]->hasLight())         CutinPer += 5;
					if(Fleets[OtherSide]->hasLight()) CutinPer -= 10;
					//乱数で成功か否かを判定
					if(!CheckPercent(CutinPer)){
						AttackCount = 1;
						Multiple = 1.0;
						isSpecialAttack = false;
						AttackType = NormalAttack;
					}
				}
				if(AttackType == DoubleAttack){
					// 連撃も確実に発動するわけではない(とりあえず99％とした)
					if(!CheckPercent(99)){
						AttackCount = 1;
						Multiple = 1.0;
						isSpecialAttack = false;
						AttackType = NormalAttack;
					}
				}
				// 攻撃処理
				switch(AttackType){
				case CutinAttackT:
					if(isShow) cout << "<<魚雷カットインが発生！>>\n";
					break;
				case CutinAttackG:
					if(isShow) cout << "<<主砲カットインが発生！>>\n";
					break;
				case CutinAttackGT:
					if (isShow) cout << "<<主魚カットインが発生！>>\n";
					break;
				case DoubleAttack:
					if(isShow) cout << "<<連撃が発生！>>\n";
					break;
				default:
					break;
				}
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack, BattlePosition,
						AllAttackPlus[j], TURN_NIGHT, isShow, Multiple, isSpecialAttack);
					if (j == 0) all_damage[i] += Damage;
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "　" << MovedKammusu->Label(j) << "が" << TargetKammusu->Label(OtherSide) << "に" << Damage << "ダメージ！\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
					} else {
						// 自軍にのみ轟沈ストッパーを適用する
						// (残り耐久を上回るダメージが来た場合、ダメージ量＝0.5×耐久+0.3×[0,耐久-1]とする)
						if(OtherSide == 0){
							TargetKammusu->HP -= static_cast<int>(0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP));
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "　　残り耐久：" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* 結果表示 */
//E勝利の条件が曖昧すぎるので、あえて判定では省いた
WIN ShowListEnd(fleets **Fleets, const bool isShowBP, const BP BattlePosition, const bool isShow) {
	if(isShow) cout << "【結果表示】\n";
	if(!isShowBP) {
		if(isShow) cout << "　交戦形態：" << BPString[BattlePosition] << "\n";
	}
	for(int i = 0; i < BattleSize; ++i) {
		if(isShow) cout << "○" << Position[i] << "\n";
		Fleets[i]->ShowList(isShow);
	}
	// 勝利判定を表示
	if(isShow) cout << "○勝利判定：";
	int AlivedKammusues[BattleSize];
	double ResultsGauge[BattleSize];
	for(int i = 0; i < BattleSize; ++i) {
		AlivedKammusues[i] = Fleets[i]->hasAlived();
		int OtherSide = BattleSize - i - 1;
		ResultsGauge[i] = Fleets[OtherSide]->ResultsGauge();
	}
	if(AlivedKammusues[FriendSide] == Fleets[FriendSide]->Members) {
		//自軍の艦が一隻でも撃沈してない場合
		if(AlivedKammusues[EnemySide] == 0) {
			//S勝利確定。以下、完全勝利かを判定
			bool isPerfectWin = true;
			for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
				if(itKammusu->ShowDamage() != NoDamage) {
					isPerfectWin = false;
					break;
				}
			}
			if(isPerfectWin){
				if(isShow) cout << "完全勝利S\n";
				return WIN_SS;
			}
			if(isShow) cout << "勝利S\n";
			return WIN_S;
		} else {
			//A勝利かを判定
			bool isMajorWin = false;
			switch(Fleets[EnemySide]->Members){
			case 2:
				if(AlivedKammusues[EnemySide] <= 1) isMajorWin = true;
				break;
			case 3:
				if(AlivedKammusues[EnemySide] <= 1) isMajorWin = true;
				break;
			case 4:
				if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
				break;
			case 5:
				if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
				break;
			case 6:
				if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
				break;
			}
			if(isMajorWin){
				if(isShow) cout << "勝利A\n";
				return WIN_A;
			}
			//敵旗艦が撃沈していれば無条件でB勝利
			if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost){
				if(isShow) cout << "戦術的勝利B\n";
				return WIN_B;
			}
			//自軍の戦果ゲージが相手の2.5倍以上の場合B勝利
			if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide] * 2.5) {
				if(isShow) cout << "戦術的勝利B\n";
				return WIN_B;
			}
			//自軍の戦果ゲージが相手より低いか0.1%以下なら敗北D、そうでなければ戦術的敗北C
			if((ResultsGauge[FriendSide] < ResultsGauge[EnemySide])
			|| (ResultsGauge[FriendSide] < 0.001)) {
				if(isShow) cout << "敗北D\n";
				return WIN_D;
			}
			if(isShow) cout << "戦術的敗北C\n";
			return WIN_C;
		}
	}//自軍の艦が一隻以上撃沈している場合
	//敵の轟沈数で分岐
	bool isMajorWin = false;
	switch(Fleets[EnemySide]->Members) {
	case 2:
		if(AlivedKammusues[EnemySide] <= 1) isMajorWin = true;
		break;
	case 3:
		if(AlivedKammusues[EnemySide] <= 1) isMajorWin = true;
		break;
	case 4:
		if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
		break;
	case 5:
		if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
		break;
	case 6:
		if(AlivedKammusues[EnemySide] <= 2) isMajorWin = true;
		break;
	}
	if(isMajorWin) {
		//自軍の戦果ゲージが相手の2.5倍以上の場合B勝利
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide] * 2.5) {
			if(isShow) cout << "戦術的勝利B\n";
			return WIN_B;
		}
		if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost){
			//旗艦を撃沈し、かつ自艦隊の撃沈数＜敵艦隊の撃沈数の時戦術的勝利B、
			//さもないと戦術的敗北C
			if((Fleets[FriendSide]->Members - AlivedKammusues[FriendSide])
			 < (Fleets[EnemySide]->Members  - AlivedKammusues[EnemySide])){
				if(isShow) cout << "戦術的勝利B\n";
				return WIN_B;
			 }
			if(isShow) cout << "戦術的敗北C\n";
			return WIN_C;
		}
		//戦果ゲージが相手の1.0倍以上の時戦術的敗北C、さもないと敗北D
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide]) {
			if(isShow) cout << "戦術的敗北C\n";
			return WIN_C;
		}
		if(isShow) cout << "敗北D\n";
		return WIN_D;
	}
	if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost) {
		//旗艦を撃沈し、かつ自艦隊の撃沈数＜敵艦隊の撃沈数の時戦術的勝利B、
		//さもないと戦術的敗北C
		if((Fleets[FriendSide]->Members - AlivedKammusues[FriendSide])
		 < (Fleets[EnemySide]->Members  - AlivedKammusues[EnemySide])) {
			if(isShow) cout << "戦術的勝利B\n";
			return WIN_B;
		}
		if(isShow) cout << "戦術的敗北C\n";
		return WIN_C;
	} else {
		//自軍の戦果ゲージが相手の1.0倍以上の場合戦術的敗北C、さもないと敗北D
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide]) {
			if(isShow) cout << "戦術的敗北C\n";
			return WIN_C;
		}
		if(isShow) cout << "敗北D\n";
		return WIN_D;
	}
}

/* -------------------- */
/* | シミュレート関係 | */
/* | (補助となる関数) | */
/* -------------------- */

/* 5の倍数に切り上げ */
int RoundUp5(int n) {
	if(n % 5 == 0) {
		return n;
	} else {
		return (n / 5 + 1) * 5;
	}
}

/* 索敵値を計算する */
double fleets::CalcSearchPower() {
	double Search = 0.0;
#ifdef USE_NEW_SEARCH_METHOD
	// 2-5式(秋)
	// http://ch.nicovideo.jp/biikame/blomaga/ar663428
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		Search += sqrt(itKammusu->Search) * 1.6841056;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			switch(itWeapon->Type) {
			case Type_PB:	//艦爆
				Search += itWeapon->Search * 1.0376255;
				break;
			case Type_WB:	//水爆
				Search += itWeapon->Search * 1.7787282;
				break;
			case Type_PA:	//艦攻
				Search += itWeapon->Search * 1.3677954;
				break;
			case Type_PS:	//艦偵
				Search += itWeapon->Search * 1.6592780;
				break;
			case Type_WS:	//水偵
				Search += itWeapon->Search * 2.0000000;
				break;
			case Type_SmallS:	//小型電探
				Search += itWeapon->Search * 1.0045358;
				break;
			case Type_LargeS:	//大型電探
				Search += itWeapon->Search * 0.9906638;
				break;
			case Type_SLight:	//探照灯
				Search += itWeapon->Search * 0.9067950;
				break;
			default:
				break;
			}
		}
	}
	Search += RoundUp5(HQLevel) * (-0.6142467);
	return floor(Search * 10.0 + 0.5) / 10.0;	//小数第2位を四捨五入
#else
	// 2-5式(旧)
	double AllKammusuSearch = 0.0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		AllKammusuSearch += itKammusu->Search;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			switch(itWeapon->Type) {
				case Type_WB:	//水爆
					Search += itWeapon->Search * 2;
					break;
				case Type_PS:	//艦偵
					Search += itWeapon->Search * 2;
					break;
				case Type_WS:	//水偵
					Search += itWeapon->Search * 2;
					break;
				case Type_SmallS:	//小型電探
					Search += itWeapon->Search * 1;
					break;
				case Type_LargeS:	//大型電探
					Search += itWeapon->Search * 1;
			}
		}
	}
	Search += sqrt(AllKammusuSearch);
	return floor(Search);	//小数点以下切り捨て
#endif
}

/* 彩雲がいるかどうかを判定する */
bool fleets::hasSaiun() {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if(itWeapon->Type == Type_PSS) return true;
		}
	}
	return false;
}

/* 戦闘終了を判定する */
bool isExit(fleets **Fleets) {
	for(int i = 0; i < BattleSize; ++i) {
		bool isAllLost = true;
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			if(itKammusu->ShowDamage() != Lost) {
				isAllLost = false;
				break;
			}
		}
		if(isAllLost) return true;
	}
	return false;
}

/* (水上機も搭載していない)潜水艦しかいないか判定する */
bool isAllSubmarine1(fleets *This){
	for(vector<kammusu>::iterator itKammusu = This->Kammusues.begin(); itKammusu != This->Kammusues.end(); ++itKammusu) {
		if(!itKammusu->isSubmarine()) return false;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if(itWeapon->isAir()) return false;
		}
	}
	return true;
}

/* 成功率がLimit[％]な事象について、成功か否かを判定する */
bool CheckPercent(const double Limit) {
	if(Rand(mt) * 100 < Limit) {
		return true;
	} else {
		return false;
	}
}

/* 生き残ってる艦娘からランダムに選択する */
int fleets::RandomKammsu(){
	//生存艦をリストアップ
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			AlivedList.push_back(i);
		}
	}
	//適当な場所の艦を返す
	return AlivedList[RandInt(AlivedList.size())];
}

/* 生き残ってる艦娘(潜水艦系)からランダムに選択する */
int fleets::RandomKammsuWithSS(){
	//生存艦をリストアップ
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			if(Kammusues[i].isSubmarine()){
				AlivedList.push_back(i);
			}
		}
	}
	//適当な場所の艦を返す
	if(AlivedList.size() == 0){
		return -1;
	} else{
		return AlivedList[RandInt(AlivedList.size())];
	}
}

/* 生き残ってる艦娘(水上艦)からランダムに選択する */
//has_bombがtrueだと陸上棲姫を避けるようになり、is_nightがtrueだと探照灯の効果を考慮する
int fleets::RandomKammsuWithoutSS(const bool has_bomb, const bool is_night){
	//生存艦をリストアップ
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			if(!Kammusues[i].isSubmarine()){
				if (has_bomb && Kammusues[i].Kind == SC_AF) continue;
				AlivedList.push_back(i);
			}
		}
	}
	//適当な場所の艦を返す
	if(AlivedList.size() == 0){
		return -1;
	}else if (AlivedList.size() == 1) {
		return AlivedList[0];
	}
	else {
		// 夜戦なら探照灯を考慮する
		tuple<int, size_t> index1 = findSearchLargeLight(AlivedList);
		tuple<int, size_t> index2 = findSearchLight(AlivedList);
		if (is_night && (get<0>(index1) != -1 || get<0>(index2) != -1)) {
			const double search_light_per1 = 30;	//大型探照灯点灯時の誘引率(％)
			const double search_light_per2 = 20;	//探照灯点灯時の誘引率(％)
			if (get<0>(index1) != -1) {
				//大型探照灯
				if (CheckPercent(search_light_per1 + Kammusues[get<0>(index1)].Weapons[get<1>(index1)].level_)) {
					return get<0>(index1);
				}
				else {
					vector<int> AlivedList2;
					for (auto &i : AlivedList) {
						if (i != get<0>(index1)) AlivedList2.push_back(i);
					}
					return AlivedList2[RandInt(AlivedList2.size())];
				}
			}
			else {
				//探照灯
				if (CheckPercent(search_light_per2 + Kammusues[get<0>(index2)].Weapons[get<1>(index2)].level_)) {
					return get<0>(index2);
				}
				else {
					vector<int> AlivedList2;
					for (auto &i : AlivedList) {
						if (i != get<0>(index2)) AlivedList2.push_back(i);
					}
					return AlivedList2[RandInt(AlivedList2.size())];
				}
			}
		}
		else {
			return AlivedList[RandInt(AlivedList.size())];
		}
	}
}

/* 探照灯を持っている艦の位置を調べる(外れなら-1) */
tuple<int, size_t> fleets::findSearchLight(const vector<int> &AlivedList) {
	for (auto &i : AlivedList) {
		for (int j = 0; j < Kammusues[i].Slots; ++j){
			if (Kammusues[i].Weapons[j].Type == Type_SLight) return tuple<int, size_t>(i, j);
		}
	}
	return tuple<int, size_t>(-1, 0);
}
tuple<int, size_t> fleets::findSearchLargeLight(const vector<int> &AlivedList) {
	for (auto &i : AlivedList) {
		for (int j = 0; j < Kammusues[i].Slots; ++j) {
			if (Kammusues[i].Weapons[j].Name == "96式150cm探照灯") return tuple<int, size_t>(i, j);
		}
	}
	return tuple<int, size_t>(-1, 0);
}


/* 与えるダメージ量を計算する */
int AttackAction(fleets *Friend, fleets *Enemy, const int Hunter, int &Target, const int BaseAttack,
                 const BP BattlePosition, const double AllAttackPlus, const TURN Turn,
                 const bool isShow, const double Multiple, const bool isSpecialAttack) {
	kammusu *HunterK = &(Friend->Kammusues[Hunter]);
	kammusu *TargetK = &( Enemy->Kammusues[Target]);
	/* 旗艦だった場合、「かばう」挙動が発生する可能性がある */
	//(水上or潜水同士で、かつかばう側が小破すらしてない場合に限る)
	if(Target == 0){
		//同型艦でかばえる相手が存在するかを調べる
		bool FlagshipIsSubmarine = TargetK->isSubmarine();
		vector<int> CanBlockList;
		for(int i = 1; i < Enemy->Members; ++i){
			if((Enemy->Kammusues[i].isSubmarine() == FlagshipIsSubmarine) && (Enemy->Kammusues[i].ShowDamage() < LightDamage)){
				CanBlockList.push_back(i);
			}
		}
		if(CanBlockList.size() != 0){
			//かばう確率は40％で、どの艦に来るかはランダムである
			if (CheckPercent(40)) {
				int BlockKammusu = CanBlockList[RandInt(CanBlockList.size())];
				if (isShow) cout << "<<旗艦への攻撃を" << Enemy->Kammusues[BlockKammusu].Name << "がかばう！>>\n";
				TargetK = &(Enemy->Kammusues[BlockKammusu]);
				Target = BlockKammusu;
			}
		}
	}
	double Damage = BaseAttack;
	/* 命中率を計算し、命中するかどうかを判定する
	 * (とりあえず雷撃戦用のものとそれ以外とに分離した) */
	double HitValue;
	if (Turn == TURN_TOR || Turn == TURN_TOR_FIRST) {
		// 開幕含めた雷撃戦用命中率
		HitValue = 0.9272;
		HitValue += 0.02178 * sqrt(HunterK->Level - 1);
		HitValue += 0.001518 * HunterK->AllTorpedo(false);
		HitValue += 0.000540 * HunterK->Torpedo;
		HitValue += 0.009017 * HunterK->AllHit();
		if (HunterK->Weapons[0].Type == Type_Torpedo) HitValue += 0.02014 * sqrt(HunterK->Weapons[0].level_);
		if (HunterK->Weapons[1].Type == Type_Torpedo) HitValue += 0.02014 * sqrt(HunterK->Weapons[1].level_);
		HitValue += 0.001463 * HunterK->Luck;
		double a;
		switch (Enemy->Formation) {
		case FOR_TRAIL:
			a = 37.40;
			break;
		case FOR_SUBTRAIL:
			a = 37.16;
			break;
		case FOR_CIRCLE:
			a = 32.77;
			break;
		default:
			a = 37.40;
			break;
		}
		double EvadeSum = TargetK->AllEvade();
		HitValue -= (EvadeSum < a ? EvadeSum / (2.0 * a) : EvadeSum / (EvadeSum + a));
		if (HitValue > 0.9691) HitValue = 0.9691 + sqrt(HitValue - 0.9691);
	}
	else {
		// 昼砲撃戦用命中率
		// 回避側
		double EvadeSum = TargetK->AllEvade();	//回避合計を計算
		if (Enemy->Formation == FOR_ECHELON || Enemy->Formation == FOR_ABREAST) EvadeSum *= 1.2;
		if (TargetK->ShowCond() == Happy) EvadeSum *= 1.8;
		double EvadeValue;
		if (EvadeSum <= 40) {
			EvadeValue = 0.03 + EvadeSum / 80;
		}
		else {
			EvadeValue = 0.03 + EvadeSum / (EvadeSum + 40);
		}
		//(速力差による回避補正は有意差無しとなりました)
		// 命中側
		HitValue = 1.0 + sqrt(HunterK->Level - 1) / 50 + HunterK->AllHit() / 100;
		if (HunterK->ShowCond() == RedFatigue) HitValue /= 1.9;
		HitValue += HunterK->Luck * 0.001 * 1.5;		//命中に運が影響する可能性が
		HitValue -= HunterK->NonFitBB();			//フィット砲補正
		if (Friend->Formation == FOR_SUBTRAIL || Friend->Formation == FOR_ECHELON || Friend->Formation == FOR_ABREAST) HitValue += 0.2;
		if (Friend->Formation == FOR_SUBTRAIL && Enemy->Formation == FOR_ABREAST) HitValue -= 0.2;
		if (Friend->Formation == FOR_ABREAST && Enemy->Formation == FOR_ECHELON) HitValue -= 0.2;
		if (Friend->Formation == FOR_ECHELON && Enemy->Formation == FOR_TRAIL) HitValue -= 0.2;
		// 最終的な命中率を計算する
		HitValue -= EvadeValue;
		if (HitValue > 0.97) HitValue = 0.97;
	}

	/* 対潜攻撃だと色々変わるので先に判定しておく */
	bool isAttackToSub = TargetK->isSubmarine();
	if((isAttackToSub) && (Turn != TURN_GUN) && (Turn != TURN_NIGHT)) return 0;

	/* キャップ前補正 */
	if(Turn != TURN_AIR) {
		//先に、対陸上棲姫向けに雷装値を無効化しておく
		if (((Turn == TURN_GUN) || (Turn == TURN_NIGHT)) && TargetK->Kind == SC_AF) {
			if(((HunterK->Kind == SC_ACV) || (HunterK->Kind == SC_CV) || HunterK->Kind == SC_CVL) || (HunterK->Kind == SC_AF) || (HunterK->Kind == SC_FT)) {
				Damage -= 1.5 * HunterK->AllTorpedo(false);
			}
			else {
				Damage -= HunterK->AllTorpedo(false);
			}
		}
		//交戦形態
		if (Turn != TURN_NIGHT) {
			switch (BattlePosition) {
			case BP_SAME:
				Damage *= 1.0;
				break;
			case BP_DIFF:
				Damage *= 0.8;
				break;
			case BP_T_PLUS:
				Damage *= 1.2;
				break;
			case BP_T_MINUS:
				Damage *= 0.6;
				break;
			default:
				break;
			}
		}
		//攻撃側陣形
		if(Turn != TURN_NIGHT) {
			switch(Friend->Formation) {
			case FOR_TRAIL:
				if(isAttackToSub) Damage *= 0.45; else Damage *= 1.0;
				break;
			case FOR_SUBTRAIL:
				if(isAttackToSub) Damage *= 0.60; else Damage *= 0.8;
				break;
			case FOR_CIRCLE:
				if(isAttackToSub) Damage *= 0.90; else Damage *= 0.7;
				break;
			case FOR_ECHELON:
				if(isAttackToSub) Damage *= 0.75; else Damage *= 0.6;
				break;
			case FOR_ABREAST:
				if(isAttackToSub) Damage *= 1.00; else Damage *= 0.6;
				break;
			}
		}
		//損傷状態
		switch(HunterK->ShowDamage()) {
		case MiddleDamage:
			Damage *= 0.7;
			break;
		case HeavyDamage:
			Damage *= 0.4;
			break;
		default:
			break;
		}
		//三式弾特効
		if(((Turn == TURN_GUN) || (Turn == TURN_NIGHT)) && TargetK->Kind == SC_AF){
			bool has_sanshiki = false;
			size_t has_wg = 0;
			for (auto &it : HunterK->Weapons) {
				if (it.Name.find("三式弾") != string::npos) has_sanshiki = true;
				if(it.Name.find("WG") != string::npos) ++has_wg;
			}
			if (has_sanshiki) Damage *= 2.5;
			const double wg_plus[] = {75, 109, 142, 162};
			Damage += wg_plus[has_wg];
		}else {
			//夜戦
			if ((Turn == TURN_NIGHT) && (isSpecialAttack)) {
				Damage *= Multiple;
			}
		}
		//軽巡軽量砲定数
		Damage += HunterK->FitCL();
	}

	/* キャップ */
	if(isAttackToSub) {
		if(Damage > 100) Damage = 100 + sqrt(Damage - 100);
	} else {
		if(Turn == TURN_NIGHT) {
			if(Damage > 300) Damage = 300 + sqrt(Damage - 300);
		} else {
			if(Damage > 150) Damage = 150 + sqrt(Damage - 150);
		}
	}

	/* キャップ後補正 */
	//徹甲弾補正
	if (TargetK->Kind == SC_BB || TargetK->Kind == SC_BBV ||
		TargetK->Kind == SC_CA || TargetK->Kind == SC_CAV ||
		TargetK->Kind == SC_CV || TargetK->Kind == SC_ACV ||
		TargetK->Kind == SC_AF || TargetK->Kind == SC_FT) {
		bool has_gun = false, has_subgun = false, has_ap = false, has_radar = false;
		for (auto &it : HunterK->Weapons) {
			if (it.Type == Type_Gun) {
				if (it.Name.find("徹甲弾") != string::npos) has_ap = true; else has_gun = true;
			}
			if (it.Type == Type_SubGun) has_subgun = true;
			if (it.Type == Type_SmallS || it.Type == Type_LargeS) has_radar = true;
		}
		if (has_gun && has_ap) {
			if (has_subgun) {
				Damage = static_cast<int>(1.15 * Damage);
			}
			else if (has_radar) {
				Damage = static_cast<int>(1.10 * Damage);
			}
			else {
				Damage = static_cast<int>(1.08 * Damage);
			}
		}
	}
	//クリティカル補正
	bool cl_flg = false;
	double cl_per_plus = 0.0;	//熟練艦載機によるCL2率上昇(試験実装)
	for (auto &it : HunterK->Weapons) {
		switch (it.Type) {
		case Type_PA:
		case Type_PB:
		case Type_PBF:
		case Type_WB:
			cl_per_plus += 0.05 * it.level_ / 7;
		default:
			break;
		}
	}
	if(Turn == TURN_AIR) {
		if(Rand(mt) < 0.025 + cl_per_plus) {
			Damage *= 1.5;
			cl_flg = true;
		}
	} else if(Turn == TURN_NIGHT){	//夜戦時におけるクリティカル補正(試験実装)
		if(Rand(mt) < 0.30 + cl_per_plus) {
			Damage *= 1.5;
			cl_flg = true;
		}
	}else if(Rand(mt) < (0.25 * HitValue / (HitValue + 1) + 0.0125) + cl_per_plus) {
		Damage *= 1.5;
		cl_flg = true;
	}
	//熟練艦載機によるダメージ補正
	if (cl_flg) {
		double multi_cv = 1.0;
		for (int i = 0; i < HunterK->Slots; ++i) {
			weapon &weapon_temp = HunterK->Weapons[i];
			switch (weapon_temp.Type) {
			case Type_PA:
			case Type_PB:
			case Type_PBF:
			case Type_WB:
				multi_cv += 0.1 * weapon_temp.level_ / 7;
				if(i == 0) multi_cv += 0.1 * weapon_temp.level_ / 7;
			default:
				break;
			}
		}
		Damage *= multi_cv;
	}
	//触接補正
	Damage *= AllAttackPlus;
	//弾着観測射撃補正
	if((Turn == TURN_GUN) && (isSpecialAttack)) {
		Damage *= Multiple;
	}
	/* 最終的なダメージ量を決定 */
	double defense2 = 0.7 * TargetK->Defense + 0.6 * RandInt(TargetK->Defense);
	Damage = Damage - defense2;
	//残り弾薬量補正
	if(HunterK->Ammo < 50) {
		Damage *= 2.0 * HunterK->Ammo / 100;
	}
	//弾着観測射撃時、カットインが入ると命中率が劇的に上昇する
	if((Turn == TURN_GUN) && (isSpecialAttack)){
		if(HitValue < 0.9) HitValue = 0.9;		//暫定的な仕様
	}
	//弾着観測射撃および夜戦魚雷カットインおよび夜戦連撃なら回避してもカスダメ、それ以外では0ダメージ
	if(HitValue < Rand(mt)) {
		if(isSpecialAttack) {
			Damage = 0.0;
		} else {
			return 0;
		}
	}
	//夜戦での対潜攻撃
	if((Turn == TURN_NIGHT) && (isAttackToSub)) Damage = 0.0;
	//カスダメ時は相手残り耐久の6～14％を与える
	if(Damage < 1.0) {
		Damage = 0.06 * TargetK->HP + 0.08 * RandInt(TargetK->HP);
	}
	return static_cast<int>(Damage);
}

/* 生存艦の数をカウントする */
int fleets::hasAlived() {
	int Alived = 0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		if(itKammusu->ShowDamage() != Lost) {
			++Alived;
		}
	}
	return Alived;
}

/* 戦果ゲージ量を計算する */
double fleets::ResultsGauge() {
	int MaxHPAll = 0, HPAll = 0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		MaxHPAll += itKammusu->MaxHP;
		HPAll += itKammusu->HP;
	}
	return static_cast<double>(MaxHPAll - HPAll) / MaxHPAll;
}

/* 探照灯や照明弾を持っているかを判定する */
bool fleets::hasLight(){
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if((itWeapon->Type == Type_SLight) || (itWeapon->Type == Type_LightB)) return true;
		}
	}
	return false;
}

/* 大破以上の艦が存在するかを判定する */
bool fleets::hasHeavyDamage() {
	for (auto &it : Kammusues) {
		if (it.ShowDamage() >= HeavyDamage) {
			return true;
		}
	}
	return false;
}

/* ------------------ */
/* |  その他の関数  | */
/* ------------------ */

/* 艦娘をセットする */
void fleets::SetKammusu(const kammusu &Kammusu) {
	Kammusues.push_back(Kammusu);
	++Members;
}

/* 状態をリセットする */
void fleets::Reset() {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		itKammusu->HP = itKammusu->MaxHP;		//無傷であると仮定
		itKammusu->cond = 49;					//定数値であることに注意
		itKammusu->Ammo = 100;					//100%が最大だが、実は50%以上は皆同じ扱い
		itKammusu->Airs = itKammusu->MaxAirs;	//艦載機数も初期化する
	}
}

/* 艦隊に関する情報を表示する */
void fleets::ShowList(const bool isShow) {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		if(isShow) cout << "　" << itKammusu->Label() << " " << itKammusu->ShowHP() << "\n";
	}
}

/* ファイルから読み込む */
void fleets::ReadData(const string Filename){
	/* 拡張子を抽出する */
	size_t pos1 = Filename.rfind('.');
	if (pos1 == string::npos) throw "艦隊データが正常に読み込めませんでした.";
	string ext = Filename.substr(pos1 + 1, Filename.size() - pos1);
	if (ext == "txt") {
		std::locale::global(std::locale("japanese"));
		/* weapon.txtから装備データを読み込む */
		vector<weapon> WeaponList = ReadWeaponData();

		/* kammusu.txtから艦娘データを読み込む */
		vector<kammusu> KammusuList = ReadKammusuData();

		/* Filenameから艦隊データを読み込む */
		fstream fin3(Filename);
		if (!fin3.is_open()) throw "艦隊データが正常に読み込めませんでした.";
		Members = 0;
		Kammusues.clear();
		string GetLine;
		int Step = 0;
		while (getline(fin3, GetLine)) {
			// 空行及び#から始まる行(コメント行)は無視する
			if (GetLine == "") continue;
			if (GetLine.substr(0, 1) == "#") continue;
			if (Step == 0) {
				//司令部レベル
				HQLevel = ToInt(GetLine);
				++Step;
			}
			else if (Step == 1) {
				//メンバー(カンマ区切り)
				string temp;
				stringstream sin(GetLine);
				while (getline(sin, temp, ',')) {
					int Number = ToInt(temp) - 1;
					if (Number < 0) throw "メンバー指定が異常です.";
					SetKammusu(KammusuList[Number]);
				}
				++Step;
			}
			else if (Step < 2 + Members) {
				//装備(カンマ区切り)
				int set = Step - 2;
				vector<string> WList = split(GetLine);
				for (size_t i = 0; i < WList.size(); ++i) {
					int Number = ToInt(WList[i]) - 1;
					if (Number < 0) break;
					if (i >= (size_t)Kammusues[set].Slots) break;
					Kammusues[set].Weapons[i] = WeaponList[Number];
				}
				++Step;
			}
			else if (Step < 2 + Members * 2) {
				//状態(カンマ区切り)
				int Set = Step - 2 - Members;
				string temp;
				stringstream sin(GetLine);
				vector<string> InfoList;
				while (getline(sin, temp, ',')) {
					InfoList.push_back(temp);
				}
				for (int i = 0; i < static_cast<int>(InfoList.size()); ++i) {
					int Number = ToInt(InfoList[i]);
					if (Number >= 0) {
						if ((i == 0) && (Number > 0) && (Number <= Kammusues[Set].MaxHP)) {
							//残り耐久
							Kammusues[Set].HP = Number;
						}
						if ((i == 1) && (Number <= 100)) {
							//cond値
							Kammusues[Set].cond = Number;
						}
						if ((i == 2) && (Number <= 100)) {
							//残り弾薬％
							Kammusues[Set].Ammo = Number;
						}
						if ((i >= 3) && (i - 3 < Kammusues[Set].Slots) && (Number <= Kammusues[Set].MaxAirs[i - 3])) {
							//1～4スロ目の残り艦載機数
							Kammusues[Set].Airs[i - 3] = Number;
						}
					}
				}
				++Step;
			}
			else if (Step < 2 + Members * 3) {
				int set = Step - 2 - Members * 2;
				//装備改修度・艦載機熟練度
				vector<string> weapon_level_list = split(GetLine);
				for (int i = 0; i < Kammusues[set].Slots; ++i) {
					Kammusues[set].Weapons[i].level_ = std::stoi(weapon_level_list[i]);
				}
				++Step;
			}
		}
	}
	else if (ext == "json") {
		std::locale::global(std::locale("japanese"));
		/* slotitems.csvから兵装データを読み込む */
		unordered_map<int, weapon> weapon_list;
		ReadWeaponData2(weapon_list);
		/* ships.csvから艦娘データを読み込む */
		/* (リスク回避のため、nullが含まれるものは読まないようにする) */
		unordered_map<size_t, kammusu> kammusu_list_1, kammusu_list_99;
		ReadKammusuData2(kammusu_list_1, kammusu_list_99, weapon_list);
		/* Filenameから艦隊データを読み込む */
		fstream fin3(Filename);
		if (!fin3.is_open()) throw "艦隊データが正常に読み込めませんでした.";
		string json((std::istreambuf_iterator<char>(fin3)),std::istreambuf_iterator<char>());
		picojson::value v;
		string err = picojson::parse(v, json);
		if (err.empty()) {
			// 第1艦隊を読み込む
			auto& o = v.get<object>();
			auto& fleet = o["f1"].get<object>();
			// 諸情報はざっくり決める
			HQLevel = 120;
			Members = fleet.size();
			Kammusues.resize(Members);
			for (int i = 0; i < Members; ++i) {
				auto& unit = fleet["s" + std::to_string(i + 1)].get<object>();
				int id = ToInt(unit["id"].get<string>());
				Kammusues[i] = calc_param(kammusu_list_1.at(id), kammusu_list_99.at(id), stoi(unit["lv"].to_str()));
				int luck = stoi(unit["luck"].to_str());
				if (luck != -1) Kammusues[i].Luck = luck;
				Kammusues[i].clear_weapons();
				auto& weapons = unit["items"].get<object>();
				size_t weapons_size = weapons.size();
				for (size_t j = 0; j < weapons_size; ++j) {
					auto& weapon = weapons["i" + std::to_string(j + 1)].get<object>();
					auto id2 = stoi(weapon["id"].to_str());
					Kammusues[i].Weapons[j] = weapon_list.at(id2);
					Kammusues[i].Weapons[j].level_ = stoi(weapon["rf"].to_str());
				}
			}
		}
		else {
			throw "艦隊データが正常に読み込めませんでした.";
		}
	}
	else {
		throw "艦隊データが正常に読み込めませんでした.";
	}
	return;
}
