#include <stdio.h>
#include <assert.h>

struct fullDataTurn {

    int (*test())(int a, int b);

};

int bot4move()

int (*test())(int a, int b) main(int argc, char** argv) {

    for (int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            assert(argv[i][1] == 'b' || argv[i][1] == 'w');

            if(argv[i][1] == 'b') {
                switch(argv[i + 1]) {
                    case "bot4":
                        return bot4move();
                }
            }
        }
    }
}