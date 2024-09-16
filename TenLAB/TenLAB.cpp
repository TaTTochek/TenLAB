//#include <iostream>
//#include <conio.h>
//#include <ctype.h>
//
//int main(int argc, char* argv[])
//{
//	int ch;
//
//	_cputs("Type 'Y' when finished typing keys: ");
//	do
//	{
//		ch = _getch();
//		printf("%i", ch);
//		ch = toupper(ch);
//	} while (ch != 'Y');
//
//	_putch(ch);
//	_putch('\r');    /* Carriage return */
//	_putch('\n');    /* Line feed       */
//
//	return 0;
//}

/*
* Как предложение короче будем делать так...
* Открываем файл (проверяем на то, можно ли в него записывать, MSDOS и расширение txt) и создаём массив из строк, в него закидываем все строки файла, а потом закрываем файл (для чтения).
* Затем мы работаем с массивом. Полностью удаляем содержимое файла и записываем вновь(если есть) или просто создаём.
* Полезные функции cstring:
* strcpy - копирует одну строку в другую
* strcat (str, str)
* 
* Функции которые нужны:
* Поиск в строке;
* Замена первых строчных букв слов прописными;
* Вставка в начало; 
* 
* Открытие файла + проверка расширения + его сохранение
* 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <algorithm>
#include <vector>

using namespace std;

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

char BufRus[256];
char* Rus(const char* text)
{
    CharToOemA(text, BufRus);
    return BufRus;
}

string getfilename1(string filepath) {
    size_t extensionindx = filepath.find_last_of(".");
    while (extensionindx != string::npos) {
        filepath = filepath.substr(0, extensionindx);
        extensionindx = filepath.find_last_of(".");
    }
    return filepath;
}

string getfilename2(string filepath) {
    size_t slashindx = filepath.find_last_of("\\/");
    if (slashindx != string::npos) {
        return filepath.substr(slashindx + 1);
    }
    return filepath;
}

void printFile(vector<string> file) {

    cout << endl << endl;
    cout << Rus("Текстовый файл: \n");
    for (int i = 0; i < file.size(); i++) {
        printf("%3d | ", i);
        cout << file[i] << endl;
    }
}
bool isNumber(string str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
void poiskString(string temp, vector<string> filesol, int nline);
void tlow(vector<string>& temp, int nline);
string vstavka(vector<string> filesol, int nline, string temp);

int main() {
    string filedir, filename, line, FileDir;
    vector<string> filesol;
    char v;
    printf(Rus("Введите адрес файла - "));
    cin >> filedir;
    FileDir = filedir;
    ifstream file(filedir.c_str());
    transform(filedir.begin(), filedir.end(), filedir.begin(), tolower);

    if (!file)
    {
        printf(Rus("Не удалось открыть файл."));
        return 0;
    }
    if ((filedir.find("//") != string::npos) || (filedir.find("/\\") != string::npos) || (filedir.find("\\/") != string::npos) || (filedir.find("\\\\") != string::npos)) {
        printf(Rus("Неверный путь к файлу."));
        return 0;
    }

    string msdosnames[24] = { "aux", "clock&", "com1", "com2", "com3", "com4", "com5", "com6", "com7", "com8", "com9", "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9", "lpt0", "con", "nul", "prn" };

    filename = getfilename2(getfilename1(filedir));

    for (int i = 0; i <= 12; i++) {
        if (filename == msdosnames[i]) {
            printf(Rus("Имя файла зарезервировано в MSDOS."));
            return 0;
        }
    }

    //printf(Rus("Содержимое файла\n"));
    while (getline(file, line)) {
        filesol.push_back(line);
    }
    if (filesol.size() == 0) {
        cout << Rus("Пустой файл") << endl;
        system("pause");
        return 0;
    }
    //cout << filesol.size() << endl;
    if (line[0] == '\0') {
        //cout << "YES" << endl;
        filesol.push_back("");
    }

    printf(Rus("1 - Поиск в строке;\n"));
    printf(Rus("2 - Замена первых строчных букв слов прописными;\n"));
    printf(Rus("3 - Вставка в начало;\n"));
    printf(Rus("4 - Выход\n"));

    int wntd, nline; string wntds, temp;

    do {
        printFile(filesol);
        cout << Rus("Введите команду: ");
        //getline(cin, wntds);
        cin >> wntds;
        if (!(wntds == "1" || wntds == "2" || wntds == "3" || wntds == "4")) wntd = 0;
        else wntd = stoi(wntds);
        //printf(Rus(""));
        if (wntd == 1) {
            printf(Rus("Выберите строку с которой хотите взаимодействовать - "));
            cin >> temp;
            if (!isNumber(temp)) {
                printf(Rus("\nНе правильно, по новой\n"));
                continue;
            }
            nline = stoi(temp);
            if (!(nline >= 0 && nline <= filesol.size() - 1)) {
                printf(Rus("Не правильно, по новой\n"));
                continue;
            }
            printf(Rus("Введите последовательность символов, которую вы хотите найти: "));
            //(cin >> temp).get();
            std::cin.ignore();
            getline(cin, temp);

            cout << "\n";
            poiskString(temp, filesol, nline);
        }
        else if (wntd == 2) {
            printf(Rus("Выберите строку с которой хотите взаимодействовать: "));
            cin >> temp;
            if (!isNumber(temp)) {
                printf(Rus("\nНе правильно, по новой\n"));
                continue;
            }
            nline = stoi(temp);
            if (!(nline >= 0 && nline <= filesol.size() - 1)) {
                printf(Rus("Не правильно, по новой\n"));
                continue;
            }
            tlow(filesol, nline);
        }
        else if (wntd == 3) {
            printf(Rus("Выберите строку с которой хотите взаимодействовать: "));
            cin >> temp;
            if (!isNumber(temp)) {
                printf(Rus("\nНе правильно, по новой\n"));
                continue;
            }
            nline = stoi(temp);
            if (!(nline >= 0 && nline <= filesol.size() - 1)) {
                printf(Rus("Не правильно, по новой\n"));
                continue;
            }
            printf(Rus("Введите последовательность символов, которую вы хотите найти: "));
            std::cin.ignore();
            getline(cin, temp);

            cout << "\n";
            filesol[nline] = vstavka(filesol, nline, temp);
            cout << filesol[nline] << endl;
        }
        else if (wntd == 4) break;
        else {
            printf(Rus("Не правильно, по новой"));
            continue;
        }


    } while (wntd != 4);
    file.close();
    wntd = 0;
    fstream File; 
    File.open(FileDir.c_str());
    if (!File.is_open()) {
        printf(Rus("Нет возможности записать информацию в файл.\n"));
        return 0;
    }
    else {
        File.close();
        File.open(FileDir.c_str());
        printf(Rus("1 - Сохранить изменения\n"));
        printf(Rus("2 - Не сохранять изменения\n"));
        do {
            cout << Rus("Введите команду: ");
            cin >> wntds;
            if (!(wntds == "1" || wntds == "2")) wntd = 0;
            else wntd = stoi(wntds);
        } while (!(wntd == 1 || wntd == 2));

        if (wntd == 1) {
            for (int i = 0; i < filesol.size(); i++) {
                if (i != filesol.size() - 1) {
                    File << filesol[i] << "\n";
                }
                else {
                    File << filesol[i];
                }
            }
            printf(Rus("Файл сохранён. Спасибо за внимание\n"));
        }
        else {
            printf(Rus("Спасибо за внимание\n"));
        }
    }
    File.close();

    return 0;
}
// C:\Users\fibik\Desktop\TANDTP\TenLAB\x64\Debug\gay.txt

void poiskString(string temp, vector<string> filesol, int nline) {
    CONSOLE_SCREEN_BUFFER_INFO start_attribute;
    GetConsoleScreenBufferInfo(hStdout, &start_attribute);
    string poisk = filesol[nline];
    if (temp == "") {
        printf(Rus("Неверный символ"));
        return;
    }
    if (poisk.size() < temp.size()) {
        printf(Rus("Вхождений нет"));
        return;
    }
    for (int i = 0; i <= poisk.length() - temp.length(); )
    {
        // получаем индекс
        size_t position = poisk.find(temp, i);
        // если не найдено ни одного вхождения с индекса i, выходим из цикла
        if (position == std::string::npos) {
            for (int j = i; j < poisk.length(); j++) {
                cout << poisk[j];
            }
            break;
        }
        if (i < position){
            for (int j = i; j < (position); j++) {
                //SetConsoleTextAttribute(hStdout, (10 << 4) | 15);
                cout << poisk[j];
            }
            i = position;
        }
        if (i == position) {
            for (int j = position; j < (position + temp.length()); j++) {
                SetConsoleTextAttribute(hStdout, (10 << 4) | 15);
                cout << poisk[j];
            }
            i += temp.length();
        }
        SetConsoleTextAttribute(hStdout, start_attribute.wAttributes);
        // если же вхождение найдено, увеличиваем счетчик вхождений
        //++count;
        // переходим к следующему индексу после position
        //i = position + 1;
    }
    return;
}

void tlow(vector<string> &temp, int nline) {
    vector<string> wordarray;
    string word;
    int x;
    int i;
    int y = temp[nline].size(); // You want to get the length of the whole temp[nline].

    if (temp[nline][0] != ' ') {
        x = temp[nline].find(" ");
        temp[nline][0] = toupper(temp[nline][0]);
        int i = x;
        for (i = 1;i < y;i++) {
            if (isalpha(temp[nline][i]) && temp[nline][i - 1] == ' ') {
                // only first letters of a word.
                temp[nline][i] = toupper(temp[nline][i]);
            }
        }
    }
    else {
        for (i = 1;i < y;i++) {
            if (isalpha(temp[nline][i]) && temp[nline][i - 1] == ' ') {
                // only first letters of a word.
                temp[nline][i] = toupper(temp[nline][i]);
            }
        }
    }

}
string vstavka(vector<string> filesol, int nline, string temp) {
    string temp2; 
    temp2.append(temp).append(filesol[nline]);
    return temp2;
}