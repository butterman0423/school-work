#include <cstdlib>
#include <unistd.h>

class ArrayList {
public:
  u_int64_t len;
  int *dataarr;

  void init_arr(u_int64_t size);

  void iterate_arr(u_int64_t i);
};