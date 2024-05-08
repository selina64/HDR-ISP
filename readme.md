# Image Signal Process For HDR CMOS Image Sensor

**[CH](docs/readme_ch.md) | [EN](./readme.md)**


# Project Description

Forked from JokerEyeAdas/HDR-ISP. I fork this repository for learning use, so I'm trying to add comments on the code, from the view of a beginner learner of ISP.

**An ISP pipeline written in C++ for HDR cameras**

- The ISP modules are written in C style
- Without other library dependencies
- Can be deployed various embedded platforms
- Customize your pipeline through json configuration

# Default Pipeline

![Pipeline](docs/pipeline.png)
# Support ISP Module Lists

- Raw Domain
    - [x] MipiUnPack: mipi raw data unpack to raw16
        - 解析数据
        - Mipi（Mobile Industry Processor Interface）：是一种移动端数据传输接口，图像传输是其中的一部分，在手机等移动设备中常见。
    - [x] DePwl: decode the pwl curve
        - PWL（PieceWise Linear）：PWL是一种压缩/解压缩方式，提高存储密度和传输效率。
    - [x] Dpc: dynamic pixel correct
        - 坏点校正：去除异常坏点
    - [x] Lsc: lens shading correct
        - 暗角校正：去除四周的暗角
    - [x] Blc: black level correct
        - 黑阶校正：类似于抑制零飘
    - [ ] Rns: raw noise filter
        - 本项目未实现
    - [x] WbGain: white balance gain
        - 白平衡增益：若在6500K光照下，白色的图像拍摄为白色。
    - [x] Demoasic: change raw to rgb
        - 去马赛克：从每个像素只有一个通道（RGB其中之一），转换到每个像素都有RGB三通道的图像。
- RGB Domain
    - [x] Ltm: local tone mapping
        - 局部色调调整 ：这是图像处理中的一种技术，主要用于高动态范围(HDR)图像的处理，使其能够在低动态范围(LDR)的显示设备上正确地显示。
        - 举例：12bit的图像，希望在10bit显示器上显示。
        - 朴素算法：直接舍去低2位信息，这样导致全局细节均有丢失
        - GTM（全局色调调整）：通过设置一条全局的映射曲线，保证部分信息不丢失，但是也牺牲了其他部分。（例如牺牲亮部保留暗部）
        - LTM：对不同的位置做不同的映射，这样亮部暗部细节均可以最大程度的保留
    - [x] RgbGamma: rgb gamma curve
        - 一种亮度校正：把亮部的对比度降低，暗部对比度提高
    - [x] Ccm: color correct matrix
        - 颜色校正矩阵（Color Correction Matrix，简称CCM）的主要作用是进行颜色的校正和转换。CCM主要由一个3x3的矩阵组成，通常用于将原始图像数据（通常是RGB颜色空间）转换成另一个颜色空间。
        - 在摄像头ISP流程中，由于不同的镜头和传感器对颜色的响应不同，直接得到的图像可能颜色偏移，所以我们需要用CCM来进行颜色校正，以得到符合人眼视觉体验的颜色表现。
        - 在实际应用中，CCM的确定通常需要通过拍摄色卡（包含多种颜色的目标）来得到。
    - [x] Rgb2Yuv: rgb domain to yuv domain
        - 色彩空间转换
- YUV Domain
    - yuv：“Y”表示明亮度（Luminance、Luma），“U”和“V”则是两种色度（Chrominance、Chroma）
    - [x] YGamma: gray gamma curve
        - 和RGB GAMMA操作基本一样，但是仅针对y通道
    - [x] Contrast: gray contrast adjust
        - y通道以127为中心，上下缩放
    - [x] Sharpen: gray usm sharpen
        - usm: Unsharp Masking （应该理解为把不sharp的部分mask掉）
    - [x] Cns: chrome noise filter
    - [x] Saturation: chrome saturation
    - [x] Yuv2Rgb: yuv domain to rgb domain

# How To Build and Run

## Linux
**Develop Env :** 
```bash
#dependencies install(cmake and opencv)
sudo apt update
sudo apt install cmake
```

**Build**

```bash
git clone https://github.com/JokerEyeAdas/HDR-ISP
cd HDR-ISP/
mkdir build
cmake ..
make -j12
```
## Windows

**Develop Env (x64):** 
- vs code
- cmake
- vs2019 c++ gen tool

![build tool](docs/compile.png) 

**Build**

```bash
git clone https://github.com/JokerEyeAdas/HDR-ISP
code HDR-ISP
#cmake choose Debug or Release
#compiler choose xxx-amd64
#build all
```
## How To Run

```bash
cd build
#cp cfgs and data
cp -r ../data/ ./
cp -r ../cfgs/ ./
#run isp
./HDR_ISP ./cfgs/isp_config_cannon.json
```
## How To Tunning Params

**By changing Json Config**

example1, change sensor params:
```json
    "raw_file": "./data/connan_raw14.raw",
    "out_file_path": "./",
    "info": {
        "sensor_name": "cannon",
        "cfa": "RGGB",
        "data_type": "RAW16",
        "bpp": 16,
        "max_bit": 14,
        "width": 6080,
        "height": 4044,
        "mipi_packed": 0
    },
```
example2, change rgb gamma params:
```json
"rgbgamma": {
        "enable": true,
        "in_bit": 10,
        "out_bit": 8,
        "gammalut_nums": 11,
        "gammalut": [
            0,
            0.3504950718773984,
            0.48243595264750255,
            0.57750428843709,
            0.6596458942714417,
            0.731034378464739,
            0.7925580792857235,
            0.8509817015104557,
            0.9029435754464383,
            0.9534255851019492,
            1.0
        ]
    }
```

# Result

## ISP Result

|Notes|Image|
|-------|-------|
|Raw|![raw](/docs/ISP/connan_raw14.png)|
|ISP Result(Ours)|![ISP](docs/ISP/isp_result.png)|
|FastOpenISP Result|![Fast ISP](docs/ISP/color_checker.png)|

------

## Detail Compare

|ISP|Image1|Image2|Notes|
|-------|-------|-------|--------|
|Ours|![Our Detail](docs/ISP/our_detail.png)|![Our Detail](docs/ISP/our_sharpen.png)|Detail and boundaries is clear|
|Fast Open Isp|![Open Isp](docs/ISP/fast_detail.png)|![Open Isp](docs/ISP/others_sharpen.png)|Color banding and detail lost|



# Follow-Up

* Support Dpc, Rns, Cns and other ISP modules;
* Write ISP tunning GUI tools.

# Project Support

- Thanks for your support, we hope that my project can help your work

![Appreciation Code](docs/AppreciationCode.png)

# Reference Repo

Thanks for the following code repository!

|Index|Repo|Language|Notes|
|-----|-----|------|-----|
|0|[OpenISP](https://github.com/cruxopen/openISP)|Python|image signal process in C style|
|1|[fast-openIsp](https://github.com/QiuJueqin/fast-openISP)|Python|open isp speed up verison in python|
|2|[ISP Lab](https://github.com/yuqing-liu-dut/ISPLab)|C++|Isp realized by c++|
|3|[xk-ISP](https://github.com/openasic-org/xkISP)|C++|C++ ISP For HLS on FPGA|



# Follow Me

**Zhihu：[EYES OF ADAS](https://www.zhihu.com/people/fen-shi-qing-nian-29) | Blog：[EYES OF ADAS](https://jokereyeadas.github.io/) | WeChat：**

**![WeChat](docs/wechat.png)**



**Eyes of Adas Copyright Reserved @2023, No commercial use without permission**

-----