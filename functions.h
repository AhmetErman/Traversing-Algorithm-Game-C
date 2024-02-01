#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define MAX 15
#define MAX_MAP_NAME 30
#define MAX_NAME_LENGTH 30
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SCORES 30
#define USER_FILE "users.bin"

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

//
void mainMenu(char *enteredUsername);
int maps(int matrix[][MAX],int *dots);
void readFromFile(int matrix[][MAX], char *fileName);
int playAuto(int matrix[MAX][MAX], int n,int game,int dots);
int playManual(int matrix[MAX][MAX], int n,int game);

//
int randomMap(int matrix[MAX][MAX]);
int move(int matrix[MAX][MAX],int moves[MAX][MAX][MAX],int connectionTimes[MAX], int *connected,int *numOfMoves, int *undoMoves, int n);
int moveAuto(int x,int y,int matrix[MAX][MAX],int n,int numOfMoves,int num,int connected[MAX],int *finished,int dots,int *draw);
void printBoard(int matrix[][MAX], int N);
int isBlocked(int matrix[MAX][MAX], int chosen[3], int x, int y);
void saveBoard(int matrix[MAX][MAX], int moves[MAX][MAX][MAX], int numOfMoves, int n);
void undo(int matrix[MAX][MAX],int moves[MAX][MAX][MAX],int *numOfMoves, int *undoMoves,int n);
void absMatrix(int matrix[MAX][MAX],int n);
int whichNum(int *x,int *y,int connected[MAX],int matrix[MAX][MAX], int n);
int checkFinished(int connected[MAX],int matrix[MAX][MAX],int n,int dots,int num,int *draw);
int isFull(int matrix[MAX][MAX],int n);

// Sign up and Login
void LoginMenu();
void signUp();
void login();
int findUserByUsername(const char *username);
int checkPassword(int n, char *enteredPassword);

// More
void howToPlay();
void topScores();
void saveScore(int score,char *username);



int move(int matrix[MAX][MAX],int moves[MAX][MAX][MAX],int connectionTimes[MAX], int *connected,int *numOfMoves, int *undoMoves, int n){
    int i,temp;
    int x,y;// x = satır, y = sutün
    int chosen[3];//chosen[0] = x chosen[1] =y chosen[2] = number


    saveBoard(matrix, moves, *numOfMoves,n);


    printf("\nChoose a square has number: (if you want to undo press -1) (if you want to leave game press -2)");
    scanf(" %d",&chosen[0]); //x

    if(chosen[0] == -1){
        if(*numOfMoves < 1)
            printf("You cannot undo!");
        else{
            if(connectionTimes[*connected] == *numOfMoves)
                *connected -= 1;

            undo(matrix,moves,numOfMoves,undoMoves,n);
        }
    }
    else if(chosen[0] == -2){
        return 1;
    }
    else{
        scanf(" %d",&chosen[1]);//y
        chosen[2] = matrix[chosen[0]][chosen[1]]; // number

        printf("You chose [%d][%d]: %d\n",chosen[0],chosen[1],chosen[2]);

        printf("\nChoose the square you want to connect: ");
        scanf(" %d %d",&x,&y);

        temp = isBlocked(matrix,chosen, x, y);

        if((chosen[0] != x && chosen[1] != y) || chosen[2] == 0)
            printf("Error: Wrong input");
        else if(temp == 1)
            printf("The way is Blocked!");
        else{
            *numOfMoves += 1;

            for ( i = chosen[0] +1; i <= x; i++)
                matrix[i][y] = chosen[2];

            for ( i = chosen[0] -1; i >= x; i--)
                matrix[i][y] = chosen[2];

            for ( i = chosen[1] +1; i <= y; i++)
                matrix[x][i] = chosen[2];

            for ( i = chosen[1] -1; i >= y; i--)
                matrix[x][i] = chosen[2];

            if( temp == 2){
                *connected += 1;
                connectionTimes[*connected] = *numOfMoves;
            }
        }
    }

            return 0;
}

// returns 1 or 0
int moveAuto(int x,int y,int matrix[MAX][MAX],int n,int numOfMoves,int num,int connected[MAX],int *finished,int dots,int *draw){
    int temp, isConnected=0, cache;
    //printBoard(matrix,n);
    if(*finished)
        return 0;

    if(numOfMoves == 1)
        num = whichNum(&x,&y,connected,matrix,n);
    else{

        if (x<0 || y<0 || x >= n || y >= n)
            return 1;
        else if(matrix[x][y] == num ){
            connected[num] = 1;
            isConnected = 1;

                if(*finished == 0 && checkFinished(connected,matrix,n,dots,num,draw)){
                    *finished = numOfMoves;
                    absMatrix(matrix,n);
                    return 0;
                }

            //printBoard(matrix,n);
            cache = whichNum(&x,&y,connected,matrix,n);
            if(cache>0){
                temp = num;
                num = cache;
            }
            else{
                connected[num]=0;
                return 1;
            }

        }
        else if(matrix[x][y] !=0 || (((matrix[x+1][y] == -num) + (matrix[x-1][y] == -num) + (matrix[x][y+1] == -num) + (matrix[x][y-1] == -num)) > 1))
            return 1;
    }

    matrix[x][y] = -num;

    if(moveAuto(x+1,y,matrix,n,numOfMoves+1,num,connected,finished,dots,draw)){
        if(moveAuto(x,y+1,matrix,n,numOfMoves+1,num,connected,finished,dots,draw)){
            if(moveAuto(x,y-1,matrix,n,numOfMoves+1,num,connected,finished,dots,draw)){
                if(moveAuto(x-1,y,matrix,n,numOfMoves+1,num,connected,finished,dots,draw)){

                }
            }
        }
    }

    if(connected[num] == 0 && isConnected != 1 && numOfMoves != 1){
            matrix[x][y] = 0;
        }


    if(isConnected == 1 && *finished == 0 ){
        connected[temp] = 0;
    }

}


void printBoard(int matrix[][MAX], int N){
    HANDLE  hConsole; // Renklendirme için
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Renklendirme için
    int i,j,k;
    printf("\n   ");
    for (k=0;k<N;k++)
        printf("  %d   ",k);
    for(i=0;i<N;i++){
        printf("\n  -");
        for (k=0;k<N;k++)
            printf("------");
        printf("\n%d |",i);

        for(j=0;j<N;j++){
            if (matrix[i][j]!=0){
                SetConsoleTextAttribute(hConsole, abs(matrix[i][j]));
                printf("  %d", abs(matrix[i][j]));
                SetConsoleTextAttribute(hConsole, 7);
                printf("  |");
            }
            else
                 printf("     |");
        }
    }
    printf("\n  -");
    for (k=0;k<N;k++)
        printf("------");

    printf("\n");
}

// It fills matrix[][] with random number according to the game rules. It returns the size of the map (n).
int randomMap(int matrix[MAX][MAX]){
    int i,j,rand1,rand2,n=0;
    srand(time(NULL));

    while(!(n >= 3 && n <= 15)){
        printf("Enter the size of the map: (between 3-15)\n");
        scanf("%d",&n);
    }

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            matrix[i][j] = 0;

    i=0;
    while( i < n*2){

        rand1 = rand() % n;
        rand2 = rand() % n;


        if(matrix[rand1][rand2] == 0){
            matrix[rand1][rand2] = i/2 +1;
            i++;
        }
    }
    return n;
}

// It checks is the way blocked. If it is blocked returns 1, not blocked 0, if its blocked but with the right number it returns 2.
int isBlocked(int matrix[MAX][MAX], int chosen[3], int x, int y){// 0: Not blocked; 1: blocked; 2: connected
    int i;

    for ( i = chosen[0] +1; i < x; i++)
        if(matrix[i][y] !=0)
            return 1;

    for ( i = chosen[0] -1; i > x; i--)
        if(matrix[i][y] !=0)
            return 1;

    for ( i = chosen[1] +1; i < y; i++)
        if(matrix[x][i] !=0)
            return 1;

    for ( i = chosen[1] -1; i > y; i--)
        if(matrix[x][i] !=0)
            return 1;

    if(matrix[x][y] == chosen[2])
        return 2;
    else if(matrix[x][y] == 0)
        return 0;
    else
        return 1;
}

//It is for move function. It saves last move from matrix[][] to moves[][][].
void saveBoard(int matrix[MAX][MAX], int moves[MAX][MAX][MAX], int numOfMoves, int n){
    int i,j;
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            moves[numOfMoves % MAX][i][j] = matrix[i][j];
}
//It is for move function. It takes last move from moves[][][] then copies to matrix[][].
void undo(int matrix[MAX][MAX],int moves[MAX][MAX][MAX],int *numOfMoves, int *undoMoves,int n){
    int i,j;
    *numOfMoves -= 1;
    *undoMoves += 1;
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            matrix[i][j] = moves[*numOfMoves % MAX][i][j];

}


// It abstracts all elememts of matrix
void absMatrix(int matrix[MAX][MAX],int n){
    int i,j;

    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            matrix[i][j] = abs(matrix[i][j]);

}

// It is for autoMove function. It returns the number for the function continue to make way
int whichNum(int *x,int *y,int connected[MAX],int matrix[MAX][MAX], int n){
    int i,j;

    for ( i = 0; i < n; i++){

        for ( j = 0; j < n; j++){
            if(matrix[i][j] != 0 && connected[abs(matrix[i][j])] == 0){
                *x = i;
                *y = j;
                return abs(matrix[i][j]);//num
            }

        }
    }

    return 0;
}


// It is for autoMove function.It checks the connected[] if it all elements are 1 then checks isFull if it is true(1) checkFinished return 1 else 0.
int checkFinished(int connected[MAX],int matrix[MAX][MAX],int n,int dots,int num,int *draw){
    int i;
    for ( i = 1; i <= dots; i++){
        if(i>*draw){
            *draw = i;
            printBoard(matrix, n);
        }

        if(connected[i] == 0)
            return 0;

    }

    if(isFull(matrix,n))
        return 1;

    return 0;
}

// It is for checkFinished function. Checks the matrix if it is full it returns 1 else 0.
int isFull(int matrix[MAX][MAX],int n){
    int i,j;
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            if(matrix[i][j] == 0)
                return 0;

    return 1;
}


// Menüyü ekrana yazdırma fonksiyonu
void LoginMenu() {
    int choice = 1;

    while (choice) {

        printf("\n---- User Account Management ----\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting program.\n");
                choice = 0;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Yeni kullanıcı kaydı fonksiyonu
void signUp() {
    struct User newUser;
    char str[MAX_NAME_LENGTH];

    printf("Enter your name: ");
//    scanf("%s",newUser.name);
    fflush(stdin);
    gets(str);
//    fgets(str,sizeof(str),stdin);
//    scanf("%[^\n]%*c", newUser.name);
    strcpy(newUser.name,str);

    printf("Enter your surname: ");
    scanf("%s", newUser.surname);

    // Kullanıcı adı benzersiz olana kadar tekrar al
    do {
        printf("Enter your username: ");
        scanf("%s", newUser.username);

        if (findUserByUsername(newUser.username)) {
            printf("Username already exists. Please choose a different one.\n");
        }
    } while (findUserByUsername(newUser.username));

    printf("Enter your password: ");
    scanf("%s", newUser.password);

    FILE *fout = fopen(USER_FILE, "ab");
    if (fout == NULL) {
        fprintf(stderr, "Error opening user file.\n");
        exit(1);
    }
    fwrite(&newUser, sizeof(struct User), 1, fout);

    /* txt dosyası için
    fprintf(fout, "%s %s %s %s\n", newUser.username, newUser.password, newUser.name, newUser.surname);*/

    fclose(fout);
    printf("Account created successfully!\n");
}

// Kullanıcı girişi fonksiyonu
void login() {
    char enteredUsername[MAX_USERNAME_LENGTH];
    char enteredPassword[MAX_PASSWORD_LENGTH];

    printf("Enter your username: ");
    scanf("%s", enteredUsername);

    int userIndex = findUserByUsername(enteredUsername);

    if (userIndex == 0) {
        printf("User not found. Please sign up first.\n");
        return;
    }

    printf("Enter your password: ");
    scanf("%s", enteredPassword);


    if (checkPassword(userIndex, enteredPassword)) {
        printf("Login successful. Welcome!\n");
        mainMenu(enteredUsername);
    } else {
        printf("Incorrect password. Please try again.\n");
    }
}

int findUserByUsername(const char *username) {
    struct User user;
    int i = 1;
    FILE *fin = fopen(USER_FILE, "rb");

    rewind(fin);

    while(fread(&user, sizeof(user), 1, fin)) {

        if (strcmp(user.username, username) == 0) {
            fclose(fin);
            return i; // Kullanıcı bulundu, dizinini indeksini döndür
        }
        i++;
    }

/* .txt için
    while ((ch = fgetc(fin)) != EOF){
        if(ch == '\n' || i==0){
            if(i==0) rewind(fin);// to get the first word we need to rewind

            i++;
            fscanf(fin,"%s",str);
            if (strcmp(str, username) == 0) {
                fclose(fin);
                return i; // Kullanıcı bulundu, dizinini indeksini döndür
            }
        }
    }*/

    fclose(fin);
    return 0; // Kullanıcı bulunamadı
}

int checkPassword(int n, char *enteredPassword){
    struct User user;
    int i = 1;
    FILE *fin = fopen(USER_FILE, "rb");

    rewind(fin);

    while(fread(&user, sizeof(user), 1, fin)) {


        if (i == n && strcmp(user.password, enteredPassword) == 0) {
            fclose(fin);
            return i; // Kullanıcı bulundu, dizinini indeksini döndür
        }
        i++;
    }

    /* .txt için
    while ((ch = fgetc(fin)) != EOF){
        if(ch == '\n' || i==0){
            i++;
            fscanf(fin,"%s",str);//username
            fscanf(fin,"%s",str);//password
            if (i ==n) {
                fclose(fin);
                if(strcmp(str,enteredPassword) == 0)
                    return 1; // Password found and correct
            }
        }
    }*/

    return 0;// Password couldn't been found
}

// Scores
void topScores(){

    int i,j,currentScore, currentIndex = 0;
    struct Score scores[MAX_SCORES];
    char currentName[MAX_NAME_LENGTH];

    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", "scores.txt");
        exit(1);
    }

    for (i = 0; i < MAX_SCORES; i++) {
        scores[i].score = 0;
        scores[i].name[0] = 0;
    }

    // Read scores from file
    while (fscanf(file, "%d %s", &currentScore, currentName) == 2) {
        if (currentIndex < MAX_SCORES) {
            scores[currentIndex].score = currentScore;
            strcpy(scores[currentIndex].name, currentName);
            currentIndex++;
        }
    }

    fclose(file);

    // bubble sort
    for (i = 0; i < MAX_SCORES - 1; i++) {
        for (j = i + 1; j < MAX_SCORES; j++) {
            if (scores[i].score < scores[j].score) {
                struct Score temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    // Print the top 5 scores
    printf("Top 5 Scores:\n");
    for (i = 0; i < 5; i++) {
        printf("%d. Score: %s %d\n", i + 1, scores[i].name, scores[i].score);
    }
}

void saveScore(int score,char *username){
    FILE *fout = fopen("scores.txt", "a");

    fprintf(fout, "%d %s\n",score, username);

    fclose(fout);
}

void howToPlay(){

    printf("Start Game: Once you select this option, you'll be presented with different maps to choose from. Select a map size between 3x3 to 15x15.\n"
           "\n"
           "Game Objective: Your goal in Dot Connection is to connect dots on the grid in such a way that each dot is connected to another dot of the same color, forming paths without intersecting each other.\n"
           "\n"
           "Gameplay:\n"
           "\n"
           "* Manual Mode: In manual mode, you'll choose the dots to connect manually. Select a square with a number, then choose another square to connect it to. Be careful not to intersect paths or block your way.\n"
           "* Auto Mode: In auto mode, the game automatically connects dots for you. Watch as the connections unfold, but keep an eye on the progress to ensure paths are not blocked.\n"
           "\n"
           "Winning: The game ends when either all dots are connected correctly, or there are no more possible moves. If you successfully connect all dots without blocking any paths, you win!\n"
           "\n"
           "Scoring: Your score is based on the number of moves you make and the efficiency of your connections. The fewer moves you make and the more efficient your connections, the higher your score will be.\n"
           "\n"
           "Enjoy the Game!: Dot Connection is a challenging yet engaging puzzle game that tests your strategic thinking and problem-solving skills. Have fun connecting the dots and aiming for the highest score possible!\n");
}
