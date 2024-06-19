#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>


using std::vector;
using std::string;

size_t state = 0;
size_t input;
size_t coin;
int main () {
    string message = "";
    bool loop = true;
    bool recieved = false;
    while(loop) {
        std::cout << "\033c";
        std::cout << message;
        std::cout << "Saldo: " << state << "\n";
        std::cout << "1 - 100\n";
        std::cout << "2 - 200\n";
        std::cout << "3 - 500\n";
        std::cout << "4 - 1000\n";
        std::cout << "5 - 2000\n";
        std::cout << "6 - 5000\n";
        std::cout << "7 - 10000\n";
        std::cout << "8 - obtener cafe\n";
        std::cout << "9 - no meter nada\n";
        std::cout << "Igresa monedas: ";
        std::cin >> input;
        
        switch (input) {
            case 1:
                coin = 100;
                break;
            case 2:
                coin = 200;
                break;
            case 3:
                coin = 500;
                break;
            case 4:
                coin = 1000;
                break;
            case 5:
                coin = 2000;
                break;
            case 6:
                coin = 5000;
                break;
            case 7:
                coin = 10000;
                break;
            case 8:
                if (state >= 3500) {
                    state -= 3500;
                    recieved = true;
                }
                coin = 0;
                break;
            default:
                coin = 0;
                break;
        }

        if (recieved) {
            message = "Recibes un cafe\n";
            recieved = false;
        }
        else if (state >= 3500) {
            message = "Ya tienes suficiente saldo\n";
            continue;
        }
        else if (state + coin >= 3500) {
            message = "Ya tienes suficiente saldo\n";
        } 
        else {
            message = "No tienes suficiente saldo\n";
        }

        state += coin;
    }
}
