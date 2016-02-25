#include "header.h"

using std::fstream;

/* プロトタイプ宣言 */
//メイン
void ShowListStart(fleets**, const bool isShow = true);
void SearchPhase(fleets**, bool*, const bool isShow = true);
BP BPSelect(fleets**);
AIR_MAS AirWarPhase(fleets**, const bool*, double*, const bool isShow = true);
void FirstTorpedoPhase(fleets**, const BP, const double*, const bool isShow = true);
void FirePhase(fleets**, const BP, const AIR_MAS, const double*, const bool isShow = true);
void FirePhase2(fleets**, const BP, const AIR_MAS, const double*, const bool isShow = true);
void TorpedoPhase(fleets**, const BP, const double*, const bool isShow = true);
void NightPhase(fleets**, const BP, const double*, const bool, const bool isShow = true);
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
WIN fleets::Simulate(fleets &Enemy, const bool isShow, const kSimulateMode simulate_mode) {
	fleets* Fleets[] = {this, &Enemy};
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
		AIR_MAS AirWarResult = AirWarPhase(Fleets, isSearchSuccess, AllAttackPlus, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 3. 開幕雷撃フェイズ */
		FirstTorpedoPhase(Fleets, BattlePosition, AllAttackPlus, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 4. 交戦形態フェイズ */
		if (isShow) cout << "【交戦形態フェイズ】\n";
		if (isShow) cout << "　交戦形態：" << BPString[BattlePosition] << "\n";
		isShowBP = true;
		/* 5. 砲撃戦フェイズ */
		FirePhase(Fleets, BattlePosition, AirWarResult, AllAttackPlus, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 5.5. 砲撃戦フェイズ(二巡目) */
		FirePhase2(Fleets, BattlePosition, AirWarResult, AllAttackPlus, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 6. 雷撃戦フェイズ */
		TorpedoPhase(Fleets, BattlePosition, AllAttackPlus, isShow);
		if (isExit(Fleets)) goto Exit;
		/* 7. 夜戦フェイズ */
		if (simulate_mode == kModeDN) {
			NightPhase(Fleets, BattlePosition, AllAttackPlus, false, isShow);
			if (isExit(Fleets)) goto Exit;
		}
	}
	else {
		/* 夜戦フェイズ2 */
		if (simulate_mode == kModeN) {
			double AllAttackPlus[BattleSize];
			AllAttackPlus[0] = AllAttackPlus[1] = 1.0;
			NightPhase(Fleets, BattlePosition, AllAttackPlus, true, isShow);
			if (isExit(Fleets)) goto Exit;
		}
	}
Exit:
	/* X. 結果表示 */
	WIN WinReason = ShowListEnd(Fleets, isShowBP, BattlePosition, isShow);

	/* 弾薬補正 */
	for (auto &it : this->Kammusues) {
		it.Ammo -= 20;
		if (it.Ammo < 0) it.Ammo = 0;
	}

	if(isShow) cout << "\n";
	return WinReason;
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
AIR_MAS AirWarPhase(fleets **Fleets, const bool *isSearchSuccess, double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "【航空戦フェイズ】\n";
	/* 制空状態の決定 */
	if(isShow) cout << "○制空権の決定\n";
	// 制空値の計算
	int AntiAirScore[BattleSize];
	bool hasAir[BattleSize];
	for(int i = 0; i < BattleSize; ++i) {
		AntiAirScore[i] = 0;
		hasAir[i] = false;
		if(!isSearchSuccess[i]) continue;	//索敵失敗時は制空値0
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			for(int j = 0; j < itKammusu->Slots; ++j) {
				if((itKammusu->Weapons[j].isAirWar()) && (itKammusu->Airs[j] != 0)) hasAir[i] = true;
				if(itKammusu->Weapons[j].isAirWar()) {
					AntiAirScore[i] += static_cast<int>(itKammusu->Weapons[j].AntiAir * sqrt(itKammusu->Airs[j]));
				}
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
		// 艦攻・夜偵・水偵・艦偵が存在するときのみ発生
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			for(int j = 0; j < itKammusu->Slots; ++j) {
				if(itKammusu->Airs[j] != 0){
					if((itKammusu->Weapons[j].Type == Type_PA)  && (CheckPercent(2))){
						AllAttackPlus[i] = AttackPlus[itKammusu->Weapons[j].Hit];
						break;
					}
					if((itKammusu->Weapons[j].Type == Type_WSN) && (CheckPercent(5))){
						AllAttackPlus[i] = AttackPlus[itKammusu->Weapons[j].Hit];
						break;
					}
					if((itKammusu->Weapons[j].Type == Type_WS)  && (CheckPercent(20))){
						AllAttackPlus[i] = AttackPlus[itKammusu->Weapons[j].Hit];
						break;
					}
					if((itKammusu->Weapons[j].isAirPS())        && (CheckPercent(50))){
						AllAttackPlus[i] = AttackPlus[itKammusu->Weapons[j].Hit];
						break;
					}
				}
			}
			if(AllAttackPlus[i] != 1.0) break;
		}
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
			KilledAirsPer[FriendSide] = Rand(mt) * 0.12 + 0.4;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.1;
			break;
		case AM_BAD:
			KilledAirsPer[FriendSide] = Rand(mt) * 0.1  + 0.2;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.4;
			break;
		case AM_NORMAL:
			KilledAirsPer[FriendSide] = Rand(mt) * 0.08 + 0.1;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.6;
			break;
		case AM_GOOD:
			KilledAirsPer[FriendSide] = Rand(mt) * 0.06 + 0.05;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.8;
			break;
		case AM_BEST:
			KilledAirsPer[FriendSide] = Rand(mt) * 0.04 + 0.025;
			KilledAirsPer[EnemySide]  = Rand(mt) * 0.9;
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
	// こちらも自軍と敵軍で計算方法が違うパターン
	// 自軍の方が死ぬほど複雑なので半分ヤケで書いてますｗ
	// (対空カットインは今回考慮しません)
	if(isShow) cout << "○対空火砲による敵攻撃機・爆撃機の迎撃・撃墜\n";
	// [自軍が敵軍の艦載機を迎撃する場合]
	//艦隊対空ボーナス値を決定
	int FleetsAntiAirBonus = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
		double AntiAirBouns = 0.0;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			//名前を直接指定だなんて反吐が出るけど仕方ないね……
			if(itWeapon->Name == "46cm三連装砲"){
				AntiAirBouns += itWeapon->AntiAir * 0.25;
			} else if(itWeapon->Name.find("高角砲") != string::npos){
				AntiAirBouns += itWeapon->AntiAir * 0.35;
			} else if(itWeapon->Name.find("対空電探") != string::npos){
				AntiAirBouns += itWeapon->AntiAir * 0.40;
			} else if(itWeapon->Name == "三式弾"){
				AntiAirBouns += itWeapon->AntiAir * 0.60;
			} else{
				AntiAirBouns += itWeapon->AntiAir * 0.20;
			}
		}
		FleetsAntiAirBonus += static_cast<int>(AntiAirBouns);
	}
	FleetsAntiAirBonus = static_cast<int>(FleetsAntiAirBonus * AntiAirBonusPer[Fleets[FriendSide]->Formation]);
	if(isShow) cout << "　自軍対空ボーナス値：" << FleetsAntiAirBonus << "\n";
	//迎撃！
	int AllKilledAirs = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[EnemySide]->Kammusues.begin(); itKammusu != Fleets[EnemySide]->Kammusues.end(); ++itKammusu) {
		for(int j = 0; j < itKammusu->Slots; ++j) {
			if(itKammusu->Weapons[j].isAirWar2()) {
				kammusu *InterceptKammusu = &(Fleets[FriendSide]->Kammusues[Fleets[FriendSide]->RandomKammsu()]);
				int KilledAirs = static_cast<int>((InterceptKammusu->AllAntiAir() + FleetsAntiAirBonus) * 0.2125);
				if (InterceptKammusu->is_kammusu_) KilledAirs += 1;
				if(itKammusu->Airs[j] > KilledAirs){
					AllKilledAirs += KilledAirs;
					itKammusu->Airs[j] -= KilledAirs;
				} else{
					AllKilledAirs += itKammusu->Airs[j];
					itKammusu->Airs[j] = 0;
				}
			}
		}
	}
	if(isShow) cout << "　自軍迎撃結果：" << AllKilledAirs << "機\n";
	// [敵軍が自軍の艦載機を迎撃する場合]
	AllKilledAirs = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
		for(int j = 0; j < itKammusu->Slots; ++j) {
			if(itKammusu->Weapons[j].isAirWar2()) {
				kammusu *InterceptKammusu = &(Fleets[EnemySide]->Kammusues[Fleets[EnemySide]->RandomKammsu()]);
				int KilledAirs = InterceptKammusu->AllAntiAir() / 25;
				if (InterceptKammusu->is_kammusu_) KilledAirs += 1;
				if(itKammusu->Airs[j] > KilledAirs){
					AllKilledAirs += KilledAirs;
					itKammusu->Airs[j] -= KilledAirs;
				} else{
					AllKilledAirs += itKammusu->Airs[j];
					itKammusu->Airs[j] = 0;
				}
			}
		}
	}
	if(isShow) cout << "　敵軍迎撃結果：" << AllKilledAirs << "機\n";
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
								BaseAttack = static_cast<int>(1.5 * MovedKammusu->Weapons[k].Bomb * sqrt(MovedKammusu->Airs[k]) + 25);
							} else {
								BaseAttack = static_cast<int>(0.8 * MovedKammusu->Weapons[k].Bomb * sqrt(MovedKammusu->Airs[k]) + 25);

							}
							break;
					}
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BP_SAME, AllAttackPlus[i], TURN_AIR, isShow);
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
void FirstTorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isShow) {
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
void FirePhase(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], const bool isShow) {
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
void FirePhase2(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], const bool isShow) {
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
void TorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isShow) {
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
void NightPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isNightGame, const bool isShow) {
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
					}
					double CutinPer = sqrt(70 * luck);
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
					case DoubleAttack:
						if(isShow) cout << "<<連撃が発生！>>\n";
						break;
				}
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack, BattlePosition,
						AllAttackPlus[j], TURN_NIGHT, isShow, Multiple, isSpecialAttack);
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
		int index = findSearchLight(AlivedList);
		if (is_night && index != -1) {
			const double search_light_per = 20;	//探照灯点灯時の誘引率(％)
			if (CheckPercent(search_light_per)) {
				return index;
			}
			else {
				vector<int> AlivedList2;
				for (auto &i : AlivedList) {
					if (i != index) AlivedList2.push_back(i);
				}
				return AlivedList2[RandInt(AlivedList2.size())];
			}
		}
		else {
			return AlivedList[RandInt(AlivedList.size())];
		}
	}
}

/* 探照灯を持っている艦の位置を調べる(外れなら-1) */
int fleets::findSearchLight(const vector<int> &AlivedList) {
	for (auto &i : AlivedList) {
		for (auto &it : Kammusues[i].Weapons) {
			if (it.Type == Type_SLight) return i;
		}
	}
	return -1;
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
	/* 命中率を計算し、命中するかどうかを判定する */
	// 回避側
	double EvadeSum = TargetK->AllEvade();	//回避合計を計算
	if(Enemy->Formation == FOR_ECHELON || Enemy->Formation == FOR_ABREAST) EvadeSum *= 1.2;
	if(TargetK->ShowCond() == Happy) EvadeSum *= 1.8;
	double EvadeValue;
	if(EvadeSum <= 40){
		EvadeValue = 0.03 + EvadeSum / 80;
	} else {
		EvadeValue = 0.03 + EvadeSum / (EvadeSum + 40);
	}
	//(速力差による回避補正は有意差無しとなりました)
	// 命中側
	double HitValue = 1.0 + sqrt(HunterK->Level - 1) / 50 + HunterK->AllHit() / 100;
	if(HunterK->ShowCond() == RedFatigue) HitValue /= 1.9;
	HitValue += HunterK->Luck * 0.001 * 1.5;		//命中に運が影響する可能性が
	HitValue -= HunterK->NonFitBB();			//フィット砲補正
	if(Friend->Formation == FOR_SUBTRAIL || Friend->Formation == FOR_ECHELON || Friend->Formation == FOR_ABREAST) HitValue += 0.2;
	if (Friend->Formation == FOR_SUBTRAIL && Enemy->Formation == FOR_ABREAST) HitValue -= 0.2;
	if (Friend->Formation == FOR_ABREAST && Enemy->Formation == FOR_ECHELON) HitValue -= 0.2;
	if (Friend->Formation == FOR_ECHELON && Enemy->Formation == FOR_TRAIL) HitValue -= 0.2;
	// 最終的な命中率を計算する
	HitValue -= EvadeValue;
	if (HitValue > 0.97) HitValue = 0.97;

	/* 対潜攻撃だと色々変わるので先に判定しておく */
	bool isAttackToSub = TargetK->isSubmarine();
	if((isAttackToSub) && (Turn != TURN_GUN) && (Turn != TURN_NIGHT)) return 0;

	/* キャップ前補正 */
	if(Turn != TURN_AIR) {
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
		}
		//三式弾特効
		if(((Turn == TURN_GUN) || (Turn == TURN_NIGHT)) && TargetK->Kind == SC_AF){
			bool has_sanshiki = false, has_wg = false;
			for (auto &it : HunterK->Weapons) {
				if (it.Name.find("三式弾") != string::npos) has_sanshiki = true;
				if (it.Name.find("WG") != string::npos) has_wg = true;
			}
			if (has_sanshiki) Damage *= 2.5;
			if (has_wg) Damage += 75.0;
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
	if(Turn == TURN_AIR) {
		if(Rand(mt) < 0.025) {
			Damage *= 1.5;
		}
	} else if(Turn == TURN_NIGHT){	//夜戦時におけるクリティカル補正(試験実装)
		if(Rand(mt) < 0.30) {
			Damage *= 1.5;
		}
	}else if(Rand(mt) < 0.24 * HitValue / (HitValue + 0.75)) {
		Damage *= 1.5;
	}
	//触接補正
	Damage *= AllAttackPlus;
	//弾着観測射撃補正
	if((Turn == TURN_GUN) && (isSpecialAttack)) {
		Damage *= Multiple;
	}
	/* 最終的なダメージ量を決定 */
	int defense2 = static_cast<int>(0.7 * TargetK->Defense + 0.6 * RandInt(TargetK->Defense));
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
	while (getline(fin3, GetLine)){
		// 空行及び#から始まる行(コメント行)は無視する
		if (GetLine == "") continue;
		if (GetLine.substr(0, 1) == "#") continue;
		if (Step == 0){
			//司令部レベル
			HQLevel = ToInt(GetLine);
			++Step;
		}else if (Step == 1){
			//メンバー(カンマ区切り)
			string temp;
			stringstream sin(GetLine);
			while (getline(sin, temp, ',')){
				int Number = ToInt(temp) - 1;
				if (Number < 0) throw "メンバー指定が異常です.";
				SetKammusu(KammusuList[Number]);
			}
			++Step;
		}else if (Step < 2 + Members){
			//装備(カンマ区切り)
			string temp;
			stringstream sin(GetLine);
			vector<string> WList;
			while (getline(sin, temp, ',')){
				WList.push_back(temp);
			}
			for (unsigned int i = 0; i < WList.size(); ++i){
				int Number = ToInt(WList[i]) - 1;
				if (Number < 0) break;
				if (static_cast<int>(i) >= Kammusues[Step - 2].Slots) break;
				Kammusues[Step - 2].Weapons[i] = WeaponList[Number];
			}
			++Step;
		} else if(Step < 2 + Members * 2) {
			//状態(カンマ区切り)
			int Set = Step - 2 - Members;
			string temp;
			stringstream sin(GetLine);
			vector<string> InfoList;
			while(getline(sin, temp, ',')) {
				InfoList.push_back(temp);
			}
			for(int i = 0; i < static_cast<int>(InfoList.size()); ++i) {
				int Number = ToInt(InfoList[i]);
				if(Number >= 0) {
					if((i == 0) && (Number > 0) && (Number <= Kammusues[Set].MaxHP)) {
						//残り耐久
						Kammusues[Set].HP = Number;
					}
					if((i == 1) && (Number <= 100)) {
						//cond値
						Kammusues[Set].cond = Number;
					}
					if((i == 2) && (Number <= 100)) {
						//残り弾薬％
						Kammusues[Set].Ammo = Number;
					}
					if((i >= 3) && (i - 3 < Kammusues[Set].Slots) && (Number <= Kammusues[Set].MaxAirs[i - 3])) {
						//1～4スロ目の残り艦載機数
						Kammusues[Set].Airs[i - 3] = Number;
					}
				}
			}
			++Step;
		}
	}
	return;
}
