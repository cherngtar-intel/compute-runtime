/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/test/unit_test/xe_hpg_core/os_agnostic_product_helper_xe_lpg_tests.h"

#include "shared/source/os_interface/product_helper.h"
#include "shared/test/common/test_macros/test.h"

using namespace NEO;
void XeLpgTests::testOverridePatIndex(const ProductHelper &productHelper) {
    uint64_t patIndex = 1u;
    bool isUncached = true;
    EXPECT_EQ(2u, productHelper.overridePatIndex(isUncached, patIndex));

    isUncached = false;
    EXPECT_EQ(patIndex, productHelper.overridePatIndex(isUncached, patIndex));
}
