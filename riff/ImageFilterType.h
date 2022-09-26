#pragma once

#include <RadeonImageFilters.h>

namespace riff
{

enum class ImageFilterType
{
    /*rif_image_filter_type*/
    Normalization      = RIF_IMAGE_FILTER_NORMALIZATION,
    GammaCorrection    = RIF_IMAGE_FILTER_GAMMA_CORRECTION,
    Resample           = RIF_IMAGE_FILTER_RESAMPLE,
    ResampleDynamic    = RIF_IMAGE_FILTER_RESAMPLE_DYNAMIC,
    RemapRange         = RIF_IMAGE_FILTER_REMAP_RANGE,

    //blurring
    GaussianBlur       = RIF_IMAGE_FILTER_GAUSSIAN_BLUR,
    MotionBlur         = RIF_IMAGE_FILTER_MOTION_BLUR,

    // tone mapping filters
    ColorSpace         = RIF_IMAGE_FILTER_COLOR_SPACE,
    HueSaturation      = RIF_IMAGE_FILTER_HUE_SATURATION,
    FilmicTonemap      = RIF_IMAGE_FILTER_FILMIC_TONEMAP,
    AcesTonemap        = RIF_IMAGE_FILTER_ACES_TONEMAP,
    Rainhard02Tonemap  = RIF_IMAGE_FILTER_REINHARD02_TONEMAP,
    ExponentialTonemap = RIF_IMAGE_FILTER_EXPONENTIAL_TONEMAP,
    LinearTonemap      = RIF_IMAGE_FILTER_LINEAR_TONEMAP,
    DragoTonemap       = RIF_IMAGE_FILTER_DRAGO_TONEMAP,
    AutolinearTonemap  = RIF_IMAGE_FILTER_AUTOLINEAR_TONEMAP,
    MaxwhiteTonemap    = RIF_IMAGE_FILTER_MAXWHITE_TONEMAP,
    PhotoLinearTonemap = RIF_IMAGE_FILTER_PHOTO_LINEAR_TONEMAP,
    PhotoTonemap       = RIF_IMAGE_FILTER_PHOTO_TONEMAP,
    FilmicUnchartedTonemap = RIF_IMAGE_FILTER_FILMIC_UNCHARTED_TONEMAP,

    //denoising filters
    BilateralDenoise   = RIF_IMAGE_FILTER_BILATERAL_DENOISE,
    LWR_Denoise        = RIF_IMAGE_FILTER_LWR_DENOISE,
    EawDenoise         = RIF_IMAGE_FILTER_EAW_DENOISE,
    MedianDenoise      = RIF_IMAGE_FILTER_MEDIAN_DENOISE,

    //machine learning based denoising filters
    AiDenoise          = RIF_IMAGE_FILTER_AI_DENOISE,
    AiUpscale          = RIF_IMAGE_FILTER_AI_UPSCALE,

    UserDefined        = RIF_IMAGE_FILTER_USER_DEFINED,
};

} // namespace riff
