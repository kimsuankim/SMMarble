//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

/*board configuration parameters*/
static int board_nr;//����ĭ�� 
static int food_nr;//����ī�尳�� 
static int festival_nr;//����ī�尳�� 
static int player_nr;//�÷��̾��� 

/*����ü: �÷��̾� ����ü ���� ���� */
typedef struct player{
	char name[MAX_CHARNAME];//�̸�
    int position;//������ġ 
	int energy;//���������� 
	int accumCredit;//�������� 
	int flag_graduate;//��������  
	int flag_lab;//������ 
}player_t;

/*�÷��̾� ����ü ����Ű�� ������ ����*/
static player_t *cur_player;//�����ͺ������� 


//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated 
void generatePlayers(int n, int initEnergy); //generate a new player*
void printGrades(int player); //print grade history of the player*
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn*
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif
/*���鸸��� �Լ�*/
void blankSpace(void)
{
	printf("\n");
    printf("\n");
    printf("\n");
} 

/*���ӽ��۾˸��Լ�*/
void opening(void)
{
	printf("===========================================================================\n");
	printf("-------------------------------WELCOME-------------------------------------\n");
	printf("--------------------------SOOKMYUNG MARBLE !-------------------------------\n");
	printf("------------------Let's Graduate(total credit: %i)-------------------------\n",GRADUATE_CREDIT);
	printf("===========================================================================\n");
} 

/*�÷��̾� ������ �ʱ�ȭ �Լ�
 �Է�: player_nr, initEnergy
 �̸��Է¹޾Ƽ� ����,  
 �÷��̾� ����ü ����(��ġ, ������, ����, ��������, �����߻���) �ʱ�ȭ */
void generatePlayers(int n, int initEnergy)
{
	int i;
     //�÷��̾� �� ��ŭ �ݺ� 
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name: ", i); 
         scanf("%s", cur_player[i].name);//�̸� ���� 
         fflush(stdin);//���ۺ��� 
         
         //�����ʱ�ȭ 
         cur_player[i].position = 0;//��ġ = �����(ù��°���)���� ���� 
         cur_player[i].energy = initEnergy;//������ = �ʱ⿡���� 
         cur_player[i].accumCredit = 0;//���� = 0 
         cur_player[i].flag_graduate = 0;//�������� = 0 (�ƴ�)
         cur_player[i].flag_lab = 0;//������ = 0 (�ƴ�)
     }
}

/* �÷��̾��ü ���� ��� �Լ� 
 �Է�: X 
 ��� �÷��̾��� ����(��ġ, ���翡����, ��������, ������ ����)���  */ 
void printPlayerStatus(void)
{
	int i;
	for(i=0; i<player_nr;i++)//�÷��̾� ����ŭ �ݺ�. ����÷��̾� ����  ��� 
	{
		printf("%s : position %i, energy %i, accumCredit %i\n",
		 cur_player[i].name,//�̸� 
		 cur_player[i].position,//��ġ 
		 cur_player[i].energy,//������ 
		 cur_player[i].accumCredit//��������
		 //cur_player[i].flag_lab//�����߿��� 
		 );
	}
}

/*�ֻ���������(�ڱ��ϸ��� ������������)
 �Է�: player(turn)
 ��ȯ: ���� �ֻ��� ��*/
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): \n");
    c = getchar();
    fflush(stdin);
    //������ ���ٸ�(press g) 
    if (c == 'g')
        printGrades(player);//��������Լ�ȣ�� 
    //�ֻ��������� 
    return (rand()%MAX_DIE + 1); //�ֻ��� 1~6 ��ȯ 
}

/*����.��ġ����
 �Է�: player(turn), step(die_result)
 (������ġ + ��ĭ) ���, ���� �ݺ� => ������ġ */
void goForward(int player, int step)
{
	void *boardPtr;
	int i;
	//������� ���, ��ġ ���� 
	for(i = 0; i<step; i++)//step��ŭ �ݺ�(step��ŭ ��ĭ ����) 
	{
		cur_player[player].position += 1;//������ġ = ��ġ+��ĭ   
	    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);//������ġ����(�������) 
		printf("=>Jump to %s\n", smmObj_getObjName(boardPtr));//������ġ ��� 
	}  
}
#if 0
int isGraduated(void)
{
	//��������>=��������/�÷��̾�� �ݺ�Ȯ�� 
	//�÷��̾� ����ü ���� �������� 
}

//�����ο�, ���� �Լ�.. 
int takeLecture(int player, char *lectureName, int credit)
{
	//������������ 
	int grade_r = (rand()%MAX_GRADE);
	printf("%i\n", grade_r);
	//��������ü ����,����  
	void* gradeObj = smmObj_genObject(lectureName, smmObjType_grade, 0, credit, 0, grade_r);//�����̸�,����,���� 
     smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradeObj);
	void* gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, 0);
        printf("%iplayer got grade:%s on %s(%i)", player, smmObj_getObjGrade(gradePtr), smmObj_getObjName(gradePtr), smmObj_getObjCredit(gradePtr));
        
	return grade_r;
}
#endif
#if 0
//���� �̷� �˻� �Լ�
int findGrade(int player, char *lectureName)
{
	void* gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, *lectureName);
	int grade = smmObj_getObjGrade(gradePtr);
	
	return grade;
}
#endif
//��������ü ��� �Լ� 
void printGrades(int player)
{
	int i;
	void *gradePtr;  
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmObj_getObjName(gradePtr), smmObj_getObjGrade(gradePtr));
	}
}

//action code when a player stays at a node
void actionNode(int player, int food_nr, int festival_nr) //�ٲ���� 
{
	void* boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getObjType(boardPtr);
    char *name = smmObj_getObjName(boardPtr);
    int credit = smmObj_getObjCredit(boardPtr);
    //void *gradePtr;
    
    
    //����ī��̱� 
    int foodcard_r = (rand()%food_nr);
    int festcard_r = (rand()%festival_nr);
    void *foodPtr = smmdb_getData(LISTNO_FOODCARD, foodcard_r);
    void *festPtr = smmdb_getData(LISTNO_FESTCARD, festcard_r);
    
    
	switch(type)//��Ÿ�Կ� ���Ѿ׼� 
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
        	//�ҿ信�����̻�, ������������, ����������� 
        	if(cur_player[player].energy >= smmObj_getObjEnergy(boardPtr) /*&& findGrade(player,name) == NULL*/)
        	{
        	 char c;
        	 printf("You can take a %s (press a to take a lecture):\n", name);
        	 c = getchar();
        	 fflush(stdin);
        	  //���� ���� ���� 
        	  if(c == 'a')
        	  {
        	  	//�����߰�, �������Ҹ� 
        	  	cur_player[player].accumCredit += smmObj_getObjCredit(boardPtr);
                cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);
                //addtail??
                //grade generation ��������ü����,���� 
                //takeLecture(player, name, credit);
                
                
			  }
			  //���ǵ������ 
			  else
			  printf("You drop %s\n",name);
			  
			  break;
			}
			//�ҿ信�����������ϰų� �����������
			printf("You can't take a %s\n(�ҿ信���� ����, ����� ����)\n", name);
         
         break;
        //�Ĵ� : ���������� 
        case SMMNODE_TYPE_RESTAURANT:
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr);
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
		//����� : ���ε��� ��� 
		case SMMNODE_TYPE_LABORATORY:
		 if (cur_player[player].flag_lab == 1)//������
		  {
		  	//���輺�� ���ذ�  
		  	int threshold = (rand()%MAX_DIE + 1);
		  	printf("->Experiment time!! Let's see if you can satisfy professor(threshold:%i)\n", threshold);
		  	//�����ϱ� = �ֻ��� ������ 
		  	int ex_result = rolldie(player);
		  	printf("->Experiment result : %i", ex_result); 
		  	//���輺������ 
		  	if(ex_result >= threshold)//����
			  {
			  	printf("success!! %s can exit this lab!", cur_player[player].name); 
			  	//������ ���� �ƴ�
			  	cur_player[player].flag_lab = 0;
				//����� Ż�� 
				//cur_player[player].position = 
		      }
		    else//����
			printf("fail.. %s cannot exit this lab.. you can try it again next turn", cur_player[player].name);
		  } 
		  else//�����߾ƴ�
		  printf("->This is not experiment time. You can go through this lab.");
		break;
		//��: ���������� :���ް��� 
		case SMMNODE_TYPE_HOME: //�������¼���! 
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr);
		 printf("WELCOME HOME\n");
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
			
		/*�����ΰ� : ����Ƿ� �̵�, ������ ���� 
		case SMMNODE_TYPE_GOTOLAB:
			//1)������ ���·� ����
			cur_player[player].flag_lab = 1;
			//2)����Ƿ� �̵� 
			cur_player[player].position = 
			break;*/
		//��������:��������ī��, ���������� 
		case SMMNODE_TYPE_FOODCHANCE:
         printf("foodcard %i : %s, energy %i\n", 
		 foodcard_r, smmObj_getObjName(foodPtr), smmObj_getObjEnergy(foodPtr));
		 cur_player[player].energy += smmObj_getObjEnergy(foodPtr);
		 break;
		//����: ��������ī��, �̼Ǽ��� 
		case SMMNODE_TYPE_FESTIVAL:
         printf("festivalcard %i : %s\n", 
		 festcard_r, smmObj_getObjName(festPtr));
		 printf("You have to complete'%s'(press any key to continue)\n", smmObj_getObjName(festPtr)); 
		 break;
		default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy; //*������� ���濡���� 
    int turn = 0;
    //void *boardPtr_f = smmdb_getData(LISTNO_NODE, 0);//�����(ù��°���) ������ �ޱ�
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }//���� �ȿ���
    
    //����ĭ ����� 
    printf("Reading board component......\n");//���� ���� 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); 
        smmdb_addTail(LISTNO_NODE, boardObj); //��ũ�帮��Ʈ ��ü ������. �޸��� �ּ� //����Ʈ ������
		 
        board_nr ++;//�������尳�� 
    }
    fclose(fp);//���ϴݱ� 
    printf("Total number of board nodes : %i\n", board_nr);//����ĭ��Ȯ�� 
    
    for (i = 0;i<board_nr;i++){
    
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    }//�� ����ĭ ���� Ȯ�� 
    //����ĭ ����
	//������ĭ�� ù��°ĭ���� ���� �����ͺ��� ������? �־ ����: ����ĭ ����Ű��, ������ĭ�� Ư���� ù��°ĭ ����Ŵ �����ʹ� ��尡��Ű��, ����ּ�!����ü�ּ� 
	//turn=turn+1%player_nr//012012012012012012012�ƴϸ� 012... 15 16=0, 17=1 (i)%board_nr �� ����!
	/*if(i>=board_nr)
	{
		int n = i%board_nr
		void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); 
        smmdb_addTail(LISTNO_NODE, boardObj); 
		
	 } */
    //�ʱ⿡���� = ��������  
    
   //����ĭ���� 
    
   
    //1-2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }//���Ͼȿ��� 
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_food, 0, 0, energy, 0); 
        smmdb_addTail(LISTNO_FOODCARD, foodObj);//�ʿ�����ʳ� ������ó���̾����Ծƴϴϱ� 
        
        food_nr ++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    for (i = 0;i<food_nr;i++)
    {	
        void *foodObj = smmdb_getData(LISTNO_FOODCARD, i);
        printf("node %i : %s, energy %i\n", 
		i, smmObj_getObjName(foodObj), smmObj_getObjEnergy(foodObj));
    }//�� ����ī��  ���� Ȯ�� 
    
    
    //1-3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }//���Ͼȿ��� 
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        //store the parameter set
        void *festivalObj = smmObj_genObject(name, smmObjType_festival, 0, 0, 0, 0); 
        smmdb_addTail(LISTNO_FESTCARD, festivalObj);
        
        festival_nr ++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    for (i = 0;i<festival_nr;i++)
    {	
        void *festivalObj = smmdb_getData(LISTNO_FESTCARD, i);
        printf("node %i : %s\n", 
		i, smmObj_getObjName(festivalObj));
    }//����ī��  ���� Ȯ�� 
 
    //0.opening
    blankSpace();
    opening();
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    /*2.1.input player number to player_nr*/
	do
    {
        printf("input player number(1~%i): ",MAX_PLAYER); 
        scanf("%d", &player_nr);//�÷��̾�� ���� 
        fflush(stdin); // ���ۺ��� 
    }
    while (player_nr < 0 || player_nr > MAX_PLAYER);//0 < �÷��̾�� < �ִ��÷��̾�� 
    
    /*2.2.�÷��̾��ü �޸��Ҵ�*/ 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));//�÷��̾� ����ü ���� �޸� �Ҵ�  
    if(cur_player == NULL){
    	printf("�����޸� �Ҵ� ����\n");
		exit(1); 
	}//�����ڵ鸵 
    
    /*2.3.�÷��̾� ���� �ʱ�ȭ*/
    //1)�ʱ⿡���� ���� 
	void *boardPtr_f = smmdb_getData(LISTNO_NODE, 0); //�����(ù��°���) ������ �ޱ� 
    initEnergy = smmObj_getObjEnergy(boardPtr_f);//�ʱ⿡���� = ������� ������ 
    
    //2)�÷��̾� �ʱ�ȭ 
    generatePlayers(player_nr, initEnergy);//�÷��̾� ������ �ʱ�ȭ �Լ� ȣ�� 
    
    
   
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //3-1. initial printing
        //1)�÷��̾� ������� 
		printPlayerStatus();
		//2)������������ 
		printf("This is %s's turn:::", cur_player[turn].name);
        //3-2. die rolling (if not in experiment)
        if(cur_player[turn].flag_lab != 1) //�������� �ƴϸ� 
        {
        	die_result = rolldie(turn);
        	printf("-->result : %i\n", die_result);
        
         //3-3. go forward
         goForward(turn, die_result); 
         
        }
		//3-4. take action at the destination node of the board
        actionNode(turn, food_nr, festival_nr); 
        
        //3-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    
    //4. SM Marble game end------------------------------------------------------------------------------------------
    //���� �������
	
    free(cur_player);//�÷��̾��ü �޸� �ݳ� 
    system("PAUSE");
    return 0;
}
