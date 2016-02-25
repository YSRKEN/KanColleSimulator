/* ---------------------------- */
/*  �͂���V�~�����[�^ Ver.0.2  */
/* ---------------------------- */

/*
   �ŏI�X�V�����F2014/12/04 03:17
   ����ҁF��RoVKHyEYUtVB
   �R�}���h���C�������F
     �uKanColleSim ���s�� �͑��f�[�^(��U) �͑��f�[�^(��U) �w�`(��U) �w�`(��U)�v
     �E���s�񐔁c�c�V�~�����[�g�����(���R��)�B�u1�v���w�肷��ƁA�퓬�̓r���o�߂��\������邪�A
                   2�ȏ���w�肷��Ɠr���o�ߕ\���������Ȃ�A����ɕ�����̃V�~�����[�g�ɂ�����
                   �͖��̎c��ϋv(����)�ƁA�퓬����(���S����S�`�s�kD)�̊������\�������B
     �E�͑��f�[�^�c�c�T���v����fleet1.txt��fleet2.txt�Ɏ������B��U�͂��̖��̒ʂ�A
                     ����ɐ�ɍU��������ɂȂ�B�d�l��A��U�̕����L���ł͂���B
     �E�w�`�c�c�P�c�w�E���c�w�E�֌`�w�E��`�w�E�P���w����1�I���B
     �Ȃ��A�R�}���h���C��������5�Ƃ��S�Ďw�肷��K�v������B
   �Ăяo����F
     �uKanColleSim 10000 fleet1.txt fleet2.txt �P�c�w �P�c�w�v
   ���ӁF
   �Ekammusu.txt��weapon.txt�͖���K���ǂݍ��܂��̂Œ���
   �E���͂ɃG���[������ꍇ���̎|���G���[���b�Z�[�W�Ƃ��Ēm�点��
   �E�ܘ_kammusu.txt��weapon.txt�A����ъ͑��f�[�^�͎��R�ɕҏW�ł��邪
   ���ꂮ������@�~�X���Ȃ��悤�ɁI
*/

#include "header.h"
#include <chrono>

int main(const int argc, const char *argv[]) {
	/* �����ݒ� */
	try{
		if(argc < 5) throw "�������s���ł�.";
		// ���s��
		stringstream sin;
		sin << argv[1];
		int BattleCount;
		sin >> BattleCount;
		if(BattleCount < 1) throw "���s�񐔂͎��R���̂ݎ�邱�Ƃ��ł��܂�.";
		// �͑��f�[�^
		fleets FleetsData[BattleSize];
		for(int i = 0; i < BattleSize; ++i){
			string FleetsDataName(argv[i + 2]);
			FleetsData[i].ReadData(FleetsDataName);
		}
		// �w�`
		for(int i = 0; i < BattleSize; ++i){
			string FormationString(argv[i + 4]);
			if(FormationString == "�P�c�w"){
				FleetsData[i].Formation = FOR_TRAIL;
			}else if(FormationString == "���c�w"){
				FleetsData[i].Formation = FOR_SUBTRAIL;
			}else if(FormationString == "�֌`�w"){
				FleetsData[i].Formation = FOR_CIRCLE;
			}else if(FormationString == "��`�w"){
				FleetsData[i].Formation = FOR_ECHELON;
			}else if(FormationString == "�P���w"){
				FleetsData[i].Formation = FOR_ABREAST;
			}else{
				throw "���̐w�`�͑��݂��܂���";
			}
		}
		/* ��1����2�͑��ւ̍U�� */
		if(BattleCount == 1) {
			// 1��̂�
			FleetsData[FriendSide].Simulate(FleetsData[EnemySide]);
		} else {
			// ������(���v���[�h)
			//����
			fleets FleetsData_[BattleSize];
			vector< vector<int> > HPSum(BattleSize);
			vector< vector<int> > HeavyDamageCount(BattleSize);
			for(int i = 0; i < BattleSize; ++i) {
				HPSum[i].resize(FleetsData[i].Members, 0);
				HeavyDamageCount[i].resize(FleetsData[i].Members, 0);
			}
			//�����񎎍s���ďW�v����
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
			//���ʂ�\������
			cout << "�y���ʕ\���z\n";
			for(int i = 0; i < BattleSize; ++i) {
				cout << "��" << Position[i] << "\n";
				for(int j = 0; j < FleetsData[i].Members; ++j) {
					FleetsData[i].Kammusues[j].HP = static_cast<int>(1.0 * HPSum[i][j] / BattleCount);
					cout << "�@" << FleetsData[i].Kammusues[j].Label() << " "
						<< 1.0 * HPSum[i][j] / BattleCount << "/" << FleetsData[i].Kammusues[j].MaxHP
						<< "(" << DMString[FleetsData[i].Kammusues[j].ShowDamage()] << ") ";
					cout << "��j���F" << (100.0 * HeavyDamageCount[i][j] / BattleCount) << "��\n";
				}
			}
			double WinPer = 100.0 * (WinReason[WIN_SS] + WinReason[WIN_S] + WinReason[WIN_A] + WinReason[WIN_B]) / BattleCount;
			cout << "����������̕��z(�����F" << WinPer << "��)\n";
			for(int i = WIN_Size - 1; i >= 0; --i) {
				cout << "�@" << WINString[i] << "�F" << WinReason[i] << "��(" << 100.0 * WinReason[i] / BattleCount << "��)\n";
			}
		}
	}
	catch(char *e){
		cout << "�G���[�F" << e << std::endl;
	}
}
/*
	//���� �ϐ���           ���O,                   ���,           ��, ��,��,��,��,��,��,��, �˒�,        ���b
	weapon Gun_127_Multi   {"12.7cm�A���C",         Type_Gun,        2,  0, 0, 2, 0, 0, 0, 0, ShortRange,  0};
	weapon Gun_140_Single  {"14cm�P���C",           Type_Gun,        2,  0, 0, 0, 0, 0, 1, 0, MiddleRange, 0};
	weapon Gun_203_Multi   {"20.3cm�A���C",         Type_Gun,        8,  0, 0, 3, 0, 0, 0, 0, MiddleRange, 0};
	weapon Gun_356_Multi   {"35.6cm�A���C",         Type_Gun,       15,  0, 0, 4, 0, 0, 0, 0, LongRange,   0};
	weapon Gun_152_Single  {"15.2cm�P���C",         Type_SubGun,     2,  0, 0, 0, 0, 0, 1, 0, MiddleRange, 0};
	weapon Torpedo_61_3    {"61cm�O�A������",       Type_Torpedo,    0,  5, 0, 0, 0, 0, 0, 0, ShortRange,  0};
	weapon Torpedo_61_4_Oxy{"61cm�l�A��(�_�f)����", Type_Torpedo,    0, 10, 0, 0, 0, 0, 0, 0, ShortRange,  0};
	weapon SpecialSS_Kou   {"�b�W�I �b",            Type_SpecialSS,  0, 12, 0, 0, 0, 0, 0, 0, NoneRange,   0};
	weapon WB_Zuiun        {"���_",                 Type_WB,         0,  0, 4, 2, 4, 6, 1, 0, NoneRange,   0};
	weapon WS_Zero         {"�뎮�����@�@",       Type_WS,         0,  0, 1, 1, 2, 5, 1, 0, NoneRange,   0};
	weapon PF_21           {"�뎮�͐�21�^",         Type_PF,         0,  0, 0, 5, 0, 0, 0, 0, NoneRange,   0};
	weapon PBF_62          {"�뎮�͐�62�^(����)",   Type_PBF,        0,  0, 4, 4, 3, 0, 0, 0, NoneRange,   0};
	weapon PB_99           {"��㎮�͔�",           Type_PB,         0,  0, 5, 0, 3, 0, 0, 0, NoneRange,   0};
	weapon PB_SuiSei       {"�a��",                 Type_PB,         0,  0, 8, 0, 3, 0, 0, 0, NoneRange,   0};
	weapon PA_97           {"�㎵���͍U",           Type_PA,         0,  5, 0, 0, 4, 1, 0, 0, NoneRange,   0};
	weapon PA_Tenzan       {"�V�R",                 Type_PA,         0,  7, 0, 0, 3, 1, 0, 0, NoneRange,   0};
	weapon AAG_77_Single   {"7.7mm�@�e",            Type_AAG,        0,  0, 0, 2, 0, 0, 0, 1, NoneRange,   0};
	//�͖�  �ϐ���              �͖�,     �͎�,   Lv, ��, ��, ��, ��, ��, ��, ��, ����,      ��, �˒�,        �^, �X,��1,��2,��3,��4
	kammusu Fubuki_Fubuki_0    ("����",   SC_DD,   1, 15, 10,  5, 27, 40, 10, 20, HighSpeed,  5, ShortRange,  17, 2);
	kammusu Kuma_Oi_1          ("����", SC_CLT, 10, 32,  8, 14, 80, 37, 13, 28, HighSpeed, 11, MiddleRange, 10, 2);
	kammusu Mogami_Mogami_1    ("�ŏ��", SC_CAV, 10, 50, 28, 39, 23, 38, 24,  0, HighSpeed, 25, MiddleRange, 10, 4,  5,  6,  5,  3);
	kammusu Akagi_Akagi_0      ("�ԏ�",   SC_CV,   1, 69,  0, 28,  0, 28, 32,  0, HighSpeed, 44, ShortRange,  12, 4, 18, 18, 27, 10);
	kammusu Ise_Ise_1          ("�ɐ���", SC_BBV, 10, 77, 63, 75,  0, 36, 45,  0, LowSpeed,  22, LongRange,   30, 4, 11, 11, 11, 14);
	kammusu E_E401_0           ("��401",  SC_SSV,  1, 20,  2,  5, 36, 13,  0,  0, LowSpeed,  15, ShortRange,  20, 1,  3);
	kammusu Kongoh_Kongoh_0    ("����",   SC_BB,   1, 63, 63, 52,  0, 30, 24,  0, HighSpeed, 13, LongRange,   12, 3,  3,  3,  3);
	kammusu Taihoh_Taihoh_0    ("��P",   SC_ACV,  1, 67,  0, 40,  0, 33, 42,  0, HighSpeed, 47, ShortRange,   2, 4, 18, 18, 18,  7);
	kammusu HohShoh_HohShoh_0  ("�P��",   SC_CVL,  1, 30,  0, 15,  0, 24, 10,  0, LowSpeed,  32, ShortRange,  20, 2,  8, 11);
	kammusu Chitose_Chitose_2  ("��΍b", SC_AV,  12, 42, 10, 22, 18, 29, 19,  0, HighSpeed, 34, ShortRange,  10, 3, 12,  6,  6);
	kammusu Hurutaka_Hurutaka_0("�Ñ�",   SC_CA,   1, 36, 30, 25, 12, 33, 16,  0, HighSpeed, 10, MiddleRange, 10, 3,  2,  2,  2);
	kammusu Tenryuu_Tenryuu_0  ("�V��",   SC_CL,   1, 23, 11,  7, 18, 35,  8, 18, HighSpeed,  7, MiddleRange, 17, 2);
	//�͑� �ϐ���
	fleets First_Fleets(10), Second_Fleets(10);
	//����
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
	//�Ґ�
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
