## GDAL/OGR 1.11.0 - General Changes

Build(Unix):
 * add Unix configure support for SOSI
 * remove pointers to old ver of ingres library files
 * add --with-libjson-c configure option to build against external libjson-c (>= 0.11) (#4676)
 * compilation fixes for iOS (#5197, #5198)
 * update to autoconf 2.69
 * add pkg-config gdal.pc (#3470)
 * configure for FileGDB: add explicit linking to libfgdbunixrtl (requires FileGDB SDK >= 1.2) (#5215); also try .dylib extension (#5221)
 * fix so that Java installs are found on the MAC to enable the MDB driver (#5267)
 * fix compilation with recent MySQL versions (5.6 for example) (#5284)
 * support --with-jp2mrsid with standalone Kakadu with MRSID v8 or later
 * Fix parallel build in Python bindings (#5346)
 * PCIDSK: don't link against libjpeg if configured --without-jpeg
 * Update configure script to pick up ECW JP2 SDK 5.1 (#5390)
 * add a 'make install' target for the Java bindings  (#5424)
 * add Vagrant configuration

Build(Windows):
 * add option to generate VC project for x64 on makegdal_gen.bat
 * nmake.opt: add WITH_PDB=1 option to optionally generate .pdb file on Release builds (#5420)
 * add support for building the OGR SOSI driver as a plugin (#3638)
 * add support for building the HDF4 driver as plugin (#5294)
 * add support for MrSID v9
 * Remove makegdalXX.bat generated files

## GDAL 1.11.0 - Overview of Changes

Port:
 * vsisubfile: fix Eof() behavior to be POSIX compliant, so that the shapefile reader can read the last feature when using /vsitar (#5093)
 * vsicache: fix for 32bit binaries when file size is over 2GB (#5170)
 * vsicache: add optional nChunkSize and nCacheSize parameters to VSICreateCachedFile()
 * vsicurl: add CPL_VSIL_CURL_USE_HEAD config option to disable use of CURL HEAD for other services like mapbox (likely lame python http implementations)
 * vsitar: avoid infinite loop in case of invalid .tar structure
 * vsizip: fix path separator in CPLFormFilename
 * vsizip: allow additional extensions listed in CPL_VSIL_ZIP_ALLOWED_EXTENSIONS config option.
 * vsizip: improve UTF-8 support of filenames inside ZIP file (#5361)
 * vsizip: fix ZIP64 support
 * vsigzip: reset EOF flag when doing a Seek() to be POSIX compliant
 * curl: add .netrc support
 * Windows CPLGetSymbol(): avoid dialog boxes to pop up when a DLL or one of its dependencies does not exist (#5211)
 * Add CPLOPrintf() and CPLOvPrintf() functions for easy CPLString formatting
 * CPLBase64DecodeInPlace() : fix to be robust to malformed base64 strings
 * CPLQuadTree: add CPLQuadTreeInsertWithBounds() where the pfnGetBounds is not needed.
 * CPLQuadTree: fix potential infinite recursion when inserting several points with identical coordinates in the mode with limited bucket size
 * Protect concurrent calls to setlocale() by a mutex (#5366)

Core:
 * RFC 45: GDAL datasets and raster bands as virtual memory mapping
 * GDALRasterBand::GetHistogram(): ignore nodata values (#4750, #5289)
 * allow auto loading of drivers to be disabled via config option
 * PAM .aux.xml and VRT: serialize Z component of a GCP as 'Z' attribute,
   for consistency, instead of GCPZ that could not be read back previously.
   In reading code, try reading 'Z' and if not found try 'GCPZ' (#5326)
 * JPEG2000: Add GDALGeorefPamDataset and GDALJP2AbstractDataset classes and use
             them in JP2KAK, JP2ECW, JP2OpenJPEG, JPEG2000 and MrSID drivers so that PAM
             georeferencing consistently overrides internal georeferencing
 * GDALDataset::IRasterIO(): don't use BlockBasedRasterIO() when INTERLEAVE=PIXEL if the request band count is just 1
 * CopyWholeRaster(): make default GDAL_SWATH_SIZE to 1/4 of GDAL_CACHEMAX instead of hard-coded value of 10 MB
 * don't report empty RAT on GDALGetDefaultRAT() (#5232)
 * modify GDALGCPsToGeotransform() to do the regression in normalized coordinates to make the math more stable.
 * expose new GDALComposeGeoTransforms() function.
 * GDALDefaultOverviews::HaveMaskFile(): avoid fetching .ovr file
 * JPEG2000: Fix reading georeferencing from some JPEG2000 files with duplicated GeoTIFF JP2Box (#5249)
 * Cleanup raster block mutex (#5296)
 * Driver registration: move JPEG2000 (Jasper based) after MrSID JPEG2000 support

Algorithms:
 * warper: fix regression with lanczos resampling when yradius > xradius (#5058)
 * warper: Make GDALCreateGenImgProjTransformer2() and GDALCreateGenImgProjTransformer3() fail when the creation of the reprojection transformer fails
 * warper: Fix warping when input pixel size is too close to 0 (#5190)
 * warper: revise formula of cubic resampling kernel, and a few optimizations (#5209)
 * warper: added DST_METHOD and support for GCP and TPS dest
 * warper: add support for DST_METHOD=RPC
 * warper: fix mode and near resampling corner computation (#5311)
 * warper: GDALGenImgProjTransform(): don't set panSuccess[i] to 1 in the middle of the function, if an intermediate transform before has set the flag to 0
 * warper: fix cutline blending (#5343)
 * warper: Average/mode kernels: make them less sensitive to numerical precision issues (#5350)
 * warper: Average/mode kernels: avoid 'holes' when the source coordinates are in a reversed order from the target coordinates (#5433)
 * warper: provide prototypes and work around strict compiler requirements on some opencl platforms (#5400)
 * RPC: fix for computation of adfGTFromLL (#5395)
 * TPS: optimization for GCC x86_64 that make computation about twice faster with huge number of GCPs
 * TPS: when using Armadillo to solve the coefficients, use solve(A,B) instead of inv(A)xB to faster resolution
 * TPS: compute direct and inverse transformations in parallel when warping option NUM_THREADS or GDAL_NUM_THREADS config. options are set to > 1
 * Geoloc: fix wrong bilinear interpolation in GDALGeoLocTransform() (#5305)
 * Geoloc: fail transformation of coordinates that is located on a nodata place of the geoloc array
 * rasterize: preliminary support for MERGE_ALG=ADD for heatmaps
 * gdal_grid: Add AVX optimized version of GDALGridInverseDistanceToAPower2NoSmoothingNoSearch
 * fill_nodata: GDALFillNodata(): Fix use of uninitialized memory and integer overflows (#4010, #5203)
 * rpc: Fix out-of-bounds read in RPC dem cubic interpolation

Utilities:
 * gdalinfo: add -listmdd and -mdd all options (#5275)
 * gdal_translate: add a -exponent option to be used with -scale
 * gdal_translate: fix output file naming scheme in gdal_translate -sds (#5119)
 * gdal_translate: fix logic in detection non-gray color table level (#5245)
 * gdal_translate: add a -norat option
 * gdal_translate: don't add 0.1 when -scale is used with a dstmin equal to dstmax (useful to generate a raster with uniform color, i.e. scaleRatio = 0)
 * gdal_translate: use floor() to compute image coordinates from world coordinates when specifying -projwin (useful when extracting from left or top of upper-left corner, which generate negative image coordinates) (#5367)
 * gdaltindex: remove annoying warning 'Warning 1: Field location of width 255 truncated to 254' (#5121)
 * gdaltindex: add -src_srs_name and -src_srs_format to go with MapServer RFC100; add also a -f and -lyr_name options to be able to create a non-shapefile tileindex
 * gdalwarp: Fix segfault where metadata values were not being nullchecked properly during conflict resolution (#5069)
 * gdalwarp: honor -s_srs when using cutline (#5081)
 * gdalwarp: copy nodata values from source to dest if -dstnodata is not given ; add option to not set dest nodata with -dstnodata None (#5087)
 * gdalwarp: do not return a non-zero exit status for warnings
 * gdalwarp: prevent from copying statistics metadata (#5319)
 * gdal_rasterize: set the progress bar to 100% even when there's nothing to do
 * gdal_grid: add support for different types of geometries (#5341)
 * gdal_grid: add  -z_increase and -z_multiply options
 * gdaldem: check that value of -z, -s, -az and -alt is numeric
 * gdalbuildvrt: validate values of -srcnodata and -vrtnodata arguments
 * gdal2tiles.py: Corrected OpenLayers code to reflect fix to geodetic resolution factor
 * gdal2tiles.py: add --tmscompatible flag so as to produce 2 tiles at zoom level 0 in geodetic profile
 * rgb2pct.py: Use python tempfile logic to avoid permissions issues with cwd (#5079)
 * gdal_edit.py: add a -ro option for drivers refusing to use the dataset in update-mode.
 * gdal_calc.py: add --allBands options (#5388)
 * Add vsipreload.cpp that can be compiled as a shared library that can be LD_PRELOAD'ed as an overload of libc to enable VSI Virtual FILE API to be used with binaries using regular libc for I/O
 * Add the wcs_virtds_params.py sample utility to be able to set the MapServer WCS virtual dataset parameters from a tileindex with rasters of mixed SRS (linked to MapServer RFC100)
 * gdalcompare.py: move to scripts
 * gdalcompare.py: ensure image dimensions match
 * gdal_ls.py: Fix issue with UTF-8 characters

Multi driver changes:
 * JPEG2000 drivers: take into account PixelIsPoint in GeoJP2 boxes, and expose AREA_OR_POINT=Point (#5437)
 * JP2KAK, JP2ECW, JP2OpenJPEG, JPEG2000 CreateCopy(): take into account AREA_OR_POINT=Point if present to write GeoJP2 box (#5437)

AAIGRID:
 * revert DECIMAL_PRECISION and add SIGNIFICANT_DIGITS to CreateCopy() (#3732)

AIGRID:
 * Turn off errors that can be triggered if the info has no VAT table related with this coverage (#3031)

BAG driver:
 * Recognise falseNorthing=10000000 as UTM South (#5152)

DIMAP driver:
 * fix memleak in error-code path

DTED driver:
 * Speed optimization to be more friendly with CPU cache in GDAL_DTED_SINGLE_BLOCK=YES mode

ECW driver:
 * fix crash in GDALDeregister_ECW() with ECW SDK 5 called from GDALDestroy() (#5214)
 * fix issue with ECW_CLEVER optimization when nPixelSpace != sizeof eBufDataType (#5262)

Envisat driver:
 * implement more reliable way of extracting GCPs from Meris tie-points (#5423)
 * add DEM corrections of TP-ADS products when present (#5423)
 * workaround dateline discontinuity in GCPs so they can be used with GDAL warping transformers (#5423)

ERS driver:
 * fix wrong interpretation of RegistrationCellX/RegistrationCellY (#2612, #3056, #5075)

GeoRaster driver:
 * fix RPC support (#4038)
 * fix read error when reading from pyramids (#5076)
 * make regular table and secure file a default for RDT (#5127)
 * fix error when reading NBIT pyramid levels (#5199)
 * show the VAT as RAT (#5200)
 * fix reading and writing of statistics metadata (#5237)
 * add generate pyramid create options (#5288)
 * fix incorrect geotransform interpretation when there is no SRS (#5323)

GRASS driver:
 * fix compilation issues for GRASS 7

GRIB driver:
 * display temperature unit as deg Celsius in metadata (#3606)

GTiff driver:
 * when compiling against internal libtiff, in read-only mode, optimization to
   avoid fetching the whole Strip/TileCounts and Strip/TileOffsets arrays
 * add validation of source overview characteristics with COPY_SRC_OVERVIEWS (#5059)
 * convert invalid TIFFTAG_RESOLUTIONUNIT=0 to 1(Unknown) (#5069)
 * fix potential issues in gt_citation.cpp / CheckUTM()
 * upgrade internal libtiff to latest CVS
 * implement reading and writing of ICC profiles (#5246)
 * make SetColorInterpretation() round-trip with GetColorInterpretation();
   read color interpretation from PAM if it exists (overrides internal tiff color interpretation);
   set TIFFTAG_PHOTOMETRIC=PHOTOMETRIC_RGB if calling SetColorInterpretation() with R,G,B and no explicit PHOTOMETRIC creation option defined
 * gt_wkt_srs.cpp: fix compilation with external libgeotiff. The file is dependent of quite a few CPL stuff, don't try to pretend otherwise
 * implement GetVirtualMemAuto() for some formulations of TIFF files (RFC 45)
 * fix reading a single-strip TIFF file where the single strip is bigger than 2GB (32bit builds only) (#5403)
 * look for .tab file before .wld/.tfw

GTX driver:
 * Add nodata support (#4660)

HDF4 driver:
 * Skip "SceneLineNumber" table if present in the list of geolocation fields of
   ASTER L1A dataset.

HDF5 driver:
 * add support for ODIM H5 georeferencing method (#5032)
 * set SRS GEOGCS in all cases (reverts r25801 and closes #4160)
 * support HDF5 NATIVE_SCHAR type, subdatsets without PAM (#5088)
 * release all opened handles so the file is closed at dataset closing (#5103)
 * better deal with dimensions of CSK-L1A HDF5 subdatasets (#4227)
 * avoid segmentation fault when H5Sget_simple_extent_ndims() returns negative value (#5291)

HFA driver:
 * add minimally tested support for u2 and u4 data in basedata
 * use direct binning for thematic layers and real instead of integer for values (#5066)
 * add a HFA_COMPRESS_OVR config option to select whether to create compressed overviews (#4866)
 * fix rewriting of statistics in existing HFA file where base data value is 8-bit (#5175)
 * implement re-writing existing histogram in HFA file, after raster editing (#5176)
 * avoid segfaults when creating a Imagine dataset with an invalid WKT (#5258)
 * expose color columns in RAT as Integer with values in range [0-255] instead of Real with values [0-1] (#5362)
 * report histogram column as GFU_PixelCount instead of GFU_Generic (#5359)
 * ensure histogram column written as float for HFA when using RAT API (#5382)

Idrisi driver:
 * Improve coordinate system handling and min/max statistics (#4980)

IRIS driver:
 * add height information on bands; rename dataset metadata item CAPPI_HEIGHT --> CAPPI_BOTTOM_HEIGHT (#5104)
 * IRIS: add support for two bytes data (#5431)

JP2ECW driver:
 * fix problem with JP2 write with SDK v5
 * fix issue with ECW_CLEVER optimization when nPixelSpace != sizeof eBufDataType (#5262)
 * avoid writing dummy GeoJP2 box when source dataset has no georeferencing (#5306)

JP2KAK driver:
 * preliminary support for Kakadu V7.x
 * fix creation of unsigned int16 with reversible compression (#4050)
 * on Windows, use VSI cache for I/O by default, instead Kakadu own I/O layer
 * remove extension from 12bit to 16bit (#5328)

JP2OpenJPEG driver:
 * avoid 'Empty SOT marker detected: Psot=12.' warning to be repeated several times
 * add support for encoding GCPs in a GeoJP2 box (#5279)
 * avoid writing dummy GeoJP2 box when source dataset has no georeferencing (#5306)

JPEG driver:
 * add autodetection of bitmasks that are msb ordered (#5102)
 * avoid memory leak when GDALOpen'ing() a JPEG through a http:// URL, and make it possible to access its overviews
 * return YCbCrK raw data for YCbCrK JPEG in GDAL_JPEG_TO_RGB = NO mode (instead of CMYK as before) (#5097)
 * implement reading and writing of ICC profiles (#5246)
 * internal libjpeg: apply patch for CVE-2013-6629
 * allow fallback to PAM to read GCPs
 * give priority to PAM GeoTransform if it exists and other source of geotransform (.wld, .tab) also exists (#5352)

KMLSuperOverlay driver:
 * recognize an alternate structure for raster KMZ file made of a single doc.kml
    and tiles whose name pattern is kml_image_L{level}_{j}_{i}.{png|jpg}
 * fix horrible speed performance in Open() (#5094)
 * fix crash at dataset closing and inability to read some big PNG tiles (#5154)
 * fix to generate files validating against OGC KML 2.2 schema
 * put Style into conformity with ATC 7
 * remove Region in root KML (ATC 41)
 * add NAME and DESCRIPTION creation options; read them back as metadata
 * add ALTITUDE and ALTITUDEMODE creation options
 * directly write into .kmz file (instead of in temporary location)
 * correctly write directories entry in .kmz file
 * add progress callback

L1B driver:
 * report correct values for GCP (#2403)
 * report more GCPS than before
 * implement geolocation array
 * add fetching of record metadata in .csv file
 * add subdatasets with solar zenith angles, cloud coverage
 * recognize NOAA-9/14 datasets whose dataset name in TBM header is encoded in EBCDIC and not in ASCII (#2848)
 * support opening a few NOAA <= 9 datasets that have no dataset name in the TBM header

LCP driver:
 * better handling of projections (#3255)
 * add CreateCopy() (#5172)

MBTiles driver:
 * add write support
 * avoid failure when there's no tile at the center of the maximum zoom level (#5278)
 * add capability to open /vsicurl/https:// signed AWS S3 URLs

MEM driver:
 * Create(): use calloc() instead of malloc()+memset() for faster creation of huge in-memory datasets

NetCDF driver:
 * fix to read netcdf-4 files with UBYTE data (#5053)
 * fix reading large netcdf-4 files with chunking and DEFLATE compression
 * fix netcdf chunking when creating file with > 2 dims ; add CHUNKING creation option (#5082 )
 * fix duplicate nodata metadata when using CreateCopy() (#5084)
 * fix copying large metadata in netcdf driver (#5113)
 * fix netcdf geotransform detection (#5114)
 * fix netcdf driver irregular grids management (#5118 and #4513)
 * only call nc_close on a valid netcdf id when closing dataset
 * try and identify .grd (and .nc3) files in netcdf-4 format (#5291), so they are identified before the hdf5 driver

NITF driver:
 * fix to support reading horizontal and/or vertical mono-block uncompressed images, even when the number of columns is <= 8192 (#3263)
 * update NITF Series list with new entries from MIL-STD-2411_1_CHG-3.pdf (#5353)
 * allow JP2KAK to be used as the JPEG2000 compression engine in the CreateCopy() case (#5386)

PDF driver:
 * Avoid reporting a Poppler error as a GDAL error on some newer USGS GeoPDF files (#5201)
 * PDF writing: automatically adjust DPI in case the page dimension exceeds the 14400 maximum value (in user units) allowed by Acrobat (#5412)

PDS driver:
 * Parse correctly MISSING_CONSTANT = 16#FF7FFFFB# as a IEEE754 single precision float expressed in hexadecimal; add support for ENCODING_TYPE = ZIP (data file compressed in a ZIP); recognize IMAGE_MAP_PROJECTION as an object included in UNCOMPRESSED_FILE object (#3939)

PNG driver:
 * Implement reading and writing of ICC profiles (#5246)

PostgisRaster driver:
 * Speed-up dataset opening (#5046).
 * Multi-tile multi-band caching added.
 * Smarter use of the information advertized in raster_columns view.
 * Avoid full table scan in situations without PKID/GIST indices.
 * Use of quadtree.

Rasdaman driver:
 * caching of tiles for datasets with more than one band (#5298)
 * connections are now kept for a whole session (#5298)
 * fixing connection-string regex (#5298)
 * fixing possible memory leaks (#5298)

Rasterlite driver:
 * fix resolution check typo in rasterlite driver

Raw drivers:
 * implement GetVirtualMemAuto() (RFC 45)
 * IRasterIO(): add special behavior to avoid going to block based IO when the dataset has INTERLEAVE=PIXEL and is eligible to direct I/O access pattern
 * allow direct I/O access even if a small proportion of scanlines are loaded (improve QGIS use case where the overview display will load sparse scanlines, which would prevent direct I/O at full resolution afterwards)
 * fix optimized RasterIO() when doing sub-sampling with non standard buffer pixel offset (#5438)

RMF driver:
 * fix decompression of 24-bit RMF DEM (#5268)

RPFTOC driver:
 * fix potential crash on some datasets when selecting the color palette (#5345)

SAGA driver:
 * add read/write support for .prj files (#5316)

SRP driver:
 * read TRANSH01.THF file to establish subdatasets (#5297)

VRT driver:
 * Implement non-linear scaling with a power function (addition of Exponent, SrcMin, SrcMax, DstMin, DstMax sub-elements in <ComplexSource>)
 * Preserve 64bit integer image offsets (#5086)
 * Make sure that VRTSourcedRasterBand::AddMaskBandSource() takes into account specified window (#5120)
 * Make GDALAutoCreateWarpedVRT() return NULL when GDALSuggestedWarpOutput() fails
 * VRTDataset::IRasterIO(): use source DatasetRasterIO even if band count is 1
 * VRTWarped: avoid setting up relative paths for things that aren't file-like
 * make relativeToVRT=1 work with NITF_IM:, NETCDF:, HDF5:, RASTERLITE:

WCS driver:
 * ensure C locale is enforced before parsing floating point values

WMS driver:
 * accept 'WMS:http://server/?SRS=EPSG:XXXX' syntax to select the preferred SRS in which to fetch layers
 * CPLHTTPFetchMulti(): avoid doing a timeout-only select when there are no file descriptor to wait on (can happen when doing a file:// URL)
 * allow cache location to be specified with GDAL_DEFAULT_WMS_CACHE_PATH configuration option if not provided in the XML (#4540)
 * Update to be able to understand slight changes in formatting of JSon output of ArcGIS mapserver protocol

XYZ driver:
 * accept datasets that have missing values at beginning and/or end of lines, such as MNT250_L93_FRANCE.XYZ
 * fix detection when there are only integral values with comma field separator
 * reopen with 'rb' flags for Windows happyness

## OGR 1.11.0 - Overview of Changes

Core:
 * GEOS support: require GEOS >= 3.1.0 and use the _r API of GEOS to avoid issues with the global GEOS error handlers
 * exportToWkb(): ISO WKB generation with wkbVariant option (#5330)
 * geocoding: when getting several answers from server for a query, report geometries on second, third, etc.. feature, and not only first one (#5057)
 * allow auto loading of drivers to be disabled via config option
 * remove obsolete OGRGeometryFactory::getGEOSGeometryFactory()
 * OGRGeometryFactory::organizePolygons() in DEFAULT method: fix a case with 2 outer rings that are touching by the first point of the smallest one
 * OGRGeometryFactory::organizePolygons(): optimization in ONLY_CCW case
 * OGRGeometryFactory::organizePolygons(): Add an experimental mode : CCW_INNER_JUST_AFTER_CW_OUTER
 * OGRLineString::segmentize() : do not set 0 as z for interpolated points, but the z from the previous point
 * OGRLineString::setNumPoints(): add an optional argument to avoid zeroing the arrays
 * Add OGRLineString::setZ()
 * Add OGRLineString::Project() and OGRLineString::getSubline()
 * OGRPolygon: add stealExteriorRing() and stealInteriorRing(int iRing)
 * OGRLinearRing::isClockwise(): optimizations and make it work in a degenerated case when a vertex is used several times in the vertex list (#5342)
 * OGRLinearRing::isPointOnRingBoundary() : optimizations and take into account bTestEnvelope
 * Add OGR_G_SetPointCount and OGR_G_SetPoints functions to API C (#5357)
 * OGREnvelope3D::Contains(): fix incorrect test
 * Layer algebra: fix handling of method field mapping to output fields when output fields are precreated (#5089)
 * Layer algebra: when an error condition is skipped, call CPLErrorReset() (#5269)
 * OGRLayer::GetFeature(): make sure that the behavior is not influenced by
   attribute or spatial filters in the generic implementation;
   upgrade OGDI, PG, MySQL, MSSQLSpatial, OCI, SDE, PGeo, ODBC, WALK, IDB, SQLite and Ingres driver  (#5309)
 * introduce OGRLayer::FindFieldIndex() / OGR_L_FindFieldIndex() to lookup potentially laundered field names (RFC 42)
 * OGR SQL: upgrade to support RFC 41 (multiple geometry fields)
 * OGR SQL: more stricter checks
 * OGR SQL: make parsing error report a useful hint where the syntax error occurred
 * OGR SQL: fix thread-safety of swq_op_registrar::GetOperator() (#5196)
 * OGR SQL: support not explicitly specifying AS keyword for aliasing a column spec
 * OGR SQL: don't call CONCAT(a_column ...) or SUBSTR(a_column ...) as a_column
 * OGR SQL: validate that arguments of MAX, MIN, AVG, SUM, COUNT are columns and not any expression since this is not supported
 * OGR SQL: make AVG field definition a OFTReal
 * OGR SQL: implement MIN(), MAX() and AVG() on a date (#5333)
 * OGR SQL: fix SELECT * on a layer with a field that has a dot character (#5379)
 * SQL SQLITE dialect: Make it available to all OGR drivers that have a specialized ExecuteSQL() implementation

OGRSpatialReference:
 * Upgrade to EPSG 8.2 database
 * identify LCC_2SP instead of LCC_1SP if lat_0==lat_1 and lat_2 is present (#5191)
 * add a variety of linear units to proj4 parsing (#5370)
 * Fix crash in CleanupESRIDatumMappingTable() if it is called twice (#5090)
 * fix order of AXIS and UNIT nodes in a VERT_CS node (#5105)
 * ecw_cs.wkt: add missing TOWGS84[-168,-60,320,0,0,0,0] to NTF datum (#5145)
 * fix OGRSpatialReference::importFromProj4() to work with non-C locale (#5147)
 * morph central_latitude to latitude_of_origin in morphFromESRI() (#3191)
 * OGRProj4CT: avoid using proj when the 2 projections are actually identical (#5188)
 * add sanity checks in OGR_SRSNode::importFromWkt() (#5193)
 * VERT_CS: when importing from proj.4 put AXIS node after UNIT; COMPD_CS: when importing from EPSG:x+y, set a more meaningful name for the COMPD_CS node
 * OGRSpatialReference::Validate() : in addition to hand-validation, use WKT grammar from OGC 01-009 CT
 * preserve authority when importing +init=auth_name:auth_code (e.g. +init=IGNF:LAMB93)

Utilities:
 * ogrlineref: new utility to deal with linear geometries.
 * ogrinfo: upgrade to support RFC 41 (multiple geometry fields)
 * ogr2ogr: upgrade to support RFC 41 (multiple geometry fields)
 * ogr2ogr: bump default value for -gt from 200 to 20000 (#5391)
 * ogr2ogr: add -addfields option to add new fields found in a source layer into an existing layer ; add -unsetFieldWidth option to unset field with and precision; add -dim layer_dim option to force the coordinate dimension of geometries to match the one of the layer geometry type
 * ogr2ogr: Check that -t_srs is also specified when -s_srs is specified
 * ogr2ogr: add an explicit error message to report FID of feature that couldn't be inserted when CreateFeature() fails
 * ogr2ogr: make relaxed lookup optional and add a switch -relaxedFieldNameMatch to allow it (RFC 42)
 * ogr2ogr: make sure that the progress bar reaches 100% when converting OSM
 * ogr2ogr: make sure that target dataset is properly closed when a CreateFeature() fails (so that truncated shapefiles have their header file properly updated)
 * ogr_dispatch.py: Sample Python script to dispatch features into layers according to the value of some fields or the geometry type
 * ogrinfo.py: sync with ogrinfo (RFC 41)
 * ogr2ogr.py: port -nlt PROMOTE_TO_MULTI option from ogr2ogr.cpp (#5139)

CSV driver:
 * avoid erroneously reset of file content when opening in update mode a file without header (#5161)
 * upgrade to support RFC 41 in read/write (multiple geometry fields)
 * allow backslash doublequote to load (#5318)

DGN driver:
 * DGN writing: added polygon inner ring (holes) writing and MSLink writing (#5381)

DXF driver:
 * fix writing of 25D linestring where z is not constant (#5210)
 * fix writing of POLYLINE objects (#5217, #5210)
 * accept reading files starting with a TABLES section (#5307)
 * support reading 3DFACE and SOLID (#5380) entities
 * fix an error when processing clockwise circle arc (#5182)
 * avoid building an invalid polygon when edges cannot be reassembled: turn it into a multilinestring
 * use CPLAtof() instead of atof() to avoid issues with locales
 * fix linear approximation of circular and elliptic arc in HATCH boundaries (#5182)

DWG driver:
 * add support for reading AcDb3dPolyline (#5260)
 * fix linear approximation of circular and elliptic arc in HATCH boundaries (#5182)

FileGDB driver:
 * implement IgnoreFields API to speed-up a bit the conversion of a sub-set of fields when there's a huge amount of them (e.g. Tiger database).
 * when writing <Length> of an attribute, use size in bytes (#5192)
 * implement ref counting of the FileGDB SDK API' Geodatabase* object to avoid issues on Linux 64bit with interleaved opening and closing of databases (#4270)
 * honour update flag to determine which operations are allowed or not
 * add a driver global mutex to protect all calls as the FileGDB API SDK is not thread-safe at all
 * add a COLUMN_TYPES layer creation option to override default column types; support reading/writing XML column types
 * optimize GetFeatureCount() and GetExtent() when there are filters set
 * set the default width for string fields to 65536.
   The width can be configured with the FGDB_STRING_WIDTH configuration option
 * fix creation and writing of Binary fields; enable reading
 * add a CREATE_MULTIPATCH creation option

FME driver:
 * fix Linux compilation

GeoJSON driver:
 * recognize alternate formats such as the ones of https://code.google.com/p/election-maps/
 * add read support for TopoJSON
 * upgrade internal libjson-c to json-c 0.11 (#4676)
 * report integer values that are int64 as strings
 * add 3d support to esri geojson reader (#5219)
 * be less strict on looking for esri field type tag (#5219)
 * fix sometimes incorrect result (significant digit lost...) when using -lco COORDINATE_PRECISION=0
 * fix handling of huge coordinates when writing (#5377)

GeoRSS driver:
 * advertise OLCCreateField capability

GFT driver:
 * switch http to https for the oauth2 link to improve security

GML driver:
 * add support for multiple geometry columns (RFC 41)
 * add support for reading Finnish National Land Survey Topographic data (MTK GML)
 * add support for support Finnish NLS cadastral data and Inspire cadastral data.
 * add support for Czech RUIAN VFR format
 * add data/gml_registry.xml file to associate feature types with schemas.
 * extend .gfs syntax to be able to fetch OGR fields from XML attributes.
 * extend .gfs syntax to support multiple geometry columns, and define a geometry property name
 * autodiscover all XML attributes as OGR fields when creating .gfs file if GML_ATTRIBUTES_TO_OGR_FIELDS is set to YES (#5418)
 * allow the <ElementPath> in .gfs to have several components that give the full XML path
 * fix writing of .xsd file to avoid fid/gml_id being written as regular fields (#5142)
 * fix writing of global srsName attribute on the global boundedBy.Envelope when all layers have same SRS (#5143)
 * support for writing .gml/.xsd with fields of type StringList, RealList, IntegerList and support for parsing such .xsd files
 * when writing .xsd for a datasource that has fields of type StringList, RealList or IntegerList, advertise SF-1 profile in the .XSD schema
 * recognize xsd:boolean in XSD parsing and map it to String (#5384)
 * add STRIP_PREFIX and WRITE_FEATURE_BOUNDED_BY dataset creation option to help minimizing the size of GML files
 * don't write top <gml:boundedBy> in GML files with multiple layers of different SRS
 * fix segfault when reading a GML file with huge coordinates (#5148)
 * avoid opening our own .xsd files as valid datasources (#5149)
 * make driver thread-safe with Xerces
 * open successfully GML datasources with 0 layers (#249, #5205)
 * fix tweaking of DescribeFeatureType requests
 * support reading WFS 2.0 GetFeature documents with wfs:FeatureCollection as a wfs:member of the top wfs:FeatureCollection
 * fix for crash on certain xlink:href with GML_SKIP_RESOLVE_ELEMS=NONE (#5417)
 * GML geometry: fix duplicated points in GML_FACE_HOLE_NEGATIVE=YES mode (TopoSurface) (#5230)
 * GML geometry: accept CompositeSurface as a child of surfaceMembers (#5369)
 * GML geometry: join multilinestrings to linestrings in rings
 * GML geometry: correctly deal with MultiSurface of Surface of PolygonPatch where a PolygonPatch has only interior ring(s) and no exterior ring (#5421)
 * GML geometry: accept formulations of 'MULTIPOINT EMPTY, MULTILINESTRING EMPTY, MULTIPOLYGON EMPTY and GEOMETRYCOLLECTION EMPTY that are valid GML 3 (and accepted by PostGIS)
 * GML geometry: make use of cs, ts and decimal attributes of (deprecated) gml:coordinates element
 * GML geometry: accept XML header and comments

GPX driver:
 * advertise OLCCreateField capability

ILI driver:
 * add support for multiple geometry columns (RFC 41)
 * use IlisMeta model reader/writer instead of IOM
 * add layers for surface and area geometries

KML driver:
 * output KML that validates the ogckml22.xsd schema by placing <Schema> elements under the <Document> level (#5068)
 * in writing mode, avoid defining an extending schema for the name and description fields (related to #5208)

LIBKML driver:
 * various checks, fixes and improvements related to OGC KML 2.2 Abstract Test Suite
 * add support for reading <gx:Track> as a LINESTRING (#5095)
 * add support for writing and reading <snippet>
 * add support for writing atom:author, atom:link, phonenumber, Region,
   ScreenOverlay, 3D model, StyleMap
 * add support for reading and generating Camera object
 * add layer creation options to generate a LookAt element at layer level
 * if UPDATE_TARGETHREF dataset creation option is defined, a NetworkLinkControl/Update document will be created
 * add dataset creation options to generate a NetworkLinkControl element
 * add dataset and layer creation options LISTSTYLE_ICON_HREF and LISTSTYLE_TYPE
 * add support for writing a NetworkLink
 * add support for creating PhotoOverlay objects
 * add support for creating BalloonStyle elements
 * offer LIBKML_USE_SIMPLEFIELD configuration option can be set to NO to use Data element instead of SimpleField
 * add layer creation option FOLDER to optionally write layers as Folder instead of Document
 * add dataset and layer creation options NAME, VISIBILITY, OPEN, SNIPPET and DESCRIPTION
 * workaround bugs in pretty serializers
 * when writing a .kmz file, put layers .kml docs into a layers/ subdirectory
 * fix mem leaks, and use after free in kml2FeatureDef() (#5240)
 * create document with default namespace set to http://www.opengis.net/kml/2.2
 * when writing, consider empty strings as unset (useful when converting from CSV)
 * don't write empty <Style /> element if OGR_STYLE is empty string
 * transform multigeometry with one single component into single geometry
 * create libkml/ subdirectory entry in .kmz

MITAB driver:
 * support reading MIF file with no associated MID file (when MIF file advertises 'Columns 0') (#5141)

MSSQLSpatial driver:
 * Fix MSSQL to be aware of removed tables (#5071)
 * Eliminate the per table server access when identifying the spatial reference (#5072)
 * Improve detection of geometry column with MSSQL select layer (#4318)
 * Fix for an issue with multicolumn primary keys (#5155)
 * Add support for handling non spatial data tables (#5155)
 * Fix creation of spatial_ref_sys and geometry_columns tables (#5339)

MySQL:
 * robustness for huge coordinates in spatial filter

NAS driver:
 * also accept XML files that have NAS-Operationen_optional.xsd in header
 * make driver thread-safe with Xerces
 * warn when geometry cannot be saved

OCI driver:
 * fix make plugin option

ODBC driver:

OSM driver:
 * support relations with more than 2000 members in a OSM XML file (#5055)
 * make the driver work with PBF files produced by osmconvert.
 * osmconf.ini: report the waterway attribute for the lines layer (#5056)
 * add an option in osmconf.ini to enable creating a 'all_tags' field, combining both fields specifically identified, and other tags
 * always use quoting of key/values in other_tags field (#5096)
 * use alternative implementation of FindNode() that is a bit more efficient when process is CPU-bound
 * fix issue with attribute filtering
 * avoid GetNextFeature() to be blocked in certain conditions in non-interleaved mode (#5404)

PG driver:
 * upgrade to support RFC 41 in read/write (multiple geometry fields)
 * use ST_Estimated_Extent() on table layers if GetExtent() is called with force = 0 (#5427)
 * add a OGR_TRUNCATE configuration option (#5091)
 * let postgres name the constraints to avoid long name truncation resulting in conflicts (#5125)
 * map PG 'numeric' to OFTReal instead of OFTInteger
 * retrieve SRID from geometry value, if not found in geometry_columns (#5131)
 * fix ResolveSRID() when the current user has no select rights on geometry_columns table (#5131)
 * fix retrieval of SRID on a table without SRID constraint, and when the datasource is opened with ' tables=fake' (#5131)
 * robustness for huge coordinates in spatial filter
 * fix delete layer bug on PG 2.0 (#5349)
 * fix to be able to detect version of EntrepriseDB (#5375)
 * Fix UTF-8 encoded string length

PGDump driver:
 * upgrade to support RFC 41 (multiple geometry fields)
 * fix error when inserting a string that has backslash in it with PostgreSQL >= 9.1 (#5160)

REC driver:
 * fix runtime compatibility for Windows

Shapefile driver:
 * fix buffer overflow when creating a field of type Integer with a big width (#5135)
 * delete temporary .cpg file earlier in REPACK
 * fix leak of file descriptor in error code paths
 * speed optimizations when reading geometries from .shp (#5272)
 * add a 2GB_LIMIT=YES layer creation option (and SHAPE_2GB_LIMIT configuration option)
 * .sbn support: increase allowed max depth from 15 to 24 (#5383)

SOSI driver:
 * fix memory leaks, and crashes
 * advertise OLCCreateField capability

S57 driver:
 * make the driver thread-safe
 * remove use of MAX_CLASSES in favor of dynamic sizing (#5227)
 * fix GetFeature() on DSID layer

SQLite/Spatialite driver:
 * Make SQLite SQL dialect compatible with multiple geometry fields (note: however, multiple geom fields is not yet supported by OGR SQLite table layers)
 * implement SetAttributeFilter() on SQL result layers, to directly inject it as a WHERE clause when possible
 * add the OGR_SQLITE_PRAGMA configuration option
 * Spatialite: correctly set proj4text field of spatial_ref_sys when inserting a new SRS in the spatial_ref_sys table (#5174)
 * Spatialite: fix insertion in spatial_ref_sys to avoid issues with non-numeric authority codes (auth_srid)
 * Spatialite: make creation of database much faster with spatialite 4.1 by using InitSpatialMetaData(1) (#5270)
 * Spatialite: use thread-safe initialization for spatialite >= 4.1.2
 * avoid Spatialite views to emit (hidden) errors that cause troubles to MapServer OGR input driver (#5060)
 * attempt to make VirtualOGR (and thus sqlite dialect) work even with a libsqlite3 compiled with SQLITE_OMIT_LOAD_EXTENSION (on Unix only)
 * add warning when calling CreateField() with a field name that is 'ROWID' since it can cause corrupted spatial index
 * serialize StringList as non-truncated strings
 * detection of DateTime/Date/Time column type on the result of a max() / min() function
 * ensure fields of type DateTime/Date/Time are properly recognized if the table is empty (#5426)

VFK driver:
 * fix memory leak
 * change VFK header check (first line starts with '&H')
 * implement OGR_VFK_DB_DELETE configuration option
 * read all data blocks by default
 * fix GetFeatureCount()
 * don't use existing internal db when it's older then original vfk file

VRT driver:
 * add support for multiple geometry columns in OGRVRTLayer, OGRVRTUnionLayer and OGRVRTWarpedLayer (RFC 41)
 * add validation of OGR VRT document against the schema (if libXML2 available);
   can be disabled by setting GDAL_XML_VALIDATION configuration option to NO
 * make relativeToVRT=1 work with CSV:filename or GPSBABEL:driver:filename (#5419)

WFS driver:
 * WFS 2.0: be a good citizen and send TYPENAMES (with a S) for GetFeature request (but still TYPENAME for DescribeFeatureType)
 * accept TYPENAME(S) in URL with characters escaped by '%' (#5354)

XLS driver:
 * don't use sheetId attribute from workbook.xml to link a sheet name to its filename. The first sheet is sheet1.xml, etc...

XPlane driver:
 * accept V1000 APT files

## SWIG Language Bindings

All bindings:
 * include constraints.i in gdal.i so that NONNULL constraints are really applied
 * add Feature.SetFieldBinaryFromHexString()
 * add SpatialReference.EPSGTreatsAsNorthingEasting (#5385)
 * map OGR_L_GetStyleTable(), OGR_L_SetStyleTable(), OGR_DS_GetStyleTable(), OGR_DS_SetStyleTable()
 * Add OGR_STBL_AddStyle() and map all OGR_STBL_ C methods to SWIG

CSharp bindings:
 * Fix handling UTF8 strings in GDAL C# (#4971)
 * Add C# typemaps for RFC-39 functions (#5264)
 * Fix typo in OGRLayerAlg.cs (#5264)
 * Add missing typemaps for C# (#5265)

Java bindings:
 * run 'make ANDROID=yes' in swig/java to generate SWIG bindings that compile for Android (#5107)
 * fix linking issue that is encountered in non libtool builds with g++ 4.6
 * add multireadtest utility
 * respect JAVA_HOME set via configure

Perl bindings:
 * ColorTable method of ColorTable class was documented but ColorEntries was
   implemented. Implemented but deprecated ColorEntries.

Python bindings:
 * fix ref-counting of callable passed to gdal.PushErrorHandler() that could cause segfaults (#5186)
 * make gdal_array.NumericTypeCodeToGDALTypeCode accept numpy dtype arguments (#5223)
 * add default xoff,yoff,xsize,ysize values to ReadRaster calls
 * make ogr.CreateGeometryFromWkt() and SpatialReference.ImportFromWkt() properly validate their argument (#5302)

# GDAL/OGR 1.10.0 Release Notes (r23656 to r25905)

## In a nutshell...

 * New GDAL drivers:
   - ARG: read/write support for ARG datasets (#4591)
   - CTable2: read/write support for CTable2 datum grid shift format
   - DDS: write-only support for DirectDraw Surface format (#5017)
   - IRIS: read support for products generated by the IRIS weather radar software (#4854)
   - MAP:  read OziExplorer .map files (#3380)
   - MBTiles: read-only support for MBTiles rasters (needs libsqlite3)
 * New OGR drivers:
   - ElasticSearch: write-only support to write into ElasticSearch databases (needs libcurl)
   - ODS : read/write support for OpenOffice .ods (Open Document Spreadsheets) (needs libexpat)
   - OSM : read-only support for .osm / .pbf OpenStreetMap files
   - PDF: read/write support for vector/structured PDF files
   - XLSX: read/write support for MS Excel 2007 and later Open Office XML .xlsx spreadsheets (needs libexpat)
 * RFC 39: OGR Layer algebra methods : http://trac.osgeo.org/gdal/wiki/rfc39_ogr_layer_algebra
 * Add a SQL SQLite dialect : http://gdal.org/ogr/ogr_sql_sqlite.html
 * Make GDAL loadable as a SQLite3 extension (named VirtualOGR) (#4782)
 * /vsicurl_streaming/: new virtual file system handler designed to read in streaming mode dynamically generated files
 * GDAL API_PROXY mechanism to run GDAL drivers in a separate process: http://gdal.org/gdal_api_proxy.html
 * Significantly improved drivers : PDF, SQLite, JP2OpenJPEG
 * Add a geocoding client : http://gdal.org/ogr/ogr__geocoding_8h.html
 * Upgrade to EPSG 8.0 database

## New installed files

 * data/ozi_datum.csv
 * data/ozi_ellips.csv
 * data/osmconf.ini
 * include/cpl_progress.h
 * include/cpl_spawn.h
 * bin/gdalserver[.exe]

## Backward compatibility issues

Due to the 2-digit number '10' in GDAL 1.10, the GDAL_VERSION_NUM macro has been changed.
The new advised way of testing the GDAL version number (for GDAL 1.10 or later) at compilation time is :

#ifdef GDAL_COMPUTE_VERSION /* only available in GDAL 1.10 or later */
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(1,10,0)
///
#endif
#endif

Testing of previous versions is of course unchanged.

## GDAL/OGR 1.10.0 - General Changes

Build(Unix):
 * New optional dependencies : libpcre (for regular expressions support in SQLite), libxml2 (validation of GML files)
 * --with-python: make it work with python3, and also accept path to python binary as argument of --with-python (#4725)
 * Use nc-config to detect netcdf compilation and linking parameters (#4424)
 * Add frmts/vrt to CONFIG_CFLAGS for development version of gdal-config (needed for postgis 2.0 compilation)
 * Fix compilation failure with iconv on FreeBSD (#4525)
 * Make FileGDBAPI detection work with FileGDBAPI v1.1 and v1.2 (#4570)
 * Fix build on Gentoo with its custom zlib 1.2.6 with the OF macro removed
 * Mark man target as phony (#4629)
 * Add guess for the directory where to find openjdk on Ubuntu 12.04 (#4643)
 * Look for geotiff headers in /usr/include/libgeotiff too (#4706)
 * For install target, create gdalplugins subdirectory in $(DESTDIR)$(INST_LIB)/ (Unix, except MacOSX)
 * Better detection of OpenCL headers and library (#4665)
 * Changed libdap test to use dap-config to detect version when possible

Build(Windows):
 * (Preliminary) support to build INGRES
 * Make CPLGetErrorHandlerUserData() exported
 * Make OGDI include path overridable (to match OSGeo4W's default location)
 * Build and install plugins

## GDAL 1.10.0 - Overview of Changes

Port:
 * CPL Thread API: add condition API, modeled on POSIX pthread_cond_ API
 * Add CPLGetNumCPUs()
 * Deserialize various forms of textual representation for positive/negative infinity
 * Add routine to validate a XML file against its XSD schema (needs libxml2); 'optimize' it for GML files
 * CPLRecodeStub(): for Windows, provide an implementation of UTF8 <--> CPxxx conversions using Windows API
 * Make VSIFileManager::Get() thread-safe
 * Fix thread-safety of CPLOpenShared() (#4848)
 * Add CPLZLibDeflate() and CPLZLibInflate()
 * Add API for OAuth2 authentication protocol.
 * Curl: allows setting the CURLOPT_PROXYAUTH setting through GDAL_PROXY_AUTH=BASIC/NTLM/DIGEST/ANY,
   allow setting CURLOPT_HTTPAUTH through GDAL_HTTP_AUTH=BASIC/NTLM/GSSNEGOTIATE/ANY (#4998)
 * /vsicurl/ and /vsicurl_streaming/ : make it possible to cache the files in RAM with VSI_CACHE = TRUE
 * /vsizip/: fix handling of Eof() that could cause missed last feature(s) of zipped shapefiles (#4748)

Core:
 * Add a DMD_SUBDATASETS driver metadata, and advertise it in relevant drivers (#4902)
 * Fix statistics computation when nodata value is +/- infinity (#4506)
 * GDALNoDataMaskBand: implement IRasterIO() for an optimization in common use case (#4488)
 * GDALVersionInfo(): add BUILD_INFO version string
 * GMLJP2: Fix bad interpretation when Lat/Long (#4657)
 * Set nodata value when creating external overviews so that AVERAGE algorithm works as expected (#4679)
 * EXIFExtractMetadata() moved to gcore/gdalexif.cpp to make it usable for other drivers
 * Fix infinite recursion in GDALOpen() (#4835)
 * GDALRasterBand::IRasterIO() : optimize downsampling/upsampling code path
 * C API: make GDALSetDefaultRAT() accept a NULL RAT. All drivers are ready for that now.
 * GDALRasterBand::GetDefaultHistogram(): change how min and max bounds are computed in the non GDT_Byte case
 * GDALDataset::BlockBasedFlushCache(): fix crash when band has sub-blocking

Algorithms:
 * GSOC Image Correlator work (preliminary state)
 * Warp: divide Lanczos resampling time by at least a factor of 4.
 * Warp: add NUM_THREADS warping option to set the number of threads to use to parallelize the computation part of the warping
 * Warp: do not stop collecting chunks to operate on just because some subchunks fail (#4795)
 * Warp: add mode and average resampling methods (#5049)
 * OpenCL warper: handle errors in set_supported_formats(), fix memory leaks in error code paths, add detection of Intel OpenCL (by the way, Intel OpenCL seems to work properly only with a Float32 working data type)
 * OpenCL warper: fix segmentation fault related to source/destination validity masks (#4840)
 * Geoloc: do not trust pabSuccess in geolocation transformer (#4794)
 * Geoloc: add bilinear interpolation of coordinates from backmap (#4907)
 * Geoloc: add GDALTransformGeolocations() and SWIG binding
 * Add nearest neighbor and cubic interpolation of DEM in GDALRPCTransform (#3634).
   User can set RPC_DEMINTERPOLATION to near, bilinear or cubic to interpolate of input DEM file which set in RPC_DEM. The default interpolation is bilinear.
 * gdal_rasterize: fix problem identifying some connected-8 polygons (#4647)
 * gdal_grid: speed-up dramatically nearest neighbour search (with radius1 == radius2) by using a search quad tree
 * gdal_grid: parallelize processing by specifying the GDAL_NUM_THREADS configuration option (default to ALL_CPUS)
 * gdal_grid: for 'invdist' algorithm with default parameters, use SSE optimized version if available (at compile and runtime). Can be disabled with GDAL_USE_SSE=NO

Utilities:
 * General: make usage message more self-explanatory in case of bad option (#4973)
 * gdalmove.py: New application for "warping" an image by just updating its SRS and geotransform.
 * gdal_edit.py: promote it as an 'official' tool (#4963)
 * gdalwarp: add "-r average" and "-r mode" resampling methods (#5049)
 * gdalwarp: copy metadata and band information from first source dataset and detect for conflicting values, new options -nomd and -cvmd (#3898)
 * gdalwarp: optimization when (-tr and -te) or (-ts and -te) are specified (#4804)
 * gdalwarp: assign color interpretation of source bands to target dataset, in the case of target VRT (#4462)
 * gdalwarp: add -setci option to set the color interpretation of the bands of the target dataset from the source dataset
 * gdal_translate: accept -srcwin or -projwin values that fall partially or completely outside the source raster extent. Introduce -epo and -eco options to error out in those situations.
 * gdallocationinfo: add a -overview overview_level option to specify an overview level, instead of the base band
 * gdalsrsinfo: try to open with GDAL and OGR even if argument is not a file (#4493)
 * gdaldem: add a -combined option to the hillshade mode to compute combined hillshading (#4753)
 * gdaldem: fix color-relief output with driver that has only CreateCopy() capability, and when the source block dimensions are not multiple of the raster dimension (#4764)
 * gdaltindex: add -t_srs option, to transform all input source bounds to same SRS (#4773)
 * gdalbuildvrt: add -a_srs option
 * gdalbuildvrt: add -sd option to select subdataset by its number
 * gdalbuildvrt: add a -b flag (#4992)
 * gdalgrid: increase working buffer of gdal_grid binary to 16 MB
 * gdal_retile.py: Don't pass creation options to the MEM driver used for generating temporary datasets (#4532)
 * gdal_edit.py: make -a_srs option work properly by expanding the user input to WKT; support -a_srs ''
 * gdal_edit.py: add support for -gcp option
 * gdal2tiles.py: make KML output conformant with KML 2.2 (#4536)
 * gdal2tiles.py: OL 2.12 support (#4742)
 * gdal_polygonize.py: add -8 option to select 8 connectedness (#4655)
 * gdal_merge.py, gdalident.py: remove using of glob() API
 * gdal2xyz.py: fix output of -csv mode with multi-band raster
 * gdal_contour / gdal_rasterize / gdal_translate: accept numeric values in scientific format.
 * crs2crs2grid.py: New sample Python script
 * gdalcompare.py: New sample Python scrip to compare GDAL datasets
 * gdal_calc.py: add --co creation option flag (#4964)
 * gdaladdo: add a -b flag
 * pct2rgb.py: deal with color tables with more than 256 entries (#4905)

Multi driver changes:
 * Add support for reading .j2w, .jp2w and .wld files for JP2ECW, JP2MrSID, JP2OPENJPEG and JPEG2000 drivers (#4651)

AAIGrid:
 * Change float format string for AAIGrid to prevent pointless padding/decimals (#3732)

ACE2 driver:
 * Fix typo that prevented dataset to be opened with explicit /vsigzip/ (#4460)

ADRG driver:
 * Various fixes when opening ill-formed datasets.

BAG driver:
 * Fix serious problems with tiled images, particularly when not multiples of tile size (#4548)
 * Added capture of dateTime attribute
 * Support WKT (with Esri style VERTCS) spatial reference
 * Allow WGS84 spatial reference
 * Include compression method in metadata.

BT driver:
 * Fixes for huge files (>2GB) support (#4765)

CEOS2 driver:
 * Add various radarsat-1 related metadata fields (#4996)

DIMAP driver:
 * Check signature in METADATA.DIM, not just file existence
 * Fixed DIMAP2 driver to get the proper absolute path in a specific case

DTED driver:
 * Write the updated value of the partial cell indicator to the file (#4687)
 * Honour 'Longitude count' field of Data Record to deal properly with files with missing columns at the left and/or right of the file (#4711)

ECW driver:
 * Add support for ECW SDK 5.0
 * Improve picking performance on large datasets (#4790)
 * Use ECW SDK to do super-sampling for SDK >= 4.X
 * Expose 256x256 block dimension instead of scanline
 * Workaround a ECW SDK 3.3 bug, when doing a RasterIO() with the total number of bands, but not in the 1,2,..n order (#4234)
 * Add heuristics to detect successive band reading pattern (such as done by QGIS). Beneficial for ECWP

ENVI driver:
 * Add support for writing RPCs and GCPs
 * Add ability to access all ENVI header fields to the ENVI reader (#4735)
 * Write in the ENVI header metadata found in the ENVI metadata domain (#4957)
 * Fix reading of .sta file on 64bit Linux
 * Assume BSQ interleaving when 'interleave' keyword missing or unknown
 * Fix category names writing
 * Remove 'envi fft result' from the blacklist
 * Report wavelength and wavelength_units as band metadata (#3682)

ENVISAT driver:
 * Ported to VSI*L

GeoRaster driver:
 * Add spatialExtent and extentSRID create-options (#4529)
 * Fix JPEG quality not updated on metadata (#4552)
 * Search for RDT as regular table
 * Add support for RPC (#4038)

GIF driver:
 * Add support for giflib 4.2.0 (#4675) and giflib 5.0

GMT driver:
 * Make GMT driver thread-safe by adding a global mutex (since the netcdf library isn't thread-safe)

GTiff driver:
 * Internal libtiff and libgeotiff refreshed from upstream
 * Use EXTRASAMPLE_UNASSALPHA by default (behavior change w.r.t. previous GDAL versions) (#4733)
 * Add support for reading GeoEye *.pvl metadata files (#4465)
 * Lossless CreateCopy'ing() from a JPEG dataset
 * Read EXIF metadata in the EXIF metadata domain
 * Ensure that rowsperstrip is never larger than ysize (#4468)
 * Fix writing of RGBA pixel-interleaved JPEG-compressed TIFF (#4732)
 * Set color interpretation to GCI_PaletteIndex after calling SetColorTable() (#4547)
 * Conversion for 8-bit unpacked CMYK (PHOTOMETRIC_SEPARATED) to RGBA.
 * Maximize EPSG compatibility where PCS is defined (#4607)
 * Ensure that unusual units with an authority node are saved nicely (like EPSG:2066)
 * Add CT_HotineObliqueMercatorAzimuthCenter support
 * Fix PolarStereographic / 9829 support
 * Make sure that GetMetadata() initializes the value of GDALMD_AREA_OR_POINT item, if not already done (#4691)
 * When building overviews, if the image has already an internal mask, then build internal overviews for the mask implicitly
 * Better handling of SetMetadata(a_string) (#4816)
 * Use GTIFAllocDefn/GTIFFreeDefn with libgeotiff 1.4.1+
 * Add support for GEO_NORMALIZE_DISABLE_TOWGS84 (#3309)
 * Improve handling of description and offset/scale without reverting to .aux.xml
 * Workaround defects in libtiff 3.X when generating several overview levels at the same time
 * Special case where the EGM96 Vertical Datum code is misused as a Vertical CS code (#4922)
 * Support unsetting geotiff tags when calling SetGeoTransform([0,1,0,0,0,1]) and SetProjection('')
 * Rework how CSV files are searched w.r.t libgeotiff (#4994)

GRIB driver:
 * Report nodata value (#4433)
 * Fix fgetc signed/unsigned problem for Grib format VSI*L reader (#4603)
 * Avoid caching more than 100 MB in case of dataset with many bands (#4682)
 * uses meshLat as the latitude_of_origin parameter of LCC projection (#4807)

GSAG driver:
 * Fix hangs when reading truncated dataset (#4889)

GS7BG driver:
 * Implement Create() and CreateCopy() (#4707)

GTX driver:
 * Support reading old GTX files where datatype was Float64

GXF driver:
 * Avoid having big buffer on stack (#4852)
 * Avoid locale floating point parsing problems (similar to r24367).
 * Implement continued lines mechanism (#4873)
 * Fix various vulnerabilities / DoS

HDF4 driver:
 * Add ability to increase the maximum number of opened HDF4 files
 * Unix build: avoid issue with system hdfeos library
 * Ensure we do not try to use the grid tile api for non-tilesized chunks (#4672)
 * Preserve more Float32 attribute precision
 * Import HDF USGS GCTP angular parameters as radians
 * Restore conventional add_offset interpretation (#4891)
 * Be more careful about missing dimensions (#4900)
 * Make HDF4 driver thread-safe by adding a global mutex (since the HDF4 library isn't thread-safe)
 * Search for "coremetadata" attribute name instead of "coremetadata."

HDF5 driver:
 * Add support for COSMO-SkyMed metadata (#4160)

HFA driver:
 * Added BASEDATA support for EPT_u1 (#4537)
 * Fix crash on dataset closing when .ige file header is corrupted (#4596)
 * .aux overviews: avoid destroying existing overviews when asking twice in a row to build overviews for exactly the same overview levels (#4831)
 * Fix sizing of RAT string column maxwidth to include null char (#4867)
 * Fix segfault in HFAAuxBuildOverviews with selected bands (#4976)

INGR driver:
 * do not reduce tile size to image size (#4856)
 * Fix value inversion when reading type 9 (bitonal RLE) untiled files (#5030)
 * fix slowness and incorrect random reading with RLE datasets (#4965)
 * Enable reading bitonal rle files wider than 22784 (0x5900) pixels (#5030)
 * Add RESOLUTION metadata/option to read/write DPI (#5030)
 * Add write support for .rle (bitonal rle files) to test the above (#5030)

ISIS3 driver:
 * Ensure scaleFactor defaults to 1 (#4499)

JP2ECW driver:
 * Workaround conflict between ECW SDK deinitialization and GDAL deinitialization, as shown by gdaljp2ecw tests of imageio-ext (#5024)
 * Promote 1bit alpha band of a RGBA dataset to 8 bits to improve general user experience (can be turned off by setting GDAL_ECW_PROMOTE_1BIT_ALPHA_AS_8BIT to NO)

JP2KAK driver:
 * Capture Corder in metadata for user convenience
 * Fix writing of resolution box where the xresolution value was written instead of the yresolution one
 * Skip bands that have different data type when reading multiple bands in IReadBlock() (#4638)
 * Default to less than 250000 lines per tile (#5034)

JP2OpenJPEG driver:
 * Require OpenJPEG 2.0.0 now
 * Use several decoding threads when processing multi-tiles IRasterIO() requests
 * Add support for writing georeferencing
 * Read and write JP2 Res box and translate it from/to TIFFTAG_XRESOLUTION, TIFFTAG_YRESOLUTION and TIFFTAG_RESOLUTIONUNIT metadata items
 * Promote 1bit alpha band of a RGBA dataset to 8 bits to improve general user experience (can be turned off by setting JP2OPENJPEG_PROMOTE_1BIT_ALPHA_AS_8BIT to NO)

JPEG driver:
 * When there are no external overviews built, take advantage of the nature of JPEG compression to expose overviews of level 2, 4 and 8
 * Don't return junk content when requesting xml:XMP but no XMP metadata is present (#4593)
 * add a INTERNAL_MASK creation option to be able to disable appending the ZLib mask if not needed
 * add support for creating a JPEG loss-less file starting with the recent IJG libjpeg v9
   (with -co ARITHMETIC=yes -co BLOCK=1 -co COLOR_TRANSFORM=RGB1)

JPEG2000 driver:
 * do not accept by default source bands of type different from Byte, Int16 or UInt16 since they seem to cause crashes in libjasper.
   This can be overridden, at your own risk, by setting JPEG2000_FORCE_CREATION configuration option to YES (#5002)

KMLSuperOverlay driver:
 * Add read support
 * Remove bogus code that limited generation to one zoom level (#4527)
 * Set minLodPixels to 1 for zoom level 0 (#4721)
 * Fix bad placing of tiles with raster of the extent of a country or more (#4834)
 * Add FIX_ANTIMERIDIAN creation option (#4528)

L1B driver:
 * Add support for NOAA19, METOP-B and guess for METOP-C (#2352)

MG4Lidar driver:
 * Add UTF-8 filename support under Windows (#4612)

NetCDF driver:
 * Fix for gdal_rasterize (#4432)
 * Enable PAM for band histogram and statistics (#4244)
 * Add longitude_of_prime_meridian value to PRIMEM
 * Fix SetNoDataValue() - do not update when already set to new value (#4484)
 * Convert longitude values in [180,360] interval to [-180,180] (#4512) - override with config option GDAL_NETCDF_CENTERLONG_180=0
 * Support 2D GEOLOCATION arrays when a projected variable has coordinates attribute and supporting long/lat arrays (#4513)
 * Ignore coordinate and bounds variables (CF sections 5.2, 5.6 and 7.1) as raster bands, but expose them as subdatasets - this allows opening files with projected SRS (or dimension bounds) directly, without specifying the variable as a subdataset
 * Better support for Gaussian grids - store original latitude values in special Y_VALUES geolocation metadata item and use it for netcdf export (#4514)
 * Write multi-dimensional variables to a single variable (not one for each unrolled band) in CreateCopy() (#2581)
 * Fix handling of km units in netcdf driver and importFromProj4() (#4769)
 * Fix detection of 1 and 2 pixel width/height netcdf datasets (#4874)
 * Fix subdataset data type info (#4932)
 * Make netCDF driver thread-safe by adding a global mutex (since the netcdf library isn't thread-safe)

NITF driver:
 * nitf_spec.xml: add definition of ACFTB and AIMIDB TREs
 * Don't escape DESDATA for sizes >10mb (#4803)
 * Fix NITF creation when both BLOCKA and TRE are passed in (#4958)
 * Allow reading JPEG-in-NITF where JPEG stream dimensions are larger than NITF dimensions (#5001)
 * Support for cases with 2 LUTs

Northwood driver:
 * Fixes for huge files (>2GB) support (#4565, #4645)
 * NWT_GRD: don't advertise scale/offset as they are transparently applied in IReadBlock() (#5839).

PDF driver:
 * Add CreateCopy() support
 * Add update support for georeferencing and metadata
 * Add support for selective layer rendering (only with poppler backend)
 * Add GDAL_PDF_BANDS = 3 or 4 config option to select RGB or RGBA rendering; add GDAL_PDF_RENDERING_OPTIONS config option to enable selective feature rendering by combining VECTOR, BITMAP and TEXT values
 * Fix parsing of some georeferencing (r24022)
 * Recognized ISO georeferencing set at the image level (and not at the page level); expose such images as subdatasets (#4695)
 * Support Poppler 0.20 (and for current Poppler development version 0.23/0.24)
 * UTF-16 support
 * Report registration points as GCPs (OGC Best Practice)
 * Allow building driver with both Poppler and Podofo (testing purposes mostly)
 * Fix crashes on some PDF files with poppler >= 0.17.0 (#4520)
 * Improve rounding of raster dimensions (#4775)
 * With podofo, avoid launching the 'pdftoppm' process in a visible console on Windows (#4864)
 * Select neatline whose description is 'Map Layers' when it is found, to keep the best neatline for USGS PDF Topo
   and add GDAL_PDF_NEATLINE config. option to override that default value.
 * Improve detection of DPI for USGS Topo PDF to get the maximum raster quality (will increase
   reported dataset height and width)
 * Extract USGS Topo PDF embedded metadata in the EMBEDDED_METADATA domain

PNG driver:
 * Internal libpng upgraded to 1.2.50

PostgisRaster driver:
 * Implement CreateCopy and Delete (#4530)
 * Supports reading of tiled raster with irregular blocking and irregular pixel size
 * Cache postgres db connection.
 * Use PG environment variables as fallback when settings are not provided in the connection string. (#4533).
 * Do not report nodata value when there is none (#4414)
 * Removed dependency on the 'rid' column.
 * Fix to make SQL queries with un-rounded floating point string representations. (#4736)
 * Fix overview support

Rasterlite driver:
 * Support all resampling methods for internal overviews (#4740)
 * Fix overview support with multi-table datasets (#4568, #4737)
 * Add RASTERLITE_OVR_OPTIONS configuration option to specify options for the tiles of the internal overviews

RMF driver:
 * Fix incorrect zone number detection for Transverse Mercator (#4766)

RPFTOC driver:
 * Relax SanityCheckOK() to avoid rejecting valid CIB datasets (#4791)
 * Avoid selecting a color table that is full black
 * Add missing NITF series code 'TF' for 'TFC' (and fix typo in some other long descriptions) (#4776)

RS2 driver:
 * added various metadata fields (#4997)

SDTS driver:
 * Various fixes when opening ill-formed datasets.

SRP driver:
 * Various fixes when opening ill-formed datasets.

TIL driver:
 * Fix TIL driver using overview (#3482)
 * Add set projection and geotransformation for TILDataset

TSX driver:
 * Fix crashes in Identify() with certain filenames, and the file is empty or doesn't exist (#4622)

VRT driver:
 * VRTWarpedDataset: add INIT_DEST=0 if no INIT_DEST specified (#4571)
 * VRTFilteredSource: fix RasterIO() to take into account source and dest windows (#4616)
 * avoid crashes due to int overflow when dealing with requests filling a buffer larger than 2 GB (#4815)
 * VRTSourcedRasterBand: override ComputeRasterMinMax(), ComputeStatistics() and GetHistogram() to make them run on sources, only when there's one VRTSimpleSource covering the whole VRTSourcedRasterBand
 * solve issue when the VRT is a symlink and that the sources filenames are RelativeToVRT (#4999)
 * Fix relariveToVRT option in VRTRawRasterBand (#5033)

WCS driver:
 * Support version 1.1.2

WebP driver:
 * Allow reading/writing alpha channel (libwebp >= 0.1.4)
 * Add support for reading XMP metadata in the xml:XMP metadata domain

WMS driver:
 * Implement GetMetadataItem(Pixel_iCol_iLine, LocationInfo) to retrieve pixel attributes
 * Retrieve nodata, min and max values, defined per band or per dataset in the config file (#4613)
 * Add GetColorTable(), used by the TiledWMS mini driver (#4613)
 * Update TiledWMS mini-driver to support new variable substitution mechanism, min/max/nodata settings, color table support (#4613)
 * Add a <UserPwd> element in the XML service description file

XYZ driver:
 * avoid rescanning output file at end of CreateCopy()

## OGR 1.10.0 - Overview of Changes

Core:
 * Add OGRLayerDecorator class (decorator base class for OGRLayer),
 * Add OGRWarpedLayer class (on-the-fly reprojection of a base layer)
 * Add OGRUnionLayer class (on-the-fly concatenation of several base layers)
 * Add OGRFieldDefn::IsSame() and OGRFeatureDefn::IsSame()
 * Add OGRAbstractProxiedLayer, OGRProxiedLayer and OGRLayerPool classes
 * OGRGeometry: set SRS of geometries created via GEOS functions from the source geometry (idem for geometries returned by OGR_G_ForceXXXX() methods) (#4572)
 * OGRFeature: Add OGR_SETFIELD_NUMERIC_WARNING option to issue a warning when not fully
               numeric values are assigned to numeric fields. If the field type is Integer,
               then also warn if the long value doesn't fit on a int.
 * Add OGR_G_PointOnSurface() and add it to SWIG bindings
 * Add OGR_G_ForceToLineString / OGRGeometryFactory::forceToLineString to join
   connected segments in line strings.
 * Better implementation of getDimension() for OGRMultiPoint, OGRMultiLineString, OGRMultiPolygon and OGRGeometryCollection
 * Fix incorrect rounding in OGRFormatDouble that affected geometry WKT output (#4614)
 * OGRSQL: support UNION ALL of several SELECTs
 * OGRSQL: defer ORDER BY evaluation until necessary, so that a spatial filter can be taken into account after layer creation
 * OGRSQL: allow using indexes when OR or AND expressions are found in the WHERE clause
 * OGRSQL: fix incorrect result with more than 2 JOINs and SELECT with expressions with field names (#4521)
 * OGRSQL: fix 'SELECT MAX(OGR_GEOM_AREA) FROM XXXX' (#4633)
 * OGRSQL: fix invalid conversion from float to integer (#4634)
 * OGRSQL: fix behavior of binary operations when one operand is a NULL value
 * OGRDataSource::CopyLayer(): take into account field renaming by output driver (e.g. Shapefile driver that might truncated field names) (#4667)
 * OGRLayer::FilterGeometry() : speed-up improvement in some cases and  use GEOSPreparedIntersects() when available (r23953, r25268)
 * OGRLayer::SetNextByIndex(): return OGRERR_FAILURE if index < 0
 * OGRLineString::setPoint( int iPoint, OGRPoint * poPoint ) : avoid promoting the line to 25D if the point is only 2D (#4688)
 * OGRGeometry::Centroid(): make it work properly on POINT EMPTY with latest SVN geos version
 * Add reversePoints() method on linestring

OGRSpatialReference:
 * Add support for OGC URLs (#4752)
 * add dumpReadable() method on OGRSpatialReference
 * Differentiation between Hotine Oblique Mercator (aka Variant A) and Oblique Mercator (aka Variant B) (#104, #2745)
 * EPSG: Map methods 1028 and 1029 to normal equidistant cylindrical (#4589)
 * EPSG: add support for EPSG:5514 (Krovak East North)
 * EPSG: Add EPSGTreatsAsNorthingEasting() to deal with CRS with non-GIS friendly northing/easting axis order similarly as what was done with lat/long geographic SRS, and automatically do coord swapping in GML/WFS drivers in that case (#4329)
 * Ozi datum support: support all Ozi datums (#3929), support UTM projection
 * USGS: support USGS angular parameters in radians
 * MorphFromESRI() fix WKT : compare SPHEROID and PRIMEM parameters instead of names (#4673)
 * MorphToESRI(): Add common New Zealand GEOGCS values (#4849)
 * OSR ESRI: add GCS mapping name for ETRS89
 * ogr_srs_esri: Make InitDatumMappingTable() thread-safe
 * ecw_cs.wkt: Add entry for European Terrestrial Reference System 1989.
 * pci_datum.txt: Add entry for D894
 * accept "+proj=XXXX [...] +wktext" as a valid PROJ.4 string, even if projection is unhandled by OGR
 * recognize +proj=etmerc when importing from PROJ.4 string; and output +proj=etmerc when exporting Transverse_Mercator to PROJ.4 string if OSR_USE_ETMERC = YES (#4853)

Utilities:
 * ogr2ogr: add a -dim option to force the coordinate dimension to 2 or 3
 * ogr2ogr: accept -nlt PROMOTE_TO_MULTI to ease conversion from shapefiles to PostGIS, by auto-promoting polygons to multipolygons and linestrings to multilinestrings
 * ogr2ogr: add -gcp, -order n and -tps options to georeference ungeoreferenced vectors (#4604)
 * ogr2ogr: fix -select with shapefile output when specified field name case doesn't match source field name case (#4502)
 * ogr2ogr: correctly deal with filenames that begin with 'polygon' or 'multipolygon' as arguments of -clipsrc (#4590)
 * ogr2ogr: return non-zero exit code if the -sql triggers an error (#4870)
 * ogr2ogr: make -t_srs work when there's no per layer source SRS, but there's per feature SRS
 * ogr2ogr: add a -datelineoffset option  to provide users with capability to set different offsets than 170 to -170 (#4098)
 * ogr2ogr: add a -fieldmap option (#5021)
 * ogr2ogr: automatically rename duplicated field names of source layer so that the target layer has unique field names
 * ogrupdate.py: new sample script to update an OGR datasource from another one, by trying to identify matches between the 2 datasources
 * ogr_layer_algebra.py: new sample script to use OGR layer algebra operations

Multi driver changes:
 * Fix bad AND priority when spatial and attribute filter are combined in PG, MySQL, SQLite MSSQLSpatial and VRT drivers (#4507)

AVCE00 driver:
 * Fix GetFeatureCount() when an attribute or spatial filter is set

AVCBin driver:
 * Fix EOF test (#3031)

CSV driver:
 * Allow creating /vsimem/foo.csv
 * Detect and remove UTF-8 BOM marker if found (#4623)
 * Fix handling of empty column names in header (#4654)
 * Allow creating a new .csv file in a directory where there are invalid .csv
   files (#4824)
 * Use a trick so that the CSV driver creates valid single column files (#4824)
 * Add WRITE_BOM option to CSV driver to write UTF8 BOM for improved Excel/unicode compatibility (#4844)
 * Don't turn \r\n into \n in CSV field output, regardless of LINEFORMAT settings (#4452)
 * Don't left-pad numbers in CSV output when a width is set (#4469)
 * Add support for opening .tsv files, in particular the specific Eurostat .tsv files

DGN driver:
 * Add support to read and write font name in the style (#3392)

DXF driver:
 * Write HATCH (Polygon) compatible with other DXF viewers
 * Write layer geometry extent in file header (#4618)
 * Ignore Spline frame control points for VERTEX of POLYLINE (#4683)
 * Add color to POINT, INSERT and HATCH
 * Take into account extrusion vector to transform from OCS to WCS for MTEXT, TEXT, POINT, LINE, CIRCLE, ELLIPSE, ARC, SPLINE and HATCH (#4842)

FileGDB driver:
 * Add the FGDB_BULK_LOAD configuration option (#4420)
 * Do compulsory field name laundering. (#4458)
 * Add special SQL commands 'GetLayerDefinition a_layer_name' and 'GetLayerMetadata a_layer_name'
 * Implement SetFeature(), DeleteFeature() and DeleteField()
 * Fix inserting features in a layer of geometry type wkbNone
 * Define CLSID/EXTCLSID to fix Feature Class alias being ignored in ArcMap (#4477)
 * Use more sensible default values for tolerance and scale parameters (#4455)
 * Use ESRI SRS DB to find the WKT definition to use when creating a layer (#4838)
 * Fix the way empty geometries are written
 * Add read support for fields of type GlobalID or GUID (#4882)
 * Add XML_DEFINITION layer creation option
 * Support setting fields of type SmallInteger, Float and GUID

Geoconcept driver:
 * Partial support of relaxed GXT export syntax and fix when line is incomplete (#4983)

GeoJSON driver:
 * Fix HTTP HEADERS field send to server (#4546)
 * GeoJSON: write crs object as a FeatureCollection attribute in generated GeoJSON files (#4995); on read, strip AXIS nodes

Geomedia driver:
 * Fix loading of 'boundary' geometries when they are 2.5D or contain more than one inner rings (#4734)

GFT driver:
 * Use OAuth2 authentication (was ClientLogin in previous versions).

GML driver:
 * Add PREFIX and TARGET_NAMESPACE dataset creation options
 * Fix typo when writing geometry type name of MultiLineString in GML3 .xsd (#4674)
 * CreateFeature(): assign spatial ref when it is unset in source feature, even for GML2 case (fixes non-compliance with missing srsName on multi-geometries)
 * Make filtering of features based on OGR_GEOMETRY work (#4428)
 * Avoid point duplication concatenating GML curve segments (#4451)
 * Add special SQL command 'SELECT ValidateSchema()'
 * Allow reading srsDimension attribute when set on LineString element, and not on posList (#4663)
 * Partial support for reading GML 3.3 compat encoding profile, limited to <gmlce:SimplePolygon>, <gmlce:SimpleRectangle>, <gmlce:SimpleTriangle>, <gmlce:SimpleMultiPoint> elements
 * Support WFS GetFeature response document to be piped and opened with /vsistdin/
 * Support specifying connection string as 'filename.gml,xsd=some_filename.xsd' to explicitly provide a XSD
 * Improve detection of extent and srs for WFS 2.0
 * Allow ISO-8859-15 encoded files to be used by Expat parser (#4829)
 * Handle CompositeCurve like MultiCurve (for NAS)
 * Remove duplicate points in rings
 * Add OGR_ARC_MINLENGTH to limit the number of segments in interpolated arcs
 * When there are several geometries per feature, use geometry inside <XX:geometry> element for Inspire compatibility
 * Return per-feature SRS if there's no global SRS for a layer
 * Parse correctly <gml:outerBoundaryIs> when there are attributes in the element (#4934)
 * Recognize <gml:GeodesicString>
 * Recognize <gml:Envelope> elements (#4941)
 * Fix crash when reading CityGML attribute with empty string (#4975)

ILI driver:
 * Support for format codes (#3972)
 * ILI1: Use Topic name from model in itf output
 * ILI1: Recode ISO 8859-1 strings
 * ILI1: Various fixes related to enumerations
 * ILI2: Fix reading with models

Ingres driver:
 * Add effuser and dbpwd connection parameters.
 * Enhance EPSG search bath on WKT AUTH ids.
 * Use system defined sequence for fid instead of global (#4567)
 * Fix a name mixup when creating the table (#4567)

KML driver:
 * Report empty layers when there is only empty layers; don't error out on empty document (#4511)
 * Recognize file whose root element is <Document> and not <kml>

Idrisi driver:
 * Add support for reading attributes from .AVL / .ADC files
 * Ignore nodata value when computing min/max of CreateCopy()'ed dataset
 * Avoid setting unset values of mean and stddev to PAM (#4878)

LIBKML driver:
 * Add stylemap support
 * Add support for GroundOverlay reading (#4738)
 * Fix memory leak in OGRLIBKMLDataSource::FindSchema() when a schema is referenced by name and not by id (#4862)
 * Correct use of temporary variable for schema field names (#4883)
 * Update the layer class internal feature count when a new feature is added

MITAB driver:
 * Fix unwanted changes in data types while converting a datasource to MIF (#3853)
 * Fix incorrect handling of Mollweide projection (#4628)
 * Fix double free in OGRTABDataSource::Create() when exiting with error (#4730)
 * Add datum mapping between EPSG/authority codes and the MapInfo definitions. Falls back to old behavior of name/string matching. (#481)
 * Avoid negative zeros in TOWGS84 read from TAB file (#4931)

MSSQLSpatial driver:
 * Report DeleteFeature/DeleteLayer capabilities
 * Fix the parser to swap coordinates with geography data type (#4642)
 * Fix to read multipoint geometries correctly (#4781)
 * Fix to read 3D geometries correctly (#4806, #4626)
 * Use MSSQL catalog if geometry_columns doesn't exist (#4967)
 * Utilize OGRFieldDefn::IsIgnored() (#4534)
 * Remove requirement for identity fid column in MSSQL tables (#4438)

MySQL:
 * Enable auto reconnect to MySQL (#4819)
 * Reset field width and precision when converting from an unhandled field type to TEXT (#4951)

NAS driver:
 * Better support for wfsext:Replace (#4555)
 * Skip elements "zeigtAufExternes" and "objektkoordinaten" (fixes PostNAS #3 and #15)
 * Issue a warning when geometry is overwritten, invalid geometry is found or featureless geometry appears
 * Issue a debug message when a existing attribute is overwritten (to catch array)
 * Use forceToLineString() on line strings (fixes PostNAS #18)
 * Handle MultiCurve and CompositeCurve
 * Merge multilinestrings
 * Accept XML files that have AAA-Fachschema.xsd in header (and remove trailing whitespace)
 * Add EPSG:25833

NULL driver:
 * No-op output driver for debugging/benchmarking purpose (Not included in build process)

OCI driver:
 * Fix for index creation when layers are created (#4497)

ODBC driver:
 * Allow opening directly *non-spatial* MS Access .MDB databases (on Windows only)
 * Make SetAttributeFilter(NULL) work (#4821)
 * Add optimized GetFeatureCount() implementation

NTF driver:
 * Various fixes when opening ill-formed datasets.

OCI driver:
 * Fix issue with pre-existing tables with laundered names not being recognized (#4966)
 * Make sure the FID counter is correctly initialized when data is appended (#4966)
 * Better quoting of identifiers (#4966)

PGeo driver:
 * PGeo / Geomedia : remove heuristics that tried to identify if a MDB file belonged to the PGeo or Geomedia driver (#4498)

PG driver:
 * Add a 'COLUMN_TYPES' layer creation option (#4788)
 * Fix insertion of features with FID set in COPY mode (#4495)
 * Honour datasource read-only mode in CreateField(), CreateFeature(), SetFeature() and DeleteFeature() (#4620)
 * Avoid fatal error when the public schema is absent (#4611)
 * Differ SRS evaluation on SQL layers (#4644)
 * Optimize SRID fetching on SQL result layer; compatibility with PostGIS 2.0 by using ST_SRID (#4699, #4700)
 * Always fetch the SRS to attach it to feature geometry of result SQL layers.

PGDump driver:
 * Add a 'COLUMN_TYPES' layer creation option (#4788)

REC driver:
 * Add robustness checks against corrupted files

SDE driver:
 * Add support for CLOB and NCLOB data type (#4801)

Shapefile driver:
 * Add support for reading ESRI .sbn spatial index (#4719)
 * Add deferred layer loading
 * Implement auto-growing of string and integer columns
 * Add a special SQL command 'RESIZE table_name' to resize (shrink) fields to their optimum size, also available as a RESIZE=YES layer creation option for convenience
 * Recode field name from UTF-8 to DBF encoding in CreateField()
 * In creation, limit fields of type OFTString to a width of 254 characters (#5052)
 * Spatial index optimization (#4472)
 * Fix GetFeatureCount() when spatial filter set, especially on big-endian hosts (#4491)
 * Fixed wrong return value of OLCStringsAsUTF8 in OGRShapeLayer::TestCapability if GDAL was compiled without iconv support (#4650)
 * Support properly creating layers that include dot character
 * Avoid assert() if SetFeature() is called on a feature with invalid FID (#4727)
 * Correctly deal with .cpg files containing 8859xx string (#4743)
 * Make TestCapability(OLCFastFeatureCount) return TRUE when an attribute filter is set only if attribute indices can be used
 * Shapelib: Fix memory leaks in error code path of DBFCreateLL() (#4860)
 * Deal better with shapefile directories with foo.shp and FOO.DBF, particularly for REPACK support, and particularly for Windows OS (on Linux, foo.shp and FOO.DBF will be 2 different layers)
 * Delete temporary _packed.cpg file generated during REPACK of a layer whose .dbf has an accompanying .cpg file
 * In DeleteDataSource(), delete .cpg file if existing

S57 driver:
 * Various fixes when opening ill-formed datasets.
 * Preliminary support for FFPT/FFPC update records (#5028)
 * Add RECODE_BY_DSS suboption to OGR_S57_OPTIONS configuration option that can be set to YES so that the attribute values are recoded to UTF-8, from the character encoding specified in the S57 DSSI record (#5048, #3421, adapted from patch by julius6)

SQLite/Spatialite driver:
 * Add support for tables with multiple geometry columns (#4768)
 * Add (preliminary) support for SpatiaLite 4.0 database changes (#4784)
 * Make GDAL loadable as a SQLite3 extension (named VirtualOGR) (#4782)
 * Add support for OFTDateTime/OFTDate/OFTTime field types
 * Add a SRID layer creation option
 * Make REGEXP function available by using libpcre (#4823)
 * Add a COMPRESS_COLUMNS layer creation option to make string columns ZLib compressed
 * Implement minimal set of spatial functions if SpatiaLite isn't available
 * SpatiaLite: make use of spatial filter on result layers when the SQL expression is simple enough (no join, etc...) (#4508)
 * SpatiaLite: spatial index can be used even when linking against regular SQLite (#4632)
 * SpatiaLite: to improve performance, defer spatial index creation at layer closing or when a spatial request is done
 * SpatiaLite: use SpatiaLite 'layer_statistics' and 'spatialite_history' to cache the row count and extent of (spatial) layers
 * Spatialite: add support for reading Spatialite 4.0 statistics (filling them on the fly still not implemented)
 * SpatiaLite: for SpatiaLite 4.0, translate INIT_WITH_EPSG=NO into InitSpatialMetaData('NONE') to avoid filling the spatial_ref_sys table
 * On SQL result layers, report the SRS of the geometry of the first feature as the layer SRS
 * Deferred layer definition building for table and view layers
 * Speed-up opening of a result layer that has an ORDER BY
 * Cache GetExtent() result
 * Allow inserting empty feature
 * Return empty layer when SELECT returns 0 rows (#4684)
 * Add compatibility for newer SQLite versions when using the VFS layer (#4783)
 * Add missing column name quoting

TIGER driver:
 * Fix opening TIGER datasource by full file name (#4443)
 * Updated to use VSI*L

VFK driver:
 * SQLite is now a compulsory dependency for VFK
 * Store VFK data in SQLite (in-memory) database
 * Don't read whole file into buffer, but only on request
 * id property of data records in VFK file exceed int limit -> use GUIntBig for id values
 * Fix reading multi-line data records
 * Recode string feature properties - convert from cp-1250 to utf-8
 * Skip invalid VFK features

VRT driver:
 * Add <OGRVRTWarpedLayer> (on-the-fly reprojection of a base layer)
 * Add <OGRVRTUnionLayer> (on-the-fly concatenation of several base layers)
 * Add <FeatureCount>, <ExtentXMin>, <ExtentYMin>, <ExtentXMax>, <ExtentYMax> to <OGRVRTLayer>
 * Add an optional 'dialect' attribute on the <SrcSQL> element
 * Fix OGR VRT sensitive to whitespace and <?xml> nodes (#4582)
 * Optimizations to avoid feature translation when possible
 * Implement GetFIDColumn() (#4637)
 * Implement StartTransaction(), CommitTransaction() and RollbackTransaction() by forwarding to the source layer
 * Auto enable bAttrFilterPassThrough when possible

WFS driver:
 * WFS paging: change default base start index to 0 (was 1 before), as now clarified by OGC (#4504)
 * Accept several type names if the TYPENAME parameter is specified
 * Forward SQL ORDER BY clause as a WFS SORTBY for WFS >= 1.1.0
 * Fix spatial filter with WFS 2.0 GeoServer
 * Check that left-side of a binary operator in an attribute filter is a property name before submitting it to server-side
 * Major overhaul of URL-escaping
 * Fix issues when querying the WFSLayerMetadata and that one of the field contains double-quote characters (#4796)
 * Remove auto-added ACCEPTVERSIONS=1.0.0,1.1.1 - when none of VERSION or ACCEPTVERSIONS were specified - because it does not work with WFS 2.0 only servers
 * Automagically convert MAXFEATURES= to COUNT= if people still (wrongly) used it for WFS 2.0
 * Honour paging when running GetFeatureCount() and that RESULTTYPE=HITS isn't available (e.g. WFS 1.0.0) (#4953)
 * Optimize WFS 1.0 (or WFS 1.1.0 where RESULTTYPE=HITS isn't available) so that in some circumstances the GML stream
   downloaded is used to compute GetFeatureCount() and GetExtent() together
 * Fix segfault on non-GML output when there's SRS axis swapping but the feature has no geometry (#5031)

XLS driver:
 * Set FID to the row number in the spreadsheet software (first row being 1). In case OGR detects a header line, the first feature will then be assigned a FID of 2 (#4586)
 * Non-ascii path support for Windows (#4927)

## SWIG Language Bindings

All bindings:
 * Add VSIReadDirRecursive() (#4658)
 * Add a osr.CreateCoordinateTransformation(src, dst) method (and for Java, a static method CoordinateTransformation.CreateCoordinateTransformation(src, dst) (#4836)
 * Add ogr.ForceToLineString()
 * Clear error before OGR_Dr_Open() (#4955)
 * Add a SetErrorHandler(handler_name) method

CSharp bindings:
 * Add C# signature for FileFromMemBuffer that accepts byte array (#4701)

Java bindings:
 * Fix compilation issue with SWIG 2.0.6 on Java bindings (#4669)
 * New test application: ogrtindex.java
 * Fix values of gdalconst.DCAP_* and gdalconst.DMD_* constants (#4828)
 * Fix check for opaque colors in getIndexColorModel()

Perl bindings:
 * Specify module files to install
 * Return values have to be mortal, this was not the case in many instances.
 * New method Driver::Name aka GetName
 * doc target in GNUmakefile to call doxygen
 * Default to first band in GetRasterBand.
 * New method Geo::OGR::Layer::DataSource
 * New method Geo::OGR::Layer::HasField
 * Geometry method accepts geometries in Perl structures
 * Fixed a bug in FeatureDefn::create which changed the fields.
 * New experimental methods ForFeatures and ForGeometries.
 * InsertFeature, Tuple and Row methods use the Tuple and Row methods from Feature.
 * Do not use pattern my var = value if ...; as it seemingly may cause unexpected things.
   target_key is optional argument.
 * Allow setting geometry type with schema argument.
 * Fix incorrect behavior of Geo::OGR::Geometry method Points in the case of a Point (#4833)
 * Preserve the coordinate dimension in Move method

Python bindings:
 * setup.py: Changes to run without setuptools (#4693)
 * setup.py: Automatically run 2to3 for Python3
 * Define __nonzero__ on Layer object to avoid GetFeatureCount() being called behind our back when doing 'if a_layer:' (#4758)
 * Fix performance problem when instantiating Feature, especially with Python 3
 * Add RasterBand.ReadBlock(), mostly for driver testing
 * Reject strings when array of strings are expected
 * make gdal.PushErrorHandler() also accept a Python error handler function as an argument (#4993)
 * Fix Feature.ExportToJSon() to write the id attribute when it is 0 (the undefined value is NullFID ## -1)

# GDAL/OGR 1.9.0 Release Notes

## In a nutshell...

 * New GDAL drivers: ACE2, CTG, E00GRID, ECRGTOC, GRASSASCIIGrid, GTA, NGSGEOID, SNODAS, WebP, ZMap
 * New OGR drivers:  ARCGEN, CouchDB, DWG, EDIGEO, FileGDB, Geomedia, GFT, IDRISI, MDB, SEGUKOOA, SEGY, SVG, XLS
 * Significantly improved drivers: NetCDF
 * Encoding support for shapefile/dbf (#882)
 * RFC 35: Delete, reorder and alter field definitions of OGR layers
 * RFC 37: Add mechanism to provide user data to CPLErrorHandler (#4295)
 * gdalsrsinfo: new supported utility to report SRS in various form (supersedes testepsg)

## New installed files

 * data/nitf_spec.xml and data/nitf_spec.xsd

## Backward compatibility issues

 * GTiff: ensure false easting/northing in geotiff geokeys are treated as being in geosys units (#3901)
 * GRIB: Fix grid vs cell-center convention (#2637)
 * OGR SQL: with DISTINCT, consider null values are such, and not as empty string (#4353)

## GDAL/OGR 1.9.0 - General Changes

Build(Unix):
 * Add --with-rename-internal-libtiff-symbols and --with-rename-internal-libgeotiff-symbols
   flags in order to safely link against an external libtiff (3.X) and a GDAL built with
   internal libtiff (4.0) support (#4144)
 * Add --with-mdb --with-java,--with-jvm-lib, --with-jvm-lib-add-rpath options
 * Add --with-podofo, --with-podofo-lib, --with-podofo-extra-lib-for-test options
 * Add --with-armadillo
 * Update to libtool 2.4
 * Fix linking against static libkml (#3909)
 * Fix Xerces detection by using LIBS instead of LDFLAGS (#4195)
 * Check for .dylib too, when configuring MrSID SDK paths (#3910)
 * Fix wrong include order in GNUmakefile of GPX and GeoRSS drivers (#3948)
 * cpl_strtod.cpp: Enable android support (#3952).
 * ensure swig-modules depends on lib-target so make -j works with swig bindings
 * Change how we check for GEOS >= 3.1  (#3990)
 * Define SDE64 on at least x86_64 platforms (#4051)
 * Make ./configure --with-rasdaman=yes work (#4349)
 * MinGW cross compilation: clear GEOS_CFLAGS and XERCES_CFLAGS
   if headers found in /usr/include, do not use Unix 64 bit IO
 * MinGW build: define __MSVCRT_VERSION__ to 0x0601 if not already set

Build(Windows):
 * Move MSVC warning disabling to nmake.opt, add SOFTWARNFLAGS for external code
 * Use nmake.local (#3959)
 * cpl_config.h.vc: fix up so it also works with mingw (#3960)
 * Build testepsg utility by default when OGR is enabled (#2554)

## GDAL 1.9.0 - Overview of Changes

Port:
 * /vsigzip/ : Avoid reading beyond file size in case of uncompressed/stored files in zip (#3908)
 * /vsicurl/ : Better support for escaped and UTF-8 characters
 * /vsicurl/ : speed-up with a per-thread Curl connection cache
 * /vsicurl/ : read https directory listing
 * /vsicurl/ : look for GDAL_DISABLE_READDIR_ON_OPEN configuration option in
   Open() and Stat() to avoid trying fetching the directory file list
 * /vsicurl/ : fix performance problem when parsing large directory listings (#4164)
 * /vsicurl/ : recognize listing of Apache 1.3
 * /vsicurl/ : fix ReadDir() after reading a file on the same server
 * /vsicurl/ : fetch more info (size, date) when listing FTP or HTTP directories and save it in cache; use those info for ReadDir() and Stat()
 * /vsicurl/: accept 225 as a valid response code for FTP downloads (#4365)
 * /vsicurl/ : add CPL_VSIL_CURL_ALLOWED_EXTENSIONS configuration option that can be used to restrict files whose existence is going to be tested.
 * /vsitar/ : Recognize additional .tar files with slightly header differences
 * /vsizip/ : wrap the returned file handle in a BufferedReader
 * /vsizip/ : fix 1900 year offset for year returned by VSIStatL()
 * /vsizip and /vsitar: remove leading './' pattern at the beginning of filenames contained in the archive
 * /vsistdout_redirect/ : New virtual file system driver that has the same
   behavior as /vsistdout/ (write-only FS) except it can redirect the output to
   any VSIVirtualFile instead of only stdout (useful for debugging purposes)
 * Implement VSI*L read caching - useful for crappy io environments like Amazon
 * VSI*L: Add Truncate() virtual method and implement it for unix, win32 and /vsimem file systems
 * VSI*L: Add ReadMultiRange() virtual method to read several ranges of data in single call; add an optimized implementation for /vsicurl/
 * VSIFEofL(): make it more POSIX compliant.
 * Fine tune CPLCorrespondingPaths() for different basenames when paths involved.
 * VSIWin32FilesystemHandler::Open() : implement append mode.  Needed by ISIS2 driver with attached label (#3944)
 * CPLString: add case insensitive find operator (ifind)
 * RFC23: Add the iconv() based implementation of the CPLRecode() function (#3950)
 * Preliminary support for wchar_t with iconv recode (#4135)
 * Avoid calling setlocale if we are already in the C locale, or GDAL_DISABLE_CPLLOCALEC is TRUE (#3979)
 * CPLMiniXML: emit warnings when encountering non-conformant XML that is however accepted by the parser
 * add CPLBase64Encode(); move cpl_base64.h contents to cpl_string.h
 * Use CRITICAL_SECTION instead of Mutex on win32
 * CPLHTTPFetch(): Add a CLOSE_PERSISTENT option to close the persistent sessions
 * CPLHTTPFetch(): Add support for "NEGOTIATE" http auth mechanism
 * CPLHTTPFetch(): Add a CUSTOMREQUEST option
 * VSIBufferedReaderHandle: fix Eof()
 * Add CPLStringList class
 * Add CPLEmergencyError() - to call when services are too screwed up for normal error services to work (#4175)
 * CPLEscapeString(,,CPLES_URL) : don't escape dot character; fix escaping of characters whose code >= 128

Core:
 * Provide for ABI specific plugin subdirectories on all platforms
 * Force cleanup of datasets when destroying the dataset manager
 * Add a GDALDataset::CloseDependentDatasets() that can be used by GDALDriverManager::~GDALDriverManager() to safely close remaining opened datasets (#3954)
 * Add GDALRasterBand::ReportError() and GDALDataset::ReportError() to prepend dataset name (and band) before error message (#4242)
 * Fix performance problem when serializing huge color tables, metadata, CategoryNames and GCPs to VRT/PAM (#3961)
 * Be careful about Nan complex values getting histogram, avoid locale issues with statistics metadata
 * GDALRasterBand::IRasterIO() default implementation : don't try to use full-res band if I/O failed on the appropriate overview band (for WMS errors)
 * RasterIO: Return earlier when a write error occurred while flushing dirty block
 * GDAL_DISABLE_READDIR_ON_OPEN can be set to EMPTY_DIR to avoid reading the dir, but it set an empty dir to avoid looking for auxiliary files
 * Use sibling file list to look for .aux.xml, .aux, .ovr, world files, tab files
 * Add GDALFindAssociatedFile() (#4008)
 * PAM: Make sure GCPs loaded from a .aux.xml override any existing ones from other sources, like an .aux file
 * PAM: Add cloning of CategoryNames
 * PAM : PamFindMatchingHistogram() - fix floating-point comparison
 * GMLJP2: Use http://www.opengis.net/gml as the schemaLocation
 * GMLJP2: Support for capturing and writing page resolution in a TIFF compatible way (#3847)
 * GDALJP2Box::SetType() : remove byte-swapping so that SetType()/GetType() correctly round-trips. Do appropriate changes in JP2KAK and ECW drivers. (#4239)
 * GDALReplicateWord(): fix off-by-one error initialization (#4090)

Algorithms:
 * polygonize: Added GDALFPolygonize() as an alternative version of GDALPolygonize() using 32b float buffers instead of int32 ones. (#4005)
 * gdalwarp: take into account memory needed by DstDensity float mask (#4042)
 * rasterfill: create working file as a bigtiff if at all needed (#4088)
 * gdalrasterize: use double instead of float to avoid precision issues (#4292)

Utilities:
 * gdalsrsinfo: new supported utility to report SRS in various form (supersedes testepsg)
 * gdalinfo: add '-nofl' option to only display the first file of the file list
 * gdalinfo: add '-sd num' option to report subdataset with the specified number.
 * gdalinfo: add '-proj4' option to gdalinfo, to report a PROJ.4 string for the CRS
 * gdal_translate: propagate INTERLEAVE metadata to intermediate VRT dataset
 * gdal_translate: force quiet mode when writing to /vsistdout/
 * gdalwarp: Disable CENTER_LONG rewrapping for cutline (#3932)
 * gdalwarp: add -refine_gcps option to discard outliers GCPs before warping (#4143)
 * gdalwarp: add warning if user specifies several of -order, -tps, -rpc or -geoloc options
 * gdalwarp: speed-up when using -tps with large number of GCPs
 * gdalwarp: add support for optional use of libarmadillo to speed-up matrix inversion in -tps mode
 * gdalwarp: detect situations where the user will override the source file
 * gdallocationinfo: do not let one off-db pixel cause all the rest to be suppressed (#4181)
 * gdal_rasterize: fix half pixel shift when rasterizing points; make gdal_rasterize utility increase the computed raster extent by a half-pixel for point layers (#3774)
 * gdal_rasterize: when source datasource has a single layer, use it implicitly if none of -l or -sql is specified
 * nearblack: add -color option (#4085)
 * nearblack: improve detection of collar
 * nearblack: remove useless restrictions on number of bands for -setmask and -setalpha options (#4124)
 * gcps2vec.py: Fix command line parsing; Add SRS definition to created vector layer; Use Point geometry when dumping pixel/line coordinates.
 * gdal_merge.py: add support for -separate with multiband inputs (#4059)
 * gdal_merge.py: add a -a_nodata option (#3981)
 * gdal_proximity.py: -co option existed, but was unused...
 * gdal_fillnodata.py: add -co option
 * Add gdal_ls.py and gdal_cp.py as Python samples
 * Add new sample utility, gdal_edit.py, to edit in place various information of an existing GDAL dataset (projection, geotransform, nodata, metadata) (#4220)
 * gdalcopyproj.py: make it copy GCPs too
 * Add warning if a target filename extension isn't consistent with the output driver
 * Add --pause for convenient debugging, document it and --locale

Multi-driver topics:
 * Implement reading XMP metadata from GIF, JPEG, PNG, GTiff, PDF and the 5 JPEG2000 drivers. The XMP metadata is stored as raw XML content in the xml:XMP metadata domain (#4153)
 * Mark BT, DIPEx, ERS, FAST, GenBIN, GSC, GSBG, GSAG, GS7BG, JDEM, JP2ECW, PNM, RMF, TIL, WCS and WMS drivers as compatible with VSI virtual files
 * Port DOQ1, DOQ2, ELAS, Idrisi, L1B, NDF, NWT_GRD, NWT_GRC, USGSDEM to VSI virtual file API
 * PAM-enable BT and BLX drivers
 * Implement Identify() for AAIGrid, ACE2, DTED, NWT_GRD, NWT_GRC, WMS, WCS, JDEM and BSB drivers
 * Make GIF, JPEG and PNG drivers return a non NULL dataset when writing to /vsistdout/
 * HFA and GTiff: add explicit error message when trying to add external overviews when there are already internal overviews (#4044)
 * Initialize overview manager to support external overviews for AAIGRID, DIPX, ELAS, GXF, FIT, FITS, GMT, GRIB, GSAG, GSBG, GS7BG, ILWIS, L1B, LCP, Leveller, NWT_GRD, NWT_GRC, RIK, SDTS and SAGA

AAIGrid:
 * Make opening from /vsicurl/ work even when the server returns an empty file list

ACE2 driver:
 * New for GDAL/OGR 1.9.0
 * Read ACE2 DEM

AIG driver:
 * Support uncompressed integer files, new in ArcGIS 10 it seems (#4035)
 * Use color table from PAM if no native one (#4021)
 * Fallback to PAM mechanism for RAT (#4021)

BSB driver:
 * Parse the GD keyword in BSB_KNP to recognize European 1950 datum (#4247)
 * fix compilation issues with -DBSB_CREATE

CEOS2 driver:
 * avoid potential crash reading past end of string. (#4065)

CTG driver:
 * New for GDAL/OGR 1.9.0
 * Read USGS LULC Composite Theme Grid files

DIMAP driver:
 * Add support for DIMAP2
 * Check underlying raster for SRS. There are cases where HORIZONTAL_CS_CODE is empty and the underlying raster is georeferenced

E00GRID driver:
 * New for GDAL/OGR 1.9.0
 * Read Arc/Info Export E00 GRID

ECRGTOC driver:
 * New for GDAL/OGR 1.9.0
 * Read TOC.xml file of ECRG products

ECW driver:
 * Use a long refresh time for ecwp:// connections to ensure we get full resolution data, make configurable
 * Re-enable writing non8bit data in jpeg2000
 * Add implementation of an Async reader (4.x SDK)
 * Improve to support all /vsi stuff (#2344)
 * Ensure ECW_ENCODE_ values are applied for direct Create as well as CreateCopy
 * force adfGeoTransform[5] sign to negative. (#393)
 * Mark GDAL_DCAP_VIRTUALIO=YES when the driver is configured in read-only mode
 * Ensure we fallback to native geotransform if no pam override
 * Try to read projection info embedded in ECW file before reading the worldfile (#4046)
 * Add support for updating geotransform and projection info of a ECW file (#4220)
 * Fix ECW_CACHE_MAXMEM that was without effect and ECW_AUTOGEN_J2I that set an unrelated ECW parameter (#4308)
 * Allow to open a ECW file with invalid EPSG code from SWIG bindings (#4187)

EHdr driver:
 * Improve floating point detection (#3933)
 * Recognize MIN_VALUE and MAX_VALUE as found in ETOPO1 header
 * Try opening the .sch file for GTOPO30 or SRTM30 source file
 * Ignore bogus .stx file where min == nodata

EIR driver:
 * Add support for DATA_TYPE keyword

ENVI driver:
 * Add support for ESRI style coordinate system string (#3312)
 * Try to guess interleave mode from file extension, if interleave keyword is missing
 * Refuse to open unsupported types, but attempt to open everything else.

ENVISAT driver:
 * Correct dfGCPLine values for stripline products (#3160, #3709)
 * Fix checking of tie points per column for MERIS GCPs (#4086)
 * Report metadata from the ASAR ADS and GADS in the RECORDS metadata domain (#4105)
 * Read MERIS metadata (#4105)
 * Read data from ERS products in ENVISAT format (#4105)
 * Improved MERIS Level 2 bands detection (#4141 and #4142)

EPSILON driver:
 * Now require libepsilon 0.9.1 to build (now dual LGPL/GPL) (#4084)

ERS driver:
 * Use case insensitive find so case does not matter (#3974)
 * Handle case of 1 m pixel resolution when CellInfo is missing (#4067)
 * Implement ERSRasterBand::SetNoDataValue() (#4207)
 * Add support for DATUM, PROJ and UNITS creation option; report the values read from the .ers file in the ERS metadata domain (#4229)

GeoRaster driver:
 * Set nodata causes invalid XML metadata (#3893)
 * Fix SetStatistics() failure (#4072)
 * Fix default interleaving (#4071)
 * modelCoordinateLocation=CENTER default (#3266)
 * Cache block/level error in update (#4089)
 * Fix sequence.nextval not supported (Oracle 10g) (#4132)
 * change BLOCKING option to OPTIMALPADDING
 * fix 'cannot specify columns on insert create option' (#4206)
 * Fix ULTCoordinate Rows/Columns swapping (#3718)
 * Fix loading of small images, FlushCache issue (#4363)

GIF driver:
 * Make CreateCopy() more friendly with writing in /vsistdout/

GRIB driver:
 * Fix grid vs cell-center convention (#2637)
 * use /vsi for all jpeg2000 files now
 * Fix to allow GFS data to show up properly (#2550)
 * Added a ConfigOption in GRIB driver to not normalize units to metric when reading the data
 * Fixed grib1 & grib2 : pixel size precision introduces error for corner coordinates (#4287)

GTA driver:
 * New for GDAL/OGR 1.9.0
 * Read/write support for Generic Tagged Arrays

GTiff driver:
 * Ensure false easting/northing in geotiff geokeys are treated as being in geosys units.  Add GTIFF_LINEAR_UNITS=BROKEN config option to try and read old broken files, and logic to cover for older libgeotiffs when reading (#3901)
 * Add support for a special tag to keep track of properly written linear units (#3901)
 * Implement deferred directory chain scanning to accelerate simple opens
 * Make GTiff COPY_SRC_OVERVIEWS to deal with unusual source overview sizes (#3905)
 * Fix bug when using -co COPY_SRC_OVERVIEWS=YES on a multiband source with external overviews (#3938)
 * Add logic to fill out partial tiles on write in for jpeg images (#4096)
 * Updated to libtiff 4.0.0 final
 * Refresh with libgeotiff 1.4.0, to support for GeogTOWGS84GeoKey
 * Add support for Geocentric SRS
 * libtiff: Enable DEFER_STRILE_LOAD
 * Turn warning 'ASCII value for tag xx into more silent CPLDebug message
 * Overviews: Improve error reporting for >16bit images to JPEG compression
 * Use CPLAtof() for geotiff and epsg .csv file handling (#3886, #3979)
 * Lots of Imagine and ESRI PE string citation handling changes from 1.8-esri. Some citation related changes only compiled in if ESRI_SPECIFIC defined.
 * Give PAM information precedence over metadata from GeoTIFF itself.   Avoid unnecessary (default) writes of scale/offset.  Treat (0,1,0,0,0,-1) as a default geotransform as well as (0,1,0,0,0,1).
 * Migrate in some ESRI only logic for 1bit color tables, AdjustLinearUnits and default for 1bit data
 * Add a GTIFF_IGNORE_READ_ERRORS configuration option (#3994)
 * Lazy loading of RPC/RPB/IMD files (#3996)
 * Add mutex protection in GTiffOneTimeInit() to avoid occasional segfaults
 * Stop interpreting 4th band as alpha when not defined
 * Also list nSubType == FILETYPE_PAGE as subdatasets
 * CreateCopy(): copies category names from the source bands
 * Add capability of writing TIFFTAG_MINSAMPLEVALUE and TIFFTAG_MAXSAMPLEVALUE
 * Don't prevent from loading GTiff driver even if libtiff version mismatch detected (#4101)
 * Use GTIFF_ESRI_CITATION flag to disable writing special meaning ESRI citations
 * Optimize GTiffRGBABand implementation (#3476)
 * Add GTIFF_DIRECT_IO config. option that can be set to YES so that IRasterIO() reads directly bytes from the file using ReadMultiRange().
 * Use VSI_TIFFOpen() in GTIFFBuildOverviews() to make it work on virtual file systems
 * Treat _UNASSALPHA as alpha

GRASSASCIIGrid driver:
 * New for GDAL/OGR 1.9.0
 * Read GRASS ASCII grids (similar to ArcInfo ASCII grids)

GRIB driver:
 * Check for memory allocation failures

HDF4 driver:
 * Use larger object name buffer
 * Handle SWopen failures

HDF5 driver:
 * Fix HDF5/BAG handle/memory leaks (#3953)
 * Better error checking
 * Do not return NULL from getprojectionref() (#4076)
 * Identify datasets whose header starts with some XML content (#4196)
 * Fixed HDF5 variable length string attributes reading (#4228)

HFA driver:
 * Add support for writing RATs (#999)
 * Add support for reading 2bit compressed .img files (#3956)
 * Update EPRJ_ list based on input from Erdas, round trip Krovak and Mercator Variant A (#3958)
 * Major push to move projections from 1.6-esri into trunk in HFA driver (#3958)
 * Reinitialize RRDNamesList and ExternalRasterDMS (#3897)
 * Ensure the whole entry is reinitialized when writing proparams and datum (#3969)
 * Ensure PEString cleared if we aren't writing it (#3969)
 * Get nodata from an overview if not present on main band.  Set geotransform[1] and [5] to 1.0 if pixelSize.width/height are 0.0.  Improve error checking if MakeData() fails.
 * atof() changed to CPLAtofM() to avoid locale issues.
 * Altered Edsc_BinFunction column "Value" to "BinValues" and changed type.
 * Equirectangular StdParallel1 changed to LatitudeOfOrigin.
 * Logic to preserve psMapInfo->proName as the PROJCS name for UTM/StatePlane.
 * Special state plane zone handling.
 * Special wisconsin handling for some LCC and TM SRSes.
 * 1-bit null blocks to default to 1 in ESRI_BUILDs
 * Add support for GDA94 (#4025)

Idrisi driver:
 * Fix segfaults when fields are missing in .ref or .rdc files (#4100)
 * Fix problem with inverse flattening when reading a SRS of a sphere (#3757)

INGR driver:
 * Set NBITS for 1 bit bands

ISIS2/ISIS3 driver:
 * Various improvements to PDS related drivers (#3944)

JaxaPalsar driver:
 * Fixed datatype of ALOS PALSAR products Level 1.5 (#4136)
 * Fixed detection of unsupported PALSAR Level 1.0 products (#2234)

JPIPKAK driver:
 * Add try to in GetNextUpdatedRegion() to protect against kakadu exceptions (#3967)
 * Fixed a serious bug in the computation of fsiz and region. (#3967)

KMLSUPEROVERLAY driver :
 * Remove spaces between coordinates in coordinate triplets as mandated by KML 2.2 spec, to restore compatibility with Google Earth 6.1.0.5001 (#4347)

LAN driver:
 * Preliminary support for writing gis/lan files

MEM driver:
 * Add support for remembered histograms and PIXELTYPE

MG4Lidar driver:
 * Clamp nOverviewCount, some LiDAR files end up with -1 overviews

MrSID driver:
 * Initialize overview manager to enable RFC 15 mask band support (#3968)
 * Mark GDAL_DCAP_VIRTUALIO=YES when the driver is configured in read-only mode
 * Handle LTI_COLORSPACE_RGBA

NetCDF driver:
 * Set cylindrical equal area representation to the proper cf-1.x notation (#3425)
 * Fix precision issue in geotransform (#4200) and metadata
 * Add support for netcdf filetypes nc2(64-bit) and nc4 to netCDFDataset (#3890, #2379)
 * Add function Identify and IdentifyFileType() (#3890, #2379)
 * Temporarily disabling PAM for netcdf driver (#4244)
 * Make creation of geographic grid CF compliant (#2129)
 * Fixes for netcdf metadata export: duplication, Band metadata, int/float/double vs. char* and add_offset/scale_factor (#4211, #4204), double precision ( 4200)
 * Fix netcdf metadata import (float and double precision) (#4211)
 * Improve import of CF projection
 * Add netcdf history metadata (#4297)
 * CF-1.5 compatible export of projected grids (optional long/lat export)
 * Fix LCC-1SP import and export (#3324)
 * Fix handling of UNITS import and export (#4402 and #3324)
 * Fix upside-down export and import of grids without projection and geotransform (#2129, #4284)
 * Support import of polar stereographic variant without standard parallel (#2893)
 * New driver options
 * Add simple progress indicator
 * Add support for netcdf-4, HDF4 and HDF5 (#4294 and #3166)
 * Add support for deflate compression
 * Add format support information and CreateOptionList to driver metadata
 * Add support for valid_range/valid_min/valid_max
 * Proper handling of signed/unsigned byte data
 * Add support for Create() function and significantly refactor code for export (#4221)
 * Improvements to CF projection support (see wiki:NetCDF_ProjectionTestingStatus)

NGSGEOID driver:
 * New for GDAL/OGR 1.9.0
 * Read NOAA NGS Geoid Height Grids

NITF driver:
 * Add a generic way of decoding TREs from a XML description file located in data/nitf_spec.xml
 * Add a new metadata domain xml:TRE to report as XML content the decoded TREs
 * Add NITF_OPEN_UNDERLYING_DS configuration option that can be set to FALSE to avoid opening the underlying image with the J2K/JPEG drivers
 * Support JP2KAK driver for jpeg2000 output, use /vsisubfile/ in all cases
 * NITFCreate(): deal with cases where image_height = block_height > 8192 or image_width = block_width > 8192 (#3922)
 * Add IREPBAND and ISUBCAT creation option (#4343)
 * Make sure scanline access is used only on single block image (#3926)
 * Add a NITF_DISABLE_RPF_LOCATION_TABLE_SANITY_TESTS configuration option that can be set to TRUE to blindly trust the RPF location table (#3930)
 * Correctly assign hemisphere for a ICORDS='U' NITF file with accompanying .nfw and .hdr files (#3931)
 * Make PAM available at band level for JPEG/JPEG2000 compressed datasets (#3985)
 * Read IMRFCA TRE for RPC info. Read CSEXRA TRE.
 * Read CSDIDA and PIAIMC TREs as metadata
 * Optional support for densifying GCPs and applying RPCs to them
 * Add GetFileList() that captures associated files with some NITF products.
 * Added ESRI only ExtractEsriMD() function.  Add raw header capture in NITF_METADATA domain in base64 encoded form.
 * Fetch NITF_DESDATA in segment data; decode specialized fields of XML_DATA_CONTENT and CSATTA DES
 * Truncate TRE name to 6 character (#4324)
 * Take into account the presence of comments when patching COMRAT for JPEG/JPEG2000 NITF (#4371)

NWT_GRD driver:
  * Fix interpolation of color when the maximum z value is below a threshold of the color scheme (#4395)

OPENJPEG driver:
 * Optimize decoding of big images made of a single block
 * Fallback to PAM to get projection and geotransform

PCIDSK driver:
 * Refresh PCIDSK SDK from upstream
 * Fix support for band description setting, add BANDDESCn creation option
 * Implement GetCategoryNames(), and color table from metadata for PCIDSK
 * Fix exception on files with bitmaps as bands in GetFileList()
 * Avoid closing and reopening file so we don't fracture the SysBMData with a metadata write
 * In read-only, if .pix is raster (resp. vector) only, then make sure that OGR (resp. GDAL) cannot open it

PDF driver:
 * Support linking against podofo library (LGPL) instead of poppler --> however
   pdftoppm binary from poppler distribution is needed for rasterization

PDS driver:
 * Add support for MISSING and MISSING_CONSTANT keywords for nodata values (#3939)
 * Add support for uncompressed images in the UNCOMPRESSED_FILE subdomain (#3943)
 * Add support for PDS_Sample/LineProjectOffset_Shift/Mult (#3940)
 * Preliminary qube write support (#3944)
 * Fix band offset computation in BSQ (#4368)

PNG driver:
 * Add compatibility with libpng >= 1.5.0 (#3914)
 * Upgrade internal libpng to 1.2.46

PNM driver:
 * Make it compatible with VSI virtual files

PostgisRaster driver:
 * Speed of PostGIS Raster driver improved. (#3228, #3233)

Rasterlite driver:
 * Robustness against buggy databases
 * Enable QUALITY creation option for WEBP tiles

RS2 driver:
 * Setup to properly support subdataset oriented metadata and overviews (#4006)
 * Allow opening subdatasets by passing in the folder (#4387)

SAGA driver:
 * Fix reading & writing .sdat files bigger than 2GB (#4104)
 * Use nodata value from source dataset in CreateCopy() (#4152)

SDE driver:
 * Break assumption that LONG==long (#4051)

SNODAS driver:
 * New for GDAL/OGR 1.9.0
 * Read Snow Data Assimilation System datasets

SRP driver:
 * Set Azimuthal Equidistant projection/geotransform info for ASRP north/south polar zones (#3946)
 * ASRP/USRP: fix skipping of padding characters on some datasets (#4254)

SRTMHGT driver:
 * Fix segfault in CreateCopy() if we cannot create the output file

Terralib driver:
 * Removed driver: was unfinished and is unmaintained (#3288)

TIL driver:
 * Implement GetFileList() (#4008)

TSX driver:
 * Add support for selecting a directory
 * Make Terrasar-X driver also open TanDEM-X data (#4390)
 * Fix memleaks

USGSDEM driver:
 * Ensure blocks read in C locale (#3886)

VRT driver:
 * Implement VRTDataset::IRasterIO() that can delegate to source Dataset::RasterIO() in particular cases
 * Implement GetMinimum() and GetMaximum()
 * GetFileList(): for /vsicurl/ resources, don't actually test their existence as it can be excruciating slow
 * VRTComplexSource: correctly deal with complex data type (#3977)
 * Fix 2 segfaults related to using '<VRTDataset', but with invalid XML, as the target filename of VRTDataset::Create()
 * Fix 'VRTDerivedRasterBand with ComplexSource and nodata value yields potentially uninitialized buffer' (#4045)
 * VRTDerivedRasterBand: Recognize PixelFunctionType and SourceTransferType options in AddBand() for  (#3925)
 * Copy GEOLOCATION metadata in CreateCopy().
 * VRTDerivedRasterBand: register pixel functions in a map for faster access (#3924)
 * VRT warped dataset: limit block size to dataset dimensions (#4137)

WCS driver:
 * Add time support (#3449)
 * Honour dimensionLimit restrictions on WCS request size.
 * Fetch projection from returned image file chunks if they have them (i.e. GeoTIFF).
 * Honour Resample option for WCS 1.0.
 * Include service url in GetFileList if ESRI_BUILD defined
 * Check validity of 'OverviewCount' parameter
 * Add support for getting the coverage offering details from the xml:CoverageOffering domain
 * Try to preserve the servers name for a CRS (WCS 1.0.0) (#3449).

WebP driver:
 * New for GDAL/OGR 1.9.0
 * Read/write GDAL driver for WebP image format

WMS driver:
 * Implementation of the OnEarth Tiled WMS minidriver (#3493)
 * Implementation of a VirtualEarth minidriver
 * Improve handling of .aux.xml files, ensure colorinterp support works for tiled wms server
 * Report subdatasets when being provided WMS:http://server.url (classic WMS), WMS:http://server.url?request=GetTileService or a url to a TMS server; recognize datasets specified as a pseudo GetMap request
 * Add capability to open the URL of a REST definition for a ArcGIS MapServer, like http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer?f=json
 * Parse WMS-C TileSet info in VendorSpecificCapabilities of GetCapabilities
 * Implement CreateCopy() when source is a WMS dataset to serialize to disk the definition of a WMS dataset
 * WMS dataset : report INTERLEAVE=PIXEL
 * Make autodetection of TMS work with http://maps.qualitystreetmap.org/tilecache
 * Add capability to set Referer http header
 * Add a TMS-specific hack for some servers that require tile numbers to have exactly 3 characters (#3997)
 * Fix to make file:// URL to work
 * Add hack for OSGEO:41001
 * Fix GDALWMSRasterBand::IReadBlock() to avoid the ReadBlocks() optimization to become an anti-optimization in some use cases
 * Add service=WMS parameter if not already provided in <ServerUrl> (#4080)
 * Add options, ZeroBlockHttpCodes and ZeroBlockOnServerException, to control which http error codes should be considered as meaning blank tile (#4169)

ZMap driver:
 * New for GDAL/OGR 1.9.0
 * Read/write GDAL driver for ZMap Plus Grid format

## OGR 1.9.0 - Overview of Changes

Core:
 * RFC35: Add OGRLayer::DeleteField(), ReorderField(), ReorderFields() and AlterFieldDefn()
 * Avoid OGRLineString::addPoint( OGRPoint * poPoint ) to always force the geometry to be 3D (#3907)
 * Add a OGREnvelope3D object and getEnvelope( OGREnvelope3D * psEnvelope ) / OGR_G_GetEnvelope3D() method
 * Add OGR_G_SimplifyPreserveTopology() / OGRGeometry::SimplifyPreserveTopology()
 * OGR SQL: recognize optional ESCAPE escape_char clause
 * OGR SQL: allow NULL to be used as a value, so that 'SELECT *, NULL FROM foo works'
 * OGR SQL: Accept doublequoting of column_name in 'SELECT DISTINCT "column_name" FROM table_name' (#3966)
 * OGR SQL: OGRGenSQLResultsLayer: if the dialect is explicitly set to OGRSQL, don't propagate the WHERE clause of the SELECT to the source layer, but evaluate it instead at the OGRGenSQLResultsLayer level (#4022)
 * OGR SQL: Avoid error emission on requests such as 'SELECT MIN(EAS_ID), COUNT(*) FROM POLY'
 * OGR SQL: Avoid setting width/precision for AVG column
 * OGR SQL: Add a mechanism to delete a layer (DROP TABLE x)
 * OGR SQL: fix segfault when evaluating a 'IS NULL' on a float column (#4091)
 * OGR SQL: add support for new special commands : 'ALTER TABLE layername ADD COLUMN columnname columntype', 'ALTER TABLE layername RENAME COLUMN oldname TO new name', 'ALTER TABLE layername ALTER COLUMN columnname TYPE columntype', 'ALTER TABLE layername DROP COLUMN columnname'
 * OGR SQL: Add implicit conversion from string to numeric (#4259)
 * OGR SQL: Correctly parse big SQL statements (#4262)
 * OGR SQL: fix joining a float column with a string column (#4321)
 * OGR SQL: with DISTINCT, consider null values are such, and not as empty string (#4353)
 * OGR SQL: fix offset conversion for SUBSTR() (#4348)
 * Add OGR_G_GetPoints()
 * Fix parsing of WKT geometries mixing 2D and 3D parts
 * OGR_Dr_CopyDataSource() and OGRSFDriver::CopyDataSource() : make sure that the driver is attached to the created datasource (#4350)
 * OGRFeature::SetFrom() supports more conversions between field types.

OGRSpatialReference:
 * Update to EPSG 7.9 database
 * Add Geocentric SRS Support
 * Add support for Interrupted Goode Homolosine projection (#4060)
 * Add SRS vertical unit support
 * Add SetVertCS(), OSRSetVertCS(), SetCompound(), IsCompound() and target oriented set/get linear units functions
 * ESRI : Improve spheroid remapping (#3904)
 * ESRI: Compare whole names in RemapNameBasedOnKeyName() (#3965).
 * ESRI: addition of ImportFromESRIStatePlaneWKT and ImportfromESRIWisconsinWKT methods
 * ESRI: importFromESRI() : support POLYCONIC projection from old style files (#3983)
 * ESRI: importFromESRI() : support LAMBERT_AZIMUTHAL projection from old style files (#4302)
 * ESRI: fix EPSG:32161 mapping
 * ESRI: fix Stereo/Oblique_Stereo/Double_Stereo (bugs #1428 and #4267)
 * ESRI: fix projection parameter mapping for Orthographic projection (#4249)
 * ESRI: add optional fixing of TOWGS84, DATUM and GEOGCS with GDAL_FIX_ESRI_WKT config. option (#4345 and #4378)
 * ESRI: fix add Krassowsky/Krasovsky 1940 spheroid mapping
 * Add EPSG:102113 in the data/esri_extra.wkt file
 * Add Germany zone 1-5 in range 31491-31495 in the data/esri_extra.wkt file
 * fix NAD_1983_Oregon_Statewide_Lambert_Feet_Intl code
 * added/updated coordinates systems provided by IGNF (#3868)
 * ERM: add support for EPSG:n based coordinate systems (#3955)
 * ImportFromEPSG(): Add default support for various degree units without supporting .csv file.
 * ImportFromEPSG(): Add support for spherical LAEA (#3828)
 * ImportFromEPSG(): use CoLatConeAxis parameter to build Krovak azimuth parameter (#4223)
 * importFromURN(): support compound SRS
 * importFromURN(): accept 'urn:opengis:crs:' syntax found in some TOP10NL GML files
 * Add CRS: support and for importing two part ESRI PE SRS with VERTCS
 * SetFromUserInput() : recognize 'IGNF:xxx'
 * Ensure that the result of importFromEPSGA() always has keyword ordering fixed up (#4178)
 * exportToERM() : deal with GDA94 datum and its UTM projections (#4208)
 * Fix ESRI_DATUM_NAME for D_MOLDREF99 and D_Philippine_Reference_System_1992 (#4378)

Utilities:
 * ogr2ogr: Make 'ogr2ogr someDirThatDoesNotExist.shp dataSourceWithMultipleLayer' create a directory
 * ogr2ogr: make -overwrite/-append work with non-spatial tables created by GDAL 1.8.0;
 * ogr2ogr: take into account fields specified in the -where clause in combinations with -select to create the correct list of fields to pass to SetIgnoredFields() (#4015)
 * ogr2ogr: fix -zfield option so that the modified geometry properly reports coordinate dimension = 3. Also avoids it to be in the list of ignored field names
 * ogr2ogr: add -simplify option to simplify geometries
 * ogr2ogr: add a warning if the target filename has an extension or a prefix that isn't consistent with the default output format (i.e. Shapefile), but which matches another driver. Can be made quiet with -q option
 * ogrinfo/ogr2ogr: exit when SetAttributeFilter() fails, instead of silently going on (#4261)

Multi driver topics:
 * RFC35 : implementation in Shapefile, Memory and PG drivers (#2671)
 * DXF, EDIGEO, KML, LIBKML, Shapefile, SDE, SOSI: Mark as supporting UTF-8
 * BNA, CSV, GPX, KML, GeoRSS, GML, LIBKML, GeoJSON, PGDump : accept both /dev/stdout and /vsistdout/ as filenames; remove 'stdout' as a valid alias that could be used in some of them (#4225, #4226)

ARCGEN driver:
 * New for GDAL/OGR 1.9.0
 * Read-only OGR driver for Arc/Info Generate files

CouchDB driver:
 * New for GDAL/OGR 1.9.0
 * Read/write OGR driver for CouchDB / GeoCouch

CSV driver:
 * Add special recognition and handling for USGS GNIS (Geographic Names Information System) files
 * Directly recognize the structure of the allCountries file from GeoNames.org
 * Implement GetFeatureCount() to be a little bit faster
 * Accept /dev/stdout as a filename for CreateDataSource()
 * Fix handling of non-numeric values in numeric columns (NULL instead of 0)
 * Fix handling of column names with numbers
 * Recognize numeric fieldnames inside quotes
 * Accept real numbers with ',' as decimal separator when ';' is the field separator (CSV export in French locale)

DXF driver:
 * Add support for DXF_ENCODING config var and DWGCODEPAGE header field (#4008)
 * Added DXF_MERGE_BLOCK_GEOMETRIES
 * Treat ATTDEFs the same as TEXT entities
 * Implement hatch polyline and elliptical arc support, hatch fill, do not polygonize closed smoothed lines
 * Add handling of hidden/frozen/off layers.

DWG driver:
 * New for GDAL/OGR 1.9.0
 * Read DWG files through the use of Open Design Alliance Teigha Libraries

EDIGEO driver:
 * New for GDAL/OGR 1.9.0
 * Read files of French EDIGEO exchange format

FileGDB driver:
 * New for GDAL/OGR 1.9.0
 * Read/write support based on FileGDB API SDK

GeoJSON driver:
 * Support writing 3D lines and polygons
 * Add a bbox attribute with the geometry bounding box if WRITE_BBOX layer creation option is set (#2392)
 * Write bbox of FeatureCollection before features when file is seekable
 * Remove unsetting of FID that caused FID not at the last position of properties to be lost
 * Properly deal with null field values in reading and writing
 * Handle OFTIntegerList, OFTRealList and OFTStringList fields
 * Recognize other arrays as OFTString field
 * Fix assertion on unhandled ESRI json (#4056)
 * Fix segfault on feature where 'properties' member exists but isn't an object (#4057)
 * Better detection of OGR type for numeric JSON fields (#4082)
 * Add COORDINATE_PRECISION layer creation option to specify the maximum number of figures after decimal point in coordinates; set to 15 by default with smart truncation of trailing zeros (like done for WKT)
 * Add OGR_G_ExportToJsonEx() to accept a list of options
 * Add ability to detect geojson files without an extension (#4314)

Geomedia driver:
 * New for GDAL/OGR 1.9.0
 * Read-only driver to read Geomedia .MDB databases

GeoRSS driver:
 * Parse RSS documents without <channel> element

GFT driver:
 * New for GDAL/OGR 1.9.0
 * Read/write driver for Google Fusion Tables

GML driver:
 * Major performance improvement when reading large multi-layer GML files. See usage of new GML_READ_MODE configuration option
 * Support gml:xlink resolving for huge GML files through GML_SKIP_RESOLVE_ELEMS=HUGE (requires SQLite)
 * Add GML_GFS_TEMPLATE config option to specify a template .gfs file that can be used for several GML files with similar structure (#4380)
 * Be able to build the driver with support of both Expat and Xerces libraries and add ability of select one at runtime. For UTF-8 documents, we select Expat if it is available, because it is faster than Xerces
 * Expose fid or gml_id as feature fields if autodetected. This behavior can be altered by the GML_EXPOSE_FID / GML_EXPOSE_GML_ID configuration option.
 * Improve handling of .gml and .xsd produced by FME (in particular for CanVec GML)
 * Be able to open .gz file directly (like OS Mastermap ones), and read/write the .gfs file next to the .gz file
 * Fix segfault when encountering an invalid (or unhandled by OGR) geometry and when the axis order is lat/long (#3935)
 * GML3: use a new method to interpret Face objects (which requires GEOS support);
         old method available if GML_FACE_HOLE_NEGATIVE config. option set to YES (#3937)
 * GML3: support Curve as a valid child for curveProperty inside directEdge parsing (#3934)
 * GML3: don't force the linestring to be 3D when inverting its orientation during parsing of directedEdge (#3936)
 * GML3: accept <pointProperty> element in <gml:LineString> or <gml:LineStringSegment>
 * OGR_G_CreateFromGML(): accept <gml:coordinates> with coordinate tuples separated by comma and coordinate components separated by space
 * Recognized schemas with <complexType> inside <element>, such as the one returned by http://deegree3-demo.deegree.org:80/deegree-utah-demo/services
 * Write the Z component of bounding box for 25D geometries
 * Force layer geometry type to 3D when there's only a .xsd file and we detect a hint that the bounding box is 3D
 * Handle layers of type wkbNone appropriately (#4154)
 * Change format of (GML 2.1.1) FID generated from Fxxx to layer_name.xxx (where xxx is the OGR FID) to ensure uniqueness (#4250)
 * Accept 'GML3Deegree' as a valid value for the dataset creation option FORMAT, to produce a .XSD that should be better handled by Deegree3 (#4252), and 'GML3.2' to produce GML file and schema that validate against GML 3.2.1 schema.
 * Don't try to parse successfully a feature type in the .xsd if there are elements we don't know how to parse. Better to rely on the .gfs mechanism (#4328)
 * Fix bug in OGRAtof() that caused wrong parsing of coordinates in GML files written in scientific notation (#4399)

GMT driver:
 * Fix GetExtent() result that swallowed the first char of the minx bound (#4260)

IDRISI driver:
 * New for GDAL/OGR 1.9.0
 * Read Idrisi .VCT vector files

ILI1 driver:
 * Fix for missing geometry in ILI1

LIBKML driver:
 * Set the OGRStylePen unit type to pixel when reading <LineStyle>
 * Avoid ingesting zip files that are not valid kmz (#4003)
 * Do not use displayname to set the field name
 * Recognize <Data> elements of <ExtendedData> in case <ExtendedData> doesn't use a <SchemaData>
 * Fix mapping of the type attribute of <SimpleType> elements inside <Schema> to OGR field type (#4171)
 * Parse correctly kml docs containing only one placemark
 * Properly set the feature style string from a placemarks style
 * Improve OGRStyleLabel <-> KmlLabelStyle mapping
 * Combine styles from the style table and features styles when LIBKML_RESOLVE_STYLE=YES (#4231)
 * Check that string values put in fields are valid UTF-8 (#4300)

MDB driver:
 * New for GDAL/OGR 1.9.0
 * Read-only driver to read PGeo and Geomedia .MDB databases
 * Relies on using the Java Jackcess library (LGPL) through JNI.

MITAB driver:
 * Add support for reading google mercator from mapinfo (#4115)
 * Fixed problem of the null datetime values (#4150)
 * Fix problem with tab delimiter used in MIF files (#4257)

MSSQLSpatial driver:
 * Removing 'Initial Catalog' which is not supported in the ODBC SQL driver connection strings.
 * Allow to specify 'Driver' in MSSQL connection strings (#4393)
 * Fix for the IDENTITY INSERT problem with MSSQL Spatial (#3992)
 * Add more verbose warnings to the geometry validator
 * Fix for the schema handling problem with MSSQL Spatial (#3951)
 * Fix for the corrupt geometry report when using the ogr2ogr -sql option (#4149)

MySQL driver:
 * Recognize columns with types POINT, LINESTRING, etc. as geometry columns

NAS driver:
 * Add support for treating wfs:Delete as a special Delete feature with typeName and FeatureId properties
 * Handle empty files gracefully (#3809)
 * Preliminary support for SRS in NAS files, including 3GKn SRS
 * Implement special treatment for <lage> to be zero passed and string (NAS #9)
 * Add special handling of punktkennung (NAS #12)
 * Add special handling for artDerFlurstuecksgrenze (#4255)
 * Add support for wfsext:Replace operations (PostNAS #11)
 * Correct NASHandler::dataHandler() to avoid trimming non-leading white space

NTF driver:
 * Create and manage height field as floating point since some DTM products have floating point elevations.

OCI driver:
 * Added TRUNCATE layer creation option (#4000)
 * Clear errors after speculative dimension calls (#4001)
 * Fix multithreading related problems (#4039)
 * Ensure that AllocAndBindForWrite does not mess up if there are no general attributes (#4063)
 * Implement DeleteLayer(int) method
 * Ensure extents updated by SyncToDisk(), and that new features are merged into existing extents (#4079)

OGDI driver:
 * Fix GetFeature() that did not like switching between layers

PG driver:
 * Write geometries as EWKB by default to avoid precision loss (#4138)
 * Return the table columns in the order they are in the database (#4194)
 * Add a NONE_AS_UNKNOWN layer creation option that can be set to TRUE to force layers with geom type = wkbNone to be created as if it was wkbUnknown (PostGIS GEOMETRY type) to be able to revert to behavior prior to GDAL 1.8.0 (#4012)
 * Add EXTRACT_SCHEMA_FROM_LAYER_NAME layer creation option that can be set to OFF to disable analysis of layer name as schema_name.table_name
 * Add FID layer creation option to specify the name of the FID column
 * ogr2ogr: make sure that for a PG datasource, -nln public.XXX can also be used with -append
 * Fix CreateFeatureViaInsert() to emit 'INSERT INTO xx DEFAULT VALUES'
 * Fix handling of Nan with fields with non-zero width (#2112)
 * Use wrapper for PQexec() to use PQexecParams() instead in most cases
 * Add proper escaping of table and column names
 * OGR SQL: add proper column name escaping and quoting for PostgreSQL datasources
 * Launder single quote character in table name
 * Better reporting of error in case of failed ExecuteSQL()
 * Create field of type OFTString and width > 0, as VARCHAR(width) (#4202)
 * Add more compat with Postgis 2.0SVN (geometry_columns- #4217, unknown SRID handling)
 * Better behavior, in particular in error reporting, of ExecuteSQL() when passed with non-select statements, or with select statements that have side-effects such as AddGeometryColumn()

PGDump driver:
 * fix handling of Nan with fields with non-zero width (#2112)
 * Add CREATE_SCHEMA and DROP_TABLE layer creation option (#4033)
 * Fix crash when inserting a feature with a geometry in a layer with a geom type of wkbNone;
 * PG and PGDump: fix insertion of features with first field being a 0-character string in a non-spatial table and without FID in COPY mode (#4040)
 * Add NONE_AS_UNKNOWN, FID, EXTRACT_SCHEMA_FROM_LAYER_NAME layer creation options
 * Better escaping of column and table names
 * Create field of type OFTString and width > 0, as VARCHAR(width) (#4202)

PGeo driver:
 * Move CreateFromShapeBin() method to an upper level
 * Only try to open .mdb files that have the GDB_GeomColumns string
 * Decode Z coordinate for a POINTZM shape
 * Aad support for decoding multipoint/multipointz geometries
 * Fix setting of the layer geometry type
 * Add support for zlib compressed streams
 * Implement MultiPatch decoding

SDE driver:
 * Add support for decoding NSTRING fields (#4053)
 * Add support in CreateLayer() to clean up partially registered tables that aren't full spatial layers
 * Add logic to force envelope for geographic coordsys objects (#4054)
 * Add USE_STRING layer creation and configuration options information
 * Set SE_MULTIPART_TYPE_MASK for multipolygon layers (#4061).
 * Change how offset and precision are set for geographic coordinate systems to more closely match SDE

SEGUKOOA driver:
 * New for GDAL/OGR 1.9.0
 * Read files in SEG-P1 and UKOOA P1/90 formats

SEGY driver:
 * New for GDAL/OGR 1.9.0
 * Read files in SEG-Y format

Shapefile driver:
 * Encoding support for shapefile/dbf (#882)
 * Allow managing datasources with several hundreds of layers (#4306)
 * Lazy loading of SRS and lazy initialization of attribute index support
 * Use VSI*L API to access .qix spatial index files
 * Add special SQL command 'RECOMPUTE EXTENT ON layer_name' to force recomputation of the layer extent (#4027)
 * Faster implementation of GetFeatureCount() in some circumstances.
 * Fix crash in CreateField() if there is no DBF file
 * Fix add field record flushing fix (#4073)
 * Fix decoding of triangle fan in a multipatch made of several parts (#4081)
 * Refuse to open a .shp in update mode if the matching .dbf exists but cannot be opened in update mode too (#4095)
 * Recognize blank values for Date fields as null values (#4265)
 * Recognize 'NULL' as a valid value for SHPT creation option as documented
 * Check that we are not trying to add too many fields.
 * Support reading measure values as Z coordinate.

SQLite/Spatialite driver:
 * Spatialite: major write support improvements (creation/update of Spatialite DB now limited to GDAL builds with libspatialite linking)
 * Spatialite: add support for 3D geometries (#4092)
 * Spatialite: speed-up spatial filter on table layers by using spatial index table (#4212)
 * Spatialite: add support for reading Spatialite views registered in the views_geometry_columns
 * Spatialite: better support for building against amalgamated or not
 * Spatialite: when it exists, use srs_wkt column in spatial_ref_sys when retrieving/inserting SRS
 * Spatialite: add COMPRESS_GEOM=YES layer creation option to generate Spatialite compressed geometries
 * Spatialite: add support for VirtualXLS layers.
 * Spatialite: imported VirtualShape support, in particular it is now possible to open on-the-fly a shapefile as a VirtualShape with 'VirtualShape:shapefile.shp' syntax as a datasource
 * Implement RFC35 (DeleteField, AlterFieldDefn, ReorderFields)
 * Implement DeleteDataSource()
 * Implement DeleteFeature()
 * Implement SetFeature() by using UPDATE instead of DELETE / INSERT
 * Add capability to use VSI Virtual File API when needed (if SQLite >= 3.6.0)
 * Make CreateDataSource(':memory:') work
 * Enforce opening update/read-only mode to allow/forbid create/delete layers, create/update features (#4215)
 * Launder single quote character in table name; properly escape table name if no laundering (#1834)
 * Use ALTER TABLE ADD COLUMN by default to create a new field; older method can still be used by defining the OGR_SQLITE_USE_ADD_COLUMN config option to FALSE in order to provide read-compat by sqlite 3.1.3 or earlier
 * Fix bug in CreateField() : if there was already one record, the content of the table was not preserved, but filled with the column names, and not their values
 * Map 'DECIMAL' columns to OGR real type (#4346)
 * Add OGR_SQLITE_CACHE configuration option for performance enhancements
 * Try to reuse INSERT statement to speed up bulk loading.

SVG driver:
 * New for GDAL/OGR 1.9.0
 * Read only driver for Cloudmade Vector Stream files

S57 driver:
 * Add support for Dutch inland ENCs (#3881)
 * Allow up to 65536 attributes, use GUInt16 for iAttr (#3881)
 * Be cautious of case where end point of a line segment has an invalid RCID
 * Correct handling of update that need to existing SG2D into an existing feature without it (#4332)

VRT driver:
 * Do not try to read too big files
 * Lazy initialization of OGRVRTLayer
 * Don't set feature field when source feature field is unset

WFS driver:
 * Add preliminary support for WFS 2.0.0, but for now don't request it by default.
 * Increase performance of layer definition building by issuing a DescribeFeatureType request for several layers at the same time
 * Better server error reporting
 * Use the layer bounding box for EPSG:4326 layers (restricted to GEOSERVER for now) (#4041)
 * Add capability of opening a on-disk Capabilities document
 * Add special (hidden) layer 'WFSLayerMetadata' to store layer metadata
 * Add special (hidden) layer 'WFSGetCapabilities' to get the raw XML result of the GetCapabilities request
 * CreateFeature()/SetFeature(): use GML3 geometries in WFS 1.1.0 (make TinyOWS happy when it validates against the schema)
 * Make spatial filtering work with strict Deegree 3 servers
 * Fix reading when layer names only differ by their prefix

XLS driver:
 * New for GDAL/OGR 1.9.0
 * Read only driver for MS XLS files and relies on FreeXL library.

XPlane driver:
 * Port to VSI*L API

XYZ driver:
 * Ignore comment lines at the beginning of files

## SWIG Language Bindings

General :
 * RFC 30: Correct the signature of Datasource.CreateDataSource() and DeleteDataSource(),  gdal.Unlink() to accept UTF-8 filenames (#3766)
 * Add Band.GetCategoryNames() and Band.SetCategoryNames()
 * Add Geometry.GetPoints() (only for Python and Java) (#4016)
 * Add Geometry.GetEnvelope3D()
 * Add Geometry.SimplifyPreserveTopology()
 * Extend SWIG Geometry.ExportToJson() to accept a list of options (#4108)
 * Add (OGR)DataSource.SyncToDisk()
 * Add SpatialReference.SetVertCS(), IsSameVertCS(), IsVertical(), SetCompound(), IsCompound()
 * Add SpatialReference.SetIGH() (#4060)
 * RFC 35: Add OGRLayer.DeleteField(), ReorderField(), ReorderFields(), AlterFieldDefn()
 * Add gdal.VSIFTruncateL()

CSharp bindings:
 * Implement the typemap for utf8_path in C# (#3766)
 * Correcting the signature of OGRDataSource.Open to make the utf8 typemap to work (#3766)

Java bindings:
 * Turn the gdalJNI, gdalconstJNI and osrJNI into package private classes
 * Make Layer.GetExtent() return null when OGR_L_GetExtent() fails.

Perl bindings:
 * The "Points" method of Geometry was not accepting its own output in the case of a single point. It accepted only a point as a list. Now it accepts a point both as a list containing one point (a ref to a point as a list) and a point as a list.
 * Fixed UTF-8 support in decoding names (datasource, layer, field etc.).
 * Assume all GDAL strings are UTF-8, handle all changes in typemaps.
 * Additions to Perl bindings due to new developments etc: Layer capabilities, GeometryType  method for Layer, improved create, new Export and Set methods for SpatialReference.
 * Detect context in a typemap which returns a array, this now returns a list in list context; the change affects at least GetExtent and GetEnvelope methods, which retain backward compatibility though new and/or changed methods: FeatureDefn::Name, FeatureDefn::GeometryIgnored, FeatureDefn::StyleIgnored, Feature::ReferenceGeometry, Feature::SetFrom, FieldDefn::Ignored, Geometry::AsJSON
 * Perl typemaps: more correct manipulation of the stack, more cases where a list is returned in a list context, better handling of callback_data @Band::COLORINTERPRETATIONS, Band methods Unit, ScaleAndOffset, GetBandNumber, RasterAttributeTable method LinearBinning
 * Typemaps for VSIF{Write|Read}L, tests and docs for some VSI* functions.
 * Perl bindings: better by name / by index logic, some checks for silent failures, return schema as a hash if wanted, support ->{field} syntax for features, return list attributes as lists or listrefs as wished so that ->{field} works for lists too (API change)

Python bindings:
 * Improvements for ogr.Feature field get/set
 * Add hack to bring Python 3.2 compatibility
 * First argument of VSIFWriteL() should accept buffers for Python3 compat
 * Fix reference leak in 'typemap(in) char **dict'
 * Add gdal.VSIStatL()
 * swig/python/setup.py : fix for virtualenv setups (#4285)
 * Layer.GetExtent() : add optional parameter can_return_null that can be set to allow returning None when OGR_L_GetExtent() fails
 * Make gdal.VSIFSeekL(), gdal.VSIFTellL() and gdal.VSIFTruncateL() use GIntBig instead of long for compat with 32bit platforms
 * Add script to build the python extensions with Python 2.7 and a mingw32 cross-compiler under Linux/Unix

Ruby bindings:
 * Build SWIG Ruby Bindings against modern Ruby versions (1.8.7 and 1.9.2) (#3999)

# GDAL/OGR 1.8.0 release notes

(Note: Most changes/bugfixes between 1.7.0 and 1.8.0 that have already gone
to the 1.7.X maintenance releases are not mentioned hereafter.)

## In a nutshell...

* New GDAL drivers : GTX, HF2, JPEGLS, JP2OpenJPEG, JPIPKAK, KMLSUPEROVERLAY,
                     LOS/LAS, MG4Lidar, NTv2, OZI, PDF, RASDAMAN, XYZ
* New OGR drivers : AeronavFAA, ArcObjects, GPSBabel, HTF, LIBKML, MSSQLSpatial, NAS,
                    OpenAir, PDS, PGDump, SOSI, SUA, WFS
* Significantly improved OGR drivers : DXF, GML
* New implemented RFCs : RFC 7, RFC 24, RFC 28, RFC 29, RFC 30, RFC 33
* New utility : gdallocationinfo

## Backward compatibility issues

* MITAB driver: use "," for the OGR Feature Style id: parameter delimiter,
  not "." as per the spec. Known impacted application :
  MapServer (http://trac.osgeo.org/mapserver/ticket/3556)
* RFC 33 changes the way PixelIsPoint is handled for GeoTIFF (#3838,#3837)
* GML driver: write valid <gml:MultiGeometry> element instead of the non-conformant
  <gml:GeometryCollection>. For backward compatibility, recognize both syntax for
  the reading part (#3683)

## GDAL/OGR 1.8.0 - General Changes

Build(All):
 * Make sure that 'import gdal' can work in a --without-ogr build

Build(Unix):
 * Fix compilation on RHEL/Centos 64bit for expat and sqlite3 (#3411)
 * Update to autoconf 2.67 and libtool 2.2.6
 * During the external libtiff autodetection check whether library version is 4.0
   or newer, fallback to internal code otherwise. It is still possible to link
   with older libtiff using the explicit configure option (#3695)
 * Make --with-threads=yes the default
 * Allow using --with-spatialite=yes
 * Check /usr/lib64/hdf for RedHat 64bit

Build(Windows):
 * Change the default MSVC version to VS2008.

## GDAL 1.8.0 - Overview of Changes

Port:
 * RFC 7 : Use VSILFILE for VSI*L Functions (#3799)
 * RFC 30 : Unicode support for filenames on Win32
 * Implement Rename() for /vsimem
 * New virtual file system handlers :
    - /vsicurl/ : to read from HTTP or FTP files (partial downloading)
    - /vsistdin/ : to read from standard input
    - /vsistdout/ : to write to standard output
    - /vsisparse/ :mainly to make testing of large file easier
    - /vsitar/ : to read in .tar or .tgz/.tar.gz files
 * Add C API to create ZIP files
 * Add support for writable /vsizip/
 * Add VSIBufferedReaderHandle class that is useful to improve performance when
   doing backward seeks by a few bytes on underlying file handles for which
   backwardseeks are very slow, such as GZip handle
 * Add service for base64 decoding
 * CPL ODBC : Add transaction support (#3745)
 * CPL ODBC: Increase the default connection timeout to 30 sec
 * Add VSIStatExL() that has a flag to specify which info is really required
   (potential speed optimization on slow virtual filesystems such as /vsicurl)
 * Add VSIIsCaseSensitiveFS() to avoid ugly #ifndef WIN32 / #endif in the code of
   various drivers
 * Add Recode() convenience method to CPLString
 * HTTP downloader: add PROXY and PROXYUSERPWD options (and GDAL_HTTP_PROXY and
   GDAL_HTTP_PROXYUSERPWD configurations option) to allow request to go through a
   proxy server.

Core:
 * RFC 24: progressive/async raster reading
 * On Unix, add capability of opening the target of a symlink through GDALOpen()
   even if it not a real filename. Useful for opening resources expressed as
   GDAL virtual filenames in software offering only file explorers (#3902)
 * Assume anything less than 100000 for GDAL_CACHEMAX is measured in megabytes.
 * Read cartesian coordinates if applicable in GDALLoadOziMapFile().
 * Avoid being overly sensitive to numeric imprecision when comparing pixel
   value and nodata value in GDALRasterBand::ComputeStatistics()/
   ComputeRasterMinMax(), especially for GeoTIFF format where nodata is
   stored as text (#3573)
 * Better handling of NaN (not a number) (#3576)
 * Add C wrapper GDALSetRasterUnitType() for GDALRasterBand::SetUnitType() (#3587)
 * Add GDALLoadRPCFile() to read RPCs from GeoEye _rpc.txt files (#3639)
 * Allow GDALLoadRPB/RPC/IMDFile() to be called directly with the RPB/RPC/IMD
   filename
 * In GDAL cache block, use 64-bit variables for cache size
 * Add GDALSetCacheMax64(), GDALGetCacheMax64() and GDALGetCacheUsed64() (#3689)
 * Improve formatting of seconds in DecToDMS()
 * Support negative nPixelOffset values for RawRasterBands
 * GDALDatasetCopyWholeRaster(): improve performance in certain cases by better
   fitting to input/output block sizes
 * Add GDALRasterBandCopyWholeRaster()
 * Make sure band descriptions are properly captured and cloned (#3780)
 * GDALDataset/GDALRasterBand::CreateMaskBand(): invalidate pre-existing raster
   band mask that could be created lazily with GetMaskBand()/GetMaskFlags(),
   so that a later GetMaskBand() returns the newly created mask band
 * Overview computation : speed improvements in resampling kernels
 * Fix dereferencing of open datasets for GetOpenDatasets (#3871)
 * Add DllMain callback to set-up and tear-down internal GDAL library
   resources automatically (#3824)
 * List .aux file if it used in GDALPamDataset::GetFileList()
 * PAM dataset : try retrieving projection from xml:ESRI metadata domain

Algorithms:
 * rasterize: Burn the attribute value in ALL the bands during rasterization. (#3396)
 * geoloc : Allow using XBAND and YBAND with height == 1 in the case of a regular
   geoloc grid, suc h as for LISOTD_HRAC_V2.2.hdf (#3316)
 * GDALFillNodata(): improve&fix progress report
 * warper : Try to determine if we will need a UnifiedSrcDensity buffer
            when doing memory computations (#3515).
 * warper : GDALSuggestedWarpOutput2(): use more sample points around the edge
            of the raster to get more accurate result (#3742)
 * warper : added (preliminary) support for mask bands that aren't nodata or alpha
 * warper : integrate Google Summer of Code OpenCL implementation of warper
 * gdalgrid: Move ParseAlgorithmAndOptions from apps/gdal_grid.cpp to
             alg/gdalgrid.cpp (#3583)
 * RPCTransformer: take into account optional DEM file to extract elevation
   offsets (RPC_HEIGHT_SCALE and RPC_DEM transformation options added) (#3634)
 * GDALReprojectImage() : correctly assign nSrcAlphaBand and nDstAlphaBand (#3821)
 * gdalgrid : Properly initialize the first nearest distance in GDALGridNearestNeighbor().

Utilities :
 * gdallocationinfo : new
 * nearblack: add -setalpha option to add/set an alpha band + -of, -q, -co
 * nearblack: add -setmask option to use a mask band to mask the nodata areas
 * gdalbuildvrt: support stacking ungeoreferenced images when using -separate,
   provided they have the same size (#3432)
 * gdalbuildvrt: implement a check to verify that all color tables are identical
 * gdalbuildvrt: automatically create a VRT mask band as soon one of the sources
   has a dataset mask band (non-trivial = neither alpha, neither alldata, neither nodata)
 * gdalbuildvrt: use OSRIsSame() to check if all source raster have same SRS (#3856)
 * gdal_translate: Transfer GEOLOCATION in the -of VRT case if spatial
   arrangement of the data is unaltered
 * gdal_translate : add support for resizing datasets with mask bands
 * gdal_translate : add -mask option to add a mask band from an input band/mask band.
   Also extend syntax for the value of the -b option to allow specifying mask band as input band
 * gdal_translate : support '-a_nodata None' as a way of unsetting the nodata value
 * gdal_translate : invalidate statistics when using -scale, -unscale, -expand, -srcwin,
                    -projwin or -outsize and a new -stats option to force their (re)computation (#3889)
 * gdal_rasterize: Add capability of creating output file (#3505)
 * gdaldem: add a new option, -compute_edges, that enable gdaldem to compute
   values at image edges or if a nodata value is found in the 3x3 window,
   by interpolating missing values
 * gdaldem : add '-alg ZevenbergenThorne' as an alternative to Horn formula
   for slope, aspect and hillshade
 * gdaldem : support GMT .cpt palette files for color-relief (#3785)
 * gdalwarp: add -crop_to_cutline to crop the extent of the target dataset to
   the extent of the cutline
 * gdalwarp: add a -overwrite option (#3759)
 * gdal_grid : Properly use the spatial filter along with the bounding box.
 * epsg_tr.py: added -copy format for INGRES COPY command
 * hsv_merge.py: support RGBA dataset as well as RGB dataset, add -q and -of
   options, avoid using hillband when it is equal to its nodata value
 * val_repl.py: copy geotransform and projection from input dataset to output
   dataset
 * gdal_retile.py : assign color interpretation (#3821)
 * gdal_retile.py : add -useDirForEachRow option to create a different output structure (#3879)
 * Make gdal_translate and gdalwarp return non-zero code when block writing failed
   for some reason (#3708)
 * loslas2ntv2.py : new utility : .los/.las to NTv2 converter
 * gdal_calc.py : new utility
 * Add -tap option to gdal_rasterize, gdalbuildvrt, gdalwarp and gdal_merge.py
   to align on a standard grid (#3772)

AAIGRID driver:
 * Cast nodata value to float to be consistent with precision of pixel data in
   GDT_Float32 case; small optimization to avoid reading the first 100K when
   we know that the datatype is already Float32
 * Allow reading files where decimal separator is comma (#3668)
 * Detect 1e+XXX as a real value among integer values (#3657)
 * Add a AAIGRID_DATATYPE configuration option that can be set to Float64
 * speed-up CreateCopy(), particularly on windows, by buffering the output

AIGrid driver:
 * Support sparse sets of tile files {w,z}001???.adf (#3541)

BSB driver:
 * Capture extension lines for headers
 * Added UNIVERSAL TRANSVERSE MERCATOR, LCC and POLYCONIC handling (#3409)
 * provide an option (BSB_IGNORE_LINENUMBERS) to ignore line numbers as some
   generators do them wrong but the image is otherwise readable (#3776)
 * Avoid turning missing values to index 255 (#3777)

DODS driver:
 * Compilation fix to support libdap 3.10

DTED driver:
 * Add origin metadata in original format (#3413)
 * Report NIMA Designator field as 'DTED_NimaDesignator' metadata (#3684)
 * Fixes to read some weird DTED3 file

ECW driver:
 * Support building against 4.1 SDK (compat with older versions maintained) (#3676)
 * Add alpha support with 4.1 SDK, and various configuration options
 * Add pseudo powers of two overviews.

EHdr driver:
 * Improvements to deal with http://www.worldclim.org/futdown.htm datasets

ENVI driver:
 * Support tabulation character in .hdr files (#3741)
 * Support reading gzipped image file (#3849)

ERS driver:
 * Read "Units" child of the "BandId" node and set it as unit type for RasterBand.

FITS driver:
 * Accept files whose metadata list doesn't end with 'END' (#3822)

GeoRaster driver :
 * Suppress error when testing SRID code as EPSG (#3326)
 * Several improvements and fixes (#3424)
 * Deprecates JPEG-B compression (#3429)
 * Fix GetColorInterpretation() on RGBA's alpha channel (#3430)
 * Allows OS authentication (#3185)
 * Add support for Point Cloud, add transaction control wrapper
 * use OCI Bind to load VAT (#3277)
 * Change order of NODATA tag on XML metadata (#3673)
 * Add support for per band NoData value - Oracle 11g (#3673)
 * Add support to ULTCoordinate - (#3718)
 * Fix interleaving cache error (#3723)
 * Fix compress vs nbits order error (#3763)
 * Fix writing interleaved jpeg #3805 and reading default blocksize #3806
 * Add create option blocking=(YES,NO,OPTIMUM) #3807, also fix #3812

GRASS driver:
 * Update GDAL and OGR GRASS drivers to compile against GRASS 7.0SVN (#2953)

GTiff driver :
 * RFC 33 : Adjust PixelIsPoint handling (#3838,#3837)
 * Refresh internal libtiff with upstream
 * Refresh internal libgeotiff with upstream
 * Add PREDICTOR_OVERVIEW configuration option to set the predictor value for
   LZW or DEFLATE compressed external overviews; Also make sure that the
   predictor value gets well propagated in the case of internal overviews (#3414)
 * Add a COPY_SRC_OVERVIEWS creation option (for CreateCopy()) that copies
   existing overviews in the source dataset.
 * Make GetScale() and GetOffset() retrieve values from PAM if not available in
   internal metadata
 * Use GCP info from PAM if available
 * Support CreateCopy() on datasets with a color indexed channel and an alpha
   channel (#3547)
 * Allow reading geotransform when opening with GTIFF_DIR prefix (#3478)
 * Add a warning when clipping pixel values for odd-bits band
 * Make sure that 16bit overviews with jpeg compression are handled using 12bit
   jpeg-in-tiff (#3539)
 * Add GDAL_TIFF_OVR_BLOCKSIZE configuration option to specify block size used
   for overviews
 * Read RPCs from GeoEye _rpc.txt files (#3639)
 * Implement GetUnitType() and SetUnitType(); make sure to remove
   TIFFTAG_GDAL_METADATA tag if it existed before and there are no more
   metadata; fix to make sure we can unset offset & scale stored in PAM
 * Speed-up writing of blocks in case of multi-band 8 bit images
 * Support TIFF_USE_OVR config option to force external overviews
 * Add special ability to for xml:ESRI metadata into PAM
 * Try to detect build-time vs runtime libtiff version mismatch (*nix only)
 * Added logic to expand verticalcs using importFromEPSG() when possible
 * Create internal masks with deflate compression if available
 * Fix jpeg quality propagation (particularly remove warning when
   using a deflate compressed internal mask band with jpeg compressed main IFD)
 * Add support for JPEG_QUALITY_OVERVIEW configuration option for internal
   overviews when adding them after dataset reopening
 * auto-promote mask band to full 8 bits by default (unless
   GDAL_TIFF_INTERNAL_MASK_TO_8BIT is set TO FALSE).
 * add LZMA compression optional support (requires latest libtiff4 CVS HEAD)
 * Supporting writing compound coordinate systems.

GTX driver:
 * New for GDAL/OGR 1.8.0
 * Read NOAA .gtx vertical datum shift files.

GXF driver:
 * Cast nodata value to float to be consistent with precision of pixel data in
   GDT_Float32 case
 * Introduce a GXF_DATATYPE configuration option that can be set to Float64
 * Use GDALGetScanline() instead of GDALGetRawScanline() so that #SENS
   is applied to normally return things in conventional orientation as
   is assumed by the geotransform.  (#3816).

HDF4 driver:
 * Prevent reading nonexistent subdatasets
 * Allow reading 1D subdatasets, in particular for GEOLOC bands
 * Workaround strange test that swaps xsize, ysize and nbands for the particular
   case of the dataset of ticket #3316
 * Speed up access to HDF4_SDS datasets; allow multi-line block dimension for
   HDF4_EOS datasets (#2208)
 * HDF4_EOS_GRID : detect tile dimensions and use them as block size; increase
   HDF4_BLOCK_PIXELS default value to 1,000,000 (#3386)
 * Support reading of L1G MTL metadata (#3532)
 * Read as HDF if HDFEOS returned 0 datasets
 * Improve fetching the geolocation data in case of one-to-one mapping and
   abcence of dimension maps (#2079)
 * Properly set the GCP projection for MODIS Aerosol L2 Product.
 * Fetch scale/offset, unit type and descriptions for some HDF-EOS datasets.

HDF5 driver:
 * Avoid setting bogus projection if we don't get georeferencing from
   CreateProjections().  Avoid trying to operate if DeltaLat/Long is zero.
   Avoid crashing on NULL poH5Object->pszPath in CreateMetadata().  (#3534)
 * Ensure backslashes are preserved in paths for UNC on win32 (#3851)

HF2 driver:
 * New for GDAL/OGR 1.8.0
 * Read and write HF2/HFZ heightfield raster

HFA driver:
 * Ensure that an .aux file created for overviews has AUX=YES set so a
   base raster will not be created.
 * Various robustness improvements (#3428)
 * Support pulling overviews from an .rrd file even if the .aux does not
   reference it (#3463)
 * avoid using empty names for layer, if we have one generate a fake name,
   use for overviews (#3570)
 * Add support for New Zealand Map Grid to HFA driver (#3613)
 * Support EPT_s8 in BASEDATA (#3819)
 * Substantial improvements for Rename/CopyFiles (#3897)

Idrisi driver:
 * Allow color items greater than maximum value (#3605)

JPEGLS driver:
 * New for GDAL/OGR 1.8.0
 * JPEG-LOSSLESS driver based on CharLS library

JP2KAK driver:
 * Allow quality as low as 0.01
 * Major restructuring, all reading now goes through DirectRasterIO (#3295)
 * Introduce YCC optimization
 * Ensure we fetch <= 8 bit images with their true precision (#3540)
 * Make JP2KAK_RESILIENT also turn off persist, and force sequential access (#4336)
 * Fix reading overviews via direct case (#4340)

JP2OpenJPEG:
 * New for GDAL/OGR 1.8.0
 * JPEG2 driver based on OpenJPEG library

JPIPKAK driver:
 * New for GDAL/OGR 1.8.0
 * JPIP driver based on Kakadu library

KMLSUPEROVERLAY driver :
 * New for GDAL/OGR 1.8.0
 * Added new plug-in GDAL Super-Overlay Driver. The driver allows converts
   raster (like TIF/GeoTIFF, JPEG2000, JPEG, PNG) into a directory structure
   of small tiles and KML files which can be displayed in Google Earth.

LOS/LAS driver:
 * New for GDAL/OGR 1.8.0
 * Read NADCON .los/.las Datum Grid Shift files

MG4Lidar driver
 * New for GDAL/OGR 1.8.0
 * Read MG4 Lidar point cloud data and expose it as a Raster. It depends on
   the current, freely-available-though-not-open-source MG4 Lidar SDK v1.1

MrSID:
 * Updated to support MrSID SDK v8.0.0 (compat with older versions maintained) (#3889)
 * Updated to support writing MG4/Raster (#3889)
 * Support reading projection from .met files accompanying NASA LandSat SID files

NetCDF driver:
 * Improve coordinate system support (#3425)
 * Add support for multiple standard_parallel tags to support LCC single
   standard parallel (#3324)
 * Add CF-1 spheroid tag support for netcdf driver
 * Add support for weather/climate files with pixel size in km
 * Attempt to fix flip image (#3575)
 * Add support for Scale and Offset (#3797)

NITF driver:
 * Ensure that igeolo corners are not messed up if irregular, keep center/edge
   of pixel location info (#3347)
 * Add capture of select RPF attribute metadata (#3413)
 * Carry raw IGEOLO and ICORDS through as metadata (#3419)
 * Added NITFPossibleIGEOLOReorientation() in an attempt to deal with
   files written with the IGEOLO corners out of order.
 * Implement readonly support for RSets (#3457)
 * Add capability of writing CGM segment as creation option (or from the source
   CGM metadata domain if no CGM= creation option); for consistency, also add
   the capability of writing TEXT segment as creation option, in addition to
   the existing capability of writing it from the source TEXT metadata domain (#3376)
 * Fix read out of buffer for NBPP < 8 and very small block size; fix decoding
   of NBPP=4 (#3517)
 * Add FILE_TRE creation option to write TRE content in XHD field of file header
 * Add SDE_TRE creation option to write GEOLOB and GEOPSB TREs. This is limited
   to geographic SRS, and to CreateCopy() for now
 * Allow using NITF header located in STREAMING_FILE_HEADER DE segment when
   header at beginning of file is incomplete
 * Improve NITF to NITF translation
 * Fetch TREs from DE segment
 * Support reading CSSHPA DES & extracting embedded shapefile
 * Support writing image comments (ICOM)
 * Add description for NITF file & image header fields in creation options XML
 * Accept A.TOC files with frame entries that have same (row,col) coordinates
 * Avoid erroring out when file or image user TRE size is just 3
 * Load subframe mask table if present (typically, for CADRG/CIB images with IC=C4/M4) (#3848)
 * A few hacks to accept some (recoverable) file inconsistencies (#3848)

NTv2 driver:
 * New for GDAL/OGR 1.8.0
 * Read&write NTv2 Datum Grid Shift files

OZI driver:
 * New for GDAL/OGR 1.8.0
 * Read OZI OZF2/OZFX3 files

PAUX driver:
 * Add support for INTERLEAVE option

PCIDSK2 driver:
 * PCIDSK SDK refreshed from upstream
 * Remove svn:external for pcidsk sdk; Copy it directly in GDAL tree
 * Support for reading and writing descriptions added to the PCIDSK SDK.
 * Add bitmap support
 * Support for reading/writing complex PCIDSK files through libpcidsk
 * Support worldfile if lacking internal georef (#3544)
 * Fix locking state at CPLThreadMutex creation (#3755)
 * Improved projection support.

PDF driver:
 * New for GDAL/OGR 1.8.0
 * Read Geospatial PDF (through poppler library), either encoded according
   to OGC Best practice or Adobe ISO32000 extensions.

PDS driver:
 * Support quoted SAMPLE_TYPE. Check for UNSIGNED in SAMPLE_TYPE for UInt16.
 * Support files where scanlines are broken over several records.
 * Support newline continuation
 * Recognize ENCODING_TYPE = "N/A" (N/A surrounded by double-quotes)
 * Take into account MINIMUM, MAXIMUM, MEAN and STANDARD_DEVIATION when
   available to set the statistics

PNG driver:
 * Update internal libpng to 1.2.44
 * Internal libpng : Make screwy MSPaint "zero chunks" only a warning,
   not error (#3416).
 * Added ZLEVEL creation option

PostGIS Raster (formerly WKTRaster driver):
 * Improved block reading and raster settings reading in WKT Raster driver
 * Functions SetRasterProperties and GetGeoTransform modified to allow both
   referenced and not referenced rasters
 * Connection string parsing simplified. Schema, table name and where clause
   can be passed with or without quotes
 * New parameter "mode"

RASDAMAN driver:
 * New for GDAL/OGR 1.8.0
 * Read rasters in rasdaman databases

RMF driver:
 * Significant improvements. Implemented decompression scheme typically
   used in DEM data.

SRP driver:
 * Relax strict equality test for TSI size for unusual products (#3862)

TerraSAR driver:
 * Enhancements related to GCPs handling (#3564).

USGSDEM driver:
 * Support non-standard DEM file (#3513)

VRT driver:
 * Preliminary Overview support on VRT bands (#3457)
 * Support for mask band : VRT may expose a mask band,
   and mask bands can be used as VRTRasterBand sources
 * Port to VSIF*L API; advertise GDAL_DCAP_VIRTUALIO=YES
 * Make format identification less strict (#3793)
 * Support for LocationInfo metadata item on bands

WCS driver:
 * Decode base64 encoded multipart data

WMS driver:
 * Adds a <UserAgent> optional parameter so that the user be able to provide
   its own useragent string for picky WMS servers (#3464)
 * Default color interpretation for wms driver (#3420)
 * Add UnsafeSSL setting (#3882)

XYZ driver:
 * New for GDAL/OGR 1.8.0
 * Read ASCII XYZ gridded datasets

## OGR 1.8.0 - Overview of Changes

Core:
 * RFC 28 : OGR SQL Generalized Expressions
 * RFC 29 : Support for ignoring fields in OGR
 * Add OGRLayer::GetName() and OGRLayer::GetGeomType() virtual methods,
   and their C and SWIG mappings (#3719)
 * On Unix, add capability of opening the target of a symlink through OGROpen()
   even if it not a real filename. Useful for opening resources expressed as
   GDAL virtual filenames in software offering only file explorers (#3902)
 * Expat based XML readers : add support for reading files with Windows-1252
   encoding
 * Use transactions in CopyLayer for better speed. (#3335)
 * OGRGeometry::importFromWkt() : allow importing SF-SQL 1.2 style WKT while
   preserving compatibility with previously recognized non conformant WKT (#3431)
 * Add C functions : OGR_G_ForceToPolygon(), OGR_G_ForceToMultiPolygon(),
                     OGR_G_ForceToMultiPoint() and OGR_G_ForceToMultiLineString()
 * Add C functions : OGR_G_Length(), OGR_G_Simplify(), OGR_G_Area(), OGR_G_Boundary()
                     OGR_G_SymDifference() and OGR_G_UnionCascaded()
 * Add C function: OGR_F_StealGeometry()
 * Move Centroid() method from OGRPolygon to OGRGeometry base class to be able
   to operate on various geometry types, and to be consistent with PostGIS
   ST_Centroid() capabilities and the underlying GEOS method
 * Make the GetStyleTable() SetStyleTable() SetStyleTableDirectly() methods on
   datasources and layers virtual (#2978)
 * Add OGRSFDriverRegistrar::DeregisterDriver() and OGRDeregisterDriver()
 * Improve detection of rounding errors when writing coordinates as text with
   OGRMakeWktCoordinate()
 * OGR SQL: allow comparing datetime columns in WHERE clause
 * OGR indexing: re-use .ind file in read-write mode when calling CreateIndex()
   but the index was opened as read-only (follow up of #1620); ensure that the
   .ind file is closed before being unlink()'ed
 * AssemblePolygon: ensure largest area ring is used as exterior ring (#3610)
 * OGRGeometryFactory::createFromGEOS() : preserve coordinate dimension
   (with GEOS >= 3.3) (#3625)
 * Allow calling transformWithOptions() with a NULL poCT
 * Improve wrapdateline, especially on LINESTRING
 * Fix getEnvelope() for OGRPolygon and OGRGeometryCollection to avoid taking
   into empty sub-geometries; Fix OGRLayer::GetExtent() to avoid taking into
   account empty geometries
 * Support attribute index scan with the sql 'IN' operator (#3686)
 * Add attribute index support for the sql queries in mapinfo tab format (#3687)
 * OGRGometry: add a swapXY() virtual method
 * Implement special field support for IsFieldSet
 * OGRLineString::transform() : allow partial reprojection if
   OGR_ENABLE_PARTIAL_REPROJECTION configuration option is set to YES (#3758)
 * Add OGR_G_ExportToGMLEx() that can take options to enable writing GML3
   geometries compliant with GML3 SF-0
 * OGRFeature::SetField() : support setting integer and real lists from a
   string in the format (n:value,value,value,...)

OGRSpatialReference:
 * Big upgrade to EPSG 7.4.1 with improved datum logic
 * Use PROJ 4.8.0 thread-safe functions if available to avoid global OGR PROJ4
   mutex when doing OGRProj4CT::TransformEx()
 * Support for defining VERT_CS and COMPD_CS from EPSG and from/to PROJ.4
 * Implement OGRSpatialReference:IsVertival() and
   OGRSpatialReference::IsSameVertCS() methods
 * add RSO gamma handling (proj #62)
 * TMSO support
 * Adjust handling of NAD27 to avoid towgs84 params, use +datum when no towgs84,
   do not emit +ellipse if +datum used (#3737)
 * exportToProj4() : add +towgs84= instead of +datum= if both information are
   available. This behavior can be turned off by setting
   OVERRIDE_PROJ_DATUM_WITH_TOWGS84=NO (#3450)
 * Add PROJ4_GRIDS EXTENSION as a way of preserving datum grids
 * ogr_srs_proj4: add a table for Prime Meridians; improve recognition of prime
   meridian to export them as names when possible with exportToProj4()
 * importFromProj4(): recognize +f= option
 * Correct handling of Mercator2SP from EPSG (#2744)
 * Make GetAxis() const
 * Improve axis orientation recognition for stuff like EPSG:3031
 * Fix Amersfoort (geotiff #22)
 * Panorama: Added zone number to the list of projection parameters list.
   Use the zone number when we need to compute Transverse Mercator projection.
 * Panorama: Use Pulkovo 42 coordinate system instead of WGS84 as a fallback
   if the CS is not specified.
 * ESRI: added support for Mercator in an old style file
 * Add Bonne, Gauss-Schreiber Transverse Mercator, Mercator (2SP), Two Point
   Equidistant and Krovak to the list of projections description dictionary.
 * SRS validation : various fixes
 * Exposure OSRImportFromERM() and OSRExportToERM() functions in C API.
 * PCI : Fix the transfer of scale for Stereographic Projection (#3840).
         Add support for Oblique Stereographic (SGDO) (#3841)
 * Substantially upgrade PCI datum conversions using PCI datum/ellips.txt files

Various drivers:
 * Improve behavior of DXF, VFK, GPX, SHAPE, PG, LIBKML, KML, VRT, CSV, GML,
   BNA, GeoRSS, GEOJSON drivers when LC_NUMERIC is not the C locale

Utilities:
 * ogr2ogr: copy datasources and layers style table
 * ogr2ogr: use OGRGeometryFactory::forceToMultiLineString() when -nlt
            MULTILINESTRING is specified
 * ogr2ogr: allow -wrapdateline if neither input or output srs is specified,
            but input layer srs is geographic
 * ogr2ogr: add -splitlistfields and -maxsubfields options to split fields of
            type IntegerList, RealList or StringList into as many subfields of
            single type as necessary.
 * ogr2ogr: accept None or Null as a special value of -a_srs to nullify the output SRS
 * ogr2ogr: ignore -overwrite options if the output datasource does not yet exist (#3825)
 * ogr2ogr: special case when output datasource is a existing single-file Shapefile :
            auto-fill the -nln argument if not specified (#2711)
 * ogr2ogr: add a -explodecollections option to split multi geometries into several features
 * ogr2ogr: add a -zfield option to set the Z coordinate of a 3D geometry from the value of
            a field

AeronavFAA driver:
 * New for GDAL/OGR 1.8.0

ArcObjects driver:
 * New for GDAL/OGR 1.8.0

BNA driver:
 * Ported to use VSIF*L API

CSV driver:
 * For files structured as CSV, but not ending with .CSV extension, the 'CSV:'
   prefix can be added before the filename to force loading by the CSV driver
 * Support reading airport data coming from http://www.faa.gov/airports/airport_safety/airportdata_5010
 * If a datasource is created with the extension .csv assume this should be
   the first layer .csv file instead of a directory
 * Skip empty lines (#3782)
 * Port to VSI*L API for read&write; support writing to /vsistdout/

DGN driver:
 * Correct computation of abyLevelsOccurring (#3554).

DODS driver:
 * Compilation fix to support libdap 3.10

DXF driver:
 * Smooth polyline entity support added
 * Read blocks as a distinct layer instead of inlining
 * Assemble file at end with changes to header template - so far only inserting
   new layer definitions
 * Support for writing user defined blocks
 * Add limited hatch support
 * Add support for writing linetypes.  Support using complete dxf files as
   the header or trailer template.
 * Apply the INSERT entity id to all features inserted in its place (#3817)
 * various fixes
 * Implement Win1252/utf8 conversion for dxf text
 * Fix issues with text angles, text escape and multiline text
 * add support for \U+xxxx unicode chars in labels

GeoJSON driver:
 * Add reader to parse JSON output of FeatureService following GeoServices REST
 * Read and write "id" member at feature object level
 * Various robustness fixes to avoid crashes
 * Fix combined spatial and attribute filtering (#3803)

GeoRSS driver:
 * Ported to use VSIF*L API
 * Recognize <gml:MultiPoint>, <gml:MultiLineString> and <gml:MultiPolygon>
 * Support reading GeoRSS inside <rdf:RDF>

GML driver:
 * Adding support for xlink:href. (#3630)
 * Add support for Polish TBD GML
 * Support reading <gml:Curve>, <gml:MultiCurve>, <gml:TopoCurve>,
   <gml:TopoSurface>, <gml:Ring>, <gml:Surface>, <gml:PolygonPatch>,
   <gml:pointMembers>, <gml:curveMembers>, <gml:surfaceMembers>
   <gml:Triangle>, <gml:Rectangle>, <gml:Tin/gml:TriangulatedSurface>,
   <gml:Arc>, <gml:Circle> elements in GML3 geometries
 * Recognize <gml:Solid> and <gml:CompositeSurface>, <gml:OrientableSurface> elements
   (dealt as an approximation as multipolygons, and not as volumes)
 * Add support for "complex structure flattening" of attributes, and OFTStringList,
   OFTRealList and OFTIntegerList field types in case of multiple occurrences of
   a GML element (such as UK Ordnance Survey Mastermap) (#3680)
 * Add support for CityGML generic attributes <stringAttribute>, <intAttribute> and
   <doubleAttribute>
 * Various improvements for better support of AIXM 5.1
 * Write and retrieve layer geometry type to/from .gfs file (#3680)
 * Support using the <GeometryElementPath> to retrieve the appropriate geometry in
   case several ones are available per feature
 * Use VSIF*L API for read&write
 * XSD reader : various improvements, in particular to support various types of schema
   returned by WFS DescribeFeatureType
 * XSD writer: change the default GeometryPropertyType to a more precise type name
   according to the layer geometry type
 * Write valid <gml:MultiGeometry> element instead of the non-conformant
   <gml:GeometryCollection>. For backward compatibility, recognize both syntax for
   the reading part (#3683)
 * Support reading SRS per layer when possible, and deal with urn:ogc:def:crs:EPSG::xxx
   geographic coordinate systems (as returned by WFS 1.1.0 for example) to restore
   (longitude, latitude) order (unless GML_INVERT_AXIS_ORDER_IF_LAT_LONG is set to NO)
   Also add a GML_CONSIDER_EPSG_AS_URN option that can be set to YES when EPSG:XXXX should
   be considered as urn:ogc:def:crs:EPSG::XXXX
 * Expose gml:id as a string field when reading <wfs:FeatureCollection>
 * Add dataset creation option FORMAT=GML3 to write GML3 SF-0 compliant data
 * Add dataset creation option SPACE_INDENTATION=YES/NO to optionally disable space indentation
   when writing GML.
 * Recognize GML answer of MapServer WMS GetFeatureInfo request
 * Fix datatype detection to fallback to Real when an integer cannot fit into a 32bit int (#3866)
 * GML/WFS : use SRS defined in global gml:Envelope if no SRS is set for any feature geometry

GMT driver:
 * Add support for multilinestring reading (#3802)

GPSBabel driver:
 * New for GDAL/OGR 1.8.0
 * Read/Write files supported by GPSBabel utility

GPX driver:
 * Port write side of the driver to VSIF Large API
 * Add LINEFORMAT dataset creation option
 * Allow writing track points and route points with their own attributes by
   writing point features in track_points and route_points layers

HTF driver:
 * New for GDAL/OGR 1.8.0
 * Read Hydrographic Transfer Format (HTF)

Ingres driver:
 * Implement support for spatial reference systems (atrofast, #3159)
 * Added support for GEOMETRYCOLLECTION as a generic geometry type, and
   fleshed out the layer creation logic to support all geometry types.

KML driver:
 * KML vertex output: avoid warning for coordinates just above 90 and 180
   degrees due to numerical imprecisions
 * Port to VSI*L API for write

LIBKML driver:
 * New for GDAL/OGR 1.8.0
 * Alternate KML driver relying on Google libkml

MITAB driver:
 * Use "," for the OGR Feature Style id: parameter delimiter, not "." as per
   the spec.
 * Synchronized with mitab CVS HEAD
 * Fixed crash when trying to get the same mitab feature twice

MSSQLSpatial driver:
 * New for GDAL/OGR 1.8.0
 * Read/write support for MS SQL Spatial databases

NAS driver:
 * New for GDAL/OGR 1.8.0
 * Reads the NAS/ALKIS format used for cadastral data in Germany

OCI driver:
 * Allows OS authentication (#3185)
 * Uppercase table_name on SQL queries #1960 - OCI: ogr2ogr with append option
   is not considering layer creation
 * Support creation of non-spatial tables (#3690)
 * Set MULTI_LOAD as default #3656, for new layer and update

OGDI driver:
 * Add OGR_OGDI_LAUNDER_LAYER_NAMES configuration option to simplify reported layer names
 * Fix GetFeatureCount() when used with SetAttributeFilter()

OpenAir driver:
 * New for GDAL/OGR 1.8.0
 * Read Special Use Airspace in OpenAir format

PCIDSK driver:
 * Implement creation/update and coordinate system support

PDS driver:
 * New for GDAL/OGR 1.8.0
 * Read NASA Planetary Data Systems TABLE objects

PGeo driver:
 * Add PGEO_DRIVER_TEMPLATE option

PGDump driver:
 * New for GDAL/OGR 1.8.0
 * To output PostgreSQL SQL dump (very similar to shp2pgsql utility)

PostgreSQL driver:
 * Add support for PostgreSQL >= 9.0 new binary data format
 * Use canonical (HEXEWKB) form to get geometry to speed-up feature retrieval.
   WKT-based retrieval can still be used if PG_USE_TEXT configuration option
   is set to YES
 * If the PG_USE_BASE64 configuration option is set to YES, geometries will be
   requested as BASE64 encoded EWKB instead of canonical HEX encoded EWKB.
   (useful when bandwidth is the limiting factor)
 * Don't instantiate layer defn at layer creation. This can speed up
   significantly database opening when they are many tables and the user just
   needs to fetch one with GetLayerByName().
 * Implement efficient OGRLayer::GetName() and OGRLayer::GetGeomType()
 * Allow creating layer with eType = wkbNone
 * Don't require to have found a layer in readonly mode to succeed in opening
   the datasource.
 * Add TEMPORARY (ON/OFF) layer creation option to create TEMPORARY tables
 * CreateLayer() : don't launder schema_name if passed string is schema_name.table_name,
   and when testing if the layer already exists prepend or remove the
   schema_name to the table_name when comparing to layer names
 * Handle Nan and Inf values for float types in INSERT, UPDATE and
   COPY SQL commands. (#3667)
 * Retrieve the FID of a newly inserted feature (#3744)
 * Remove use of deprecated PostGIS functions when running against PostGIS 2.0SVN

S57 driver:
 * Avoid crashing if there is a missing VRPT field in a vector record.

SOSI driver:
 * New for GDAL/OGR 1.8.0
 * Read Norwegian SOSI-standard

Shapefile driver:
 * Use VSI*L API for .prj file so it works in virtual circumstances (#3536).
 * CreateLayer(): Check that the layer doesn't already exist
 * Implement multipatch read support
 * Remove ESRI style spatial indexes from Shapefile on update via OGR (#2798)
 * Fix conflict between spatial and attribute indexes (#3722)
 * Create integer fields with unknown width as 10 characters instead of 11
   to avoid them getting immediately considered to be real when reopening (#2151)
 * Make 'ogr2ogr -overwrite dst.shp src.shp -nln dst' work when dst.shp already exists

SQLite driver:
 * Spatialite : use MBRIntersects operator instead of MBRWithin (#3810)
 * Spatialite: avoid executing some Spatialite functions several times when
   issuing SQL queries
 * Add a INIT_WITH_EPSG dataset creation option to fill the spatial_ref_sys
   table with content of EPSG CSV files (default to NO); several fixes to be
   robust to single quote characters in SRS strings
 * Fix to make CopyLayer() work when src layer is a SQL result layer (#3617)
 * Add OGR_SQLITE_SYNCHRONOUS configuration option that, when set to OFF,
   can speed up considerably write operations (e.g. on EXT4 filesystems),
   at the expense of extra robustness w.r.t system crashes.
 * Spatialite: when creating a spatialiate DB, add a srs_wkt column in the
   spatial_ref_sys table, as introduced in libspatialite 2.4.0
 * Implement the DeleteLayer() interface and report the ODsCDeleteLayer capability

SUA driver:
 * New for GDAL/OGR 1.8.0
 * Read Special Use Airspace in Tim Newport-Peace's format

VRT driver :
 * Fix GetExtent() on non VGS_Direct VRT layers (#3783)

WFS driver:
 * New for GDAL/OGR 1.8.0
 * WFS client that brings read & write (WFS-T) support for WFS 1.0.0 and 1.1.0

## SWIG Language Bindings

General :
 * Add Geometry.Length(), Geometry.Area(), Geometry.Simplify(), Geometry.UnionCascaded(),
   Geometry.SymDifference, Geometry.Boundary()
 * Add SpatialReference.GetUTMZone()
 * Add Geometry.ForceToPolygon(), Geometry.ForceToMultiPolygon(),
       Geometry.ForceToMultiPoint() and Geometry.ForceToMultiLineString()
 * Add Band.SetScale() Band.SetOffset(), and Band.SetUnitType()
 * Add ogr.RegisterDriver() and ogr.DeregisterDriver()
 * Move ogr.GeometryTypeToName() and ogr.GetFieldTypeName() from Java bindings
   to all bindings
 * Initialize return values of GetRasterStatistics() so that we know if they
   have been updated
 * Add Feature.SetFromWithMap()
 * Add gdal.GridCreate() (#3661)
 * Expose OSR GetSemiMajor(), GetSemiMinor(), GetInvFlattening() and
   ImportFromERM() functions

CSharp bindings:
 * Use the .NET Framework 2.0 transparency rules (level 1 transparency) for the
   VC2010 builds (#3559)
 * Fix GDAL_DMD_ and GDAL_DCAP_ constants for Csharp (#3601)

Java bindings:
 * Add GdalGrid.java, GDALContour.java
 * Add 'make test' target for Unix/Windows

Perl bindings:
 * Support polygons in TransformPoints.
 * Test for existence of capability before comparison (avoid unnecessary warning)
 * Added CAPABILITIES lists to driver, datasource and layer classes;
   Capabilities and TestCapability methods return and use strings as documented
   already earlier; added FIELD_TYPES, JUSTIFY_TYPES, GEOMETRY_TYPES and
   BYTE_ORDER_TYPES lists into appropriate classes
 * "create" constructor for FeatureDefn
 * Add aliases Equals and Intersects as mentioned in #3492
 * Changes to support RFC 30
 * Add bindings for ReadDir, Stat, FillNodata
 * Simple support for PostGIS HEX EWKB (remove/add SRID)
 * The Schema method of FeatureDefn returns a schema, where each field hash contains
   also key 'Index' and the field index as the value.

Python bindings:
 * Fix compilation of Python bindings with Python 3.X on 64 bit platform
 * Optimize Band.ReadRaster() and Dataset.ReadRaster() to avoid extra buffer
   copy; also add the capability to pass the result buffer such as
   result_buf = ' '; ReadRaster(0, 0, 1, 1, buf_obj = result_buf) (#3521)
 * NumPy Band.WriteArray() : use numpy object directly instead of converting to
   string
 * Band.ReadAsArray() : instantiate the numpy object before calling RasterIO()
   instead of creating it from a string
 * NumPy : add support for signed byte
 * Make sure that feat.SetField('field_name', double_value) goes through
   OGR_F_SetFieldDouble() instead of being first converted to string
 * Add an optional parameter can_return_null to Dataset.GetGeoTransform();
   when used and set to True, None is returned when GDALGetGeoTransform()
   returns CE_Failure (instead of the fake (0,1,0,0,0,1)); backward
   compatibility preserved when the parameter isn't specified
 * Avoid suppressing warnings and errors when exceptions are used (#3632)
 * Add gdalinfo.py, ogrinfo.py and ogr2ogr.py as sample scripts, direct ports
   of corresponding C/C++ utilities
 * Allow manipulating buffer > 2 GB on 64bit builds for ReadRaster() and WriteRaster()
 * Map gdal.GetCacheMax(), gdal.SetCacheMax() and gdal.GetCacheUsed() to the
   corresponding 64bit new API
 * Reset error status before new GDAL/OGR call when using gdal.UseExceptions() (#3077)
 * Changes to support RFC 30
 * Fix Feature.ExportToJson() (#3870)

# GDAL/OGR 1.7.0

(Some of the bug fixes mentioned below might also have gone into 1.6.X maintenance releases.)

## In a nutshell...

* New GDAL drivers : BAG, EPSILON, Northwood/VerticalMapper, R, Rasterlite,
                     SAGA GIS Binary, SRP (USRP/ASRP), EarthWatch .TIL, WKT Raster
* GDAL PCIDSK driver using the new PCIDSK SDK by default
* New OGR drivers : DXF, GeoRSS, GTM, PCIDSK and VFK
* New utility : gdaldem, gdalbuildvrt now compiled by default
* Add support for Python 3.X. Compatibility with Python 2.X preserved (#3265)
* Remove old-generation Python bindings.
* Significantly improved GDAL drivers: GeoRaster, GeoTIFF, HFA, JPEG2000 Jasper, JPEG2000 Kakadu, NITF
* Significantly improved OGR drivers: CSV, KML, SQLite (SpatiaLite support), VRT
* WARNING: incompatibility between MrSID GeoDSDK and libgeotiff 1.3.0 or internal libgeotiff on some platforms (see #3309)

## GDAL/OGR 1.7.0 - General Changes

Build (All) :
 * Add gdalbuildvrt to the list of utilities built by default (#2747)
 * Improve Mingw compatibility (#2649)
 * Add Expat read support for GML driver when Xerces is unavailable (#2999)
 * Fix GML and ILI reading problems with Xerces 3.x (#3156)
 * Add 8/12bit JPEG-in-TIFF support
 * Fix trunk compilation against libdap 3.9.X (#3105)

Build (Windows) :
 * Check for CURL_LIB instead of CURL_DIR
 * ensure OGR_ENABLED gets defined if INCLUDE_OGR_FRMTS set (#2784)
 * Change quoting in VCDIR and SETARGV to avoid likely problems.
 * added dll and target-lib targets
 * fix _findfirst handle type for win64 (#3035)
 * Add support to compile OGR-postgis as a plugin (#3125)
 * Trap failures in subdirectories and stop build

Build (Unix) :
 * Use proper object file names when building prerequisites lists (#1878)
 * Updated man page generation
 * Add new ./configure test to check that the GCC 4.1 built-in functions for atomic memory access are really available
 * Handle external libz (#2942)
 * Add support for 64bit file i/o on BSD systems, through fseeko/ftello
 * Add support for linking against libspatialite to benefit from spatial functions in SQL queries (#2666)
 * Fix support for --with-threads configure option on BSDs (tested on DragonFlyBSD 2.2.1)
 * Add support for autodetection of ogdi 3.2 in ./configure (#3007)
 * Remove additional dependency to libgdal.so added during linking in gdal/apps (#2970)
 * Improved ax_oracle_oci.m4 macro to handle libnnzXX for Oracle 10 and 11.
 * support using the Oracle Instant Client SDK
 * Make --with-ingres work with newer versions (#3126)
 * Search for alternative HDF4 flavor in HDF4 testing macro. Alternative HDF4
   (libmfhdfalt/libdfalt) build is NetCDF-compatible and used in Debian.
 * Support MacOSX "fat" binary building

Port :
 * Provide API and implementation for thread and SMP safe atomic increments (#2648)
 * Add /vsisubfile virtual file driver
 * Added gzip write implementation
 * VSI ZIP : Allow natural chaining of VSI drivers without requiring double slash
 * Add a shortcut when looking for .csv files that are already open
 * Add CPLSetThreadLocalConfigOption()
 * Add CPLIsUTF8() and CPLForceToASCII(); Use them in GML, KML, GPX and GeoRSS drivers (#2971)
 * Add CPLStrlcpy() and CPLStrlcat(), clones of BSD strlcpy() and strlcat() functions
 * Add CPLStrnlen()
 * Add CSLLoad2() and CPLReadLine2L() with max number of lines and chars per line allowed
 * cplkeywordparser.cpp: Support parsing IMD files with values of the form list of lists
 * odbc: Fixed the null terminators for columns of length (x*511)+1, where x>1. (#2727)
 * unix VSIF*L : reworked to avoid unnecessary seeks which can be expensive
 * added HTTPAUTH and USERPWD options for http fetch (#3091)

## GDAL 1.7.0 - Overview of Changes

Algorithms:
 * cutline : optimize by clipping cutline to region of interest (#2736)
 * cutline : avoid scanline to scanline blending problems (#2740)
 * rasterfill : substantially reworked
 * rasterfill : deprecate bConicSearch option - unused
 * rasterize : optimized
 * rasterize : Added GDALRasterizeLayersBuf() function to rasterize geometries directly
               into the supplied raster array.
 * rasterize : Add ALL_TOUCHED rasterize option (#2840)
 * rasterize : Added 3D support for rasterizing points and lines. (#3202)
 * rasterize : correct case of vertical/horizontal lines on raster right/bottom edge (#3268)
 * Added GDALCreateGenImgProjTransformer3() function
 * warp: Reduce destination file size, especially when it has compression (#1688)
 * warp: Fix crash when reprojecting to irrelevant SRS (#3079)
 * warp: avoid using the destination nodata value for integer datatypes (#3142)
 * warp: fix panDstValid generation, and avoid using it improperly (#3222)
 * warp: Restore support of reprojection of a lat-long image crossing 180E/180W longitude (#3206)
 * contour: Fix name of GDAL_CG_Create in contour.cpp (#2795)
 * contour: Generate contours with correct orientation (#3129)
 * gdalgeoloc: Improve geoloc backmap interpolation (#2501)
 * overview: added support for cubic convolution overviews
 * gdal_grid: 3 new metrics: data point count, average distance between data points
              and a grid node, average distance between data points.
 * gdal_grid: Properly cast the poOptions parameter in data metrics computation functions. (#3207)

Core :
 * Added mechanism to clear overviews if zero overviews requested. Implemented by GTiff and HFA drivers (#2915)
 * Support for overviews in subdatasets
 * Support for overviews in proxydb directory (#2432)
 * SetColorTable(NULL) is permitted and means delete (#2421)
 * Preserve NBITS and SIGNEDBYTE in CreateCopy() for supporting drivers (#2902)
 * GDALCopyWords() : performance optimizations
 * Add GDALCheckDatasetDimensions() and GDALCheckBandCount()
 * Add GDALGetColorInterpretationByName()
 * Use tiff .ovr for overviews if .aux is just metadata (#2854)
 * Add missing argument in function declaration for GDALRATTranslateToColorTable()
 * Do not use colortable for bit2grayscale overviews (#2914)
 * Support world files with blank lines (ESRI Merge)
 * Add worldfiles in GetFileList() (ESRI Merge)
 * Fix rpb/imd loading with a path (#3047)
 * Add support for using overviews in GDALDataset::BlockBasedRasterIO() (#3124)
 * Take into account SIGNEDBYTE for GetStatistics() & GetHistogram() (#3151)
 * Add GDALReadOziMapFile() and GDALLoadOziMapFile() to read projection
   and georeferencing information from OziExplorer .MAP files.
 * Added declarations for GDALLoadTabFile() and GDALReadTabFile()
 * Add missing case for CInt16 and CInt32 in GDALNoDataMaskBand and GDALNoDataValuesMaskBand

Utilities :
 * gdaldem: new for GDAL/OGR 1.7.0 (#2640)
 * gdalinfo:
    - add -norat switch
    - do not report RPC or GEOLOCATION metadata with -nomd
    - Use pretty wkt to display GCP projection
 * gdalwarp:
    - fix cutline blend distance setting (#2733)
    - in -te case, adjust the resolution after computing the image dimensions
    - improved cutline support (#2733, #2847, #2884, #2984)
    - avoid overwriting an existing destination file that cannot be opened in update mode with a new GTiff file
    - better heuristics to guess output extent when projection from lat/long world
      extent to other world global projections (#2305)
 * gdaltindex:
    - Avoid unnecessary error message in gdaltindex when creating a new shapefile (#2779)
    - Rewritten to use OGR API instead of ShapeLib API, so as to produce .prj files more easily (#982)
 * gdal_contour:
    - make -3d option work even after -fl option (#2793)
    - Call GDALGetProjectionRef() on the dataset, not the raster band (#3062)
 * gdalbuildvrt:
    - Add -separate, -allow_projection_difference, -te, -tr, -q, -addalpha options
    - Add -srcnodata and -vrtnodata options (#3254)
    - Add -hidenodata option (#3327)
    - Avoid accidental overwriting of a non VRT dataset due to reversed filename order
    - Fix -resolution lowest/highest (#3198)
 * gdaladdo: add -clean option (#2915)
 * gdaladdo: add -q option
 * gdal_grid: Add support for spatial filtering with -clipsrc option
 * gdal_translate: support translation of a dataset with subdatasets if the parent has bands
 * gdal_translate: Add 'gray' as a value of -expand rgb option
 * gdal_translate: Add -unscale commandline option
 * gdal_merge.py: Add progress report
 * gdal_vrtmerge.py: Fix -separate case (#2836)
 * gdal_vrtmerge.py: Write the <SourceProperties> element (#1985)
 * gdal_retile.py: add lanczos resampling (#2859)
 * gdal_fillnodata.py: ensure dstfile support works by copying source (#2866)
 * GDAL2Tiles: --srcnodata support + fixed KML rendering for -p raster
 * rgb2pct.py: Added ability to use a preexisting color table from a file (#2958)
 * pct2rgb.py and rgb2pct.py : Copy the GCPs and their projection to the target dataset.
 * classify.py: Fix order of args to numpy.ones() and numpy.zeros() (#3089)
 * hsv_merge.py: New sample script to greyscale as intensity into an RGB image,
                 for instance to apply hillshading to a dem colour relief.
 * support filename globbing for various Python scripts (#2783)
 * --formats will add 'v' in report on drivers that support virtual io
 * all utilities and scripts : consistently advertise -q as the official quiet
                               option, but accept both -q and -quiet (#3820)

Changes in various drivers :
 * Implement support for overviews on subdatasets for HDF4, HDF5, NetCDF, NITF, XPM, TERRAGEN, PCIDSK (#2719)
 * Add support for 64bit offsets in /vsisubfile, JPEG and JP2ECW drivers
 * External overviews support added to some drivers (JDEM, ...)
 * Avoid incorrect GEOGCS AXIS settings (#2713)
 * Use GDALCheckDatasetDimensions() and GDALCheckBandCount() in various drivers
 * Many memory leak fixes (HDF5, HKV, Leveler, MFF, NITF, RMF, JPEG2000, WCS ... drivers)
 * Many fixes to improve robustness against corrupt data in many drivers
 * Error out when trying to open a read-only dataset in update mode (#3147)
 * Ensure that the same JPEG2000 driver that has been used with CreateCopy() is used to re-open it (#1596)

ADRG driver:
 * Support PSP != 100 (#3193)

AIG driver:
 * Differ opening of the RAT only when GetDefaultRat() is called.
   Will improve performances and make less likely the error message of #3031

AAIGRID driver:
 * Fix bad reading of AAIGRID dataset whose last line doesn't have a linebreak character (#3022)
 * Make the ArcInfo ASCII driver more Mac-compatible. (#3212)

BAG driver :
 * New for GDAL/OGR 1.7.0

BLX driver:
 * Implement progress callback in CreateCopy() (#2830)

BMP driver:
 * Modify GetGeoTransform() to return geotransform based on the resolution
   information in the BMP header if nothing else is available (ESRI merge).
 * use pam functions properly for geotransform when not using world file

BSB driver:
 * Fix several issues with BSB files reading (#2782)
 * Handle properly more than 256 GCPs (#2777)
 * Add logic to chip GCPs around to avoid split over dateline problems (#2809)
 * Add logic to reproject GCPs into Mercator for mercator projected images.

DTED driver:
 * Re-enable DTED driver to recognize files not starting by a UHL record (#2951)

ECW driver:
 * Enable the JP2ECW driver to open JP2 files with the VSILAPI
 * Fix build with MSVC2008  (#2850)
 * Fix memory overwrite while zooming an ECW image (#2934)
 * Speed-up de-registration of the driver that can take up to 3 seconds (#3134)

EHDR driver:
 * Add color table update.  Add limited support for floating point
   files (.flt) (ESRI Merge)
 * added support for SIGNEDBYTE pixels (#2717)
 * Restructure stats handling so pam capture works right when stx write fails
 * improve a bit .clr reading (#3253)

ENVI driver:
 * Move RPC info into the RPC domain (#3063)
 * Converted to using VSI Large File API (#3274)
 * re-enabled complex support (#3174)

EPSILON driver:
 * New for GDAL/OGR 1.7.0
 * Mainly used coupled with Rasterlite

ERS driver:
 * Added PIXELTYPE support to report/create SIGNEDBYTE (#2902)
 * Give precedence to PAM SRS, and even check for .aux file.  (ESRI Merge)

FAST driver:
 * Support 7 bands (#3307)

Fujibas driver:
 * Fix to work on big-endian hosts

GenBin driver:
 * Implemented custom class for 1bit (U1) files (#2730)
 * Fix inverse flattening computation (#2755).
 * Added U2 and U4 support
 * Look for LSB, assuming MSB if not found (#2730)

GeoRaster driver:
 * Fix compression problems
 * Add MaskBand support
 * Support UNICODE metadata
 * Support cross database schema/user access
 * add COORDLOCATOR create option

GeoTIFF driver :
 * Add 8/12bit jpeg in tiff support
 * Add support for creating external BigTIFF overview files,
   with BIGTIFF_OVERVIEW configuration option. (#2785)
 * Add support for deleting a color table (#2421)
 * Add logic for Imagine citation parsing
 * Add logic for encoding and reading ESRI PE string from citation.
 * Add support for reading and writing vertical datum info from geotiff
 * Changes to units handling.  (#2755)
 * Optimize opening speed by deferring fetching the coordinate system till GetProjectionRef (#2957)
 * Optimize GTiffRasterBand::IReadBlock() for multi-band interleaved case.
 * Avoid unnecessary re-writing the TIFF directory (#3021)
 * Use official value for inverse flattening of the WGS84 ellipsoid (#2787)
 * Add metadata domain for XML documents (#2786)
 * Make GTiff driver friendly with files with huge number of bands and pixel interleaving (#2838)
 * Avoid precaching other bands if block cache size is not big enough to accommodate them (#2838)
 * Internal libtiff (4.0.0beta5) and libgeotiff (1.3.0beta) upgraded
 * use the SetCitationToSRS call for the PCSCitationGeoKey in a similar fashion to the GTCitationGeoKey (#2933)
 * NBITS set for GTiffOddBits.  YCbCr JPEG added as a compression type.
   generate MINISWHITE IMAGESTRUCTURE metadata item.  Set missing blocks
   to the nodata value if there is one.  (ESRI Merge)
 * Support GeoTIFF with only ProjectedCSTypeGeoKey defined (#3019)
 * External overviews: try to preserve the bit depth of the original image
 * Allow reading and creation of big all-in-one-strip 8bit TIFF (#3094)
 * Handle projection methods for Google Mercator special case (#3217)

GFF driver :
 * Fix support for big endian host (#2832)
 * Add pam, and overview support.  Switch to VSI*L API to support virtualio (#3014)

GIF driver :
 * Introduced a BIGGIF driver to handle GIF files without using the
   slurp into memory approach. (#2542)
 * CreateCopy() reports now progress
 * Replace internal libungif by giflib-4.1.6 (#1825)
 * Read projection and georeferencing from OziExplorer .MAP file if possible.

GRASS driver:
 * Add support for GRASS 7.0 GDAL and OGR plugins (#2953)
 * Use GRASS_GISBASE for GDAL GRASS driver instead of hard-coded path (#2721)

GRIB driver:
 * only scan for PDS templates in GRIB2 files (#2858)
 * Avoid dumping GribLen debug message if built with -DDEBUG.
 * Remove verbosity from GRIB driver (#2887)
 * Make GRIB detection thread safe (#3209)
 * Check that bands have the same dimensions (#3246)

GS7BG driver:
  * Recognize version 2 datasets (#3123)

HDF4 driver :
 * Allow HDF4 subdataset name to include Windows letter drive (#2823)

HDF5 driver :
 * subdatsets need to be numbered from 1 not 0 (#2462)
 * Block size recognition. (#2270)
 * Fix initial value for nGCPCount in HDF5ImageDataset (#2774)
 * Fixes to type classification, and to avoid listing subdatsets for unsupported pixel data types (#2941)
 * Mark PAM for not saving .aux.xml files at this level.  Directly open
   subdatasets in HDF5Dataset::Open() if there is only one subdataset.

HFA driver:
 * Support reading and evaluating 3rd order xforms (#2716)
 * Various improvements to SRS support, particularly to preserve PE
   compatibility.  (#2755)
 * Added HFAEntry::RemoveAndDestroy() method to remove nodes from tree (#2421)
 * Added support for deleting color tables (#2421)
 * Add a scaled progress monitor for HFADataset::IBuildOverviews()
 * Fix HFA u2 compression/decompression (ESRI merge)
 * Add support for reading compressed s8 HFA (#3152)
 * Defer opening overviews till they are first requested (#3155)
 * Support multiple excluded values (#3252)
 * added a variety of additional coordinate system based types missing in some files (#3262)
 * Various fixes (#2421, #2842, #2755, #3082, #2730)

Idrisi driver:
 * Writing text file in CRLF format (#3199)
 * forward porting esri changes + other changes
 * provide default values on Create() (#3243)

INGR driver:
 * Fix INGR driver that was failing on big endian hosts (#2898)
 * Fix RLE support (#3106)
 * Added overview building (#2904)

JPEG driver:
 * Enable the JPEG driver to read and create 12bit JPEG images when JPEG12_ENABLED=yes
 * Internal libjpeg: additional changes to ensure operation with IPP enabled apps (#2606,#2845)
 * JPEG read optimizations : differ extraction of EXIF metadata and internal maskband

JPEG2000 (JasPer) Driver:
 * Allow proper reading of JP2 images where dimensions are not multiple of 256 (#2399)
 * Add a virtual I/O interface to able to read from/write to JPEG2000-NITF files
 * Do not deregister jas_image_clearfmts() to avoid failure when gdal_translat'ing from JP2 streams
 * Add proper reading of YCbCr images as RGB
 * fix decoding of bit depth for BPCC and PCLR boxes

JP2KAK (Kakadu) Driver :
 * Fix band selection from ycbcr to rgb converted images in DirectRasterIO (#2732)
 * Support jpc vsisubfile streams
 * add handling of reversibly compressed data with 9 to 16 bits precision (#2964)
 * Modify transfer_bytes() buf32 case to offset/scale based on precision. (#2964)
   Fixed _WriteTile() lossless 16bit case to avoid improper 32K offset.
   Added support for NBITS image structure metadata, and creation option.
 * Added logic to limit tiles to 64K due to jpeg2000 limitation. (ESRI Merge)
 * Fix offsetting of 16U buf32 data (#3027)
 * Support 16u/16s imagery through DirectRasterIO interface (#3049)
 * Support external overviews as an override to internal overviews
 * Rework jp2kak support to use natural kakadu builds (Windows build)
 * ensure external overviews get used if available (#3276)
 * add preliminary multi-threading read support via DirectRasterIO()


LAN driver:
 * Give preference to PAM coordinate system since built-in info is very
  limited.  Fallback to PAM geotransform if internal missing. (ESRI Merge)

LCP driver:
 * Add projection file support (#3255)

MEM driver:
 * Allow creating bands of more than 2GB each if size_t is large enough.
 * Added GetInternalHandle() implementation to fetch band memory buffer

MrSID driver:
 * Implement faster resampling for 1:1 case
 * Improve stream implementation so it works for jp2 with v7
 * Make the JP2MrSID driver accept .ntf extension to allow reading jpeg2000
   datastream in NITF files
 * Avoid reporting large metadata objects.  Add MG version to metadata.

NetCDF driver:
 * Fix handling of pixel validity mask (#3112)
 * correct a problem with 5+ dimensional data access (#2583)
 * fix y flip detection in common case (#2654)
 * add support for subdataset names with Windows full path names, like NETCDF:D:\...

NITF driver:
 * Add support for reading & creating large (>4GB) NITF files.
 * Add support for NITF/JPEG2000 overviews (JP2KAK)
 * Add support for reading & creating 12bit JPEG compressed NITF files when JPEG12_ENABLED=yes
 * Add support for creating a NITF file with many bands and big TRE content
 * Add support for creating several uncompressed images in a NITF file (#2989)
 * Add support for creating M3 (masked multi-block JPEG compressed) images
 * Add support for unpacking a wider variety of pixel depths.
 * Add support for overriding IGEOLO with GEOLOB TRE precision georef (#3180)
 * Add support for for CFloat32 reading & writing (#2526)
 * Add support for reading and writing NITF file with large single block (#3263)
 * Allow Jasper driver to be used for NITF IC=C8 (JPEG2000) CreateCopy() if JP2ECW is not available
 * Allow JP2MrSID driver to be used for reading JPEG2000 datastreams in NITF
 * Avoid issues when reading M3 single block images
 * Fix CreateCopy() of multi block JPEG-NITF
 * Various bugfixes (#2940, #2912, #3029, #3088)
 * Support NITF file with a color table and JPEG2000 data content (#3110)

NWT_GRC / NWG_GRD drivers (Northwood/VerticalMapper) :
 * New for GDAL/OGR 1.7.0

OGDI driver:
 * improve finding of PROJ.4 include files for OGDI (#1242)

PCIDSK driver (old driver):
 * Added worldfile reading. Added PAM fallback for geotransform.
 * Added support for default overviews (i.e. .ovr or .rrd).  (ESRI Merge)
 * fail somewhat gracefully on compressed images

PCIDSK driver (new driver):
 * New for GDAL/OGR 1.7.0, using the PCIDSK SDK

PDS driver:
 * Transfer various keywords to metadata
 * Made keyword handler more in complaint with ODL (#2956)
 * Support detached files with an offset (#3177)
 * Support .LBL labelled compressed files

PNG driver :
 * Upgrade internal libpng to 1.2.35
 * Only write a world file if the source datasource has a geotransform
 * Allow writing a nodata value of 0 as the transparent color value (#3208)

R driver:
 * New for GDAL/OGR 1.7.0

Rasterlite driver
 * New for GDAL/OGR 1.7.0

RIK driver:
 * Improved error checking

SAGA GIS Binary driver:
 * New for GDAL/OGR 1.7.0

SDE driver :
 * Fix exporting ArcSDE raster results in a displaced image (#2063)

SRP driver (ASRP/USRP):
 * New for GDAL/OGR 1.7.0

SRTM driver :
 * Set GDALMD_AOP_POINT metadataitem (#1884)

TIL driver (EarthWatch .TIL) driver:
 * New for GDAL/OGR 1.7.0

VRT driver :
 * Honour the INIT_DEST warp option (#2724)
 * Improve performance of LUTs in VRTComplexSource from O(n) to O(log2(n)) (#3003)
 * Implement (advertized in doc) support for SetMetadataItem( "source_0", szFilterSourceXML, "vrt_sources" ) on a VRTSourcedRasterBand (#3052)
 * Implement GetFileList() to list the source files
 * Fix wrong initialization of destination buffer in VRTSourcedRasterBand::IRasterIO() in case of not standard pixel/line spacing. (#2867)

WCS driver:
 * do not try to parse HTML content, which is returned by some provider when the server doesn't exist
 * added HttpAuth and UserPwd options for authentication (#3091)

WKT Raster driver:
 * New for GDAL/OGR 1.7.0

WMS driver:
 * Support TMS/formatted URLs in WMS minidriver (#2878)
 * Be tolerant if we have required 3 bands and got 4, or the other way round
 * Declare a user agent string

## OGR 1.7.0 - Overview of Changes

Utilities:
 * ogrinfo: Preserve order of source layers specified on ogrinfo command line,
            and use GetLayerByName() which enables to read some hidden layers
            like public.<table> layers from a PG database (#2922, #2026)
 * ogr2ogr:
    - Add -clipsrc and -clipdst option to clip geometries to the specified extents
    - Add -fieldTypeToString option to conveniently cast any fields of given type to fields of type string (#2968)
    - Add -progress option for ogr2ogr to displaying progress (#2998)
    - Add -wrapdateline option to ogr2ogr to deal with geometries that cross 180 degree longitude (#3158)
    - Add -dialect flag to specify SQL dialect
    - Preserve order of source layers specified on command line (#2922)
    - -overwrite and -append now automatically imply -update (#3048)
    - Support converting to a format after field name "laundering" (#3247)
  * ogrtindex:
    - Skip layers whose schema does not match instead of terminating (#3141)
    - Add a -accept_different_schemas option for non-MapServer use cases (#3141)
    - Set SRS to tileindex when one is found in the tiles
  * ogr2vrt.py : new script that will create a VRT corresponding to a
    source datasource (sample script for the moment, not promoted officially)

Core :
 * Improved OGR feature style (#2875, #2808)
 * Considerable speed-up of ExportToWkt() for very large geometries
 * Added new OGR_GEOM_AREA special field (#2949)
 * ensure forceToMultiLineString() works for MultiPolygons (#2735)
 * Various fixes in OGR SQL engine (r16116, #2996, #2788, #3143, #3144)
 * Add OGREnvelope::Intersect()
 * Add OGR_G_ApproximateArcAngles() for ellipses
 * Fix crash on Ubuntu 8.10 in GetFieldAsString() because of (too) strict guard logic (#2896)
 * add field type max so we can iterate through all possible values
 * Avoid making a 2D5 geometry from a 2D only linestring when reprojecting

OGRSpatialReference :
 * Upgrade EPSG derived files to EPSG 7.1
 * Added support to operate on COMPD_CS coordinate systems
 * Added support for importing spatial reference definitions from the OziExplorer .MAP files.
 * Introduce static methods to destroy OGRSpatialReference and OGRCoordinateTransformation objects
 * Expose more of the axis orientation API to C
 * Add missing Eckert 1, 2, 3 and 5 projections
 * Fix typos in proj4 conversion for Wagner projections
 * Hack in EXTENSION nodes for Google Mercator (#3136)
 * Validates PROJCS with AXIS definitions (#2739)
 * Added support for urn:ogc:def:crs:OGC::CRS:84 (and CRS:83, CRS:27) per WMS spec.
 * Wide variety of improvements to preserve PE strings through a morphFromESRI()
   and morphToESRI() process (#2755)
 * Fix inversion of dictionary filename and GEOGCS/PROJCS name in OGRSpatialReference::exportToERM() (#2819)
 * Fix SpatialReference::IsSame() for LOCAL_CS case (#2849)
 * Fix bug in ImportFromXML that prevented from retrieving projection method
 * Accept both href and xlink:href in OGC XML
 * improve us foot translation handling (#2901)
 * OGRSpatialReference::importFromUrl() : add a default 10 second timeout to avoid waiting forever when remote server is stalled
 * ensure we can translate mercator1sp with non-zero origin to proj4 (#3026)
 * ensure scalefactor preserved in somerc translation (#3032)
 * SRS_ESRI: attempt to correct equidistant cylindrical parameter morph (#3036)
 * SRS_ESRI: improve plate_carree parameter morphing (#3036)
 * SRS_PCI : Fix PCI projection string handling for UTM
 * esri_extra.wkt: correct equidistant conic definitions (#3086)
 * SRS_PANORAMA : Added support for British National Grid and Pulkovo 1995 datums.
 * Improve recognition of WKT text strings when translating into proj4 hard-coded datum names, in particular nzgd49
   (also add ggrs87, carthage, hermannskogel, ire65); Fix ellipsoid parameters for modified airy (#3104)
 * OSRGetEllipsoidInfo() available for various OGR SRS modules
 * added support for OGC:CRS84 and similar srses from wms/wcs spec in SetFromUserInput (#3090)

BNA driver :
 * Fix output of BNA driver with polygons with inner ring (#2985)
 * Proper CRLF output on Windows (#3256)

CSV driver :
 * Add support for reading and writing CSV files with semicolon or tabulation
   as the field separator (#2925, #2136)
 * Add automatic treatment of WKT column as geometry
 * Add 'CREATE_CSVT' layer creation option

DXF driver :
 * New for GDAL/OGR 1.7.0

Geoconcept driver:
 * Fix 'private Class and SubClass headers are inverted' (#2919)
 * Fix error in writing 2.5D/3D export file (#2921)

GeoJSON driver:
 * updated JSON-C library to version 0.9
 * use VSIF*L API

GML driver :
 * Speed-up considerably parsing of GML coordinates in big geometries, in particular on Windows
 * Add support for gml3.1.1 srsDimension attribute, to deal with 3D geometries (#2311)
 * Support multiple <gml:pos> elements in linearrings of polygons (#3244)
 * Limited support for GML3
 * Support direct use of FIDs as long as they are all numeric or they have a completely fixed prefix (#1017)
 * Fix OGRGMLLayer::GetFeatureCount() if there's a .XSD file available (#2969)
 * Added support for out-of-band attributes on features (for NAS)
 * Adding the date field type to xsd writer and precision info for OFTReal fields. (#2857)

GPX driver:
 * Add GPX_SHORT_NAMES configuration option to make the GPX driver report shorter field names
   and avoid duplicated field names once translated to shapefile (#2966)
 * Write the <bounds> element (write only)
 * Avoid escaping XML content when writing <extensions>.
 * Add appropriate xmlns when detecting Garmin GPX extensions

GRASS driver:
 * Do not report 3D geometries for 2D GRASS maps (#3009)

GTM (GPSTrackMaker) driver :
 * New for GDAL/OGR 1.7.0 (#3113)

ILI driver :
 * Improved curve segmentation algorithm
 * ILI1: Support for multiple point geomtries
 * ILI1: Support Real and Integer column types

Ingres driver:
 * Utilize the new OGC based ingres capabilities (#3159)

KML driver:
 * Support reading MultiGeometry and layers with mixed geometry type.
 * Speed-up considerably the reading of huge KML geometries (#3005)
 * Speed-up considerably with huge number of layers
 * Moved the location of the Style element to match the OGC Schema (#2858)
 * Advertise 25D geometry type when relevant (#1803, #1853, #2181)
 * Relax KML driver about xmlns (#3004)

MySQL driver :
 * Fix mysql driver compilation with mysql 5.1/g++-4.3.2 (Mandriva 2009.1) (#2972)
 * Fixed bug MySQL driver truncating decimal places for double field type. (#2852)

OCI driver :
 * OCI varchar2 columns can be up to 4000 bytes (#2876)

ODBC driver :
 * make it slightly less likely that the srs_tablename parsing will interfere with complex DSNs.
 * support for schemas (#1969)

OGDI driver :
  * fix to avoid applying old spatial filter to unrelated layer
  * fix to force ResetReading() when changing current layer

PGEO driver :
 * Recognize more ESRI shape type constants. (#2991, #3100)
 * implement GetFIDColumn() and GetGeometryColumn() methods (#2694)

PostgreSQL driver:
 * Add support for tables with 'geography' column type introduced in PostGIS 1.5 (#3216)
 * Extend support of schemas in PG driver with 2 new options in the connection string:
   active_schema=foo and schemas=foo[,bar] (#522 and #525)
 * Implement OGRPGTableLayer::CreateFeature() by using UPDATE instead of DELETE + INSERT (#2557)
 * Implement SetNextByIndex() for layers of PG datasources (#3117)
 * Support PG 'real' data type in tables (#3006)
 * Speed-up PG database opening by avoiding 2 SQL requests per table
 * Avoid evaluating GetFieldIndex() on each field of each feature,
   which can be very expensive if the layer has many fields
 * allow ST_AsBinary with non binary connections
 * added a configuration option PG_SKIP_VIEWS.

GeoRSS driver :
 * New for GDAL/OGR 1.7.0 (#2726)

OCI driver:
 * support blob column binding

PCIDSK driver :
 * New for GDAL/OGR 1.7.0

Shape driver :
 * Handle duplicate field names in shapefile driver. (#3247)
 * Support for opening and handling .DBF files > 2 GB (#3011)
 * Optimize to use shape bounds for spatial test before organizing poly (#2775)
 * Support for alternate date format (#2746)
 * Improve/fix TestCapability() on OGRShapeLayer
 * Refreshed shapelib from upstream

S57 driver :
 * Fix incorrect return value of GetFeatureCount() on S57 SOUNDG layer when SPLIT_MULTIPOINT=ON;
   also avoid warning on that layer when ADD_SOUNDG_DEPTH=OFF (#3163)

SQLite driver:
 * Add creation and write support in SpatiaLite-compatible databases
 * Add SPATIAL_INDEX creation option for SpatiaLite tables if linked against
   libspatialite (default to YES)
 * Implement OGRSQLiteTableLayer::TestCapability(OLCFastFeatureCount)
 * Implement OGRSQLiteLayer::GetFIDColumn() and GetGeometryColumn()
 * Implement TestCapability(OLCRandomRead)
 * Add a SQLITE_LIST_ALL_TABLES configuration option to list all(non-spatial)
   tables into a SQLite DB even if there are spatial tables
 * Avoid reporting the primary key column as a regular column.
 * Better precision for double values in CreateFeature()

VFK driver:
  * New for GDAL/OGR 1.7.0

VRT driver:
 * Allow fast spatial filtering in the VGS_Direct case
 * Add support for CreateFeature(), SetFeature() and DeleteFeature() operations
 * Added field definition and style control
 * Added new vrt/schema creation capability (@dummy@ datasource, ogr2vrt.py script)
 * Implement 'SrcRegion' element
 * Add a 'reportSrcColumn' attribute to the 'GeometryField' to avoid reporting
   the x,y,wkt or wkb source fields in the VRT layer field definition
 * Forward TestCapability(), GetExtent(), SetNextByIndex() to source layer when possible

XPlane/Flightgear driver:
 * Improve handling of Bezier curves (#3030)
 * Support new file names used by XPlane 9.00 & later
 * Cut into 2 pieces airway segments that cross the antemeridian
 * Add new layer 'Stopway' that contains the shape of the stopway/blastpad/over-run of a runway
 * Recognize code 16 and 17 for seaplane bases and heliports and add a new field to APT layer

## SWIG Language Bindings

General:
 * Recommended SWIG version is 1.3.39
 * Added API :
    - GDAL :
        gdal.FilldoData(), gdal.FileFromMemBuffer(), gdal.Unlink()
        gdal.ApplyGeoTransform(), gdal.InvGeoTransform()
        Band.GetUnitType(), Band.GetBand()
        Band.ComputeStatistics(), Band.HasArbitraryOverviews()
        RasterAttributeTable.GetLinearBinning()  and SetLinearBinning()
        extend [Band|Dataset].[ReadRaster|WriteRaster] to accept pixel, line and band spacing parameters
    - OGR:
        ogr.GeneralCmdLineProcessor(), Geometry.Segmentize(), FieldDefn.GetTypeName(),
        Geometry.ApproximateArcAngles()
    - OSR :
        osr.ImportFromMICoordSys(), osr.ExportToMICoordSys(), SpatialReference.Clone()
        osr.EPSGTreatsAsLatLong(), osr.ImportFromEPSGA()
 * Make resampling an optional parameter for gdal.RegenerateOverview(),
   to be consistent with gdal.RegenerateOverviews()
 * NONNULL checks have been added to check various arguments of methods
 * add missing constants : DCAP_VIRTUALIO, color interpretations, OGR constants

CSharp bindings :
 * Add support for GetFieldAsStringList, GetFieldAsDoubleList and
   GetFieldAsIntegerList in the C# wrapper (#2839)
 * Support MSVC2008 builds with the csharp interface (#2862)
 * Change the dll mapping rules to support the recent MONO versions
 * Use GC pinned arrays instead of the double copy in the RasterIO functions (#3073)
 * Add typemaps to support custom CPLErrorHandler via C# swig bindings

Perl bindings :
 * in Polygonize make a local copy of the parameters as they are potentially edited

Python bindings :
 * Add support for Python 3.X. Compatibility with Python 2.X preserved (#3265)
 * Remove old-generation Python bindings.
 * Add Python binding's version description capabilities (#3137)
 * NUMPY : Make Band|Dataset.ReadAsArray() take into account preallocated array (#2658, #3028)
 * Various memory leaks fixed
 * Fix gdal.RegenerateOverviews(), Feature.GetFieldAsStringList(),
   Feature.GetFieldAsIntegerList(), Feature.GetFieldAsDoubleList(),
   Transform.TransformPoints and CoordinateTransformation.TransformPoints
 * Extend python TransformPoints typemap to accept any sequence (#3020)
 * Make Geometry iterable (#1886)

Java bindings (general changes):
 * Maintained again. A lot of changes to get them into clean state.
 * RasterIO API : added API for standard Java arrays in addition to DirectByteBuffer
 * Javadoc available at http://gdal.org/java

# GDAL/OGR 1.6.0

## GDAL/OGR 1.6.0 - General Changes

Build (Unix):
 * Added basic support for LDFLAGS
 * Try prefix/lib before prefix/src for proj.4 linking (#1345)
 * Allow specification of a python binary for --with-python (#2258)
 * Added NAS driver config support
 * Fixed Expat detection problem on MinGW (#2050)
 * Fix INST_DATA setting (/share/gdal instead of /share) (#2382)
 * Build MSGN driver on Unix-like platforms
 * Added MSG driver support to configure.in. EUMETSAT Wavelet Transform software is only detected on request, option --with-msg specified.
 * Improve cross-compilation
 * Fix linking with HDF4 library when configuring with --with-hdf4 or --with-hdf4=yes (#2602)
 * Fixes for compilation with GCC 4.3

Build (Windows)
 * Default to non-debug builds.  Use /GR in pre 1400 builds so that dynamic_cast doesn't just cause an blowout.  Use .pdb file with version embedded.
 * Make vc++ builds usable from mingw (#2216)
 * Updated nmake.opt for building with Visual C++ adding /W3 flag for release build and /W4 for debug build.
 * Add VS Makefile for GDAL and OGR DODS drivers (#2383)

Build (All)
 * Remove support for "Panorama" GIS SDK (#2669)

Port:
 * RFC 19: Added VSIMalloc2() and VSIMalloc3() API and use them in GDAL drivers
 * RFC 23: Added implementation of recode API
 * Added infrastructure to detect bad arguments to printf-like functions
 * Added CPLHashSet data structure
 * Added quad tree implementation derived from shapelib & mapserv
 * Added support for reading on-the-fly .gz files and .zip files (#1369)
 * Added CSLFindName()
 * Added two new flags to CSLTokenizeString2() function: CSLT_STRIPLEADSPACES and CSLT_STRIPENDSPACES to strip leading and ending spaces from the token.
 * Added CSVGetNextLine() to fetch next record based on just csv filename
 * Added CPL_ACCUM_ERROR_MSG=ON for CPLQuietErrorHandler
 * Added CPL_MAX_ERROR_REPORTS config option (#2409).
 * Added CPL_INLINE macro
 * Added UNREFERENCED_PARAM macro to cpl_port.h.
 * Added CPLGenerateTempFilename()
 * Improve performance of CPLParseXMLString from O(n*n) to O(n) where n is the number of siblings node
 * Fix bug with url encoding in CPLEscapeString() (#2314)
 * Various fixes in CPLList implementation (CPLListInsert and CPLListRemove) (#2134)
 * VSIMEM: added path normalization so everything is converted to forward slashes
 * VSIMEM: prevent file extension, or write/update to files opened in readonly mode
 * cpl_path.cpp: Add CPLAssert to check that the string inputs are not the result static buffer

Utilities:
 * Added a --utility_version that displays the version of GDAL used for compiling the utility and add runtime checks to see if GDAL library is compatible with the utility version

## GDAL 1.6.0 - Overview of Changes

Core :
 * RFC 22 : Added RPC and IMD support
 * Added support for computing statistics for datasets with arbitrary overviews in GDALRasterBand::ComputeStatistics()
 * Added Gaussian resampling in overview building (#2137)
 * Added Mode resampling in overview building (#2347)
 * Allow fast NONE overview generation (#2677)
 * Added in GDALRasterBand::GetRasterSampleOverview() and ComputeRasterMinMax() (#2148)
 * Preliminary gmljp2 specific changes to address axis orientation (#2131)
 * Added GDALProxyDataset and GDALProxyRasterBand abstract classes to be able to make proxy datasets and raster bands
 * Added a proxy dataset class, GDALProxyPoolDataset, that differ at the maximum the opening of the underlying dataset and keep the number of simultaneous opened underlying datasets under a limit (similar to what is done in the RPFTOC driver)
 * Migrate GDALRegenerateOverviews() to C API
 * Added GDALDestroyDriver()
 * Added special case in GDALCopyWholeRaster to be more friendly when writing a tiled compressed GeoTIFF (massive reduction of destination file size)
 * Added GDALRegenerateOverviewsMultiBand to process all the bands at the same time to optimize the generations of compressed pixel-interleaved overviews (such as JPEG-In-TIFF). Optimization triggered in some cases for external and internal GeoTIFF overviews. PHOTOMETRIC_OVERVIEW and INTERLEAVE_OVERVIEW config options added for external overviews. -ro option added to gdaladdo to generate external overviews for GeoTIFF. Result : divide by 2 to 3 the size of JPEG-In-TIFF overviews on big RGB datasets
 * Add a new class GDALNoDataValuesMaskBand to create a per-dataset nodata mask band (GMF_PER_DATASET | GMF_NODATA) when the metadata item NODATA_VALUES is found on the dataset (#2149)

 * Fix segfault when building overviews with --config USE_RRD YES (#2145)
 * PAM: save floating point nodata in IEEE floating point binary format
 * Fix division by zero in GDALGetRandomRasterSample (#2429)
 * GDALOpen: Use EOVERFLOW if defined otherwise use hardcoded likely values (#2437)
 * Replace implementation of arrays for maintaining the list of opened shared datasets by a CPLHashSet to avoid O(n*n) complexity
 * Fix GDALRasterBand::IRasterIO fails to read data from dataset when block cache is too small (#2457)
 * Modify GDALFindAssociatedAuxFile() to only select .aux files that have the same raster configuration as the target dataset (PxLxB).  (#2471).
 * When available use arbitrary overviews for computations in GDALRasterBand::ComputeRasterMinMax() and GDALRasterBand::GetHistogram().
 * Fix crash in GDALValidateCreationOptions when passed a creation option not in format key=value (#2499)
 * Fix 'GDALNoDataMaskBand::IReadBlock doesn't behave correctly when eWrkDT != eDataType' (#2504)
 * Use nodata masks when generating overviews (#2149)
 * Improve error propagation when GDALRasterBlock::Write() fails (#2524)
 * gdalnodatamaskband: add case for uint16 and uint32 as uint32 - fixes neg. nodata values for these
 * Add a special case for dealing with RasterIO expansion on writes (#773)
 * Add GDALValidateCreationOptions() checks in GDALDriver::Create() and GDALDriver::CreateCopy(). Can be disabled with GDAL_VALIDATE_CREATION_OPTIONS=NO
 * Optimization of GDALCopyWords for transfer from buffer of packed bytes to buffer of bytes with interleaving (#2536)
 * Use BlockBasedRasterIO in GDALDataset::IRasterIO for pixel-interleaved datasets when source and destination have the same size (#2536)
 * PAM: Allow empty category in .aux.xml  (#2562)
 * GDALDataTypeUnion(): Add missing GDT_CInt16 case that was triggering a CPLAssert(FALSE) (linked to #2564)
 * PAM: improve the find existing histogram logic to check approx and out of range
 * Fix validation of values for creation option parameters of type float
 * Fix memory leak related to PAM histograms
 * Restrict dataset sharing to a one thread by tracking owning pid (#2229)
 * rasterio.cpp: Handle >2GB memory arrays on a 64-bit build (#2199)

Algorithms:
 * Added GDALPolygonize() function
 * Added sieve filter
 * Add implementation of raster hole filler
 * Added proximity algorithm implementation
 * Added GDALRasterizeLayers() function to rasterize the all the features in the list of layers rather than individual geometries.
 * Added support for point geometries rasterization
 * Added line rasterization routine.
 * Added GDALCreateGenImgProjTransformer2()

 * warper: massive upgrade that fixes number of problems with Cubic Spline and Lanczos resamplers, multiple performance improvements.
 * Implement overview building for paletted rasterbands with average resampling by selecting the nearest entry after averaging on R,G,B components (#2408)
 * Fix destination coordinate system setting logic in GDALReprojectImage (#2231)
 * Modify GDALChecksum to give it a deterministic behavior when given a GDT_[C]Float[32|64] rasterband with NaN and Inf values. The result is backward compatible with previous implementations with finite values
 * Add options to RPC transformer, use for RPC_HEIGHT offset
 * TPS : fix uninitialized variables (#2300), fix wrong behavior with negative coordinates  (#2615)
 * gdalgeoloc.cpp : Fix crash in GDALCreateGeoLocTransformer if X_DATASET, etc... cannot be opened (#2434)
 * warper: Prevent crashes when srcAlphaBand and dstAlphaBand are wrong
 * Fix tiling in gdal_grid and output of geotransform when -txe and -tye not specified (#2508)
 * warper: Wait for the threads to complete before leaving GDALWarpOperation::ChunkAndWarpMulti() (#2518)
 * warper: When warping an RGBA image whose borders have alpha=0, avoid writing alpha=255 with bilinear, cubic, cubic spline resampling
 * warper: Properly set a resample window size for Cubic Spline kernel. (#2414)
 * gdalsimplewarp.cpp: fix pointer array allocation (#2586)

Utilities:
 * gdal_lut.py: New for 1.6.0. Sample app applying a greyscale lookup table
 * gdal_polygonize.py : New for 1.6.0
 * gdal_proximity.py : New for 1.6.0
 * gdal_sieve.py: New for 1.6.0
 * densify.py: New for 1.6.0. A generic Translator class for ogr2ogr-like operations that can be easily overridden for special translation operations
 * gdalflattenmask : New utility to merge regular data bands with the mask bands, for applications not being able to use the mask band concept. (Not compiled by default)
 * gdal2ogr: New for 1.6.0. to create an OGR datasource from the values of a GDAL dataset; May be useful to test gdal_grid and generate its input OGR file (Not compiled by default)

 * Fix crash in gdalenhance
 * Add -nln switch for gdal_contour
 * gdalgrid: Fixed zero search radius handling in nearest neighbor interpolation method.
 * gdalgrid: Added support for data metrics (minimum, maximum and range) computation.
 * gdalgrid: Added spatial filter support applied on the OGR data source
 * gdalgrid: Added ability to read values from the attribute field using the "-zfield" option.
 * gdalgrid: fix crash on features with NULL geometry or when no point geometry is found on a layer (#2424)
 * esri2wkt.py: Fix esri2wkt for NG python bindings (#2548)
 * Build testepsg utility by default when OGR is enabled (Ticket #2554).
 * gdaltranslate: new -expand rgb|rgba option to make color table expansion
 * gdaltindex: Use correct index variable name
 * gdal2tiles.py: Bug fix: switched axes in BoundingBox of tilemapresource.xml.
 * gdal2tiles.py: Bug fix: wrong Origin in tilemapresource.xml.
 * gdal2tiles.py: New version of GDAL2Tiles (SoC 2008 - GDAL2Tiles Improvements)
 * gdal_retile.py: Some minor enhancement optionally storing the  georeferencing data of created tiles in a csv file
 * gdal_vrtmerge.py: add support for NODATA
 * gdalinfo: Don't show RAT if -nomdd is used.
 * gdalinfo: Display checksums on overviews when -checksum is specified
 * gdalinfo: Display whether the mask band has overviews
 * ogr2ogr: reset -gt to 1 with -skipfailures, document -gt (#2409)
 * ogr2ogr: Output error messages on stderr to be consistent; Make error message about failed reprojection more clearer (hopefully); Advertise the use of -skipfailures in error message (#2588)
 * nearblack: Add support for scanning from top and bottom as well as from the sides.
 * Prevent crash in gdalwarpsimple utility and in GDALSimpleImageWarp() when source dataset has no raster band
 * gdal_rasterize: check that coordinates systems match (Ticket #1937)
 * gdalwarp: Add cutline support

Various drivers:
 * Reports GDAL_DCAP_VIRTUALIO=YES for drivers supporting it (#2193)
 * Add warnings in CreateCopy() methods for drivers not supporting color tables (#1939)
 * Simplify and harmonize how ESRI world file extensions are handled for BMP, GIF, JPEG, PNG and GTiff drivers (fix #1734)
 * Windows : enable bigtiff by default when using internal libtiff (#2257)
 * Added plugin building support for ECW, MrSID, HDF5, NetCDF, FITS and SDE drivers.
 * fix case of capabilities xml elements to match properly (#2322)
 * Add ALPHA creation option in the list of creation options

AAIGrid Driver:
 * Re-fix nodata test when determining AAIGrid data type (Ticket #2107).
 * fix yllcorner computation (#1794)
 * mark driver as supporting virtualio.
 * Fix wrong data type used to read the source band AAIGCreateCopy (#2369)
 * Add DECIMAL_PRECISION creation option for AAIGRID (#2430)

ADRG driver:
 * Initialize PAM to avoid creating .aux.xml file when gdalinfo an ADRG dataset
 * Prevent opening in update mode && fix reading of blocks in creation mode
 * Prevent error message coming from ADRG driver when trying to open in update mode a non-existing dataset
 * Avoid error reporting and subsequent failure on some DIGEST files where the last record ends in a non-standard way

BLX Magellan Topo driver:
 * New for 1.6.0 (#2254 and #2570)

BSB driver:
 * Add support for alternate palettes via config option
 * Fix Valgrind warning about read of uninitialized memory; Replace a CPLAssert by a test in case of corrupted dataset; Remove static buffer in BSBReadHeaderLine to improve thread-safety
 * Update BSB write support to use VSIF*L API (still disabled by default); fix palette handling (last color entry was lost); add GCP/Geotransform writing

COSAR driver:
 * Initialize integral variables to prevent failure of test condition in case file read operation fails; clean signed/unsigned mismatch warning.
 * Additional tests to protect against corrupted datasets

DIMAP driver:
 * Fix use of static CPL buffers
 * Implement GetFileList() and support for opening directory for DIMAP format
 * Add metadata at the raster band level by reading the Spectral_Band_Info tag

DODS driver:
 * Add using namespace libdap for version 3.8.2 (#2404)

DTED driver:
 * Add the GDAL_DTED_SINGLE_BLOCK config option to make a single block of a whole DTED file. This can speed-up a lot scanline oriented algorithms (#1909)
 * Add support for DTED products strictly following MIL-D-89020 that was buggy
 * Fix compilation without CPL
 * Improve thread safety

ECW driver:
 * Fix memory leaks (#2210)
 * Be more careful deciding what geotransforms to ignore (#1935)
 * Rename ecw plugin gdal_ECW_JP2ECW.so/dll (#2320)
 * Properly set default nPixelSpace and nLineSpace in ECWRasterBand::IRasterIO().
 * Added pixel data type checking in ECWCreateCopy() (#2593).

EHDR driver:
 * Port EHDR driver to large file API (by apetkov) (#2580)

EIR (Erdas Imagine Raw format) driver:
 * New for 1.6.0

ENVI driver:
 * fix problems with old/new state plane zone codes (#2227)

ERS driver:
 * add support for HeaderOffset keyword (#2598)

FAST driver:
 * Order the GCP in TL, TR, BR, BL order to benefit from the GDALGCPsToGeoTransform optimization
 * Add support for Euromap FAST datasets for IRS-1C/IRS-1D PAN/LISS3/WIFS (#2329)
 * Fix USGS projection decoding for EOSAT FAST format Rev C (#2381)
 * Add support for detection of FAST IRS P6 and CARTOSAT-1 band filenames

FITS driver:
 * Fix crash in FITS when dataset has metadata; Fix 2 minor Valgrind warnings (#2396)
 * Fix crash on int32 test case in fits.py for 64-bit GDAL build (#2579)

GeoRaster driver:
 * New for 1.6.0

GFF driver:
 * Close file pointer in dataset destructor

GIF driver:
 * Disable opening of large files which hang GDALOpen() (#2542)

GRASS driver:
 * Fix obvious memory leaks in GRASS driver (#2537)
 * fix to use G_free() instead of free() (#1983)

GRIB driver:
 * Moved from spike to trunk: grib now standard, but support --without-grib option

GSAG driver:
 * Prevent crash on huge number of rows
 * Prevent Valgrind warnings on bogus short GSAG files
 * Major update to correct upside problems, removing Create (#2224, #1616, #2191)

GS7BG driver:
 * Fixed geotransformation matrix calculation (#2132)
 * Properly read the header on big-endian system.
 * Fix bigendian support (#2172)

GTiff driver:
 * Add support for reading and writing embedded nodata masks of a TIFF file (TIFFTAG_SUBFILETYPE=FILETYPE_MASK)
 * Added SUBDATASETS support
 * Add the ability to create files with PHOTOMETRIC=PALETTE.
 * Add the ability to update palette on existing files (#2421)
 * Enforce PROFILE properly for Create (#1527)
 * Add support for reading a CMYK TIFF. By default, it will be opened with the RGBA interface (CMKY->RGBA translation done by libtiff. Said to be *very* crude), unless 'GTIFF_RAW:' is specified before the filename. In that later case, the CMYK bands will be presented. Also add support for translating to a CMYK TIFF too : the source dataset must have CMYK bands. No colorspace translation is done
 * Internal libtiff : refresh from upstream libtiff
 * Added GTiffSplitBitmapBand to treat one row 1bit files as scanline blocks (#2622)
 * Don't use GCS if it is less than 1 (#2183).
 * Modified so that the RGBA interface is not used for YCbCr mode JPEG compressed data.  Set JPEGCOLORMODE to RGB at the point a directory is read to avoid error report if doing it later during writing.  This fixes the GDAL 1.5.0 issues with writing YCbCr JPEG compressed data (#2189).
 * Fix memory leak in gt_wkt_srs.cpp
 * Prevent crash in GTiff driver in case we cannot GDALOpen the newly create-copied file
 * Fix buffer overflow when calling GTIFDirectoryInfo in GTIFGetOGISDefn (#2372)
 * add special handling for 24bit data which gets byteswapped by libtiff (#2361)
 * Replace hard-coded 3 byte increment by iPixelByteSkip in int24 gtiff decoding (#2361)
 * Cleaunup frmt/gtiff directory by removing unused files. Move TIFF_WriteOverview to gt_overview.cpp. Create gt_overview.h to declare TIFF_WriteOverview and GTIFFBuildOverviewMetadata
 * Add a ENDIANNESS creation option to GTiff driver for debug purpose mostly
 * Fix writing of blocks on TIFF files in non-native endianness (#2398)
 * Push extra bands of pixel interleaved data into block cache (#2435)
 * Improve integration with PAM metadata loading and saving (#2448)
 * Fix potential buffer overflow in GTIFAngleStringToDD (committed in upstream libgeotiff) - #2228
 * Fix GTiffOddBitsBand::IWriteBlock with GDT_UInt16/32; Error properly with GDT_Float32; Support creating files with NBITS>8; Handle NBITS=1 IReadBlock/IWriteBlock in GTiffOddBitsBand; Prevent subtle IReadBlock/IWriteBlock round-tripping bug for NBITS<8 (#2360)
 * Set the TIFFTAG_COMPRESSION compression before asking the default strip size, so that in the case of JPEG compression, the correct strip height is selected (either 8 or 16). Tested with libtiff-3.8.2 and internal libtiff
 * Prevent crash on tiff_ovr_9 when JPEG-In-TIFF support is not built
 * In GTiffDataset::Create(), set TIFFTAG_JPEGCOLORMODE=JPEGCOLORMODE_RGB when creating a TIFF with COMPRESS=JPEG and PHOTOMETRIC=YCBCR; In Crystalize(), backup the value of TIFFTAG_JPEGCOLORMODE and set it again after writing the directory (#2645)
 * Handle more gracefully the case where we open or create a TIFF file with a compression method not built in libtiff (use of TIFFIsCODECConfigured)
 * Don't fail when TIFFTAG_JPEGCOLORMODE tag cannot be read
 * IPP libjpeg compatibility changes (#2606)
 * ensure zip/jpeg quality is preserved in crystalize. (#2642)
 * support handling nodata via pam/aux mechanisms (#2505)
 * ensure TIFFFlush() is called in FlushCache() (#2512)
 * Replace Crystalize() by SetDirectory() in GTiffDataset::IBuildOverviews() so that 2 consecutive ds.BuildOverviews() calls work without needing to close and reopen the dataset in between
 * Prevent crash when disk is full
 * Add detection of big-endian bigtiffs when BIGTIFF_SUPPORT is *NOT* defined
 * Add missing ScaleAtCenter parameter for Hotine Oblique Mercator when writing the geotiff keys
 * Added logic to pre-clear geotiff tags when updating a file (#2546)
 * Add ExtraSample tag in overviews too (#2572)
 * Fix handling of non-degree angular units (#601)

GXF driver:
 * Add GXFRasterBand::GetNoDataValue (fix #835)
 * Avoid crash on bogus GXF file

HDF4 driver:
 * add support for projected NRL products (#2225)
 * make a block consist of several scanlines for SDS case to speed up (#2208)
 * Add H4ST prefix to names of HDF4SubdatasetType enumeration values. (#2296).
 * Remove useless and dangerous redefinition of sincos in HDF-EOS (#2494)
 * Added compatibility definitions for HDF 4.2 library (#2609)
 * Read HDF raster images containing in HDF-EOS datasets (#2656)

HDF5 driver:
 * Fix minor memory leaks and one incorrect memory usage in HDF5
 * implement support for 1.8+ hdf library versions (#2297)

HFA driver:
 * Avoid possible uninitialized variable usage in HFAWriteXFormStack()
 * Fix BASEDATA count value (preceding pointer) at 1 (#2144)
 * Incorporate generalization of EPT_f32 reduced precision handling (#1000)
 * Add missing creation options, fix doc to refer to COMPRESSED instead of COMPRESS (#2167)
 * remove static buffer to improve thread-safety
 * Read invalid blocks as nodata value if available.  Create new files with all blocks marked invalid.  Support writing to invalid blocks as long as there is already a pointer to valid data.  (#2427)
 * add support for writing 1, 2 and 4 bit data (#2436)
 * Attempt to preserve PROJCS name in sMapInfo.proName, and to capture it as the PROJCS name when reading.  This will hopefully preserve symbolic names like NAD_1983_StatePlane_Ohio_South_FIPS_3402_Feet instead of replacing them with something generic like "Lambert_Conformal_Conic" (#2422).
 * avoid reducing array sizes if writing them in random order (#2427)
 * Prevent writing out cached information after the file has been closed.  Loosely related to (#2524).
 * Error out gracefully and early on attempts to write to readonly file (#2524)
 * Open the dependent file(s) with same permissions as master (#2425)
 * Fix crash in HFACompress::compressBlock when compressing random data with m_nDataTypeNumBits >= 16 (#2525)
 * Fix reading of a non-initialized compressed HFA file (#2523)
 * Add FORCETOPESTRING, and ensure ProjectionX applied to all bands (#2243)
 * Added support for unique values color tables (#2419)

HTTP driver:
 * Fix HTTP driver when falling back to /tmp (#2363)

IDRISI driver:
 * Force min/max calculation on IWriteBlock
 * remove conditional from CreateColorRamp() call
 * Fix #2444 (lat/long) and #2442 (uppercase file extension)

ILWIS driver:
 * Modified to use VSI*L API for reading and writing.  Modify ReadBlock() so that data written on newly created datasets can still be read back.
 * Fix memory leaks in ILWIS driver
 * Avoid writing an ILWIS file to disk when it is a src_dataset. Design of responsible class (IniFile) is simplified, to prevent this from happening unintentionally.
 * Spend extra effort to find the most compact GDAL data-type for storing the ILWIS data; Added missing ILWIS-system domains to the list; Initialized variables before they are used; Added comments to code.
 * Solved unwanted rounding in the pixel size, that resulted in wrong map size calculation.

INGR driver:
 * Support splitting bitonal images into scanline blocks too (#1959)
 * Fix compilation of INGR driver on big-endian target (#2613)

ISIS3 driver:
 * fix earth model, already in meters, not kilometers! (#2321)
 * ensure we adjust first tile offset depending on band (#2573)

ISO8211 driver:
 * corrections to handle double byte attributes better (#1526)
 * add a -xml option to 8211dump utility; add a 8211createfromxml utility to generate a ISO8211 file from the output of 8211dump -xml
 * robustness fixes

JP2KAK driver:
 * Add VSI*L reading and writing (vsil_target) (#2255)
 * Remove KAKADU4 related ifdefs, we now assume at least KAKADU 4.2.
 * disable JPIP - not working with modern Kakadu

JPEG driver:
 * Added support for reading georeferencing from .tab files. Fixes #682.
 * Add support for reading images in CMYK and YCbCrK color spaces (#2443)
 * make sure bHasDoneJpegStartDecompress is set in Reset() (#2535)
 * Added fill/flush support compatible with IPP libjpeg (#2606)

LCP (FARSITE) driver:
 * New for 1.6.0

L1B driver:
 * Added support for NOAA-18(N) and METOP-2 datasets; tiny code refactoring.
 * L1B : add auto guess of data format when it is 2 spaces or empty string
 * The GAC GCPs are not tied to the center of pixel.
 * Serious code rewriting in order to read datasets without archive header

MEM driver:
 * Avoid failure when doing mem_driver->Create('MEM:::')

MrSID driver:
 * Use VSI Virtual File API in MRSID DSDK I/O routines. MrSID reading now
works through the VSI calls as any other GDAL driver.
 * Added support for MrSID DSDK 7.x (#2410)
 * Use int 32 types instead of long types for LTI_METADATA_DATATYPE_UINT32 and LTI_METADATA_DATATYPE_SINT32 metadata (#2629)

MSG driver:
 * Fixes and improvements to enable compilation with GCC 4.x (Ticket #2168).

NDF driver:
 * Support NDF2 files in other than the current directory (#2274)
 * Added somewhat improved coordinate system support (#2623)

NetCDF driver:
 * Handle very large attributes properly (#2196)
 * NETCDF plugin name doesn't correspond to the loader entry name causes an error in AutoLoadDrivers (#2464)
 * Fix allocation of panBandZLev (#2582)
 * Fix accidentally too large memory allocation (#2591)
 * Do not report char variables as subdataset (#2599)
 * Fix LAEA projection (#2584)

NITF driver:
 * Add support for RPB and IMD files
 * Handle NITF JPEG-compressed image with data mask subheader (IC=M3) multi-blocks (#2364)
 * Implement SetProjection for NITF (#2095)
 * Added support for decoding 12 bit images (#2532)
 * Added support for writing TEXT segments in CreateCopy()
 * Added support for writing arbitrary user defined TREs
 * Fix #2249 : shift when writing NITF color table with nColors < 256
 * Prevent crash with LUT entry count > 256
 * Disable unnecessary VSIFFlush() calls that slowdown writing on some systems
 * Apply untested RPC00A remapping (#2040)
 * Fix #2135 by narrowing workaround test made for #1750
 * Prevent crash on NITF file without image segment (#2362)
 * Additional fix for handling 1-bit uncompressed NITF images with NITF_IC=NM  (#1854)
 * Set IREP=RGB implicitly when the first 3 channels of an image are R,G,B (#2343)
 * Allocate one extra byte for the NULL terminating character when reading TEXT data in NITF file (#2366)
 * Fix 'adding BLOCKA TRE precludes writing IGEOLO' (#2475)
 * Add GDAL_DMD_CREATIONOPTIONLIST for NITF
 * Prevent crash when using a bad value for TRE creation option
 * Fallback to pam info for nodata (#2596)

PAUX driver:
 * Check for either generated spelling of AuxiliaryTarget (#2219)

PCRaster driver:
 * Add overview support
 * Added support for CSF version 1. Updated nodata values to be equal to gdal's internal ones.

PDS driver:
 * Add support for # style comments (#2176)
 * Improve PDS dataset identification & fixes image segment offset (#2397)
 * Add LSB_SIGNED_INTEGER

PGCHIP driver:
 * Many memory leak fixes & cleanups, add an extra parameter '%name=my_name' for handling several rasters in the same table, add support for reading&writing geotransform

PNM driver:
 * Fix potential buffer overflow in case of bad PNM file
 * Fix logical tests in PNM Identify (bug #2190)

RAW drivers:
 * manage RawRasterBand NODATA values at PAM level
 * RawRasterBand : add extra parameter bOwnsFP to enable the RawRasterBand to take ownership of the fpRaw so as to close it properly in its destructor
 * Fix crash in rawdataset.cpp with pixeloffset=0 (#2576)

RMF driver:
 * Do not forget to swap block size/offset table on big-endian archs. (#2169)
 * Added support for reading and writing extended header..
 * RMF driver can crash / corrupt stack when importing projection from Panorama (#2277)
 * Fixed error checking code returned by color table read function.
 * Added support for reading big endian variant of the RSW files.
 * Report units and dataset statistics (#2670)

RPFTOC driver:
 * Enable external overview building on RPFTOC subdatasets
 * Use new proxy API instead of RPFTOCGDALDatasetCache
 * Initialize PAM for RPFTocDataset
 * Implement GetFileList() for RPFTOCDataset and RPFTOCSubDataset

RS2 driver:
 * Added projection reading
 * Updates to RADARSAT-2 driver to account for tiled GeoTIFF images.
 * Capture all files for GetFileList().
 * Support selecting directory as well as product.xml to open the dataset.
 * Various other enhancements

SDTS driver:
 * Prevent infinite recursion in SDTSRasterReader::GetBlock when CEL0 file is truncated
 * SDTS DEM : Read metadata in the IDEN file

SGI driver:
 * Implemented SGI write support (always RLE)
 * Fix SGI driver that misidentified SRTMHGT files as SGI files (#2289)

Terragen Driver:
 * Fix overflow in implicit constant conversion (#2119)

Terralib driver:
 * New for 1.6.0

TSX driver:
 * Added support to extract GCPs from XML metadata for TerraSAR-X SSC products.
 * Provide an error message if the sceneInfo tag cannot be found in the TerraSAR-X image metadata.
 * Fix lat/long inversion (whoops, #2565); expose additional metadata items

USGSDEM driver:
 * mark NTS and INTERNALNAME as legal options
 * make parser more permissive (#2348)
 * add missing ZRESOLUTION creation option in GDAL_DMD_CREATIONOPTIONLIST
 * USGSDEM: add precisions to creation options documentation; check that source dataset dimensions are at least 2x2 for CreateCopy()
 * USGSDEM: fix USGSDEMDecToPackedDMS when input is very close to an integer degree value

VRT driver:
 * Use VSIF Large API in VRTDataset::Open to fix #1070
 * recover from failure to create transformer (#2240)
 * Added LUT based transformation support to the VRTComplexSource
 * Extend the output of <SimpleSource> in a forward and backward compatible way, and make use of GDALProxyPoolDataset when possible
 * Add the <ColorTableComponent> element to <ComplexSource> to do color table expansion in the VRT
 * Fix failure when attempting to read a warped VRT made from a 3-band dataset with -dstalpha option (#2502)
 * In VRTDerivedRasterBand::IRasterIO() don't call RasterIO() on sources with 0,0 for nPixelSpace and nLineSpace as most sources, except VRTSimpleSource, don't translate them.
 * Allow empty category in VRT rasterband (#2562)
 * Use nodata in VRTKernelFilteredSource::FilterData (#1739)
 * Fix VRT average resampling when resampling factor > 100% (#1725)

WCS driver:
 * Improvements to identify Band field name
 * More fiddling with GetCoverage() bounding boxes.  Avoid half pixel bounding box shift south east.  When using GridOffset/GridStep values expand the bounding box out by 1% of a pixel to avoid "on edge" rounding issues.
 * Strip namespaces off DescribeCoverage response (early), and add a bug
workaround for GeoServer WCS 1.1 responses.
 * Correct wcs 1.1 band identification logic
 * URL encode format and coverage name.  Improve error recognition.
 * GridOffset should be top left corner of pixel center oriented bounds

WMS driver:
 * Add ClampRequests setting (#2450)
 * Fix WMS driver to make it work when ReadBlockFromFile() must deal with blocks already in block-cache but that are not the band to fill (#2647)
 * Add a <Timeout> option (#2646)

XPM driver:
 * Prevent crash when opening an XPM file with large file API


## OGR 1.6.0 - Overview of Changes

General:
  * RFC 21: OGR SQL type cast and field name alias (#2171)
  * Added support for outline color in OGRStyleLabel (#2480)
  * Added support for symbol outline color in OGR Style Strings (#2509)
  * Added geometry type merger
  * Added SetEquirectangular2()
  * Added SetLinearUnitsAndUpdateParameters() to C API
  * Add support to translate OGRPolygon to OGRMultiLineString
  * Add a segmentize() method to OGRGeometry to modify the geometry such it has no segment longer then the given distance; add a -segmentize option to ogr2ogr
  * Many performance fixes in OGRGeometryFactory::organizePolygons (#1217, #2428, #2589)
  * Changed OGRFeature::GetStyleString() to return the value of the OGR_STYLE field if no style string have been specified.
  * Ensure OpenShared sharing is only with same thread (#2229)
  * ogrfeaturestyle : OGRSTBrushAngle parameter should not be georeferenced.
  * Strip whitespaces at the start and end of parsed pairs of style elements in OGRStyleTool::Parse(). As per #1413.
  * Remove empty linestrings from multilinestring objects
  * Allow 'POINT EMPTY' in WKT (bug #1628)
  * Fix OGRGeometryCollection::getCoordinateDimension() (#2334)
  * Make OGRLineString::importFromWkb and OGRPolygon::importFromWkb with EMPTY geometries
  * Implement IsEmpty() for all geometries without using GEOS (for speed purpose, and also because GEOS 2.2.3 is buggy with multipolygons with empty polygon inside); Make exportToWkt() export a valid WKT when multipoints/linestrings/polygons have an empty geometry inside
  * Reintroduce OFTWideString and OFTWideStringList but mark them as deprecated (#2359).
  * Fixed segmentation fault in swq_select_finish_summarize when SQL query issued on layer without any attributes like empty shapefile (Ticket #2358).
  * Fix crash in OGRDataSource::ExecuteSQL with an empty SQL query (#2386)
  * Make OGRLayer::FilterGeometry more restrictive in the geometries it selects (#2454)
  * OGRStyleVector class and related stuff has been removed (#2070).

  * Fixed OGR SQL to properly escape single quotes in string literals (Ticket #2220).
  * Prevent an OGR driver from being registered several times (#2544)

Utilities:
 * Start on a dissolve utility based on ogr2ogr
 * Add --version and --licence options for OGR CLI utilities

OGRSpatialReference:
 * RFC 20: Axes methods
 * Upgrade to EPSG 6.17
 * Added support for "International Map of the World Polyconic" and "Wagner I-VII" projections.
 * Add EquidistantCylindricalSphere and GaussLabordeReunion (#2134)
 * Fix exportToProj() translation for OSGB36 (#2160)
 * ogr_srs_panorama.cpp : Fixed search in ellipsoid list
 * ogr_srs_pci.cpp : List of ellipsoids updated
 * Treat spherical mercator 1SP similarly to normal mercator 1sp (proj #9).
 * Ensure Clear() clears the bNormInfoSet flag and use Clear() from the various import methods to wipe old state (#2533).
 * add import/export for MITAB CoordSys
 * Added declarations for OSRImportFromMICoordSys()/OSRExportToMICoordSys(); make OSRImportFromPanorama()/OSRExportToPanorama() externally visible again.
 * Ensure rectified_grid_angle gets stripped for HOM projections (#2575)
 * ogr_srs_esri.cpp: Correct test of iRGAChild (#2575).
 * Recent EPSG releases seem to use PolarLongOrigin instead of ProjCenterLong for the Krovak projection parameters.  Handle either (#2559).
 * Test that input pointer is not NULL in OGRSpatialReference::importFromWkt().

AVC driver:
 * Make AVCE00 a distinct driver
 * Update from AVCE00 master, includes the fixes for #2495 (GCC warnings)
 * Detect compressed E00 input files and refuse to open them instead of crashing (#2513)
 * Avoid scanning the whole E00 input file in AVCE00ReadOpenE00() if the file does not start with an EXP line (#1989)

BNA driver:
 * Fix crash when trying to write features with empty geometries
 * Add support for Unix End-Of-Line characters on Windows

CSV driver:
 * Add support for writing the geometry of features through the new GEOMETRY layer creation option
 * Allow to define field width via .csvt text file (bug #2142)
 * Handle more gracefully CSV files with an empty column title (#2538)

DGN driver:
 * add some experimental linkage testing

DODS driver:
 * add using namespace libdap for version 3.8.2 (#2404)

Geoconcept Export driver:
 * New for 1.6.0

GeoJSON driver:
 * Fixed GeoJSON driver crash when writing features with null geometry (#2212)
 * GeoJSON: enabled read/write of 25D geometry types.
 * Improved GeoJSON driver to gracefully handle JSON strings that do not encode GeoJSON content.
 * Support GeoJSON 1.0 Spec CRS 'link' and 'name' members. (#2665)

GPX driver:
 * XML Datetime can be expressed without explicit timezone mention
 * <extensions> is valid inside <rtept> and <trkpt> too
 * Add support for GPX 1.0 reading
 * Handle degenerate and NULL geometries in creation mode
 * Remove noisy CPL_DEBUG message when GPX driver built without Expat and when the input file doesn't look like GPX (#2394)
 * Prevent GPX and KML drivers to read too much of a non GPX/KML file (#2395)
 * GPX writer: Remove leading spaces for a numeric field (#2638)

GML driver:
 * Support reading GML 3 posList geometry (#2311)
 * Add logic to potentially track geometry type (mostly for NAS just now)
 * Fix #2141 : GML driver recognizes improperly strings as integers
 * Do geometry element name test (IsGeometryElement) case sensitive to avoid false positives on property names, and such (#2215).
 * Fix memory bug in OGRGMLLayer::GetNextFeature() when using attribute filter (#2349)

GMT driver :
 * Remove spaces from numeric field values before writing to avoid unnecessary quoting.

GRASS driver:
 * Call Vect_close() in the OGR GRASS driver (#2537)

INGRES driver:
 * New for 1.6.0

Interlis 1 driver:
 * Support for SURFACE polygons spread over multiple OBJECTs Polygonize on demand. Generation of area layer
 * Fix a crash (#2201)
 * Fix memory leaks & apps/test_ogrsf correctness in OGRILI1 (#2203)
 * Prevent crash in OGRILI1DataSource::Open with an empty string (#2335)
 * Fixed column order detection for some Interlis 1 models (#2595)

Interlis 2 driver:
 * Fix memory usage and leaks in OGRILI2 (bug #2203)
 * Fix logic to detect ILI2 datasets (#2516)

KML driver:
 * Updated KML write driver to support KML v2.2.
 * Added support for "SchemaData" - typed KML fields that maintain feature data.
 * Support of date types, written out as strings (#2264)
 * Added automatic coordinate system transformation to WGS84 (the only CS that KML recognizes). (#2271)
 * Report XML parsing error in KML driver in a similar way it's done in the GPX driver
 * Speedup detection of KML documents
 * Use VSI Large File API
 * Corrected case on schemaUrl attribute and added the id attribute to the schema element. (#1897)

MITAB driver:
 * Upgraded to current dev version of MITAB - includes a number of TAB StyleString improvements
 * Support font point outline color

MySQL Driver:
 * Fix memory leaks in MySQL driver
 * Fix crash with very long WHERE clause in MySQL driver
 * Use assignSpatialReference for read features
 * Fix SRS cache in FetchSRS()
 * MYSQL: Add backquotes around table and column names to enable the use of reserved keywords (#2315)
 * Fix reporting of capabilities for OGRMySQLResultLayer
 * return proper results for various writing capabilities (#2184)

NTF driver:
 * Update for new meridian and strategi products (#2600)

OCI driver:
 * Fixed memory leaks in OCI driver reported by Linda Thompson (#2120)
 * Support for OCI + VRT to access non spatial data (#2202)
 * Prevent from calling CPLError when SDO_GEOMETRY is missing (non spatial) (#2202)
 * Added support for fields of type DATE and TIMESTAMP [WITH [LOCAL] TIME ZONE] as OFTDate and OFTDateTime.
 * An extra space is needed for the decimal separator when retrieving the numeric fields (#2350)
 * Improved OCI driver to query spatial extent of layer using SDO_GEOM_METADATA for better performance (Ticket #543).
 * OCI: Filter out MDSYS.CS_SRS entries with NULL value of WKTEXT.
 * add compound and stroked-arc read support

PG (Postgres/PostGIS) Driver:
 * Add the ability to specify a list of tables to be treated as layers with the 'tables=' connection string option (#1699)
 * Add SPATIAL_INDEX creation option to create GIST index. Turned ON by default (#959)
 * Add support for listing and correctly fetching geometry type of inherited spatial tables (#2558)
 * Add support for Postgis tables with multiple geometry columns (#1476)
 * Fixes to Postgres binary cursor mode and improvement/fixes to data types handling (#2312)
 * Implement efficient GetFeatureCount, SetSpatialFilter and GetExtent for OGRPGResultLayer
 * Apply spatial filter in OGRPGDataSource::ExecuteSQL()
 * Make binary cursor work with non-PostGIS geometry column
 * Fix memory leaks
 * Fix write outside of allocated buffer in OGR PG driver (#2303)
 * Use assignSpatialReference for read features
 * Fix geometry filter when there is no PostGIS geometry column
 * Fix getting the PK of a table with PostgreSQL <= 7.3; Fix CreateFeatureViaCopy when geometry column is not PostGIS geometry
 * Use the 'auth_srid' to avoid mismatches between OGR and PostGIS WKTs of EPSG codes (#2123)
 * Fix reporting of capabilities and handling of spatial and attribute filters by OGRPGResultLayer
 * Add PG_USE_POSTGIS to be able to disable PostGIS for debug purpose
 * Re-enable PQsetClientEncoding but set it to UNICODE now that the driver advertises OLCStringsAsUTF8
 * Replace use of risky sprintf by CPLString to avoid potential buffer overflows
 * In non PostGIS mode, skip tables of schema 'information_schema'
 * Allow VACUUM through ExecuteSQL() without a transaction (#2619).

PGEO driver:
 * correct testcapability results (#2601)
 * treat type 50 geometry as SHPT_ARC (#1484)

SDE driver:
 * Versioned editing/write support for SDE.

SQLite driver:
 * Added support for geometry_columns, and WKB support
 * Added preliminary FGF to geometry support
 * Added spatial_ref_sys support
 * Added preliminary support for spatialite geometries

SHAPE driver:
 * CreateField() now works on populated layers (#2672)
 * Cleanup to remove unused old classification code for multipolygons (#2174)
 * Fix error class in VSI_SHP_Error (#2177)
 * Fix crash with polygon with nParts == 0
 * Change SHAPE driver to return a NULL geometry instead of an empty OGRMultiPoint, OGRMultiLineString and OGRMultiPolygon (bug #2217)
 * Fix crashes on corrupted geometries (#2218 and #2610)
 * Fix crash when dealing with unhandled field types in shape driver, in DEBUG mode (#2309)
 * Add case for wkbMultiLineString and 25D in OGRShapeLayer::CreateFeature
 * Make SHPWriteOGRObject write a SHPT_NULL object for empty geometries and handle correctly multigeometries with empty geometries inside
 * Make sure field type set to OFTDate when OFTDataTime requested (#2474)
 * Implement OGRShapeDataSource::DeleteLayer() (#2561)

S57 driver:
 * Ensure SOUNDG in multipoint form is MultiPoint25D not 2D.
 * Print out contents of OGR_S57_OPTIONS environment variable if set (for debug
purposes).
 * Correct control for applying updates, now done in Ingest method.

VRT driver:
 * carry style string through VRT layer
 * Pass the envelope of the geometry as the spatial filter in the sub-query (#2214)
 * Add "shared" attribute on SrcDataSource to control sharing.  Default to OFF for SrcLayer layers, and ON for SrcSQL layers.  This endeavors to avoid conflicts of layer state. (#2229)

XPlane/Flightgear driver:
 * New for 1.6.0



## SWIG Language Bindings

SWIG General :
 * Added GetHistogram
 * Added SetLinearUnitsAndUpdateParameters
 * Added GetSubDatasets method on Dataset
 * Added SetEquirectangular2
 * Cast returned value to OGRDriverShadow in GetDriver method instead of OGRLayerShadow
 * Make it possible to skip adding the inline C functions into the wrapper
 * Fix SWIG ReadRaster_internal and DSReadRaster_internal may crash (#2140)
 * Modify GeneralCmdLineProcessor() to recognise that a <= 0 return result is special and means NULL should be returned indicating a need to terminate the calling application.
 * Added SetMetadataItem/GetMetadataItem
 * Added gdaltransformer wrapper for RFC 22
 * implement the BuildFromEdges function #2380
 * Added ComputeProximity
 * Added GDALRegenerateOverviews
 * Added GetFileList
 * Added GRA_Lanczos
 * Added gdal.Polygonize()
 * expose GDALDestroyDriverManager() to improve leak detection
 * Added RasterizeLayer() and SieveFilter()
 * If we receive an odd array type in BandWriteArray(), cast the array to float64 which we do support (#2285).

SWIG C# related changes:
 * Implement GDALProgressFunc callback for C# (fix for #2122)
 * Fixed that passing null as the options parameter of Driver.CreateCopy causes access violation (#2185).
 * Added GDALCreateCopy sample application
 * Support for signing the GDAL C# assemblies (#2186)
 * Added raster specific tests
 * Reworked the wrapper implementation
 * Added the bandMap parameter to the C# Dataset.ReadRaster and Dataset.WriteRaster API.
 * Added a C# sample to demonstrate the GDALDatasetRasterIO operations.
 * Added Band.GetHistogram to the C# bindings
 * Sample application for Band.GetHistogram.
 * Added the GDALAdjustContrast sample to demonstrate the image correction at the C# side.
 * Added Dataset.GetGCPs, Dataset.SetGCPs and GCPsToGeoTransform in the C# bindings (bugs #2426, #1986 and #1677
 * Added support for using OGR.Layer in Gdal.Polygonize
 * Changed the behavior to use OSR.SpatialReference and OSR.CoordinateTransformation instead of defining the same classes in the OGR namespace
 * Changed the scope from internal to public of the required functions

SWIG Python related changes:
 * Fix layer __getitem__ bug (#2187)
 * add some sugar to ogr.DataSource.DeleteLayer to be able to take in either an index or a layer name.  If you give it a name, it *will* loop through all of the layers on the datasource.  This might be expensive depending on the driver.
 * add date/time fetching support to the generic GetField implementation... note this is not real 'datetime' support yet
 * a typemap for taking in lists of GDAL objects #2458
 * don't always return 0 for OGRErrs #2498
 * Added GetDefaultHistogram() with Python implementation
 * support for mingw Windows builds
 * Link with gdal_i.lib instead of gdal.lib when building with MSVC compiler. (#2578)

SWIG Perl
 * Added a more verbose description to the error message if projection method test fails. Skip testing parameters of International Map of the World projection since it fails (a bug?).
 * driver's create method's 2nd parameter, if given, is a listref
 * Support Cygwin by adding -lstdc++ to LIBS if OS is cygwin
 * Add GetDriver method as an alias to _GetDriver for DataSource. This fixes an unnoticed side-effect of rewrapping GetDriver for root class OGR.
 * force name to be a string for _GetLayerByName and make default for name 0
 * fix Layer::Schema
 * use perl hash also for fields in schema (the new API was not really implemented), add Schema method also for Feature
 * croak in Geometry::create unless type, wkt, wkb, or gml given
 * do not call UseExceptions when booting OGR wrappers since it is only done once when booting GDAL (of which OGR is a part), do not include inline functions from cpl_exceptions.i into OGR wrappers
 * add exception support as for OGR
 * use geometry factory methods in Geometry::create; accept also only coordinate parameters (i.e. auto-add 0 if needed) in Point method for Point type
 * Add wrappers for field types datetime and lists. Do not use the overloaded (field name) versions of the get/set functions.
 * GetField and SetField methods, which check for goodness of the field (name, index) and support dates, times, datetimes, and lists. In Row and Tuple the field value may be a listref.
 * Support HEXWKB in Geo::OGR::Geometry::create.
 * As* methods for Geometry as aliases for ExportTo; SpatialReference->create constructor
 * made needed links from parameters to typemaps to make Get- and SetDefaultHistogram methods work in Perl
 * support for mingw Windows builds

SWIG Java:
 * removed colortable from java bindings for now (#2231)

# GDAL/OGR 1.5.0

## GDAL/OGR 1.5.0 - General Changes

Build:
 * CFG environment variable now ignored.  Instead set CFLAGS and CXXFLAGS
   environment variables to desired compilation options, or use --enable-debug
   for a debug build.  Default is "-g -O2" like most other packages.
 * Added --with-hide-internal-symbols to restrict exported API from .so files
   to be the GDAL public API (as marked with CPL_DLL).

Other:
 * OGR and GDAL C APIs now generally check for NULL objects and recover
   with an error report instead of crashing.


## GDAL 1.5.0 - Overview of Changes

Core:
 * Enable Persistent Auxiliary Metadata (.aux.xml) by default.
 * Support for "pam proxies" for files in read-only locations.
 * Create and !CreateCopy pre-Delete output existing dataset.
 * Added Identify() method on drivers (per RFC 11: Fast Format Identify)
 * Implement !GetFileList() on datasets (per RFC 12).
 * Implement Delete(), Rename(), Copy() based on !GetFileList() (per RFC 12).
 * vrtdataset.h, memdataset.h and rawdataset.h are now considered part of
   the public GDAL API, and will be installed along with gdal.h, etc.
 * Support nodata/validity masks per RFC 14: Band Masks.
 * Plugin drivers test for ABI compatibility at load time.
 * Creation flags can now be validated (this is used by gdal_translate)
 * Default block cache size changed to 40MB from 10MB.

Algorithms / Utilities:
 * gdal_grid: New utility to interpolate point data to a grid.
 * gdal2tiles.py is new for 1.5.0.
 * gdaltransform: stdin/stdout point transformer similar to PROJ.4 cs2cs.
 * gdalwarp: Several fixes related to destination "nodata" handling and
   nodata mixing in resampling kernels.
 * gdalwarp: Added Lanczos Windows Sinc resampling.
 * gdal_rasterize: added -i flag to rasterize all areas outside geometry.
 * gdalenhance: new utility for applying histogram equalization enhancements.
 * gdalmanage: Utility for managing datasets (identify, delete, copy, rename)
 * nearblack: Utility for fixing lossy compressed nodata collars.

Intergraph Raster Driver:
 * New for 1.5.0.

COSAR (TerraSAR-X) Driver:
 * New for 1.5.0.
 * SAR Format.

COASP Driver:
 * New for 1.5.0
 * SAR format produced by DRDC CASP SAR Processor.

GFF Driver:
 * New for 1.5.0

GENBIN (Generic Binary) Driver:
 * New for 1.5.0.

ISIS3 Driver:
 * New for 1.5.0.
 * Also PDS and ISIS2 driver improved substantially and all moved to frmts/pds

WMS Driver:
 * New for 1.5.0.

SDE Raster Driver:
 * New for 1.5.0.

SRTMHGT Driver:
 * New for 1.5.0.

PALSAR Driver:
 * New for 1.5.0.
 * SAR format.

ERS Driver:
 * New for 1.5.0.
 * ERMapper ASCII Header

HTTP Driver:
 * New for 1.5.0.
 * Fetches file by http and then GDALOpen()s.

GSG Driver:
 * New for 1.5.0.
 * Golden Software Surfer Grid.

GS7 Driver:
 * New for 1.5.0.
 * Golden Software Surfer 7 Binary Grid.

Spot DIMAP Driver:
 * New for 1.5.0.

RPFTOC Driver:
 * New for 1.5.0.

ADRG Driver:
 * New for 1.5.0.

NITF Driver:
 * Added support for writing JPEG compressed (IC=C3).
 * Added support for reading text segments and TREs as metadata.
 * Added support for 1bit images.
 * Added support for GeoSDE TRE for georeferencing.
 * Support PAM for subdatasets.
 * Improved NSIF support.
 * Support C1 (FAX3) compression.
 * Improved CADRG support (#913, #1750, #1751, #1754)

ENVI Driver:
 * Many improvements, particularly to coordinate system handling and metadata.

JP2KAK (Kakadu JPEG2000) Driver:
 * Now builds with libtool enabled.

GTIFF (GeoTIFF) Driver:
 * Now supports BigTIFF (read and write) with libtiff4 (internal copy ok).
 * Upgraded to include libtiff 4.0 (alpha2) as the internal option.
 * Support AVERAGE_BIT2GRAYSCALE overviews.
 * Produce pixel interleaved files instead of band interleaved by default.
 * Support TIFF files with odd numbers of bits (1-8, 11, etc).
 * Add ZLEVEL creation option to specify level of compression for DEFLATE method

GIF Driver:
 * Nodata/transparency support added.

JPEG Driver:
 * Support in-file masks.

AIGrid Driver:
 * Supports reading associated info table as a Raster Attribute Table.

HFA Driver:
 * Support MapInformation/xform nodes for read and write.
 * Support AVERAGE_BIT2GRAYSCALE overviews.
 * Support Signed Byte pixel type.
 * Support 1/2/4 bit pixel types.
 * Support PE_STRING coordinate system definitions.
 * Support nodata values (#1567)

WCS Driver:
 * Support WCS 1.1.0

DTED Driver:
 * Can now perform checksum verification.
 * Better datum detection.

HDF4 Driver:
 * Support PAM for subdatasets.

Leveller Driver:
 * Added write support.
 * Added v7 (Leveller 2.6) support.

## OGR 1.5.0 - Overview of Changes

General:
 * Plugin drivers test for ABI compatibility at load time.
 * SFCOM/OLEDB stuff all removed (moved to /spike in subversion).
 * Various thread safety improvements made.
 * Added PointOnSurface implementation for OGRPolygon.
 * Added C API interface to OGR Feature Style classes (RFC 18).

Utilities:
 * All moved to gdal/apps.

OGRSpatialReference:
 * Supports URL SRS type.
 * Upgraded to EPSG 6.13.
 * Operating much better in odd numeric locales.

BNA Driver:
 * New for 1.5.0.

GPX Driver:
 * New for 1.5.0.

GeoJSON Driver:
 * New for 1.5.0.

GMT ASCII Driver:
 * New for 1.5.0.

KML Driver:
 * Preliminary read support added.

DXF / DWG Driver:
 * Removed due to licensing issues with some of the source code.  Still
   available in subversion from under /spike if needed.

PG (Postgres/PostGIS) Driver:
 * Added support for recognising primary keys other than OGR_FID to use as FID.
 * Improved schema support.
 * Performance improvements related to enabling SEQSCAN and large cursor pages

Shapefile Driver:
 * Do not keep .shx open in read only mode (better file handle management).
 * Use GEOS to classify rings into polygons with holes and multipolygons if it is available.
 * Support dbf files larger than 2GB.

MySQL Driver:
 * Added support for BLOB fields.

MITAB (MapInfo) Driver:
 * Upgraded to MITAB 1.6.4.

Interlis Drivers:
 * Support datasources without imported Interlis TID
 * Remove ili2c.jar (available from http://home.gdal.org/dl/ili2c.jar
 * Support for inner rings in Surface geometries.
 * Support spatial and attribute filters.

## SWIG Language Bindings

 * The "Next Generation" Python SWIG bindings are now the default.
 * Python utility and sample scripts migrated to swig/python/scripts and
   swig/python/samples.
 * Added Raster Attribute Tables to swig bindings.
 * Added Geometry.ExportToKML
 * Added CreateGeometryFromJson
 * Added Geometry.ExportToJson

SWIG C# related changes:
 * Support for the enumerated types of the C# interface
 * C# namespace names and module names follows the .NET framework naming guidelines
 * Changed the names of the Windows builds for a better match with the GNU/Linux/OSX builds
 * The gdalconst assembly is now deprecated
 * GDAL C# libtool build support
 * !CreateFromWkb support
 * Dataset.!ReadRaster, Dataset.!WriteRaster support
 * Added support for Dataset.!BuildOverviews
 * More examples added

SWIG Python related changes:
 * Progress function callback support added.  You can use a Python function, or the standard GDALTermProgress variant
 * Sugar, sweet, sweet sugar.
    * ogr.Feature.geometry()
    * ogr.Feature.items()
    * ogr.Feature.keys()
 * doxygen-generated docstrings for ogr.py
 * geometry pickling
 * setuptools support
 * !PyPi http://pypi.python.org/pypi/GDAL/
 * setup.cfg for configuring major significant items (libs, includes, location of gdal-config0
 * support building the bindings from *outside* the GDAL source tree

SWIG Java:
 * SWIG Java bindings are orphaned and believed to be broken at this time.


# GDAL/OGR 1.4.0 - General Changes

Perl Bindings:
 - Added doxygen based documentation.

NG Python Bindings:
 - Implemented numpy support.

CSharp Bindings:
 - Now mostly operational.

WinCE Porting:
 - CPL
 - base OGR, OSR and mitab and shape drivers.
 - GDAL, including GeoTIFF, DTED, AAIGrid drivers
 - Added test suite (gdalautotest/cpp)

Mac OSX Port:
 - Added framework support (--with-macosx-framework)

## GDAL 1.4.0 - Overview Of Changes

WCS Driver:
 - New

PDS (Planetary Data Set) Driver:
 - New

ISIS (Mars Qubes) Driver:
 - New

HFA (.img) Driver:
 - Support reading ProjectionX PE strings.
 - Support producing .aux files with statistics.
 - Fix serious bugs with u1, u2 and u4 compressed data.

NITF Driver:
 - Added BLOCKA reading support.
 - Added ICORDS='D'
 - Added jpeg compression support (readonly)
 - Support multiple images as subdatasets.
 - Support CGM data (as metadata)

AIGrid Driver:
 - Use VSI*L API (large files, in memory, etc)
 - Support upper case filenames.
 - Support .clr file above coverage.

HDF4 Driver:
 - Added support for access to geolocation arrays (see RFC 4).
 - External raw raster bands supported.

PCIDSK (.pix) Driver:
 - Support METER/FEET as LOCAL_CS.
 - Fix serious byte swapping error on creation.

BMP Driver:
 - Various fixes, including 16bit combinations, and non-intel byte swapping.

GeoTIFF Driver:
 - Fixed in place update for LZW and Deflated compressed images.

JP2KAK (JPEG2000) Driver:
 - Added support for reading and writing gmljp2 headers.
 - Read xml boxes as metadata.
 - Accelerate YCbCr handling.

JP2MrSID (JPEG2000) Driver:
 - Added support for reading gmljp2 headers.

EHDR (ESRI BIL) Driver:
 - Support 1-7 bit data.
 - Added statistics support.

## OGR 1.4.0 - Overview of Changes

OGR SQL:
 - RFC 6: Added support for SQL/attribute filter access to geometry, and
   style strings.

OGRSpatialReference:
 - Support for OGC SRS URNs.
 - Support for +wktext/EXTENSION stuff for preserving PROJ.4 string in WKT.
 - Added Two Point Equidistant projection.
 - Added Krovak projection.
 - Updated support files to EPSG 6.11.

OGRCoordinateTransformation:
 - Support source and destination longitude wrapping control.

OGRFeatureStyle:
 - Various extensions and improvements.

INFORMIX Driver:
 - New

KML Driver:
 - New (write only)

E00 Driver:
 - New (read only)
 - Polygon (PAL) likely not working properly.

Postgres/PostGIS Driver:
 - Updated to support new EWKB results (PostGIS 1.1?)
 - Fixed serious bug with writing SRSes.
 - Added schema support.

GML Driver:
 - Strip namespaces off field names.
 - Handle very large geometries gracefully.

ODBC Driver:
 - Added support for spatial_ref_sys table.

SDE Driver:
 - Added logic to speed things up while actually detecting layer geometry types

PGeo Driver:
 - Added support for MDB Tools ODBC driver on linux/unix.

VRT Driver:
 - Added useSpatialSubquery support.


# GDAL/OGR 1.3.2 - General Changes

WinCE Porting:
 - Support for MS WinCE new for this release.

Java SWIG Bindings:
 - Preliminary support implemented.


## GDAL 1.3.2 - Overview of Changes

Locale:
 - Force numeric locale to "C" at a few strategic points.

Idrisi Driver:
 - New for 1.3.2.
 - Includes reading and writing.
 - Limited coordinate system support.

DIPEx Driver:
 - New for GDAL 1.3.2 (related to ELAS format).

Leveller Driver:
 - New for GDAL 1.3.2.

NetCDF Driver:
 - Improved autoidentification of x, y dimensions.
 - Improved CF support.

JPEG2000 (JasPer) Driver:
 - Use GDALJP2Metadata to support various kinds of georeferencing.

JPEG2000 (JP2KAK) Driver:
 - Support writing tiles outputs so that very large images can be written.

GeoTIFF Driver:
 - Report error when attempting to create >4GB uncompressed file.
 - Updated to latest libtiff, now supports "old jpeg" fairly well.
 - Improved support for subsampled YCbCr images.

Imagine (HFA) Driver:
 - Support reading affine polynomial transforms as geotransform.
 - Support overviews of different type than base band.
 - Support reading RDO style "nodata" indicator.

PCI Aux Driver:
 - Support projections requiring parameters.

MrSID Driver;
 - Fixed problem with writing files other than 1 or 3 bands.
 - Support ESDK 6.x.

BMP Driver:
 - Added support for 32bit images with bitfields compression.

DODS Driver:
 - Upgraded to support libdap 3.6.x.
 - Upgraded to support [-x][-y] to flip image.

gdal_rasterize Utility:
 - New for GDAL 1.3.2.
 - Rasterize OGR polygons into a raster.

## OGR 1.3.2 - Overview of Changes

OGRFeature:
 - Added support for OFTDate, OFTTime and OFTDateTime field types.
 - Also applied to a few drivers (shapefile, mysql, postgres)

OGRLayer:
 - GetFIDColumn() and GetGeometryColumn() added.

Generic OGR SQL:
 - Proper support for spatial and attribute filters installed on
   OGR SQL resultsets.

OGRSpatialReference:
 - Upgraded data files to EPSG 6.9

PostGIS Driver:
 - Include proj4text in new spatial_ref_sys entries.
 - Fixed support for very large queries.
 - Fixed DeleteLayer() implementation.
 - Added COPY support for accelerated loading.

MySQL Driver:
 - Added read and write support for Spatial types.
 - Support spatial_ref_sys and geometry_columns tables.
 - Various other improvements (dates, smallint, tinyint, etc)
 - More robust auto-detection of column types for layers
   created from SQL statements

ArcSDE Driver:
 - New for 1.3.2.
 - Read-only support for all geometry types.
 - Supports coordinate systems.
 - Requires SDE C API from ESRI.

Shapefile Driver:
 - Avoid posting errors when .dbf's without .shps are opened.
 - Added pseudo-SQL REPACK command after deleting features.
 - Implement DeleteFeature()

S-57 Driver:
 - Added support for Arcs.
 - Added special DSID_DSSI feature class to capture header info.

DGN Driver:
 - Support writing geometry collections.

DWG/DXF Driver:
 - New for OGR 1.3.2
 - Only supports writing DWG and DXF.
 - Depends on DWGdirect library.


# GDAL 1.3.1 - Overview of Changes

Next Generation SWIG Wrappers (GDAL and OGR):
 - Python, Perl and Ruby bindings considered to be ready to use.
 - C#, Java, PHP are at best initial prototypes.
 - Added configure options for most NG options.

PCRaster Driver:
 - libcsf is now included as part of GDAL.
 - PCRaster enabled by default on win32.
 - --with-pcraster=internal option now supported on unix (but not yet default)

VSI Virtualization:
 - The "large file API" (VSI*L) has been re-engineered to allow installing
   additional file handlers at runtime.
 - Added "in memory" VSI handler so that now any driver using VSI*L
   functions for data access can operate on in-memory files.
 - PNG, JPEG and GeoTIFF drivers upgraded to work with in-memory support.

Raster Attribute Tables:
 - Implemented new Raster Attribute Tables support.  See the
   GDALRasterAttributeTable class for more information.

Erdas Imagine Overviews:
 - Erdas Imagine driver upgraded to support building internal overviews.
 - Generic overview handler updated to support overviews in Erdas Imagine
   format for any file format.  Set USE_RRD config option to YES to enable.

gdalwarp:
 - Added proper support for "unified source nodata", so the -srcnodata
   switch works well.

RIK Driver:
 - New Swedish format driver implemented by Daniel Wallner.

JPEG Driver:
 - Substantial improvements to EXIF support.

MrSID Driver:
 - Updated with proper JPEG2000 support as JP2MRSID driver, including
   encoding with ESDK.
 - Updated to support MrSID Version 5.x SDKs.

PNG Driver:
 - Fixed serious bugs with 16bit file support.
 - Added NODATA_VALUES to identify RGB sets that indicate a nodata pixel.


## OGR 1.3.1 - Overview of Changes

Reference Counting:
 - OGRSpatialReference and OGRFeatureDefn now honour reference counting
   semantics.
 - Note that, especially for the OGRFeatureDefn, it is now critical that
   all drivers be careful with reference counting.  Any OGR drivers not in
   the core distribution will likely crash if not updated.

ESRI Personal Geodatabase Driver:
 - New driver implemented for ESRI Personal Geodatabase (.mdb) files.
 - Uses ODBC, enabled by default on win32.

ODBC Driver:
 - Updated to support binary fields.
 - Updated to support WKB geometry fields.
 - Updated to support DSN-less connections.

S57 Driver:
 - Added support for Inland Waterways, and Additional Military Layers profiles

# GDAL 1.3.0 - Overview of Changes

Multithreading:
 - Lots of work done to implement support for multiple threads reading
   from distinct GDALDataset objects at the same time.

GDALRasterBand / Persistent Auxiliary Metadata (PAM):
 - Support for preserving a variety of metadata in a supporting XML file.
 - GDALRasterBand now supports "remembering" histograms, and has a concept
   of the default histogram.
 - GDALRasterBand now supports remembering image statistics.
 - Disabled by default (set GDAL_PAM_ENABLED=YES to turn on).
 - Supported by *most* drivers with some caveats.

GDALCopyWords():
 - This function is a low level work horse for copying and converting pixel
   data in GDAL.  It has been substantially optimized by Steve Soule (Vexcel).

Next Generation Bindings:
 - Kevin Ruland and Howard Butler are working on reworked support for
   SWIG to generate Python, PHP, Java, C# and other language bindings for GDAL
   and OGR.

VB6 Bindings:
 - Now substantially complete, see VB6 directory.

HDF5 Driver:
 - New HDF5 driver implemented by Denis Nadeau.

RMF Driver:
 - New driver for Raster Matrix Format by Andrey Kislev.

MSGN (Meteosat Second Generation Native) Driver:
 - New driver implemented by Frans van der Bergh.

VRT Driver:
 - Fixed whopper of a memory leak in warped raster case.

NetCDF Driver:
 - Preliminary CF conventions support by Denis Nadeau.

NITF Driver:
 - NITF files between 2 and 4 GB in size can now be read and written.

JPEG Driver:
 - Added support for reading EXIF as metadata by Denis Nadeau.

DODS Driver:
 - Fixed up libdap 3.5.x compatibility.

JP2ECW (JPEG2000 ECW SDK) Driver:
 - Implemented support for new GML-in-JPEG2000 specification.
 - Implemented support for old MSI "worldfile" box.

JP2KAK (JPEG2000 Kakadu) Driver:
 - Implemented support for new GML-in-JPEG2000 specification.
 - Implemented support for old MSI "worldfile" box.

PCIDSK Driver:
 - tiled files now supported for reading.
 - overviews now supported for reading.

HFA (Imagine) Driver:
 - Supports creating internal overviews in very large files.
 - Support reading class names.
 - Support creating compressed files.

GeoTIFF Driver:
 - Support reading files with odd bit depths (i.e. 3, 12, etc).
 - Support 16/24bit floating point TIFFs (per Technote 3) (Andrey).
 - Support 12bit jpeg compressed imagery using libjpeg "MK1" library.

HDF4 Driver:
 - Added support for ASTER Level 1A, 1B and 2 products (Andrey).

## OGR 1.3.0 - Overview of Changes

OGRGeometry:
 - WKT (and GML) encoding now attempts to preserve pretty much full double
   precision.
 - geometries are now "coordinate dimension preserving" rather than dynamically
   figuring out dimension depending on whether Z is set.  So a geometry can
   now be 3D even if all z values are zero.
 - Fixed up proper EMPTY geometry support per standard.

GRASS Driver:
 - New driver for GRASS 6 vector data written by Radim Blazek.

Interlis Driver:
 - New driver for Swiss Interlis format from Permin Kalberer (SourcePole).

Shape Driver:
 - Fixed logic for degenerate polygons (Baumann Konstantin).

PostgreSQL/PostGIS Driver:
 - Implemented fast GetExtent() method (Oleg Semykin).
 - Implemented layer type from geometry_columns (Oleg Semykin).
 - Handle PostGIS 1.0 requirements for coordinate dimension exactness.
 - Handle EWKT type in PostGIS 1.0.
 - Generally PostGIS 0.x and 1.0 should now be supported fairly gracefully.
 - Added PostGIS "binary cursor" mode for faster geometry access.

VRT Driver:
 - Pass through attribute queries to underlying driver.
 - Pass through spatial queries as attribute filters on the underlying layer.

S57 Driver:
 - Added concept of supporting different profiles.
 - Added prototype AML profile support.

MySQL Driver:
 - Fixed for FID recognition (eg. mediumint).

GML Driver:
 - Various fixes for generated GML correctness (Tom Kralidis).

TIGER/Line Driver:
 - Added Tiger 2004 support.

Oracle Driver:
 - Use VARCHAR2 for fixed size string fields.
 - Use OCI_FID config variable when creating layers, and reading select results



# GDAL 1.2.6 - Overview of Changes

gdal_translate:
 - Added -sds switch to copy all subdatasets.

gdalwarp:
 - Added Thin Plate Spline support (-tps switch).

GDALRasterBand:
 - Now uses two level block cache allowing efficient access to files
   with a very large number of tiles.
 - Added support for YCbCr color space for raster band color interpretations.
 - Added AdviseRead() method - currently only used by ECW driver and OGDI
   drivers.

ILWIS Driver:
 - New driver for the raster format of the ILWIS software.

ECW Driver:
 - Updated to use ECW SDK 3.1 (older ECW SDK no longer supported!)

ECWJP2 Driver:
 - Added JPEG2000 support driver based on ECW/JPEG2000 SDK with a variety
   of features.

NITF Driver:
 - Added support for reading *and* writing JPEG2000 compressed NITF files
   using the ECW/JPEG2000 SDK.
 - Added ICHIPB support.

HDF Driver:
 - Add support for georeferencing from some additional metadata formats.
 - Fixed bug with multi-band HDF-EOS datasets.

MrSID Driver:
 - Driver can now be built as a plugin on win32.
 - Split out MrSID 3.x SDK support - not readily buildable now.
 - Implemented accelerated IO cases for MrSID 4.x SDK.
 - Support for writing MrSID files added (improved?)

Imagine Driver:
 - Fixed bug reading some large multiband Imagine files.
 - Added support for writing compressed files.

Win32 Builds:
 - Added versioning information to GDAL DLL.

L1B Driver:
 - Only return a reduced grid of control points.

IDA (WinDisp4) Driver:
 - New read/write driver for the Image Display and Analysis raster format
   used by WinDisp 4.

NDF (NLAPS) Driver:
 - Added NDF/NLAPS read driver for version 1 and 2.

MSG Driver:
 - Added support for the Metosat Second Generation raw file format.

GTiff Driver:
 - Added support for offset/scale being saved and loaded (special metadata).
 - Added Cylindrical Equal Area.
 - Added PROFILE creation option to limit extra tags.

PNG Driver:
 - Updated internal code for libpng to libpng 1.2.8.

## OGR 1.2.6 - Overview of Changes

OGRSFDriverRegistrar:
 - Added support for autoloading plugin drivers from ogr_<driver>.so.

ogr.py:
 - Geometry, and Feature now take care of their own reference counting and
   will delete themselves when unreferenced.  Care must still be taken to
   unreference all features before destroying the corresponding
   layer/datasource.
 - ogr.Feature fields can now be fetched and set directly as attributes.
 - Geometry constructor can now take various formats (wkt, gml, and wkb).
 - Added docstrings.
 - Added better __str__ methods on several objects.
 - Various other improvements.

OGRLayer:
 - Re-wrote generic spatial search support to be faster in case of rectangular
   filters.
 - Intersects() method now really uses GEOS.  This also affects all OGR
   layer spatial filtering (with non-rectangular filters).
 - Added SetNextByIndex() method on OGRLayer.

OGRSpatialReference:
 - Automatically generate +towgs84 from EPSG tables when translating to
   PROJ.4 if available and TOWGS84 not specified in source WKT.
 - Updated GML CRS translation to follow OGC 05-011 more closely.  Still
   incomplete but operational for some projections.
 - Added support for FIPSZONE State Plane processing for old ESRI .prjs.
 - Added Goode Homolosine support.
 - Added GEOS (Geostationary Satellite) support.

OCI (Oracle) Driver:
 - Added GEOMETRY_NAME creation option to control the name of the field to
   hold the geometry.

PostGIS Driver:
 - Fixed some problems with truncation for integer and float list fields.

Shapefile Driver:
 - Added support for MapServer style spatial index (.qix).

GML Driver:
 - Improved support for 3L0 (GML 3 - Level 0 profile) reading and writing.
   On read we can now use the .xsd instead of needing to build a .gfs file.


# GDAL 1.2.5 - Overview of Changes

gdalwarp Utility:
 - Added "proper" source and destination alpha support.

PCRaster Driver:
 - added write support, now consider ready for regular use.

MrSID Driver:
 - Initial support for writing to MrSID with encoding SDK.

GeoTIFF Driver:
 - Updated internal copy of libtiff to fix overview building ... really!
 - Fixed bug when writing south-up images.

## OGR 1.2.5 - Overview of Changes

OGRSpatialReference:
 - Added Bonne projection.

Docs:
 - Added OGR C++ API Tutorial (reading and writing).

PostGIS Driver:
 - Implemented SetFeature() and DeleteFeature() methods for in-place updates.

Oracle (OCI) Driver:
 - Fixed support for writing into Oracle 10g.
 - Fixed serious memory leak of geometries.
 - Fixed bug with 3D multipolygons.
 - Added support for selecting tables in the datasource name.


# GDAL 1.2.4 - Overview of Changes

gdalwarp:
  - Fixed some issues with only partially transformable regions.
  - Added Alpha mask generation support (-dstalpha switch).

HFA/Imagine Driver:
  - bug fix in histogram handling.
  - improved support for large colormaps.

Envi Driver:
  - Capture category names and colormaps when reading.

SAR CEOS Driver:
  - Added support for PALSAR/ALOS Polarimetric Datasets.

RadarSat 2 Driver:
  - New.  Reads RadarSat 2 Polarimetric datasets with a "product.xml" and
    imagery in TIFF files.

OGDI Driver:
  - Important bug fix for downsampled access.

GeoTIFF Driver:
  - Lots of libtiff upgrades, including some quite serious bug fixes.
  - Added better support for 16bit colormaps.
  - Write projection information even if we don't have a geotransform or GCPs.
  - Improved alpha support.
  - Generate graceful error message for BigTIFF files.

DODS Driver:
  - Almost completely reimplemented.   Uses chunk-by-chunk access.  Supports
    reading several bands from separate objects.  Some new limitations too.

NetCDF Driver:
  - Separated out a GMT NetCDF driver and a more generic but partially broken
    NetCDF driver (Radim).

JP2KAK Driver:
  - Added alpha support, including greyscale+alpha.

AirSAR Driver:
  - New, reads AirSAR Polarimetric Radar format.

## OGR 1.2.4 - Overview of Changes

epsg_tr.py:
  - Added escaping rules when generating PostGIS output.

tigerpoly.py:
  - Discard dangles and degenerate rings.

VRT Driver:
  - Fixed serious error in handling cleanup of VRT datasources, was often
   causing a crash.

SQLLite Driver:
  - Fixed substantial memory leaks.

MySQL Driver:
  - New readonly non-spatial MySQL driver implemented.

MITAB Driver:
  - Updated from upstream, several fixes.

TIGER/Line Driver:
  - Fixed serious bug with handling "full" records at end of .RT2 file.

OCI/Oracle Driver:
  - Added OCI_FID environment support to control FID selection.

OGRGeometry:
  - Added Centroid() implementation (from GEOS?)

# GDAL 1.2.3 - Overview of Changes

GeoTIFF Driver:
    - Fixed many missing compression codecs when built with the internal
      libtiff.
    - Modified driver metadata to only list available compression types.

DODS Driver:
    - Added support for OPeNDAP version after 3.4.x (use of opendap-config).

GRASS Driver:
    - Fixed support for building with grass57.

MrSID Driver:
    - Fixed support for MrSID Encoding SDK.

NITF Driver:
    - Fixed serious bug with non-square output files.


## OGR 1.2.3 - Overview of Changes

OGRSpatialReference:
    - Corrected memory leaks - OSRCleanup() cleans up temporary tables.
    - Fixed build problem with ogrct.cpp on Solaris.

TIGER Driver:
    - Improved generality of support for GDT files.

OGRGeometry:
    - Added getArea() method for rings, polygons and multipolygons.


# GDAL 1.2.2 - Overview of Changes

GRASS Driver:
    - Add Radim's version of the driver submitted by Radim.  This version
      uses GRASS 5.7 libraries directly instead of using libgrass.

DODS Driver:
    - Added support for spatial_ref, FlipX and FlipY .das info.

CPG Driver:
    - added new driver for Convair Polarimetric format.

HDF Driver:
    - Significant bugs fixed.

USGS DEM Driver:
    - Support writing UTM projected files.

PNG Driver:
    - Upgraded to libpng 1.2.6.

MrSID Driver:
    - Substantial performance improvements.
    - Support for DSDK 4.x
    - Support JPEG2000 files via MrSID SDK.

NITF Driver:
    - Support JPEG2000 compressed files (with Kakadu support)

ESRI BIL:
    - Support .clr color files.

VRT Driver:
    - Added support for describing raw files with VRTRawRasterBand.
    - Added support for virtual warped files with VRTWarpedRasterBand.

GeoTIFF Driver:
    - Fix support for 16bit image color tables.
    - Write ExtraSamples tag for files with more samples than expected
      in photometric interpretation.
    - External overviews now built for read-only files.

Erdas Imagine Driver:
    - Fixed support for compressed floating point layers.
    - Various other fixes for compatible with newer Imagine versions.
    - improved metadata handling.

gdal_merge.py:
    - sets projection on output file.

## OGR 1.2.2 - Overview of Changes

SQLite Driver:
    - New read/write driver implemented for SQLite databases.

CSV Driver:
    - New read/write driver implemented for comma separated value files.

S-57 Driver:
    - Substantial performance improvements.

ODBC Driver:
    - Arbitrary length field values now supported.

GEOS:
    - Integration a series of methods utilizing GEOS when available.  Note
      that Intersect() is still just an envelope comparison.

OGRSpatialReference:
    - Fixed Swiss Oblique Mercator support.

===========================================================================

# GDAL 1.2.1 - Overview of Changes

gdal_contour:
    - Now build and installed by default.

HDF4 Driver:
    - Added some degree of HDF-EOS support.  HDFEOS layer now part of GDAL.

DODS Driver:
    - Substantial fixes, support for flipped datasets.

HFA (Erdas Imagine) Driver:
    - Fixed bug with files between 2 and 4GB.
    - Capture statistics as metadata.

Erdas 7.x LAN/GIS Driver:
    - Newly implemented.

USGS DEM Driver:
    - Various fixes to creation support / CDED product.

NITF Driver:
    - Capture USE001 and STDIDC TREs as metadata.
    - Capture all sorts of header information as metadata.
    - Support geocentric corner coordinate specification.

MrSID Driver:
    - Support added for DSDK 4.0.x.

ECW Driver:
    - Added preliminary support for using 3.0 SDK for JPEG2000 support.
    - Fix oversampling assertion problem.

ArcInfo Binary Grids:
    - Added support for 0x01 and 0x20 block type.

## OGR 1.2.1 - Overview of Changes

OGRSpatialReference:
    - Various fixes related to prime meridians.

PostgreSQL/PostGIS Driver:
    - Added layer name laundering.
    - Launder names on by default.
    - Clean stale entries in geometry_columns table when creating a layer.
    - Support treating names views as layers.
    - Handle long command strings.

S57 Driver:
    - Fixed serious bugs with support for auto-applying update files.
    - Improvements to S57 writing support.

# GDAL 1.2.0 - Overview of Changes

Configuration:
    - Libtool used by default with Unix style builds.  Use --without-libtool
      to avoid this.
    - PROJ.4 can now be linked statically using --with-static-proj4.
    - Added --without-bsb option for those averse to legal risk.

DODS/OPeNDAP Driver:
    - Preliminary DODS (OPeNDAP) driver implemented (James Gallagher @ URI).

PCIDSK Driver:
    - PCIDSK read/write raster driver implemented (Andrey).

Erdas Imagine / HFA Driver:
    - Support recent Imagine versions (data dictionary changes).
    - Better logic to search for .rrd file locally.
    - Support creating files in the 2GB to 4GB size range.

GIF Driver:
    - Updated to libungif 4.1.0.
    - Various hacks to try and identify transparent colors better.

BMP Driver:
    - Handle 32bit BMPs properly.

HDF4 Driver:
    - Added proper support for multi-sample GR datasets.
    - Various fixes and improvements for specific product types.

GeoTIFF Driver:
    - Added PHOTOMETRIC option to control photometric interp of new files.

JPEG2000/Kakadu Driver:
    - Support reading/creating lossless 16bit files.
    - Updated to support Kakadu 4.1 library.

NITF Driver:
    - Implement support for IGEOLO="U" (MGRS/UTM) coordinates.
    - Added overview (as external GeoTIFF file) support.

MrSID Driver:
    - Support DSDK 4.2.x.

PNG Driver:
    - Support required byte swapping of 16bit PNG data.

FAST Driver:
    - lots of fixes, supports more datums and ellipsoids.

NetCDF Driver:
    - New driver implemented for netCDF support.
    - Pretty much tied to form of netCDF used in GMT for now.

VTerrain .bt Driver:
    - New driver for VTerrain .bt elevation format.

ECW Driver:
    - support supersampled reads efficiently.
    - special case for dataset level RasterIO() implemented for much better
      performance in some applications.

ESRI BIL (EHdr) Driver:
    - Support world files.

VRT Driver:
    - Implement filtering support.

GIO (Arc/Info Binary Grid via avgridio.dll):
   - Driver disabled ... to undependable.


Python:
    - Preliminary support for numarray in addition to numpy (Numeric).

Contouring:
    - New gdal_contour utility program implementing contour generation.
    - Underlying algorithm in gdal/alg.

Warping:
    - Improved support in GDALSuggestedWarpOutput() for "world" sized
      files that are only partially transformable.
    - Bicubic resampler improved.
    - What was gdalwarptest is now gdalwarp, and the old gdalwarp is now
      gdalwarpsimple.  The sophisticated warper is now the default.

Man Pages:
    - Man pages for GDAL utilities now being maintained and installed (Silke).

## OGR 1.2.0 - Overview of Changes

OGRSpatialReference:
   - Added methods for converting to/from GCTP representation.
   - Added HOM 2 points on centerline variant.

DODS (OPeNDAP) Driver:
   - Preliminary implementation.

TIGER/Line Driver:
   - Added support for GDT ASCII TIGER-like format.
   - Support TIGER/Line 2003 format.

S-57 Driver:
   - Preliminary export support implemented.
   - Support capture of FFPT (feature to feature) linkages.
   - Support capture of TOPI from VRPT.
   - Support capture of primitives as additional layers.

Shapefile Driver:
   - gdal/frmts/shapelib removed from GDAL source tree, now just a
     copy of required shapelib files are kept in gdal/ogr/ogrsf_frmts/shape.
   - Attempt identify polygons that are really multi-polygons and convert them
     into multi-polygons properly (Radim Blazek).
   - Create FID attribute in .dbf file if no attribute added by application.

GML Driver:
   - Lots of fixes and improvements for reading and writing.
   - Now writes a schema file by default.
   - Field types are set now when reading based on data found on first pass.
   - Added support for the various kinds of geometry collections.

DGN Driver:
   - Now using dgnlib 1.9 - this carries with it various new element types
     and some important bug fixes.

ODBC Driver:
   - New ODBC driver implemented.  Build by default on Windows, and buildable
     on Unix (with unixodbc).

VRT Driver:
   - New "virtual" OGR Datasource format implemented.
   - Configuration stored in XML control file.

Oracle (OCI) Driver:
   - support reading views.

OGR Core:
   - Added support for WKT EMPTY geometry objects (like "MULTIPOINT(EMPTY)").
   - Added DeleteFeature() method on OGRLayer class.

NTF Driver:
   - Support CHG_TYPE attribute for landline plus product.


# GDAL 1.1.9 - Overview of Changes

 o MrSID Driver: New for 1.1.9, read-only, includes good coordinate system
   support, and should be high performance.

 o ECW Driver: Now reads coordinate system information (but doesn't write).

 o HDF Driver: Added support for Hyperion Level 1, Aster Level 1A/1B/2, MODIS
   Level 1B(earth-view)/2/3, SeaWIFS Level 3.

 o L1B Driver: Now reads GCPs for georeferencing.

 o NITF Driver: Support for reading RPC, variety of bugs fixes for reading and
   writing.  Also some general RPC infrastructure added to GDAL.

 o JP2KAK Driver: Can be used with Kakadu 4.0.2 now.  Compatibility fixes
   for internal geotiff to improve compatibility with Mapping Science tools.
   Added palette support.

 o HFA (Imagine) Driver: Added read/write support for color table opacity.
   Added write support for large (spill) files.

 o "core" directory renamed to "gcore" to avoid confusing configure script.

 o Added support for GDAL_DATA environment variable to point to GDAL support
   data files (those in gdal/data directory).

 o Added GDALDataset::RasterIO() for more efficient reading of multiple bands
   in one request (in some cases anyways).

 o High performance warp api considered to be complete now, and substantially
   optimized.

 o gdal_merge.py: supported multiple bands, copying PCT.


## OGR 1.1.9 - Overview of Changes

 o Oracle Spatial: New generic read/write, and implemented highly optimized
   loading support.

 o Tiger driver: added support for TIGER/Line 2002 product.

 o GML driver:  now supports Xerces versions from 1.6 up to 2.3.  Lots of
   bugs fixes and improvements.   GML Geometry now in OGR core.

 o Improved support for translating to and from ESRI WKT, including a complete
   mapping between EPSG related ESRI datum names and OGR's expected names.

 o Improved support for alternate prime meridians in coordinate system code.

 o Shapefiles: Can write features with NULL geometry,

 o DGN: added 3d write support.

 o Implemented generic attribute indexing support (only used for shapefile
   at this point).  Use in SQL where clauses and ExecuteSQL().

 o WKT MULTIPOINT in/out formatting fixed.

 o Added SynToDisk() method on OGRDataset and OGRLayer.

 o Implemented "Web Coordinate Transformation Service" (ogr/wcts).

 o Implemented "in memory" format driver.

 o C API documented.


# GDAL 1.1.8 - Overview of Changes

 o Implemented HDF 4 read/write support. This includes HDF EOS reading.

 o Implemented Windows BMP read/write support.

 o Implemented NITF read/write support.

 o Implemented NOAA Polar Orbiter L1B format driver.

 o Implemented EOSAT FAST format driver.

 o Implemented a JasPer based JPEG2000 driver (several limitations).

 o Implemented a Kakadu based JPEG2000/GeoJP2(tm) driver (full featured, but
   Kakadu is not open source).

 o Implemented new 'gdalwarp' application for projection and GCP based image
   warping.  See gdal/alg for underlying algorithms.  Currently gdalwarp only
   supports 8 bit images and holds the whole source image in memory.

 o Implemented write support for ESRI ASCII Grids.

 o Lots of improvements to GeoTIFF driver.  Metadata writing, update of
   georeferencing, and support for writing PCS codes based on AUTHORITY fields
   in WKT.

 o Implemented support for uncompressed 1bit data in Erdas Imagine files,
   as well as generic metadata.

 o Fixed 0xFF compression support in the Arc/Info Binary Grid (AIG) driver.

 o Lots of improvements to BSB drive, including preliminary uncompressed
   output support, support for reading BSB 3.0 and GEO/NOS.

 o Lots of work on VRT format.

 o ECW: Fixed bug with reading a more than full resolution.

 o Envisat driver now supports AATSR TOA and MERIS data.

 o Fixes for nodata support in GRASS driver.

 o Added the --version and --formats options to many utility programs.

 o gdal_translate:
    - added -projwin flag to copy a window specified in projection coordinates.
    - added the -a_srs option to assign a user supplied SRS to output file.
    - translation with subsetting to any format now support (uses VRT inside).

 o Lots of metadata now attached to driver objects describing their
   capabilities.

 o Implemented GDALDestroyDriverManager() to ensure full memory cleanup of
   GDAL related resources.

 o Added a 'devinstall' target on Windows to support easy installation of
   include files and stub libraries on Windows.  Also many other improvements
   to Windows build.  Most options can be easily turned on and off from the
   nmake.opt file now.


## OGR 1.1.8 - Overview of Changes

 o Implemented support for writing 2D DGN files.   Added support for MSLINK
   and Text values available as attributes.

 o Implemented FMEObjects based read driver.

 o Implemented ExecuteSQL() method on OGRDataSource.  Generic code supports
   fairly full featured SELECT statements.

 o Various fixes to 3D shapefile support.

 o Fixes to binary representation for 2.5D geometries.  Fixed MULTIPOINT WKT
   geometry representation.

 o Upgraded OGRSpatialReference.importFromEPSG() to use the new EPSG 6.2.2
   tables instead of the old EPSG 4.x tables.

 o Many fixes to PostGIS driver, including special creation options for
   "laundering" field names to save tokens.

 o Many improvements to standards conformance of OGRSpatialReference WKT
   representation to the OGC Coordinate Transformations specification.  Still
   some quirks related to prime meridians and coordinate systems with units
   other than degrees.

 o Implemented support for Meridian 2 NTF files in NTF driver.  Better
   support for GENERIC_CPOLY geometries.

 o  Added support for [NOT] IN, [NOT] LIKE and IS [NOT] NULL predicates in
   WHERE clauses.

 o Implemented a C API for accessing OGR.

 o Implemented support for building OLE DB Provider with Visual Studio.NET
   (many changes in ATL templates).  Lots of other OLE DB improvements for
   better MapGuide compatibility.


# GDAL 1.1.7 - Overview of Changes

 o Add XPM (X11 Pixmap) format.

 o Added rough ENVI raster format read support.

 o Added --version support (and supporting GDALVersionInfo() function).

 o Special hooks for getting raw record data from sar ceos files and Envisat
   via the metadata api.

 o Upgraded TIFF/GeoTIFF support to CVS version ... includes new extension
   API and removes need for private libtiff include files entirely.

 o gdal_translate now has scaling option (-scale).

 o Added utility documentation.

## OGR 1.1.7 - Overview of Changes

 o Added Arc/Info binary coverage format read support.

 o Added ogrtindex for building MapServer compatible OGR tile indexes.

 o Added default implementation of GetFeature(fid) method on OGRLayer.

 o Shape driver now supports reading and creating free standing .dbf files
   for layers without geometry.

 o Added utility documentation.

 o Fixed major memory/file handle leak in SDTS access.

 o Added ADSK_GEOM_EXTENT support for OLE DB provider.

 o Ensure shapefiles written with correct polygon ring winding direction
   plus various other shapefile support fixes.

 o GML read/write working reasonable well, including use of .gfs files.


# GDAL 1.1.6 - Overview of Changes

 o Add > 2GB file support on Linux 2.4.

 o Implemented USGS DEM reading.

 o Implemented BSB Format (Nautical Chart Format) read support.

 o Preliminary implementation of Virtual Datasets (gdal/frmts/vrt).

 o Support for writing DTED files.

 o Some raw formats (i.e. PAux, HKV) support files larger than 2GB.

 o Add the AddBand() method on GDALDataset.

 o PAux: Added color table read support.

 o Various fixes to OGDI driver.

 o Stripped out the GDALProjDef related capabilities.  Superseded by
   OGRSpatialReference, and OGRCoordinateTransformation functionality.

 o Improved CEOS support, notable for ESA LANDSAT files, D-PAF ERS-1 and
   Telaviv ERS data.

 o geotiff: upgraded libtiff support to approximately libtiff 3.5.7.

 o DGN: Added support for complex shapes, shapes assembled from many elements.
   Various other improvements.


## OGR 1.1.6 - Overview of Changes

 o Fixed OGDI driver so that gltp urls with drive letters work properly on
   windows.

 o Many improvements to OLE DB provider during the process of making it
   compatible with the MapGuide (SDP) client.  These include implementing
   restrictions for schema rowsets, treating missing information like WKT
   coordinate systems as NULL fields, and setting ISLONG on geometry fields.
   Also made thread safe.

 o DGN: Threat SHAPE elements as polygons.  Set style information for text.
   Added 3D support for most elements.

 o Fixed bugs in WKT format for some OGR geometry types (i.e. multipoint).

 o Added support for morphing to/from ESRI WKT format for OGRSpatialReference.

 o NTF: Don't try to cache all the records from multiple files at once.

 o Added experimental XML SRS support ... not the final schema.  Added
   supporting "minixml" support to CPL.

 o PostGIS: Upgraded to PostGIS 0.6.  Added "soft transaction" semantics.
   Many create feature calls can now be part of one transaction.  Transactions
   are now a general OGR concept although only implemented for PostGIS.

 o Added transform() and transformTo() methods for reprojecting geometries and
   added user options for this in ogr2ogr.

 o Very preliminary GML read/write support.  Needs Xerces C++ XML parser for
   read support.

# GDAL 1.1.5 New Features

o AIGrid:
- Return nodata value.

o OGDI:
- Added format user documentation.
- Added Sub Dataset support.
- Utilize OGDI 3.1 style capabilities metadata.

o SAR_CEOS:
- Added support for Alaska SAR Toolbox naming convention.
- Read map projection record for corner GCPs.

o PNG Driver:
- read/write support for transparency via colortable and nodata value.

o Erdas Imagine (HFA) Driver:
- Added support for reading external large image files.
- Added support for uncompressed, but reduced precision blocks.

o GIF Driver:
- Added .wld world file support.
- Added transparency read support.
- Upgraded to libungif 4.x.

o JPEG Driver:
- Added .wld world file support.

o PAux Driver:
- Added limited gcp and projection read support.

o GeoTIFF Driver:
- Added specialized support for 1 bit files.
- Upgraded world file reading (added .wld files), use
GDALReadWorldFile().

o JDEM Driver is new (Japanese DEM format).

o FujiBAS Driver is new.

o ERMapper ECW Driver is new.

o GDAL Bridge: upgraded to include new entry points, like GCP access and
nodata api.

o gdal_translate: added the -not_strict option.

o GDALGetRandomRasterSample(): Return magnitude for random samples.

o Added use of CPL_CVSID macro in most source files. Running the RCS ident
command on any GDAL executable or shared library should now give a listing
of most object file versions from which it was built.

o Various improvements so that static builds will work under Cygwin.

o Various improvements so that builds can be done on MacOS X.

o Overviews: Implement AVERAGE_MAGPHASE option for complex image overviews.

o Added support for sub datasets to gdalinfo, core api and OGDI raster driver.

o The size of the GDAL cache can now be overridden with the GDAL_CACHEMAX
environment variable (measured in MB).

o Added Driver implementation tutorial to documentation.

o Added apps/gdaltindex.c - application for building tile indexed raster
datasets suitable for use with UMN MapServer.


## GDAL 1.1.5 Significant Bug Fixes

o SAR_CEOS:
- Don't try to get GCPs from scanlines with no prefix data.

o GeoTIFF:
- Fixed handling of RGBA band ordering on big endian systems.
- Fixed bugs in overview generation, especially when updating in place.

o gdal-config should work properly in all situations now.

o JPEG Driver: improved magic number tested to avoid ignoring some jpeg files.

o FITS Driver: lots of fixes and improvements.


## OGR 1.1.5 New Features

o Implemented support for attribute query filters (SetAttributeFilter())
on OGRLayer, provided SWQ based implementation, plugged into all
drivers and added hooks to ogrinfo.

o MapInfo Driver:
- Added accelerated spatial query support.
- Upgraded to current MITAB source as of GDAL release date.

o S-57 Driver:
- Added support for applying S-57 updates automatically.

o SDTS Driver:
- Added ENID and SNID to line features.
- Return coordinate system in WKT instead of PROJ.4 format.

o Shapefile Driver:
- Auto determine shapefile type from first object written.
- Added good support for NULL shapes, and NULL attribute fields.
- Added support for .prj files (read and write).

o PostgreSQL Driver:
- Added PostGIS support.
- Pass attribute queries through to PostgreSQL.

o NTF Driver:
- Added support for GTYPE 5 geometries (a type of arc).
- Added support for GEOMETRY3D records in indexed (generic) datasets.

o TIGER/Line Driver:
- Added write support.
- Improved read support for TIGER 2000.

o OLE DB Provider:
- Added support for spatial queries via ICommand parameters.
- Added support for attribute queries by parsing out WHERE clause.
- In general substantial rework and extensions were made to make it
work with ESRI and AutoDesk clients.

o Added gdal/data/stateplane.txt - a test file with one line per state plane
zone for applications wanting to present options to users.

o Install ogrsf_frmts.a on install if building with OGR support enabled.

o Reports layer extents in ogrinfo.

## OGR 1.1.5 Significant Bug Fixes

o OGRSpatialReference:
- Fix bug with extracting linear units from EPSG derived definitions.
- Fixed bug translating LCC from EPSG to WKT (importFromEPSG()).
- Improved IsSame() test for GEOGCS now.
- Fixed crash if PROJECTION missing from PROJCS definition.

o S-57:
- Improve recovery from corrupt line geometries.
- Read objects as generic if the object class is not recognised.
- Handle LIST attributes as a string, instead of as a single int.

o NTF:
- Fixed circle conversion to polylines to close the circle properly.
- Upped MAX_LINK to 5000 to handle much more complex geometries.

o DGN:
- Don't include elements with the complex bit set in extents
computations.

o OGRGeometry:
- Fixed WKT format (import and export) for various container types.
- WKT import fixed for coordinates, and Z coordinates.
