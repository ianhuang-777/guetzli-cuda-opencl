#pragma once
#include <vector>
#include "CL/cl.h"
#include "guetzli/processor.h"
#include "guetzli/butteraugli_comparator.h"
#include "ocl.h"
#include "clguetzli.cl.h"

extern bool g_useOpenCL;
extern bool g_checkOpenCL;

void clOpsinDynamicsImage(
    float *r, float *g, float *b, 
    const size_t xsize, const size_t ysize);

void clDiffmapOpsinDynamicsImage(
    float* result,
    const float* r,  const float* g,  const float* b,
    const float* r2, const float* g2, const float* b2,
    const size_t xsize, const size_t ysize,
    const size_t step);

void clComputeBlockZeroingOrder(
    guetzli::CoeffData *output_order_batch,
    const channel_info orig_channel[3],
    const float *orig_image_batch,
    const float *mask_scale,
    const int image_width,
    const int image_height,
    const channel_info mayout_channel[3],
    const int factor,
    const int comp_mask,
    const float BlockErrorLimit
    );

void clMask(
    float* mask_r,   float* mask_g,   float* mask_b,
    float* maskdc_r, float* maskdc_g, float* maskdc_b, 
    const size_t xsize, const size_t ysize,
    const float* r,  const float* g,  const float* b,
    const float* r2, const float* g2, const float* b2);

void clConvolutionEx(
    cl_mem result/*out*/,
    const cl_mem inp, size_t xsize, size_t ysize,
    const cl_mem multipliers, size_t len,
    int xstep, int offset, double border_ratio);

void clConvolutionX(
    cl_mem result/*out*/, 
    const cl_mem inp, size_t xsize, size_t ysize,
    const cl_mem multipliers, size_t len,
    int xstep, int offset, double border_ratio);

void clConvolutionY(
    cl_mem result/*out*/,
    const cl_mem inp, size_t xsize, size_t ysize,
    const cl_mem multipliers, size_t len,
    int xstep, int offset, double border_ratio);

void clUpsampleEx2(
    cl_mem result/*out*/,
    const cl_mem image, size_t xsize, size_t ysize,
    size_t xstep, size_t ystep);

void clUpsampleEx(
    cl_mem result/*out*/,
    const cl_mem image, 
    const size_t xsize, const size_t ysize,
    const size_t xstep, const size_t ystep);

void clBlurEx(cl_mem image/*out, opt*/, const size_t xsize, const size_t ysize,
    const double sigma, const double border_ratio,
    cl_mem result = nullptr/*out, opt*/);

void clBlurEx2(cl_mem image/*out, opt*/, size_t xsize, size_t ysize,
    double sigma, double border_ratio,
    cl_mem result = NULL/*out, opt*/);

void clOpsinDynamicsImageEx(ocl_channels &rgb, const size_t xsize, const size_t ysize);

void clMaskHighIntensityChangeEx(
    ocl_channels &xyb0/*in,out*/,
	ocl_channels &xyb1/*in,out*/,
	const size_t xsize, const size_t ysize);

void clEdgeDetectorMapEx(
    cl_mem result/*out*/,
    const ocl_channels &rgb, const ocl_channels &rgb2,
    const size_t xsize, const size_t ysize, const size_t step);

void clBlockDiffMapEx(
    cl_mem block_diff_dc/*out*/, 
    cl_mem block_diff_ac/*out*/,
    const ocl_channels &rgb, const ocl_channels &rgb2,
	const size_t xsize, const size_t ysize, const size_t step);

void clEdgeDetectorLowFreqEx(
    cl_mem block_diff_ac/*in,out*/,
    const ocl_channels &rgb, const ocl_channels &rgb2,
    const size_t xsize, const size_t ysize, const size_t step);

void clDiffPrecomputeEx(
    ocl_channels &mask/*out*/,
    const ocl_channels &xyb0, const ocl_channels &xyb1, 
    const size_t xsize, const size_t ysize);

void clScaleImageEx(cl_mem img/*in, out*/, size_t size, double w);

void clAverage5x5Ex(cl_mem img/*in,out*/, const size_t xsize, const size_t ysize);

void clMinSquareValEx(
    cl_mem img/*in,out*/, 
    const size_t xsize, const size_t ysize, 
    const size_t square_size, const size_t offset);

void clMaskEx(
    ocl_channels mask/*out*/, ocl_channels mask_dc/*out*/,
    const ocl_channels &rgb, const ocl_channels &rgb2,
	const size_t xsize, const size_t ysize);

void clCombineChannelsEx(
    cl_mem result/*out*/,
	const ocl_channels &mask,
	const ocl_channels &mask_dc,
    const size_t xsize, const size_t ysize,
	const cl_mem block_diff_dc,
	const cl_mem block_diff_ac,
	const cl_mem edge_detector_map,
	const size_t res_xsize,
	const size_t step);

void clUpsampleSquareRootEx(cl_mem diffmap, const size_t xsize, const size_t ysize, const int step);

void clCalculateDiffmapEx(cl_mem diffmap/*in,out*/, size_t xsize, size_t ysize, int step);

void clRemoveBorderEx(cl_mem out, const cl_mem in, const size_t xsize, const size_t ysize, const int step);

void clAddBorderEx(cl_mem out, const size_t xsize, const size_t ysize, const int step, const cl_mem in);

void clCalculateDiffmapEx(cl_mem diffmap/*in,out*/, const size_t xsize, const size_t ysize, const int step);

class guetzli::OutputImage;

namespace guetzli {

    class ButteraugliComparatorEx : public ButteraugliComparator
    {
    public:
        ButteraugliComparatorEx(const int width, const int height,
            const std::vector<uint8_t>* rgb,
            const float target_distance, ProcessStats* stats);

        void StartBlockComparisons() override;
        void FinishBlockComparisons() override;

        double CompareBlock(const OutputImage& img, int off_x, int off_y, const coeff_t* candidate_block, const int comp_mask) const override;
    public:
        std::vector<float> imgOpsinDynamicsBlockList;   // [RR..RRGG..GGBB..BB]:blockCount
        std::vector<float> imgMaskXyzScaleBlockList;    // [RGBRGB..RGBRGB]:blockCount
    };
}