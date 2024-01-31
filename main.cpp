//============================================================================
// Name        : Epos4_2bar_test.cpp
// Author      : YongJae Lee
// Version     : 0.1
// Copyright   : maxon motor ag 2014-2021
// Description : Epos controller 2bar link test in C++
//============================================================================


#include "MaxonMotorWrap.hpp"

using namespace std;


unsigned short m_NormCurrent = 2730;//mA
unsigned short m_MaxCurrent = 3000;//mA
//EAppMode g_eAppMode = AM_DEMO;

MaxonJoint shoulder;

//keyboard int 
void  INThandler(int);

int main(int argc, char** argv)
{
  int lResult = MMC_FAILED;
  unsigned int ulErrorCode = 0;
  unsigned int lErrorCode = 0;
  int g_position = 0;
  int pos_deg = 0;
  float pos_rad = 0;
  shoulder.SetDefaultParameters();
  signal(SIGINT, INThandler);
  if((lResult = shoulder.OpenDevice(&ulErrorCode))!=MMC_SUCCESS)
    {
      shoulder.LogError("OpenDevice", lResult, ulErrorCode);
      return lResult;
    }
  if((lResult = shoulder.PrepareDemo(&ulErrorCode))!=MMC_SUCCESS)
			{
				shoulder.LogError("PrepareDemo", lResult, ulErrorCode);
				return lResult;
			}
  if((lResult = shoulder.EposSetMode(shoulder.g_pKeyHandle, g_usNodeId, lErrorCode))!=MMC_SUCCESS)
			{
				shoulder.LogError("Set Mode", lResult, ulErrorCode);
				return lResult;
			}

  while(1)
  {
    if(lResult = shoulder.EposPositionFeedback(g_pKeyHandle, g_usNodeId, &g_position, lErrorCode)!=MMC_SUCCESS)
    {
      shoulder.LogError("Position read", lResult, ulErrorCode);
      return lResult;
    }

    pos_deg = g_position *360 / 5000 / 26;
    pos_rad = deg2rad(pos_deg);
    cout << "POS_RADIAN : "<<pos_rad << " gravity : " << (1-cos(abs(pos_rad)))*100<<endl;

    //EposGoalCurrent(g_pKeyHandle, g_usNodeId, lErrorCode, (1-cos(abs(pos_rad)))*100);
    // sleep(1);
  }

  return 0;
}