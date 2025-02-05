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
void House();
void TrainingQuest(Player &Player, Enemy &Enemy);
void TrainingGrounds(Player &Player);
void MarketQuest(Player &Player, Enemy &Enemy);
void Market();
void BerryQuest(Player &Player, Enemy &Enemy);



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
"     --.   /|",
"   _\\''/_.'/",
" .'._._,.'",
" :/ \\{}/",
"(L  /--',----._",
"    |          \\\\",
"   : /-\\ .'-'\\ / |",
"    \\\\, ||    \\|",
"     \\/ ||    ||",
"        zz    zz"
};
//e3
const int DrawEnemy2_Lines = 7;
string DrawEnemy2[DrawEnemy2_Lines] = {
"        .-'-.",
"      _/.~.~.\\_",
"     ( ( o o ) )  _",
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
"/ |/ ,--~    \\ ",
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
" \\ / \\   /   \\ / ",
"  |  =   =    | ",
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

const int FoodLines = 6;
string Food[FoodLines] = {
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

const int StickLines = 11;
string Stick[StickLines] = {
"            _+",
"     +     / 7  +",
"          / /",
"         / / +",
"      + / /",
"       / /     +",
" +    / /",
"     / /   +",
"    / /",
"  +/ /   +",
"  /_/ "
};

const int BasketLines = 8;
string Basket[BasketLines] = {
"     ..",
"     |\\\\",
".====^^\\\\==.",
"|\\     ||   \\ ",
":\"====^^===='",
" \\|---------|",
"  '''''''''''"
};

string BasketBerries[BasketLines] = {
"     ..",
"     |\\\\",
".====^^\\\\==.",
"|\\oO0o8||O8o\\ ",
":\"====^^===='",
" \\|---------|",
"  '''''''''''"
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

const int EmptyLines = 11;
string Empty[EmptyLines] = {
" "
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

    int menuMargin = 5;
    int max = 0;
    int borderLength;

    for (int i = 0; i < slotsize; i++) {
        int len = slot[i].length();
        if (slot[i][0] == '>') {
            len--;  
        }
        if (len > max) {
            max = len;
        }
    }   
    borderLength = max + menuMargin * 2; 


    // Draw the menu

    cout << string(MID_MARGIN, ' ') << "+" << string(borderLength, '-') << "+" << endl;

    for (int i = 0; i < slotsize; i++ ){
        int rmg_space = static_cast<int>(borderLength - menuMargin - slot[i].length() - 1);

        cout << string(MID_MARGIN, ' ') << "|" << string(menuMargin, ' ');
        if (slot[i].find('>') < 1){
            White_HL(slot[i]);
        }
        else{
            cout << slot[i];
        }
        if (rmg_space > 0){
            cout << string(rmg_space + 1, ' ');
        }
        cout << "|" << endl;
    }

    cout << string(MID_MARGIN, ' ') << "+" << string(borderLength, '-') << "+" << endl;

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

    int n;
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
            n = z;
            z = 0;
            return array[n];
            break;
        default:
            break;
        }
    }
}

string choice;

void Bedroom(Player &Player){

    DrawDialog("You wake up in an unfamiliar room. \nIt's filled with a lot of cute furniture and decorations. "
               "\nThe bed you're on feels very soft and comfortable as if made of fluff. ", 1);
    DrawDialog("\n\nYou could not remember what happened, or how you even got here. \nThe only thing you're certain of is that your head hurts A LOT. ", 1);
    DrawDialog("\n\nIn your state of confusion, a soft-looking bunny comes into the room. \n\n", 1);
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("Oh! You're finally awake! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

        system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("The bunny said, hopping excitedly towards you.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("My husband and I found you sleeping on the ground at the Berry Garden. \n\n", 2);
    DrawDialog_Margin("Thank goodness no wild Prunicus got to you before we did.\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("What's your name, child?\n\n", 2);
    DrawDialog_Margin("Your name: ", 2);
    getline (cin, Player.name);

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("", 1); cout << Player.name; DrawDialog("! What a lovely name!\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("The bunny jumps in joy.", 1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl;
    cout << endl;
    DrawDialog_Margin("I'm Cream! My husband is downstairs having lunch. You should join us when you're ready. \n\n", 2 );
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("The bunny hops out of the room.", 1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");


    do{
    system("cls");
    string option1[4] = {"Go back to sleep", "Check the mirror", "Use the bathroom", "Go downstairs"};
    choice = optionselect("What would you like to do?", option1, 4, Door, DoorLines);

    if (choice == "Go back to sleep"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You decided to lie down for a few more minutes. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    }
    else if (choice == "Check the mirror"){
        system("cls");
        printMidCharacter(Mirror,MirrorLines);
        cout << endl;
        DrawDialog_Margin("You look like your normal self, just a bit more well-rested than usual.\n\n", 1);
        DrawDialog_Margin("Perhaps you needed that sleep more than anything.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    }
    else if (choice == "Use the bathroom"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You feel refreshed.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    }
    else if (choice == "Go downstairs"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You got off the bed and exited the room. \n\n", 1);
        DrawDialog_Margin("As you walked downstairs, you noticed a few portraits of a happy family. \n\n", 1);
        DrawDialog_Margin("One of the portraits was of Cream, a bear, and a bear cub. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    }
    system("pause");
    }while (choice != "Go downstairs");

}

void DiningRoom(Player &Player){
    
    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You walk into the dining room and see Cream and the bear from the portrait sitting at the \n\n", 1);
    DrawDialog_Margin("dining table.", 1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << string(50, ' '); cout << "???" << endl;
    cout << endl;
    DrawDialog_Margin("Ah! Good afternoon, kiddo! Here I thought you weren't going to wake up.\n\n", 2);
    DrawDialog_Margin("The bear laughs heartily at his own remark.", 1 );
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << string(50, ' '); cout << "Teddy" << endl;
    cout << endl;
    DrawDialog_Margin("I'm Teddy! My wife tells me your name is " , 2); cout << Player.name;
    DrawDialog(". Come, let's eat lunch together. \n\n", 2);
    DrawDialog_Margin("You must be starving after sleeping for so long." , 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralTeddy,TeddyLines);
    cout << endl;
    DrawDialog_Margin("You took a seat at the wooden dining table with Teddy.\n\n", 1);
    DrawDialog_Margin("The table was sturdy and perfect for hosting family gatherings.",1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Food,FoodLines);
    cout << endl;
    DrawDialog_Margin("Cream opens the oven and brings out a golden-crusted blueberry pie.\n\n", 1);
    DrawDialog_Margin("She puts the blueberry pie on the table.",1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Food,FoodLines);
    cout << string(50, ' '); cout << "Teddy" << endl;
    cout << endl;
    DrawDialog_Margin("Eat up, ", 2); cout << Player.name; DrawDialog("! My wife makes the BEST pies!", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Food,FoodLines);
    cout << endl;
    DrawDialog_Margin("Cream takes a seat next to him. ", 1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Food,FoodLines);
    cout << endl;
    DrawDialog_Margin("You take a slice of pie and take a bite. The buttery and flaky crust melts \n\n", 1);
    DrawDialog_Margin("in your mouth.",1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Food,FoodLines);
    cout << endl;
    DrawDialog_Margin("The blueberry is sweet, tangy, bursting with flavor as if they were picked at \n\n",1);
    DrawDialog_Margin("the perfect moment.", 1);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");
    
    system("cls");
    printMidCharacter(Food,FoodLines);    
    cout << endl;
    DrawDialog_Margin("Just like how mother used to make it.", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option1[3] = {"It's pulchritudinous.","It tastes good!", "Wow... This pie slaps."};
    optionselect("Cream: So how's the pie?", option1, 3, neutralCream, CreamLines);

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl;    
    cout << endl;
    DrawDialog_Margin("I'm glad you think so!", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("Cream is overjoyed by your response.", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << string(50, ' '); cout << "Teddy" << endl;
    cout << endl;
    DrawDialog_Margin("Teddy wipes the blueberry puree off his face and hands. ", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option2[3] = {"I'm lost.","I actually don't know where I am.", "I live around the corner."};
    choice = optionselect("Teddy: So where are you from? ", option2, 3, neutralTeddy,TeddyLines);
    
    if (choice == "I'm lost."){
        system("cls");
        printMidCharacter(neutralTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("Lost? So, you don't even know where you are!", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
    }
    else if(choice == "I actually don't know where I am."){
        system("cls");
        printMidCharacter(neutralTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("You don't? So, We basically just kidnapped ya!", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
    }
    else if(choice == "I live around the corner."){
        system("cls");
        printMidCharacter(neutralTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("Quit pulling our leg here, kid! I remember every face in this neighborhood, \n\n", 2);
        DrawDialog_Margin("and I sure haven't seen you around here in all my years...", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(happyTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("How long have we lived here again?", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("Teddy scratches his head trying to remember.", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(happyTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl;
        cout << endl;
        DrawDialog_Margin("'Ah! That doesn't matter right now!'\n\n", 2);
        DrawDialog_Margin("Teddy says waving his bear paws.", 2);
        cout << endl << endl;
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
    }

    system("cls");
    printMidCharacter(neutralTeddy,TeddyLines);
    cout << string(50, ' '); cout << "Teddy" << endl;
    cout << endl;
    DrawDialog_Margin("Well, you're in the Potato Kingdom!", 2);
    cout << endl << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("The Potato Kingdom? Does such a place even exist?\n\n", 2);
    DrawDialog_Margin("This all seems too unreal, a talking bunny and a bear. \n\n", 2);
    DrawDialog_Margin("Not to mention you're eating blueberry pie with them.  \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option3[1] = {"How do I get home?"};
    choice = optionselect(" ", option3, 1, neutralTeddy, TeddyLines);

    system("cls");
    printMidCharacter(neutralTeddy,TeddyLines);
    cout << string(50, ' '); cout << "Teddy" << endl << endl;
    DrawDialog_Margin("What are ya asking us for? We don't even know where ya from!\n\n", 2);
    DrawDialog_Margin("You could try asking around the kingdom, though. \n\n", 2);
    DrawDialog_Margin("I'm sure someone here knows about transportation. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << endl;
    DrawDialog_Margin("Teddy laughs heartily.\n\n", 2);
    DrawDialog_Margin("Teddy and Cream don't seem to know much about where you're from, \n\n", 2);
    DrawDialog_Margin("nor how you ended up in this situation. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
      cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("Are you healthy enough to be going so soon?\n\n", 2);
    DrawDialog_Margin("If you'd like to rest here some more, we don't mind! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("The room you were sleeping in used to be our son's, but he moved out recently. \n\n", 2);
    DrawDialog_Margin("We prepare the room for him when he returns from time to time \n\n", 2);
    DrawDialog_Margin("we don't think he'll return until next month. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << endl;
    DrawDialog_Margin("Cream speaks of their son fondly.\n\n", 2);
    DrawDialog_Margin("Perhaps the bear cub from the portrait is their son. \n\n", 2);
    DrawDialog_Margin("He must be old enough to be living on his own now. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option4[1] = {"Thank you for your hospitality. But I should really get home."};
    choice = optionselect(" ", option4, 1, neutralCream, CreamLines);

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("Alrighty then! Stay safe out there!\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << string(50, ' '); cout << "Teddy" << endl << endl;
    DrawDialog_Margin("Safe travels, kiddo!\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyTeddy,TeddyLines);
    cout << endl;
    DrawDialog_Margin("Teddy laughs heartily once again.\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option5[2] = {"Stay here a bit longer","Leave the house"};
    choice = optionselect(" ", option5, 2, neutralCream, CreamLines);

    if (choice == "Stay here a bit longer"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You look around Cream and Teddy's house a little longer.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
        House();
    }
    else if (choice == "Leave the house"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You leave the humbe abode.\n\n", 1);
        DrawDialog_Margin("Teddy and Cream wave at you goodbye.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
    }

}


void House(){
    //What do you want to do in the house?
    do{
    string option1[4] = {"Talk to Teddy","Talk to Cream", "Upstairs", "Leave the house"};
    choice = optionselect("What would you like to do?", option1, 4, Empty, EmptyLines);

    //Talk to Teddy
    if (choice == "Talk to Teddy"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You go to the living room to talk to Teddy. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("The living room has three comfortable-looking couches,\n\n", 1);
        DrawDialog_Margin("one longer couch facing the television,\n\n", 1);
        DrawDialog_Margin("while the other two face each other.  \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("The wall behind the long couch has a couple of watercolor paintings.\n\n", 1);
        DrawDialog_Margin("There are drawers on each side of the long couch, \n\n", 1);
        DrawDialog_Margin("both with small portraits of happy family photos on them.  \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You see Teddy attentively watching the kingdom news on the television.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
        
        system("cls");
        printMidCharacter(happyTeddy,TeddyLines);
        cout << string(50, ' '); cout << "Teddy" << endl << endl;
        DrawDialog_Margin("Hey kiddo! Back already? The kingdom is pretty big.\n\n", 2);
        DrawDialog_Margin("I'm sure you have a lot of places to wander around!  \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        //What do you want to talk about with Teddy?
        do{
        system("cls");
        string option2[4] = {"Kingdom affairs","Watercolor paintings", "Family photo", "Goodbye!"};
        choice = optionselect("", option2, 4, neutralTeddy, TeddyLines);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;

        if (choice == "Kingdom affairs"){
            system("cls");
            printMidCharacter(neutralTeddy,TeddyLines);
            cout << string(50, ' '); cout << "Teddy" << endl << endl;
            DrawDialog_Margin("Seems like the prices for berries have increased again. \n\n", 2);
            DrawDialog_Margin("It's better to just pick the berries off the bushes ourselves! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(happyTeddy,TeddyLines);
            cout << string(50, ' '); cout << "Teddy" << endl << endl;
            DrawDialog_Margin("HARHARHAR! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Watercolor paintings"){
            system("cls");
            printMidCharacter(neutralTeddy,TeddyLines);
            cout << string(50, ' '); cout << "Teddy" << endl << endl;
            DrawDialog_Margin("Oh those watercolor paintings? The frog triplets made those. \n\n", 2);
            DrawDialog_Margin("Talented bunch, that's for sure! \n\n", 2);
            DrawDialog_Margin("If only I could tell the difference between the three... \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Family photo"){
            system("cls");
            printMidCharacter(happyTeddy,TeddyLines);
            cout << string(50, ' '); cout << "Teddy" << endl << endl;
            DrawDialog_Margin("Ah~ Those were the days when our little cub was... well, little! \n\n", 2);
            DrawDialog_Margin("He's all grown up now, living on his own and such!  \n\n", 2);
            DrawDialog_Margin("It's enough to make a grown bear cry! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Goodbye!"){
            system("cls");
            printMidCharacter(happyTeddy,TeddyLines);
            cout << string(50, ' '); cout << "Teddy" << endl << endl;
            DrawDialog_Margin("See ya later, kiddo! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        system("pause");
        }while (choice != "Goodbye!");
    }

    //Talk to Cream
    else if (choice == "Talk to Cream"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You go to the kitchen to talk to Cream. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("The kitchen is clean and tidy. \n\n", 1);
        DrawDialog_Margin("It seems like Cream has already cleaned up after baking the delicious blueberry pie.\n\n", 1);
        DrawDialog_Margin("You see various utensils and cutleries around, \n\n", 1);
        DrawDialog_Margin("all meant to prepare specific dishes and desserts. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("There is a drawing on the fridge, \n\n", 1);
        DrawDialog_Margin("nicely placed with cute berry-shaped magnets holding them up. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("Cream is munching on a carrot next to the fridge. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
        
        system("cls");
        printMidCharacter(happyCream,CreamLines);
        cout << string(50, ' '); cout << "Cream" << endl << endl;
        DrawDialog_Margin("Is there anything you need? \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        //What do you want to talk about with Cream?
        do{
        system("cls");
        string option2[4] = {"What's for dinner?","The drawing", "The magnets", "Goodbye!"};
        choice = optionselect("", option2, 4, neutralCream, CreamLines);

        if (choice == "What's for dinner?"){
            system("cls");
            printMidCharacter(neutralCream,CreamLines);
            cout << string(50, ' '); cout << "Cream" << endl << endl;
            DrawDialog_Margin("Hungry for dinner already?  \n\n", 2);
            DrawDialog_Margin("We're having berry salad for dinner!  \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(happyCream,CreamLines);
            cout << string(50, ' '); cout << "Cream" << endl << endl;
            DrawDialog_Margin("A wide variety of berries with some green! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "The drawing"){
            system("cls");
            printMidCharacter(happyCream,CreamLines);
            cout << string(50, ' '); cout << "Cream" << endl << endl;
            DrawDialog_Margin("My dear son made it when he was a little cub. \n\n", 2);
            DrawDialog_Margin("It was so cute, I just had to hang it up! \n\n", 2);
            DrawDialog_Margin("It's been up there ever since. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "The magnets"){
            system("cls");
            printMidCharacter(happyCream,CreamLines);
            cout << string(50, ' '); cout << "Cream" << endl << endl;
            DrawDialog_Margin("Aren't they cute? \n\n", 2);
            DrawDialog_Margin("They're all different types of berries, \n\n", 2);
            DrawDialog_Margin("all our favorite kinds! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Goodbye!"){
            system("cls");
            printMidCharacter(happyCream,CreamLines);
            cout << string(50, ' '); cout << "Cream" << endl << endl;
            DrawDialog_Margin("You're always welcome here. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        system("pause");
        }while (choice != "Goodbye!");
    }

    //Upstairs
    else if (choice == "Upstairs"){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("You go upstairs. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("The room is filled with a lot of cute furniture and decorations. \n\n", 1);
        DrawDialog_Margin("The bed is very soft and comfortable as if made of fluff. \n\n", 1);
        DrawDialog_Margin("The dresser has a mirror big enough to see your entire upper body, clean and shiny.  \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("The bathroom is attached to the room, \n\n", 1);
        DrawDialog_Margin("making it convenient when you really need to go handle business. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        //What do you want to talk about with Cream?
        do{
        system("cls");
        string option1[4] = {"Take a nap", "Check the mirror", "Use the bathroom", "Go downstairs"};
        choice = optionselect("What would you like to do?", option1, 4, Door, DoorLines);

        if (choice == "Take a nap"){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You laid down on the comfortable bed and took a nap. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Check the mirror"){
            system("cls");
            printMidCharacter(Mirror,MirrorLines);
            cout << endl;
            DrawDialog_Margin("You look like your normal self.\n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Use the bathroom"){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You feel refreshed.\n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Go downstairs"){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You exited the room. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        system("pause");
        }while (choice != "Go downstairs");
    }

    }while (choice != "Leave the house");

    //Leave the house
    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You leave the humbe abode.\n\n", 1);
    DrawDialog_Margin("Teddy and Cream wave at you goodbye.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

}

void Outside(Player &Player){
    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You step outside and are met with a wave of cool, clean air.  \n\n", 1);
    DrawDialog_Margin("The sun is shining brightly and the birds are chirping in songs. \n\n", 1);
    DrawDialog_Margin("You feel a sense of peace and tranquility. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("", 1); cout << Player.name; DrawDialog("! Before you go, could you do me a favor? \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("Here's a map of the Potato Kingdom. \n\n", 2);
    DrawDialog_Margin("I labeled them for you so that you know where to go. \n\n", 2);
    DrawDialog_Margin("You can get berries at the Berry Garden.  \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("But be careful, wild Prunicus are lurking around there sometimes. \n\n", 2);
    DrawDialog_Margin("You can try asking around the kingdom for any clues on how you can get home.  \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("I marked where our home is too so that you can come back anytime! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option1[1] = {"Thank you."};
    choice = optionselect(" ", option1, 1, neutralCream, CreamLines);

    system("cls");
    printMidCharacter(happyCream,CreamLines);
    cout << string(50, ' '); cout << "Cream" << endl << endl;
    DrawDialog_Margin("Take care on your journey! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("Cream hops back into her humble home. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

}

bool TGQ_Complete = false;
int TGQ_Counter = 0;
void TrainingGroundsQuest(Player &Player, Enemy &Enemy){
    if (TGQ_Counter == 0){
        DrawDialog("The training ground is a wide, open area. \nArchery targets and climbing scaffolds are set up on one side. "
                   "\nTraining weapons and equipment are provided on their respective racks, waiting to be used. ", 1);
        DrawDialog("\n\nThis place seems to have more people than the gymnasium at home. \nSome are training together while some are training amongst themselves.", 1);
        DrawDialog("\n\nA lemur overseeing the bunch who are together stands out to you.", 1);
        DrawDialog("\nYou decide to approach the lemur.\n\n", 1);
        TGQ_Counter++;

        system("pause");

        system("cls");
        string option1[1] = {"Excuse me."};
        choice = optionselect("...", option1, 1, neutralYahoo, YahooLines);

        system("cls");
        string option2[3] = {"My name is " + Player.name + ".","I'd rather not say.","You don't need to know."};
        choice = optionselect("Hm? Who might you be?", option2, 3, neutralYahoo, YahooLines);

        if (choice == "I'd rather not say." || choice == "You don't need to know."){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "???" << endl << endl;
            DrawDialog_Margin("Playing mysterious, are you? \n\n", 2);
            DrawDialog_Margin("I'll find out one way or another. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }

        system("cls");
        printMidCharacter(neutralYahoo,YahooLines);
        cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
        DrawDialog_Margin("I'm Commander Yahoo. \n\n", 2);
        DrawDialog_Margin("Me and my soldiers came here to sharpen our skills wherever we lacked. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralYahoo,YahooLines);
        cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
        DrawDialog_Margin("Surely, you're here to train too. \n\n", 2);
        DrawDialog_Margin("These training grounds are open to everyone, after all, Papadum Army or not. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option3[1] = {"Do you know how I can get home?"};
        choice = optionselect("", option3, 1, neutralYahoo, YahooLines);

        system("cls");
        printMidCharacter(neutralYahoo,YahooLines);
        cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
        DrawDialog_Margin("A way home? What an odd thing to ask a stranger. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralYahoo,YahooLines);
        cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
        DrawDialog_Margin("Hmmm... But I have to keep an eye on my troops to make sure they aren't slacking off.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralYahoo,YahooLines);
        cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
        DrawDialog_Margin("Do you know how to fight? How about you go and spar with one of my soldiers \n\n", 2);
        DrawDialog_Margin("and motivate them to train harder?\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option4[2] = {"Sure! I'll fight them right now!","I'll think about it."};
        choice = optionselect("", option4, 2, neutralYahoo, YahooLines);

        //Accepts quest and battle
        if (choice == "Sure! I'll fight them right now!"){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Alright everyone! I need someone to spar with. \n\n", 2);
            DrawDialog_Margin("Show me if you can beat this random person.\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
    
        //Battle happens here
        bool gameloop = true;
        while (gameloop){
        KeySwitch(Player,Enemy);
        if (Player.health <= 0){
            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Player", " lost!");
            gameloop = false;
        }
        if (Enemy.health <= 0){
            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Enemy", " lost!");
            gameloop = false;
            TGQ_Complete = true;
        }
        }

        }
        if (choice == "I'll think about it."){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Coward.\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
    }

    if  (TGQ_Complete == false && TGQ_Counter > 0){
        do{
        string option6[2] = {"Talk to Commander Yahoo","Leave the training grounds"};
        choice = optionselect("What do you want to do?", option6, 2, Empty, EmptyLines);

        if (choice == "Talk to Commander Yahoo"){
            system("cls");
            string option5[2] = {"Sure! I'll fight them right now!","I'll think about it."};
            choice = optionselect("Wanna spar with my soldiers?", option5, 2, neutralYahoo, YahooLines);

            //Accepts quest and battle
            if (choice == "Sure! I'll fight them right now!"){
                system("cls");
                printMidCharacter(neutralYahoo,YahooLines);
                cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
                DrawDialog_Margin("Alright everyone! I need someone to spar with. \n\n", 2);
                DrawDialog_Margin("Show me if you can beat this random person.\n\n", 2);
                cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
                system("pause");
    
            //Battle happens here
            bool gameloop = true;
            while (gameloop){
            KeySwitch(Player,Enemy);
            if (Player.health <= 0){
                DrawChacters_AND_HealthBar(Player,Enemy);
                PlaceDialog(Player, Enemy, "Player", " lost!");
                gameloop = false;
            }
            if (Enemy.health <= 0){
                DrawChacters_AND_HealthBar(Player,Enemy);
                PlaceDialog(Player, Enemy, "Enemy", " lost!");
                gameloop = false;
                TGQ_Complete = true;

                system("cls");
                printMidCharacter(Empty,EmptyLines);
                cout << endl;
                DrawDialog_Margin("Papadum Soldier has been defeated.\n\n", 2);
                cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
                system("pause");
            }
            }
            
            //Battle ends
            //if (TGQ_Complete == true){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Well, you're a far better fighter than I thought! \n\n", 2);
            DrawDialog_Margin("Now I can bring this moment up whenever they start slacking off.\n\n", 2);
            DrawDialog_Margin("Have you ever considered joining the Papadum Army?\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Oh wait, you were looking for a way home, right? \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            string option7[3] = {"It's not in this world.","I don't know.","Not here."};
            choice = optionselect("Where is your home?", option7, 3, neutralYahoo, YahooLines);

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Gee, I can't give you directions without an address. \n\n", 2);
            DrawDialog_Margin("Maybe this could help you out instead. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Stick,StickLines);
            cout << endl;
            DrawDialog_Margin("Obtained [Cool Stick]! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("That's all the help I can give you. \n\n", 2);
            DrawDialog_Margin("Maybe you could use the teleporter at the Potato Palace. \n\n", 2);
            DrawDialog_Margin("Not sure if it works for otherworldly destinations though. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
            //}

            if (choice == "I'll think about it."){
                system("cls");
                printMidCharacter(neutralYahoo,YahooLines);
                cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
                DrawDialog_Margin("Coward.\n\n", 2);
                cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
                system("pause");
            }
        }
        }
        }while (choice != "Leave the training grounds" && TGQ_Complete != true);


        //Leave the Training Grounds
        if  (choice == "Leave the training grounds" && TGQ_Complete == false && TGQ_Counter > 0){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You leave the training grounds.\n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
        if  (choice == "Leave the training grounds" && TGQ_Complete == true){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << endl;
            DrawDialog_Margin("Yahoo nods his head in respect as you leave.\n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
    }
    if (TGQ_Complete == true){
        TrainingGrounds(Player);
    }
}

void TrainingGrounds(Player &Player){
    do{
    string option1[3] = {"Talk to Commander Yahoo","Check on the fainted Papadum Soldier", "Leave the Training Grounds"};
    choice = optionselect("What would you like to do?", option1, 3, Empty, EmptyLines);

        if (choice == "Talk to Commander Yahoo"){
        //What do you want to talk about with Yahoo?
        do{
        system("cls");
        string option2[4] = {"The Papadum Army","Training", "Teleporter", "Goodbye!"};
        choice = optionselect("Need anything, "+ Player.name +"?", option2, 4, neutralYahoo, YahooLines);

        if (choice == "The Papadum Army"){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("The Papadum Army serves to protect the Potato Kingdom. \n\n", 2);
            DrawDialog_Margin("We never had the chance to actually fight a war \n\n", 2);
            DrawDialog_Margin("since the Potato Kingdom has always been peaceful. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("We mostly goof around and make jokes. \n\n", 2);
            DrawDialog_Margin("Training is serious though. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Training"){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("We train whenever we can. \n\n", 2);
            DrawDialog_Margin("The public likes to join us whenever we train so we prefer training here.\n\n", 2);
            DrawDialog_Margin("Plus, maybe we could recruit potential soldiers from the civilians. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("The soldiers being beaten by regular civilians is pretty humbling too,\n\n", 2);
            DrawDialog_Margin("it becomes great motivation for them to train harder. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Pride and ego could be great motivators if you know how to use them right. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Teleporter"){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("The Potato Queen likes to teleport from place to place in the kingdom. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("She could walk, \n\n", 2);
            DrawDialog_Margin("but 'a potato can only walk so far' she says. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("She's probably just lazy. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Goodbye!"){
            system("cls");
            printMidCharacter(neutralYahoo,YahooLines);
            cout << string(50, ' '); cout << "Commander Yahoo" << endl << endl;
            DrawDialog_Margin("Good luck finding your way. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        system("pause");
        }while (choice != "Goodbye!");
        }

        else if (choice == "Check on the fainted Papadum Soldier"){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You walk over to the fainted Papadum Soldier. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("A few of the other Papadum Soldiers surround the Papadum Soldier you defeated, \n\n", 1);
            DrawDialog_Margin("who is now fainted on the ground. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("Surely, he's fine. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
    }while (choice != "Leave the Training Grounds");

    //Leave the Training Grounds
    system("cls");
    printMidCharacter(neutralYahoo,YahooLines);
    cout << endl;
    DrawDialog_Margin("Yahoo nods his head in respect as you leave.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");
}

bool MQ_Complete = false; 
void MarketQuest(Player &Player, Enemy &Enemy){
    DrawDialog("The market is bustling with people. \nMany stalls sell a variety of products. "
                "\nSome are selling clothing, some are selling food, some are selling items, \nand some others are selling nick-nacks that might not make any sense to you.", 1);
    DrawDialog("\n\nIt seems they are of use to the people of this world, \nso you probably shouldn't ponder on it too much.", 1);
    DrawDialog("\n\nJust as you were walking around, suddenly a shout was heard from afar.\n\n", 1);
    system("pause");

    system("cls");
    printMidCharacter(panicPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("THIEF! SOMEONE STOP THAT THIEF! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(panicPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("A pink cow is shouting for someone to catch the thief they're chasing. \n\n", 1);
    DrawDialog_Margin("The thief is running in your direction. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(panicPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("You can't help but feel obligated to help catch this thief, \n\n", 1);
    DrawDialog_Margin("even if it means spending energy on fighting them.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    //Battle happens here
    system("cls");
    bool gameloop = true;
    while (gameloop){
    KeySwitch(Player,Enemy);
        if (Player.health <= 0){
            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Player", " lost!");
            gameloop = false;
        }
        if (Enemy.health <= 0){
            DrawChacters_AND_HealthBar(Player,Enemy);
            PlaceDialog(Player, Enemy, "Enemy", " lost!");
            gameloop = false;
            MQ_Complete = true;
            
        }
    }
   //End of battle

    if (MQ_Complete == true){
    system("cls");
    printMidCharacter(panicPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("Thank you! Thank you so much!\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(panicPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("The pink cow rummages through the thief's belongings and pulls out...\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("MY HOAGIE!!!\n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("... A hoagie? \n\n", 2);
    DrawDialog_Margin("THAT'S what the thief stole??? \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "???" << endl << endl;
    DrawDialog_Margin("Now I can enjoy eating my hoagie! \n\n", 2);
    DrawDialog_Margin("That thief stole it from me when I put it on the table. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin("I owe you one! \n\n", 2);
    DrawDialog_Margin("My name is Pinkery, I sell milk at that stall! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("Pinkery points to her stall. \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option1[1] = {"I'm " + Player.name + "."};
    choice = optionselect("May I know your name?", option1, 1, neutralPinkery, PinkeryLines);

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin(Player.name + "! If you ever need a discount from any of the stalls in the market,\n\n", 2);
    DrawDialog_Margin("just come to me! I know everyone here, \n\n", 2);
    DrawDialog_Margin("so I can get you the highest quality products for the lowest prices! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    string option2[1] = {"Do you know how I can get home?"};
    choice = optionselect("", option2, 1, neutralPinkery, PinkeryLines);

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin("Home? You're going to need to be more specific than that. \n\n", 2);
    DrawDialog_Margin("I don't think I can give you directions, but this will definitely help! \n\n", 2);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Milk,MilkLines);
    cout << endl;
    DrawDialog_Margin("Obtained [Special Strawberry Milk]! \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin("It's special strawberry milk! \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("Pinkery leans in as if to tell you a secret. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(neutralPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin("It's special because it helps you grow. \n\n", 1);
    DrawDialog_Margin("You'll probably get home faster.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << string(50, ' '); cout << "Pinkery" << endl << endl;
    DrawDialog_Margin("Anyway, I'm going back to my stall. Stop by anytime! \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    Market();
    }
}

void Market(){
    do{
    string option1[3] = {"Talk to Pinkery","Check on the thief", "Leave the Market"};
    choice = optionselect("What would you like to do?", option1, 3, Empty, EmptyLines);

        if (choice == "Talk to Pinkery"){
        //What do you want to talk about with Pinkery?
        do{
        system("cls");
        string option2[4] = {"Milk flavors","Source of milk", "Open hours", "Goodbye!"};
        choice = optionselect("Welcome to my stall!", option2, 4, neutralPinkery, PinkeryLines);

        if (choice == "Milk flavors"){
            system("cls");
            printMidCharacter(joyPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("I sell all types of milk flavors! \n\n", 2);
            DrawDialog_Margin("I have strawberry milk, banana milk, grape milk, mango milk, coconut milk, and many more! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(neutralPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("Oh, but the milk I gave you is special. \n\n", 2);
            DrawDialog_Margin("These milks don't give you special effects like the one I gave you.\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Source of milk"){
            system("cls");
            printMidCharacter(neutralPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("Where do I get these milks? Well, how else? \n\n", 2);
            DrawDialog_Margin("I get it based on what flavor it is of course! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(joyPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("I get strawberry milk from strawberries, banana milk from bananas, mango milk from mangoes, \n\n", 2);
            DrawDialog_Margin("grape milk from grapes, coconut milk from coconut, you get the idea. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Open hours"){
            system("cls");
            printMidCharacter(neutralPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("I'm here from noon to evening!\n\n", 2);
            DrawDialog_Margin("I come here every day.\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(joyPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("These milks aren't going to sell themselves after all!\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        else if (choice == "Goodbye!"){
            system("cls");
            printMidCharacter(joyPinkery,PinkeryLines);
            cout << string(50, ' '); cout << "Pinkery" << endl << endl;
            DrawDialog_Margin("Pleasure doing business with moo! \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        }
        system("pause");
        }while (choice != "Goodbye!");
        }

        else if (choice == "Check on the thief"){
            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("You check on the fainted thief. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("He doesn't seem to be moving. \n\n", 1);
            DrawDialog_Margin("You try poking him. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("No response. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("He should've thought twice before stealing someone else's lunch. \n\n", 1);
            DrawDialog_Margin("He'll live. \n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");

            system("cls");
            printMidCharacter(Empty,EmptyLines);
            cout << endl;
            DrawDialog_Margin("Probably.\n\n", 1);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
    }while (choice != "Leave the Market");

    //Leave the Market
    system("cls");
    printMidCharacter(joyPinkery,PinkeryLines);
    cout << endl;
    DrawDialog_Margin("Pinkery moos at you, it seems like that's her way of saying goodbye.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");
}

bool BG_Complete=false;
void BerryGarden(Player &Player, Enemy &Enemy){

    DrawDialog("The garden is lush and vibrant with all the berries there could ever exist grown."
               "\nThe berry bushes and trees are stretched far, their colors naturally beautifying the garden. "
               "\n\nThe air is filled with a sweet aroma, ", 1);
    DrawDialog("\nits scent changing depending on which section of berries you'd wander into. ", 1);
    DrawDialog("\n\nYou could probably get lost in this garden, but maybe that wouldn't be such a bad thing.\n\n", 1);
    system("pause");

    system("cls");
    string option1[2] = {"Pick berries","Leave the Berry Garden"};
    choice = optionselect(" ", option1, 2, Empty, EmptyLines);

    if (choice == "Pick berries"){
    system("cls");
    printMidCharacter(Basket,BasketLines);
    cout << endl;
    DrawDialog_Margin("You find a basket on the ground, perfect to put the berries in.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    if (choice == "Pick berries"){
    system("cls");
    printMidCharacter(Basket,BasketLines);
    cout << endl;
    DrawDialog_Margin("You walk around the garden, plucking the berries that you know are safe and edible. \n\n", 1);
    DrawDialog_Margin("There are some berries you can't identify, \n\n", 1);
    DrawDialog_Margin("so you decide to leave them as you don't know what could or couldn't be eaten in this world. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(BasketBerries,BasketLines);
    cout << endl;
    DrawDialog_Margin("Obtained [Basket full of berries]!\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(BasketBerries,BasketLines);
    cout << endl;
    DrawDialog_Margin("A basket full should be enough for Cream to prepare dinner with.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("As you were about to leave the garden, you hear loud rustling from a distance.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You find it suspicious, maybe it's a wild Prunicus that Cream mentioned? \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You probably shouldn't stay to find out. You're pretty deep into the garden, \n\n", 1);
    DrawDialog_Margin("so you should start speed-walking in the other direction. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You try to retrace your steps back to the entrance of the garden, \n\n", 1);
    DrawDialog_Margin("but it seems you've forgotten where you came from.\n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("There's no one else in the garden to help you. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(Empty,EmptyLines);
    cout << endl;
    DrawDialog_Margin("You hear louder rustling... and a deep whisper, \n\n", 1);
    DrawDialog_Margin("as if it's right behind you. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(DrawEnemy3,DrawEnemy3_Lines);
    cout << endl;
    DrawDialog_Margin("You turn around and see a decaying tree towering over you. \n\n", 1);
    DrawDialog_Margin("It bears plum fruit, but even the fruit it holds have gone bad. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    system("cls");
    printMidCharacter(DrawEnemy3,DrawEnemy3_Lines);
    cout << endl;
    DrawDialog_Margin("It tries to swing at you with its branch, but you duck just in the nick of time. \n\n", 1);
    DrawDialog_Margin("You must fight to leave. \n\n", 1);
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    system("pause");

    //Battle happens here
    system("cls");
    bool gameloop = true;
    while (gameloop){
    KeySwitch(Player,Enemy);
        if (Player.health <= 0){
            DrawChacters_AND_HealthBar(Player, Enemy);
            PlaceDialog(Player, Enemy, "Player", " lost!");
            gameloop = false;
        }
        if (Enemy.health <= 0){
            DrawChacters_AND_HealthBar(Player, Enemy);
            PlaceDialog(Player, Enemy, "Enemy", " lost!");
            gameloop = false;
            BG_Complete = true;
        }
    }
    //Story continues after wnning the battle
    if (BG_Complete == true){
        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("Wild Prunicus has been defeated\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "???" << endl << endl;
        DrawDialog_Margin("Woah, you did a number on that Prunicus.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << endl;
        DrawDialog_Margin("A bear stands behind you, he looks oddly familiar.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option1[3] = {"How long were you standing there?","Why didn't you help me?", "Were you just watching me fight the whole time?!"};
        choice = optionselect(" ", option1, 3, neutralBrownie, BrownieLines);

        if (choice == "How long were you standing there?"){
            system("cls");
            printMidCharacter(neutralBrownie,BrownieLines);
            cout << string(50, ' '); cout << "???" << endl << endl;
            DrawDialog_Margin("Around 3 minutes.\n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
        else if (choice == "Why didn't you help me?"){
            system("cls");
            printMidCharacter(neutralBrownie,BrownieLines);
            cout << string(50, ' '); cout << "???" << endl << endl;
            DrawDialog_Margin("Eh, you looked like you had it covered. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }
        else if (choice == "Were you just watching me fight the whole time?!"){
            system("cls");
            printMidCharacter(neutralBrownie,BrownieLines);
            cout << string(50, ' '); cout << "???" << endl << endl;
            DrawDialog_Margin("Yeah, it looked fun to spectate instead of fighting the Prunicus for once. \n\n", 2);
            cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
            system("pause");
        }

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("I'm Brownie. Thanks for dealing with this Prunicus for me. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << endl;
        DrawDialog_Margin("Brownie walks over to the Prunicus and kneels to take something from it. \n\n", 1);
        DrawDialog_Margin("He rummages the leaves and branches and pulls out a plum. \n\n", 1);
        DrawDialog_Margin("It doesn't look rotten at all. It might actually be edible.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Some people really can't hold their tongue from talking smack about the plum trees. \n\n", 2);
        DrawDialog_Margin("Don't they know that plants have feelings too?\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << endl;
        DrawDialog_Margin("Brownie walks over to a vacant area of soil and opens the plum fruit. \n\n", 1);
        DrawDialog_Margin("He extracts the pit and places it in the soil. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Every time someone insults a plum tree; \n\n", 2);
        DrawDialog_Margin("it turns into a Prunicus and eats the other berries.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("And every time a plum tree turns into a Prunicus, \n\n", 2);
        DrawDialog_Margin("someone else has to deal with it. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("They're lucky I'm around to keep them in check once in a while. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("My parents frequently pluck berries from this garden,\n\n", 2);
        DrawDialog_Margin("so I can't help but worry that a plum tree turns again. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Why do people want to badmouth these plants anyway? \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("It would be much better if they complimented them instead, \n\n", 2);
        DrawDialog_Margin("then the plants would grow faster. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("They could have at least kept their mouth shut if they didn't have anything nice to say.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(frustBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Hey. Are you still listening?\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option2[4] = {"Yes.","No.", "Sorry, what were you saying?","(snore)"};
        choice = optionselect("Hey. Are you still listening?", option2, 4, frustBrownie, BrownieLines);

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("(sigh) Anyway, are you picking berries here? \n\n", 2);
        DrawDialog_Margin("You have a whole basket full of berries. \n\n", 2);
        DrawDialog_Margin("Are you making a feast or something? \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option3[1] = {"Cream's favor"};
        choice = optionselect("Are you making a feast or something?", option3, 1, neutralBrownie, BrownieLines);

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("She asked you to get more berries? \n\n", 2);
        DrawDialog_Margin("Looks like we're eating good tonight.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << endl;
        DrawDialog_Margin("Brownie's eyes light up.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("What are you doing after this?\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option4[1] = {"I need to get home."};
        choice = optionselect("What are you doing after this?", option4, 1, neutralBrownie, BrownieLines);

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Oh, I see. I'll deliver these berries for you. \n\n", 2);
        DrawDialog_Margin("You can be on your way home now. \n\n", 2);
        DrawDialog_Margin("Thanks for helping my mama with the errand. \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(BasketBerries,BasketLines);
        cout << endl;
        DrawDialog_Margin("You gave Brownie [basket full of berries]. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        string option5[2] = {"I'm not from here.","I don't know how to get home."};
        choice = optionselect("", option5, 2, neutralBrownie, BrownieLines);

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("You don't? Hmm... \n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << endl;
        DrawDialog_Margin("Brownie ponders for a moment, looking at you up at down. \n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("I guess you don't look like you're from anywhere here.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("You'll need this.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBeary,BearyLines);
        cout << endl;
        DrawDialog_Margin("Obtained [Beary]!\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("I'll guide you out of the garden. \n\n", 2);
        DrawDialog_Margin("Don't worry about the Prunicus, it'll sort itself out.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("Brownie guides you through the maze of berries. \n\n", 1);
        DrawDialog_Margin("You realize you were pretty close to the exit had you just made another right turn.\n\n", 1);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(neutralBrownie,BrownieLines);
        cout << string(50, ' '); cout << "Brownie" << endl << endl;
        DrawDialog_Margin("Alright, this is where we part ways. \n\n", 2);
        DrawDialog_Margin("Don't worry about the Prunicus, it'll sort itself out.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");

        system("cls");
        printMidCharacter(Empty,EmptyLines);
        cout << endl;
        DrawDialog_Margin("Brownie walks away. \n\n", 2);
        DrawDialog_Margin("You never want to go back into that berry maze.\n\n", 2);
        cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
        system("pause");
    }
    }
    }

}




int main(){

    srand(time(0));

    Player p1("Player",100,DrawPlayer,DrawPlayer_Lines);
    Player p2("Beary",1000,DrawBeary,DrawBeary_Lines);
    //Bedroom(p1);
    //DiningRoom(p1);
    //House();
    //Outside(p1);
    //TrainingGrounds(p1);
    //Market();

    //int enemyIndex = 1;

    Enemy e1("Soldier",200,DrawEnemy1, DrawEnemy1_Lines);
    Enemy e2("Thief",100,DrawEnemy2, DrawEnemy2_Lines);
    Enemy e3("Prunicus",400,DrawEnemy3, DrawEnemy3_Lines);
    Enemy e4("Dragon",999,DrawEnemy4, DrawEnemy4_Lines);
    //TrainingGroundsQuest(p1,e1);
    //MarketQuest(p1,e2);
    BerryGarden(p1,e3);


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
