#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm> //transform
#include <cctype> //tolower
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <thread>

using namespace std;

#define KEY_ESCAPE 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

using namespace std;

int err = 0;//to show no. of errors

void autocreateFile(const string& filename, const string& content) //for new encounters/progress
{
    ofstream file(filename, ios::trunc);//open
    if (file)
    {
        file << content;
        file.close();
        cout << "\nNew progress! '" << filename << "' added into encyclopedia. \n";
    }


    else //file.fail()
    {
        cout << "\nError. Unable to add '" << filename << "' into encyclopedia!\n";
        err++;
        return;
    }
}

void createEmptyFile(const string& filename){

    ofstream file(filename, ios::trunc);//open
    if (file)
    {
        file.close();
        cout << filename << " created. \n";
    }

    else //file.fail()
    {
        cout << "Error. Unable to add '" << filename << "'!\n";
        err++;
        return;
    }
}

void autoappendFile(const string& filename, const string& content)//for create/update enemies.txt & character.txt
{
    cout << "Opening file '" << filename << "' for appending..." << endl;
    ofstream file(filename, ios::app); //declare and open output file
    if (file.fail())
    {
        cout << "Error. Unable to append file '" << filename << "'! Exiting...\n" << endl;
        err++;
        return;
    }

    cout << "Successfully open '" << filename << "'!\n";
        file << "\n" << content;
        file.close();
        cout << "End of appending file '" << filename << "'!\n";

}

void manualcreateFile()
{
    string filename, content;
    cout << "\nEnter filename for new file: ";
    getline (cin, filename);

    cout << "Enter content: ";
    getline (cin, content);

    autocreateFile(filename, content);
}

void readFile(const string& filename)
{
    string tmp;
    fstream file(filename); //declare read/write file
    if (file.fail())
    {
        cout << "\nError. File not found! \n";
        err++;
        return;
    }
    cout << "\nContents of '" << filename << "': \n";
    while (getline(file, tmp))
    {
        cout << tmp << endl;
    }
    file.close();
}

void viewwithUserInput()
{
    string filename;
    cout << "\nEnter file name to view: ";
    getline (cin, filename);

    readFile(filename);
}

void searchinFile (const string& filename, const string& searchLine)
{

    ifstream file(filename); //open input file

    if (file.fail())
    {
        cout << "\nError. File '" << filename << "' does not exist! Exiting...\n";
        system("pause");
        err++;
        return;
    }

    string tmpOri;
    int tmpLine = 0;
    bool found = true;

    //Convert searchLine to lowercase
    string searchLineLower = searchLine;
    transform(searchLineLower.begin(), searchLineLower.end(), searchLineLower.begin(), ::tolower);

    cout << "\nSearching for '" <<searchLine << "' in " << filename << ": ";

    while (getline(file,tmpOri))
    {
        tmpLine ++;

        //Convert current line to lower case
        string tmpLower = tmpOri;
        transform(tmpLower.begin(), tmpLower.end(), tmpLower.begin(), ::tolower);

        if (tmpLower.find(searchLineLower) != string::npos)
        {
            cout << "\nLine " << tmpLine << ": " << tmpOri << " found!";
            found = true;
        }
    }
    if (!found)
    {
        cout << "\n" << searchLine << " not found in " << filename << ".\n";
    }

}

bool bool_searchinFile (const string& filename, const string& searchLine)
{

    ifstream file(filename); //open input file

    if (file.fail())
    {
        cout << "\nError. File '" << filename << "' does not exist! Exiting...\n";
        err++;
        return false;
    }

    string tmpOri;
    int tmpLine = 0;
    bool found = true;

    //Convert searchLine to lowercase
    string searchLineLower = searchLine;
    transform(searchLineLower.begin(), searchLineLower.end(), searchLineLower.begin(), ::tolower);

    while (getline(file,tmpOri))
    {
        tmpLine ++;

        //Convert current line to lower case
        string tmpLower = tmpOri;
        transform(tmpLower.begin(), tmpLower.end(), tmpLower.begin(), ::tolower);

        if (tmpLower.find(searchLineLower) != string::npos)
        {
            return true;
        }
    }

    return false;

}

void searchinFilewithUserInput()
{
    string filename, searchLine;
    cout << "\nEnter the file name to search: ";
    cin >> filename;
    cout << "Enter the text to search for: ";
    cin >> searchLine;

    // Call the searchinFile function with user input
    searchinFile(filename, searchLine);

}

void deleteFile(const string& filename)
{

    //check existence
    ifstream file (filename);
    if (file)
    {
        file.close(); //prevent unwanted errors
        if (remove(filename.c_str()) == 0)
        {
            cout << "\nFile '" << filename << "' successfully deleted! \n";
        }
        else
        {
            cout << "\nError. File '" << filename << "' failed to delete. \n";
            err++;
            return;
        }
    }
    else
    {
        cout << "\nError. Unable to open file '" << filename << "'! Check if file exists or not.\n";
        err++;
        return;
    }

}


const int BORDER_WIDTH = 105;

int z = 0;

void White_HL(string text){
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 112);
    cout << text;
    SetConsoleTextAttribute(hConsole, 15);
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


void DrawStoryMenu(string move, string array[], int arraysize, string text){


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

    cout << string(MID_MARGIN, ' ') << text << endl;
    menuselectionbox(c_array, arraysize);
}


string optionselect(string text, string array[], int arraysize){

    int n;
    cout << string(MID_MARGIN, ' ') << text << endl;
    DrawStoryMenu(" ", array, arraysize, text);

    while(true){
        switch(getch()){
        case KEY_UP:
            DrawStoryMenu("UP", array, arraysize, text);
            break;
        case KEY_DOWN:
            DrawStoryMenu("DOWN", array, arraysize, text);
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

void maingameloop(int startAt);
bool FileExists(const string& filename);
int loadsave(const string& filename);
void MainMenu();


bool PressEnter(){
    cout << endl;
    cout << endl;
    cout << endl;

    string option1[3] = {"Resume","Load Game","Return to Menu"};
    string choice;

    cout << "Press ENTER to continue or ESC to pause...";
    switch(getch()){
        case KEY_ENTER:
            return false;
            break;
        case KEY_ESCAPE:
            choice = optionselect("Game Paused", option1, 3);
            break;
    }

    if (choice == "Resume") return true;
    if (choice == "Load Game") maingameloop(loadsave("savefile"));
    if (choice == "Return to Menu") MainMenu();

    return true;
}

const int numofsaves = 4;

int loadsave(const string& filename){
    int n = 0;
    string saves[numofsaves];
    string areaName;
    string tmp;
    fstream file(filename); //declare read/write file

    while (getline(file, tmp))
    {
        saves[n++] = tmp;
    }

    while (n < numofsaves){
        saves[n++] = " ";
    }

    file.close();

    while(true){
        areaName = optionselect("Load Save", saves, numofsaves);
        if (areaName == "outside") return 1;
        if (areaName == "papadum") return 2;
        if (areaName == "wild") return 3;
        if (areaName == "dragon") return 4;
    }

}
void outside(){

    do{
        system("cls");
        cout << "outside happens" << endl;
    }while(PressEnter());
}

void papadum(){
    if(bool_searchinFile("savefile","papadum") == false){
        autoappendFile("savefile", "papadum");
    }

    do{
        system("cls");
        cout << "Papadum happens" << endl;
    }while(PressEnter());

}

void wild(){

    if(bool_searchinFile("savefile","wild") == false){
        autoappendFile("savefile", "wild");
    }

    do{
        system("cls");
        cout << "Wild happens" << endl;
    }while(PressEnter());

}

void dragon(){

    if(bool_searchinFile("savefile","dragon") == false){
        autoappendFile("savefile", "dragon");
    }

    do{
        system("cls");
        cout << "dragon happens" << endl;
    }while(PressEnter());

    MainMenu();
}
bool isRunning = true;
bool isPaused = true;
bool isMainMenu = true;

void maingameloop(int startAt){
    isMainMenu = false;

    switch(startAt){
        case 1:
            outside();
        case 2:
            papadum();
        case 3:
            wild();
        case 4:
            dragon();
        default:
            break;
    }

}


bool FileExists(const string& filename)
{
    // opening the file
    fstream file(filename); //declare read/write file

    if (file)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void encyclopedia(){
    string pediamenu[4] = {"Enemy", "Character", "Search in file", "Exit to Main"};
    string choice;

        choice = optionselect("Encyclopedia", pediamenu, 4);
        if (choice == "Enemy"){

            FileExists("enemy.txt");
            if (FileExists("enemy.txt")){
                system("cls");
                readFile("enemy.txt");
                viewwithUserInput();
            }
            else
            {
                cout << "Error. File not found. Please defeat at least one enemy first.\n";
                err++;
                system("pause");
            }
            encyclopedia();
        }
        else if (choice == "Character"){

            FileExists("character.txt");
            if (FileExists("character.txt")){
                system("cls");
                readFile("character.txt");
                viewwithUserInput();
            }
            else {
                cout << "Error. File not found. Please play the game first.\n";
                err++;
                system("pause");
            }
            encyclopedia();
        }
        else if (choice == "Search in file"){
            searchinFilewithUserInput();
            encyclopedia();
        }
        else {
            MainMenu();
        }
}

void MainMenu(){

    isMainMenu = true;
    string mainmenu[4] = {"New Game", "Load Game", "Encyclopedia", "Exit"};
    string confirm[2] = {"Yes", "No"};
    string menuchoice;
    string y_n;

    bool loop = true;
    do{
        menuchoice = optionselect("Main Menu", mainmenu, 4);
        if (menuchoice == "New Game"){
            if (FileExists("savefile")){
                system("cls");
                y_n = optionselect("This will overwrite the save file, Are you sure?", confirm, 2);
                if (y_n == "Yes"){
                    loop = false;
                }
                else{
                    break;
                }
            }
            else{
                loop = false;
            }
        }
        if (menuchoice == "Load Game"){
            if (FileExists("savefile")){
                loop = false;
            }
            else{
                system("cls");
                cout << "No existing save file!" << endl;
                PressEnter();
            }
        }
        if (menuchoice == "Encyclopedia")
        {
            encyclopedia();
        }
        if (menuchoice == "Exit"){
            exit(err); //exit program and show no. of errors
        }
    }while(loop == true);

    if (menuchoice == "New Game"){
        deleteFile("savefile");
        autocreateFile("savefile", "outside");
        maingameloop(1);
    }
    if (menuchoice == "Load Game"){
        maingameloop(loadsave("savefile"));
    }
}

int main()
{

    MainMenu();

    return err; //show number of errors
}

