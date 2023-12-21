//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE         0 
#define SMMNODE_TYPE_RESTAURANT      1
#define SMMNODE_TYPE_LABORATORY      2 
#define SMMNODE_TYPE_HOME            3
#define SMMNODE_TYPE_GOTOLAB         4
#define SMMNODE_TYPE_FOODCHANCE      5
#define SMMNODE_TYPE_FESTIVAL        6

#define SMMNODE_TYPE_MAX             7

//유형종류 
typedef enum smmObjType {
	smmObjType_board = 0,
	smmObjType_food,
	smmObjType_festival,
	smmObjType_grade
}smmObjType_e;

//성적종류
typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Am, 
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
	
}smmObjGrade_e; //자료형임 



//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade);

//member retrieving 멤버접근 
char* smmObj_getObjName(void* obj);//이름접근 
int smmObj_getObjType(void* obj);//유형접근 
int smmObj_getObjCredit(void* obj);//학점접근 
int smmObj_getObjEnergy(void* obj);//에너지접근 
smmObjGrade_e smmObj_getObjGrade(void* obj);//성적접근 



//element to string
char* smmObj_getTypeName(int type);//타입에따른 유형이름 
char* smmObj_getGradeName(smmObjGrade_e grade);//성적에따른 성적이름 

#endif /* smm_object_h */
