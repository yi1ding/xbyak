#include "xbyak/xbyak.h"
#include "xbyak/xbyak_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#ifdef XBYAK64_GCC
class TestVPMOVSDB : public Xbyak::CodeGenerator {
public:
  TestVPMOVSDB() {
    Xbyak::util::StackFrame sf(this, 2, 1);
    vmovdqu8(v_src, ptr[sf.p[0]]);
    mov(sf.t[0], 0b0101010101010101);
    kmovd(k_zebra, sf.t[0].cvt32());
    vpmovsdb(ptr[sf.p[1]] | k_zebra, v_src);
  }
  void (*get() const)(int *, int8_t *) {
    return getCode<void (*)(int *, int8_t *)>();
  }

private:
  Xbyak::Zmm v_src = zmm31;
  Xbyak::Opmask k_zebra = k7;
};
#endif

int main() {
  try {
    int src[16] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 128, 129, 130, 131, 132, 133,
    };
    int8_t dst[16];
    for (size_t i = 0; i < 16; ++i) {
      dst[i] = 0;
    }

    TestVPMOVSDB a;
    void (*code)(int *, int8_t *) = a.get();
    code(src, dst);

    for (size_t i = 0; i < 16; ++i) {
      printf("%d ", dst[i]);
    }
    puts("OK");
  } catch (std::exception &e) {
    printf("ERR:%s\n", e.what());
  } catch (...) {
    printf("unknown error\n");
  }
}
