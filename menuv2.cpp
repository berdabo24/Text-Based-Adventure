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

void White_HL(string text){
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 112);
    cout << text;
    SetConsoleTextAttribute(hConsole, 15);
}

static int randomiser (int chance, int min, int max)
{
    int randomvalue = rand() % (max - min + 1) + min; //generate random no between 0 and (max-min)
                                                    //+ min so that range is between [min, max]
    int bias = (chance * (randomvalue - min)) / 100; // converts value to 0-based scale and skews value to min when chance is low; max when high
    return min + bias;
}

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
            cout << name << " is " << state << "!" << endl;
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

};

class Enemy : public Character
{
    public:
        Enemy(string name, int hp): Character(name,hp) {};
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

void Battle(Player &Player, Enemy &Enemy, string Action, string FirstMove){

    if (FirstMove == "Player"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Enemy.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;
                cout << Enemy.name << " blocked " << block_dmg << "!" << endl;
            }
            if (Enemy.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;
                    cout << Enemy.name << " Dodged!" << endl;
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;
                    cout << Enemy.name << " Got hit!" << endl;
                }
            }
            cout << Player.name << " deals " << attack_dmg << "!" << endl;
            Enemy.takeDamage(attack_dmg);
        }

        if (Action == "Block"){
            Player.SetState("Blocking");
        }

        if (Action == "Dodge"){
            Player.SetState("Dodging");
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            Player.useHeal(Heal_amount);
            cout << Player.name << " healed " << Heal_amount << "!" << endl;
        }

    }

    if (FirstMove == "Enemy"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Player.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;
                cout << Player.name << " blocked " << block_dmg << "!" << endl;
            }
            if (Player.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;
                    cout << Player.name << " Dodged!" << endl;
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;
                    cout << Player.name << " Got hit!" << endl;
                }
            }
            cout << Enemy.name << " deals " << attack_dmg << "!" << endl;
            Player.takeDamage(attack_dmg);
        }

        if (Action == "Block"){
            Enemy.SetState("Blocking");
        }

        if (Action == "Dodge"){
            Enemy.SetState("Dodging");
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            Enemy.useHeal(Heal_amount);
            cout << Enemy.name << " healed " << Heal_amount << "!" << endl;
        }

    }
}

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

}

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

void DrawDialog(string text){

    // Max 86 characters per line
    int length = text.length();
    int maxlength = BORDER_WIDTH - length - 1;

    cout << "| "; cout << text;
    cout << string(maxlength, ' ') << "|" << endl;

}

const int CHARACTERS_BORDER = 105;
const int LEFT_CHAR_POS = 0;
const int RIGHT_CHAR_POS = 85;

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

const int DrawEnemy_Lines = 11;
string DrawEnemy[DrawEnemy_Lines]= { // array size is number of lines in the drawing
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

void DrawMenu(string move,  Player &Player, Enemy &Enemy){

    system("cls");

    SLOT[0] = " Attack";
    SLOT[1] = " Guard";
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
        SLOT[1] = ">Guard";
    }
    if(x == 0 && y == 0){
        SLOT[2] = ">Dodge";
    }
    if(x == 1 && y == 0){
        SLOT[3] = ">Heal";
    }

    printCharacters(DrawPlayer,DrawPlayer_Lines,DrawEnemy,DrawEnemy_Lines);
    cout << Player.name << " " << Player.health << "/" << Player.maxHealth << "                                                                                     " << Enemy.health << "/" << Enemy.maxHealth << " " << Enemy.name << endl;
    Player.PlayerHealthBar(); cout << "   "; Enemy.EnemyHealthBar(); cout << endl;
    DrawTopBorder();
    DrawOptions(SLOT);
    DrawBottomBorder();

    DrawTopBorder();
    DrawDialog("Test");
    DrawDialog("awdawdoawidnaowdinawodinaowidnaowdonaowdnawo");
    DrawDialog("awdawdoawidnaowdinawodinaowidnaowdonaowdnawo");
    DrawBottomBorder();
}

void KeySwitch(Player &Player, Enemy &Enemy){

    DrawMenu(" ", Player, Enemy);

    while(true){

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
        default:
            break;
        }
    }

}

int main(){

    srand(time(0));

    Player p1("Jul",100);
    Enemy e1("Jewl",100);

    KeySwitch(p1,e1);

    cin.get();

    return 0;
}
