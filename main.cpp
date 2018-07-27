#include <iostream>
#include <fstream>
#include <Cstring>

using namespace std;

void valid_input(char **input, int &num, int &fromplace) {

    while (!(((strncmp(input[0], "CREATE", 7) == 0) && (strncmp(input[1], "TABLE", 6) == 0) && input[3][0] == '(' &&
              input[2][0] == '`') ||
             ((strncmp(input[0], "INSERT", 7) == 0) && (strncmp(input[1], "INTO", 5) == 0) &&
              (strncmp(input[3], "VALUES", 7) == 0)) || (strncmp(input[0], "SELECT", 7) == 0) ||
             ((strncmp(input[0], "UPDATE", 7) == 0) && (strncmp(input[2], "SET", 4) == 0)) ||
             ((strncmp(input[0], "DELETE", 7) == 0) && (strncmp(input[1], "FROM", 5) == 0) &&
              (strncmp(input[3], "WHERE", 6) == 0)) ||
             ((strncmp(input[0], "DROP", 5) == 0) && (strncmp(input[1], "TABLE", 6) == 0)))) {
        cout
                << "what do u want to do?(plz input them correctly and don't forget to enter commands in UPPER CASE and use full stop (dot) in the end of SELECT,UPDATE,DELETE and DROP commands,with a spaace before it)"
                << endl;
        num = 0;
        do {
            *(input + num) = new char[20];
            cin >> input[num];
            if ((strncmp(input[num], "FROM", 5) == 0))
                fromplace = num;
            num++;
        } while ((input[num - 1][strlen(input[num - 1]) - 1] != ')')
                 &&
                 ((input[num - 1][strlen(input[num - 1]) - 1] != '.')));

    }
}

int what2do(char **input) {
    if (strncmp(input[0], "CREATE", 7) == 0)
        return 0;
    if (strncmp(input[0], "INSERT", 7) == 0)
        return 1;
    if (strncmp(input[0], "SELECT", 7) == 0)
        return 2;
    if (strncmp(input[0], "UPDATE", 7) == 0)
        return 3;
    if (strncmp(input[0], "DELETE", 7) == 0)
        return 4;
    if (strncmp(input[0], "DROP", 5) == 0)
        return 5;
}

void
findstr(char **token, int rows, string str, int *returnvalue) {//i've added this one at last to handle select part:D
    returnvalue[0] = 0;
    returnvalue[1] = 0;
    for (int i = 0; i < rows && !returnvalue[0]; i++) {
        if (strncmp(token[i], str.c_str(), strlen(token[i])) == 0) {
            returnvalue[0] = 1;
            returnvalue[1] = i;
        }
    }
}

void CREATE(char **input, int num) {
    char *filename;
    filename = new char[strlen(input[2]) - 2 + 1];
    char *names[(num - 3) / 2], *types[(num - 3) / 2];//-3 is for create & table & table_name
    int counter1 = 0, counter2 = 0;
    for (int i = 0; i < strlen(input[2]) - 2; i++) {//because of ``
        filename[i] = input[2][i + 1];
    }

    for (int i = 3; i < num; i++) {
        if (i == 3) {
            names[counter1] = new char[strlen(input[3]) - 3 + 1];
            for (int j = 0; j < strlen(input[3]) - 3; j++) {//-3 is for (''
                names[counter1][j] = input[3][j + 2];
            }
            names[counter1][strlen(input[3]) - 3] = NULL;
            counter1++;
        } else {
            if (!((3 + counter1 + counter2) % 2)) {
                types[counter2] = new char[strlen(input[counter1 + counter2 + 3]) + 1];
                for (int j = 0;
                     j < strlen(input[counter1 + counter2 + 3]) - 1; j++) {//-1 is for comma in the end or bracket
                    types[counter2][j] = input[counter1 + counter2 + 3][j];
                }
                types[counter2][strlen(input[counter1 + counter2 + 3]) - 1] = NULL;
                counter2++;
            } else {
                names[counter1] = new char[strlen(input[counter1 + counter2 + 3]) + 1];
                for (int j = 0; j < strlen(input[counter1 + counter2 + 3]) - 2; j++) {//-2 is for ``
                    names[counter1][j] = input[counter1 + counter2 + 3][j + 1];
                }
                names[counter1][strlen(input[counter1 + counter2 + 3]) - 2] = NULL;
                counter1++;
            }
        }
    }
    bool correct_types = 1;
    for (int i = 0; i < counter1 && correct_types; i++) {
        if (!((strncmp(types[i], "string", 6) == 0) || (strncmp(types[i], "float", 5) == 0) ||
              (strncmp(types[i], "int", 3) == 0)))
            correct_types = 0;
    }
    if (correct_types) {
        ofstream myfilevalues;
        ofstream myfilesubjects;
        myfilesubjects.open("subjects" + string(filename) + ".txt");
        myfilevalues.open("types" + string(filename) + ".txt");
        for (int j = 0; j < counter1; j++) {
            myfilevalues << types[j] << " ";
            myfilesubjects << names[j] << " ";
            delete[]names[j];
            delete[]types[j];
        }
        delete[]filename;
        myfilesubjects.close();
        myfilevalues.close();
        cout << "done ;)";
    } else {
        cout
                << "i've warn you to input ur command correctly,but u input forbidden values ,now u must restart program to input it correctly[only float,int,string]";
        for (int j = 0; j < counter1; j++) {
            delete[]names[j];
            delete[]types[j];
        }
        delete[]filename;
    }
}

void INSERT(char **input, int num) {

}

void SELECT(char **input, int num, int fromplace) {
}

void UPDATE(char **input, int num) {

}

void DELETE(char **input, int num) {
}

void DROP(char **input) {

}

int main() {
    int check, num = 0, fromplace = 0;
    char **input;
    input = new char *[50];
    valid_input(input, num, fromplace);
    check = what2do(input);
    switch (check) {
        case 0:
            CREATE(input, num);
            break;
        case 1:
            INSERT(input, num);
            break;
        case 2:
            SELECT(input, num, fromplace);
            break;
        case 3:
            UPDATE(input, num);
            break;
        case 4:
            DELETE(input, num);
            break;
        case 5:
            DROP(input);
            break;
    }

}