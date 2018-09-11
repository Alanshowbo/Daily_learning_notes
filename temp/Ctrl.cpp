
#define ECMOTIONLIB_BUILD

/*-INCLUDES------------------------------------------------------------------*/
#include "EcOs.h"

#include <math.h>
#include <limits.h>
#include "AtEthercat.h"
#include "JRCtrl.h"
#include "AtXmlParser.h"
#include "string.h"
using namespace std;

//#include <rtosstdio.h>
//JariRobot Functions

//rpy角转旋转矩阵*
EC_T_VOID RpyToMat(const RPY *prpy, RMATRIX *pmatrix)
{
	pmatrix->Nx = cos(prpy->Pitch)*cos(prpy->Yaw);
	pmatrix->Ny = cos(prpy->Pitch)*sin(prpy->Yaw);
	pmatrix->Nz = -sin(prpy->Pitch);

	pmatrix->Ox = sin(prpy->Roll)*sin(prpy->Pitch)*cos(prpy->Yaw) - cos(prpy->Roll)*sin(prpy->Yaw);
	pmatrix->Oy = sin(prpy->Roll)*sin(prpy->Pitch)*sin(prpy->Yaw) + cos(prpy->Roll)*cos(prpy->Yaw);
	pmatrix->Oz = sin(prpy->Roll)*cos(prpy->Pitch);

	pmatrix->Ax = cos(prpy->Roll)*sin(prpy->Pitch)*cos(prpy->Yaw) + sin(prpy->Roll)*sin(prpy->Yaw);
	pmatrix->Ay = cos(prpy->Roll)*sin(prpy->Pitch)*sin(prpy->Yaw) - sin(prpy->Roll)*cos(prpy->Yaw);
	pmatrix->Az = cos(prpy->Roll)*cos(prpy->Pitch);		
}

//旋转矩阵转rpy角*
EC_T_VOID MatToRpy(const RMATRIX *pmatrix, RPY *prpy)
{
	prpy->Yaw = atan2(pmatrix->Ny,pmatrix->Nx);	//gama
	prpy->Pitch = atan2(-pmatrix->Nz, sqrt(pow(pmatrix->Nx,2)+pow(pmatrix->Ny,2)));	//beta
	prpy->Roll = atan2(pmatrix->Oz,pmatrix->Az); //alpha	
}

//运动学正解,返回空间坐标中末段点坐标
CARTPOS Forward(const JOINT *pjoint, const LINK *plink)
{
	double s1,s2,s3,s4,s5,s6;		/*各关节正弦*/
	double c1,c2,c3,c4,c5,c6;		/*各关节余弦*/

	CARTPOS pos;

		//	RMATRIX temp;
		//temp.Nx = 0.710439		;
		//temp.Ny = -0.703758		;
		//temp.Nz = -0.000753721	;
		//temp.Ox = 0.703757		;
		//temp.Oy = 0.710436		;
		//temp.Oz = 0.00190553	;
		//temp.Ax = -0.000805564;
		//temp.Ay = -0.0018842	;
		//temp.Az = 0.999996	;
		//RPY temprpy;
		//MatToRpy(&temp,&temprpy);

		//			RMATRIX temp;
		//temp.Nx = -0.234031		;
		//temp.Ny = -0.972229		;
		//temp.Nz = 0.000142405	;
		//temp.Ox = 0.972229		;
		//temp.Oy = -0.234031		;
		//temp.Oz = -0.000483964	;
		//temp.Ax = 0.000503851;
		//temp.Ay = 0.0000251873	;
		//temp.Az = 1	;
		//RPY temprpy;
		//MatToRpy(&temp,&temprpy);


		//RMATRIX temp;
		//temp.Nx = -0.226541	;
		//temp.Ny = -0.974001	;
		//temp.Nz = -0.000779144	;
		//temp.Ox = 0.974001;
		//temp.Oy = -0.226541;
		//temp.Oz = -0.0000841496	;
		//temp.Ax = -0.0000945464;
		//temp.Ay = -0.000777951;
		//temp.Az = 0.999999;
		//RPY temprpy;
		//MatToRpy(&temp,&temprpy);

	s1 = sin(pjoint->Theta1);
	s2 = sin(pjoint->Theta2);
	s3 = sin(pjoint->Theta3);
	s4 = sin(pjoint->Theta4);
	s5 = sin(pjoint->Theta5);
	s6 = sin(pjoint->Theta6);

	c1 = cos(pjoint->Theta1);
	c2 = cos(pjoint->Theta2);
	c3 = cos(pjoint->Theta3);
	c4 = cos(pjoint->Theta4);
	c5 = cos(pjoint->Theta5);
	c6 = cos(pjoint->Theta6);

	//求正解过程中的旋转矩阵
	pos.Rmatrix.Nx = s6*(c4*s1 - s4*(c1*c2*c3 - c1*s2*s3)) - c6*(s5*(c1*c2*s3 + c1*c3*s2) - 
		c5*(s1*s4 + c4*(c1*c2*c3 - c1*s2*s3)));
	pos.Rmatrix.Ny = -c6*(s5*(c2*s1*s3 + c3*s1*s2) + c5*(c1*s4 - c4*(c2*c3*s1 - s1*s2*s3))) - 
		s6*(c1*c4 + s4*(c2*c3*s1 - s1*s2*s3));
	pos.Rmatrix.Nz = s4*s6*(c2*s3 + c3*s2) - c6*(s5*(c2*c3 - s2*s3) + c4*c5*(c2*s3 + c3*s2));

	pos.Rmatrix.Ox = s6*(s5*(c1*c2*s3 + c1*c3*s2) - c5*(s1*s4 + c4*(c1*c2*c3 - c1*s2*s3))) + 
		c6*(c4*s1 - s4*(c1*c2*c3 - c1*s2*s3));
	pos.Rmatrix.Oy = s6*(s5*(c2*s1*s3 + c3*s1*s2) + c5*(c1*s4 - c4*(c2*c3*s1 - s1*s2*s3))) - 
		c6*(c1*c4 + s4*(c2*c3*s1 - s1*s2*s3));
	pos.Rmatrix.Oz = s6*(s5*(c2*c3 - s2*s3) + c4*c5*(c2*s3 + c3*s2)) + c6*s4*(c2*s3 + c3*s2);

	pos.Rmatrix.Ax = - c5*(c1*c2*s3 + c1*c3*s2) - s5*(s1*s4 + c4*(c1*c2*c3 - c1*s2*s3));
	pos.Rmatrix.Ay = s5*(c1*s4 - c4*(c2*c3*s1 - s1*s2*s3)) - c5*(c2*s1*s3 + c3*s1*s2);
	pos.Rmatrix.Az = c4*s5*(c2*s3 + c3*s2) - c5*(c2*c3 - s2*s3);



	pos.Px = c1*plink->L6 - plink->L5*(c5*c1*(c2*s3 + c3*s2) + s5*(s1*s4 + c4*c1*(c2*c3 - s2*s3))) + 
		plink->L3*c1*(c2*c3 - s2*s3) - plink->L4*c1*(c2*s3 + c3*s2) + plink->L2*c1*c2;
	pos.Py = plink->L6*s1 - plink->L5*(c5*(c2*s1*s3 + c3*s1*s2) - s5*(c1*s4 - c4*(c2*c3*s1 - s1*s2*s3))) + 
		plink->L3*(c2*c3*s1 - s1*s2*s3) - plink->L4*(c2*s1*s3 + c3*s1*s2) + plink->L2*c2*s1 ;

	pos.Pz = plink->L1 - plink->L2*s2 - plink->L3*(c2*s3 + c3*s2) - plink->L4*(c2*c3 - s2*s3) - 
		plink->L5*(c5*(c2*c3 - s2*s3) - c4*s5*(c2*s3 + c3*s2));	

	 	//pos.Px = plink->L7*s1 + c1*plink->L6 - plink->L5*(c5*c1*(c2*s3 + c3*s2) + s5*(s1*s4 + c4*c1*(c2*c3 - s2*s3))) + 
	 		//plink->L3*c1*(c2*c3 - s2*s3) - plink->L4*c1*(c2*s3 + c3*s2) + plink->L2*c1*c2;
	// 	
	// 	pos.Py = -plink->L7*c1 + plink->L6*s1 - plink->L5*(c5*(c2*s1*s3 + c3*s1*s2) - s5*(c1*s4 - c4*(c2*c3*s1 - s1*s2*s3))) + 
	// 		plink->L3*(c2*c3*s1 - s1*s2*s3) - plink->L4*(c2*s1*s3 + c3*s1*s2) + plink->L2*c2*s1 ;
	robot.robotEndPos = pos;
	MatToRpy(&pos.Rmatrix,&robot.robotEndPos.Rpy);
	//增加工具坐标系
	CARTPOS posTemp;
	posTemp.Px = pos.Rmatrix.Nx*robot.toolPos.Px + pos.Rmatrix.Ox*robot.toolPos.Py + pos.Rmatrix.Ax*robot.toolPos.Pz+pos.Px;
	posTemp.Py = pos.Rmatrix.Ny*robot.toolPos.Px + pos.Rmatrix.Oy*robot.toolPos.Py + pos.Rmatrix.Ay*robot.toolPos.Pz+pos.Py;
	posTemp.Pz = pos.Rmatrix.Nz*robot.toolPos.Px + pos.Rmatrix.Oz*robot.toolPos.Py + pos.Rmatrix.Az*robot.toolPos.Pz+pos.Pz;
	posTemp.Rmatrix.Nx =  pos.Rmatrix.Nx*robot.toolPos.Rmatrix.Nx + pos.Rmatrix.Ox*robot.toolPos.Rmatrix.Ny + pos.Rmatrix.Ax*robot.toolPos.Rmatrix.Nz;
	posTemp.Rmatrix.Ny =  pos.Rmatrix.Ny*robot.toolPos.Rmatrix.Nx + pos.Rmatrix.Oy*robot.toolPos.Rmatrix.Ny + pos.Rmatrix.Ay*robot.toolPos.Rmatrix.Nz;
	posTemp.Rmatrix.Nz =  pos.Rmatrix.Nz*robot.toolPos.Rmatrix.Nx + pos.Rmatrix.Oz*robot.toolPos.Rmatrix.Ny + pos.Rmatrix.Az*robot.toolPos.Rmatrix.Nz;
	posTemp.Rmatrix.Ox =  pos.Rmatrix.Nx*robot.toolPos.Rmatrix.Ox + pos.Rmatrix.Ox*robot.toolPos.Rmatrix.Oy + pos.Rmatrix.Ax*robot.toolPos.Rmatrix.Oz;
	posTemp.Rmatrix.Oy =  pos.Rmatrix.Ny*robot.toolPos.Rmatrix.Ox + pos.Rmatrix.Oy*robot.toolPos.Rmatrix.Oy + pos.Rmatrix.Ay*robot.toolPos.Rmatrix.Oz;
	posTemp.Rmatrix.Oz =  pos.Rmatrix.Nz*robot.toolPos.Rmatrix.Ox + pos.Rmatrix.Oz*robot.toolPos.Rmatrix.Oy + pos.Rmatrix.Az*robot.toolPos.Rmatrix.Oz;
	posTemp.Rmatrix.Ax =  pos.Rmatrix.Nx*robot.toolPos.Rmatrix.Ax + pos.Rmatrix.Ox*robot.toolPos.Rmatrix.Ay + pos.Rmatrix.Ax*robot.toolPos.Rmatrix.Az;
	posTemp.Rmatrix.Ay =  pos.Rmatrix.Ny*robot.toolPos.Rmatrix.Ax + pos.Rmatrix.Oy*robot.toolPos.Rmatrix.Ay + pos.Rmatrix.Ay*robot.toolPos.Rmatrix.Az;
	posTemp.Rmatrix.Az =  pos.Rmatrix.Nz*robot.toolPos.Rmatrix.Ax + pos.Rmatrix.Oz*robot.toolPos.Rmatrix.Ay + pos.Rmatrix.Az*robot.toolPos.Rmatrix.Az;
	pos.Px = posTemp.Px;
	pos.Py = posTemp.Py;
	pos.Pz = posTemp.Pz;
	pos.Rmatrix = posTemp.Rmatrix;
	MatToRpy(&pos.Rmatrix,&pos.Rpy);
	return pos;	
}

//运动学反解，未指定序号，解出8组关节角
MC_T_INT Inverse(const CARTPOS *ppos,const LINK *plink,JOINT *pjoint)
{
	/*机器人关节数据*/
	MC_T_REAL Theta1,Theta2,Theta3,Theta4,Theta5,Theta6;
	/*机器人连杆数据*/
	MC_T_REAL L1,L2,L3,L4,L5,L6;
	/*定义矩阵T中相关元素*/
	MC_T_REAL Ox,Oy,Oz,Ax,Ay,Az,Px,Py,Pz;
	/*定义T16矩阵中相关元素*/
	MC_T_REAL r12,r22,r32,r13,r23,r33,Qx,Qy,Qz;

	MC_T_REAL ktmp;

	L1 = plink->L1;
	L2 = plink->L2;
	L3 = plink->L3;
	L4 = plink->L4;
	L5 = plink->L5;
	L6 = plink->L6;
	//增加工具坐标系
	CARTPOS posTemp;
	posTemp.Rmatrix.Nx =  ppos->Rmatrix.Nx*robot.tpInverse.Nx + ppos->Rmatrix.Ox*robot.tpInverse.Ny + ppos->Rmatrix.Ax*robot.tpInverse.Nz;
	posTemp.Rmatrix.Ny =  ppos->Rmatrix.Ny*robot.tpInverse.Nx + ppos->Rmatrix.Oy*robot.tpInverse.Ny + ppos->Rmatrix.Ay*robot.tpInverse.Nz;
	posTemp.Rmatrix.Nz =  ppos->Rmatrix.Nz*robot.tpInverse.Nx + ppos->Rmatrix.Oz*robot.tpInverse.Ny + ppos->Rmatrix.Az*robot.tpInverse.Nz;
	posTemp.Rmatrix.Ox =  ppos->Rmatrix.Nx*robot.tpInverse.Ox + ppos->Rmatrix.Ox*robot.tpInverse.Oy + ppos->Rmatrix.Ax*robot.tpInverse.Oz;
	posTemp.Rmatrix.Oy =  ppos->Rmatrix.Ny*robot.tpInverse.Ox + ppos->Rmatrix.Oy*robot.tpInverse.Oy + ppos->Rmatrix.Ay*robot.tpInverse.Oz;
	posTemp.Rmatrix.Oz =  ppos->Rmatrix.Nz*robot.tpInverse.Ox + ppos->Rmatrix.Oz*robot.tpInverse.Oy + ppos->Rmatrix.Az*robot.tpInverse.Oz;
	posTemp.Rmatrix.Ax =  ppos->Rmatrix.Nx*robot.tpInverse.Ax + ppos->Rmatrix.Ox*robot.tpInverse.Ay + ppos->Rmatrix.Ax*robot.tpInverse.Az;
	posTemp.Rmatrix.Ay =  ppos->Rmatrix.Ny*robot.tpInverse.Ax + ppos->Rmatrix.Oy*robot.tpInverse.Ay + ppos->Rmatrix.Ay*robot.tpInverse.Az;
	posTemp.Rmatrix.Az =  ppos->Rmatrix.Nz*robot.tpInverse.Ax + ppos->Rmatrix.Oz*robot.tpInverse.Ay + ppos->Rmatrix.Az*robot.tpInverse.Az;

	posTemp.Px = ppos->Px - (posTemp.Rmatrix.Nx*robot.toolPos.Px + posTemp.Rmatrix.Ox*robot.toolPos.Py + posTemp.Rmatrix.Ax*robot.toolPos.Pz);
	posTemp.Py = ppos->Py - (posTemp.Rmatrix.Ny*robot.toolPos.Px + posTemp.Rmatrix.Oy*robot.toolPos.Py + posTemp.Rmatrix.Ay*robot.toolPos.Pz);
	posTemp.Pz = ppos->Pz - (posTemp.Rmatrix.Nz*robot.toolPos.Px + posTemp.Rmatrix.Oz*robot.toolPos.Py + posTemp.Rmatrix.Az*robot.toolPos.Pz);

	Px = posTemp.Px;
	Py = posTemp.Py;
	Pz = posTemp.Pz;

	//printf("px: %.3f, py: %.3f, pz: %.3f\n",Px,Py,Pz);
// 	Px = ppos->Px;
// 	Py = ppos->Py;
// 	Pz = ppos->Pz;

	Ox = posTemp.Rmatrix.Ox;
	Oy = posTemp.Rmatrix.Oy;
	Oz = posTemp.Rmatrix.Oz;
		 
	Ax = posTemp.Rmatrix.Ax;
	Ay = posTemp.Rmatrix.Ay;
	Az = posTemp.Rmatrix.Az;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第一组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	/*L7为0,求解Theta1*/
	Theta1 = atan2((Py - L5*Ay),(Px - L5*Ax));	

	r12 = cos(Theta1)*Ox + sin(Theta1)*Oy;
	r22 = -sin(Theta1)*Ox + cos(Theta1)*Oy;
	r32 = Oz;

	r13 = cos(Theta1)*Ax + sin(Theta1)*Ay;
	r23 = -sin(Theta1)*Ax + cos(Theta1)*Ay;
	r33 = Az;

	Qx = cos(Theta1)*Px + sin(Theta1)*Py;
	Qy = -sin(Theta1)*Px + cos(Theta1)*Py;
	Qz = Pz - L1;

	ktmp = (pow((Qx-L6-r13*L5),2)+pow((Qz-r33*L5),2)-pow(L2,2)-pow(L3,2)-pow(L4,2))/2/L2;

	/*求解Theta3,第一个Theta3*/
	Theta3 = atan2(L3,L4)-atan2( ktmp, sqrt(pow(L3,2)+pow(L4,2)-pow(ktmp,2)) );

	/*求解Theta2*/
	Theta2 = atan2( (Qx-L6-r13*L5)*(L2*sin(Theta3)-L4) - (Qz-r33*L5)*(L2*cos(Theta3)+L3),
		(Qx-L6-r13*L5)*(L2*cos(Theta3)+L3)+(Qz-r33*L5)*(L2*sin(Theta3)-L4) ) - Theta3;
	//2015.11.04 chris
	if(Theta2 > PI)
	{
		Theta2 = Theta2 - 2*PI;
	}
	if(Theta2 < -PI)
	{
		Theta2 = Theta2 + 2*PI; 
	}

	//change by cwq at 2015.7.28
	//针对关节角5等于零时出现奇异位形时求解关节角四和关节角6
	//实现方法：对atan2()函数中两项进行判断，当两项都等于零时认为关节角5的正弦为零，根据关节角5的范围，可以认为关节角5为零
	if (fabs(r23) < DOUBLE_ABS_MIN && fabs(r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3)) < DOUBLE_ABS_MIN)
	{
		//关节角5正弦等于零,认为关节5等于零
		Theta5 = 0;
		Theta4 = 0;//指定关节角4，根据关节角4求解关节角6
	} 
	else
	{
		/*求解Theta4*/
		Theta4 = atan2(r23,r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3));

		/*求解Theta5*/
		if (fabs(sin(Theta4)) < 0.01 )
		{
			Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
				-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );	
		}
		else 
		{
			Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
		}
	}
	//change by cwq at 2015.7.28

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );

		//		Theta6 = atan2((r32*cos(Theta2+Theta3)+r12*sin(Theta2+Theta3))/sin(Theta5),
		//			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22);
	}

	pjoint->Theta1 = Theta1;
	pjoint->Theta2 = Theta2;
	pjoint->Theta3 = Theta3;
	pjoint->Theta4 = Theta4;
	pjoint->Theta5 = Theta5;
	pjoint->Theta6 = Theta6;
	pjoint->nSolutionNum = 1;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第二组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	if(Theta4 > 0)
	{
		Theta4 = Theta4 - PI;
	}
	else
	{
		Theta4 = Theta4 + PI;
	}
	/*求解Theta5*/
	if (fabs(sin(Theta4)) < 0.01 )
	{
		Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
			-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
	}
	else 
	{
		Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
	}

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );	
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+1)->Theta1 = Theta1;
	(pjoint+1)->Theta2 = Theta2;
	(pjoint+1)->Theta3 = Theta3;
	(pjoint+1)->Theta4 = Theta4;
	(pjoint+1)->Theta5 = Theta5;
	(pjoint+1)->Theta6 = Theta6;
	(pjoint+1)->nSolutionNum = 2;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第三组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	/*求解Theta3,第二个Theta3*/
	Theta3 = atan2(L3,L4)-atan2( ktmp, -sqrt(pow(L3,2)+pow(L4,2)-pow(ktmp,2)) );

	/*求解Theta2*/
	Theta2 = atan2( (Qx-L6-r13*L5)*(L2*sin(Theta3)-L4) - (Qz-r33*L5)*(L2*cos(Theta3)+L3),
		(Qx-L6-r13*L5)*(L2*cos(Theta3)+L3)+(Qz-r33*L5)*(L2*sin(Theta3)-L4) ) - Theta3;

	//add 2015.11.04 chris
	if(Theta2 > PI)
	{
		Theta2 = Theta2 - 2*PI;
	}
	if(Theta2 < -PI)
	{
		Theta2 = Theta2 + 2*PI; 
	}


	//change by cwq at 2015.7.28
	//针对关节角5等于零时出现奇异位形时求解关节角四和关节角6
	//实现方法：对atan2()函数中两项进行判断，当两项都等于零时认为关节角5的正弦为零，根据关节角5的范围，可以认为关节角5为零
	if (fabs(r23) < DOUBLE_ABS_MIN && fabs(r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3)) < DOUBLE_ABS_MIN)
	{
		//关节角5正弦等于零,认为关节5等于零
		Theta5 = 0;
		Theta4 = 0;//指定关节角4，根据关节角4求解关节角6
	} 
	else
	{
		/*求解Theta4*/
		Theta4 = atan2(r23,r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3));

		/*求解Theta5*/
		if (fabs(sin(Theta4)) < 0.01 )
		{
			Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
				-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
		}
		else 
		{
			Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
		}
	}
	//change by cwq at 2015.7.28

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+2)->Theta1 = Theta1;
	(pjoint+2)->Theta2 = Theta2;
	(pjoint+2)->Theta3 = Theta3;
	(pjoint+2)->Theta4 = Theta4;
	(pjoint+2)->Theta5 = Theta5;
	(pjoint+2)->Theta6 = Theta6;
	(pjoint+2)->nSolutionNum = 3;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第四组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	if(Theta4 > 0)
	{
		Theta4 = Theta4 - PI;
	}
	else
	{
		Theta4 = Theta4 + PI;
	}
	/*求解Theta5*/
	if (fabs(sin(Theta4)) < 0.01 )
	{
		Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
			-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
	}
	else 
	{
		Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
	}

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );	
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+3)->Theta1 = Theta1;
	(pjoint+3)->Theta2 = Theta2;
	(pjoint+3)->Theta3 = Theta3;
	(pjoint+3)->Theta4 = Theta4;
	(pjoint+3)->Theta5 = Theta5;
	(pjoint+3)->Theta6 = Theta6;
	(pjoint+3)->nSolutionNum = 4;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第五组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	if(Theta1 > 0)
	{
		Theta1 = Theta1-PI; 
	}
	else
	{
		Theta1 = Theta1+PI;
	}	

	r12 = cos(Theta1)*Ox + sin(Theta1)*Oy;
	r22 = -sin(Theta1)*Ox + cos(Theta1)*Oy;
	r23 = Oz;

	r13 = cos(Theta1)*Ax + sin(Theta1)*Ay;
	r23 = -sin(Theta1)*Ax + cos(Theta1)*Ay;
	r33 = Az;

	Qx = cos(Theta1)*Px + sin(Theta1)*Py;
	Qy = -sin(Theta1)*Px + cos(Theta1)*Py;
	Qz = Pz - L1;

	ktmp = (pow((Qx-L6-r13*L5),2)+pow((Qz-r33*L5),2)-pow(L2,2)-pow(L3,2)-pow(L4,2))/2/L2;


	/*求解Theta3,第一个Theta3*/
	Theta3 = atan2(L3,L4)-atan2( ktmp, sqrt(pow(L3,2)+pow(L4,2)-pow(ktmp,2)) );

	/*求解Theta2*/
	Theta2 = atan2( (Qx-L6-r13*L5)*(L2*sin(Theta3)-L4) - (Qz-r33*L5)*(L2*cos(Theta3)+L3),
		(Qx-L6-r13*L5)*(L2*cos(Theta3)+L3)+(Qz-r33*L5)*(L2*sin(Theta3)-L4) ) - Theta3;

	//2015.11.04 modify by chris
	if(Theta2 > PI)
	{
		Theta2 = Theta2 - 2*PI;
	}
	if(Theta2 < -PI)
	{
		Theta2 = Theta2 + 2*PI; 
	}
	//change by cwq at 2015.7.28
	//针对关节角5等于零时出现奇异位形时求解关节角四和关节角6
	//实现方法：对atan2()函数中两项进行判断，当两项都等于零时认为关节角5的正弦为零，根据关节角5的范围，可以认为关节角5为零
	if (fabs(r23) < DOUBLE_ABS_MIN && fabs(r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3)) < DOUBLE_ABS_MIN)
	{
		//关节角5正弦等于零,认为关节5等于零
		Theta5 = 0;
		Theta4 = 0;//指定关节角4，根据关节角4求解关节角6
	} 
	else
	{
		/*求解Theta4*/
		Theta4 = atan2(r23,r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3));

		/*求解Theta5*/
		if (fabs(sin(Theta4)) < 0.01 )
		{
			Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
				-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
		}
		else 
		{
			Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
		}
	}
	//change by cwq at 2015.7.28

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );	
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+4)->Theta1 = Theta1;
	(pjoint+4)->Theta2 = Theta2;
	(pjoint+4)->Theta3 = Theta3;
	(pjoint+4)->Theta4 = Theta4;
	(pjoint+4)->Theta5 = Theta5;
	(pjoint+4)->Theta6 = Theta6;
	(pjoint+4)->nSolutionNum = 5;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第六组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	if(Theta4 > 0)
	{
		Theta4 = Theta4 - PI;
	}
	else
	{
		Theta4 = Theta4 + PI;
	}
	/*求解Theta5*/
	if (fabs(sin(Theta4)) < 0.01 )
	{
		Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
			-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );	
	}
	else 
	{
		Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
	}

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+5)->Theta1 = Theta1;
	(pjoint+5)->Theta2 = Theta2;
	(pjoint+5)->Theta3 = Theta3;
	(pjoint+5)->Theta4 = Theta4;
	(pjoint+5)->Theta5 = Theta5;
	(pjoint+5)->Theta6 = Theta6;
	(pjoint+5)->nSolutionNum = 6;

	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第七组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	/*求解Theta3,第二个Theta3*/
	Theta3 = atan2(L3,L4)-atan2( ktmp, -sqrt(pow(L3,2)+pow(L4,2)-pow(ktmp,2)) );

	/*求解Theta2*/
	Theta2 = atan2( (Qx-L6-r13*L5)*(L2*sin(Theta3)-L4) - (Qz-r33*L5)*(L2*cos(Theta3)+L3),
		(Qx-L6-r13*L5)*(L2*cos(Theta3)+L3)+(Qz-r33*L5)*(L2*sin(Theta3)-L4) ) - Theta3;
	//2015.11.04 chris
	if(Theta2 > PI)
	{
		Theta2 = Theta2 - 2*PI;
	}
	if(Theta2 < -PI)
	{
		Theta2 = Theta2 + 2*PI; 
	}

	//change by cwq at 2015.7.28
	//针对关节角5等于零时出现奇异位形时求解关节角四和关节角6
	//实现方法：对atan2()函数中两项进行判断，当两项都等于零时认为关节角5的正弦为零，根据关节角5的范围，可以认为关节角5为零
	if (fabs(r23) < DOUBLE_ABS_MIN && fabs(r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3)) < DOUBLE_ABS_MIN)
	{
		//关节角5正弦等于零,认为关节5等于零
		Theta5 = 0;
		Theta4 = 0;//指定关节角4，根据关节角4求解关节角6
	} 
	else
	{
		/*求解Theta4*/
		Theta4 = atan2(r23,r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3));

		/*求解Theta5*/
		if (fabs(sin(Theta4)) < 0.01 )
		{
			Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
				-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
		}
		else 
		{
			Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
		}
	}
	//change by cwq at 2015.7.28

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+6)->Theta1 = Theta1;
	(pjoint+6)->Theta2 = Theta2;
	(pjoint+6)->Theta3 = Theta3;
	(pjoint+6)->Theta4 = Theta4;
	(pjoint+6)->Theta5 = Theta5;
	(pjoint+6)->Theta6 = Theta6;
	(pjoint+6)->nSolutionNum = 7;
	/////////////////////////////////////////////////////////////////////////////////////
	/**求解第八组解**/
	/////////////////////////////////////////////////////////////////////////////////////

	if(Theta4 > 0)
	{
		Theta4 = Theta4 - PI;
	}
	else
	{
		Theta4 = Theta4 + PI;
	}

	/*求解Theta5*/
	if (fabs(sin(Theta4)) < 0.01 )
	{
		Theta5 = atan2( (r33*sin(Theta2+Theta3)-r13*cos(Theta2+Theta3))/cos(Theta4), 
			-r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3) );
	}
	else 
	{
		Theta5 = atan2( r23/sin(Theta4), -r13*sin(Theta2+Theta3)-r33*cos(Theta2+Theta3));
	}

	/*求解Theta6*/
	if (fabs(sin(Theta5)) < 0.01)   /*原程序此处有错误*/
	{	
		Theta6 = atan2( (r22*sin(Theta4)+cos(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3)))/cos(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}
	else
	{
		Theta6 = atan2( (r12*sin(Theta2+Theta3)+r32*cos(Theta2+Theta3))/sin(Theta5), 
			sin(Theta4)*(r32*sin(Theta2+Theta3)-r12*cos(Theta2+Theta3))-cos(Theta4)*r22 );
	}

	(pjoint+7)->Theta1 = Theta1;
	(pjoint+7)->Theta2 = Theta2;
	(pjoint+7)->Theta3 = Theta3;
	(pjoint+7)->Theta4 = Theta4;
	(pjoint+7)->Theta5 = Theta5;
	(pjoint+7)->Theta6 = Theta6;
	(pjoint+7)->nSolutionNum = 8;
	return MC_NO_ERROR;
};

//检查角度是否合理
MC_T_INT checkAngle(const JOINT *pjnt, const int PntNum, JOINT *pObjJnt)
{
	int angleNum=0;

	//检查关节是否超范围
	for(int i = 0;i < PntNum; i++)
	{
		if( ( (pjnt+i)->Theta1 <= robot.RobotInfo.MaxDhJnt[J1]) && ( (pjnt+i)->Theta1 >= robot.RobotInfo.MinDhJnt[J1]) &&

// 			((((pjnt+i)->Theta2 <= PI*20/180)&&((pjnt+i)->Theta2 >= -PI)) || (((pjnt+i)->Theta2 <= PI)&&((pjnt+i)->Theta2 >= PI*160/180))) &&	
// 			((((pjnt+i)->Theta3 <= PI*40/180)&&((pjnt+i)->Theta3 >= -PI)) || (((pjnt+i)->Theta3 <= PI)&&((pjnt+i)->Theta3 >= PI*140/180))) &&
			( (pjnt+i)->Theta2 <= robot.RobotInfo.MaxDhJnt[J2]) && ( (pjnt+i)->Theta2 >= robot.RobotInfo.MinDhJnt[J2]) &&
			( (pjnt+i)->Theta3 <= robot.RobotInfo.MaxDhJnt[J3]) && ( (pjnt+i)->Theta3 >= robot.RobotInfo.MinDhJnt[J3]) &&

			( (pjnt+i)->Theta4 <= robot.RobotInfo.MaxDhJnt[J4]) && ( (pjnt+i)->Theta4 >= robot.RobotInfo.MinDhJnt[J4]) &&
			( (pjnt+i)->Theta5 <= robot.RobotInfo.MaxDhJnt[J5]) && ( (pjnt+i)->Theta5 >= robot.RobotInfo.MinDhJnt[J5]) &&
			( (pjnt+i)->Theta6 <= robot.RobotInfo.MaxDhJnt[J6]) && ( (pjnt+i)->Theta6 >= robot.RobotInfo.MinDhJnt[J6]) )
		{
			(pObjJnt+angleNum)->Theta1=(pjnt+i)->Theta1;
			(pObjJnt+angleNum)->Theta2=(pjnt+i)->Theta2;
			(pObjJnt+angleNum)->Theta3=(pjnt+i)->Theta3;
			(pObjJnt+angleNum)->Theta4=(pjnt+i)->Theta4;
			(pObjJnt+angleNum)->Theta5=(pjnt+i)->Theta5;
			(pObjJnt+angleNum)->Theta6=(pjnt+i)->Theta6;

			angleNum++;
		}
	}

	return angleNum;
}

//*角度转编码器值
EC_T_VOID AngleToEncoder(const MC_T_REAL ang_array[6], int enc_array[6])
{
	int i;
	for(i = 0; i < 6; i++) 	
	{
		if( ang_array[i] > 0)//四舍五入,正数加0.5
		{
			enc_array[i] = (int) (ang_array[i]/360*robot.RobotInfo.cnt_per_cycle + 0.5);
		}
		else
		{
			if( ang_array[i] < 0) //四舍五入,负数减0.5
			{
				enc_array[i] = (int) (ang_array[i]/360*robot.RobotInfo.cnt_per_cycle - 0.5);
			}
			else //角度为0
			{
				enc_array[i] = 0;
			}
		}
	}
}

//*检测两组关节角度的连续性，没有跳变返回1，有跳变返回0
MC_T_BOOL checkJntContinuity(const JOINT *m_pJnt1, const JOINT *m_pJnt2)
{
	double j1_diff = m_pJnt1->Theta1 - m_pJnt2->Theta1;
	double j2_diff = m_pJnt1->Theta2 - m_pJnt2->Theta2;
	if((j2_diff > 2*PI-0.1)&&(j2_diff < 2*PI+0.1))
	{
		j2_diff = j2_diff - 2*PI;
	}
	if((j2_diff < -2*PI + 0.1)&&(j2_diff > -2*PI-0.1))
	{
		j2_diff = j2_diff + 2*PI;
	}	

	double j5_diff = m_pJnt1->Theta5 - m_pJnt2->Theta5;
	if((j5_diff > 2*PI-0.1)&&(j5_diff < 2*PI+0.1))
	{
		j5_diff = j5_diff - 2*PI;
	}
	if((j5_diff < -2*PI + 0.1)&&(j5_diff > -2*PI-0.1))
	{
		j5_diff = j5_diff + 2*PI;
	}

	double j6_diff = m_pJnt1->Theta6 - m_pJnt2->Theta6;
	if((j6_diff > 2*PI-0.1)&&(j6_diff < 2*PI+0.1))
	{
		j6_diff = j6_diff - 2*PI;
	}
	if((j6_diff < -2*PI + 0.1)&&(j6_diff > -2*PI-0.1))
	{
		j6_diff = j6_diff + 2*PI;
	}

	//2015.11.05 modify by chris 
	double j3_diff = m_pJnt1->Theta3 - m_pJnt2->Theta3;
	if((j3_diff > 2*PI-0.1)&&(j3_diff < 2*PI+0.1))
	{
		j3_diff = j3_diff - 2*PI;
	}
	if((j3_diff < -2*PI + 0.1)&&(j3_diff > -2*PI-0.1))
	{
		j3_diff = j3_diff + 2*PI;
	}

	//2015.11.07 modify by chris
	double j4_diff = m_pJnt1->Theta4 - m_pJnt2->Theta4;
	if((j4_diff > 2*PI-0.1)&&(j4_diff < 2*PI+0.1))
	{
		j4_diff = j4_diff - 2*PI;
	}
	if((j4_diff < -2*PI + 0.1)&&(j4_diff > -2*PI-0.1))
	{
		j4_diff = j4_diff + 2*PI;
	}

	if ( (fabs(j1_diff) > 0.6*1.5/*RobotInfo.MaxJntPerMs[J1]*/) ||
		(fabs(j2_diff) > 0.45*1.5/*RobotInfo.MaxJntPerMs[J2]*/) ||
		(fabs(j3_diff) > 0.7*1.5/*RobotInfo.MaxJntPerMs[J3]*/) ||
		(fabs(j4_diff) > 0.72*1.5/*RobotInfo.MaxJntPerMs[J4]*/) ||
		(fabs(j5_diff) > 1.1*1.5/*RobotInfo.MaxJntPerMs[J5]*/) ||
		(fabs(j6_diff) > 1.44*1.5/*RobotInfo.MaxJntPerMs[J6])*/) )
	{
		//有跳变
		return 0;
	} 
	else 
	{
		//没有跳变
		return 1;
	}
}

//编码器脉冲数转换为角度,带减速比
EC_T_VOID Encoder2DHAngle(const int m_enc[6], JOINT *m_jonit)
{
	JOINT m_tempJoint;
	m_tempJoint.Theta1 = (m_enc[J1] - robot.RobotInfo.enc_home[J1])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J1]);
	m_tempJoint.Theta2 = (m_enc[J2] - robot.RobotInfo.enc_home[J2])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J2]);
	m_tempJoint.Theta3 = (m_enc[J3] - robot.RobotInfo.enc_home[J3])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J3]);
	m_tempJoint.Theta4 = (m_enc[J4] - robot.RobotInfo.enc_home[J4])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J4]);
	m_tempJoint.Theta5 = (m_enc[J5] - robot.RobotInfo.enc_home[J5])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J5]);
	m_tempJoint.Theta6 = (m_enc[J6] - robot.RobotInfo.enc_home[J6])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J6]);

	m_jonit->Theta1 = robot.RobotInfo.motorDir[J1]*m_tempJoint.Theta1 + robot.RobotInfo.joint_home[J1];
	m_jonit->Theta2 = robot.RobotInfo.motorDir[J2]*m_tempJoint.Theta2 + robot.RobotInfo.joint_home[J2];
	m_jonit->Theta3 = robot.RobotInfo.motorDir[J3]*m_tempJoint.Theta3 + robot.RobotInfo.joint_home[J3];
	m_jonit->Theta4 = robot.RobotInfo.motorDir[J4]*m_tempJoint.Theta4 + robot.RobotInfo.joint_home[J4];
	m_jonit->Theta5 = robot.RobotInfo.motorDir[J5]*m_tempJoint.Theta5 + robot.RobotInfo.joint_home[J5];
	m_jonit->Theta6 = robot.RobotInfo.motorDir[J6]*m_tempJoint.Theta6 + robot.RobotInfo.joint_home[J6];	

	//考虑耦合比的影响   耦合比的存在造成电机转角与实际轴角不一致，DH角对应实际轴角
	m_jonit->Theta6 = robot.RobotInfo.cpRatio[R46]*m_jonit->Theta4 + robot.RobotInfo.cpRatio[R56]*m_jonit->Theta5 + m_jonit->Theta6;
	m_jonit->Theta5 = robot.RobotInfo.cpRatio[R45]*m_jonit->Theta4 + m_jonit->Theta5;

	//检查角度是否超限
	if (m_jonit->Theta1 > robot.RobotInfo.MaxDhJnt[0])
		robot.outRangeJnt = J1B;
	else if (m_jonit->Theta1 < robot.RobotInfo.MinDhJnt[0])
		robot.outRangeJnt = J1S;
	else if (m_jonit->Theta2 > robot.RobotInfo.MaxDhJnt[1])
		robot.outRangeJnt = J2B;
	else if (m_jonit->Theta2 < robot.RobotInfo.MinDhJnt[1])
		robot.outRangeJnt = J2S;
	else if (m_jonit->Theta3 > robot.RobotInfo.MaxDhJnt[2])
		robot.outRangeJnt = J3B;
	else if (m_jonit->Theta3 < robot.RobotInfo.MinDhJnt[2])
		robot.outRangeJnt = J3S;
	else if (m_jonit->Theta4 > robot.RobotInfo.MaxDhJnt[3])
		robot.outRangeJnt = J4B;
	else if (m_jonit->Theta4 < robot.RobotInfo.MinDhJnt[3])
		robot.outRangeJnt = J4S;
	else if (m_jonit->Theta5 > robot.RobotInfo.MaxDhJnt[4])
		robot.outRangeJnt = J5B;
	else if (m_jonit->Theta5 < robot.RobotInfo.MinDhJnt[4])
		robot.outRangeJnt = J5S;
	else if (m_jonit->Theta6 > robot.RobotInfo.MaxDhJnt[5])
		robot.outRangeJnt = J6B;
	else if (m_jonit->Theta6 < robot.RobotInfo.MinDhJnt[5])
		robot.outRangeJnt = J6S;
	else
	{
		robot.outRangeJnt = NONE;
		if(robot.wErrorID == KYN_JNT_OUT_RANGE)
			robot.wErrorID = 0;
	}
}

//一组DH模型角度(弧度)转换为对应的编码器值
EC_T_VOID DHAngle2Encoder(const JOINT *m_pDHAngle, JNTS_ENC *m_pEncoder)
{
	JOINT joint_temp;//中间变量

	//考虑耦合比的影响
	joint_temp.Theta5 = m_pDHAngle->Theta5 - robot.RobotInfo.cpRatio[R45]*m_pDHAngle->Theta4;
	joint_temp.Theta6 = m_pDHAngle->Theta6 - robot.RobotInfo.cpRatio[R46]*m_pDHAngle->Theta4 - robot.RobotInfo.cpRatio[R56]*joint_temp.Theta5;

	joint_temp.Theta1 = -robot.RobotInfo.motorDir[J1]*(robot.RobotInfo.joint_home[J1] - m_pDHAngle->Theta1);  
	joint_temp.Theta2 = -robot.RobotInfo.motorDir[J2]*(robot.RobotInfo.joint_home[J2] - m_pDHAngle->Theta2);	
	joint_temp.Theta3 = -robot.RobotInfo.motorDir[J3]*(robot.RobotInfo.joint_home[J3] - m_pDHAngle->Theta3);  
	joint_temp.Theta4 = -robot.RobotInfo.motorDir[J4]*(robot.RobotInfo.joint_home[J4] - m_pDHAngle->Theta4);  
	joint_temp.Theta5 = -robot.RobotInfo.motorDir[J5]*(robot.RobotInfo.joint_home[J5] - joint_temp.Theta5);  
	joint_temp.Theta6 = -robot.RobotInfo.motorDir[J6]*(robot.RobotInfo.joint_home[J6] - joint_temp.Theta6);  

	//角度转换成编码器的值
	m_pEncoder->Encoder[J1] = robot.RobotInfo.enc_home[J1] +
		(int) (joint_temp.Theta1 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J1]/2/PI);
	m_pEncoder->Encoder[J2] = robot.RobotInfo.enc_home[J2] +
		(int) (joint_temp.Theta2 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J2]/2/PI);
	m_pEncoder->Encoder[J3] = robot.RobotInfo.enc_home[J3] +
		(int) (joint_temp.Theta3 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J3]/2/PI);
	m_pEncoder->Encoder[J4] = robot.RobotInfo.enc_home[J4] +
		(int) (joint_temp.Theta4 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J4]/2/PI);
	m_pEncoder->Encoder[J5] = robot.RobotInfo.enc_home[J5] +
		(int) (joint_temp.Theta5 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J5]/2/PI);
	m_pEncoder->Encoder[J6] = robot.RobotInfo.enc_home[J6] +
		(int) (joint_temp.Theta6 * robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[J6]/2/PI);
}

//处理耦合比问题
EC_T_VOID Axis2Drive(RC_T_JNTS *jntsEnd, RC_T_JNTS *jntsStart)
{
	RC_T_JNTS temp;
	temp.fJnts[J4] = jntsEnd->fJnts[J4] - jntsStart->fJnts[J4];
	temp.fJnts[J5] = jntsEnd->fJnts[J5] - jntsStart->fJnts[J5] - robot.RobotInfo.cpRatio[R45]*temp.fJnts[J4];
	temp.fJnts[J6] = jntsEnd->fJnts[J6] - jntsStart->fJnts[J6] - robot.RobotInfo.cpRatio[R46]*temp.fJnts[J4] - robot.RobotInfo.cpRatio[R56]*temp.fJnts[J5];
	jntsEnd->fJnts[J5] = jntsStart->fJnts[J5] + temp.fJnts[J5];
	jntsEnd->fJnts[J6] = jntsStart->fJnts[J6] + temp.fJnts[J6];
}

//返回值，返回0-7表示选中了一组解，返回大于7的整数表示选解失败
//m_pAllSolutionJ所有合理解，m_solutionNum合理解数量，m_pRefeJ参考角度，m_pSelectJ选定的解
MC_T_INT selectSolutionByMinMove(const JOINT *m_pAllSolutionJ, const int m_solutionNum, const JOINT *m_pReferJ, JOINT *m_pSelectJ)
{	
	int m_solutionId=0;//返回值，把解指向第一组关节解

	//创建一个1*8的数组，存放当前选解角度和参考角度之间的绝对差
	MC_T_REAL sum[8]={0}, m_minSum;

	//解得组数为零时，选解会失败,返回一个大于7的常数表示选解失败
	if (m_solutionNum == 0)
	{
		return NO_SOLUTION;
		//合理解个数为零，表示无解   change by cwq at 2015.9.14
	}

	//2015.11.08 modify by chris
	double diff0[6] = {0.0};
	double diff0_pow[6] = {0.0};
	diff0[J1] =  m_pAllSolutionJ[0].Theta1 - m_pReferJ->Theta1;
	// 	if( (diff0[J1] > (2*PI - 0.1)) && (diff0[J1] < 2*PI +0.1) )
	// 	{
	// 		diff0[J1] = diff0[J1] - 2*PI;
	// 	}
	// 	if(diff0[J1] < (-2*PI + 0.1) && (diff0[J1] > -2*PI - 0.1))
	// 	{
	// 		diff0[J1] = diff0[J1] + 2*PI;
	// 	}

	diff0[J2] =  m_pAllSolutionJ[0].Theta2 - m_pReferJ->Theta2;

	if( (diff0[J2] > (2*PI - 0.1)) && (diff0[J1] < 2*PI +0.1) )
	{
		diff0[J2] = diff0[J2] - 2*PI;
	}
	if(diff0[J2] < (-2*PI + 0.1) && (diff0[J2] > -2*PI - 0.1))
	{
		diff0[J2] = diff0[J2] + 2*PI;
	}

	diff0[J3] =  m_pAllSolutionJ[0].Theta3 - m_pReferJ->Theta3;
	if( (diff0[J3] > (2*PI - 0.1)) && (diff0[J3] < 2*PI +0.1) )
	{
		diff0[J3] = diff0[J3] - 2*PI;
	}
	if(diff0[J3] < (-2*PI + 0.1) && (diff0[J3] > -2*PI - 0.1))
	{
		diff0[J3] = diff0[J3] + 2*PI;
	}

	diff0[J4] =  m_pAllSolutionJ[0].Theta4 - m_pReferJ->Theta4;
	if( (diff0[J4] > (2*PI - 0.1)) && (diff0[J4] < 2*PI +0.1) )
	{
		diff0[J4] = diff0[J4] - 2*PI;
	}
	if(diff0[J4] < (-2*PI + 0.1) && (diff0[J4] > -2*PI - 0.1))
	{
		diff0[J4] = diff0[J4] + 2*PI;
	}

	diff0[J5] =  m_pAllSolutionJ[0].Theta5 - m_pReferJ->Theta5;
	if( (diff0[J5] > (2*PI - 0.1)) && (diff0[J5] < 2*PI +0.1) )
	{
		diff0[J5] = diff0[J5] - 2*PI;
	}
	if(diff0[J5] < (-2*PI + 0.1) && (diff0[J5] > -2*PI - 0.1))
	{
		diff0[J5] = diff0[J5] + 2*PI;
	}

	diff0[J6] =  m_pAllSolutionJ[0].Theta6 - m_pReferJ->Theta6;
	if( (diff0[J6] > (2*PI - 0.1)) && (diff0[J6] < 2*PI +0.1) )
	{
		diff0[J6] = diff0[J6] - 2*PI;

	}
	if(diff0[J6] < (-2*PI + 0.1) && (diff0[J6] > -2*PI - 0.1))
	{
		diff0[J6] = diff0[J6] + 2*PI;
	}

	diff0_pow[J1] = pow(diff0[J1],2);
	diff0_pow[J2] = pow(diff0[J2],2);
	diff0_pow[J3] = pow(diff0[J3],2);
	diff0_pow[J4] = pow(diff0[J4],2);
	diff0_pow[J5] = pow(diff0[J5],2);
	diff0_pow[J6] = pow(diff0[J6],2);

	//获得第一个绝对值和，这里这样做也缺乏考虑
	// 	sum[0]=sqrt(pow(m_pAllSolutionJ->Theta1 - m_pReferJ->Theta1,2)+
	// 		pow(m_pAllSolutionJ->Theta2 - m_pReferJ->Theta2,2)+
	// 		pow(m_pAllSolutionJ->Theta3 - m_pReferJ->Theta3,2)+
	// 		pow(m_pAllSolutionJ->Theta4 - m_pReferJ->Theta4,2)+
	// 		pow(m_pAllSolutionJ->Theta5 - m_pReferJ->Theta5,2)+
	// 		pow(m_pAllSolutionJ->Theta6 - m_pReferJ->Theta6,2));

	sum[0]=sqrt(diff0_pow[J1] + diff0_pow[J2] + diff0_pow[J3] +
		diff0_pow[J4] + diff0_pow[J5] + diff0_pow[J6]);

	m_minSum=sum[0];//把第一组关节解作为最小的绝对值和

	//找出差值最小的一组关节角，得知道序号
	for (int i=1; i<m_solutionNum; i++)
	{


		//2015.11.08 modify by chris
		diff0[J1] =  m_pAllSolutionJ[i].Theta1 - m_pReferJ->Theta1;
		// 		if( (diff0[J1] > (2*PI - 0.1)) && (diff0[J1] < 2*PI +0.1) )
		// 		{
		// 			diff0[J1] = diff0[J1] - 2*PI;
		// 		}
		// 		if(diff0[J1] < (-2*PI + 0.1) && (diff0[J1] > -2*PI - 0.1))
		// 		{
		// 			diff0[J1] = diff0[J1] + 2*PI;
		// 		}

		diff0[J2] =  m_pAllSolutionJ[i].Theta2 - m_pReferJ->Theta2;
		if( (diff0[J2] > (2*PI - 0.1)) && (diff0[J1] < 2*PI +0.1) )
		{
			diff0[J2] = diff0[J2] - 2*PI;
		}
		if(diff0[J2] < (-2*PI + 0.1) && (diff0[J2] > -2*PI - 0.1))
		{
			diff0[J2] = diff0[J2] + 2*PI;
		}

		diff0[J3] =  m_pAllSolutionJ[i].Theta3 - m_pReferJ->Theta3;
		if( (diff0[J3] > (2*PI - 0.1)) && (diff0[J3] < 2*PI +0.1) )
		{
			diff0[J3] = diff0[J3] - 2*PI;
		}
		if(diff0[J3] < (-2*PI + 0.1) && (diff0[J3] > -2*PI - 0.1))
		{
			diff0[J3] = diff0[J3] + 2*PI;
		}

		diff0[J4] =  m_pAllSolutionJ[i].Theta4 - m_pReferJ->Theta4;
		if( (diff0[J4] > (2*PI - 0.1)) && (diff0[J4] < 2*PI +0.1) )
		{
			diff0[J4] = diff0[J4] - 2*PI;
		}
		if(diff0[J4] < (-2*PI + 0.1) && (diff0[J4] > -2*PI - 0.1))
		{
			diff0[J4] = diff0[J4] + 2*PI;
		}

		diff0[J5] =  m_pAllSolutionJ[i].Theta5 - m_pReferJ->Theta5;
		if( (diff0[J5] > (2*PI - 0.1)) && (diff0[J5] < 2*PI +0.1) )
		{
			diff0[J5] = diff0[J5] - 2*PI;
		}
		if(diff0[J5] < (-2*PI + 0.1) && (diff0[J5] > -2*PI - 0.1))
		{
			diff0[J5] = diff0[J5] + 2*PI;
		}

		diff0[J6] =  m_pAllSolutionJ[i].Theta6 - m_pReferJ->Theta6;
		if( (diff0[J6] > (2*PI - 0.1)) && (diff0[J6] < 2*PI +0.1) )
		{
			diff0[J6] = diff0[J6] - 2*PI;

		}
		if(diff0[J6] < (-2*PI + 0.1) && (diff0[J6] > -2*PI - 0.1))
		{
			diff0[J6] = diff0[J6] + 2*PI;

		}

		diff0_pow[J1] = pow(diff0[J1],2);
		diff0_pow[J2] = pow(diff0[J2],2);
		diff0_pow[J3] = pow(diff0[J3],2);
		diff0_pow[J4] = pow(diff0[J4],2);
		diff0_pow[J5] = pow(diff0[J5],2);
		diff0_pow[J6] = pow(diff0[J6],2);
		diff0[J6] =  m_pAllSolutionJ[i].Theta6 - m_pReferJ->Theta6;
		if( (diff0[J6] > (2*PI - 0.1)) && (diff0[J6] < 2*PI +0.1) )
		{
			diff0[J6] = diff0[J6] - 2*PI;

		}
		if(diff0[J6] < (-2*PI + 0.1) && (diff0[J6] > -2*PI - 0.1))
		{
			diff0[J6] = diff0[J6] + 2*PI;

		}		

		sum[i]=sqrt(diff0_pow[J1]+diff0_pow[J2]+diff0_pow[J3]+
			diff0_pow[J4]+diff0_pow[J5]+diff0_pow[J6]);

		//这里应该对sum进行判断，看其是不是数字或者是不是无穷大的数，先暂不考虑  add by cwq	
		if (sum[i] < m_minSum)
		{
			m_minSum = sum[i];
			m_solutionId = i;
		}	
	}

	//找到最小值之后把相应的关节角放到m_pSelectJ中
	m_pSelectJ->Theta1 = (m_pAllSolutionJ+m_solutionId)->Theta1;
	m_pSelectJ->Theta2 = (m_pAllSolutionJ+m_solutionId)->Theta2;
	m_pSelectJ->Theta3 = (m_pAllSolutionJ+m_solutionId)->Theta3;
	m_pSelectJ->Theta4 = (m_pAllSolutionJ+m_solutionId)->Theta4;
	m_pSelectJ->Theta5 = (m_pAllSolutionJ+m_solutionId)->Theta5;
	m_pSelectJ->Theta6 = (m_pAllSolutionJ+m_solutionId)->Theta6;


	/////////////////针对J4范围受限于-180~180,扩大为-360~360//////////////////////20180123
	diff0[J4] =  m_pSelectJ->Theta4 - m_pReferJ->Theta4;
	if( (diff0[J4] > (2*PI - 0.1)) && (diff0[J4] < 2*PI +0.1) )
	{
		m_pSelectJ->Theta4 -= 2*PI;
	}
	if(diff0[J4] < (-2*PI + 0.1) && (diff0[J4] > -2*PI - 0.1))
	{
		m_pSelectJ->Theta4 += 2*PI;
	}
	//////////////////////////////////////////////////////////

	/////////////////20160410 chris and survivor///////////////////////////////////
	/////////////////针对J6范围受限于-180~180,扩大为-360~360//////////////////////
	diff0[J6] =  m_pSelectJ->Theta6 - m_pReferJ->Theta6;
	if( (diff0[J6] > (2*PI - 0.1)) && (diff0[J6] < 2*PI +0.1) )
	{
		m_pSelectJ->Theta6 -= 2*PI;
	}
	if(diff0[J6] < (-2*PI + 0.1) && (diff0[J6] > -2*PI - 0.1))
	{
		m_pSelectJ->Theta6 += 2*PI;
	}
	//////////////////////////////////////////////////////////

	if(fabs(m_pSelectJ->Theta5) < 0.03)
	{
		return KYN_SINGULAR;
	}

	//这地方需要考虑的是在什么情况下会出现选解失败？解得组数为零可能
	return m_solutionId;
}

//作用:检查各轴数据的有效性（是否超范围、角度是否跳变）             
MC_T_INT SafeDataCheck(const JOINT *pjnt)
{
	if (pjnt->Theta1 > robot.RobotInfo.MaxDhJnt[J1])
		robot.outRangeJnt = J1B;
	else if (pjnt->Theta1 < robot.RobotInfo.MinDhJnt[J1])
		robot.outRangeJnt = J1S;
	else if (pjnt->Theta2 > robot.RobotInfo.MaxDhJnt[J2])
		robot.outRangeJnt = J2B;
	else if (pjnt->Theta2 < robot.RobotInfo.MinDhJnt[J2])
		robot.outRangeJnt = J2S;
	else if (pjnt->Theta3 > robot.RobotInfo.MaxDhJnt[J3])
		robot.outRangeJnt = J3B;
	else if (pjnt->Theta3 < robot.RobotInfo.MinDhJnt[J3])
		robot.outRangeJnt = J3S;
	else if (pjnt->Theta4 > robot.RobotInfo.MaxDhJnt[J4])
		robot.outRangeJnt = J4B;
	else if (pjnt->Theta4 < robot.RobotInfo.MinDhJnt[J4])
		robot.outRangeJnt = J4S;
	else if (pjnt->Theta5 > robot.RobotInfo.MaxDhJnt[J5])
		robot.outRangeJnt = J5B;
	else if (pjnt->Theta5 < robot.RobotInfo.MinDhJnt[J5])
		robot.outRangeJnt = J5S;
	else if (pjnt->Theta6 > robot.RobotInfo.MaxDhJnt[J6])
		robot.outRangeJnt = J6B;
	else if (pjnt->Theta6 < robot.RobotInfo.MinDhJnt[J6])
		robot.outRangeJnt = J6S;
	else
	{
		robot.outRangeJnt = NONE;
		if(robot.wErrorID == KYN_JNT_OUT_RANGE)
			robot.wErrorID = 0;
	}

	if (robot.outRangeJnt != NONE)
		return KYN_JNT_OUT_RANGE;

	return 0;
}

/*  未使用！
	从文件中读取一行，结果存放在指定的地址
	输入参数：buf  字符型指针，指向存放内容的开始地址
	输出参数：buf  字符串，以\0结尾。
	返回值：整型，一行内容的字符个数，0 表示文件结束。
	size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	ptr -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
	size -- 这是要读取的每个元素的大小，以字节为单位。
	nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
*/
int read_a_line(char *buf,FILE *fp)
{
	int i = 0;

	while (fread(&buf[i], 1, 1, fp) != 0)
	{
		if (buf[i] == ',') 
			break;
		i++;
	}
	buf[i] = 'n';
	return i;
}
//未使用！
int getItem(char *pbuf, double *v)
{
	int i, j;
	char tmpBuf[20];

	i = 0;
	j = 0;
// 	while (pbuf[i] != ' ' && pbuf[i] != 'n')
// 	{
// 			tmpBuf[j] = pbuf[i];
// 			j++;
// 			i++;
// 	}
// 	tmpBuf[j] = 0;
// 	if (v != NULL)
// 		*v = atof(tmpBuf);
// 
// 	while (pbuf[i] == ' ')
// 	{
// 		i++;
// 		if (pbuf[i] != '\t')
// 		{
// 			return i+1;
// 		}
// 	}
// 	if (pbuf[i] == 'n')
// 	{
// 		return (i+1);
// 	}

	while (pbuf[i] != '\t' && pbuf[i] != 'n')
	{
		tmpBuf[j] = pbuf[i];
		j++;
		i++;	
	}
	tmpBuf[j] = 0;
	if (v != NULL)
		*v = atof(tmpBuf);
	return (i+1);	
}

//获取工具坐标系
CARTPOS getToolPos(_tcp *t)
{
	CARTPOS temp;
	temp.Px =t->para[0];
	temp.Py = t->para[1];
	temp.Pz = t->para[2];
	temp.Rpy.Roll = t->para[3]*PI/180;
	temp.Rpy.Pitch = t->para[4]*PI/180;
	temp.Rpy.Yaw = t->para[5]*PI/180;
	RpyToMat(&temp.Rpy,&temp.Rmatrix);
	return temp;
}

//获取工具坐标系姿态矩阵的逆矩阵,Matrix3d? Eigen?
void getTpInverse()
{
	Matrix3d tempM1,tempM2;
	tempM1 << robot.toolPos.Rmatrix.Nx,robot.toolPos.Rmatrix.Ox,robot.toolPos.Rmatrix.Ax,
		robot.toolPos.Rmatrix.Ny,robot.toolPos.Rmatrix.Oy,robot.toolPos.Rmatrix.Ay,
		robot.toolPos.Rmatrix.Nz,robot.toolPos.Rmatrix.Oz,robot.toolPos.Rmatrix.Az;
	tempM2 = tempM1.inverse();
	robot.tpInverse.Nx = tempM2(0);
	robot.tpInverse.Ny = tempM2(1);
	robot.tpInverse.Nz = tempM2(2);
	robot.tpInverse.Ox = tempM2(3);
	robot.tpInverse.Oy = tempM2(4);
	robot.tpInverse.Oz = tempM2(5);
	robot.tpInverse.Ax = tempM2(6);
	robot.tpInverse.Ay = tempM2(7);
	robot.tpInverse.Az = tempM2(8);
}

//计算三维矩阵与三维向量的乘积
CARTPOS	MultMatVec(RMATRIX R, Pos3d pos)
{
	CARTPOS cart;
	cart.Px = R.Nx*pos.Px + R.Ox*pos.Py + R.Ax*pos.Pz;
	cart.Py = R.Ny*pos.Px + R.Oy*pos.Py + R.Ay*pos.Pz;
	cart.Pz = R.Nz*pos.Px + R.Oz*pos.Py + R.Az*pos.Pz;
	return cart;
}

//计算三维矩阵与三维向量的乘积,再叠加一个基础向量
CARTPOS	MultMatVec(RMATRIX R, Pos3d pos, CARTPOS baseCart)
{
	CARTPOS cart;
	cart.Px = R.Nx*pos.Px + R.Ox*pos.Py + R.Ax*pos.Pz + baseCart.Px;
	cart.Py = R.Ny*pos.Px + R.Oy*pos.Py + R.Ay*pos.Pz + baseCart.Py;
	cart.Pz = R.Nz*pos.Px + R.Oz*pos.Py + R.Az*pos.Pz + baseCart.Pz;
	return cart;
}

//计算三维矩阵与三维向量的乘积,再叠加一个基础向量.CARTPOS类型输入向量
CARTPOS	MultMatVec(RMATRIX R, CARTPOS pos, CARTPOS baseCart)
{
	CARTPOS cart;
	cart.Px = R.Nx*pos.Px + R.Ox*pos.Py + R.Ax*pos.Pz + baseCart.Px;
	cart.Py = R.Ny*pos.Px + R.Oy*pos.Py + R.Ay*pos.Pz + baseCart.Py;
	cart.Pz = R.Nz*pos.Px + R.Oz*pos.Py + R.Az*pos.Pz + baseCart.Pz;
	return cart;
}

/*计算安全立方体*/
void calcSafeCube()
{
	//立方体的8个顶点 以六轴末端圆心为参考，以TCP在世界坐标系里的方向为依据，z+第1,2,3,4象限，而后z-第1,2,3,4象限。（x+，y+，z+）,（x-，y+，z+）,（x-，y-，z+）,（x+，y-，z+）,（x+，y+，z-）,（x-，y+，z-）,（x-，y-，z-）,（x+，y-，z-）...
	Pos3d vec;
	CARTPOS pos;
	//
	vec.Px = robot.sfCubePara.xp; vec.Py = robot.sfCubePara.yp; vec.Pz = robot.sfCubePara.zp; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[0] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xm; vec.Py = robot.sfCubePara.yp; vec.Pz = robot.sfCubePara.zp; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[1] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xm; vec.Py = robot.sfCubePara.ym; vec.Pz = robot.sfCubePara.zp; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[2] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xp; vec.Py = robot.sfCubePara.ym; vec.Pz = robot.sfCubePara.zp; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[3] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xp; vec.Py = robot.sfCubePara.yp; vec.Pz = robot.sfCubePara.zm; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[4] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xm; vec.Py = robot.sfCubePara.yp; vec.Pz = robot.sfCubePara.zm; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[5] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xm; vec.Py = robot.sfCubePara.ym; vec.Pz = robot.sfCubePara.zm; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[6] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);

	vec.Px = robot.sfCubePara.xp; vec.Py = robot.sfCubePara.ym; vec.Pz = robot.sfCubePara.zm; 
	pos = MultMatVec(robot.toolPos.Rmatrix,vec);
	robot.cubePos[7] = MultMatVec(robot.robotEndPos.Rmatrix,pos,robot.robotEndPos);
}

/*检查安全边界
robot.cubePos[i].Px < robot.sLimitPos.xMin...
*/
bool checkSafeBorder()
{
	calcSafeCube();
	int i;
	bool bSafeBorderX = true;
	bool bSafeBorderY = true;
	bool bSafeBorderZ = true;
	bool bSafeBorder = false;
	bool isX = (bool)robot.sfCubePara.isX;
	bool isY = (bool)robot.sfCubePara.isY;
	bool isZ = (bool)robot.sfCubePara.isZ;
	for(i = 0; i<8; i++)
	{
		if(isX && (robot.cubePos[i].Px < robot.sLimitPos.xMin || robot.cubePos[i].Px > robot.sLimitPos.xMax)) //判断x方向，以内包含规则判断，后续可以加外延规则，即大于最大值或小于最小值则安全
		{
			bSafeBorderX = false;
			break;
		}
		if(isY && (robot.cubePos[i].Py < robot.sLimitPos.yMin || robot.cubePos[i].Py > robot.sLimitPos.yMax)) //判断z方向，以内包含规则判断
		{
			bSafeBorderZ = false;
			break;
		}
		if(isZ && (robot.cubePos[i].Pz < robot.sLimitPos.zMin || robot.cubePos[i].Pz > robot.sLimitPos.zMax)) //判断z方向，以内包含规则判断
		{
			bSafeBorderZ = false;
			break;
		}			
	}

	if(bSafeBorderX && bSafeBorderY && bSafeBorderZ)//两个条件只要有一个满足就可以,这里没有用Y方向判断
		bSafeBorder = true;

	return bSafeBorder;
}

void Robot::InitRobotState()
{
	wExecute = 0;
	wBuffer = 0;
	wRemainInst = 0;
	bBufferFull = false;
	bFinished = false;
	bExecute = false;	
	bExecuteNew = true;
	bPushNew = true;
	bTrans = false;
	bConstantVel = false;
	nReadyForPush = 0;
	bInitIn = false;
	bInitOut = false;
	bIO = false;
	ioGetId = 2;
	bIoGet = true;
	bReverseIo = false;
	bReadyForRotate = false;
	bCanBeCalled = true;
	bSafeForRotate = false;
	nWorksNum = 0;
	nCallCnt = 0;
	nCallNum = 0;
	tempScanFile = NULL;
	bGenFile = false;
	nGenFileCycle = 0;
	isBoundSafe = true;
	bSetBound = false;
#ifdef _SCAN_SEW_
	nScanTime = 0;
#endif
}
//初始化机器人参数
EC_T_BOOL Robot::initRobotFile()
{
	double tempDouble =0;
	int tempInt =0;
	float tempFloat =0;
	char WHString[80];
	InitRobotState();
	nTcpIndex = 0; //只初始化一次的参数
	//robot.scanPosFile = fopen("scanPos.csv","w");

	//robotFile = fopen("simu.csv","a+");
	myGetPrivateProfileString("MAX_JOINT", "J1", "150", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG,读取配置文件
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta1 = tempDouble;
	myGetPrivateProfileString("MAX_JOINT", "J2", "70", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta2 = tempDouble;
	myGetPrivateProfileString("MAX_JOINT", "J3", "60", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta3 = tempDouble;
	myGetPrivateProfileString("MAX_JOINT", "J4", "180", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta4 = tempDouble;
	myGetPrivateProfileString("MAX_JOINT", "J5", "125", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta5 = tempDouble;
	myGetPrivateProfileString("MAX_JOINT", "J6", "360", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.max_joint.Theta6 = tempDouble;
	//获取最小关节角

	myGetPrivateProfileString("MIN_JOINT", "J1", "-150", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta1 = tempDouble;
	myGetPrivateProfileString("MIN_JOINT", "J2", "-45", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta2 = tempDouble;
	myGetPrivateProfileString("MIN_JOINT", "J3", "-70", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta3 = tempDouble;
	myGetPrivateProfileString("MIN_JOINT", "J4", "-180", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta4 = tempDouble;
	myGetPrivateProfileString("MIN_JOINT", "J5", "-125", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta5 = tempDouble;
	myGetPrivateProfileString("MIN_JOINT", "J6", "-360", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);
	RobotInfo.min_joint.Theta6 = tempDouble;

	/************************************************************************/
	/* DH模型中的最大最小角                                                 */
	/************************************************************************/
	RobotInfo.MaxDhJnt[J1] = (RobotInfo.max_joint.Theta1)/180*PI;
	RobotInfo.MaxDhJnt[J2] = (RobotInfo.max_joint.Theta2-90)/180*PI;
	RobotInfo.MaxDhJnt[J3] = (RobotInfo.max_joint.Theta3)/180*PI;
	RobotInfo.MaxDhJnt[J4] = (RobotInfo.max_joint.Theta4)/180*PI;
	RobotInfo.MaxDhJnt[J5] = (RobotInfo.max_joint.Theta5)/180*PI;
	RobotInfo.MaxDhJnt[J6] = (RobotInfo.max_joint.Theta6)/180*PI;

	RobotInfo.MinDhJnt[J1] = (RobotInfo.min_joint.Theta1)/180*PI;
	RobotInfo.MinDhJnt[J2] = (RobotInfo.min_joint.Theta2-90)/180*PI;
	RobotInfo.MinDhJnt[J3] = (RobotInfo.min_joint.Theta3)/180*PI;
	RobotInfo.MinDhJnt[J4] = (RobotInfo.min_joint.Theta4)/180*PI;
	RobotInfo.MinDhJnt[J5] = (RobotInfo.min_joint.Theta5)/180*PI;
	RobotInfo.MinDhJnt[J6] = (RobotInfo.min_joint.Theta6)/180*PI;
	//////////////////////////////////////////////////////////////////////////


	//机器人各关节减速比
	myGetPrivateProfileString("RE_RADIO", "J1", "171", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);

	RobotInfo.re_radio[J1] = tempDouble;
	myGetPrivateProfileString("RE_RADIO", "J2", "164.0769", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);


	RobotInfo.re_radio[J2] = tempDouble;
	myGetPrivateProfileString("RE_RADIO", "J3", "126", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);


	RobotInfo.re_radio[J3] = tempDouble;
	myGetPrivateProfileString("RE_RADIO", "J4", "118.3846", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);


	RobotInfo.re_radio[J4] = tempDouble;
	myGetPrivateProfileString("RE_RADIO", "J5", "80", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);


	RobotInfo.re_radio[J5] = tempDouble;
	myGetPrivateProfileString("RE_RADIO", "J6", "80", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempDouble = atof(WHString);


	RobotInfo.re_radio[J6] = tempDouble;

	//获取编码器每圈脉冲数 17位绝对值编码器
	myGetPrivateProfileString("CNT_PER_CYCLE", "cnt_per_cycle", "131072", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt  = atoi(WHString);
	RobotInfo.cnt_per_cycle = tempInt;

	//计算归一化因子
	RobotInfo.JntUniRadio[J1] = 1;
	RobotInfo.JntUniRadio[J2] = RobotInfo.re_radio[J2]/RobotInfo.re_radio[J1];
	RobotInfo.JntUniRadio[J3] = RobotInfo.re_radio[J3]/RobotInfo.re_radio[J1];
	RobotInfo.JntUniRadio[J4] = RobotInfo.re_radio[J4]/RobotInfo.re_radio[J1];
	RobotInfo.JntUniRadio[J5] = RobotInfo.re_radio[J5]/RobotInfo.re_radio[J1];
	RobotInfo.JntUniRadio[J6] = RobotInfo.re_radio[J6]/RobotInfo.re_radio[J1];

	/************************************************************************/
	/* 各轴电机最大转速（单位: r/min）                                      */
	/************************************************************************/
	RobotInfo.MaxMotorSpeed[J1] = 1500;
	RobotInfo.MaxMotorSpeed[J2] = 1500;
	RobotInfo.MaxMotorSpeed[J3] = 1500;
	RobotInfo.MaxMotorSpeed[J4] = 1500;
	RobotInfo.MaxMotorSpeed[J5] = 1500;
	RobotInfo.MaxMotorSpeed[J6] = 1500;

	/************************************************************************/
	/* 机器人每ms关节的最大移动量(弧度/毫秒)                                */
	/************************************************************************/
	RobotInfo.MaxJntPerMs[J1] = RobotInfo.MaxMotorSpeed[J1]/60/1000/RobotInfo.re_radio[J1]*2*PI;
	RobotInfo.MaxJntPerMs[J2] = RobotInfo.MaxMotorSpeed[J2]/60/1000/RobotInfo.re_radio[J2]*2*PI;
	RobotInfo.MaxJntPerMs[J3] = RobotInfo.MaxMotorSpeed[J3]/60/1000/RobotInfo.re_radio[J3]*2*PI;
	RobotInfo.MaxJntPerMs[J4] = RobotInfo.MaxMotorSpeed[J4]/60/1000/RobotInfo.re_radio[J4]*2*PI;
	RobotInfo.MaxJntPerMs[J5] = RobotInfo.MaxMotorSpeed[J5]/60/1000/RobotInfo.re_radio[J5]*2*PI;
	RobotInfo.MaxJntPerMs[J6] = RobotInfo.MaxMotorSpeed[J6]/60/1000/RobotInfo.re_radio[J6]*2*PI;

	/************************************************************************/
	/* 获取编码器原点                                                       */
	/************************************************************************/
	myGetPrivateProfileString("ENC_HOME", "J1", "-15434", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J1] = tempInt;    //45335

	myGetPrivateProfileString("ENC_HOME", "J2", "3025189", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J2] = tempInt;  //2664706

	myGetPrivateProfileString("ENC_HOME", "J3", "-2288582", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J3] = tempInt; //-2063030;

	myGetPrivateProfileString("ENC_HOME", "J4", "78902", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J4] = tempInt;   //292611;	

	myGetPrivateProfileString("ENC_HOME", "J5", "-2666195", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J5] = tempInt;   //-867643;

	myGetPrivateProfileString("ENC_HOME", "J6", "16368567", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.enc_home[J6] = tempInt;   //661985;

#ifdef _3D_SEWING_
	myGetPrivateProfileString("ENC", "SWAY_HOOK_ENC", "16368567", WHString, 80, "system\\ExpInfor.ini");
	tempInt = atoi(WHString);
	sewCalcPara.swayHookEnc = tempInt;

	myGetPrivateProfileString("ENC", "SPINDLE_HOME_ENC", "16368567", WHString, 80, "system\\ExpInfor.ini");
	tempInt = atoi(WHString);
	sewCalcPara.spindleHomeEnc = tempInt;
#endif

	/*机器人最高速度为60度/s */
	RobotInfo.MaxSpeed = 100;
	RobotInfo.MaxSpeedAcc = 300;

	/*机器人最大线速度为100mm/s = 1m/s */
	RobotInfo.MaxLineSpeed = 500;
	/*机器人最大线加速度为100mm/s2 = 1m/s2*/
	RobotInfo.MaxLineAcc = 600;

	/*机器人默认速度与最高速度的百分比*/
	RobotInfo.RobotDyn.vel_percent = 100;

	/*机器人默认加速度与最高加速度的百分比*/
	RobotInfo.RobotDyn.acc_percent = 100;

	//杰瑞12kg机器人
	myGetPrivateProfileString("ROBOTLINK", "L1", "433", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L1 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L2", "670", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L2 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L3", "115", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L3 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L4", "725", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L4 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L5", "139.9", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L5 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L6", "185", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L6 = tempFloat;

	myGetPrivateProfileString("ROBOTLINK", "L7", "10", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.RobotLink.L7 = tempFloat;

	//电机方向与DH方向的正负关系
	myGetPrivateProfileString("MOTOR_DIR", "J1", "-1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J1] = tempInt;

	myGetPrivateProfileString("MOTOR_DIR", "J2", "1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J2] = tempInt;

	myGetPrivateProfileString("MOTOR_DIR", "J3", "1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J3] = tempInt; 

	myGetPrivateProfileString("MOTOR_DIR", "J4", "-1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J4] = tempInt;

	myGetPrivateProfileString("MOTOR_DIR", "J5", "1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J5] = tempInt;

	myGetPrivateProfileString("MOTOR_DIR", "J6", "-1", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempInt = atoi(WHString);
	RobotInfo.motorDir[J6] = tempInt;

	//4,5,6轴耦合比
	myGetPrivateProfileString("COUPLING_RATIO", "R45", "0", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.cpRatio[R45] = tempFloat;

	myGetPrivateProfileString("COUPLING_RATIO", "R46", "0", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.cpRatio[R46] = tempFloat;

	myGetPrivateProfileString("COUPLING_RATIO", "R56", "0", WHString, 80, "system\\RobotInfor.ini");/////////默认12KG
	tempFloat = (float)atof(WHString);
	RobotInfo.cpRatio[R56] = tempFloat;


	//DH模型初始角度
	RobotInfo.joint_home[J1] = 0;
	RobotInfo.joint_home[J2] = -PI/2;
	RobotInfo.joint_home[J3] = 0;
	RobotInfo.joint_home[J4] = 0;
	RobotInfo.joint_home[J5] = 0;
	RobotInfo.joint_home[J6] = 0;

	//DH 方向与实际相同为 -1，相反为1 2015.11.01
	// 	RobotInfo.motorDir[J1]=-1;    //12kg
	// 	RobotInfo.motorDir[J2]=1;     
	// 	RobotInfo.motorDir[J3]=1;    
	// 	RobotInfo.motorDir[J4]=-1;   
	// 	RobotInfo.motorDir[J5]=1;     
	// 	RobotInfo.motorDir[J6]=-1;    

	// 	RobotInfo.motorDir[J1]=-1;    //60kg
	// 	RobotInfo.motorDir[J2]=1;     
	// 	RobotInfo.motorDir[J3]=1;    
	// 	RobotInfo.motorDir[J4]=-1;   
	// 	RobotInfo.motorDir[J5]=1;     
	// 	RobotInfo.motorDir[J6]=-1; 
	// 
	// 	RobotInfo.motorDir[J1]=-1;    
	// 	RobotInfo.motorDir[J2]=1;     
	// 	RobotInfo.motorDir[J3]=1;    
	// 	RobotInfo.motorDir[J4]=1;   
	// 	RobotInfo.motorDir[J5]=1;     
	// 	RobotInfo.motorDir[J6]=1;  

	// 	//机器人各关节最大速度对应的编码器步长
	// 	RobotInfo.MaxEncStep[J1] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J1]/360);
	// 	RobotInfo.MaxEncStep[J2] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J2]/360);
	// 	RobotInfo.MaxEncStep[J3] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J3]/360);
	// 	RobotInfo.MaxEncStep[J4] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J4]/360);
	// 	RobotInfo.MaxEncStep[J5] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J5]/360);
	// 	RobotInfo.MaxEncStep[J6] = (int)(RobotInfo.MaxSpeed*RobotInfo.cnt_per_cycle*RobotInfo.re_radio[J6]/360);

	RobotInfo.manual_rate[J1] = (int)RobotInfo.re_radio[J1]*RobotInfo.cnt_per_cycle/360/1000*4; //
	RobotInfo.manual_rate[J2] = (int)RobotInfo.re_radio[J2]*RobotInfo.cnt_per_cycle/360/1000*4;
	RobotInfo.manual_rate[J3] = (int)RobotInfo.re_radio[J3]*RobotInfo.cnt_per_cycle/360/1000*4;
	RobotInfo.manual_rate[J4] = (int)RobotInfo.re_radio[J4]*RobotInfo.cnt_per_cycle/360/1000*4;
	RobotInfo.manual_rate[J5] = (int)RobotInfo.re_radio[J5]*RobotInfo.cnt_per_cycle/360/1000*4;
	RobotInfo.manual_rate[J6] = (int)RobotInfo.re_radio[J6]*RobotInfo.cnt_per_cycle/360/1000*4;

	MC_T_REAL j_max[6];
	MC_T_REAL j_min[6];

	//获取编码器最大范围
	j_max[J1] = RobotInfo.max_joint.Theta1;
	j_max[J2] = RobotInfo.max_joint.Theta2;
	j_max[J3] = RobotInfo.max_joint.Theta3;
	j_max[J4] = RobotInfo.max_joint.Theta4;
	j_max[J5] = RobotInfo.max_joint.Theta5;
	j_max[J6] = RobotInfo.max_joint.Theta6;
	AngleToEncoder(j_max, RobotInfo.max_enc);
	for(int i = 0; i < 6; i++)
	{
		RobotInfo.max_enc[i] = (int)(RobotInfo.max_enc[i]*RobotInfo.re_radio[i]);
		RobotInfo.max_enc[i] += RobotInfo.enc_home[i];
	}

	//获取编码器最小范围
	j_min[J1] = RobotInfo.min_joint.Theta1;
	j_min[J2] = RobotInfo.min_joint.Theta2;
	j_min[J3] = RobotInfo.min_joint.Theta3;
	j_min[J4] = RobotInfo.min_joint.Theta4;
	j_min[J5] = RobotInfo.min_joint.Theta5;
	j_min[J6] = RobotInfo.min_joint.Theta6;
	AngleToEncoder(j_min, RobotInfo.min_enc);

	for (int i = 0; i < 6; i++)
	{
		RobotInfo.min_enc[i] = (int) (RobotInfo.min_enc[i]*RobotInfo.re_radio[i]);
		RobotInfo.min_enc[i] += RobotInfo.enc_home[i];
	}

	//这里应该初始化工具坐标系与末端坐标系之间的相对位置
	//cwq2015.11.1工具末端相对于第六轴x y z偏差值
	// 	RobotInfo.toolRelativeEndPos.X = 0;//-4.54;//
	// 	RobotInfo.toolRelativeEndPos.Y = 0;//10.76;
	// 	RobotInfo.toolRelativeEndPos.Z = 0;//-380.32;

	//2015.11.02  revise_l
	// 	RobotInfo.toolRelativeEndPos.X = lh_tool.Vx;//7.017;//4.44
	// 	RobotInfo.toolRelativeEndPos.Y = lh_tool.Vy;//-19.16;//23.63
	// 	RobotInfo.toolRelativeEndPos.Z = lh_tool.Vz;//413.3;//423.03


	//机器人每个运动周期最大的运动脉冲数,用来对要发送的脉冲进行连续性检测  add by cwq at 2015.9.15  
	MC_T_REAL m_tempJnt[6];
	m_tempJnt[J1] = RobotInfo.MaxJntPerMs[J1]*180/PI;
	m_tempJnt[J2] = RobotInfo.MaxJntPerMs[J2]*180/PI;
	m_tempJnt[J3] = RobotInfo.MaxJntPerMs[J3]*180/PI;
	m_tempJnt[J4] = RobotInfo.MaxJntPerMs[J4]*180/PI;
	m_tempJnt[J5] = RobotInfo.MaxJntPerMs[J5]*180/PI;
	m_tempJnt[J6] = RobotInfo.MaxJntPerMs[J6]*180/PI;

	AngleToEncoder(m_tempJnt, RobotInfo.MaxPulsePerMs);

	for (int i = 0; i < 6; i++)
	{
		RobotInfo.MaxPulsePerMs[i] = (int) (RobotInfo.MaxPulsePerMs[i]*RobotInfo.re_radio[i]);
		//RobotInfo.MaxPulsePerMs[i] += RobotInfo.enc_home[i];
	}
	
	//tcp初始化
	toolPos.Px = 0;			toolPos.Py = 0;			toolPos.Pz = 0;
	toolPos.Rmatrix.Nx = 1; toolPos.Rmatrix.Ox = 0; toolPos.Rmatrix.Ax = 0;
	toolPos.Rmatrix.Ny = 0; toolPos.Rmatrix.Oy = 1; toolPos.Rmatrix.Ay = 0;
	toolPos.Rmatrix.Nz = 0; toolPos.Rmatrix.Oz = 0; toolPos.Rmatrix.Az = 1;

	getTpInverse();

	return true;
}


//反解一个点得出编码器值
MC_T_INT invCart2Enc(CARTPOS *pdPos,JOINT *pRefJoint,JNTS_ENC *pEncoder)
{
	JOINT *pJoint = (JOINT *)malloc(sizeof(JOINT)*8);
	JOINT *m_pReasJoint = (JOINT *)malloc(sizeof(JOINT)*8);
	JOINT *m_pSelectJoint = (JOINT *)malloc(sizeof(JOINT));

	int wErrorID = MC_NO_ERROR;

	wErrorID = Inverse(pdPos,&robot.RobotInfo.RobotLink,pJoint);
	MC_T_INT m_reaSoluNum = checkAngle(pJoint, 8, m_pReasJoint);
	if (m_reaSoluNum == 0)
	{
		//没有合理逆解，返回报错
		wErrorID = INVERSE_FAIL;
		return wErrorID;
	} 
	else
	{			
		//返回值大于7表示选解失败,否则继续选解，并把选择的解放在m_pSelectJoint+i中
		int sel_val = selectSolutionByMinMove(m_pReasJoint,m_reaSoluNum, pRefJoint, m_pSelectJoint);
		//printf("after select : %.12f\n",m_pSelectJoint->Theta4);
		if(sel_val == KYN_SINGULAR)
		{
			wErrorID = KYN_SINGULAR;//revise_l
			return wErrorID;
		}

		//检测当前选择的角度与上一组角度之间是否有跳变

		if (checkJntContinuity(pRefJoint,m_pSelectJoint) == 0)
		{
			wErrorID = SALTUS_STEP;//revise_l  跳变
			return wErrorID;
		}
		else
		{
			if( m_pSelectJoint->Theta3 > PI*140/180 && m_pSelectJoint->Theta3 < PI)
			{
				m_pSelectJoint->Theta3 = m_pSelectJoint->Theta3 - 2*PI;
			}

			//20180123 解除-180~180限制
			/*if(m_pSelectJoint->Theta4 > PI)
			{
			m_pSelectJoint->Theta4 = m_pSelectJoint->Theta4 - 2*PI;
			}
			if(m_pSelectJoint->Theta4 < -PI)
			{
			m_pSelectJoint->Theta4 = m_pSelectJoint->Theta4 + 2*PI;
			}
			*/
			if(m_pSelectJoint->Theta5 > PI)
			{
				m_pSelectJoint->Theta5 = m_pSelectJoint->Theta5 - 2*PI;
			}
			if(m_pSelectJoint->Theta5 < -PI)
			{
				m_pSelectJoint->Theta5 = m_pSelectJoint->Theta5 + 2*PI;
			}

			//把选择的角度序列作为下次选择的参考角度
			pRefJoint->Theta1 = m_pSelectJoint->Theta1;
			pRefJoint->Theta2 = m_pSelectJoint->Theta2;
			pRefJoint->Theta3 = m_pSelectJoint->Theta3;
			pRefJoint->Theta4 = m_pSelectJoint->Theta4;
			pRefJoint->Theta5 = m_pSelectJoint->Theta5;
			pRefJoint->Theta6 = m_pSelectJoint->Theta6;
			//printf("after check : %.12f\n",pRefJoint->Theta4);
			if(SafeDataCheck(m_pSelectJoint) != MC_NO_ERROR)
			{
				wErrorID = KYN_JNT_OUT_RANGE;
				robot.wErrorID = KYN_JNT_OUT_RANGE;
				return wErrorID;
			}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		}
	}
	DHAngle2Encoder(m_pSelectJoint,pEncoder);
	free(pJoint);
	pJoint = NULL;
	free(m_pReasJoint);
	m_pReasJoint = NULL;
	free(m_pSelectJoint);
	m_pSelectJoint = NULL;
	return wErrorID;
}


void TransPlane(const CARTPOS pos1, const CARTPOS pos2,const CARTPOS pos3, RMATRIX *RM)
{
	//1点到2点之间的距离
	double d12;
	//向量p12和向量p13
	double p1_x,p1_y,p1_z;
	double p2_x,p2_y,p2_z;
	//向量p12和向量p13的法向量
	double a,b,c;
	//p12和p13法向量的模
	double d_v;
	p1_x = pos2.Px-pos1.Px;
	p1_y = pos2.Py-pos1.Py;
	p1_z = pos2.Pz-pos1.Pz;
	p2_x = pos3.Px-pos1.Px;
	p2_y = pos3.Py-pos1.Py;
	p2_z = pos3.Pz-pos1.Pz;
	//求向量p12和向量p13的法向量	
	a = p1_y*p2_z - p1_z*p2_y;
	b = p1_z*p2_x - p2_z*p1_x;
	c = p1_x*p2_y - p2_x*p1_y;

	d12 = sqrt( pow(pos2.Px-pos1.Px,2)+pow(pos2.Py-pos1.Py,2)+pow(pos2.Pz-pos1.Pz,2) );
	//求向量p12的单位向量
	RM->Nx = (pos2.Px-pos1.Px)/d12;
	RM->Ny = (pos2.Py-pos1.Py)/d12;
	RM->Nz = (pos2.Pz-pos1.Pz)/d12;
	//求z轴方向上的单位向量
	d_v = sqrt( pow(a,2)+pow(b,2)+pow(c,2));
	RM->Ax = a/d_v;
	RM->Ay = b/d_v;
	RM->Az = c/d_v;
	//求y轴方向上的单位向量=z×x=（z2*x3-z3*x2,z3*x1-z1*x3,z1*x2-z2*x1）
	RM->Ox = RM->Ay*RM->Nz - RM->Az*RM->Ny;
	RM->Oy = RM->Az*RM->Nx - RM->Ax*RM->Nz;
	RM->Oz = RM->Ax*RM->Ny - RM->Ay*RM->Nx;
}

void CalcNewVel(double* vel, float dist, float time)
{
	switch(robot.weldpara.waveType)
	{
	case TRIGLR_PLANE:
		{
			if(robot.weldpara.bStayStill)
				*vel = dist * (1+robot.weldpara.wTech.fFre*robot.weldpara.wTech.fLStay+robot.weldpara.wTech.fFre*robot.weldpara.wTech.fRStay)/time;
			break;
		}
	case TRIGLR_3D:
		{
			if(robot.weldpara.bStayStill)
				*vel = dist * (1/robot.weldpara.wTech.fFre+robot.weldpara.wTech.fLStay+robot.weldpara.wTech.fRStay+robot.weldpara.wTech.fMStay)/time/(1/3.4142/robot.weldpara.wTech.fFre);
			else
				*vel = dist * (1/robot.weldpara.wTech.fFre+robot.weldpara.wTech.fLStay+robot.weldpara.wTech.fRStay+robot.weldpara.wTech.fMStay)/time/((1/3.4142/robot.weldpara.wTech.fFre)+robot.weldpara.wTech.fLStay+robot.weldpara.wTech.fRStay+robot.weldpara.wTech.fMStay);
			break;
		}
	case TRIGLR_CHR:
		{
			*vel = (2*robot.weldpara.fBackDist + dist/time*(1/robot.weldpara.wTech.fFre + robot.weldpara.wTech.fMStay))/(1/robot.weldpara.wTech.fFre - 1.414*robot.weldpara.wTech.fAmp/robot.weldpara.fHriVel);
			robot.weldpara.fVel = *vel;
			break;
		}
	}
}

void CalcWeldPara(float time, float fDist, CARTPOS startPos , CARTPOS endPos)
{
	switch (robot.weldpara.waveType)
	{
	case TRIGLR_PLANE:
		{
			robot.weldpara.N = floor(time*1e6/robot.dwCycleTime);
			robot.weldpara.N2 = floor(robot.weldpara.wTech.fLStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N4 = floor(robot.weldpara.wTech.fRStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N1 = floor(1.0/robot.weldpara.wTech.fFre*1e6 / (4*robot.dwCycleTime));
			robot.weldpara.N3 = 2 * robot.weldpara.N1;
			robot.weldpara.N5 = robot.weldpara.N1;
			robot.weldpara.Nx = robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5;
			robot.weldpara.waveFrame.Ax = startPos.Rmatrix.Ax;
			robot.weldpara.waveFrame.Ay = startPos.Rmatrix.Ay;
			robot.weldpara.waveFrame.Az = startPos.Rmatrix.Az;
			break;
		}
	case TRIGLR_3D:
		{
			robot.weldpara.N = floor(time*1e6/robot.dwCycleTime);
			robot.weldpara.N2 = floor(robot.weldpara.wTech.fLStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N4 = floor(robot.weldpara.wTech.fMStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N1 = floor(1.0/robot.weldpara.wTech.fFre*1e6 / (3.4142*robot.dwCycleTime));
			robot.weldpara.N3 = 1.4142 * robot.weldpara.N1;
			robot.weldpara.N5 = robot.weldpara.N1;
			robot.weldpara.N6 = floor(robot.weldpara.wTech.fRStay*1e6 / robot.dwCycleTime);
			robot.weldpara.Nx = robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6;
			robot.weldpara.waveFrame.Ax = 0;
			robot.weldpara.waveFrame.Ay = 0;
			robot.weldpara.waveFrame.Az = 1;
			break;
		}

	case TRIGLR_CHR:
		{
			robot.weldpara.N = floor(time*1e6/robot.dwCycleTime);
			robot.weldpara.N1 = floor(robot.weldpara.wTech.fMStay *1e6 / robot.dwCycleTime);
			robot.weldpara.N2 = floor(robot.weldpara.fBackDist/robot.weldpara.fVel *1e6 / robot.dwCycleTime);
			robot.weldpara.N3 = floor(1.414*robot.weldpara.wTech.fAmp/robot.weldpara.fHriVel *1e6 / robot.dwCycleTime);
			robot.weldpara.N4 = floor((robot.weldpara.fBackDist + fDist/time*(1/robot.weldpara.wTech.fFre + robot.weldpara.wTech.fMStay))/robot.weldpara.fVel *1e6 / robot.dwCycleTime);
			robot.weldpara.Nx = robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4;
			TransPlane(startPos,robot.weldpara.planeCalcPos,endPos,&robot.weldpara.waveFrame);//此处只利用该函数求法向量，即Ax，Ay，Az
			break;
		}

	case TRIGLR_LEAN:
		{
			robot.weldpara.N = floor(time*1e6/robot.dwCycleTime);
			robot.weldpara.N2 = floor(robot.weldpara.wTech.fLStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N4 = floor(robot.weldpara.wTech.fMStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N6 = floor(robot.weldpara.wTech.fRStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N8 = floor(robot.weldpara.wTech.fMStay*1e6 / robot.dwCycleTime);
			robot.weldpara.N1 = floor(1.0/robot.weldpara.wTech.fFre*1e6 / (4*robot.dwCycleTime));
			robot.weldpara.N3 = robot.weldpara.N1;
			robot.weldpara.N5 = robot.weldpara.N1;
			robot.weldpara.N7 = robot.weldpara.N1;
			robot.weldpara.Nx = robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6+robot.weldpara.N7+robot.weldpara.N8;

			robot.weldpara.Nk = time/(1.0/robot.weldpara.wTech.fFre + robot.weldpara.wTech.fLStay + robot.weldpara.wTech.fMStay*2.0 + robot.weldpara.wTech.fRStay);//摆动周期数
			if (robot.weldpara.bStayStill)
				robot.weldpara.fWaveDist = fDist/robot.weldpara.Nk;
			else
				robot.weldpara.fWaveDist = fDist/robot.weldpara.Nk - fDist/time*(robot.weldpara.wTech.fLStay + robot.weldpara.wTech.fMStay*2.0 + robot.weldpara.wTech.fRStay);
			robot.weldpara.fVright = (robot.weldpara.wTech.fAmp*tan(robot.weldpara.fLeanAngle*PI/180.0) - 1.0/4.0*robot.weldpara.fWaveDist)*4.0*robot.weldpara.wTech.fFre;// Lright = A*tan(beta) - 1/4*fWaveDist 
			robot.weldpara.fVleft = (robot.weldpara.wTech.fAmp*tan(robot.weldpara.fLeanAngle*PI/180.0) + 1.0/4.0*robot.weldpara.fWaveDist)*4.0*robot.weldpara.wTech.fFre;// Lleft = A*tan(beta) + 1/4*fWaveDist 
			robot.weldpara.fVright = robot.weldpara.fVright * robot.dwCycleTime / 1E6;
			robot.weldpara.fVleft = robot.weldpara.fVleft * robot.dwCycleTime / 1E6;
			robot.weldpara.waveFrame.Ax = startPos.Rmatrix.Ax;
			robot.weldpara.waveFrame.Ay = startPos.Rmatrix.Ay;
			robot.weldpara.waveFrame.Az = startPos.Rmatrix.Az;
			break;
		}

	case NO_WAVE:
		{
			robot.weldpara.waveFrame.Ax = startPos.Rmatrix.Ax;
			robot.weldpara.waveFrame.Ay = startPos.Rmatrix.Ay;
			robot.weldpara.waveFrame.Az = startPos.Rmatrix.Az;

			robot.weldpara.waveFrame.Ax = 0;//激光传感器没有标定的时候用此组参数
			robot.weldpara.waveFrame.Ay = 0;
			robot.weldpara.waveFrame.Az = 1;
			break;
		}

	default:
		break;
	}

	robot.weldpara.waveFrame.Ox = (endPos.Px - startPos.Px) / fDist;
	robot.weldpara.waveFrame.Oy	= (endPos.Py - startPos.Py) / fDist;
	robot.weldpara.waveFrame.Oz	= (endPos.Pz - startPos.Pz) / fDist;

	robot.weldpara.waveFrame.Nx = robot.weldpara.waveFrame.Oy*robot.weldpara.waveFrame.Az - robot.weldpara.waveFrame.Oz*robot.weldpara.waveFrame.Ay;
	robot.weldpara.waveFrame.Ny = robot.weldpara.waveFrame.Oz*robot.weldpara.waveFrame.Ax - robot.weldpara.waveFrame.Ox*robot.weldpara.waveFrame.Az;
	robot.weldpara.waveFrame.Nz = robot.weldpara.waveFrame.Ox*robot.weldpara.waveFrame.Ay - robot.weldpara.waveFrame.Oy*robot.weldpara.waveFrame.Ax;

	robot.weldpara.waveFrame.Ax = robot.weldpara.waveFrame.Ny*robot.weldpara.waveFrame.Oz - robot.weldpara.waveFrame.Nz*robot.weldpara.waveFrame.Oy;
	robot.weldpara.waveFrame.Ay = robot.weldpara.waveFrame.Nz*robot.weldpara.waveFrame.Ox - robot.weldpara.waveFrame.Nx*robot.weldpara.waveFrame.Oz;
	robot.weldpara.waveFrame.Az = robot.weldpara.waveFrame.Nx*robot.weldpara.waveFrame.Oy - robot.weldpara.waveFrame.Ny*robot.weldpara.waveFrame.Ox;

	robot.weldpara.nCycleNum = 0;
}

//起始参考点更新 输入：当前参考点 当前周期是否停留
void UpdateStartPos(CARTPOS *pos , float fNextVel)
{
	//CARTPOS tempPos;

	switch (robot.weldpara.waveType)
	{
	case TRIGLR_PLANE:
		{
			if(robot.weldpara.nCycleNum<robot.weldpara.N)
			{	   
				int ik=robot.weldpara.nCycleNum % robot.weldpara.Nx;
				if((ik>=0) && (ik<=(robot.weldpara.N1-1)))
				{
					//N1阶段	     
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N1;
					robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else if((ik>=robot.weldpara.N1) && (ik<=(robot.weldpara.N1+robot.weldpara.N2-1)))
				{
					//N2阶段
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						robot.weldpara.bStilling = true;
					}
					else                                                                                             
						robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Px = 0;					
					robot.weldpara.wavePos.Pz = 0;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3-1)))		   
				{
					//N3阶段
					robot.weldpara.wavePos.Px = -2.0*robot.weldpara.wTech.fAmp/robot.weldpara.N3;
					robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}

				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4-1)))			   
				{
					//N4阶段
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						robot.weldpara.bStilling = true;
					}
					else
						robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Px = 0;					
					robot.weldpara.wavePos.Pz = 0;
				}
				else if(ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4) && (ik<=robot.weldpara.Nx-1)) 				   
				{
					//N5阶段
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N5;
					robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				robot.weldpara.nCycleNum++;
				pos->Px += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nx + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Ox + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ax;
				pos->Py += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Ny + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oy + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ay;
				pos->Pz += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nz + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oz + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Az;
			}
			else
				robot.weldpara.bStilling = false;

			robot.weldpara.totalPx += robot.weldpara.wavePos.Px;
			robot.laserTrack.posY = 0;
			if(robot.weldpara.nCycleNum % 50 == 1)
				robot.laserTrack.fNextOffset = (robot.laserTrack.posY - robot.weldpara.totalPx)/50;
			if(robot.laserTrack.bLaserTrack)
			{			
				pos->Px += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nx;
				pos->Py += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Ny;
				pos->Pz += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nz;
				robot.weldpara.totalPx += robot.laserTrack.fNextOffset;
			}	

			break;
		}
	case TRIGLR_3D:
		{
			if(robot.weldpara.nCycleNum<robot.weldpara.N)
			{	   
				int ik=robot.weldpara.nCycleNum % robot.weldpara.Nx;
				if((ik>=0) && (ik<=(robot.weldpara.N1-1)))
				{
					//N1阶段	     
					robot.weldpara.wavePos.Px = 0;
					robot.weldpara.wavePos.Py = -1.0*fNextVel;
					robot.weldpara.wavePos.Pz = robot.weldpara.wTech.fAmp/robot.weldpara.N1;;
					robot.weldpara.bStilling = true;
				}
				else if((ik>=robot.weldpara.N1) && (ik<=(robot.weldpara.N1+robot.weldpara.N2-1)))
				{
					//N2阶段
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Px = 0;					
					robot.weldpara.wavePos.Pz = 0;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3-1)))		   
				{
					//N3阶段
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N3;
					robot.weldpara.wavePos.Py = -1.0*fNextVel;
					robot.weldpara.wavePos.Pz = -robot.weldpara.wTech.fAmp/robot.weldpara.N3;;
					robot.weldpara.bStilling = true;
				}

				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4-1)))			   
				{
					//N4阶段
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Px = 0;					
					robot.weldpara.wavePos.Pz = 0;
				}
				else if(ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5-1))) 				   
				{
					//N5阶段
					robot.weldpara.wavePos.Px = -robot.weldpara.wTech.fAmp/robot.weldpara.N5;
					robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else
				{
					//N6阶段
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Px = 0;					
					robot.weldpara.wavePos.Pz = 0;
				}
				robot.weldpara.nCycleNum++;
				pos->Px += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nx + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Ox + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ax;
				pos->Py += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Ny + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oy + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ay;
				pos->Pz += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nz + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oz + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Az;
			}
			else
				robot.weldpara.bStilling = false;

			robot.weldpara.totalPx += robot.weldpara.wavePos.Px;
			//robot.laserTrack.posY = 0;
			if(robot.laserTrack.posY > 5.0)
				robot.laserTrack.posY = 5.0;
			if(robot.laserTrack.posY < -5.0)
				robot.laserTrack.posY = -5.0;
			if(robot.weldpara.nCycleNum % 300 == 1)
				robot.laserTrack.fNextOffset = (robot.laserTrack.posY - robot.weldpara.totalPx)/300;
			if(robot.laserTrack.bLaserTrack)
			{			
				pos->Px += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nx;
				pos->Py += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Ny;
				pos->Pz += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nz;
				robot.weldpara.totalPx += robot.laserTrack.fNextOffset;
			}	

			break;
		}

	case TRIGLR_CHR:
		{
			if(robot.weldpara.nCycleNum<robot.weldpara.N)
			{	   
				int ik=robot.weldpara.nCycleNum % robot.weldpara.Nx;
				if((ik>=0) && (ik<=(robot.weldpara.N1-1)))
				{
					//N1阶段	     
					robot.weldpara.wavePos.Px = 0;
					robot.weldpara.wavePos.Py = -1.0*fNextVel;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = true;
				}
				else if((ik>=robot.weldpara.N1) && (ik<=(robot.weldpara.N1+robot.weldpara.N2-1)))
				{
					//N2阶段
					robot.weldpara.wavePos.Px = 0;
					robot.weldpara.wavePos.Py = -2.0*fNextVel;
					robot.weldpara.wavePos.Pz = robot.weldpara.wTech.fAmp/robot.weldpara.N2;
					robot.weldpara.bStilling = true;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3-1)))		   
				{
					//N3阶段
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N3;
					robot.weldpara.wavePos.Py = -1.0*fNextVel;
					robot.weldpara.wavePos.Pz = -robot.weldpara.wTech.fAmp/robot.weldpara.N3;
					robot.weldpara.bStilling = true;
				}

				else		   
				{
					//N4阶段
					robot.weldpara.wavePos.Px = -robot.weldpara.wTech.fAmp/robot.weldpara.N4;
					robot.weldpara.wavePos.Py = 0;
					robot.weldpara.wavePos.Pz = 0;
					if( ik >= robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3 + robot.weldpara.N2)
						robot.weldpara.bStilling = false;
					else
						robot.weldpara.bStilling = true;
				}

				robot.weldpara.nCycleNum++;
				pos->Px += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nx + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Ox + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ax;
				pos->Py += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Ny + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oy + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ay;
				pos->Pz += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nz + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oz + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Az;
			}
			else
				robot.weldpara.bStilling = false;

			robot.weldpara.totalPx += robot.weldpara.wavePos.Px;
			break;
		}
	case TRIGLR_LEAN:
		{
			if(robot.weldpara.nCycleNum<robot.weldpara.N)
			{	   
				int ik=robot.weldpara.nCycleNum % robot.weldpara.Nx;
				if((ik>=0) && (ik<=(robot.weldpara.N1-1)))
				{
					//N1阶段	     
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N1;
					robot.weldpara.wavePos.Py = robot.weldpara.fVleft - fNextVel;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else if((ik>=robot.weldpara.N1) && (ik<=(robot.weldpara.N1+robot.weldpara.N2-1)))
				{
					//N2阶段
					robot.weldpara.wavePos.Px = 0;
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						//robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Pz = 0;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3-1)))		   
				{
					//N3阶段
					robot.weldpara.wavePos.Px = -robot.weldpara.wTech.fAmp/robot.weldpara.N3;
					robot.weldpara.wavePos.Py = -1.0*fNextVel - robot.weldpara.fVright;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4-1)))
				{
					//N4阶段
					robot.weldpara.wavePos.Px = 0;
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						//robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Pz = 0;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5-1)))		   
				{
					//N5阶段
					robot.weldpara.wavePos.Px = -robot.weldpara.wTech.fAmp/robot.weldpara.N5;
					robot.weldpara.wavePos.Py = -1.0*fNextVel - robot.weldpara.fVright;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6-1)))
				{
					//N6阶段
					robot.weldpara.wavePos.Px = 0;
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						//robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Pz = 0;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6+robot.weldpara.N7-1)))
				{
					//N7阶段	     
					robot.weldpara.wavePos.Px = robot.weldpara.wTech.fAmp/robot.weldpara.N7;
					robot.weldpara.wavePos.Py = robot.weldpara.fVleft - fNextVel;
					robot.weldpara.wavePos.Pz = 0;
					robot.weldpara.bStilling = false;
				}
				else if((ik>=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6+robot.weldpara.N7)) && (ik<=(robot.weldpara.N1+robot.weldpara.N2+robot.weldpara.N3+robot.weldpara.N4+robot.weldpara.N5+robot.weldpara.N6+robot.weldpara.N7+robot.weldpara.N8-1)))
				{
					//N8阶段
					robot.weldpara.wavePos.Px = 0;
					if(robot.weldpara.bStayStill)
					{
						robot.weldpara.wavePos.Py = -1.0*fNextVel;
						//robot.weldpara.bStilling = true;
					}
					else
					{
						robot.weldpara.wavePos.Py = 0;
						robot.weldpara.bStilling = false;
					}
					robot.weldpara.wavePos.Pz = 0;
				} 
				robot.weldpara.nCycleNum++;
				pos->Px += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nx + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Ox + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ax;
				pos->Py += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Ny + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oy + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Ay;
				pos->Pz += robot.weldpara.wavePos.Px * robot.weldpara.waveFrame.Nz + robot.weldpara.wavePos.Py * robot.weldpara.waveFrame.Oz + robot.weldpara.wavePos.Pz * robot.weldpara.waveFrame.Az;
			}
			else
				robot.weldpara.bStilling = false;

			robot.weldpara.totalPx += robot.weldpara.wavePos.Px;
			break;
		}
	case NO_WAVE:
		{
			robot.weldpara.bStilling = false;
			robot.weldpara.nCycleNum++;
			if(robot.laserTrack.bLaserTrack)
			{
				if(robot.laserTrack.posY > 5.0)
					robot.laserTrack.posY = 5.0;
				if(robot.laserTrack.posY < -5.0)
					robot.laserTrack.posY = -5.0;
				if(robot.weldpara.nCycleNum % 300 == 1)//跟踪周期按照前导距离计算
				{
					robot.laserTrack.fNextOffset = robot.laserTrack.posY/300;
					printf("posy: %.2f , off: %.4f\n", robot.laserTrack.posY, robot.laserTrack.fNextOffset);
				}

				pos->Px += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nx;
				pos->Py += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Ny;
				pos->Pz += robot.laserTrack.fNextOffset * robot.weldpara.waveFrame.Nz;
				//robot.weldpara.totalPx += robot.laserTrack.fNextOffset;
			}	

			break;
		}
	default:
		break;
	}
}

void CalcPosFound()
{
	float tempY,tempZ;
	CARTPOS pos,tempPos;
	Pos3d temp3d;
	tempY = robot.laserTrack.laserSearch[robot.laserTrack.nLastSearchIndex].fPy;
	tempZ = robot.laserTrack.laserSearch[robot.laserTrack.nLastSearchIndex].fPz;
	pos = robot.laserTrack.laserSearch[robot.laserTrack.nLastSearchIndex].robotPos;
	temp3d.Px = 0; temp3d.Py = tempY; temp3d.Pz = tempZ;
	switch(robot.laserTrack.eSearchType)
	{
	case SEARCH_STARTPOS:
		{
			tempPos = MultMatVec(robot.toolPos.Rmatrix,temp3d,robot.toolPos);//此处tcp应该已经切换成相机的
			robot.laserTrack.startPosFound = MultMatVec(pos.Rmatrix,tempPos,pos);
			robot.laserTrack.bFoundS = true;
			break;
		}
	case SEARCH_ENDPOS:
		{
			tempPos = MultMatVec(robot.toolPos.Rmatrix,temp3d,robot.toolPos);//此处tcp应该已经切换成相机的
			robot.laserTrack.endPosFound = MultMatVec(pos.Rmatrix,tempPos,pos);
			robot.laserTrack.bFoundE = true;
			break;
		}
	}
}

//激光寻位
void LaserSearch()
{
	//robot.laserTrack.fScanPy[robot.laserTrack.nScanReadIndex] = robot.laserTrack.posY;
	//robot.laserTrack.nScanReadIndex++;
	robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].fPy = robot.laserTrack.posY;
	robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].fPz = robot.laserTrack.posZ;
	robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].robotPos = robot.robotEndPos;
	switch(robot.laserTrack.eSearchType)
	{
	case SEARCH_GAP:
		{
			if(robot.laserTrack.nInspectCycle > 0 && !robot.laserTrack.bMeasValid)
				robot.laserTrack.nInspectCycle =0;
			if(robot.laserTrack.bMeasValid && robot.laserTrack.posY > -5 && robot.laserTrack.posY < 5)
				robot.laserTrack.nInspectCycle++;
			if(robot.laserTrack.nInspectCycle > 100)
			{
				robot.laserTrack.bPosFound = true;
				robot.laserTrack.nInspectCycle = 0;
			}
			break;
		}
	case SEARCH_STARTPOS:
	case SEARCH_ENDPOS:
		{
			if(robot.laserTrack.nSearchWriteIndex == 0)
			{
				robot.laserTrack.nLastSearchIndex = 0;
				robot.laserTrack.fLastSearchPy = robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].fPy;
			}
			else if(fabs((robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].fPy - robot.laserTrack.fLastSearchPy)) < 0.1)
			{
				robot.laserTrack.nLastSearchIndex = robot.laserTrack.nSearchWriteIndex;
				robot.laserTrack.fLastSearchPy = robot.laserTrack.laserSearch[robot.laserTrack.nSearchWriteIndex].fPy;
				robot.laserTrack.nInspectCycle = 0;
			}
			else
			{
				robot.laserTrack.nInspectCycle++;
				if(robot.laserTrack.nInspectCycle > 200)
				{
					robot.laserTrack.bPosFound = true;
					robot.laserTrack.nInspectCycle = 0;
					robot.laserTrack.nSearchWriteIndex = 0;
					CalcPosFound();
				}
			}
			/*if(robot.laserTrack.nInspectCycle > 0 && robot.laserTrack.bMeasValid)  //利用焊缝是否有效来判断
				robot.laserTrack.nInspectCycle =0;
			if(!robot.laserTrack.bMeasValid)
				robot.laserTrack.nInspectCycle++;
			if(robot.laserTrack.nInspectCycle > 100)
			{
				robot.laserTrack.bPosFound = true;
				robot.laserTrack.nInspectCycle = 0;
			}*/
			break;
		}
	default:
		break;
	}
	robot.laserTrack.nSearchWriteIndex++;
	if(robot.laserTrack.nSearchWriteIndex > SCAN_BUFFER-1)
		robot.laserTrack.nSearchWriteIndex = 0;
	
}


/************************************************************************/
/* Power                                                                */
/************************************************************************/
void MC_GROUP_POWER::OnCycle()
{
	MC_T_BOOL               bEnable = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;
	EC_T_WORD               wCurrState = eEcatState_UNKNOWN;
	EC_T_WORD               wReqState;
	MC_T_WORD               wLastStatusWord = 0;            /* Status Word */
	MC_T_WORD               wLastControlWord = 0;           /* Control Word */
	MC_T_INT				nStatus = 0;
#ifdef DEBUG
	EC_T_INT                nTrueCnt = 0;
#endif
	int safe_op_ready = 0;
	int status_ready = 0;
	for (int i=0;i<ROBOT_AXES_NUM;i++)//如果外部轴不分开使能，则此处改成 DEMO_MAX_NUM_OF_AXIS
	{
		/* get slave state */
		if (i < ROBOT_AXES_NUM)
			pwrAxis = robot.Axes.RobotAxis[i];
		else if (i == ROBOT_AXES_NUM)
			pwrAxis = robot.Axes.AdditionalAxis[0];
		else
			pwrAxis = robot.Axes.AdditionalAxis[1];
		
		if (pwrAxis->pEcatGetSlaveState)
		{
			pwrAxis->pEcatGetSlaveState(pwrAxis->dwSlaveID, &wCurrState, &wReqState);
		}

		/* Get input data */
		wLastStatusWord = pwrAxis->wStatusWord;
		wLastControlWord = pwrAxis->wControlWord;
		pwrAxis->wStatusWord = 0;
		pwrAxis->lActualPosition = 0;

		if (wCurrState == eEcatState_SAFEOP || wCurrState == eEcatState_OP)
		{
			if (pwrAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL
				&& pwrAxis->pwPdStatusWord != MC_NULL
				&& pwrAxis->pwPdControlWord != MC_NULL)
			{

				pwrAxis->wStatusWord = EC_GET_FRM_WORD(pwrAxis->pwPdStatusWord);
				pwrAxis->wControlWord = EC_GET_FRM_WORD(pwrAxis->pwPdControlWord);

				if ((pwrAxis->nVerbose > 2) && (wLastStatusWord != pwrAxis->wStatusWord))
				{
					OsDbgMsg("Cycle=%8d: StatusWord 0x%04x -> 0x%04x\n", pwrAxis->dwCycleCount, wLastStatusWord, pwrAxis->wStatusWord);
				}
			}
			safe_op_ready++;
			MC_DriveGetInputs(pwrAxis);
		}
		/* state machine, note 2  */
		if (!pwrAxis->bStatus)
		{
#ifdef EC_MOTION_TRAJECTORY_GEN
			//MC_AbortCommand(robot.Axes.RobotAxis[i], MCFB_ID_POWER);//jari
#endif
			if (!pwrAxis->bError)
			{
				pwrAxis->ePLCOpenState = MC_PLCOPEN_STATE_DISABLED; // Set inital state
			}
		}
		if (wCurrState == eEcatState_OP)   
			DS402StateMachine(pwrAxis);
		else   
			pwrAxis->eDS402State = DRV_DEV_STATE_NOT_READY;

		if (bEnable && !this->bError)
		{
			this->bValid = MC_TRUE;
		}
		else
		{
			this->bValid = MC_FALSE;
		}

		/* state machine, note 5 */
		if (pwrAxis->ePLCOpenState == MC_PLCOPEN_STATE_DISABLED)
		{
			if (bEnable && pwrAxis->bStatus)
			{
				pwrAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
			}
		}

		/* save for future use */
		pwrAxis->bMcPowerStatus = pwrAxis->bStatus;
		pwrAxis->bMcPowerEnable = bEnable;

		/* update control word in process data output image */
		if (pwrAxis->eAxisType == MC_AXIS_TYPE_REAL_ALL)
		{
			if ((pwrAxis->nVerbose > 2) && (wLastControlWord != pwrAxis->wControlWord))
			{
				OsDbgMsg("Cycle=%8d: ControlWord 0x%04x -> 0x%04x\n", pwrAxis->dwCycleCount, wLastControlWord, pwrAxis->wControlWord);
			}
			if (pwrAxis->pwPdControlWord != EC_NULL)
			{
				EC_SET_FRM_WORD(pwrAxis->pwPdControlWord, pwrAxis->wControlWord);
			}
			if (pwrAxis->pbyPdModeOfOperation != EC_NULL)
			{
				*pwrAxis->pbyPdModeOfOperation = (EC_T_BYTE)pwrAxis->wProfileOperationMode;
			}
		}

		pwrAxis->dwCycleCount++;      /* for logging purpose only */

		if ((pwrAxis->nVerbose > 1) && (pwrAxis->eLastPLCOpenState != pwrAxis->ePLCOpenState))
		{
			OsDbgMsg("PLCOpen State '%s' -> '%s'\n", PLCOpenStateText(pwrAxis->eLastPLCOpenState), PLCOpenStateText(pwrAxis->ePLCOpenState));
			pwrAxis->eLastPLCOpenState = pwrAxis->ePLCOpenState;
		}

		if (pwrAxis->bStatus)
		{
			if(nStatus < 6)
			{
				nStatus++;
			}
		}else
		{
			if(nStatus > 0)
			{
				nStatus--;
			}
		}

		if (nStatus == ROBOT_AXES_NUM)
		{
			this->bStatus = MC_TRUE;
		}
		else
			this->bStatus = MC_FALSE;
	}
}

void MC_GROUP_POWER::DS402StateMachine(MC_T_AXIS_REF *pAxis)
{
	MC_T_BOOL               bEnable  = this->bEnable && this->bEnable_Positive && this->bEnable_Negative;
	EC_T_WORD               wStatus = pAxis->wStatusWord;

	switch(pAxis->eDS402State)
	{
	default:
	case DRV_DEV_STATE_NOT_READY:           /* Not ready to switch on */
		if(bEnable)            
		{
			/* check operation mode */
			if (pAxis->IsOpModeValid(pAxis->wProfileOperationMode))
			{
				if(((wStatus & 0x4F) == 0) || ((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS))
				{
					pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
				}
				else if(wStatus & DRV_STAT_FAULT)
				{
					pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
					pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
					pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
				}
				else if((wStatus & DRV_STAT_RDY_SWITCH_ON) == DRV_STAT_RDY_SWITCH_ON)
				{
					pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
				}
			}
			else
			{
				this->bError = MC_TRUE;
				this->wErrorID = MC_ERR_INVALID_OPERATION_MODE;
			}
		}
		else
		{
			/* The 慐rror?outputs are reset with the falling edge of 慐nable?as soon as possible */
			this->bError = MC_FALSE;
		}
		break;

	case DRV_DEV_STATE_SWITCHON_DIS:        /* Optional state: Switch on disabled     */
		if(bEnable)            
		{
			if((wStatus & 0x4F) == DRV_STAT_SWITCH_ON_DIS)
			{
				pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SHUTDOWN);
				pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
			}
			else if((wStatus & 0x4F) == DRV_STAT_RDY_SWITCH_ON)
			{
				pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
			}
			else if((wStatus & 0x69) == (DRV_STAT_QUICK_STOP | DRV_STAT_RDY_SWITCH_ON))
			{
				pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
			}
			else if(wStatus & DRV_STAT_FAULT)
			{
				pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
				pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
				pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
			}
		}
		else
		{
			pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_VOLTAGE);
			/* The 慐rror?outputs are reset with the falling edge of 慐nable?as soon as possible */
			this->bError = MC_FALSE;
			this->bStatus = MC_FALSE;
		}
		break;

	case DRV_DEV_STATE_READY_TO_SWITCHON:   /* Ready to switch on    */
		if(bEnable)            
		{
			if((wStatus & 0x69) == (DRV_STAT_QUICK_STOP | DRV_STAT_RDY_SWITCH_ON))
			{
				pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SWITCHON);
				pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
			}
		}
		else
		{
			pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_VOLTAGE);
			pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
		}
		if(pAxis->wProfileOperationMode == DRV_MODE_OP_INTER_POS)
		{
			pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CTRL_INTER_POS_ENA);       /* disable interpolation */
		}
		break;

	case DRV_DEV_STATE_SWITCHED_ON:         /* Switched on */
		pAxis->bStatus    = MC_FALSE;
		if(bEnable)
		{
			if((wStatus & 0x6B) == (DRV_STAT_QUICK_STOP | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
			{
				pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_ENA_OPERATION);

				MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);
				MC_BufferClear(pAxis);          /* initialize FB buffer queue */

				pAxis->eDS402State = DRV_DEV_STATE_OP_ENABLED;
			}
		}
		else
		{
			pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_SHUTDOWN);
			pAxis->eDS402State = DRV_DEV_STATE_READY_TO_SWITCHON;
		}
		break;

	case DRV_DEV_STATE_OP_ENABLED:          /* Operation enabled  */
		if(bEnable)
		{
			if((wStatus & 0x7F) == (DRV_STAT_QUICK_STOP | DRV_STAT_VOLTAGE_ENABLED | DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
			{
				/* rising edge ? */
				if(!pAxis->bStatus)
				{
					/* set initial axis state */
					//pAxis->ePLCOpenState = MC_PLCOPEN_STATE_STAND_STILL;
					MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
					pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;
				}

				/* set all flags to enabled state */
				if((wStatus & 0x1000) == 0x1000)
				{
					pAxis->bStatus         = MC_TRUE;
		//			printf("wStatus: %x\n",wStatus);
				}

				this->bError          = MC_FALSE;
				this->wErrorID        = MC_NO_ERROR;
				if(pAxis->wProfileOperationMode == DRV_MODE_OP_INTER_POS)
				{
					pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CTRL_INTER_POS_ENA);       /* enable interpolation */
				}

				/* check for following error */
				if (pAxis->wStatusWord & DRV_STAT_FOLLOW_ERR)
				{
					this->wErrorID = MC_ERR_FOLLOWING;
					this->bError   = MC_TRUE;
					pAxis->bStatus  = MC_FALSE;
				}
			}
			else if(pAxis->bStatus)
			{
				if ((pAxis->ePLCOpenState == MC_PLCOPEN_STATE_STOPPING) && ((pAxis->wControlWord & DRV_CTRL_CMD_QUICK_STOP) == 0))
				{
					pAxis->eDS402State = DRV_DEV_STATE_QUICK_STOP;
				}
				else
				{
					/* unexpected change in status word */
					pAxis->bStatus = MC_FALSE;
					this->bError = MC_TRUE;
					this->wErrorID = MC_ERR_INVALID_STATUS_WORD;

					/* axis fault ? */
					if (wStatus & DRV_STAT_FAULT)
					{
						pAxis->eDS402State = DRV_DEV_STATE_MALFCT_REACTION;
						pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;     /* state machine, note 1 */
						pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
					}

					/* quick stop in progress ? */
					else if ((wStatus & DRV_STAT_QUICK_STOP) == 0)
					{
						pAxis->eDS402State = DRV_DEV_STATE_QUICK_STOP;
						pAxis->ePLCOpenState = MC_PLCOPEN_STATE_ERROR_STOP;
						pAxis->SetAxisError(MC_ERR_AXIS_FAULT);
					}
				}
			}
		}
		else
		{
			pAxis->wControlWord = (EC_T_WORD)((pAxis->wControlWord & ~DRV_CTRL_CMD_MASK) | DRV_CTRL_CMD_DIS_OPERATION);
			pAxis->eDS402State = DRV_DEV_STATE_SWITCHED_ON;
		}
		break;

	case DRV_DEV_STATE_QUICK_STOP:          /* Optional state: Quick stop active  */
		if((wStatus & 0x6F) == (DRV_STAT_OP_ENABLED | DRV_STAT_SWITCHED_ON | DRV_STAT_RDY_SWITCH_ON))
		{
			/* stay in this state */
		}
		else
		{
			pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
		}
		break;

	case DRV_DEV_STATE_MALFCT_REACTION:     /* Malfunction reaction active */

		pAxis->oMove.lNextMoveCorrection = 0;
		MC_InitMovement(&pAxis->oMove, MC_FALSE, 0);
		pAxis->oMove.lCommandVel = pAxis->oMove.lCommandAcc = 0;
		MC_DriveSetTargetPosition(pAxis, pAxis->lActualPosition);

		pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord & ~DRV_CRTL_FAULT_RESET);
		pAxis->eDS402State = DRV_DEV_STATE_MALFUNCTION;
		break;

	case DRV_DEV_STATE_MALFUNCTION:         /* Malfunction                 */
		if(bEnable)
		{
			pAxis->wControlWord = (EC_T_WORD)(pAxis->wControlWord | DRV_CRTL_FAULT_RESET);
			pAxis->eDS402State = DRV_DEV_STATE_SWITCHON_DIS;
			this->bError          = MC_FALSE;
		}
		break;
	}
}


/************************************************************************/
/* PTP     S形                                                          */
/************************************************************************/
#ifndef ptpT
void MC_MoveDirectAbsolute::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_MoveDirectAbsolute::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

	switch (this->fbState)
	{
	case FBS_IDLE:
		{
			if (this->bExecute)
			{
				StateTrans_Execute();
				if (this->fbState == FBS_ERROR)
				{
					this->bError = MC_TRUE;
					continueSm = MC_TRUE;
				}
				else if (this->fbState == FBS_BUSY)
				{
					this->bBusy = MC_TRUE;
					continueSm = MC_TRUE;
				}
			}
			break;
		}
	case FBS_BUSY:
		{
			DoState_Busy();
			if (this->fbState == FBS_ACTIVE)
			{
				StateTrans_Active();
				this->bActive = MC_TRUE;
				continueSm = MC_TRUE;
			}

			if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_ACTIVE:
		{
			DoState_Active();
			if (this->fbState == FBS_DONE)
			{
				this->bDone = MC_TRUE;
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				continueSm = MC_TRUE;
			}
			else if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
	case FBS_ERROR:
	case FBS_DONE:
		{
			this->bDone = MC_FALSE;           
			this->bCommandAborted = MC_FALSE;
			this->bError = MC_FALSE;
			this->wErrorID = MC_NO_ERROR;
			//  this->fbState = FBS_IDLE;
			break;
		}
	}

	return continueSm;
}

void MC_MoveDirectAbsolute::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */

	if(fMaxVel > robot.RobotInfo.MaxSpeed)
		fMaxVel = robot.RobotInfo.MaxSpeed;
	if(fMaxAcc > robot.RobotInfo.MaxSpeedAcc)
		fMaxAcc = robot.RobotInfo.MaxSpeedAcc;
	if(fMaxVel <= 0)
		fMaxVel = 1.0;
	if(fMaxAcc <= 0)
		fMaxAcc = 1.0;
	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
	//printf("StateTrans_Execute \n");
	this->fbState = FBS_BUSY;
}

void MC_MoveDirectAbsolute::DoState_Busy()
{
	//robot.ePLCOpenState = robot.ePLCOpenState;
	switch (robot.ePLCOpenState)
	{
	case GROUP_PLCOPEN_STATE_STANDBY:
		// 	   //test
		//    case GROUP_PLCOPEN_STATE_MOVING:
		// 	   //test
		{
			MC_T_BOOL  fbActive = false;
			/*省略buffer相关*/
			if (this->wErrorID == MC_NO_ERROR)
			{
				robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
				this->fbState = FBS_ACTIVE;
			}
			else
			{
				this->fbState = FBS_ERROR;
				//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			}
			break;
		}
	case GROUP_PLCOPEN_STATE_STOPPING:
		{
			this->wErrorID = MC_ERR_BUSY_STOPPING;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			break;
		}
	case GROUP_PLCOPEN_STATE_PAUSE:
		printf("pause:\n");
		break;
	default:
		{
			this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;

		}
	}
	printf("DoState_Busy \n");
}

void MC_MoveDirectAbsolute::StateTrans_Active()
{
	printf("StateTrans_Active \n");
	MC_GROUP_MOVEMENT* pMove = &robot.Movement;
	this->fMaxDistance = 0;

	robot.dwCalcIncPerMM = robot.RobotInfo.cnt_per_cycle;

	//robot.fVelToInc = (MC_T_REAL)(robot.dwCycleTime / 1E6);//可以放到power里初始化，此处dwCalcIncPerMM应该是每圈编码器增量
	//robot.fAccToInc = (MC_T_REAL)(robot.fVelToInc  * robot.dwCycleTime / 1E6);

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;//用实际位置还是指定的起始位置？PTP同 revise_l
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
		lstartEnc[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}

	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	//startPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);

	jntsStart.fJnts[0] = pRefJoint->Theta1*180/PI;
	jntsStart.fJnts[1] = pRefJoint->Theta2*180/PI + 90;
	jntsStart.fJnts[2] = pRefJoint->Theta3*180/PI;
	jntsStart.fJnts[3] = pRefJoint->Theta4*180/PI;
	jntsStart.fJnts[4] = pRefJoint->Theta5*180/PI;
	jntsStart.fJnts[5] = pRefJoint->Theta6*180/PI;

	if (robot.bSewPlane && robot.bufferBlock[robot.wExecute].type == LIN)
	{
		pRefJoint->Theta1 = jntsTarget.fJnts[0]*PI/180;
		pRefJoint->Theta2 = jntsTarget.fJnts[1]*PI/180 - PI/2;
		pRefJoint->Theta3 = jntsTarget.fJnts[2]*PI/180;
		pRefJoint->Theta4 = jntsTarget.fJnts[3]*PI/180;
		pRefJoint->Theta5 = jntsTarget.fJnts[4]*PI/180;
		pRefJoint->Theta6 = jntsTarget.fJnts[5]*PI/180;

		endPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);
		RMATRIX tempRM;
		float ox,oy,oz,norm;
		ox = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0] - endPos.Px;
		oy = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[1] - endPos.Py;
		oz = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[2] - endPos.Pz;
		norm = sqrt(ox*ox + oy*oy + oz*oz);
		ox = ox/norm;
		oy = oy/norm;
		oz = oz/norm;
		tempRM.Ox = ox;
		tempRM.Oy = oy;
		tempRM.Oz = oz;
		tempRM.Ax = 0;
		tempRM.Ay = 0;
		tempRM.Az = 1;
		tempRM.Nx = tempRM.Oy*tempRM.Az - tempRM.Oz*tempRM.Ay;
		tempRM.Ny = tempRM.Oz*tempRM.Ax - tempRM.Ox*tempRM.Az;
		tempRM.Nz = tempRM.Ox*tempRM.Ay - tempRM.Oy*tempRM.Ax;
		tempRM.Ox = tempRM.Ay*tempRM.Nz - tempRM.Az*tempRM.Ny;
		tempRM.Oy = tempRM.Az*tempRM.Nx - tempRM.Ax*tempRM.Nz;
		tempRM.Oz = tempRM.Ax*tempRM.Ny - tempRM.Ay*tempRM.Nx;
		endPos.Rmatrix = tempRM;
		MatToRpy(&tempRM,&endPos.Rpy);

	}

	Axis2Drive(&jntsTarget, &jntsStart);

	for (int i=0;i<ROBOT_AXES_NUM;i++)
	{
		fDistance[i] = jntsTarget.fJnts[i] - jntsStart.fJnts[i];
		fMaxDistance=EC_MAX(fMaxDistance,fabs(fDistance[i]));
	}

	fConstantVel = fMaxVel;
	pMove->fAxisCurVel = 0;
	pMove->fAxisCurAcc = 0;

	nCycleCv = 0;
	acc = 0;
	vel = 0;
	dJnt = 0;

	/*double sf;

	sf = 2*fMaxVel*fMaxVel/fMaxAcc;
	if (fMaxDistance > sf)
	{
		nCycleCv = (fMaxDistance-sf)/fMaxVel/robot.dwCycleTime*1E6;
	}
	else
	{
		nCycleCv = 0;
		fMaxVel = pow(fMaxAcc*fMaxDistance/2,0.5);
	}
	

	J = fMaxAcc*fMaxAcc/fMaxVel;
	J = J*robot.dwCycleTime*robot.dwCycleTime*robot.dwCycleTime/1E6/1E6/1E6;
	fMaxAcc=  fMaxAcc*robot.dwCycleTime*robot.dwCycleTime/1E6/1E6;
	fMaxVel = fMaxVel*robot.dwCycleTime/1E6;*/

	double sf,tr;//两个斜坡段的总最大长度，单个斜坡段的运动时间
	int it,it2,n;
	sf = 2*fMaxVel*fMaxVel/fMaxAcc;
	nCycleCv = (fMaxDistance-sf)/fMaxVel/robot.dwCycleTime*1E6 + 0.5;//四舍五入
	printf("nCycleCv:%d\n",nCycleCv);
	if (nCycleCv > 0)
	{
		tr = 2*fMaxVel/fMaxAcc;
		it = tr/robot.dwCycleTime*1E6;
		it2 = it/2;
		it = it2*2;//单个斜坡运动周期取通信周期的偶数倍
		
		tr = it*robot.dwCycleTime/1E6;
		J = 2*fMaxAcc/tr*robot.dwCycleTime*robot.dwCycleTime*robot.dwCycleTime/1E6/1E6/1E6;
		n = it2;//半个斜坡段的加速周期数
		//sf = 2*J*n*n*(n+1);
		sf = J*(2*n*n*(n+1) - (n*n - n*(n+1)/2));
		//fMaxAcc = n*J;
		//fMaxVel = n*n*J;

		fConstantVel = (fMaxDistance-sf)/nCycleCv;//匀速段的速度 mm/cycle
	}
	else
	{
		fMaxVel = pow(fMaxAcc*fMaxDistance/2,0.5);
		tr = 2*fMaxVel/fMaxAcc;
		n = tr/2.0/robot.dwCycleTime*1E6;

		J = fMaxDistance/(2*n*n*(n+1) - (n*n - n*(n+1)/2));
		//fMaxAcc = n*J;
		//fMaxVel = n*n*J;
		fConstantVel = n*n*J;
		nCycleCv = 0;
	} 
	this->nHalfRamp = n;

	fRemainDist = 0;
	currVelState = aacc;
	pMove->fAxisMaxAcc = n*J;


}

void MC_MoveDirectAbsolute::DoState_Active()
{
	//printf("DoState_Active \n");
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;

	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_STOPPING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_PAUSE) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}


	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING || robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)
	{
		pMove->fAxisTargetVel = 0;
		if (pMove->fAxisCurVel - pMove->fAxisMaxAcc >= pMove->fAxisTargetVel)//减速
		{
			vel = pMove->fAxisCurVel - pMove->fAxisMaxAcc;
		}
		else //最后一个周期
		{
			vel = pMove->fAxisTargetVel;
			bDone = true;
		}
		dJnt += vel;
	}
	else
	{
		if (currVelState == aacc)
		{
			vel += acc;
			dJnt += vel;
			acc += J;
			if (acc >= pMove->fAxisMaxAcc)
			{
				acc = pMove->fAxisMaxAcc;
				currVelState = dacc;
			}
		}
		else if (currVelState == dacc)
		{				
			vel += acc;
			dJnt += vel;
			acc -= J;
			if (acc <= 0)
			{
				acc = 0;
				currVelState = topv;
			}
		}
		else if (currVelState == topv)
		{
			dJnt += vel;
			currVelState = cv;
		}
		else if (currVelState == cv)
		{
			nCycleCv--;
			vel = fConstantVel;//new 20180629
			dJnt += vel;
			if (nCycleCv <= 0)
			{
				currVelState = adcc;
				vel = nHalfRamp*nHalfRamp*J;
			}
		}
		else if (currVelState == adcc)
		{
			//printf("adcc\n");
			vel -= acc;
			dJnt += vel;
			acc += J;
			if (acc >= pMove->fAxisMaxAcc)
			{
				acc = pMove->fAxisMaxAcc;
				currVelState = ddcc;
			}

		}
		else if (currVelState == ddcc)
		{
			//if(!bChangeJerk)
			//{
			//	J = acc*sqrt(acc/(6*fRemainDist+acc));
			//	bChangeJerk = true;
			//}
			vel -= acc;
			dJnt += vel;
			acc -= J;
			if (acc <= 0)
				acc = 0;
			else
			{
				if (vel < 0)
				{
					vel = 0;
				}

				////
				//if(fRemainDist <= vel)
				//{
				//	vel = fRemainDist;
				//}
				////
			}
		}
		fRemainDist = fMaxDistance - dJnt;
		if ((currVelState == ddcc && acc <= 0.0000001) /*|| fRemainDist <= 0*/)//阈值设定
		{
			bDone = true;
		}
	}
	pMove->fAxisCurVel = vel;
	//printf("vel: %.6f,  remaindist: %.6f\n",vel*500,fRemainDist);
	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{ 
		if(robot.RobotInfo.motorDir[i] == 1)
		{
			robot.Axes.RobotAxis[i]->lCommandedPosition = lstartEnc[i] + (MC_T_VELACC)(dJnt*fDistance[i]/fMaxDistance/360*robot.RobotInfo.re_radio[i]*robot.dwCalcIncPerMM);
		}
		else
		{
			robot.Axes.RobotAxis[i]->lCommandedPosition = lstartEnc[i] - (MC_T_VELACC)(dJnt*fDistance[i]/fMaxDistance/360*robot.RobotInfo.re_radio[i]*robot.dwCalcIncPerMM);
		}

		EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);//revise_l
	}

	//fprintf(robot.robotFile,"%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", robot.actJnt.Theta1*180/PI+dJnt*fDistance[0]/fMaxDistance, robot.actJnt.Theta2*180/PI+90+dJnt*fDistance[1]/fMaxDistance, robot.actJnt.Theta3*180/PI+dJnt*fDistance[2]/fMaxDistance, robot.actJnt.Theta4*180/PI+dJnt*fDistance[3]/fMaxDistance, robot.actJnt.Theta5*180/PI+dJnt*fDistance[4]/fMaxDistance, robot.actJnt.Theta6*180/PI+dJnt*fDistance[5]/fMaxDistance,acc,vel,fRemainDist);


	if (bDone && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING)
	{
		this->fbState = FBS_DONE;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  单个轴的plcopen状态？
		this->Execute = MC_FALSE;
	}
	else if (bDone && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)//暂停
	{
		this->fbState = FBS_BUSY;
		bDone = false;
		robot.bPause = true;
		printf("pause!\n");
	}
	else if (bDone)
	{
		robot.eLastFb = PTP;
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  单个轴的plcopen状态？
		this->Execute = MC_FALSE;
		bChangeJerk = false;
		printf("PTP DONE! \n");
		printf("actjnt: %.6f,  %.6f,  %.6f,  %.6f,  %.6f,  %.6f\n",robot.actJnt.Theta1*180/PI,(robot.actJnt.Theta2*180/PI+90),robot.actJnt.Theta3*180/PI,robot.actJnt.Theta4*180/PI,robot.actJnt.Theta5*180/PI,robot.actJnt.Theta6*180/PI);
	}
}
#endif

/************************************************************************/
/* Lin       预读                                                       */
/************************************************************************/
#ifdef PRE_IPR
void MC_MoveLinearAbsolute::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_MoveLinearAbsolute::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
	   {
		   this->bDone = MC_FALSE;          
		   this->bCommandAborted = MC_FALSE;
		   this->bError = MC_FALSE;
		   this->wErrorID = MC_NO_ERROR;
		//   this->fbState = FBS_IDLE;;
		   break;
      }
   } 
   
   return continueSm;
}

void MC_MoveLinearAbsolute::StateTrans_Execute()
{
	if(fMaxVel > robot.RobotInfo.MaxLineSpeed)
		fMaxVel = robot.RobotInfo.MaxLineSpeed;
	if(fMaxAcc > robot.RobotInfo.MaxLineAcc)
		fMaxAcc = robot.RobotInfo.MaxLineAcc;
	if(fMaxVel <= 0)
		fMaxVel = 1.0;
	if(fMaxAcc <= 0)
		fMaxAcc = 1.0;
	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		return;
	}

	this->fbState = FBS_BUSY;
}

void MC_MoveLinearAbsolute::DoState_Busy()
{
   switch (robot.ePLCOpenState)
   {
   case GROUP_PLCOPEN_STATE_STANDBY:
// 	   //test
//    case GROUP_PLCOPEN_STATE_MOVING:
// 	   //test
      {
         MC_T_BOOL  fbActive = false;

		 if (this->wErrorID == MC_NO_ERROR)
		 {
			 robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
			 this->fbState = FBS_ACTIVE;
		 }
         else
         {
            this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
         }
         break;
      }
   case GROUP_PLCOPEN_STATE_STOPPING:
      {
         this->fbState = FBS_DONE;
		 printf("stopping: fbs_done\n");
		// robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
         break;
      }
   case GROUP_PLCOPEN_STATE_PAUSE:
	   printf("pause:\n");
	   break;
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->fbState = FBS_ERROR;
		 //robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
      }
   }
}

void MC_MoveLinearAbsolute::StateTrans_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;
	MC_T_POS_CALC   lMaxDistance = 0;
	MC_T_REAL       fMaxDistance = 0;

	pMove->bContUpdate = this->bContUpdate;

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;//用实际位置还是指定的起始位置？PTP同 revise_l
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}

	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	startPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);

    endPos.Px = posTarget.fpos[0];
	endPos.Py = posTarget.fpos[1];
	endPos.Pz = posTarget.fpos[2];
	endPos.Rpy.Pitch = posTarget.fpos[3];
	endPos.Rpy.Roll = posTarget.fpos[4];
	endPos.Rpy.Yaw = posTarget.fpos[5];

	if(this->bUseStartPosFound && robot.laserTrack.bFoundS)
	{
		endPos.Px = robot.laserTrack.startPosFound.Px;
		endPos.Py = robot.laserTrack.startPosFound.Py;
		endPos.Pz = robot.laserTrack.startPosFound.Pz;
	}

	if(this->bUseEndPosFound && robot.laserTrack.bFoundE)
	{
		endPos.Px = robot.laserTrack.endPosFound.Px;
		endPos.Py = robot.laserTrack.endPosFound.Py;
		endPos.Pz = robot.laserTrack.endPosFound.Pz;
	}

	CARTPOS tempPos;
	float tempTotalDist;
	if (robot.eLastFb == TRANS)
	{
		//pRefJoint->Theta1 = robot.Trans.pRefJoint->Theta1;//just for simu
		//pRefJoint->Theta2 = robot.Trans.pRefJoint->Theta2;//just for simu
		//pRefJoint->Theta3 = robot.Trans.pRefJoint->Theta3;//just for simu
		//pRefJoint->Theta4 = robot.Trans.pRefJoint->Theta4;//just for simu
		//pRefJoint->Theta5 = robot.Trans.pRefJoint->Theta5;//just for simu
		//pRefJoint->Theta6 = robot.Trans.pRefJoint->Theta6;//just for simu
		startPos = robot.pdPos;
		tempPos = robot.Trans.pos2;
		tempTotalDist = sqrt(pow((endPos.Px-tempPos.Px),2)+pow((endPos.Py-tempPos.Py),2)+pow((endPos.Pz-tempPos.Pz),2));
	}

	if((endPos.Rpy.Pitch - startPos.Rpy.Pitch) > PI)
		endPos.Rpy.Pitch -= 2*PI;
	else if ((endPos.Rpy.Pitch - startPos.Rpy.Pitch) <-PI)
		endPos.Rpy.Pitch += 2*PI;
	if((endPos.Rpy.Roll - startPos.Rpy.Roll) > PI)
		endPos.Rpy.Roll -= 2*PI;
	else if ((endPos.Rpy.Roll - startPos.Rpy.Roll) <-PI)
		endPos.Rpy.Roll += 2*PI;
	if((endPos.Rpy.Yaw - startPos.Rpy.Yaw) > PI)
		endPos.Rpy.Yaw -= 2*PI;
	else if ((endPos.Rpy.Yaw - startPos.Rpy.Yaw) <-PI)
		endPos.Rpy.Yaw += 2*PI;
// 	if (robot.bSetSewPara)
// 	{
// 		this->fMaxVel = robot.sewPara.fRobSpeed;
// 	}

	if (robot.bSewPlane)
	{
		RMATRIX tempRM;
		float ox,oy,oz,norm;
		ox = endPos.Px - startPos.Px;
		oy = endPos.Py - startPos.Py;
		oz = endPos.Pz - startPos.Pz;
		norm = sqrt(ox*ox + oy*oy + oz*oz);
		ox = ox/norm;
		oy = oy/norm;
		oz = oz/norm;
		tempRM.Ox = ox;
		tempRM.Oy = oy;
		tempRM.Oz = oz;
		tempRM.Ax = 0;
		tempRM.Ay = 0;
		tempRM.Az = 1;
		tempRM.Nx = tempRM.Oy*tempRM.Az - tempRM.Oz*tempRM.Ay;
		tempRM.Ny = tempRM.Oz*tempRM.Ax - tempRM.Ox*tempRM.Az;
		tempRM.Nz = tempRM.Ox*tempRM.Ay - tempRM.Oy*tempRM.Ax;
		tempRM.Ox = tempRM.Ay*tempRM.Nz - tempRM.Az*tempRM.Ny;
		tempRM.Oy = tempRM.Az*tempRM.Nx - tempRM.Ax*tempRM.Nz;
		tempRM.Oz = tempRM.Ax*tempRM.Ny - tempRM.Ay*tempRM.Nx;
		endPos.Rmatrix = tempRM;
		MatToRpy(&tempRM,&endPos.Rpy);
	}

	//start点应该是当前点?
	this->fTotalDist = sqrt(pow((endPos.Px-startPos.Px),2)+pow((endPos.Py-startPos.Py),2)+pow((endPos.Pz-startPos.Pz),2));
	if (robot.eLastFb != TRANS)
	{
		tempTotalDist = fTotalDist;
	}
	if (!robot.bTrans)
	{
		pMove->fCartEndVel = 0;
	}
	if (robot.eLastFb != TRANS)
		pMove->fCartCurVel = 0;

	//20180313新增，用速度差判断最大速度
	//pMove->fCurVel = pMove->fCartCurVel*1E6/(float)robot.dwCycleTime;
	//if(pMove->fCurVel >= this->fMaxVel)
	//{
	//	pMove->fMinDeltaVel = pMove->fCurVel - this->fMaxVel;
	//	if(sqrt(this->fTotalDist*this->fMaxAcc) >= pMove->fMinDeltaVel)
	//		this->fConstantVel = this->fMaxVel;
	//	else
	//		this->fConstantVel = pMove->fCurVel + sqrt(this->fTotalDist*this->fMaxAcc);
	//	this->fConstantVel = EC_MIN(pMove->fMinDeltaVel,sqrt(this->fTotalDist*this->fMaxAcc)) + ;
	//}
	//if(pMove->fCurVel < this->fMaxVel)
	//{
	//	pMove->fMinDeltaVel = this->fMaxVel - pMove->fCurVel;
	//}
	//this->fConstantVel = EC_MIN(pMove->fMinDeltaVel,sqrt(this->fTotalDist*this->fMaxAcc));
	//
	this->fConstantVel = EC_MIN(this->fMaxVel,sqrt(this->fTotalDist*this->fMaxAcc));
	float totalTime = fTotalDist/fConstantVel;
/***********/
	//robot.weldpara.bStayStill = true;
	robot.weldpara.totalPx = 0;
	if(robot.bWave || robot.laserTrack.bLaserTrack)
	{
		CalcNewVel(&fConstantVel,fTotalDist,totalTime);
	}
/***********/
	pMove->fCartConstantVel =  (this->fConstantVel* robot.dwCycleTime / 1E6);
	pMove->fCartMaxAcc =  (this->fMaxAcc* robot.dwCycleTime / 1E6* robot.dwCycleTime / 1E6);
	MC_T_REAL tRamp = this->fConstantVel/this->fMaxAcc;
	this->fDist3to4 = this->fConstantVel*tRamp/2;
	pMove->fCartDist3to4 = this->fDist3to4;
	pMove->fOrgMaxVelocity = this->fMaxVel;
	pMove->fOrgAcceleration = this->fMaxAcc;
	pMove->fCartTotalDist = this->fTotalDist;
	pMove->fCartRemainDist = pMove->fCartTotalDist;	

	if (robot.bTrans)
	{
		float lNext;
		int ovlRelMax;  
		lNext = sqrt(pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-endPos.Px),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[1]-endPos.Py),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[2]-endPos.Pz),2));
		ovlRelMax = sqrt(fMaxAcc*lNext)/fConstantVel*100;//过渡段的长度不能超过下一段lin总长度的一半

		ovlRel = ovlRel > ovlRelMax ? ovlRelMax:ovlRel;
		pMove->fCartEndVel = pMove->fCartConstantVel*(float)ovlRel/100.0;
#ifdef LONG_CIRC
		fTransDist = EC_MIN(tempTotalDist/2,lNext/2);//just for test
#endif
		//fTransDist = fConstantVel*fConstantVel*(float)ovlRel*(float)ovlRel/fMaxAcc/2.0;
	}
/* 指定半径
	float l1,l2,l3,l4,delta,tempR;
	l1 = this->fTotalDist;
	l2 = sqrt(pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-endPos.Px),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[1]-endPos.Py),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[2]-endPos.Pz),2));
	l3 = sqrt(pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-startPos.Px),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-startPos.Py),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-startPos.Pz),2));
	delta = acosf((l1*l1 + l2*l2 - l3*l3)/2*l1*l2);
	l4 = (l1>l2) ? l2:l1;
	tempR = l4/2.0*tan(delta/2.0);
	fRtrans = (fRtrans > tempR) ? tempR:fRtrans;
	fTransDist = fRtrans / tan(delta/2.0);
*/
	this->pdPos = startPos;
	robot.Trans.pos1 = startPos;
	robot.Trans.pos2 = endPos;

// 	int i;
// 	i = ((robot.wExecute+1) == PRE_IPR_NUM ? 0:(robot.wExecute+1));
// 	if (robot.bTrans && robot.bufferBlock[i].type == LIN)
// 		pMove->fCartEndVel = robot.bufferBlock[i].Lin.fMaxVel;
// 	if (robot.bTrans && robot.bufferBlock[i].type == CIRC)
// 		pMove->fCartEndVel = robot.bufferBlock[i].Circ.fMaxVel;

	//rpy变化参数赋值
#ifdef RPY_ACC
	if (robot.eLastFb != TRANS)
	{
		if (robot.bTrans)
		{
			pMove->fRpyMaxDiff = 0;
			pMove->fPitchDiff = (endPos.Rpy.Pitch - startPos.Rpy.Pitch) * (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100) / pMove->fCartTotalDist;
			pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fPitchDiff ));
			pMove->fRollDiff  = (endPos.Rpy.Roll - startPos.Rpy.Roll) * (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100) / pMove->fCartTotalDist;
			pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fRollDiff));
			pMove->fYawDiff = (endPos.Rpy.Yaw - startPos.Rpy.Yaw) * (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100) / pMove->fCartTotalDist;
			pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fYawDiff));

			pMove->fRpyAcc = 4 * pMove->fRpyMaxDiff * fConstantVel * fConstantVel / (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100) / (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100);//4*theta*v^2/l^2  假定没有匀速段，只有加速和减速
		    pMove->fRpyMaxVelCycle = 2*pMove->fRpyMaxDiff * fConstantVel / (pMove->fCartTotalDist - pMove->fCartDist3to4*ovlRel/100) * robot.dwCycleTime/1E6;
			pMove->fRpyAccCycle = pMove->fRpyAcc * robot.dwCycleTime*robot.dwCycleTime/1E6/1E6;
			pMove->fRpyCurVel = 0;
			pMove->bRpyRamp = false;
		}
	}
#endif

	if(robot.bWave || robot.laserTrack.bLaserTrack)
		CalcWeldPara(totalTime, fTotalDist, startPos , endPos);
	if(this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
	printf("done: %d\n",bDone);
}

void MC_MoveLinearAbsolute::DoState_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;

	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_STOPPING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_PAUSE) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING || (robot.laserTrack.eSearchType != SEARCH_NOTHING && robot.laserTrack.bPosFound) || robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)  //stop功能嵌入各个功能块    stop完成，参数输出如何做？
	{
		pMove->fCartTargetVel = 0;
		if (pMove->fCartCurVel - pMove->fCartMaxAcc >= pMove->fCartTargetVel)//减速
		{
			pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
		}
		else //最后一个周期
		{
			pMove->fCartNextVel = pMove->fCartTargetVel;
		}
	} 
	else
	{	//检查更新
		if(pMove->bContUpdate)
		{
			if (pMove->fCartTotalDist != this->fTotalDist)//距离改变
			{
				if (pMove->fCartRemainDist <= pMove->fCartDist3to4)
				{
					this->wErrorID = INVALID_CHANGE;//有必要报错吗？还是继续跑？
					this->fbState = FBS_ERROR;
					//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
					return;
				}
				else
				{
					pMove->fCartRemainDist += this->fTotalDist - pMove->fCartTotalDist;
					pMove->fCartTotalDist = this->fTotalDist;
				}			
			}
			if ((pMove->fOrgAcceleration != this->fMaxAcc) || (pMove->fOrgMaxVelocity != this->fMaxVel))
			{
				this->fConstantVel = EC_MIN(this->fMaxVel,sqrt(this->fTotalDist*this->fMaxAcc));//先计算新的斜坡长度
				MC_T_REAL tRamp = this->fConstantVel/this->fMaxAcc;
				this->fDist3to4 = this->fConstantVel*tRamp/2;
				if (pMove->fCartRemainDist <= this->fDist3to4)
				{
					this->wErrorID = INVALID_CHANGE;//有必要报错吗？还是继续跑？
					this->fbState = FBS_ERROR;
					//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
					return;
				}
				else
				{
					pMove->fCartConstantVel =  (this->fConstantVel* robot.dwCycleTime / 1E6);
					pMove->fCartMaxAcc =  (this->fMaxAcc* robot.dwCycleTime / 1E6* robot.dwCycleTime / 1E6);
					pMove->fOrgAcceleration = this->fMaxAcc;
					pMove->fOrgMaxVelocity = this->fMaxVel;
				}		
			}
		}
		//轨迹插补
		if (pMove->fCartRemainDist >= pMove->fCartDist3to4 + pMove->fCartConstantVel)
		{
			pMove->fCartTargetVel = pMove->fCartConstantVel;
			if (pMove->fCartCurVel + pMove->fCartMaxAcc < pMove->fCartTargetVel)
			{
				pMove->fCartNextVel = pMove->fCartCurVel  + pMove->fCartMaxAcc;
			}
			else 
			{
				pMove->fCartNextVel = pMove->fCartTargetVel;
				robot.bConstantVel = true;
			}
		}
		else
		{
			//robot.bConstantVel = false;
			if(!bChangeAcc)
			{
				pMove->fCartMaxAcc = pMove->fCartCurVel*pMove->fCartCurVel/(2*pMove->fCartRemainDist + pMove->fCartCurVel);
				bChangeAcc = true;
			}
			pMove->fCartTargetVel = pMove->fCartEndVel;
			if (!robot.bTrans)
			{
				//if ((pMove->fCartRemainDist < pMove->fCartCurVel)||((pMove->fCartRemainDist >= pMove->fCartCurVel)&&(pMove->fCartCurVel < pMove->fCartMaxAcc)))
				//{
				//	pMove->fCartNextVel = 0;
				//}
				if ((pMove->fCartRemainDist <= 0 )||(pMove->fCartCurVel <= pMove->fCartMaxAcc))
				{
					pMove->fCartNextVel = 0;
				}
				else
				{
					pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
				}
			}
#ifndef LONG_CIRC
			else  //just for test
			{
				if (pMove->fCartCurVel > (pMove->fCartTargetVel + 0.001))
				{
					pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
				}
				else
				{
					bDone = MC_TRUE;
				}
			}
#endif

		}
	}

	//MC_T_BOOL bDone = MC_FALSE;
	refPos = pdPos;
	pdPos.Px = pdPos.Px + ((endPos.Px-startPos.Px)/this->fTotalDist)*pMove->fCartNextVel;
	pdPos.Py = pdPos.Py + ((endPos.Py-startPos.Py)/this->fTotalDist)*pMove->fCartNextVel;
	pdPos.Pz = pdPos.Pz + ((endPos.Pz-startPos.Pz)/this->fTotalDist)*pMove->fCartNextVel;
#ifndef RPY_ACC
	pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + ((endPos.Rpy.Pitch-startPos.Rpy.Pitch)/this->fTotalDist)*pMove->fCartNextVel;
	pdPos.Rpy.Roll  = pdPos.Rpy.Roll + ((endPos.Rpy.Roll-startPos.Rpy.Roll)/this->fTotalDist)*pMove->fCartNextVel;
	pdPos.Rpy.Yaw   = pdPos.Rpy.Yaw + ((endPos.Rpy.Yaw-startPos.Rpy.Yaw)/this->fTotalDist)*pMove->fCartNextVel;
#else
	if (robot.eLastFb != TRANS && !robot.bTrans)
	{
		pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + ((endPos.Rpy.Pitch-startPos.Rpy.Pitch)/this->fTotalDist)*pMove->fCartNextVel;
		pdPos.Rpy.Roll  = pdPos.Rpy.Roll + ((endPos.Rpy.Roll-startPos.Rpy.Roll)/this->fTotalDist)*pMove->fCartNextVel;
		pdPos.Rpy.Yaw   = pdPos.Rpy.Yaw + ((endPos.Rpy.Yaw-startPos.Rpy.Yaw)/this->fTotalDist)*pMove->fCartNextVel;
	}
	else
	{
		if (pMove->fRpyCurVel <= pMove->fRpyMaxVelCycle && !pMove->bRpyRamp)
		{
			pMove->fRpyCurVel += pMove->fRpyAccCycle;
		}
		else
		{
			pMove->bRpyRamp = true;
			pMove->fRpyCurVel -= pMove->fRpyAccCycle;
			pMove->fRpyCurVel = EC_MAX(0,pMove->fRpyCurVel);
		}	

		if (pMove->fRpyMaxDiff > 0.0001)
		{
			pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + pMove->fPitchDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
			pdPos.Rpy.Roll = pdPos.Rpy.Roll + pMove->fRollDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
			pdPos.Rpy.Yaw = pdPos.Rpy.Yaw + pMove->fYawDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
		}
	}
#endif

	RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);
	if(robot.bWave || robot.laserTrack.bLaserTrack)
		UpdateStartPos(&pdPos , pMove->fCartNextVel);

	LaserSearch();

	robot.Trans.pdPos = pdPos;
	this->wErrorID = invCart2Enc(&pdPos,pRefJoint,pEncoder);
	//printf("%.4f	%.4f	%.4f	%.4f	%.4f	%.4f,",pRefJoint->Theta1,pRefJoint->Theta2+PI/2,pRefJoint->Theta3,pRefJoint->Theta4,pRefJoint->Theta5,pRefJoint->Theta6);
	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		printf("error: %d\n",wErrorID);
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		robot.Axes.RobotAxis[i]->lCommandedPosition = pEncoder->Encoder[i];
		EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);//revise_l
	}
	//fprintf(robot.robotFile,"lin: %.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", pdPos.Px,pdPos.Py,pdPos.Pz,pdPos.Rpy.Pitch,pdPos.Rpy.Roll,pdPos.Rpy.Yaw);
	//fprintf(robot.robotFile,"%.6f\n",pMove->fCartNextVel*500);
	if(!robot.weldpara.bStilling)
		pMove->fCartRemainDist -= pMove->fCartNextVel;
	//printf("vel: %.6f,  remaindist: %.6f\n",pMove->fCartNextVel*500,pMove->fCartRemainDist);
#ifndef LONG_CIRC
	fTransDist = pMove->fCartRemainDist;//just for test
#endif
	pMove->fCartCurVel = pMove->fCartNextVel;
	if (!robot.bTrans && (pMove->fCartRemainDist <= 0 || pMove->fCartCurVel == 0))
	{
		bDone = MC_TRUE;
		robot.bConstantVel = false;
		//printf("Lin bDone");
	}
#ifdef LONG_CIRC
	if (robot.bTrans && pMove->fCartRemainDist <= fTransDist)//有过渡，则运行到过渡处即结束  just for test
	{
		bDone = MC_TRUE;
		robot.Trans.pdPos = pdPos;
		fTransDist = pMove->fCartRemainDist;
	}
#endif
	if (pMove->fCartCurVel == 0 && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING)
	{
		this->fbState = FBS_DONE;
		this->Execute = MC_FALSE;
		robot.eLastFb = LIN;
		this->bUseEndPosFound = false;
		this->bUseStartPosFound = false;
	}
	else if (pMove->fCartCurVel == 0 && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)//暂停
	{
		this->fbState = FBS_BUSY;
		robot.eLastFb = LIN;
		this->bUseEndPosFound = false;
		this->bUseStartPosFound = false;
		bDone = false;
		robot.bPause = true;
		printf("pause!");
	}
	else if (bDone || (pMove->fCartCurVel == 0 && robot.laserTrack.eSearchType != SEARCH_NOTHING && robot.laserTrack.bPosFound))
	{
		robot.eLastFb = LIN;
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
		this->Execute = MC_FALSE;
		bChangeAcc = false;
		robot.laserTrack.eSearchType = SEARCH_NOTHING;
		robot.laserTrack.bPosFound = false;
		robot.laserTrack.nInspectCycle = 0;
		robot.laserTrack.fFakeValue = robot.laserTrack.posY;
		this->bUseEndPosFound = false;
		this->bUseStartPosFound = false;
		printf("Lin Done\n");
	}
}
#endif


/************************************************************************/
/* Circ                                                                 */
/************************************************************************/
void MC_MoveCircle::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_MoveCircle::StateMachine()
{
   MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
	   {
		   this->bDone = MC_FALSE;          
		   this->bCommandAborted = MC_FALSE;
		   this->bError = MC_FALSE;
		   this->wErrorID = MC_NO_ERROR;
		//   this->fbState = FBS_IDLE;;
		   break;
      }
   } 
   
   return continueSm;
}

void MC_MoveCircle::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */

	if(fMaxVel > robot.RobotInfo.MaxLineSpeed)
		fMaxVel = robot.RobotInfo.MaxLineSpeed;
	if(fMaxAcc > robot.RobotInfo.MaxLineAcc)
		fMaxAcc = robot.RobotInfo.MaxLineAcc;
	if(fMaxVel <= 0)
		fMaxVel = 1.0;
	if(fMaxAcc <= 0)
		fMaxAcc = 1.0;

	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	this->fbState = FBS_BUSY;
}

void MC_MoveCircle::DoState_Busy()
{
   switch (robot.ePLCOpenState)
   {
   case GROUP_PLCOPEN_STATE_STANDBY:
// 	   //test
//    case GROUP_PLCOPEN_STATE_MOVING:
// 	   //test
      {
         MC_T_BOOL  fbActive = false;

		 if (this->wErrorID == MC_NO_ERROR)
		 {
			 robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
			 this->fbState = FBS_ACTIVE;
		 }
         else
         {
            this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
         }
         break;
      }
   case GROUP_PLCOPEN_STATE_STOPPING:
      {
         /* 3.3 MC_Stop. The axis rejects motion commands as int as MC_Stop parameter
          * Execute = TRUE, FB MC_MoveVelocity reports an error indicating the busy MC_Stop command.
          */
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->fbState = FBS_ERROR;
		 //robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
         break;
      }
   case GROUP_PLCOPEN_STATE_PAUSE:
	   printf("pause:\n");
	   break;
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->fbState = FBS_ERROR;
		 //robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
      }
   }
}

void MC_MoveCircle::StateTrans_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;
	//fp = fopen("circ.txt","a+");
    pMove->bContUpdate = this->bContUpdate;

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}
	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	startPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);
	if (robot.bTrans)
	{
		startPos = robot.pdPos;
		robot.Trans.pos1 = startPos;
		robot.Trans.pos2 = midPos;
		robot.Trans.pos3 = endPos;
	}
	TransPlane(startPos,midPos,endPos,&TZ);

	double d1,d2,d3,theta;
	d1 = sqrt(pow(midPos.Px-startPos.Px,2)+pow(midPos.Py-startPos.Py,2)+pow(midPos.Pz-startPos.Pz,2));
	d2 = sqrt(pow(endPos.Px-midPos.Px,2)+pow(endPos.Py-midPos.Py,2)+pow(endPos.Pz-midPos.Pz,2));
	x1 = y1 = y2 = 0;
	x2 = d1;
	//求pos3点在该平面的坐标,先求p12与p13的夹角
	d3 = sqrt(pow(endPos.Px-startPos.Px,2)+pow(endPos.Py-startPos.Py,2)+pow(endPos.Pz-startPos.Pz,2));
	theta = acos( (d1*d1+d3*d3-d2*d2)/2/d1/d3 );
	if (theta>-0.001 && theta<0.001)//不是圆弧
	{
		;
	}
	else
	{
		x3 = cos(theta)*d3;
		y3 = sin(theta)*d3;
		x0 = d1/2;
		y0 = (d3*d3/2-x0*x3)/y3;
		R = sqrt(x0*x0+y0*y0);
		alpha1 = atan2(y1-y0,x1-x0);
		alpha3 = atan2(y3-y0,x3-x0);
		Alpha = alpha3 - alpha1;
		//运动总路程
		fTotalDist = fabs(Alpha)*R;
		nCycle = fTotalDist/pMove->fCartConstantVel;
		px0 = TZ.Nx*x0 +TZ.Ox*y0 + startPos.Px;
		py0 = TZ.Ny*x0 +TZ.Oy*y0 + startPos.Py;
		pz0 = TZ.Nz*x0 +TZ.Oz*y0 + startPos.Pz;	
	}
#ifdef _3D_SEWING_
	if (robot.bSetSewPara)
	{
		this->fMaxVel = robot.sewPara.fRobSpeed;
	}
#endif
	this->fConstantVel = EC_MIN(this->fMaxVel,sqrt(this->fTotalDist*this->fMaxAcc));
	pMove->fCartConstantVel =  (this->fConstantVel* robot.dwCycleTime / 1E6);
	pMove->fCartMaxAcc =  (this->fMaxAcc* robot.dwCycleTime / 1E6* robot.dwCycleTime / 1E6);
	if(!robot.bTrans)
		pMove->fCartCurVel = 0;
	pMove->fCartEndVel = 0;
	MC_T_REAL tRamp = this->fConstantVel/this->fMaxAcc;
	this->fDist3to4 = this->fConstantVel*tRamp/2;
	pMove->fCartDist3to4 = this->fDist3to4;

	pMove->fOrgMaxVelocity = this->fMaxVel;
	pMove->fOrgAcceleration = this->fMaxAcc;
	pMove->fCartTotalDist = this->fTotalDist;
	pMove->fCartRemainDist = pMove->fCartTotalDist;	

	dAlpha = 0;
	pdPos = startPos;	
}

void MC_MoveCircle::DoState_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;


	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING || robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)  //stop功能嵌入各个功能块    stop完成，参数输出如何做？
	{
		pMove->fCartTargetVel = 0;
		if (pMove->fCartCurVel - pMove->fCartMaxAcc >= pMove->fCartTargetVel)//减速
		{
			pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
		}
		else //最后一个周期
		{
			pMove->fCartNextVel = pMove->fCartTargetVel;
		}
	} 
	else
	{
		//轨迹插补
		if (pMove->fCartRemainDist >= pMove->fCartDist3to4 + pMove->fCartConstantVel)
		{
			pMove->fCartTargetVel = pMove->fCartConstantVel;
			if (pMove->fCartCurVel + pMove->fCartMaxAcc < pMove->fCartTargetVel)
			{
				pMove->fCartNextVel = pMove->fCartCurVel  + pMove->fCartMaxAcc;
			}
			else 
			{
				pMove->fCartNextVel = pMove->fCartTargetVel;
			}
		}
		else
		{
			if (!robot.bTrans)
			{
				pMove->fCartTargetVel = pMove->fCartEndVel;
				if ((pMove->fCartRemainDist < pMove->fCartCurVel)||((pMove->fCartRemainDist >= pMove->fCartCurVel)&&(pMove->fCartCurVel < pMove->fCartMaxAcc)))
				{
					//pMove->fCartNextVel = pMove->fCartRemainDist;20170516
					pMove->fCartNextVel = 0;
				}
				else
				{
					pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;//revise_l
				}
			}
			else
			{
				pMove->fCartNextVel = pMove->fCartCurVel;
				bDone = TRUE;
			}
		}
	}
	

	dAlpha += pMove->fCartNextVel/R;

	if(Alpha > 0)
	{
		x = x0 + R*cos(alpha1+dAlpha);
		y = y0 + R*sin(alpha1+dAlpha);
	}
	else
	{
		x = x0 + R*cos(alpha1-dAlpha);
		y = y0 + R*sin(alpha1-dAlpha);
	}


	//printf("%.6f,  %.6f,  %.6f\n", pdPos.Rpy.Pitch,pdPos.Rpy.Roll,pdPos.Rpy.Yaw);
	pdPos.Px = TZ.Nx*x +TZ.Ox*y + startPos.Px;
	pdPos.Py = TZ.Ny*x +TZ.Oy*y + startPos.Py;
	pdPos.Pz = TZ.Nz*x +TZ.Oz*y + startPos.Pz;			

	SewMat.Nx = (pdPos.Px - px0)/R;
	SewMat.Ny = (pdPos.Py - py0)/R;
	SewMat.Nz = (pdPos.Pz - pz0)/R;
// 	SewMat.Ax = TZ.Ax;
// 	SewMat.Ay = TZ.Ay;
// 	SewMat.Az = TZ.Az;
	SewMat.Ax = 0;
	SewMat.Ay = 0;
	SewMat.Az = 1;  //直接给定法向
	SewMat.Ox = SewMat.Ay*SewMat.Nz - SewMat.Az*SewMat.Ny;
	SewMat.Oy = SewMat.Az*SewMat.Nx - SewMat.Ax*SewMat.Nz;
	SewMat.Oz = SewMat.Ax*SewMat.Ny - SewMat.Ay*SewMat.Nx;
	MatToRpy(&SewMat,&pdPos.Rpy);
// 	pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + ((endPos.Rpy.Pitch-startPos.Rpy.Pitch)/fTotalDist)*pMove->fCartNextVel;  /////姿态均匀变化，不按照圆弧切向走
// 	pdPos.Rpy.Roll = pdPos.Rpy.Roll + ((endPos.Rpy.Roll-startPos.Rpy.Roll)/fTotalDist)*pMove->fCartNextVel;
// 	pdPos.Rpy.Yaw = pdPos.Rpy.Yaw + ((endPos.Rpy.Yaw-startPos.Rpy.Yaw)/fTotalDist)*pMove->fCartNextVel;

	RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);
	//printf("%.4f,  %.4f,  %.4f,  %.4f,  %.4f,  %.4f\n",(pRefJoint->Theta1)*180/PI,(pRefJoint->Theta2+PI/2)*180/PI,(pRefJoint->Theta3)*180/PI,(pRefJoint->Theta4)*180/PI,(pRefJoint->Theta5)*180/PI,(pRefJoint->Theta6)*180/PI);
	//printf("%d,  %d,  %d,  %d,  %d,  %d\n", pEncoder->Encoder[0],pEncoder->Encoder[1],pEncoder->Encoder[2],pEncoder->Encoder[3],pEncoder->Encoder[4],pEncoder->Encoder[5]);
	this->wErrorID = invCart2Enc(&pdPos,pRefJoint,pEncoder);
	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		printf("error: %d\n",wErrorID);
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		robot.Axes.RobotAxis[i]->lCommandedPosition = pEncoder->Encoder[i];
		EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);//revise_l
	}
	//fprintf(fp,"%.6f,%.6f,%.6f\n", pdPos.Px,pdPos.Py,pdPos.Pz);
	pMove->fCartRemainDist -= pMove->fCartNextVel;
	pMove->fCartCurVel = pMove->fCartNextVel;
	if (pMove->fCartRemainDist <= 0 || pMove->fCartCurVel == 0)
	{
		bDone = MC_TRUE;
	}

	if (bDone && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING)
	{
		this->fbState = FBS_DONE;
		this->Execute = MC_FALSE;
	}
	else if (bDone && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)//暂停
	{
		this->fbState = FBS_BUSY;
		robot.eLastFb = CIRC;
		bDone = false;
		robot.bPause = true;
		printf("pause!");
	}
	else if (bDone)
	{
		robot.eLastFb = CIRC;
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
		this->Execute = MC_FALSE;
	}
}

/************************************************************************/
/* Trans    有预读                                                      */
/************************************************************************/
#ifdef PRE_IPR
void MC_Trans::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_Trans::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

	switch (this->fbState)
	{
	case FBS_IDLE:
		{
			if (this->bExecute)
			{
				StateTrans_Execute();
				if (this->fbState == FBS_ERROR)
				{
					this->bError = MC_TRUE;
					continueSm = MC_TRUE;
				}
				else if (this->fbState == FBS_BUSY)
				{
					this->bBusy = MC_TRUE;
					continueSm = MC_TRUE;
				}
			}
			break;
		}
	case FBS_BUSY:
		{
			DoState_Busy();
			if (this->fbState == FBS_ACTIVE)
			{
				StateTrans_Active();
				this->bActive = MC_TRUE;
				continueSm = MC_TRUE;
			}

			if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_ACTIVE:
		{
			DoState_Active();
			if (this->fbState == FBS_DONE)
			{
				this->bDone = MC_TRUE;
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				continueSm = MC_TRUE;
			}
			else if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
	case FBS_ERROR:
	case FBS_DONE:
		{
			this->bDone = MC_FALSE;           
			this->bCommandAborted = MC_FALSE;
			this->bError = MC_FALSE;
			this->wErrorID = MC_NO_ERROR;
			//  this->fbState = FBS_IDLE;
			break;
		}
	}

	return continueSm;
}

void MC_Trans::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */


	this->fbState = FBS_BUSY;
}

void MC_Trans::DoState_Busy()
{
	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)
	{
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
		this->Execute = MC_FALSE;
	}
	this->fbState = FBS_ACTIVE;
	robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;

}

void MC_Trans::StateTrans_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;
	pos3.Px = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0];//注意索引
	pos3.Py = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[1];
	pos3.Pz = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[2];
	pos3.Rpy.Pitch = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[3];
	pos3.Rpy.Roll  = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[4];
	pos3.Rpy.Yaw   = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[5];
	TransPlane(pos1,pos2,pos3,&TZ);
	endPos = pos3;
	startPos = pdPos;

	if((pos3.Rpy.Pitch - pos2.Rpy.Pitch) > PI)
		pos3.Rpy.Pitch -= 2*PI;
	else if ((pos3.Rpy.Pitch - pos2.Rpy.Pitch) <-PI)
		pos3.Rpy.Pitch += 2*PI;
	if((pos3.Rpy.Roll - pos2.Rpy.Roll) > PI)
		pos3.Rpy.Roll -= 2*PI;
	else if ((pos3.Rpy.Roll - pos2.Rpy.Roll) <-PI)
		pos3.Rpy.Roll += 2*PI;
	if((pos3.Rpy.Yaw - pos2.Rpy.Yaw) > PI)
		pos3.Rpy.Yaw -= 2*PI;
	else if ((pos3.Rpy.Yaw - pos2.Rpy.Yaw) <-PI)
		pos3.Rpy.Yaw += 2*PI;

	if((endPos.Rpy.Pitch - startPos.Rpy.Pitch) > PI)
		endPos.Rpy.Pitch -= 2*PI;
	else if ((endPos.Rpy.Pitch - startPos.Rpy.Pitch) <-PI)
		endPos.Rpy.Pitch += 2*PI;
	if((endPos.Rpy.Roll - startPos.Rpy.Roll) > PI)
		endPos.Rpy.Roll -= 2*PI;
	else if ((endPos.Rpy.Roll - startPos.Rpy.Roll) <-PI)
		endPos.Rpy.Roll += 2*PI;
	if((endPos.Rpy.Yaw - startPos.Rpy.Yaw) > PI)
		endPos.Rpy.Yaw -= 2*PI;
	else if ((endPos.Rpy.Yaw - startPos.Rpy.Yaw) <-PI)
		endPos.Rpy.Yaw += 2*PI;

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;//用实际位置还是指定的起始位置？PTP同 revise_l
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}
	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	//pRefJoint->Theta1 = robot.Lin.pRefJoint->Theta1;//just for simu
	//pRefJoint->Theta2 = robot.Lin.pRefJoint->Theta2;//just for simu
	//pRefJoint->Theta3 = robot.Lin.pRefJoint->Theta3;//just for simu
	//pRefJoint->Theta4 = robot.Lin.pRefJoint->Theta4;//just for simu
	//pRefJoint->Theta5 = robot.Lin.pRefJoint->Theta5;//just for simu
	//pRefJoint->Theta6 = robot.Lin.pRefJoint->Theta6;//just for simu
	//pos1,pos3更新，pos2不变
	pos1 = pdPos;
	double tempDist;
	CARTPOS tempPos;
	tempDist = sqrt(pow((pos3.Px-pos2.Px),2)+pow((pos3.Py-pos2.Py),2)+pow((pos3.Pz-pos2.Pz),2));
	tempPos.Px = pos2.Px + (pos3.Px-pos2.Px)*robot.Lin.fTransDist/tempDist;
	tempPos.Py = pos2.Py + (pos3.Py-pos2.Py)*robot.Lin.fTransDist/tempDist;
	tempPos.Pz = pos2.Pz + (pos3.Pz-pos2.Pz)*robot.Lin.fTransDist/tempDist;
	tempPos.Rpy.Pitch = pos2.Rpy.Pitch + (pos3.Rpy.Pitch-pos2.Rpy.Pitch)*robot.Lin.fTransDist/tempDist;
	tempPos.Rpy.Roll = pos2.Rpy.Roll + (pos3.Rpy.Roll-pos2.Rpy.Roll)*robot.Lin.fTransDist/tempDist;
	tempPos.Rpy.Yaw = pos2.Rpy.Yaw + (pos3.Rpy.Yaw-pos2.Rpy.Yaw)*robot.Lin.fTransDist/tempDist;
#ifdef LONG_CIRC
	tempPos.Rpy.Pitch = pos3.Rpy.Pitch;  //just for test
	tempPos.Rpy.Roll = pos3.Rpy.Roll;
	tempPos.Rpy.Yaw = pos3.Rpy.Yaw;
#endif
	pos3 = tempPos;

	notTrans = false;
	d1 = sqrt(pow(pos2.Px-pos1.Px,2)+pow(pos2.Py-pos1.Py,2)+pow(pos2.Pz-pos1.Pz,2));
	d2 = sqrt(pow(pos3.Px-pos2.Px,2)+pow(pos3.Py-pos2.Py,2)+pow(pos3.Pz-pos2.Pz,2));
	x1 = y1 = y2 = 0;
	x2 = d1;
	//求pos3点在该平面的坐标,先求p12与p13的夹角
	d3 = sqrt(pow(pos3.Px-pos1.Px,2)+pow(pos3.Py-pos1.Py,2)+pow(pos3.Pz-pos1.Pz,2));
	theta13 = acos( (d1*d1+d3*d3-d2*d2)/2.0/d1/d3 );
	theta12 = acos( (d1*d1+d2*d2-d3*d3)/2.0/d1/d2 );
	if (theta13>-0.001 && theta13<0.001)//无需过渡*****************************************************************???????????????
	{
		totalDist = 2.0*robot.Lin.fDist3to4;
		nCycle = totalDist/pMove->fCartConstantVel;
		notTrans = true;
		pMove->fCartRemainDist = d3;
	}
	else
	{
		x3 = cos(theta13)*d3;
		y3 = sin(theta13)*d3;
		x0 = 0;
		//y0 = y3-(x2-x3)*x3/y3;
		//R = y0;
		R = d3*sqrt(1.0/2.0/(1.0+cos(theta12)));
		y0 = R;
		Alpha = acos(1.0-d3*d3/R/R/2);
		//运动总路程
		totalDist = fabs(Alpha)*R;

		px0 = TZ.Nx*x0 +TZ.Ox*y0 + pos1.Px;
		py0 = TZ.Ny*x0 +TZ.Oy*y0 + pos1.Py;
		pz0 = TZ.Nz*x0 +TZ.Oz*y0 + pos1.Pz;	
	}
	float fEndVel;
	fEndVel = EC_MIN(robot.bufferBlock[robot.wExecute].Lin.fMaxVel,tempDist*robot.bufferBlock[robot.wExecute].Lin.fMaxAcc);
	pMove->fCartEndVel = fEndVel * robot.dwCycleTime / 1E6;
	dAlpha = 0;


	//求解三次多项式的系数
	float Acur = pMove->fCartMaxAcc/robot.dwCycleTime/robot.dwCycleTime*1E6*1E6;
	float Vcur = pMove->fCartCurVel/robot.dwCycleTime*1E6;
	float Tacc = fabs(fEndVel - Vcur)/Acur;
	float Lacc = (Vcur + fEndVel)*Tacc/2.0;
	float Tconst = EC_MAX((totalDist-Lacc),0)/fEndVel;
	float Ltotal = sqrt(pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[0]-pos3.Px),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[1]-pos3.Py),2)+pow((robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[2]-pos3.Pz),2));
	CARTPOS temp1;
	temp1.Rpy.Pitch = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[3];
	temp1.Rpy.Roll  = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[4];
	temp1.Rpy.Yaw   = robot.bufferBlock[robot.wExecute].Lin.posTarget.fpos[5];
	if((temp1.Rpy.Pitch - pos3.Rpy.Pitch) > PI)
		temp1.Rpy.Pitch -= 2*PI;
	else if ((temp1.Rpy.Pitch - pos3.Rpy.Pitch) <-PI)
		temp1.Rpy.Pitch += 2*PI;
	if((temp1.Rpy.Roll - pos3.Rpy.Roll) > PI)
		temp1.Rpy.Roll -= 2*PI;
	else if ((temp1.Rpy.Roll - pos3.Rpy.Roll) <-PI)
		temp1.Rpy.Roll += 2*PI;
	if((temp1.Rpy.Yaw - pos3.Rpy.Yaw) > PI)
		temp1.Rpy.Yaw -= 2*PI;
	else if ((temp1.Rpy.Yaw - pos3.Rpy.Yaw) <-PI)
		temp1.Rpy.Yaw += 2*PI;

	Ttrans = Tacc + Tconst;
	omega0[0] = (robot.Lin.endPos.Rpy.Roll - robot.Lin.startPos.Rpy.Roll)/robot.Lin.fTotalDist*Vcur;
	omega0[1] = (robot.Lin.endPos.Rpy.Pitch - robot.Lin.startPos.Rpy.Pitch)/robot.Lin.fTotalDist*Vcur;
	omega0[2] = (robot.Lin.endPos.Rpy.Yaw - robot.Lin.startPos.Rpy.Yaw)/robot.Lin.fTotalDist*Vcur;
	omega1[0] = (temp1.Rpy.Roll - pos3.Rpy.Roll)/Ltotal*fEndVel;
	omega1[1] = (temp1.Rpy.Pitch - pos3.Rpy.Pitch)/Ltotal*fEndVel;
	omega1[2] = (temp1.Rpy.Yaw - pos3.Rpy.Yaw)/Ltotal*fEndVel;
	a0[0] = pos1.Rpy.Roll;	a1[0] = omega0[0];  a2[0] = (3.0*(pos3.Rpy.Roll - pos1.Rpy.Roll) - (2.0*omega0[0] + omega1[0])*Ttrans)/Ttrans/Ttrans;	 a3[0] = (-2.0*(pos3.Rpy.Roll - pos1.Rpy.Roll) + (omega0[0] + omega1[0])*Ttrans)/Ttrans/Ttrans/Ttrans;
	a0[1] = pos1.Rpy.Pitch;	a1[1] = omega0[1];	a2[1] = (3.0*(pos3.Rpy.Pitch - pos1.Rpy.Pitch) - (2.0*omega0[1] + omega1[1])*Ttrans)/Ttrans/Ttrans;  a3[1] = (-2.0*(pos3.Rpy.Pitch - pos1.Rpy.Pitch) + (omega0[1] + omega1[1])*Ttrans)/Ttrans/Ttrans/Ttrans;
	a0[2] = pos1.Rpy.Yaw;	a1[2] = omega0[2];	a2[2] = (3.0*(pos3.Rpy.Yaw - pos1.Rpy.Yaw) - (2.0*omega0[2] + omega1[2])*Ttrans)/Ttrans/Ttrans;		 a3[2] = (-2.0*(pos3.Rpy.Yaw - pos1.Rpy.Yaw) + (omega0[2] + omega1[2])*Ttrans)/Ttrans/Ttrans/Ttrans;
	Tcycle = 0;
	//以上...求解三次多项式的系数

#ifdef RPY_ACC
	//rpy变化参数赋值
	pMove->fRpyMaxDiff = 0;
	pMove->fPitchDiff = endPos.Rpy.Pitch - startPos.Rpy.Pitch;
	pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fPitchDiff ));
	pMove->fRollDiff  = endPos.Rpy.Roll - startPos.Rpy.Roll;
	pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fRollDiff));
	pMove->fYawDiff = endPos.Rpy.Yaw - startPos.Rpy.Yaw;
	pMove->fRpyMaxDiff = EC_MAX(pMove->fRpyMaxDiff , fabs(pMove->fYawDiff));

	pMove->fRpyAcc = 4 * pMove->fRpyMaxDiff * fEndVel * fEndVel / (totalDist+tempDist - 2*robot.Lin.fTransDist) / (totalDist+tempDist - 2*robot.Lin.fTransDist);//4*theta*v^2/l^2
	pMove->fRpyMaxVelCycle = 2*pMove->fRpyMaxDiff * fEndVel / (totalDist+tempDist - 2*robot.Lin.fTransDist) * robot.dwCycleTime/1E6;
	pMove->fRpyAccCycle = pMove->fRpyAcc * robot.dwCycleTime*robot.dwCycleTime/1E6/1E6;
	pMove->fRpyCurVel = 0;
	pMove->bRpyRamp = false;
#endif
}

void MC_Trans::DoState_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;
	nCycle--;

	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_STOPPING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_PAUSE) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING || robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)  //stop功能嵌入各个功能块    stop完成，参数输出如何做？
	{
		pMove->fCartTargetVel = 0;
		if (pMove->fCartCurVel - pMove->fCartMaxAcc >= pMove->fCartTargetVel)//减速
		{
			pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
		}
		else //最后一个周期
		{
			pMove->fCartNextVel = pMove->fCartTargetVel;
		}
		pdPos.Px = pdPos.Px + ((pos3.Px-pos1.Px)/d3)*pMove->fCartNextVel;
		pdPos.Py = pdPos.Py + ((pos3.Py-pos1.Py)/d3)*pMove->fCartNextVel;
		pdPos.Pz = pdPos.Pz + ((pos3.Pz-pos1.Pz)/d3)*pMove->fCartNextVel;
	} 
	else
	{
		if (notTrans)
		{
			if (pMove->fCartCurVel > (pMove->fCartEndVel + 0.001))
				pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
			else if (pMove->fCartCurVel < (pMove->fCartEndVel - 0.001))
				pMove->fCartNextVel = pMove->fCartCurVel + pMove->fCartMaxAcc;
			else
				pMove->fCartNextVel = pMove->fCartEndVel;

			pdPos.Px = pdPos.Px + ((pos3.Px-pos1.Px)/d3)*pMove->fCartNextVel;
			pdPos.Py = pdPos.Py + ((pos3.Py-pos1.Py)/d3)*pMove->fCartNextVel;
			pdPos.Pz = pdPos.Pz + ((pos3.Pz-pos1.Pz)/d3)*pMove->fCartNextVel;
#ifndef RPY_ACC
			//测试三次多项式注释掉
/*			pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + ((pos3.Rpy.Pitch-pos1.Rpy.Pitch)/d3)*pMove->fCartNextVel;
			pdPos.Rpy.Roll  = pdPos.Rpy.Roll + ((pos3.Rpy.Roll-pos1.Rpy.Roll)/d3)*pMove->fCartNextVel;
			pdPos.Rpy.Yaw   = pdPos.Rpy.Yaw + ((pos3.Rpy.Yaw-pos1.Rpy.Yaw)/d3)*pMove->fCartNextVel;  */  

#endif
			pMove->fCartRemainDist -= pMove->fCartNextVel;
		}
		else
		{
			if (dAlpha < Alpha)
			{
				if (pMove->fCartCurVel > (pMove->fCartEndVel + 0.001))
					pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
				else if (pMove->fCartCurVel < (pMove->fCartEndVel - 0.001))
					pMove->fCartNextVel = pMove->fCartCurVel + pMove->fCartMaxAcc;
				else
					pMove->fCartNextVel = pMove->fCartEndVel;
			}

			dAlpha += pMove->fCartNextVel/R;
			x = R*sin(dAlpha);
			y = R-R*cos(dAlpha);

			pdPos.Px = TZ.Nx*x +TZ.Ox*y + pos1.Px;
			pdPos.Py = TZ.Ny*x +TZ.Oy*y + pos1.Py;
			pdPos.Pz = TZ.Nz*x +TZ.Oz*y + pos1.Pz;			
#ifndef RPY_ACC
			//测试三次多项式注释掉
			//pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + ((pos3.Rpy.Pitch-pos1.Rpy.Pitch)/totalDist)*pMove->fCartNextVel;
			//pdPos.Rpy.Roll = pdPos.Rpy.Roll + ((pos3.Rpy.Roll-pos1.Rpy.Roll)/totalDist)*pMove->fCartNextVel;
			//pdPos.Rpy.Yaw = pdPos.Rpy.Yaw + ((pos3.Rpy.Yaw-pos1.Rpy.Yaw)/totalDist)*pMove->fCartNextVel;
#endif		
			if (robot.bSewPlane)
			{
				SewMat.Nx = (pdPos.Px - px0)/R;
				SewMat.Ny = (pdPos.Py - py0)/R;
				SewMat.Nz = (pdPos.Pz - pz0)/R;
				// 	SewMat.Ax = TZ.Ax;
				// 	SewMat.Ay = TZ.Ay;
				// 	SewMat.Az = TZ.Az;
				SewMat.Ax = 0;
				SewMat.Ay = 0;
				SewMat.Az = 1;  //直接给定法向
				SewMat.Ox = SewMat.Ay*SewMat.Nz - SewMat.Az*SewMat.Ny;
				SewMat.Oy = SewMat.Az*SewMat.Nx - SewMat.Ax*SewMat.Nz;
				SewMat.Oz = SewMat.Ax*SewMat.Ny - SewMat.Ay*SewMat.Nx;

				MatToRpy(&SewMat,&pdPos.Rpy);
			}
		}
#ifdef RPY_ACC 
		if (pMove->fRpyCurVel <= pMove->fRpyMaxVelCycle && !pMove->bRpyRamp)
		{
			pMove->fRpyCurVel += pMove->fRpyAccCycle;
		}
		else
		{
			pMove->bRpyRamp = true;
			pMove->fRpyCurVel -= pMove->fRpyAccCycle;
			pMove->fRpyCurVel = EC_MAX(0,pMove->fRpyCurVel);
		}		

		if (pMove->fRpyMaxDiff > 0.0001)
		{
			pdPos.Rpy.Pitch = pdPos.Rpy.Pitch + pMove->fPitchDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
			pdPos.Rpy.Roll = pdPos.Rpy.Roll + pMove->fRollDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
			pdPos.Rpy.Yaw = pdPos.Rpy.Yaw + pMove->fYawDiff/pMove->fRpyMaxDiff * pMove->fRpyCurVel;
		}
#endif

	}
	
	//测试三次多项式
	Tcycle += robot.dwCycleTime/1E6;
	pdPos.Rpy.Roll  = a0[0] + a1[0]*Tcycle + a2[0]*Tcycle*Tcycle + a3[0]*Tcycle*Tcycle*Tcycle;
	pdPos.Rpy.Pitch = a0[1] + a1[1]*Tcycle + a2[1]*Tcycle*Tcycle + a3[1]*Tcycle*Tcycle*Tcycle;
	pdPos.Rpy.Yaw   = a0[2] + a1[2]*Tcycle + a2[2]*Tcycle*Tcycle + a3[2]*Tcycle*Tcycle*Tcycle;


	pMove->fCartCurVel = pMove->fCartNextVel;

	RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);

	this->wErrorID = invCart2Enc(&pdPos,pRefJoint,pEncoder);
	//printf("%.4f	%.4f	%.4f	%.4f	%.4f	%.4f,",pRefJoint->Theta1,pRefJoint->Theta2+PI/2,pRefJoint->Theta3,pRefJoint->Theta4,pRefJoint->Theta5,pRefJoint->Theta6);
	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		printf("error: %d\n",wErrorID);
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		robot.Axes.RobotAxis[i]->lCommandedPosition = pEncoder->Encoder[i];
		EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);//revise_l
	}
	//fprintf(robot.robotFile,"trans, %.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", pdPos.Px,pdPos.Py,pdPos.Pz,pdPos.Rpy.Pitch,pdPos.Rpy.Roll,pdPos.Rpy.Yaw);
	//fprintf(robot.robotFile,"%.6f\n",pMove->fCartNextVel*500);
	if (dAlpha >= Alpha && !notTrans && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_MOVING)
		bDone = true;

	else if (notTrans && pMove->fCartRemainDist <= 0 && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_MOVING)
			bDone = true;

	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING && pMove->fCartCurVel == 0)
	{
		bDone = true;
		this->fbState = FBS_DONE;
		this->Execute = MC_FALSE;
		robot.eLastFb = TRANS;
	}
	else if (pMove->fCartCurVel == 0 && robot.ePLCOpenState == GROUP_PLCOPEN_STATE_PAUSE)//暂停
	{
		this->fbState = FBS_BUSY;
		robot.eLastFb = AIR;
		bDone = false;
		robot.bPause = true;
		printf("pause!");
	}
	else if (bDone)
	{
		robot.eLastFb = TRANS;
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
		this->Execute = MC_FALSE;
		robot.pdPos = pdPos;
		printf("Trans DONE! \n");
	}
}
#endif


/************************************************************************/
/* 三角摆动                                                             */
/************************************************************************/
void MC_MoveTrigle::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_MoveTrigle::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.
	//printf("StateMachine!\n");
   switch (this->fbState)
   {
   case FBS_IDLE:
      {
         if (this->bExecute)
         {
            StateTrans_Execute();
            if (this->fbState == FBS_ERROR)
            {
               this->bError = MC_TRUE;
               continueSm = MC_TRUE;
            }
            else if (this->fbState == FBS_BUSY)
            {
               this->bBusy = MC_TRUE;
               continueSm = MC_TRUE;
            }
         }
         break;
      }
   case FBS_BUSY:
      {
         DoState_Busy();
         if (this->fbState == FBS_ACTIVE)
         {
            StateTrans_Active();
            this->bActive = MC_TRUE;
            continueSm = MC_TRUE;
         }

         if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_ACTIVE:
      {
         DoState_Active();
         if (this->fbState == FBS_DONE)
         {
            this->bDone = MC_TRUE;
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            continueSm = MC_TRUE;
         }
         else if (this->fbState == FBS_ERROR)
         {
            this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
            this->bActive = MC_FALSE;
            this->bError = MC_TRUE;
            continueSm = MC_TRUE;
         }
         break;
      }
   case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
   case FBS_ERROR:
   case FBS_DONE:
	   {
		   this->bDone = MC_FALSE;          
		   this->bCommandAborted = MC_FALSE;
		   this->bError = MC_FALSE;
		   this->wErrorID = MC_NO_ERROR;
		  // this->fbState = FBS_IDLE;
		   break;
      }
   } 
   
   return continueSm;
}

void MC_MoveTrigle::StateTrans_Execute()
{
	/* revise_l  考虑增加一些条件限制 */

	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
	printf("StateTrans_Execute!\n");
	this->fbState = FBS_BUSY;
}

void MC_MoveTrigle::DoState_Busy()
{
	printf("DoState_Busy!\n");
   switch (robot.ePLCOpenState)
   {
   case GROUP_PLCOPEN_STATE_STANDBY:
  // case GROUP_PLCOPEN_STATE_MOVING:
      {
         MC_T_BOOL  fbActive = false;

		 if (this->wErrorID == MC_NO_ERROR)
		 {
			 robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
			 this->fbState = FBS_ACTIVE;
		 }
         else
         {
            this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
         }
         break;
      }
   case GROUP_PLCOPEN_STATE_STOPPING:
      {
         this->wErrorID = MC_ERR_BUSY_STOPPING;
         this->fbState = FBS_ERROR;
		 //robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
         break;
      }
   default:
      {
         this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
         this->fbState = FBS_ERROR;
		 //robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
      }
   }
}

void MC_MoveTrigle::StateTrans_Active()
{
	printf("StateTrans_Active!\n");
	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;//用实际位置还是指定的起始位置？PTP同 revise_l
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}

	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	startPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);

	endPos.Px = posTarget.fpos[0];
	endPos.Py = posTarget.fpos[1];
	endPos.Pz = posTarget.fpos[2];
	endPos.Rpy.Pitch = posTarget.fpos[3];
	endPos.Rpy.Roll = posTarget.fpos[4];
	endPos.Rpy.Yaw = posTarget.fpos[5];

// 	endPos = startPos;    //test
// 	endPos.Px += 100;
   bRemainInit = false;
   nCycleNum = 0;
   nRemainNum = 0;
//    tpa.Apinlv = 1.5;
//    tpa.Azhenfu = 15;
//    tpa.WSpeed = 6;
//    Wetime.T_stay1 = 0.3;
//    Wetime.T_stay2 = 0.3;
//    PF.Vx = 0;
//    PF.Vy = 0;
//    PF.Vz = 1;
   tpa = tpa;
   PF = PF;
   Wetime = Wetime;

	//一个单一周期运动分成5段      //前面部分放到statetrans里面
	posWeld.Px = 0;
	posWeld.Py = 0;
	posWeld.Pz = 0;
	double Tsin=1/tpa.Apinlv;

	N2=floor(Wetime.T_stay1*1e6/robot.dwCycleTime);
	N4=floor(Wetime.T_stay2*1e6/robot.dwCycleTime);
	N1=floor(Tsin*1e6/(4*robot.dwCycleTime));
	N3=2*N1;
	N5=N1;
	Nx=N1+N2+N3+N4+N5;

	this->fTotalDist = sqrt(pow(endPos.Px-startPos.Px,2)+pow(endPos.Py-startPos.Py,2)+pow(endPos.Pz-startPos.Pz,2));
	Wetime.T_totle=fTotalDist/tpa.WSpeed;
	N=floor(Wetime.T_totle*1e6/robot.dwCycleTime);//计算运动的总点数
	int k_move=floor(Wetime.T_totle/(Tsin+Wetime.T_stay1+Wetime.T_stay2)+0.9);//计算运动的周期个数
	Nk=floor(k_move*(Tsin*1e6/robot.dwCycleTime));//计算y运动的点数

	//计算焊缝参数

	Vector pfy,pfx;
	pfy.Vx=(endPos.Px-startPos.Px)/fTotalDist;
	pfy.Vy=(endPos.Py-startPos.Py)/fTotalDist;
	pfy.Vz=(endPos.Pz-startPos.Pz)/fTotalDist;

	pfx.Vx=pfy.Vy*PF.Vz-pfy.Vz*PF.Vy;
	pfx.Vy=pfy.Vz*PF.Vx-PF.Vz*pfy.Vx;
	pfx.Vz=pfy.Vx*PF.Vy-PF.Vx*pfy.Vy;

	matWeld.Nx = pfx.Vx; matWeld.Ox = pfy.Vx; matWeld.Ax = PF.Vx;
	matWeld.Ny = pfx.Vy; matWeld.Oy = pfy.Vy; matWeld.Ay = PF.Vy;
	matWeld.Nz = pfx.Vz; matWeld.Oz = pfy.Vz; matWeld.Az = PF.Vz;

	pdPos = startPos;

	if(this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
	printf("Plane: x = %.1f, y = %.1f, z = %.1f\n ",PF.Vx,PF.Vy,PF.Vz);
}

void MC_MoveTrigle::DoState_Active()
{
	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_STOPPING) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_MOVING)
	{
		if(nCycleNum<N)
		{	   
			int ik=nCycleNum%Nx;
			if((ik>=0) && (ik<=(N1-1)))
			{
				//N1阶段	     
				posWeld.Px = (ik+1)*(tpa.Azhenfu/N1);
				posWeld.Py += fTotalDist/Nk;
				posWeld.Pz = 0;

				pdPos.Px=startPos.Px + posWeld.Px*matWeld.Nx + posWeld.Py*matWeld.Ox + posWeld.Pz*matWeld.Ax;
				pdPos.Py=startPos.Py + posWeld.Px*matWeld.Ny + posWeld.Py*matWeld.Oy + posWeld.Pz*matWeld.Ay;
				pdPos.Pz=startPos.Pz + posWeld.Px*matWeld.Nz + posWeld.Py*matWeld.Oz + posWeld.Pz*matWeld.Az;

				posWeld.Rpy.Roll += (endPos.Rpy.Roll - startPos.Rpy.Roll)/Nk;
				posWeld.Rpy.Pitch += (endPos.Rpy.Pitch - startPos.Rpy.Pitch)/Nk;
				posWeld.Rpy.Yaw += (endPos.Rpy.Yaw - startPos.Rpy.Yaw)/Nk;

				pdPos.Rpy.Roll = startPos.Rpy.Roll + posWeld.Rpy.Roll;
				pdPos.Rpy.Pitch = startPos.Rpy.Pitch + posWeld.Rpy.Pitch;
				pdPos.Rpy.Yaw = startPos.Rpy.Yaw + posWeld.Rpy.Yaw;

				RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);		 
			}
			else if((ik>=N1) && (ik<=(N1+N2-1)))
			{
				//N2阶段
				pdPos = pdPos;
			}
			else if((ik>=(N1+N2)) && (ik<=(N1+N2+N3-1)))		   
			{
				//N3阶段
				posWeld.Px = tpa.Azhenfu-(ik+1-N1-N2)*(2*tpa.Azhenfu/N3);
				posWeld.Py += fTotalDist/Nk;
				posWeld.Pz = 0;

				pdPos.Px=startPos.Px + posWeld.Px*matWeld.Nx + posWeld.Py*matWeld.Ox + posWeld.Pz*matWeld.Ax;
				pdPos.Py=startPos.Py + posWeld.Px*matWeld.Ny + posWeld.Py*matWeld.Oy + posWeld.Pz*matWeld.Ay;
				pdPos.Pz=startPos.Pz + posWeld.Px*matWeld.Nz + posWeld.Py*matWeld.Oz + posWeld.Pz*matWeld.Az;

				posWeld.Rpy.Roll += (endPos.Rpy.Roll - startPos.Rpy.Roll)/Nk;
				posWeld.Rpy.Pitch += (endPos.Rpy.Pitch - startPos.Rpy.Pitch)/Nk;
				posWeld.Rpy.Yaw += (endPos.Rpy.Yaw - startPos.Rpy.Yaw)/Nk;

				pdPos.Rpy.Roll = startPos.Rpy.Roll + posWeld.Rpy.Roll;
				pdPos.Rpy.Pitch = startPos.Rpy.Pitch + posWeld.Rpy.Pitch;
				pdPos.Rpy.Yaw = startPos.Rpy.Yaw + posWeld.Rpy.Yaw;

				RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);
			}

			else if((ik>=(N1+N2+N3)) && (ik<=(N1+N2+N3+N4-1)))			   
			{
				//N4阶段
				pdPos = pdPos;
			}
			else if(ik>=(N1+N2+N3+N4) && (ik<=Nx-1)) 				   
			{
				//N5阶段
				posWeld.Px = -tpa.Azhenfu+(ik+1-(N1+N2+N3+N4))*(tpa.Azhenfu/N5);
				posWeld.Py += fTotalDist/Nk;
				posWeld.Pz = 0;

				pdPos.Px=startPos.Px + posWeld.Px*matWeld.Nx + posWeld.Py*matWeld.Ox + posWeld.Pz*matWeld.Ax;
				pdPos.Py=startPos.Py + posWeld.Px*matWeld.Ny + posWeld.Py*matWeld.Oy + posWeld.Pz*matWeld.Ay;
				pdPos.Pz=startPos.Pz + posWeld.Px*matWeld.Nz + posWeld.Py*matWeld.Oz + posWeld.Pz*matWeld.Az;

				posWeld.Rpy.Roll += (endPos.Rpy.Roll - startPos.Rpy.Roll)/Nk;
				posWeld.Rpy.Pitch += (endPos.Rpy.Pitch - startPos.Rpy.Pitch)/Nk;
				posWeld.Rpy.Yaw += (endPos.Rpy.Yaw - startPos.Rpy.Yaw)/Nk;

				pdPos.Rpy.Roll = startPos.Rpy.Roll + posWeld.Rpy.Roll;
				pdPos.Rpy.Pitch = startPos.Rpy.Pitch + posWeld.Rpy.Pitch;
				pdPos.Rpy.Yaw = startPos.Rpy.Yaw + posWeld.Rpy.Yaw;

				RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);
			}
			nCycleNum++;		
		}
		else
		{
			if (bRemainInit == false)
			{
				this->fRemainDist = sqrt(pow(endPos.Px-pdPos.Px,2)+pow(endPos.Py-pdPos.Py,2)+pow(endPos.Pz-pdPos.Pz,2));
				Nremain = floor(fRemainDist/tpa.WSpeed/robot.dwCycleTime*1E6);
				refPos = pdPos;
				bRemainInit = true;
			}
			pdPos.Px += (endPos.Px -refPos.Px)/Nremain; 
			pdPos.Py += (endPos.Py -refPos.Py)/Nremain; 
			pdPos.Pz += (endPos.Pz -refPos.Pz)/Nremain; 
			pdPos.Rpy.Roll += (endPos.Rpy.Roll -refPos.Rpy.Roll)/Nremain; 
			pdPos.Rpy.Pitch += (endPos.Rpy.Pitch -refPos.Rpy.Pitch)/Nremain; 
			pdPos.Rpy.Yaw += (endPos.Rpy.Yaw -refPos.Rpy.Yaw)/Nremain; 
			RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix);
			nRemainNum++;
			if (nRemainNum == Nremain)
			{
				bDone = true;
			}
		}

		this->wErrorID = invCart2Enc(&pdPos,pRefJoint,pEncoder);
		if (this->wErrorID != MC_NO_ERROR)
		{
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			return;
		}

		for (int i=0; i<ROBOT_AXES_NUM; i++)
		{
			robot.Axes.RobotAxis[i]->lCommandedPosition = pEncoder->Encoder[i];
			//////////////EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);//revise_l
		} 

		if (bDone)
		{
			robot.eLastFb = MOVETRIGLE;
			this->fbState = FBS_DONE;
			this->Execute = MC_FALSE;
			robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  plcopenstate
		}
	}
	else{;}
	
}

/************************************************************************/
/* 关节坐标系示教                                                        */
/************************************************************************/
void MC_JogAxes::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_JogAxes::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

	switch (this->fbState)
	{
	case FBS_IDLE:
		{
			if (this->bExecute)
			{
				StateTrans_Execute();
				if (this->fbState == FBS_ERROR)
				{
					this->bError = MC_TRUE;
					continueSm = MC_TRUE;
				}
				else if (this->fbState == FBS_BUSY)
				{
					this->bBusy = MC_TRUE;
					continueSm = MC_TRUE;
				}
			}
			break;
		}
	case FBS_BUSY:
		{
			DoState_Busy();
			if (this->fbState == FBS_ACTIVE)
			{
				StateTrans_Active();
				this->bActive = MC_TRUE;
				continueSm = MC_TRUE;
			}

			if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_ACTIVE:
		{
			DoState_Active();
			if (this->fbState == FBS_DONE)
			{
				this->bDone = MC_TRUE;
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				continueSm = MC_TRUE;
			}
			else if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
	case FBS_ERROR:
	case FBS_DONE:
		{
			this->bDone = MC_FALSE;           
			this->bCommandAborted = MC_FALSE;
			this->bError = MC_FALSE;
			this->wErrorID = MC_NO_ERROR;
			//  this->fbState = FBS_IDLE;
			break;
		}
	}

	return continueSm;
}

void MC_JogAxes::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */
	if (keySign <= J6M)
	{
		if(fMaxVel > robot.RobotInfo.MaxSpeed)
		{
			this->wErrorID = MC_ERR_VEL_OUT_OF_RANGE;
		}
		else if(fMaxAcc > robot.RobotInfo.MaxSpeedAcc)
		{
			this->wErrorID = MC_ERR_ACC_OUT_OF_RANGE;
		}
	}

// 	if (this->wErrorID != MC_NO_ERROR)  //1218
// 	{
// 		this->fbState = FBS_ERROR;
// 		return;
// 	}

	this->fbState = FBS_BUSY;
}

void MC_JogAxes::DoState_Busy()
{
	switch (robot.ePLCOpenState)
	{
	case GROUP_PLCOPEN_STATE_STANDBY:
// 		//test
// 	 case GROUP_PLCOPEN_STATE_MOVING:
// 		 //test
		{
			MC_T_BOOL  fbActive = false;
			/*省略buffer相关*/
			if (this->wErrorID == MC_NO_ERROR)
			{
				robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
				this->fbState = FBS_ACTIVE;
			}
			else
			{
				this->fbState = FBS_ERROR;

				robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;//1218
				this->fbState = FBS_ACTIVE;//1218
			}
			break;
		}
	case GROUP_PLCOPEN_STATE_STOPPING:
		{
			this->wErrorID = MC_ERR_BUSY_STOPPING;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			break;
		}
	default:
		{
			this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		}
	}
}

void MC_JogAxes::StateTrans_Active()
{
	MC_GROUP_MOVEMENT* pMove = &robot.Movement;
	if (keySign == J1P || keySign == J1M)		nKeyNum = J1;
	if (keySign == J2P || keySign == J2M)		nKeyNum = J2;
	if (keySign == J3P || keySign == J3M)		nKeyNum = J3;
	if (keySign == J4P || keySign == J4M)		nKeyNum = J4;
	if (keySign == J5P || keySign == J5M)		nKeyNum = J5;
	if (keySign == J6P || keySign == J6M)		nKeyNum = J6;
#ifdef _3D_SEWING_
	if (keySign == EX1P || keySign == EX1M)     nKeyNum = SPINDLE;
	if (keySign == EX2P || keySign == EX2M)     nKeyNum = SWAY;
	if (keySign == EX3P || keySign == EX3M)     nKeyNum = EX3;
#endif
	robot.dwCalcIncPerMM = robot.RobotInfo.cnt_per_cycle;

	robot.fVelToInc = (MC_T_REAL)(robot.dwCycleTime / 1E6);
	robot.fAccToInc = (MC_T_REAL)(robot.fVelToInc  * robot.dwCycleTime / 1E6);

	this->fConstantVel = this->fMaxVel;

	pMove->fAxisMaxAcc = this->fMaxAcc*robot.fAccToInc;
	pMove->fAxisConstantVel = this->fConstantVel*robot.fVelToInc;
	pMove->fAxisCurVel = 0;
	pMove->fAxisEndVel = 0;

	pMove->fOrgAcceleration = this->fMaxAcc;
	pMove->fOrgMaxVelocity = this->fMaxVel;

	if (keySign <= J6M)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[nKeyNum]);
		robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition = robot.Axes.RobotAxis[nKeyNum]->lActualPosition;
	}
#ifdef _3D_SEWING_
	else
	{
		MC_DriveGetInputs(robot.Axes.AdditionalAxis[nKeyNum]);
		robot.Axes.AdditionalAxis[nKeyNum]->lCommandedPosition = robot.Axes.AdditionalAxis[nKeyNum]->lActualPosition;
	}
#endif	

	robot.axis5InitEnc = robot.Axes.RobotAxis[J5]->lCommandedPosition;
	robot.axis6InitEnc = robot.Axes.RobotAxis[J6]->lCommandedPosition;
	robot.axisJntPlus = 0;

// 	if(this->wErrorID != MC_NO_ERROR)//1218
// 	{
// 		this->fbState = FBS_ERROR;
// 		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
// 		return;
// 	}
}

void MC_JogAxes::DoState_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;
	//printf("jog axes\n");
	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	if (bKeyState == KEY_DOWN)//按键按下
	{
		pMove->fAxisTargetVel = pMove->fAxisConstantVel;
		if (pMove->fAxisCurVel + pMove->fAxisMaxAcc <= pMove->fAxisTargetVel)//加速段
		{
			pMove->fAxisNextVel = pMove->fAxisCurVel + pMove->fAxisMaxAcc;
		}
		else //匀速段
		{
			pMove->fAxisNextVel = pMove->fAxisTargetVel;
		}
	}
	else//按键弹起
	{
		pMove->fAxisTargetVel = 0;
		if (pMove->fAxisCurVel - pMove->fAxisMaxAcc >= pMove->fAxisTargetVel)//减速段
		{
			pMove->fAxisNextVel = pMove->fAxisCurVel - pMove->fAxisMaxAcc;
		}
		else //最后一个周期
		{
			pMove->fAxisNextVel = pMove->fAxisTargetVel;
		}
	}

	if (keySign <= J6P)//增加角度
	{ 
		if(robot.RobotInfo.motorDir[nKeyNum] == 1)
		{
			robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition += (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.RobotInfo.re_radio[nKeyNum]*robot.dwCalcIncPerMM);
		}
		else
		{
			robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition -= (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.RobotInfo.re_radio[nKeyNum]*robot.dwCalcIncPerMM);
		}
	}
	else if(keySign <= J6M)//减小角度
	{
		if(robot.RobotInfo.motorDir[nKeyNum] == 1)
		{
			robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition -= (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.RobotInfo.re_radio[nKeyNum]*robot.dwCalcIncPerMM);
		}
		else
		{
			robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition += (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.RobotInfo.re_radio[nKeyNum]*robot.dwCalcIncPerMM);
		}
	}
#ifdef _3D_SEWING_
	else if (keySign == EX1P || keySign == EX2P || keySign == EX3P)
		robot.Axes.AdditionalAxis[nKeyNum]->lCommandedPosition += (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.dwCalcIncPerMM);
	else 
		robot.Axes.AdditionalAxis[nKeyNum]->lCommandedPosition -= (MC_T_VELACC)(pMove->fAxisNextVel/360*robot.dwCalcIncPerMM);
#endif
	if (keySign <= J6M)
	{
		jntAngle = (robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition - robot.RobotInfo.enc_home[nKeyNum])*2*PI/(robot.RobotInfo.cnt_per_cycle * robot.RobotInfo.re_radio[nKeyNum]);
		jntAngle = robot.RobotInfo.motorDir[nKeyNum]*jntAngle + robot.RobotInfo.joint_home[nKeyNum];

		int nOut,nKey;
		nKey = keySign;
		nOut = robot.outRangeJnt;
		if (nOut != 0 && abs(nOut - nKey) != ROBOT_AXES_NUM)
		{
			this->wErrorID = KYN_JNT_OUT_RANGE;
			robot.outRangeJnt = OUT_RANGE_JNT(nKey);
			if (this->wErrorID == KYN_JNT_OUT_RANGE)
				robot.errLevel = GENERAL;
			robot.wErrorID = this->wErrorID;
			this->fbState = FBS_ERROR;
			this->Execute = MC_FALSE;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			return;
		}
		//20180123 增加奇异点处理
		if(robot.wErrorID == KYN_SINGULAR && keySign != J5M & keySign != J5P)
		{
			this->wErrorID = KYN_SINGULAR;
		    this->fbState = FBS_ERROR;
			this->Execute = MC_FALSE;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			return;
		}
		else if(robot.wErrorID == KYN_SINGULAR && (keySign == J5M || keySign == J5P))
		{
			this->wErrorID == MC_NO_ERROR;
			robot.wErrorID = this->wErrorID;
		}

		EC_SET_FRM_DWORD(robot.Axes.RobotAxis[nKeyNum]->plPdTargetPosition,robot.Axes.RobotAxis[nKeyNum]->lCommandedPosition);
		robot.axisJntPlus += pMove->fAxisNextVel;//robot.axisJntPlus += pMove->fAxisNextVel;
		//耦合比
		if (keySign == J4P)
		{
			robot.Axes.RobotAxis[J5]->lCommandedPosition = robot.axis5InitEnc - (MC_T_VELACC)(robot.RobotInfo.cpRatio[R45] * robot.RobotInfo.motorDir[J5] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J5] * robot.dwCalcIncPerMM);
			robot.Axes.RobotAxis[J6]->lCommandedPosition = robot.axis6InitEnc - (MC_T_VELACC)(robot.RobotInfo.cpRatio[R46] * robot.RobotInfo.motorDir[J6] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J6] * robot.dwCalcIncPerMM);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J5]->plPdTargetPosition,robot.Axes.RobotAxis[J5]->lCommandedPosition);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J6]->plPdTargetPosition,robot.Axes.RobotAxis[J6]->lCommandedPosition);
		}

		if (keySign == J4M)
		{
			robot.Axes.RobotAxis[J5]->lCommandedPosition = robot.axis5InitEnc + (MC_T_VELACC)(robot.RobotInfo.cpRatio[R45] * robot.RobotInfo.motorDir[J5] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J5] * robot.dwCalcIncPerMM);
			robot.Axes.RobotAxis[J6]->lCommandedPosition = robot.axis6InitEnc + (MC_T_VELACC)(robot.RobotInfo.cpRatio[R46] * robot.RobotInfo.motorDir[J6] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J6] * robot.dwCalcIncPerMM);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J5]->plPdTargetPosition,robot.Axes.RobotAxis[J5]->lCommandedPosition);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J6]->plPdTargetPosition,robot.Axes.RobotAxis[J6]->lCommandedPosition);
		}

		if (keySign == J5P)
		{
			robot.Axes.RobotAxis[J6]->lCommandedPosition = robot.axis6InitEnc - (MC_T_VELACC)(robot.RobotInfo.cpRatio[R56] * robot.RobotInfo.motorDir[J6] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J6] * robot.dwCalcIncPerMM);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J6]->plPdTargetPosition,robot.Axes.RobotAxis[J6]->lCommandedPosition);
		}

		if (keySign == J5M)
		{
			robot.Axes.RobotAxis[J6]->lCommandedPosition = robot.axis6InitEnc + (MC_T_VELACC)(robot.RobotInfo.cpRatio[R56] * robot.RobotInfo.motorDir[J6] * robot.axisJntPlus/360 * robot.RobotInfo.re_radio[J6] * robot.dwCalcIncPerMM);
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[J6]->plPdTargetPosition,robot.Axes.RobotAxis[J6]->lCommandedPosition);
		}
	}
#ifdef _3D_SEWING_
	else //if (nKeyNum == SPINDLE || (nKeyNum == SWAY && robot.Axes.AdditionalAxis[SWAY]->lCommandedPosition >= robot.swayEncMinus &&robot.Axes.AdditionalAxis[SWAY]->lCommandedPosition <= robot.swayEncPlus))
	{
		EC_SET_FRM_DWORD(robot.Axes.AdditionalAxis[nKeyNum]->plPdTargetPosition,robot.Axes.AdditionalAxis[nKeyNum]->lCommandedPosition);
	}
#endif

	pMove->fAxisCurVel = pMove->fAxisNextVel;

	MC_T_BOOL bDone = MC_FALSE;
	if (bKeyState == KEY_UP && pMove->fAxisCurVel == 0)//严格等于0？
	{
		bDone = MC_TRUE;
	}

	if (bDone)
	{
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  单个轴的plcopen状态？
		this->Execute = MC_FALSE;
		robot.eLastFb = JOGAXES;
	}
}

/************************************************************************/
/* 笛卡尔空间示教                                                        */
/************************************************************************/
void MC_JogCart::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL MC_JogCart::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

	switch (this->fbState)
	{
	case FBS_IDLE:
		{
			if (this->bExecute)
			{
				StateTrans_Execute();
				if (this->fbState == FBS_ERROR)
				{
					this->bError = MC_TRUE;
					continueSm = MC_TRUE;
				}
				else if (this->fbState == FBS_BUSY)
				{
					this->bBusy = MC_TRUE;
					continueSm = MC_TRUE;
				}
			}
			break;
		}
	case FBS_BUSY:
		{
			DoState_Busy();
			if (this->fbState == FBS_ACTIVE)
			{
				StateTrans_Active();
				this->bActive = MC_TRUE;
				continueSm = MC_TRUE;
			}

			if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_ACTIVE:
		{
			DoState_Active();
			if (this->fbState == FBS_DONE)
			{
				this->bDone = MC_TRUE;
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				continueSm = MC_TRUE;
			}
			else if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
	case FBS_ERROR:
	case FBS_DONE:
		{
			this->bDone = MC_FALSE;           
			this->bCommandAborted = MC_FALSE;
			this->bError = MC_FALSE;
			this->wErrorID = MC_NO_ERROR;
			//  this->fbState = FBS_IDLE;
			break;
		}
	}

	return continueSm;
}

void MC_JogCart::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */

	if(fMaxVel > robot.RobotInfo.MaxSpeed)
	{
		this->wErrorID = MC_ERR_VEL_OUT_OF_RANGE;
	}
	else if(fMaxAcc > robot.RobotInfo.MaxSpeedAcc)
	{
		this->wErrorID = MC_ERR_ACC_OUT_OF_RANGE;
	}

	if (this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	this->fbState = FBS_BUSY;
}

void MC_JogCart::DoState_Busy()
{
	switch (robot.ePLCOpenState)
	{
	case GROUP_PLCOPEN_STATE_STANDBY:
// 		//test
// 		 case GROUP_PLCOPEN_STATE_MOVING:
// 		//test
		{
			MC_T_BOOL  fbActive = false;
			/*省略buffer相关*/
			if (this->wErrorID == MC_NO_ERROR)
			{
				robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
				this->fbState = FBS_ACTIVE;
			}
			else
			{
				this->fbState = FBS_ERROR;
				//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			}
			break;
		}
	case GROUP_PLCOPEN_STATE_STOPPING:
		{
			this->wErrorID = MC_ERR_BUSY_STOPPING;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			break;
		}
	default:
		{
			this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		}
	}
}

void MC_JogCart::StateTrans_Active()
{
	MC_GROUP_MOVEMENT* pMove = &robot.Movement;

// 	//test
// 	this->fMaxVel = 2;
// 	this->fMaxAcc = 6;
// 	this->bKeyState = KEY_DOWN;
// 	this->keySign = RZM;
// 	//test

	robot.dwCalcIncPerMM = robot.RobotInfo.cnt_per_cycle;

	robot.fVelToInc = (MC_T_REAL)(robot.dwCycleTime / 1E6);
	robot.fAccToInc = (MC_T_REAL)(robot.fVelToInc  * robot.dwCycleTime / 1E6);

	this->fConstantVel = this->fMaxVel;

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		MC_DriveGetInputs(robot.Axes.RobotAxis[i]);
		robot.Axes.RobotAxis[i]->lCommandedPosition = robot.Axes.RobotAxis[i]->lActualPosition;
		pEncoder->Encoder[i] = robot.Axes.RobotAxis[i]->lActualPosition;
	}

	Encoder2DHAngle(pEncoder->Encoder,pRefJoint);
	startPos = Forward(pRefJoint,&robot.RobotInfo.RobotLink);
	tempj = *pRefJoint;
	pdPos = startPos;

	pMove->fCartMaxAcc = this->fMaxAcc*robot.fAccToInc;
	pMove->fCartConstantVel = this->fConstantVel*robot.fVelToInc;
	pMove->fCartCurVel = 0;
	pMove->fCartEndVel = 0;

	pMove->fOrgAcceleration = this->fMaxAcc;
	pMove->fOrgMaxVelocity = this->fMaxVel;

	if(this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

}

void MC_JogCart::DoState_Active()
{
	MC_GROUP_MOVEMENT*  pMove = &robot.Movement;

	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	if (bKeyState == KEY_DOWN)//按键按下
	{
		pMove->fCartTargetVel = pMove->fCartConstantVel;
		if (pMove->fCartCurVel + pMove->fCartMaxAcc <= pMove->fCartTargetVel)//加速段
		{
			pMove->fCartNextVel = pMove->fCartCurVel + pMove->fCartMaxAcc;
		}
		else //匀速段
		{
			pMove->fCartNextVel = pMove->fCartTargetVel;
		}
	}
	else//按键弹起
	{
		pMove->fCartTargetVel = pMove->fCartEndVel;
		if (pMove->fCartCurVel - pMove->fCartMaxAcc >= pMove->fCartTargetVel)//减速段
		{
			pMove->fCartNextVel = pMove->fCartCurVel - pMove->fCartMaxAcc;
		}
		else //最后一个周期
		{
			pMove->fCartNextVel = pMove->fCartTargetVel;
		}
	}

	if(robot.eCurWs == eWorldSpace)
	{
		switch (this->keySign)
		{
		case XP:{ pdPos.Px += pMove->fCartNextVel; break; }
		case XM:{ pdPos.Px -= pMove->fCartNextVel; break; }
		case YP:{ pdPos.Py += pMove->fCartNextVel; break; }
		case YM:{ pdPos.Py -= pMove->fCartNextVel; break; }
		case ZP:{ pdPos.Pz += pMove->fCartNextVel; break; }
		case ZM:{ pdPos.Pz -= pMove->fCartNextVel; break; }
		case RXP:{ pdPos.Rpy.Roll += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }  //角速度与线速度统一用fCartNextVel，具体数值由示教器给定
		case RXM:{ pdPos.Rpy.Roll -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RYP:{ pdPos.Rpy.Pitch += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RYM:{ pdPos.Rpy.Pitch -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RZP:{ pdPos.Rpy.Yaw += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RZM:{ pdPos.Rpy.Yaw -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		}
	}
	else if(robot.eCurWs == eTOOLSPACE)
	{	
		switch (this->keySign)
		{
		case XP:{ pdPos.Px += pdPos.Rmatrix.Nx*pMove->fCartNextVel; pdPos.Py += pdPos.Rmatrix.Ny*pMove->fCartNextVel; pdPos.Pz += pdPos.Rmatrix.Nz*pMove->fCartNextVel;break;}
		case XM:{ pdPos.Px -= pdPos.Rmatrix.Nx*pMove->fCartNextVel; pdPos.Py -= pdPos.Rmatrix.Ny*pMove->fCartNextVel; pdPos.Pz -= pdPos.Rmatrix.Nz*pMove->fCartNextVel;break;}
		case YP:{ pdPos.Px += pdPos.Rmatrix.Ox*pMove->fCartNextVel; pdPos.Py += pdPos.Rmatrix.Oy*pMove->fCartNextVel; pdPos.Pz += pdPos.Rmatrix.Oz*pMove->fCartNextVel;break;}
		case YM:{ pdPos.Px -= pdPos.Rmatrix.Ox*pMove->fCartNextVel; pdPos.Py -= pdPos.Rmatrix.Oy*pMove->fCartNextVel; pdPos.Pz -= pdPos.Rmatrix.Oz*pMove->fCartNextVel;break;}
		case ZP:{ pdPos.Px += pdPos.Rmatrix.Ax*pMove->fCartNextVel; pdPos.Py += pdPos.Rmatrix.Ay*pMove->fCartNextVel; pdPos.Pz += pdPos.Rmatrix.Az*pMove->fCartNextVel;break;}
		case ZM:{ pdPos.Px -= pdPos.Rmatrix.Ax*pMove->fCartNextVel; pdPos.Py -= pdPos.Rmatrix.Ay*pMove->fCartNextVel; pdPos.Pz -= pdPos.Rmatrix.Az*pMove->fCartNextVel;break;}
		case RXP:{ pdPos.Rpy.Roll += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }  //角速度与线速度统一用fCartNextVel，具体数值由示教器给定
		case RXM:{ pdPos.Rpy.Roll -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RYP:{ pdPos.Rpy.Pitch += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RYM:{ pdPos.Rpy.Pitch -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RZP:{ pdPos.Rpy.Yaw += pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		case RZM:{ pdPos.Rpy.Yaw -= pMove->fCartNextVel*PI/180; RpyToMat(&pdPos.Rpy,&pdPos.Rmatrix); break; }
		}
	}
	//printf("before jog:  %.12f\n",pRefJoint->Theta4);
	this->wErrorID = invCart2Enc(&pdPos,pRefJoint,pEncoder);
	if (this->wErrorID != MC_NO_ERROR)
	{
		printf("error: %d\n",wErrorID);
		//if (this->wErrorID == KYN_JNT_OUT_RANGE)
			robot.errLevel = GENERAL;
		robot.wErrorID = this->wErrorID;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		this->Execute = MC_FALSE;
		return;
	}

	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		robot.Axes.RobotAxis[i]->lCommandedPosition = pEncoder->Encoder[i];
// 		if (abs(robot.Axes.RobotAxis[i]->lCommandedPosition - robot.Axes.RobotAxis[i]->lActualPosition)>200)//最大速度100度每秒，换算为编码器值
// 		{
// 			printf("Vel overRun!\n");
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[0] - robot.Axes.RobotAxis[0]->lActualPosition));
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[1] - robot.Axes.RobotAxis[1]->lActualPosition));
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[2] - robot.Axes.RobotAxis[2]->lActualPosition));
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[3] - robot.Axes.RobotAxis[3]->lActualPosition));
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[4] - robot.Axes.RobotAxis[4]->lActualPosition));
// 			printf("dencoder: %d\n",abs(pEncoder->Encoder[5] - robot.Axes.RobotAxis[5]->lActualPosition));
// 			this->wErrorID = SALTUS_STEP;
// 			this->fbState = FBS_ERROR;
// 			return;
// 		}
// 		else
// 		{
			EC_SET_FRM_DWORD(robot.Axes.RobotAxis[i]->plPdTargetPosition,robot.Axes.RobotAxis[i]->lCommandedPosition);
			//robot.Axes.RobotAxis[i]->lActualPosition = robot.Axes.RobotAxis[i]->lCommandedPosition;//直接赋值还是在函数开始时读取实际值？
		//}//revise_l

	}
	//printf("after jog : %.12f\n",pRefJoint->Theta4);
	//printf("actpos, %.3f, %.3f, %.3f, , %.3f, %.3f, %.3f%\n",robot.actPos.Rpy.Roll,robot.actPos.Rpy.Pitch,robot.actPos.Rpy.Yaw,pdPos.Rpy.Roll,pdPos.Rpy.Pitch,pdPos.Rpy.Yaw);
	//fprintf(robot.robotFile,"%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", pdPos.Px,pdPos.Py,pdPos.Pz,pdPos.Rpy.Pitch,pdPos.Rpy.Roll,pdPos.Rpy.Yaw);
	pMove->fCartCurVel = pMove->fCartNextVel;
	for (int i=0; i<ROBOT_AXES_NUM; i++)
	{
		robot.Axes.RobotAxis[i]->lActualPosition = pEncoder->Encoder[i];
	}

	MC_T_BOOL bDone = MC_FALSE;
	if (bKeyState == KEY_UP && pMove->fCartCurVel == 0)//严格等于0？
	{
		bDone = MC_TRUE;
	}

	if (bDone)
	{
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  单个轴的plcopen状态？
		this->Execute = MC_FALSE;
		robot.eLastFb = JOGCART;
	}
}


/************************************************************************/
/* Wait                                                                */
/************************************************************************/
void WaitTime::OnCycle()
{
	while(StateMachine());
}

MC_T_BOOL WaitTime::StateMachine()
{
	MC_T_BOOL continueSm = false; // false if state machine should terminate, true to keep running.

	switch (this->fbState)
	{
	case FBS_IDLE:
		{
			if (this->bExecute)
			{
				StateTrans_Execute();
				if (this->fbState == FBS_ERROR)
				{
					this->bError = MC_TRUE;
					continueSm = MC_TRUE;
				}
				else if (this->fbState == FBS_BUSY)
				{
					this->bBusy = MC_TRUE;
					continueSm = MC_TRUE;
				}
			}
			break;
		}
	case FBS_BUSY:
		{
			DoState_Busy();
			if (this->fbState == FBS_ACTIVE)
			{
				StateTrans_Active();
				this->bActive = MC_TRUE;
				continueSm = MC_TRUE;
			}

			if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE; // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_ACTIVE:
		{
			DoState_Active();
			if (this->fbState == FBS_DONE)
			{
				this->bDone = MC_TRUE;
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				continueSm = MC_TRUE;
			}
			else if (this->fbState == FBS_ERROR)
			{
				this->bBusy = MC_FALSE;  // Busy is ... RESET when one of the outputs Done, Aborted or Error is set.
				this->bActive = MC_FALSE;
				this->bError = MC_TRUE;
				continueSm = MC_TRUE;
			}
			break;
		}
	case FBS_COMMAND_ABORTED: // External state change. Triggered by foreign MCFB.
	case FBS_ERROR:
	case FBS_DONE:
		{
			this->bDone = MC_FALSE;           
			this->bCommandAborted = MC_FALSE;
			this->bError = MC_FALSE;
			this->wErrorID = MC_NO_ERROR;
			//  this->fbState = FBS_IDLE;
			break;
		}
	}

	return continueSm;
}

void WaitTime::StateTrans_Execute()
{
	/* check if move(input) parameters are valid */

	if( time <0 )
	{
		this->wErrorID = TIME_LESS_THEN_ZERO;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	this->fbState = FBS_BUSY;
}

void WaitTime::DoState_Busy()
{
	switch (robot.ePLCOpenState)
	{
	case GROUP_PLCOPEN_STATE_STANDBY:
		{
			MC_T_BOOL  fbActive = false;
			/*省略buffer相关*/
			if (this->wErrorID == MC_NO_ERROR)
			{
				robot.ePLCOpenState = GROUP_PLCOPEN_STATE_MOVING;
				this->fbState = FBS_ACTIVE;
			}
			else
			{
				this->fbState = FBS_ERROR;
				//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			}
			break;
		}
	case GROUP_PLCOPEN_STATE_STOPPING:
		{
			this->wErrorID = MC_ERR_BUSY_STOPPING;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
			break;
		}
	default:
		{
			this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
			this->fbState = FBS_ERROR;
			//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;

		}
	}
}

void WaitTime::StateTrans_Active()
{
	printf("StateTrans_Active \n");
	count = floor(time/robot.dwCycleTime*1E6);

	if(this->wErrorID != MC_NO_ERROR)
	{
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}
}

void WaitTime::DoState_Active()
{
	//printf("DoState_Active \n");
	//printf("posy:%.6f\n",robot.laserTrack.fTrueValue);
	if(robot.ePLCOpenState != GROUP_PLCOPEN_STATE_MOVING && robot.ePLCOpenState != GROUP_PLCOPEN_STATE_STOPPING & robot.ePLCOpenState != GROUP_PLCOPEN_STATE_PAUSE) 
	{
		this->wErrorID = MC_ERR_INVALID_PLCOPEN_STATE;
		this->fbState = FBS_ERROR;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_ERROR_STOP;
		return;
	}

	if (robot.ePLCOpenState == GROUP_PLCOPEN_STATE_STOPPING)
	{
		this->fbState = FBS_DONE;
		//robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;//revise_l  单个轴的plcopen状态？
		this->Execute = MC_FALSE;
		return;
	}

    if (count)
    {
		count--;
		//printf("count = %d\n",count);
    }
	else
	{
		bDone = MC_TRUE;
	}
	
	if (bDone)
	{
		robot.eLastFb = WAIT;
		this->fbState = FBS_DONE;
		robot.ePLCOpenState = GROUP_PLCOPEN_STATE_STANDBY;
		this->Execute = MC_FALSE;
		printf("wait DONE! \n");
	}
}


void GetNewInst()
{
	robot.bExecuteNew = true;
	if (!robot.bFinished)
		robot.nReadyForPush++;	
}
#ifdef _3D_SEWING_
void SewStart()
{
	if(robot.sewPara.fGap >10 || robot.sewPara.fGap<=0)
		robot.sewPara.fGap = 5.0;//默认针距5毫米
	float fSpindleDist;
	fSpindleDist = (float)robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Axis->oMove.lDoneDist/131072 - robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Axis->oMove.lDoneDist/131072;
	if (robot.sewCalcPara.fSewSpeed>0.0)
	{
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Execute = true;      //主轴持续反转
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Acceleration =  100000;         
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Deceleration =  100000;         
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Velocity     =  robot.sewCalcPara.fSewSpeed/robot.sewPara.fGap;      
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Direction		= MC_DIR_NEGATIVE;
		//robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Jerk         =  10000000;
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.ContinuousUpdate = true;
		if(fSpindleDist <= robot.sewCalcPara.fSyncOut && fSpindleDist >= robot.sewCalcPara.fSyncIn && robot.nSway%2 == 0) //滑块匹配运动
		{
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Execute = true;
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Acceleration =  5000000;         
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Deceleration =  5000000;         
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Velocity     = robot.sewCalcPara.fSewSpeed/4.0;
			//robot.S_aAxisList[7].pFb->SwayMoveVelocity.Jerk         =  100000000; 
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Direction		= MC_DIR_POSITIVE;
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.ContinuousUpdate = true;
		}
		else if (fSpindleDist <= (robot.sewCalcPara.fSyncOut+0.03) && fSpindleDist > robot.sewCalcPara.fSyncOut && robot.nSway%2 == 0) //匹配运动结束，滑块停止
		{
			robot.S_aAxisList[7].pFb->SwayMoveVelocity.Execute = false;
			robot.S_aAxisList[7].pFb->SwayStop.Execute = true;
			robot.S_aAxisList[7].pFb->SwayStop.Deceleration = 5000000;
			robot.S_aAxisList[7].pFb->SwayStop.Jerk = 100000000;
		}
		else if(fSpindleDist > (robot.sewCalcPara.fSyncOut+0.03) && fSpindleDist <= 1 && robot.nSway%2 == 1)   //滑块快速返回
		{
			if (robot.sewCalcPara.bswayBack)   //滑块确认停止，可以返回
			{
				robot.sewCalcPara.bswayBack = false;
				robot.sewCalcPara.swayHomeDist = (float)(robot.sewCalcPara.swayHookEnc - robot.Axes.AdditionalAxis[1]->lCommandedPosition)/robot.RobotInfo.cnt_per_cycle;       
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = true;
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance     =  robot.sewCalcPara.swayHomeDist;
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Acceleration =  5000000;         
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Deceleration =  5000000;         
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Velocity     =  fabs(robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance/(robot.sewPara.fGap/robot.sewCalcPara.fSewSpeed*0.16))*2.0;        /* mm/sec   */
				robot.S_aAxisList[7].pFb->SwayMoveRelative.Jerk         =  100000000; 
			}
		}

		robot.S_aAxisList[6].pFb->SpindleStop.OnCycle();
		robot.S_aAxisList[7].pFb->SwayStop.OnCycle();

		if (robot.S_aAxisList[7].pFb->SwayStop.Done)
		{
			robot.S_aAxisList[7].pFb->SwayStop.Execute = false;
			robot.nSway++;
			robot.sewCalcPara.bswayBack = true;
		}
		if (robot.S_aAxisList[7].pFb->SwayMoveRelative.Done)
		{
			robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = false;
			robot.nSway++;
		}
	}
}
bool SewHome()
{
	bool bDone = false;
	if(!robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute)
	{
		//printf("1\n");
		robot.sewCalcPara.bspindleHome = false;
		robot.sewCalcPara.bswayHome = false;
		robot.sewCalcPara.swayHomeDist = (float)(robot.sewCalcPara.swayHookEnc - robot.Axes.AdditionalAxis[1]->lCommandedPosition)/robot.RobotInfo.cnt_per_cycle;
		robot.sewCalcPara.spindleHomeDist = -(float)((robot.Axes.AdditionalAxis[0]->lCommandedPosition - robot.sewCalcPara.spindleHomeEnc)%robot.RobotInfo.cnt_per_cycle)/robot.RobotInfo.cnt_per_cycle;
		if (robot.sewCalcPara.spindleHomeDist < -0.5) //主轴回零行程不超过半圈
			robot.sewCalcPara.spindleHomeDist += 1;
		else if(robot.sewCalcPara.spindleHomeDist>0.5)
			robot.sewCalcPara.spindleHomeDist -= 1;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute		= true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Acceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Deceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Velocity     =  1;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance     =  robot.sewCalcPara.swayHomeDist;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Jerk         =  25000;  

		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute	   = true;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Acceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Deceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Velocity     =  1;  
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Distance	   =  robot.sewCalcPara.spindleHomeDist;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Jerk         =  50; 
	}
	if (robot.sewCalcPara.bspindleHome && robot.sewCalcPara.bswayHome)
	{
		bDone = true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = false;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute = false;
	}
	if (robot.S_aAxisList[6].pFb->SpindleMoveRelative.Done)
	{
		robot.sewCalcPara.bspindleHome = true;
	}
	if (robot.S_aAxisList[7].pFb->SwayMoveRelative.Done)
	{
		robot.sewCalcPara.bswayHome = true;
	}
	return bDone;
}
bool SewHomeAll()
{
	bool bDone = false;
	if(!robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute)
	{
		robot.sewCalcPara.bspindleHome = false;
		robot.sewCalcPara.bswayHome = false;
		robot.sewCalcPara.swayHomeDist = (float)(robot.sewCalcPara.swayHookEnc - robot.Axes.AdditionalAxis[1]->lCommandedPosition)/robot.RobotInfo.cnt_per_cycle;
		robot.sewCalcPara.spindleHomeDist = -(float)((robot.Axes.AdditionalAxis[0]->lCommandedPosition - robot.sewCalcPara.spindleHomeEnc)%robot.RobotInfo.cnt_per_cycle)/robot.RobotInfo.cnt_per_cycle;
#ifdef _WORKS_
		robot.sewCalcPara.rotateHomeDist = -(float)(robot.Axes.AdditionalAxis[2]->lCommandedPosition - robot.sewCalcPara.rotateHomeEnc)/robot.RobotInfo.cnt_per_cycle/24.0;
		if(robot.sewCalcPara.rotateHomeDist > 1)
			robot.sewCalcPara.rotateHomeDist = robot.sewCalcPara.rotateHomeDist - floor(robot.sewCalcPara.rotateHomeDist);
		else if(robot.sewCalcPara.rotateHomeDist < -1)
			robot.sewCalcPara.rotateHomeDist = robot.sewCalcPara.rotateHomeDist + floor(fabs(robot.sewCalcPara.rotateHomeDist));

		if (robot.sewCalcPara.rotateHomeDist < -0.5) 
			robot.sewCalcPara.rotateHomeDist += 1;
		else if(robot.sewCalcPara.rotateHomeDist > 0.5)
			robot.sewCalcPara.rotateHomeDist -= 1;

		robot.S_aAxisList[8].pFb->WorksMoveRelative.Execute		 = true;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Acceleration =  3;         
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Deceleration =  3;         
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Velocity     =  6;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Distance	 =  robot.sewCalcPara.rotateHomeDist*24.0;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Jerk         =  2500; 
#endif
		if (robot.sewCalcPara.spindleHomeDist < -0.5) //主轴回零行程不超过半圈
			robot.sewCalcPara.spindleHomeDist += 1;
		else if(robot.sewCalcPara.spindleHomeDist>0.5)
			robot.sewCalcPara.spindleHomeDist -= 1;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute		= true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Acceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Deceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Velocity     =  1;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance     =  robot.sewCalcPara.swayHomeDist;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Jerk         =  25000;  

		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute	   = true;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Acceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Deceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Velocity     =  1;  
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Distance	   =  robot.sewCalcPara.spindleHomeDist;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Jerk         =  50; 
	}
#ifdef _WORKS_
	if (robot.sewCalcPara.bspindleHome && robot.sewCalcPara.bswayHome && robot.sewCalcPara.brotateHome)
#else
	if (robot.sewCalcPara.bspindleHome && robot.sewCalcPara.bswayHome)
#endif
	{
		printf("Sew Home!\n");
		robot.bSewHome = false;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = false;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute = false;
		robot.bIoGet = true;
		robot.bReverseIo = false;
		robot.bCanBeCalled = true;
		bDone = true;

#ifdef _WORKS_
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Execute = false;
		robot.bReadyForRotate = false;
		robot.bSafeForRotate = false;
#endif
	}
	if (robot.S_aAxisList[6].pFb->SpindleMoveRelative.Done)
	{
		robot.sewCalcPara.bspindleHome = true;
		//printf("spindleHome!\n");
	}
	if (robot.S_aAxisList[7].pFb->SwayMoveRelative.Done)
	{
		robot.sewCalcPara.bswayHome = true;
		//printf("swayHome!\n");
	}
#ifdef _WORKS_
	if (robot.S_aAxisList[8].pFb->WorksMoveRelative.Done)
	{
		robot.sewCalcPara.brotateHome = true;
	}
#endif
	return bDone;
}
bool SewStop()
{
	bool bDone = false;
	if (robot.S_aAxisList[6].pFb->SpindleStop.Execute == false)
	{
		robot.bSewStart = false;
		robot.sewCalcPara.bswayStop = false;
		robot.sewCalcPara.bspindleStop = false;
		robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Execute = false;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute = false;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute    = false;
		robot.S_aAxisList[7].pFb->SwayMoveVelocity.Execute	  = false;
		robot.S_aAxisList[6].pFb->SpindleStop.Execute = true;
		robot.S_aAxisList[6].pFb->SpindleStop.Deceleration = 10000;
		robot.S_aAxisList[6].pFb->SpindleStop.Jerk = 500000;
		robot.S_aAxisList[7].pFb->SwayStop.Execute = true;
		robot.S_aAxisList[7].pFb->SwayStop.Deceleration = 500000;
		robot.S_aAxisList[7].pFb->SwayStop.Jerk = 12000000;
	}
	if (robot.S_aAxisList[6].pFb->SpindleStop.Done)
	{
		robot.sewCalcPara.bspindleStop = true;
	}
	if (robot.S_aAxisList[7].pFb->SwayStop.Done)
	{
		robot.sewCalcPara.bswayStop = true;
	}
	if (robot.sewCalcPara.bspindleStop && robot.sewCalcPara.bswayStop)
	{
		robot.S_aAxisList[6].pFb->SpindleStop.Execute = false;
		robot.S_aAxisList[7].pFb->SwayStop.Execute = false;
		bDone = true;
	}
	return bDone;
}
bool DropNeedle()
{
	bool bDone = false;
	if (!robot.sewCalcPara.bFirstCalcDrop)
	{
		robot.sewCalcPara.bFirstCalcDrop = true;
		printf("Needle Dropping!\n");
		robot.sewCalcPara.bSliderDrop1 = false;
		robot.sewCalcPara.bSliderDrop2 = false;
		robot.sewCalcPara.bSpindleDrop  = false;
		robot.sewCalcPara.fNeedleDropDist = 1;
		robot.sewCalcPara.fSliderDropDist = -0.5;

		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute		= true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Acceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Deceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Velocity     =  1;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance     =  robot.sewCalcPara.fSliderDropDist;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Jerk         =  25000;  

	}

	if (robot.S_aAxisList[7].pFb->SwayMoveRelative.Done && robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance < 0 && !robot.sewCalcPara.bSliderDrop1)
	{
		robot.sewCalcPara.bSliderDrop1 = true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute         = false;
		printf("bSliderDrop1 !\n");
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute	   = true;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Acceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Deceleration =  10;         
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Velocity     =  1;  
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Distance	   =  robot.sewCalcPara.fNeedleDropDist;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Jerk         =  50; 
	}

	if (robot.S_aAxisList[6].pFb->SpindleMoveRelative.Done && !robot.sewCalcPara.bSpindleDrop)
	{
		robot.sewCalcPara.bSpindleDrop = true;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute   = false;
		printf("bSpindleDrop !\n");
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute		= true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Acceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Deceleration =  5000;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Velocity     =  1;         
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance     =  -robot.sewCalcPara.fSliderDropDist;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Jerk         =  25000;  
	}

	if (robot.S_aAxisList[7].pFb->SwayMoveRelative.Done && robot.S_aAxisList[7].pFb->SwayMoveRelative.Distance > 0 && !robot.sewCalcPara.bSliderDrop2 && robot.sewCalcPara.bSpindleDrop)
	{
		robot.sewCalcPara.bSliderDrop2 = true;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = false;
		printf("bSliderDrop2 !\n");
	}

	if (robot.sewCalcPara.bSliderDrop2)
	{
		printf("Needle Dropped!\n");
		bDone = true;
		robot.bDropNeedle = false;
		robot.S_aAxisList[7].pFb->SwayMoveRelative.Execute = false;
		robot.S_aAxisList[6].pFb->SpindleMoveRelative.Execute = false;
		robot.sewCalcPara.bFirstCalcDrop = false;
	}
	return bDone;
}
bool RotateWorks()
{
	bool bDone = false;
	if (robot.S_aAxisList[8].pFb->WorksMoveRelative.Execute == false && robot.bReadyForRotate/* && robot.bSafeForRotate*/)
	{
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Execute		 = true;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Acceleration =  3;         
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Deceleration =  3;         
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Velocity     =  6;  
		if (robot.S_aAxisList[8].pFb->WorksMoveRelative.Distance >= 0)//-180 ~ 180
			robot.S_aAxisList[8].pFb->WorksMoveRelative.Distance = -12;
		else
			robot.S_aAxisList[8].pFb->WorksMoveRelative.Distance = 12;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Jerk         =  2500;  
	}
	if (robot.S_aAxisList[8].pFb->WorksMoveRelative.Done)
	{
		printf("rotate done!\n");
		bDone = true;
		robot.bRotateWorks = false;
		robot.bReadyForRotate = false;
		robot.nWorksNum++;
		//robot.bSafeForRotate = false;
		robot.S_aAxisList[8].pFb->WorksMoveRelative.Execute = false;
	}
	return bDone;
}
void AutoSew()
{
	if (robot.bSewStart)//开始缝纫
	{
		robot.sewCalcPara.fSewSpeed = robot.Movement.fCartNextVel/robot.dwCycleTime*1E6;
		SewStart();
	}
	if (robot.bSewStop || (robot.bHardStop && !robot.bHardSewStop))//停止缝纫
	{
		bool bDone;
		bDone = SewStop();
		if (bDone)
		{
			printf("Sew Stop!\n");
			robot.bSewStop = false;
			robot.bHardSewStop = false;
			robot.S_aAxisList[6].pFb->SpindleStop.Execute = false;
			robot.S_aAxisList[7].pFb->SwayStop.Execute = false;
			robot.nSway = 0;
			if(!robot.bHardStop)
			{
				GetNewInst();	
			}
			else
				robot.bHardSewStop = true;
		}

		robot.S_aAxisList[6].pFb->SpindleStop.OnCycle();
		robot.S_aAxisList[7].pFb->SwayStop.OnCycle();
	}

	if (robot.bSewHome)
	{
		if (SewHome())
		{
			printf("Sew Home!\n");
			robot.bSewHome = false;
			GetNewInst();	
		}
	}

	if (robot.bDropNeedle)
	{
		if (DropNeedle())
		{
			GetNewInst();	
		}
	}
#ifdef _WORKS_
	if (robot.bRotateWorks)
	{
		if (RotateWorks())
		{
			GetNewInst();	
		}
	}
#endif
	robot.S_aAxisList[6].pFb->SpindleMoveVelocity.OnCycle();
	robot.S_aAxisList[7].pFb->SwayMoveVelocity.OnCycle();
	robot.S_aAxisList[6].pFb->SpindleMoveRelative.OnCycle();
	robot.S_aAxisList[7].pFb->SwayMoveRelative.OnCycle();
	robot.S_aAxisList[6].pFb->SpindlePower.OnCycle();
	robot.S_aAxisList[7].pFb->SwayPower.OnCycle();
#ifdef _WORKS_
	robot.S_aAxisList[8].pFb->WorksMoveRelative.OnCycle();
	robot.S_aAxisList[8].pFb->WorksPower.OnCycle();
#endif
}
void ManualSew()
{
	if (robot.bSewHome)//一键回零
	{
		bool bSewHomeAll;
		bSewHomeAll = SewHomeAll();
	}
	if (robot.bSewTest)//缝纫测试
	{
		if(!robot.bHomeDone) //缝纫测试之前先回零
		{
			if (SewHome())  
			{
				printf("Sew Home!\n");
				robot.bHomeDone = true;
			}
		}
		else if(!robot.bSewStop && robot.bHomeDone)//开始测试
		{
			robot.sewCalcPara.fSewSpeed = robot.sewPara.fRobSpeed;
			if(robot.sewCalcPara.fSewSpeed > 51 || robot.sewCalcPara.fSewSpeed <= 0)
				robot.sewCalcPara.fSewSpeed = 5.0;//默认速度5mm/s
			SewStart();					
		}

		if (robot.bSewStop)//停止测试
		{
			if (SewStop())
			{
				printf("Sew Stop!\n");
				robot.nSway = 0;
				robot.bSewStop = false;
				robot.bSewTest = false;
				robot.bHomeDone = false;
				robot.bStopDone = true;
			}
			robot.S_aAxisList[6].pFb->SpindleStop.OnCycle();
			robot.S_aAxisList[7].pFb->SwayStop.OnCycle();
		}
	}

	if(robot.bStopDone)//测试停止后继续回零
	{
		if (SewHome())
		{
			robot.bStopDone = false;
		}
	}
	robot.S_aAxisList[6].pFb->SpindleMoveRelative.OnCycle();
	robot.S_aAxisList[7].pFb->SwayMoveRelative.OnCycle();
	robot.S_aAxisList[8].pFb->WorksMoveRelative.OnCycle();
	robot.S_aAxisList[6].pFb->SpindleMoveVelocity.OnCycle();
	robot.S_aAxisList[7].pFb->SwayMoveVelocity.OnCycle();
	//printf("spindle vel: %d,  sway vel: %d, robot vel: %.6f\n",robot.S_aAxisList[6].pFb->SpindleMoveVelocity.Axis->oMove.lCommandVel,robot.S_aAxisList[7].pFb->SwayMoveVelocity.Axis->oMove.lCommandVel,robot.sewCalcPara.fSewSpeed);
}
#endif