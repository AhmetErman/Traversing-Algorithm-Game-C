#include "functions.h"


#define MAX 15

#define MAX_MAP_NAME 30
#define MAX_NAME_LENGTH 30
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SCORES 30
#define USER_FILE "users.bin"

typedef struct PointList{
    char name[9];
    int score ;
}pList;

struct User {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct Score {
    char name[MAX_NAME_LENGTH];
    int score;
};

int inputReadFromFile(int matrix[][MAX],int *dots);
void readFromFile(int matrix[][MAX], char *fileName);
void mainMenu();
int playAuto(int matrix[MAX][MAX], int n,int game,int dots);
int playManual(int matrix[MAX][MAX], int n,int game);


int main(){

	mainMenu();

  return 0;
}


//Dosyaların tektek adlarını girmemek için bir fonksiyonda topladım Seçilen haritanın boyutunu (n) return eder. 
int inputReadFromFile(int matrix[][MAX], int *dots){
    int i,j,num;

    scanf(" %d",&num);

    //Online Compilerda denemek için örnekler

   //5x5.txt
    // matrix[0][0] =1;matrix[1][4] =1;matrix[1][0] =2;matrix[3][3] =2;matrix[1][1] =3;matrix[1][3] =3;matrix[2][1] =4;matrix[3][4] =4;matrix[4][0] =5;matrix[4][4] =5;
    // *dots = 5;
    // return 5;

    //7x7.txt
    // matrix[0][0] = 1;matrix[4][4] = 1;matrix[2][6] = 2;matrix[0][1] = 2;matrix[1][1] = 3;matrix[2][4] = 3;matrix[1][2] = 4;matrix[6][6] = 4;
    // matrix[4][0] = 5;matrix[4][5] = 5;matrix[4][1] = 6;matrix[4][3] = 6;matrix[5][1] = 7;matrix[5][4] = 7;
    // *dots=7;
    // return 7;

    //8x8.txt
    // matrix[1][0] = 1;matrix[5][5] = 1;matrix[2][0] = 2;matrix[6][6] = 2;matrix[7][0] = 3;matrix[2][5] = 3;matrix[4][1] = 4;matrix[4][5] = 4;
    // matrix[4][2] = 5;matrix[6][2] = 5;matrix[3][5] = 6;matrix[6][3] = 6;
    // *dots=6;
    // return 8;

    //9x9.txt
    // matrix[1][1] = 1;matrix[0][5] = 1;matrix[0][4]=2;matrix[5][4]=2;matrix[3][2]=3;matrix[7][1]=3;matrix[4][4]=4;matrix[6][6]=4;matrix[6][4]=5;matrix[8][4]=5;
    // matrix[8][5]=6;matrix[3][8]=6;matrix[0][6]=7;matrix[0][8]=7;matrix[1][6]=8;matrix[1][8]=8;matrix[7][6]=9;matrix[2][8]=9;
    // *dots = 9;
    // return 9;


    switch (num){
    case 1:
	readFromFile(matrix,"TestVerileri/5x5_test_case1.txt");
    *dots = 5;
	return 5;
        break;
   case 2:
	readFromFile(matrix,"TestVerileri/5x5_test_case2.txt");
    *dots = 5;
	return 5;
        break;
    case 3:
	readFromFile(matrix,"TestVerileri/5x5_test_case3.txt");
    *dots = 5;
	return 7;
        break;
    case 4:
	readFromFile(matrix,"TestVerileri/7X7_test_case1.txt");
    *dots = 6;
	return 7;
        break;
    case 5:
	readFromFile(matrix,"TestVerileri/7X7_test_case2.txt");
    *dots=7;
	return 7;
        break;
    case 6:
	readFromFile(matrix,"TestVerileri/7X7_test_case3.txt");
    *dots=8;
	return 7;
        break;
    case 7:
	readFromFile(matrix,"TestVerileri/7X7_test_case4.txt");
    *dots=8;
	return 7;
        break;
    case 8:
	readFromFile(matrix,"TestVerileri/11x11_test_case1.txt");
    *dots=11;
	return 11;
    case 9:
	readFromFile(matrix,"TestVerileri/11x11_test_case2.txt");
    *dots=9;
	return 11;
    case 10:
	readFromFile(matrix,"TestVerileri/11x11_test_case3.txt");
    *dots = 10;
	return 11;
        break;
    
    default:
        break;
    }
    
}

void readFromFile(int matrix[][MAX], char *fileName){
	int i,j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("Dosya Acilamadi!");
		return;
    }
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matrix[i][j]=temp; 
    }  
  	fclose(data); 
   // Buraya kadar olan bolumde dosyadan okuma yapiliyor. Kodu degistirmeyiniz.
}

void mainMenu(){
    int i,j,n;
    int dots;
    int game; // 1: Prepared puzzle    2: Random puzzle    3: Scoreboard    4: Exit
    int mod; // 1: Manuel mode       2: Automatic mode 
    int matrix[MAX][MAX];
    pList points[MAX];
    int players=0;

    do{
        for ( i = 0; i < MAX; i++)
            for ( j = 0; j < MAX; j++)
                matrix[i][j] = 0;

        
        printf("\n        Main Menu\n-Play prepared puzzles (1)\n-Play free game (2)\n-Look the Scoreboard (3)\n-Exit (4)\n");
        scanf(" %d",&game);

        // Seçilen oyun türüne göre haritayı ayarlama
        if(game == 1){
            printf("Prepared Game:\n");

            printf("Enter your nickname (Max 9 character): ");
            scanf(" %s",&points[players].name);
            
            printf("Which game mode do you play?\nManuel: (1)\nAuto: (2)\nBack to MainMenu (3)\n");
            scanf(" %d", &mod);
            
            if(mod !=3){
                printf("Prepared Puzzles:\nChoose a preperad level between 1-10:");
                n = inputReadFromFile(matrix,&dots);
            }
            
            if(mod == 1)
                points[players].score = playManual(matrix,n, game);
            else if(mod == 2)
                points[players].score = playAuto(matrix,n, game,dots);
            else{
                printf("Returning to the MainMenu...\n");
                players--;
            }

            players++;
        }
        else if( game == 2){
            printf("Free Game:\n");

            printf("Enter your nickname (Max 9 character): ");
            scanf(" %s",&points[players].name);
            
            printf("Which game mode do you play?\nManuel: (1)\nAuto: (2)\nBack to MainMenu (3)\n");
            scanf(" %d", &mod);
            
            printf("Enter size of game (between 5-11):");
            scanf( " %d", &n);

            randomMatrix(matrix, n);
            dots = n;
            
            if(mod == 1)
                points[players].score = playManual(matrix,n, game);
            else if(mod == 2)
                points[players].score = playAuto(matrix,n, game,dots);
            else{
                printf("Returning to the MainMenu...");
                players--;
            }

            players++;
        }
        else if (game == 3)
        {
            if(players>0){
                printf("\n------------------------\n");
                printf("      SCOREBOARD:    \n");
                for(i =0; i< players; i++){
                    printf("*****%s : %d*****\n", points[i].name, points[i].score);
                }
                printf("\n------------------------\n");
            }
            else
                printf("There are no player entries yet.\n");
        }
        else if (game == 4)
            printf("Exiting...");
        else
            printf("Error: Wrong input\n");
    }while(game != 4);
    
}

// It returns the score earned.
int playManual(int matrix[MAX][MAX], int n,int game){
    int connected =0;
    int numOfMoves =0;
    int undoMoves =0;
    int score;
    int exit = 0; // 0: do nothing (continue)   1: exit the game 
    time_t timer = time(NULL);
    int connectionTimes[MAX];
    int moves[MAX][MAX][MAX];// first dimensions is number of moves, others are the saved board.
    

    while(connected < n && exit < 1){

        drawBoard(matrix,n);

        printf("\n\n%d. MOVE  (total moves = %d, undo moves =%d)\n",numOfMoves, numOfMoves+undoMoves,  undoMoves);

        exit = move(matrix, moves, connectionTimes, &connected, &numOfMoves,&undoMoves, n);

        printf("\n %d NUMBERS CONNECTED\n",connected);
    }
    drawBoard(matrix,n);
     
    timer = time(NULL) - timer;
    score = connected*1000*(10*n*4*game-numOfMoves-2*undoMoves)/timer;
    printf("Your score: %d",score);
    
    return score;

}


int playAuto(int matrix[MAX][MAX], int n,int game,int dots){
    int x =0, y = 0;
    int num =1;
    int numOfMoves =1;
    int finished =0;
    int draw =1;
    int score;
    int connected[MAX]={0};
    int exit = 0; // 0: do nothing (continue)   1: exit the game 
    time_t timer = time(NULL);

        drawBoard(matrix,n);
        printf("\nThe game is being solved automatically, please wait...\n");

        moveAuto(x,y,matrix,n,numOfMoves,num,connected,&finished,dots,&draw);
        
        timer = time(NULL)-timer;

        if(finished){
            drawBoard(matrix,n);
            printf("Program finished in %d seconds %d moves\n",timer,finished);
            printf("Congurulations game solved!\n");
            if(timer == 0)
                score = (pow(10,n-3) * dots * game)/(finished * 1);
            else
                score = (pow(10,n-3) * dots * game)/(finished * timer);
            
        }
        else{
            printf("There are no solution for this game!\n");
            score = 0;
        }


        printf("Your score: %d\npress a number to continue\n",score);

        scanf(" %d",&x);
    return score;

}

