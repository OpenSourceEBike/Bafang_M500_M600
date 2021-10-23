## Overview

This tool generates *.c* files that can be converted into *.mex* files by the
configuration tool. For each component in the project, it retrieves *PEx* data
configuration and maps it to *YAML* output format.

## Usage

    gen_s32ct.exe -p /path/to/PEx_file.pe -o /path/to/output/directory -r [release_name] -s [sourceVersion]

 Output files will be generated in **/path/to/output/directory** folder.
 If the folder already exists and contains filenames that start with **peripherals_**,
 these files will be removed before generating the new files
 sourceVersion is SDK release version, default will be 303 (s32sdk_s32k1xx_rtm_3.0.3)
 release_name is optional. By default files will be generated for s32sdk_s32k1xx_rtm_402.
 You can specify the release name to override the value.
