#ifndef __PUBG_H__
#define __PUBG_H__



typedef struct _S_PUBG_
{
    unsigned short N3_AttackX;unsigned short N4_AttackY;
    unsigned short N5_MoveX;unsigned short N6_MoveY;
    unsigned short N7_JumpX;unsigned short N8_JumpY;
    unsigned short N9_SquatX;unsigned short N10_SquatY;
    unsigned short N11_LieX;unsigned short N12_LieY;
    unsigned short N13_FaceX;unsigned short N14_FaceY;
    unsigned short N15_WatchX;unsigned short N16_WatchY;
    unsigned short N17_PackageX;unsigned short N18_PackageY;
    unsigned short N19_ArmsLeftX;unsigned short N20_ArmsLeftY;
    unsigned short N21_ArmsRightX;unsigned short N22_ArmsRightY;
    unsigned short N23_MapX;unsigned short N24_MapY;
    unsigned short N25_AimX;unsigned short N26_AimY;
    unsigned short N27_CheckPackageX;unsigned short N28_CheckPackageY;
    unsigned short N29_DoorX;unsigned short N30_DoorY;
    unsigned short N31_DriveX;unsigned short N32_DriveY;
    unsigned short N33_GetOffX;unsigned short N34_GetOffY;
    unsigned short N35_GrenadeX;unsigned short N36_GrenadeY;
    unsigned short N37_MedicineX;unsigned short N38_MedicineY;
    unsigned short N39_ReloadX;unsigned short N40_ReloadY;
    unsigned short N41_SaveX;unsigned short N42_SaveY;
    unsigned short N43_SprintX;unsigned short N44_SprintY;
    unsigned short N45_FollowX;unsigned short N46_FollowY;
    unsigned short N47_PickX;unsigned short N48_PickY;
    unsigned short N49_RideX;unsigned short N50_RideY;
    unsigned short N51_Pick1X;unsigned short N52_Pick1Y;
    unsigned short N53_Pick2X;unsigned short N54_Pick2Y;
    unsigned short N55_Pick3X;unsigned short N56_Pick3Y;
	
}S_PUBG;

extern S_PUBG s_pubg;

void get_pubg_buffer(char *buf);

#endif
