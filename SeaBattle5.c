#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define tries 100
#define a 'a'
#define p 'p'

int entries = 0, miss = 0, hits = 0, AIHits = 0;
int playerShips = 0, AIShips = 0;
int difficulty, refrenCell;
char manual, in0, in1;
int first, second;
int counter = 0;
char aiAttacks[100];
char aiIn1, aiIn2;
bool aiHit = false;
char confirmedArray[100];
int confirmedCells = 0;
int inputSize = 3;
char input[3];

const char valid_letters[] = "ABCDEFGHI";
const char valid_numbers[] = "123456789";
const char command_letters[] = "bsaiq";

int hitCells[100][2], missCells[100][2];
char inputCells[1][2], samePlayerCells[100][2], AIAttack[1][2];;

char AISubmarineShip1[1][2], AISubmarineShip2[1][2], AIFrigateShip1[2][2], AIFrigateShip2[2][2], AICruiserShip[3][2], AIBattleShip[4][2], AICarrier[5][2];

char playerSubmarineShip1[1][2], playerSubmarineShip2[1][2], playerFrigateShip1[2][2], playerFrigateShip2[2][2], playerCruiserShip[3][2], playerBattleShip[4][2], playerCarrierShip[5][2];

struct AIShip{
  int counter;
} submarine1, submarine2, frigateShip1, frigateShip2, cruiserShip, battleShip, carrierShip;

struct playerShip{
  int playerCounter;
} playerSubmarine1, playerSubmarine2, playerFrigate1, playerFrigate2, playerCruiser, playerBattle, playerCarrier;

void playerShipSettings(){
  playerSubmarine1.playerCounter = 1;
  playerSubmarine2.playerCounter = 1;  
  playerFrigate1.playerCounter = 2;
  playerFrigate2.playerCounter = 2;
  playerCruiser.playerCounter = 3;
  playerBattle.playerCounter = 4;
  playerCarrier.playerCounter = 5;
}

void AIShipSettings(){
  submarine1.counter = 1;
  submarine2.counter = 1;  
  frigateShip1.counter = 2;
  frigateShip2.counter = 2;
  cruiserShip.counter = 3;
  battleShip.counter = 4;
  carrierShip.counter = 5;
}

void manualAsk();
void seaBattleManual(char);
void battleSituation();
void addingPlayerCells(int, int, int);
void addSubmarine();
void addFrigate();
void checkPlayerCellsRefren();
void setupPlayerShips();
int randomInt(int, int);
void setupBattlefield(int);
void setTime();
void setupDifficulty();
void printPlayerShips(int);
void printAIShips(int);
void hello();
int statusCheck();
void checkIfMiss(int, char, char);
void checkIfResultDraw();
void checkIfQuit();
bool checkIfBattleSituation();
bool checkIfRefrenCell(int, int);
void playerInput(int);
void AIAttacks(int);
void checkIfAIMisses(int);
bool checkIfPrintAIShips();
void turn(int);

bool checkSubmarine(char[1][2], char, char, int, char);
bool checkFirstFrigate(char[2][2], char, char, char);
bool checkSecondFrigate(char[2][2], char, char, char);
bool checkCruiser(char[3][2], char, char, int, char);
bool checkBattleship(char[4][2], char, char, int, char);
bool checkCarrier(char[5][2], char, char, int, char);

int main()
{
    setTime();
    hello();
    manualAsk(); //вивід інструкції за бажанням

    AIShipSettings();
    playerShipSettings();

    setupDifficulty(); //визначення складності гри
    setupBattlefield(difficulty); //додавання кораблів в залежності від складності
    printPlayerShips(difficulty);

    for(int i = 0; i < tries; i++) {
        turn(i);

        if(!aiHit){ // якщо комп'ютер вистрелив повз
            playerInput(i);
            checkIfResultDraw(); //перевірка чи досягнуто максимуму можливих ходів. Якщо так, гру завершено у нічию
            checkIfQuit(); //перевірка чи гравець не ввів команду завершення гри
            if(checkIfRefrenCell(refrenCell, i)) {i--; continue;} //перевірка на унікальність ходу гравця

            hitCells[i][i] = inputCells[0][0]; //передача координат що пройшли перевірку у глобальний массив координат гравця
            hitCells[i][i+1] = inputCells[0][1];

            in0 = inputCells[0][0]; //передача координат що пройшли перевірку у локальні змінні для перевірки влучності
            in1 = inputCells[0][1];

            if(checkIfBattleSituation()) {i--; continue;} //перевірка чи були введені додаткові команди
            if(checkIfPrintAIShips()) {i--; continue;}

            int missPlayerHits = hits; //після перевірки влучності перевіряємо чи значення не змінилось

            //перевірка влучності гравця
            if(checkSubmarine(AISubmarineShip1, in0, in1, submarine1.counter, p)) {continue;}  
            if(checkSubmarine(AISubmarineShip2, in0, in1, submarine2.counter, p)) {continue;}
            if(checkFirstFrigate(AIFrigateShip1, in0, in1, p)) {continue;}
            if(checkSecondFrigate(AIFrigateShip2, in0, in1, p)) {continue;}
            if(checkCruiser(AICruiserShip, in0, in1, cruiserShip.counter, p)) {continue;}
            if(checkBattleship(AIBattleShip, in0, in1, battleShip.counter, p)) {continue;}
            if(checkCarrier(AICarrier, in0, in1, carrierShip.counter, p)) {continue;}

            checkIfMiss(missPlayerHits, in0, in1);
        } else {
            entries++; //якщо комп'ютер влучив, то збільшуємо ходи для коректного підрахунку нічиї і т.д.
        }

        int missAIHits = AIHits;//після перевірки влучності перевіряємо чи значення не змінилось
        aiHit = false;
        AIAttacks(i);

        //перевірка влучності комп'ютера
        if(checkSubmarine(playerSubmarineShip1, AIAttack[0][0], AIAttack[0][1], playerSubmarine1.playerCounter, a)) {continue;}
        if(checkSubmarine(playerSubmarineShip2, AIAttack[0][0], AIAttack[0][1], playerSubmarine2.playerCounter, a)) {continue;}
        if(checkFirstFrigate(playerFrigateShip1, AIAttack[0][0], AIAttack[0][1], a)) {continue;}
        if(checkSecondFrigate(playerFrigateShip2, AIAttack[0][0], AIAttack[0][1], a)) {continue;}
        if(checkCruiser(AICruiserShip, AIAttack[0][0], AIAttack[0][1], playerCruiser.playerCounter, a)) {continue;}
        if(checkBattleship(AIBattleShip, AIAttack[0][0], AIAttack[0][1], playerBattle.playerCounter, a)) {continue;}
        if(checkCarrier(AICarrier, AIAttack[0][0], AIAttack[0][1], playerCarrier.playerCounter, a)) {continue;}
        
        checkIfAIMisses(missAIHits);
    }
    return 0;
  }

  void hello()
{
    printf("Тебе вітає гра Морський бій 9х9! У тебе є %d спроб щоб потопити всі ворожі кораблі!\n", tries);
}

void manualAsk()
{
    printf("Хочеш прочитати ігрову Довідку? Введи Y або N\n");
    for(int i = 0; i < 1; i++)
    {
        int c;
        scanf("%1s", &manual);
        while((c = fgetc(stdin)) != '\n' && c != EOF);
    }
    seaBattleManual(manual);
}

void seaBattleManual(char man)
{
    switch(man)
    {
        case 'Y':
        printf("\nБій відбувається проти комп'ютера.\n");
        printf("Ігровий процес відбувається у текстовому режимі, за допомогою консолі\n");
        printf("Поле для бою - від A до E по вертикалі, від 1 до 9 по горизонталі\n");
        printf("У грі є 5 видів кораблів, кожен з них має різну кількість клітин:\n");
        printf("Субмарина - 1, Фрегат - 2, Крейсер - 3, Лінкор - 4, Авіаносець - 5\n");
        printf("Ти можеш обрати три рівня складності від 1 до 3\n");
        printf("На 1 рівні у тебе 2 Субмарини, 2 Фрегата, 1 Крейсер\n");
        printf("На 2 рівні у тебе 2 Субмарини, 2 Фрегата, 1 Крейсер, 1 Лінкор\n");
        printf("На 3 рівні у тебе 2 Субмарини, 2 Фрегата, 1 Крейсер, 1 Лінкор, 1 Авіаносець\n");
        printf("Приклад вводу координат твоєї субмарини - C5, Крейсера - A2A3A4\n");
        printf("Тобі доступні 3 команди, які ти можеш ввести під час свого ходу:\n");
        printf("bs - поточна ситуація на полі бою, ai - розташування кораблів ворога, qq - завершити гру\n");
        case 'N':
            printf("Вдалої гри!\n");
            break;
        default:
            manualAsk();
    }
}

void battleSituation()
{
    printf("Поточна ситуація на полі бою:\n");
    printf("У тебе залишилось %d кораблів на плаву\n", playerShips);
    printf("У ворога залишилось %d кораблів на плаву\n", AIShips);
    printf("Піднажми! :)\n");
}

int statusCheck()
{
     if(playerShips == 0)
    {
        printf("Ти програв!\n");
        printf("Ворогу знадобилось %d ходів", entries);
        exit(0);
    }

    if(AIShips == 0)
    {
        printf("Ти виграв!\n");
        printf("Тобі знадобилось %d ходів", entries);
        exit(0);
    }

    else {
        return 1;
    }
}

void checkInput(char ship[first][second])
{
    int shipSize = (first * second) + 1; //останній символ - \n
    char readArray[shipSize];
    fflush( stdout );
    counter = 0;
    bool correct = true;
    
    //перевірка на довжину символів
    if(fgets(readArray, shipSize, stdin) != NULL && (readArray[shipSize - 1] != '\n'))
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n'){
            dropped++;}

        if (dropped > 0)
        {
            printf("Не так швидко! Ти перебрав ліміт координат на %d символів, спробуй ще!\n", dropped );
            counter += dropped;
            memset(readArray, 0, shipSize * (sizeof readArray[0]) ); //якщо довжина не ок - очистка масиву
            correct = false;
            checkInput(ship); // повертаємо рекурсією той самий корабель
    } else {
        readArray[shipSize -1] = '\0'; 
    }
    }

    if(strlen(readArray) == shipSize - 1)
    {
        for(int i = 0, k = 0; i < shipSize; i++)
        {
            for(int j = 0; j < 2; j++, counter++)
            {
                ship[i][j] = readArray[counter];
            }
            int j = 0;
        }  
    }

    //перевірка на коректність значення
    for(int i = 0; i < shipSize - 1; i+=2)
    {
        if (!(strchr(valid_letters, readArray[i])) || !(strchr(valid_numbers, readArray[i+1]))){
            printf("Некоректне значення. Повтори спробу\n");
            memset(readArray, 0, shipSize * (sizeof readArray[0]) );
            correct = false;
            checkInput(ship);
        }
    }

    //якщо довжина і коректність ок - добавляємо у глобальний масив
    if(correct) 
    {
        for(int i = 0; i < shipSize; i++)
        {
            confirmedArray[confirmedCells + i] = readArray[i];
        }
        confirmedCells += (shipSize - 1);
    }

    //перевірка на унікальність
    for(int i = 0; i < confirmedCells - (shipSize - first); i += 2)
    {
        for(int j = i + 2; j < confirmedCells; j += 2)
        {
            if((confirmedArray[i] == confirmedArray[j]) && confirmedArray[i + 1] == confirmedArray[j + 1])
            {
                printf("Ти ввів однакові координати! Грай чесно, спробуй ще\n");
                memset(readArray, 0, shipSize * (sizeof readArray[0]) );
                //цикл очищає масив перевірених координат на розмір останнього корабля
                for(int m = (confirmedCells - (shipSize - 1)); m < confirmedCells + 1; m++) 
                {
                    for(int n = confirmedCells - (shipSize - 1); n < confirmedCells + 1; n++ )
                    {
                        confirmedArray[n] = confirmedArray[n + 1];
                    }
                }
                confirmedCells -= (shipSize - 1); //віднімаємо від лічильника перевірених координат розмір введених координат
                checkInput(ship);
            }
        }
    }
}

void addFirstSubmarine()
{
    printf("Введи координату своєї першої Субмарини\n");
    first = 1;
    second = 2;
    checkInput(playerSubmarineShip1);

    printf("ПЕРША СУБМАРИНА: %c%c\n", playerSubmarineShip1[0][0], playerSubmarineShip1[0][1]);
    playerShips++;
}

void addSecondSubmarine()
{
    printf("Введи координату своєї другої Субмарини\n");
    first = 1;
    second = 2;
    checkInput(playerSubmarineShip2);

    printf("ДРУГА СУБМАРИНА: %c%c\n", playerSubmarineShip2[0][0], playerSubmarineShip2[0][1]);
    playerShips++;
}

void addFirstFrigate()
{
    printf("Введи координату свого першого Фрегата\n");
    first = 2;
    second = 2;
    checkInput(playerFrigateShip1);

    printf("ПЕРШИЙ ФРЕГАТ: %c%c%c%c\n", playerFrigateShip1[0][0], playerFrigateShip1[0][1], playerFrigateShip1[1][0], playerFrigateShip1[1][1]);
    playerShips++;
}

void addSecondFrigate()
{
    printf("Введи координату свого другого Фрегата\n");
    first = 2;
    second = 2;
    checkInput(playerFrigateShip2);

    printf("ДРУГИЙ ФРЕГАТ: %c%c%c%c\n", playerFrigateShip2[0][0], playerFrigateShip2[0][1], playerFrigateShip2[1][0], playerFrigateShip2[1][1]);
    playerShips++;
}

void addCruiser()
{
    printf("Введи координату свого Крейсера\n");
    first = 3;
    second = 2;
    checkInput(playerCruiserShip);

    printf("КРЕЙСЕР: %c%c%c%c%c%c\n", playerCruiserShip[0][0], playerCruiserShip[0][1], playerCruiserShip[1][0], playerCruiserShip[1][1], playerCruiserShip[2][0], playerCruiserShip[2][1]);
    playerShips++;
}

void addBattleShip()
{
    printf("Введи координату свого Крейсера\n");
    first = 4;
    second = 2;
    checkInput(playerBattleShip);

    printf("КРЕЙСЕР: %c%c%c%c%c%c%c%c\n", playerBattleShip[0][0], playerBattleShip[0][1], playerBattleShip[1][0], playerBattleShip[1][1], playerBattleShip[2][0], playerBattleShip[2][1], playerBattleShip[3][0], playerBattleShip[3][1]);
    playerShips++;
}

void addCarrier()
{
    printf("Введи координату свого Крейсера\n");
    first = 5;
    second = 2;
    checkInput(playerCarrierShip);

    printf("АВІАНОСЕЦЬ: %c%c%c%c%c%c%c%c%c%c\n", playerCarrierShip[0][0], playerCarrierShip[0][1], playerCarrierShip[1][0], playerCarrierShip[1][1], playerCarrierShip[2][0], playerCarrierShip[2][1], playerCarrierShip[3][0], playerCarrierShip[3][1], playerCarrierShip[4][0], playerCarrierShip[4][1]);
    playerShips++;
}

void printPlayerShips(difficulty)
{
    printf("Твої кораблі на полі бою:\n");

    switch(difficulty)
    {
        case 3:
        printf("Авіаносець: %c%c%c%c%c%c%c%c%c%c\n", playerCarrierShip[0][0], playerCarrierShip[0][1], playerCarrierShip[1][0], playerCarrierShip[1][1], playerCarrierShip[2][0], playerCarrierShip[2][1], playerCarrierShip[3][0], playerCarrierShip[3][1], playerCarrierShip[4][0], playerCarrierShip[4][1]);
        case 2:
        printf("Лінкор: %c%c%c%c%c%c%c%c\n", playerBattleShip[0][0], playerBattleShip[0][1], playerBattleShip[1][0], playerBattleShip[1][1], playerBattleShip[2][0], playerBattleShip[2][1], playerBattleShip[3][0], playerBattleShip[3][1]);
        case 1:
        printf("Крейсер: %c%c%c%c%c%c\n", playerCruiserShip[0][0], playerCruiserShip[0][1], playerCruiserShip[1][0], playerCruiserShip[1][1], playerCruiserShip[2][0], playerCruiserShip[2][1]);
        printf("Фрегати: %c%c%c%c,%c%c%c%c\n", playerFrigateShip1[0][0], playerFrigateShip1[0][1], playerFrigateShip1[1][0], playerFrigateShip1[1][1], playerFrigateShip2[0][0], playerFrigateShip2[0][1], playerFrigateShip2[1][0], playerFrigateShip2[1][1]);
        printf("Субмарини: %c%c,%c%c\n", playerSubmarineShip1[0][0], playerSubmarineShip1[0][1], playerSubmarineShip2[0][0], playerSubmarineShip2[0][1]);
        printf("Флот готовий до бою. Гарної гри!\n");
        printf("Введи координату клітини, наприклад D5\n");
        break;
        default:
        setupDifficulty();
    }
}

void printAIShips(difficulty)
{
    printf("Розташування кораблів ворога:\n");

    switch(difficulty)
    {
        case 3:
        printf("Координати Авіаносця = %c%c\t %c%c\t %c%c\t %c%c\t %c%c\n", AICarrier[0][0], AICarrier[0][1], AICarrier[1][0], AICarrier[1][1], AICarrier[2][0], AICarrier[2][1], AICarrier[3][0], AICarrier[3][1], AICarrier[4][0], AICarrier[4][1]); 
        case 2:
        printf("Координати Лінкора = %c%c\t %c%c\t %c%c\t %c%c\n", AIBattleShip[0][0], AIBattleShip[0][1], AIBattleShip[1][0],AIBattleShip[1][1], AIBattleShip[2][0], AIBattleShip[2][1], AIBattleShip[3][0], AIBattleShip[3][1]);
        case 1:
        printf("Координати першої Субмарини = %c%c\n", AISubmarineShip1[0][0], AISubmarineShip1[0][1]);
        printf("Координати другої Субмарини = %c%c\n", AISubmarineShip2[0][0], AISubmarineShip2[0][1]);
        printf("Координати першого Фрегату = %c%c \t %c%c\n", AIFrigateShip1[0][0], AIFrigateShip1[0][1], AIFrigateShip1[1][0], AIFrigateShip1[1][1]);
        printf("Координати другого Фрегату = %c%c \t %c%c\n", AIFrigateShip2[0][0], AIFrigateShip2[0][1], AIFrigateShip2[1][0], AIFrigateShip2[1][1]);
        printf("Координати Ксейсера = %c%c\t %c%c\t %c%c\n", AICruiserShip[0][0], AICruiserShip[0][1], AICruiserShip[1][0], AICruiserShip[1][1], AICruiserShip[2][0], AICruiserShip[2][1]);
        break;
    }
  
}

void setTime()
{
    time_t t;
    srand((unsigned) time(&t));
}

void setupFirstSubmarine(char submarine[1][2])
{
    submarine[0][0] = 'A' + (rand() % 5);
    submarine[0][1] = '1';

    AIShips++;
}

void setupSecondSubmarine(char submarine[1][2])
{
    submarine[0][0] = 'F' + (rand() % 4);
    submarine[0][1] = '1';

    AIShips++;
}

void setupFirstFrigate(char frigate[2][2])
{
    char cell = 'A' + randomInt(0, 5);

    for(int i = 0; i < 2; i++)
    {
        frigate[i][0] = cell;
        frigate[i][1] = '2' + i;
    }

    AIShips++;
}

void setupSecondFrigate(char frigate[2][2])
{
    char first = 'A' + randomInt(1, 3);
    char second = randomInt(4, 2) + '0';

    for(int i = 0; i < 2; i++)
    {
        frigate[i][0] = first + i;
        frigate[i][1] = second;
    }

    AIShips++;
}

int randomInt(int x, int y)
{
    setTime();
    return x + rand() % y; // значення у діапазоні від x до y
}

void setupDifficulty()
{
    printf("Вибери складність від 1 до 3, де 1 - найлегша:\n");

    for(int i = 0; i < 1; i++)
    {
        int value;
        scanf("%1d", &difficulty);
        while((value = fgetc(stdin)) != '\n' && value != EOF);

    }
    
    if(!(difficulty == 1 || difficulty == 2 || difficulty == 3))
    {
        setupDifficulty();
    }
    else { printf("Вибрано складність %d\n", difficulty); }
}

void setupCruiser(char cruiser[3][2])
{
    int cruiserPosition = randomInt(1, 8);
    char cell;

    if (cruiserPosition <= 4){
        int flag = randomInt(0, 4);
        int letter = 'F' + randomInt(0, 2);
        cell = randomInt(2, 3) + '0';
   
        for(int i = 0; i < 3; i++)
        {
            cruiser[i][1] = cell;
            if(flag < 4)
            {
                cruiser[i][0] = letter + i;
            } else 
            { 
                cruiser[i][0] = letter - i;
            }
        }
    } 
    else {
        cell = 'F' + rand() % 4;

        for(int i = 0; i < 3; i++)
        {
            cruiser[i][0] = cell;
            cruiser[i][1] = 2 + i + '0';
        }
    }

    AIShips++;
}

void setupBattleship(char battleship[4][2])
{
    int battleshipPosition = randomInt(1, 9);

    if (battleshipPosition <= 5){
        int bsRandomInt = randomInt(0, 4);
        char cell = randomInt(6, 4) + '0';
        int randomPos = randomInt(1, 9);

        if((bsRandomInt==1) || (bsRandomInt==0) || (bsRandomInt==4) || (bsRandomInt==3 && randomPos <= 5)){
            for(int i = 0; i < 4; i++)
            {
                battleship[i][0] = 'A' + i;
            }
        }
        else {
            for(int i = 0; i < 4; i++)
            {
                battleship[i][0] = 'B' + i;
            }
        }

        for(int i = 0; i < 4; i++)
        {
            battleship[i][1] = cell;
        }
    } else 
    {
        int flag = randomInt(6, 3);
        char cell = 'A' + rand() % 5;

        for(int i = 0; i < 4; i++)
        {
            battleship[i][0] = cell;
        }

        if(flag == 6){
            for(int i = 0; i < 4; i++)
            {
                battleship[i][1] = '0' + (flag + i);
            }
        }
        if(flag == 7) {
            for(int i = 0; i < 4; i++)
            {
                battleship[i][1] = '0' + (flag - 1) + i;
            }
        }
        if(flag == 8) {
            for(int i = 0; i < 4; i++)
            {
                battleship[i][1] = '0' + (flag - 2) + i;
            }
        }
        if(flag == 9){
            for (int i = 0; i < 4; i++)
            {
                battleship[i][1] = '0' + (flag - i);
            }
        }
    }

    AIShips++;
}

void setupCarrier(char carrier[5][2])
{
    char cell = 'F' + (rand() % 4);

    for(int i = 0; i < 5; i++)
    {
        carrier[i][0] = cell;
        carrier[i][1] = '5' + i;
    }

    AIShips++;
}

void setupBattlefield(int difficulty)
{
    switch(difficulty)
    {
        case 3:
        addCarrier();
        setupCarrier(AICarrier);
        case 2:
        addBattleShip();
        setupBattleship(AIBattleShip);
        case 1:
        addFirstSubmarine();
        addSecondSubmarine();
        addFirstFrigate();
        addSecondFrigate();
        addCruiser();
        setupFirstSubmarine(AISubmarineShip1);
        setupSecondSubmarine(AISubmarineShip2);
        setupFirstFrigate(AIFrigateShip1);
        setupSecondFrigate(AIFrigateShip2);
        setupCruiser(AICruiserShip);
        break;
        default:
        setupDifficulty();
    }
}

bool checkSubmarine(char submarine[1][2], char first, char second, int counter, char turn)
{
    bool hit = false;
    if((first == submarine[0][0]) && (second == submarine[0][1])){
            counter--;
            hit = true;
            switch(turn)
            {
                case p:
                printf("Вітаємо! Ти потопив Субмарину!\n");
                hits+=1;
                AIShips--;
                hitCells[hits][0] = first;
                hitCells[hits][1] = second;
                break;
                case a:
                printf("Ворог потопив твою Субмарину!\n");
                playerShips--;
                AIHits++;
                aiHit = true;
                break;
            }
            statusCheck();
        }
        return hit;
}
       

bool checkFirstFrigate(char frigate[2][2], char first, char second, char turn)
{
    bool hit = false;
    if(((first == frigate[0][0]) && (second == frigate[0][1])) || (first == frigate[1][0] && (second == frigate[1][1]))){
        hit = true;
            switch(turn)
            {
            case p:
            frigateShip1.counter--;
            hits++;
            hitCells[hits][0] = first;
            hitCells[hits][1] = second;
            if(frigateShip1.counter > 0){
                printf("Ранив!\n");
            }
            else {
                AIShips--;
                printf("Вітаємо! Ти потопив Фрегат!\n");
            }
            break;
            case a:
            playerFrigate1.playerCounter--;
            AIHits++;
            aiHit = true;
            if(playerFrigate1.playerCounter > 0){
                printf("Ворог ранив твій Фрегат!\n");
            }
            else {
                playerShips--;
                printf("Ворог потопив твій Фрегат!\n");
            }
            break;
            }
        statusCheck();
    }
    return hit;
}

bool checkSecondFrigate(char frigate[2][2], char first, char second, char turn)
{
    bool hit = false;
    if(((first == frigate[0][0]) && (second == frigate[0][1])) || (first == frigate[1][0] && (second == frigate[1][1]))){
        hit = true;
            switch(turn)
            {
            case p:
            frigateShip2.counter--;
            hits++;
            hitCells[hits][0] = first;
            hitCells[hits][1] = second;
            if(frigateShip2.counter > 0){
                printf("Ранив!\n");
            }
            else {
                AIShips--;
                printf("Вітаємо! Ти потопив Фрегат!\n");
            }
            break;
            case a:
            playerFrigate2.playerCounter--;
            AIHits++;
            aiHit = true;
            if(playerFrigate2.playerCounter > 0){
                printf("Ворог ранив твій Фрегат!\n");
            }
            else {
                playerShips--;
                printf("Ворог потопив твій Фрегат!\n");
            }
            break;
            }
        statusCheck();
    }
    return hit;
}

bool checkCruiser(char cruiser [3][2], char first, char second, int counter, char turn)
{
    bool hit = false;
    if (((first == cruiser[0][0]) && (second == cruiser[0][1])) || (first == AICruiserShip[1][0] && (second == cruiser[1][1])) || (first == cruiser [2][0] && (second == cruiser[2][1]))){
        hit = true;
        switch(turn)
        {
            case p:
            cruiserShip.counter--;
            hits++;
            hitCells[hits][0] = first;
            hitCells[hits][1] = second;
            if(cruiserShip.counter > 0)
            {
                printf("Ранив!\n");
                hit = true;
            }
            else{
                AIShips--;
                printf("Вітаємо! Ти пототив Крейсер!\n");
            }
            break;
            case a:
            playerCruiser.playerCounter--;
            AIHits++;
            aiHit = true;
            if(playerCruiser.playerCounter > 0)
            {
                printf("Ворог ранив твій Крейсер!\n");
            }
            else
            {
                playerShips--;
                printf("Ворог потопив твій Крейсер!\n");
            }
            break;
        }
        statusCheck();
    }
    return hit;
}

bool checkBattleship(char battleship[4][2], char first, char second, int counter, char turn)
{
    bool hit = false;
    if (((first == battleship[0][0]) && (second == battleship[0][1])) || (first == battleship[1][0] && (second == battleship[1][1])) || (first == battleship [2][0] && (second == battleship[2][1])) || ((first == battleship[3][0]) && (second == battleship[3][1]))){
        hit = true;
        switch(turn)
        {
            case p:
            battleShip.counter--;
            hits++;
            hitCells[hits][0] = first;
            hitCells[hits][1] = second;
            if(battleShip.counter > 0){
                printf("Ранив!\n");
            }
            else{
                AIShips--;
                printf("Вітаємо! Ти потопив Лінкор!\n");
            }
            break;
            case a:
            playerBattle.playerCounter--;
            AIHits++;
            aiHit = true;
            if(playerBattle.playerCounter > 0){
                printf("Ворог ранив твій Лінкор!\n");
            }
            else{
                playerShips--;
                printf("Ворог потопив твій Лінкор!\n");
            }
            break;
        }
        statusCheck();
    }
    return hit;
}

bool checkCarrier(char carrier[5][2], char first, char second, int counter, char turn)
{
    bool hit = false;
    if (((first == carrier[0][0]) && (second == carrier[0][1])) || (first == carrier[1][0] && (second == carrier[1][1])) || (first == carrier [2][0] && (second == carrier[2][1])) || ((first == carrier[3][0]) && (second == carrier[3][1])) || ((first == carrier[4][0]) && (second == carrier[4][1]))){
        hit = true;
        switch(turn)
        {
            case p:
            carrierShip.counter--;
            hits++;
            hitCells[hits][0] = first;
            hitCells[hits][1] = second;
            if(carrierShip.counter > 0)
            {
                printf("Ранив!\n");
            }
            else{
                AIShips--;
                printf("Вітаємо! Ти потопив Авіаносець!\n");
            }
            break;
            case a:
            playerCarrier.playerCounter--;
            AIHits++;
            aiHit = true;
            if(playerCarrier.playerCounter>0){
                printf("Ворог ранив твій Авіаносець!\n");
            }
            else{
                playerShips--;
                printf("Ворог потопив твій Авіаносець!\n");
            }
            break;
        }
        statusCheck();
    }
    return hit;
}           

void checkIfMiss(int missHits, char first, char second)
{
    if (missHits == hits){
            printf("Мимо\n");
            missCells[missHits][0] = first;
            missCells[missHits][1] = second;
            miss++;
        }
}

void checkIfResultDraw()
{
    if(entries == tries){
    printf("Нічия!\nСпробуй ще :)\n");
    exit(0);
    }
}

void checkIfQuit()
{
    if(inputCells[0][0] == 'q' && inputCells[0][1] == 'q'){
        printf("Хороша работа. Повертайся знову :)");
        exit(0);
    }
}

bool checkIfBattleSituation()
{
    bool show = false;
    if(inputCells[0][0] == 'b' && inputCells[0][1] == 's')
        {
            battleSituation();
            entries--; //зменшуємо лічильник ходів, тому що хід не зараховано
            show = true;
        }
    return show;
}

bool checkIfPrintAIShips()
{
    bool show = false;
    if(inputCells[0][0] == 'a' && inputCells[0][1] == 'i')
    {
        printAIShips(difficulty);
        entries--; //зменшуємо лічильник ходів, тому що хід не зараховано
        show = true;
    }
    return show;
}

bool checkIfRefrenCell(int refrenCell, int i)
{
    refrenCell = 0;

        for(int i = 0; i < tries; i++){
            if (((inputCells[0][0] == hitCells[i][0]) && (inputCells[0][1] == hitCells[i][1])) || ((inputCells[0][0] == missCells[i][0]) && (inputCells[i][0] == missCells[i][1])))
            refrenCell++;
        }

    bool refren = false;
    if(refrenCell > 0){
          printf("Ти ввів координату більше одного разу\n");
          entries--; //зменшуємо лічильник ходів, тому що хід не зараховано
          refren = true;
        }

    entries++;
    return refren;
}

void playerInput(int i)
{
    if(fgets(input, inputSize , stdin) != NULL && (input[inputSize - 1] != '\n'))
    {
        int dropped = 0;
        while (fgetc(stdin) != '\n'){
            dropped++;}

        if (dropped > 0)
        {
            printf("Не так швидко! Ти перебрав ліміт координат на %d символів, спробуй ще!\n", dropped );
            memset(input, 0, inputSize * (sizeof input[0]));
            playerInput(i);
    } else {
        input[inputSize - 1] = '\0';
    }
    }

    //перевірка на корректність координати атаки гравця, враховуючи існуючі додаткові команди
    if((strchr(valid_letters, input[0])) &&  (strchr(valid_numbers, input[1]))){  
        inputCells[0][0] = input[0];
        inputCells[0][1] = input[1];
    } else {
        if((strchr(command_letters, input[0])) && (strchr(command_letters, input[1]))){
            inputCells[0][0] = input[0];
            inputCells[0][1] = input[1];
        } else {
            printf("Некоректне значення. Повтори спробу\n");
            memset(input, 0, inputSize * (sizeof input[0]) );
            playerInput(i);
        }
    }
}

void AIAttacks(int i)
{
    aiIn1 = 'A' + (rand() % 8);
    aiIn2 = 1 + rand() % 9 + '0';

    //перевірка на унікальність атаки комп'ютера
    for(int x = 0; x < i * 2; x += 2){
        if ((aiIn1 == aiAttacks[x]) && (aiIn2 == aiAttacks[x+1])){
            AIAttacks(i);
        } else { 
            AIAttack[0][0] = aiIn1;
            AIAttack[0][1] = aiIn2;
        }
    }

     if(i == 0){ //на першому ході копіюємо координати у перші дві клітини масиву
        aiAttacks[0] = aiIn1;
        aiAttacks[1] = aiIn2;
    } else {
        aiAttacks[i*2] = aiIn1;
        aiAttacks[i*2+1] = aiIn2;
    }

    printf("Ворог стріляє в координату %c%c\n", AIAttack[0][0], AIAttack[0][1]);
}

void checkIfAIMisses(missAIHits)
{
    if (missAIHits == AIHits){
        printf("Ворог стріляє мимо\n");
    }
}

void turn(int i)
{
    printf("%d хід:\n", (i + 1));
}