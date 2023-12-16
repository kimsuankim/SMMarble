//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100
 

//문자열 배열
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] =
{
  "강의","식당","실험실","집","실험실로이동","음식찬스","축제시간"
};

char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];//char가리키는 포인터반환 
 } 
#if 0
static char smmObj_name [MAX_NODE][MAX_CHARNAME];
static int smmObj_type [MAX_NODE];
static int smmObj_credit [MAX_NODE];
static int smmObj_energy [MAX_NODE];
static int smmObj_noNode=0; // ????
#endif
static int smmObj_noNode=0; //지금까지 저장한 개수 

typedef enum smmObjType {
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
}smmObjType_e; //옮기기 메인에서도 필요힘 
typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
	
}smmObjGrade_e; 

//1.구조체 형식 정의
typedef struct smmobject {
	char name[MAX_CHARNAME];
	smmObjType_e objType; 
	int type;
	int credit;
	int energy;
	smmObjType_e grade;
}smmObject_t;

//2.구조체 배열 변수 정의
//smmObject_t smm_node[MAX_NODE];

//3.관련 함수 변경 

//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjType_e grade) //노드를 만드는 함수 
{
	smmObject_t* ptr;
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));//동적노드할당 
	
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
	/*smmObj_type[smmObj_noNode] = type;
    smmObj_credit[smmObj_noNode] = credit;
    smmObj_energy[smmObj_noNode] = energy;
    #endif
    strcpy(smm_node[smmObj_noNode].name, name);
    smm_node[smmObj_noNode].type = type;
    smm_node[smmObj_noNode].credit = credit;
    smm_node[smmObj_noNode].energy = energy;
    
	smmObj_noNode++; */
	return ptr;
}

//메인! 데이터에 접근해 
char* smmObj_getNodeName(void* obj) //주소만 받아서 
{
	smmObject_t* ptr = (smmObject_t*)obj;//형변환, 멤버에 접근 
	return ptr -> name;
}
int smmObj_getNodeType(int node_nr)
{
	return smm_node[node_nr].type;
}
int smmObj_getNodeCredit(int node_nr)
{
	return smm_node[node_nr].credit;
}
int smmObj_getNodeEnergy(int node_nr)
{
	return smm_node[node_nr].energy;
}


//member retrieving



/*element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}
*/

