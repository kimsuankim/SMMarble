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
 

//노드이름변수(type에따른) 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] =
{
  "강의",
  "식당",
  "실험실",
  "집",
  "실험실로이동",
  "음식찬스",
  "축제시간"
};
//노드이름주소반환함수 
char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
 } 

//1.구조체 형식정의
//노드 구조체 형식 정의 
typedef struct smmobject {
	char name[MAX_CHARNAME];//노드이름 
	smmObjType_e objType; //보드,카드 
	int type;//노드이름과대응되는 유형 
	int credit;//학점 
	int energy;//에너지 
	smmObjGrade_e grade;//성적 
}smmObject_t;

//2.구조체 배열 변수 정의
//smmObject_t smm_node[MAX_NODE];

//3.관련 함수 변경 

//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade) //노드를 만드는 함수 
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));//동적노드할당 
	
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
	return ptr;//동적노드주소반환 
}//메모리반납안해?? 

//메인! 데이터에 접근해//저장된 노드구조체 멤버반환 함수들 
char* smmObj_getObjName(void* obj)  
{
	smmObject_t* ptr = (smmObject_t*)obj;//형변환, 멤버에 접근 
	return ptr -> name;
} //노드이름주소반환
 
int smmObj_getObjType(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr -> type;
}//노드타입반환 

int smmObj_getObjCredit(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> credit;
}//노드학점반환 

int smmObj_getObjEnergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> energy;
}//노드에너지반환
 
int smmObj_getObjGrade(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> grade;
}//노드성적반환 



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

