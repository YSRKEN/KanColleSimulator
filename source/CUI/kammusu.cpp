#include "header.h"

/* ���Q�󋵂����� */
DAMAGE kammusu::ShowDamage(){
	if(HP == MaxHP) return NoDamage;
	if(HP == 0) return Lost;
	if(HP * 4 > MaxHP * 3) return VeryLightDamage;
	if(HP * 4 > MaxHP * 2) return LightDamage;
	if(HP * 4 > MaxHP * 1) return MiddleDamage;
	return HeavyDamage;
}

/* �����͌n���𔻒� */
bool kammusu::isSubmarine(){
	if((Kind == SC_SS)
	|| (Kind == SC_SSV)) {
		return true;
	} else {
		return false;
	}
}

/* ���΋�l��Ԃ� */
int kammusu::AllAntiAir(){
	int AASum = AntiAir;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		AASum += itWeapon->AntiAir;
	}
	return AASum;
}

/* �ϋv����Ԃ� */
string kammusu::ShowHP() {
	stringstream Output;
	Output << HP << "/" << MaxHP << "(" << DMString[ShowDamage()] << ")";
	return Output.str();
}

/* �ȈՓI�Ȍ�������Ԃ� */
string kammusu::Label(const int pos) {
	stringstream Output;
	if(pos >= 0) {
		Output << "[" + Position[pos] + "]";
	}
	Output << Name << "(Lv" << Level << ")";
	return Output.str();
}

/* �������Ԃ� */
int kammusu::AllEvade() {
	int ESum = Evade;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		ESum += itWeapon->Evade;
	}
	return ESum;
}

/* ��������Ԃ� */
int kammusu::AllHit() {
	int HSum = 0;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		HSum += itWeapon->Hit;
	}
	return HSum;
}

/* ��J��Ԃ�Ԃ� */
//�Ԕ�J��L���L���ȊO�ł̐퓬�ɂ�����e���͂͂����肵�Ȃ�
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

/* �J�������\���𔻒� */
bool kammusu::isFirstTorpedo(){
	//Lv10�ȏ�̐����͌n�͖�������OK
	if((isSubmarine()) && (Level >= 10)) return true;
	//����ȊO�̐����͌n�E�����E����@��͍͂b�W�I���K�{
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
	//�����G����p�̐ݒ�
	if(Name.find("�����G����") != string::npos){
		return true;
	}
	return false;
}

/* ��������Ԃ� */
int kammusu::AllTorpedo() {
	int TSum = Torpedo;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		TSum += itWeapon->Torpedo;
	}
	return TSum;
}

/* ����ōU���\���𔻒� */
bool kammusu::isMoveInGun() {
	//����ōU���s�\�c�c�����ρA���j���A��j���b���A�͍ڋ@�؂���A�����͌n
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

/* �ő�˒���Ԃ� */
RANGE kammusu::MaxRange(){
	RANGE MaxR = Range;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(MaxR < itWeapon->Range) MaxR = itWeapon->Range;
	}
	return MaxR;
}

/* �ΐ��U���\����Ԃ� */
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
	if(Kind == SC_CVL){
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

/* ���ΐ��\�͂�Ԃ� */
int kammusu::AllAntiSub(){
	int BaseAS = AntiSub;	//�f�̑ΐ�
	int WeaponAS = 0;		//�����ΐ�
	bool hasDP = false, hasSonar = false;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		WeaponAS += itWeapon->AntiSub;
		if(itWeapon->Type == Type_DP) hasDP = true;
		if(itWeapon->Type == Type_Sonar) hasSonar = true;
		if((itWeapon->Type == Type_PS)
		|| (itWeapon->Type == Type_PSS)
		|| (itWeapon->Type == Type_WS)
		|| (itWeapon->Type == Type_WSN)){
			//��@�@�̏ꍇ�͑����ΐ��ł͂Ȃ��f�̑ΐ��ɒǉ������H
			WeaponAS -= itWeapon->AntiSub;
			BaseAS += itWeapon->AntiSub;
		}
	}
	//�U���͂��v�Z
	int AntiSubPower = BaseAS / 5 + WeaponAS * 2;
	//�U���ʕ␳
	if(hasDP || hasSonar){
		//������\�i�[�ɂ��U���̏ꍇ
		AntiSubPower += 25;
	} else{
		//�q��@�ɂ��U���̏ꍇ
		AntiSubPower += 10;
	}
	//�V�i�W�[�␳
	if(hasDP && hasSonar) AntiSubPower = static_cast<int>(AntiSubPower * 1.15);
	return AntiSubPower;
}

/* ���U���͂�Ԃ� */
int kammusu::AllAttack(){
	int BaseAttack = 0;
	if((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL)){
		// ���n
		// (���S��0����Ȃ����肠����̂Ƃ��Ĉ����Ƃ�����d�l)
		int AllTorpedo = 0, AllBomb = 0;
		for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			AllTorpedo += itWeapon->Torpedo;
			AllBomb += itWeapon->Bomb;
		}
		BaseAttack = static_cast<int>((Attack + AllTorpedo) * 1.5 + AllBomb * 2 + 55);

	} else{
		// ����n
		BaseAttack += Attack;
		for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
			BaseAttack += itWeapon->Attack;
		}
		BaseAttack += 5;
	}
	return BaseAttack;
}

/* �����\���𔻒�@*/
bool kammusu::isTorpedo() {
	// �f�̗����l��0���Ɨ����s�\
	if(Torpedo == 0) return false;
	// �쒀�E�y���E�����E�d���E�q���E�����͖͂������ɉ\
	if((Kind == SC_DD)
	|| (Kind == SC_CL)
	|| (Kind == SC_CLT)
	|| (Kind == SC_CA)
	|| (Kind == SC_CAV)
	|| (Kind == SC_SS)
	|| (Kind == SC_SSV)) {
		return true;
	}
	// ����@��͂́u���v�u�b�v�̂݉\
	// (�f�̗����l��0���Ɨ����ł��Ȃ�)
	if(Kind == SC_AV) {
		if((Name.find("��") != string::npos)
		|| (Name.find("�b") != string::npos)) {
			return true;
		}
		return false;
	}
	// Bismarck drei�͗����l�����̂ŁA��͂Ȃ̂ɗ����\
	if(Name == "Bismarck drei") return true;
	return false;
}

/* ���\���𔻒� */
bool kammusu::isMoveInNight() {
	if(ShowDamage() >= HeavyDamage) return false;
	if((Kind == SC_ACV) || (Kind == SC_CV) || (Kind == SC_CVL)) return false;
	return true;
}

/* ��펞�ɑΐ��U���\���𔻒� */
bool kammusu::isAntiSubInNight(){
	if((Kind == SC_CL)
	|| (Kind == SC_CLT)
	|| (Kind == SC_DD)) {
		if(AntiSub != 0) return true;
	}
	return false;
}

/* ���Η͂�Ԃ� */
int kammusu::AllAttackInNight(){
	int BaseAttack = Attack + Torpedo;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		BaseAttack += itWeapon->Attack;
		BaseAttack += itWeapon->Torpedo;
	}
	return BaseAttack;
}

/* ��@�@�������Ă��邩�𔻒� */
bool kammusu::isSearchAir(){
	for(int i = 0; i < Slots; ++i) {
		if((Airs[i] > 0) && ((Weapons[i].Type == Type_WS) || (Weapons[i].Type == Type_WB))){
			return true;
		}
	}
	return false;
}

/* �����\�Ȓe���ϑ��ˌ��̎�ނ�Ԃ� */
void kammusu::ShowAttackType(vector<int> &isAttackType){
	// �e�����̐��𐔂���
	int GunCount = 0, SubGunCount = 0, APCount = 0, RadarCount = 0;
	AT AttackType = NormalAttack;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Type == Type_Gun)     ++GunCount;
		if(itWeapon->Type == Type_SubGun) ++SubGunCount;
		if(itWeapon->Name.find("�O�b�e") != string::npos) ++APCount;
		if((itWeapon->Type == Type_SmallS) || (itWeapon->Type == Type_LargeS)) ++RadarCount;
	}
	// ��ނ𔻒�
	//���J�b�g�C���E��O�J�b�g�C���E��d�J�b�g�C���E�啛�J�b�g�C���E�A����
	//���ꂼ��A�`E�ƒu���ƁAA��E�AB��D�AC��D�AD��E�͕�������\��������
	if((GunCount == 2) && (SubGunCount == 0) && (APCount == 1) && (RadarCount == 0)) isAttackType[0] = 1;
	if((GunCount == 1) && (SubGunCount == 1) && (APCount == 1) && (RadarCount == 0)) isAttackType[1] = 1;
	if((GunCount == 1) && (SubGunCount == 1) && (APCount == 0) && (RadarCount == 1)) isAttackType[2] = 1;
	if((GunCount >= 1) && (SubGunCount >= 1)) isAttackType[3] = 1;
	if(GunCount >= 2) isAttackType[4] = 1;
}

/* ��ԓ���U���̎�ށE����є{����Ԃ� */
AT kammusu::ShowAttackTypeInNight(int &AttackCount, double &Multiple, bool &isSpecialAttack){
	// �e�����̐��𐔂���
	int GunCount = 0, SubGunCount = 0, TorpedoCount = 0;
	AT AttackType = NormalAttack;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Type == Type_Gun)     ++GunCount;
		if(itWeapon->Type == Type_SubGun ) ++SubGunCount;
		if(itWeapon->Type == Type_Torpedo) ++TorpedoCount;
	}
	// ��ނ𔻒�
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
		//����������J�b�g�C���Ɋ܂߂Ă����̂��͂悭������Ȃ��c�c
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

/* �t�B�b�g���Ȃ��C�ɂ�閽�����̋t�␳ */
//��ʂɂ́A�����E�ɐ��E�}�K�^��35.6�ABismarck��38�A����^��41�A��a�^��46cm��
//�t�B�b�g�C�Ƃ���Ă���A������傫�ȖC��ςނƖ��������ቺ���邻����
//�����A��̓I�Ȓቺ�������s���������̂ŁA�t�B�b�g���Ȃ�41cm��1�ɂ�3%�A
//�t�B�b�g���Ȃ�46cm��1�ɂ�3%(����^)�A6%(���̑�)�Ƃ����喡�ȕ␳���|���Ă���
//(�}�K�^����ɂ��ẮA������41cm�܂ő��v�Ȃ悤�ɂ���)
double kammusu::NonFit(){
	double MinusHit = 0.0;
	// ���𐔂��Ă���
	int Sum_41 = 0, Sum_46 = 0;
	for(vector<weapon>::iterator itWeapon = Weapons.begin(); itWeapon != Weapons.end(); ++itWeapon) {
		if(itWeapon->Name.find("41cm") != string::npos) ++Sum_41;
		if(itWeapon->Name.find("46cm") != string::npos) ++Sum_46;
	}
	// ��ނɂ�茸���ʂ����肷��
	if((Name.find("����") != string::npos)
	|| (Name.find("��b") != string::npos)
	|| (Name.find("�Y��") != string::npos)
	|| (Name.find("����") != string::npos)
	|| (Name.find("�ɐ�") != string::npos)
	|| (Name.find("����") != string::npos)){
		MinusHit = Sum_41 * 0.03 + Sum_46 * 0.06;
	}
	if((Name.find("�}�K") != string::npos)
	|| (Name.find("�R��") != string::npos)){
		if(Name.find("����") != string::npos){
			MinusHit = Sum_46 * 0.03;
		} else{
			MinusHit = Sum_41 * 0.03 + Sum_46 * 0.06;
		}
	}
	if((Name.find("����") != string::npos)
	|| (Name.find("����") != string::npos)){
		MinusHit = Sum_46 * 0.03;
	}
	return MinusHit;
}
