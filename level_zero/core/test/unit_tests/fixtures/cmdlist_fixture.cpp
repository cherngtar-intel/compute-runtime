/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "level_zero/core/test/unit_tests/fixtures/cmdlist_fixture.h"

namespace L0 {
namespace ult {

void CommandListFixture::setUp() {
    DeviceFixture::setUp();
    ze_result_t returnValue;
    commandList.reset(whiteboxCast(CommandList::create(device->getHwInfo().platform.eProductFamily, device, NEO::EngineGroupType::RenderCompute, 0u, returnValue)));

    ze_event_pool_desc_t eventPoolDesc = {};
    eventPoolDesc.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE;
    eventPoolDesc.count = 2;

    ze_event_desc_t eventDesc = {};
    eventDesc.index = 0;
    eventDesc.wait = 0;
    eventDesc.signal = 0;

    eventPool = std::unique_ptr<EventPool>(EventPool::create(driverHandle.get(), context, 0, nullptr, &eventPoolDesc, returnValue));
    event = std::unique_ptr<Event>(Event::create<uint32_t>(eventPool.get(), &eventDesc, device));
}

void CommandListFixture::tearDown() {
    event.reset(nullptr);
    eventPool.reset(nullptr);
    commandList.reset(nullptr);
    DeviceFixture::tearDown();
}

void MultiTileCommandListFixtureInit::setUp() {
    DebugManager.flags.EnableImplicitScaling.set(1);
    osLocalMemoryBackup = std::make_unique<VariableBackup<bool>>(&NEO::OSInterface::osEnableLocalMemory, true);
    apiSupportBackup = std::make_unique<VariableBackup<bool>>(&NEO::ImplicitScaling::apiSupport, true);

    SingleRootMultiSubDeviceFixture::setUp();
}

void MultiTileCommandListFixtureInit::setUpParams(bool createImmediate, bool createInternal, bool createCopy) {
    ze_result_t returnValue;

    NEO::EngineGroupType cmdListEngineType = createCopy ? NEO::EngineGroupType::Copy : NEO::EngineGroupType::RenderCompute;

    if (!createImmediate) {
        commandList.reset(whiteboxCast(CommandList::create(device->getHwInfo().platform.eProductFamily, device, cmdListEngineType, 0u, returnValue)));
    } else {
        const ze_command_queue_desc_t desc = {};
        commandList.reset(whiteboxCast(CommandList::createImmediate(device->getHwInfo().platform.eProductFamily, device, &desc, createInternal, cmdListEngineType, returnValue)));
    }
    ASSERT_EQ(ZE_RESULT_SUCCESS, returnValue);

    ze_event_pool_desc_t eventPoolDesc = {};
    eventPoolDesc.flags = ZE_EVENT_POOL_FLAG_HOST_VISIBLE;
    eventPoolDesc.count = 2;

    ze_event_desc_t eventDesc = {};
    eventDesc.index = 0;
    eventDesc.wait = 0;
    eventDesc.signal = 0;

    eventPool = std::unique_ptr<EventPool>(EventPool::create(driverHandle.get(), context, 0, nullptr, &eventPoolDesc, returnValue));
    event = std::unique_ptr<Event>(Event::create<uint32_t>(eventPool.get(), &eventDesc, device));
}

} // namespace ult
} // namespace L0