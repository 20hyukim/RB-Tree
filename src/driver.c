#include "rbtree.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    rbtree* rbt = new_rbtree();

    while(1) {
        printf("\n1. insert / 2. find and erase / 3. delete rbtree and exit / 4. test\n: ");
        int input;
        scanf("%d", &input);

        printf("\n");
        int key;
        switch(input) {
            case 1:
                printf("insert key: ");
                scanf("%d", &key);
                rbtree_insert(rbt, key);
                print_RBTREE(rbt);
            break;

            case 2:
                printf("delete key: ");
                scanf("%d", &key);
                rbtree_erase(rbt, rbtree_find(rbt, key));
                print_RBTREE(rbt);
            break;

            case 3:
                delete_rbtree(rbt);
                return;
            break;

            case 4:
                left_rotate(rbt, rbt->root);
                print_RBTREE(rbt);
            break;
        }
    }
}