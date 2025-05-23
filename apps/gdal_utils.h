/******************************************************************************
 *
 * Project:  GDAL Utilities
 * Purpose:  GDAL Utilities Public Declarations.
 * Author:   Faza Mahamood, fazamhd at gmail dot com
 *
 * ****************************************************************************
 * Copyright (c) 1998, Frank Warmerdam
 * Copyright (c) 2007-2015, Even Rouault <even.rouault at spatialys.com>
 * Copyright (c) 2015, Faza Mahamood
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#ifndef GDAL_UTILS_H_INCLUDED
#define GDAL_UTILS_H_INCLUDED

/**
 * \file gdal_utils.h
 *
 * Public (C callable) GDAL Utilities entry points.
 *
 * @since GDAL 2.1
 */

#include "cpl_port.h"
#include "gdal.h"

CPL_C_START

/*! Options for GDALInfo(). Opaque type */
typedef struct GDALInfoOptions GDALInfoOptions;

/** Opaque type */
typedef struct GDALInfoOptionsForBinary GDALInfoOptionsForBinary;

GDALInfoOptions CPL_DLL *
GDALInfoOptionsNew(char **papszArgv,
                   GDALInfoOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALInfoOptionsFree(GDALInfoOptions *psOptions);

char CPL_DLL *GDALInfo(GDALDatasetH hDataset, const GDALInfoOptions *psOptions);

/*! Options for GDALTranslate(). Opaque type */
typedef struct GDALTranslateOptions GDALTranslateOptions;

/** Opaque type */
typedef struct GDALTranslateOptionsForBinary GDALTranslateOptionsForBinary;

GDALTranslateOptions CPL_DLL *
GDALTranslateOptionsNew(char **papszArgv,
                        GDALTranslateOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALTranslateOptionsFree(GDALTranslateOptions *psOptions);

void CPL_DLL GDALTranslateOptionsSetProgress(GDALTranslateOptions *psOptions,
                                             GDALProgressFunc pfnProgress,
                                             void *pProgressData);

GDALDatasetH CPL_DLL GDALTranslate(const char *pszDestFilename,
                                   GDALDatasetH hSrcDataset,
                                   const GDALTranslateOptions *psOptions,
                                   int *pbUsageError);

/*! Options for GDALWarp(). Opaque type */
typedef struct GDALWarpAppOptions GDALWarpAppOptions;

/** Opaque type */
typedef struct GDALWarpAppOptionsForBinary GDALWarpAppOptionsForBinary;

GDALWarpAppOptions CPL_DLL *
GDALWarpAppOptionsNew(char **papszArgv,
                      GDALWarpAppOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALWarpAppOptionsFree(GDALWarpAppOptions *psOptions);

void CPL_DLL GDALWarpAppOptionsSetProgress(GDALWarpAppOptions *psOptions,
                                           GDALProgressFunc pfnProgress,
                                           void *pProgressData);
void CPL_DLL GDALWarpAppOptionsSetQuiet(GDALWarpAppOptions *psOptions,
                                        int bQuiet);
void CPL_DLL GDALWarpAppOptionsSetWarpOption(GDALWarpAppOptions *psOptions,
                                             const char *pszKey,
                                             const char *pszValue);

GDALDatasetH CPL_DLL GDALWarp(const char *pszDest, GDALDatasetH hDstDS,
                              int nSrcCount, GDALDatasetH *pahSrcDS,
                              const GDALWarpAppOptions *psOptions,
                              int *pbUsageError);

/*! Options for GDALVectorTranslate(). Opaque type */
typedef struct GDALVectorTranslateOptions GDALVectorTranslateOptions;

/** Opaque type */
typedef struct GDALVectorTranslateOptionsForBinary
    GDALVectorTranslateOptionsForBinary;

GDALVectorTranslateOptions CPL_DLL *GDALVectorTranslateOptionsNew(
    char **papszArgv, GDALVectorTranslateOptionsForBinary *psOptionsForBinary);

void CPL_DLL
GDALVectorTranslateOptionsFree(GDALVectorTranslateOptions *psOptions);

void CPL_DLL GDALVectorTranslateOptionsSetProgress(
    GDALVectorTranslateOptions *psOptions, GDALProgressFunc pfnProgress,
    void *pProgressData);

GDALDatasetH CPL_DLL GDALVectorTranslate(
    const char *pszDest, GDALDatasetH hDstDS, int nSrcCount,
    GDALDatasetH *pahSrcDS, const GDALVectorTranslateOptions *psOptions,
    int *pbUsageError);

/*! Options for GDALDEMProcessing(). Opaque type */
typedef struct GDALDEMProcessingOptions GDALDEMProcessingOptions;

/** Opaque type */
typedef struct GDALDEMProcessingOptionsForBinary
    GDALDEMProcessingOptionsForBinary;

GDALDEMProcessingOptions CPL_DLL *GDALDEMProcessingOptionsNew(
    char **papszArgv, GDALDEMProcessingOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALDEMProcessingOptionsFree(GDALDEMProcessingOptions *psOptions);

void CPL_DLL GDALDEMProcessingOptionsSetProgress(
    GDALDEMProcessingOptions *psOptions, GDALProgressFunc pfnProgress,
    void *pProgressData);

GDALDatasetH CPL_DLL
GDALDEMProcessing(const char *pszDestFilename, GDALDatasetH hSrcDataset,
                  const char *pszProcessing, const char *pszColorFilename,
                  const GDALDEMProcessingOptions *psOptions, int *pbUsageError);

/*! Options for GDALNearblack(). Opaque type */
typedef struct GDALNearblackOptions GDALNearblackOptions;

/** Opaque type */
typedef struct GDALNearblackOptionsForBinary GDALNearblackOptionsForBinary;

GDALNearblackOptions CPL_DLL *
GDALNearblackOptionsNew(char **papszArgv,
                        GDALNearblackOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALNearblackOptionsFree(GDALNearblackOptions *psOptions);

void CPL_DLL GDALNearblackOptionsSetProgress(GDALNearblackOptions *psOptions,
                                             GDALProgressFunc pfnProgress,
                                             void *pProgressData);

GDALDatasetH CPL_DLL GDALNearblack(const char *pszDest, GDALDatasetH hDstDS,
                                   GDALDatasetH hSrcDS,
                                   const GDALNearblackOptions *psOptions,
                                   int *pbUsageError);

/*! Options for GDALGrid(). Opaque type */
typedef struct GDALGridOptions GDALGridOptions;

/** Opaque type */
typedef struct GDALGridOptionsForBinary GDALGridOptionsForBinary;

GDALGridOptions CPL_DLL *
GDALGridOptionsNew(char **papszArgv,
                   GDALGridOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALGridOptionsFree(GDALGridOptions *psOptions);

void CPL_DLL GDALGridOptionsSetProgress(GDALGridOptions *psOptions,
                                        GDALProgressFunc pfnProgress,
                                        void *pProgressData);

GDALDatasetH CPL_DLL GDALGrid(const char *pszDest, GDALDatasetH hSrcDS,
                              const GDALGridOptions *psOptions,
                              int *pbUsageError);

/*! Options for GDALContour(). Opaque type */
typedef struct GDALContourOptions GDALContourOptions;

/** Opaque type */
typedef struct GDALContourOptionsForBinary GDALContourOptionsForBinary;

GDALContourOptions CPL_DLL *
GDALContourOptionsNew(char **papszArgv,
                      GDALContourOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALContourOptionsFree(GDALContourOptions *psOptions);

void CPL_DLL GDALContourOptionsSetProgress(GDALContourOptions *psOptions,
                                           GDALProgressFunc pfnProgress,
                                           void *pProgressData);

///@cond Doxygen_Suppress
// Cannot be in gdal_utils_priv.h because it's used in the bindings (that
// define CPL_SUPRESS_CPLUSPLUS making it impossible to import this header
// because it uses CPLStringList which is a C++ class and it's not included
// if CPL_SUPRESS_CPLUSPLUS is on).
void CPL_DLL GDALContourOptionsSetDestDataSource(GDALContourOptions *psOptions,
                                                 const char *pszDestDatasource);

// Finally got the third star! https://wiki.c2.com/?ThreeStarProgrammer
CPLErr CPL_DLL GDALContourProcessOptions(GDALContourOptions *psOptions,
                                         char ***ppapszStringOptions,
                                         GDALDatasetH *hSrcDS,
                                         GDALRasterBandH *hBand,
                                         GDALDatasetH *hDstDS,
                                         OGRLayerH *hLayer);

///@endcond

/*! Options for GDALRasterize(). Opaque type */
typedef struct GDALRasterizeOptions GDALRasterizeOptions;

/** Opaque type */
typedef struct GDALRasterizeOptionsForBinary GDALRasterizeOptionsForBinary;

GDALRasterizeOptions CPL_DLL *
GDALRasterizeOptionsNew(char **papszArgv,
                        GDALRasterizeOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALRasterizeOptionsFree(GDALRasterizeOptions *psOptions);

void CPL_DLL GDALRasterizeOptionsSetProgress(GDALRasterizeOptions *psOptions,
                                             GDALProgressFunc pfnProgress,
                                             void *pProgressData);

GDALDatasetH CPL_DLL GDALRasterize(const char *pszDest, GDALDatasetH hDstDS,
                                   GDALDatasetH hSrcDS,
                                   const GDALRasterizeOptions *psOptions,
                                   int *pbUsageError);

/*! Options for GDALFootprint(). Opaque type */
typedef struct GDALFootprintOptions GDALFootprintOptions;

/** Opaque type */
typedef struct GDALFootprintOptionsForBinary GDALFootprintOptionsForBinary;

GDALFootprintOptions CPL_DLL *
GDALFootprintOptionsNew(char **papszArgv,
                        GDALFootprintOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALFootprintOptionsFree(GDALFootprintOptions *psOptions);

void CPL_DLL GDALFootprintOptionsSetProgress(GDALFootprintOptions *psOptions,
                                             GDALProgressFunc pfnProgress,
                                             void *pProgressData);

GDALDatasetH CPL_DLL GDALFootprint(const char *pszDest, GDALDatasetH hDstDS,
                                   GDALDatasetH hSrcDS,
                                   const GDALFootprintOptions *psOptions,
                                   int *pbUsageError);

/*! Options for GDALBuildVRT(). Opaque type */
typedef struct GDALBuildVRTOptions GDALBuildVRTOptions;

/** Opaque type */
typedef struct GDALBuildVRTOptionsForBinary GDALBuildVRTOptionsForBinary;

GDALBuildVRTOptions CPL_DLL *
GDALBuildVRTOptionsNew(char **papszArgv,
                       GDALBuildVRTOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALBuildVRTOptionsFree(GDALBuildVRTOptions *psOptions);

void CPL_DLL GDALBuildVRTOptionsSetProgress(GDALBuildVRTOptions *psOptions,
                                            GDALProgressFunc pfnProgress,
                                            void *pProgressData);

GDALDatasetH CPL_DLL GDALBuildVRT(const char *pszDest, int nSrcCount,
                                  GDALDatasetH *pahSrcDS,
                                  const char *const *papszSrcDSNames,
                                  const GDALBuildVRTOptions *psOptions,
                                  int *pbUsageError);

/*! Options for GDALMultiDimInfo(). Opaque type */
typedef struct GDALMultiDimInfoOptions GDALMultiDimInfoOptions;

/** Opaque type */
typedef struct GDALMultiDimInfoOptionsForBinary
    GDALMultiDimInfoOptionsForBinary;

GDALMultiDimInfoOptions CPL_DLL *GDALMultiDimInfoOptionsNew(
    char **papszArgv, GDALMultiDimInfoOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALMultiDimInfoOptionsFree(GDALMultiDimInfoOptions *psOptions);

char CPL_DLL *GDALMultiDimInfo(GDALDatasetH hDataset,
                               const GDALMultiDimInfoOptions *psOptions);

/*! Options for GDALMultiDimTranslate(). Opaque type */
typedef struct GDALMultiDimTranslateOptions GDALMultiDimTranslateOptions;

/** Opaque type */
typedef struct GDALMultiDimTranslateOptionsForBinary
    GDALMultiDimTranslateOptionsForBinary;

GDALMultiDimTranslateOptions CPL_DLL *GDALMultiDimTranslateOptionsNew(
    char **papszArgv,
    GDALMultiDimTranslateOptionsForBinary *psOptionsForBinary);

void CPL_DLL
GDALMultiDimTranslateOptionsFree(GDALMultiDimTranslateOptions *psOptions);

void CPL_DLL GDALMultiDimTranslateOptionsSetProgress(
    GDALMultiDimTranslateOptions *psOptions, GDALProgressFunc pfnProgress,
    void *pProgressData);

GDALDatasetH CPL_DLL GDALMultiDimTranslate(
    const char *pszDest, GDALDatasetH hDstDataset, int nSrcCount,
    GDALDatasetH *pahSrcDS, const GDALMultiDimTranslateOptions *psOptions,
    int *pbUsageError);

/*! Options for GDALVectorInfo(). Opaque type */
typedef struct GDALVectorInfoOptions GDALVectorInfoOptions;

/** Opaque type */
typedef struct GDALVectorInfoOptionsForBinary GDALVectorInfoOptionsForBinary;

GDALVectorInfoOptions CPL_DLL *
GDALVectorInfoOptionsNew(char **papszArgv,
                         GDALVectorInfoOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALVectorInfoOptionsFree(GDALVectorInfoOptions *psOptions);

char CPL_DLL *GDALVectorInfo(GDALDatasetH hDataset,
                             const GDALVectorInfoOptions *psOptions);

/*! Options for GDALTileIndex(). Opaque type */
typedef struct GDALTileIndexOptions GDALTileIndexOptions;

/** Opaque type */
typedef struct GDALTileIndexOptionsForBinary GDALTileIndexOptionsForBinary;

GDALTileIndexOptions CPL_DLL *
GDALTileIndexOptionsNew(char **papszArgv,
                        GDALTileIndexOptionsForBinary *psOptionsForBinary);

void CPL_DLL GDALTileIndexOptionsSetProgress(GDALTileIndexOptions *psOptions,
                                             GDALProgressFunc pfnProgress,
                                             void *pProgressData);

void CPL_DLL GDALTileIndexOptionsFree(GDALTileIndexOptions *psOptions);

GDALDatasetH CPL_DLL GDALTileIndex(const char *pszDest, int nSrcCount,
                                   const char *const *papszSrcDSNames,
                                   const GDALTileIndexOptions *psOptions,
                                   int *pbUsageError);

CPL_C_END

#endif /* GDAL_UTILS_H_INCLUDED */
