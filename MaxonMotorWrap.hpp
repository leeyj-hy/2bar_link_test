#ifndef GRAVITYCOMPENSATION__
#define GRAVITYCOMPENSATION__

#ifndef MMC_SUCCESS
	#define MMC_SUCCESS 0
#endif

#ifndef MMC_FAILED
	#define MMC_FAILED 1
#endif


#include "Definitions.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <list>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>

#include <signal.h>
#include <cmath>

#define _PI 3.1416
#define _hPI 1.5708
#define _encResolution 5000
#define _gearRatio 26

using namespace std;


typedef void* HANDLE;
typedef int BOOL;

class MaxonJoint
{
private:
  /* data */
public:
  MaxonJoint(/* args */);
  ~MaxonJoint();
};

MaxonJoint::MaxonJoint(/* args */)
{
}

MaxonJoint::~MaxonJoint()
{
}


/* 

void* g_pKeyHandle = 0;
unsigned short g_usNodeId = 1;
string g_deviceName;
string g_protocolStackName;
string g_interfaceName;
string g_portName;
int g_baudrate = 0;

const string g_programName = "Epo4_test";

void  LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode);
void  LogInfo(string message);

int   OpenDevice(unsigned int* p_pErrorCode);
int   CloseDevice(unsigned int* p_pErrorCode);

int   PrepareDemo(unsigned int* p_pErrorCode);
void  SetDefaultParameters();
int   EposSetMotorType();
int   EposSetMotorParameter();

int   EposGoalCurrent(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode, int targetCurrent);
int   EposHaltPositionMovement(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   EposSetMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode);
int   EposPositionFeedback(HANDLE p_DeviceHandle, unsigned short p_usNodeId, int* p_Positionals , unsigned int & p_rlErrorCode);

float deg2rad(int inputDegree);


//example code start

void LogError(string functionName, int p_lResult, unsigned int p_ulErrorCode)
{
	cerr << g_programName << ": " << functionName << " failed (result=" << p_lResult << ", errorCode=0x" << std::hex << p_ulErrorCode << ")"<< endl;
}

void LogInfo(string message)
{
	cout << message << endl;
}


void SetDefaultParameters()
{
	//USB
	g_usNodeId = 1;
	g_deviceName = "EPOS4"; 
	g_protocolStackName = "MAXON SERIAL V2"; 
	g_interfaceName = "USB"; 
	g_portName = "USB0"; 
	g_baudrate = 1000000; 
}

int OpenDevice(unsigned int* p_pErrorCode)
{
	int lResult = MMC_FAILED;

	char* pDeviceName = new char[255];
	char* pProtocolStackName = new char[255];
	char* pInterfaceName = new char[255];
	char* pPortName = new char[255];

	strcpy(pDeviceName, g_deviceName.c_str());
	strcpy(pProtocolStackName, g_protocolStackName.c_str());
	strcpy(pInterfaceName, g_interfaceName.c_str());
	strcpy(pPortName, g_portName.c_str());

	LogInfo("Open device...");

	g_pKeyHandle = VCS_OpenDevice(pDeviceName, pProtocolStackName, pInterfaceName, pPortName, p_pErrorCode);

	if(g_pKeyHandle!=0 && *p_pErrorCode == 0)
	{
		unsigned int lBaudrate = 0;
		unsigned int lTimeout = 0;

		if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
		{
			if(VCS_SetProtocolStackSettings(g_pKeyHandle, g_baudrate, lTimeout, p_pErrorCode)!=0)
			{
				if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, p_pErrorCode)!=0)
				{
					if(g_baudrate==(int)lBaudrate)
					{
						lResult = MMC_SUCCESS;
            LogInfo("Device Successfully Opened!");
					}
				}
			}
		}
	}
	else
	{
		g_pKeyHandle = 0;
	}

	delete []pDeviceName;
	delete []pProtocolStackName;
	delete []pInterfaceName;
	delete []pPortName;

	return lResult;
}

int CloseDevice(unsigned int* p_pErrorCode)
{
	int lResult = MMC_FAILED;
  

	*p_pErrorCode = 0;

	LogInfo("Close device");

	if(VCS_CloseDevice(g_pKeyHandle, p_pErrorCode)!=0 && *p_pErrorCode == 0)
	{
		lResult = MMC_SUCCESS;
	}

	return lResult;
}

int PrepareDemo(unsigned int* p_pErrorCode)
{
	int lResult = MMC_SUCCESS;
	BOOL oIsFault = 0;

	if(VCS_GetFaultState(g_pKeyHandle, g_usNodeId, &oIsFault, p_pErrorCode ) == 0)
	{
		LogError("VCS_GetFaultState", lResult, *p_pErrorCode);
		lResult = MMC_FAILED;
	}

	if(lResult==0)
	{
		if(oIsFault)
		{
			stringstream msg;
			msg << "clear fault, node = '" << g_usNodeId << "'";
			LogInfo(msg.str());

			if(VCS_ClearFault(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
			{
				LogError("VCS_ClearFault", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}
		}

		if(lResult==0)
		{
			BOOL oIsEnabled = 0;

			if(VCS_GetEnableState(g_pKeyHandle, g_usNodeId, &oIsEnabled, p_pErrorCode) == 0)
			{
				LogError("VCS_GetEnableState", lResult, *p_pErrorCode);
				lResult = MMC_FAILED;
			}

			if(lResult==0)
			{
				if(!oIsEnabled)
				{
					if(VCS_SetEnableState(g_pKeyHandle, g_usNodeId, p_pErrorCode) == 0)
					{
						LogError("VCS_SetEnableState", lResult, *p_pErrorCode);
						lResult = MMC_FAILED;
					}
				}
			}
		}
	}
	return lResult;
}
//example code end


//user code begin

int EposGoalCurrent(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode, int targetCurrent)
{
  int lResult = MMC_SUCCESS;
  if(VCS_SetCurrentMustEx(p_DeviceHandle, p_usNodeId, targetCurrent, &p_rlErrorCode) == 0)
			{
				LogError("VCS_SetCurrentMustEx", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
			}
  return lResult;
}

float deg2rad(int inputDegree)
{
  return inputDegree*_PI/180;
}

int EposHaltPositionMovement(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
  int lResult = MMC_SUCCESS;
  if(VCS_HaltPositionMovement(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
			{
				LogError("VCS_HaltPositionMovement", lResult, p_rlErrorCode);
				lResult = MMC_FAILED;
			}
    return lResult;
}

void  INThandler(int sig)
{
    char  c;
    int lResult = MMC_FAILED;
    unsigned int ulErrorCode = 0;
    unsigned int lErrorCode = 0;
     signal(sig, SIG_IGN);
     printf("Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
     {
      EposHaltPositionMovement(g_pKeyHandle, g_usNodeId, lErrorCode);
      if(VCS_SetDisableState(g_pKeyHandle, g_usNodeId, &lErrorCode) == 0)
        {
          LogError("VCS_SetDisableState", lResult, lErrorCode);
          lResult = MMC_FAILED;
        }
      if((lResult = CloseDevice(&ulErrorCode))!=MMC_SUCCESS)
        {
          LogError("Closing Device and shutting down", lResult, ulErrorCode);
        }
      exit(0);
     }
          
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}


int EposSetMode(HANDLE p_DeviceHandle, unsigned short p_usNodeId, unsigned int & p_rlErrorCode)
{
  int lResult = MMC_SUCCESS;
	stringstream msg;

	msg << "set profile current mode, node = " << p_usNodeId;
	LogInfo(msg.str());

	if(VCS_ActivateCurrentMode(p_DeviceHandle, p_usNodeId, &p_rlErrorCode) == 0)
	{
		LogError("VCS_ActivateCurrentMode", lResult, p_rlErrorCode);
		lResult = MMC_FAILED;
	}
  return lResult;
}


int EposPositionFeedback(HANDLE p_DeviceHandle, unsigned short p_usNodeId, int* p_Positionals , unsigned int & p_rlErrorCode)
{
  int lResult = MMC_SUCCESS;
  stringstream msg;

  if(VCS_GetPositionIs(p_DeviceHandle, p_usNodeId, p_Positionals, &p_rlErrorCode)==0)
  {
    msg<<"pos : "<< p_Positionals;
    LogInfo(msg.str());
    lResult = MMC_FAILED;
  }
  return lResult;
}
 */
//user code end

#endif // !GRAVITYCOMPENSATION__
