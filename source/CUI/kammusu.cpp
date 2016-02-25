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
	int HSum = 0;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		HSum += itWeapon->Hit;
	}
	return HSum;
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
int kammusu::AllTorpedo() {
	int TSum = Torpedo;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		TSum += itWeapon->Torpedo;
	}
	return TSum;
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
	|| (Kind == SC_DD)) {
		if(AntiSub != 0) return true;
	}
	return false;
}

/* 総対潜能力を返す */
int kammusu::AllAntiSub(){
	int BaseAS = AntiSub;	//素の対潜
	int WeaponAS = 0;		//装備対潜
	bool hasDP = false, hasSonar = false;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		WeaponAS += itWeapon->AntiSub;
		if(itWeapon->Type == Type_DP) hasDP = true;
		if(itWeapon->Type == Type_Sonar) hasSonar = true;
		if((itWeapon->Type == Type_PS)
		|| (itWeapon->Type == Type_PSS)
		|| (itWeapon->Type == Type_WS)
		|| (itWeapon->Type == Type_WSN)){
			//偵察機の場合は装備対潜ではなく素の対潜に追加される？
			WeaponAS -= itWeapon->AntiSub;
			BaseAS += itWeapon->AntiSub;
		}
	}
	//攻撃力を計算
	int AntiSubPower = BaseAS / 5 + WeaponAS * 2;
	//攻撃別補正
	if(hasDP || hasSonar){
		//爆雷やソナーによる攻撃の場合
		AntiSubPower += 25;
	} else{
		//航空機による攻撃の場合
		AntiSubPower += 10;
	}
	//シナジー補正
	if(hasDP && hasSonar) AntiSubPower = static_cast<int>(AntiSubPower * 1.15);
	return AntiSubPower;
}

/* 総攻撃力を返す */
int kammusu::AllAttack(){
	int BaseAttack = 0;
	if((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL) || (Kind == SC_AF) || (Kind == SC_FT)){
		// 空母系
		// (完全に0じゃない限りあるものとして扱うという謎仕様)
		int AllTorpedo = 0, AllBomb = 0;
		for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			AllTorpedo += itWeapon->Torpedo;
			AllBomb += itWeapon->Bomb;
		}
		BaseAttack = static_cast<int>((Attack + AllTorpedo) * 1.5 + AllBomb * 2 + 55);

	} else{
		// 非空母系
		BaseAttack += Attack;
		for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			BaseAttack += itWeapon->Attack;
		}
		BaseAttack += 5;
	}
	return BaseAttack;
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
	|| (Kind == SC_SSV)) {
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
	|| (Kind == SC_DD)) {
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
	int BaseAttack = Attack + Torpedo;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		BaseAttack += itWeapon->Attack;
		BaseAttack += itWeapon->Torpedo;
	}
	return BaseAttack;
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
	AT AttackType = NormalAttack;
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
		//これを魚雷カットインに含めていいのかはよく分からない……
		AttackCount = 2;
		Multiple = 1.3;
		isSpecialAttack = true;
		AttackType = CutinAttackT;
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

/* フィットしない砲による命中率の逆補正 */
//一般には、金剛・伊勢・扶桑型→35.6、Bismarck→38、長門型→41、大和型→46cmが
//フィット砲とされており、それより大きな砲を積むと命中率が低下するそうな
//ただ、具体的な低下割合が不明だったので、フィットしない41cm→1つにつき3%、
//フィットしない46cm→1つにつき3%(長門型)、6%(その他)という大味な補正を掛けている
//(扶桑型改二については、情報を受け41cmまで大丈夫なようにした)
double kammusu::NonFit(){
	double MinusHit = 0.0;
	// 数を数えておく
	int Sum_41 = 0, Sum_46 = 0;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Name.find("41cm") != string::npos) ++Sum_41;
		if(itWeapon->Name.find("46cm") != string::npos) ++Sum_46;
	}
	// 種類により減衰量を決定する
	if((Name.find("金剛") != string::npos)
	|| (Name.find("比叡") != string::npos)
	|| (Name.find("榛名") != string::npos)
	|| (Name.find("霧島") != string::npos)
	|| (Name.find("伊勢") != string::npos)
	|| (Name.find("日向") != string::npos)){
		MinusHit = Sum_41 * 0.03 + Sum_46 * 0.06;
	}
	if((Name.find("扶桑") != string::npos)
	|| (Name.find("山城") != string::npos)){
		if(Name.find("改二") != string::npos){
			MinusHit = Sum_46 * 0.03;
		} else{
			MinusHit = Sum_41 * 0.03 + Sum_46 * 0.06;
		}
	}
	if((Name.find("長門") != string::npos)
	|| (Name.find("陸奥") != string::npos)){
		MinusHit = Sum_46 * 0.03;
	}
	return MinusHit;
}
