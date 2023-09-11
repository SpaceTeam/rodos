/***************************************************************************//**
 * @brief RAIL Configuration
 * @details
 *   WARNING: Auto-Generated Radio Config  -  DO NOT EDIT
 *   Radio Configurator Version: 2022.1.1
 *   RAIL Adapter Version: 2.4.18
 *   RAIL Compatibility: 2.x
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_device.h"
#include "rail_config.h"

uint32_t RAILCb_CalcSymbolRate(RAIL_Handle_t railHandle)
{
    (void) railHandle;
    return 0U;
}

uint32_t RAILCb_CalcBitRate(RAIL_Handle_t railHandle)
{
    (void) railHandle;
    return 0U;
}

void RAILCb_ConfigFrameTypeLength(RAIL_Handle_t railHandle,
                                  const RAIL_FrameType_t *frameType)
{
    (void) railHandle;
    (void) frameType;
}

static const uint8_t irCalConfig[] = {
        25, 63, 1, 6, 4, 16, 1, 0, 0, 1, 1, 6, 0, 16, 39, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0
};

static RAIL_ChannelConfigEntryAttr_t channelConfigEntryAttr = {
#if RAIL_SUPPORTS_OFDM_PA
        {
    { 0xFFFFFFFFUL },
    { 0xFFFFFFFFUL, 0xFFFFFFFFUL }
  }
#else // RAIL_SUPPORTS_OFDM_PA
        { 0xFFFFFFFFUL },
#endif // RAIL_SUPPORTS_OFDM_PA
};

static const uint32_t phyInfo[] = {
        11UL,
        0x00C30C30UL, // 195.04761904761907
        (uint32_t) NULL,
        (uint32_t) irCalConfig,
#ifdef RADIO_CONFIG_ENABLE_TIMING
        (uint32_t) &Channel_Group_1_timing,
#else
        (uint32_t) NULL,
#endif
        0x00000000UL,
        12000000UL,
        42000000UL,
        2000000UL,
        0x00000101UL,
        0x02006147UL,
        (uint32_t) NULL,
        (uint32_t) NULL,
        (uint32_t) NULL,
        0UL,
        0UL,
        2000488UL,
};

const uint32_t Protocol_Configuration_modemConfigBase[] = {
        0x01041FDCUL, 0x00000000UL,
        /*    1FE0 */ 0x00000000UL,
        /*    1FE4 */ 0x00000000UL,
        /*    1FE8 */ 0x00000000UL,
        0x01041FF0UL, 0x0037003FUL,
        /*    1FF4 */ 0x00000000UL,
        /*    1FF8 */ (uint32_t) &phyInfo,
        /*    1FFC */ 0x00000000UL,
        0x00020004UL, 0x0001C104UL,
        /*    0008 */ 0x00000FFEUL,
        0x00020018UL, 0x00000000UL,
        /*    001C */ 0x00000000UL,
        0x00070028UL, 0x00000000UL,
        /*    002C */ 0x00000000UL,
        /*    0030 */ 0x00000000UL,
        /*    0034 */ 0x00000000UL,
        /*    0038 */ 0x00000000UL,
        /*    003C */ 0x00000000UL,
        /*    0040 */ 0x000007A4UL,
        0x00010048UL, 0x00000000UL,
        0x00020054UL, 0x00000000UL,
        /*    0058 */ 0x00000000UL,
        0x000400A0UL, 0x00004001UL,
        /*    00A4 */ 0x00004CFFUL,
        /*    00A8 */ 0x00004101UL,
        /*    00AC */ 0x00004DFFUL,
        0x00012000UL, 0x000007C4UL,
        0x00012010UL, 0x00000000UL,
        0x00012018UL, 0x0000A001UL,
        0x00013008UL, 0x0100AC13UL,
        0x00023030UL, 0x00106147UL,
        /*    3034 */ 0x00000001UL,
        0x00013040UL, 0x00000000UL,
        0x000140A0UL, 0x0F00277AUL,
        0x000140B8UL, 0x00F3C000UL,
        0x000140F4UL, 0x00001020UL,
        0x00024134UL, 0x00000880UL,
        /*    4138 */ 0x000087E6UL,
        0x00024140UL, 0x0088008FUL,
        /*    4144 */ 0x4D52E6C0UL,
        0x00044160UL, 0x00000000UL,
        /*    4164 */ 0x00000000UL,
        /*    4168 */ 0x00000006UL,
        /*    416C */ 0x00000006UL,
        0x00086014UL, 0x00000010UL,
        /*    6018 */ 0x04000000UL,
        /*    601C */ 0x0002000FUL,
        /*    6020 */ 0x00005000UL,
        /*    6024 */ 0x000C1000UL,
        /*    6028 */ 0x03000000UL,
        /*    602C */ 0x00000000UL,
        /*    6030 */ 0x00000000UL,
        0x00066050UL, 0x00FF0264UL,
        /*    6054 */ 0x00000C41UL,
        /*    6058 */ 0x00000000UL,
        /*    605C */ 0x00140012UL,
        /*    6060 */ 0x0000B16FUL,
        /*    6064 */ 0x00000000UL,
        0x000C6078UL, 0x13C00714UL,
        /*    607C */ 0x00000000UL,
        /*    6080 */ 0x003B0011UL,
        /*    6084 */ 0x0195907BUL,
        /*    6088 */ 0x000002A4UL,
        /*    608C */ 0x22140A04UL,
        /*    6090 */ 0x4F4A4132UL,
        /*    6094 */ 0x00000000UL,
        /*    6098 */ 0x00000000UL,
        /*    609C */ 0x00000000UL,
        /*    60A0 */ 0x00000000UL,
        /*    60A4 */ 0x00000000UL,
        0x000760E4UL, 0x8BA80080UL,
        /*    60E8 */ 0x00000000UL,
        /*    60EC */ 0x07830464UL,
        /*    60F0 */ 0x3AC81388UL,
        /*    60F4 */ 0x0006209CUL,
        /*    60F8 */ 0x00206100UL,
        /*    60FC */ 0x208556B7UL,
        0x00036104UL, 0x001247A9UL,
        /*    6108 */ 0x00003020UL,
        /*    610C */ 0x0000BB88UL,
        0x00016120UL, 0x00000000UL,
        0x10017014UL, 0x0007F800UL,
        0x30017014UL, 0x000000F8UL,
        0x00067018UL, 0x00000300UL,
        /*    701C */ 0x84EA0060UL,
        /*    7020 */ 0x00000000UL,
        /*    7024 */ 0x00000082UL,
        /*    7028 */ 0x01800000UL,
        /*    702C */ 0x000000D5UL,
        0x00027048UL, 0x00003D3CUL,
        /*    704C */ 0x000019BCUL,
        0x00037070UL, 0x00220105UL,
        /*    7074 */ 0x0008300BUL,
        /*    7078 */ 0x00552300UL,
        0xFFFFFFFFUL,
};

const RAIL_ChannelConfigEntry_t Protocol_Configuration_channels[] = {
        {
                .phyConfigDeltaAdd = NULL,
                .baseFrequency = 2450000000,
                .channelSpacing = 1000000,
                .physicalChannelOffset = 0,
                .channelNumberStart = 0,
                .channelNumberEnd = 20,
                .maxPower = RAIL_TX_POWER_MAX,
                .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
                .entryType = 0
#endif
        },
};

const RAIL_ChannelConfig_t Protocol_Configuration_channelConfig = {
        .phyConfigBase = Protocol_Configuration_modemConfigBase,
        .phyConfigDeltaSubtract = NULL,
        .configs = Protocol_Configuration_channels,
        .length = 1U,
        .signature = 0UL,
};

const RAIL_ChannelConfig_t *channelConfigs[] = {
        &Protocol_Configuration_channelConfig,
        NULL
};

uint32_t protocolAccelerationBuffer[189];
