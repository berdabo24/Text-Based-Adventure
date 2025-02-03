#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13


// Highlight Function
void White_HL(string text){
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 112);
    cout << text;
    SetConsoleTextAttribute(hConsole, 15);
}


// Randomizer Function
static int randomiser (int chance, int min, int max)
{
    int randomvalue = rand() % (max - min + 1) + min; //generate random no between 0 and (max-min)
                                                    //+ min so that range is between [min, max]
    int bias = (chance * (randomvalue - min)) / 100; // converts value to 0-based scale and skews value to min when chance is low; max when high
    return min + bias;
}

// Character class
class Character {
    public:
        int health, maxHealth;
        string name, state;

        Character(string name,int hp): health(hp), maxHealth(hp), name(name) {}
        virtual ~Character() = default;

        void takeDamage(int damage){
            health -= damage;
            if (health < 0){
                health = 0;
            }
        }

        void useHeal(int healAmount){
            health += healAmount;
            if (health >= maxHealth){
                health = maxHealth;
            }
        }

        void SetState(string state){
            this->state = state;
        }

        string Attack(){
            return "Attack";
        }
        string Block(){
            return "Block";
        }
        string Dodge(){
            return "Dodge";
        }
        string Heal(){
            return "Heal";
        }
};

// Player class with healthbar
class Player : public Character
{
    public:
        Player(string name, int hp): Character(name,hp) {};
        void PlayerHealthBar()
        {

            int percent = (health * 100) / maxHealth;

            int bars = percent / 2;

            cout << "[";
            for (int i = 0; i < bars; i++){
                cout << "|";
            }
            for (int i = 0; i < 50 - bars; i++){
                cout << " ";
            }
            cout << "] ";

        }

        void resetHealth(){
            health = maxHealth;
        }

};


//Enemy Class with healthbar
class Enemy : public Character
{
    public:
        string* drawing;
        int drawingLines;


        Enemy(string name, int hp,string*drawing, int drawingLines): Character(name,hp),drawing(drawing),drawingLines(drawingLines) {};
        void EnemyHealthBar()
        {

            int percent = (health * 100) / maxHealth;
            int bars = percent / 2;

            cout << "[";
            for (int i = 0; i < 50 - bars; i++){
                cout << " ";
            }
            for (int i = 0; i < bars; i++){
                cout << "|";
            }
            cout << "] ";
        }

};

void White_HL(string text);
static int randomiser (int chance, int min, int max);
void Battle(Player &Player, Enemy &Enemy, string Action, string FirstMove);
void WhoFirst(Player &Player, Enemy &Enemy, string PlayerTurn, string EnemyTurn);
void DrawTopBorder();
void DrawBottomBorder();
void DrawOptions(string SLOT[]);
void DrawDialog(string text , int typeSpeed);
void printCharacters(string char1[], int char1_size, string char2[], int char2_size);
void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy);
void DrawMenu(string move,  Player &Player, Enemy &Enemy);
string MoveSelect(int x, int y, Player &Player);
void InitiateBattle(string PlayerTurn, Player &Player, Enemy &Enemy);
void KeySwitch(Player &Player, Enemy &Enemy);
void PressEnter();

const int MARGIN = 20;

// Battle function
void Battle(Player &Player, Enemy &Enemy, string Action, string FirstMove){

    if (FirstMove == "Player"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Enemy.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;

                DrawChacters_AND_HealthBar(Player, Enemy);
                cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 0); cout << endl;
                cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 1); DrawDialog(" blocked ", 1); cout << block_dmg; DrawDialog("!", 1);
                PressEnter();
            }
            if (Enemy.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;

                    DrawChacters_AND_HealthBar(Player, Enemy);
                    cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 0); cout << endl;
                    cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 1); DrawDialog(" Dodged!", 1); cout << endl;
                    PressEnter();
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;

                    DrawChacters_AND_HealthBar(Player, Enemy);
                    cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 0); cout << endl;
                    cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 1); DrawDialog(" Got hit!", 1); cout << endl;
                    PressEnter();
                }
            }
            Enemy.takeDamage(attack_dmg);

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Player.name, 0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Player.name, 1); DrawDialog(" deals ", 1); cout << attack_dmg; DrawDialog("!", 1);
            PressEnter();
        }

        if (Action == "Block"){
            Player.SetState("Blocking");

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Player.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Player.name,1); DrawDialog(" is blocking!",1);
            PressEnter();
        }

        if (Action == "Dodge"){
            Player.SetState("Dodging");

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Player.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Player.name,1); DrawDialog(" is dodging!",1);
            PressEnter();
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            Player.useHeal(Heal_amount);

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Player.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Player.name, 1); DrawDialog(" healed ", 1); cout << Heal_amount; DrawDialog("!", 1);
            PressEnter();
        }

    }

    if (FirstMove == "Enemy"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Player.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;

                DrawChacters_AND_HealthBar(Player, Enemy);
                cout << string(MARGIN, ' '); DrawDialog(Player.name, 0); cout << endl;
                cout << string(MARGIN, ' '); DrawDialog(Player.name, 1); DrawDialog(" blocked ", 1); cout << block_dmg; DrawDialog("!", 1);
                PressEnter();
            }
            if (Player.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;

                    DrawChacters_AND_HealthBar(Player, Enemy);
                    cout << string(MARGIN, ' '); DrawDialog(Player.name, 0); cout << endl;
                    cout << string(MARGIN, ' '); DrawDialog(Player.name, 1); DrawDialog(" Dodged!", 1); cout << endl;
                    PressEnter();
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;

                    DrawChacters_AND_HealthBar(Player, Enemy);
                    cout << string(MARGIN, ' '); DrawDialog(Player.name, 0); cout << endl;
                    cout << string(MARGIN, ' '); DrawDialog(Player.name, 1); DrawDialog(" Got hit!", 1); cout << endl;
                    PressEnter();
                }
            }
            Player.takeDamage(attack_dmg);

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 1); DrawDialog(" deals ", 1); cout << attack_dmg; DrawDialog("!", 1);
            PressEnter();
        }

        if (Action == "Block"){
            Enemy.SetState("Blocking");

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name,1); DrawDialog(" is blocking!",1);
            PressEnter();
        }

        if (Action == "Dodge"){
            Enemy.SetState("Dodging");

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name,1); DrawDialog(" is dodging!",1);
            PressEnter();
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            Enemy.useHeal(Heal_amount);

            DrawChacters_AND_HealthBar(Player, Enemy);
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name,0); cout << endl;
            cout << string(MARGIN, ' '); DrawDialog(Enemy.name, 1); DrawDialog(" healed ", 1); cout << Heal_amount; DrawDialog("!", 1);
            PressEnter();
        }

    }

}

// Decides whether player or enemy goes first, Dodge and Block first, attack second, heal last
void WhoFirst(Player &Player, Enemy &Enemy, string PlayerTurn, string EnemyTurn){

    int Player_priority = 0;
    int Enemy_priority = 0;

    if (PlayerTurn == "Block" || PlayerTurn == "Dodge"){
        Player_priority = 1;
    }

    if (EnemyTurn == "Block" || EnemyTurn == "Dodge"){
        Enemy_priority = 1;
    }

    if (PlayerTurn == "Attack"){
        Player_priority = 2;
    }

    if (EnemyTurn == "Attack"){
        Enemy_priority = 2;
    }

    if (PlayerTurn == "Heal"){
        Player_priority = 3;
    }

    if (EnemyTurn == "Heal"){
        Enemy_priority = 3;
    }

    if (Enemy_priority < Player_priority){
        Battle(Player,Enemy,EnemyTurn,"Enemy");
        Battle(Player,Enemy,PlayerTurn,"Player");
    }
    else{
        Battle(Player,Enemy,PlayerTurn,"Player");
        Battle(Player,Enemy,EnemyTurn,"Enemy");
    }

    Player.SetState("Neutral");
    Enemy.SetState("Neutral");
}

// Border initializations
const int BORDER_WIDTH = 105; // Adjustable size of border, affects every draw function.
const int NUM_SLOT = 4;
string SLOT[NUM_SLOT];
const int LEFT_SLOT_POS = 40;
const int RIGHT_SLOT_POS = 60;
// Default pos of highlight
int x = 0;
int y = 1;

void DrawTopBorder() {
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
}

void DrawBottomBorder() {
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
}

// Draws the options
void DrawOptions(string SLOT[]){

    int n = 0;

    for (int i = 0; i < 2; i++){
        int middle_space = RIGHT_SLOT_POS - LEFT_SLOT_POS - (static_cast<int>(SLOT[n].length()));
        int rmg_space = BORDER_WIDTH - RIGHT_SLOT_POS - (static_cast<int>(SLOT[n+1].length()));

        cout << "|" << string(LEFT_SLOT_POS, ' ');

        if (SLOT[n].find('>') < 1){ // checks if '>' exists in the text
            White_HL(SLOT[n]);
        }
        else{
            cout << SLOT[n];
        }

        cout << string(middle_space, ' ');

        if (SLOT[n+1].find('>') < 1){ // checks if '>' exists in the text
            White_HL(SLOT[++n]);
        }
        else{
            cout << SLOT[++n];
        }

        cout << string(rmg_space, ' ');

        cout << "|" << endl;

        if (i == 0){
            cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
        }

        n++;
    }

}

// Draws the dialog
void DrawDialog(string text , int typeSpeed){

    for (int i = 0; i < text.length(); i++){
        cout << text[i] << flush;
        Sleep(typeSpeed);
    }

    //cout << string(maxlength, ' ') << " " << endl;
}


// Character art initialization
const int CHARACTERS_BORDER = 105;
const int LEFT_CHAR_POS = 0;
const int RIGHT_CHAR_POS = 85;

// Draws the characters
void printCharacters(string char1[], int char1_size, string char2[], int char2_size){

    int n = 0;
    int e = 0;
    int smallest;


    if (char1_size > char2_size){

        smallest = char2_size;

        for (int i = 0; i < (char1_size - char2_size); i++){
            int rmg_space = CHARACTERS_BORDER - LEFT_CHAR_POS - (static_cast<int>(char1[n].length()));

            cout << string(LEFT_CHAR_POS, ' ');

            cout << char1[n];

            if (rmg_space > 0){
                cout << string(rmg_space, ' ');
            }

            cout << endl;
            n++;
        }

    }

    if (char2_size > char1_size){

        smallest = char1_size;

        for (int i = 0; i < (char2_size - char1_size); i++){
            int rmg_space = CHARACTERS_BORDER - RIGHT_CHAR_POS - (static_cast<int>(char2[e].length()));


            cout << string(RIGHT_CHAR_POS, ' ');

            cout << char2[e];

            if (rmg_space > 0){
                cout << string(rmg_space, ' ');
            }

            cout << endl;
            e++;
        }

    }


    for (int i = 0; i < smallest; i++){
        int middle_space = RIGHT_CHAR_POS - LEFT_CHAR_POS - (static_cast<int>(char1[n].length()));
        int rmg_space = CHARACTERS_BORDER - RIGHT_CHAR_POS - (static_cast<int>(char2[e].length()));

        cout << string(LEFT_CHAR_POS, ' ');

        cout << char1[n];

        cout << string(middle_space, ' ');

        cout << char2[e];

        cout << string(rmg_space, ' ');

        cout << endl;

        n++;
        e++;
    }

    cout << endl;
}

const int DrawPlayer_Lines = 6;
string DrawPlayer[DrawPlayer_Lines] = { // array size is number of lines in the drawing
    "    _|/^ ",
    "    (_oo ",
    "     |   ",
    "    /|L  ",
    "     |   ",
    "     LL  "
};


const int DrawEnemy1_Lines = 11; // array size is number of lines in the drawing
string DrawEnemy1[DrawEnemy1_Lines] = {
 //Centaur
"  <=======]}=====",
"   --.   /|",
"   _\"/_.'/",
" .'._._,.'",
" :/ \{}/",
"(L  /--',----._",
"    |          \\",
"   : /-\ .'-'\ / |",
"    \\, ||    \|",
"     \/ ||    ||",
"       zz    zz"
};
//e3
const int DrawEnemy2_Lines = 7;
string DrawEnemy2[DrawEnemy2_Lines] = {
"        .-'-.",
"      _/.~.~.\_",
"     ( ( o o ) )",
"      |/  `  \|   ;;",
"       \. ^ ./   ((",
"       /`~~~`\"   ))",
"      /       \  //"
};

const int DrawEnemy3_Lines = 11;
string DrawEnemy3[DrawEnemy3_Lines]= {
"       ccee88oo",
"  C8O8O8Q8PoOb o8oo",
" dOB69QO8PdUOpugoO9bD",
"CgggbU8OU qOp qOdoUOdcb",
"    6OuU  /p u gcoUodpP",
"      \\//  /douUP",
"        \\////",
"         |||/ " ,
"         |||/" ,
"         |||||" ,
"   .....//||||...."
};

//e4
const int DrawEnemy4_Lines = 15;
string DrawEnemy4[DrawEnemy4_Lines]= {
"                /           /",
"               /' .,,,,  ./",
"              /';'     ,/",
"             / /   ,,//,`'`",
"            ( ,, '_,  ,,,' ``",
"            |    /@  ,,, ;' `",
"           /    .   ,''/' `,``",
"          /   .     ./, `,, ` ;",
"       ,./  .   ,-,',` ,,/''\,'",
"      |   /; ./,,'`,,'' |   |",
"       |     /   ','    /    |",
"        \___/'   '     |     |",
"          `,,'  |      /     `\ ",
"              /      |        ~\ ",
"             '       ("
};

void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy){
    system("cls");
    printCharacters(DrawPlayer,DrawPlayer_Lines,Enemy.drawing,Enemy.drawingLines);
    cout << Player.name << " " << Player.health << "/" << Player.maxHealth; cout << string(85, ' '); cout << Enemy.health << "/" << Enemy.maxHealth << " " << Enemy.name << endl;
    Player.PlayerHealthBar(); cout << "   "; Enemy.EnemyHealthBar(); cout << endl;

    cout << endl;
    cout << endl;
}


void DrawMenu(string move,  Player &Player, Enemy &Enemy){


    SLOT[0] = " Attack";
    SLOT[1] = " Block";
    SLOT[2] = " Dodge";
    SLOT[3] = " Heal";

    // Increase/decrease y or x based on input
    if (move == "RIGHT" && x == 0){
        x++;
    }
    if (move == "LEFT" && x == 1){
        x--;
    }
    if (move == "UP" && y == 0){
        y++;
    }
    if (move == "DOWN" && y == 1){
        y--;
    }

    //Places arrow on correct slot based on x,y coordinate
    if (x == 0 && y == 1){
        SLOT[0] = ">Attack";
    }
    if (x == 1 && y == 1){
        SLOT[1] = ">Block";
    }
    if(x == 0 && y == 0){
        SLOT[2] = ">Dodge";
    }
    if(x == 1 && y == 0){
        SLOT[3] = ">Heal";
    }

    DrawChacters_AND_HealthBar(Player, Enemy);
    DrawTopBorder();
    DrawOptions(SLOT);
    DrawBottomBorder();

}

string MoveSelect(int x, int y, Player &Player){

    string move;

    if (x == 0 && y == 1){
       move = Player.Attack();
    }
    if (x == 1 && y == 1){
       move = Player.Block();
    }
    if(x == 0 && y == 0){
       move = Player.Dodge();
    }
    if(x == 1 && y == 0){
       move = Player.Heal();
    }

    return move;
}

void InitiateBattle(string PlayerTurn, Player &Player, Enemy &Enemy){

    int EnemyAI = randomiser(100, 1, 4);
    string EnemyTurn;
    if (EnemyAI == 1){
        EnemyTurn = Enemy.Attack();
    }
    if (EnemyAI == 2){
        EnemyTurn = Enemy.Block();
    }
    if (EnemyAI == 3){
        EnemyTurn = Enemy.Dodge();
    }
    if (EnemyAI == 4){
        EnemyTurn = Enemy.Heal();
    }

    WhoFirst(Player, Enemy, PlayerTurn, EnemyTurn);
}

void KeySwitch(Player &Player, Enemy &Enemy){

    bool loop = true;

    DrawMenu(" ", Player, Enemy);

    while(loop){

        switch(getch()){
        case KEY_UP:
            DrawMenu("UP", Player, Enemy);
            break;
        case KEY_DOWN:
            DrawMenu("DOWN", Player, Enemy);
            break;
        case KEY_LEFT:
            DrawMenu("LEFT", Player, Enemy);
            break;
        case KEY_RIGHT:
            DrawMenu("RIGHT", Player, Enemy);
            break;
        case KEY_ENTER:
            DrawChacters_AND_HealthBar(Player, Enemy);
            InitiateBattle(MoveSelect(x,y,Player), Player, Enemy);
            loop = false;
            break;
        default:
            break;
        }
    }

}




void PressEnter(){
    cout << endl;
    cout << endl;
    cout << endl;

    bool loop = true;
    cout << "Press ENTER to continue...";
    while(loop){
        if (getch() == KEY_ENTER){
            loop = false;
        }
    }
}

int main(){

    srand(time(0));

    int enemyIndex = 1;

    Player p1("Player",100);
    Enemy e1("Soldier",100,DrawEnemy1, DrawEnemy1_Lines);

    //if initiate 2nd enemy
    //enemyIndex++;
    //p1.resetHealth();
    //Enemy e2 ("Thief", 100, DrawEnemy2, DrawEnemy2_Lines);
    //KeySwitch(p1,e2);

    while (true){
    KeySwitch(p1,e1);
    if (e1.health <= 0)
    {
        //enemyIndex++;
        //p1.resetHealth();
        //Enemy e2 ("Thief", 100, DrawEnemy2, DrawEnemy2_Lines);
        //KeySwitch(p1,e2);
    }
    }

    return 0;
}
