/* ライブラリ */
#include <cmath>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <random>
/* その他プリプロセッサ */
#define USE_NEW_SEARCH_METHOD	//コメントアウトすると、索敵計算式が(秋)から(旧)に変わる

/* using宣言 */
using std::cout;
using std::fstream;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;
using std::tuple;
using std::get;

/* 定数宣言 */
// 艦種
enum SC{
	SC_BB,		//戦艦
	SC_BBV,		//航空戦艦
	SC_CV,		//正規空母
	SC_ACV,		//装甲空母
	SC_CVL,		//軽空母
	SC_AV,		//水上機母艦
	SC_CA,		//重巡洋艦
	SC_CAV,		//航空巡洋艦
	SC_CL,		//軽巡洋艦
	SC_CLT,		//重雷装巡洋艦
	SC_DD,		//駆逐艦
	SC_SS,		//潜水艦
	SC_SSV,		//潜水空母
	SC_LST,		//輸送艦
	SC_AF,		//陸上棲姫
	SC_FT,		//護衛要塞
	SC_CP,		//練習巡洋艦
	SC_Other	//その他(揚陸艦とか工作艦とか潜水母艦とかはどう扱うのん？)
};
// 種別
// 艦偵の彩雲・水偵の夜偵が特別扱いなのに注意
enum TYPE{
	Type_None,		//無し
	Type_Gun,		//主砲(徹甲弾含む)
	Type_SubGun,	//副砲
	Type_Torpedo,	//魚雷
	Type_SpecialSS,	//特殊潜航艇(要するに甲標的)
	Type_PF,		//艦戦
	Type_PBF,		//爆戦
	Type_PB,		//艦爆
	Type_WB,		//水爆
	Type_PA,		//艦攻
	Type_PS,		//艦偵(彩雲以外。要するに二式艦上偵察機)
	Type_PSS,		//艦偵(彩雲)
	Type_WS,		//水偵
	Type_WSN,		//夜偵(要するに九八式水上偵察機)
	Type_ASPP,		//対潜哨戒機(要するに三式指揮連絡機)
	Type_AJ,		//オートジャイロ(要するにカ号観測機)
	Type_SmallS,	//小型電探
	Type_LargeS,	//大型電探
	Type_AAG,		//対空機銃(三式弾含む)
	Type_AAD,		//高射装置
	Type_DP,		//爆雷
	Type_Sonar,		//ソナー
	Type_SLight,	//探照灯
	Type_LightB,	//照明弾
	Type_Other		//その他(大発とか)
};
//陣形(単縦陣・複縦陣・輪形陣・梯形陣・単横陣)
enum FORMATION {FOR_TRAIL, FOR_SUBTRAIL, FOR_CIRCLE, FOR_ECHELON, FOR_ABREAST};
const string FORString[] = {"単縦陣", "複縦陣", "輪形陣", "梯形陣", "単横陣"};
const double AntiAirBonusPer[] = {1.0, 1.2, 1.6, 1.0, 1.0};
//交戦形態(同航戦・反航戦・T字有利・T字不利) BPはBattlePositionの意
enum BP{BP_SAME, BP_DIFF, BP_T_PLUS, BP_T_MINUS};
const string BPString[] = {"同航戦", "反航戦", "T字有利", "T字不利"};
//損害状況(無傷・健在・小破・中破・大破・撃沈)
enum DAMAGE {NoDamage, VeryLightDamage, LightDamage, MiddleDamage, HeavyDamage, Lost};
const string DMString[] = {"無傷", "健在", "小破", "中破", "大破", "撃沈"};
//制空権(制空権喪失・航空劣勢・航空均衡・航空優勢・制空権確保)
enum AIR_MAS {AM_WORST, AM_BAD, AM_NORMAL, AM_GOOD, AM_BEST};
const string AMString[] = {"制空権喪失", "航空劣勢", "航空均衡", "航空優勢", "制空権確保"};
//状況(航空戦・開幕雷撃・砲撃戦・雷撃戦・夜戦)
enum TURN {TURN_AIR, TURN_TOR_FIRST, TURN_GUN, TURN_TOR, TURN_NIGHT};
//状況2(砲撃戦・対潜・雷撃戦・夜戦)
enum kTurn2 {kTurn2Gun, kTurn2AntiSub, kTurn2Torpedo, kTurn2Night};
//疲労状態(赤疲労・橙疲労・普通・キラキラ)
enum COND {RedFatigue, OrangeFatigue, Normal, Happy};
//攻撃方法(魚雷カットイン・主魚カットイン・主砲カットイン・連撃・通常)
enum AT {CutinAttackT, CutinAttackGT, CutinAttackG, DoubleAttack, NormalAttack};
//勝利判定(敗北D・戦術的敗北C・戦術的勝利B・勝利A・勝利S・完全勝利S)
enum WIN {WIN_D, WIN_C, WIN_B, WIN_A, WIN_S, WIN_SS, WIN_Size};
const string WINString[] = {"敗北D", "戦術的敗北C", "戦術的勝利B", "勝利A", "勝利S", "完全勝利S"};
//その他
enum SPEED {LowSpeed, HighSpeed};	//速力
enum RANGE {NoneRange, ShortRange, MiddleRange, LongRange, VeryLongRange};	//射程
enum kSimulateMode {kModeDN, kModeD, kModeN};	//戦闘モード(昼戦＋夜戦、昼戦のみ、開幕夜戦)
enum {FriendSide, EnemySide};
const string Position[] = {"自軍", "敵軍"};
const int BattleSize = 2;			//戦闘を行うのは自軍と敵軍の2艦隊のみ
const double AttackPlus[] = {1.12, 1.12, 1.17, 1.20};	//触接による攻撃力補正
const int MaxKanmusu = 6;			//艦隊が保有できる最大艦娘数

/* クラス宣言 */
struct weapon {
	/* メンバ変数 */
	string Name;	//名前
	TYPE Type;		//種別
	int Attack;		//火力
	int Torpedo;	//雷装
	int Bomb;		//爆装
	int AntiAir;	//対空
	int AntiSub;	//対潜
	int Search;		//索敵
	int Hit;		//命中
	int Evade;		//回避
	RANGE Range;	//射程
	int Defense;	//装甲
	int level_;		//装備改修度(0-10)・艦載機熟練度(0-7)
	/* メンバ関数 */
	weapon();			//コンストラクタ
	bool isAir();		//艦載機かを判定
	bool isAirWar();	//航空戦に参加できるかを判定
	bool isAirPS();		//艦上偵察機かを判定
	bool isAirWar2();	//対空砲火で撃ち落とせるかを判定
	bool isAntiSub();	//対潜攻撃可能な装備かを判定
	bool isAntiSub1();	//対潜攻撃可能な装備かを判定(航戦・航巡・水母)
	bool isAntiSub2();	//対潜攻撃可能な装備かを判定(軽空母)
	bool isAntiSub3();	//対潜攻撃可能な装備かを判定(水雷系)
};
struct kammusu {
	/* メンバ変数 */
	string Name;	//艦名
	SC Kind;		//艦種
	int Level;		//レベル
	int MaxHP;		//耐久
	int Attack;		//火力
	int Defense;	//装甲
	int Torpedo;	//雷装
	int Evade;		//回避
	int AntiAir;	//対空
	int AntiSub;	//対潜
	SPEED Speed;	//速力
	int Search;		//索敵
	RANGE Range;	//射程
	int Luck;		//運
	int Slots;		//装備スロット数
	vector<weapon> Weapons;	//装備
	vector<int> MaxAirs;	//艦載機数
	int HP;					//現在の耐久
	int cond;				//現在のcond値
	int Ammo;				//現在の残弾薬量割合
	vector<int> Airs;		//現在の艦載機数
	bool is_kammusu_;		//艦娘ならtrue、深海棲艦ならfalse
	/* メンバ関数 */
	string ShowHP();					//耐久情報を返す
	string Label(const int pos = -1);	//簡易的な肩書きを返す
	COND ShowCond();					//疲労状態を返す
	DAMAGE ShowDamage();				//損害状況を示す
	int AllAntiAir();					//総対空値を返す
	int AllEvade();						//総回避を返す
	int AllHit();						//総命中を返す
	int AllTorpedo(const bool is_torpedo_phase = true);	//総雷装を返す
	int AllAntiSub();					//総対潜能力を返す
	int AllAttack();					//総攻撃力を返す
	int AllAttackInNight();				//夜戦火力を返す
	double NonFitBB();					//戦艦にフィットしない砲による命中率の逆補正
	double FitCL();						//軽巡系にフィットする砲による威力の補正
	RANGE MaxRange();					//最大射程を返す
	void ShowAttackType(vector<int>&);	//発動可能な弾着観測射撃の種類を返す
	AT ShowAttackTypeInNight(int&, double&, bool&);	//夜間特殊攻撃の種類・および倍率を返す
	bool isSubmarine();				//潜水艦系かを判定
	bool isFirstTorpedo();			//開幕雷撃可能かを判定
	bool isMoveInGun();				//昼戦で攻撃可能かを判定
	bool isSearchAir();				//偵察機を持っているかを判定
	bool isAntiSub();				//対潜攻撃可能かを判定
	bool isTorpedo();				//雷撃可能かを判定
	bool isMoveInNight();			//夜戦可能かを判定
	bool isAntiSubInNight();		//夜戦時に対潜攻撃可能かを判定
	bool hasBomb();					//艦爆を持っているかを判定
	bool hasWatch();				//熟練見張員を所持しているかを判定
	void changeCond(const int);		//cond値を修正する
};
struct fleets{
	/* メンバ変数 */
	vector<kammusu> Kammusues;
	int HQLevel;			//司令部レベル(索敵に関係あるとかマジなのかよ……？)
	int Members;			//所属する艦娘の数
	FORMATION Formation;	//陣形
	/* メンバ関数 */
	//シミュレート関係(メインとなる関数)
	tuple<WIN, size_t> Simulate(fleets&, const bool, const kSimulateMode);	//戦闘をシミュレートする
	//シミュレート関係(補助となる関数)
	double CalcSearchPower();			//索敵値を計算する
	bool hasSaiun();					//彩雲がいるかどうかを判定する
	bool hasLight();					//探照灯や照明弾を持っているかを判定する
	bool hasHeavyDamage();				//大破以上の艦が存在するかを判定する
	int RandomKammsu();					//生き残ってる艦娘からランダムに選択する
	int RandomKammsuWithSS();			//生き残ってる艦娘(潜水艦系)からランダムに選択する
	int RandomKammsuWithoutSS(const bool, const bool is_night = false);	//生き残ってる艦娘(水上機系)からランダムに選択する
	int hasAlived();					//生存艦の数をカウントする
	double ResultsGauge();				//戦果ゲージ量を計算する
	tuple<int, size_t> findSearchLight(const vector<int>&);	//探照灯を持っている艦の位置を調べる(外れなら-1)
	tuple<int, size_t> findSearchLargeLight(const vector<int>&);	//大型探照灯を持っている艦の位置を調べる(外れなら-1)
	//その他
	void SetKammusu(const kammusu&);	//艦娘をセットする
	void Reset();						//状態をリセットする
	void ShowList(const bool isShow = true);	//艦隊に関する情報を表示する
	void ReadData(const string);		//ファイルから読み込む
};

/* プロトタイプ宣言 */
/* 乱数の定義 */
extern std::random_device rd;
extern std::mt19937 mt;
extern std::uniform_real_distribution<> Rand;

extern int RandInt(const int);
extern void ReadMapData(vector<vector<fleets>>&, vector<vector<kSimulateMode>>&, const string);
extern vector<weapon> ReadWeaponData();
extern vector<kammusu> ReadKammusuData();
extern vector<string> split(const string);
