# Image Signal Process For HDR CMOS Image Sensor

**知乎主页：[ADAS之眼](https://www.zhihu.com/people/fen-shi-qing-nian-29) | 个人网站：[ADAS之眼](https://jokereyeadas.github.io/) | 公众号：**

**![公众号](wechat.png)**

# 支持的ISP模块列表

- Raw Domain
- [x] MipiUnPack: mipi raw data unpack to raw16
- [x] DePwl: decode the pwl curve
- [ ] Dpc: dynamic pixel correct
- [ ] Lsc: lens shding correct
- [x] Blc: black level correct
- [ ] Rns: raw noise filter
- [x] WbGain: white balance gain
- [x] Demoasic: change raw to rgb
- RGB Domain
- [x] Ltm: local tone mapping
- [x] RgbGamma: rgb gamma curve
- [x] Ccm: color correct matrix
- [x] Rgb2Yuv: rgb domain to yuv domain
- YUV Domain
- [x] YGamma: gray gamma curve
- [x] Contrast: gray contrast adjust
- [x] Sharpen: gray usm sharpen
- [ ] Cns: chrome noise filter
- [x] Saturation: chrome saturation
- [x] Yuv2Rgb: yuv domain to rgb domain

# 如何编译与运行

## Linux系统
**开发环境:** 
```bash
#dependencies install(cmake and opencv)
sudo apt update
sudo apt install libopencv-dev python3-opencv cmake
```

**编译**

```bash
git clone https://github.com/JokerEyeAdas/HDRImageSignalProcess
cd HDRImageSignalProcess/
mkdir build
cmake ..
make -j12
```
## Windows系统

**开发环境(x64):** 
- vs code
- cmake
- vs2019 c++ gen tool

![build tool](compile.png) 

**编译**

```bash
git clone https://github.com/JokerEyeAdas/HDRImageSignalProcess
code HDRImageSignalProcess
#cmake 选择 Debug or Release
#compiler 选择 xxx-amd64
#build all
```

## 运行

```bash
cd build
#cp cfgs and data
cp -r ../data/ ./
cp -r ../cfgs/ ./
#run isp
./HDR_ISP ./cfgs/isp_config_cannon.json
```


# 结果

## Pipeline运行结果

|备注|图像|
|-------|-------|
|Raw|![raw](ISP/connan_raw14.png)|
|ISP Result(Ours)|![ISP](ISP/isp_result.png)|
|FastOpenISP Result|![Fast ISP](ISP/color_checker.png)|

------

## 细节对比

|ISP|图像|备注|
|-------|-------|-------|
|Ours|![Our Detail](ISP/our_detail.png)|细节与边界明显保留|
|Fast Open Isp|![Open Isp](ISP/fast_detail.png)|细节丢失以及Color banding|
|Ours|![Our Detail](ISP/our_sharpen.png)|锐化效果、细节保留|
|Fast Open Isp|![Open Isp](ISP/others_sharpen.png)|细节丢失|

# 项目支持

- 感谢您的支持，希望我们的项目对您有帮助

![Appreciation Code](AppreciationCode.png)

# 参考仓库

感谢下述仓库以及仓库作者！！

|序号|仓库|开发语言|备注|
|-----|-----|-----|-----|
|1|[fast-openIsp](https://github.com/QiuJueqin/fast-openISP)|Python|open isp 快速版本|
|2|[ISP Lab](https://github.com/yuqing-liu-dut/ISPLab)|C++|C++版本开源ISP|
|3|[xk-ISP](https://github.com/openasic-org/xkISP)|C++|复旦大学开源HLS ISP|

-----

**ADAS之眼权利保留@2023**