#ifndef _JRCTRL_H_
#define _JRCTRL_H_

#include "ECMotionControl.h"
#include "ShmInterface.h"
#include "myFileOperation.h"
//jari ERR
#define INVERSE_FAIL	86   //ѡ��ʧ��
#define NO_SOLUTION		87
#define KYN_SINGULAR    88   //�����
#define SALTUS_STEP     89   //����
#define INVALID_CHANGE  90
#define KYN_JNT_OUT_RANGE 91 //�ؽڳ���
#define MC_ERR_ZERO_PARAM 92

#define TCP_TYPE_TCP		0
#define TCP_TYPE_DEFAULT	1
#define TCP_TYPE_CAMERA		2
#define MAX_TCP_NUM			16
#define MAX_CUBE_NUM        9
#define MAX_CHAR_INTP		64 //����ܽ������ַ�������

#define TIME_LESS_THEN_ZERO 93
#define SCAN_BUFFER			1000
#define SCAN_CYCLE			8000
//#define _PROFIBUS_
//#define _3D_SEWING_
//#define TOOL_RPY
//#define LONG_CIRC
//#define	RPY_ACC
//#define _SCAN_SEW_
//#define _IO_
//#define _WORKS_

/*Jari Define*/
//#define ptpT
#define DOUBLE_ABS_MIN                  1e-15
#define PI								3.141592654
#define J1								0
#define J2								1
#define J3								2
#define J4								3
#define J5								4
#define J6								5
#define ROBOT_AXES_NUM					6
#define KEY_DOWN						1
#define KEY_UP							0
#define DEMO_MAX_NUM_OF_AXIS            6                       /* number of axis */ //9
#define ADDITIONAL_AXES_NUM				3
#define PRE_IPR_NUM						16
#define PRE_IPR
#define R45								0
#define R46								1
#define R56								2
#ifdef _3D_SEWING_
	#define SPINDLE							0
	#define SWAY							1
	#define EX3								2
#endif
#define MIN_IO_GET_CNT					1

int read_a_line(char *buf,FILE *fp);
int getItem(char *pbuf, float *v);
//JARI
enum MC_GROUP_PLCOPEN_STATE
{
	GROUP_PLCOPEN_STATE_STANDBY		= 0,
	GROUP_PLCOPEN_STATE_HOMING		= 1,
	GROUP_PLCOPEN_STATE_STOPPING	= 2,
	GROUP_PLCOPEN_STATE_ERROR_STOP	= 3,
	GROUP_PLCOPEN_STATE_MOVING		= 4,
	GROUP_PLCOPEN_STATE_DISABLED	= 5,
	GROUP_PLCOPEN_STATE_PAUSE		= 6,
	GROUP_PLCOPEN_STATE_UNKNOWN		= 7
};

enum ePtp{
	aacc,   //�Ӽ���
	cacc,   //�ȼ���
	dacc,	//������
	topv,   //����
	cv,		//����
	adcc,	//�Ӽ���
	cdcc,	//�ȼ���
	ddcc	//������
};


/*��ת������,N,O,A*/
struct RMATRIX
{
	/*nomal����*/
	MC_T_REAL Nx;
	MC_T_REAL Ny;
	MC_T_REAL Nz;
	/*orientationָ��*/
	MC_T_REAL Ox;
	MC_T_REAL Oy;
	MC_T_REAL Oz;
	/*approach�ӽ�*/
	MC_T_REAL Ax;
	MC_T_REAL Ay;
	MC_T_REAL Az;
};

/*������������ƫ���Ƕ���*/
struct RPY
{
	MC_T_REAL Roll;
	MC_T_REAL Pitch;
	MC_T_REAL Yaw;
};


/*��������ϵ*/
enum FRAME_TYPE
{
	WorldFrame = 1,
	ToolFrame,
	UserFrame
};
/*---*/
enum KEY_SIGN
{
	J1P = 1, J2P, J3P, J4P, J5P, J6P,
	J1M, J2M, J3M, J4M, J5M, J6M,
	XP, YP, ZP, RXP, RYP, RZP,
	XM, YM, ZM, RXM, RYM, RZM,
	MOT, VP, VM, JOG, NOP,
	PROG_LOAD, PROG_KILL,
	PROG_START, PROG_STOP,
	PROG_SAVE, PNT_SAVE,
	GET_FULL_PRJS, ADD_PRJ, DEL_PRJ, RENAME_PRJ, 
	GET_FILE_CONTENT, ADD_FILE, DEL_FILE, RENAME_FILE,
	GET_PNT_NAME, GET_PNT_DATA
	,EX1P,EX1M,EX2P,EX2M,EX3P,EX3M
};

enum OUT_RANGE_JNT
{
	NONE,J1B, J2B, J3B, J4B, J5B, J6B,
	J1S, J2S, J3S, J4S, J5S, J6S
};


enum WORK_SPACE
{
	eAxesSpace =0, eWorldSpace,eTOOLSPACE
};

#ifdef _3D_SEWING_
typedef struct 
{
	float fRobSpeed;
	float fGap;
	float fNeedleFre;
	int	  needleHome;
	int	  sliderHome;
	float fSyncIn;
	float fSyncOut;
	int   rotateHome;
}SEWING_PARA;
#endif

struct CARTPOS
{
	MC_T_REAL   Px;
	MC_T_REAL   Py;
	MC_T_REAL   Pz;

	RMATRIX   Rmatrix;	
	RPY       Rpy;
	FRAME_TYPE Frametype;   /*����ϵ����*/
};

/************************************************************************/
/* �����˱��������ض���                                               */
/************************************************************************/
/*�����˻�е�����˽ṹ��*/
typedef struct
{
	MC_T_REAL L1;
	MC_T_REAL L2;
	MC_T_REAL L3;
	MC_T_REAL L4;
	MC_T_REAL L5;
	MC_T_REAL L6;
	float L7;
}LINK,*PLINK;

/**�����˹ؽ����ݽṹ(6��)**/
typedef struct
{
	MC_T_REAL Theta1;
	MC_T_REAL Theta2;
	MC_T_REAL Theta3;
	MC_T_REAL Theta4;
	MC_T_REAL Theta5;
	MC_T_REAL Theta6;
	MC_T_INT nSolutionNum;
}JOINT,*PJOINT;

/************************************************************************/
/* �������˶���̬�����ṹ��                                             */
/************************************************************************/
typedef struct  
{
	/*�ٶȰٷֱ�,100Ϊ���*/
	int vel_percent;
	/*���ٶȰٷֱ�*/
	int acc_percent;
	/*���ٶȰٷֱ�*/
	int dec_percent;
	/*�滮��ʽ*/
	int plan_method;
}DYNAMIC,*PDYNAMIC;

//��������ϵ����ڻ�е��ĩ�˵�λ�ã�������ϵֻ��λ�ò�ͬ����̬��ͬ
typedef struct
{
	MC_T_REAL   X;
	MC_T_REAL   Y;
	MC_T_REAL   Z;
}relativePos, *pRelativePos;


struct Pos3d
{
	float Px;
	float Py;
	float Pz;
};


struct _tcp   //����tcp�ṹ��
{
	char name[16];
	float para[6];
};

struct _sTcp  //tcp����
{
	int num;
	_tcp tcp[MAX_TCP_NUM];
};

/*ROBOT_INFO:�����˲���*/
typedef struct  
{
	JOINT cur_joint;		/*��ǰ�ؽڽ�*/   //Ŀǰû��ʹ��  add by cwq at 2015.9.10
	JOINT max_joint;		/*���ؽ��˶���Χ*/	
	JOINT min_joint;		/*��С�ؽ��˶���Χ*/
	/////////////////////////////////////////////////////////////
	//������ת��
	MC_T_REAL MaxMotorSpeed[ROBOT_AXES_NUM];
	//DHģ���еĹؽڷ�Χ	
	MC_T_REAL MaxDhJnt[ROBOT_AXES_NUM];     /*���ؽ��˶���Χ*/
	MC_T_REAL MinDhJnt[ROBOT_AXES_NUM];     /*��С�ؽ��˶���Χ*/

	MC_T_REAL MaxJntPerMs[ROBOT_AXES_NUM];   /*���ؽڲ���*/
	int MaxPulsePerMs[ROBOT_AXES_NUM];   /*ÿ�������������*/

	/////////////////////////////////////////////////////////////
	int cnt_per_cycle;		/*������ÿȦ������*/
	int enc_home[ROBOT_AXES_NUM];		//������ԭ��

	int max_enc[ROBOT_AXES_NUM];		/*���������Χ*/
	int min_enc[ROBOT_AXES_NUM];		/*��������С��Χ*/

	MC_T_REAL joint_home[ROBOT_AXES_NUM];	//���ؽڳ�ʼ��	
	MC_T_REAL re_radio[ROBOT_AXES_NUM];	//������ٱ�

	MC_T_REAL MaxSpeed;       //������Ĭ������ٶ�,10��/s
	MC_T_REAL MaxSpeedAcc;
	MC_T_REAL MaxLineSpeed;	//������������ٶ�
	MC_T_REAL MaxLineAcc;		//����������߼��ٶ�
	DYNAMIC RobotDyn;       //�������˶�ѧ����

	LINK RobotLink;			//�����˸˳�����

	MC_T_REAL JntUniRadio[ROBOT_AXES_NUM]; //�ؽڹ�һ������

	/*	long MaxEncStep[6];		//�����˸��ؽ�����ٶȶ�Ӧ�ı���������*/

	// 	int DefJSpeed[6];		//Ĭ�ϻ����˹ؽ��ٶ�ֵ
	// 	int DefSpeedRadio[6];	//Ĭ�ϻ������ٶȵ��ڱ�

	int motorDir[ROBOT_AXES_NUM];//ԭ�����Լ������ڻ����˱���ṹ��֮�⣬����Ϊ���������õ������˱���ṹ������  change by cwq

	int manual_rate[ROBOT_AXES_NUM];//ԭ�����ڻ�е�۱���ṹ�����⣬�Լ��õ���е�۱���ṹ������  change by cwq

	float cpRatio[3];

	relativePos toolRelativeEndPos;//��������ϵ����ڻ�е��ĩ������ϵ��λ��  add by cwq at 2015.9.17

}ROBOT_INFO,*PROBOT_INFO;

/*������6�ؽڱ��������ݽṹ*/
typedef struct  
{
	int Encoder[ROBOT_AXES_NUM];
}JNTS_ENC,*PJNTS_ENC;

/* Index for process data variable */
struct EC_T_VARIDX
{
	EC_T_DWORD dwPdoIdx;
	EC_T_DWORD dwVarIdx;
};

enum eMoveState
{
	EMoveSetup,
	EMoving,
	EPause
};


struct EC_T_MOTION_PARAMETER
{
	MC_T_REAL fCommandPos, fCommandVel, fCommandAcc, fCommandJerk;
	MC_T_REAL fFollowingError;
};

struct EC_T_ROB_MOTION_PARAMETER
{
	MC_T_REAL fXPos, fYPos, fZPos, fRollPos,fPitchPos,fYawPos ;
	MC_T_LONG lEnc[ROBOT_AXES_NUM];
};


struct EC_T_LOGINFO
{
	EC_T_MOTION_PARAMETER curPos;
};

//���󱨾���Ϣ
typedef struct
{
	unsigned short code; //��Ϣ����
	EC_T_BYTE type;           //�������ͼ���Դ, 0��RC��1��RM��2��ʾ����
	EC_T_BYTE degree;         //����ȼ�, 0,1,2 ��Ϣ��3,4,5���棻6,7,8����
}ALARMINFO;

//Robot Log Info
struct EC_T_ROB_LOGINFO
{
	EC_T_ROB_MOTION_PARAMETER curPos;
	EC_T_ROB_MOTION_PARAMETER actuPos;
};

//�����˶�ʱ��
struct WeldTime
{
	double   T_totle;
	double   T_stay1;
	double   T_stay2;
};

//�������ڲ���
struct Tpara
{
	double Azhenfu;
	double Apinlv;
	double WSpeed;
	double RAngle;   //��ƽ����ת�Ƕ�
};

enum eWaveType
{
	NO_WAVE,        //�ްڶ�
	TRIGLR_PLANE,   //ƽ�����ǲ�
	TRIGLR_3D,		//�ռ����ǲ�
	TRIGLR_L,		//L�Ͱ�
	TRIGLR_CHR,		//ʥ������
	TRIGLR_LEAN		//б��
};

/* data for IO module */
typedef struct  
{
	EC_T_DWORD                  dwSlaveId;
	EC_T_DWORD                  dwPdOffsOut;
	EC_T_DWORD                  dwPdSizeOut;
	EC_T_DWORD                  dwPdOffsOut2;
	EC_T_DWORD                  dwPdSizeOut2;

	EC_T_DWORD                  dwPdOffsIn;
	EC_T_DWORD                  dwPdSizeIn;
	EC_T_DWORD                  dwPdOffsIn2;
	EC_T_DWORD                  dwPdSizeIn2;

	//����
	EC_T_BYTE	byIn1;
	EC_T_BYTE	byIn2;
	//���
	EC_T_BYTE	byOut1;
	EC_T_BYTE   byOut2;
	EC_T_WORD   wPwmout;
} EC_T_DEMO_IO_MODULE;

/* Data for a movement */
struct MC_GROUP_MOVEMENT
{
	MC_T_REAL			fAxisTotalDist;             /* [Inc] Total distance  */
	MC_T_REAL			fAxisRemainDist;            /* [Inc] Remaining distance  */
	MC_T_REAL			fAxisDoneDist;              /* [Inc] Done distance  */
	MC_T_REAL			fAxisNextMoveCorrection;    /* [Inc] Correction for next move (remainder previous) */
	MC_T_REAL			fAxisDist1to2;           /* [Inc] Distance of segments 1-2  */
	MC_T_REAL			fAxisDist3to4;           /* [Inc] Distance of segments 3-4  */

	MC_T_REAL			fAxisStartVel;              /* [Inc/Cycle] Value of velocity and start of movement */
	MC_T_REAL			fAxisStartAcc;              /* [Inc/Cycle] Value of commanded acc and start of movement */
	MC_T_REAL			fAxisConstantVel;           /* [Inc/Cycle] Value of the constant (middle) velocity */
	MC_T_REAL			fAxisEndVel;                /* [Inc/Cycle] Value of velocity and end of movement */
	MC_T_REAL			fAxisMaxAcc;                /* [Inc/Cycle^2] Value of the max. acceleration  */
	MC_T_REAL			fAxisMaxDec;                /* [Inc/Cycle^2] Value of max. deceleration  */
	MC_T_REAL			fAxisStopDec;
	MC_T_REAL			fAxisJerk;                  /* [Inc/Cycle^3] Value of jerk */
	MC_T_REAL			fAxisTargetVel;
	MC_T_REAL			fAxisTargetAcc;
	MC_T_REAL			fAxisCurVel;
	MC_T_REAL			fAxisCurAcc;
	MC_T_REAL			fAxisNextVel;

	MC_T_REAL			fAxisCommandVel;            /* [Inc/Cycle] commanded velocity */
	MC_T_REAL			fAxisCommandAcc;            /* +/- [Inc/Cycle^2] commanded acceleration */
	MC_T_REAL			fAxisCommandJerk;           /* +/- [Inc/Cycle^3] commanded jerk */
	MC_T_REAL			fAxisRemainStep;            /* [Inc] Remainder package */
	MC_T_REAL			fAxisTargetPosStep;         /* [Inc] change of target pos in current cycle */
	MC_T_REAL			fAxisLastTargetPosStep;     /* copy of lTargetPosStep just for logging */
	MC_T_REAL			fAxisLastCommandAcc;        /* for calc of jerk */

	MC_T_REAL			fCartTotalDist;             /* [Inc] Total distance  */
	MC_T_REAL			fCartRemainDist;            /* [Inc] Remaining distance  */
	MC_T_REAL			fCartDoneDist;              /* [Inc] Done distance  */
	MC_T_REAL			fCartNextMoveCorrection;    /* [Inc] Correction for next move (remainder previous) */
	MC_T_REAL			fCartDist1to2;           /* [Inc] Distance of segments 1-2  */
	MC_T_REAL			fCartDist3to4;           /* [Inc] Distance of segments 3-4  */

	MC_T_REAL			fCartStartVel;              /* [Inc/Cycle] Value of velocity and start of movement */
	MC_T_REAL			fCartStartAcc;              /* [Inc/Cycle] Value of commanded acc and start of movement */
	MC_T_REAL			fCartConstantVel;           /* [Inc/Cycle] Value of the constant (middle) velocity */
	MC_T_REAL			fCartEndVel;                /* [Inc/Cycle] Value of velocity and end of movement */
	MC_T_REAL			fCartMaxAcc;                /* [Inc/Cycle^2] Value of the max. acceleration  */
	MC_T_REAL			fCartMaxDec;                /* [Inc/Cycle^2] Value of max. deceleration  */
	MC_T_REAL			fCartStopDec;
	MC_T_REAL			fCartJerk;                  /* [Inc/Cycle^3] Value of jerk */
	MC_T_REAL			fCartTargetVel;
	MC_T_REAL			fCartTargetAcc;
	MC_T_REAL			fCartCurVel;
	MC_T_REAL			fCartNextVel;
	MC_T_REAL			fCurVel;
	MC_T_REAL			fMinDeltaVel;
	MC_T_REAL			fMaxDeltaVel;

	MC_T_REAL			fCartCommandVel;            /* [Inc/Cycle] commanded velocity */
	MC_T_REAL			fCartCommandAcc;            /* +/- [Inc/Cycle^2] commanded acceleration */
	MC_T_REAL			fCartCommandJerk;           /* +/- [Inc/Cycle^3] commanded jerk */
	MC_T_REAL			fCartRemainStep;            /* [Inc] Remainder package */
	MC_T_REAL			fCartTargetPosStep;         /* [Inc] change of target pos in current cycle */
	MC_T_REAL			fCartLastTargetPosStep;     /* copy of lTargetPosStep just for logging */
	MC_T_REAL			fCartLastCommandAcc;        /* for calc of jerk */

	MC_T_REAL			fRpyCurVel;                /*rpy�ǵ��ٶ�*/
	MC_T_REAL			fRpyAccCycle;				/*rpy�ǵļ��ٶ�*/
	MC_T_REAL			fRpyMaxVelCycle;
	MC_T_REAL			fRpyAcc;
	MC_T_REAL			fRpyMaxDiff;
	MC_T_REAL			fRollDiff;
	MC_T_REAL			fPitchDiff;
	MC_T_REAL			fYawDiff;
	bool				bRpyRamp;

	MC_T_DWORD          dwTotalMoveCycles;      /* total cycles for move */
	MC_T_DWORD          dwDoneCycles;           /* Done cycles since start of move */
	MC_T_BOOL           bRemainStepCalc;
	MC_T_BOOL           bEndless;               /* if true, endless move */
	MC_T_BOOL           bContUpdate;            /* Continuous Update */
	MC_T_REAL           fOrgMaxVelocity;        /* Original/inital max velocity when move was calculated */
	MC_T_REAL           fOrgEndVelocity;        /* Original/inital end velocity when move was calculated */
	MC_T_REAL           fOrgAcceleration;       /* Original/inital acc when move was calculated */
	MC_T_REAL           fOrgDeceleration;       /* Original/inital dec when move was calculated */
	MC_T_REAL           fOrgJerk;               /* Original/inital jerk when move was calculated */
	MC_T_REAL			fOrgDist;
	MC_T_POS_CALC       lOrgTargetPosition;     /* Original/inital target position when move was calculated */
	MC_T_TRAJECT_STATE  eTrajectState;          /* Status of trajectory generator */
};


/* Reference to the axes group */
struct AXES_GROUP_REF
{
	MC_T_AXIS_REF   	*RobotAxis[ROBOT_AXES_NUM];
	MC_T_AXIS_REF		*AdditionalAxis[ADDITIONAL_AXES_NUM];
};

//Jari
EC_T_VOID				RpyToMat(const RPY *prpy, RMATRIX *pmatrix);
EC_T_VOID               MatToRpy(const RMATRIX *pmatrix, RPY *prpy);
MC_T_INT				Inverse(const CARTPOS *ppos, const LINK *plink, JOINT *pjoint);
EC_T_VOID				AngleToEncoder(const MC_T_REAL ang_array[6], int enc_array[6]);
MC_T_INT				checkAngle(const JOINT *pjnt, const int PntNum, JOINT *pObjJnt);
MC_T_BOOL				checkJntContinuity(const JOINT *m_pJnt1, const JOINT *m_pJnt2);
EC_T_VOID				Encoder2DHAngle(const int m_enc[6], JOINT *m_jonit);
EC_T_VOID				DHAngle2Encoder(const JOINT *m_pDHAngle, JNTS_ENC *m_pEncoder);
CARTPOS                 Forward(const JOINT *pjoint, const LINK *plink);
MC_T_INT                selectSolutionByMinMove(const JOINT *m_pAllSolutionJ, const int m_solutionNum, const JOINT *m_pReferJ, JOINT *m_pSelectJ);
MC_T_INT                SafeDataCheck(const JOINT *pjnt);
MC_T_INT                invCart2Enc(CARTPOS *pdPos,JOINT *pRefJoint,JNTS_ENC *pEncoder);

CARTPOS					getToolPos(_tcp *t);
void					getTpInverse();

CARTPOS					MultMatVec(RMATRIX R, Pos3d pos);
CARTPOS					MultMatVec(RMATRIX R, Pos3d pos, CARTPOS baseCart);
CARTPOS					MultMatVec(RMATRIX R, CARTPOS pos, CARTPOS baseCart);

void					calcSafeCube();
bool					checkSafeBorder();
void					GetNewInst();//��ȡ�µ�ָ��
#ifdef _3D_SEWING_
void					SewStart();
bool					SewHome();//ǰ�����ⲿ�����
bool					SewHomeAll();//�����ⲿ�����
bool					SewStop();
bool					DropNeedle();
bool					RotateWorks();
void					AutoSew();
void					ManualSew();
#endif

EC_T_WORD				MC_SetRobDrivePosition(AXES_GROUP_REF *pAxesGroup);
EC_T_WORD				MC_GetRobDrivePosition(AXES_GROUP_REF *pAxesGroup);


/* PTP */
class MC_MoveDirectAbsolute
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_BOOL           ContinuousUpdate;        /* IN(E): Continuous Update (Trapezoid profile only) */
	MC_T_REAL           fDistance[ROBOT_AXES_NUM];     // �����ؽڵ�Ŀ��λ�� 
	MC_T_REAL			fMaxDistance;
	MC_T_INT64          Encoder[ROBOT_AXES_NUM];       // �ؽڱ�����ֵ
	MC_T_INT64			lDistance[ROBOT_AXES_NUM];     // [Inc]�����ؽڵ��ƶ�����
	MC_T_INT64			lDist3to4;               // ���ٶξ���
	MC_T_INT64			lRemainDist;             // ʣ�����
	MC_T_INT64			lTotalDist;
	MC_T_REAL           fDist3to4;
	MC_T_REAL			fRemainDist;
	MC_T_REAL			fTotalDist;
	MC_T_VELACC         lAxisAcc[ROBOT_AXES_NUM];
	MC_T_VELACC         lAxisVel[ROBOT_AXES_NUM];
	MC_T_VELACC			lMaxVel;
	MC_T_VELACC			lMaxAcc;
	MC_T_VELACC			lConstantVel;
	MC_T_VELACC			lTarVel;
	MC_T_VELACC			lNextVel;
	MC_T_VELACC			lCurVel;
	MC_T_VELACC			lEndVel;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	MC_T_WORD           wErrorID;       /* OUT: Error identification */

	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	JNTS_ENC			startPosEnc;
	JNTS_ENC			endPosEnc;

	CARTPOS				startPos;
	CARTPOS				endPos;

	double				a0,a1,a2,a3,a4,a5;//����ʽϵ��
	double				theata_0,theata_f;//��ֹ�Ƕ�
	double				tf;//����ʱ��
	double				dJnt;//�ؽڽ�����
	double				t;//ÿһʱ��
	RC_T_JNTS			jntsStart;	//��ʼ�ؽڽ�
	EC_T_INT64			lstartEnc[ROBOT_AXES_NUM];
	double				t_stop;
	double				dist_stop;
	ePtp				currVelState;

	double              frampDist,J,acc,vel;
	int					nCycleCv;
	int					nCycleCacc;
	bool				bHalf;
	bool				bChangeJerk;
	RC_T_JNTS			jntsTarget;
	int					nHalfRamp;//���б�¶����õ�������
	bool				bPreDone;//��������
	MC_MoveDirectAbsolute(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		ContinuousUpdate(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		bContUpdate(static_cast<const MC_T_BOOL &>(ContinuousUpdate)),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */
	const MC_T_BOOL     &bContUpdate;    /* IN: Continuous Update (only trapeze) */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

/* Line Absolute */
class MC_MoveLinearAbsolute
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL			bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL			bCommandAborted;  

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_BOOL           ContinuousUpdate;        /* IN(E): Continuous Update (Trapezoid profile only) */
	CARTPOS				startPos;                //ֱ�Ӹ�������ͨ����ȡ��ǰ������ֵ��ȡ��
	CARTPOS             endPos;
	CARTPOS				pdPos;
	CARTPOS				refPos;
	MC_T_INT	        nSolutionNum;            //��ʾ�ڼ���� 
	MC_T_REAL           fDistance;              
	MC_T_INT64          Encoder[ROBOT_AXES_NUM];       // �ؽڱ�����ֵ
	MC_T_REAL           fDist3to4;
	MC_T_REAL			fRemainDist;
	MC_T_REAL			fTotalDist;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	JNTS_ENC			startPosEnc;
	JNTS_ENC			endPosEnc;
	MC_T_WORD           wErrorID; 
	bool				bChangeAcc;

	RC_T_POS            posTarget;

	int					nAdditAxisMoveCycle;
	int					nAdditAxisMovePeriod;
	int					nAdditP1;
	int					nAdditP2;
	int					nAdditP3;
	int					nAdditP4;
	int					nAdditP5;
	int					nAdditP6;
	int					nAdditPtt;
	float				fAdditAxisMoveDist;
	float				fAdditJerk;
	float				fAdditAcc;
	//FILE				*fp;

	bool				bTrans;
	float				fRtrans;//���ɰ뾶
	float				fTransDist;
	int					ovlRel;
	bool				bUseStartPosFound;
	bool				bUseEndPosFound;

	MC_MoveLinearAbsolute (AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		ContinuousUpdate(0),
		bDone(0),  
		bBusy(0),
		bTrans(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		bContUpdate(static_cast<const MC_T_BOOL &>(ContinuousUpdate)),

		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */
	const MC_T_BOOL     &bContUpdate;    /* IN: Continuous Update (only trapeze) */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();

} ;

/* Circ */
class MC_MoveCircle
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL			bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL			bCommandAborted;  

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_BOOL           ContinuousUpdate;        /* IN(E): Continuous Update (Trapezoid profile only) */
	CARTPOS				startPos;                
	CARTPOS				midPos;
	CARTPOS             endPos;
	CARTPOS				pdPos;
	CARTPOS				refPos;
	MC_T_INT	        nSolutionNum;            //��ʾ�ڼ���� 
	MC_T_REAL           fDistance;              
	MC_T_INT64          Encoder[ROBOT_AXES_NUM];       // �ؽڱ�����ֵ
	MC_T_REAL           fDist3to4;
	MC_T_REAL			fRemainDist;
	MC_T_REAL			fTotalDist;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	JNTS_ENC			startPosEnc;
	JNTS_ENC			endPosEnc;
	MC_T_WORD           wErrorID; 

	RC_T_POS            posTarget;

	int					nCycle;
	CARTPOS				pos1;
	CARTPOS				pos2;
	CARTPOS				pos3;
	RMATRIX				TZ;
	RMATRIX				SewMat;
	double				x0,y0,x1,y1,x2,y2,x3,y3,x,y,px0,py0,pz0;
	double				R,Alpha,dAlpha,alpha1,alpha3;
	double				totalDist;

	//FILE				*fp;

	bool				bTrans;

	MC_MoveCircle(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		ContinuousUpdate(0),
		bDone(0),  
		bBusy(0),
		bTrans(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		bContUpdate(static_cast<const MC_T_BOOL &>(ContinuousUpdate)),

		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */
	const MC_T_BOOL     &bContUpdate;    /* IN: Continuous Update (only trapeze) */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();

} ;

class _MC_API MC_GROUP_POWER
{
public:

	// OUT's
	const MC_T_BOOL     &Status;                 /* OUT(B): Effective state of the power stage */
	const MC_T_BOOL     &Valid;                  /* OUT(E): If TRUE a valid set of outputs is available */
	const MC_T_BOOL     &Error;
	MC_T_BOOL           bError ;
	MC_T_WORD			wErrorID;
	// IN's
	MC_T_BOOL           Enable;                  /* IN(B): As long as is true, power is on */
	MC_T_BOOL           EnablePositive;          /* NOT SUPP: IN(E): As long as is true, permits motion in positive direction only */
	MC_T_BOOL           EnableNegative;          /* NOT SUPP: IN(E): As long as is true, permits motion in negative direction only */

	MC_T_AXIS_REF		*pwrAxis;

	MC_GROUP_POWER(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Status(static_cast<const MC_T_BOOL &>(bStatus)),
		Valid(static_cast<const MC_T_BOOL &>(bValid)),
		Error(static_cast<const MC_T_BOOL &>(bError)),
		// Init variables
		Enable(0), 
		EnablePositive(0),
		EnableNegative(0),
		bStatus(0),
		bValid(0),
		// Init internal IN's
		bEnable(static_cast<const MC_T_BOOL &>(Enable)),
		bEnable_Positive(static_cast<const MC_T_BOOL &>(EnablePositive)),
		bEnable_Negative(static_cast<const MC_T_BOOL &>(EnableNegative)),
	    pwrAxis((MC_T_AXIS_REF*)malloc(sizeof(MC_T_AXIS_REF)))
	{}

	void _MC_THIS_API OnCycle(); 

private:

	MC_T_BOOL           bStatus;                 /* OUT: Effective state of the power stage */
	MC_T_BOOL           bValid;                  /* OUT: If TRUE a valid set of outputs is available */

	const MC_T_BOOL     &bEnable;                /* IN: As long as is true, power is on */
	const MC_T_BOOL     &bEnable_Positive;       /* NOT SUPP: IN: As long as is true, permits motion in positive direction only */
	const MC_T_BOOL     &bEnable_Negative;       /* NOT SUPP: IN: As long as is true, permits motion in negative direction only */

	ASSIGNOP(MC_GROUP_POWER);
	void DS402StateMachine(MC_T_AXIS_REF *pAxis);
	void InitAxis();

};

class WaitTime
{
public:
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  

	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_WORD           wErrorID;       /* OUT: Error identification */

	MC_T_REAL			time;
	MC_T_INT			count;

	WaitTime()
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

/* Motion Control Function Blocks (MCFB's) 
* This structure contains C++ objects. Don't initialize to zero!
*/
class EC_T_FUNCTION_BLOCKS
{
public:

	MC_T_AXIS_INIT                    AxisInitData;            // MCFB axis initialization data
	MC_T_AXIS_REF                     Axis;                    // MCFB axis reference

	MC_POWER_T                        Power;
#if defined(EC_MOTION_TRAJECTORY_GEN)
	MC_STOP_T                         Stop;
	MC_HALT_T                         Halt;
#endif /* defined(EC_MOTION_TRAJECTORY_GEN) */
	MC_RESET_T                        Reset;
#if defined(EC_MOTION_TRAJECTORY_GEN)
	MC_MOVE_RELATIVE_T                MoveRelative;
	MC_MOVE_ABSOLUTE_T                MoveAbsolute;
	MC_MOVE_VELOCITY_T                MoveVelocity;
#ifdef _3D_SEWING_
	MC_POWER_T                        SpindlePower;
	MC_MOVE_RELATIVE_T                SpindleMoveRelative;
	MC_MOVE_ABSOLUTE_T                SpindleMoveAbsolute;
	MC_MOVE_VELOCITY_T                SpindleMoveVelocity;
	MC_STOP_T						  SpindleStop;

	MC_POWER_T                        SwayPower;
	MC_MOVE_RELATIVE_T                SwayMoveRelative;
	MC_MOVE_ABSOLUTE_T                SwayMoveAbsolute;
	MC_MOVE_VELOCITY_T                SwayMoveVelocity;
	MC_STOP_T						  SwayStop;

	MC_POWER_T                        WorksPower;
	MC_MOVE_RELATIVE_T                WorksMoveRelative;
	MC_MOVE_ABSOLUTE_T                WorksMoveAbsolute;
	MC_MOVE_VELOCITY_T                WorksMoveVelocity;
	MC_STOP_T						  WorksStop;
#endif
#endif /* defined(EC_MOTION_TRAJECTORY_GEN) */
	MC_READ_ACTUAL_POSITION_T         ReadActualPosition;
	MC_READ_PARAMETER_T               ReadParameter;
	MC_READ_BOOL_PARAMETER_T          ReadBoolParameter;
	MC_WRITE_PARAMETER_T              WriteParameter;
	MC_WRITE_BOOL_PARAMETER_T         WriteBoolParameter;
#if defined(EC_MOTION_TRAJECTORY_GEN)
	MC_READ_MOTION_STATE_T            ReadMotionState;
#endif /* defined(EC_MOTION_TRAJECTORY_GEN) */
	MC_READ_AXIS_INFO_T               ReadAxisInfo;
	AMC_CHECK_TARGETPOS_REACHED_T     CheckTargetposReached;
	AMC_HALT_RECOVERY_T               HaltRecovery;
#ifdef EC_MOTION_SUPPORT_PP_MODE
	MC_HOME_T                         Home;
#endif
	MC_READ_DIGITAL_INPUT_T           ReadDigitalInput;
	MC_READ_DIGITAL_OUTPUT_T          ReadDigitalOutput;
	MC_WRITE_DIGITAL_OUTPUT_T         WriteDigitalOutput;
private:

	EC_T_FUNCTION_BLOCKS & operator=(const EC_T_FUNCTION_BLOCKS &) { return *this; }
};

/* data for one axis */
typedef struct  
{
	/* read from DemoConfig.xml */
	EC_T_WORD                    wDriveFixAddress;       
	EC_T_BYTE                    byDriveModesOfOperation;
	EC_T_BYTE                    bySercosDriveNo;
	EC_T_DWORD                   dwCoeIdxOpMode;
	EC_T_DWORD                   dwDriveJerk;
	EC_T_DWORD                   dwDriveAcc;
	EC_T_DWORD                   dwDriveDec;
	EC_T_DWORD                   dwDriveVel;
	EC_T_DWORD                   dwDriveDistance;
	EC_T_DWORD                   dwDriveIncPerMM;
	EC_T_DWORD                   dwDriveIncFactor;
	EC_T_DWORD                   dwPosWindow;
	EC_T_DWORD                   dwVelocityGain;        /* CSV-Mode: Velocity Gain */
	/* CSP-Mode: Velocity Gain for Feed Forward Object 0x60B1 */  
	MC_T_DWORD                   dwTorqueGain;          /* CSP and CSV-Mode: Torque Gain for Feed Forward Object 0x60B2 */  
	EC_T_VARIDX                  IdxStatusword;         /* default for DS402: DRV_OBJ_STATUS_WORD                 0x6041 */
	EC_T_VARIDX                  IdxControlword;        /* default for DS402: DRV_OBJ_CONTROL_WORD                0x6040 */
	EC_T_VARIDX                  IdxActualPos;          /* default for DS402: DRV_OBJ_POSITION_ACTUAL_VALUE       0x6064 */
	EC_T_VARIDX                  IdxTargetPos;          /* default for DS402: DRV_OBJ_TARGET_POSITION             0x607A */
	EC_T_VARIDX                  IdxTargetVel;          /* default for DS402: DRV_OBJ_TARGET_VELOCITY             0x60FF */
	EC_T_VARIDX                  IdxTargetTrq;          /* default for DS402: DRV_OBJ_TARGET_TORQUE               0x6071 */
	EC_T_VARIDX                  IdxActualTrq;          /* default for DS402: DRV_OBJ_ACTUAL_TORQUE               0x6077 */
	EC_T_VARIDX                  IdxVelOffset;          /* default for DS402: DRV_OBJ_VELOCITY_OFFSET             0x60B1 */
	EC_T_VARIDX                  IdxTorOffset;          /* default for DS402: DRV_OBJ_TORQUE_OFFSET               0x60B2 */
	EC_T_VARIDX                  IdxModeOfOperation;    /* default for DS402: DRV_OBJ_MODES_OF_OPERATION          0x6060 */
	EC_T_VARIDX                  IdxModeOfOperationDisplay;    /* default for DS402: DRV_OBJ_MODES_OF_OPERATION_DISPLAY          0x6061 */

	MC_T_AXIS_PROFILE            eDriveProfile;

	/* EtherCAT info */
	EC_T_DWORD                   dwSlaveId;
	EC_T_WORD                   *pwPdStatusWord;         /* Drive status word */
	EC_T_WORD                   *pwPdControlWord;        /* Drive control word */

	EC_T_SDWORD                 *plPdActualPosition;     /* ptr to actual position in process data */
	/* Data type DINT (INTEGER32 ?147483648 to +2147483627) */
	EC_T_SDWORD                 *plPdTargetPosition;     /* ptr to target position: Data type DINT (INTEGER32 ?147483648 to +2147483627)
														 In Cyclic Synchronous Position mode, it is always interpreted as an absolute value */

	EC_T_SDWORD                 *plPdTargetVelocity;     /* ptr to target velocity: Data type DINT (INTEGER32 ?147483648 to +2147483627) */

	EC_T_SDWORD                 *plPdVelocityOffset;     /* ptr to velocity offset for feed forward: Data type DINT (INTEGER32 ?147483648 to +2147483627) */
	EC_T_SWORD                  *psPdTorqueOffset;       /* ptr to torque offset for feed forward: Data type INT (INTEGER16 ?2768 to +32767) */

	EC_T_SWORD                  *psPdActualTorque;       /* ptr to actual torque: Data type INT (INTEGER16 ?2768 to +32767) */
	EC_T_SWORD                  *psPdTargetTorque;       /* ptr to target torque: Data type INT (INTEGER16 ?2768 to +32767) */
	EC_T_BYTE                   *pbyPdModeOfOperation;   /* ptr to mode of operation (DS402 0x6060) */
	EC_T_BYTE                   *pbyPdModeOfOperationDisplay;   /* ptr to mode of operation display (DS402 0x6061) */

	EC_T_DWORD                  *pdwDigInputs;
	EC_T_DWORD                  *pdwDigOutputs;

#ifdef EC_MOTION_SUPPORT_PP_MODE
	EC_T_DWORD                  *plPdProfileVelocity;    /* ptr to profile velocity for PP mode and homing mode */
	EC_T_DWORD                  *plPdProfileAcc;         /* ptr to profile acc for PP mode and homing mode */
	EC_T_DWORD                  *plPdProfileDec;         /* ptr to profile dec for PP mode and homing mode */
#endif

	/* MCFB data */
	EC_T_FUNCTION_BLOCKS        *pFb;
	EC_T_BOOL                    bTriggerMoveRel;
	EC_T_BOOL                    bTriggerMoveAbs;
	EC_T_BOOL                    bTriggerMoveVelo;
	EC_T_BOOL                    bTriggerCheckPos;
	EC_T_BOOL                    bTorqueLimitActive;

	/* helpers for autonomous mode */
	EC_T_INT                     eDistanceState;
	eMoveState                   eMotionState;
	EC_T_INT                     nWaitCounter1;

	/* Logging */
	EC_T_LOGINFO                 log;
} EC_T_DEMO_AXIS;


// typedef struct 
// {
// 	MC_MoveLinearAbsolute Lin1;
// 	MC_MoveLinearAbsolute Lin2;
// }LinTrans;

class MC_MoveTrigle
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL			bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL			bCommandAborted;  
	bool				bRemainInit;
	int					Nremain;
	int					nRemainNum;

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_BOOL           ContinuousUpdate;        /* IN(E): Continuous Update (Trapezoid profile only) */
	CARTPOS				startPos;                //ֱ�Ӹ�������ͨ����ȡ��ǰ������ֵ��ȡ��
	CARTPOS             endPos;
	CARTPOS				pdPos;
	CARTPOS				refPos;
	CARTPOS				posWeld;
	MC_T_INT	        nSolutionNum;            //��ʾ�ڼ���� 
	MC_T_REAL           fDistance;              
	MC_T_INT64          Encoder[ROBOT_AXES_NUM];       // �ؽڱ�����ֵ
	MC_T_REAL           fDist3to4;
	MC_T_REAL			fRemainDist;
	MC_T_REAL			fTotalDist;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	JNTS_ENC			endPosEnc;	
	MC_T_WORD           wErrorID; 

	WeldTime			Wetime;
	Vector				PF;
	Tpara               tpa;
	RMATRIX				matWeld;
	MC_T_INT			N1;
	MC_T_INT			N2;
	MC_T_INT			N3;
	MC_T_INT			N4;
	MC_T_INT			N5;
	MC_T_INT			Nx;
	MC_T_INT			Nk;
	MC_T_INT			N;
	MC_T_INT			nCycleNum;
	MC_T_INT			nSend;//test

	RC_T_POS            posTarget;

	MC_MoveTrigle (AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		ContinuousUpdate(0),
		bDone(0),  
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		bContUpdate(static_cast<const MC_T_BOOL &>(ContinuousUpdate)),

		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */
	const MC_T_BOOL     &bContUpdate;    /* IN: Continuous Update (only trapeze) */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();

} ;

/* �ؽ�����ϵʾ�� */
class MC_JogAxes
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_VELACC         lAxisAcc[ROBOT_AXES_NUM];
	MC_T_VELACC         lAxisVel[ROBOT_AXES_NUM];
	MC_T_VELACC			lMaxVel;
	MC_T_VELACC			lMaxAcc;
	MC_T_VELACC			lConstantVel;
	MC_T_VELACC			lTarVel;
	MC_T_VELACC			lNextVel;
	MC_T_VELACC			lCurVel;
	MC_T_VELACC			lEndVel;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	MC_T_WORD           wErrorID;       /* OUT: Error identification */

	KEY_SIGN			keySign;
	MC_T_BOOL			bKeyState;
	MC_T_INT			nKeyNum;

	MC_T_REAL			jntAngle;

	MC_T_INT			nJogTest;

	MC_JogAxes(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute))
	{}

	void  OnCycle(); 

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

/* �ѿ����ռ�ʾ�� */
class MC_JogCart
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  

	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	MC_T_VELACC         lAxisAcc[ROBOT_AXES_NUM];
	MC_T_VELACC         lAxisVel[ROBOT_AXES_NUM];
	MC_T_VELACC			lMaxVel;
	MC_T_VELACC			lMaxAcc;
	MC_T_VELACC			lConstantVel;
	MC_T_VELACC			lTarVel;
	MC_T_VELACC			lNextVel;
	MC_T_VELACC			lCurVel;
	MC_T_VELACC			lEndVel;
	MC_T_REAL			fMaxAcc;
	MC_T_REAL			fMaxVel;
	MC_T_REAL			fConstantVel;
	MC_T_WORD           wErrorID;       /* OUT: Error identification */

	KEY_SIGN			keySign;
	MC_T_BOOL			bKeyState;
	MC_T_INT			nKeyNum;

	MC_T_REAL			jntAngle;

	CARTPOS				startPos;
	CARTPOS				pdPos;
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	JOINT				tempj;
	MC_JogCart(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

class MC_Trans
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  
	MC_T_WORD			wErrorID;
	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	int					nCycle;
	CARTPOS				pos1;
	CARTPOS				pos2;
	CARTPOS				pos3;
	CARTPOS				pdPos;
	CARTPOS				endPos;
	CARTPOS				startPos;
	RMATRIX				TZ;
	double				x0,y0,x1,y1,x2,y2,x3,y3,x,y;
	double				R,Alpha,dAlpha,alpha1,alpha3;
	RMATRIX				SewMat;
	double				px0,py0,pz0;
	double				d1,d2,d3,theta13,theta12;
	double				totalDist;
	bool				notTrans;


	//������ʽ��ֵ��̬��
	double				omega0[3],omega1[3]; //��̬�ǳ�ʼ�ٶȺͽ����ٶ�
	double				a0[3],a1[3],a2[3],a3[3];  //����ʽϵ��
	double				Ttrans;       //trans���ܿ��ִ��ʱ�䣬����̬�ǲ岹��Ҫ��ʱ��
	double				Tcycle;		//������ʱ��
	//����...���ζ���ʽ��ֵ��̬��


	MC_Trans(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

class MC_CircT
{
public:

	// OUT's
	const MC_T_BOOL     &Done;                   /* OUT(B): The axis is within a range close to the target position */
	const MC_T_BOOL     &Busy;                   /* OUT(E): The FB is not finished and new output values are to be expected */
	MC_T_BOOL           bError; 
	MC_T_FBSTATE        fbState;
	MC_T_BOOL           bActive;                           /* OUT: Indicates that the FB has control on the axis */
	MC_T_BOOL           bCommandAborted;  
	MC_T_WORD			wErrorID;
	// IN's
	MC_T_BOOL           Execute;                 /* IN(B): Start the motion at rising edge */
	JOINT				*pJoint;
	JOINT				*m_pReasJoint;
	JOINT				*m_pSelectJoint;
	JOINT				*pRefJoint;
	JNTS_ENC			*pEncoder;
	int					nCycle;
	CARTPOS				pos1;
	CARTPOS				pos2;
	CARTPOS				pos3;
	CARTPOS				pdPos;
	CARTPOS				endPos;
	CARTPOS				startPos;
	RMATRIX				TZ;
	double				x0,y0,x1,y1,x2,y2,x3,y3,x,y;
	double				R,Alpha,dAlpha,alpha1,alpha3;
	RMATRIX				SewMat;
	double				px0,py0,pz0;
	double				d1,d2,d3,theta13,theta12;
	double				totalDist;
	bool				notTrans;

	MC_CircT(AXES_GROUP_REF *pAxes = MC_NULL)
		: 
	Done(static_cast<const MC_T_BOOL &>(bDone)),
		Busy(static_cast<const MC_T_BOOL &>(bBusy)),
		// Init variables
		Execute(0),
		bDone(0), 
		bBusy(0),
		// Init internal IN's
		bExecute(static_cast<const MC_T_BOOL &>(Execute)),
		pJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pReasJoint((JOINT *)malloc(sizeof(JOINT)*8)),
		m_pSelectJoint((JOINT *)malloc(sizeof(JOINT))),
		pRefJoint((JOINT *)malloc(sizeof(JOINT))),
		pEncoder((JNTS_ENC *)malloc(sizeof(JNTS_ENC)))
	{}

	void  OnCycle(); 
	//void ChangeState(MC_T_FBSTATE newState);

private:

	MC_T_BOOL           bDone;           /* OUT: The axis is within a range close to the target position */
	MC_T_BOOL           bBusy;           /* OUT: The FB is not finished and new output values are to be expected */

	const MC_T_BOOL     &bExecute;       /* IN: Start the motion at rising edge */

	void StateTrans_Execute();
	void StateTrans_Active();
	void DoState_Active();
	void DoState_Busy();
	MC_T_BOOL StateMachine();
} ;

enum eSetInstType
{
	WAVEPLANE,
	IO,
	SEWPLANE,
	SEWSTART,
	SEWSTOP,
	SEWHOME,
	SETTCP,
	SETBOUND,
	REMOVEBOUND,
	DROPNEEDLE,
	ROTATEWORKS,
	DPULSE,
	CALL,
	SEWCUT,
	FOOTUP,
	FOOTDOWN,
	LASERTRACK,
	WAVE,
	WAVE3D,
	WAVEL,
	WAVEEND,
	SEARCHGAP,
	SEARCHSTARTPOS,
	SEARCHENDPOS,
	WAVECHR,
	ARCON,
	ARCOFF,
	CAMON,
	GENFILE,
	CLOSEFILE
};

enum eBlockType
{
	AIR,
	PTP,
	LIN,
	CIRC,
	TRANS,
	MOVETRIGLE,
	WAIT,
	SET,
	JOGAXES,
	JOGCART//��ֹstop��ִ��jog���ܵ����ٴ�����lin���ܿ����
};

//���տ�����
struct WeldTech
{
	//UINT	uID;
	char	Name[32];
	//���Ӆ���
	float	fCurrent;
	float	fVolt;
	float	fWireSpeed;
	//�[�Ӆ���
	float	fFre;
	float	fAmp;
	float	fLStay;
	float	fMStay;
	float	fRStay;
};

struct WeldPara
{
	float		fAmp;
	float		fFeq;
	float		fSpeed;
	float		fRAngle;   //��ƽ����ת�Ƕ�
	float		fTotalTime;
	float		fTimeStayL;
	float		fTimeStayR;
	float		fVolt;//��ѹ
	float		fElecCur;//����
	bool		bStayStill;
	eWaveType	waveType;
	RMATRIX		waveFrame;
	int			N1;
	int			N2;
	int			N3;
	int			N4;
	int			N5;
	int			N6;
	int			N7;
	int			N8;
	int			Nx;
	int			Nk;
	int			N;
	int			nCycleNum;
	CARTPOS		wavePos;
	bool		bStilling;
	float		totalPx;
	WeldTech	wTech;
	int			nWelderTaskId;
	//ʥ�����������
	float   fBackDist;//���˾���
	float   fHriVel;//ˮƽ�ƶ����ٶ�
	float   fVel;//ǰ���������ٶ�
	CARTPOS	planeCalcPos;//����ƽ�淨����Ҫ�Ĳο���
	//б�ڶ��������
	float   fVleft;//��б���ٶ�
	float	fVright;//��б���ٶ�
	float	fWaveDist;//ÿ���ڰڶ�ǰ���ľ���
	float	fLeanAngle;//��б�Ƕ�

	bool    bWeld;
};

class SetInst  
{
public:
	eSetInstType			type;
	bool					bIO;
	bool					bInitOut;
	bool					bInitIn;
	int 					ioSetId;
	bool					bplane;
	bool					Execute;
	int						nCmdTcpType;
	int						nCallNum;
#ifdef _3D_SEWING_
	bool					bSewPlane;
	bool					bSewCut;
#endif
	bool					bCamOn;
	bool					bLaserTrack;
	WeldTech				setWeldTech;
	int						nWelderTaskID;
	bool					bStayStill;
	//ʥ�����������
	float					fBackDist;//���˾���
	float					fHriVel;//ˮƽ�ƶ����ٶ�
	CARTPOS					planeCalcPos;//����ƽ�淨����Ҫ�Ĳο���
	bool					bWeld;

	float					fLeanAngle;//��б�Ƕ�

	char					tcpName[MAX_CHAR_INTP];
	char					fileName[MAX_CHAR_INTP];
	char					cubeName[MAX_CHAR_INTP];
	int						nGenFileCycle;
	SetInst() {}
	virtual ~SetInst() {}
};

struct MC_BufferBlock{
	eBlockType				type;//���ܿ�����
	MC_MoveDirectAbsolute	PTP;
	MC_MoveLinearAbsolute	Lin;
	MC_MoveCircle			Circ;
	MC_MoveTrigle			MoveTrigle;
	WaitTime				Wait;
	MC_Trans				Trans;
	SetInst					setInst;			
};


#ifdef _3D_SEWING_
struct SewCalcPara{
	float	swayHookDist;			//���鹴�ߵ������ľ��루Ȧ����
	float	swayHomeDist;			//���鵱ǰ�������ľ��루Ȧ����
	float   spindleHomeDist;		//���ᵱǰ�������ľ��루Ȧ����
	float   rotateHomeDist;			//��װ̨��ǰ�������ľ��루Ȧ����
	int		swayHookEnc;			//���鹴�ߵ������ֵ
	int		swayHomeEnc;			//������������ֵ
	int		spindleHomeEnc;			//������������ֵ
	int		rotateHomeEnc;				//��װ̨��������ֵ
	float	swayStillProp;			//����ǰ�ھ�ֹʱ��ռ�ڶ����ڵı���
	float	swayMatchProp;			//����ƥ��������ٶȵ�ʱ��ռ�ڶ����ڵı���
	float   swayHome2HookProp;		//���������ߵ�ʱ��ռƥ��ʱ��ı���
	bool	bswayHome;
	bool	bspindleHome;
	bool    brotateHome;
	bool	bSliderDrop1;
	bool	bSliderDrop2;
	bool	bSpindleDrop;
	bool    bFirstCalcDrop;
	bool	bswayStop;
	bool	bspindleStop;
	bool	bswayBack;
	float	fSewSpeed;
	float   fSyncIn;
	float   fSyncOut;
	float   fSliderDropDist;
	float   fNeedleDropDist;
};

#endif

enum ErrorLevel
{
	GENERAL,
	MEDIUM,
	SERIOUS
};


enum SeamStatus
{
	NOT_USED,//δʹ��
	VALID_MEAS,//��Ч����
	INVALID_MEAS//��Ч����
};

enum LaserSearchType
{
	SEARCH_NOTHING,//ɶҲ����
	SEARCH_GAP,//Ѱ�Һ���
	SEARCH_STARTPOS,//Ѱ�����
	SEARCH_ENDPOS	//Ѱ���յ�
};

struct sLaserSearch
{
	float				fPy;//Ѱλ����py
	float				fPz;//Ѱλ����pz
	CARTPOS				robotPos;//����������ĩ��λ��
};

struct LaserTrack
{
	bool				bLaserTrack;//�Ƿ����������
	bool				bStartTracking;//��ʼ����....��Ҫ�ߵ���һ����ǰ����ȥһ���������ھ����λ��
	float				fScanPy[SCAN_BUFFER];//ɨ�����ݻ�����
	int					nScanWriteIndex;//������д������
	int					nScanReadIndex;//��������ȡ����
	int					nOffsetCalcNum;//�Ѿ������ƫ�ƴ���
	int					nRobotCommuNum;//������ͨ��������
	float				fNextOffset;//��һ��ͨ��������Ҫ���ӵ�ƫ��
	float				posY;
	float				posZ;
	int					nLastMeasStatus;//��һ�κ�����Ϣ״̬
	int					nMeasStatus;//���κ�����Ϣ״̬
	bool				bMeasFromValid2Invalid;//����״̬����Ч����Ч
	bool				bMeasFromInvalid2Valid;//����״̬����Ч����Ч
	bool				bMeasValid;
	int					nInspectCycle;//ѰλУ������
	LaserSearchType		eSearchType;//Ѱλ����
	bool				bPosFound;//�ҵ���
	CARTPOS				startPosFound;
	CARTPOS				endPosFound;
	SeamStatus			eSeamStatus;
	float				fTrueValue;
	float				fFakeValue;
	sLaserSearch		laserSearch[SCAN_BUFFER];
	int					nSearchWriteIndex;//������д������
	float				fLastSearchPy;
	int					nLastSearchIndex;
	bool				bFoundS;//�ҵ�����ʼ��
	bool				bFoundE;//�ҵ��˽�����
};

struct sLimitSafePos
{
	float xMax;
	float xMin;
	float yMax;
	float yMin;
	float zMax;
	float zMin;
};

//�γɰ�ȫ�������6������
struct s_fCubePara
{
	char name[16];
	float xp;   
	float yp;  
	float zp;
	float xm;   
	float ym;  
	float zm;
	float xMax;
	float xMin; 
	float yMax; 
	float yMin; 
	float zMax;	
	float zMin;
	float isX; 
	float isY; 
	float isZ;
};

struct s_AllCube  //�����弯��
{
	int num;
	s_fCubePara Cube[MAX_CUBE_NUM];
};

class Robot  
{
public:
	ROBOT_INFO				RobotInfo;//�����˽ṹ����
	MC_GROUP_MOVEMENT		Movement;//���浱ǰ���ں���һ���ڵ��˶�����
	EC_T_DEMO_AXIS          S_aAxisList[DEMO_MAX_NUM_OF_AXIS];
	eBlockType				eLastFb;

	MC_BufferBlock			bufferBlock[PRE_IPR_NUM];
	MC_MoveDirectAbsolute	PTP;
	MC_MoveLinearAbsolute	Lin;
	MC_MoveCircle			Circ;
	MC_CircT				CircT;
	MC_GROUP_POWER          rPower;
	MC_MoveTrigle			MoveTrigle;
	MC_JogAxes				JogAxes;
	MC_JogCart				JogCart;
	WaitTime				Wait;
	MC_Trans				Trans;
	SetInst					setInst;	

	EC_T_WORD				wBuffer;
	EC_T_WORD				wExecute;

	bool					bIO;
	bool					bInitOut;
	bool					bInitIn;
	bool					bWelderManualControl;
	bool					bFinished;
	bool					bBufferFull;
	bool					bExecuteNew;//����ִ���µ�ָ����
	bool					bExecute;
	bool					bPushNew;
	bool					bTrans;
	int						nReadyForPush;
	EC_T_WORD				wRemainInst;
	int 					ioSetId;
	int 					ioGetId;
	int						ioGetCnt;
	int						welderCtrlId;
	int						nWorksNum;
	bool					bReadyForRotate;
	bool					bSafeForRotate;
	bool					bIoGet;//��֤ͬһ�β�̤����ֻ�ܴ���һ��io���
	bool					bReverseIo;//״̬��ת
	int						nPrint;
	long long				nCycle;
	int						nCycleScan;
	CARTPOS					pdPos;
	CARTPOS					robotEndPos;
	bool					bplane;
	bool					bConstantVel;
	ErrorLevel				errLevel;
	FILE				    *robotFile;
	FILE					*scanPosFile;
	RTOSLIB_HANDLE			*tempScanFile;
	int						nScanTime;
	bool					bSewPlane;
	int						curLineNum;
	bool					camOn;
	bool					camOff;
	bool					bHardStop;
	bool					bCalling;
	int						nCallNum;
	int						nCallCnt;
	bool					bCanBeCalled;
	bool					bPause;
	bool					bGenFile;
	int						nGenFileCycle;
	bool					bSetBound;
	bool					isBoundSafe;
#ifdef _3D_SEWING_
	RMATRIX					sewMat;	
	RPY						sewRpy;
	int						nSway;
	int						nErrorMatch;
	float					fspindleDist;
	bool					bGetSwayParam;
	SEWING_PARA				sewPara;
	bool					bSetSewPara;	
	int						swayEncMinus;
	int						swayEncPlus;
	bool					bSewStart;
	bool					bSewStop;
	bool					bSewHome;
	bool					bDropNeedle;
	bool					bSewTest;
	bool					bHomeDone;
	bool					bStopDone;
	bool					bHardSewStop;
	bool					bRotateWorks;
	bool					bRotateDone;
	SewCalcPara				sewCalcPara;
#endif
	JOINT					actJnt;
	CARTPOS					actPos;
	WORK_SPACE				eCurWs;
	JOINT					teachJnt;
	long					addiEnc[ADDITIONAL_AXES_NUM];				
	CARTPOS					teachPos;

	CARTPOS					toolPos;
	_sTcp					allTcp;
	RMATRIX					tpInverse;
	int						nTcpIndex;
	int						nCubeIndex;

	JNTS_ENC				actEncoder;

	//Laser Track
	LaserTrack				laserTrack;
	WeldPara				weldpara;
	bool					bWave;
	WeldTech				sWeldTech[10][10];

	CARTPOS					cubePos[8];//�������8������ ������ĩ��Բ��Ϊ�ο���z+��1,2,3,4���ޣ�����z-��1,2,3,4���ޡ���x+��y+��z+��,��x-��y+��z+��,��x-��y-��z+��,��x+��y-��z+��,��x+��y+��z-��,��x-��y+��z-��,��x-��y-��z-��,��x+��y-��z-��...
	s_AllCube				allCube;
	s_fCubePara				sfCubePara;
	sLimitSafePos		    sLimitPos;

	EC_T_BOOL				bTriggerMoveLinRel;
	EC_T_BOOL				bTriggerMoveLinAbs;
	EC_T_BOOL				bTriggerMovePtp;
	EC_T_BOOL				bTriggerMoveVelo;
	EC_T_BOOL				bTriggerCheckPos;
	MC_T_VELACC				axis5InitEnc;
	MC_T_VELACC				axis6InitEnc;
	MC_T_REAL				axisJntPlus;

	AXES_GROUP_REF			Axes;
	//MC_T_AXIS_REF			RobotAxis[ROBOT_AXES_NUM];//revise_l
	MC_GROUP_PLCOPEN_STATE  ePLCOpenState;          /* PLCOpen state        */
	MC_GROUP_PLCOPEN_STATE  eLastPLCOpenState;      /* previous PLCOpen state (just for print state change)  */
	MC_T_DWORD              dwCycleCount;           /* just for logging purpose */

	MC_T_INT                dwCalcIncPerMM;         /* calculation (internal) increments per mm */
	MC_T_REAL               fVelToInc;              /* conversion factor from mm/s to inc/cycle */
	MC_T_REAL               fAccToInc;              /* conversion factor from mm/s^2 to inc/cycle^2 */
	MC_T_REAL               fJerkToInc;             /* conversion factor from mm/s^3 to inc/cycle^3 */

	MC_T_INT                lVeltoAcc;              /* conversion factor from inc/cycle^2 to inc/cycle^3 */

	MC_T_BOOL               bError;                 /* Indicates if an error has occurred */
	MC_T_WORD               wErrorID;               /* Error identification */
	OUT_RANGE_JNT			outRangeJnt;
	MC_T_INT                dwCycleTime;            /* [usec] EtherCAT bus cycle time */
	MC_T_INT                dwIncPerMM;             /* Increments per mm */

public:
	EC_T_BOOL				initRobotFile();
	void					InitRobotState();
	//��¼�����˵�ǰλ����־ 
	EC_T_ROB_LOGINFO        robInfo;

public:
	Robot() {}
	virtual ~Robot() {}
};

extern Robot robot;//jari

#endif