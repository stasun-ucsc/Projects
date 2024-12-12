#include "set.h"

#include <stdio.h>

/*int main(){
	uint32_t x = set_universal();
	printf("%x\n", x);
	x = set_empty();
	printf("%x\n", x);
	x = set_insert(x, 3);
	x = set_insert(x, 1);
	x = set_insert(x, 0);
	printf("%x\n", x);
	x = set_remove(x, 1);
	printf("%x\n", x);
	printf(set_member(x, 1) ? "true\n" : "false\n");
	printf(set_member(x, 0) ? "true\n" : "false\n");
	uint32_t y = set_union(x, set_empty());
	printf("%x\n", y);
	y = set_intersect(x, set_universal());
	printf("%x\n", y);
	x = set_difference(set_universal(), x);
	printf("%x\n", x);
	x = set_complement(x);
	printf("%x\n", x);
}*/

/*I received some help from John Yu on some of these functions*/

Set set_empty(void) {
    return 0x00000000;
}

Set set_universal(void) {
    return 0xFFFFFFFF;
}

Set set_insert(Set s, uint8_t x) {
    return s | (1 << x);
}

Set set_remove(Set s, uint8_t x) {
    return s & ~(1 << x);
}

bool set_member(Set s, uint8_t x) {
    if ((s & (1 << x)) == set_empty()) {
        return false;
    } else {
        return true;
    }
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_difference(Set s, Set t) {
    return s & ~t;
}

Set set_complement(Set s) {
    return ~s;
}
