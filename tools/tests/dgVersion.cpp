#include <dragon/dragon.h>

int main(void) {
    if(!dgInit()) return 1;
    dgEngine* e = dgCreateEngine("dgVersionInfo");
    dgPrintVersionInfo(e);

    return 0;
}