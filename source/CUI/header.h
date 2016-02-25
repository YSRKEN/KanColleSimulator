/* ���C�u���� */
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
/* ���̑��v���v���Z�b�T */
#define USE_NEW_SEARCH_METHOD	//�R�����g�A�E�g����ƁA���G�v�Z����(�H)����(��)�ɕς��

/* using�錾 */
using std::cout;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;

/* �萔�錾 */
// �͎�
enum SC{
	SC_BB,		//���
	SC_BBV,		//�q����
	SC_CV,		//���K���
	SC_ACV,		//���b���
	SC_CVL,		//�y���
	SC_AV,		//����@���
	SC_CA,		//�d���m��
	SC_CAV,		//�q�󏄗m��
	SC_CL,		//�y���m��
	SC_CLT,		//�d�������m��
	SC_DD,		//�쒀��
	SC_SS,		//������
	SC_SSV,		//�������
	SC_Other	//���̑�(�g���͂Ƃ��H��͂Ƃ�������͂Ƃ��͂ǂ������̂�H)
};
// ���
// �͒�̍ʉ_�E����̖�オ���ʈ����Ȃ̂ɒ���
enum TYPE{
	Type_None,		//����
	Type_Gun,		//��C(�O�b�e�܂�)
	Type_SubGun,	//���C
	Type_Torpedo,	//����
	Type_SpecialSS,	//������q��(�v����ɍb�W�I)
	Type_PF,		//�͐�
	Type_PBF,		//����
	Type_PB,		//�͔�
	Type_WB,		//����
	Type_PA,		//�͍U
	Type_PS,		//�͒�(�ʉ_�ȊO�B�v����ɓ񎮊͏��@�@)
	Type_PSS,		//�͒�(�ʉ_)
	Type_WS,		//����
	Type_WSN,		//���(�v����ɋ㔪�������@�@)
	Type_ASPP,		//�ΐ������@(�v����ɎO���w���A���@)
	Type_AJ,		//�I�[�g�W���C��(�v����ɃJ���ϑ��@)
	Type_SmallS,	//���^�d�T
	Type_LargeS,	//��^�d�T
	Type_AAG,		//�΋�@�e(�O���e�܂�)
	Type_AAD,		//���ˑ��u
	Type_DP,		//����
	Type_Sonar,		//�\�i�[
	Type_SLight,	//�T�Ɠ�
	Type_LightB,	//�Ɩ��e
	Type_Other		//���̑�(�唭�Ƃ�)
};
//�w�`(�P�c�w�E���c�w�E�֌`�w�E��`�w�E�P���w)
enum FORMATION {FOR_TRAIL, FOR_SUBTRAIL, FOR_CIRCLE, FOR_ECHELON, FOR_ABREAST};
const string FORString[] = {"�P�c�w", "���c�w", "�֌`�w", "��`�w", "�P���w"};
const double AntiAirBonusPer[] = {1.0, 1.2, 1.6, 1.0, 1.0};
//���`��(���q��E���q��ET���L���ET���s��) BP��BattlePosition�̈�
enum BP{BP_SAME, BP_DIFF, BP_T_PLUS, BP_T_MINUS};
const string BPString[] = {"���q��", "���q��", "T���L��", "T���s��"};
//���Q��(�����E���݁E���j�E���j�E��j�E����)
enum DAMAGE {NoDamage, VeryLightDamage, LightDamage, MiddleDamage, HeavyDamage, Lost};
const string DMString[] = {"����", "����", "���j", "���j", "��j", "����"};
//����(���󌠑r���E�q��򐨁E�q��ύt�E�q��D���E���󌠊m��)
enum AIR_MAS {AM_WORST, AM_BAD, AM_NORMAL, AM_GOOD, AM_BEST};
const string AMString[] = {"���󌠑r��", "�q���", "�q��ύt", "�q��D��", "���󌠊m��"};
//��(�q���E�J�������E�C����E������E���)
enum TURN {TURN_AIR, TURN_TOR_FIRST, TURN_GUN, TURN_TOR, TURN_NIGHT};
//��J���(�Ԕ�J�E���J�E���ʁE�L���L��)
enum COND {RedFatigue, OrangeFatigue, Normal, Happy};
//�U�����@(�����J�b�g�C���E��C�J�b�g�C���E�A���E�ʏ�)
enum AT {CutinAttackT, CutinAttackG, DoubleAttack, NormalAttack};
//��������(�s�kD�E��p�I�s�kC�E��p�I����B�E����A�E����S�E���S����S)
enum WIN {WIN_D, WIN_C, WIN_B, WIN_A, WIN_S, WIN_SS, WIN_Size};
const string WINString[] = {"�s�kD", "��p�I�s�kC", "��p�I����B", "����A", "����S", "���S����S"};
//���̑�
enum SPEED {LowSpeed, HighSpeed};	//����
enum RANGE {NoneRange, ShortRange, MiddleRange, LongRange, VeryLongRange};	//�˒�
enum {FriendSide, EnemySide};
const string Position[] = {"���R", "�G�R"};
const int BattleSize = 2;			//�퓬���s���͎̂��R�ƓG�R��2�͑��̂�
const double AttackPlus[] = {1.12, 1.12, 1.17, 1.20};	//�G�ڂɂ��U���͕␳
const int MaxKanmusu = 6;			//�͑����ۗL�ł���ő�͖���

/* �N���X�錾 */
struct weapon {
	/* �����o�ϐ� */
	string Name;	//���O
	TYPE Type;		//���
	int Attack;		//�Η�
	int Torpedo;	//����
	int Bomb;		//����
	int AntiAir;	//�΋�
	int AntiSub;	//�ΐ�
	int Search;		//���G
	int Hit;		//����
	int Evade;		//���
	RANGE Range;	//�˒�
	int Defense;	//���b
	/* �����o�֐� */
	weapon();			//�R���X�g���N�^
	bool isAir();		//�͍ڋ@���𔻒�
	bool isAirWar();	//�q���ɎQ���ł��邩�𔻒�
	bool isAirPS();		//�͏��@�@���𔻒�
	bool isAirWar2();	//�΋�C�΂Ō������Ƃ��邩�𔻒�
	bool isAntiSub();	//�ΐ��U���\�ȑ������𔻒�
	bool isAntiSub1();	//�ΐ��U���\�ȑ������𔻒�(�q��E�q���E����)
	bool isAntiSub2();	//�ΐ��U���\�ȑ������𔻒�(�y���)
	bool isAntiSub3();	//�ΐ��U���\�ȑ������𔻒�(�����n)
};
struct kammusu {
	/* �����o�ϐ� */
	string Name;	//�͖�
	SC Kind;		//�͎�
	int Level;		//���x��
	int MaxHP;		//�ϋv
	int Attack;		//�Η�
	int Defense;	//���b
	int Torpedo;	//����
	int Evade;		//���
	int AntiAir;	//�΋�
	int AntiSub;	//�ΐ�
	SPEED Speed;	//����
	int Search;		//���G
	RANGE Range;	//�˒�
	int Luck;		//�^
	int Slots;		//�����X���b�g��
	vector<weapon> Weapons;	//����
	vector<int> MaxAirs;	//�͍ڋ@��
	int HP;					//���݂̑ϋv
	int cond;				//���݂�cond�l
	int Ammo;				//���݂̎c�e��ʊ���
	vector<int> Airs;		//���݂͍̊ڋ@��
	/* �����o�֐� */
	string ShowHP();					//�ϋv����Ԃ�
	string Label(const int pos = -1);	//�ȈՓI�Ȍ�������Ԃ�
	COND ShowCond();					//��J��Ԃ�Ԃ�
	DAMAGE ShowDamage();				//���Q�󋵂�����
	int AllAntiAir();					//���΋�l��Ԃ�
	int AllEvade();						//�������Ԃ�
	int AllHit();						//��������Ԃ�
	int AllTorpedo();					//��������Ԃ�
	int AllAntiSub();					//���ΐ��\�͂�Ԃ�
	int AllAttack();					//���U���͂�Ԃ�
	int AllAttackInNight();				//���Η͂�Ԃ�
	double NonFit();					//�t�B�b�g���Ȃ��C�ɂ�閽�����̋t�␳
	RANGE MaxRange();					//�ő�˒���Ԃ�
	void ShowAttackType(vector<int>&);				//�����\�Ȓe���ϑ��ˌ��̎�ނ�Ԃ�
	AT ShowAttackTypeInNight(int&, double&, bool&);	//��ԓ���U���̎�ށE����є{����Ԃ�
	bool isSubmarine();				//�����͌n���𔻒�
	bool isFirstTorpedo();			//�J�������\���𔻒�
	bool isMoveInGun();				//����ōU���\���𔻒�
	bool isSearchAir();				//��@�@�������Ă��邩�𔻒�
	bool isAntiSub();				//�ΐ��U���\���𔻒�
	bool isTorpedo();				//�����\���𔻒�
	bool isMoveInNight();			//���\���𔻒�
	bool isAntiSubInNight();		//��펞�ɑΐ��U���\���𔻒�
};
struct fleets{
	/* �����o�ϐ� */
	vector<kammusu> Kammusues;
	int HQLevel;			//�i�ߕ����x��(���G�Ɋ֌W����Ƃ��}�W�Ȃ̂���c�c�H)
	int Members;			//��������͖��̐�
	FORMATION Formation;	//�w�`
	/* �����o�֐� */
	//�V�~�����[�g�֌W(���C���ƂȂ�֐�)
	WIN Simulate(fleets&, const bool isShow = true);	//�퓬���V�~�����[�g����
	//�V�~�����[�g�֌W(�⏕�ƂȂ�֐�)
	double CalcSearchPower();			//���G�l���v�Z����
	bool hasSaiun();					//�ʉ_�����邩�ǂ����𔻒肷��
	bool hasLight();					//�T�Ɠ���Ɩ��e�������Ă��邩�𔻒肷��
	int RandomKammsu();					//�����c���Ă�͖����烉���_���ɑI������
	int RandomKammsuWithSS();			//�����c���Ă�͖�(�����͌n)���烉���_���ɑI������
	int RandomKammsuWithoutSS();		//�����c���Ă�͖�(����@�n)���烉���_���ɑI������
	int hasAlived();					//�����͂̐����J�E���g����
	double ResultsGauge();				//��ʃQ�[�W�ʂ��v�Z����
	//���̑�
	void SetKammusu(const kammusu&);	//�͖����Z�b�g����
	void Reset();						//��Ԃ����Z�b�g����
	void ShowList(const bool isShow = true);	//�͑��Ɋւ������\������
	void ReadData(const string);		//�t�@�C������ǂݍ���
};
