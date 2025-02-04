#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib> 
#include <ctime>
#include <string>
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

        void resetHealth(){
            health = maxHealth;
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
        string* drawing;
        int drawingLines;

        Player(string name, int hp,string*drawing, int drawingLines): Character(name,hp),drawing(drawing),drawingLines(drawingLines) {};
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
void PlaceDialog(Player &Player, Enemy &Enemy, string character, string text);
void PlaceDialog(Player &Player, Enemy &Enemy, string character, string text, int variable, string text2);
void printCharacters(string char1[], int char1_size, string char2[], int char2_size);
void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy);
void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy, int int_damagetext_P, int int_damagetext_E, string sign);
void DrawMenu(string move, Player &Player, Enemy &Enemy);
string MoveSelect(int x, int y, Player &Player);
void InitiateBattle(string PlayerTurn, Player &Player, Enemy &Enemy);
void KeySwitch(Player &Player, Enemy &Enemy);
void PressEnter();



// Battle function
void Battle(Player &Player, Enemy &Enemy, string Action, string FirstMove){

    if (FirstMove == "Player"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Enemy.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;

                DrawChacters_AND_HealthBar(Player,Enemy);
                PlaceDialog(Player, Enemy, "Enemy", " blocked ", block_dmg, "!");
            }
            if (Enemy.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;

                    DrawChacters_AND_HealthBar(Player,Enemy);
                    PlaceDialog(Player, Enemy, "Enemy", " Dodged!");
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;

                    DrawChacters_AND_HealthBar(Player,Enemy);
                    PlaceDialog(Player, Enemy, "Enemy", " Got hit!");
                }
            }
            Enemy.takeDamage(attack_dmg);

            DrawChacters_AND_HealthBar(Player,Enemy,0,attack_dmg, "-");
            PlaceDialog(Player, Enemy, "Player", " deals ", attack_dmg, "!");
        }

        if (Action == "Block"){
            Player.SetState("Blocking");

            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Player", " is blocking!");
        }

        if (Action == "Dodge"){
            Player.SetState("Dodging");

            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Player", " is dodging!");
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            if (Player.health > 0){
                Player.useHeal(Heal_amount);

                DrawChacters_AND_HealthBar(Player,Enemy,Heal_amount,0, "+");
                PlaceDialog(Player, Enemy, "Player", " healed ", Heal_amount, "!");
            }
        }

    }

    if (FirstMove == "Enemy"){
        if (Action == "Attack"){
            int attack_dmg = randomiser(50, 0, 100);
            if (Player.state == "Blocking"){
                int block_dmg = randomiser(50, 0, attack_dmg);
                attack_dmg -= block_dmg;

                DrawChacters_AND_HealthBar(Player,Enemy);
                PlaceDialog(Player, Enemy, "Player", " blocked ", block_dmg, "!");
            }
            if (Player.state == "Dodging"){
                int dodge_chance = randomiser(70, 0, 100);
                if (dodge_chance > 30){
                    attack_dmg = 0;

                    DrawChacters_AND_HealthBar(Player,Enemy);
                    PlaceDialog(Player, Enemy, "Player", " Dodged!");
                }
                else{
                    int boost_dmg = randomiser(20, 0, attack_dmg);
                    attack_dmg += boost_dmg;

                    DrawChacters_AND_HealthBar(Player,Enemy);
                    PlaceDialog(Player, Enemy, "Player", " Got hit!");
                }
            }
            Player.takeDamage(attack_dmg);

            DrawChacters_AND_HealthBar(Player,Enemy,attack_dmg,0,"-");
            PlaceDialog(Player, Enemy, "Enemy", " deals ", attack_dmg, "!");
        }

        if (Action == "Block"){
            Enemy.SetState("Blocking");

            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Enemy", " is blocking!");
        }

        if (Action == "Dodge"){
            Enemy.SetState("Dodging");

            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Enemy", " is dodging!");
        }

        if (Action == "Heal"){
            int Heal_amount = randomiser(50, 10 , Player.health);
            if (Enemy.health > 0){
                Enemy.useHeal(Heal_amount);

                DrawChacters_AND_HealthBar(Player,Enemy,0,Heal_amount, "+");
                PlaceDialog(Player, Enemy, "Enemy", " healed ", Heal_amount, "!");
            }
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
        if (Player.health > 0){
            Battle(Player,Enemy,PlayerTurn,"Player");
        }
    }
    else {
        Battle(Player,Enemy,PlayerTurn,"Player");
        if (Enemy.health > 0) {
            Battle(Player,Enemy,EnemyTurn,"Enemy");
        }
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

// Default pos of highlight for battle menu
int x = 0;
int y = 1;

// default pos for story menu
int z = 0;

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

const int MARGIN = 10;
void DrawDialog_Margin(string text , int typeSpeed){

    cout << string(MARGIN, ' ');
    for (int i = 0; i < text.length(); i++){
        cout << text[i] << flush;
        Sleep(typeSpeed);
    }
}

//Easier function to place text in dialog
void PlaceDialog(Player &Player, Enemy &Enemy, string character, string text){

    if (character == "Player"){
        DrawDialog_Margin(Player.name, 0); cout << ":"<< endl;
        DrawDialog_Margin(Player.name, 1); DrawDialog(text, 1);
        PressEnter();
    }
    if (character == "Enemy"){
        DrawDialog_Margin(Enemy.name, 0); cout << ":"<< endl;
        DrawDialog_Margin(Enemy.name, 1); DrawDialog(text, 1);
        PressEnter();
    }
}
// overloaded function of ^ , use this if you want to place variable within text
void PlaceDialog(Player &Player, Enemy &Enemy, string character, string text, int variable, string text2){

    if (character == "Player"){
        DrawDialog_Margin(Player.name, 0); cout << ":"<< endl;
        DrawDialog_Margin(Player.name, 1); DrawDialog(text, 1); cout << variable; DrawDialog(text2, 1);
        PressEnter();
    }
    if (character == "Enemy"){
        DrawDialog_Margin(Enemy.name, 0); cout << ":"<< endl;
        DrawDialog_Margin(Enemy.name, 1); DrawDialog(text, 1); cout << variable; DrawDialog(text2, 1);
        PressEnter();
    }
}

// Character art initialization
const int CHARACTERS_BORDER = 105;
const int LEFT_CHAR_POS = 0;
const int RIGHT_CHAR_POS = 86;

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

        for (int i = 0; i <= rmg_space; i++){
            cout << ' ';
        }

        cout << endl;

        n++;
        e++;
    }

    cout << endl;
}

const int MID_CHAR_POS = 45;

void printMidCharacter(string char1[], int char1_size){
    for (int i = 0; i < char1_size; i++){
        cout << string(MID_CHAR_POS, ' ') << char1[i] << endl;
    }
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
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

const int DrawBeary_Lines = 15;
string DrawBeary[DrawBeary_Lines] = { // array size is number of lines in the drawing
    "     ,-.____,-. ",
    "    (_c \\   /c_) ",
    "     /  O   O \\      _ ",
    "    (  (._*_,) )    / 7 ",
    "     >._`---',<    / / ",
    "   ,'/  / | \\ \\`. / / ",
    " ,' /   \\_|_/  \\ / / ",
    "(  (            ) /) ",
    " `-'\\          /_/' ",
    "    |`-.____.-'| ",
    "    |     Y    | ",
    "    |     |    | ",
    "    /     |    \\ ",
    "   (______|_____) "
};

const int DrawEnemy1_Lines = 11; // array size is number of lines in the drawing
string DrawEnemy1[DrawEnemy1_Lines] = {
 //Centaur
"  <=======]}=====",
"   --.   /|",
"   _\"/_.'/",
" .'._._,.'",
" :/ \\{}/",
"(L  /--',----._",
"    |          \\",
"   : /-\\ .'-'\\ / |",
"    \\, ||    \\|",
"     \\/ ||    ||",
"       zz    zz"
};
//e3
const int DrawEnemy2_Lines = 7;
string DrawEnemy2[DrawEnemy2_Lines] = {
"        .-'-.",
"      _/.~.~.\\_",
"     ( ( o o ) )",
"      |/  `  \\|   ;;",
"       \\. ^ ./   ((",
"       /`~~~`\\'   ))",
"      /       \\  //"
};

const int DrawEnemy3_Lines = 11;
string DrawEnemy3[DrawEnemy3_Lines]= {
"    cceeoo",
"  C8O8 Q8PoOb",
" dO;69QO8P /7  o8oo",
"Cg(#)bU OU_/OpqO;oUOdb",
"  dT|6uU /pT ug(_)odP",
"     \\\\//   /do;UP",
"      ;\\\\ ////(#)",
"     (#)\\x|x//" ,
"        /|0|/" ,
"        ||||\\" ,
"   ....//||||\\...."
};

//e4
const int DrawEnemy4_Lines = 18;
string DrawEnemy4[DrawEnemy4_Lines]= {
"                                             __----~~~~~~~~~~~------___",
"                                  .  .   ~~//====......          __--~ ~~",
"                  -.            \\_|//     |||\\\\  ~~~~~~::::... /~",
"               ___-==_       _-~o~  \\/    |||  \\            _/~~-",
"       __---~~~.==~||\\=_    -_--~/_-~|-   |\\\\   \\\\        _/~",
"   _-~~     .=~    |  \\\\-_    '-~7  /-   /  ||    \\      /",
" .~       .~       |   \\\\ -_    /  /-   /   ||      \\   /",
"/  ____  /         |     \\\\ ~-_/  /|- _/   .||       \\ /",
"|~~    ~~|--~~~~--_ \\     ~==-/   | \\~--===~~        .\\",
"         '         ~-|      /|    |-~\\~~       __--~~",
"                     |-~~-_/ |    |   ~\\_   _-~            /\\",
"                          /  \\     \\__   \\/~                \\__",
"                      _--~ _/ | .-~~____--~-/                  ~~==.",
"                     ((->/~   '.|||' -_|    ~~-/ ,              . _||",
"                                -_     ~\\      ~~---l__i__i__i--~~_/",
"                                _-~-__   ~)  \\--______________--~~",
"                              //.-~~~-~_--~- |-------~~~~~~~~",
"                                     //.-~~~--\\"
};

const int CreamLines = 11;
string neutralCream[CreamLines] = {
"      .-------.",
"     /         \\ ",
"    /o  o  |   |",
"   ( ~*~   |   |",
"    >-__---|   |",
"   /  /\\   |   / ",
"  /   \\/ ,  \\_/",
" /| ,---'    |",
"/ |/ ,--~    \\ ",
"\\//\\/      .- \\ ",
" /        /    \\ "
};
string happyCream[CreamLines] = {
"      .-------.",
"     /         \\ ",
"    /^  ^  |   |",
"   ( ~*~   |   |",
"    >-__---|   |",
"   /  /\\   |   /",
"  /   \\/ ,  \\_/",
" /| ,---'    |",
"/ |/ ,--~    \\ "
"\\//\\/      .- \\ ",
" /        /    \\ "
};

const int DoorLines = 11;
string Door[DoorLines] = {
" ____________ ",
"|            |",
"|            |",
"|            |",
"|            |",
"|            |",
"|o--         |",
"|            |",
"|            |",
"|            |",
"|____________|"
};

const int TeddyLines = 11;
string neutralTeddy[TeddyLines] = {
"      _    _ ",
"     (()__(() ",
"     /       \\ ",
"    /         \\ ",
"   (  ^  ^     \\ ",
"    \\ O  O     / ",
"    (__*__)___/ \\ ",
"    / _/ | \\___  \\ ",
"   (   | | |      ) ",
"   /\\_/ V V'-.___/\\_ ",
"  / (        /      \\ "

};
string happyTeddy[TeddyLines] = {
"      _    _",
"     (()__(() ",
"     /       \\ ",
"    /         \\ ",
"   (  ^  ^     \\ ",
"    \\ >  <     / ",
"    (__*__)___/ \\ ",
"    / _/ | \\___  \\ ",
"   (   | | |      ) ",
"   /\\_/ V V'-.___/\\_",
"  / (        /      \\ "
};

const int BrownieLines = 11;
string neutralBrownie[BrownieLines] = {
"  __         __",
" / )`------./ )\\ ",
" \\ / -   -   \\ / ",
"  |  O   O    | ",
"  \\ .-----.   / ",
"   '\\_*___/ -' ",
"   | '---'    \\ ",
"  /            \\ ",
" /       ___\\   \\ ",
"( |    (         )",
" \\|     '-------' "
};

string frustBrownie[BrownieLines] = {
"  __         __",
" / )`------./ )\\ ",
" \\ / -   -   \\ / ",
"  |  O   O    | ",
"  \\ .-----.   / ",
"   '\\_*___/ -' ",
"   | '---'    \\ ",
"  /            \\ ",
" /       ___\\   \\ ",
"( |    (         )",
" \\|     '-------' "
};

const int YahooLines = 11;
string neutralYahoo[YahooLines] = {
" _",
"| | ",
"| | ",
"| |  __,------.__ ",
"| | / /_    _  \\ \\  ____ ",
"| | \\/())  ())  \\/ /_|__\\ ",
"| |  \\.-@---.   / |_| /_/ ",
"| |   '--------'  \\_\\ ",
"| |__//    __  \\   \\_\\ ",
"| ||_/    (_|_/ \\__/_/",
"| |  |----------|_|_/ "
};

const int PinkeryLines = 11;
string neutralPinkery[PinkeryLines] = {
"        /)  (\\",
"   .~._((===.))_.~. ",
"    `~. O  O   .~' ",
"      /.+--+.  \\ ",
"     {(.__.  )  } ",
"      )'----'  ( ",
"     /`-._  _ .-\\ ",
"    /   |      _~\\ ",
"  .-X   /  .~~'  X-. ",
" /   \\~'  (     /   \\",
"(     )| | \\| |(     ) "
};

string panicPinkery[PinkeryLines] = {
"        /)  (\\",
"   .~._((===.))_.~. ",
"    `~. ;  ;   .~' ",
"      /.+--+.  \\ ",
"     {( __   )  } ",
"      )'----'  ( ",
"     /`-._  _ .-\\ ",
"    /   |      _~\\ ",
"  .-X   /  .~~'  X-. ",
" /   \\~'  (     /   \\",
"(     )| | \\| |(     ) "
};

string joyPinkery[PinkeryLines] = {
"        /)  (\\",
"   .~._((===.))_.~. ",
"    `~. ^  ^   .~' ",
"      /.+--+.  \\ ",
"     {(.__.  )  } ",
"      )'----'  ( ",
"     /`-._  _ .-\\ ",
"    /   |      _~\\ ",
"  .-X   /  .~~'  X-. ",
" /   \\~'  (     /   \\",
"(     )| | \\| |(     ) "
};

const int BearyLines = 11;
string neutralBeary[BearyLines] = {
"     ,-.____,-.",
"    (_c      c_) ",
"     / O   O  \\ ",
"    ( (._*_,)  ) ",
"     >.`---'_,< ",
"   ,'/ / | \\  \\`. ",
" ,' /  \\_|_/   \\ `. ",
"(  (            )  ) ",
" `-'\\          /`-' ",
"    |`-.____.-'| ",
"    |    Y     | "
};

const int PotatoLines = 10;
string Potato_OvO[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`O   O'  | ",
"/    v     | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_XD[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`>   <'  | ",
"/    U     | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_O_o[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`O _ o'  | ",
"/          | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_cry[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`; _ ;'  | ",
"/          | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_crysmile[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`;   ;'  | ",
"/    v     | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_nervous[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`o _ o'. | ",
"/        , | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

string Potato_bruhsmile[PotatoLines] = {
"_          _",
"\\\\/\\/\\/\\/\\// ",
" \\________/ ",
" /        \\ ",
" |         | ",
" /`=   ='  | ",
"/    v     | ",
"|          | ",
"\\         / ",
" \\_______/ "
};

const int DragonLines = 15;
string Dragon_head[DragonLines] = {
"                /           /",
"               /' .,,,,  ./",
"              /';'     ,/",
"             / /   ,,//,`'`",
"            ( ,, '_,  ,,,' ``",
"            |    /@  ,,, ;' `",
"           /    .   ,''/' `,``",
"          /   .     ./, `,, ` ;",
"       ,./  .   ,-,',` ,,/''\\,'",
"      |   /; ./,,'`,,'' |   |",
"       |     /   ','    /    |",
"        \\___/'   '     |     |",
"          `,,'  |      /     `\\ ",
"              /      |        ~\\ ",
"             '       ("
};

const int PieLines = 6;
string Pie[PieLines] = {
"      +  (      + ",
" +        )  + ",
"    +__..---..__   + ",
" ,-='  /  |  \\  `=-. ",
":--..___________..--; ",
" \\.,_____________,./ "
};

const int MilkLines = 8;
string Milk[MilkLines] = {
"  _____ ",
" |_____| ",
"/_____/_\\ ",
"|M  /*|+| ",
"| I \\_|S| ",
"|_ L  |B| ",
"|*\\ K |+| ",
"|_/___|,' "
};

const int MirrorLines = 19;
string Mirror[MirrorLines] = {
"     ____________________ ",
"    /____________________\\ ",
"   //                    \\\\ ",
"  //  \\\\                  \\\\ ",
" //    `    __,~---,   \\   \\\\ ",
"||      ,-,/        \\   `   || ",
"||     /    .~,      '\\     || ",
"||    /  /\\/^--}_      \\    || ",
"||    } |  -     }__,  {    || ",
"||   <_,/  O     O  \\  _7   || ",
"||     \\\\     _      }/     || ",
"||      \\\\__________// .    || ",
"||     ____/|    |\\____ \\\\  || ",
"||.   /    \\`\\__/'/    \\  \\ || ",
"|| \\ |      \\/O|\\/      |   || ",
"\\\\   |   |     |    |   |   // ",
" \\\\  |   |     |    |   |  // ",
"  \\\\______________________// ",
"   \\______________________/ "
};





void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy){
    system("cls");
    printCharacters(Player.drawing,Player.drawingLines,Enemy.drawing,Enemy.drawingLines);

    string PlayerHealth = to_string(Player.health) + "/" + to_string(Player.maxHealth);
    string EnemyHealth = to_string(Enemy.health) + "/" + to_string(Enemy.maxHealth);

    int space = BORDER_WIDTH - Player.name.length() - PlayerHealth.length() - EnemyHealth.length() - Enemy.name.length();

    cout << Player.name << " " << PlayerHealth << string(space, ' ') << EnemyHealth << " " << Enemy.name << endl;

    Player.PlayerHealthBar(); cout << "   "; Enemy.EnemyHealthBar(); cout << endl;
    cout << endl;
    cout << endl;
}

void DrawChacters_AND_HealthBar(Player &Player, Enemy &Enemy, int int_damagetext_P, int int_damagetext_E, string sign){
    system("cls");
    printCharacters(Player.drawing,Player.drawingLines,Enemy.drawing,Enemy.drawingLines);

    string PlayerHealth = to_string(Player.health) + "/" + to_string(Player.maxHealth);
    string EnemyHealth = to_string(Enemy.health) + "/" + to_string(Enemy.maxHealth);

    string str_damagetext_P;
    string str_damagetext_E;

    if (int_damagetext_P == 0){
        str_damagetext_P = " ";
    }
    else{
        str_damagetext_P = sign + to_string(int_damagetext_P);
    }

    if (int_damagetext_E == 0){
        str_damagetext_E = " ";
    }
    else{ 
        str_damagetext_E = sign + to_string(int_damagetext_E);
    }

    int ALLIGN_DAMAGETEXT_P = 36;
    int ALLIGN_DAMAGETEXT_E = 6;

    int space = BORDER_WIDTH - Player.name.length() - PlayerHealth.length() - EnemyHealth.length() - Enemy.name.length() - ALLIGN_DAMAGETEXT_P - ALLIGN_DAMAGETEXT_E - str_damagetext_P.length() - str_damagetext_E.length();

    cout << Player.name << " " << PlayerHealth << string(ALLIGN_DAMAGETEXT_P, ' ') << str_damagetext_P << string(ALLIGN_DAMAGETEXT_E, ' ') << str_damagetext_E << string(space, ' ') << EnemyHealth << " " << Enemy.name << endl;

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


int const MID_MARGIN = 40;

void menuselectionbox(string slot[], int slotsize){

    int menuMargin = 17;

    // Draw the menu

    int n = 0;
    cout << string(MID_MARGIN, ' ') << "+-------------------------+" << endl;

    for (int i = 0; i < slotsize; i++ ){
        int rmg_space = static_cast<int>(menuMargin - slot[n].length());

        cout << string(MID_MARGIN, ' ') << "|    ";
        if (slot[n].find('>') < 1){
            White_HL(slot[n]);
        }
        else{
            cout << slot[n];
        }
        if (rmg_space > 0){
            cout << string(rmg_space, ' ');
        }
        cout << "    |" << endl;
        n++;
    }

    cout << string(MID_MARGIN, ' ') << "+-------------------------+" << endl;

}


void DrawStoryMenu(string move, string array[], int arraysize, string *character, int charsize, string text){


    system("cls");

    string c_array[4];

    for (int i = 0; i < arraysize; i++){
        c_array[i] = array[i];
    }

    // Increase/decrease z based on input
    if (move == "DOWN" && z >= 0 && z < arraysize - 1){
        z++;
    }
    if (move == "UP" && z > 0 && z <= arraysize - 1){
        z--;
    }

    switch(z){
        case 0:
            c_array[0] = ">" + array[0];
            break;
        case 1:
            c_array[1] = ">" + array[1];
            break;
        case 2:
            c_array[2] = ">" + array[2];
            break;
        case 3:
            c_array[3] = ">" + array[3];
            break;
        default:
            break;
    }

    printMidCharacter(character,charsize);
    cout << string(MID_MARGIN, ' ') << text << endl;
    menuselectionbox(c_array, arraysize);
}


string optionselect(string text, string array[], int arraysize, string *character, int charsize){

    
    printMidCharacter(character,charsize);
    cout << string(MID_MARGIN, ' ') << text << endl;
    DrawStoryMenu(" ", array, arraysize, character, charsize, text);

    while(true){
        switch(getch()){
        case KEY_UP:
            DrawStoryMenu("UP", array, arraysize, character, charsize, text);
            break;
        case KEY_DOWN:
            DrawStoryMenu("DOWN", array, arraysize, character, charsize, text);
            break;
        case KEY_ENTER:
            return array[z];
            break;
        default:
            break;
        }
    }
}

void Bedroom(Player &Player){

    DrawDialog("You wake up in an unfamiliar room. It's filled with a lot of cute furniture and decorations. "
               "The bed you're on feels very soft and comfortable as if made of fluff. ", 1);
    DrawDialog("\n\nYou could not remember what happened, or how you even got here. The only thing you're certain of is that your head hurts A LOT. ", 1);
    DrawDialog("\n\nIn your state of confusion, a soft-looking bunny comes into the room. \n\n", 1);
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "???" << endl;
    DrawDialog_Margin(" 'Oh! You're finally awake! '\n\n", 2);
    DrawDialog_Margin("The bunny said, hopping excitedly towards you.\n\n", 1);
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "???" << endl;
    DrawDialog_Margin("'My husband and I found you sleeping on the ground at the Berry Garden.' \n", 2);
    DrawDialog_Margin("Thank goodness no wild Prunicus got to you before we did.\n", 2);
    DrawDialog_Margin("'What's your name, child?'\n", 2);
    DrawDialog_Margin("Your name: ", 2);
    getline (cin, Player.name);

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "???" << endl;
    DrawDialog_Margin("???: ", 1); cout << Player.name; DrawDialog("! What a lovely name!'\n\n", 2);
    DrawDialog_Margin("The bunny jumps in joy.\n\n", 1);
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl;

    cout << endl;
    DrawDialog_Margin("Cream: 'I'm Cream! My husband is downstairs having lunch. You should join us when you're ready.' \n\n", 2 );
    DrawDialog_Margin("The bunny hops out of the room.\n\n", 1);
    cout << endl << endl;
    system("pause");

    system("cls");

    string option1[4] = {"Go back to sleep","Check the mirror", "Use the bathroom", "Go downstairs"};
    string choice = optionselect("What would you like to do?", option1, 4, Door, DoorLines);

    if (choice == "Go back to sleep"){
        DrawDialog("You decided to lie down for a few more minutes. ", 1);
    }
    else if (choice == "Check the mirror"){
        DrawDialog("You look like your normal self, just a bit more well-rested than usual. Perhaps you needed that sleep more than anything.", 1);
    }
    else if (choice == "Use the bathroom"){
        DrawDialog("You feel refreshed.", 1);
    }
    else if (choice == "Go downstairs"){
        DrawDialog("You got off the bed and exited the room. ", 1);
        DrawDialog("\nAs you walked downstairs, you noticed a few portraits of a happy family. One of the portraits was of Cream, a bear, and a bear cub. ", 1);
    }
    PressEnter();
    //DiningRoom(Player);
}

int main(){

    srand(time(0));

    Player p1("Player",100,DrawPlayer,DrawPlayer_Lines);
    Bedroom(p1);

    //int enemyIndex = 1;

    Enemy e1("Soldier",100,DrawEnemy1, DrawEnemy1_Lines);

    bool gameloop = true;

    while (gameloop){
    KeySwitch(p1,e1);
        if (p1.health <= 0){
            DrawChacters_AND_HealthBar(p1,e1);
            PlaceDialog(p1, e1, "Player", " lost!");
            gameloop = false;
        }
        if (e1.health <= 0){
            DrawChacters_AND_HealthBar(p1,e1);
            PlaceDialog(p1, e1, "Enemy", " lost!");
            gameloop = false;
        }
    }

    return 0;
}
