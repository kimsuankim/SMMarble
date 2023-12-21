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
 

//��������̸�(type������) 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] =
{
  "����",
  "�Ĵ�",
  "�����",
  "��",
  "����Ƿ��̵�",
  "��������",
  "�����ð�"
};
//����̸��ּҹ�ȯ�Լ� 
char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
 } 

//1.����ü ��������
//��� ����ü ���� ���� 
typedef struct smmobject {
	char name[MAX_CHARNAME];//����̸� 
	smmObjType_e objType; //����,ī�� 
	int type;//����̸��������Ǵ� ���� 
	int credit;//���� 
	int energy;//������ 
	smmObjGrade_e grade;//���� 
	//struct smmobject* next;//������带 ����Ű�� �����ͺ��� 
}smmObject_t;

//2.����ü �迭 ���� ����
//smmObject_t smm_node[MAX_NODE];

//3.���� �Լ� ���� 

//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade /*struct smmobject* next*/) //��带 ����� �Լ� 
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));//��������Ҵ� 
	
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
	return ptr;//��������ּҹ�ȯ 
}//�޸𸮹ݳ�����?? 

//����! �����Ϳ� ������//����� ��屸��ü �����ȯ �Լ��� 
char* smmObj_getObjName(void* obj)  
{
	smmObject_t* ptr = (smmObject_t*)obj;//����ȯ, ����� ���� 
	return ptr -> name;
} //����̸��ּҹ�ȯ
 
int smmObj_getObjType(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr -> type;
}//���Ÿ�Թ�ȯ 

int smmObj_getObjCredit(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> credit;
}//���������ȯ 

int smmObj_getObjEnergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> energy;
}//��忡������ȯ
 
int smmObj_getObjGrade(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj; 
	return ptr -> grade;
}//��强����ȯ 



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

