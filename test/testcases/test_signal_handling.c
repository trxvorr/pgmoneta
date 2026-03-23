#include <pgmoneta.h>
#include <mctf.h>
#include <tscommon.h>
#include <utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

MCTF_TEST(test_signal_handling_segv)
{
   char* output = NULL;
   int exit_code = 0;

   pgmoneta_test_setup();

   /* Run a command that causes a segmentation fault */
   /* bash -c 'kill -SEGV $$' is a reliable way to check pclose signal handling */
   MCTF_ASSERT_INT_EQ(pgmoneta_test_exec_command("bash -c 'kill -SEGV $$'", &output, &exit_code), 0, cleanup, "Failed to execute command");

   /* 128 + SIGSEGV (11) = 139 */
   MCTF_ASSERT_INT_EQ(exit_code, 139, cleanup, "Expected exit code 139 for SIGSEGV, got %d. Output: %s", exit_code, output);

cleanup:
   free(output);
   pgmoneta_test_teardown();
   MCTF_FINISH();
}

MCTF_TEST(test_signal_handling_abrt)
{
   char* output = NULL;
   int exit_code = 0;

   pgmoneta_test_setup();

   /* Run a command that causes an abort */
   MCTF_ASSERT_INT_EQ(pgmoneta_test_exec_command("bash -c 'kill -ABRT $$'", &output, &exit_code), 0, cleanup, "Failed to execute command");

   /* 128 + SIGABRT (6) = 134 */
   MCTF_ASSERT_INT_EQ(exit_code, 134, cleanup, "Expected exit code 134 for SIGABRT, got %d. Output: %s", exit_code, output);

cleanup:
   free(output);
   pgmoneta_test_teardown();
   MCTF_FINISH();
}
