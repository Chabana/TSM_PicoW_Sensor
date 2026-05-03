/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_UNIT_TESTS
#include "tests.h"
#include "unity.h"
#include "McuUnity.h"
#include "McuRTOS.h"
#include "McuShell.h"
#include "McuRTT.h"
#include "McuUtility.h"
#include "McuLog.h"

#if 0 /* TODO add tests for sensor */
  #include "test_sensor.h"
#endif

#include "test_leds.h"
#include "test_dns_resolver.h"

static void TestArgFailed(void) {
  TEST_ASSERT_MESSAGE(false, "wrong test_arg value");
}

void Tests_Run(void) {
  int nofFailures;
  int nofBytes;
  unsigned char buf[32];

  nofBytes = McuUnity_RTT_GetArgs(buf, sizeof(buf));

  UNITY_BEGIN();

  if (nofBytes > 0) {
    if (McuUtility_strcmp(buf, "led") == 0) {
      RUN_TEST(TestLeds_OnOff);
      RUN_TEST(TestLeds_Toggle);
    } else if (McuUtility_strcmp(buf, "sensor") == 0) {
#if 0 /* TODO add tests for sensor */
      /* RUN_TEST(TestSensor_...); */
#else
      RUN_TEST(TestArgFailed);
#endif
    } else if (McuUtility_strcmp(buf, "dns") == 0) {
      RUN_TEST(TestDnsResolver_Test);
    } else {
      RUN_TEST(TestArgFailed);
    }
  } else {
    RUN_TEST(TestArgFailed);
  }

  nofFailures = UNITY_END();

#if PL_CONFIG_USE_RTT
  McuUnity_Exit_JRun_RTT(nofFailures == 0);
#else
  McuUnity_Exit_LinkServer_Log(nofFailures == 0);
#endif
}

static void TestTask(void *pv) {
  McuLog_info("starting test task");
  vTaskDelay(pdMS_TO_TICKS(100));
  Tests_Run();
  vTaskDelete(NULL);
}

void Tests_Init(void) {
  if (xTaskCreate(
      TestTask,
      "Test",
      1500 / sizeof(StackType_t),
      (void*)NULL,
      tskIDLE_PRIORITY,
      (TaskHandle_t*)NULL
    ) != pdPASS)
  {
    McuLog_fatal("Failed creating task");
    for(;;) {}
  }
}
#endif /* PL_CONFIG_USE_UNIT_TESTS */