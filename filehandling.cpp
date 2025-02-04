#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm> //transform
#include <cctype> //tolower

using namespace std;

void autocreateFile(const string& filename, const string& content) //for new encounters
{
    ofstream file(filename, ios::trunc);//open
    if (file)
    {
        file << content;
        file.close();
        cout << "New progress! '" << filename << "' added. \n";
    }


    else //file.fail()
    {
        cout << "Error. Unable to add '" << filename << "'! Exiting...\n";
        exit(1);
    }
}

void autoappendFile(const string& filename, const string& content)//for create/update defeated_enemies.txt
{
    cout << "Opening file '" << filename << "' for appending..." << endl;
    ofstream file(filename, ios::app); //declare and open output file
    if (file.fail())
    {
        cout << "Error. Unable to append file '" << filename << "'! Exiting...\n" << endl;
        exit(1);
    }

    cout << "Successfully open '" << filename << "'!\n";
        file << "\n" << content;
        file.close();
        cout << "End of appending '" << filename << "'!\n";

}

void manualcreateFile()
{
    string filename, content;
    cout << "\nEnter new filename: ";
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
        cout << "Error. File not found! \n";
        exit(1);
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
        cout << "Error. File '" << filename << "' does not exist! Exiting...\n";
        exit(1);
    }

    string tmpOri;
    int tmpLine = 0;
    bool found = false;

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

void searchwithUserInput()
{
    string filename, searchLine;
    cout << "\nEnter the file name to search: ";
    cin >> filename;
    cout << "Enter the text to search for: ";
    cin >> searchLine;

    // Call the searchinFile function with user input
    searchinFile(filename, searchLine);
}

void deleteFile()
{
    string filename;
    cout << "\nEnter filename to delete: ";
    getline(cin, filename);

    //check existence
    ifstream file (filename);
    if (file)
    {
        file.close(); //prevent unwanted errors
        if (remove(filename.c_str()) == 0)
        {
            cout << "File '" << filename << "' successfully deleted! \n";
        }
        else
        {
            cout << "Error! Unable to delete file '" << filename << "'. \n";
        }
    }
    else
    {
        cout << "Error! File '" << filename << "does not exist. \n";
    }

}

int main()
{
    autocreateFile("test.txt", "testing1\n");
    autoappendFile("test.txt", "testing2\ntesting3\n");

    readFile("test.txt");

    viewwithUserInput();


    return 0;
}

