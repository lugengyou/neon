#include <iostream>
#include <arm_neon.h>

#define WIDTH 300
#define HEIGHT 300
#define CHANNEL 3
#define SIZE WIDTH * HEIGHT

using namespace std;

/*
 * 描述：将输入的图像数据进行归一化并由 HWC 转为 CHW 格式
 */
void NeonNorm2CHW(float *image, float *output, float *mean, float *std) {
    // HWC image data BGR format : BGRBGR...
    // float image[CHANNEL * SIZE] = {0,}; 
    float *din = image;

    // output data
    // float output[CHANNEL * SIZE] = {0,};
    float *dout0 = output;
    float *dout1 = output + SIZE;
    float *dout2 = output + SIZE * 2;

    // mean value 
    // float mean[3] = {0.485, 0.456, 0.406};
    // std value
    // float std[3] = {0.229, 0.224, 0.225};

    // neon vector mean
    float32x4_t vmean0 = vdupq_n_f32(mean[0]);
    float32x4_t vmean1 = vdupq_n_f32(mean[1]);
    float32x4_t vmean2 = vdupq_n_f32(mean[2]);

    // neon vector std
    float32x4_t vstd0 = vdupq_n_f32(std[0]);
    float32x4_t vstd1 = vdupq_n_f32(std[1]);
    float32x4_t vstd2 = vdupq_n_f32(std[2]);

    // for loop
    int loop = SIZE / 4;
    int i = 0;
    for ( ; i < loop; ++i) {
        // load 4 pixel data 3 channel
        float32x4x3_t vin = vld3q_f32(din);

        // sub mean
        float32x4_t vout0 = vsubq_f32(vin.val[0], vmean0);
        float32x4_t vout1 = vsubq_f32(vin.val[1], vmean1);
        float32x4_t vout2 = vsubq_f32(vin.val[2], vmean2);

        // div std
        float32x4_t vs0 = vmulq_f32(vout0, vstd0);
        float32x4_t vs1 = vmulq_f32(vout1, vstd1);
        float32x4_t vs2 = vmulq_f32(vout2, vstd2);

        // store: nhwc -> nchw
        vst1q_f32(dout0, vs0);
        vst1q_f32(dout1, vs1);
        vst1q_f32(dout2, vs2);

        // memory offset
        din += 4 * 3;
        dout0 += 4;
        dout1 += 4;
        dout2 += 4;
    }

    // 计算剩余未处理的数据
    for ( ; i < SIZE; ++i) {
        (*dout0++) = ((*din++) - mean[0]) * std[0];
        (*dout1++) = ((*din++) - mean[1]) * std[1];
        (*dout2++) = ((*din++) - mean[2]) * std[2];        
    }

}


int main(void) {

   

    return 0;
}
