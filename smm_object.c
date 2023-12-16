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
 

//���ڿ� �迭
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] =
{
  "����","�Ĵ�","�����","��","����Ƿ��̵�","��������","�����ð�"
};

char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];//char����Ű�� �����͹�ȯ 
 } 
#if 0
static char smmObj_name [MAX_NODE][MAX_CHARNAME];
static int smmObj_type [MAX_NODE];
static int smmObj_credit [MAX_NODE];
static int smmObj_energy [MAX_NODE];
static int smmObj_noNode=0; // ????
#endif
static int smmObj_noNode=0; //���ݱ��� ������ ���� 

typedef enum smmObjType {
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
}smmObjType_e; //�ű�� ���ο����� �ʿ��� 
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

//1.����ü ���� ����
typedef struct smmobject {
	char name[MAX_CHARNAME];
	smmObjType_e objType; 
	int type;
	int credit;
	int energy;
	smmObjType_e grade;
}smmObject_t;

//2.����ü �迭 ���� ����
//smmObject_t smm_node[MAX_NODE];

//3.���� �Լ� ���� 

//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjType_e grade) //��带 ����� �Լ� 
{
	smmObject_t* ptr;
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));//��������Ҵ� 
	
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

//����! �����Ϳ� ������ 
char* smmObj_getNodeName(void* obj) //�ּҸ� �޾Ƽ� 
{
	smmObject_t* ptr = (smmObject_t*)obj;//����ȯ, ����� ���� 
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

