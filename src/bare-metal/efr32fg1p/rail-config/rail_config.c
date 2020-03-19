/***************************************************************************//**
 * @brief RAIL Configuration
 * @details
 *   WARNING: Auto-Generated Radio Config  -  DO NOT EDIT
 *   Radio Configurator Version: 4.9.1
 *   RAIL Adapter Version: 2.4.5
 *   RAIL Compatibility: 2.x
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of  Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "em_common.h"
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
  { 0xFFFFFFFFUL }
};

static const uint32_t phyInfo[] = {
  4UL,
  0x00CCCCCCUL, // 204.8
  (uint32_t) NULL,
  (uint32_t) irCalConfig,
#ifdef RADIO_CONFIG_ENABLE_TIMING
  (uint32_t) &Channel_Group_1_timing,
#else
  (uint32_t) NULL,
#endif
  0x00000000UL,
  16392000UL,
  32000000UL,
  2000000UL,
  (1UL << 8) | 1UL,
  0x02004924UL,
};

const uint32_t Protocol_Configuration_modemConfigBase[] = {
  0x01040FF0UL, (uint32_t) &phyInfo,
  /*    0FF4 */ 0x00000000UL,
  /*    0FF8 */ 0x0003C000UL,
  /*    0FFC */ 0x0003C00FUL,
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
  0x000400A0UL, 0x00004801UL,
  /*    00A4 */ 0x00004CFFUL,
  /*    00A8 */ 0x00004901UL,
  /*    00AC */ 0x00004DFFUL,
  0x00012000UL, 0x000007C4UL,
  0x00012010UL, 0x00000000UL,
  0x00012018UL, 0x0000A001UL,
  0x00013008UL, 0x0000AC3FUL,
  0x00023030UL, 0x00104924UL,
  /*    3034 */ 0x00000001UL,
  0x00013040UL, 0x00000000UL,
  0x000140A0UL, 0x0F00277AUL,
  0x000140F4UL, 0x00001020UL,
  0x00024134UL, 0x00000880UL,
  /*    4138 */ 0x000087E6UL,
  0x00024140UL, 0x0088006DUL,
  /*    4144 */ 0x1153E6C0UL,
  0x00156014UL, 0x00000010UL,
  /*    6018 */ 0x04000000UL,
  /*    601C */ 0x0C00000FUL,
  /*    6020 */ 0x00003000UL,
  /*    6024 */ 0x000C1000UL,
  /*    6028 */ 0x03000000UL,
  /*    602C */ 0x00000000UL,
  /*    6030 */ 0x00FF0264UL,
  /*    6034 */ 0x000008A2UL,
  /*    6038 */ 0x00000001UL,
  /*    603C */ 0x00140012UL,
  /*    6040 */ 0x0000B16FUL,
  /*    6044 */ 0x00000000UL,
  /*    6048 */ 0x1DE00714UL,
  /*    604C */ 0x00000000UL,
  /*    6050 */ 0x002B0011UL,
  /*    6054 */ 0x00000000UL,
  /*    6058 */ 0x00000000UL,
  /*    605C */ 0x22140A04UL,
  /*    6060 */ 0x504B4133UL,
  /*    6064 */ 0x00000000UL,
  0x10017014UL, 0x0007F800UL,
  0x30017014UL, 0x000000F8UL,
  0x00027018UL, 0x00000100UL,
  /*    701C */ 0x82730000UL,
  0x00017028UL, 0x01800000UL,
  0x00027048UL, 0x00003D3CUL,
  /*    704C */ 0x000019BCUL,
  0x00037070UL, 0x00020105UL,
  /*    7074 */ 0x00000433UL,
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
    .attr = &channelConfigEntryAttr
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

uint32_t protocolAccelerationBuffer[133];
