#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h> // Для корректного ввода/вывода UTF-8
#endif

using namespace std;

namespace {
    random_device rd;
    mt19937_64 random_generator { rd() };

    unsigned char s_box[256] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    unsigned char rcon[19] = {
        0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
        0x2f, 0x5e, 0xbc
    };

    const auto Nk = 4;
    const auto Nb = 4;
    const auto Nr = 10;
}

//  Вспомогательные функции вывода
void print_hex(ostream& out, const vector<unsigned char>& data, const string& label) {
    out << label << endl;
    for (size_t i = 0; i < data.size(); ++i) {
        out << hex << setw(2) << setfill('0') << (int)data[i] << " ";
        if ((i + 1) % 16 == 0) out << endl;
    }
    if (data.size() % 16 != 0) out << endl;
    out << endl;
}

void print_text(ostream& out, const string& text, const string& label) {
    out << label << endl << text << endl << endl;
}

// Вывод матрицы состояния 4x4
void print_state(ostream& out, unsigned char state[4][4]) {
    out << "state:" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out << hex << setw(2) << setfill('0') << (int)state[i][j] << " ";
        }
        out << endl;
    }
}

//  Ввод данных
string read_file(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл '" << filename << "'" << endl;
        exit(1);
    }
    ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

string get_input() {
    cout << "Выберите источник ввода:" << endl;
    cout << "  1 — ввести текст в консоль" << endl;
    cout << "  2 — загрузить из файла .txt" << endl;
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;
    cin.ignore(); 

    if (choice == 1) {
        string text;
        cout << "Введите текст: ";
        getline(cin, text);
        return text;
    } else if (choice == 2) {
        string filename;
        cout << "Введите имя файла (например, input.txt): ";
        getline(cin, filename);
        return read_file(filename);
    } else {
        cerr << "Неверный выбор." << endl;
        exit(1);
    }
}

//  Вывод результатов
void output_results(
    const string& key,
    const vector<unsigned char>& ciphertext,
    const string& decrypted,
    const vector<unsigned char>& iv
) {
    cout << endl;
    cout << "Выберите место вывода результатов:" << endl;
    cout << "  1 — только в консоль" << endl;
    cout << "  2 — только в файл .txt" << endl;
    cout << "  3 — в консоль и в файл" << endl;
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string out_filename;
    ofstream file_out;

    if (choice == 2 || choice == 3) {
        cout << "Введите имя выходного файла (например, output.txt): ";
        getline(cin, out_filename);
        file_out.open(out_filename, ios::binary); 
        if (!file_out.is_open()) {
            cerr << "Ошибка: не удалось открыть файл для записи." << endl;
            exit(1);
        }
    }

    auto do_output = [&](ostream& out) {
        out << " Результаты AES-128 OFB " << endl << endl;
        print_text(out, key, "Ключ шифрования:");

        out << "IV (hex):" << endl;
        for (auto b : iv) out << hex << setw(2) << setfill('0') << (int)b << " ";
        out << endl << endl;

        print_hex(out, ciphertext, "Зашифрованный текст (hex):");
        print_text(out, decrypted, "Расшифрованный текст:");
    };

    if (choice == 1 || choice == 3) {
        do_output(cout);
    }
    if (choice == 2 || choice == 3) {
        do_output(file_out);
        file_out.close();
        cout << "Результаты сохранены в '" << out_filename << "'" << endl;
    }
    if (choice < 1 || choice > 3) {
        cerr << "Неверный выбор." << endl;
        exit(1);
    }
}

//  AES-128 вспомогательные функции
vector<unsigned char> expand_key(const std::string& key) {
    vector<unsigned char> expanded_key((Nr + 1) * Nk * Nb, 0);

    for (int i = 0; i < 16; ++i)
        expanded_key[i] = static_cast<unsigned char>(key[i]);

    int bytes_generated = 16;
    int rcon_iter = 1;
    unsigned char temp[4];

    while (bytes_generated < 176) {
        for (int i = 0; i < 4; ++i)
            temp[i] = expanded_key[bytes_generated - 4 + i];

        if (bytes_generated % 16 == 0) {
            unsigned char t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            
            for (int i = 0; i < 4; ++i)
                temp[i] = s_box[temp[i]];
            
            temp[0] ^= rcon[rcon_iter++];
        }

        for (int i = 0; i < 4; ++i) {
            expanded_key[bytes_generated] = expanded_key[bytes_generated - 16] ^ temp[i];
            ++bytes_generated;
        }
    }

    // Key Schedule
    cout << "Key Schedule (hex):" << endl;
    for (int round = 0; round <= Nr; ++round) {
        if (round < 10) cout << "round " << dec << round << ":" << endl;
        else cout << "round a:" << endl; 
        for (int word = 0; word < 4; ++word) {
            for (int byte = 0; byte < 4; ++byte) {
                int idx = round * 16 + word * 4 + byte;
                cout << hex << setw(2) << setfill('0') << (int)expanded_key[idx] << " ";
            }
            cout << endl;
        }
    }
    cout << dec << endl;

    return expanded_key;
}

void sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[i][j] = s_box[state[i][j]];
}

void shift_rows(unsigned char state[4][4]) {
    unsigned char tmp;
    tmp = state[1][0]; state[1][0] = state[1][1]; state[1][1] = state[1][2]; state[1][2] = state[1][3]; state[1][3] = tmp;
    tmp = state[2][0]; unsigned char tmp2 = state[2][1]; state[2][0] = state[2][2]; state[2][1] = state[2][3]; state[2][2] = tmp; state[2][3] = tmp2;
    tmp = state[3][3]; state[3][3] = state[3][2]; state[3][2] = state[3][1]; state[3][1] = state[3][0]; state[3][0] = tmp;
}

unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void mix_columns(unsigned char state[4][4]) {
    unsigned char tmp[4];
    for (int i = 0; i < 4; ++i) {
        tmp[0] = gmul(0x02, state[0][i]) ^ gmul(0x03, state[1][i]) ^ state[2][i]           ^ state[3][i];
        tmp[1] = state[0][i]             ^ gmul(0x02, state[1][i]) ^ gmul(0x03, state[2][i]) ^ state[3][i];
        tmp[2] = state[0][i]             ^ state[1][i]             ^ gmul(0x02, state[2][i]) ^ gmul(0x03, state[3][i]);
        tmp[3] = gmul(0x03, state[0][i]) ^ state[1][i]             ^ state[2][i]             ^ gmul(0x02, state[3][i]);
        for (int j = 0; j < 4; ++j)
            state[j][i] = tmp[j];
    }
}

void add_round_key(unsigned char state[4][4], const vector<unsigned char>& round_key, int round) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[j][i] ^= round_key[round * 16 + i * 4 + j];
}


//  AES-128 основные функции
vector<unsigned char> aes128_encrypt_block(const unsigned char in[16], const vector<unsigned char>& expanded_key) {
    unsigned char state[4][4];
    vector<unsigned char> out_block(16);

    for (int i = 0; i < 16; ++i)
        state[i % 4][i / 4] = in[i];

    print_state(cout, state);

    add_round_key(state, expanded_key, 0);

    for (int round = 1; round < Nr; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, expanded_key, round);
        print_state(cout, state); // Логируем состояние после каждого промежуточного раунда
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, expanded_key, Nr);
    print_state(cout, state);

    for (int i = 0; i < 16; ++i)
        out_block[i] = state[i % 4][i / 4];

    return out_block;
}

vector<unsigned char> aes128_ofb(
    const vector<unsigned char>& input,
    const vector<unsigned char>& expanded_key,
    vector<unsigned char> iv,
    bool silent = false 
) {
    vector<unsigned char> output;
    output.reserve(input.size());
    vector<unsigned char> prev = iv;

    size_t bytes_processed = 0;
    while (bytes_processed < input.size()) {
        vector<unsigned char> keystream;
        if (!silent) {
            keystream = aes128_encrypt_block(prev.data(), expanded_key);
        } else {
            // отладочный вывод во временный буфер (чтобы скрыть его)
            ostringstream oss;
            unsigned char state[4][4];
            for (int i = 0; i < 16; ++i) state[i % 4][i / 4] = prev[i];
            add_round_key(state, expanded_key, 0);
            for (int round = 1; round < Nr; ++round) {
                sub_bytes(state); shift_rows(state); mix_columns(state); add_round_key(state, expanded_key, round);
            }
            sub_bytes(state); shift_rows(state); add_round_key(state, expanded_key, Nr);
            keystream.resize(16);
            for (int i = 0; i < 16; ++i) keystream[i] = state[i % 4][i / 4];
        }

        for (int i = 0; i < 16 && bytes_processed < input.size(); ++i, ++bytes_processed) {
            output.push_back(input[bytes_processed] ^ keystream[i]);
        }
        prev = keystream; 
    }
    return output;
}

string random_key() {
    static const string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string key(16, 0);
    uniform_int_distribution<> dist(0, chars.size() - 1);
    for (int i = 0; i < 16; ++i)
        key[i] = chars[dist(random_generator)];
    return key;
}

int main() {
    setlocale(LC_ALL, ".UTF-8");
    
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    //  Чтение входного текста 
    string input = get_input();
    if (input.empty()) {
        cerr << "Пустой ввод!" << endl;
        return 1;
    }

    //  Генерация и сохранение ключа
    const string key = random_key();
    cout << endl << "Сгенерированный ключ: " << key << endl;

    {
        ofstream key_file("key.txt");
        if (key_file.is_open()) {
            key_file << key;
        }
    }

    //  Расширение ключа 
    const auto expanded_key = expand_key(key);

    //  Инициализация вектора IV
    vector<unsigned char> iv(16, 0x00);

    //  Шифрование в режиме OFB 
    vector<unsigned char> input_bytes(input.begin(), input.end());
    auto ciphertext = aes128_ofb(input_bytes, expanded_key, iv, false);

    //  Расшифрование в режиме OFB 
    auto decrypted_bytes = aes128_ofb(ciphertext, expanded_key, iv, true);
    string decrypted(decrypted_bytes.begin(), decrypted_bytes.end());

    //  Интерактивный вывод результатов
    output_results(key, ciphertext, decrypted, iv);

    return 0;
}

