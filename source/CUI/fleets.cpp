#include <fstream>
#include <random>
#include "header.h"

using std::fstream;

/* �����̒�` */
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<> Rand(0.0, 1.0);

/* �v���g�^�C�v�錾 */
//���C��
void ShowListStart(fleets**, const bool isShow = true);
void SearchPhase(fleets**, bool*, const bool isShow = true);
BP BPSelect(fleets**);
AIR_MAS AirWarPhase(fleets**, const bool*, double*, const bool isShow = true);
void FirstTorpedoPhase(fleets**, const BP, const double*, const bool isShow = true);
void FirePhase(fleets**, const BP, const AIR_MAS, const double*, const bool isShow = true);
void FirePhase2(fleets**, const BP, const AIR_MAS, const double*, const bool isShow = true);
void TorpedoPhase(fleets**, const BP, const double*, const bool isShow = true);
void NightPhase(fleets**, const BP, const double*, const bool isShow = true);
WIN ShowListEnd(fleets**, const bool, const BP, const bool isShow = true);
//�⏕
int RoundUp5(int);
bool isExit(fleets**);
bool isAllSubmarine1(fleets*);
int RandInt(const int);
bool CheckPercent(const double);
int AttackAction(fleets*, fleets*, const int, int&, const int, const BP, const double, const TURN,
                 const bool isShow = true, const double Multiple = 1.0, const bool isSpecialAttack = false);
TYPE ToType(const string);
int ToInt(const string);
RANGE ToRange(const string);
SC ToKind(const string);
SPEED ToSpeed(const string);

/* ---------------------- */
/* |  �V�~�����[�g�֌W  | */
/* | (���C���ƂȂ�֐�) | */
/* ---------------------- */

/* �퓬���V�~�����[�g���� */
WIN fleets::Simulate(fleets &Enemy, const bool isShow) {
	fleets* Fleets[] = {this, &Enemy};
	/* 0. ���\�� */
	ShowListStart(Fleets, isShow);
	/* 1. ���G�t�F�C�Y */
	bool isSearchSuccess[2];
	SearchPhase(Fleets, isSearchSuccess, isShow);
	/* 1.5. �͑��s���̌��� */
	BP BattlePosition = BPSelect(Fleets);
	bool isShowBP = false;	//�J�������܂łɏI�������ۂɁA���`�Ԃ�\�����邽�߂̃I�v�V����
	/* 2. �q���t�F�C�Y */
	double AllAttackPlus[BattleSize];
	AIR_MAS AirWarResult = AirWarPhase(Fleets, isSearchSuccess, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
	/* 3. �J�������t�F�C�Y */
	FirstTorpedoPhase(Fleets, BattlePosition, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
	/* 4. ���`�ԃt�F�C�Y */
	if(isShow) cout << "�y���`�ԃt�F�C�Y�z\n";
	if(isShow) cout << "�@���`�ԁF" << BPString[BattlePosition] << "\n";
	isShowBP = true;
	/* 5. �C����t�F�C�Y */
	FirePhase(Fleets, BattlePosition, AirWarResult, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
	/* 5.5. �C����t�F�C�Y(�񏄖�) */
	FirePhase2(Fleets, BattlePosition, AirWarResult, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
	/* 6. ������t�F�C�Y */
	TorpedoPhase(Fleets, BattlePosition, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
	/* 7. ���t�F�C�Y */
	NightPhase(Fleets, BattlePosition, AllAttackPlus, isShow);
	if(isExit(Fleets)) goto Exit;
Exit:
	/* X. ���ʕ\�� */
	WIN WinReason = ShowListEnd(Fleets, isShowBP, BattlePosition, isShow);

	if(isShow) cout << "\n";
	return WinReason;
}

/* ���\�� */
void ShowListStart(fleets **Fleets, const bool isShow) {
	if(isShow) cout << "�y���\���z\n";
	for(int i = 0; i < BattleSize; ++i) {
		if(isShow) cout << "��" << Position[i] << "\n";
		Fleets[i]->ShowList(isShow);
	}
	if(isShow) cout << "���w�`\n";
	if(isShow) cout << "�@" << FORString[Fleets[FriendSide]->Formation] << "vs" << FORString[Fleets[EnemySide]->Formation] << "\n";
}

/* ���G�t�F�C�Y */
void SearchPhase(fleets **Fleets, bool *isSearchSuccess, const bool isShow) {
	if(isShow) cout << "�y���G�t�F�C�Y�z\n";
	for(int i = 0; i < BattleSize; ++i) {
		double SearchPower = Fleets[i]->CalcSearchPower();
		if(isShow) cout << "�@" << Position[i] << "���G�l�F" << SearchPower << "\n";
		//���G�̐����������悭������Ȃ��̂Ŏ��͓K���������肷��
		if(SearchPower > 10.0) {
			if(isShow) cout << "�@" << Position[i] << "���G���ʁF����\n";
			isSearchSuccess[i] = true;
		} else {
			if(isShow) cout << "�@" << Position[i] << "���G���ʁF���s\n";
			isSearchSuccess[i] = false;
		}
	}
}

/* ���`�Ԃ̌��� */
BP BPSelect(fleets **Fleets) {
	double p = Rand(mt);
	if(p < 0.45) {
		return BP_SAME;		//���q��
	} else if(p < 0.75) {
		return BP_DIFF;		//���q��
	} else if(p < 0.90) {
		return BP_T_PLUS;	//T���L��
	} else {
		// �ʉ_�������ꍇ�͔��q��A�����łȂ��ꍇ��T���s��
		if((Fleets[FriendSide]->hasSaiun()) || (Fleets[EnemySide]->hasSaiun())) {
			return BP_DIFF;
		} else {
			return BP_T_MINUS;
		}
	}
}

/* �q���t�F�C�Y */
AIR_MAS AirWarPhase(fleets **Fleets, const bool *isSearchSuccess, double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y�q���t�F�C�Y�z\n";
	/* �����Ԃ̌��� */
	if(isShow) cout << "�����󌠂̌���\n";
	// ����l�̌v�Z
	int AntiAirScore[BattleSize];
	bool hasAir[BattleSize];
	for(int i = 0; i < BattleSize; ++i) {
		AntiAirScore[i] = 0;
		hasAir[i] = false;
		if(!isSearchSuccess[i]) continue;	//���G���s���͐���l0
		for(vector<kammusu>::iterator itKammusu = Fleets[i]->Kammusues.begin(); itKammusu != Fleets[i]->Kammusues.end(); ++itKammusu) {
			for(int j = 0; j < itKammusu->Slots; ++j) {
				if((itKammusu->Weapons[j].isAir()) && (itKammusu->Airs[j] != 0)) hasAir[i] = true;
				if(itKammusu->Weapons[j].isAirWar()) {
					AntiAirScore[i] += static_cast<int>(itKammusu->Weapons[j].AntiAir * sqrt(itKammusu->Airs[j]));
				}
			}
		}
		if(isShow) cout << "�@" << Position[i] << "����l�F" << AntiAirScore[i] << "\n";
	}
	// �����Ԃ̊m�F
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
	if(isShow) cout << "�@���ʁF" << AMString[AirWarResult] << "\n";
	/* �G�ڔ��� */
	// ���̔�����@�����m�Ȃ̂��͔��������Ǘ��΂ɂ���[�Ȃ����Ǝv���
	if(isShow) cout << "���G�ڔ���\n";
	for(int i = 0; i < BattleSize; ++i) {
		AllAttackPlus[i] = 1.0;
		if(isShow) cout << "�@" << Position[i] << "�G�ځF";
		// �����E�G���ɍq��򐨈ȏ�łȂ��Ɣ������Ȃ�
		if(((i == FriendSide) && ((AirWarResult == AM_WORST) || (!hasAir[i])))
		|| ((i == EnemySide) && ((AirWarResult == AM_BEST) || (!hasAir[i])))){
			if(isShow) cout << "�s�\\n";
			continue;
		}
		// �͍U�E���E����E�͒オ���݂���Ƃ��̂ݔ���
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
			if(isShow) cout << "�\(�����A�{" << static_cast<int>((AllAttackPlus[i] - 1) * 100) << "��)\n";
		} else{
			if(isShow) cout << "�\(���s)\n";
		}
	}
	/* �󒆐� */
	// ���Ċ����͈�l�����ɂ������ǂ���ł����̂��낤���H
	// �ƌ���������C�Ȃ����R�ƓG�R�ŗ��Ƃ���������Ȃ�Ⴄ���Ă̂�
	// �V�~����ҋ���������ȁc�c�܂��K���ɂł����グ����
	if(isShow) cout << "���퓬�@�ɂ��G�͍ڋ@�̍U���E����\n";
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
		if(isShow) cout << "�@" << Position[i] << "�F" << floor(KilledAirsPer[i] * 1000 + 0.5) / 10 << "��\n";
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
	/* �h��U���E�΋���� */
	// ����������R�ƓG�R�Ōv�Z���@���Ⴄ�p�^�[��
	// ���R�̕������ʂقǕ��G�Ȃ̂Ŕ������P�ŏ����Ă܂���
	// (�΋�J�b�g�C���͍���l�����܂���)
	if(isShow) cout << "���΋�ΖC�ɂ��G�U���@�E�����@�̌}���E����\n";
	// [���R���G�R�͍̊ڋ@���}������ꍇ]
	//�͑��΋�{�[�i�X�l������
	int FleetsAntiAirBonus = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
		double AntiAirBouns = 0.0;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			//���O�𒼐ڎw�肾�Ȃ�Ĕ��f���o�邯�ǎd���Ȃ��ˁc�c
			if(itWeapon->Name == "46cm�O�A���C"){
				AntiAirBouns += itWeapon->AntiAir * 0.25;
			} else if(itWeapon->Name.find("���p�C") != string::npos){
				AntiAirBouns += itWeapon->AntiAir * 0.35;
			} else if(itWeapon->Name.find("�΋�d�T") != string::npos){
				AntiAirBouns += itWeapon->AntiAir * 0.40;
			} else if(itWeapon->Name == "�O���e"){
				AntiAirBouns += itWeapon->AntiAir * 0.60;
			} else{
				AntiAirBouns += itWeapon->AntiAir * 0.20;
			}
		}
		FleetsAntiAirBonus += static_cast<int>(AntiAirBouns);
	}
	FleetsAntiAirBonus = static_cast<int>(FleetsAntiAirBonus * AntiAirBonusPer[Fleets[FriendSide]->Formation]);
	if(isShow) cout << "�@���R�΋�{�[�i�X�l�F" << FleetsAntiAirBonus << "\n";
	//�}���I
	int AllKilledAirs = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[EnemySide]->Kammusues.begin(); itKammusu != Fleets[EnemySide]->Kammusues.end(); ++itKammusu) {
		for(int j = 0; j < itKammusu->Slots; ++j) {
			if(itKammusu->Weapons[j].isAirWar2()) {
				kammusu *InterceptKammusu = &(Fleets[FriendSide]->Kammusues[Fleets[FriendSide]->RandomKammsu()]);
				int KilledAirs = static_cast<int>((InterceptKammusu->AllAntiAir() + FleetsAntiAirBonus) * 0.2125);
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
	if(isShow) cout << "�@���R�}�����ʁF" << AllKilledAirs << "�@\n";
	// [�G�R�����R�͍̊ڋ@���}������ꍇ]
	AllKilledAirs = 0;
	for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
		for(int j = 0; j < itKammusu->Slots; ++j) {
			if(itKammusu->Weapons[j].isAirWar2()) {
				kammusu *InterceptKammusu = &(Fleets[EnemySide]->Kammusues[Fleets[EnemySide]->RandomKammsu()]);
				int KilledAirs = InterceptKammusu->AllAntiAir() / 25;
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
	if(isShow) cout << "�@�G�R�}�����ʁF" << AllKilledAirs << "�@\n";
	/* �J������ */
	// �S�͓����Ȃ̂ŁA�S�Ŕ���͂Ђƒʂ�I�������ɍs��
	// (�L���b�v�O�␳�͖����Ȃ̂ŁAAttackAction�֐��̈����ɂ�����BP_SAME�̓_�~�[)
	if(isShow) cout << "���J������\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//�_���[�W�v�Z
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			for(int k = 0; k < MovedKammusu->Slots; ++k) {
				if((MovedKammusu->Airs[k] != 0) && (MovedKammusu->Weapons[k].isAirWar2())) {
					int Target = RandInt(Fleets[OtherSide]->Members);
					int BaseAttack;
					switch(MovedKammusu->Weapons[k].Type) {
						case Type_PB:
						case Type_PBF:
						case Type_WB:
							//�����͓��{�_���[�W
							BaseAttack = static_cast<int>(MovedKammusu->Weapons[k].Bomb * sqrt(MovedKammusu->Airs[k]) + 25);
							break;
						case Type_PA:
							//������150����80�����������_���Ō��܂�
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
		//�_���[�W����
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(isShow) cout << "�@" << MovedKammusu->Label(OtherSide) << "��" << AllDamage[j] << "�_���[�W�I\n";
				if(MovedKammusu->HP > AllDamage[j]) {
					MovedKammusu->HP -= AllDamage[j];
					if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
				} else {
					// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
					// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
					if(OtherSide == 0){
						MovedKammusu->HP -= 0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP);
						if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
						if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
					}else{
						MovedKammusu->HP = 0;
						if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
					}
				}
			}
		}
	}
	

	return AirWarResult;
}

/* �J�������t�F�C�Y */
void FirstTorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y�J�������t�F�C�Y�z\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//�_���[�W�v�Z
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j){
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			if((MovedKammusu->isFirstTorpedo()) && (MovedKammusu->ShowDamage() != Lost)){
				int Target = Fleets[OtherSide]->RandomKammsuWithoutSS();
				if(Target != -1){
					int BaseAttack = MovedKammusu->AllTorpedo() + 5;
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BattlePosition, AllAttackPlus[i], TURN_TOR_FIRST, isShow);
					AllDamage[Target] += Damage;
					isTarget[Target] = true;
				}
			}
		}
		//�_���[�W����
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(MovedKammusu->ShowDamage() != Lost){
					if(isShow) cout << "�@" << MovedKammusu->Label(OtherSide) << "��" << AllDamage[j] << "�_���[�W�I\n";
					if(MovedKammusu->HP > AllDamage[j]) {
						MovedKammusu->HP -= AllDamage[j];
						if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
					} else {
						// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
						// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
						if(OtherSide == 0){
							MovedKammusu->HP -= 0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP);
							if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
							if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
						}else{
							MovedKammusu->HP = 0;
							if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
						}
					}
				}
			}
		}
	}
}

/* �C����t�F�C�Y */
void FirePhase(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y�C����t�F�C�Y�z\n";
	/* �U���������� */
	vector< vector<int> >ShotList(BattleSize);
	for(int i = 0; i < BattleSize; ++i) {
		//�s���\�Ȋ͖��ꗗ���쐬����
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			if(Fleets[i]->Kammusues[j].isMoveInGun()) {
				ShotList[i].push_back(j);
			}
		}
		//�ꗗ���V���b�t������
		random_shuffle(ShotList[i].begin(), ShotList[i].end());
		//�ꗗ���˒��ň���\�[�g����
		//(����ɂ��A���˒��ł͏��Ԃ������_���ɂȂ�)
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
	/* �U���J�n */
	for(unsigned int i = 0; i < MaxKanmusu; ++i){
		for(int j = 0; j < BattleSize; ++j){
			if(ShotList[j].size() <= i) continue;
			if(!Fleets[j]->Kammusues[ShotList[j][i]].isMoveInGun()) continue;
			// �_���[�W�v�Z
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[ShotList[j][i]]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSub()){
				// �ΐ��U���\�͂̏ꍇ�A�����͌n���c���Ă���ۂɑΐ��U����D�悳����
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1){
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack){
				// �ΐ��U���łȂ���΁A����͂Ɍ������ĖC�����s��
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS();
				if(Target != -1){
					BaseAttack = MovedKammusu->AllAttack();
					isNormalAttack = true;
				}
			}
			// �_���[�W����
			// �ΐ��U���̏ꍇ
			if(isAntiSubAttack){
				int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
					BattlePosition, AllAttackPlus[j], TURN_GUN, isShow);
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
				} else {
					// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
					// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
					if(OtherSide == 0){
						TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//�ʏ�U���̏ꍇ
			if(isNormalAttack){
				// �U���̎�ނ𔻕�
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				if(((i == 0) && (AirWarResult > AM_NORMAL)) || ((i == 1) && (AirWarResult < AM_NORMAL))){
					//��@�@�����݂��Ȃ��Ƃ��������e���ϑ��ˌ����悤���Ȃ����A��j����{�I�ɑʖ�
					if((MovedKammusu->isSearchAir()) && (MovedKammusu->ShowDamage() < HeavyDamage)) {
						//�e���ϑ��ˌ��͕������͔����m�����㏸�������������̂ŁA
						//�Ƃ肠����1�̉\���ɂ������m����30%�Ƃ��A����ɑ����G�␳�E
						//���͕␳�E�m�ێ��␳�������A����������m���𓙕�����悤�ɂ���
						vector<int> isAttackType(5, 0);		//vector<bool>�͗�ɂ���Ĕ�����
						MovedKammusu->ShowAttackType(isAttackType);
						int SpecialPer = 0, SpecialKinds = 0;
						for(vector<int>::iterator itAT = isAttackType.begin(); itAT != isAttackType.end(); ++itAT){
							if(*itAT == 1){
								SpecialPer += 30;
								++SpecialKinds;
							}
						}
						if(SpecialPer != 0){
							//�����G�␳�E���͕␳�E�m�ێ��␳��������
							SpecialPer += static_cast<int>(Fleets[i]->CalcSearchPower() * 0.06);
							if(ShotList[j][i] == 0) SpecialPer += 10;
							if(AirWarResult == AM_BEST) SpecialPer += 10;
							//����
							int p = static_cast<int>(Rand(mt) * 100);
							if(p < SpecialPer){
								int APKind = p * SpecialKinds / SpecialPer;
								int APKind_ = 0;
								for(unsigned int i = 0; i < isAttackType.size(); ++i){
									if(isAttackType[i]){
										if(APKind == APKind_){
											switch(i){
												case 0:
													//���J�b�g�C��
													if(isShow) cout << "<<���J�b�g�C���������I>>\n";
													Multiple = 1.5;
													break;
												case 1:
													//��O�J�b�g�C��
													if(isShow) cout << "<<��O�J�b�g�C���������I>>\n";
													Multiple = 1.3;
													break;
												case 2:
													//��d�J�b�g�C��
													if(isShow) cout << "<<��d�J�b�g�C���������I>>\n";
													Multiple = 1.2;
													break;
												case 3:
													//�啛�J�b�g�C��
													if(isShow) cout << "<<�啛�J�b�g�C���������I>>\n";
													Multiple = 1.1;
													break;
												case 4:
													//�A��
													if(isShow) cout << "<<�A���������I>>\n";
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
				//�U������
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_GUN, isShow, Multiple, isSpecialAttack);
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					} else {
						// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
						// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
						if(OtherSide == 0){
							TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* �C����t�F�C�Y(�񏄖�) */
void FirePhase2(fleets **Fleets, const BP BattlePosition, const AIR_MAS AirWarResult, const double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y�C����t�F�C�Y(�񏄖�)�z\n";
	/* �Œ�1�ǈȏ��͂����݂��邩�𔻒肷�� */
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
	/* �U���������� */
	vector< vector<int> >ShotList(BattleSize);
	for(int i = 0; i < BattleSize; ++i) {
		//�s���\�Ȋ͖��ꗗ���쐬����
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			if(Fleets[i]->Kammusues[j].isMoveInGun()) {
				ShotList[i].push_back(j);
			}
		}
	}
	/* �U���J�n */
	for(unsigned int i = 0; i < MaxKanmusu; ++i) {
		for(int j = 0; j < BattleSize; ++j) {
			if(ShotList[j].size() <= i) continue;
			if(!Fleets[j]->Kammusues[ShotList[j][i]].isMoveInGun()) continue;
			// �_���[�W�v�Z
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[ShotList[j][i]]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSub()) {
				// �ΐ��U���\�͂̏ꍇ�A�����͌n���c���Ă���ۂɑΐ��U����D�悳����
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack) {
				// �ΐ��U���łȂ���΁A����͂Ɍ������ĖC�����s��
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAttack();
					isNormalAttack = true;
				}
			}
			// �_���[�W����
			// �ΐ��U���̏ꍇ
			if(isAntiSubAttack){
				int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
					BattlePosition, AllAttackPlus[j], TURN_GUN, isShow);
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
				} else {
					// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
					// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
					if(OtherSide == 0){
						TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//�ʏ�U���̏ꍇ
			if(isNormalAttack) {
				// �U���̎�ނ𔻕�
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				if(((i == 0) && (AirWarResult > AM_NORMAL)) || ((i == 1) && (AirWarResult < AM_NORMAL))){
					//��@�@�����݂��Ȃ��Ƃ��������e���ϑ��ˌ����悤���Ȃ����A��j����{�I�ɑʖ�
					if((MovedKammusu->isSearchAir()) && (MovedKammusu->ShowDamage() < HeavyDamage)) {
						//�e���ϑ��ˌ��͕������͔����m�����㏸�������������̂ŁA
						//�Ƃ肠����1�̉\���ɂ������m����30%�Ƃ��A����ɑ����G�␳�E
						//���͕␳�E�m�ێ��␳�������A����������m���𓙕�����悤�ɂ���
						vector<int> isAttackType(5, 0);		//vector<bool>�͗�ɂ���Ĕ�����
						MovedKammusu->ShowAttackType(isAttackType);
						int SpecialPer = 0, SpecialKinds = 0;
						for(vector<int>::iterator itAT = isAttackType.begin(); itAT != isAttackType.end(); ++itAT){
							if(*itAT == 1){
								SpecialPer += 30;
								++SpecialKinds;
							}
						}
						if(SpecialPer != 0){
							//�����G�␳�E���͕␳�E�m�ێ��␳��������
							SpecialPer += static_cast<int>(Fleets[i]->CalcSearchPower() * 0.06);
							if(ShotList[j][i] == 0) SpecialPer += 10;
							if(AirWarResult == AM_BEST) SpecialPer += 10;
							//����
							int p = static_cast<int>(Rand(mt) * 100);
							if(p < SpecialPer){
								int APKind = p * SpecialKinds / SpecialPer;
								int APKind_ = 0;
								for(unsigned int i = 0; i < isAttackType.size(); ++i){
									if(isAttackType[i]){
										if(APKind == APKind_){
											switch(i){
												case 0:
													//���J�b�g�C��
													if(isShow) cout << "<<���J�b�g�C���������I>>\n";
													Multiple = 1.5;
													break;
												case 1:
													//��O�J�b�g�C��
													if(isShow) cout << "<<��O�J�b�g�C���������I>>\n";
													Multiple = 1.3;
													break;
												case 2:
													//��d�J�b�g�C��
													if(isShow) cout << "<<��d�J�b�g�C���������I>>\n";
													Multiple = 1.2;
													break;
												case 3:
													//�啛�J�b�g�C��
													if(isShow) cout << "<<�啛�J�b�g�C���������I>>\n";
													Multiple = 1.1;
													break;
												case 4:
													//�A��
													if(isShow) cout << "<<�A���������I>>\n";
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
				//�U������
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], ShotList[j][i], Target, BaseAttack,
						BattlePosition, AllAttackPlus[j], TURN_GUN, isShow, Multiple, isSpecialAttack);
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					} else {
						// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
						// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
						if(OtherSide == 0){
							TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* ������t�F�C�Y */
void TorpedoPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y������t�F�C�Y�z\n";
	for(int i = 0; i < BattleSize; ++i) {
		int OtherSide = BattleSize - i - 1;
		vector<bool> isTarget(Fleets[OtherSide]->Members, false);
		//�_���[�W�v�Z
		vector<int> AllDamage(Fleets[OtherSide]->Members, 0);
		for(int j = 0; j < Fleets[i]->Members; ++j) {
			kammusu *MovedKammusu = &(Fleets[i]->Kammusues[j]);
			if((MovedKammusu->isTorpedo()) && (MovedKammusu->ShowDamage() < MiddleDamage)) {
				int Target = Fleets[OtherSide]->RandomKammsuWithoutSS();
				if(Target != -1) {
					int BaseAttack = MovedKammusu->AllTorpedo() + 5;
					int Damage = AttackAction(Fleets[i], Fleets[OtherSide], j, Target, BaseAttack,
						BattlePosition, AllAttackPlus[i], TURN_TOR, isShow);
					AllDamage[Target] += Damage;
					isTarget[Target] = true;
				}
			}
		}
		//�_���[�W����
		for(int j = 0; j < Fleets[OtherSide]->Members; ++j) {
			if(isTarget[j]) {
				kammusu *MovedKammusu = &(Fleets[OtherSide]->Kammusues[j]);
				if(MovedKammusu->ShowDamage() != Lost) {
					if(isShow) cout << "�@" << MovedKammusu->Label(OtherSide) << "��" << AllDamage[j] << "�_���[�W�I\n";
					if(MovedKammusu->HP > AllDamage[j]) {
						MovedKammusu->HP -= AllDamage[j];
						if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
					} else {
						// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
						// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
						if(OtherSide == 0){
							MovedKammusu->HP -= 0.5 * MovedKammusu->HP + 0.3 * RandInt(MovedKammusu->HP);
							if(MovedKammusu->HP <= 0) MovedKammusu->HP = 1;
							if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
						}else{
							MovedKammusu->HP = 0;
							if(isShow) cout << "�@�@�c��ϋv�F" << MovedKammusu->ShowHP() << "\n";
						}
					}
				}
			}
		}
	}
}

/* ���t�F�C�Y */
void NightPhase(fleets **Fleets, const BP BattlePosition, const double AllAttackPlus[], const bool isShow) {
	if(isShow) cout << "�y���t�F�C�Y�z\n";
	for(unsigned int i = 0; i < MaxKanmusu; ++i) {
		for(int j = 0; j < BattleSize; ++j) {
			if(Fleets[j]->Members <= static_cast<int>(i)) continue;
			if(!Fleets[j]->Kammusues[i].isMoveInNight()) continue;
			// �_���[�W�v�Z
			kammusu *MovedKammusu = &(Fleets[j]->Kammusues[i]);
			int OtherSide = BattleSize - j - 1;
			int BaseAttack, Target;
			bool isAntiSubAttack = false, isNormalAttack = false;
			if(MovedKammusu->isAntiSubInNight()) {
				// �ΐ��U���\�͂̏ꍇ�A�����͌n���c���Ă���ۂɑΐ��U����D�悳����
				Target = Fleets[OtherSide]->RandomKammsuWithSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAntiSub();
					isAntiSubAttack = true;
				}
			}
			if(!isAntiSubAttack) {
				// �ΐ��U���łȂ���΁A����͂Ɍ������ĖC�����s��
				Target = Fleets[OtherSide]->RandomKammsuWithoutSS();
				if(Target != -1) {
					BaseAttack = MovedKammusu->AllAttackInNight();
					isNormalAttack = true;
				}
			}
			// �_���[�W����
			//�ΐ��U���̏ꍇ
			if(isAntiSubAttack) {
				int Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack,
					BattlePosition, AllAttackPlus[j], TURN_NIGHT, isShow);
				kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
				if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
				if(TargetKammusu->HP > Damage) {
					TargetKammusu->HP -= Damage;
					if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
				} else {
					// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
					// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
					if(OtherSide == 0){
						TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
						if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}else{
						TargetKammusu->HP = 0;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					}
					if(isExit(Fleets)) return;
				}
			}
			//�ʏ�U���̏ꍇ
			if(isNormalAttack) {
				// �U���̎�ނ𔻕�
				int AttackCount = 1;
				double Multiple = 1.0;
				bool isSpecialAttack = false;
				AT AttackType = MovedKammusu->ShowAttackTypeInNight(AttackCount, Multiple, isSpecialAttack);
				if((AttackType == CutinAttackT) || (AttackType == CutinAttackG)){
					// �J�b�g�C���m�����v�Z���A���s�����ꍇ�͒ʏ�U���ɖ߂�
					double CutinPer = 15.0;
					//�J�b�g�C����ʂɂ���Ă͉^�L���b�v���l������
					if(AttackType == CutinAttackT){
						//�����J�b�g�C���̉^�L���b�v��50
						if(MovedKammusu->Luck > 50){
							CutinPer += 50;
						} else{
							CutinPer += MovedKammusu->Luck;
						}
					} else{
						//��C�J�b�g�C���̉^�L���b�v��40
						if(MovedKammusu->Luck > 40){
							CutinPer += 40;
						} else{
							CutinPer += MovedKammusu->Luck;
						}
					}
					//�z�u�␳
					if(i == 0) CutinPer += 15;
					//�����␳
					if(MovedKammusu->ShowDamage() == LightDamage)  CutinPer += 10;
					if(MovedKammusu->ShowDamage() == MiddleDamage) CutinPer += 20;
					//�T�Ɠ��E�Ɩ��e�␳
					if(Fleets[j]->hasLight())         CutinPer += 5;
					if(Fleets[OtherSide]->hasLight()) CutinPer -= 10;
					//�����Ő������ۂ��𔻒�
					if(!CheckPercent(CutinPer)){
						AttackCount = 1;
						Multiple = 1.0;
						isSpecialAttack = false;
						AttackType = NormalAttack;
					}
				}
				if(AttackType == DoubleAttack){
					// �A�����m���ɔ�������킯�ł͂Ȃ�(�Ƃ肠����99���Ƃ���)
					if(!CheckPercent(99)){
						AttackCount = 1;
						Multiple = 1.0;
						isSpecialAttack = false;
						AttackType = NormalAttack;
					}
				}
				// �U������
				switch(AttackType){
					case CutinAttackT:
						if(isShow) cout << "<<�����J�b�g�C���������I>>\n";
						break;
					case CutinAttackG:
						if(isShow) cout << "<<��C�J�b�g�C���������I>>\n";
						break;
					case DoubleAttack:
						if(isShow) cout << "<<�A���������I>>\n";
						break;
				}
				for(int k = 0; k < AttackCount; ++k){
					int Damage = AttackAction(Fleets[j], Fleets[OtherSide], i, Target, BaseAttack, BattlePosition,
						AllAttackPlus[j], TURN_NIGHT, isShow, Multiple, isSpecialAttack);
					kammusu *TargetKammusu = &(Fleets[OtherSide]->Kammusues[Target]);
					if(isShow) cout << "�@" << MovedKammusu->Label(j) << "��" << TargetKammusu->Label(OtherSide) << "��" << Damage << "�_���[�W�I\n";
					if(TargetKammusu->HP > Damage) {
						TargetKammusu->HP -= Damage;
						if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
					} else {
						// ���R�ɂ̂ݍ����X�g�b�p�[��K�p����
						// (�c��ϋv������_���[�W�������ꍇ�A�_���[�W�ʁ�0.5�~�ϋv+0.3�~[0,�ϋv-1]�Ƃ���)
						if(OtherSide == 0){
							TargetKammusu->HP -= 0.5 * TargetKammusu->HP + 0.3 * RandInt(TargetKammusu->HP);
							if(TargetKammusu->HP <= 0) TargetKammusu->HP = 1;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}else{
							TargetKammusu->HP = 0;
							if(isShow) cout << "�@�@�c��ϋv�F" << TargetKammusu->ShowHP() << "\n";
						}
					}
				}
				if(isExit(Fleets)) return;
			}
		}
	}
}

/* ���ʕ\�� */
//E�����̏������B��������̂ŁA�����Ĕ���ł͏Ȃ���
WIN ShowListEnd(fleets **Fleets, const bool isShowBP, const BP BattlePosition, const bool isShow) {
	if(isShow) cout << "�y���ʕ\���z\n";
	if(!isShowBP) {
		if(isShow) cout << "�@���`�ԁF" << BPString[BattlePosition] << "\n";
	}
	for(int i = 0; i < BattleSize; ++i) {
		if(isShow) cout << "��" << Position[i] << "\n";
		Fleets[i]->ShowList(isShow);
	}
	// ���������\��
	if(isShow) cout << "����������F";
	int AlivedKammusues[BattleSize];
	double ResultsGauge[BattleSize];
	for(int i = 0; i < BattleSize; ++i) {
		AlivedKammusues[i] = Fleets[i]->hasAlived();
		int OtherSide = BattleSize - i - 1;
		ResultsGauge[i] = Fleets[OtherSide]->ResultsGauge();
	}
	if(AlivedKammusues[FriendSide] == Fleets[FriendSide]->Members) {
		//���R�̊͂���ǂł��������ĂȂ��ꍇ
		if(AlivedKammusues[EnemySide] == 0) {
			//S�����m��B�ȉ��A���S�������𔻒�
			bool isPerfectWin = true;
			for(vector<kammusu>::iterator itKammusu = Fleets[FriendSide]->Kammusues.begin(); itKammusu != Fleets[FriendSide]->Kammusues.end(); ++itKammusu) {
				if(itKammusu->ShowDamage() != NoDamage) {
					isPerfectWin = false;
					break;
				}
			}
			if(isPerfectWin){
				if(isShow) cout << "���S����S\n";
				return WIN_SS;
			}
			if(isShow) cout << "����S\n";
			return WIN_S;
		} else {
			//A�������𔻒�
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
				if(isShow) cout << "����A\n";
				return WIN_A;
			}
			//�G���͂��������Ă���Ζ�������B����
			if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost){
				if(isShow) cout << "��p�I����B\n";
				return WIN_B;
			}
			//���R�̐�ʃQ�[�W�������2.5�{�ȏ�̏ꍇB����
			if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide] * 2.5) {
				if(isShow) cout << "��p�I����B\n";
				return WIN_B;
			}
			//���R�̐�ʃQ�[�W��������Ⴂ��0.1%�ȉ��Ȃ�s�kD�A�����łȂ���ΐ�p�I�s�kC
			if((ResultsGauge[FriendSide] < ResultsGauge[EnemySide])
			|| (ResultsGauge[FriendSide] < 0.001)) {
				if(isShow) cout << "�s�kD\n";
				return WIN_D;
			}
			if(isShow) cout << "��p�I�s�kC\n";
			return WIN_C;
		}
	}//���R�̊͂���ǈȏ㌂�����Ă���ꍇ
	//�G�̍������ŕ���
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
		//���R�̐�ʃQ�[�W�������2.5�{�ȏ�̏ꍇB����
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide] * 2.5) {
			if(isShow) cout << "��p�I����B\n";
			return WIN_B;
		}
		if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost){
			//���͂��������A�����͑��̌��������G�͑��̌������̎���p�I����B�A
			//�����Ȃ��Ɛ�p�I�s�kC
			if((Fleets[FriendSide]->Members - AlivedKammusues[FriendSide])
			 < (Fleets[EnemySide]->Members  - AlivedKammusues[EnemySide])){
				if(isShow) cout << "��p�I����B\n";
				return WIN_B;
			 }
			if(isShow) cout << "��p�I�s�kC\n";
			return WIN_C;
		}
		//��ʃQ�[�W�������1.0�{�ȏ�̎���p�I�s�kC�A�����Ȃ��Ɣs�kD
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide]) {
			if(isShow) cout << "��p�I�s�kC\n";
			return WIN_C;
		}
		if(isShow) cout << "�s�kD\n";
		return WIN_D;
	}
	if(Fleets[EnemySide]->Kammusues[0].ShowDamage() == Lost) {
		//���͂��������A�����͑��̌��������G�͑��̌������̎���p�I����B�A
		//�����Ȃ��Ɛ�p�I�s�kC
		if((Fleets[FriendSide]->Members - AlivedKammusues[FriendSide])
		 < (Fleets[EnemySide]->Members  - AlivedKammusues[EnemySide])) {
			if(isShow) cout << "��p�I����B\n";
			return WIN_B;
		}
		if(isShow) cout << "��p�I�s�kC\n";
		return WIN_C;
	} else {
		//���R�̐�ʃQ�[�W�������1.0�{�ȏ�̏ꍇ��p�I�s�kC�A�����Ȃ��Ɣs�kD
		if(ResultsGauge[FriendSide] >= ResultsGauge[EnemySide]) {
			if(isShow) cout << "��p�I�s�kC\n";
			return WIN_C;
		}
		if(isShow) cout << "�s�kD\n";
		return WIN_D;
	}
}

/* -------------------- */
/* | �V�~�����[�g�֌W | */
/* | (�⏕�ƂȂ�֐�) | */
/* -------------------- */

/* 5�̔{���ɐ؂�グ */
int RoundUp5(int n) {
	if(n % 5 == 0) {
		return n;
	} else {
		return (n / 5 + 1) * 5;
	}
}

/* ���G�l���v�Z���� */
double fleets::CalcSearchPower() {
	double Search = 0.0;
#ifdef USE_NEW_SEARCH_METHOD
	// 2-5��(�H)
	// http://ch.nicovideo.jp/biikame/blomaga/ar663428
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		Search += sqrt(itKammusu->Search) * 1.6841056;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			switch(itWeapon->Type) {
				case Type_PB:	//�͔�
					Search += itWeapon->Search * 1.0376255;
					break;
				case Type_WB:	//����
					Search += itWeapon->Search * 1.7787282;
					break;
				case Type_PA:	//�͍U
					Search += itWeapon->Search * 1.3677954;
					break;
				case Type_PS:	//�͒�
					Search += itWeapon->Search * 1.6592780;
					break;
				case Type_WS:	//����
					Search += itWeapon->Search * 2.0000000;
					break;
				case Type_SmallS:	//���^�d�T
					Search += itWeapon->Search * 1.0045358;
					break;
				case Type_LargeS:	//��^�d�T
					Search += itWeapon->Search * 0.9906638;
					break;
				case Type_SLight:	//�T�Ɠ�
					Search += itWeapon->Search * 0.9067950;
					break;
			}
		}
	}
	Search += RoundUp5(HQLevel) * (-0.6142467);
	return floor(Search * 10.0 + 0.5) / 10.0;	//������2�ʂ��l�̌ܓ�
#else
	// 2-5��(��)
	double AllKammusuSearch = 0.0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		AllKammusuSearch += itKammusu->Search;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			switch(itWeapon->Type) {
				case Type_WB:	//����
					Search += itWeapon->Search * 2;
					break;
				case Type_PS:	//�͒�
					Search += itWeapon->Search * 2;
					break;
				case Type_WS:	//����
					Search += itWeapon->Search * 2;
					break;
				case Type_SmallS:	//���^�d�T
					Search += itWeapon->Search * 1;
					break;
				case Type_LargeS:	//��^�d�T
					Search += itWeapon->Search * 1;
			}
		}
	}
	Search += sqrt(AllKammusuSearch);
	return floor(Search);	//�����_�ȉ��؂�̂�
#endif
}

/* �ʉ_�����邩�ǂ����𔻒肷�� */
bool fleets::hasSaiun() {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if(itWeapon->Type == Type_PSS) return true;
		}
	}
	return false;
}

/* �퓬�I���𔻒肷�� */
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

/* (����@�����ڂ��Ă��Ȃ�)�����͂������Ȃ������肷�� */
bool isAllSubmarine1(fleets *This){
	for(vector<kammusu>::iterator itKammusu = This->Kammusues.begin(); itKammusu != This->Kammusues.end(); ++itKammusu) {
		if(!itKammusu->isSubmarine()) return false;
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if(itWeapon->isAir()) return false;
		}
	}
	return true;
}

/* [0, N - 1]�Ȑ��������𐶐����� */
int RandInt(const int n) {
	return static_cast<int>(Rand(mt) * n);
}

/* ��������Limit[��]�Ȏ��ۂɂ��āA�������ۂ��𔻒肷�� */
bool CheckPercent(const double Limit) {
	if(Rand(mt) * 100 < Limit) {
		return true;
	} else {
		return false;
	}
}

/* �����c���Ă�͖����烉���_���ɑI������ */
int fleets::RandomKammsu(){
	//�����͂����X�g�A�b�v
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			AlivedList.push_back(i);
		}
	}
	//�K���ȏꏊ�̊͂�Ԃ�
	return AlivedList[RandInt(AlivedList.size())];
}

/* �����c���Ă�͖�(�����͌n)���烉���_���ɑI������ */
int fleets::RandomKammsuWithSS(){
	//�����͂����X�g�A�b�v
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			if(Kammusues[i].isSubmarine()){
				AlivedList.push_back(i);
			}
		}
	}
	//�K���ȏꏊ�̊͂�Ԃ�
	if(AlivedList.size() == 0){
		return -1;
	} else{
		return AlivedList[RandInt(AlivedList.size())];
	}
}

/* �����c���Ă�͖�(�����)���烉���_���ɑI������ */
int fleets::RandomKammsuWithoutSS(){
	//�����͂����X�g�A�b�v
	vector<int> AlivedList;
	for(int i = 0; i < Members; ++i) {
		if(Kammusues[i].ShowDamage() != Lost){
			if(!Kammusues[i].isSubmarine()){
				AlivedList.push_back(i);
			}
		}
	}
	//�K���ȏꏊ�̊͂�Ԃ�
	if(AlivedList.size() == 0){
		return -1;
	} else{
		return AlivedList[RandInt(AlivedList.size())];
	}
}

/* �^����_���[�W�ʂ��v�Z���� */
int AttackAction(fleets *Friend, fleets *Enemy, const int Hunter, int &Target, const int BaseAttack,
                 const BP BattlePosition, const double AllAttackPlus, const TURN Turn,
                 const bool isShow, const double Multiple, const bool isSpecialAttack) {
	kammusu *HunterK = &(Friend->Kammusues[Hunter]);
	kammusu *TargetK = &( Enemy->Kammusues[Target]);
	/* ���͂������ꍇ�A�u���΂��v��������������\�������� */
	//(����or�������m�ŁA�����΂��������j���炵�ĂȂ��ꍇ�Ɍ���)
	if(Target == 0){
		//���^�͂ł��΂��鑊�肪���݂��邩�𒲂ׂ�
		bool FlagshipIsSubmarine = TargetK->isSubmarine();
		vector<int> CanBlockList;
		for(int i = 1; i < Enemy->Members; ++i){
			if((Enemy->Kammusues[i].isSubmarine() == FlagshipIsSubmarine) && (Enemy->Kammusues[i].ShowDamage() < LightDamage)){
				CanBlockList.push_back(i);
			}
		}
		if(CanBlockList.size() != 0){
			//���΂��m����1�ǖ���5�����Ƃ���(��������)
			for(unsigned int i = 0; i < CanBlockList.size(); ++i){
				if(CheckPercent(5)){
					int BlockKammusu = CanBlockList[i];
					if(isShow) cout << "<<���͂ւ̍U����" << Enemy->Kammusues[BlockKammusu].Name << "�����΂��I>>\n";
					TargetK = &(Enemy->Kammusues[BlockKammusu]);
					Target = BlockKammusu;
				}
			}
		}
	}


	double Damage = BaseAttack;
	/* ���������v�Z���A�������邩�ǂ����𔻒肷�� */
	// ���
	double EvadeSum = TargetK->AllEvade();	//������v���v�Z
	if(Friend->Formation == FOR_SUBTRAIL) EvadeSum /= 2;	//���c�w���Ƒ���̉�𗦂�������
	if(TargetK->ShowCond() == Happy) EvadeSum *= 1.8;
	double EvadeValue;
	if(EvadeSum <= 37.5){
		EvadeValue = EvadeSum / 75;
	} else {
		EvadeValue = EvadeSum / (EvadeSum + 37.5);
	}
	//(���͍��ɂ����␳�B������񎎌�����)
	bool isEqualSpeed = true;
	for(int i = 0; i < Enemy->Members; ++i){
		if(Enemy->Kammusues[i].Speed != Enemy->Kammusues[0].Speed){
			isEqualSpeed = false;
			break;
		}
	}
	if(!isEqualSpeed) EvadeValue -= 0.01;
	// ������
	double HitValue = 1.0 + (sqrt((HunterK->Level - 1) * 5) + HunterK->AllHit()) / 100;
	if(HunterK->ShowCond() == RedFatigue) HitValue /= 2;
	HitValue += HunterK->Luck * 0.001;		//�����ɉ^���e������\�����H
	HitValue -= HunterK->NonFit();			//�t�B�b�g�C�␳
	if(Turn == TURN_NIGHT) HitValue *= 1.5;	//��펞�ɂ����閽���␳(��������)
	// �ŏI�I�Ȗ��������v�Z����
	HitValue -= EvadeValue;

	/* �ΐ��U�����ƐF�X�ς��̂Ő�ɔ��肵�Ă��� */
	bool isAttackToSub = TargetK->isSubmarine();
	if((isAttackToSub) && (Turn != TURN_GUN) && (Turn != TURN_NIGHT)) return 0;

	/* �L���b�v�O�␳ */
	if(Turn != TURN_AIR) {
		//�U�����w�`
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
		//���`��
		if(Turn != TURN_NIGHT) {
			switch(BattlePosition) {
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
		//�������
		switch(HunterK->ShowDamage()) {
			case MiddleDamage:
				Damage *= 0.7;
				break;
			case HeavyDamage:
				Damage *= 0.4;
				break;
		}
		//���
		if((Turn == TURN_NIGHT) && (isSpecialAttack)) {
			Damage *= Multiple;
		}
	}

	/* �L���b�v */
	if(isAttackToSub) {
		if(Damage > 100) Damage = 100 + sqrt(Damage - 100);
	} else {
		if(Turn == TURN_NIGHT) {
			if(Damage > 300) Damage = 300 + sqrt(Damage - 300);
		} else {
			if(Damage > 150) Damage = 150 + sqrt(Damage - 150);
		}
	}

	/* �L���b�v��␳ */
	//�N���e�B�J���␳
	if(Turn == TURN_AIR) {
		if(Rand(mt) < 0.025) {
			Damage *= 1.5;
		}
	} else if(Turn == TURN_NIGHT){	//��펞�ɂ�����N���e�B�J���␳(��������)
		if(Rand(mt) < 0.30) {
			Damage *= 1.5;
		}
	}else if(Rand(mt) < 0.15) {
		Damage *= 1.5;
	}
	//�G�ڕ␳
	Damage *= AllAttackPlus;
	//�e���ϑ��ˌ��␳
	if((Turn == TURN_GUN) && (isSpecialAttack)) {
		Damage *= Multiple;
	}
	/* �ŏI�I�ȃ_���[�W�ʂ����� */
	Damage = Damage - TargetK->Defense * (Rand(mt) * 2 / 3 + 2.0 / 3);
	//�c��e��ʕ␳
	if(HunterK->Ammo < 50) {
		Damage *= 2 * HunterK->Ammo / 100;
	}
	//�e���ϑ��ˌ����A�J�b�g�C��������Ɩ����������I�ɏ㏸����
	if((Turn == TURN_GUN) && (isSpecialAttack)){
		if(HitValue < 0.9) HitValue = 0.9;		//�b��I�Ȏd�l
	}
	//�e���ϑ��ˌ�����і�틛���J�b�g�C������і��A���Ȃ������Ă��J�X�_���A����ȊO�ł�0�_���[�W
	if(HitValue < Rand(mt)) {
		if(isSpecialAttack) {
			Damage = 0.0;
		} else {
			return 0;
		}
	}
	//���ł̑ΐ��U��
	if((Turn == TURN_NIGHT) && (isAttackToSub)) Damage = 0.0;
	//�J�X�_�����͑���c��ϋv��6�`14����^����
	if(Damage < 1.0) {
		Damage = TargetK->HP * (Rand(mt) * 0.08 + 0.06);
	}
	return static_cast<int>(Damage);
}

/* �����͂̐����J�E���g���� */
int fleets::hasAlived() {
	int Alived = 0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		if(itKammusu->ShowDamage() != Lost) {
			++Alived;
		}
	}
	return Alived;
}

/* ��ʃQ�[�W�ʂ��v�Z���� */
double fleets::ResultsGauge() {
	int MaxHPAll = 0, HPAll = 0;
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		MaxHPAll += itKammusu->MaxHP;
		HPAll += itKammusu->HP;
	}
	return static_cast<double>(MaxHPAll - HPAll) / MaxHPAll;
}

/* �T�Ɠ���Ɩ��e�������Ă��邩�𔻒肷�� */
bool fleets::hasLight(){
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		for(vector<weapon>::iterator itWeapon = itKammusu->Weapons.begin(); itWeapon != itKammusu->Weapons.end(); ++itWeapon) {
			if((itWeapon->Type == Type_SLight) || (itWeapon->Type == Type_LightB)) return true;
		}
	}
	return false;
}

/* ------------------ */
/* |  ���̑��̊֐�  | */
/* ------------------ */

/* �͖����Z�b�g���� */
void fleets::SetKammusu(const kammusu &Kammusu) {
	Kammusues.push_back(Kammusu);
	++Members;
}

/* ��Ԃ����Z�b�g���� */
void fleets::Reset() {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		itKammusu->HP = itKammusu->MaxHP;		//�����ł���Ɖ���
		itKammusu->cond = 49;					//�萔�l�ł��邱�Ƃɒ���
		itKammusu->Ammo = 100;					//100%���ő傾���A����50%�ȏ�͊F��������
		itKammusu->Airs = itKammusu->MaxAirs;	//�͍ڋ@��������������
	}
}

/* �͑��Ɋւ������\������ */
void fleets::ShowList(const bool isShow) {
	for(vector<kammusu>::iterator itKammusu = Kammusues.begin(); itKammusu != Kammusues.end(); ++itKammusu) {
		if(isShow) cout << "�@" << itKammusu->Label() << " " << itKammusu->ShowHP() << "\n";
	}
}

/* �t�@�C������ǂݍ��� */
void fleets::ReadData(const string Filename){
	std::locale::global(std::locale("japanese"));
	/* weapon.txt���瑕���f�[�^��ǂݍ��� */
	fstream fin1("weapon.csv");
	if(!fin1.is_open()) throw "weapon.txt������ɓǂݍ��߂܂���ł���.";
	// 2�s�ڈȍ~�𑕔��f�[�^�Ƃ��ēǂݍ���
	vector<weapon> WeaponList;
	string GetLine;
	getline(fin1, GetLine);
	while (getline(fin1, GetLine)){
		//CSV�����
		vector<string> WeaponData;
		string temp;
		stringstream sin(GetLine);
		while(getline(sin, temp, ',')){
			WeaponData.push_back(temp);
		}
		if(WeaponData.size() < 13) throw "�����f�[�^�̍��ڐ����ُ�ł�.";
		//�����f�[�^�ɕϊ�����
		weapon Weapon;
		Weapon.Name    = WeaponData[1];
		Weapon.Type    = ToType(WeaponData[2]);
		Weapon.Attack  = ToInt(WeaponData[3]);
		Weapon.Torpedo = ToInt(WeaponData[4]);
		Weapon.Bomb    = ToInt(WeaponData[5]);
		Weapon.AntiAir = ToInt(WeaponData[6]);
		Weapon.AntiSub = ToInt(WeaponData[7]);
		Weapon.Search  = ToInt(WeaponData[8]);
		Weapon.Hit     = ToInt(WeaponData[9]);
		Weapon.Evade   = ToInt(WeaponData[10]);
		Weapon.Range   = ToRange(WeaponData[11]);
		Weapon.Defense = ToInt(WeaponData[12]);
		//���X�g�ɒǉ�����
		WeaponList.push_back(Weapon);
	}
	fin1.close();

	/* kammusu.txt����͖��f�[�^��ǂݍ��� */
	fstream fin2("kammusu.csv");
	if(!fin2.is_open()) throw "kammusu.txt������ɓǂݍ��߂܂���ł���.";
	// 2�s�ڈȍ~�𑕔��f�[�^�Ƃ��ēǂݍ���
	vector<kammusu> KammusuList;
	getline(fin2, GetLine);
	while (getline(fin2, GetLine)){
		//CSV�����
		vector<string> KammusuData;
		string temp;
		stringstream sin(GetLine);
		while(getline(sin, temp, ',')){
			KammusuData.push_back(temp);
		}
		KammusuData.push_back(temp);
		if(KammusuData.size() < 16) throw "�͖��f�[�^�̍��ڐ����ُ�ł�.";
		//�͖��f�[�^�ɕϊ�����
		kammusu Kammusu;
		Kammusu.Name    = KammusuData[1];
		Kammusu.Kind    = ToKind(KammusuData[2]);
		Kammusu.Level   = ToInt(KammusuData[3]);
		Kammusu.MaxHP   = ToInt(KammusuData[4]);
		Kammusu.Attack  = ToInt(KammusuData[5]);
		Kammusu.Defense = ToInt(KammusuData[6]);
		Kammusu.Torpedo = ToInt(KammusuData[7]);
		Kammusu.Evade   = ToInt(KammusuData[8]);
		Kammusu.AntiAir = ToInt(KammusuData[9]);
		Kammusu.AntiSub = ToInt(KammusuData[10]);
		Kammusu.Speed   = ToSpeed(KammusuData[11]);
		Kammusu.Search  = ToInt(KammusuData[12]);
		Kammusu.Range   = ToRange(KammusuData[13]);
		Kammusu.Luck    = ToInt(KammusuData[14]);
		Kammusu.Slots   = ToInt(KammusuData[15]);
		Kammusu.Weapons.resize(Kammusu.Slots);
		Kammusu.MaxAirs.resize(Kammusu.Slots);
		weapon Weapon_None;
		for (int i = 0; i < Kammusu.Slots; ++i){
			Kammusu.Weapons[i] = Weapon_None;
			Kammusu.MaxAirs[i] = ToInt(KammusuData[16 + i]);
		}
		Kammusu.HP = Kammusu.MaxHP;
		Kammusu.cond = 49;
		Kammusu.Ammo = 100;
		Kammusu.Airs = Kammusu.MaxAirs;
		//���X�g�ɒǉ�����
		KammusuList.push_back(Kammusu);
	}
	fin2.close();

	/* Filename����͑��f�[�^��ǂݍ��� */
	fstream fin3(Filename);
	if (!fin3.is_open()) throw "�͑��f�[�^������ɓǂݍ��߂܂���ł���.";
	Members = 0;
	Kammusues.clear();
	int Step = 0;
	while (getline(fin3, GetLine)){
		// ��s�y��#����n�܂�s(�R�����g�s)�͖�������
		if (GetLine == "") continue;
		if (GetLine.substr(0, 1) == "#") continue;
		if (Step == 0){
			//�i�ߕ����x��
			HQLevel = ToInt(GetLine);
			++Step;
		}else if (Step == 1){
			//�����o�[(�J���}��؂�)
			string temp;
			stringstream sin(GetLine);
			while (getline(sin, temp, ',')){
				int Number = ToInt(temp) - 1;
				if (Number < 0) throw "�����o�[�w�肪�ُ�ł�.";
				SetKammusu(KammusuList[Number]);
			}
			++Step;
		}else if (Step < 2 + Members){
			//����(�J���}��؂�)
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
			//���(�J���}��؂�)
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
						//�c��ϋv
						Kammusues[Set].HP = Number;
					}
					if((i == 1) && (Number <= 100)) {
						//cond�l
						Kammusues[Set].cond = Number;
					}
					if((i == 2) && (Number <= 100)) {
						//�c��e��
						Kammusues[Set].Ammo = Number;
					}
					if((i >= 3) && (i - 3 < Kammusues[Set].Slots) && (Number <= Kammusues[Set].MaxAirs[i - 3])) {
						//1�`4�X���ڂ̎c��͍ڋ@��
						Kammusues[Set].Airs[i - 3] = Number;
					}
				}
			}
			++Step;
		}
	}
	return;
}

/* �����̎�� */
TYPE ToType(const string TypeStr){
	if(TypeStr == "����"){
		return Type_None;
	}else if(TypeStr == "��C"){
		return Type_Gun;
	}else if(TypeStr == "���C"){
		return Type_SubGun;
	}else if(TypeStr == "����"){
		return Type_Torpedo;
	}else if(TypeStr == "������q��"){
		return Type_SpecialSS;
	}else if(TypeStr == "�͏�퓬�@"){
		return Type_PF;
	}else if(TypeStr == "�͏㔚���@(����)"){
		return Type_PBF;
	}else if(TypeStr == "�͏㔚���@"){
		return Type_PB;
	}else if(TypeStr == "���㔚���@"){
		return Type_WB;
	}else if(TypeStr == "�͏�U���@"){
		return Type_PA;
	}else if(TypeStr == "�͏��@�@"){
		return Type_PS;
	}else if(TypeStr == "�͏��@�@(�ʉ_)"){
		return Type_PSS;
	}else if(TypeStr == "�����@�@"){
		return Type_WS;
	}else if(TypeStr == "��Ԓ�@�@"){
		return Type_WSN;
	}else if(TypeStr == "�ΐ������@"){
		return Type_ASPP;
	}else if(TypeStr == "�I�[�g�W���C��"){
		return Type_AJ;
	}else if(TypeStr == "���^�d�T"){
		return Type_SmallS;
	}else if(TypeStr == "��^�d�T"){
		return Type_LargeS;
	}else if(TypeStr == "�΋�@�e"){
		return Type_AAG;
	}else if(TypeStr == "���ˑ��u"){
		return Type_AAD;
	}else if(TypeStr == "����"){
		return Type_DP;
	}else if(TypeStr == "�\�i�["){
		return Type_Sonar;
	}else if(TypeStr == "�T�Ɠ�"){
		return Type_SLight;
	}else if(TypeStr == "�Ɩ��e"){
		return Type_LightB;
	}else{
		return Type_Other;
	}
}

/* ������ */
int ToInt(const string String){
	stringstream ss;
	ss << String;
	int Int;
	ss >> Int;
	return Int;
}

/* �˒� */
RANGE ToRange(const string RangeStr){
	if(RangeStr == "��"){
		return NoneRange;
	}else if(RangeStr == "�Z"){
		return ShortRange;
	}else if(RangeStr == "��"){
		return MiddleRange;
	}else if(RangeStr == "��"){
		return LongRange;
	}else if(RangeStr == "����"){
		return VeryLongRange;
	}else{
		throw "���̎˒��͑��݂��܂���.";
	}
}

/* �͎� */
SC ToKind(const string KindStr){
	if (KindStr == "���"){
		return SC_BB;
	}else if (KindStr == "�q����"){
		return SC_BBV;
	}else if (KindStr == "���K���"){
		return SC_CV;
	}else if (KindStr == "���b���"){
		return SC_ACV;
	}else if (KindStr == "�y���"){
		return SC_CVL;
	}else if (KindStr == "����@���"){
		return SC_AV;
	}else if (KindStr == "�d���m��"){
		return SC_CA;
	}else if (KindStr == "�q�󏄗m��"){
		return SC_CAV;
	}else if (KindStr == "�y���m��"){
		return SC_CL;
	}else if (KindStr == "�d�������m��"){
		return SC_CLT;
	}else if (KindStr == "�쒀��"){
		return SC_DD;
	}else if (KindStr == "������"){
		return SC_SS;
	}else if (KindStr == "�������"){
		return SC_SSV;
	}else{
		return SC_Other;
	}
}

/* ���� */
SPEED ToSpeed(const string SpeedStr){
	if (SpeedStr == "�ᑬ"){
		return LowSpeed;
	}
	else if (SpeedStr == "����"){
		return HighSpeed;
	}else{
		throw "���̑��͂͑��݂��܂���.";
	}
}
