#include "header.h"

/* 損害状況を示す */
DAMAGE kammusu::ShowDamage(){
	if(HP == MaxHP) return NoDamage;
	if(HP == 0) return Lost;
	if(HP * 4 > MaxHP * 3) return VeryLightDamage;
	if(HP * 4 > MaxHP * 2) return LightDamage;
	if(HP * 4 > MaxHP * 1) return MiddleDamage;
	return HeavyDamage;
}

/* 潜水艦系かを判定 */
bool kammusu::isSubmarine(){
	if((Kind == SC_SS)
	|| (Kind == SC_SSV)) {
		return true;
	} else {
		return false;
	}
}

/* 総対空値を返す */
int kammusu::AllAntiAir(){
	int AASum = AntiAir;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		AASum += itWeapon->AntiAir;
	}
	return AASum;
}

/* 耐久情報を返す */
string kammusu::ShowHP() {
	stringstream Output;
	Output << HP << "/" << MaxHP << "(" << DMString[ShowDamage()] << ")";
	return Output.str();
}

/* 簡易的な肩書きを返す */
string kammusu::Label(const int pos) {
	stringstream Output;
	if(pos >= 0) {
		Output << "[" + Position[pos] + "]";
	}
	Output << Name << "(Lv" << Level << ")";
	return Output.str();
}

/* 総回避を返す */
int kammusu::AllEvade() {
	int ESum = Evade;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		ESum += itWeapon->Evade;
	}
	return ESum;
}

/* 総命中を返す */
int kammusu::AllHit() {
	double HSum = 0;
	for(auto &it : Weapons){
		HSum += it.Hit;
		//命中補正(試験実装)
		HSum += 1.5 * sqrt(it.level_);
	}
	return static_cast<int>(HSum);
}

/* 疲労状態を返す */
//赤疲労やキラキラ以外での戦闘における影響ははっきりしない
COND kammusu::ShowCond() {
	if(cond < 20){
		return RedFatigue;
	} else if(cond < 30){
		return OrangeFatigue;
	} else if(cond < 50){
		return Normal;
	} else{
		return Happy;
	}
}

/* 開幕雷撃可能かを判定 */
bool kammusu::isFirstTorpedo(){
	//Lv10以上の潜水艦系は無条件でOK
	if((isSubmarine()) && (Level >= 10)) return true;
	//深海棲艦なら無条件でOK
	if((!is_kammusu_) && isSubmarine()) return true;
	//それ以外の潜水艦系・雷巡・水上機母艦は甲標的が必須
	if((Kind == SC_SS)
	|| (Kind == SC_SSV)
	|| (Kind == SC_CLT)
	|| (Kind == SC_AV)){
		for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			if(itWeapon->Type == Type_SpecialSS){
				return true;
			}
		}
	}
	//阿武隈改二用の設定
	if(Name.find("阿武隈改二") != string::npos){
		for (vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			if (itWeapon->Type == Type_SpecialSS) {
				return true;
			}
		}
	}
	//レ級elite用の設定
	if (Name.find("レ級") != string::npos) {
		if (Name.find("エリ") != string::npos) return true;
		if (Name.find("elite") != string::npos) return true;
	}
	// 駆逐水鬼用の設定
	if (Name.find("駆逐水鬼") != string::npos) return true;
	return false;
}

/* 総雷装を返す */
int kammusu::AllTorpedo(const bool is_torpedo_phase) {
	double TSum = Torpedo;
	for (auto &it : Weapons) {
		TSum += it.Torpedo;
		if (is_torpedo_phase) {
			switch (it.Type) {
			case Type_Torpedo:
			case Type_AAG:
				TSum += 1.2 * sqrt(it.level_);
				break;
			default:
				break;
			}
		}
	}
	return static_cast<int>(TSum);
}

/* 昼戦で攻撃可能かを判定 */
bool kammusu::isMoveInGun() {
	//昼戦で攻撃不可能……撃沈済、中破空母、大破装甲空母、艦載機切れ空母、潜水艦系
	if(ShowDamage() == Lost) return false;
	if((ShowDamage() == HeavyDamage)  &&  (Kind == SC_ACV)) return false;
	if((ShowDamage() >= MiddleDamage) && ((Kind == SC_CV) || (Kind == SC_CVL))) return false;
	if((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL)){
		bool isAttackAirs = false;
		for(int i = 0; i < Slots; ++i){
			if((Weapons[i].isAirWar2()) && (Airs[i] != 0)){
				isAttackAirs = true;
				break;
			}
		}
		if(!isAttackAirs) return false;
	}
	if(isSubmarine()) return false;
	return true;
}

/* 最大射程を返す */
RANGE kammusu::MaxRange(){
	RANGE MaxR = Range;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(MaxR < itWeapon->Range) MaxR = itWeapon->Range;
	}
	return MaxR;
}

/* 対潜攻撃可能かを返す */
bool kammusu::isAntiSub(){
	if((Kind == SC_BBV)
	|| (Kind == SC_AV)
	|| (Kind == SC_CAV)){
		bool isAS = false;
		for(int i = 0; i < Slots; ++i){
			if((Weapons[i].isAntiSub1()) && (Airs[i] != 0)){
				isAS = true;
				break;
			}
		}
		if(isAS) return true;
	}
	if((Kind == SC_CVL)
	|| (Kind == SC_AF)){
		bool isAS = false;
		for(int i = 0; i < Slots; ++i){
			if((Weapons[i].isAntiSub2()) && (Airs[i] != 0)){
				isAS = true;
				break;
			}
		}
		if(isAS) return true;
	}
	if((Kind == SC_CL)
	|| (Kind == SC_CLT)
	|| (Kind == SC_DD)
	|| (Kind == SC_CP)) {
		if(AntiSub != 0) return true;
	}
	return false;
}

/* 総対潜能力を返す */
int kammusu::AllAntiSub(){
	int BaseAS = AntiSub;	//素の対潜
	int WeaponAS = 0;		//装備対潜
	double AddAS = 0.0;		//改修強化値
	bool hasDP = false, hasSonar = false;
	for(auto &it : Weapons) {
		WeaponAS += it.AntiSub;
		switch (it.Type) {
		case Type_DP:
			hasDP = true;
			AddAS += sqrt(it.level_);
			break;
		case Type_Sonar:
			hasSonar = true;
			AddAS += sqrt(it.level_);
			break;
		case Type_PS:
		case Type_PSS:
		case Type_WS:
		case Type_WSN:
		case Type_LargeS:
		case Type_SmallS:
			//偵察機・電探の場合は装備対潜ではなく素の対潜に追加される
			WeaponAS -= it.AntiSub;
			BaseAS += it.AntiSub;
		default:
			break;
		}
	}
	//攻撃力を計算
	double AntiSubPower = sqrt(BaseAS) * 2 + 1.5 * WeaponAS + AddAS;
	//艦種別補正
	if(hasDP || hasSonar){
		//爆雷やソナーによる攻撃の場合
		AntiSubPower += 13;
	} else{
		//航空機による攻撃の場合
		AntiSubPower += 8;
	}
	//シナジー補正
	if(hasDP && hasSonar) AntiSubPower = static_cast<int>(AntiSubPower * 1.15);
	return  static_cast<int>(AntiSubPower);
}

/* 総攻撃力(砲撃戦火力)を返す */
int kammusu::AllAttack(){
	double BaseAttack = 0.0;
	if((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL) || (Kind == SC_AF) || (Kind == SC_FT)){
		// 空母系
		// (完全に0じゃない限りあるものとして扱うという謎仕様)
		int AllTorpedo = 0, AllBomb = 0;
		double gamma = 0.0;
		for(auto &it : Weapons) {
			AllTorpedo += it.Torpedo;
			AllBomb += it.Bomb;
			if (it.Type == Type_SubGun) gamma += sqrt(it.level_);
		}
		BaseAttack = static_cast<int>((Attack + AllTorpedo + static_cast<int>(1.3 * AllBomb) + gamma) * 1.5 + 55);
	} else{
		// 非空母系
		BaseAttack += Attack;
		for(auto &it : Weapons) {
			BaseAttack += it.Attack;
			//改修による効果も加算する
			switch (it.Type) {
			case Type_Gun:	// 主砲は大口径主砲か否かによって分類する
				if (it.Range >= LongRange) {
					BaseAttack += 1.5 * sqrt(it.level_);
				}
				else {
					BaseAttack += sqrt(it.level_);
				}
				break;
			case Type_SubGun:	//副砲
			case Type_AAG:		//対空機銃
			case Type_AAD:		//高射装置
			case Type_SLight:	//探照灯
				BaseAttack += sqrt(it.level_);
				break;
			case Type_Sonar:	//ソナー
			case Type_DP:		//爆雷
				BaseAttack += 0.75 * sqrt(it.level_);
				break;
			default:
				break;
			}
		}
		BaseAttack += 5;
	}
	return static_cast<int>(BaseAttack);
}

/* 雷撃可能かを判定　*/
bool kammusu::isTorpedo() {
	// 素の雷装値が0だと雷撃不可能
	if(Torpedo == 0) return false;
	// 駆逐・軽巡・雷巡・重巡・航巡・潜水艦は無条件に可能
	if((Kind == SC_DD)
	|| (Kind == SC_CL)
	|| (Kind == SC_CLT)
	|| (Kind == SC_CA)
	|| (Kind == SC_CAV)
	|| (Kind == SC_SS)
	|| (Kind == SC_SSV)
	|| (Kind == SC_CP)) {
		return true;
	}
	// 水上機母艦は「改」「甲」のみ可能
	// (素の雷装値が0だと雷撃できない)
	if(Kind == SC_AV) {
		if((Name.find("改") != string::npos)
		|| (Name.find("甲") != string::npos)) {
			return true;
		}
		return false;
	}
	// その他、雷撃可能な連中
	if(Name.find("Bismarck drei") != string::npos) return true;
	if(Name.find("レ級") != string::npos) return true;
	if (Name.find("鬼") != string::npos) return true;
	if (Name.find("姫") != string::npos) return true;
	if (Name.find("護衛要塞") != string::npos) return true;
	return false;
}

/* 夜戦可能かを判定 */
bool kammusu::isMoveInNight() {
	if(ShowDamage() >= HeavyDamage) return false;
	if ((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL)) {
		// 敵空母及びグラーフは夜戦攻撃可能、そうでない空母は不可
		if (!is_kammusu_) {
			if ((Name.find("フラ") != string::npos) || (Name.find("flagship") != string::npos)) {
				if (Name.find("ヲ級") != string::npos) return true;
				if (Name.find("ヌ級") != string::npos) return true;
			}
		}
		if (Name.find("Graf") != string::npos) return true;
		if (Name.find("グラーフ") != string::npos) return true;
		return false;
	}
	return true;
}

/* 夜戦時に対潜攻撃可能かを判定 */
bool kammusu::isAntiSubInNight(){
	if((Kind == SC_CL)
	|| (Kind == SC_CLT)
	|| (Kind == SC_DD)
	|| (Kind == SC_CP)) {
		if(AntiSub != 0) return true;
	}
	return false;
}

/* 艦爆を持っているかを判定 */
bool kammusu::hasBomb() {
	for (auto &it : Weapons) {
		if (it.Type == Type_PB) return true;
	}
	return false;
}

/* 夜戦火力を返す */
int kammusu::AllAttackInNight(){
	double BaseAttack = Attack + Torpedo;
	for (auto &it : Weapons) {
		BaseAttack += it.Attack;
		BaseAttack += it.Torpedo;
		switch (it.Type) {
		case Type_Gun:		// 主砲
		case Type_SubGun:	//副砲
		case Type_Torpedo:	//魚雷
		case Type_AAD:		//高射装置
		case Type_SLight:	//探照灯
			BaseAttack += sqrt(it.level_);
			break;
		default:
			break;
		}
	}
	return static_cast<int>(BaseAttack);
}

/* 偵察機を持っているかを判定 */
bool kammusu::isSearchAir(){
	for(int i = 0; i < Slots; ++i) {
		if((Airs[i] > 0) && ((Weapons[i].Type == Type_WS) || (Weapons[i].Type == Type_WB))){
			return true;
		}
	}
	return false;
}

/* 発動可能な弾着観測射撃の種類を返す */
void kammusu::ShowAttackType(vector<int> &isAttackType){
	// 各装備の数を数える
	int GunCount = 0, SubGunCount = 0, APCount = 0, RadarCount = 0;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Type == Type_Gun)     ++GunCount;
		if(itWeapon->Type == Type_SubGun) ++SubGunCount;
		if(itWeapon->Name.find("徹甲弾") != string::npos) ++APCount;
		if((itWeapon->Type == Type_SmallS) || (itWeapon->Type == Type_LargeS)) ++RadarCount;
	}
	// 種類を判定
	//主主カットイン・主徹カットイン・主電カットイン・主副カットイン・連撃を
	//それぞれA～Eと置くと、AとE、BとD、CとD、DとEは複合する可能性がある
	if((GunCount == 2) && (SubGunCount == 0) && (APCount == 1) && (RadarCount == 0)) isAttackType[0] = 1;
	if((GunCount == 1) && (SubGunCount == 1) && (APCount == 1) && (RadarCount == 0)) isAttackType[1] = 1;
	if((GunCount == 1) && (SubGunCount == 1) && (APCount == 0) && (RadarCount == 1)) isAttackType[2] = 1;
	if((GunCount >= 1) && (SubGunCount >= 1)) isAttackType[3] = 1;
	if(GunCount >= 2) isAttackType[4] = 1;
}

/* 夜間特殊攻撃の種類・および倍率を返す */
AT kammusu::ShowAttackTypeInNight(int &AttackCount, double &Multiple, bool &isSpecialAttack){
	// 各装備の数を数える
	int GunCount = 0, SubGunCount = 0, TorpedoCount = 0;
	AT AttackType = NormalAttack;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Type == Type_Gun)     ++GunCount;
		if(itWeapon->Type == Type_SubGun ) ++SubGunCount;
		if(itWeapon->Type == Type_Torpedo) ++TorpedoCount;
	}
	// 種類を判定
	if(TorpedoCount >= 2){
		AttackCount = 2;
		Multiple = 1.5;
		isSpecialAttack = true;
		AttackType = CutinAttackT;
	} else if(GunCount >= 3){
		AttackCount = 1;
		Multiple = 2.0;
		isSpecialAttack = true;
		AttackType = CutinAttackG;
	} else if((GunCount == 2) && (SubGunCount >= 1)){
		AttackCount = 1;
		Multiple = 1.75;
		isSpecialAttack = true;
		AttackType = CutinAttackG;
	} else if(((GunCount == 1) || (GunCount == 2)) && (TorpedoCount == 1)){
		AttackCount = 2;
		Multiple = 1.3;
		isSpecialAttack = true;
		AttackType = CutinAttackGT;
	} else if((GunCount == 2) && (SubGunCount == 0) && (TorpedoCount == 0)){
		AttackCount = 2;
		Multiple = 1.2;
		isSpecialAttack = true;
		AttackType = DoubleAttack;
	} else if((GunCount == 1) && (SubGunCount >= 1) && (TorpedoCount == 0)){
		AttackCount = 2;
		Multiple = 1.2;
		isSpecialAttack = true;
		AttackType = DoubleAttack;
	} else if((GunCount == 0) && (SubGunCount >= 2) && ((TorpedoCount == 0) || (TorpedoCount == 1))){
		AttackCount = 2;
		Multiple = 1.2;
		isSpecialAttack = true;
		AttackType = DoubleAttack;
	}
	return AttackType;
}

/* 熟練見張員を所持しているかを判定 */
bool kammusu::hasWatch() {
	for (auto &it : Weapons) {
		if (it.Name == "熟練見張員") {
			return true;
		}
	}
	return false;
}

/* フィットしない砲による命中率の逆補正 */
double kammusu::NonFitBB(){
	double MinusHit = 0.0;
	// 通常命中率低下は赤疲労検証での減少率÷2ぐらいでちょうどいいのでは？
	const double fit[] = { 0.0, -0.01365, -0.0315, -0.0261, -0.0319};
	const double unfit_small[] = { 0.0, 0.00845, 0.04, 0.0422, 0.04255};
	const double unfit_large[] = {0.0, 0.05375, 0.06365, 0.08415, 0.09585};
	// 数を数えておく
	int Sum_356 = 0, Sum_38 = 0, Sum_381 = 0, Sum_41 = 0, Sum_46 = 0, Sum_46X = 0;
	for (auto &it : Weapons) {
		if (it.Name.find("35.6cm") != string::npos) ++Sum_356;
		if (it.Name.find("38cm"  ) != string::npos) ++Sum_38;
		if (it.Name.find("381mm" ) != string::npos) ++Sum_381;
		if (it.Name.find("41cm"  ) != string::npos) ++Sum_41;
		if (it.Name.find("46cm") != string::npos) {
			if (it.Name.find("試製") != string::npos) ++Sum_46X; else ++Sum_46;
		}
	}
	// 種類により減衰量を決定する
	//伊勢型および扶桑型
	if ((Name.find("伊勢") != string::npos)
		|| (Name.find("日向") != string::npos)
		|| (Name.find("扶桑") != string::npos)
		|| (Name.find("山城") != string::npos)) {
		if (Name.find("改") != string::npos) {
			// 航戦
			MinusHit = fit[Sum_41] + unfit_large[Sum_46] + unfit_large[Sum_46X];
		}
		else {
			// 戦艦
			MinusHit = fit[Sum_356] + fit[Sum_38] + fit[Sum_381] + unfit_large[Sum_46] + unfit_small[Sum_46X];
			if ((Name.find("扶桑") != string::npos)
				|| (Name.find("山城") != string::npos)) {
				MinusHit += fit[Sum_41];
			}
		}
	}
	//金剛型およびビスマルク
	if ((Name.find("金剛") != string::npos)
		|| (Name.find("比叡") != string::npos)
		|| (Name.find("榛名") != string::npos)
		|| (Name.find("霧島") != string::npos)
		|| (Name.find("Bismarck") != string::npos)) {
		MinusHit = fit[Sum_356] + fit[Sum_38] + unfit_small[Sum_41] + unfit_large[Sum_46] + unfit_small[Sum_46X];
		if (Name.find("Bismarck") == string::npos) {
			MinusHit += unfit_small[Sum_381];
		}
	}
	//イタリア艦
	if ((Name.find("Littorio") != string::npos)
		|| (Name.find("Italia") != string::npos)
		|| (Name.find("Roma") != string::npos)) {
		MinusHit = fit[Sum_356] + fit[Sum_381] + unfit_small[Sum_41] + unfit_large[Sum_46] + unfit_large[Sum_46X];
	}
	//長門型
	if ((Name.find("長門") != string::npos)
		|| (Name.find("陸奥") != string::npos)) {
		MinusHit = fit[Sum_356] + fit[Sum_381] + fit[Sum_41] + unfit_small[Sum_46] + unfit_small[Sum_46X];
	}
	//大和型
	if ((Name.find("大和") != string::npos)
		|| (Name.find("武蔵") != string::npos)) {
		MinusHit = fit[Sum_41];
	}
	return MinusHit;
}

/* 軽巡系にフィットする砲による威力の補正 */
double kammusu::FitCL() {
	if (Kind != SC_CL && Kind != SC_CLT && Kind != SC_CP) return 0.0;
	// 数を数えておく
	int light_gun_single = 0, light_gun_double = 0;
	for (auto &it : Weapons) {
		if (it.Name == "14cm単装砲") ++light_gun_single;
		if (it.Name == "15.2cm単装砲") ++light_gun_single;
		if (it.Name == "14cm連装砲") ++light_gun_double;
		if (it.Name == "15.2cm連装砲") ++light_gun_double;
		if (it.Name == "15.2cm連装砲改") ++light_gun_double;
	}
	return sqrt(light_gun_single) + 2.0 * sqrt(light_gun_double);
}

/* cond値を修正する */
void kammusu::changeCond(const int new_cond) {
	cond += new_cond;
	if (cond < 0) cond = 0;
	if (cond > 100) cond = 100;
}
