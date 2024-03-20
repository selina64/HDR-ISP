/**
 * @file wb_gain.cpp
 * @author joker.mao (joker_mao@163.com)
 * @brief
 *          "d65_gain"指的是在D65光源下用于白平衡校正的增益值.
 *          获取方式为： 在6500K色温下拍摄标准白色卡，得到传感器读数R:GR:GB:B = 2626:1000:1000:1263
 *                      则白平衡增益分别为2.626, 1, 1, 1.263，这里将G通道的增益值规定为1，原因应该是
 *                      它的读数最小，如此规定能让“增益”大于1。
 *          值得注意的是，虽然这里实现了6500K色温下的白平衡校正，但是对于其他条件下的光源并不能适用，即
 *          例如在色温较冷的条件下，白色的物体记录出来并非白色。这种条件下的白平衡并非本步骤任务，而后续
 *          步骤应该会尝试解决它（？）。不过即使如此，D65的校正也是一个很好的起点，对后续处理也很有益。
 * @version 0.1
 * @date 2023-07-27
 *
 * Copyright (c) of ADAS_EYES 2023
 *
 */

#include "modules/modules.h"

#define MOD_NAME "wbgain"

static int WbGain(Frame *frame, const IspPrms *isp_prm)
{
    if ((frame == nullptr) || (isp_prm == nullptr))
    {
        LOG(ERROR) << "input prms is null";
        return -1;
    }
    int pixel_idx = 0;
    int pwl_idx = 0;

    int32_t *raw32_in = reinterpret_cast<int32_t *>(frame->data.raw_s32_i);
    int32_t *raw32_out = reinterpret_cast<int32_t *>(frame->data.raw_s32_o);

    // default d65
    float r_gain = isp_prm->wb_gains.d65_gain[0];
    float gr_gain = isp_prm->wb_gains.d65_gain[1];
    float gb_gain = isp_prm->wb_gains.d65_gain[2];
    float b_gain = isp_prm->wb_gains.d65_gain[3];

    FOR_ITER(h, frame->info.height)
    {
        FOR_ITER(w, frame->info.width)
        {
            pixel_idx = h * frame->info.width + w;

            int cfa_id = static_cast<int>(frame->info.cfa);
            switch (kPixelCfaLut[cfa_id][w % 2][h % 2])
            {
            case PixelCfaTypes::R:
                raw32_out[pixel_idx] = (int32_t)(raw32_in[pixel_idx] * r_gain);
                break;
            case PixelCfaTypes::GR:
                raw32_out[pixel_idx] = (int32_t)(raw32_in[pixel_idx] * gr_gain);
                break;
            case PixelCfaTypes::GB:
                raw32_out[pixel_idx] = (int32_t)(raw32_in[pixel_idx] * gb_gain);
                break;
            case PixelCfaTypes::B:
                raw32_out[pixel_idx] = (int32_t)(raw32_in[pixel_idx] * b_gain);
                break;
            default:
                break;
            }

            ClipMinMax<int32_t>(raw32_out[pixel_idx], (int32_t)isp_prm->info.max_val, 0);
        }
    }

    SwapMem<void>(frame->data.raw_s32_i, frame->data.raw_s32_o);

    return 0;
}

void RegisterWbGaincMod()
{
    IspModule mod;

    mod.in_type = DataPtrTypes::TYPE_INT32;
    mod.out_type = DataPtrTypes::TYPE_INT32;

    mod.in_domain = ColorDomains::RAW;
    mod.out_domain = ColorDomains::RAW;

    mod.name = MOD_NAME;
    mod.run_function = WbGain;

    RegisterIspModule(mod);
}