/**
 * @file rgbgamma.cpp
 * @author joker.mao (joker_mao@163.com)
 * @brief
 * 
 *          一间黑屋子中，点亮了一只灯泡A，人眼会感觉照亮整间屋子，持续点亮第2个、第2个......灯泡后，人眼会感觉屋子逐渐变得明亮，
 *          此时再点亮第N+1个灯泡，其实人眼没有什么感觉甚至微乎其微，为什么？
 *          
 *          亮度对人眼的刺激是非线性的，第1个和最后一个灯泡点亮对人眼的刺激感觉是不同的；人眼感觉黑->白范围“有限”，灯泡可以无限，但感觉会趋于一个有限制值；
 * 
 *          gamma修正基于的现实就是：人眼对于亮度的感知是非线性的，而显示设备的亮度输出通常是线性的。
 *          在低亮度下，人眼对于亮度变化的敏感度较高；而在高亮度下，人眼对于亮度变化的敏感度较低。这就是所谓的Weber-Fechner定律。
 *          因此，我们也需要根据人眼的非线性视觉响应特性来选择合适的gamma修正值，降低亮部对比度，提高暗部对比度。
 *          gamma校正的一般公式是I_corrected = I_input ^ (1 / gamma)
 * 
 * 
 * TODO: 对Gamma校正有很深的疑问
 * 
 *   (1)            非线性
 *      自然环境-------------->人眼感受
 * 
 *   (2)            线性               线性               非线性
 *      自然环境-------------->相机-------------->屏幕-------------->人眼感受
 * 
 *      以上(1), (2)两种人眼感受，只是在中间插入了两个线性环节，按道理不会有区别，所以不需要gamma校正。不过(2)的预设是人眼对屏幕的感受和对自然物体的感受一致。
 * 
 *      我猜测是，屏幕显示和自然环境的本质区别在于，屏幕作为一种自发光的设备，显示的颜色会改变进入人眼的颜色的亮度，影响人眼对颜色的判断，变成类似线性环节，
 *      如以下(3)所示。
 * 
 *   (3)            线性              非线性               线性
 *      自然环境-------------->相机-------------->屏幕-------------->人眼感受
 *      
 *      假设屏幕由于自发光，所以人眼感受是线性的，此时对照片进行非线性的gamma校正便成了必要。
 * 
 *      即使以上猜测正确，但是新的问题又出现了，即打印的照片也是利用反光显示颜色，那照片经过gamma校正再被打印出来，是否也会造成对比度区别的问题？
 *      
 *      换言之，文件作为一种标准的存储方式，是否不应该考虑“屏幕”或“打印机”这种特定输出设备的特性？gamma校正是否应该交给输出设备进行？
 *      （事实上，我的显示器确实也有gamma校正的设置，gamma值也是2.2~2.4可选）
 * 
 * @version 0.1
 * @date 2023-08-10
 *
 * Copyright (c) of ADAS_EYES 2023
 *
 */

#include "modules/modules.h"

#define MOD_NAME "rgbgamma"

static int RgbGamma(Frame *frame, const IspPrms *isp_prm)
{
    if ((frame == nullptr) || (isp_prm == nullptr))
    {
        LOG(ERROR) << "input prms is null";
        return -1;
    }
    int pixel_idx = 0;

    const auto &gamma_prm = isp_prm->rgb_gamma;

    float step_coff = (float)(gamma_prm.nums - 1) / (1 << gamma_prm.in_bits);
    float out_max = (1 << gamma_prm.out_bits) - 1;

    int32_t *bgr_i = reinterpret_cast<int32_t *>(frame->data.bgr_s32_i);
    int32_t *bgr_o = reinterpret_cast<int32_t *>(frame->data.bgr_s32_o);

    FOR_ITER(h, frame->info.height)
    {
        FOR_ITER(w, frame->info.width)
        {
            pixel_idx = h * frame->info.width + w;

            FOR_ITER(color_idx, 3)
            {
                auto color = bgr_i[3 * pixel_idx + color_idx];

                float cuvre_id_f = color * step_coff;
                int curve_id = static_cast<int>(cuvre_id_f);
                // scale to 0~1
                float scale = (cuvre_id_f - curve_id) * (gamma_prm.curve[curve_id + 1] - gamma_prm.curve[curve_id]) + gamma_prm.curve[curve_id];
                // get scale value
                color = out_max * scale;
                // ClipMinMax(color, isp_prm->info.max_val, 0);
                bgr_o[3 * pixel_idx + color_idx] = color;
            }
        }
    }

    SwapMem<void>(frame->data.bgr_s32_i, frame->data.bgr_s32_o);

    return 0;
}

void RegisterRgbGammaMod()
{
    IspModule mod;

    mod.in_type = DataPtrTypes::TYPE_INT32;
    mod.out_type = DataPtrTypes::TYPE_INT32;

    mod.in_domain = ColorDomains::BGR;
    mod.out_domain = ColorDomains::BGR;

    mod.name = MOD_NAME;
    mod.run_function = RgbGamma;

    RegisterIspModule(mod);
}