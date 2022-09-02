#include <dragon/dragon.h>

int main(void) {
    if(!dgInit()) return 1;
    try {
        dgPrintVersionInfo();
    } catch(std::exception e) {
        printf(e.what());
    }

    return 0;
}