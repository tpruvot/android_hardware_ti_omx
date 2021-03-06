/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
#ifndef TI_VIDEO_CONFIG_PARSER_H_INCLUDED
#define TI_VIDEO_CONFIG_PARSER_H_INCLUDED

#include "oscl_base.h"
#include "oscl_types.h"
#include "pvmf_format_type.h"
#include "oscl_stdstring.h"

#define WVGA_MAX_WIDTH 864
#define WVGA_MAX_HEIGHT 480
#define MAX_WIDTH_720P 1280
#define MAX_HEIGHT_720P 720
#define TI_VID_DEC "OMX.TI.Video.Decoder"
#define ITTIAM_VID_DEC "OMX.TI.720P.Decoder"
typedef struct
{
    uint8 *inPtr;
    uint32 inBytes;
    PVMFFormatType iMimeType;
} tiVideoConfigParserInputs;

typedef struct _tiVideoConfigParserOutputs
{
    uint32 width;
    uint32 height;
    uint32 profile;
    uint32 level;
    uint32 entropy; //interlaced in newer sources (OMX_BOOL) 0-progressive. 1-interlaced
} tiVideoConfigParserOutputs;

OSCL_IMPORT_REF int16 ti_video_config_parser(
        tiVideoConfigParserInputs *aInputs,
        tiVideoConfigParserOutputs *aOutputs,
        char * pComponentName);

#endif //TI_VIDEO_CONFIG_PARSER_H_INCLUDED


