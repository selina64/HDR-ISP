/**
 * @file depwl.cpp
 * @author joker.mao (joker_mao@163.com)
 * @brief decode pwl curve
 *        事实上就是把分段函数代入进去，不过uint16转换为了int32。
 *        分段函数在json文件里有定义（可能相机拍的的raw文件里也会有？）
* TODO:   为什么要转换成有符号数？按道理到这一步为止，应该都大于0呀？
          是不是说，既然要转数据类型，不如一步到位转到int32，省得后面再转一次了？
 * @version 0.1
 * @date 2023-07-27
 * Copyright (c) of ADAS_EYES 2023
 */
#include "modules/modules.h"

#define MOD_NAME "depwl"

static int Depwl(Frame *frame, const IspPrms *isp_prm)
{
    if ((frame == nullptr) || (isp_prm == nullptr))
    {
        LOG(ERROR) << "input prms is null";
        return -1;
    }
    int pixel_idx = 0;
    int pwl_idx = 0;

    uint16_t* raw16_in = reinterpret_cast<uint16_t *>(frame->data.raw_u16_i);
    int32_t*  raw32_out = reinterpret_cast<int32_t *>(frame->data.raw_s32_o);

    const DePwlPrms *pwl_prm = &(isp_prm->depwl_prm);

    FOR_ITER(h, frame->info.height)
    {
        FOR_ITER(w, frame->info.width)
        {
            pixel_idx = h * frame->info.width + w;
            for(int index = 1; index < pwl_prm->pwl_nums; ++index){
                if (raw16_in[pixel_idx] <= pwl_prm->x_cood[index]) {
                    pwl_idx = index;
                    break;
                }
                pwl_idx = index;
            }
            if (pwl_idx == 0) {
                return -1;
            }
            //y = slope * (Xn - Xn-1) + Yn-1
            raw32_out[pixel_idx] = (raw16_in[pixel_idx] - pwl_prm->x_cood[pwl_idx - 1]) * pwl_prm->slope[pwl_idx] \
                                 + pwl_prm->y_cood[pwl_idx - 1];

            ClipMinMax(raw32_out[pixel_idx], isp_prm->info.max_val, 0);
        }
    }

    SwapMem<void>(frame->data.raw_s32_o, frame->data.raw_s32_i);

    return 0;
}

void RegisterDePwlMod()
{
    IspModule mod;

    mod.in_type = DataPtrTypes::TYPE_UINT16;
    mod.out_type = DataPtrTypes::TYPE_INT32;


    mod.in_domain = ColorDomains::RAW;
    mod.out_domain = ColorDomains::RAW;

    mod.name = MOD_NAME;
    mod.run_function = Depwl;

    RegisterIspModule(mod);
}