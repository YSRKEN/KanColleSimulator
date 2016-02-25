#include "header.h"

weapon::weapon() {
	Name = "������";
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
}

/* �͍ڋ@���𔻒� */
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

/* �q���ɎQ���ł��邩�𔻒� */
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

/* �͏��@�@���𔻒� */
bool weapon::isAirPS() {
	if((Type == Type_PS)
	|| (Type == Type_PSS)) {
		return true;
	} else {
		return false;
	}
}

/* �΋�C�΂Ō������Ƃ��邩�𔻒� */
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

/* �ΐ��U���\�ȑ������𔻒� */
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

/* �ΐ��U���\�ȑ������𔻒�(�q��E�q���E����) */
bool weapon::isAntiSub1(){
	if((Type == Type_WB)
	|| (Type == Type_ASPP)
	|| (Type == Type_AJ)){
		return true;
	}
	return false;
}

/* �ΐ��U���\�ȑ������𔻒�(�y���) */
bool weapon::isAntiSub2(){
	if((Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_PA)){
		return true;
	}
	return false;
}

/* �ΐ��U���\�ȑ������𔻒�(�����n) */
bool weapon::isAntiSub3(){
	if((Type == Type_DP)
	|| (Type == Type_Sonar)){
		return true;
	}
	return false;
}
