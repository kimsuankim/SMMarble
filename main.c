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
int isGraduated(void); //check if any player is graduated*
void generatePlayers(int n, int initEnergy); //generate a new player*
void printGrades(int player); //print grade history of the player*
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn*
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
int getGraduatedPlayer(void);//������ �÷��̾� ��ȯ�Լ�
int findLab(void);//����ǳ�� ��ȯ�Լ� 
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
	printf("------------------------------------------------------------------\n");
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
	printf("------------------------------------------------------------------\n");
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
    /*if (c == 'g')
        printGrades(player);//��������Լ�ȣ�� */
    //�ֻ��������� 
    return (rand()%MAX_DIE + 1); //�ֻ��� 1~6 ��ȯ 
}

/*����.��ȯ�����Ǳ��� 
 �Է�: player(turn), step(die_result), homeEnergy(����忡����)
 ��ĭ�� �̵�, ����� ����ĥ�� �׼�*/
void goForward(int player, int step, int homeEnergy)
{
	void *boardPtr;
	int i;
	int home = 1;
	//��ĭ�� �̵�, �̵���� ��� 
	for(i = 0; i<step; i++)//step��ŭ �ݺ�(���� ������) 
	{
		//��ĭ�̵�, ��ȯ������ ����(�÷��̾���ġ : 0 ~ board_nr-1) 
		cur_player[player].position = (cur_player[player].position += 1)%board_nr;//������ġ = (������ġ+1)%���尳��
		//��ĭ�̵��� ��� ������ �ޱ�   
	    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
		//��ĭ�̵��� ��� ��� 
		printf("=>Jump to %s\n", smmObj_getObjName(boardPtr));
		//����� ����ġ��
		if(cur_player[player].position == 0)
		{
	     //���������� 
    	 cur_player[player].energy += homeEnergy; //����������+ �ʱ⿡����(������� ������) 
		 printf("-->You Pass HOME\n");
		 printf("-->You got %i energy!(energy:%i)\n", homeEnergy, cur_player[player].energy);
	    } 
	}  
}

/*�������� Ȯ��
 ��ȯ: ���ӳ� ����
 �������� ������ �÷��̾� ���� -> �ִٸ� ���ӳ� ��ȯ(end = 0)*/ 
int isGraduated(void)
{
	int i;
	int end = 1;//���� ������ ���� 
	//��������>=�������� : �������� 
	for(i = 0; i<player_nr; i++)
	{
		if(cur_player[i].accumCredit >= GRADUATE_CREDIT)
		 cur_player[i].flag_graduate = 1;
	}
	//�÷��̾� ����ü ��� �������� & ������ 
	for(i = 0; i<player_nr; i++)
	{
		if(cur_player[i].flag_graduate == 1 && cur_player[i].position == 0)//�������� ���� 
		end = 0;//���ӳ� 
		break;//�ݺ���Ż�� 
	} 
	return end;
}
/*������ �÷��̾� ��ȯ�Լ�
 ��ȯ:������ �÷��̾�*/ 
int getGraduatedPlayer(void)
{
	int i;
	int gra_player=0;//������ �÷��̾� ���� 
	for(i=0;i<player_nr;i++)//�÷��̾� �� ��ŭ �ݺ� 
	{
		if(cur_player[i].flag_graduate == 1 && cur_player[i].position == 0)//�������� ���� 
		 gra_player = i;//������ �÷��̾� ã�� 
		break;//ã���� �ݺ��� ���� 
	}
	return gra_player;//������ �÷��̾� ��ȯ 
}

#if 0
//�����ο�, ���� �Լ�.. 
smmObjGrade_e takeLecture(int player, char *lectureName, int credit)
{
	//������������ 
	smmObjGrade_e grade = (rand()%MAX_GRADE);
	printf("%s\n", grade);
	//��������ü ����,����  
	void* gradeObj = smmObj_genObject(lectureName, smmObjType_grade, 0, credit, 0, grade);//�����̸�,����,���� 
     smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradeObj);
	
	return grade;
}


//���� �̷� �˻� �Լ�
int findGrade(int player, char *lectureName)
{
	int i;
	void *gradePtr;
	int already = 0;
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		if(smmObj_getObjName(gradePtr) == lectureName)
		already = 1;
	}
	
	return already;
}

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
#endif
/*����ǳ�� ��ȯ �Լ�
 ��ȯ: ����ǳ�� */
int findLab(void)
{
	int i;
	void *labPtr;  
	for(i=0;i<board_nr;i++)//�����Ȯ�� 
	{
		labPtr = smmdb_getData(LISTNO_NODE, i);//��嵥���͹ޱ� 
		//���Ÿ�� == ������̸� 
		if(smmObj_getObjType(labPtr) == SMMNODE_TYPE_LABORATORY) 
		break;//�ݺ������� 
	}
	return i;//����ǳ���ȯ 
} 
//action code when a player stays at a node
void actionNode(int player) //�ٲ���� 
{
	//�������� 
	void* boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);//�÷��̾� ��ġ���� ��屸��ü�ּ�  
    int type = smmObj_getObjType(boardPtr);//��屸��ü ���� 
    char *name = smmObj_getObjName(boardPtr);//��屸��ü �̸� 
    int credit = smmObj_getObjCredit(boardPtr);//��� ����ü ����
	int energy = smmObj_getObjEnergy(boardPtr);//��� ����ü ������ 
    //void *gradePtr =  smmdb_getData(LISTNO_OFFSET_GRADE + player, cur_player[player].lecture+1);//���ǵ����� �� ���� ��� 
    //smmObjGrade_e grade_r = smmObj_getObjGrade(gradePtr)//�����ޱ� 
    
    //����ī��̱� 
    int foodcard_r = (rand()%food_nr);
    int festcard_r = (rand()%festival_nr);
    void *foodPtr = smmdb_getData(LISTNO_FOODCARD, foodcard_r);
    void *festPtr = smmdb_getData(LISTNO_FESTCARD, festcard_r);
    
	switch(type)//��Ÿ�Կ� ���Ѿ׼� 
    {
        /*1.���ǳ��(0)*/
        case SMMNODE_TYPE_LECTURE:
        	//�ҿ信�����̻�, ������������, ����������� 
        	if(cur_player[player].energy >= smmObj_getObjEnergy(boardPtr) /*&& findGrade(player,name) == NULL*/)
        	{
        	 char c;
        	 printf("-->Lecture %s(credit:%i, energy:%i) starts!\n(press a to take a lecture):"
			  ,name, credit, energy);
        	 c = getchar();
        	 fflush(stdin);
        	  //���� ���� ���� 
        	  if(c == 'a')
        	  {
        	  	//�����߰�, �������Ҹ� 
        	  	cur_player[player].accumCredit += smmObj_getObjCredit(boardPtr);
                cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);
                /*grade generation ��������ü����,���� 
                smmObjGrade_e grade_r = takeLecture(player, name, credit);
                //������ ��ȭ ��� 
                printf("-->%s successfully takes the lecture %s with grade %s, remained energy : %i", //(average:%i)
				 cur_player[player].name, name, grade_r, cur_player[player].energy);*/
			  }
			  //���ǵ������ 
			  else
			  printf("You drop %s\n",name);
			  
			break;
			}
			//�ҿ信�����������ϰų� �����������
			printf("You can't take a %s\n(�ҿ信���� ����, ����� ����)\n", name);
         
         break;
         
        /*�Ĵ�(1) : ���������� */
        case SMMNODE_TYPE_RESTAURANT:
         //������ ����
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr); 
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
		 
		/*�����(2) : ���ε��� ���*/ 
		case SMMNODE_TYPE_LABORATORY:
		 //2-1.������
		 if (cur_player[player].flag_lab == 1)
		  {
		  	//���輺�� ���ذ�  
		  	int threshold = (rand()%MAX_DIE + 1);
		  	printf("->Experiment time!! Let's see if you can satisfy professor(threshold:%i)\n", threshold);
		  	//�����ϱ� = �ֻ��� ������, ������ �Ҹ� 
		  	int ex_result = rolldie(player);
		  	printf("->Experiment result : %i\n", ex_result); 
		  	cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);//�������Ҹ� 
		  	//���輺������ 
		  	//--���� 
		  	if(ex_result >= threshold)
			  {
			  	printf("success!! %s can exit this lab!", cur_player[player].name); 
			  	//������ ���� �ƴ�
			  	cur_player[player].flag_lab = 0;
				//����� Ż��   
		      }
		    //--���� 
		    else
			printf("fail.. %s cannot exit this lab.. you can try it again next turn", cur_player[player].name);
		  } 
		  //2-2.�����߾ƴ� 
		  else
		  printf("->This is not experiment time. You can go through this lab.");
		break;
		
		/*��(3): ���������� :���ް���
		case SMMNODE_TYPE_HOME: 
	    goForward �Լ��� ������;���� �Լ� 
		*/	
		
		/*����Ƿΰ�(4) : ����Ƿ� �̵�, ������ ���� */
		case SMMNODE_TYPE_GOTOLAB:
			//1)������ ���·� ����
			cur_player[player].flag_lab = 1;
			//2)����Ƿ� �̵� :����ǳ���ȯ�Լ� 
			cur_player[player].position = findLab();//����Ƿ� �̵��� 
			actionNode(player);//����ǿ����׼� 
			break;
			
		/*��������(5):��������ī��, ����������*/
		case SMMNODE_TYPE_FOODCHANCE:
		 //��������ī�� �̱� 
         printf("foodcard %i : %s, energy %i\n", 
		 foodcard_r, smmObj_getObjName(foodPtr), smmObj_getObjEnergy(foodPtr));
		 //���������� 
		 cur_player[player].energy += smmObj_getObjEnergy(foodPtr);
		 break;
		 
		/*����(6): ��������ī��, �̼Ǽ��� */
		case SMMNODE_TYPE_FESTIVAL:
		 //��������ī�� �̱� 
         printf("festivalcard %i : %s\n", 
		 festcard_r, smmObj_getObjName(festPtr));
		 //�̼��ϱ� 
		 printf("You have to complete'%s'(press any key to continue)\n", smmObj_getObjName(festPtr)); 
		 break;
		 
		default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;//���������� 
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy; //*������� ���濡���� 
    int turn = 0; 
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    //1. import parameters ---------------------------------------------------------------------------------
    /*1-1. boardConfig : �����屸��ü ����, ��������, ��������,��屸��ü ���*/
    //0)���Ͼȿ��� 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    //1)������ ����ü ���� �� ���� 
    printf("Reading board component......\n");//���� ���� 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //*store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);//��� ����ü �����Լ�, ����ü �������� 
        smmdb_addTail(LISTNO_NODE, boardObj); //��� ����ü �ּ� ���� 
		 
        board_nr ++;//�������尳�� 
    }
    fclose(fp);//���ϴݱ� 
    //��尳��Ȯ��:board_nr 
    printf("Total number of board nodes : %i\n", board_nr); 
    
    //2)����� ��� ���� ��� 
    for (i = 0;i<board_nr;i++)//���ĭ����ŭ 
	{
		//��屸��ü ������ �ޱ�
        void *boardObj = smmdb_getData(LISTNO_NODE, i);//��屸��ü �ּ� ����Լ� ȣ�� 
        //����屸��ü ������ ���(��� ����ü ��� ���� �Լ� ȣ��) 
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    } 
    
   
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
 
 
    //���ϱ����� 
	//opening -------------------------------------------------------------------------------------------------
    blankSpace();//�����Լ� 
    opening();//���ӿ����� 
    
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
    while (isGraduated() == 1) //is anybody graduated?
    {
        int die_result;
        int passHome = 1;
        
        //3-1. initial printing
        //1)�÷��̾� �������
		blankSpace(); 
		printPlayerStatus();
		//2)������������ 
		printf("This is %s's turn:::", cur_player[turn].name);
        //3-2. die rolling (if not in experiment)
        if(cur_player[turn].flag_lab != 1) //�������� �ƴϸ� 
        {
        	die_result = rolldie(turn);
        	printf("-->result : %i\n", die_result);
        
         //3-3. go forward
		 goForward(turn, die_result, initEnergy); 
         //������ġ���� ����(passHome = 0) 
         
        }
		//3-4. take action at the destination node of the board
        actionNode(turn); 
        
        //3-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    
    //4. SM Marble game end------------------------------------------------------------------------------------------
    //������ �÷��̾� Ȯ��, ��� 
    blankSpace(); 
    int player_gr = getGraduatedPlayer();//�������÷��̾� 
    printf("GAME END!!\n");
    printf("GraduatedPlayer is %s\n", cur_player[player_gr].name);
    //������ �÷��̾��� �����̷� ��� 
    
	printf("%s graduate SMWU!!!!!Congratuation!!!!!\n", cur_player[player_gr].name);
	
    
	
    free(cur_player);//�÷��̾��ü �޸� �ݳ� 
    system("PAUSE");
    return 0;
}
