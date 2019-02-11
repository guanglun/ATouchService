#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "log.h"
#include "pubg.h"

S_PUBG s_pubg;

void SetAttack(unsigned short px,unsigned short py) { s_pubg.N3_AttackX = px;s_pubg.N4_AttackY = py; }
void SetMove(unsigned short px,unsigned short py) { s_pubg.N5_MoveX = px;s_pubg.N6_MoveY = py; }
void SetJump(unsigned short px,unsigned short py) { s_pubg.N7_JumpX = px;s_pubg.N8_JumpY = py; }
void SetSquat(unsigned short px,unsigned short py) { s_pubg.N9_SquatX = px;s_pubg.N10_SquatY = py; }
void SetLie(unsigned short px,unsigned short py) { s_pubg.N11_LieX = px;s_pubg.N12_LieY = py; }
void SetFace(unsigned short px,unsigned short py) { s_pubg.N13_FaceX = px;s_pubg.N14_FaceY = py; }
void SetWatch(unsigned short px,unsigned short py) { s_pubg.N15_WatchX = px;s_pubg.N16_WatchY = py; }
void SetPackage(unsigned short px,unsigned short py) { s_pubg.N17_PackageX = px;s_pubg.N18_PackageY = py; }
void SetArmsLeft(unsigned short px,unsigned short py) { s_pubg.N19_ArmsLeftX = px;s_pubg.N20_ArmsLeftY = py; }
void SetArmsRight(unsigned short px,unsigned short py) { s_pubg.N21_ArmsRightX = px;s_pubg.N22_ArmsRightY = py; }
void SetMap(unsigned short px,unsigned short py) { s_pubg.N23_MapX = px;s_pubg.N24_MapY = py; }
void SetAim(unsigned short px,unsigned short py) { s_pubg.N25_AimX = px;s_pubg.N26_AimY = py; }
void SetCheckPackage(unsigned short px,unsigned short py) { s_pubg.N27_CheckPackageX = px;s_pubg.N28_CheckPackageY = py; }
void SetDoor(unsigned short px,unsigned short py) { s_pubg.N29_DoorX = px;s_pubg.N30_DoorY = py; }
void SetDrive(unsigned short px,unsigned short py) { s_pubg.N31_DriveX = px;s_pubg.N32_DriveY = py; }
void SetGetOff(unsigned short px,unsigned short py) { s_pubg.N33_GetOffX = px;s_pubg.N34_GetOffY = py; }
void SetGrenade(unsigned short px,unsigned short py) { s_pubg.N35_GrenadeX = px;s_pubg.N36_GrenadeY = py; }
void SetMedicine(unsigned short px,unsigned short py) { s_pubg.N37_MedicineX = px;s_pubg.N38_MedicineY = py; }
void SetReload(unsigned short px,unsigned short py) { s_pubg.N39_ReloadX = px;s_pubg.N40_ReloadY = py; }
void SetSave(unsigned short px,unsigned short py) { s_pubg.N41_SaveX = px;s_pubg.N42_SaveY = py; }
void SetSprint(unsigned short px,unsigned short py) { s_pubg.N43_SprintX = px;s_pubg.N44_SprintY = py; }
void SetFollow(unsigned short px,unsigned short py) { s_pubg.N45_FollowX = px;s_pubg.N46_FollowY = py; }
void SetPick(unsigned short px,unsigned short py) { s_pubg.N47_PickX = px;s_pubg.N48_PickY = py; }
void SetRide(unsigned short px,unsigned short py) { s_pubg.N49_RideX = px;s_pubg.N50_RideY = py; }
void SetPick1(unsigned short px,unsigned short py) { s_pubg.N51_Pick1X = px;s_pubg.N52_Pick1Y = py; }
void SetPick2(unsigned short px,unsigned short py) { s_pubg.N53_Pick2X = px;s_pubg.N54_Pick2Y = py; }
void SetPick3(unsigned short px,unsigned short py) { s_pubg.N55_Pick3X = px;s_pubg.N56_Pick3Y = py; }

void get_pubg_buffer(char *buf)
{
    int i = 0;

    SetAttack(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetMove(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetJump(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetSquat(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetLie(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetFace(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetWatch(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetPackage(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetArmsLeft(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetArmsRight(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetMap(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));

    SetAim(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetCheckPackage(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetDoor(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetDrive(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetGetOff(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetGrenade(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetMedicine(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetReload(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetSave(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetSprint(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetFollow(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetPick(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetRide(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetPick1(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetPick2(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
    SetPick3(char2short(buf[i++],buf[i++]),char2short(buf[i++],buf[i++]));
}