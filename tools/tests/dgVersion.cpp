#include <dragon/dragon.h>

int main(void) {
    if(!dgInit()) return 1;
    dgPrintVersionInfo();

    return 0;
}