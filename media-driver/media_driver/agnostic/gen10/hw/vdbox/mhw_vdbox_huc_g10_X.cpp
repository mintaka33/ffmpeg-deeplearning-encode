/*
* Copyright (c) 2017-2018, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file     mhw_vdbox_huc_g10_X.cpp
//! \brief    Constructs VdBox Huc commands on Gen10 based platforms

#include "mhw_vdbox_huc_g10_X.h"
#include "mhw_vdbox_vdenc_hwcmd_g10_X.h"
#include "mhw_mmio_g10.h"

void MhwVdboxHucInterfaceG10::InitMmioRegisters()
{
    MmioRegistersHuc *mmioRegisters = &m_mmioRegisters[MHW_VDBOX_NODE_1];

    mmioRegisters->hucUKernelHdrInfoRegOffset = HUC_UKERNEL_HDR_INFO_REG_OFFSET_NODE_1_INIT_G10;
    mmioRegisters->hucStatusRegOffset         = HUC_STATUS_REG_OFFSET_NODE_1_INIT_G10;
    mmioRegisters->hucStatus2RegOffset        = HUC_STATUS2_REG_OFFSET_NODE_1_INIT_G10;

    mmioRegisters = &m_mmioRegisters[MHW_VDBOX_NODE_2];

    mmioRegisters->hucUKernelHdrInfoRegOffset = HUC_UKERNEL_HDR_INFO_REG_OFFSET_NODE_2_INIT_G10;
    mmioRegisters->hucStatusRegOffset         = HUC_STATUS_REG_OFFSET_NODE_2_INIT_G10;
    mmioRegisters->hucStatus2RegOffset        = HUC_STATUS2_REG_OFFSET_NODE_2_INIT_G10;
}

MOS_STATUS MhwVdboxHucInterfaceG10::GetHucStateCommandSize(
    uint32_t                        mode,
    uint32_t                        *commandsSize,
    uint32_t                        *patchListSize,
    PMHW_VDBOX_STATE_CMDSIZE_PARAMS params)
{
    MHW_FUNCTION_ENTER;

    MHW_MI_CHK_NULL(commandsSize);
    MHW_MI_CHK_NULL(patchListSize);

    MHW_MI_CHK_STATUS((MhwVdboxHucInterfaceGeneric<mhw_vdbox_huc_g10_X, mhw_mi_g10_X>::
        GetHucStateCommandSize(mode, commandsSize, patchListSize, params)));

    *commandsSize  += mhw_vdbox_vdenc_g10_X::VD_PIPELINE_FLUSH_CMD::byteSize;
    *patchListSize += PATCH_LIST_COMMAND(VD_PIPELINE_FLUSH_CMD);

    return MOS_STATUS_SUCCESS;
}

MOS_STATUS MhwVdboxHucInterfaceG10::AddHucPipeModeSelectCmd(
    MOS_COMMAND_BUFFER                  *cmdBuffer,
    MHW_VDBOX_PIPE_MODE_SELECT_PARAMS   *params)
{
    MHW_MI_CHK_NULL(cmdBuffer);
    MHW_MI_CHK_NULL(params);

    mhw_vdbox_huc_g10_X::HUC_PIPE_MODE_SELECT_CMD       cmd;

    if (!params->disableProtectionSetting)
    {
        MHW_MI_CHK_STATUS(m_cpInterface->SetProtectionSettingsForHucPipeModeSelect((uint32_t *)&cmd));
    }
    cmd.DW1.IndirectStreamOutEnable = params->bStreamOutEnabled;
    cmd.DW1.HucStreamObjectEnable = params->bStreamObjectUsed;
    cmd.DW2.MediaSoftResetCounterPer1000Clocks = params->dwMediaSoftResetCounterValue;

    MHW_MI_CHK_STATUS(Mos_AddCommand(cmdBuffer, &cmd, cmd.byteSize));

    return MOS_STATUS_SUCCESS;
}
