#include <stdio.h>

#include "math.h"

int main(){
    Math::Vector2 pos{5.f, 1.f};
    printf("Pos: %f, %f\n", pos.x, pos.y);
    Math::Vector2 norm_pos = pos.GetNormalized();
    printf("Normalized Pos: %f, %f -> %f, %f", pos.x, pos.y, norm_pos.x, norm_pos.y);
}