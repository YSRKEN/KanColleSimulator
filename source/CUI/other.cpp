#include "header.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<> Rand(0.0, 1.0);

/* [0, N - 1]な整数乱数を生成する */
int RandInt(const int n) {
	return static_cast<int>(Rand(mt) * n);
}

/* 装備の種類 */
TYPE ToType(const string TypeStr) {
	if (TypeStr == "無し") {
		return Type_None;
	}
	else if (TypeStr == "主砲") {
		return Type_Gun;
	}
	else if (TypeStr == "副砲") {
		return Type_SubGun;
	}
	else if (TypeStr == "魚雷") {
		return Type_Torpedo;
	}
	else if (TypeStr == "特殊潜航艇") {
		return Type_SpecialSS;
	}
	else if (TypeStr == "艦上戦闘機") {
		return Type_PF;
	}
	else if (TypeStr == "艦上爆撃機(爆戦)") {
		return Type_PBF;
	}
	else if (TypeStr == "艦上爆撃機") {
		return Type_PB;
	}
	else if (TypeStr == "水上爆撃機") {
		return Type_WB;
	}
	else if (TypeStr == "艦上攻撃機") {
		return Type_PA;
	}
	else if (TypeStr == "艦上偵察機") {
		return Type_PS;
	}
	else if (TypeStr == "艦上偵察機(彩雲)") {
		return Type_PSS;
	}
	else if (TypeStr == "水上偵察機") {
		return Type_WS;
	}
	else if (TypeStr == "夜間偵察機") {
		return Type_WSN;
	}
	else if (TypeStr == "対潜哨戒機") {
		return Type_ASPP;
	}
	else if (TypeStr == "オートジャイロ") {
		return Type_AJ;
	}
	else if (TypeStr == "小型電探") {
		return Type_SmallS;
	}
	else if (TypeStr == "大型電探") {
		return Type_LargeS;
	}
	else if (TypeStr == "対空機銃") {
		return Type_AAG;
	}
	else if (TypeStr == "高射装置") {
		return Type_AAD;
	}
	else if (TypeStr == "爆雷") {
		return Type_DP;
	}
	else if (TypeStr == "ソナー") {
		return Type_Sonar;
	}
	else if (TypeStr == "探照灯") {
		return Type_SLight;
	}
	else if (TypeStr == "照明弾") {
		return Type_LightB;
	}
	else {
		return Type_Other;
	}
}

/* 整数化 */
int ToInt(const string String) {
	stringstream ss;
	ss << String;
	int Int;
	ss >> Int;
	return Int;
}

/* 射程 */
RANGE ToRange(const string RangeStr) {
	if (RangeStr == "無") {
		return NoneRange;
	}
	else if (RangeStr == "短") {
		return ShortRange;
	}
	else if (RangeStr == "中") {
		return MiddleRange;
	}
	else if (RangeStr == "長") {
		return LongRange;
	}
	else if (RangeStr == "超長") {
		return VeryLongRange;
	}
	else {
		throw "その射程は存在しません.";
	}
}

/* 艦種 */
SC ToKind(const string KindStr) {
	if (KindStr == "戦艦") {
		return SC_BB;
	}
	else if (KindStr == "航空戦艦") {
		return SC_BBV;
	}
	else if (KindStr == "正規空母") {
		return SC_CV;
	}
	else if (KindStr == "装甲空母") {
		return SC_ACV;
	}
	else if (KindStr == "軽空母") {
		return SC_CVL;
	}
	else if (KindStr == "水上機母艦") {
		return SC_AV;
	}
	else if (KindStr == "重巡洋艦") {
		return SC_CA;
	}
	else if (KindStr == "航空巡洋艦") {
		return SC_CAV;
	}
	else if (KindStr == "軽巡洋艦") {
		return SC_CL;
	}
	else if (KindStr == "重雷装巡洋艦") {
		return SC_CLT;
	}
	else if (KindStr == "駆逐艦") {
		return SC_DD;
	}
	else if (KindStr == "潜水艦") {
		return SC_SS;
	}
	else if (KindStr == "潜水空母") {
		return SC_SSV;
	}
	else if (KindStr == "輸送艦") {
		return SC_LST;
	}
	else if (KindStr == "陸上棲姫") {
		return SC_AF;
	}
	else if (KindStr == "護衛要塞") {
		return SC_FT;
	}
	else if (KindStr == "練習巡洋艦") {
		return SC_CP;
	}
	else {
		return SC_Other;
	}
}

/* 速力 */
SPEED ToSpeed(const string SpeedStr) {
	if (SpeedStr == "低速") {
		return LowSpeed;
	}
	else if (SpeedStr == "高速") {
		return HighSpeed;
	}
	else {
		throw "その速力は存在しません.";
	}
}

vector<weapon> ReadWeaponData() {
	vector<weapon> WeaponList;
	std::locale::global(std::locale("japanese"));
	fstream fin1("weapon.csv");
	if (!fin1.is_open()) throw "weapon.txtが正常に読み込めませんでした.";
	string GetLine;
	getline(fin1, GetLine);
	while (getline(fin1, GetLine)) {
		//CSVを解析
		vector<string> WeaponData;
		string temp;
		stringstream sin(GetLine);
		while (getline(sin, temp, ',')) {
			WeaponData.push_back(temp);
		}
		if (WeaponData.size() < 13) throw "装備データの項目数が異常です.";
		//装備データに変換する
		weapon Weapon;
		Weapon.Name = WeaponData[1];
		Weapon.Type = ToType(WeaponData[2]);
		Weapon.Attack = ToInt(WeaponData[3]);
		Weapon.Torpedo = ToInt(WeaponData[4]);
		Weapon.Bomb = ToInt(WeaponData[5]);
		Weapon.AntiAir = ToInt(WeaponData[6]);
		Weapon.AntiSub = ToInt(WeaponData[7]);
		Weapon.Search = ToInt(WeaponData[8]);
		Weapon.Hit = ToInt(WeaponData[9]);
		Weapon.Evade = ToInt(WeaponData[10]);
		Weapon.Range = ToRange(WeaponData[11]);
		Weapon.Defense = ToInt(WeaponData[12]);
		Weapon.level_ = 0;
		//リストに追加する
		WeaponList.push_back(Weapon);
	}
	fin1.close();
	return WeaponList;
}

vector<kammusu> ReadKammusuData() {
	vector<kammusu> KammusuList;
	std::locale::global(std::locale("japanese"));
	fstream fin2("kammusu.csv");
	if (!fin2.is_open()) throw "kammusu.txtが正常に読み込めませんでした.";
	string GetLine;
	getline(fin2, GetLine);
	while (getline(fin2, GetLine)) {
		//CSVを解析
		vector<string> KammusuData;
		string temp;
		stringstream sin(GetLine);
		while (getline(sin, temp, ',')) {
			KammusuData.push_back(temp);
		}
		KammusuData.push_back(temp);
		if (KammusuData.size() < 16) throw "艦娘データの項目数が異常です.";
		//艦娘データに変換する
		kammusu Kammusu;
		Kammusu.Name = KammusuData[1];
		Kammusu.Kind = ToKind(KammusuData[2]);
		Kammusu.Level = ToInt(KammusuData[3]);
		Kammusu.MaxHP = ToInt(KammusuData[4]);
		Kammusu.Attack = ToInt(KammusuData[5]);
		Kammusu.Defense = ToInt(KammusuData[6]);
		Kammusu.Torpedo = ToInt(KammusuData[7]);
		Kammusu.Evade = ToInt(KammusuData[8]);
		Kammusu.AntiAir = ToInt(KammusuData[9]);
		Kammusu.AntiSub = ToInt(KammusuData[10]);
		Kammusu.Speed = ToSpeed(KammusuData[11]);
		Kammusu.Search = ToInt(KammusuData[12]);
		Kammusu.Range = ToRange(KammusuData[13]);
		Kammusu.Luck = ToInt(KammusuData[14]);
		Kammusu.Slots = ToInt(KammusuData[15]);
		Kammusu.Weapons.resize(Kammusu.Slots);
		Kammusu.MaxAirs.resize(Kammusu.Slots);
		weapon Weapon_None;
		for (int i = 0; i < Kammusu.Slots; ++i) {
			Kammusu.Weapons[i] = Weapon_None;
			Kammusu.MaxAirs[i] = ToInt(KammusuData[16 + i]);
		}
		if (KammusuData[KammusuData.size() - 1] == "○") {
			Kammusu.is_kammusu_ = true;
		}else {
			Kammusu.is_kammusu_ = false;
		}
		Kammusu.HP = Kammusu.MaxHP;
		Kammusu.cond = 49;
		Kammusu.Ammo = 100;
		Kammusu.Airs = Kammusu.MaxAirs;
		//リストに追加する
		KammusuList.push_back(Kammusu);
	}
	fin2.close();
	return KammusuList;
}

void ReadMapData(vector<vector<fleets>> &MapData, vector<vector<kSimulateMode>> &MapSim, const string Filename) {
	std::locale::global(std::locale("japanese"));

	/* weapon.txtから装備データを読み込む */
	vector<weapon> WeaponList = ReadWeaponData();

	/* kammusu.txtから艦娘データを読み込む */
	vector<kammusu> KammusuList = ReadKammusuData();

	fstream fin3(Filename);
	string GetLine;
	int Step = 0, all_patterns = 0, all_kammusues = 0;
	int idx1 = 0, idx2 = 0, idx3 = 0, idx4 = 0, idx5 = 0;
	while (getline(fin3, GetLine)) {
		// 空行及び#から始まる行(コメント行)は無視する
		if (GetLine == "") continue;
		if (GetLine.substr(0, 1) == "#") continue;
		if (Step == 0) {
			// マスの数
			MapData.resize(ToInt(GetLine));
			MapSim.resize(ToInt(GetLine));
			++Step;
		}else if (Step == 1) {
			// マスごとのパターン数
			string temp;
			stringstream sin(GetLine);
			rsize_t count = 0;
			while (getline(sin, temp, ',')) {
				int Number = ToInt(temp);
				if (Number <= 0) throw "パターン数指定が異常です.";
				MapData[count].resize(Number);
				MapSim[count].resize(Number);
				all_patterns += Number;
				++count;
			}
			++Step;
		}else if (Step >= 2 &&Step < 2 + all_patterns) {
			// パターンごとの艦娘(深海棲艦)
			string temp;
			stringstream sin(GetLine);
			vector<string> temp2;
			while (getline(sin, temp, ',')) {
				temp2.push_back(temp);
			}
			int members = temp2.size() - 1;
			// 艦娘
			for (rsize_t i = 0; i < static_cast<rsize_t>(members); ++i) {
				MapData[idx1][idx2].SetKammusu(KammusuList[ToInt(temp2[i]) - 1]);
			}
			string form_str = temp2[members].substr(0, 6), sim_str = temp2[members].substr(6, temp2[members].size() - 6);
			// 陣形
			if (form_str == "単縦陣") {
				MapData[idx1][idx2].Formation = FOR_TRAIL;
			}
			else if (form_str == "複縦陣") {
				MapData[idx1][idx2].Formation = FOR_SUBTRAIL;
			}
			else if (form_str == "輪形陣") {
				MapData[idx1][idx2].Formation = FOR_CIRCLE;
			}
			else if (form_str == "梯形陣") {
				MapData[idx1][idx2].Formation = FOR_ECHELON;
			}
			else if (form_str == "単横陣") {
				MapData[idx1][idx2].Formation = FOR_ABREAST;
			}
			else {
				throw "その陣形は存在しません";
			}
			// 状況
			if (sim_str == "(昼戦)") {
				MapSim[idx1][idx2] = kModeD;
			}
			else if (sim_str == "(夜戦)") {
				MapSim[idx1][idx2] = kModeN;
			}
			else if (sim_str == "(昼夜)") {
				MapSim[idx1][idx2] = kModeDN;
			}
			else {
				throw "その状況は存在しません";
			}
			all_kammusues += members;
			++idx2;
			if (idx2 == MapData[idx1].size()) {
				++idx1;
				idx2 = 0;
			}
			++Step;
		}else if (Step >= 2 + all_patterns && Step < 2 + all_patterns + all_kammusues) {
			// 艦娘(深海棲艦)ごとの装備
			vector<string> temp2 = split(GetLine);
			for (rsize_t i = 0; i < temp2.size(); ++i) {
				int Number = ToInt(temp2[i]) - 1;
				if (Number < 0) break;
				MapData[idx3][idx4].Kammusues[idx5].Weapons[i] = WeaponList[Number];
			}
			++idx5;
			if (idx5 == MapData[idx3][idx4].Members) {
				++idx4;
				idx5 = 0;
			}
			if (idx4 == MapData[idx3].size()) {
				++idx3;
				idx4 = 0;
			}
			++Step;
		}
	}
	return;
}

/* 文字列を分割する */
vector<string> split(const string str) {
	string temp;
	stringstream sin(str);
	vector<string> output;
	while (getline(sin, temp, ',')) {
		output.push_back(temp);
	}
	return output;
}
vector<string> split(const string str, const string sp) {
	string temp;
	stringstream sin(str);
	vector<string> output;
	while (getline(sin, temp, sp[0])) {
		output.push_back(temp);
	}
	return output;
}

void ReadWeaponData2(unordered_map<int, weapon> &weapon_list) {
	weapon_list.clear();
	std::locale::global(std::locale("japanese"));
	fstream fin1("slotitems.csv");
	if (!fin1.is_open()) throw "slotitems.csvが正常に読み込めませんでした.";
	string GetLine;
	getline(fin1, GetLine);
	while (getline(fin1, GetLine)) {
		vector<string> WeaponData = split(GetLine);
		//装備データに変換する
		weapon Weapon;
		Weapon.Name = WeaponData[1];
		Weapon.Type = ToType(WeaponData[2]);
		Weapon.Defense = ToInt(WeaponData[3]);
		Weapon.Attack = ToInt(WeaponData[4]);
		Weapon.Torpedo = ToInt(WeaponData[5]);
		Weapon.Bomb = ToInt(WeaponData[6]);
		Weapon.AntiAir = ToInt(WeaponData[7]);
		Weapon.AntiSub = ToInt(WeaponData[8]);
		Weapon.Hit = ToInt(WeaponData[9]);
		Weapon.Evade = ToInt(WeaponData[10]);
		Weapon.Search = ToInt(WeaponData[11]);
		Weapon.Range = static_cast<RANGE>(ToInt(WeaponData[12]));
		Weapon.level_ = 0;
		//リストに追加する
		weapon_list[ToInt(WeaponData[0])] = Weapon;
	}
	weapon Weapon_None;
	weapon_list[-1] = Weapon_None;
}

void ReadKammusuData2(unordered_map<size_t, kammusu> &kammusu_list_1,
                      unordered_map<size_t, kammusu> &kammusu_list_99,
                      const unordered_map<int, weapon> &weapon_list) {
	kammusu_list_1.clear(); kammusu_list_99.clear();
	std::locale::global(std::locale("japanese"));
	fstream fin1("ships.csv");
	if (!fin1.is_open()) throw "ships.csvが正常に読み込めませんでした.";
	string GetLine;
	getline(fin1, GetLine);
	//17(揚陸艦),19(工作艦),20(潜水母艦),22(給油艦)は未対応
	const SC kinds[] = { SC_Other , SC_Other , SC_DD ,SC_CL ,SC_CLT ,SC_CA ,SC_CAV ,SC_CVL ,SC_BB ,SC_BB ,SC_BBV ,SC_CV ,
	                     SC_Other ,SC_SS ,SC_SSV ,SC_LST ,SC_AV ,SC_Other ,SC_ACV ,SC_Other,SC_Other ,SC_CP ,SC_Other };
	while (getline(fin1, GetLine)) {
		if (GetLine.find("null") != string::npos) continue;	//データにnullが含まれるものは避ける
		vector<string> KammusuData = split(GetLine);

		// 艦娘データに変換する
		kammusu Kammusu_1;
		//艦種
		Kammusu_1.Kind = kinds[ToInt(KammusuData[2])];
		if (KammusuData[9] == "0") Kammusu_1.Kind = SC_AF;
		//速力
		if (KammusuData[9] == "10") Kammusu_1.Speed = HighSpeed; else Kammusu_1.Speed = LowSpeed;
		//初期艤装および初期スロット数
		Kammusu_1.Slots = ToInt(KammusuData[11]);
		Kammusu_1.MaxAirs.resize(Kammusu_1.Slots);
		Kammusu_1.Weapons.resize(Kammusu_1.Slots);
		auto slots = split(KammusuData[12], "."), slot_weapons = split(KammusuData[16], ".");
		for (int i = 0; i < Kammusu_1.Slots; ++i) {
			Kammusu_1.Weapons[i] = weapon_list.at(ToInt(slot_weapons[i]));
			Kammusu_1.MaxAirs[i] = ToInt(slots[i]);
		}
		Kammusu_1.Airs = Kammusu_1.MaxAirs;
		//その他
		Kammusu_1.Name = KammusuData[1];
		Kammusu_1.Level = 1;
		Kammusu_1.MaxHP = ToInt(split(KammusuData[3], ".")[0]);
		Kammusu_1.HP = Kammusu_1.MaxHP;
		Kammusu_1.Luck = ToInt(split(KammusuData[8], ".")[0]);
		Kammusu_1.Range = static_cast<RANGE>(ToInt(KammusuData[10]));
		Kammusu_1.cond = 49;
		Kammusu_1.Ammo = 100;
		Kammusu_1.is_kammusu_ = static_cast<bool>(ToInt(KammusuData[17]));
		Kammusu_1.Defense = ToInt(split(KammusuData[4], ".")[0]);
		Kammusu_1.Attack = ToInt(split(KammusuData[5], ".")[0]);
		Kammusu_1.Torpedo = ToInt(split(KammusuData[6], ".")[0]);
		Kammusu_1.AntiAir = ToInt(split(KammusuData[7], ".")[0]);
		Kammusu_1.Evade = ToInt(split(KammusuData[13], ".")[0]);
		Kammusu_1.AntiSub = ToInt(split(KammusuData[14], ".")[0]);
		Kammusu_1.Search = ToInt(split(KammusuData[15], ".")[0]);
		//Lv99とLv1との違い
		kammusu Kammusu_99 = Kammusu_1;
		Kammusu_99.Defense = ToInt(split(KammusuData[4], ".")[1]);
		Kammusu_99.Attack = ToInt(split(KammusuData[5], ".")[1]);
		Kammusu_99.Torpedo = ToInt(split(KammusuData[6], ".")[1]);
		Kammusu_99.AntiAir = ToInt(split(KammusuData[7], ".")[1]);
		Kammusu_99.Evade = ToInt(split(KammusuData[13], ".")[1]);
		Kammusu_99.AntiSub = ToInt(split(KammusuData[14], ".")[1]);
		Kammusu_99.Search = ToInt(split(KammusuData[15], ".")[1]);
		//書き込む
		kammusu_list_1[ToInt(KammusuData[0])] = Kammusu_1;
		kammusu_list_99[ToInt(KammusuData[0])] = Kammusu_99;
	}
}

/* 艦娘のパラメータを逆算する */
kammusu calc_param(const kammusu &kammusu_1, const kammusu &kammusu_99, int level) {
	kammusu set_kammusu;
	//不変な部分はそのまま代入する
	//(改修可能な部分は、運を除いて当然最大まで行っていると見なす)
	set_kammusu = kammusu_99;
	//不変ではない場合、適宜計算する(運は元のjsonにあるのでデフォルトのままにする)
	set_kammusu.Level = level;
	set_kammusu.Evade = static_cast<int>(1.0 * (kammusu_99.Evade - kammusu_1.Evade) * level / 99 + kammusu_1.Evade);
	set_kammusu.AntiSub = static_cast<int>(1.0 * (kammusu_99.AntiSub - kammusu_1.AntiSub) * level / 99 + kammusu_1.AntiSub);
	set_kammusu.Search = static_cast<int>(1.0 * (kammusu_99.Search - kammusu_1.Search) * level / 99 + kammusu_1.Search);
	return set_kammusu;
}
