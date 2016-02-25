#include "header.h"

weapon::weapon() {
	Name = "未装備";
	Type = Type_None;
	Attack = 0;
	Torpedo = 0;
	Bomb = 0;
	AntiAir = 0;
	AntiSub = 0;
	Search = 0;
	Hit = 0;
	Evade = 0;
	Range = NoneRange;
	Defense = 0;
	level_ = 0;
}

/* 艦載機かを判定 */
bool weapon::isAir() {
	if((Type == Type_PF)
	|| (Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_WB)
	|| (Type == Type_PA)
	|| (Type == Type_PS)
	|| (Type == Type_PSS)
	|| (Type == Type_WS)
	|| (Type == Type_WSN)
	|| (Type == Type_ASPP)
	|| (Type == Type_AJ)) {
		return true;
	} else {
		return false;
	}
}

/* 航空戦に参加できるかを判定 */
bool weapon::isAirWar() {
	if((Type == Type_PF)
	|| (Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_WB)
	|| (Type == Type_PA)) {
		return true;
	} else {
		return false;
	}
}

/* 艦上偵察機かを判定 */
bool weapon::isAirPS() {
	if((Type == Type_PS)
	|| (Type == Type_PSS)) {
		return true;
	} else {
		return false;
	}
}

/* 対空砲火で撃ち落とせるかを判定 */
bool weapon::isAirWar2() {
	if((Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_WB)
	|| (Type == Type_PA)) {
		return true;
	} else {
		return false;
	}
}

/* 対潜攻撃可能な装備かを判定 */
bool weapon::isAntiSub(){
	if((Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_WB)
	|| (Type == Type_PA)
	|| (Type == Type_ASPP)
	|| (Type == Type_AJ)
	|| (Type == Type_DP)
	|| (Type == Type_Sonar)){
		return true;
	}
	return false;
}

/* 対潜攻撃可能な装備かを判定(航戦・航巡・水母) */
bool weapon::isAntiSub1(){
	if((Type == Type_WB)
	|| (Type == Type_ASPP)
	|| (Type == Type_AJ)){
		return true;
	}
	return false;
}

/* 対潜攻撃可能な装備かを判定(軽空母) */
bool weapon::isAntiSub2(){
	if((Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_PA)){
		return true;
	}
	return false;
}

/* 対潜攻撃可能な装備かを判定(水雷系) */
bool weapon::isAntiSub3(){
	if((Type == Type_DP)
	|| (Type == Type_Sonar)){
		return true;
	}
	return false;
}
