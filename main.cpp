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
    char *filename;
    filename = new char[strlen(input[2]) - 2 + 1];
    for (int i = 0; i < strlen(input[2]) - 2; i++) {//because of ``
        filename[i] = input[2][i + 1];
    }
    filename[strlen(input[2]) - 2] = NULL;
    ifstream myfilenames;
    ifstream myfiletypes;
    myfilenames.open("subjects" + string(filename) + ".txt");
    myfiletypes.open("types" + string(filename) + ".txt");
    if (myfilenames.is_open()) {
        char *names[num - 4], *types[num - 4];//inja dige amalan bikhod ba dynamic raftam
        int counter = 0;
        do {
            names[counter] = new char[25];
            types[counter] = new char[25];
            myfilenames >> names[counter];
            myfiletypes >> types[counter];
            counter++;
        } while (!myfilenames.eof());
        bool check_values = 1;
        for (int i = 0; i < counter - 1 && check_values; i++) {
            check_values = 0;
            if (i == 0) {
                if (input[4 + i][1] == '`' && (strncmp(types[i], "string", 6) == 0))
                    check_values = 1;
                if (input[4 + i][1] != '`' &&
                    ((strncmp(types[i], "int", 3) == 0) || (strncmp(types[i], "float", 5) == 0)))
                    check_values = 1;
            } else {
                if (input[4 + i][0] == '`' && strncmp(types[i], "string", 6) == 0)
                    check_values = 1;
                if (input[4 + i][0] != '`' &&
                    ((strncmp(types[i], "int", 3) == 0) || (strncmp(types[i], "float", 5) == 0)))
                    check_values = 1;
            }
        }
        for (int i = 0; i < num - 4; i++) {
            //delete[]types[i];
            delete[]names[i];
        }
        myfilenames.close();
        myfiletypes.close();
        if (check_values) {
            char *values[(num - 4)];//because of INSERT INTO `table_name` VALUES
            int secondcounter = 0;
            for (int i = 4; i < num; i++) {
                if (i == 4) {
                    if (input[4][1] == '`') {
                        values[secondcounter] = new char[strlen(input[4]) - 4 + 1];
                        for (int j = 0; j < strlen(input[4]) - 4; j++) {//-4 is for ('',
                            values[secondcounter][j] = input[4][j + 2];
                        }
                        values[secondcounter][strlen(input[4]) - 4] = NULL;
                        secondcounter++;
                    } else {
                        values[secondcounter] = new char[strlen(input[4]) - 2 + 1];
                        for (int j = 0; j < strlen(input[4]) - 2; j++) {//-3 is for ('',
                            values[secondcounter][j] = input[4][j + 1];
                        }
                        values[secondcounter][strlen(input[4]) - 2] = NULL;
                        secondcounter++;
                    }
                } else {
                    if (input[i][0] == '`') {
                        values[secondcounter] = new char[strlen(input[i]) - 3 + 1];
                        for (int j = 0;
                             j < strlen(input[i]) - 3; j++) {//-1 is for comma in the end or bracket
                            values[secondcounter][j] = input[i][j + 1];
                        }
                        values[secondcounter][strlen(input[i]) - 3] = NULL;
                        secondcounter++;
                    } else {
                        values[secondcounter] = new char[strlen(input[i]) - 1 + 1];
                        for (int j = 0;
                             j < strlen(input[i]) - 1; j++) {//-1 is for comma in the end or bracket
                            values[secondcounter][j] = input[i][j];
                        }
                        values[secondcounter][strlen(input[i]) - 1] = NULL;
                        secondcounter++;
                    }
                }
            }
            bool final_check = 1;//for int and float variables
            for (int i = 0; i < secondcounter && final_check; i++) {
                if ((strncmp(types[i], "float", 6) == 0)) {
                    double a = atof(values[i]);
                    if (int(a) == a) {
                        int b = int(a);
                        int length1 = 0;
                        while (b > 0) {
                            b = b / 10;
                            length1++;
                        }
                        if (length1 == strlen(values[i]))
                            final_check = 0;
                    }
                }
                if ((strncmp(types[i], "int", 4) == 0)) {
                    int a = atoi(values[i]);
                    float b = atof(values[i]);
                    if (a != b)
                        final_check = 0;
                }
            }
            if (final_check) {
                ofstream myfile;
                myfile.open((string(filename) + ".txt"), fstream::app);
                for (int j = 0; j < secondcounter; j++) {
                    myfile << values[j] << " ";
                    delete[]values[j];
                }
                cout << "done ;) ";
            } else {
                cout << "you didn't pay attention to int and float types,try again plz...";
            }
        } else {
            cout << "values doesn't match with their type " << endl;
        }
    } else {
        cout << "the file doesn't exist!" << endl;
        myfilenames.close();
        myfiletypes.close();
    }
}

void SELECT(char **input, int num, int fromplace) {
    char *filename;
    filename = new char[strlen(input[fromplace + 1]) - 2 + 1];
    for (int i = 0; i < strlen(input[fromplace + 1]) - 2; i++) {//because of ``
        filename[i] = input[fromplace + 1][i + 1];
    }
    filename[strlen(input[fromplace + 1]) - 2] = NULL;
    ifstream myfile;
    myfile.open(string(filename) + ".txt");
    if (myfile.is_open()) {
        char *names[fromplace - 1];
        for (int i = 1; i < fromplace; i++) {
            if (i == (fromplace - 1)) {
                names[i - 1] = new char[strlen((input[i]) - 2)];
                for (int j = 0; j < (strlen(input[i]) - 2); j++) {
                    names[i - 1][j] = input[i][j + 1];
                }
                names[i - 1][strlen(input[i]) - 2] = NULL;
            } else {
                names[i - 1] = new char[strlen((input[i]) - 3)];
                for (int j = 0; j < (strlen(input[i]) - 3); j++) {
                    names[i - 1][j] = input[i][j + 1];
                }
                names[i - 1][strlen(input[i]) - 3] = NULL;
            }
        }
        ifstream myfilesubjects;
        int check = 0;
        int counter = 0;//counter is subject counter,second counter is all variables counter
        bool finder[50] = {0};//finder shows that what  subjects are wanted to be shown
        myfilesubjects.open("subjects" + string(filename) + ".txt");
        char subjects[100][20] = {NULL};
        while (!myfilesubjects.eof()) {
            myfilesubjects >> subjects[counter];
            for (int i = 1; i < fromplace && !finder[counter]; i++) {
                if (strncmp(subjects[counter], names[i - 1], strlen(subjects[counter]) + 1) == 0) {
                    check++;
                    finder[counter] = true;
                }
            }
            counter++;
        }
        counter--;//because of the character of eof !
        myfilesubjects.close();
        int secondcounter = 0;
        char variables[60 * counter][20] = {NULL};
        while (!myfile.eof()) {
            myfile >> variables[secondcounter];
            secondcounter++;
        }
        secondcounter--;//because of the character of eof !
        int times = (secondcounter / counter);
        myfile.close();
        if (names[0][0] != '*') {
            if ((num - fromplace > 3) && (check == (fromplace - 1))) {//dige majbur shodam az strtok estefade konam :))
                char **token = new char *[num - fromplace - 3];
                int token_counter = 0;
                while (token_counter != num - fromplace - 3) {
                    token[token_counter] = strtok(input[token_counter + fromplace + 2],
                                                  " ");//+2 vase ineke az baade esme file shuru kone be gereftane dadehash
                    token_counter++;
                }
                int *returnvalue = new int[2];
                int *returnvalue1 = new int[2];
                int *returnvalue2 = new int[2];
                findstr(token, num - fromplace - 3, "WHERE", returnvalue);
                if (returnvalue[0] == 1) {
                    findstr(token, num - fromplace - 3, "OR", returnvalue1);
                    findstr(token, num - fromplace - 3, "AND", returnvalue2);
                    if ((returnvalue1[0] + returnvalue2[0]) ==
                        0) {//so it just have WHERE,and doesn't have any ORs & ANDs :D
                        char *firsttoken[2];
                        int token_check = 0;//vase ineke dafeye avval bayad yejure dige farakhuni konim strtoko
                        while (token_check < 2) {
                            if (token_check == 0) {
                                firsttoken[token_check] = strtok(input[fromplace + 3], "=`");
                                token_check++;
                            } else {
                                firsttoken[token_check] = strtok(NULL, "=`");
                                token_check++;
                            }
                        }
                        bool subjectcheck = 0;
                        for (int i = 0; i < counter && !subjectcheck; i++) {
                            if (strncmp(firsttoken[0], subjects[i], strlen(subjects[i]) + 1) == 0)
                                subjectcheck = 1;
                        }
                        if (subjectcheck) {
                            bool variablecheck = 0;
                            int whatrow2show = 0;
                            for (int i = 0; i < secondcounter && !variablecheck; i++) {
                                if (strncmp(firsttoken[1], variables[i], strlen(variables[i]) + 1) == 0) {
                                    variablecheck = 1;
                                    whatrow2show = i / counter;
                                }
                            }
                            if (variablecheck) {
                                for (int i = 0; i < counter; i++) {
                                    if (finder[i]) {
                                        for (int j = 0; j < times; j++)
                                            if (j == whatrow2show) {
                                                cout << variables[i + j * counter] << " ";
                                                cout << endl;
                                            }
                                    }
                                }
                            } else
                                cout
                                        << "check ur inputs(specialy variables after WHERE),it doesn't match with saved data";

                        } else
                            cout
                                    << "check ur inputs(specialy subjects after WHERE),it doesn't not match with saved data";
                    } else if (num - fromplace - 3 >= 4 && num - fromplace - 3 <= 6) {
                        char *firsttoken[2];
                        int token_check = 0;//vase ineke dafeye avval bayad yejure dige farakhuni konim strtoko
                        while (token_check < 2) {
                            if (token_check == 0) {
                                firsttoken[token_check] = strtok(input[fromplace + 3], "=`");
                                token_check++;
                            } else {
                                firsttoken[token_check] = strtok(NULL, "=`");
                                token_check++;
                            }
                        }
                        char *secondtoken[2];
                        char *thirdtoken[2];
                        int token_check2 = 0;//vase ineke dafeye avval bayad yejure dige farakhuni konim strtoko
                        while (token_check2 < 2) {
                            if (token_check2 == 0) {
                                secondtoken[token_check2] = strtok(input[fromplace + 5], "=`");
                                token_check2++;
                            } else {
                                secondtoken[token_check2] = strtok(NULL, "=`");
                                token_check2++;
                            }
                        }
                        if (num - fromplace - 3 == 6) {
                            int token_check3 = 0;//vase ineke dafeye avval bayad yejure dige farakhuni konim strtoko
                            while (token_check3 < 2) {
                                if (token_check3 == 0) {
                                    thirdtoken[token_check3] = strtok(input[fromplace + 7], "=`");
                                    token_check3++;
                                } else {
                                    thirdtoken[token_check3] = strtok(NULL, "=`");
                                    token_check3++;
                                }
                            }
                        }
                        bool subjectcheck = 0;
                        for (int i = 0; i < counter && !subjectcheck; i++) {
                            if (strncmp(firsttoken[0], subjects[i], strlen(subjects[i]) + 1) == 0)
                                for (int j = 0; j < counter && !subjectcheck; j++) {
                                    if (strncmp(secondtoken[0], subjects[j], strlen(subjects[j]) + 1) == 0)
                                        if (num - fromplace - 3 == 6) {
                                            for (int k = 0; k < counter && !subjectcheck; k++)
                                                if (strncmp(thirdtoken[0], subjects[k], strlen(subjects[k]) + 1) == 0)
                                                    subjectcheck = 1;
                                        } else {
                                            subjectcheck = 1;
                                        }
                                }
                        }

                        if (subjectcheck) {
                            bool variablecheck = 0;
                            int whatrow2show[3] = {0};
                            for (int i = 0; i < secondcounter && !variablecheck; i++) {
                                if (strncmp(firsttoken[1], variables[i], strlen(variables[i]) + 1) == 0) {
                                    whatrow2show[0] = i / counter;
                                    for (int j = 0; j < secondcounter && !variablecheck; j++) {
                                        if (strncmp(secondtoken[1], variables[j], strlen(variables[j]) + 1) == 0) {
                                            whatrow2show[1] = j / counter;
                                            if (num - fromplace - 3 == 6) {
                                                for (int k = 0; k < secondcounter && !variablecheck; k++)
                                                    if (strncmp(thirdtoken[1], variables[k],
                                                                strlen(variables[k]) + 1) ==
                                                        0) {
                                                        variablecheck = 1;
                                                        whatrow2show[2] = k / counter;
                                                    }
                                            } else {
                                                variablecheck = 1;
                                            }
                                        }
                                    }
                                }
                            }
                            if (variablecheck) {
                                bool continue_or_not = 1;
                                for (int i = 0; i < counter && continue_or_not; i++) {
                                    if (finder[i]) {
                                        for (int j = 0; j < times && continue_or_not; j++) {
                                            if (num - fromplace - 3 == 4) {
                                                if (strncmp(token[2], "OR", 3) == 0) {
                                                    if (j == whatrow2show[0] || j == whatrow2show[1]) {
                                                        cout << variables[i + j * counter] << " ";
                                                    }
                                                }
                                                if (strncmp(token[2], "AND", 4) == 0) {
                                                    if (whatrow2show[0] == whatrow2show[1]) {
                                                        if (j == whatrow2show[0]) {
                                                            cout << variables[i + j * counter] << " ";
                                                        }
                                                    } else {
                                                        continue_or_not = 0;
                                                        cout << "tese two doesn't have anything in common...";
                                                    }
                                                }
                                            }
                                            if (num - fromplace - 3 == 6) {
                                                if (strncmp(token[2], "OR", 3) == 0) {
                                                    if (strncmp(token[4], "OR", 3) == 0) {
                                                        if (j == whatrow2show[0] || j == whatrow2show[1] ||
                                                            whatrow2show[2]) {
                                                            cout << variables[i + j * counter] << " ";
                                                        }
                                                    }
                                                    if (strncmp(token[4], "AND", 4) == 0) {
                                                        if ((whatrow2show[0] == whatrow2show[2]) ||
                                                            (whatrow2show[1] == whatrow2show[2])) {
                                                            if (j == whatrow2show[2]) {
                                                                cout << variables[i + j * counter] << " ";
                                                            }
                                                        } else {
                                                            continue_or_not = 0;
                                                            cout << "there isn't such data ...";
                                                        }

                                                    }
                                                }
                                                if (strncmp(token[2], "AND", 4) == 0) {
                                                    if (strncmp(token[4], "OR", 3) == 0) {
                                                        if (whatrow2show[0] == whatrow2show[1]) {
                                                            if (j == whatrow2show[2] || whatrow2show[0]) {
                                                                cout << variables[i + j * counter] << " ";
                                                            }
                                                        } else {
                                                            if (j == whatrow2show[2]) {
                                                                cout << variables[i + j * counter] << " ";
                                                            }
                                                        }
                                                    }
                                                    if (strncmp(token[4], "AND", 4) == 0) {
                                                        if (whatrow2show[0] == whatrow2show[2] &&
                                                            whatrow2show[1] == whatrow2show[2]) {
                                                            if (j == whatrow2show[2]) {
                                                                cout << variables[i + j * counter] << " ";
                                                            }
                                                        } else {
                                                            continue_or_not = 0;
                                                            cout << "there isn't such data ...";
                                                        }

                                                    }
                                                }
                                            }
                                        }
                                        cout << endl;
                                    }
                                }
                            } else {
                                cout
                                        << "check ur inputs(specialy variables after WHERE),it doesn't match with saved data";
                            }

                        } else {
                            cout
                                    << "check ur inputs(specialy subjects after WHERE),it doesn't not match with saved data";
                        }

                        delete[]firsttoken;
                        delete[]secondtoken;
                        delete[]thirdtoken;
                    }
                } else {
                    cout << "wrong syntax,check ur input plz... ";
                }
                for (int i = 0; i < num - fromplace - 3; i++) {
                    delete[]token[i];
                }
            }
            if (num - fromplace == 3 && check == ((fromplace) - 1)) {
                for (int i = 0; i < counter; i++) {
                    if (finder[i]) {
                        for (int j = 0; j < times; j++)
                            cout << variables[i + j * counter] << " ";
                        cout << endl;
                    }
                }
            }
            if (check != (fromplace) - 1) {
                cout << "check ur input,it doesn't match with saved data";
            }
            for (int i = 0; i < fromplace - 2; i++) {
                delete[]names[i];
            }
        } else {
            for (int i = 0; i < counter; i++) {
                for (int j = 0; j < times; j++)
                    cout << variables[i + j * counter] << " ";
                cout << endl;
            }

        }

    } else {
        cout << "file isn't available,or it's empty...";
    }
    delete[]filename;
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