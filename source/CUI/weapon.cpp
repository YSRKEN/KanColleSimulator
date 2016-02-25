#include "header.h"

weapon::weapon() {
	Name = "–¢‘•”õ";
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

/* ŠÍÚ‹@‚©‚ğ”»’è */
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

/* q‹óí‚ÉQ‰Á‚Å‚«‚é‚©‚ğ”»’è */
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

/* ŠÍã’ã@‹@‚©‚ğ”»’è */
bool weapon::isAirPS() {
	if((Type == Type_PS)
	|| (Type == Type_PSS)) {
		return true;
	} else {
		return false;
	}
}

/* ‘Î‹ó–C‰Î‚ÅŒ‚‚¿—‚Æ‚¹‚é‚©‚ğ”»’è */
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

/* ‘ÎöUŒ‚‰Â”\‚È‘•”õ‚©‚ğ”»’è */
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

/* ‘ÎöUŒ‚‰Â”\‚È‘•”õ‚©‚ğ”»’è(qíEq„E…•ê) */
bool weapon::isAntiSub1(){
	if((Type == Type_WB)
	|| (Type == Type_ASPP)
	|| (Type == Type_AJ)){
		return true;
	}
	return false;
}

/* ‘ÎöUŒ‚‰Â”\‚È‘•”õ‚©‚ğ”»’è(Œy‹ó•ê) */
bool weapon::isAntiSub2(){
	if((Type == Type_PBF)
	|| (Type == Type_PB)
	|| (Type == Type_PA)){
		return true;
	}
	return false;
}

/* ‘ÎöUŒ‚‰Â”\‚È‘•”õ‚©‚ğ”»’è(…—‹Œn) */
bool weapon::isAntiSub3(){
	if((Type == Type_DP)
	|| (Type == Type_Sonar)){
		return true;
	}
	return false;
}
