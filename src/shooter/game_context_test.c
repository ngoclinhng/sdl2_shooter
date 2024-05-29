#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "shooter/game_context.h"

static void test_GameContext_Init(void** state) {
  (void) state;

  GameContext ctx;

  GameContext_Init(&ctx, "Shooter", 1152, 648);

  assert_non_null(ctx.window);
  assert_non_null(ctx.renderer);

  GameContext_Free(&ctx);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_GameContext_Init)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
