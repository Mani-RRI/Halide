#include "Halide.h"

#include "halide_benchmark.h"
#include <cstdio>

using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char **argv) {
    Target target = get_jit_target_from_environment();
    if (target.arch == Target::WebAssembly) {
        printf("[SKIP] Performance tests are meaningless and/or misleading under WebAssembly interpreter.\n");
        return 0;
    }

    Var x;

    ImageParam a(Int(32), 1);
    Buffer<int> b(1), c(1);
    b(0) = 17;
    c(0) = 0;
    a.set(c);

    int expected = 0;
    double t = benchmark([&]() {
        Func f;
        f(x) = a(x) + b(x);
        f.realize(c);
        expected += 17;
        assert(c(0) == expected);
    });

    printf("%g ms per jit compilation\n", t * 1e3);

    printf("Success!\n");
    return 0;
}
