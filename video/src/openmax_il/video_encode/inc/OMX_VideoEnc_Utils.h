
/*
 * Copyright (C) Texas Instruments - http://www.ti.com/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
/* =============================================================================
*             Texas Instruments OMAP(TM) Platform Software
*  (c) Copyright Texas Instruments, Incorporated.  All Rights Reserved.
*
*  Use of this software is controlled by the terms and conditions found 
*  in the license agreement under which this software has been supplied.
* =========================================================================== */
/**
* @file OMX_VideoEnc_Utils.h
*
* This file implements OMX Component for MPEG-4 encoder that 
* is fully compliant with the OMX specification 1.5.
*
* @path  $(CSLPATH)\inc
*
* @rev  0.1
*/
/* -------------------------------------------------------------------------- */
/* ============================================================================= 
*! 
*! Revision History 
*! ================================================================
*!
*! 02-Feb-2006 mf: Revisions appear in reverse chronological order; 
*! that is, newest first.  The date format is dd-Mon-yyyy.  
* =========================================================================== */

#ifndef OMX_VIDEOENC_UTILS__H
#define OMX_VIDEOENC_UTILS__H

#include "LCML_DspCodec.h"
#include "LCML_Types.h"
#include "LCML_CodecInterface.h"
#ifdef RESOURCE_MANAGER_ENABLED
#include <ResourceManagerProxyAPI.h>
#endif
#include "OMX_VideoEnc_DSP.h"
#ifdef __PERF_INSTRUMENTATION__
    #include "perf.h"
    #endif
#include <OMX_Component.h>

#ifdef UNDER_CE
    #include <oaf_debug.h>
    #include <pthread.h>
#endif

/* this is the max of VIDENC_MAX_NUM_OF_IN_BUFFERS and VIDENC_MAX_NUM_OF_OUT_BUFFERS */
#define VIDENC_MAX_NUM_OF_BUFFERS     5
#define VIDENC_MAX_NUM_OF_IN_BUFFERS  5
#define VIDENC_MAX_NUM_OF_OUT_BUFFERS 4 
#define VIDENC_NUM_OF_IN_BUFFERS  4
#define VIDENC_NUM_OF_OUT_BUFFERS 4 
#define VIDENC_NUM_OF_PORTS 2


#if 1 
    #define GPP_PRIVATE_NODE_HEAP
#endif

#if 1
    #define VIDENC_NUM_CUSTOM_INDEXES 22
    #define __NAL_IMPLEMENTATION__
#else
    #define VIDENC_NUM_CUSTOM_INDEXES 20
#endif

#if 1
    #define __KHRONOS_CONF__
#endif

#if 1
    #define __KHRONOS_CONF_1_1__
#endif
    
#define VIDENC_MAX_COMPONENT_TIMEOUT 0xFFFFFFFF 
#define OMX_NOPORT 0xFFFFFFFE
#define MAXNUMSLCGPS 8  /*< max. number of slice groups*/
/* Remove after OMX 1.1 migration */ 
#ifndef __KHRONOS_CONF_1_1__
    #define OMX_BUFFERFLAG_SYNCFRAME 0x00000040 
#endif
#define OMX_CFRAMETYPE_H264 1
#define OMX_CFRAMETYPE_MPEG4 1
/*Select Timeout */
#define  VIDENC_TIMEOUT_SEC 120;
#define  VIDENC_TIMEOUT_USEC 0;

/*
 * Redirects control flow in an error situation.
 * The OMX_CONF_CMD_BAIL label is defined inside the calling function.
 */
#define OMX_CONF_BAIL_IF_ERROR(_eError)                     \
{                                                           \
    if(_eError != OMX_ErrorNone) {                          \
        OMX_TRACE("*Error : %x\n", eError);                 \
        goto OMX_CONF_CMD_BAIL;                             \
        }                                                   \
}

#define OMX_VIDENC_BAIL_IF_ERROR(_eError, _hComp)           \
{                                                           \
    if(_eError != OMX_ErrorNone) {	\
        _eError = OMX_VIDENC_HandleError(_hComp, _eError);  \
        if(_eError != OMX_ErrorNone) {                      \
            OMX_TRACE("*Fatal Error : %x\n", _eError);      \
            goto OMX_CONF_CMD_BAIL;                         \
        }                                                   \
    }                                                       \
}

/*
 * Sets error type and redirects control flow to error handling and cleanup section
 */
#define OMX_CONF_SET_ERROR_BAIL(_eError, _eCode)\
{                                                           \
    _eError = _eCode;                                       \
    OMX_TRACE("*Error : %x \n", eError);                     \
    goto OMX_CONF_CMD_BAIL;                                 \
}

#define OMX_VIDENC_SET_ERROR_BAIL(_eError, _eCode, _hComp)\
{                   \
    _eError = _eCode;                                       \
    OMX_TRACE("*Fatal Error : %x\n", eError);               \
    OMX_VIDENC_HandleError(_hComp, _eError);                \
    goto OMX_CONF_CMD_BAIL;                                 \
}

/*
 * Checking paramaters for non-NULL values.
 * The macro takes three parameters because inside the code the highest 
 *   number of parameters passed for checking in a single instance is three.
 * In case one or two parameters are passed, the ramaining parameters 
 *   are set to 1 (or a nonzero value). 
 */
#define OMX_CONF_CHECK_CMD(_ptr1, _ptr2, _ptr3)             \
{                                                           \
    if(!_ptr1 || !_ptr2 || !_ptr3){                         \
        eError = OMX_ErrorBadParameter;                     \
        OMX_TRACE("*Error : %x\n", eError);                 \
        goto OMX_CONF_CMD_BAIL;                             \
    }                                                       \
}

/*
* Initialize the Circular Buffer data. The Tail and Head pointers are NULL.
*The number of nodes inside the circular buffer is equal to zero. 
*Also the number of nodes that contains BufferData is iqual zero.
*It should be in the ComponentInit call of the Component.
*/
#define OMX_CONF_CIRCULAR_BUFFER_INIT(_pPrivateData_)       \
    (_pPrivateData_)->sCircularBuffer.pHead = NULL;         \
    (_pPrivateData_)->sCircularBuffer.pTail = NULL;         \
    (_pPrivateData_)->sCircularBuffer.nElements = 0;        \
    (_pPrivateData_)->sCircularBuffer.nFillElements = 0;

/*
*Restart the Circular Buffer. The tail points to the same node as the head. The 
*number of fill elements is set to zero. It should be put in the Idle->Execution
*transition. 
*/
#define OMX_CONF_CIRCULAR_BUFFER_RESTART(_sCircular_)       \
    (_sCircular_).pTail = (_sCircular_).pHead;              \
    (_sCircular_).nFillElements = 0;
    
/*
*Add node to the Circular Buffer.  Should be use when UseBuffer or AllocateBuffer
*is call. The new node is insert in the head of the list. The it will go the last node 
*and rewrite pNext with the new address of the Head.
*/
#define OMX_CONF_CIRCULAR_BUFFER_ADD_NODE(_pPrivateData_, _sCircular_)\
{                                                           \
    if((_sCircular_).nElements < VIDENC_MAX_NUM_OF_BUFFERS) \
    {                                                       \
        OMX_U8 _i_ = 0;                                      \
        OMX_CONF_CIRCULAR_BUFFER_NODE* pTmp = (_sCircular_).pHead;\
        VIDENC_MALLOC( (_sCircular_).pHead,                 \
                        sizeof(OMX_CONF_CIRCULAR_BUFFER_NODE),\
                        OMX_CONF_CIRCULAR_BUFFER_NODE,      \
                        (_pPrivateData_)->pMemoryListHead); \
        (_sCircular_).nElements++;                          \
        if(!pTmp){                                           \
            (_sCircular_).pHead->pNext = (_sCircular_).pHead;\
            (_sCircular_).pTail = (_sCircular_).pHead;      \
        }                                                   \
        else{                                               \
            (_sCircular_).pHead->pNext = pTmp;              \
            for(_i_=2 ; _i_ < (_sCircular_).nElements; _i_++) \
                    pTmp = pTmp->pNext;                     \
            pTmp->pNext = (_sCircular_).pHead;              \
        }                                                   \
    }                                                       \
}

/*
* Will move the Tail of the Cirular Buffer to the next element. In the tail resides the last buffer to enter 
*the component from the Application layer. It will get all the Data to be propageted from
* the pBufferHeader and write it in the node. Then it will move the Tail to the next element.
*It should be put in the function that handles the filled buffer from the application.
*/
#define OMX_CONF_CIRCULAR_BUFFER_MOVE_TAIL(_pBufHead_, _sCircular_, _pPrivateData_)\
{                                                           \
    if((_pPrivateData_)->pMarkBuf){                        \
        (_sCircular_).pTail->pMarkData = (_pPrivateData_)->pMarkBuf->pMarkData;\
        (_sCircular_).pTail->hMarkTargetComponent = (_pPrivateData_)->pMarkBuf->hMarkTargetComponent;\
        (_pPrivateData_)->pMarkBuf = NULL;                  \
    }                                                       \
    else{                                                   \
        (_sCircular_).pTail->pMarkData = (_pBufHead_)->pMarkData; \
        (_sCircular_).pTail->hMarkTargetComponent = (_pBufHead_)->hMarkTargetComponent;\
    }                                                       \
    (_sCircular_).pTail->nTickCount = (_pBufHead_)->nTickCount;\
    (_sCircular_).pTail->nTimeStamp = (_pBufHead_)->nTimeStamp;\
    (_sCircular_).pTail->nFlags = (_pBufHead_)->nFlags;      \
    (_sCircular_).pTail = (_sCircular_).pTail->pNext;       \
    (_sCircular_).nFillElements++;                          \
    if(((_sCircular_).pTail == (_sCircular_).pHead) &&      \
       ((_sCircular_).nFillElements != 0)){                 \
        OMX_TRACE("**Warning:Circular Buffer Full.\n");       \
    }                                                       \
}

/*
*Will move the Head of the Circular Buffer to the next element. In the head is the Data of the first Buffer
*to enter to the Application layer. It will propagate the Data and put it in the pBufferHeader
*that goes to the Application layer. Then it will move the Head to the Next element.
*It should be put in the function that handles the filled buffers that comes from the DSP.
*/
#define OMX_CONF_CIRCULAR_BUFFER_MOVE_HEAD(_pBufHead_, _sCircular_)\
{                                                           \
    (_pBufHead_)->pMarkData = (_sCircular_).pHead->pMarkData;\
    (_pBufHead_)->hMarkTargetComponent = (_sCircular_).pHead->hMarkTargetComponent;\
    (_pBufHead_)->nTickCount = (_sCircular_).pHead->nTickCount;\
    (_pBufHead_)->nTimeStamp = (_sCircular_).pHead->nTimeStamp;\
    (_pBufHead_)->nFlags = (_sCircular_).pHead->nFlags;      \
    (_sCircular_).pHead = (_sCircular_).pHead->pNext;       \
    (_sCircular_).nFillElements--;                          \
    if(((_sCircular_).pTail == (_sCircular_).pHead) &&      \
       ((_sCircular_).nFillElements == 0)){                 \
        OMX_TRACE("**Warning:Circular Buffer Empty.\n");      \
    }                                                       \
}

/*
*This Macro will delete a node from the Circular Buffer. It will rearrenge the conections
*between the nodes, and restart the CircularBuffer. The Tail and Head will point to the same
*location and the nFillElement will be set to 0. It should be in the FreeBuffer call.
*/
#define OMX_CONF_CIRCULAR_BUFFER_DELETE_NODE(_pPrivateData_, _sCircular_)\
{                                                           \
    OMX_CONF_CIRCULAR_BUFFER_NODE* pTmp1 = (_sCircular_).pHead;\
    OMX_CONF_CIRCULAR_BUFFER_NODE* pTmp2 = NULL;            \
    if(((_sCircular_).pHead != NULL) &&                     \
       ((_sCircular_).pTail != NULL)){                      \
        while(pTmp1->pNext != (_sCircular_).pHead){         \
            pTmp2 = pTmp1;                                  \
            pTmp1 = pTmp1->pNext;                           \
        }                                                   \
        VIDENC_FREE(pTmp1,(_pPrivateData_)->pMemoryListHead);\
        (_sCircular_).nElements--;                          \
        (_sCircular_).nFillElements = 0;                    \
        if(pTmp2 != NULL){                                  \
            pTmp2->pNext = (_sCircular_).pHead;             \
            (_sCircular_).pTail = (_sCircular_).pHead;      \
        }                                                   \
        else {                                              \
            (_sCircular_).pHead = NULL;                     \
            (_sCircular_).pTail = NULL;                     \
        }                                                   \
    }                                                       \
}

/*
 * Checking for version compliance.
 * If the nSize of the OMX structure is not set, raises bad parameter error.
 * In case of version mismatch, raises a version mismatch error.
 */


#define OMX_CONF_CHK_VERSION(_s_, _name_, _e_)              \
    if((_s_)->nSize != sizeof(_name_)) _e_ = OMX_ErrorBadParameter; \
    if(((_s_)->nVersion.s.nVersionMajor != 0x1)||           \
       ((_s_)->nVersion.s.nVersionMinor != 0x0)||           \
       ((_s_)->nVersion.s.nRevision != 0x0)||               \
       ((_s_)->nVersion.s.nStep != 0x0)) _e_ = OMX_ErrorVersionMismatch;\
    if(_e_ != OMX_ErrorNone) goto OMX_CONF_CMD_BAIL;

/* 
 * Initializes a data structure using a pointer to the structure.
 * The initialization of OMX structures always sets up the nSize and nVersion fields 
 *   of the structure.
 */
#define OMX_CONF_INIT_STRUCT(_s_, _name_)       \
    (_s_)->nSize = sizeof(_name_);              \
    (_s_)->nVersion.s.nVersionMajor = 0x1;      \
    (_s_)->nVersion.s.nVersionMinor = 0x0;      \
    (_s_)->nVersion.s.nRevision     = 0x0;      \
    (_s_)->nVersion.s.nStep         = 0x0    
    
    
/* Event Handler Macro*/
#define OMX_VIDENC_EVENT_HANDLER(_hComponent_, _eEvent_, _nData1_, _nData2_, _pEventData_) \
{                                                           \
    if((_hComponent_)->bHideEvents != OMX_TRUE )            \
        (_hComponent_)->sCbData.EventHandler((_hComponent_)->pHandle, \
                                            (_hComponent_)->pHandle->pApplicationPrivate, \
                                            _eEvent_,       \
                                            _nData1_,       \
                                            _nData2_,       \
                                            _pEventData_);  \
                                                            \
        OMX_TRACE("EventHandler : %x : %x : %x \n", _eEvent_, _nData1_, _nData2_); \
                                                            \
}                                        

#define VIDENC_MALLOC(_p_, _s_, _c_, _h_)                   \
    _p_ = (_c_*)malloc(_s_);                                \
    if (_p_ == NULL) {                                      \
        OMX_EPRINT("malloc() error.\n");                    \
        eError = OMX_ErrorInsufficientResources;            \
        goto OMX_CONF_CMD_BAIL;                             \
    }                                                       \
    else {                                                  \
        OMX_TRACE(": %d :malloc() -> %p\n", __LINE__, _p_); \
    }                                                       \
    memset((_p_), 0x0, _s_);                                \
    if ((_p_) == NULL) {                                    \
        OMX_EPRINT("memset() error.\n");                    \
        eError = OMX_ErrorUndefined;                        \
        goto OMX_CONF_CMD_BAIL;                             \
    }                                                       \
    eError = OMX_VIDENC_ListAdd(_h_, _p_);                  \
    if (eError == OMX_ErrorInsufficientResources) {         \
        OMX_EPRINT("malloc() error.\n");                    \
        goto OMX_CONF_CMD_BAIL;                             \
    } 
    
#define VIDENC_FREE(_p_, _h_)                               \
    OMX_VIDENC_ListRemove(_h_, _p_);                        \
    _p_ = NULL;                                             \

typedef struct VIDENC_NODE {
    OMX_PTR pData;
    struct VIDENC_NODE* pNext;
}VIDENC_NODE;
    
typedef enum VIDEOENC_PORT_INDEX {
    VIDENC_INPUT_PORT = 0x0,
    VIDENC_OUTPUT_PORT
} VIDEOENC_PORT_INDEX;

/* Custom set/get param */
typedef struct VIDENC_CUSTOM_DEFINITION
{
    OMX_U8 cCustomName[128];
    OMX_INDEXTYPE nCustomIndex;
} VIDENC_CUSTOM_DEFINITION;

#ifdef __NAL_IMPLEMENTATION__ 
/*Structures for NAL Support*/
typedef struct VIDENC_NAL_NODE {
    OMX_U32 Size;
    struct VIDENC_NAL_NODE* pNext;
}VIDENC_NAL_NODE;
#endif

typedef struct OMX_CONF_CIRCULAR_BUFFER_NODE {
    OMX_HANDLETYPE hMarkTargetComponent;
    OMX_PTR pMarkData;
    OMX_U32 nTickCount;
    OMX_TICKS nTimeStamp;
    OMX_U32 nFlags;
    struct OMX_CONF_CIRCULAR_BUFFER_NODE* pNext;
} OMX_CONF_CIRCULAR_BUFFER_NODE;

typedef struct OMX_CONF_CIRCULAR_BUFFER{
    struct OMX_CONF_CIRCULAR_BUFFER_NODE* pHead;
    struct OMX_CONF_CIRCULAR_BUFFER_NODE* pTail;
    OMX_U8 nElements;
    OMX_U8 nFillElements;
} OMX_CONF_CIRCULAR_BUFFER;

typedef enum VIDENC_CUSTOM_INDEX {
    VideoEncodeCustomParamIndexVBVSize = OMX_IndexIndexVendorStartUnused,
    VideoEncodeCustomParamIndexDeblockFilter,
    VideoEncodeCustomConfigIndexForceIFrame,
    VideoEncodeCustomConfigIndexIntraFrameInterval,
    VideoEncodeCustomConfigIndexTargetFrameRate,
    VideoEncodeCustomConfigIndexQPI,
    VideoEncodeCustomConfigIndexAIRRate,
    VideoEncodeCustomConfigIndexTargetBitRate,
    VideoEncodeCustomConfigIndexNAL,
    VideoEncodeCustomConfigIndexNALnd,
    /*ASO*/
    VideoEncodeCustomConfigIndexNumSliceASO,
    VideoEncodeCustomConfigIndexAsoSliceOrder,
    /*FMO*/
    VideoEncodeCustomConfigIndexNumSliceGroups,
    VideoEncodeCustomConfigIndexSliceGroupMapType,
    VideoEncodeCustomConfigIndexSliceGroupChangeDirectionFlag,
    VideoEncodeCustomConfigIndexSliceGroupChangeRate,
    VideoEncodeCustomConfigIndexSliceGroupChangeCycle,
    VideoEncodeCustomConfigIndexSliceGroupParams,
	/*Segment mode information*/
	VideoEncodeCustomConfigIndexMVDataEnable,
	VideoEncodeCustomConfigIndexResyncDataEnable,
	/*others*/
	VideoEncodeCustomConfigIndexMIRRate,
    VideoEncodeCustomConfigIndexMaxMVperMB,
    VideoEncodeCustomConfigIndexIntra4x4EnableIdc,
    /*only for H264*/
    VideoEncodeCustomParamIndexEncodingPreset
} VIDENC_CUSTOM_INDEX;

typedef enum VIDENC_BUFFER_OWNER {
    VIDENC_BUFFER_WITH_CLIENT = 0x0,
    VIDENC_BUFFER_WITH_COMPONENT,
    VIDENC_BUFFER_WITH_DSP,
    VIDENC_BUFFER_WITH_TUNNELEDCOMP 
} VIDENC_BUFFER_OWNER;

typedef struct VIDENC_BUFFER_PRIVATE {
	OMX_PTR pMetaData;/*pointer to metadata structure, this structure is used when MPEG4 segment mode is enabled  */
    OMX_BUFFERHEADERTYPE* pBufferHdr;
    OMX_PTR pUalgParam;
    VIDENC_BUFFER_OWNER eBufferOwner;
    OMX_BOOL bAllocByComponent;
    OMX_BOOL bReadFromPipe;
} VIDENC_BUFFER_PRIVATE;

typedef struct VIDENC_MPEG4_SEGMENTMODE_METADATA{
	unsigned int mvDataSize;/*unsigned int*/
	unsigned int numPackets;/*unsigned int*/
	OMX_PTR pMVData;/*pointer to unsigned char MVData[3264]*/
	OMX_PTR pResyncData;/*pointer to unsigned char ResyncData[5408]*/
}VIDENC_MPEG4_SEGMENTMODE_METADATA;

typedef struct VIDEOENC_PORT_TYPE {
    OMX_U32 nBufferCnt;
    OMX_U32 nTunnelPort;
    OMX_HANDLETYPE hTunnelComponent;
    OMX_BUFFERSUPPLIERTYPE eSupplierSetting;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDef;
    OMX_VIDEO_PARAM_PORTFORMATTYPE* pPortFormat;

#ifdef __KHRONOS_CONF_1_1__
	OMX_VIDEO_PARAM_PROFILELEVELTYPE* pProfileType;
	OMX_CONFIG_FRAMERATETYPE* pFrameRateConfig;
	OMX_VIDEO_CONFIG_BITRATETYPE* pBitRateTypeConfig;
	OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE* pErrorCorrectionType;
	OMX_VIDEO_PARAM_INTRAREFRESHTYPE* pIntraRefreshType;
#endif

	OMX_VIDEO_PARAM_BITRATETYPE* pBitRateType;
    VIDENC_BUFFER_PRIVATE* pBufferPrivate[VIDENC_MAX_NUM_OF_BUFFERS];
} VIDEOENC_PORT_TYPE;

/**
 * The VIDENC_COMPONENT_PRIVATE data structure is used to store component's 
 *                              private data.
 */
typedef struct VIDENC_COMPONENT_PRIVATE{
    OMX_PORT_PARAM_TYPE* pPortParamType;
    VIDEOENC_PORT_TYPE* pCompPort[VIDENC_NUM_OF_PORTS];
#ifdef __KHRONOS_CONF_1_1__
	OMX_PORT_PARAM_TYPE* pPortAudioType;
	OMX_PORT_PARAM_TYPE* pPortImageType;
	OMX_PORT_PARAM_TYPE* pPortOtherType;
#endif

    OMX_PRIORITYMGMTTYPE* pPriorityMgmt;
    OMX_VIDEO_PARAM_AVCTYPE* pH264;
    OMX_VIDEO_CONFIG_AVCINTRAPERIOD*  pH264IntraPeriod;  /* for intraFrameInterval */
    OMX_VIDEO_PARAM_MOTIONVECTORTYPE* pMotionVector;     /* for searchRange, maxMVperMB, qpi */
    OMX_VIDEO_PARAM_MPEG4TYPE* pMpeg4;
    OMX_VIDEO_PARAM_H263TYPE* pH263;
    OMX_VIDEO_PARAM_BITRATETYPE* pVidParamBitrate;
    OMX_VIDEO_PARAM_QUANTIZATIONTYPE* pQuantization;

    OMX_CALLBACKTYPE sCbData;
    OMX_COMPONENTTYPE* pHandle;
    OMX_STATETYPE eState;
    OMX_VERSIONTYPE ComponentVersion;
    OMX_VERSIONTYPE SpecVersion;
    OMX_STRING cComponentName;
    int nFree_oPipe[2];
    int nFilled_iPipe[2];
    int nCmdPipe[2];
    int nCmdDataPipe[2]; 
    void* pModLcml;
    void* pLcmlHandle;
    LCML_DSP_INTERFACE* pLCML;
    int nFrameCnt;
#ifdef __PERF_INSTRUMENTATION__
    PERF_OBJHANDLE pPERF, pPERFcomp;
    OMX_U32 nLcml_nCntIp;         
    OMX_U32 nLcml_nCntOpReceived;
#endif
    unsigned int nVBVSize; 
    OMX_MARKTYPE* pMarkBuf;
    OMX_PTR pMarkData;
    OMX_HANDLETYPE hMarkTargetComponent; 
    OMX_U32 nFlags; 
    OMX_U32 nCounter;
    /* these are duplicates */
    unsigned int nIntraFrameInterval;  /* should be OMX_VIDEO_CONFIG_AVCINTRAPERIOD */
    unsigned int nTargetFrameRate;  /* should be OMX_CONFIG_FRAMERATETYPE */
    unsigned int nQPI;              /* same as OMX_VIDEO_PARAM_QUANTIZATIONTYPE */
    unsigned int nAIRRate;          /* same as OMX_VIDEO_PARAM_INTRAREFRESHTYPE */
    unsigned int nTargetBitRate;    /* should be OMX_VIDEO_CONFIG_BITRATETYPE */
	OMX_U32 nMIRRate;
#ifndef UNDER_CE
    pthread_mutex_t mVideoEncodeBufferMutex; 
#endif
    OMX_BOOL bDeblockFilter;
    OMX_BOOL bCodecStarted;
    OMX_BOOL bCodecLoaded;
    OMX_BOOL bDSPStopAck;
    OMX_BOOL bForceIFrame; 
    OMX_BOOL bFlushComplete;
    OMX_BOOL bEmptyPipes;
    OMX_BOOL bHideEvents;
    OMX_BOOL bHandlingFatalError;
    OMX_BOOL bUnresponsiveDsp;
    VIDENC_NODE*  pMemoryListHead;
#ifdef __NAL_IMPLEMENTATION__    
    VIDENC_NAL_NODE* pNALListHead;
    OMX_BOOL bNAL;
    OMX_U32 nNALCounter;
    OMX_U32* pNAL_nd;
#endif
    OMX_CONF_CIRCULAR_BUFFER sCircularBuffer;

#ifdef __KHRONOS_CONF__
#ifdef __KHRONOS_CONF_1_1__
    OMX_PARAM_COMPONENTROLETYPE componentRole;
#endif
    OMX_BOOL bPassingIdleToLoaded;
    OMX_BOOL bErrorLcmlHandle;
    pthread_t ComponentThread;
#endif

/*ASO*/
    OMX_U32 numSliceASO;
    OMX_U32 asoSliceOrder[MAXNUMSLCGPS];
/*FMO*/
    OMX_U32 numSliceGroups;
    OMX_U32 sliceGroupMapType;
    OMX_U32 sliceGroupChangeDirectionFlag;
    OMX_U32 sliceGroupChangeRate;
    OMX_U32 sliceGroupChangeCycle;
    OMX_U32 sliceGroupParams[MAXNUMSLCGPS];
#ifndef UNDER_CE
	pthread_mutex_t videoe_mutex;	/* pthread_cond_t  control_cond; */
	pthread_mutex_t videoe_mutex_app;
	pthread_cond_t  populate_cond;
	pthread_cond_t  unpopulate_cond;
	pthread_cond_t  stop_cond;
	pthread_cond_t  flush_cond;
#else
    OMX_Event AlloBuf_event;
    OMX_U8 AlloBuf_waitingsignal;

    OMX_Event InLoaded_event;
    OMX_U8 InLoaded_readytoidle;

    OMX_Event InIdle_event;
    OMX_U8 InIdle_goingtoloaded;
#endif
	unsigned int nEncodingPreset;
	OMX_BOOL bMVDataEnable;
	OMX_BOOL bResyncDataEnable;
	IH264VENC_Intra4x4Params intra4x4EnableIdc;
	OMX_U32 maxMVperMB;	
	#ifdef RESOURCE_MANAGER_ENABLED
	RMPROXY_CALLBACKTYPE cRMCallBack;
	#endif
	OMX_BOOL bPreempted;
} VIDENC_COMPONENT_PRIVATE;

typedef OMX_ERRORTYPE (*fpo)(OMX_HANDLETYPE);

/*--------function prototypes ---------------------------------*/

#ifndef UNDER_CE
    OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE hComp);
#endif
OMX_ERRORTYPE OMX_VIDENC_HandleLcmlEvent(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, TUsnCodecEvent eEvent);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSet(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSet (VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSetIdle(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSetLoaded (VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSetExecuting(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandStateSetPause (VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandFlush(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1, OMX_BOOL bInternalFlush);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandDisablePort(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1);

OMX_ERRORTYPE OMX_VIDENC_HandleCommandEnablePort(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1);

OMX_ERRORTYPE OMX_VIDENC_Process_FilledInBuf(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_Process_FilledOutBuf(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_BUFFERHEADERTYPE* pBufHead);

OMX_ERRORTYPE OMX_VIDENC_Process_FreeInBuf(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_BUFFERHEADERTYPE* pBufHead);

OMX_ERRORTYPE OMX_VIDENC_Process_FreeOutBuf(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_InitLCML(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_InitDSP_H264Enc(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_InitDSP_Mpeg4Enc(VIDENC_COMPONENT_PRIVATE* pComponentPrivate);

OMX_ERRORTYPE OMX_VIDENC_LCML_Callback(TUsnCodecEvent event, void* argsCb [10]);

OMX_ERRORTYPE OMX_VIDENC_Allocate_DSPResources (OMX_IN VIDENC_COMPONENT_PRIVATE* pComponentPrivate,  
                                                   OMX_IN OMX_U32 nPortIndex);
OMX_ERRORTYPE OMX_VIDENC_EmptyDataPipes (void* pThreadData);

OMX_ERRORTYPE OMX_VIDENC_ListCreate(struct VIDENC_NODE** pListHead);

OMX_ERRORTYPE OMX_VIDENC_ListAdd(struct VIDENC_NODE* pListHead, OMX_PTR pData);

OMX_ERRORTYPE OMX_VIDENC_ListRemove(struct VIDENC_NODE* pListHead, OMX_PTR pData);

OMX_ERRORTYPE OMX_VIDENC_ListDestroy(struct VIDENC_NODE* pListHead);

OMX_ERRORTYPE OMX_VIDENC_HandleError(VIDENC_COMPONENT_PRIVATE* pComponentPrivate, OMX_ERRORTYPE eError);
#ifdef RESOURCE_MANAGER_ENABLED
void OMX_VIDENC_ResourceManagerCallBack(RMPROXY_COMMANDDATATYPE cbData);
#endif

#endif