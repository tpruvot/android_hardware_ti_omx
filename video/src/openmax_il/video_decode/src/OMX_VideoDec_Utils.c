
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
* @file OMX_Video_Dec_Utils.c
*
* This file implements OMX Component for MPEG-4 decoder that
* is fully compliant with the Khronos OMX specification 1.0.
*
* @path  $(CSLPATH)\src
*
* @rev  0.1
*/
/* -------------------------------------------------------------------------- */
/* =============================================================================
*!
*! Revision History
*! ===================================
*!
*! 02-Feb-2006 mf: Revisions appear in reverse chronological order;
*! that is, newest first.  The date format is dd-Mon-yyyy.
* =========================================================================== */

/* ------compilation control switches ----------------------------------------*/
/*******************************************************************************
*  INCLUDE FILES
*******************************************************************************/
/* ----- system and platform files -------------------------------------------*/
#include "OMX_VideoDecoder.h"
#include "OMX_VideoDec_Utils.h"
#include "OMX_VideoDec_DSP.h"
#include "OMX_VideoDec_Thread.h"


/*-------macro definitions ---------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_Log() prints the messages to display info.
  **/
/*----------------------------------------------------------------------------*/
void VIDDEC_Log(const char *szFileName, int iLineNum, const char *szFunctionName, const char *strFormat, ...)
{
#ifndef UNDER_CE
    va_list list;

    fprintf(stdout, "%s", DBG_TEXT_WHITE);
    fprintf(stdout, "%s::", szFileName);
    fprintf(stdout, "%s", DBG_TEXT_WHITE);
    fprintf(stdout, "%s():%d: ", szFunctionName, iLineNum);
    fprintf(stdout, "%s", DBG_TEXT_WHITE);
    va_start(list, strFormat);
    vfprintf(stdout, strFormat, list);
    va_end(list);
#endif
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_eprint() prints the error messages to display info.
  **/
/*----------------------------------------------------------------------------*/
void VIDDEC_eprint(int iLineNum, const char *szFunctionName, const char *strFormat, ...)
{
#ifndef UNDER_CE
    va_list list;

    /*fprintf(stdout, "%s", DBG_TEXT_WHITE);
    fprintf(stdout, "%s::", szFileName);
    fprintf(stdout, "%s", DBG_TEXT_WHITE);*/
    fprintf(stdout, "ERROR::%s():%d: ", szFunctionName, iLineNum);
    fprintf(stdout, "%s", DBG_TEXT_WHITE);
    va_start(list, strFormat);
    vfprintf(stdout, strFormat, list);
    va_end(list);
#endif
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_GetRMFrecuency() Return the value for frecuecny to use RM.
  **/
/*----------------------------------------------------------------------------*/
OMX_U32 VIDDEC_GetRMFrecuency(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
   OMX_U32 nReturnValue = VIDDEC_MPU;

   VIDDECODER_DPRINT("+++ENTERING\n");
#ifdef RESOURCE_MANAGER_ENABLED
        /*resolution for greater than CIF*/
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > VIDDEC_CIF_WIDTH) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > VIDDEC_CIF_HEIGHT)) {
            if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                nReturnValue = VIDDEC_RM_FREC_H264_VGA;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                nReturnValue = VIDDEC_RM_FREC_WMV_VGA;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                nReturnValue = VIDDEC_RM_FREC_MPEG4_VGA;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                nReturnValue = VIDDEC_RM_FREC_MPEG2_VGA;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                nReturnValue = VIDDEC_RM_FREC_H263_VGA;
            }
#ifdef VIDDEC_SPARK_CODE
            else if (VIDDEC_SPARKCHECK) {
                nReturnValue = VIDDEC_RM_FREC_SPARK_VGA;
            }
#endif
            else {
                nReturnValue = VIDDEC_MPU;
            }
        }
        /*resolution from QCIF up to CIF*/
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= VIDDEC_CIF_WIDTH) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > VIDDEC_QCIF_WIDTH)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= VIDDEC_CIF_HEIGHT) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > VIDDEC_QCIF_HEIGHT))) {
            if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                nReturnValue = VIDDEC_RM_FREC_H264_CIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                nReturnValue = VIDDEC_RM_FREC_WMV_CIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                nReturnValue = VIDDEC_RM_FREC_MPEG4_CIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                nReturnValue = VIDDEC_RM_FREC_MPEG2_CIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                nReturnValue = VIDDEC_RM_FREC_H263_CIF;
            }
#ifdef VIDDEC_SPARK_CODE
            else if (VIDDEC_SPARKCHECK) {
                nReturnValue = VIDDEC_RM_FREC_SPARK_CIF;
            }
#endif
            else {
                nReturnValue = VIDDEC_MPU;
            }
        }
        /*resolution up to QCIF*/
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= VIDDEC_QCIF_WIDTH) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= VIDDEC_MIN_WIDTH)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= VIDDEC_QCIF_HEIGHT) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= VIDDEC_MIN_HEIGHT))) {
            if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                nReturnValue = VIDDEC_RM_FREC_H264_QCIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                nReturnValue = VIDDEC_RM_FREC_WMV_QCIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                nReturnValue = VIDDEC_RM_FREC_MPEG4_QCIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                nReturnValue = VIDDEC_RM_FREC_MPEG2_QCIF;
            }
            else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                nReturnValue = VIDDEC_RM_FREC_H263_QCIF;
            }
#ifdef VIDDEC_SPARK_CODE
            else if (VIDDEC_SPARKCHECK) {
                nReturnValue = VIDDEC_RM_FREC_SPARK_QCIF;
            }
#endif
            else {
                nReturnValue = VIDDEC_MPU;
            }
        }
        else {
            nReturnValue = VIDDEC_MPU;
    }
    VIDDECODER_DPRINT("Used RM Frec value = %d\n",(int)nReturnValue);
    VIDDECODER_DPRINT("---EXITING\n");
    return nReturnValue;
#else
    VIDDECODER_DPRINT("Used RM Frec defaulted value = %d\n",(int)nReturnValue);
    VIDDECODER_DPRINT("---EXITING\n");
    return nReturnValue;
#endif

}

OMX_ERRORTYPE VIDDEC_Queue_Init(VIDDEC_QUEUE_TYPE *queue, VIDDEC_QUEUE_TYPES type)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32 count;

    queue->nHead = 0;
    queue->nTail = 0;
    queue->nElements = 0;

    switch(type)
    {
        case VIDDEC_QUEUE_OMX_U32:
            queue->Elements = (void*)malloc(VIDDEC_MAX_QUEUE_SIZE*sizeof(OMX_U32));
            /*OMX_MALLOC_STRUCT_SIZED(queue->Elements, void, VIDDEC_MAX_QUEUE_SIZE*sizeof(OMX_U32));*/
            break;
        case VIDDEC_QUEUE_OMX_MARKTYPE:
            queue->Elements = (void*)malloc(VIDDEC_MAX_QUEUE_SIZE*sizeof(OMX_MARKTYPE));
            /*OMX_MALLOC_STRUCT_SIZED(queue->Elements, void, VIDDEC_MAX_QUEUE_SIZE*sizeof(OMX_MARKTYPE));*/
            break;
    }

    pthread_mutex_init(&(queue->mMutex), NULL);

    for(count=0; count < VIDDEC_MAX_QUEUE_SIZE; count++)
    {
        queue->CounterElements[count] = 0;
    }

    eError = OMX_ErrorNone;
/*EXIT:*/
    return eError;
}

OMX_ERRORTYPE VIDDEC_Queue_Flush(VIDDEC_QUEUE_TYPE *queue)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32 count;

    if (pthread_mutex_lock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }
    queue->nHead = 0;
    queue->nTail = 0;
    queue->nElements = 0;

    for(count=0; count < VIDDEC_MAX_QUEUE_SIZE; count++)
    {
        queue->CounterElements[count] = 0;
    }
    eError = OMX_ErrorNone;

    if (pthread_mutex_unlock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
    }

EXIT:
    return eError;
}

OMX_ERRORTYPE VIDDEC_Queue_Add(VIDDEC_QUEUE_TYPE *queue, OMX_PTR pElement, VIDDEC_QUEUE_TYPES type)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;

    if (pthread_mutex_lock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }
    if(queue->nElements == 0)
    {
        switch(type)
        {
            case VIDDEC_QUEUE_OMX_U32:
                {
                    OMX_U32 *pLocal = (OMX_U32 *)queue->Elements;
                    pLocal[queue->nHead] = *(OMX_U32 *)pElement;
                }
                break;
            case VIDDEC_QUEUE_OMX_MARKTYPE:
                {
                    OMX_MARKTYPE *pLocal = (OMX_MARKTYPE *)queue->Elements;
                    pLocal[queue->nHead] = *(OMX_MARKTYPE *)pElement;
                    /*memcpy(&pLocal[queue->nHead], pElement, sizeof(OMX_MARKTYPE));*/
                }
                break;
        }
        queue->CounterElements[queue->nHead] = 1;
        queue->nElements++;
        eError = OMX_ErrorNone;
    }
    else
    {
        switch(type)
        {
            case VIDDEC_QUEUE_OMX_U32:
                {
                    OMX_U32 *pLocal = (OMX_U32 *)queue->Elements;

                    if(pLocal[queue->nHead] == *(OMX_U32 *)pElement)
                    {
                        queue->CounterElements[queue->nHead]++;
                        eError = OMX_ErrorNone;
                    }
                    else
                    {
                        if(queue->nElements >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            eError = OMX_ErrorInsufficientResources;
                            goto UNLOCK;
                        }
                        queue->nHead++;
                        if(queue->nHead >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            queue->nHead = 0;
                        }
                        pLocal[queue->nHead] = *(OMX_U32 *)pElement;
                        queue->CounterElements[queue->nHead] = 1;
                        queue->nElements++;
                    }
                }
                break;
            case VIDDEC_QUEUE_OMX_MARKTYPE:
                {
                    OMX_MARKTYPE *pLocal = (OMX_MARKTYPE *)queue->Elements;
                    
                    if(pLocal[queue->nHead].hMarkTargetComponent == ((OMX_MARKTYPE *)pElement)->hMarkTargetComponent
                        && pLocal[queue->nHead].pMarkData == ((OMX_MARKTYPE *)pElement)->pMarkData)
                    {
                        queue->CounterElements[queue->nHead]++;
                        eError = OMX_ErrorNone;
                    }
                    else
                    {
                        if(queue->nElements >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            eError = OMX_ErrorInsufficientResources;
                            goto UNLOCK;
                        }
                        queue->nHead++;
                        if(queue->nHead >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            queue->nHead = 0;
                        }
                        pLocal[queue->nHead] = *(OMX_MARKTYPE *)pElement;
                        queue->CounterElements[queue->nHead] = 1;
                        queue->nElements++;
                    }
                }
                break;
        }

        eError = OMX_ErrorNone;
    }
UNLOCK:
    if (pthread_mutex_unlock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
    }
EXIT:
    return eError;

}

OMX_ERRORTYPE VIDDEC_Queue_Remove(VIDDEC_QUEUE_TYPE *queue, OMX_PTR pElement, VIDDEC_QUEUE_TYPES type)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;

    if (pthread_mutex_lock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }

    if(queue->nElements > 0)
    {
        if(pElement)
        {
            switch(type)
            {
                case VIDDEC_QUEUE_OMX_U32:
                {
                    OMX_U32 *pLocal = (OMX_U32 *)queue->Elements;
                    *(OMX_U32 *)pElement = pLocal[queue->nTail];
                    break;
                }
                case VIDDEC_QUEUE_OMX_MARKTYPE:
                {
                    OMX_MARKTYPE *pLocal = (OMX_MARKTYPE *)queue->Elements;
                    *(OMX_MARKTYPE *)pElement = pLocal[queue->nTail];
                    break;
                }
            }
        }
        queue->CounterElements[queue->nTail]--;
        if(queue->CounterElements[queue->nTail] == 0)
        {
            queue->nElements--;
            if(queue->nElements > 0)
            {
                queue->nTail++;
                if(queue->nTail == VIDDEC_MAX_QUEUE_SIZE)
                {
                    queue->nTail = 0;
                }
            }
        }
        eError = OMX_ErrorNone;
    }

    if (pthread_mutex_unlock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
    }
EXIT:
    return eError;
}

OMX_ERRORTYPE VIDDEC_Queue_Get_Tail(VIDDEC_QUEUE_TYPE *queue, OMX_PTR pElement, VIDDEC_QUEUE_TYPES type)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;

    if (pthread_mutex_lock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }
    if(queue->nElements > 0)
    {
        switch(type)
        {
            case VIDDEC_QUEUE_OMX_U32:
            {
                OMX_U32 *pLocal = (OMX_U32 *)queue->Elements;
                *(OMX_U32 *)pElement = pLocal[queue->nTail];
                break;
            }
            case VIDDEC_QUEUE_OMX_MARKTYPE:
            {
                OMX_MARKTYPE *pLocal = (OMX_MARKTYPE *)queue->Elements;
                *(OMX_MARKTYPE *)pElement = pLocal[queue->nTail];
                break;
            }
        }
        eError = OMX_ErrorNone;
    }
    if (pthread_mutex_unlock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
    }

EXIT:
    return eError;
}

OMX_ERRORTYPE VIDDEC_Queue_Replace_Tail(VIDDEC_QUEUE_TYPE *queue, OMX_PTR pElement, VIDDEC_QUEUE_TYPES type)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;

    if (pthread_mutex_lock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }
    if(queue->nElements > 0)
    {
        switch(type)
        {
            case VIDDEC_QUEUE_OMX_U32:
            {
                OMX_U32 *pLocal = (OMX_U32 *)queue->Elements;
                if(*(OMX_U32 *)pElement != pLocal[queue->nTail])
                {
                    if(queue->CounterElements[queue->nTail] != 1)
                    {
                        if(queue->nElements >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            eError = OMX_ErrorInsufficientResources;
                            goto UNLOCK;
                        }
                        queue->CounterElements[queue->nTail]--;
                        queue->nTail--;
                        if( (queue->nTail >= VIDDEC_MAX_QUEUE_SIZE) || (queue->nTail < 0) )
                        {
                            queue->nTail = VIDDEC_MAX_QUEUE_SIZE-1;
                        }
                        queue->CounterElements[queue->nTail] = 1;
                        queue->nElements++;
                    }
                    pLocal[queue->nTail] = *(OMX_U32 *)pElement;
                    eError = OMX_ErrorNone;
                    goto UNLOCK;
                }
                break;
            }
            case VIDDEC_QUEUE_OMX_MARKTYPE:
            {
                OMX_MARKTYPE *pLocal = (OMX_MARKTYPE *)queue->Elements;
                if(pLocal[queue->nTail].hMarkTargetComponent != ((OMX_MARKTYPE *)pElement)->hMarkTargetComponent
                    || pLocal[queue->nTail].pMarkData != ((OMX_MARKTYPE *)pElement)->pMarkData)
                {
                    if(queue->CounterElements[queue->nTail] != 1)
                    {
                        if(queue->nElements >= VIDDEC_MAX_QUEUE_SIZE)
                        {
                            eError = OMX_ErrorInsufficientResources;
                            goto UNLOCK;
                        }
                        queue->CounterElements[queue->nTail]--;
                        queue->nTail--;
                        if( (queue->nTail >= VIDDEC_MAX_QUEUE_SIZE) || (queue->nTail < 0) )
                        {
                            queue->nTail = VIDDEC_MAX_QUEUE_SIZE-1;
                        }
                        queue->CounterElements[queue->nTail] = 1;
                        queue->nElements++;
                    }
                    pLocal[queue->nTail] = *(OMX_MARKTYPE *)pElement;
                    eError = OMX_ErrorNone;
                    goto UNLOCK;
                }
                break;
            }
        }
    }
UNLOCK:
    if (pthread_mutex_unlock (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
    }
EXIT:
    return eError;
}

OMX_ERRORTYPE VIDDEC_Queue_Free(VIDDEC_QUEUE_TYPE *queue)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;

    queue->nHead = 0;
    queue->nTail = 0;
    queue->nElements = 0;

    if(queue->Elements)
    {
        free(queue->Elements);
    }
    queue->Elements = NULL;

    if (pthread_mutex_destroy (&(queue->mMutex)) != 0) {
        eError = OMX_ErrorUndefined;
        goto EXIT;
    }

    eError = OMX_ErrorNone;
EXIT:
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Init()
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_Init(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;
    OMX_U32 nCount = 0;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pTempCBuffer->m_lock != NULL) {
        free(pTempCBuffer->m_lock);
        pTempCBuffer->m_lock = NULL;
    }
    OMX_MALLOC_STRUCT(pTempCBuffer->m_lock, pthread_mutex_t,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel3]);
    /*pTempCBuffer->m_lock = malloc(sizeof(pthread_mutex_t));*/
    pthread_mutex_init(pTempCBuffer->m_lock, NULL);
#endif
    for(nCount = 0; nCount >= CBUFFER_SIZE; nCount++) {
        pTempCBuffer->pElement[nCount] = NULL;
    }
EXIT:
    pTempCBuffer->nCount = 0;
    pTempCBuffer->nHead = 0;
    pTempCBuffer->nTail = 0;

    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Flush()
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_Flush(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;
    OMX_U32 nCount = 0;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_lock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    for (nCount = pTempCBuffer->nTail; nCount <= pTempCBuffer->nHead; nCount++){
        pTempCBuffer->pElement[nCount] = NULL;
    }
    pTempCBuffer->nCount = 0;
    pTempCBuffer->nHead = 0;
    pTempCBuffer->nTail = 0;

#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_unlock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_DeInit()
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_DeInit(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;
    OMX_U32 nCount = 0;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pTempCBuffer->m_lock != NULL) {
        free(pTempCBuffer->m_lock);
        pTempCBuffer->m_lock = NULL;
    }
#endif
    for(nCount = 0; nCount >= CBUFFER_SIZE; nCount++) {
        pTempCBuffer->pElement[nCount] = NULL;
    }
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Add() set the last element in the Circular Buffer
  * return the error number in case of exist an error.
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_Add(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex, OMX_PTR pElement)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_lock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    pTempCBuffer->pElement[pTempCBuffer->nHead++] = pElement;
    pTempCBuffer->nCount++;
    if(pTempCBuffer->nHead >= CBUFFER_SIZE){
        pTempCBuffer->nHead = 0;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_unlock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Remove() get the first element in the Circular Buffer
  * return the error number in case of exist an error.
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_Remove(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex, OMX_PTR* pElement)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_lock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
	if(pTempCBuffer->nCount)
	{
	    *pElement = pTempCBuffer->pElement[pTempCBuffer->nTail];
	    pTempCBuffer->pElement[pTempCBuffer->nTail++] = NULL;
	    pTempCBuffer->nCount--;
	    if(pTempCBuffer->nTail >= CBUFFER_SIZE){
	        pTempCBuffer->nTail = 0;
	    }
	}
	else
	{
	    *pElement = NULL;
	}
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_unlock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Count() get the number of elements in the Circular Buffer
  * return the error number in case of exist an error.
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_CircBuf_Count(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex, OMX_U8* pCount)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        eError = OMX_ErrorBadParameter;
        pCount = 0;
        return eError;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_lock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif

    *pCount = pTempCBuffer->nCount;

#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_unlock(pTempCBuffer->m_lock) != 0) {
        eError = OMX_ErrorHardware;
        return eError;
    }
#endif
    return eError;
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_CircBuf_Head() get the number of elements in the Circular Buffer
  * return the error number in case of exist an error.
  **/
/*----------------------------------------------------------------------------*/
OMX_U8 VIDDEC_CircBuf_GetHead(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, VIDDEC_CBUFFER_TYPE nTypeIndex, VIDDEC_PORT_INDEX nPortIndex)
{
    /*OMX_ERRORTYPE eError = OMX_ErrorNone;*/
    VIDDEC_CIRCULAR_BUFFER *pTempCBuffer = NULL;
    OMX_U8 ucHead = 0;

    if(nTypeIndex == VIDDEC_CBUFFER_TIMESTAMP){
        pTempCBuffer = &pComponentPrivate->pCompPort[nPortIndex]->eTimeStamp;
    }
    else {
        ucHead = 0;
        return 0;
    }
#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_lock(pTempCBuffer->m_lock) != 0) {
        return 0;
    }
#endif

    ucHead = pTempCBuffer->nHead;

#ifdef VIDDEC_CBUFFER_LOCK
    if(pthread_mutex_unlock(pTempCBuffer->m_lock) != 0) {
        return 0;
    }
#endif
    return ucHead;
}

/* ========================================================================== */
/**
  *  VIDDEC_Load_Defaults() function will be called by the component to
  *
  *                         load the default values
  *
  * @param pComponentPrivate         Pointer to the pComponentPrivatePrivate
  *
  * @retval OMX_NoError              Success, ready to roll
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_Load_Defaults (VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_S32 nPassing)
{
    OMX_U32 iCount = 0;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    switch(nPassing){
        case VIDDEC_INIT_ALL:
        case VIDDEC_INIT_STRUCTS:
            pComponentPrivate->nInBufIndex  = 0;
            pComponentPrivate->nOutBufIndex = 0;

            pComponentPrivate->pCompPort[0]->hTunnelComponent = NULL;
            pComponentPrivate->pCompPort[1]->hTunnelComponent = NULL;

            /* Set component version */
            pComponentPrivate->pComponentVersion.s.nVersionMajor                = VERSION_MAJOR;
            pComponentPrivate->pComponentVersion.s.nVersionMinor                = VERSION_MINOR;
            pComponentPrivate->pComponentVersion.s.nRevision                    = VERSION_REVISION;
            pComponentPrivate->pComponentVersion.s.nStep                        = VERSION_STEP;

            /* Set spec version */
            pComponentPrivate->pSpecVersion.s.nVersionMajor                     = VERSION_MAJOR;
            pComponentPrivate->pSpecVersion.s.nVersionMinor                     = VERSION_MINOR;
            pComponentPrivate->pSpecVersion.s.nRevision                         = VERSION_REVISION;
            pComponentPrivate->pSpecVersion.s.nStep                             = VERSION_STEP;

            pComponentPrivate->iEndofInput = 0;

            pComponentPrivate->pHandle->pApplicationPrivate = NULL;
            /* Set pPortParamType defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pPortParamType, OMX_PORT_PARAM_TYPE);
            pComponentPrivate->pPortParamType->nPorts                           = NUM_OF_PORTS;
            pComponentPrivate->pPortParamType->nStartPortNumber                 = VIDDEC_INPUT_PORT;
#ifdef __STD_COMPONENT__
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pPortParamTypeAudio, OMX_PORT_PARAM_TYPE);
            pComponentPrivate->pPortParamTypeAudio->nPorts                      = VIDDEC_ZERO;
            pComponentPrivate->pPortParamTypeAudio->nStartPortNumber            = VIDDEC_ZERO;
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pPortParamTypeImage, OMX_PORT_PARAM_TYPE);
            pComponentPrivate->pPortParamTypeImage->nPorts                      = VIDDEC_ZERO;
            pComponentPrivate->pPortParamTypeImage->nStartPortNumber            = VIDDEC_ZERO;
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pPortParamTypeOthers, OMX_PORT_PARAM_TYPE);
            pComponentPrivate->pPortParamTypeOthers->nPorts                     = VIDDEC_ZERO;
            pComponentPrivate->pPortParamTypeOthers->nStartPortNumber           = VIDDEC_ZERO;
#endif

            pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->nBufferCnt         = 0;
            pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->nBufferCnt        = 0;

            /* Set pInPortDef defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pInPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
            pComponentPrivate->pInPortDef->nPortIndex                           = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortDef->eDir                                 = OMX_DirInput;
            pComponentPrivate->pInPortDef->nBufferCountActual                   = VIDDEC_BUFFERMINCOUNT; /*MAX_PRIVATE_IN_BUFFERS;*/
            pComponentPrivate->pInPortDef->nBufferCountMin                      = VIDDEC_BUFFERMINCOUNT;
            pComponentPrivate->pInPortDef->nBufferSize                          = VIDDEC_DEFAULT_INPUT_BUFFER_SIZE;
            pComponentPrivate->pInPortDef->bEnabled                             = VIDDEC_PORT_ENABLED;
            pComponentPrivate->pInPortDef->bPopulated                           = VIDDEC_PORT_POPULATED;
            pComponentPrivate->pInPortDef->eDomain                              = VIDDEC_PORT_DOMAIN;
            pComponentPrivate->pInPortDef->format.video.pNativeRender           = VIDDEC_INPUT_PORT_NATIVERENDER;
            pComponentPrivate->pInPortDef->format.video.nFrameWidth             = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight            = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nStride                 = VIDDEC_INPUT_PORT_STRIDE;
            pComponentPrivate->pInPortDef->format.video.nSliceHeight            = VIDDEC_INPUT_PORT_SLICEHEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate                = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate              = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.cMIMEType               = VIDDEC_MIMETYPEMPEG4;
            pComponentPrivate->pInPortDef->format.video.bFlagErrorConcealment   = VIDDEC_INPUT_PORT_FLAGERRORCONCEALMENT;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat      = VIDDEC_INPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pInPortDef->format.video.eColorFormat            = VIDDEC_COLORFORMATUNUSED;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortDef->format.video.pNativeWindow           = 0;
#endif

            /* Set pOutPortDef defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pOutPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
            pComponentPrivate->pOutPortDef->nPortIndex                          = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->pOutPortDef->eDir                                = OMX_DirOutput;
            pComponentPrivate->pOutPortDef->nBufferCountActual                  = VIDDEC_BUFFERMINCOUNT;/*MAX_PRIVATE_OUT_BUFFERS;*/
            pComponentPrivate->pOutPortDef->nBufferCountMin                     = VIDDEC_BUFFERMINCOUNT;
            pComponentPrivate->pOutPortDef->nBufferSize                         = VIDDEC_DEFAULT_OUTPUT_BUFFER_SIZE;
            pComponentPrivate->pOutPortDef->bEnabled                            = VIDDEC_PORT_ENABLED;
            pComponentPrivate->pOutPortDef->bPopulated                          = VIDDEC_PORT_POPULATED;
            pComponentPrivate->pOutPortDef->eDomain                             = VIDDEC_PORT_DOMAIN;
            pComponentPrivate->pOutPortDef->format.video.cMIMEType              = VIDDEC_MIMETYPEYUV;
            pComponentPrivate->pOutPortDef->format.video.pNativeRender          = VIDDEC_OUTPUT_PORT_NATIVERENDER;
            pComponentPrivate->pOutPortDef->format.video.nFrameWidth            = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pOutPortDef->format.video.nFrameHeight           = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pOutPortDef->format.video.nStride                = VIDDEC_OUTPUT_PORT_STRIDE;
            pComponentPrivate->pOutPortDef->format.video.nSliceHeight           = VIDDEC_OUTPUT_PORT_SLICEHEIGHT;
            pComponentPrivate->pOutPortDef->format.video.nBitrate               = VIDDEC_OUTPUT_PORT_BITRATE;
            pComponentPrivate->pOutPortDef->format.video.xFramerate             = VIDDEC_OUTPUT_PORT_FRAMERATE;
            pComponentPrivate->pOutPortDef->format.video.bFlagErrorConcealment  = VIDDEC_OUTPUT_PORT_FLAGERRORCONCEALMENT;
            pComponentPrivate->pOutPortDef->format.video.eCompressionFormat     = VIDDEC_OUTPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pOutPortDef->format.video.eColorFormat           = VIDDEC_COLORFORMAT420;
#ifdef KHRONOS_1_1
            pComponentPrivate->pOutPortDef->format.video.pNativeWindow           = 0;
#endif
            for (iCount = 0; iCount < MAX_PRIVATE_BUFFERS; iCount++) {
                OMX_MALLOC_STRUCT(pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[iCount], VIDDEC_BUFFER_PRIVATE,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0]);
                pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr = NULL;
            }

            for (iCount = 0; iCount < MAX_PRIVATE_BUFFERS; iCount++) {
                OMX_MALLOC_STRUCT(pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[iCount], VIDDEC_BUFFER_PRIVATE,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0]);
                pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr = NULL;
            }

            /* Set pInPortFormat defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pInPortFormat, OMX_VIDEO_PARAM_PORTFORMATTYPE);
            pComponentPrivate->pInPortFormat->nPortIndex                        = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                            = VIDDEC_DEFAULT_INPUT_INDEX_MPEG4;
            pComponentPrivate->pInPortFormat->eCompressionFormat                = VIDDEC_INPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pInPortFormat->eColorFormat                      = VIDDEC_COLORFORMATUNUSED;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                        = VIDDEC_INPUT_PORT_FRAMERATE;
#endif

            /* Set pOutPortFormat defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pOutPortFormat, OMX_VIDEO_PARAM_PORTFORMATTYPE);
            pComponentPrivate->pOutPortFormat->nPortIndex                       = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->pOutPortFormat->nIndex                           = VIDDEC_DEFAULT_OUTPUT_INDEX_PLANAR420;
            pComponentPrivate->pOutPortFormat->eCompressionFormat               = VIDDEC_OUTPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pOutPortFormat->eColorFormat                     = VIDDEC_COLORFORMAT420;
#ifdef KHRONOS_1_1
            pComponentPrivate->pOutPortFormat->xFramerate                       = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            /* Set pPriorityMgmt defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pPriorityMgmt, OMX_PRIORITYMGMTTYPE);
            pComponentPrivate->pPriorityMgmt->nGroupPriority                    = -1;
            pComponentPrivate->pPriorityMgmt->nGroupID                          = -1;

            /* Buffer supplier setting */
            pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->eSupplierSetting   = OMX_BufferSupplyOutput;

            /* Set pInBufSupplier defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pInBufSupplier, OMX_PARAM_BUFFERSUPPLIERTYPE );
            pComponentPrivate->pInBufSupplier->nPortIndex                       = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInBufSupplier->eBufferSupplier                  = VIDDEC_INPUT_PORT_BUFFERSUPPLIER;

            /* Set pOutBufSupplier defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pOutBufSupplier, OMX_PARAM_BUFFERSUPPLIERTYPE );
            pComponentPrivate->pOutBufSupplier->nPortIndex                      = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->pOutBufSupplier->eBufferSupplier                 = VIDDEC_OUTPUT_PORT_BUFFERSUPPLIER;

#ifdef KHRONOS_1_1
            /*MBError Reporting code       */
            /* Set eMBErrorReport defaults */
            OMX_CONF_INIT_STRUCT(&pComponentPrivate->eMBErrorReport, OMX_CONFIG_MBERRORREPORTINGTYPE );
            pComponentPrivate->eMBErrorReport.nPortIndex  = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->eMBErrorReport.bEnabled    = OMX_FALSE;
            /*MBError Reporting code       */
            /* Set eMBErrorMapType defaults */
            for (iCount = 0; iCount < MAX_PRIVATE_BUFFERS; iCount++) {
                OMX_CONF_INIT_STRUCT(&pComponentPrivate->eMBErrorMapType[iCount], OMX_CONFIG_MACROBLOCKERRORMAPTYPE_TI );
                pComponentPrivate->eMBErrorMapType[iCount].nPortIndex  = VIDDEC_OUTPUT_PORT;
                pComponentPrivate->eMBErrorMapType[iCount].nErrMapSize = (VIDDEC_DEFAULT_WIDTH * VIDDEC_DEFAULT_HEIGHT) / 256;
            }
            pComponentPrivate->cMBErrorIndexIn = 0;
            pComponentPrivate->cMBErrorIndexOut = 0;

#endif

            /* Set pMpeg4 defaults */
            OMX_CONF_INIT_STRUCT (pComponentPrivate->pMpeg4, OMX_VIDEO_PARAM_MPEG4TYPE);
            pComponentPrivate->pMpeg4->nPortIndex               = VIDDEC_DEFAULT_MPEG4_PORTINDEX;
            pComponentPrivate->pMpeg4->nSliceHeaderSpacing      = VIDDEC_DEFAULT_MPEG4_SLICEHEADERSPACING;
            pComponentPrivate->pMpeg4->bSVH                     = VIDDEC_DEFAULT_MPEG4_SVH;
            pComponentPrivate->pMpeg4->bGov                     = VIDDEC_DEFAULT_MPEG4_GOV;
            pComponentPrivate->pMpeg4->nPFrames                 = VIDDEC_DEFAULT_MPEG4_PFRAMES;
            pComponentPrivate->pMpeg4->nBFrames                 = VIDDEC_DEFAULT_MPEG4_BFRAMES;
            pComponentPrivate->pMpeg4->nIDCVLCThreshold         = VIDDEC_DEFAULT_MPEG4_IDCVLCTHRESHOLD;
            pComponentPrivate->pMpeg4->bACPred                  = VIDDEC_DEFAULT_MPEG4_ACPRED;
            pComponentPrivate->pMpeg4->nMaxPacketSize           = VIDDEC_DEFAULT_MPEG4_MAXPACKETSIZE;
            pComponentPrivate->pMpeg4->nTimeIncRes              = VIDDEC_DEFAULT_MPEG4_TIMEINCRES;
            pComponentPrivate->pMpeg4->eProfile                 = VIDDEC_DEFAULT_MPEG4_PROFILE;
            pComponentPrivate->pMpeg4->eLevel                   = VIDDEC_DEFAULT_MPEG4_LEVEL;
            pComponentPrivate->pMpeg4->nAllowedPictureTypes     = VIDDEC_DEFAULT_MPEG4_ALLOWEDPICTURETYPES;
            pComponentPrivate->pMpeg4->nHeaderExtension         = VIDDEC_DEFAULT_MPEG4_HEADEREXTENSION;
            pComponentPrivate->pMpeg4->bReversibleVLC           = VIDDEC_DEFAULT_MPEG4_REVERSIBLEVLC;

            /* Set pMpeg2 defaults */
            OMX_CONF_INIT_STRUCT (pComponentPrivate->pMpeg2, OMX_VIDEO_PARAM_MPEG2TYPE);
            pComponentPrivate->pMpeg2->nPortIndex               = VIDDEC_DEFAULT_MPEG2_PORTINDEX;
            pComponentPrivate->pMpeg2->nPFrames                 = VIDDEC_DEFAULT_MPEG2_PFRAMES;
            pComponentPrivate->pMpeg2->nBFrames                 = VIDDEC_DEFAULT_MPEG2_BFRAMES;
            pComponentPrivate->pMpeg2->eProfile                 = VIDDEC_DEFAULT_MPEG2_PROFILE;
            pComponentPrivate->pMpeg2->eLevel                   = VIDDEC_DEFAULT_MPEG2_LEVEL;

            /* Set pH264 defaults */
            OMX_CONF_INIT_STRUCT(pComponentPrivate->pH264, OMX_VIDEO_PARAM_AVCTYPE);
            pComponentPrivate->pH264->nPortIndex                = VIDDEC_DEFAULT_H264_PORTINDEX;
            pComponentPrivate->pH264->nSliceHeaderSpacing       = VIDDEC_DEFAULT_H264_SLICEHEADERSPACING;
            pComponentPrivate->pH264->nPFrames                  = VIDDEC_DEFAULT_H264_PFRAMES;
            pComponentPrivate->pH264->nBFrames                  = VIDDEC_DEFAULT_H264_BFRAMES;
            pComponentPrivate->pH264->bUseHadamard              = VIDDEC_DEFAULT_H264_USEHADAMARD;
            pComponentPrivate->pH264->nRefFrames                = VIDDEC_DEFAULT_H264_REFFRAMES;
            pComponentPrivate->pH264->nRefIdx10ActiveMinus1     = VIDDEC_DEFAULT_H264_REFIDX10ACTIVEMINUS1;
            pComponentPrivate->pH264->nRefIdx11ActiveMinus1     = VIDDEC_DEFAULT_H264_REFIDX11ACTIVEMINUS1;
            pComponentPrivate->pH264->bEnableUEP                = VIDDEC_DEFAULT_H264_ENABLEUEP;
            pComponentPrivate->pH264->bEnableFMO                = VIDDEC_DEFAULT_H264_ENABLEFMO;
            pComponentPrivate->pH264->bEnableASO                = VIDDEC_DEFAULT_H264_ENABLEASO;
            pComponentPrivate->pH264->bEnableRS                 = VIDDEC_DEFAULT_H264_ENABLERS;
            pComponentPrivate->pH264->eProfile                  = VIDDEC_DEFAULT_H264_PROFILE;
            pComponentPrivate->pH264->eLevel                    = VIDDEC_DEFAULT_H264_LEVEL;
            pComponentPrivate->pH264->nAllowedPictureTypes      = VIDDEC_DEFAULT_H264_ALLOWEDPICTURETYPES;
            pComponentPrivate->pH264->bFrameMBsOnly             = VIDDEC_DEFAULT_H264_FRAMEMBSONLY;
            pComponentPrivate->pH264->bMBAFF                    = VIDDEC_DEFAULT_H264_MBAFF;
            pComponentPrivate->pH264->bEntropyCodingCABAC       = VIDDEC_DEFAULT_H264_ENTROPYCODINGCABAC;
            pComponentPrivate->pH264->bWeightedPPrediction      = VIDDEC_DEFAULT_H264_WEIGHTEDPPREDICTION;
            pComponentPrivate->pH264->nWeightedBipredicitonMode = VIDDEC_DEFAULT_H264_WEIGHTEDBIPREDICITONMODE;
            pComponentPrivate->pH264->bconstIpred               = VIDDEC_DEFAULT_H264_CONSTIPRED;
            pComponentPrivate->pH264->bDirect8x8Inference       = VIDDEC_DEFAULT_H264_DIRECT8X8INFERENCE;
            pComponentPrivate->pH264->bDirectSpatialTemporal    = VIDDEC_DEFAULT_H264_DIRECTSPATIALTEMPORAL;
            pComponentPrivate->pH264->nCabacInitIdc             = VIDDEC_DEFAULT_H264_CABACINITIDC;
            pComponentPrivate->pH264->eLoopFilterMode           = VIDDEC_DEFAULT_H264_LOOPFILTERMODE;

            pComponentPrivate->pH263->nPortIndex                = VIDDEC_DEFAULT_H263_PORTINDEX;
            pComponentPrivate->pH263->nPFrames                  = VIDDEC_DEFAULT_H263_PFRAMES;
            pComponentPrivate->pH263->nBFrames                  = VIDDEC_DEFAULT_H263_BFRAMES;
            pComponentPrivate->pH263->eProfile                  = VIDDEC_DEFAULT_H263_PROFILE;
            pComponentPrivate->pH263->eLevel                    = VIDDEC_DEFAULT_H263_LEVEL;
            pComponentPrivate->pH263->bPLUSPTYPEAllowed         = VIDDEC_DEFAULT_H263_PLUSPTYPEALLOWED;
            pComponentPrivate->pH263->nAllowedPictureTypes      = VIDDEC_DEFAULT_H263_ALLOWEDPICTURETYPES;
            pComponentPrivate->pH263->bForceRoundingTypeToZero  = VIDDEC_DEFAULT_H263_FORCEROUNDINGTYPETOZERO;
            pComponentPrivate->pH263->nPictureHeaderRepetition  = VIDDEC_DEFAULT_H263_PICTUREHEADERREPETITION;
            pComponentPrivate->pH263->nGOBHeaderInterval        = VIDDEC_DEFAULT_H263_GOBHEADERINTERVAL;

            OMX_CONF_INIT_STRUCT(pComponentPrivate->pWMV, OMX_VIDEO_PARAM_WMVTYPE);
            pComponentPrivate->pWMV->nPortIndex                 = VIDDEC_DEFAULT_WMV_PORTINDEX;
            pComponentPrivate->pWMV->eFormat                    = VIDDEC_DEFAULT_WMV_FORMAT;

            pComponentPrivate->ProcessMode                      = 0;
#ifdef VIDDEC_FLAGGED_EOS
            pComponentPrivate->bUseFlaggedEos                   = OMX_TRUE;
#endif
            pComponentPrivate->wmvProfile                       = VIDDEC_WMV_PROFILEMAX;
#ifdef UNDER_CE
            pComponentPrivate->bIsNALBigEndian                   = OMX_TRUE;
#else
            pComponentPrivate->bIsNALBigEndian                   = OMX_FALSE;
#endif
            pComponentPrivate->eLCMLState                       = VidDec_LCML_State_Unload;
            pComponentPrivate->bLCMLHalted                      = OMX_TRUE;
#ifndef UNDER_CE
            pComponentPrivate->bLCMLOut                         = OMX_FALSE;
#endif
            pComponentPrivate->eRMProxyState                    = VidDec_RMPROXY_State_Unload;
            pComponentPrivate->nWMVFileType                     = 0;
            pComponentPrivate->bParserEnabled                   = OMX_TRUE;
            pComponentPrivate->H264BitStreamFormat              = 0;

            VIDDEC_CircBuf_Init(pComponentPrivate, VIDDEC_CBUFFER_TIMESTAMP, VIDDEC_INPUT_PORT);
#ifndef UNDER_CE
            VIDDEC_PTHREAD_MUTEX_INIT(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_SEMAPHORE_INIT(pComponentPrivate->sInSemaphore);
            VIDDEC_PTHREAD_SEMAPHORE_INIT(pComponentPrivate->sOutSemaphore);
#endif
            VIDDEC_Queue_Init(&pComponentPrivate->qBuffMark, VIDDEC_QUEUE_OMX_MARKTYPE);
            VIDDEC_Queue_Init(&pComponentPrivate->qCmdMarkData, VIDDEC_QUEUE_OMX_MARKTYPE);
            VIDDEC_Queue_Init(&pComponentPrivate->qBytesSent, VIDDEC_QUEUE_OMX_U32);
            for (iCount = 0; iCount < CBUFFER_SIZE; iCount++) {
                pComponentPrivate->aBufferFlags[iCount].nTimeStamp = 0;
                pComponentPrivate->aBufferFlags[iCount].nFlags = 0;
                pComponentPrivate->aBufferFlags[iCount].pMarkData = NULL;
                pComponentPrivate->aBufferFlags[iCount].hMarkTargetComponent = NULL;
            }
            pComponentPrivate->pBufferRCV.sStructRCV.nNumFrames = 0xFFFFFF; /*Infinite frame number*/
            pComponentPrivate->pBufferRCV.sStructRCV.nFrameType = 0x85; /*85*/
            pComponentPrivate->pBufferRCV.sStructRCV.nID = 0x04; /*WMV3*/
            pComponentPrivate->pBufferRCV.sStructRCV.nStructData = 0x018a3106; /*0x06318a01zero fill 0x018a3106*/
            pComponentPrivate->pBufferRCV.sStructRCV.nVertSize = 352; /*720*/
            pComponentPrivate->pBufferRCV.sStructRCV.nHorizSize = 288; /*576*/
            OMX_CONF_INIT_STRUCT( &pComponentPrivate->pBufferTemp, OMX_BUFFERHEADERTYPE);
            pComponentPrivate->pBufferTemp.nFilledLen = sizeof(VIDDEC_WMV_RCV_header);
            pComponentPrivate->pBufferTemp.nAllocLen = sizeof(VIDDEC_WMV_RCV_header);

        case VIDDEC_INIT_VARS:
            /* Set the process mode to zero, frame = 0, stream = 1 */
            VIDDEC_CircBuf_Flush(pComponentPrivate, VIDDEC_CBUFFER_TIMESTAMP, VIDDEC_INPUT_PORT);
            VIDDEC_Queue_Flush(&(pComponentPrivate->qBuffMark));
            VIDDEC_Queue_Flush(&(pComponentPrivate->qCmdMarkData));
            VIDDEC_Queue_Flush(&(pComponentPrivate->qBytesSent));
            pComponentPrivate->bIsPaused                        = 0;
            pComponentPrivate->bIsStopping                      = 0;
            pComponentPrivate->bFirstBuffer                     = 1;
            pComponentPrivate->eIdleToLoad                      = OMX_StateInvalid;
            pComponentPrivate->iEndofInputSent                  = 0;
            pComponentPrivate->nInputBCountDsp                  = 0;
            pComponentPrivate->nOutputBCountDsp                 = 0;
            pComponentPrivate->nInputBCountApp                  = 0;
            pComponentPrivate->nOutputBCountApp                 = 0;
            pComponentPrivate->bPlayCompleted                   = OMX_FALSE;
            pComponentPrivate->frameCounter                     = 0;
            pComponentPrivate->bMult16Size                      = OMX_FALSE;
            pComponentPrivate->bBuffFound                       = OMX_FALSE;
            pComponentPrivate->bFlushOut                        = OMX_FALSE;
            pComponentPrivate->nBytesConsumed                   = 0;
            pComponentPrivate->bBuffMarkTaked                   = OMX_FALSE;
            pComponentPrivate->bBuffalreadyMarked               = OMX_FALSE;
            pComponentPrivate->bFirstHeader                     = OMX_FALSE;
            pComponentPrivate->nCurrentMPEG4ProfileIndex        = 0;
            pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]  = 0;
            pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel2]  = 0;
            pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel3]  = 0;
            pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]  = 0;
#ifdef KHRONOS_1_1
            pComponentPrivate->eMBErrorReport.bEnabled            = OMX_FALSE;
#endif
        break;

case VIDDEC_INIT_IDLEEXECUTING:
            /* Set the process mode to zero, frame = 0, stream = 1 */
            pComponentPrivate->bIsPaused                        = 0;
            pComponentPrivate->bIsStopping                      = 0;
            pComponentPrivate->bFirstBuffer                     = 1;
            pComponentPrivate->iEndofInputSent                  = 0;
            pComponentPrivate->nInputBCountDsp                  = 0;
            pComponentPrivate->nOutputBCountDsp                 = 0;
            pComponentPrivate->nInputBCountApp                  = 0;
            pComponentPrivate->nOutputBCountApp                 = 0;
            pComponentPrivate->bPlayCompleted                   = OMX_FALSE;
            pComponentPrivate->frameCounter                     = 0;
            pComponentPrivate->bMult16Size                      = OMX_FALSE;
            pComponentPrivate->bBuffFound                       = OMX_FALSE;
            pComponentPrivate->bFlushOut                        = OMX_FALSE;
            pComponentPrivate->bFirstHeader                     = OMX_FALSE;
			pComponentPrivate->eFirstBuffer.bSaveFirstBuffer    = OMX_FALSE;
			pComponentPrivate->eFirstBuffer.pFirstBufferSaved   = NULL;
			pComponentPrivate->eFirstBuffer.nFilledLen          = 0;
        break;

        case VIDDEC_INIT_H263:
            pComponentPrivate->pH263->nPortIndex                            = VIDDEC_DEFAULT_H263_PORTINDEX;
            pComponentPrivate->pH263->nPFrames                              = VIDDEC_DEFAULT_H263_PFRAMES;
            pComponentPrivate->pH263->nBFrames                              = VIDDEC_DEFAULT_H263_BFRAMES;
            pComponentPrivate->pH263->eProfile                              = OMX_VIDEO_H263ProfileBaseline;
            pComponentPrivate->pH263->eLevel                                = OMX_VIDEO_H263Level10;
            pComponentPrivate->pH263->bPLUSPTYPEAllowed                     = VIDDEC_DEFAULT_H263_PLUSPTYPEALLOWED;
            pComponentPrivate->pH263->nAllowedPictureTypes                  = VIDDEC_DEFAULT_H263_ALLOWEDPICTURETYPES;
            pComponentPrivate->pH263->bForceRoundingTypeToZero              = VIDDEC_DEFAULT_H263_FORCEROUNDINGTYPETOZERO;
            pComponentPrivate->pH263->nPictureHeaderRepetition              = VIDDEC_DEFAULT_H263_PICTUREHEADERREPETITION;
            pComponentPrivate->pH263->nGOBHeaderInterval                    = VIDDEC_DEFAULT_H263_GOBHEADERINTERVAL;

            pComponentPrivate->pInPortFormat->nPortIndex                     = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_H263;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingH263;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = OMX_VIDEO_CodingH263;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = OMX_COLOR_FormatUnused;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;
            pComponentPrivate->bIsSparkInput = OMX_FALSE;
            break;
#ifdef VIDDEC_SPARK_CODE
        case VIDDEC_INIT_SPARK:
            pComponentPrivate->pInPortFormat->nPortIndex                     = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_H263;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingUnused;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = pComponentPrivate->pInPortFormat->eCompressionFormat;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = pComponentPrivate->pInPortFormat->eColorFormat;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;
            pComponentPrivate->bIsSparkInput = OMX_TRUE;
            break;
#endif

        case VIDDEC_INIT_H264:
            pComponentPrivate->pH264->nPortIndex                            = VIDDEC_DEFAULT_H264_PORTINDEX;
            pComponentPrivate->pH264->nPFrames                              = VIDDEC_DEFAULT_H264_PFRAMES;
            pComponentPrivate->pH264->nBFrames                              = VIDDEC_DEFAULT_H264_BFRAMES;
            pComponentPrivate->pH264->eProfile                              = OMX_VIDEO_AVCProfileBaseline;
            pComponentPrivate->pH264->eLevel                                = OMX_VIDEO_AVCLevel1;

            pComponentPrivate->pInPortFormat->nPortIndex                    = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_H264;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingAVC;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = OMX_VIDEO_CodingAVC;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = OMX_COLOR_FormatUnused;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;
            pComponentPrivate->bIsSparkInput                                = OMX_FALSE;
            break;

        case VIDDEC_INIT_MPEG2:
            pComponentPrivate->pMpeg2->nPortIndex                           = VIDDEC_DEFAULT_MPEG2_PORTINDEX;
            pComponentPrivate->pMpeg2->nPFrames                             = VIDDEC_DEFAULT_MPEG2_PFRAMES;
            pComponentPrivate->pMpeg2->nBFrames                             = VIDDEC_DEFAULT_MPEG2_BFRAMES;
            pComponentPrivate->pMpeg2->eProfile                             = OMX_VIDEO_MPEG2ProfileSimple;
            pComponentPrivate->pMpeg2->eLevel                               = OMX_VIDEO_MPEG2LevelLL;

            pComponentPrivate->pInPortFormat->nPortIndex                    = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_MPEG2;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingMPEG2;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = OMX_VIDEO_CodingMPEG2;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = OMX_COLOR_FormatUnused;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;
            pComponentPrivate->bIsSparkInput                                = OMX_FALSE;
            break;

        case VIDDEC_INIT_MPEG4:
            pComponentPrivate->pMpeg4->nPortIndex                           = VIDDEC_DEFAULT_MPEG4_PORTINDEX;
            pComponentPrivate->pMpeg4->nPFrames                             = VIDDEC_DEFAULT_MPEG4_PFRAMES;
            pComponentPrivate->pMpeg4->nBFrames                             = VIDDEC_DEFAULT_MPEG4_BFRAMES;
            pComponentPrivate->pMpeg4->eProfile                             = OMX_VIDEO_MPEG4ProfileSimple;
#ifdef KHRONOS_1_1
            pComponentPrivate->pMpeg4->eLevel                               = OMX_VIDEO_MPEG4Level1;
#else
            pComponentPrivate->pMpeg4->eLevel                               = OMX_VIDEO_MPEG4Levell;
#endif
            pComponentPrivate->pInPortFormat->nPortIndex                    = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_MPEG4;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingMPEG4;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = OMX_VIDEO_CodingMPEG4;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = OMX_COLOR_FormatUnused;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;
            pComponentPrivate->bIsSparkInput                                = OMX_FALSE;
            break;

        case VIDDEC_INIT_WMV9:
            pComponentPrivate->pWMV->nPortIndex                             = VIDDEC_DEFAULT_WMV_PORTINDEX;
            pComponentPrivate->pWMV->eFormat                                = OMX_VIDEO_WMVFormat9;

            pComponentPrivate->pInPortFormat->nPortIndex                    = VIDDEC_INPUT_PORT;
            pComponentPrivate->pInPortFormat->nIndex                        = VIDDEC_DEFAULT_INPUT_INDEX_WMV9;
            pComponentPrivate->pInPortFormat->eCompressionFormat            = OMX_VIDEO_CodingWMV;
            pComponentPrivate->pInPortFormat->eColorFormat                  = OMX_COLOR_FormatUnused;
#ifdef KHRONOS_1_1
            pComponentPrivate->pInPortFormat->xFramerate                    = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pInPortDef->format.video.nFrameWidth         = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight        = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pInPortDef->format.video.nBitrate            = VIDDEC_INPUT_PORT_BITRATE;
            pComponentPrivate->pInPortDef->format.video.xFramerate          = VIDDEC_INPUT_PORT_FRAMERATE;
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat  = OMX_VIDEO_CodingWMV;
            pComponentPrivate->pInPortDef->format.video.eColorFormat        = OMX_COLOR_FormatUnused;
            pComponentPrivate->pInPortDef->nBufferSize                      = pComponentPrivate->pInPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pInPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;

            pComponentPrivate->nWMVFileType                                 = 0;
            pComponentPrivate->bIsSparkInput                                = OMX_FALSE;
            break;

        case VIDDEC_INIT_PLANAR420:

            pComponentPrivate->pOutPortFormat->nPortIndex                   = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->pOutPortFormat->nIndex                       = VIDDEC_DEFAULT_OUTPUT_INDEX_PLANAR420;
            pComponentPrivate->pOutPortFormat->eCompressionFormat           = OMX_VIDEO_CodingUnused;
            pComponentPrivate->pOutPortFormat->eColorFormat                 = VIDDEC_COLORFORMAT420;
#ifdef KHRONOS_1_1
            pComponentPrivate->pOutPortFormat->xFramerate                   = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pOutPortDef->format.video.nFrameWidth        = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pOutPortDef->format.video.nFrameHeight       = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pOutPortDef->format.video.nBitrate           = VIDDEC_OUTPUT_PORT_BITRATE;
            pComponentPrivate->pOutPortDef->format.video.xFramerate         = VIDDEC_OUTPUT_PORT_FRAMERATE;
            pComponentPrivate->pOutPortDef->format.video.eCompressionFormat = VIDDEC_OUTPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pOutPortDef->format.video.eColorFormat       = VIDDEC_COLORFORMAT420;
            pComponentPrivate->pOutPortDef->nBufferSize                     = pComponentPrivate->pOutPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pOutPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT420;

            break;

        case VIDDEC_INIT_INTERLEAVED422:
            pComponentPrivate->pOutPortFormat->nPortIndex                   = VIDDEC_OUTPUT_PORT;
            pComponentPrivate->pOutPortFormat->nIndex                       = VIDDEC_DEFAULT_OUTPUT_INDEX_INTERLEAVED422;
            pComponentPrivate->pOutPortFormat->eCompressionFormat           = OMX_VIDEO_CodingUnused;
            pComponentPrivate->pOutPortFormat->eColorFormat                 = VIDDEC_COLORFORMAT422;
#ifdef KHRONOS_1_1
            pComponentPrivate->pOutPortFormat->xFramerate                   = VIDDEC_INPUT_PORT_FRAMERATE;
#endif
            pComponentPrivate->pOutPortDef->format.video.nFrameWidth        = VIDDEC_DEFAULT_WIDTH;
            pComponentPrivate->pOutPortDef->format.video.nFrameHeight       = VIDDEC_DEFAULT_HEIGHT;
            pComponentPrivate->pOutPortDef->format.video.nBitrate           = VIDDEC_OUTPUT_PORT_BITRATE;
            pComponentPrivate->pOutPortDef->format.video.xFramerate         = VIDDEC_OUTPUT_PORT_FRAMERATE;
            pComponentPrivate->pOutPortDef->format.video.eCompressionFormat = VIDDEC_OUTPUT_PORT_COMPRESSIONFORMAT;
            pComponentPrivate->pOutPortDef->format.video.eColorFormat       = VIDDEC_COLORFORMAT422;
            pComponentPrivate->pOutPortDef->nBufferSize                     = pComponentPrivate->pOutPortDef->format.video.nFrameWidth *
                                                                              pComponentPrivate->pOutPortDef->format.video.nFrameHeight *
                                                                              VIDDEC_FACTORFORMAT422;

            break;

    }

EXIT:
    return(eError);
}

/*----------------------------------------------------------------------------*/
/**
  * VIDDEC_Start_ComponentThread() starts the component thread and all the pipes
  * to achieve communication between dsp and application for commands and buffer
  * interchanging
  **/
/*----------------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_Start_ComponentThread(OMX_HANDLETYPE hComponent)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pComp = (OMX_COMPONENTTYPE*)hComponent;
    VIDDEC_COMPONENT_PRIVATE* pComponentPrivate = (VIDDEC_COMPONENT_PRIVATE*)pComp->pComponentPrivate;

    pComponentPrivate->bIsStopping =    0;

    VIDDECODER_DPRINT("+++ENTERING\n");
    /* create the pipe used to maintain free input buffers*/
    eError = pipe(pComponentPrivate->free_inpBuf_Q);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* create the pipe used to maintain free input buffers*/
    eError = pipe(pComponentPrivate->free_outBuf_Q);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* create the pipe used to maintain input buffers*/
    eError = pipe(pComponentPrivate->filled_inpBuf_Q);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* create the pipe used to maintain dsp output/encoded buffers*/
    eError = pipe(pComponentPrivate->filled_outBuf_Q);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* create the pipe used to send commands to the thread */
    eError = pipe(pComponentPrivate->cmdPipe);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* create the pipe used to send commands to the thread */
    eError = pipe(pComponentPrivate->cmdDataPipe);
    if (eError) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    /* Create the Component Thread */
    eError = pthread_create(&(pComponentPrivate->ComponentThread),
                            NULL,
                            OMX_VidDec_Thread,
                            pComponentPrivate);

    VIDDECODER_DPRINT("pthread_create 0x%x\n",pComponentPrivate->ComponentThread);
    if (eError || !pComponentPrivate->ComponentThread) {
        VIDDECODER_EPRINT("pthread_create  0x%x\n",eError);
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

#ifdef __PERF_INSTRUMENTATION__
    PERF_ThreadCreated(pComponentPrivate->pPERF,
                       pComponentPrivate->ComponentThread,
                       PERF_FOURS("VD T"));
#endif

EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
* @Stop_ComponentThread() This function is called by the component during
* de-init to close component thread, Command pipe & data pipes.
*
* @param pComponent  handle for this instance of the component
*
* @pre
*
* @post
*
* @return none
*/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_Stop_ComponentThread(OMX_HANDLETYPE pComponent)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*)pComponent;
    VIDDEC_COMPONENT_PRIVATE* pComponentPrivate = (VIDDEC_COMPONENT_PRIVATE*)pHandle->pComponentPrivate;
    OMX_ERRORTYPE threadError = OMX_ErrorNone;
    OMX_ERRORTYPE err = OMX_ErrorNone;
    int pthreadError = 0;

    /* Join the component thread */
    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pthread_join 0x%x\n",pComponentPrivate->ComponentThread);

#ifdef UNDER_CE
    pthreadError = pthread_join(pComponentPrivate->ComponentThread, (void*)&threadError);
    if (0 != pthreadError) {
        eError = OMX_ErrorHardware;
    }
#else
    if(pComponentPrivate->bLCMLOut == OMX_TRUE) {
        /*pthreadError = pthread_cancel(pComponentPrivate->ComponentThread);*/
        if (0 != pthreadError) {
            eError = OMX_ErrorHardware;
        }
    }
    else{
        pthreadError = pthread_join(pComponentPrivate->ComponentThread, (void*)&threadError);
        if (0 != pthreadError) {
            eError = OMX_ErrorHardware;
        }
    }
#endif

    /* Check for the errors */
    if (OMX_ErrorNone != eError) {
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorSevere,
                                               "Error while closing Component Thread\n");
    }

    /* close the data pipe handles */
    err = close(pComponentPrivate->free_inpBuf_Q[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->free_outBuf_Q[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->filled_inpBuf_Q[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->filled_outBuf_Q[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->free_inpBuf_Q[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->free_outBuf_Q[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->filled_inpBuf_Q[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    err = close(pComponentPrivate->filled_outBuf_Q[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing data pipe\n");
    }

    /* Close the command pipe handles */
    err = close(pComponentPrivate->cmdPipe[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing cmd pipe\n");
    }

    err = close(pComponentPrivate->cmdPipe[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing cmd pipe\n");
    }

    /* Close the command data pipe handles */
    err = close (pComponentPrivate->cmdDataPipe[VIDDEC_PIPE_READ]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing cmd pipe\n");
    }

    err = close (pComponentPrivate->cmdDataPipe[VIDDEC_PIPE_WRITE]);
    if (0 != err) {
        eError = OMX_ErrorHardware;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               eError,
                                               OMX_TI_ErrorMajor,
                                               "Error while closing cmd pipe\n");
    }
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  * Disable Port()
  *
  * Called by component thread, handles commands sent by the app.
  *
  * @param
  *
  * @retval OMX_ErrorNone                  success, ready to roll
  *
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_DisablePort (VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x nParam1 0x%x\n",pComponentPrivate, nParam1);

#ifdef UNDER_CE
    while(1) {
        if (nParam1 == VIDDEC_INPUT_PORT && !pComponentPrivate->pInPortDef->bPopulated) {
            /* return cmdcomplete event if input unpopulated */
            VIDDECODER_DPRINT("Unpopulated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_OUTPUT_PORT && !pComponentPrivate->pOutPortDef->bPopulated) {
            /* return cmdcomplete event if output unpopulated */
            VIDDECODER_DPRINT("Unpopulated VIDDEC_OUTPUT_PORT OUT 0x%x\n",pComponentPrivate->pOutPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_OUTPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_BOTH_PORT && !pComponentPrivate->pInPortDef->bPopulated &&
                                  !pComponentPrivate->pOutPortDef->bPopulated) {
            /* return cmdcomplete event if inout & output unpopulated */
            VIDDECODER_DPRINT("Unpopulated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            VIDDECODER_DPRINT("Unpopulated VIDDEC_OUTPUT_PORT OUT 0x%x\n",pComponentPrivate->pOutPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_OUTPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_BOTH_PORT && !pComponentPrivate->pInPortDef->bPopulated &&
                                  (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL)) {
            /* return cmdcomplete event if inout & output unpopulated */
            VIDDECODER_DPRINT("Unpopulated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            break;
        }
        VIDDEC_WAIT_CODE();
    }
#else
    if (nParam1 == VIDDEC_INPUT_PORT) {
        if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pInPortDef->bPopulated) ||
            pComponentPrivate->sInSemaphore.bSignaled) {
            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
        }
        VIDDECODER_DPRINT("Unpopulated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
        pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandPortDisable,
                                                VIDDEC_INPUT_PORT,
                                                NULL);
    }
    else if (nParam1 == VIDDEC_OUTPUT_PORT) {
        if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pOutPortDef->bPopulated) ||
                pComponentPrivate->sOutSemaphore.bSignaled) {
            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
        }

	eError = VIDDEC_UnloadCodec(pComponentPrivate);
	if (eError != OMX_ErrorNone) {
		goto EXIT;
	}

        VIDDECODER_DPRINT("Unpopulated VIDDEC_OUTPUT_PORT OUT 0x%x\n",pComponentPrivate->pOutPortDef->bPopulated);
        pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandPortDisable,
                                                VIDDEC_OUTPUT_PORT,
                                                NULL);
    }
    else if (nParam1 == VIDDEC_BOTH_PORT) {
        if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pInPortDef->bPopulated) ||
                pComponentPrivate->sInSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
            }
            VIDDECODER_DPRINT("Unpopulated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
        }
        else {
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pInPortDef->bPopulated) ||
                pComponentPrivate->sInSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
            }
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pOutPortDef->bPopulated) ||
                pComponentPrivate->sOutSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
            }
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortDisable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
        }
    }
#endif
    goto EXIT;
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  * Return Buffers()
  *
  * Called by Disable and Enable Buffers, return the buffers to their respective source.
  *
  * @param
  *
  * @retval OMX_ErrorNone                  success, ready to roll
  *
  **/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_ReturnBuffers (VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1, OMX_BOOL bRetDSP)
{
    OMX_U8 i = 0;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE *pBuffHead;

    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x nParam1 0x%x bRetDSP 0x%x\n",pComponentPrivate,nParam1,bRetDSP);
    OMX_VidDec_Return(pComponentPrivate);
    OMX_VidDec_Return(pComponentPrivate);
       if (nParam1 == pComponentPrivate->pInPortFormat->nPortIndex || nParam1 == VIDDEC_BOTH_PORT) {
            for (i = 0; i < pComponentPrivate->pInPortDef->nBufferCountActual; i++) {
                    if((pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->eBufferOwner == VIDDEC_BUFFER_WITH_DSP) && bRetDSP){
                        VIDDEC_BUFFERPRINT("inBuffer 0x%x eBufferOwner 0x%x\n",pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->pBufferHdr,
                            pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->eBufferOwner);
                        pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                        pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen = 0;

#ifdef __PERF_INSTRUMENTATION__
                        PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                          pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->pBufferHdr->pBuffer,
                                          pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen,
                                          PERF_ModuleHLMM);
#endif

                        eError = pComponentPrivate->cbInfo.EmptyBufferDone((OMX_HANDLETYPE *)pComponentPrivate->pHandle,
                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                    (OMX_BUFFERHEADERTYPE*)pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[i]->pBufferHdr);
                    }
            }
       }
        if (nParam1 == pComponentPrivate->pOutPortFormat->nPortIndex || nParam1 == VIDDEC_BOTH_PORT) {
            if (pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->hTunnelComponent != NULL) {
                for (i = 0; i < pComponentPrivate->pOutPortDef->nBufferCountActual; i++) {
                   VIDDEC_BUFFERPRINT("tunnelVideoDecBuffer[%x]=%x-%x\n",i,
                   pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner,pComponentPrivate->pOutPortDef->nBufferCountActual);
                        VIDDEC_BUFFERPRINT("enter return %x\n",pComponentPrivate->pOutPortDef->nBufferCountActual);
                        if((pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner == VIDDEC_BUFFER_WITH_DSP) && bRetDSP){
                               pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner = VIDDEC_BUFFER_WITH_TUNNELEDCOMP;
                                VIDDEC_BUFFERPRINT("Buffer 0x%x eBufferOwner 0x%x\n",(int)pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr
                                ,pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner);
                            pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen = 0;

#ifdef __PERF_INSTRUMENTATION__
                            PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                              pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->pBuffer,
                                              pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen,
                                              PERF_ModuleLLMM);
#endif

                            VIDDECODER_DPRINT("VideDec->PostProc EmptyThisBuffer\n");
                            VIDDECODER_DPRINT("wait to return buffer\n");
                            pBuffHead = (OMX_BUFFERHEADERTYPE*)pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr;
                            VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                            eError = OMX_EmptyThisBuffer(pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->hTunnelComponent, pBuffHead);
                        }
                }
            }
            else {
                VIDDECODER_DPRINT("non tunneling\n");
                for (i = 0; i < pComponentPrivate->pOutPortDef->nBufferCountActual; i++) {
                        if((pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner == VIDDEC_BUFFER_WITH_DSP) && bRetDSP){
                            VIDDEC_BUFFERPRINT("xBuffer 0x%x eBufferOwner 0x%x\n",pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr,
                                pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner);
                            pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                            pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen = 0;

#ifdef __PERF_INSTRUMENTATION__
                            PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                              pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->pBuffer,
                                              pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr->nFilledLen,
                                              PERF_ModuleHLMM);
#endif

                            pBuffHead = (OMX_BUFFERHEADERTYPE*)pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr;
                            VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                            eError = pComponentPrivate->cbInfo.FillBufferDone((OMX_HANDLETYPE *)pComponentPrivate->pHandle,
                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                        (OMX_BUFFERHEADERTYPE*)pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[i]->pBufferHdr);
                       }
                }
           }
        }
    VIDDEC_BUFFERPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}


/* ========================================================================== */
/**
  * Enable Port()
  *
  * Called by component thread, handles commands sent by the app.
  *
  * @param
  *
  * @retval OMX_ErrorNone                  success, ready to roll
  *
  **/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_EnablePort (VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_U32 nParam1)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x nParam1 0x%x\n",pComponentPrivate, nParam1);

#ifdef UNDER_CE
    while(1) {
        if (nParam1 == VIDDEC_INPUT_PORT && (pComponentPrivate->eState == OMX_StateLoaded || pComponentPrivate->pInPortDef->bPopulated)) {
            /* return cmdcomplete event if input unpopulated */
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT 0x%x\n",pComponentPrivate->pInPortDef->bEnabled);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_OUTPUT_PORT && (pComponentPrivate->eState == OMX_StateLoaded ||
                                    pComponentPrivate->pOutPortDef->bPopulated)) {
            /* return cmdcomplete event if output unpopulated */
            VIDDECODER_DPRINT("Populated VIDDEC_OUTPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_OUTPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_BOTH_PORT && (pComponentPrivate->eState == OMX_StateLoaded ||
                                    (pComponentPrivate->pInPortDef->bPopulated &&
                                    pComponentPrivate->pOutPortDef->bPopulated))) {
            /* return cmdcomplete event if inout & output unpopulated */
            VIDDECODER_DPRINT("Enabling VIDDEC_OUTPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            VIDDECODER_DPRINT("Populated VIDDEC_OUTPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_OUTPUT_PORT,
                                                    NULL);
            break;
        }
        else if (nParam1 == VIDDEC_BOTH_PORT && && (pComponentPrivate->eState == OMX_StateLoaded ||
                                    pComponentPrivate->pInPortDef->bPopulated) &&
                                  (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL)) {
            /* return cmdcomplete event if inout & output unpopulated */
            VIDDECODER_DPRINT("Enabling VIDDEC_OUTPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT 0x%x\n",pComponentPrivate->pOutPortDef->bEnabled);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            break;
        }
        VIDDEC_WAIT_CODE();
    }
#else
    if (nParam1 == VIDDEC_INPUT_PORT) {
        VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
        if((!(pComponentPrivate->eState == OMX_StateLoaded) && !pComponentPrivate->pInPortDef->bPopulated) ||
            pComponentPrivate->sInSemaphore.bSignaled) {
            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
        }

		if(pComponentPrivate->eLCMLState == VidDec_LCML_State_Unload){
			eError = VIDDEC_LoadCodec(pComponentPrivate);
			if(eError != OMX_ErrorNone){
				goto EXIT;
			}
		}
        pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandPortEnable,
                                                VIDDEC_INPUT_PORT,
                                                NULL);
    }
    else if (nParam1 == VIDDEC_OUTPUT_PORT) {
        VIDDECODER_DPRINT("Populated VIDDEC_OUTPUT_PORT OUT 0x%x\n",pComponentPrivate->pOutPortDef->bPopulated);
        if((!(pComponentPrivate->eState == OMX_StateLoaded) && !pComponentPrivate->pOutPortDef->bPopulated) ||
            pComponentPrivate->sOutSemaphore.bSignaled) {
            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
        }

		if(pComponentPrivate->eLCMLState == VidDec_LCML_State_Unload){
			eError = VIDDEC_LoadCodec(pComponentPrivate);
			if(eError != OMX_ErrorNone){
				goto EXIT;
			}
		}
        pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandPortEnable,
                                                VIDDEC_OUTPUT_PORT,
                                                NULL);
    }
    else if (nParam1 == VIDDEC_BOTH_PORT) {
        if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && !pComponentPrivate->pInPortDef->bPopulated) ||
                pComponentPrivate->sInSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
            }

			if(pComponentPrivate->eLCMLState == VidDec_LCML_State_Unload){
				eError = VIDDEC_LoadCodec(pComponentPrivate);
				if(eError != OMX_ErrorNone){
					goto EXIT;
				}
			}
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
        }
        else {
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && !pComponentPrivate->pInPortDef->bPopulated) ||
                pComponentPrivate->sInSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
            }
			if(pComponentPrivate->eLCMLState == VidDec_LCML_State_Unload){
				eError = VIDDEC_LoadCodec(pComponentPrivate);
				if(eError != OMX_ErrorNone){
					goto EXIT;
				}
            }
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_INPUT_PORT,
                                                    NULL);
            if((!(pComponentPrivate->eState == OMX_StateLoaded) && !pComponentPrivate->pOutPortDef->bPopulated) ||
                pComponentPrivate->sOutSemaphore.bSignaled) {
                VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
            }
            VIDDECODER_DPRINT("Populated VIDDEC_INPUT_PORT IN 0x%x\n",pComponentPrivate->pInPortDef->bPopulated);
            pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                    pComponentPrivate->pHandle->pApplicationPrivate,
                                                    OMX_EventCmdComplete,
                                                    OMX_CommandPortEnable,
                                                    VIDDEC_OUTPUT_PORT,
                                                    NULL);
        }
    }
#endif
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  * HandleCommandFlush()
  *
  * Called by component thread, handles the flush command from thread.
  *
  * @param
  *
  * @retval OMX_ErrorNone                  success, ready to roll
  *
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleCommandFlush(VIDDEC_COMPONENT_PRIVATE *pComponentPrivate, OMX_U32 nParam1, OMX_BOOL bPass)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_U32 aParam[4];
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;

    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x nParam1 0x%x\n",pComponentPrivate, nParam1);

    if ( nParam1 == VIDDEC_INPUT_PORT || nParam1 == VIDDEC_BOTH_PORT){
        if(bPass) {
            OMX_VidDec_Return(pComponentPrivate);
            OMX_VidDec_Return(pComponentPrivate);
            OMX_VidDec_Return(pComponentPrivate);
        }

        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
            pComponentPrivate->pLCML != NULL && pComponentPrivate->bLCMLHalted != OMX_TRUE){
            aParam[0] = USN_STRMCMD_FLUSH;
            aParam[1] = VIDDEC_INPUT_PORT;
            aParam[2] = 0;
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlStrmCtrl, (void*)aParam);
            if (eError != OMX_ErrorNone) {
                eError = OMX_ErrorHardware;
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                goto EXIT;
            }
            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);

        }
        VIDDEC_CircBuf_Flush(pComponentPrivate, VIDDEC_CBUFFER_TIMESTAMP, VIDDEC_INPUT_PORT);
        OMX_VidDec_Return(pComponentPrivate);
        OMX_VidDec_Return(pComponentPrivate);
        VIDDEC_ReturnBuffers(pComponentPrivate, VIDDEC_INPUT_PORT, OMX_TRUE);
        if(bPass) {
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandFlush,
                                                VIDDEC_INPUT_PORT,
                                                NULL);
        }
    }

    if ( nParam1 == VIDDEC_OUTPUT_PORT || nParam1 == VIDDEC_BOTH_PORT){
        if(bPass){
            OMX_VidDec_Return(pComponentPrivate);
            OMX_VidDec_Return(pComponentPrivate);
            OMX_VidDec_Return(pComponentPrivate);
        }
        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
            pComponentPrivate->pLCML != NULL && pComponentPrivate->bLCMLHalted != OMX_TRUE){
            aParam[0] = USN_STRMCMD_FLUSH;
            aParam[1] = VIDDEC_OUTPUT_PORT;
            aParam[2] = 0;
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlStrmCtrl, (void*)aParam);
            if (eError != OMX_ErrorNone) {
                eError = OMX_ErrorHardware;
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                goto EXIT;
            }
            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        }
        OMX_VidDec_Return(pComponentPrivate);
        OMX_VidDec_Return(pComponentPrivate);
        VIDDEC_ReturnBuffers(pComponentPrivate, VIDDEC_OUTPUT_PORT, OMX_TRUE);
        if(bPass) {
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventCmdComplete,
                                                OMX_CommandFlush,
                                                VIDDEC_OUTPUT_PORT,
                                                NULL);

        }
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;

}

/* ========================================================================== */
/**
  * HandleCommandMarkBuffer()
  *
  * Called by component thread, handles the Mark Buffer command from thread.
  *
  * @param
  *
  * @retval OMX_ErrorNone                  success, ready to roll
  *
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleCommandMarkBuffer(VIDDEC_COMPONENT_PRIVATE *pComponentPrivate, OMX_U32 nParam1, OMX_PTR pCmdData)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x nParam1 0x%x pCmdData 0x%x\n",pComponentPrivate, nParam1, pCmdData);
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;

}
/* ========================================================================== */
/**
  * OMX_HandleCommand() state machine in charge of interpretation of every
  * command received from application, depending on which state the component
  * is.
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleCommand (OMX_HANDLETYPE phandle, OMX_U32 nParam1)
{
    OMX_U32 message[4];
    OMX_U32 iCount = 0;
    VIDDEC_COMPONENT_PRIVATE* pComponentPrivate = (VIDDEC_COMPONENT_PRIVATE*)phandle;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDefIn = pComponentPrivate->pInPortDef;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDefOut = pComponentPrivate->pOutPortDef;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    unsigned int cmd_rcv = 0;
    OMX_HANDLETYPE hLCML = NULL;
    void* p = NULL;

#ifdef UNDER_CE
   typedef OMX_ERRORTYPE (*LPFNDLLFUNC1)(OMX_HANDLETYPE);
   HINSTANCE hDLL;
   LPFNDLLFUNC1 fpGetHandle1;
#else
   void* pMyLCML;
   VIDDEC_fpo fpGetHandle;
   char* error;
#endif

    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x phandle 0x%x\n",pComponentPrivate, nParam1);
    pComponentPrivate->frameCounter = 0;

    message[0] = 0x400;
    message[1] = 100;
    message[2] = 0;
    p = (void*)&message;
    cmd_rcv = (unsigned int)nParam1;

    switch (nParam1) {
        case OMX_StateIdle:
        VIDDECODER_DPRINT("Transitioning to OMX_StateIdle C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
        if (pComponentPrivate->eState == OMX_StateIdle) {
            eError = OMX_ErrorSameState;
            VIDDECODER_EPRINT("Same State 0x%x\n", eError);
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   OMX_ErrorSameState,
                                                   OMX_TI_ErrorMinor,
                                                   "Same state");
            break;
        }
        else if (( pComponentPrivate->eState == OMX_StateLoaded) || pComponentPrivate->eState == OMX_StateWaitForResources ) {
#ifdef __PERF_INSTRUMENTATION__
            PERF_Boundary(pComponentPrivate->pPERFcomp,
                          PERF_BoundaryStart | PERF_BoundarySetup);
#endif

            if ((pPortDefIn->bEnabled == OMX_TRUE && pPortDefOut->bEnabled == OMX_TRUE) ||
                (pPortDefIn->bEnabled == OMX_TRUE && pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL)) {
                VIDDECODER_DPRINT("Before pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",pPortDefIn->bEnabled, pPortDefOut->bEnabled);
#ifdef UNDER_CE
                while (1) {
                    if (pPortDefIn->bPopulated && pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                        VIDDECODER_DPRINT("tunneling pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",
                            pPortDefIn->bEnabled, pPortDefOut->bEnabled);
                        break;
                    }
                    if (pPortDefIn->bPopulated && pPortDefOut->bPopulated) {
                        VIDDECODER_DPRINT("Standalone pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",
                            pPortDefIn->bEnabled, pPortDefOut->bEnabled);
                        break;
                    }
                    /* Sleep for a while, so the application thread can allocate buffers */
                    VIDDEC_WAIT_CODE();
                }
#else
                if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                    if((!pComponentPrivate->pInPortDef->bPopulated) || pComponentPrivate->sInSemaphore.bSignaled) {
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        VIDDECODER_DPRINT("tunneling pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",
                            pPortDefIn->bEnabled, pPortDefOut->bEnabled);
                    }
                }
                else {
                    if((!pComponentPrivate->pOutPortDef->bPopulated) || pComponentPrivate->sOutSemaphore.bSignaled) {
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
                        VIDDECODER_DPRINT("Standalone pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",
                            pPortDefIn->bEnabled, pPortDefOut->bEnabled);
                    }
                }
#endif
                VIDDECODER_DPRINT("After pPortDefIn->bEnabled 0x%x pPortDefOut->bEnabled 0x%x\n",pPortDefIn->bEnabled, pPortDefOut->bEnabled);
            }
#ifndef UNDER_CE
            else {
                if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                    if(pComponentPrivate->sInSemaphore.bSignaled){
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                    }
                }
                else {
                    if(pComponentPrivate->sInSemaphore.bSignaled){
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                    }
                    if(pComponentPrivate->sOutSemaphore.bSignaled){
                        VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
                    }
                }
            }
#endif

#if 1
#ifndef UNDER_CE
                pMyLCML = dlopen("libLCML.so", RTLD_LAZY);
                if (!pMyLCML) {
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    fputs(dlerror(), stderr);
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                fpGetHandle = dlsym(pMyLCML, "GetHandle");
                if ((error = dlerror()) != NULL) {
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    fputs(error, stderr);
                    dlclose(pMyLCML);
                    pMyLCML = NULL;
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                eError = (*fpGetHandle)(&hLCML);
                if (eError != OMX_ErrorNone) {
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    dlclose(pMyLCML);
                    pMyLCML = NULL;
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                pComponentPrivate->pModLCML = pMyLCML;
#else
                hDLL = LoadLibraryEx(TEXT("OAF_BML.dll"), NULL, 0);
                if (hDLL == NULL) {
                    VIDDECODER_EPRINT("BML Load Failed!!!\n");
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                fpGetHandle1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,TEXT("GetHandle"));
                if (!fpGetHandle1) {
                    /* handle the error */
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    FreeLibrary(hDLL);
					hDLL = NULL;
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                /* call the function */
                eError = fpGetHandle1(&hLCML);
                if (eError != OMX_ErrorNone) {
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    FreeLibrary(hDLL);
                    hDLL = NULL;
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
                pComponentPrivate->pModLCML = hDLL;
#endif

                pComponentPrivate->eLCMLState = VidDec_LCML_State_Load;
                VIDDECODER_DPRINT("LCML Handler 0x%x\n",hLCML);
                /*(LCML_DSP_INTERFACE*)pComponentPrivate->pLCML = (LCML_DSP_INTERFACE*)hLCML;*/
                pComponentPrivate->pLCML = (LCML_DSP_INTERFACE*)hLCML;
                pComponentPrivate->pLCML->pComponentPrivate = pComponentPrivate;
#endif

#ifdef __PERF_INSTRUMENTATION__
                pComponentPrivate->lcml_nCntOpReceived = 0;
#endif
                eError = OMX_ErrorNone;
#ifndef UNDER_CE
                pComponentPrivate->bLCMLOut = OMX_TRUE;
#endif
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                    eError = VIDDEC_InitDSP_H264Dec(pComponentPrivate);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                         pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                    eError = VIDDEC_InitDSP_Mpeg4Dec(pComponentPrivate);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                    eError = VIDDEC_InitDSP_Mpeg2Dec(pComponentPrivate);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                    eError = VIDDEC_InitDSP_WMVDec(pComponentPrivate);
                }
#ifdef VIDDEC_SPARK_CODE
                else if (VIDDEC_SPARKCHECK) {
                    eError = VIDDEC_InitDSP_SparkDec(pComponentPrivate);
                }
#endif
                else {
                    VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting\n");
                    eError = OMX_ErrorUnsupportedSetting;
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventError,
                                                           OMX_ErrorUnsupportedSetting,
                                                           OMX_TI_ErrorMinor,
                                                           "DSP Initialization");
                    goto EXIT;
                }
                /************************************************************************/
                /************************************************************************/
                /************************************************************************/
                if(eError != OMX_ErrorNone){
                    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                        pComponentPrivate->pModLCML != NULL){
#ifndef UNDER_CE
                        if(pComponentPrivate->pModLCML != NULL){
                            dlclose(pComponentPrivate->pModLCML);
                            pComponentPrivate->pModLCML = NULL;
                            pComponentPrivate->pLCML = NULL;
                            pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
                        }
#else
                        if(pComponentPrivate->pModLCML != NULL){
                            FreeLibrary(pComponentPrivate->pModLCML);
                            pComponentPrivate->pModLCML = NULL;
                            pComponentPrivate->pLCML = NULL;
                            pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
                        }
#endif

                        pComponentPrivate->bLCMLHalted = OMX_TRUE;
                    }
                    VIDDECODER_EPRINT("LCML Error %x\n", pComponentPrivate->eState);
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventError,
                                                           eError,
                                                           OMX_TI_ErrorSevere,
                                                           "DSP Initialization");
                     goto EXIT;
                }

#ifndef UNDER_CE
                pComponentPrivate->bLCMLOut = OMX_FALSE;
#endif
                pComponentPrivate->bLCMLHalted = OMX_FALSE;
                pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                if (!pLcmlHandle) {
                    VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
                    eError = OMX_ErrorHardware;
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventError,
                                                           OMX_ErrorHardware,
                                                           OMX_TI_ErrorSevere,
                                                           "Lcml Handle NULL");
                    goto EXIT;
                }
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Init;

#if 1
#ifdef RESOURCE_MANAGER_ENABLED
                pComponentPrivate->rmproxyCallback.RMPROXY_Callback = (void *) VIDDEC_ResourceManagerCallback;
                if(pComponentPrivate->eRMProxyState != VidDec_RMPROXY_State_Unload){
                    VIDDECODER_DPRINT("memory usage 1 %d : %d bytes\n",(unsigned int)pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0],(unsigned int)VIDDEC_MEMUSAGE);
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_H264_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_MPEG4_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_H263_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_MPEG2_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_WMV_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
#ifdef VIDDEC_SPARK_CODE
                    else if (VIDDEC_SPARKCHECK) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_RequestResource, OMX_MPEG4_Decode_COMPONENT, VIDDEC_GetRMFrecuency(pComponentPrivate), VIDDEC_MEMUSAGE, &(pComponentPrivate->rmproxyCallback));
                    }
#endif
                    else {
                        eError = OMX_ErrorUnsupportedSetting;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               OMX_ErrorUnsupportedSetting,
                                                               OMX_TI_ErrorMajor,
                                                               "RM SendCommand OMX_ErrorUnsupportedSetting Error");
                        VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting 0x%x\n",eError);
                        goto EXIT;
                    }
                    if (eError != OMX_ErrorNone) {
                        pComponentPrivate->eState = OMX_StateLoaded;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               OMX_ErrorInsufficientResources,
                                                               OMX_TI_ErrorMajor,
                                                               "RM SendCommand Error");
                        VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting 0x%x\n",eError);
                        break;
                    }
                    pComponentPrivate->eRMProxyState = VidDec_RMPROXY_State_Registered;
                }
#endif
#endif

#ifdef VIDDEC_FLAGGED_EOS
                if (pComponentPrivate->bUseFlaggedEos) {
                    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                        pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                        pComponentPrivate->pLCML != NULL &&
                        pComponentPrivate->bLCMLHalted != OMX_TRUE){
                        VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlUsnEos 0x%x\n",pLcmlHandle);
                        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlUsnEos, NULL);
                        if (eError != OMX_ErrorNone) {
                            VIDDECODER_EPRINT("error in EMMCodecControlUsnEos %x\n",eError);
                            eError = OMX_ErrorHardware;
                            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                                   OMX_EventError,
                                                                   OMX_ErrorHardware,
                                                                   OMX_TI_ErrorSevere,
                                                                   "LCML_ControlCodec EMMCodecControlUsnEos function");
                            VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
                            goto EXIT;
                        }
                    }
                }
#endif
               if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat != OMX_VIDEO_CodingWMV) {
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        message[1] = 4;
                    }
                    else {
                        message[1] = 100;
                    }
                    message[0] = 0x400;
                    message[2] = 0;
                    p = (void*)&message;
                    VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlSendDspMessage 0x%x\n",pLcmlHandle);
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlSendDspMessage, (void *)p);
                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT("error in EMMCodecControlSendDspMessage %x\n",eError);
                        eError = OMX_ErrorHardware;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               OMX_ErrorHardware,
                                                               OMX_TI_ErrorSevere,
                                                               "LCML_ControlCodec function");
                        VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        goto EXIT;
                    }
                    VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                    VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                }
               }

                pComponentPrivate->eState = OMX_StateIdle;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
               VIDDECODER_DPRINT("Transition to OMX_StateIdle\n");
               break;
            }
            else if (pComponentPrivate->eState == OMX_StateExecuting || pComponentPrivate->eState == OMX_StatePause) {
                /*Set the bIsStopping bit*/
                pComponentPrivate->bIsStopping = 1;
                VIDDEC_BUFFERPRINT("bIsStopping 0x%x\n",pComponentPrivate->bIsStopping);
                VIDDEC_BUFFERPRINT("eExecuteToIdle 0x%x\n",pComponentPrivate->eExecuteToIdle);
                OMX_VidDec_Return(pComponentPrivate);
                OMX_VidDec_Return(pComponentPrivate);

#ifdef __PERF_INSTRUMENTATION__
                PERF_Boundary(pComponentPrivate->pPERFcomp,
                              PERF_BoundaryComplete | PERF_BoundarySteadyState);
#endif
                    pComponentPrivate->bIsPaused = 0;
                    pComponentPrivate->iEndofInput = 0;
                    pComponentPrivate->iEndofInputSent = 0;
/********************************************************************************************************************/
           if (pComponentPrivate->bIsStopping == OMX_TRUE) {
                pComponentPrivate->bIsPaused = OMX_FALSE;
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                    pLcmlHandle = (LCML_DSP_INTERFACE *)pComponentPrivate->pLCML;
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, MMCodecControlStop, NULL);
                    if (eError != OMX_ErrorNone) {
                        eError = OMX_ErrorHardware;
                        VIDDECODER_EPRINT("Error Occurred in Codec Stop...\n");
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        break;
                    }
                    pComponentPrivate->eLCMLState = VidDec_LCML_State_Stop;
                    VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                    VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                }
                VIDDEC_HandleCommandFlush(pComponentPrivate, -1, OMX_FALSE);

#if 1
#ifdef RESOURCE_MANAGER_ENABLED
            if(pComponentPrivate->eRMProxyState != VidDec_RMPROXY_State_Unload){
                VIDDECODER_DPRINT("memory usage 2 %d : %d bytes\n",(unsigned int)pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0],(unsigned int)VIDDEC_MEMUSAGE);
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_H264_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG4_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_H263_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG2_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_WMV_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
#ifdef VIDDEC_SPARK_CODE
                else if (VIDDEC_SPARKCHECK) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG4_Decode_COMPONENT, OMX_StateIdle, VIDDEC_MEMUSAGE, NULL);
                }
#endif
            }
#endif
#endif
                if(pComponentPrivate->pUalgParams != NULL) {
                    free(pComponentPrivate->pUalgParams);
                    pComponentPrivate->pUalgParams = NULL;
                }

                pComponentPrivate->bIsStopping = OMX_FALSE;
                pComponentPrivate->eState = OMX_StateIdle;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
                eError = OMX_ErrorNone;
                pComponentPrivate->bTransPause = 0;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->eExecuteToIdle = OMX_StateInvalid;
            }
/********************************************************************************************************************/

            }
            else {
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorIncorrectStateTransition,
                                                       OMX_TI_ErrorMinor,
                                                       NULL);
                VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
            }
            pComponentPrivate->eExecuteToIdle = OMX_StateInvalid;
            VIDDEC_BUFFERPRINT("Passing bIsStopping 0x%x\n",pComponentPrivate->bIsStopping);
            VIDDEC_BUFFERPRINT("Passing eExecuteToIdle 0x%x\n",pComponentPrivate->eExecuteToIdle);
            break;

        case OMX_StateExecuting:
#ifdef __PERF_INSTRUMENTATION__
                pComponentPrivate->lcml_nCntOpReceived = 0;
#endif
            VIDDEC_BUFFERPRINT("Transitioning to OMX_StateExecuting C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            if (pComponentPrivate->eState == OMX_StateExecuting) {
                eError = OMX_ErrorSameState;
                pComponentPrivate->cbInfo.EventHandler (pComponentPrivate->pHandle,
                                                        pComponentPrivate->pHandle->pApplicationPrivate,
                                                        OMX_EventError,
                                                        OMX_ErrorSameState,
                                                        OMX_TI_ErrorMinor,
                                                        "Invalid State");
                 VIDDECODER_EPRINT("OMX_ErrorSameState 0x%x\n",eError);
            }
            else if (pComponentPrivate->eState == OMX_StateIdle || pComponentPrivate->eState == OMX_StatePause) {
                pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->bFirstBuffer = 1;
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlStart 0x%x\n",pLcmlHandle);
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlStart,NULL);
                    if (eError != OMX_ErrorNone) {
                        eError = OMX_ErrorHardware;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               OMX_ErrorHardware,
                                                               OMX_TI_ErrorSevere,
                                                               "LCML_ControlCodec Start");
                        break;
                        VIDDECODER_EPRINT("Occurred in Codec Start... 0x%x\n",eError);
                    }
                }
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Start;
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC &&
                    pComponentPrivate->eState == OMX_StateIdle) {
                    H264_Iualg_Cmd_SetStatus* pDynParams = NULL;
                    char* pTmp = NULL;
                    OMX_U32 cmdValues[3] = {0, 0, 0};

                    VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);
                    VIDDECODER_DPRINT("Initializing DSP for h264 eCompressionFormat 0x%x\n",
                    pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
                    OMX_MALLOC_STRUCT_SIZED(pDynParams, H264_Iualg_Cmd_SetStatus, sizeof(H264_Iualg_Cmd_SetStatus) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
                    if (pDynParams == NULL) {
                       VIDDECODER_EPRINT("Error: Malloc failed\n");
                       eError = OMX_ErrorInsufficientResources;
                       goto EXIT;
                   }
                    memset(pDynParams, 0, sizeof(H264_Iualg_Cmd_SetStatus) + VIDDEC_PADDING_FULL);
                    pTmp = (char*)pDynParams;
                    pTmp += VIDDEC_PADDING_HALF;
                    pDynParams = (H264_Iualg_Cmd_SetStatus*)pTmp;
#ifdef VIDDEC_SN_R8_14
                    pDynParams->size = sizeof(H264_Iualg_Cmd_SetStatus);
#endif
                    pDynParams->ulDecodeHeader = 1;
                    pDynParams->ulDisplayWidth = 0;
                    pDynParams->ulFrameSkipMode = 0;
                    pDynParams->ulPPType = 0;

                    cmdValues[0] = IUALG_CMD_SETSTATUS;
                    cmdValues[1] = (OMX_U32)(pDynParams);
                    cmdValues[2] = sizeof(H264_Iualg_Cmd_SetStatus);

                    p = (void*)&cmdValues;
                    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                        pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                        pComponentPrivate->pLCML != NULL &&
                        pComponentPrivate->bLCMLHalted != OMX_TRUE){
                        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                                   EMMCodecControlAlgCtrl,
                                                   (void*)p);
                        if (eError != OMX_ErrorNone) {
                            eError = OMX_ErrorHardware;
                            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                            break;
                        }
                        VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                    }
                    pTmp = (char*)pDynParams;
                    pTmp -= VIDDEC_PADDING_HALF;
                    pDynParams = (H264_Iualg_Cmd_SetStatus*)pTmp;
                    free(pDynParams);

                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                        goto EXIT;
                    }
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV &&
                    pComponentPrivate->eState == OMX_StateIdle) {
                    WMV9DEC_UALGDynamicParams* pDynParams = NULL;
                    char* pTmp = NULL;
                    OMX_U32 cmdValues[3] = {0, 0, 0};

                    VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);
                    VIDDECODER_DPRINT("Initializing DSP for wmv9 eCompressionFormat 0x%x\n",
                        pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
                    OMX_MALLOC_STRUCT_SIZED(pDynParams, WMV9DEC_UALGDynamicParams, sizeof(WMV9DEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
                    if (pDynParams == NULL) {
                       VIDDECODER_EPRINT("Error: Malloc failed\n");
                       eError = OMX_ErrorInsufficientResources;
                       goto EXIT;
                    }
                    memset(pDynParams, 0, sizeof(WMV9DEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
                    pTmp = (char*)pDynParams;
                    pTmp += VIDDEC_PADDING_HALF;
                    pDynParams = (WMV9DEC_UALGDynamicParams*)pTmp;

#ifdef VIDDEC_SN_R8_14
                    pDynParams->size = sizeof(WMV9DEC_UALGDynamicParams);
#endif
                    pDynParams->ulDecodeHeader = 0;
                    pDynParams->ulDisplayWidth = 0;
                    pDynParams->ulFrameSkipMode = 0;
                    pDynParams->ulPPType = 0;

                    if (pComponentPrivate->nWMVFileType == VIDDEC_WMV_ELEMSTREAM) {
                        pDynParams->usIsElementaryStream = VIDDEC_SN_WMV_ELEMSTREAM;
                    }
                    else {
                        pDynParams->usIsElementaryStream = VIDDEC_SN_WMV_RCVSTREAM;
                    }

                    cmdValues[0] = IUALG_CMD_SETSTATUS; /* add #define IUALG_CMD_SETSTATUS 3 */
                    cmdValues[1] = (OMX_U32)(pDynParams);
                    cmdValues[2] = sizeof(WMV9DEC_UALGDynamicParams);

                    p = (void*)&cmdValues;
                    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                        pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                        pComponentPrivate->pLCML != NULL &&
                        pComponentPrivate->bLCMLHalted != OMX_TRUE){
                        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                                   EMMCodecControlAlgCtrl,
                                                   (void*)p);
                         if(eError != OMX_ErrorNone){
                            eError = OMX_ErrorHardware;
                            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                                   OMX_EventError,
                                                                   eError,
                                                                   OMX_TI_ErrorSevere,
                                                                   "LCML_ControlCodec function");
                            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                            goto EXIT;
                        }
                        VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                    }

                    pTmp = (char*)pDynParams;
                    pTmp -= VIDDEC_PADDING_HALF;
                    pDynParams = (WMV9DEC_UALGDynamicParams*)pTmp;
                    free(pDynParams);

                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                        goto EXIT;
                    }
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2 &&
                    pComponentPrivate->eState == OMX_StateIdle) {
                    VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);
                }
#ifdef VIDDEC_SPARK_CODE
                else if (VIDDEC_SPARKCHECK) {
                    if(pComponentPrivate->eState == OMX_StateIdle) {
                        SPARKVDEC_UALGDynamicParams* pDynParams = NULL;
                        char* pTmp = NULL;
                        OMX_U32 cmdValues[3] = {0, 0, 0};

                        VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);
                        VIDDECODER_DPRINT("Initializing DSP for mpeg4 and h263 eCompressionFormat 0x%x\n",
                        pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
                        OMX_MALLOC_STRUCT_SIZED(pDynParams, SPARKVDEC_UALGDynamicParams, sizeof(SPARKVDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
                        if (pDynParams == NULL) {
                           VIDDECODER_EPRINT("Error: Malloc failed\n");
                           eError = OMX_ErrorInsufficientResources;
                           goto EXIT;
                        }
                        memset(pDynParams, 0, sizeof(SPARKVDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
                        pTmp = (char*)pDynParams;
                        pTmp += VIDDEC_PADDING_HALF;
                        pDynParams = (SPARKVDEC_UALGDynamicParams*)pTmp;
    #ifdef VIDDEC_SN_R8_14
                        pDynParams->size = sizeof(SPARKVDEC_UALGDynamicParams);
    #endif
                        pDynParams->ulDecodeHeader = 0;
                        pDynParams->ulDisplayWidth = 0;
                        pDynParams->ulFrameSkipMode = 0;
                        pDynParams->ulPPType = 0;

                        cmdValues[0] = IUALG_CMD_SETSTATUS;
                        cmdValues[1] = (OMX_U32)(pDynParams);
                        cmdValues[2] = sizeof(SPARKVDEC_UALGDynamicParams);

                        /*pComponentPrivate->bTransPause = 0;*//*flag to wait for the generated event*/
                        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                        p = (void*)&cmdValues;
                        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                            pComponentPrivate->pLCML != NULL &&
                            pComponentPrivate->bLCMLHalted != OMX_TRUE){
                            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                                       EMMCodecControlAlgCtrl,
                                                       (void*)p);
                            if (eError != OMX_ErrorNone) {
                                eError = OMX_ErrorHardware;
                                goto EXIT;
                            }
                            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        }

                        pTmp = (char*)pDynParams;
                        pTmp -= VIDDEC_PADDING_HALF;
                        pDynParams = (SPARKVDEC_UALGDynamicParams*)pTmp;
                        free(pDynParams);

                        if (eError != OMX_ErrorNone) {
                            VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                            goto EXIT;
                        }
                    }
                }
#endif
                else {
                    if(pComponentPrivate->eState == OMX_StateIdle) {
                        MP4VDEC_UALGDynamicParams* pDynParams = NULL;
                        char* pTmp = NULL;
                        OMX_U32 cmdValues[3] = {0, 0, 0};

                        VIDDECODER_DPRINT("Initializing DSP for mpeg4 and h263 eCompressionFormat 0x%x\n",
                        pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
                        OMX_MALLOC_STRUCT_SIZED(pDynParams, MP4VDEC_UALGDynamicParams, sizeof(MP4VDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
                        if (pDynParams == NULL) {
                           VIDDECODER_EPRINT("Error: Malloc failed\n");
                           eError = OMX_ErrorInsufficientResources;
                           goto EXIT;
                        }
                        memset(pDynParams, 0, sizeof(MP4VDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
                        pTmp = (char*)pDynParams;
                        pTmp += VIDDEC_PADDING_HALF;
                        pDynParams = (MP4VDEC_UALGDynamicParams*)pTmp;
    #ifdef VIDDEC_SN_R8_14
                        pDynParams->size = sizeof(MP4VDEC_UALGDynamicParams);
    #endif
                        pDynParams->ulDecodeHeader = 0;
                        pDynParams->ulDisplayWidth = 0;
                        pDynParams->ulFrameSkipMode = 0;
                        pDynParams->ulPPType = 0;

                        cmdValues[0] = IUALG_CMD_SETSTATUS;
                        cmdValues[1] = (OMX_U32)(pDynParams);
                        cmdValues[2] = sizeof(MP4VDEC_UALGDynamicParams);

                        /*pComponentPrivate->bTransPause = 0;*//*flag to wait for the generated event*/
                        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                        p = (void*)&cmdValues;
                        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                            pComponentPrivate->pLCML != NULL &&
                            pComponentPrivate->bLCMLHalted != OMX_TRUE){
                            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                                       EMMCodecControlAlgCtrl,
                                                       (void*)p);
                            if (eError != OMX_ErrorNone) {
                                eError = OMX_ErrorHardware;
                                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                                goto EXIT;
                            }
                            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        }

                        pTmp = (char*)pDynParams;
                        pTmp -= VIDDEC_PADDING_HALF;
                        pDynParams = (MP4VDEC_UALGDynamicParams*)pTmp;
                        free(pDynParams);

                        if (eError != OMX_ErrorNone) {
                            VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                            goto EXIT;
                        }
                    }
                }

                /* Start existing code */
#if 1
#ifdef RESOURCE_MANAGER_ENABLED
            if(pComponentPrivate->eRMProxyState != VidDec_RMPROXY_State_Unload){
                VIDDECODER_DPRINT("memory usage 3 %d : %d bytes\n",(unsigned int)pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0],(unsigned int)VIDDEC_MEMUSAGE);
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_H264_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG4_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_H263_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG2_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_WMV_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
#ifdef VIDDEC_SPARK_CODE
                else if (VIDDEC_SPARKCHECK) {
                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_StateSet, OMX_MPEG4_Decode_COMPONENT, OMX_StateExecuting, VIDDEC_MEMUSAGE, NULL);
                }
#endif
                else {
                    eError = OMX_ErrorUnsupportedSetting;
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventError,
                                                           OMX_ErrorUnsupportedSetting,
                                                           OMX_TI_ErrorMinor,
                                                           "RM SendCommand OMX_ErrorUnsupportedSetting Error");
                    VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting 0x%x\n",eError);
                    goto EXIT;
                }
                if (eError != OMX_ErrorNone) {
                    pComponentPrivate->eState = OMX_StateLoaded;
                     pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                            pComponentPrivate->pHandle->pApplicationPrivate,
                                                            OMX_EventError,
                                                            OMX_ErrorInsufficientResources,
                                                            OMX_TI_ErrorMajor,
                                                            "RM SendCommand Error");
                     VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting 0x%x\n",eError);
                     break;
                }
            }
#endif
#endif

                eError = OMX_ErrorNone;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->iEndofInput = 0;
                pComponentPrivate->iEndofInputSent = 0;
                pComponentPrivate->eState = OMX_StateExecuting;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
                VIDDECODER_DPRINT("Transition to OMX_StateExecuting\n");
            }
            else {
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorIncorrectStateTransition,
                                                       OMX_TI_ErrorMinor,
                                                       "Incorrect State Transition");
                 VIDDECODER_EPRINT("OMX_ErrorIncorrectStateTransition 0x%x\n",eError);
            }
            break;
        case OMX_StateLoaded:
            VIDDECODER_DPRINT("Transitioning to OMX_StateLoaded C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            if (pComponentPrivate->eState == OMX_StateLoaded) {
                eError = OMX_ErrorSameState;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorSameState,
                                                       OMX_TI_ErrorMinor,
                                                       "Same State");
                 VIDDECODER_EPRINT("OMX_ErrorSameState 0x%x\n",eError);
            }
            else if (pComponentPrivate->eState == OMX_StateIdle) {
#ifdef __PERF_INSTRUMENTATION__
                PERF_Boundary(pComponentPrivate->pPERFcomp,
                              PERF_BoundaryStart | PERF_BoundaryCleanup);
#endif
                OMX_VidDec_Return(pComponentPrivate);
                OMX_VidDec_Return(pComponentPrivate);
                pComponentPrivate->eIdleToLoad = OMX_StateLoaded;
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlDestroy 1x%x\n",pLcmlHandle);
                    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, EMMCodecControlDestroy, NULL);
                    if (eError != OMX_ErrorNone) {
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               OMX_ErrorHardware,
                                                               OMX_TI_ErrorSevere,
                                                               NULL);
                        VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
                        break;
                    }
                    pComponentPrivate->eLCMLState = VidDec_LCML_State_Destroy;
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlDestroy 0x%x\n",pLcmlHandle);
                }

                VIDDECODER_DPRINT("Closing LCML lib 0x%x\n",pComponentPrivate->pModLCML);

#ifndef UNDER_CE
                if(pComponentPrivate->pModLCML != NULL){
                    dlclose(pComponentPrivate->pModLCML);
                    pComponentPrivate->pModLCML = NULL;
                    pComponentPrivate->pLCML = NULL;
                }
#else
                if(pComponentPrivate->pModLCML != NULL){
                    FreeLibrary(pComponentPrivate->pModLCML);
                    pComponentPrivate->pModLCML = NULL;
                    pComponentPrivate->pLCML = NULL;
                }
#endif
            pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;

               VIDDECODER_DPRINT("Closed LCML lib 0x%x\n",pComponentPrivate->pModLCML);
               VIDDECODER_DPRINT("Waiting for unpopulate ports IN 0x%x OUT 0x%x\n",pPortDefIn->bEnabled,pPortDefOut->bEnabled);
               VIDDECODER_DPRINT("Tunneling 0x%x\n",(pComponentPrivate->pCompPort[1]->hTunnelComponent));
               if ((pPortDefIn->bEnabled == OMX_TRUE && pPortDefOut->bEnabled == OMX_TRUE) ||
                (pPortDefIn->bEnabled == OMX_TRUE && pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL)) {
#ifdef UNDER_CE
                    while(1) {
                        if(!pPortDefIn->bPopulated && !pPortDefOut->bPopulated) {
                            VIDDECODER_DPRINT("Standalone unpopulated ports IN 0x%x OUT 0x%x\n",pPortDefIn->bEnabled,pPortDefOut->bEnabled);
                            eError = OMX_ErrorNone;
                            pComponentPrivate->bIsPaused = 0;
                            pComponentPrivate->eState = OMX_StateLoaded;
                            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                                   OMX_EventCmdComplete,
                                                                   OMX_CommandStateSet,
                                                                   pComponentPrivate->eState,
                                                                   NULL);
                            VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                            VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_VARS);
                            pComponentPrivate->eIdleToLoad = OMX_StateInvalid;
                            break;
                        }
                        else if(!pPortDefIn->bPopulated && (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL)) {
                            VIDDECODER_DPRINT("Tunneling unpopulated ports IN 0x%x TUNNEL 0x%x\n",
                                pPortDefIn->bEnabled,pComponentPrivate->pCompPort[1]->hTunnelComponent);
                            eError = OMX_ErrorNone;
                            pComponentPrivate->bIsPaused = 0;
                            pComponentPrivate->eState = OMX_StateLoaded;
                            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                                   OMX_EventCmdComplete,
                                                                   OMX_CommandStateSet,
                                                                   pComponentPrivate->eState,
                                                                   NULL);
                            VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                            VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_VARS);
                            pComponentPrivate->eIdleToLoad = OMX_StateInvalid;
                            break;
                        }
                        VIDDEC_WAIT_CODE();
                    }
#else
                    if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                        if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pInPortDef->bPopulated) ||
                            pComponentPrivate->sInSemaphore.bSignaled) {
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        }
                        VIDDECODER_DPRINT("Tunneling unpopulated ports IN 0x%x TUNNEL 0x%x\n",
                            pPortDefIn->bEnabled,pComponentPrivate->pCompPort[1]->hTunnelComponent);
                        eError = OMX_ErrorNone;
                        pComponentPrivate->bIsPaused = 0;
                        pComponentPrivate->eState = OMX_StateLoaded;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventCmdComplete,
                                                               OMX_CommandStateSet,
                                                               pComponentPrivate->eState,
                                                               NULL);
                        VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                        VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_VARS);
                        pComponentPrivate->eIdleToLoad = OMX_StateInvalid;
                    }
                    else {
                        if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pInPortDef->bPopulated) ||
                            pComponentPrivate->sInSemaphore.bSignaled) {
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        }
                        if((!(pComponentPrivate->eState == OMX_StateLoaded) && pComponentPrivate->pOutPortDef->bPopulated) ||
                            pComponentPrivate->sOutSemaphore.bSignaled) {
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
                        }
                        VIDDECODER_DPRINT("Standalone unpopulated ports IN 0x%x OUT 0x%x\n",pPortDefIn->bEnabled,pPortDefOut->bEnabled);
                        eError = OMX_ErrorNone;
                        pComponentPrivate->bIsPaused = 0;
                        pComponentPrivate->eState = OMX_StateLoaded;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventCmdComplete,
                                                               OMX_CommandStateSet,
                                                               pComponentPrivate->eState,
                                                               NULL);
                        VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                        VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_VARS);
                        pComponentPrivate->eIdleToLoad = OMX_StateInvalid;

                    }


#endif
#ifdef __PERF_INSTRUMENTATION__
                    PERF_Boundary(pComponentPrivate->pPERFcomp,
                                  PERF_BoundaryComplete | PERF_BoundaryCleanup);
#endif

                }
#ifndef UNDER_CE
                else {
                    if(pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                        if(pComponentPrivate->sInSemaphore.bSignaled){
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        }
                    }
                    else {
                        if(pComponentPrivate->sInSemaphore.bSignaled){
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sInSemaphore);
                        }
                        if(pComponentPrivate->sOutSemaphore.bSignaled){
                            VIDDEC_PTHREAD_SEMAPHORE_WAIT(pComponentPrivate->sOutSemaphore);
                        }
                    }
                    pComponentPrivate->eState = OMX_StateLoaded;
                    pComponentPrivate->bIsPaused = 0;
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventCmdComplete,
                                                           OMX_CommandStateSet,
                                                           pComponentPrivate->eState,
                                                           NULL);
                    pComponentPrivate->eIdleToLoad = OMX_StateInvalid;
                    VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_VARS);
                    VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                    break;
                }
#endif
            }
            else if (pComponentPrivate->eState == OMX_StateWaitForResources) {
                pComponentPrivate->eState = OMX_StateLoaded;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
                VIDDECODER_DPRINT("Transition to OMX_StateLoaded\n");
                break;
            }
            else {
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorIncorrectStateTransition,
                                                       OMX_TI_ErrorMinor,
                                                       NULL);
                VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n",eError);
            }
            break;
        case OMX_StatePause:
            OMX_VidDec_Return(pComponentPrivate);
                OMX_VidDec_Return(pComponentPrivate);

            VIDDECODER_DPRINT("Transitioning to OMX_StatePause C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            if (pComponentPrivate->eState == OMX_StatePause) {
                eError = OMX_ErrorSameState;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorSameState,
                                                       OMX_TI_ErrorMinor,
                                                       NULL);
                VIDDECODER_EPRINT("Same State 0x%x\n",eError);
                break;
            }
            else if (pComponentPrivate->eState == OMX_StateExecuting) {
#ifdef __PERF_INSTRUMENTATION__
                pComponentPrivate->lcml_nCntOpReceived = 0;
                PERF_Boundary(pComponentPrivate->pPERFcomp,
                              PERF_BoundaryComplete | PERF_BoundarySteadyState);
#endif

                VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                pComponentPrivate->bIsPaused = 1;
                OMX_VidDec_Return(pComponentPrivate);
                OMX_VidDec_Return(pComponentPrivate);
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlPause 0x%x\n",pLcmlHandle);
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, EMMCodecControlPause, NULL);
                    if (eError != OMX_ErrorNone) {
                        eError = OMX_ErrorHardware;
                        VIDDECODER_EPRINT("Error during EMMCodecControlPause...\n");
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               eError,
                                                               OMX_TI_ErrorSevere,
                                                               NULL);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        break;
                    }
                    eError = OMX_ErrorNone;
                    VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                    VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                }
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Pause;
                OMX_VidDec_Return(pComponentPrivate);
                OMX_VidDec_Return(pComponentPrivate);
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Pause;
                eError = OMX_ErrorNone;
                pComponentPrivate->bIsPaused = 1;
                pComponentPrivate->eState = OMX_StatePause;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);

                break;
            }
            else if (pComponentPrivate->eState == OMX_StateIdle) {
                pComponentPrivate->bIsPaused = 1;
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlPause 0x%x\n",pLcmlHandle);
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, EMMCodecControlPause, NULL);
                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT("During EMMCodecControlPause...\n");
                        eError = OMX_ErrorHardware;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               eError,
                                                               OMX_TI_ErrorSevere,
                                                               NULL);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                        break;
                    }
                    eError = OMX_ErrorNone;
                    VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                    VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                }
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Pause;
                eError = OMX_ErrorNone;
                pComponentPrivate->bIsPaused = 1;
                pComponentPrivate->eState = OMX_StatePause;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
                VIDDECODER_EPRINT("Transition to OMX_StatePause\n");
            }
            else {
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorIncorrectStateTransition,
                                                       OMX_TI_ErrorMinor,
                                                       "Incorrect State Transition");
                VIDDECODER_EPRINT("Incorrect State Transition\n");
            }
            break;
        case OMX_StateInvalid:
            VIDDECODER_DPRINT("Transitioning to OMX_StateInvalid C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            if (pComponentPrivate->eState == OMX_StateInvalid) {
                eError = OMX_ErrorSameState;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorSameState,
                                                       OMX_TI_ErrorCritical,
                                                       "Same State");
                VIDDECODER_EPRINT("Same State...\n");
                break;
            }
            else if (pComponentPrivate->eState == OMX_StateIdle || pComponentPrivate->eState == OMX_StateExecuting) {
                pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
                if (pComponentPrivate->eState == OMX_StateExecuting) {
                    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                        pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                        pComponentPrivate->pLCML != NULL &&
                        pComponentPrivate->bLCMLHalted != OMX_TRUE) {
                        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
                        VIDDECODER_DPRINT("LCML_ControlCodec called MMCodecControlStop 0x%x\n",eError);
                        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, MMCodecControlStop, NULL);
                        if (eError != OMX_ErrorNone) {
                            VIDDECODER_EPRINT("Occurred in Codec Stop...\n");
                            eError = OMX_ErrorHardware;
                            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                                   OMX_EventError,
                                                                   eError,
                                                                   OMX_TI_ErrorCritical,
                                                                   NULL);
                            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                            break;
                        }
                        VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                    }

                    pComponentPrivate->eLCMLState = VidDec_LCML_State_Stop;
                }
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->pLCML != NULL &&
                    pComponentPrivate->bLCMLHalted != OMX_TRUE){
                    eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, EMMCodecControlDestroy, NULL);
                    VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlDestroy 0x%x\n",pLcmlHandle);
                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT("Occurred in Codec Destroy...\n");
                        eError = OMX_ErrorHardware;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                                               OMX_EventError,
                                                               eError,
                                                               OMX_TI_ErrorCritical,
                                                               NULL);
                        VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
                        break;
                    }
                }

                pComponentPrivate->eLCMLState = VidDec_LCML_State_Destroy;
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload) {
#ifndef UNDER_CE
                    if(pComponentPrivate->pModLCML != NULL){
					    dlclose(pComponentPrivate->pModLCML);
					    pComponentPrivate->pModLCML = NULL;
					    pComponentPrivate->pLCML = NULL;
					    pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
				    }
#else
                    if(pComponentPrivate->pModLCML != NULL){
					    FreeLibrary(pComponentPrivate->pModLCML);
					    pComponentPrivate->pModLCML = NULL;
					    pComponentPrivate->pLCML = NULL;
					    pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
				    }
#endif
                }
                for (iCount = 0; iCount < MAX_PRIVATE_BUFFERS; iCount++) {
                    if(pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[iCount]->bAllocByComponent == OMX_TRUE){
                        if(pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr != NULL) {
		                    OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
		                    OMX_U8* pTemp = NULL;
		                    pBuffHead = pComponentPrivate->pCompPort[VIDDEC_INPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr;
                            if(pBuffHead->pBuffer != NULL){
                                pTemp = (OMX_U8*)(pBuffHead->pBuffer);
                                pTemp -= VIDDEC_PADDING_HALF;
                                pBuffHead->pBuffer = (OMX_U8*)pTemp;
                                free(pBuffHead->pBuffer);
                                pBuffHead->pBuffer = NULL;
                                pTemp = NULL;
                            }
		                }
                    }
                }

                for (iCount = 0; iCount < MAX_PRIVATE_BUFFERS; iCount++) {
		            if(pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[iCount]->bAllocByComponent == OMX_TRUE){
                        if(pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr != NULL) {
		                    OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
		                    OMX_U8* pTemp = NULL;
		                    pBuffHead = pComponentPrivate->pCompPort[VIDDEC_OUTPUT_PORT]->pBufferPrivate[iCount]->pBufferHdr;
                            if(pBuffHead->pBuffer != NULL){
                                pTemp = (OMX_U8*)(pBuffHead->pBuffer);
                                pTemp -= VIDDEC_PADDING_HALF;
                                pBuffHead->pBuffer = (OMX_U8*)pTemp;
                                free(pBuffHead->pBuffer);
                                pBuffHead->pBuffer = NULL;
                                pTemp = NULL;
                            }
		                }
                    }
                }
#ifdef RESOURCE_MANAGER_ENABLED
/*#ifndef UNDER_CE*/
                if(pComponentPrivate->eRMProxyState == VidDec_RMPROXY_State_Registered){
                    VIDDECODER_DPRINT("memory usage 4 %d : %d bytes\n",(unsigned int)pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel0],(unsigned int)VIDDEC_MEMUSAGE);
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
			            eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_H264_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                        eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_WMV_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4) {
    	                eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_MPEG4_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
    	                eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_H263_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
    	                eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_MPEG2_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
#ifdef VIDDEC_SPARK_CODE
                    else if (VIDDEC_SPARKCHECK) {
   	                    eError = RMProxy_NewSendCommand(pComponentPrivate->pHandle, RMProxy_FreeResource, OMX_MPEG4_Decode_COMPONENT, 0, VIDDEC_MEMUSAGE, NULL);
                        if (eError != OMX_ErrorNone) {
                             VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                        }
                    }
#endif
                    else {
                        eError = OMX_ErrorUnsupportedSetting;
                        goto EXIT;
                    }
                    pComponentPrivate->eRMProxyState = VidDec_RMPROXY_State_Load;
                }
                if(pComponentPrivate->eRMProxyState != VidDec_RMPROXY_State_Unload){
                    eError = RMProxy_DeinitalizeEx(OMX_COMPONENTTYPE_VIDEO);
                    if (eError != OMX_ErrorNone) {
                        VIDDECODER_EPRINT ("Error returned from destroy ResourceManagerProxy thread\n");
                    }
                    pComponentPrivate->eRMProxyState = VidDec_RMPROXY_State_Unload;
                }

/*#endif*/
#endif
                eError = OMX_ErrorInvalidState;
                pComponentPrivate->eState = OMX_StateInvalid;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorInvalidState,
                                                       OMX_TI_ErrorCritical,
                                                       "Invalid State");
                VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
                break;
            }
            else{
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->eState = OMX_StateInvalid;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorInvalidState,
                                                       OMX_TI_ErrorCritical,
                                                       "Incorrect State Transition");
                VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);

            }
            break;
        case OMX_StateWaitForResources:
            VIDDECODER_DPRINT("Transitioning to OMX_StateWaitForResources C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            if (pComponentPrivate->eState == OMX_StateWaitForResources) {
                eError = OMX_ErrorSameState;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorSameState,
                                                       OMX_TI_ErrorMinor,
                                                       NULL);
                VIDDECODER_EPRINT("Same State 0x%x\n", eError);
            }
            else if (pComponentPrivate->eState == OMX_StateLoaded) {
                /*add code to wait for resources*/
                eError = OMX_ErrorNone;
                pComponentPrivate->bIsPaused = 0;
                pComponentPrivate->eState = OMX_StateWaitForResources;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventCmdComplete,
                                                       OMX_CommandStateSet,
                                                       pComponentPrivate->eState,
                                                       NULL);
            }
            else {
                eError = OMX_ErrorIncorrectStateTransition;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorIncorrectStateTransition,
                                                       OMX_TI_ErrorMinor,
                                                       NULL);
                VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
            }

            break;

        case OMX_StateMax:
            VIDDECODER_DPRINT("Transitioning to OMX_StateMax C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            eError = OMX_ErrorIncorrectStateTransition;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   OMX_ErrorIncorrectStateTransition,
                                                   OMX_TI_ErrorMinor,
                                                   NULL);
            VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
            break;
        default:
            VIDDECODER_DPRINT("Transitioning to default C 0x%x N 0x%x\n",pComponentPrivate->eState, nParam1);
            eError = OMX_ErrorIncorrectStateTransition;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   OMX_ErrorIncorrectStateTransition,
                                                   OMX_TI_ErrorMinor,
                                                   NULL);
            VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
            break;
    } /* End of Switch */



EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/******************************************************************************/
/**
  *  Sets free output buffers
  **/
/******************************************************************************/

OMX_ERRORTYPE VIDDEC_HandleFreeOutputBufferFromApp(VIDDEC_COMPONENT_PRIVATE *pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pBuffHead;
    OMX_U32 size_out_buf;
    int ret;
    LCML_DSP_INTERFACE* pLcmlHandle;
    VIDDEC_BUFFER_PRIVATE* pBufferPrivate = NULL;

    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x\n", (int*)pComponentPrivate);
    size_out_buf = (OMX_U32)pComponentPrivate->pOutPortDef->nBufferSize;
    pLcmlHandle = (LCML_DSP_INTERFACE*)(pComponentPrivate->pLCML);
    ret = read(pComponentPrivate->free_outBuf_Q[0], &pBuffHead, sizeof(pBuffHead));


    if (ret == -1) {
        VIDDECODER_EPRINT ("%d :: Error while reading from the pipe\n",__LINE__);
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
    VIDDEC_BUFFERPRINT("pBuffHead 0x%x eExecuteToIdle 0x%x\n", pBuffHead, pComponentPrivate->eExecuteToIdle);

    if(pBuffHead->pOutputPortPrivate != NULL) {
        pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pOutputPortPrivate;
#ifdef VIDDEC_FLAGGED_EOS
        if (!pComponentPrivate->bUseFlaggedEos) {
#endif
            if((pComponentPrivate->bPlayCompleted == 0 && pComponentPrivate->bBuffFound == OMX_FALSE) ||
                (pComponentPrivate->bPlayCompleted == 1 && pComponentPrivate->bBuffFound == OMX_TRUE)) {
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL){
#ifdef KHRONOS_1_1
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                         pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                        MP4VD_GPP_SN_UALGOutputParams* pUalgOutParams = NULL;
                        pUalgOutParams = (MP4VD_GPP_SN_UALGOutputParams *)pBufferPrivate->pUalgParam;
                        if (pComponentPrivate->eMBErrorReport.bEnabled) {
                            pUalgOutParams->lMbErrorBufFlag = 1;
                        }
                        else {
                            pUalgOutParams->lMbErrorBufFlag = 0;
                        }
                    }
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        H264VDEC_UALGOutputParam* pUalgOutParams = NULL;
                        pUalgOutParams = (H264VDEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
                        if (pComponentPrivate->eMBErrorReport.bEnabled) {
                            pUalgOutParams->lMBErrStatFlag = 1;
                        }
                        else {
                            pUalgOutParams->lMBErrStatFlag = 0;
                        }
                     }
#endif
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_DSP;
                    VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
                    PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                      pBuffHead->pBuffer,
                                      pBuffHead->nFilledLen,
                                      PERF_ModuleCommonLayer);
#endif
                    eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                              EMMCodecOuputBuffer,
                                              pBuffHead->pBuffer,
                                              pBuffHead->nAllocLen,
                                              pBuffHead->nFilledLen,
                                              (OMX_U8*)(pBufferPrivate->pUalgParam),
                                              (OMX_S32)pBufferPrivate->nUalgParamSize,
                                              (OMX_U8*)pBuffHead);
                    if (eError != OMX_ErrorNone){
                        VIDDECODER_EPRINT("LCML_QueueBuffer 0x%x\n", eError);
                        eError = OMX_ErrorHardware;
                        goto EXIT;
                    }
                }
                else {
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
            }
            else {
                pComponentPrivate->bBuffFound = OMX_TRUE;
                if (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                        pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;
                        pBuffHead->nFilledLen = 0;
                        pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_TUNNELEDCOMP;
                        VIDDEC_BUFFERPRINT("tunnel eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
                        PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                          pBuffHead->pBuffer,
                                          pBuffHead->nFilledLen,
                                          PERF_ModuleLLMM);
#endif
                        VIDDEC_BUFFERPRINT("nFlags %x nFilledLen %x\n",(int)pBuffHead->nFlags,(int)pBuffHead->nFilledLen);
                        VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                        eError = OMX_EmptyThisBuffer(pComponentPrivate->pCompPort[1]->hTunnelComponent, pBuffHead);
                }
                else {
                    pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;
                    pBuffHead->nFilledLen = 0;
                    VIDDEC_BUFFERPRINT("nFlags %x nFilledLen %x\n\n",(int)pBuffHead->nFlags,(int)pBuffHead->nFilledLen);
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                    VIDDEC_BUFFERPRINT("standalone buffer eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
                    PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                      pBuffHead->pBuffer,
                                      pBuffHead->nFilledLen,
                                      PERF_ModuleHLMM);
#endif

                    VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventBufferFlag,
                                                           VIDDEC_OUTPUT_PORT,
                                                           OMX_BUFFERFLAG_EOS,
                                                           NULL);
                    pComponentPrivate->cbInfo.FillBufferDone(pComponentPrivate->pHandle,
                                                             pComponentPrivate->pHandle->pApplicationPrivate,
                                                             pBuffHead);

                }
            }
#ifdef VIDDEC_FLAGGED_EOS
        }
        else {
            if (pComponentPrivate->bPlayCompleted == 0 ||
                (pComponentPrivate->bBuffFound = OMX_TRUE && pComponentPrivate->bPlayCompleted == 1)) {
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL) {
#ifdef KHRONOS_1_1
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                         pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                        MP4VD_GPP_SN_UALGOutputParams* pUalgOutParams = NULL;
                        pUalgOutParams = (MP4VD_GPP_SN_UALGOutputParams *)pBufferPrivate->pUalgParam;
                        if (pComponentPrivate->eMBErrorReport.bEnabled) {
                            pUalgOutParams->lMbErrorBufFlag = 1;
                        }
                        else {
                            pUalgOutParams->lMbErrorBufFlag = 0;
                        }
                    }
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        H264VDEC_UALGOutputParam* pUalgOutParams = NULL;
                        pUalgOutParams = (H264VDEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
                        if (pComponentPrivate->eMBErrorReport.bEnabled) {
                            pUalgOutParams->lMBErrStatFlag = 1;
                        }
                        else {
                            pUalgOutParams->lMBErrStatFlag = 0;
                        }
                     }
#endif
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_DSP;
                    VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                    PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                      pBuffHead->pBuffer,
                                      pBuffHead->nFilledLen,
                                      PERF_ModuleCommonLayer);
    #endif
                    eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                              EMMCodecOuputBuffer,
                                              pBuffHead->pBuffer,
                                              pBuffHead->nAllocLen,
                                              pBuffHead->nFilledLen,
                                              (OMX_U8*)(pBufferPrivate->pUalgParam),
                                              (OMX_S32)pBufferPrivate->nUalgParamSize,
                                              (OMX_U8*)pBuffHead);
                    if (eError != OMX_ErrorNone){
                        VIDDECODER_EPRINT("LCML_QueueBuffer 0x%x\n", eError);
                        eError = OMX_ErrorHardware;
                        goto EXIT;
                    }
                }
                else {
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
            }
            else { /*if the last output buffer wans't marked then return the next input buffer*/
                pComponentPrivate->bBuffFound = OMX_TRUE;
                if (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                        pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;
                        pBuffHead->nFilledLen = 0;
                        pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_TUNNELEDCOMP;
                        VIDDEC_BUFFERPRINT("tunnel eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                  pBuffHead->pBuffer,
                                  pBuffHead->nFilledLen,
                                  PERF_ModuleLLMM);
    #endif
                        VIDDEC_BUFFERPRINT("nFlags %x nFilledLen %x\n",(int)pBuffHead->nFlags,(int)pBuffHead->nFilledLen);
                        VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                        eError = OMX_EmptyThisBuffer(pComponentPrivate->pCompPort[1]->hTunnelComponent, pBuffHead);
                }
                else {
                    pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;
                    pBuffHead->nFilledLen = 0;
                    VIDDEC_BUFFERPRINT("nFlags %x nFilledLen %x\n\n",(int)pBuffHead->nFlags,(int)pBuffHead->nFilledLen);
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                    VIDDEC_BUFFERPRINT("standalone buffer eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                    PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                      pBuffHead->pBuffer,
                                      pBuffHead->nFilledLen,
                                      PERF_ModuleHLMM);
    #endif

                    VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventBufferFlag,
                                                           VIDDEC_OUTPUT_PORT,
                                                           OMX_BUFFERFLAG_EOS,
                                                           NULL);
                    pComponentPrivate->cbInfo.FillBufferDone(pComponentPrivate->pHandle,
                                                             pComponentPrivate->pHandle->pApplicationPrivate,
                                                             pBuffHead);

                }
            }
        }
#endif
    }
    else {
        VIDDECODER_EPRINT("null elelemnt ***********************************\n");
    }

EXIT:
    VIDDEC_BUFFERPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

#ifdef VIDDEC_ACTIVATEPARSER
OMX_S32 GET_NUM_BIT_REQ(OMX_U32 num)
{
	OMX_S32 i;
	for ( i = 31; i >= 0; i--)
	{
		if (num & (0x1 << i) ) break;
	}
	return (i+1);
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/*  ==========================================================================*/
/*  func    VIDDEC_ParseVideo_WMV9_VC1                                        */
/*                                                                            */
/*  desc                                                                      */
/*  ==========================================================================*/
OMX_ERRORTYPE VIDDEC_ParseVideo_MPEG2( OMX_S32* nWidth, OMX_S32* nHeight, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32    nTempValue = 0;
    /*OMX_U8*    pTempValue = 0;*/
    /*OMX_U8*    pTempSize = 0;*/
    /*OMX_U32    nProfile = 0;*/
    /*OMX_U32    nLevel = 0;*/
    OMX_U32    nBitPosition = 0;
    OMX_U8*    pHeaderStream = (OMX_U8*)pBuffHead->pBuffer;
    OMX_BOOL   nStartFlag = OMX_FALSE;
    OMX_U32    nInBytePosition = 0;
    OMX_U32    nTotalInBytes = 0;
    OMX_U32    nNalUnitType = 0;

    nTotalInBytes = pBuffHead->nFilledLen;

    do{
        for (; (!nStartFlag) && (nInBytePosition < nTotalInBytes - 3); ) {
           if (VIDDEC_GetBits(&nBitPosition, 24, pHeaderStream, OMX_FALSE) != 0x000001) {
                nBitPosition += 8;
                nInBytePosition++;
           }
           else {
               nStartFlag = OMX_TRUE;
               nBitPosition += 24;
               nInBytePosition += 3;
           }
        }
        if (!nStartFlag) {
            eError = OMX_ErrorStreamCorrupt;
            goto EXIT;
        }
        nNalUnitType = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        nInBytePosition++;
        if (nNalUnitType != 0xB3) {
            nStartFlag = OMX_FALSE;
        }
    }while (nNalUnitType != 0xB3);

    if (nNalUnitType == 0xB3) {
        nTempValue = VIDDEC_GetBits(&nBitPosition, 12, pHeaderStream, OMX_TRUE);
        (*nWidth) = (nTempValue);
        nTempValue = VIDDEC_GetBits(&nBitPosition, 12, pHeaderStream, OMX_TRUE);
        (*nHeight) = (nTempValue);
        eError = OMX_ErrorNone;
    }

EXIT:
    return eError;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/*  ==========================================================================*/
/*  func    VIDDEC_ParseVideo_WMV9_VC1                                        */
/*                                                                            */
/*  desc                                                                      */
/*  ==========================================================================*/
OMX_ERRORTYPE VIDDEC_ParseVideo_WMV9_VC1( OMX_S32* nWidth, OMX_S32* nHeight, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32    nTempValue = 0;
    /*OMX_U8*    pTempValue = 0;*/
    /*OMX_U8*    pTempSize = 0;*/
    OMX_U32    nProfile = 0;
    OMX_U32    nLevel = 0;
    OMX_U32    nBitPosition = 0;
    OMX_U8*    pHeaderStream = (OMX_U8*)pBuffHead->pBuffer;
    OMX_BOOL   nStartFlag = OMX_FALSE;
    OMX_U32    nInBytePosition = 0;
    OMX_U32    nTotalInBytes = 0;
    OMX_U32    nNalUnitType = 0;

    nTotalInBytes = pBuffHead->nFilledLen;

    do{
        for (; (!nStartFlag) && (nInBytePosition < nTotalInBytes - 3); ) {
           if (VIDDEC_GetBits(&nBitPosition, 24, pHeaderStream, OMX_FALSE) != 0x000001) {
                nBitPosition += 8;
                nInBytePosition++;
           }
           else {
               nStartFlag = OMX_TRUE;
               nBitPosition += 24;
               nInBytePosition += 3;
           }
        }
        if (!nStartFlag) {
            eError = OMX_ErrorStreamCorrupt;
            goto EXIT;
        }
        nNalUnitType = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        nInBytePosition++;
        if (nNalUnitType != 0x0f && nNalUnitType != 0x0e) {
            nStartFlag = OMX_FALSE;
        }
    }while (nNalUnitType != 0x0f && nNalUnitType != 0x0e);

    if (nNalUnitType == 0x0f || nNalUnitType == 0x0e) {
        nProfile = VIDDEC_GetBits(&nBitPosition, 2, pHeaderStream, OMX_TRUE);
        nLevel = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
        nTempValue = VIDDEC_GetBits(&nBitPosition, 11, pHeaderStream, OMX_TRUE);
        nTempValue = VIDDEC_GetBits(&nBitPosition, 12, pHeaderStream, OMX_TRUE);
        (*nWidth) = (nTempValue * 2) + 2;
        nTempValue = VIDDEC_GetBits(&nBitPosition, 12, pHeaderStream, OMX_TRUE);
        (*nHeight) = (nTempValue * 2) + 2;
        eError = OMX_ErrorNone;
    }

EXIT:
    return eError;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/*  ==========================================================================*/
/*  func    VIDDEC_ParseVideo_WMV9_RCV                                        */
/*                                                                            */
/*  desc                                                                      */
/*  ==========================================================================*/
OMX_ERRORTYPE VIDDEC_ParseVideo_WMV9_RCV( OMX_S32* nWidth, OMX_S32* nHeight, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32    nTempValue = 0;
    OMX_U8*    pTempValue = 0;
    /*OMX_U8*    pTempSize = 0;*/
    OMX_U32    Profile = 0;
    /*OMX_U32    i = 0;*/
    OMX_U32    nBitPosition = 0;
    OMX_U8*    pHeaderStream = (OMX_U8*)pBuffHead->pBuffer;

    if (pBuffHead->nFilledLen >= 20) {
        nTempValue = VIDDEC_GetBits(&nBitPosition, 32, pHeaderStream, OMX_TRUE);
        nTempValue = VIDDEC_GetBits(&nBitPosition, 32, pHeaderStream, OMX_TRUE);
        Profile = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);
        nTempValue = VIDDEC_GetBits(&nBitPosition, 28, pHeaderStream, OMX_TRUE);

        pTempValue = (OMX_U8*)&nTempValue;
        pTempValue[0] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[1] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[2] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[3] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        (*nHeight) = nTempValue;

        pTempValue[0] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[1] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[2] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        pTempValue[3] = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);
        (*nWidth) = nTempValue;
        eError = OMX_ErrorNone;
    }
    else {
        (*nWidth) = 0;
        (*nHeight) = 0;
        eError = OMX_ErrorUndefined;
    }

    return eError;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/*  ==========================================================================*/
/*  func    VIDDEC_ParseVideo_MPEG4                                             */
/*                                                                            */
/*  desc                                                                      */
/*  ==========================================================================*/
OMX_ERRORTYPE VIDDEC_ParseVideo_MPEG4( OMX_S32* nWidth, OMX_S32* nHeight, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_ERRORTYPE eError = OMX_ErrorUndefined;
    OMX_U32    nSartCode = 0;
    OMX_U32    nBitPosition = 0;
    OMX_BOOL   bHeaderParseCompleted = OMX_FALSE;
    OMX_BOOL   bFillHeaderInfo = OMX_FALSE;
    OMX_U8* pHeaderStream = (OMX_U8*)pBuffHead->pBuffer;

    /*OMX_U32 nTotalInBytes = pBuffHead->nFilledLen;*/
    VIDDEC_MPEG4_ParserParam MPEG4_Param;
    VIDDEC_MPEG4UncompressedVideoFormat iOutputFormat;
    VIDDEC_MPEG4_ParserParam* sMPEG4_Param = &MPEG4_Param;
    VIDDEC_VideoPictureHeader sPictHeaderDummy;
    VIDDEC_MPEG4VisualVOLHeader sVolHeaderDummy;
    VIDDEC_VideoPictureHeader* pPictHeaderPtr = &sPictHeaderDummy;
    VIDDEC_MPEG4VisualVOLHeader* sVolHeaderPtr = &sVolHeaderDummy;

    pPictHeaderPtr->cnOptional = (OMX_U8*)malloc( sizeof(VIDDEC_MPEG4VisualVOLHeader));
    while (!bHeaderParseCompleted)
    {
        nSartCode = VIDDEC_GetBits(&nBitPosition, 32, pHeaderStream, OMX_TRUE);
        if (nSartCode == 0x1B0)
        {
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
            pPictHeaderPtr->nProfile = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
            pPictHeaderPtr->nLevel = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);
        }
        else if (nSartCode == 0x1B5)
        {
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nIsVisualObjectIdentifier = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
            if (sMPEG4_Param->nIsVisualObjectIdentifier)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 7);
                (void)VIDDEC_GetBits(&nBitPosition, 7, pHeaderStream, OMX_TRUE); /* DISCARD THIS INFO (7 bits)*/
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
            sMPEG4_Param->nVisualObjectType = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);
            if (sMPEG4_Param->nVisualObjectType== 1|| sMPEG4_Param->nVisualObjectType== 2)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                sMPEG4_Param->nVideoSignalType = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                if (sMPEG4_Param->nVideoSignalType)
                {
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 3);
                    sMPEG4_Param->nVideoFormat = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    sMPEG4_Param->nVideoRange = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    sMPEG4_Param->nColorDescription = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                    if (sMPEG4_Param->nColorDescription)
                    {
                        /*Discard this info*/
                        OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 24);
                        (void)VIDDEC_GetBits(&nBitPosition, 24, pHeaderStream, OMX_TRUE);
                    }
                }
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->NBitZero = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
            while ((nBitPosition%8)!= 0) {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);/*discard align bits*/
            }
        }
        else if ((nSartCode >= 0x100)&&(nSartCode <= 0x11F))
        {
            /*Do nothing*/
            /*    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 8);
                  (void)VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);*/
        }
        else if (nSartCode == 0x1B3) /*GOV*/
        {
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 20);
            (void)VIDDEC_GetBits(&nBitPosition, 20, pHeaderStream, OMX_TRUE);
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->NBitZero = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
            while ((nBitPosition%8)!= 0){
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);/*discard align bits*/
            }
        }
        else if (nSartCode == 0x1B2) /*user data*/
        {
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 24);
            while (VIDDEC_GetBits(&nBitPosition, 24, pHeaderStream, OMX_TRUE)!= 0x1)
                nBitPosition-=16;        /*discard only 8 bits and try againg until*/
                                        /*the next start code is found*/
            nBitPosition -=24;            /* prepare to read the entire start code*/
        /*    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->NBitZero = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
            PRINT("sMPEG4_Param->NBitZero = %d", sMPEG4_Param->NBitZero);
            while ((nBitPosition%8)!= 0) {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);*//*discard align bits*/
            /*}*/
        }
        else if ((nSartCode >= 0x120)&&(nSartCode <= 0x12F))
        {
            sVolHeaderPtr->nVideoObjectLayerId = nSartCode&0x0000000f;
            sVolHeaderPtr->bShortVideoHeader = 0;
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            pPictHeaderPtr->bIsRandomAccessible = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);    /*1 bit*/
            sVolHeaderPtr->bRandomAccessibleVOL = pPictHeaderPtr->bIsRandomAccessible;
            if (pPictHeaderPtr->bIsRandomAccessible)
            {
                /* it seems this never happens*/
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 8);
            sMPEG4_Param->nVideoObjectTypeIndication = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);    /* 8 bits*/
            sVolHeaderPtr->nVideoObjectTypeIndication = sMPEG4_Param->nVideoObjectTypeIndication;
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nIsVisualObjectLayerIdentifier = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);/*1 bit*/
            sVolHeaderPtr->nVideoObjectLayerId = sMPEG4_Param->nIsVisualObjectLayerIdentifier;
            sMPEG4_Param->nLayerVerId = 0;
            if (sMPEG4_Param->nIsVisualObjectLayerIdentifier)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
                sMPEG4_Param->nLayerVerId = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);                        /*4 bits*/
                sVolHeaderPtr->nVideoObjectLayerVerId = sMPEG4_Param->nLayerVerId;
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 3);
                sMPEG4_Param->nLayerPriority = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);            /*3 bits*/
                sVolHeaderPtr->nVideoObjectLayerPriority = sMPEG4_Param->nLayerPriority;
            }

            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
            sMPEG4_Param->nAspectRadio = VIDDEC_GetBits(&nBitPosition, 4, pHeaderStream, OMX_TRUE);                    /*4 bits*/
            if (sMPEG4_Param->nAspectRadio == 0xf)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 8);
                sMPEG4_Param->nParWidth = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);                    /*8 bits*/
                sVolHeaderPtr->nAspectRatioNum = sMPEG4_Param->nParWidth;
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 8);
                sMPEG4_Param->nParHeight = VIDDEC_GetBits(&nBitPosition, 8, pHeaderStream, OMX_TRUE);                /*8 bits*/
                sVolHeaderPtr->nAspectRatioDenom = sMPEG4_Param->nParHeight;
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nControlParameters = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);            /*1 bit*/
            if ( sMPEG4_Param->nControlParameters )
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 2);
                sMPEG4_Param->nChromaFormat = VIDDEC_GetBits(&nBitPosition, 2, pHeaderStream, OMX_TRUE);                /*2 bits*/
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                sMPEG4_Param->nLowDelay = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                    /*1 bit*/
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                sMPEG4_Param->nVbvParameters = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);            /*1 bit*/
                if (sMPEG4_Param->nVbvParameters)
                {
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 15);
                    sMPEG4_Param->nBitRate = VIDDEC_GetBits(&nBitPosition, 15, pHeaderStream, OMX_TRUE)<<15;                /*15 bit*/
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                        /*1 bit*/
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 15);
                    sMPEG4_Param->nBitRate |= VIDDEC_GetBits(&nBitPosition, 15, pHeaderStream, OMX_TRUE);                    /*15 bit*/
                    sVolHeaderPtr->sVbvParams.nBitRate = sMPEG4_Param->nBitRate;
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 15);
                    sMPEG4_Param->nFirstHalfVbvBufferSize = VIDDEC_GetBits(&nBitPosition, 15, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 3);
                    sMPEG4_Param->nLatterHalfVbvBufferSize = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
                    sVolHeaderPtr->sVbvParams.nVbvBufferSize =
                        (((sMPEG4_Param->nFirstHalfVbvBufferSize) << 3) + sMPEG4_Param->nLatterHalfVbvBufferSize) * 2048;
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 11);
                    sMPEG4_Param->nFirstHalfVbvOccupancy = VIDDEC_GetBits(&nBitPosition, 11, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 15);
                    sMPEG4_Param->nLatterHalfVbvOccupancy = VIDDEC_GetBits(&nBitPosition, 15, pHeaderStream, OMX_TRUE);
                    sVolHeaderPtr->sVbvParams.nVbvOccupancy =
                        (((sMPEG4_Param->nFirstHalfVbvOccupancy) << 15) + sMPEG4_Param->nLatterHalfVbvOccupancy) * 2048;
                    OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                    (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);

                    /*OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 48);
                    (void)VIDDEC_GetBits(&nBitPosition, 48, pHeaderStream, OMX_TRUE);*/
                }
                else
                {
                    sMPEG4_Param->nBitRate = 0;
                }
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 2);
            sMPEG4_Param->nLayerShape = VIDDEC_GetBits(&nBitPosition, 2, pHeaderStream, OMX_TRUE);                    /*2 bits*/
            /*skip one marker_bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                                /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 16);
            sMPEG4_Param->nTimeIncrementResolution = VIDDEC_GetBits(&nBitPosition, 16, pHeaderStream, OMX_TRUE);        /*16 bits*/
            sVolHeaderPtr->nVOPTimeIncrementResolution = sMPEG4_Param->nTimeIncrementResolution;
            /*skip one market bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                                /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nFnXedVopRate = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                    /*1 bit*/
            sVolHeaderPtr->bnFnXedVopRate = sMPEG4_Param->nFnXedVopRate;
            if (sMPEG4_Param->nFnXedVopRate)
            {
                sMPEG4_Param->nNum_bits = GET_NUM_BIT_REQ (sMPEG4_Param->nTimeIncrementResolution);
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, sMPEG4_Param->nNum_bits);
                sVolHeaderPtr->nFnXedVOPTimeIncrement = VIDDEC_GetBits (&nBitPosition, sMPEG4_Param->nNum_bits, pHeaderStream, OMX_TRUE);
            }
            /*skip one market bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                                /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 13);
            (*nWidth) = VIDDEC_GetBits(&nBitPosition, 13, pHeaderStream, OMX_TRUE);                        /*13 bits*/
            /*skip one market bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                                /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 13);
            (*nHeight) = VIDDEC_GetBits(&nBitPosition, 13, pHeaderStream, OMX_TRUE);                        /*13 bits*/

            /*skip one market bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                                /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nInterlaced = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                    /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nObmc = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                            /*1 bit*/
            if (sMPEG4_Param->nLayerVerId)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                sMPEG4_Param->NSpriteNotSupported = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);        /*1 bit*/
                if (sMPEG4_Param->NSpriteNotSupported)
                {
                }
            }
            else
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 2);
                sMPEG4_Param->NSpriteNotSupported = VIDDEC_GetBits(&nBitPosition, 2, pHeaderStream, OMX_TRUE);        /*2 bits*/
                if (sMPEG4_Param->NSpriteNotSupported)
                {
                }
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nNot8Bit = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                        /*1 bits*/
            sMPEG4_Param->nQuantPrecision = 5;
            sMPEG4_Param->nBitsPerPnXel = 8;
            if (sMPEG4_Param->nNot8Bit)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
                sMPEG4_Param->nQuantPrecision = VIDDEC_GetBits(&nBitPosition,4, pHeaderStream, OMX_TRUE);                    /* 4 bits*/
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 4);
            sMPEG4_Param->nBitsPerPnXel = VIDDEC_GetBits(&nBitPosition,4, pHeaderStream, OMX_TRUE);                    /* 4 bits*/
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nIsInverseQuantMethodFirst = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);    /*1 bits*/
            if (sMPEG4_Param->nLayerVerId !=1)
            {
                /*does not support quater sample*/
                /*kip one market bit*/
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                (void)VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                            /*1 bit*/
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nComplexityEstimationDisable = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);    /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nIsResyncMarkerDisabled = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);        /*1 bit*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nIsDataPartitioned = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);            /*1 bit*/
            sVolHeaderPtr->bDataPartitioning = sMPEG4_Param->nIsDataPartitioned;
            if (sMPEG4_Param->nIsDataPartitioned)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
                sMPEG4_Param->nRvlc = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                        /*1 bit*/
                sVolHeaderPtr->bReversibleVLC = sMPEG4_Param->nRvlc;
                if (sMPEG4_Param->nRvlc)
                {
                }
            }
            if (sMPEG4_Param->nLayerVerId !=1)
            {
                OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 2);
                (void)VIDDEC_GetBits(&nBitPosition, 2, pHeaderStream, OMX_TRUE);                            /*2 bit*/
            }
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 1);
            sMPEG4_Param->nScalability = VIDDEC_GetBits(&nBitPosition, 1, pHeaderStream, OMX_TRUE);                    /*1 bit*/
            /*pPictHeaderPtr->sSizeInMemory.nWidth              = (*nWidth);
            pPictHeaderPtr->sSizeInMemory.nHeight             = (*nHeight);
            pPictHeaderPtr->sDisplayedRect                    = TRect(TSize((*nWidth),(*nHeight)));*/
            if (iOutputFormat.iYuvFormat.iPattern == 0x00000001)
                pPictHeaderPtr->nPostDecoderBufferSize    = (*nHeight) * (*nWidth) * 3 / 2;    /*YUV 420 Planar*/
            else if (iOutputFormat.iYuvFormat.iPattern == 0x00000008)
                pPictHeaderPtr->nPostDecoderBufferSize    = (*nHeight) * (*nWidth) * 2;    /*YUV 422 Interleaved*/
            pPictHeaderPtr->nOptions |= 0x00000008;
            if(bFillHeaderInfo)
            {
                ;
            }
            bHeaderParseCompleted = OMX_TRUE;
            eError = OMX_ErrorNone;
        }
        else if ( (nSartCode&0xfffffc00) == 0x00008000 )
        {
            sVolHeaderPtr->bShortVideoHeader = 1;
            /* discard 3 bits for split_screen_indicator, document_camera_indicator*/
            /* and full_picture_freeze_release*/
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 3);
            (void)VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
            OMX_PARSER_CHECKLIMIT(nTotalInBytes, nBitPosition, 3);
            sMPEG4_Param->nSourceFormat = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
            if (sMPEG4_Param->nSourceFormat == 0x1)
            {
                (*nWidth) = 128;
                (*nHeight) = 96;
            }
            else if (sMPEG4_Param->nSourceFormat == 0x2)
            {
                (*nWidth) = 176;
                (*nHeight) = 144;
            }
            else if (sMPEG4_Param->nSourceFormat == 0x3)
            {
                (*nWidth) = 352;
                (*nHeight) = 288;
            }
            else if (sMPEG4_Param->nSourceFormat == 0x4)
            {
                (*nWidth) = 704;
                (*nHeight) = 576;
            }
            else if (sMPEG4_Param->nSourceFormat == 0x5)
            {
                (*nWidth) = 1408;
                (*nHeight) = 1152;
            }
            else if (sMPEG4_Param->nSourceFormat == 0x7)
            {
                sMPEG4_Param->nUFEP = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
                if(sMPEG4_Param->nUFEP == 1) {
                    sMPEG4_Param->nSourceFormat = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
                    if (sMPEG4_Param->nSourceFormat == 0x1)
                    {
                        (*nWidth) = 128;
                        (*nHeight) = 96;
                    }
                    else if (sMPEG4_Param->nSourceFormat == 0x2)
                    {
                        (*nWidth) = 176;
                        (*nHeight) = 144;
                    }
                    else if (sMPEG4_Param->nSourceFormat == 0x3)
                    {
                        (*nWidth) = 352;
                        (*nHeight) = 288;
                    }
                    else if (sMPEG4_Param->nSourceFormat == 0x4)
                    {

                        (*nWidth) = 704;
                        (*nHeight) = 576;
                    }
                    else if (sMPEG4_Param->nSourceFormat == 0x5)
                    {
                        (*nWidth) = 1408;
                        (*nHeight) = 1152;
                    }
                    else if (sMPEG4_Param->nSourceFormat == 0x7)
                    {
                        sMPEG4_Param->nSourceFormat = VIDDEC_GetBits(&nBitPosition, 3, pHeaderStream, OMX_TRUE);
                        (*nWidth) = 1408;
                        (*nHeight) = 1152;
                    }
                    else
                    {
                        eError = OMX_ErrorUnsupportedSetting;
                        goto EXIT;
                    }
                }
            }
            else
            {
                eError = OMX_ErrorUnsupportedSetting;
                goto EXIT;
            }
            bHeaderParseCompleted = OMX_TRUE;
            eError = OMX_ErrorNone;
        }
        else
        {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }
EXIT:
    if(pPictHeaderPtr->cnOptional != NULL)
    {
            free( pPictHeaderPtr->cnOptional);
            pPictHeaderPtr->cnOptional = NULL;
    }
    return eError;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
OMX_ERRORTYPE VIDDEC_ParseVideo_H264(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate,
                                     OMX_BUFFERHEADERTYPE* pBuffHead,OMX_S32* nWidth,
                                     OMX_S32* nHeight, OMX_S32* nCropWidth,
                                     OMX_S32* nCropHeight, OMX_U32 nType)
{
    OMX_ERRORTYPE eError = OMX_ErrorBadParameter;
    OMX_S32 i = 0;
    VIDDEC_AVC_ParserParam* sParserParam;
    /*OMX_S32 nRetVal = 0;*/
    OMX_BOOL nStartFlag = OMX_FALSE;
    OMX_U32 nBitPosition = 0;
    OMX_U32 nRbspPosition = 0;
    OMX_U32 nTotalInBytes = 0;
    OMX_U32 nInBytePosition = 0;
    OMX_U32 nInPositionTemp = 0;
    OMX_U32 nNumOfBytesInRbsp = 0;
    OMX_S32 nNumBytesInNALunit = 0;
    OMX_U8* nBitStream = 0;
    OMX_U32 nNalUnitType = 0;
    OMX_U8* nRbspByte = NULL;

    OMX_U8 *pDataBuf;

    nTotalInBytes = pBuffHead->nFilledLen;
    nBitStream = (OMX_U8*)pBuffHead->pBuffer;/* + (OMX_U8*)pBuffHead->nOffset;*/
    nRbspByte = (OMX_U8*)malloc(nTotalInBytes);
    memset(nRbspByte, 0x0, nTotalInBytes);
    sParserParam = (VIDDEC_AVC_ParserParam *)malloc(sizeof(VIDDEC_AVC_ParserParam));

    if (nType == 0) {
        do{
            for (; (!nStartFlag) && (nInBytePosition < nTotalInBytes - 3); )
            {
               if (VIDDEC_GetBits(&nBitPosition, 24, nBitStream, OMX_FALSE) != 0x000001)
               {
                    nBitPosition += 8;
                    nInBytePosition++;
               }
               else
               {
                   /*Start Code found*/
                   nStartFlag = OMX_TRUE;
                   nBitPosition += 24;
                   nInBytePosition += 3;
               }
            }
            nStartFlag = OMX_FALSE;
            /* offset to NumBytesInNALunit*/
            nNumBytesInNALunit = nInBytePosition;
            sParserParam->nBitPosTemp = nBitPosition;
              for (;(!nStartFlag)&&(nNumBytesInNALunit < nTotalInBytes-3); )
            {
                if (VIDDEC_GetBits(&sParserParam->nBitPosTemp, 24, nBitStream, OMX_FALSE) != 0x000001)
                /*find start code*/
                {
                    sParserParam->nBitPosTemp += 8;
                    nNumBytesInNALunit++;
                }
                else
                {
                   /*Start Code found*/
                    nStartFlag = OMX_TRUE;
                    sParserParam->nBitPosTemp += 24;
                    nNumBytesInNALunit += 3;
                }
            }

            if (!nStartFlag)
            {
                eError = OMX_ErrorStreamCorrupt;
                goto EXIT;
            }
            /* forbidden_zero_bit */
            sParserParam->nForbiddenZeroBit = VIDDEC_GetBits(&nBitPosition, 1, nBitStream, OMX_TRUE);
            /* nal_ref_idc */
            sParserParam->nNalRefIdc = VIDDEC_GetBits(&nBitPosition, 2, nBitStream, OMX_TRUE);
            /* nal_unit_type */
            nNalUnitType = VIDDEC_GetBits(&nBitPosition, 5, nBitStream, OMX_TRUE);
            nInBytePosition++;

            /* This code is to ensure we will get parameter info */
            if (nNalUnitType != 7)
            {
                VIDDEC_BUFFERPRINT("nal_unit_type does not specify parameter information need to look for next startcode");
                nStartFlag = OMX_FALSE;
            }
        }while (nNalUnitType != 7);
    }
    else {
         pDataBuf = (OMX_U8*)nBitStream;
         do {
            if (pComponentPrivate->H264BitStreamFormat == 1)
                if (pComponentPrivate->bIsNALBigEndian) {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition];
                }
                else {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition];
                }
            else if (pComponentPrivate->H264BitStreamFormat == 2)
                if (pComponentPrivate->bIsNALBigEndian) {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition] << 8 | pDataBuf[nInBytePosition+1];
                }
                else {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition] << 0 | pDataBuf[nInBytePosition+1] << 8 ;
                }
            else if (pComponentPrivate->H264BitStreamFormat == 4){
                if (pComponentPrivate->bIsNALBigEndian) {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition]<<24 | pDataBuf[nInBytePosition+1] << 16 | pDataBuf[nInBytePosition+2] << 8 | pDataBuf[nInBytePosition+3];
                }
                else {
                    nNumBytesInNALunit = (OMX_U32)pDataBuf[nInBytePosition]<<0 | pDataBuf[nInBytePosition+1] << 8 | pDataBuf[nInBytePosition+2] << 16 | pDataBuf[nInBytePosition+3]<<24;
                }
            }
            else {
                eError = OMX_ErrorBadParameter;
                goto EXIT;
            }
            nBitPosition = (nInPositionTemp + nType) * 8;
            nInBytePosition = nInPositionTemp + nType;
            nInPositionTemp += nNumBytesInNALunit + nType;
            if (nInBytePosition > nTotalInBytes) {
                eError = OMX_ErrorBadParameter;
                goto EXIT;
            }
            /* forbidden_zero_bit */
            sParserParam->nForbiddenZeroBit = VIDDEC_GetBits(&nBitPosition, 1, nBitStream, OMX_TRUE);
            /* nal_ref_idc */
            sParserParam->nNalRefIdc = VIDDEC_GetBits(&nBitPosition, 2, nBitStream, OMX_TRUE);
            /* nal_unit_type */
            nNalUnitType = VIDDEC_GetBits(&nBitPosition, 5, nBitStream, OMX_TRUE);
            nInBytePosition++;
            /* This code is to ensure we will get parameter info */
            if (nNalUnitType != 7) {
                /*nBitPosition += (nNumBytesInNALunit - 1) * 8;
                nInBytePosition += (nNumBytesInNALunit - 1);*/
                nBitPosition = (nInPositionTemp) * 8;
                nInBytePosition = (nInPositionTemp);

            }
        } while (nNalUnitType != 7);
        nNumBytesInNALunit += 8 + nInBytePosition;/*sum to keep the code flow*/
                                /*the buffer must had enough space to enter this number*/
    }
    for (i=0; nInBytePosition < nNumBytesInNALunit - 3; )
    {

        if (((nInBytePosition + 2) < nNumBytesInNALunit - 3)&&
            (VIDDEC_GetBits(&nBitPosition, 24, nBitStream, OMX_FALSE) == 0x000003))
        {
            VIDDEC_BUFFERPRINT("discard emulation prev byte");
            nRbspByte[i++] = nBitStream[nInBytePosition++];
            nRbspByte[i++] = nBitStream[nInBytePosition++];
            nNumOfBytesInRbsp += 2;
            /* discard emulation prev byte */
            nInBytePosition++;
            nBitPosition += 24;
        }
        else
        {
            nRbspByte[i++] = nBitStream[nInBytePosition++];
            nNumOfBytesInRbsp++;
            nBitPosition += 8;
        }
    }


    /*Parse RBSP sequence*/
    /*///////////////////*/
    /*  profile_idc u(8) */
    sParserParam->nProfileIdc = VIDDEC_GetBits(&nRbspPosition, 8, nRbspByte, OMX_TRUE);
    /* constraint_set0_flag u(1)*/
    sParserParam->nConstraintSet0Flag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    /* constraint_set1_flag u(1)*/
    sParserParam->nConstraintSet1Flag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    /* constraint_set2_flag u(1)*/
    sParserParam->nConstraintSet2Flag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    /* reserved_zero_5bits u(5)*/
    sParserParam->nReservedZero5bits = VIDDEC_GetBits(&nRbspPosition, 5, nRbspByte, OMX_TRUE);
    /* level_idc*/
    sParserParam->nLevelIdc = VIDDEC_GetBits(&nRbspPosition, 8, nRbspByte, OMX_TRUE);
    sParserParam->nSeqParameterSetId = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    sParserParam->nLog2MaxFrameNumMinus4 = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    sParserParam->nPicOrderCntType = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);

    if ( sParserParam->nPicOrderCntType == 0 )
    {
        sParserParam->nLog2MaxPicOrderCntLsbMinus4 = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    }
    else if( sParserParam->nPicOrderCntType == 1 )
    {
        /* delta_pic_order_always_zero_flag*/
        VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
        sParserParam->nOffsetForNonRefPic = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        if (sParserParam->nOffsetForNonRefPic > 1)
              sParserParam->nOffsetForNonRefPic = sParserParam->nOffsetForNonRefPic & 0x1 ?
                                                sParserParam->nOffsetForNonRefPic >> 1 :
                                              -(sParserParam->nOffsetForNonRefPic >> 1);
        sParserParam->nOffsetForTopToBottomField = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        sParserParam->nNumRefFramesInPicOrderCntCycle = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        for(i = 0; i < sParserParam->nNumRefFramesInPicOrderCntCycle; i++ )
            VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte); /*offset_for_ref_frame[i]*/
    }

    sParserParam->nNumRefFrames = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    sParserParam->nGapsInFrameNumValueAllowedFlag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    sParserParam->nPicWidthInMbsMinus1 = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    (*nWidth) = (sParserParam->nPicWidthInMbsMinus1 + 1) * 16;
    sParserParam->nPicHeightInMapUnitsMinus1 = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
    (*nHeight) = (sParserParam->nPicHeightInMapUnitsMinus1 + 1) * 16;
    /* Checking for cropping in picture saze */
    /* getting frame_mbs_only_flag */
    sParserParam->nFrameMbsOnlyFlag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    if (!sParserParam->nFrameMbsOnlyFlag)
    {
        sParserParam->nMBAdaptiveFrameFieldFlag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    }
    /*getting direct_8x8_inference_flag and frame_cropping_flag*/
    sParserParam->nDirect8x8InferenceFlag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    sParserParam->nFrameCroppingFlag = VIDDEC_GetBits(&nRbspPosition, 1, nRbspByte, OMX_TRUE);
    /*getting the crop values if exist*/
    if (sParserParam->nFrameCroppingFlag)
    {
        sParserParam->nFrameCropLeftOffset = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        sParserParam->nFrameCropRightOffset = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        sParserParam->nFrameCropTopOffset = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        sParserParam->nFrameCropBottomOffset = VIDDEC_UVLC_dec(&nRbspPosition, nRbspByte);
        /* Update framesize taking into account the cropping values */
        (*nCropWidth) = (2 * sParserParam->nFrameCropLeftOffset + 2 * sParserParam->nFrameCropRightOffset);
        (*nCropHeight) = (2 * sParserParam->nFrameCropTopOffset + 2 * sParserParam->nFrameCropBottomOffset);
    }
    eError = OMX_ErrorNone;

EXIT:
    free( nRbspByte);
    free( sParserParam);
    return eError;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/*  =========================================================================*/
/*  func    GetBits                                                          */
/*                                                                           */
/*  desc    Gets aBits number of bits from position aPosition of one buffer  */
/*            and returns the value in a TUint value.                        */
/*  =========================================================================*/
OMX_U32 VIDDEC_GetBits(OMX_U32* nPosition, OMX_U8 nBits, OMX_U8* pBuffer, OMX_BOOL bIcreasePosition)
{
    OMX_U32 nOutput;
    OMX_U32 nNumBitsRead = 0;
    OMX_U32 nBytePosition = 0;
    OMX_U8  nBitPosition =  0;
    nBytePosition = *nPosition / 8;
    nBitPosition =  *nPosition % 8;

    if (bIcreasePosition)
        *nPosition += nBits;
    nOutput = ((OMX_U32)pBuffer[nBytePosition] << (24+nBitPosition) );
    nNumBitsRead = nNumBitsRead + (8 - nBitPosition);
    if (nNumBitsRead < nBits)
    {
        nOutput = nOutput | ( pBuffer[nBytePosition + 1] << (16+nBitPosition));
        nNumBitsRead = nNumBitsRead + 8;
    }
    if (nNumBitsRead < nBits)
    {
        nOutput = nOutput | ( pBuffer[nBytePosition + 2] << (8+nBitPosition));
        nNumBitsRead = nNumBitsRead + 8;
    }
    if (nNumBitsRead < nBits)
    {
        nOutput = nOutput | ( pBuffer[nBytePosition + 3] << (nBitPosition));
        nNumBitsRead = nNumBitsRead + 8;
    }
    nOutput = nOutput >> (32 - nBits) ;
    return nOutput;
}


OMX_S32 VIDDEC_UVLC_dec(OMX_U32 *nPosition, OMX_U8* pBuffer)
{

    OMX_U32 nBytePosition = (*nPosition) / 8;
    OMX_U8 cBitPosition =  (*nPosition) % 8;
    OMX_U32 nLen = 1;
    OMX_U32 nCtrBit = 0;
    OMX_U32 nVal = 1;
    OMX_S32 nInfoBit=0;

    nCtrBit = pBuffer[nBytePosition] & (0x1 << (7-cBitPosition));
    while (nCtrBit==0)
    {
        nLen++;
        cBitPosition++;
        (*nPosition)++;
        if (!(cBitPosition%8))
        {
            cBitPosition=0;
            nBytePosition++;
        }
        nCtrBit = pBuffer[nBytePosition] & (0x1<<(7-cBitPosition));
    }
    for(nInfoBit=0; (nInfoBit<(nLen-1)); nInfoBit++)
    {
        cBitPosition++;
        (*nPosition)++;

        if (!(cBitPosition%8))
        {
            cBitPosition=0;
            nBytePosition++;
        }
        nVal=(nVal << 1);
        if(pBuffer[nBytePosition] & (0x01 << (7 - cBitPosition)))
            nVal |= 1;
    }
    (*nPosition)++;
    nVal -= 1;
    return nVal;
}
#endif

#ifdef VIDDEC_ACTIVATEPARSER
/* ========================================================================== */
/**
  *  Parse the input buffer to get the correct width and height
  **/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_ParseHeader(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_ERRORTYPE eError = OMX_ErrorBadParameter;
    OMX_S32 nWidth = 0;
    OMX_S32 nHeight = 0;
    OMX_S32 nCropWidth = 0;
    OMX_S32 nCropHeight = 0;

    OMX_U32 nOutMinBufferSize = 0;
	OMX_BOOL bInPortSettingsChanged = OMX_FALSE;
	OMX_BOOL bOutPortSettingsChanged = OMX_FALSE;

    if(!pComponentPrivate) {
        goto EXIT;
    }

    if( (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) ||
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV ||
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2 ||
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {

        if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
            eError = VIDDEC_ParseVideo_H264( pComponentPrivate, pBuffHead, &nWidth, &nHeight,
                &nCropWidth, &nCropHeight, pComponentPrivate->H264BitStreamFormat);
            VIDDEC_BUFFERPRINT(" %x C(%d-%d) I(%d-%d) - %x \n", (unsigned int)pBuffHead, (unsigned int)nWidth,
                (unsigned int)nHeight, (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameWidth,
                (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameHeight,eError);
        }
        else if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4  ||
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
            if(!( pComponentPrivate->pInPortDef->format.video.nFrameWidth > 730 ||
                pComponentPrivate->pInPortDef->format.video.nFrameHeight > 730)) {
                VIDDEC_ParseVideo_MPEG4( &nWidth, &nHeight, pBuffHead);
            }
            else {
                nWidth = pComponentPrivate->pInPortDef->format.video.nFrameWidth;
                nHeight = pComponentPrivate->pInPortDef->format.video.nFrameHeight;
                if (pComponentPrivate->pInPortDef->format.video.nFrameWidth == 864) {
                    pComponentPrivate->pInPortDef->format.video.nFrameWidth = 854;
                    nWidth = 854;
                }
                if (pComponentPrivate->pInPortDef->format.video.nFrameHeight == 864) {
                    pComponentPrivate->pInPortDef->format.video.nFrameHeight = 854;
                    nHeight = 854;
                }
            }
            VIDDEC_BUFFERPRINT(" %x C(%d-%d) I(%d-%d) - %x \n", (unsigned int)pBuffHead, (unsigned int)nWidth,
            (unsigned int)nHeight, (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameWidth,
            (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameHeight,eError);
        }
        else if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
            VIDDEC_ParseVideo_MPEG2( &nWidth, &nHeight, pBuffHead);
            VIDDEC_BUFFERPRINT(" %x C(%d-%d) I(%d-%d) - %x \n", (unsigned int)pBuffHead, (unsigned int)nWidth,
            (unsigned int)nHeight, (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameWidth,
            (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameHeight,eError);
        }
        else if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
            if (pComponentPrivate->nWMVFileType == VIDDEC_WMV_ELEMSTREAM) {
                eError = VIDDEC_ParseVideo_WMV9_VC1( &nWidth, &nHeight, pBuffHead);
            }
            else {
                eError = VIDDEC_ParseVideo_WMV9_RCV( &nWidth, &nHeight, pBuffHead);
            }
            VIDDEC_BUFFERPRINT(" %x C(%d-%d) I(%d-%d) - %x \n", (unsigned int)pBuffHead, (unsigned int)nWidth,
            (unsigned int)nHeight, (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameWidth,
            (unsigned int)pComponentPrivate->pInPortDef->format.video.nFrameHeight,eError);
        }


		/*Verify correct values in the initial setup*/

		/*Get minimum output buffer size & verify if the actual size is enough*/
		if (pComponentPrivate->pOutPortFormat->eColorFormat == VIDDEC_COLORFORMAT420){
			nOutMinBufferSize = nWidth*nHeight*VIDDEC_FACTORFORMAT420;	
		}
		else{
			nOutMinBufferSize = nWidth*nHeight*VIDDEC_FACTORFORMAT422;
		}

		if(pComponentPrivate->pOutPortDef->nBufferSize < nOutMinBufferSize){
			pComponentPrivate->pOutPortDef->nBufferSize = nOutMinBufferSize;
			bOutPortSettingsChanged = OMX_TRUE;
		}

		/*TODO: Get minimum INPUT buffer size & verify if the actual size is enough*/
		
		/*Verify if actual width & height parameters are correct*/
        if (pComponentPrivate->pInPortDef->format.video.nFrameWidth != nWidth ||
            pComponentPrivate->pInPortDef->format.video.nFrameHeight != nHeight) {
            if((nWidth >= 1500) || (nHeight >= 1500)){
                pComponentPrivate->pInPortDef->format.video.nFrameHeight = 576;
                pComponentPrivate->pInPortDef->format.video.nFrameWidth = 720;
                eError = OMX_ErrorBadParameter;
                goto EXIT;
            }
            else if(((nWidth < 16) || (nHeight < 16))){
                pComponentPrivate->pInPortDef->format.video.nFrameHeight = 576;
                pComponentPrivate->pInPortDef->format.video.nFrameWidth = 720;
                eError = OMX_ErrorBadParameter;
                goto EXIT;
            }
            pComponentPrivate->pInPortDef->format.video.nFrameWidth = nWidth;
            pComponentPrivate->pInPortDef->format.video.nFrameHeight = nHeight;
			bInPortSettingsChanged = OMX_TRUE;
		}
		if(pComponentPrivate->pOutPortDef->format.video.nFrameWidth != nWidth ||
            pComponentPrivate->pOutPortDef->format.video.nFrameHeight != nHeight) {

			pComponentPrivate->pOutPortDef->format.video.nFrameWidth = nWidth;
            pComponentPrivate->pOutPortDef->format.video.nFrameHeight = nHeight;
			bOutPortSettingsChanged = OMX_TRUE;
		}

		if(bOutPortSettingsChanged || bInPortSettingsChanged){
			if(bOutPortSettingsChanged && bInPortSettingsChanged){
            	pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                	pComponentPrivate->pHandle->pApplicationPrivate,
                                                	OMX_EventPortSettingsChanged,
                                                	VIDDEC_BOTH_PORT,
                                                	0,
                                                	NULL);
				eError = OMX_ErrorBadParameter;
            	goto EXIT;
			}
			else {
				pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                pComponentPrivate->pHandle->pApplicationPrivate,
                                                OMX_EventPortSettingsChanged,
                                                bOutPortSettingsChanged ? VIDDEC_OUTPUT_PORT : VIDDEC_INPUT_PORT,
                                                0,
                                                NULL);
				eError = OMX_ErrorBadParameter;
            	goto EXIT;
			}
		}
        else {
            eError = OMX_ErrorNone;
        }
    }
    else {
        eError = OMX_ErrorNone;
    }
EXIT:
    return eError;
}
#endif

/* ========================================================================== */
/**
  *  Handle Data Buff function from application
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleDataBuf_FromApp(VIDDEC_COMPONENT_PRIVATE *pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
    VIDDEC_BUFFER_PRIVATE* pBufferPrivate = NULL;
    OMX_U32 inpBufSize;
    int ret = 0;
    OMX_U32 size_dsp;

    void* pUalgInpParams = NULL;
    LCML_DSP_INTERFACE* pLcmlHandle;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDefIn = pComponentPrivate->pInPortDef;
    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x iEndofInputSent 0x%x\n", (int*)pComponentPrivate,(int*)pComponentPrivate->iEndofInputSent);
    inpBufSize = pComponentPrivate->pInPortDef->nBufferSize;
    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    ret = read(pComponentPrivate->filled_inpBuf_Q[0], &(pBuffHead), sizeof(pBuffHead));
    if (ret == -1) {
        VIDDECODER_EPRINT("%d :: Error while reading from the pipe\n",__LINE__);
        eError = OMX_ErrorHardware;
        goto EXIT;
    }

#ifdef VIDDEC_ACTIVATEPARSER
        if( ((pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) ||
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2 ||
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV ||
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) &&
            pComponentPrivate->bParserEnabled) {
            if (pComponentPrivate->bFirstHeader == OMX_FALSE) {
            pComponentPrivate->bFirstHeader = OMX_TRUE;
            if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV &&
                pBuffHead->nFlags & OMX_BUFFERFLAG_EXTRADATA &&
                pComponentPrivate->nWMVFileType == VIDDEC_WMV_RCVSTREAM) {
				OMX_OTHER_EXTRADATATYPE* pExtraData;
				pExtraData = (OMX_OTHER_EXTRADATATYPE*)&pBuffHead->pBuffer[pBuffHead->nFilledLen];
				if ((pExtraData->nSize == sizeof(OMX_OTHER_EXTRADATATYPE)) &&
					pExtraData->nDataSize == sizeof(OMX_U32)) {
					if(pComponentPrivate->pUalgParams == NULL) {
						OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams,
										  WMV9DEC_UALGInputParam,
										  pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
					}
					if (pComponentPrivate->bIsNALBigEndian) {
						pComponentPrivate->pBufferRCV.sStructRCV.nStructData = (OMX_U32)pExtraData->data[0]<<24 |
																						pExtraData->data[1] << 16 |
																						pExtraData->data[2] << 8 |
																						pExtraData->data[3];
					}
					else {
						pComponentPrivate->pBufferRCV.sStructRCV.nStructData = (OMX_U32)pExtraData->data[0]<<0 |
																						pExtraData->data[1] << 8 |
																						pExtraData->data[2] << 16 |
																						pExtraData->data[3]<<24;
					}
					size_dsp = sizeof(WMV9DEC_UALGInputParam);
					((WMV9DEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount = pComponentPrivate->frameCounter;
					eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)
												pLcmlHandle)->pCodecinterfacehandle,
												EMMCodecInputBuffer,
												(OMX_U8*)&pComponentPrivate->pBufferRCV.pBuffer,
												/*(OMX_U8*)&pComponentPrivate->pRCVExtendedHeader,*/
												sizeof(VIDDEC_WMV_RCV_struct),
												sizeof(VIDDEC_WMV_RCV_struct),
												(OMX_U8 *)pUalgInpParams,
												size_dsp,
												(OMX_U8*)&pComponentPrivate->pBufferTemp);
				}
				else {
					VIDDECODER_EPRINT("Incorrect EXTRADATA values\n");
					eError = OMX_ErrorHardware;
					goto EXIT;
				}
            }
            else {
                eError = VIDDEC_ParseHeader( pComponentPrivate, pBuffHead);
                if(eError != OMX_ErrorNone)
                {
                    if (pBuffHead != NULL) {

#ifdef ANDROID
						eError = VIDDEC_SaveBuffer(pComponentPrivate, pBuffHead);
						if(eError != OMX_ErrorNone){
							goto EXIT;
						}
#endif
						
                        pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pInputPortPrivate;
                        pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                        VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
                #ifdef __PERF_INSTRUMENTATION__
                        PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                          pBuffHead->pBuffer,
                                          pBuffHead->nFilledLen,
                                          PERF_ModuleHLMM);
                #endif

                        pComponentPrivate->cbInfo.EmptyBufferDone(pComponentPrivate->pHandle,
                                                                  pComponentPrivate->pHandle->pApplicationPrivate,
                                                                  pBuffHead);
                    }
                    eError = OMX_ErrorNone;
                    goto EXIT;
                }
                else {
                    eError = OMX_ErrorNone;
                }
            }
        }
    }
#else
        if (pComponentPrivate->bFirstHeader == OMX_FALSE) {
        pComponentPrivate->bFirstHeader = OMX_TRUE;
        if( pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV &&
            pBuffHead->nFlags & OMX_BUFFERFLAG_EXTRADATA &&
            pComponentPrivate->nWMVFileType == VIDDEC_WMV_RCVSTREAM) {
			OMX_OTHER_EXTRADATATYPE* pExtraData;
			pExtraData = (OMX_OTHER_EXTRADATATYPE*)&pBuffHead->pBuffer[pBuffHead->nFilledLen];
			if ((pExtraData->nSize == sizeof(OMX_OTHER_EXTRADATATYPE)) &&
				pExtraData->nDataSize == sizeof(OMX_U32)) {
				if(pComponentPrivate->pUalgParams == NULL) {
					OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams,
									  WMV9DEC_UALGInputParam,
									  pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
				}
				if (pComponentPrivate->bIsNALBigEndian) {
					pComponentPrivate->pBufferRCV.sStructRCV.nStructData = (OMX_U32)pExtraData->data[0]<<24 |
																					pExtraData->data[1] << 16 |
																					pExtraData->data[2] << 8 |
																					pExtraData->data[3];
				}
				else {
					pComponentPrivate->pBufferRCV.sStructRCV.nStructData = (OMX_U32)pExtraData->data[0]<<0 |
																					pExtraData->data[1] << 8 |
																					pExtraData->data[2] << 16 |
																					pExtraData->data[3]<<24;
				}
				size_dsp = sizeof(WMV9DEC_UALGInputParam);
				((WMV9DEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount = pComponentPrivate->frameCounter;
				eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)
											pLcmlHandle)->pCodecinterfacehandle,
											EMMCodecInputBuffer,
											(OMX_U8*)&pComponentPrivate->pBufferRCV.pBuffer,
											/*(OMX_U8*)&pComponentPrivate->pRCVExtendedHeader,*/
											sizeof(VIDDEC_WMV_RCV_struct),
											sizeof(VIDDEC_WMV_RCV_struct),
											(OMX_U8 *)pUalgInpParams,
											size_dsp,
											(OMX_U8*)&pComponentPrivate->pBufferTemp);
			}
			else {
				VIDDECODER_EPRINT("Incorrect EXTRADATA values\n");
				eError = OMX_ErrorHardware;
				goto EXIT;
			}
        }
    }
#endif

    pComponentPrivate->MTbuffMark.hMarkTargetComponent = pBuffHead->hMarkTargetComponent;
    pComponentPrivate->MTbuffMark.pMarkData = pBuffHead->pMarkData;
    if(pComponentPrivate->qCmdMarkData.nElements && !(pBuffHead->hMarkTargetComponent))
    {
        VIDDEC_Queue_Remove(&(pComponentPrivate->qCmdMarkData), &(pComponentPrivate->MTbuffMark), VIDDEC_QUEUE_OMX_MARKTYPE);
    }
    eError = VIDDEC_Queue_Add(&pComponentPrivate->qBuffMark, &(pComponentPrivate->MTbuffMark), VIDDEC_QUEUE_OMX_MARKTYPE);
    if(eError != OMX_ErrorNone)
    {
        VIDDECODER_EPRINT("Error in queue resources\n");
        goto EXIT;
    }

    eError = VIDDEC_Queue_Add(&pComponentPrivate->qBytesSent, &(pBuffHead->nFilledLen), VIDDEC_QUEUE_OMX_U32);
    if(eError != OMX_ErrorNone)
    {
        VIDDECODER_EPRINT("Error in queue resources\n");
        goto EXIT;
    }

    VIDDEC_BUFFERPRINT("pBuffHead 0x%x eExecuteToIdle 0x%x\n", pBuffHead,pComponentPrivate->eExecuteToIdle);
    VIDDEC_BUFFERPRINT("nFilledLen 0x%x nFlags 0x%x\n", pBuffHead->nFilledLen,pBuffHead->nFlags);
    pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pInputPortPrivate;

    if(pBuffHead->nFlags & OMX_BUFFERFLAG_EOS){
        pComponentPrivate->iEndofInput = 0x1;
        pComponentPrivate->nFlags = pBuffHead->nFlags;
    }

    if(pComponentPrivate->iEndofInput == 0x1) {
            VIDDEC_BUFFERPRINT("End of Input EOS 0x%x nFilledLen 0x%x\n", pComponentPrivate->iEndofInput, pBuffHead->nFilledLen);
            if(pBuffHead->nFilledLen != 0) {
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        pUalgInpParams = (OMX_PTR)pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((H264VDEC_UALGInputParam *)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;

                        /* prepare buffer and input parameter if H264BitStreamFormat = 1 */
                        /*     the orignial buffer is: NAL1_Len NAL1 NAL2_Len NAL2...*/
                        /*     we need to pack the data buffer as: NAL1 NAL2 NAL3..*/
                        /*     and put the length info to the parameter array*/
                            if (pComponentPrivate->H264BitStreamFormat) {
                                OMX_U32 nal_len, i;
                                OMX_U8 *pDataBuf;
                                OMX_U32 length_pos = 0;
                                OMX_U32 data_pos = 0;
                                OMX_U32 buf_len;
                                H264VDEC_UALGInputParam *pParam;

                                buf_len = pBuffHead->nFilledLen;
                                pDataBuf = pBuffHead->pBuffer;
                                pParam = (H264VDEC_UALGInputParam *)pUalgInpParams;
                                pParam->ulNumOfNALU = 0;
                                while (pBuffHead->nFilledLen > length_pos+pComponentPrivate->H264BitStreamFormat) {
                                    if (pComponentPrivate->H264BitStreamFormat == 1)
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos];
                                        }
                                    else if (pComponentPrivate->H264BitStreamFormat == 2)
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos] << 8 | pDataBuf[length_pos+1];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos] << 0 | pDataBuf[length_pos+1] << 8 ;
                                        }
                                    else if (pComponentPrivate->H264BitStreamFormat == 4){
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos]<<24 | pDataBuf[length_pos+1] << 16 | pDataBuf[length_pos+2] << 8 | pDataBuf[length_pos+3];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos]<<0 | pDataBuf[length_pos+1] << 8 | pDataBuf[length_pos+2] << 16 | pDataBuf[length_pos+3]<<24;
                                        }
                                    }
                                    else {
                                        eError = OMX_ErrorBadParameter;
                                        goto EXIT;
                                    }
                                    length_pos += pComponentPrivate->H264BitStreamFormat;
                                    if (nal_len > buf_len - length_pos) {
                                        eError = OMX_ErrorBadParameter;
                                        goto EXIT;
                                    }
                                    /* move the memory*/
                                    for (i=0; i<nal_len; i++)
                                        pDataBuf[data_pos+i] = pDataBuf[length_pos+i];
                                    data_pos += nal_len;
                                    length_pos += nal_len;
                                    /* save the size*/
                                    pParam->pNALUSizeArray[pParam->ulNumOfNALU++] = nal_len;
                                }
                                /* update with the new data size*/
                                pBuffHead->nFilledLen = data_pos;
                            }
                        }
                        size_dsp = sizeof(H264VDEC_UALGInputParam);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                        pUalgInpParams = (OMX_PTR)pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((WMV9DEC_UALGInputParam*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(WMV9DEC_UALGInputParam);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                             pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                        pUalgInpParams = (OMX_PTR)pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((MP4VD_GPP_SN_UALGInputParams*)pUalgInpParams)->nBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(MP4VD_GPP_SN_UALGInputParams);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                        pUalgInpParams = (OMX_PTR)pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((MP2VDEC_UALGInputParam*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(MP2VDEC_UALGInputParam);
                    }
#ifdef VIDDEC_SPARK_CODE
                    else if (VIDDEC_SPARKCHECK) {
                        pUalgInpParams = (OMX_PTR)pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((SPARKVD_GPP_SN_UALGInputParams*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        ((SPARKVD_GPP_SN_UALGInputParams*)pUalgInpParams)->nIsSparkInput = 1;
                        size_dsp = sizeof(SPARKVD_GPP_SN_UALGInputParams);
                    }
#endif
                    else {
                        eError = OMX_ErrorUnsupportedSetting;
                        VIDDECODER_EPRINT("VIDDEC_HandleDataBuf_FromApp 0x%x\n", eError);
                        goto EXIT;
                    }

                    if (pComponentPrivate->ProcessMode == 0) {
                        OMX_U8 ucIndex = 0;
                        OMX_PTR pBufferFlags = NULL;
                        ucIndex = VIDDEC_CircBuf_GetHead(pComponentPrivate,
                                                         VIDDEC_CBUFFER_TIMESTAMP,
                                                         VIDDEC_INPUT_PORT);
                        pComponentPrivate->aBufferFlags[ucIndex].nTimeStamp = pBuffHead->nTimeStamp;
                        pComponentPrivate->aBufferFlags[ucIndex].nTickCount = pBuffHead->nTickCount;
                        pBuffHead->nFlags &= ~OMX_BUFFERFLAG_EOS;
                        pComponentPrivate->aBufferFlags[ucIndex].nFlags = pBuffHead->nFlags;
                        pBufferFlags = &pComponentPrivate->aBufferFlags[ucIndex];
                        VIDDEC_CircBuf_Add(pComponentPrivate,
                                           VIDDEC_CBUFFER_TIMESTAMP,
                                           VIDDEC_INPUT_PORT,
                                           pBufferFlags);
                    }
                    else {
                        pComponentPrivate->arrBufIndex[pComponentPrivate->nInBufIndex] = pBuffHead->nTimeStamp;
                        pComponentPrivate->nInBufIndex++;
                        pComponentPrivate->nInBufIndex %= pPortDefIn->nBufferCountActual;
                    }
                VIDDEC_BUFFERPRINT("pBuffHead->nTimeStamp %lld\n", pBuffHead->nTimeStamp);

#ifdef __PERF_INSTRUMENTATION__
                PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                  pBuffHead->pBuffer,
                                  pBuffHead->nFilledLen,
                                  PERF_ModuleCommonLayer);
#endif

                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL){
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_DSP;
#ifdef VIDDEC_FLAGGED_EOS
                    if (pComponentPrivate->bUseFlaggedEos) {
                        /*pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;*/
                        pBuffHead->nFlags = 0;
                    }
#endif
                    VIDDEC_BUFFERPRINT("Sending EOS Filled eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
                    eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)
                                                pLcmlHandle)->pCodecinterfacehandle,
                                                EMMCodecInputBuffer,
                                                pBuffHead->pBuffer,
                                                pBuffHead->nAllocLen,
                                                pBuffHead->nFilledLen,
                                                (OMX_U8 *)pUalgInpParams,
                                                size_dsp,
                                                (OMX_U8 *)pBuffHead);
                    if (eError != OMX_ErrorNone){
                        VIDDECODER_DPRINT("pComponentPrivate->iEndofInput Error 0x%x\n", eError);
                        eError = OMX_ErrorHardware;
                        goto EXIT;
                    }
                }
                else {
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
            }
            else {
                pBuffHead->nFilledLen = 0;
                pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
                VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
                ret = write(pComponentPrivate->free_inpBuf_Q[1], &pBuffHead, sizeof(pBuffHead));
                if(ret == -1){
                    VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,ret);
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                                           OMX_EventError,
                                                           OMX_ErrorInsufficientResources,
                                                           OMX_TI_ErrorSevere,
                                                           "Error writing to the output pipe");
                }
            }

            if(pComponentPrivate->iEndofInputSent == 0){
                pComponentPrivate->iEndofInputSent = 1;
                VIDDEC_BUFFERPRINT("Sending EOS Empty eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
                if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                    if(pComponentPrivate->pUalgParams == NULL)
                    {
                        OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams, H264VDEC_UALGInputParam,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
                    }
                        size_dsp = sizeof(H264VDEC_UALGInputParam);
                        ((H264VDEC_UALGInputParam *)pComponentPrivate->pUalgParams)->lBuffCount = -1;
                        VIDDEC_BUFFERPRINT("lBuffCount 0x%x\n",
                            ((H264VDEC_UALGInputParam *)pComponentPrivate->pUalgParams)->lBuffCount);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                    if(pComponentPrivate->pUalgParams == NULL)
                    {
                        OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams, WMV9DEC_UALGInputParam,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
                    }
                        size_dsp = sizeof(WMV9DEC_UALGInputParam);
                        ((WMV9DEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount = -1;
                        VIDDEC_BUFFERPRINT("lBuffCount 0x%x\n",
                            ((WMV9DEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                         pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                    if(pComponentPrivate->pUalgParams == NULL)
                    {
                        OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams, MP4VD_GPP_SN_UALGInputParams,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
                    }
                        size_dsp = sizeof(MP4VD_GPP_SN_UALGInputParams);
                        ((MP4VD_GPP_SN_UALGInputParams*)pComponentPrivate->pUalgParams)->nBuffCount = -1;
                        VIDDEC_BUFFERPRINT("lBuffCount 0x%x\n",
                            ((MP4VD_GPP_SN_UALGInputParams*)pComponentPrivate->pUalgParams)->nBuffCount);
                }
                else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                    if(pComponentPrivate->pUalgParams == NULL)
                    {
                        OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams, MP2VDEC_UALGInputParam,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
                    }
                        size_dsp = sizeof(MP2VDEC_UALGInputParam);
                        ((MP2VDEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount = -1;
                        VIDDEC_BUFFERPRINT("lBuffCount 0x%x\n",
                            ((MP2VDEC_UALGInputParam*)pComponentPrivate->pUalgParams)->lBuffCount);
                }
#ifdef VIDDEC_SPARK_CODE
                else if (VIDDEC_SPARKCHECK) {
                    if(pComponentPrivate->pUalgParams == NULL)
                    {
                        OMX_MALLOC_STRUCT(pComponentPrivate->pUalgParams, SPARKVD_GPP_SN_UALGInputParams,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel1]);
                    }
                    size_dsp = sizeof(SPARKVD_GPP_SN_UALGInputParams);
                    ((SPARKVD_GPP_SN_UALGInputParams*)pComponentPrivate->pUalgParams)->lBuffCount = -1;
                    ((SPARKVD_GPP_SN_UALGInputParams*)pComponentPrivate->pUalgParams)->nIsSparkInput = 1;
                    VIDDEC_BUFFERPRINT("lBuffCount 0x%x\n",
                        ((SPARKVD_GPP_SN_UALGInputParams*)pComponentPrivate->pUalgParams)->lBuffCount);

                }
#endif
                else {
                    eError = OMX_ErrorUnsupportedSetting;
                    goto EXIT;
                }

#ifdef __PERF_INSTRUMENTATION__
                PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                  NULL, 0,
                                  PERF_ModuleCommonLayer);
#endif
                if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                    pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                    pComponentPrivate->pLCML != NULL){
#ifdef VIDDEC_FLAGGED_EOS
                    if (pComponentPrivate->bUseFlaggedEos) {
                        pComponentPrivate->pTempBuffHead.nFlags = 0;
                        pComponentPrivate->pTempBuffHead.nFlags |= OMX_BUFFERFLAG_EOS;
                        pComponentPrivate->pTempBuffHead.nFilledLen = 0;
                        pComponentPrivate->pTempBuffHead.pBuffer = NULL;
                    }
#endif
#ifdef VIDDEC_FLAGGED_EOS
                if (!pComponentPrivate->bUseFlaggedEos) {
#endif
                    eError = LCML_QueueBuffer(pLcmlHandle->pCodecinterfacehandle,
                                                  EMMCodecInputBuffer,
                                                  NULL,
                                                  0,
                                                  0,
                                                  (OMX_U8 *)pComponentPrivate->pUalgParams,
                                                  size_dsp,
                                                  NULL);
#ifdef VIDDEC_FLAGGED_EOS
                }
                else {
                    eError = LCML_QueueBuffer(pLcmlHandle->pCodecinterfacehandle,
                                                  EMMCodecInputBuffer,
                                                  NULL,
                                                  0,
                                                  0,
                                                  (OMX_U8 *)pComponentPrivate->pUalgParams,
                                                  size_dsp,
                                                  (OMX_PTR)&pComponentPrivate->pTempBuffHead);
                }
#endif
                    if (eError != OMX_ErrorNone){
                        VIDDECODER_EPRINT("LCML_QueueBuffer 1 (0x%x)\n",eError);
                        eError = OMX_ErrorHardware;
                        goto EXIT;
                    }
                }
                else {
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
                pComponentPrivate->nFlags = pBuffHead->nFlags;
            }
    }
    else {
        pComponentPrivate->iEndofInputSent = 0;
        if(pBuffHead->nFilledLen != 0) {
                    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                        pUalgInpParams = pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((H264VDEC_UALGInputParam *)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        /* prepare buffer and input parameter if H264BitStreamFormat = 1 */
                        /*     the orignial buffer is: NAL1_Len NAL1 NAL2_Len NAL2...*/
                        /*     we need to pack the data buffer as: NAL1 NAL2 NAL3..*/
                        /*     and put the length info to the parameter array*/
                            if (pComponentPrivate->H264BitStreamFormat) {
                                OMX_U32 nal_len, i;
                                OMX_U8 *pDataBuf;
                                OMX_U32 length_pos = 0;
                                OMX_U32 data_pos = 0;
                                OMX_U32 buf_len;
                                H264VDEC_UALGInputParam *pParam;

                                buf_len = pBuffHead->nFilledLen;
                                pDataBuf = pBuffHead->pBuffer;
                                pParam = (H264VDEC_UALGInputParam *)pUalgInpParams;
                                pParam->ulNumOfNALU = 0;
                                while (pBuffHead->nFilledLen > length_pos+pComponentPrivate->H264BitStreamFormat) {
                                    if (pComponentPrivate->H264BitStreamFormat == 1)
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos];
                                        }
                                    else if (pComponentPrivate->H264BitStreamFormat == 2)
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos] << 8 | pDataBuf[length_pos+1];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos] << 0 | pDataBuf[length_pos+1] << 8 ;
                                        }
                                    else if (pComponentPrivate->H264BitStreamFormat == 4){
                                        if (pComponentPrivate->bIsNALBigEndian) {
                                            nal_len = (OMX_U32)pDataBuf[length_pos]<<24 | pDataBuf[length_pos+1] << 16 | pDataBuf[length_pos+2] << 8 | pDataBuf[length_pos+3];
                                        }
                                        else {
                                            nal_len = (OMX_U32)pDataBuf[length_pos]<<0 | pDataBuf[length_pos+1] << 8 | pDataBuf[length_pos+2] << 16 | pDataBuf[length_pos+3]<<24;
                                        }
                                    }
                                    else {
                                        eError = OMX_ErrorBadParameter;
                                        goto EXIT;
                                    }
                                    length_pos += pComponentPrivate->H264BitStreamFormat;
                                    if (nal_len > buf_len - length_pos) {
                                        eError = OMX_ErrorBadParameter;
                                        goto EXIT;
                                    }
                                    /* move the memory*/
                                    for (i=0; i<nal_len; i++)
                                        pDataBuf[data_pos+i] = pDataBuf[length_pos+i];
                                    data_pos += nal_len;
                                    length_pos += nal_len;
                                    /* save the size*/
                                    pParam->pNALUSizeArray[pParam->ulNumOfNALU++] = nal_len;
                                }
                                /* update with the new data size*/
                                pBuffHead->nFilledLen = data_pos;
                            }
                        }
                        size_dsp = sizeof(H264VDEC_UALGInputParam);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
                        pUalgInpParams = pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((WMV9DEC_UALGInputParam*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(WMV9DEC_UALGInputParam);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                             pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                        pUalgInpParams = pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((MP4VD_GPP_SN_UALGInputParams*)pUalgInpParams)->nBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(MP4VD_GPP_SN_UALGInputParams);
                    }
                    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
                        pUalgInpParams = pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((MP2VDEC_UALGInputParam*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        size_dsp = sizeof(MP2VDEC_UALGInputParam);
                    }
#ifdef VIDDEC_SPARK_CODE
                    else if (VIDDEC_SPARKCHECK) {
                        pUalgInpParams = pBufferPrivate->pUalgParam;
                        if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS) == 0) {
                            ((SPARKVD_GPP_SN_UALGInputParams*)pUalgInpParams)->lBuffCount = ++pComponentPrivate->frameCounter;
                        }
                        ((SPARKVD_GPP_SN_UALGInputParams*)pUalgInpParams)->nIsSparkInput = 1;
                        size_dsp = sizeof(SPARKVD_GPP_SN_UALGInputParams);
                    }
#endif
                    else {
                        eError = OMX_ErrorUnsupportedSetting;
                        VIDDECODER_EPRINT("VIDDEC_HandleDataBuf_FromApp 0x%x\n", eError);
                        goto EXIT;
                    }
            /* Store time stamp information */
            if (pComponentPrivate->ProcessMode == 0) {
                OMX_U8 ucIndex = 0;
                OMX_PTR pBufferFlags = NULL;
                ucIndex = VIDDEC_CircBuf_GetHead(pComponentPrivate,
                                                 VIDDEC_CBUFFER_TIMESTAMP,
                                                 VIDDEC_INPUT_PORT);
                pComponentPrivate->aBufferFlags[ucIndex].nTimeStamp = pBuffHead->nTimeStamp;
                pBuffHead->nFlags &= ~OMX_BUFFERFLAG_EOS;
                pComponentPrivate->aBufferFlags[ucIndex].nFlags = pBuffHead->nFlags;
                pComponentPrivate->aBufferFlags[ucIndex].nTickCount = pBuffHead->nTickCount;
                pBufferFlags = &pComponentPrivate->aBufferFlags[ucIndex];
                VIDDEC_CircBuf_Add(pComponentPrivate,
                                   VIDDEC_CBUFFER_TIMESTAMP,
                                   VIDDEC_INPUT_PORT,
                                   pBufferFlags);
            }
            else {
                pComponentPrivate->arrBufIndex[pComponentPrivate->nInBufIndex] = pBuffHead->nTimeStamp;
                pComponentPrivate->nInBufIndex++;
                pComponentPrivate->nInBufIndex %= pPortDefIn->nBufferCountActual;
            }
            VIDDEC_BUFFERPRINT("pBuffHead->nTimeStamp %lld\n", pBuffHead->nTimeStamp);
    #ifdef __PERF_INSTRUMENTATION__
            PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                              pBuffHead->pBuffer,
                              pBuffHead->nFilledLen,
                              PERF_ModuleCommonLayer);
    #endif
            if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                pComponentPrivate->pLCML != NULL){
                pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_DSP;
                VIDDEC_BUFFERPRINT("Sending Filled eBufferOwner 0x%x f%x\n", pBufferPrivate->eBufferOwner, pComponentPrivate->frameCounter);

#ifdef ANDROID
					if(pComponentPrivate->eFirstBuffer.bSaveFirstBuffer == OMX_TRUE){
						eError = VIDDEC_CopyBuffer(pComponentPrivate, pBuffHead);
						if(eError != OMX_ErrorNone){
							VIDDECODER_EPRINT("VIDDEC_HandleDataBuf_FromApp: VIDDEC_CopyBuffer()= 0x%x\n", eError);
							if(eError == OMX_ErrorInsufficientResources){
								goto EXIT;
							}
						}
						
					}
#endif
                eError = LCML_QueueBuffer(((LCML_DSP_INTERFACE*)
                                            pLcmlHandle)->pCodecinterfacehandle,
                                            EMMCodecInputBuffer,
                                            pBuffHead->pBuffer,
                                            pBuffHead->nAllocLen,
                                            pBuffHead->nFilledLen,
                                            (OMX_U8 *)pUalgInpParams,
                                            size_dsp,
                                            (OMX_U8 *)pBuffHead);
                if (eError != OMX_ErrorNone){
                    VIDDECODER_EPRINT("LCML_QueueBuffer 2 (0x%x)\n",eError);
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
            }
            else {
                eError = OMX_ErrorHardware;
                goto EXIT;
            }
        }
        else {
            pBuffHead->nFilledLen = 0;
            pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
            VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
            ret = write(pComponentPrivate->free_inpBuf_Q[1], &pBuffHead, sizeof(pBuffHead));
            if(ret == -1){
                VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,ret);
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorInsufficientResources,
                                                       OMX_TI_ErrorSevere,
                                                       "Error writing to the output pipe");
            }
        }
    }
EXIT:
    VIDDEC_BUFFERPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  *  Handle Data Buff function from DSP
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleDataBuf_FromDsp(VIDDEC_COMPONENT_PRIVATE *pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_ERRORTYPE eExtendedError = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pBuffHead;
    VIDDEC_BUFFER_PRIVATE* pBufferPrivate = NULL;
    int ret;

    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x\n", (int*)pComponentPrivate);
    ret = read(pComponentPrivate->filled_outBuf_Q[0], &pBuffHead, sizeof(pBuffHead));
    if (ret == -1) {
        VIDDECODER_EPRINT ("%d :: Error while reading from dsp out pipe\n",__LINE__);
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
    if (pComponentPrivate->eState == OMX_StateLoaded || pComponentPrivate->eState == OMX_StateIdle) {
        eError = OMX_ErrorNone;
        goto EXIT;
    }
    VIDDEC_BUFFERPRINT("pBuffHead 0x%x eExecuteToIdle 0x%x\n", pBuffHead,pComponentPrivate->eExecuteToIdle);
    if(pBuffHead != NULL) {
        OMX_S32 nErrorCode = 0;
        OMX_S32 nInternalErrorCode = 0;
        OMX_U32 ulDisplayID = 0;
        pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pOutputPortPrivate;
        /*if(pBufferPrivate->eBufferOwner == VIDDEC_BUFFER_WITH_CLIENT){
            VIDDECODER_EPRINT("buffer dropped pBuffHead VIDDEC_HandleDataBuf_FromDsp %x\n",pBuffHead);
            goto EXIT;
        }*/
        pBuffHead->nFlags &= ~(OMX_BUFFERFLAG_SYNCFRAME);
        pBuffHead->nFlags &= ~(VIDDEC_BUFFERFLAG_FRAMETYPE_MASK);
        pBuffHead->nFlags &= ~(VIDDEC_BUFFERFLAG_EXTENDERROR_MASK);
        if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
            H264VDEC_UALGOutputParam* pUalgOutParams = NULL;
            pUalgOutParams = (H264VDEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
            nErrorCode = (pUalgOutParams->iErrorCode);
            ulDisplayID = pUalgOutParams->ulDisplayID;
            if(pUalgOutParams->ulDecodedFrameType == VIDDEC_I_FRAME) {
                pBuffHead->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_I_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_P_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_P_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_B_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_B_FRAME;
            }
            else {
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_IDR_FRAME;
            }
            /*VIDDEC_ISFLAGSET*/
        }
        else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
            WMV9DEC_UALGOutputParam* pUalgOutParams = NULL;
            pUalgOutParams = (WMV9DEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
            nErrorCode = (pUalgOutParams->iErrorCode);
            ulDisplayID = pUalgOutParams->ulDisplayID;
            if(pUalgOutParams->ulDecodedFrameType == VIDDEC_I_FRAME) {
                pBuffHead->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_I_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_P_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_P_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_B_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_B_FRAME;
            }
            else {
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_IDR_FRAME;
            }
        }
        else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
                     pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
            MP4VD_GPP_SN_UALGOutputParams* pUalgOutParams = NULL;
            pUalgOutParams = (MP4VD_GPP_SN_UALGOutputParams *)pBufferPrivate->pUalgParam;
            nErrorCode = (pUalgOutParams->iErrorCode);
            ulDisplayID = pUalgOutParams->ulDisplayID;
            if(pUalgOutParams->ulDecodedFrameType == VIDDEC_I_FRAME) {
                pBuffHead->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_I_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_P_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_P_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_B_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_B_FRAME;
            }
            else {
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_IDR_FRAME;
            }
        }
        else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
            MP2VDEC_UALGOutputParam* pUalgOutParams = NULL;
            pUalgOutParams = (MP2VDEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
            nErrorCode = (pUalgOutParams->lErrorCode);
            ulDisplayID = pUalgOutParams->ulDisplayID;
            if(pUalgOutParams->ulDecodedFrameType == VIDDEC_I_FRAME) {
                pBuffHead->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_I_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_P_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_P_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_B_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_B_FRAME;
            }
            else {
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_IDR_FRAME;
            }
        }
#ifdef VIDDEC_SPARK_CODE
        else if (VIDDEC_SPARKCHECK) {
            SPARKVD_GPP_SN_UALGOutputParams* pUalgOutParams = NULL;
            pUalgOutParams = (SPARKVD_GPP_SN_UALGOutputParams *)pBufferPrivate->pUalgParam;
            nErrorCode = (pUalgOutParams->iErrorCode);
            ulDisplayID = pUalgOutParams->ulDisplayID;
            if(pUalgOutParams->ulDecodedFrameType == VIDDEC_I_FRAME) {
                pBuffHead->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_I_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_P_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_P_FRAME;
            }
            else if(pUalgOutParams->ulDecodedFrameType == VIDDEC_B_FRAME){
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_B_FRAME;
            }
            else {
                pBuffHead->nFlags |= VIDDEC_BUFFERFLAG_FRAMETYPE_IDR_FRAME;
            }
        }
#endif
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
        pBuffHead->nFlags |= (nErrorCode<<12);
        /*VIDDECODER_EPRINT("nErrorCode %x nFlags %x\n", (int *)nErrorCode, (int *)pBuffHead->nFlags);*/
        if((nErrorCode & 0xff) != 0){/*OMX_BUFFERFLAG_DATACORRUPT*/
            nInternalErrorCode = ((nErrorCode & VIDDEC_BUFFERFLAG_EXTENDERROR_DIRTY)>>12);
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_FATALERROR)){
                eExtendedError = OMX_ErrorStreamCorrupt;
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Not Recoverable Error Detected in Buffer in buffer %p %d(int# %x/%d) OMX_ErrorStreamCorrupt\n",
                        pBuffHead, ulDisplayID, (nErrorCode & VIDDEC_BUFFERFLAG_EXTENDERROR_DIRTY), pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_APPLIEDCONCEALMENT)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                VIDDECODER_EPRINT("Applied Concealment in buffer %p %d(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_INSUFFICIENTDATA)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Insufficient Data in buffer %p %d(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_CORRUPTEDDATA)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Corrupted Data in buffer %p %x(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_CORRUPTEDHEADER)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Corrupted Header in buffer %p %d(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_UNSUPPORTEDINPUT)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Unsupported Input in buffer %p %d(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
            if(VIDDEC_ISFLAGSET(nErrorCode,VIDDEC_XDM_UNSUPPORTEDPARAM)){
                pBuffHead->nFlags |= OMX_BUFFERFLAG_DATACORRUPT;
                pBuffHead->nFilledLen = 0;
                VIDDECODER_EPRINT("Unsupported Parameter in buffer %p %d(int# %x/%d)\n",
                        pBuffHead, ulDisplayID, nInternalErrorCode, pBuffHead->nFilledLen);
            }
        }
#ifdef KHRONOS_1_1
        if (pComponentPrivate->eMBErrorReport.bEnabled) {/* && pBuffHead->nFilledLen != 0*/
            OMX_U8* ErrMapFrom = NULL;
            OMX_U8* ErrMapTo = NULL;
            /*OMX_U32 nlooping = 0;*/
            OMX_U32 nErrMapSize = 0;
            if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
             pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
                MP4VD_GPP_SN_UALGOutputParams* pUalgOutParams = NULL;
                pUalgOutParams = (MP4VD_GPP_SN_UALGOutputParams *)pBufferPrivate->pUalgParam;
                ErrMapFrom = pUalgOutParams->usMbErrorBuf;
                /*todo add code to use ualg_array*/
                nErrMapSize = pComponentPrivate->pOutPortDef->format.video.nFrameWidth *
                              pComponentPrivate->pOutPortDef->format.video.nFrameHeight / 256;
                ErrMapTo = pComponentPrivate->eMBErrorMapType[pComponentPrivate->cMBErrorIndexIn].ErrMap;
                pComponentPrivate->eMBErrorMapType[pComponentPrivate->cMBErrorIndexIn].nErrMapSize = nErrMapSize;
                memcpy(ErrMapTo, ErrMapFrom, nErrMapSize);
                pComponentPrivate->cMBErrorIndexIn++;
                pComponentPrivate->cMBErrorIndexIn %= pComponentPrivate->pOutPortDef->nBufferCountActual;
            }
            if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
                H264VDEC_UALGOutputParam* pUalgOutParams = NULL;
                pUalgOutParams = (H264VDEC_UALGOutputParam *)pBufferPrivate->pUalgParam;
                ErrMapFrom = pUalgOutParams->pMBErrStatOutBuf;
                /*todo add code to use ualg_array*/
                nErrMapSize = pComponentPrivate->pOutPortDef->format.video.nFrameWidth *
                              pComponentPrivate->pOutPortDef->format.video.nFrameHeight / 256;
                ErrMapTo = pComponentPrivate->eMBErrorMapType[pComponentPrivate->cMBErrorIndexIn].ErrMap;
                pComponentPrivate->eMBErrorMapType[pComponentPrivate->cMBErrorIndexIn].nErrMapSize = nErrMapSize;
                memcpy(ErrMapTo, ErrMapFrom, nErrMapSize);
                pComponentPrivate->cMBErrorIndexIn++;
                pComponentPrivate->cMBErrorIndexIn %= pComponentPrivate->pOutPortDef->nBufferCountActual;
            }
        }
#endif
        if (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
            if(pComponentPrivate->bFirstBuffer) {
                VIDDEC_BUFFERPRINT("**** Setting OMX_BUFFERFLAG_STARTTIME\n");
                pBuffHead->nFlags |= OMX_BUFFERFLAG_STARTTIME;
                pComponentPrivate->bFirstBuffer = 0;
            }
            else {
                pBuffHead->nFlags &= ~(OMX_BUFFERFLAG_STARTTIME);
            }
            if(pBuffHead != NULL){
                if((pBuffHead->nFlags & OMX_BUFFERFLAG_DECODEONLY) == 0) {
                    pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_TUNNELEDCOMP;
                    VIDDEC_BUFFERPRINT("tunnel eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                    PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                      pBuffHead->pBuffer,
                                      pBuffHead->nFilledLen,
                                      PERF_ModuleLLMM);
    #endif
                    VIDDEC_BUFFERPRINT("VIDDEC_HandleDataBuf_FromDsp %x   %x\n",(int)pBuffHead->nFlags,(int)pBuffHead->nFilledLen);
                    VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                    eError = OMX_EmptyThisBuffer(pComponentPrivate->pCompPort[1]->hTunnelComponent, pBuffHead);
                }
                else {
                    write(pComponentPrivate->free_outBuf_Q[1],&pBuffHead,sizeof(pBuffHead));
                }
            }
        }
        else {
            if(pBuffHead != NULL) {
    #ifdef __PERF_INSTRUMENTATION__
                PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                                  pBuffHead->pBuffer,
                                  pBuffHead->nFilledLen,
                                  PERF_ModuleHLMM);
    #endif

                VIDDEC_Propagate_Mark(pComponentPrivate, pBuffHead);
                pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
                VIDDEC_BUFFERPRINT("standalone buffer eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef VIDDEC_FLAGGED_EOS
                if (!pComponentPrivate->bUseFlaggedEos) {
    #endif
                    if(pComponentPrivate->bPlayCompleted == OMX_TRUE && pComponentPrivate->bBuffFound == OMX_FALSE){
                        pBuffHead->nFlags |= OMX_BUFFERFLAG_EOS;
                        /*check values to ZERO*/
                        pComponentPrivate->bBuffFound = OMX_TRUE;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                            pComponentPrivate->pHandle->pApplicationPrivate,
                                                            OMX_EventBufferFlag,
                                                            VIDDEC_OUTPUT_PORT,
                                                            OMX_BUFFERFLAG_EOS,
                                                            NULL);
                    }
    #ifdef VIDDEC_FLAGGED_EOS
                }
                else {
                    if((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS)){
                        pComponentPrivate->bBuffFound = OMX_TRUE;
                        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                            pComponentPrivate->pHandle->pApplicationPrivate,
                                                            OMX_EventBufferFlag,
                                                            VIDDEC_OUTPUT_PORT,
                                                            OMX_BUFFERFLAG_EOS,
                                                            NULL);
                    }
                }
    #endif

                pComponentPrivate->cbInfo.FillBufferDone(pComponentPrivate->pHandle,
                                                         pComponentPrivate->pHandle->pApplicationPrivate,
                                                         pBuffHead);
            }
        }
    }

EXIT:
    if(eExtendedError != OMX_ErrorNone) {
        eError = eExtendedError;

    }
    VIDDEC_BUFFERPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  *  Handle Free Data Buff
  **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_HandleFreeDataBuf( VIDDEC_COMPONENT_PRIVATE *pComponentPrivate )
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pBuffHead;
    VIDDEC_BUFFER_PRIVATE* pBufferPrivate = NULL;
    int ret;
    int inputbufsize = (int)pComponentPrivate->pInPortDef->nBufferSize;

    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x\n", (int*)pComponentPrivate);
    ret = read(pComponentPrivate->free_inpBuf_Q[0], &pBuffHead, sizeof(pBuffHead));
    if (ret == -1) {
        VIDDECODER_EPRINT ("%d :: Error while reading from the free Q\n",__LINE__);
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
    if (pComponentPrivate->eState == OMX_StateLoaded || pComponentPrivate->eState == OMX_StateIdle) {
        eError = OMX_ErrorNone;
        goto EXIT;
    }
    VIDDEC_BUFFERPRINT("pBuffHead 0x%x eExecuteToIdle 0x%x\n", pBuffHead,pComponentPrivate->eExecuteToIdle);
    if (pBuffHead != NULL) {
        pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pInputPortPrivate;
        pBuffHead->nAllocLen = inputbufsize;
        pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_CLIENT;
        VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
        PERF_SendingFrame(pComponentPrivate->pPERFcomp,
                          pBuffHead->pBuffer,
                          pBuffHead->nFilledLen,
                          PERF_ModuleHLMM);
#endif

        pComponentPrivate->cbInfo.EmptyBufferDone(pComponentPrivate->pHandle,
                                                  pComponentPrivate->pHandle->pApplicationPrivate,
                                                  pBuffHead);
    }
    VIDDEC_BUFFERPRINT("---EXITING(0x%x) \n",eError);
EXIT:
    return eError;
}

/*-------------------------------------------------------------------*/
/**
  *  Function to fill DSP structures via LCML
  *
  *
  *
  * @retval OMX_NoError              Success, ready to roll
  *
  **/
/*-------------------------------------------------------------------*/
OMX_ERRORTYPE VIDDEC_InitDSP_WMVDec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    LCML_DSP *lcml_dsp = NULL;
    OMX_U32 nInpBuff = MAX_PRIVATE_IN_BUFFERS;
    OMX_U32 nInpBuffSize = 0;
    OMX_U32 nOutBuff = MAX_PRIVATE_OUT_BUFFERS;
    OMX_U32 nOutBuffSize = 0;
    WMV9DEC_SNCreatePhArg* pCreatePhaseArgs = NULL;
    OMX_U16 arr[100];
    LCML_CALLBACKTYPE cb;

    VIDDECODER_DPRINT("+++ENTERING\n");
    nInpBuff = pComponentPrivate->pInPortDef->nBufferCountActual;
    nOutBuff = pComponentPrivate->pOutPortDef->nBufferCountActual;

    /* Back it up for further use in this function */
    nInpBuffSize = pComponentPrivate->pInPortDef->nBufferSize;
    nOutBuffSize = pComponentPrivate->pOutPortDef->nBufferSize;

    pLcmlHandle = (LCML_DSP_INTERFACE *)pComponentPrivate->pLCML;
    lcml_dsp = (((LCML_DSP_INTERFACE*)pLcmlHandle)->dspCodec);

    lcml_dsp->DeviceInfo.TypeofDevice = 0;
    lcml_dsp->DeviceInfo.DspStream    = NULL;

    lcml_dsp->In_BufInfo.nBuffers     = nInpBuff;
    lcml_dsp->In_BufInfo.nSize        = nInpBuffSize;
    lcml_dsp->In_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->Out_BufInfo.nBuffers     = nOutBuff;
    lcml_dsp->Out_BufInfo.nSize        = nOutBuffSize;
    lcml_dsp->Out_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->NodeInfo.nNumOfDLLs = OMX_WMVDEC_NUM_DLLS;
    lcml_dsp->NodeInfo.AllUUIDs[0].uuid = (struct DSP_UUID *)&WMVDSOCKET_TI_UUID;
    strcpy ((char*)(lcml_dsp->NodeInfo.AllUUIDs[0].DllName),(char*)WMV_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[0].eDllType = DLL_NODEOBJECT;

    lcml_dsp->NodeInfo.AllUUIDs[1].uuid = (struct DSP_UUID *)&WMVDSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[1].DllName,(char*)WMV_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[1].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[2].uuid = (struct DSP_UUID *)&USN_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[2].DllName,(char*)USN_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[2].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[3].uuid = (struct DSP_UUID *)&RINGIO_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[3].DllName,(char*)RINGIO_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[3].eDllType = DLL_DEPENDENT;

    lcml_dsp->SegID     = 0;
    lcml_dsp->Timeout   = -1;
    lcml_dsp->Alignment = 0;
    lcml_dsp->Priority  = 5;

    if(pComponentPrivate->ProcessMode == 0){
        if(pComponentPrivate->wmvProfile == VIDDEC_WMV_PROFILEMAX)
        {
            if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 640) ||
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 480)) {
                lcml_dsp->ProfileID = 4;
            }
            else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 640) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352)) ||
                ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 480) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288))) {
                lcml_dsp->ProfileID = 2;
            }
            else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
                ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
                lcml_dsp->ProfileID = 1;
            }
            else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
                ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
                (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
                lcml_dsp->ProfileID = 0;
            }
            else {
                eError = OMX_ErrorUnsupportedSetting;
                goto EXIT;
            }
        }
        else
        {
            switch(pComponentPrivate->wmvProfile)
            {
            case VIDDEC_WMV_PROFILE0:
                lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID0;
                break;
            case VIDDEC_WMV_PROFILE1:
                lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID1;
                break;
            case VIDDEC_WMV_PROFILE2:
                lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID2;
                break;
            default:
                {
                    eError = OMX_ErrorBadParameter;
                    goto EXIT;
                }
            }
        }
    } else if(pComponentPrivate->ProcessMode == 1)
    {
        switch(pComponentPrivate->wmvProfile)
        {
        case VIDDEC_WMV_PROFILE3:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID3;
            break;
        case VIDDEC_WMV_PROFILE4:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID4;
            break;
        case VIDDEC_WMV_PROFILE5:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID5;
            break;
        case VIDDEC_WMV_PROFILE6:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID6;
            break;
        case VIDDEC_WMV_PROFILE7:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID7;
            break;
        case VIDDEC_WMV_PROFILE8:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID8;
            break;
        case VIDDEC_WMV_PROFILEMAX:
            lcml_dsp->ProfileID = VIDDEC_WMV_PROFILE_ID8;
            break;
        default:
            eError = OMX_ErrorBadParameter;
            goto EXIT;
        }
    }
    else
    {
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    OMX_MALLOC_STRUCT(pCreatePhaseArgs, WMV9DEC_SNCreatePhArg,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
    if (pCreatePhaseArgs == NULL) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    pCreatePhaseArgs->unNumOfStreams            = 2;
    pCreatePhaseArgs->unInputStreamID           = 0;
    pCreatePhaseArgs->unInputBufferType         = 0;
    pCreatePhaseArgs->unInputNumBufsPerStream   = (OMX_U16)nInpBuff;
    pCreatePhaseArgs->unOutputStreamID          = 1;
    pCreatePhaseArgs->unOutputBufferType        = 0;
    pCreatePhaseArgs->unOutputNumBufsPerStream  = (OMX_U16)nOutBuff;
    pCreatePhaseArgs->ulMaxWidth                = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    pCreatePhaseArgs->ulMaxHeight               = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);

    if (pComponentPrivate->nWMVFileType != VIDDEC_WMV_ELEMSTREAM) {
        pComponentPrivate->pBufferRCV.sStructRCV.nVertSize = (OMX_U32)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);
        pComponentPrivate->pBufferRCV.sStructRCV.nHorizSize = (OMX_U32)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    }

    if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT422) {
        pCreatePhaseArgs->ulYUVFormat           = WMV9VIDDEC_YUVFORMAT_INTERLEAVED422;
    }
    else if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT420) {
        pCreatePhaseArgs->ulYUVFormat           = WMV9VIDDEC_YUVFORMAT_PLANAR420;
    }
    else
    {
        VIDDECODER_EPRINT("Incorrect Color format %x\n",pComponentPrivate->pOutPortDef->format.video.eColorFormat);
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pCreatePhaseArgs->ulMaxFrameRate            = 0;
    pCreatePhaseArgs->ulMaxBitRate              = 0;
    pCreatePhaseArgs->ulDataEndianness          = 1;
    pCreatePhaseArgs->ulProfile                 = -1;
    pCreatePhaseArgs->ulMaxLevel                = -1;
    pCreatePhaseArgs->ulProcessMode             = pComponentPrivate->ProcessMode;
    pCreatePhaseArgs->lPreRollBufConfig         = 0;

    if (pComponentPrivate->nWMVFileType == VIDDEC_WMV_ELEMSTREAM) {
        pCreatePhaseArgs->usIsElementaryStream = VIDDEC_SN_WMV_ELEMSTREAM;
    }
    else {
        pCreatePhaseArgs->usIsElementaryStream = VIDDEC_SN_WMV_RCVSTREAM;
    }

    pCreatePhaseArgs->endArgs                   = END_OF_CR_PHASE_ARGS;

    memcpy (arr, pCreatePhaseArgs, sizeof(WMV9DEC_SNCreatePhArg));
    lcml_dsp->pCrPhArgs = arr;
    cb.LCML_Callback = (void *) VIDDEC_LCML_Callback;
    free(pCreatePhaseArgs);
    pCreatePhaseArgs = NULL;

    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
        pComponentPrivate->pLCML != NULL){
        eError = LCML_InitMMCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, NULL, &pLcmlHandle, NULL, &cb);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT ("LCML_InitMMCodec Failed!...%x\n",eError);
            eError = OMX_ErrorHardware;
            goto EXIT;
        }
    }
    else {
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/*-------------------------------------------------------------------*/
/**
  *  Function to fill DSP structures via LCML
  *
  *
  *
  * @retval OMX_NoError              Success, ready to roll
  *
  **/
/*-------------------------------------------------------------------*/

OMX_ERRORTYPE VIDDEC_InitDSP_H264Dec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    LCML_DSP *lcml_dsp = NULL;
    OMX_U32 nInpBuff = MAX_PRIVATE_IN_BUFFERS;
    OMX_U32 nInpBuffSize = 0;
    OMX_U32 nOutBuff = MAX_PRIVATE_OUT_BUFFERS;
    OMX_U32 nOutBuffSize = 0;
    H264VDEC_SNCreatePhArg* pCreatePhaseArgs = NULL;
    OMX_U16 arr[100];
    LCML_CALLBACKTYPE cb;

    VIDDECODER_DPRINT("+++ENTERING\n");
    /* Get number of input and output buffers */
    nInpBuff = pComponentPrivate->pInPortDef->nBufferCountActual;
    nOutBuff = pComponentPrivate->pOutPortDef->nBufferCountActual;

    /* Back it up for further use in this function */
    nInpBuffSize = pComponentPrivate->pInPortDef->nBufferSize;
    nOutBuffSize = pComponentPrivate->pOutPortDef->nBufferSize;

    pLcmlHandle = (LCML_DSP_INTERFACE *)pComponentPrivate->pLCML;
    lcml_dsp = (((LCML_DSP_INTERFACE*)pLcmlHandle)->dspCodec);

    lcml_dsp->DeviceInfo.TypeofDevice = 0;
    lcml_dsp->DeviceInfo.DspStream    = NULL;

    lcml_dsp->In_BufInfo.nBuffers     = nInpBuff;
    lcml_dsp->In_BufInfo.nSize        = nInpBuffSize;
    lcml_dsp->In_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->Out_BufInfo.nBuffers     = nOutBuff;
    lcml_dsp->Out_BufInfo.nSize        = nOutBuffSize;
    lcml_dsp->Out_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->NodeInfo.nNumOfDLLs = OMX_H264DEC_NUM_DLLS;
    lcml_dsp->NodeInfo.AllUUIDs[0].uuid = (struct DSP_UUID *)&H264VDSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[0].DllName,(char*)H264_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[0].eDllType = DLL_NODEOBJECT;

    lcml_dsp->NodeInfo.AllUUIDs[1].uuid = (struct DSP_UUID *)&H264VDSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[1].DllName,(char*)H264_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[1].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[2].uuid = (struct DSP_UUID *)&USN_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[2].DllName,(char*)USN_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[2].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[3].uuid = (struct DSP_UUID *)&RINGIO_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[3].DllName,(char*)RINGIO_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[3].eDllType = DLL_DEPENDENT;

    lcml_dsp->SegID     = 0;
    lcml_dsp->Timeout   = -1;
    lcml_dsp->Alignment = 0;
    lcml_dsp->Priority  = 5;

   if(pComponentPrivate->ProcessMode == 0){
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288)) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
   }
   else if(pComponentPrivate->ProcessMode == 1) {
        /*OMX_VIDEO_AVCLevelMax*/
        switch(pComponentPrivate->pH264->eLevel) {
            case OMX_VIDEO_AVCLevel1:
                lcml_dsp->ProfileID = 8;
                break;
            case OMX_VIDEO_AVCLevel1b:
                lcml_dsp->ProfileID = 9;
                break;
            case OMX_VIDEO_AVCLevel11:
                lcml_dsp->ProfileID = 10;
                break;
            case OMX_VIDEO_AVCLevel12:
                lcml_dsp->ProfileID = 11;
                break;
            case OMX_VIDEO_AVCLevel13:
            case OMX_VIDEO_AVCLevel2:
                lcml_dsp->ProfileID = 12;
                break;
            case OMX_VIDEO_AVCLevel21:
            case OMX_VIDEO_AVCLevel22:
                lcml_dsp->ProfileID = 13;
                break;
            default:
                lcml_dsp->ProfileID = 14;
                break;
        }
   }
   /*add code to error*/

    OMX_MALLOC_STRUCT(pCreatePhaseArgs, H264VDEC_SNCreatePhArg,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
    if (pCreatePhaseArgs == NULL) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    pCreatePhaseArgs->unNumOfStreams            = 2;
    pCreatePhaseArgs->unInputStreamID           = 0;
    pCreatePhaseArgs->unInputBufferType         = 0;
    pCreatePhaseArgs->unInputNumBufsPerStream   = (OMX_U16)nInpBuff;
    pCreatePhaseArgs->unOutputStreamID          = 1;
    pCreatePhaseArgs->unOutputBufferType        = 0;
    pCreatePhaseArgs->unOutputNumBufsPerStream  = (OMX_U16)nOutBuff;
    pCreatePhaseArgs->ulMaxWidth                = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    pCreatePhaseArgs->ulMaxHeight               = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);

    if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT422) {
        pCreatePhaseArgs->ulYUVFormat           = H264VIDDEC_YUVFORMAT_INTERLEAVED422;
    }
    else if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT420) {
        pCreatePhaseArgs->ulYUVFormat           = H264VIDDEC_YUVFORMAT_PLANAR420;
    }
    else
    {
        VIDDECODER_EPRINT("Incorrect Color format %x\n",pComponentPrivate->pOutPortDef->format.video.eColorFormat);
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pCreatePhaseArgs->ulMaxFrameRate            = 0;
    pCreatePhaseArgs->ulMaxBitRate              = 0;
    pCreatePhaseArgs->ulDataEndianness          = 1;
    pCreatePhaseArgs->ulProfile                 = 0;
    pCreatePhaseArgs->ulMaxLevel            = -1;
    pCreatePhaseArgs->ulProcessMode             = pComponentPrivate->ProcessMode;
    pCreatePhaseArgs->lPreRollBufConfig         = 0;
    pCreatePhaseArgs->ulBitStreamFormat         = (pComponentPrivate->H264BitStreamFormat>0?1:0);

    pCreatePhaseArgs->endArgs                   = END_OF_CR_PHASE_ARGS;

    memcpy (arr, pCreatePhaseArgs, sizeof(H264VDEC_SNCreatePhArg));
    lcml_dsp->pCrPhArgs = arr;
    cb.LCML_Callback = (void *) VIDDEC_LCML_Callback;
    free(pCreatePhaseArgs);
    pCreatePhaseArgs = NULL;

    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
        pComponentPrivate->pLCML != NULL){
        eError = LCML_InitMMCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, NULL, &pLcmlHandle, NULL, &cb);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT ("LCML_InitMMCodec Failed!...%x\n",eError);
            eError = OMX_ErrorHardware;
            goto EXIT;
        }
    }
    else {
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/*-------------------------------------------------------------------*/
/**
  *  Function to fill DSP structures via LCML
  *
  *
  *
  * @retval OMX_NoError              Success, ready to roll
  *
  **/
/*-------------------------------------------------------------------*/

OMX_ERRORTYPE VIDDEC_InitDSP_Mpeg4Dec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    LCML_DSP *lcml_dsp = NULL;
    OMX_U32 nInpBuff = MAX_PRIVATE_IN_BUFFERS;
    OMX_U32 nInpBuffSize = 0;
    OMX_U32 nOutBuff = MAX_PRIVATE_OUT_BUFFERS;
    OMX_U32 nOutBuffSize = 0;
    MP4VD_GPP_SN_Obj_CreatePhase* pCreatePhaseArgs = NULL;
    OMX_U16 arr[100];
    LCML_CALLBACKTYPE cb;

    VIDDECODER_DPRINT("+++ENTERING\n");
    /* Get number of input and output buffers */
    nInpBuff = pComponentPrivate->pInPortDef->nBufferCountActual;
    nOutBuff = pComponentPrivate->pOutPortDef->nBufferCountActual;

    /* Back it up for further use in this function */
    nInpBuffSize = pComponentPrivate->pInPortDef->nBufferSize;
    nOutBuffSize = pComponentPrivate->pOutPortDef->nBufferSize;

    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    lcml_dsp = (((LCML_DSP_INTERFACE*)pLcmlHandle)->dspCodec);

    lcml_dsp->DeviceInfo.TypeofDevice = 0;
    lcml_dsp->DeviceInfo.DspStream    = NULL;

    lcml_dsp->In_BufInfo.nBuffers      = nInpBuff;
    lcml_dsp->In_BufInfo.nSize         = nInpBuffSize;
    lcml_dsp->In_BufInfo.DataTrMethod  = DMM_METHOD;

    lcml_dsp->Out_BufInfo.nBuffers     = nOutBuff;
    lcml_dsp->Out_BufInfo.nSize        = nOutBuffSize;
    lcml_dsp->Out_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->NodeInfo.nNumOfDLLs       = OMX_MP4DEC_NUM_DLLS;
    lcml_dsp->NodeInfo.AllUUIDs[0].uuid = (struct DSP_UUID *)&MP4DSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[0].DllName,(char*)MP4_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[0].eDllType = DLL_NODEOBJECT;

    lcml_dsp->NodeInfo.AllUUIDs[1].uuid = (struct DSP_UUID *)&MP4DSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[1].DllName,(char*)MP4_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[1].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[2].uuid = (struct DSP_UUID *)&RINGIO_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[2].DllName,(char*)RINGIO_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[2].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[3].uuid = (struct DSP_UUID *)&USN_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[3].DllName,(char*)USN_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[3].eDllType = DLL_DEPENDENT;

    lcml_dsp->SegID     = 0;
    lcml_dsp->Timeout   = -1;
    lcml_dsp->Alignment = 0;
    lcml_dsp->Priority  = 5;

    if(pComponentPrivate->ProcessMode == 0){
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 640) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 480)) {
            lcml_dsp->ProfileID = 4;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 640) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 480) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288))) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }
    else if(pComponentPrivate->ProcessMode == 1) {
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 640) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 480)) {
            lcml_dsp->ProfileID = 4;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 640) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 480) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288))) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }

    OMX_MALLOC_STRUCT(pCreatePhaseArgs, MP4VD_GPP_SN_Obj_CreatePhase,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
    if (pCreatePhaseArgs == NULL) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    pCreatePhaseArgs->unNumOfStreams            = 2;
    pCreatePhaseArgs->unInputStreamID           = 0;
    pCreatePhaseArgs->unInputBufferType         = 0;
    pCreatePhaseArgs->unlInputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pInPortDef->nBufferCountActual);
    pCreatePhaseArgs->unOutputStreamID          = 1;
    pCreatePhaseArgs->unOutputBufferType        = 0;
    pCreatePhaseArgs->unOutputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pOutPortDef->nBufferCountActual);
    if (pComponentPrivate->pOutPortDef->format.video.nFrameWidth < 854) {
        pCreatePhaseArgs->ulMaxWidth                = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    }
    else {
        pCreatePhaseArgs->ulMaxWidth                = 864;
    }
    if (pComponentPrivate->pOutPortDef->format.video.nFrameHeight < 854) {
        pCreatePhaseArgs->ulMaxHeight               = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);
    }
    else {
        pCreatePhaseArgs->ulMaxHeight                = 864;
    }
    if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT422) {
        pCreatePhaseArgs->ulYUVFormat           = MP4VIDDEC_YUVFORMAT_INTERLEAVED422;
    }
    else if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT420) {
        pCreatePhaseArgs->ulYUVFormat           = MP4VIDDEC_YUVFORMAT_PLANAR420;
    }
    else
    {
        VIDDECODER_EPRINT("Incorrect Color format %x\n",pComponentPrivate->pOutPortDef->format.video.eColorFormat);
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pCreatePhaseArgs->ulMaxFrameRate            = 0;
    pCreatePhaseArgs->ulMaxBitRate              = -1;
    pCreatePhaseArgs->ulDataEndianness          = 1;
    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4){
        pCreatePhaseArgs->ulProfile                 = 0;
    }
    else {
        pCreatePhaseArgs->ulProfile                 = 8;
    }
    pCreatePhaseArgs->ulMaxLevel                = -1;
    pCreatePhaseArgs->ulProcessMode             = pComponentPrivate->ProcessMode;
    pCreatePhaseArgs->ulPreRollBufConfig        = 0;
    pCreatePhaseArgs->endArgs                   = END_OF_CR_PHASE_ARGS;

    memcpy(arr, pCreatePhaseArgs, sizeof(MP4VD_GPP_SN_Obj_CreatePhase));
    lcml_dsp->pCrPhArgs = arr;
    cb.LCML_Callback = (void*) VIDDEC_LCML_Callback;
    free(pCreatePhaseArgs);
    pCreatePhaseArgs = NULL;

    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
        pComponentPrivate->pLCML != NULL){
        eError = LCML_InitMMCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, NULL, &pLcmlHandle, NULL, &cb);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT ("LCML_InitMMCodec Failed!...%x\n",eError);
            eError = OMX_ErrorHardware;
            goto EXIT;
        }
    }
    else {
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}


/*-------------------------------------------------------------------*/
/**
  *  Function to fill DSP structures via LCML
  *
  *
  *
  * @retval OMX_NoError              Success, ready to roll
  *
  **/
/*-------------------------------------------------------------------*/

OMX_ERRORTYPE VIDDEC_InitDSP_Mpeg2Dec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    LCML_DSP *lcml_dsp = NULL;
    OMX_U32 nInpBuff = MAX_PRIVATE_IN_BUFFERS;
    OMX_U32 nInpBuffSize = 0;
    OMX_U32 nOutBuff = MAX_PRIVATE_OUT_BUFFERS;
    OMX_U32 nOutBuffSize = 0;
    MP2VDEC_SNCreatePhArg* pCreatePhaseArgs = NULL;
    OMX_U16 arr[100];
    LCML_CALLBACKTYPE cb;

    VIDDECODER_DPRINT("+++ENTERING\n");
    /* Get number of input and output buffers */
    nInpBuff = pComponentPrivate->pInPortDef->nBufferCountActual;
    nOutBuff = pComponentPrivate->pOutPortDef->nBufferCountActual;

    /* Back it up for further use in this function */
    nInpBuffSize = pComponentPrivate->pInPortDef->nBufferSize;
    nOutBuffSize = pComponentPrivate->pOutPortDef->nBufferSize;

    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    lcml_dsp = (((LCML_DSP_INTERFACE*)pLcmlHandle)->dspCodec);

    lcml_dsp->DeviceInfo.TypeofDevice = 0;
    lcml_dsp->DeviceInfo.DspStream    = NULL;

    lcml_dsp->In_BufInfo.nBuffers      = nInpBuff;
    lcml_dsp->In_BufInfo.nSize         = nInpBuffSize;
    lcml_dsp->In_BufInfo.DataTrMethod  = DMM_METHOD;

    lcml_dsp->Out_BufInfo.nBuffers     = nOutBuff;
    lcml_dsp->Out_BufInfo.nSize        = nOutBuffSize;
    lcml_dsp->Out_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->NodeInfo.nNumOfDLLs       = OMX_MP2DEC_NUM_DLLS;
    lcml_dsp->NodeInfo.AllUUIDs[0].uuid = (struct DSP_UUID *)&MP2DSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[0].DllName,(char*)MP2_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[0].eDllType = DLL_NODEOBJECT;

    lcml_dsp->NodeInfo.AllUUIDs[1].uuid = (struct DSP_UUID *)&MP2DSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[1].DllName,(char*)MP2_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[1].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[2].uuid = (struct DSP_UUID *)&RINGIO_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[2].DllName,(char*)RINGIO_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[2].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[3].uuid = (struct DSP_UUID *)&USN_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[3].DllName,(char*)USN_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[3].eDllType = DLL_DEPENDENT;

    lcml_dsp->SegID     = 0;
    lcml_dsp->Timeout   = -1;
    lcml_dsp->Alignment = 0;
    lcml_dsp->Priority  = 5;

    if(pComponentPrivate->ProcessMode == 0){
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288)) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }
    else if(pComponentPrivate->ProcessMode == 1) {
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288)) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }

    OMX_MALLOC_STRUCT(pCreatePhaseArgs, MP2VDEC_SNCreatePhArg,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
    if (pCreatePhaseArgs == NULL) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    pCreatePhaseArgs->unNumOfStreams            = 2;
    pCreatePhaseArgs->unInputStreamID           = 0;
    pCreatePhaseArgs->unInputBufferType         = 0;
    pCreatePhaseArgs->unInputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pInPortDef->nBufferCountActual);
    pCreatePhaseArgs->unOutputStreamID          = 1;
    pCreatePhaseArgs->unOutputBufferType        = 0;
    pCreatePhaseArgs->unOutputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pOutPortDef->nBufferCountActual);
    pCreatePhaseArgs->ulMaxWidth                = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    pCreatePhaseArgs->ulMaxHeight               = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);

    if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT422) {
        pCreatePhaseArgs->ulYUVFormat           = MP2VIDDEC_YUVFORMAT_INTERLEAVED422;
    }
    else if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT420) {
        pCreatePhaseArgs->ulYUVFormat           = MP2VIDDEC_YUVFORMAT_PLANAR420;
    }
    else
    {
        VIDDECODER_EPRINT("Incorrect Color format %x\n",pComponentPrivate->pOutPortDef->format.video.eColorFormat);
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pCreatePhaseArgs->ulMaxFrameRate            = 0;
    pCreatePhaseArgs->ulMaxBitRate              = -1;
    pCreatePhaseArgs->ulDataEndianness          = 1;
    pCreatePhaseArgs->ulProfile                 = 0;
    if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 700) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 480)) {
        pCreatePhaseArgs->lMaxLevel                = 5;
    }
    else {
        pCreatePhaseArgs->lMaxLevel                = -1;
    }
    pCreatePhaseArgs->ulProcessMode             = pComponentPrivate->ProcessMode;
    pCreatePhaseArgs->lPreRollBufConfig        = 0;
    pCreatePhaseArgs->endArgs                   = END_OF_CR_PHASE_ARGS;


    memcpy(arr, pCreatePhaseArgs, sizeof(MP2VDEC_SNCreatePhArg));
    lcml_dsp->pCrPhArgs = arr;
    cb.LCML_Callback = (void*) VIDDEC_LCML_Callback;
    free(pCreatePhaseArgs);
    pCreatePhaseArgs = NULL;

    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
        pComponentPrivate->pLCML != NULL){
        eError = LCML_InitMMCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, NULL, &pLcmlHandle, NULL, &cb);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT ("LCML_InitMMCodec Failed!...%x\n",eError);
            eError = OMX_ErrorHardware;
            goto EXIT;
        }
    }
    else {
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/*-------------------------------------------------------------------*/
/**
  *  Function to fill DSP structures via LCML
  *
  *
  *
  * @retval OMX_NoError              Success, ready to roll
  *
  **/
/*-------------------------------------------------------------------*/

OMX_ERRORTYPE VIDDEC_InitDSP_SparkDec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
    LCML_DSP *lcml_dsp = NULL;
    OMX_U32 nInpBuff = MAX_PRIVATE_IN_BUFFERS;
    OMX_U32 nInpBuffSize = 0;
    OMX_U32 nOutBuff = MAX_PRIVATE_OUT_BUFFERS;
    OMX_U32 nOutBuffSize = 0;
    SPARKVD_GPP_SN_Obj_CreatePhase* pCreatePhaseArgs = NULL;
    OMX_U16 arr[100];
    LCML_CALLBACKTYPE cb;

    VIDDECODER_DPRINT("+++ENTERING\n");
    /* Get number of input and output buffers */
    nInpBuff = pComponentPrivate->pInPortDef->nBufferCountActual;
    nOutBuff = pComponentPrivate->pOutPortDef->nBufferCountActual;

    /* Back it up for further use in this function */
    nInpBuffSize = pComponentPrivate->pInPortDef->nBufferSize;
    nOutBuffSize = pComponentPrivate->pOutPortDef->nBufferSize;

    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    lcml_dsp = (((LCML_DSP_INTERFACE*)pLcmlHandle)->dspCodec);

    lcml_dsp->DeviceInfo.TypeofDevice = 0;
    lcml_dsp->DeviceInfo.DspStream    = NULL;

    lcml_dsp->In_BufInfo.nBuffers      = nInpBuff;
    lcml_dsp->In_BufInfo.nSize         = nInpBuffSize;
    lcml_dsp->In_BufInfo.DataTrMethod  = DMM_METHOD;

    lcml_dsp->Out_BufInfo.nBuffers     = nOutBuff;
    lcml_dsp->Out_BufInfo.nSize        = nOutBuffSize;
    lcml_dsp->Out_BufInfo.DataTrMethod = DMM_METHOD;

    lcml_dsp->NodeInfo.nNumOfDLLs       = OMX_SPARKDEC_NUM_DLLS;
    lcml_dsp->NodeInfo.AllUUIDs[0].uuid = (struct DSP_UUID *)&SPARKDSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[0].DllName,(char*)SPARK_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[0].eDllType = DLL_NODEOBJECT;

    lcml_dsp->NodeInfo.AllUUIDs[1].uuid = (struct DSP_UUID *)&SPARKDSOCKET_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[1].DllName,(char*)SPARK_DEC_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[1].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[2].uuid = (struct DSP_UUID *)&RINGIO_TI_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[2].DllName,(char*)RINGIO_NODE_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[2].eDllType = DLL_DEPENDENT;

    lcml_dsp->NodeInfo.AllUUIDs[3].uuid = (struct DSP_UUID *)&USN_UUID;
    strcpy ((char*)lcml_dsp->NodeInfo.AllUUIDs[3].DllName,(char*)USN_DLL);
    lcml_dsp->NodeInfo.AllUUIDs[3].eDllType = DLL_DEPENDENT;

    lcml_dsp->SegID     = 0;
    lcml_dsp->Timeout   = -1;
    lcml_dsp->Alignment = 0;
    lcml_dsp->Priority  = 5;


    if(pComponentPrivate->ProcessMode == 0){
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 640) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 480)) {
            lcml_dsp->ProfileID = 4;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 640) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 480) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288))) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }
    else if(pComponentPrivate->ProcessMode == 1) {
        if ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 640) ||
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 480)) {
            lcml_dsp->ProfileID = 4;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 640) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 352)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 480) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 288))) {
            lcml_dsp->ProfileID = 3;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 352) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth > 176)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 288) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight > 144))) {
            lcml_dsp->ProfileID = 2;
        }
        else if (((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth <= 176) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth >= 16)) ||
            ((OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight <= 144) &&
            (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight >= 16))) {
            lcml_dsp->ProfileID = 1;
        }
        else {
            eError = OMX_ErrorUnsupportedSetting;
            goto EXIT;
        }
    }

    OMX_MALLOC_STRUCT(pCreatePhaseArgs, SPARKVD_GPP_SN_Obj_CreatePhase,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
    if (pCreatePhaseArgs == NULL) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    pCreatePhaseArgs->unNumOfStreams            = 2;
    pCreatePhaseArgs->unInputStreamID           = 0;
    pCreatePhaseArgs->unInputBufferType         = 0;
    pCreatePhaseArgs->unlInputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pInPortDef->nBufferCountActual);
    pCreatePhaseArgs->unOutputStreamID          = 1;
    pCreatePhaseArgs->unOutputBufferType        = 0;
    pCreatePhaseArgs->unOutputNumBufsPerStream  = (OMX_U16)(pComponentPrivate->pOutPortDef->nBufferCountActual);
    pCreatePhaseArgs->ulMaxWidth                = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameWidth);
    pCreatePhaseArgs->ulMaxHeight               = (OMX_U16)(pComponentPrivate->pOutPortDef->format.video.nFrameHeight);

    if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT422) {
        pCreatePhaseArgs->ulYUVFormat           = SPARKVIDDEC_YUVFORMAT_INTERLEAVED422;
    }
    else if (pComponentPrivate->pOutPortDef->format.video.eColorFormat == VIDDEC_COLORFORMAT420) {
        pCreatePhaseArgs->ulYUVFormat           = SPARKVIDDEC_YUVFORMAT_PLANAR420;
    }
    else
    {
        VIDDECODER_EPRINT("Incorrect Color format %x\n",pComponentPrivate->pOutPortDef->format.video.eColorFormat);
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pCreatePhaseArgs->ulMaxFrameRate            = 0;
    pCreatePhaseArgs->ulMaxBitRate              = -1;
    pCreatePhaseArgs->ulDataEndianness          = 1;
    pCreatePhaseArgs->ulProfile                 = 0;
    pCreatePhaseArgs->ulMaxLevel                = -1;
    pCreatePhaseArgs->ulProcessMode             = pComponentPrivate->ProcessMode;
    pCreatePhaseArgs->ulPreRollBufConfig        = 0;
    pCreatePhaseArgs->endArgs                   = END_OF_CR_PHASE_ARGS;

    memcpy(arr, pCreatePhaseArgs, sizeof(SPARKVD_GPP_SN_Obj_CreatePhase));
    lcml_dsp->pCrPhArgs = arr;
    cb.LCML_Callback = (void*) VIDDEC_LCML_Callback;
    free(pCreatePhaseArgs);
    pCreatePhaseArgs = NULL;

    if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
        pComponentPrivate->pLCML != NULL){
        eError = LCML_InitMMCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, NULL, &pLcmlHandle, NULL, &cb);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT ("LCML_InitMMCodec Failed!...%x\n",eError);
            eError = OMX_ErrorHardware;
            goto EXIT;
        }
    }
    else {
        eError = OMX_ErrorHardware;
        goto EXIT;
    }
EXIT:
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

/* ========================================================================== */
/**
  *  VIDDEC_Handle_InvalidState() Function called for a non recoverable error
  *
  * @param pComponentPrivate         This is the pointer to the private structure
  *
  * @retval OMX_NoError              Success, ready to roll
  *         OMX_ErrorHardware        If OMX_StateInvalid is the actual state
 **/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_Handle_InvalidState (VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    VIDDECODER_DPRINT("+++ENTERING\n");
    VIDDECODER_DPRINT("pComponentPrivate 0x%x\n", (int*)pComponentPrivate);

    if(pComponentPrivate->eState != OMX_StateInvalid) {
        pComponentPrivate->eState = OMX_StateInvalid;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               OMX_ErrorInvalidState,
                                               OMX_TI_ErrorCritical,
                                               "Transitioning to Invalid State");
        eError = OMX_ErrorNone;
    }
    else {
        eError = OMX_ErrorHardware;
    }
    VIDDECODER_DPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}


/* ========================================================================== */
/**
  *  VIDDEC_PROPAGATE_MARK() Function called for propagate mark from input buffers to output buffers
  *
  * @param pComponentPrivate         This is the pointer to the private structure
  * @param pBuffHead                       This is the pointer to the output buffer
  *
  * @retval OMX_NoError              Success
  *         OMX_ErrorUnsupportedSetting        If CompressionFormat has not a valid value
 **/
/* ========================================================================== */
OMX_ERRORTYPE VIDDEC_Propagate_Mark(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate, OMX_BUFFERHEADERTYPE *pBuffHead)
{
    OMX_U32 BytesSent;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    if(pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263 ||
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4)
    {
        pComponentPrivate->nBytesConsumed =
            ((MP4VD_GPP_SN_UALGOutputParams *)(((VIDDEC_BUFFER_PRIVATE *)(pBuffHead->pOutputPortPrivate))->pUalgParam))->uBytesConsumed;
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC)
    {
        pComponentPrivate->nBytesConsumed =
            ((H264VDEC_UALGOutputParam *)(((VIDDEC_BUFFER_PRIVATE *)(pBuffHead->pOutputPortPrivate))->pUalgParam))->ulBytesConsumed;
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV)
    {
        pComponentPrivate->nBytesConsumed =
            ((WMV9DEC_UALGOutputParam *)(((VIDDEC_BUFFER_PRIVATE *)(pBuffHead->pOutputPortPrivate))->pUalgParam))->ulBytesConsumed;
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2)
    {
        pComponentPrivate->nBytesConsumed =
            ((MP2VDEC_UALGOutputParam *)(((VIDDEC_BUFFER_PRIVATE *)(pBuffHead->pOutputPortPrivate))->pUalgParam))->ulBytesConsumed;
    }
#ifdef VIDDEC_SPARK_CODE
    else if (VIDDEC_SPARKCHECK) {
        pComponentPrivate->nBytesConsumed =
            ((SPARKVD_GPP_SN_UALGOutputParams *)(((VIDDEC_BUFFER_PRIVATE *)(pBuffHead->pOutputPortPrivate))->pUalgParam))->uBytesConsumed;
    }
#endif
    else
    {
        pComponentPrivate->nBytesConsumed = 0;
        eError = OMX_ErrorUnsupportedSetting;
        goto EXIT;
    }

    pComponentPrivate->bBuffalreadyMarked = OMX_FALSE;
    /* difference with other marks like nFlags mark is erased each time */
    pComponentPrivate->MTbuffMark.hMarkTargetComponent = NULL;
    pComponentPrivate->MTbuffMark.pMarkData = NULL;
    pBuffHead->hMarkTargetComponent = NULL;
    pBuffHead->pMarkData = NULL;

    while(pComponentPrivate->qBytesSent.nElements != 0)
    {
        if(pComponentPrivate->bBuffMarkTaked == OMX_FALSE)
        {
            VIDDEC_Queue_Remove(&(pComponentPrivate->qBuffMark), &(pComponentPrivate->MTbuffMark), VIDDEC_QUEUE_OMX_MARKTYPE);
            pComponentPrivate->bBuffMarkTaked = OMX_TRUE;
        }
        if(pComponentPrivate->bBuffalreadyMarked == OMX_FALSE)
        {
            if(pComponentPrivate->MTbuffMark.hMarkTargetComponent)
            {
                pBuffHead->hMarkTargetComponent =
                    pComponentPrivate->MTbuffMark.hMarkTargetComponent;
                pBuffHead->pMarkData =
                    pComponentPrivate->MTbuffMark.pMarkData;
                pComponentPrivate->bBuffalreadyMarked = OMX_TRUE;
            }
        }
        VIDDEC_Queue_Get_Tail(&(pComponentPrivate->qBytesSent), &BytesSent, VIDDEC_QUEUE_OMX_U32);
        if(pComponentPrivate->nBytesConsumed >= BytesSent)
        {
            pComponentPrivate->nBytesConsumed -= BytesSent;
            pComponentPrivate->bBuffMarkTaked = OMX_FALSE;
            VIDDEC_Queue_Remove(&(pComponentPrivate->qBytesSent), NULL, VIDDEC_QUEUE_OMX_U32);
        }
        else
        {
            BytesSent -= pComponentPrivate->nBytesConsumed;
            if(BytesSent == 0)
            {
                pComponentPrivate->bBuffMarkTaked = OMX_FALSE;
                VIDDEC_Queue_Remove(&(pComponentPrivate->qBytesSent), NULL, VIDDEC_QUEUE_OMX_U32);
            }
            else
            {
                VIDDEC_Queue_Replace_Tail(&(pComponentPrivate->qBytesSent), &BytesSent, VIDDEC_QUEUE_OMX_U32);
            }
            break;
        }
    }
    if(pBuffHead->hMarkTargetComponent == pComponentPrivate->pHandle)
    {
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                        pComponentPrivate->pHandle->pApplicationPrivate,
                        OMX_EventMark,
                        0,
                        0,
                        pBuffHead->pMarkData);
    }

EXIT:
    return eError;
}



/* ========================================================================== */
/**
  *  Callback() function will be called LCML component to write the msg
  *
  * @param msgBuffer                 This buffer will be returned by the LCML
  *
  * @retval OMX_NoError              Success, ready to roll
  *         OMX_Error_BadParameter   The input parameter pointer is null
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_LCML_Callback (TUsnCodecEvent event,void * argsCb [10])
{
    VIDDEC_COMPONENT_PRIVATE* pComponentPrivate = NULL;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    VIDDEC_BUFFER_PRIVATE* pBufferPrivate = NULL;
    OMX_S32 nRetVal = 0;

#if 0
    switch(event) {
        case EMMCodecDspError:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecDspError\n");
            break;

        case EMMCodecInternalError:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecInternalError\n");
            break;

        case EMMCodecInitError:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecInitError\n");
            break;

        case EMMCodecDspMessageRecieved:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecDspMessageRecieved\n");
            break;
        case EMMCodecBufferProcessed:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecBufferProcessed %x\n",(int)argsCb [0]);
            break;
        case EMMCodecProcessingStarted:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecProcessingStarted\n");
            break;

        case EMMCodecProcessingPaused:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecProcessingPaused\n");
            break;

        case EMMCodecProcessingStoped:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecProcessingStoped\n");
            break;

        case EMMCodecProcessingEof:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecProcessingEof\n");
            break;
        case EMMCodecBufferNotProcessed:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecBufferNotProcessed %x\n",(int)argsCb [0]);
            break;
        case EMMCodecAlgCtrlAck:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecAlgCtrlAck\n");
            break;

        case EMMCodecStrmCtrlAck:
            VIDDECODER_DPRINT("[LCML CALLBACK EVENT]  EMMCodecStrmCtrlAck\n");
            break;
    }
#endif

    VIDDEC_BUFFERPRINT("+++ENTERING\n");
    pComponentPrivate = (VIDDEC_COMPONENT_PRIVATE*)((LCML_DSP_INTERFACE*)argsCb[6])->pComponentPrivate;
    VIDDEC_BUFFERPRINT("pComponentPrivate 0x%x 0x%x\n", (int*)pComponentPrivate,event);

    if(pComponentPrivate->pCompPort[0] == NULL){
        VIDDECODER_EPRINT("*****************************error in lcmlcalback******************************\n");
        goto EXIT;
    }
    if (event == EMMCodecProcessingPaused) {
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_SIGNAL(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        pComponentPrivate->bTransPause = 1;
    }
    if (event == EMMCodecAlgCtrlAck) {
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_SIGNAL(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        pComponentPrivate->bTransPause = 1;
    }
    if (event == EMMCodecProcessingStoped) {
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_SIGNAL(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        pComponentPrivate->bTransPause = 1;
        pComponentPrivate->bIsPaused = 0;
    }
    if (event == EMMCodecProcessingStarted) {
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_SIGNAL(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        pComponentPrivate->bTransPause = 1;
        pComponentPrivate->bIsPaused = 0;
    }
    if (event == EMMCodecBufferProcessed) {
        VIDDEC_BUFFERPRINT("EMMCodecBufferProcessed 0x%x\n", (OMX_U32)argsCb [0]);
        if ((OMX_U32)argsCb [0] == EMMCodecOuputBuffer) {
            OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
            VIDDEC_BUFFER_PRIVATE* pBuffPriv = NULL;
            OMX_U8* pBuffer;
            OMX_PARAM_PORTDEFINITIONTYPE* pPortDefOut = pComponentPrivate->pOutPortDef;

        if (pComponentPrivate->eState != OMX_StateLoaded && pComponentPrivate->eState != OMX_StateIdle) {
            pBuffHead = (OMX_BUFFERHEADERTYPE*)argsCb[7];
            if(pBuffHead != NULL)
            {
                VIDDEC_BUFFERPRINT("pBuffHead Output 0x%x pBuffer 0x%x\n", (int*)pBuffHead, (int*)argsCb[1]);
                pBuffPriv = (VIDDEC_BUFFER_PRIVATE*)pBuffHead->pOutputPortPrivate;
                if (pBuffPriv != NULL) {
                    if(pBuffPriv->eBufferOwner != VIDDEC_BUFFER_WITH_CLIENT)
                    {
#ifdef __PERF_INSTRUMENTATION__
                        pComponentPrivate->lcml_nCntOpReceived++;
                        if (pComponentPrivate->lcml_nCntOpReceived == 4) {
                            PERF_Boundary(pComponentPrivate->pPERFcomp,
                                          PERF_BoundaryStart | PERF_BoundarySteadyState);
                        }
#endif
                        pBuffer = (OMX_U8*)argsCb[1];
                        /* Retrieve time stamp information */
                        if (pComponentPrivate->ProcessMode == 0) {
                            OMX_PTR pBufferFlags;
                            VIDDEC_CircBuf_Remove(pComponentPrivate,
                                               VIDDEC_CBUFFER_TIMESTAMP,
                                               VIDDEC_INPUT_PORT,
                                               &pBufferFlags);
                            if(pBufferFlags != NULL){
                                pBuffHead->nTimeStamp = (OMX_TICKS)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nTimeStamp;
                                /*pBuffHead->nFlags = 0;
                                pBuffHead->nFlags = (OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nFlags;*/
                                pBuffHead->nFlags |= (((OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nFlags) & OMX_BUFFERFLAG_DECODEONLY);
                                pBuffHead->nTickCount = (OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nTickCount;
                            }
                            else {
                                pBuffHead->nTimeStamp = 0;
                                pBuffHead->nTickCount = 0;
                            }
                        }
                        else {
                            pBuffHead->nTimeStamp = (OMX_TICKS)pComponentPrivate->arrBufIndex[pComponentPrivate->nOutBufIndex];
                            pComponentPrivate->nOutBufIndex++;
                            pComponentPrivate->nOutBufIndex %= pPortDefOut->nBufferCountActual;
                        }
                        VIDDEC_BUFFERPRINT("pBuffHead->nTimeStamp %lld\n", pBuffHead->nTimeStamp);
                        if(pBuffHead != NULL){
                            /*if (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                                pBuffHead->nFilledLen = (OMX_S32)argsCb[2];
                            }
                            else {*/
                                pBuffHead->nFilledLen = (OMX_S32)argsCb[8];
                            /*}*/
                            VIDDEC_BUFFERPRINT("pBuffHead->nFilledLen %lld\n", pBuffHead->nFilledLen);
                            pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pOutputPortPrivate;
                            pComponentPrivate->nOutputBCountDsp++;
                            pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
                            VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
                            PERF_ReceivedFrame(pComponentPrivate->pPERFcomp,
                                               pBuffHead->pBuffer,
                                               pBuffHead->nFilledLen,
                                               PERF_ModuleCommonLayer);
#endif

#ifdef VIDDEC_FLAGGED_EOS
                            if (pComponentPrivate->bUseFlaggedEos) {
                                if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS)) {
                                    pComponentPrivate->bPlayCompleted = OMX_TRUE;
                                }
                            }
#endif

                            nRetVal = write(pComponentPrivate->filled_outBuf_Q[1],&pBuffHead,sizeof(pBuffHead));
                            if(nRetVal == -1){
                                VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,nRetVal);
                                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                                       OMX_EventError,
                                                                       OMX_ErrorInsufficientResources,
                                                                       OMX_TI_ErrorSevere,
                                                                       "Error writing to the output pipe");
                            }
                        }
                    }
                    else {
                        VIDDECODER_DPRINT("buffer dropped lcml out process pBuffHead %x owner %d\n",pBuffHead,pBuffPriv->eBufferOwner);
                    }
                }
            }
        }
        }
        if ((OMX_U32)argsCb [0] == EMMCodecInputBuffer) {
            OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
            VIDDEC_BUFFER_PRIVATE* pBuffPriv = NULL;
            OMX_U8* pBuffer;
            if (pComponentPrivate->eState != OMX_StateLoaded && pComponentPrivate->eState != OMX_StateIdle) {
            pBuffHead = (OMX_BUFFERHEADERTYPE*)argsCb[7];
            if(pBuffHead != NULL)
            {
                pBuffPriv = (VIDDEC_BUFFER_PRIVATE*)pBuffHead->pInputPortPrivate;
                if (pBuffPriv == NULL ) {
                    goto EXIT;
                }
                else {
                    if(pBuffPriv->eBufferOwner != VIDDEC_BUFFER_WITH_CLIENT)
                    {
                        VIDDEC_BUFFERPRINT("pBuffHead Input 0x%x pBuffer 0x%x\n", (int*)pBuffHead, (int*)argsCb[1]);
                        pBuffer = (OMX_U8*)argsCb[1];
                        if(pBuffer != NULL){
                            pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pInputPortPrivate;
                            pComponentPrivate->nInputBCountDsp++;
                            pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
                            VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                            PERF_ReceivedFrame(pComponentPrivate->pPERFcomp,
                                               PREF(pBuffHead,pBuffer),
                                               PREF(pBuffHead,nFilledLen),
                                               PERF_ModuleCommonLayer);
    #endif
                            pBuffHead->nFilledLen = 0;
                            nRetVal = write(pComponentPrivate->free_inpBuf_Q[1], &pBuffHead, sizeof(pBuffHead));
                            if(nRetVal == -1){
                                VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,nRetVal);
                                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                                       OMX_EventError,
                                                                       OMX_ErrorInsufficientResources,
                                                                       OMX_TI_ErrorSevere,
                                                                       "Error writing to the output pipe");
                            }
                        }
                    }
                    else {
                        VIDDECODER_DPRINT("buffer dropped lcml in notprocess pBuffHead %x owner %d\n",pBuffHead,pBuffPriv->eBufferOwner);
                    }
                }
            }
         }
        }
	}
    /************************************************************************************************/
    if (event == EMMCodecBufferNotProcessed) {
        if ((OMX_U32)argsCb [0] == EMMCodecOuputBuffer) {
            OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
            VIDDEC_BUFFER_PRIVATE* pBuffPriv = NULL;
            OMX_U8* pBuffer;
            OMX_PARAM_PORTDEFINITIONTYPE* pPortDefOut = pComponentPrivate->pOutPortDef;

        if (pComponentPrivate->eState != OMX_StateLoaded && pComponentPrivate->eState != OMX_StateIdle) {
            pBuffHead = (OMX_BUFFERHEADERTYPE*)argsCb[7];
            if(pBuffHead != NULL)
            {
                pBuffPriv = (VIDDEC_BUFFER_PRIVATE*)pBuffHead->pOutputPortPrivate;
                if (pBuffPriv != NULL) {
                    if(pBuffPriv->eBufferOwner != VIDDEC_BUFFER_WITH_CLIENT)
                    {
#ifdef __PERF_INSTRUMENTATION__
                        pComponentPrivate->lcml_nCntOpReceived++;
                        if (pComponentPrivate->lcml_nCntOpReceived == 4) {
                            PERF_Boundary(pComponentPrivate->pPERFcomp,
                                          PERF_BoundaryStart | PERF_BoundarySteadyState);
                        }
#endif
                        pBuffer = (OMX_U8*)argsCb[1];
                        /* Retrieve time stamp information */
                        if (pComponentPrivate->ProcessMode == 0) {
                            OMX_PTR pBufferFlags;
                            VIDDEC_CircBuf_Remove(pComponentPrivate,
                                               VIDDEC_CBUFFER_TIMESTAMP,
                                               VIDDEC_INPUT_PORT,
                                               &pBufferFlags);
                            if(pBufferFlags != NULL){
                                pBuffHead->nTimeStamp = (OMX_TICKS)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nTimeStamp;
                                /*pBuffHead->nFlags = 0;
                                pBuffHead->nFlags = (OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nFlags;*/
                                pBuffHead->nFlags |= (((OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nFlags) & OMX_BUFFERFLAG_DECODEONLY);
                                pBuffHead->nTickCount = (OMX_U32)((VIDDEC_CBUFFER_BUFFERFLAGS*)pBufferFlags)->nTickCount;
                            }
                            else {
                                pBuffHead->nTimeStamp = 0;
                                pBuffHead->nTickCount = 0;
                            }
                        }
                        else {
                            pBuffHead->nTimeStamp = (OMX_TICKS)pComponentPrivate->arrBufIndex[pComponentPrivate->nOutBufIndex];
                            pComponentPrivate->nOutBufIndex++;
                            pComponentPrivate->nOutBufIndex %= pPortDefOut->nBufferCountActual;
                        }
                        VIDDEC_BUFFERPRINT("pBuffHead->nTimeStamp %lld\n", pBuffHead->nTimeStamp);
                        if(pBuffHead != NULL){
                            /*if (pComponentPrivate->pCompPort[1]->hTunnelComponent != NULL) {
                                pBuffHead->nFilledLen = (OMX_S32)argsCb[2];
                            }
                            else {*/
                                pBuffHead->nFilledLen = (OMX_S32)argsCb[8];
                            /*}*/
                            VIDDEC_BUFFERPRINT("pBuffHead->nFilledLen %lld\n", pBuffHead->nFilledLen);
                            pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pOutputPortPrivate;
                            pComponentPrivate->nOutputBCountDsp++;
                            pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
                            VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
#ifdef __PERF_INSTRUMENTATION__
                            PERF_ReceivedFrame(pComponentPrivate->pPERFcomp,
                                               pBuffHead->pBuffer,
                                               pBuffHead->nFilledLen,
                                               PERF_ModuleCommonLayer);
#endif
#ifdef VIDDEC_FLAGGED_EOS
                            if (pComponentPrivate->bUseFlaggedEos) {
                                if ((pBuffHead->nFlags & OMX_BUFFERFLAG_EOS)) {
                                    pComponentPrivate->bPlayCompleted = OMX_TRUE;
                                }
                            }
#endif
                            nRetVal = write(pComponentPrivate->filled_outBuf_Q[1],&pBuffHead,sizeof(pBuffHead));
                            if(nRetVal == -1){
                                VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,nRetVal);
                                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                                       OMX_EventError,
                                                                       OMX_ErrorInsufficientResources,
                                                                       OMX_TI_ErrorSevere,
                                                                       "Error writing to the output pipe");
                            }
                        }
                    }
                    else {
                        VIDDECODER_DPRINT("buffer dropped lcml out process pBuffHead %x owner %d\n",pBuffHead,pBuffPriv->eBufferOwner);
                    }
                }
            }
        }
        }
        if ((OMX_U32)argsCb [0] == EMMCodecInputBuffer) {
            OMX_BUFFERHEADERTYPE* pBuffHead = NULL;
            VIDDEC_BUFFER_PRIVATE* pBuffPriv = NULL;
            OMX_U8* pBuffer;
            if (pComponentPrivate->eState != OMX_StateLoaded && pComponentPrivate->eState != OMX_StateIdle) {
            pBuffHead = (OMX_BUFFERHEADERTYPE*)argsCb[7];
            if(pBuffHead != NULL)
            {
                pBuffPriv = (VIDDEC_BUFFER_PRIVATE*)pBuffHead->pInputPortPrivate;
                if (pBuffPriv == NULL ) {
                    goto EXIT;
                }
                else {
                    if(pBuffPriv->eBufferOwner != VIDDEC_BUFFER_WITH_CLIENT)
                    {
                        VIDDEC_BUFFERPRINT("pBuffHead Input 0x%x pBuffer 0x%x\n", (int*)pBuffHead, (int*)argsCb[1]);
                        pBuffer = (OMX_U8*)argsCb[1];
                        if(pBuffer != NULL){
                            pBufferPrivate = (VIDDEC_BUFFER_PRIVATE* )pBuffHead->pInputPortPrivate;
                            pComponentPrivate->nInputBCountDsp++;
                            pBufferPrivate->eBufferOwner = VIDDEC_BUFFER_WITH_COMPONENT;
                            VIDDEC_BUFFERPRINT("eBufferOwner 0x%x\n", pBufferPrivate->eBufferOwner);
    #ifdef __PERF_INSTRUMENTATION__
                            PERF_ReceivedFrame(pComponentPrivate->pPERFcomp,
                                               PREF(pBuffHead,pBuffer),
                                               PREF(pBuffHead,nFilledLen),
                                               PERF_ModuleCommonLayer);
    #endif
                            pBuffHead->nFilledLen = 0;
                            nRetVal = write(pComponentPrivate->free_inpBuf_Q[1], &pBuffHead, sizeof(pBuffHead));
                            if(nRetVal == -1){
                                VIDDEC_EPRINT("writing to the input pipe %x (%d)\n", OMX_ErrorInsufficientResources,nRetVal);
                                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                                       OMX_EventError,
                                                                       OMX_ErrorInsufficientResources,
                                                                       OMX_TI_ErrorSevere,
                                                                       "Error writing to the output pipe");
                            }
                        }
                    }
                    else {
                        VIDDECODER_DPRINT("buffer dropped lcml in notprocess pBuffHead %x owner %d\n",pBuffHead,pBuffPriv->eBufferOwner);
                    }
                }
            }
            }
        }
	}
    /************************************************************************************************/
    if (event == EMMCodecDspError) {
        if ((int)argsCb [0] == USN_DSPMSG_EVENT) {
            if ((int)argsCb [4] == USN_ERR_WARNING) {
                if ((int)argsCb [5] == IUALG_WARN_PLAYCOMPLETED) {
                    pComponentPrivate->bPlayCompleted = OMX_TRUE;
                    VIDDECODER_DPRINT("playcompleted %x\n", pComponentPrivate->bPlayCompleted);

                }
                else {
                    pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                           OMX_EventError,
                                           OMX_ErrorHardware,
                                           OMX_TI_ErrorCritical,
                                           "Error from the DSP");
                }
            }
            else {
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                           pComponentPrivate->pHandle->pApplicationPrivate,
                                           OMX_EventError,
                                           OMX_ErrorHardware,
                                           OMX_TI_ErrorCritical,
                                           "Error from the DSP");
            }
        }
        else {
            pComponentPrivate->bLCMLHalted = OMX_TRUE;
            pComponentPrivate->pHandle->SendCommand( pComponentPrivate->pHandle, OMX_CommandStateSet, -2, 0);

        }
    }
    if (event == EMMCodecInternalError || event == EMMCodecInitError) {
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               OMX_ErrorHardware,
                                               OMX_TI_ErrorCritical,
                                               "Error from the DSP");
    }
    if (event == EMMCodecStrmCtrlAck) {
        if ((int)argsCb [0] == USN_ERR_NONE) {
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_SIGNAL(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        }
    }

EXIT:
    VIDDEC_BUFFERPRINT("---EXITING(0x%x)\n",eError);
    return eError;
}

#ifdef RESOURCE_MANAGER_ENABLED
void VIDDEC_ResourceManagerCallback(RMPROXY_COMMANDDATATYPE cbData)
{

    /*OMX_COMMANDTYPE Cmd = OMX_CommandStateSet;
    OMX_U32 state ;
    OMX_COMPONENTTYPE *pHandle = (OMX_COMPONENTTYPE *)cbData.hComponent;*/
    VIDDEC_COMPONENT_PRIVATE *pCompPrivate = NULL;
    pCompPrivate = NULL;
}
#endif

#ifdef ANDROID
/* ========================================================================== */
/**
  *  VIDDEC_SaveBuffer() function will be use to copy a buffer at private space, to be used later by VIDDEC_CopyBuffer()
  *
  * @param 
  *		pComponentPrivate            Component private structure
  *		pBuffHead					 Header of the buffer to be store
  *
  * @retval OMX_ErrorNone              Success, ready to roll
  *         OMX_ErrorInsufficientResources   Not enough memory to save first buffer
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_SaveBuffer(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate,
                                     OMX_BUFFERHEADERTYPE* pBuffHead)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	pComponentPrivate->eFirstBuffer.bSaveFirstBuffer = OMX_TRUE;
	
	OMX_MALLOC_STRUCT_SIZED(pComponentPrivate->eFirstBuffer.pFirstBufferSaved, OMX_U8, pBuffHead->nFilledLen, NULL);
	memcpy(pComponentPrivate->eFirstBuffer.pFirstBufferSaved, pBuffHead->pBuffer, pBuffHead->nFilledLen);

	pComponentPrivate->eFirstBuffer.nFilledLen = pBuffHead->nFilledLen;
	
EXIT:
	return eError;
}



/* ========================================================================== */
/**
  *  VIDDEC_CopyBuffer() function will insert an the begining of pBuffer the buffer stored using VIDDEC_SaveBuffer() 
  *		and update nFilledLen of the buffer header
  *
  * @param 
  *		pComponentPrivate            Component private structure
  *		pBuffHead					 Header of the buffer to be store
  *
  * @retval OMX_ErrorNone              Success, ready to roll
  *			OMX_ErrorUndefined		 No buffer to be copy.
  *         OMX_ErrorInsufficientResources   Not enough memory to save buffer
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_CopyBuffer(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate,
                                     OMX_BUFFERHEADERTYPE* pBuffHead)
{
	OMX_ERRORTYPE eError = OMX_ErrorNone;
	if(pComponentPrivate->eFirstBuffer.bSaveFirstBuffer == OMX_FALSE){
		eError = OMX_ErrorUndefined;
		goto EXIT;
	}

	OMX_PTR pTemp = NULL;
	pComponentPrivate->eFirstBuffer.bSaveFirstBuffer = OMX_FALSE;
	OMX_MALLOC_STRUCT_SIZED(pTemp, OMX_U8, pBuffHead->nFilledLen, NULL);
	memcpy(pTemp, pBuffHead->pBuffer, pBuffHead->nFilledLen); /*copy somewere actual buffer*/
	memcpy(pBuffHead->pBuffer, pComponentPrivate->eFirstBuffer.pFirstBufferSaved, pComponentPrivate->eFirstBuffer.nFilledLen); /*copy first buffer to the beganing of pBuffer.*/
	memcpy(pBuffHead->pBuffer+((OMX_U8)pComponentPrivate->eFirstBuffer.nFilledLen), (OMX_U8 *)pTemp, pBuffHead->nFilledLen); /* copy back actual buffer after first buffer*/
	pBuffHead->nFilledLen += (OMX_U8)pComponentPrivate->eFirstBuffer.nFilledLen; /*Add first buffer size*/

	free(pTemp);
	free(pComponentPrivate->eFirstBuffer.pFirstBufferSaved);
	pComponentPrivate->eFirstBuffer.pFirstBufferSaved = NULL;
	
EXIT:
	return eError;
}
#endif


/* ========================================================================== */
/**
  *  VIDDEC_LoadCodec() function will get LCML resource and start the codec.
  *
  * @param 
  *		pComponentPrivate            Component private structure
  *
  * @retval OMX_ErrorNone              Success, ready to roll
  *			OMX_ErrorUndefined		 
  *         OMX_ErrorInsufficientResources   Not enough memory
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_LoadCodec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
	OMX_ERRORTYPE eError = OMX_ErrorUndefined;
	OMX_U32 message[4];
    LCML_DSP_INTERFACE *pLcmlHandle = NULL;
	OMX_HANDLETYPE hLCML = NULL;
    void* p = NULL;
#ifdef UNDER_CE
   typedef OMX_ERRORTYPE (*LPFNDLLFUNC1)(OMX_HANDLETYPE);
   HINSTANCE hDLL;
   LPFNDLLFUNC1 fpGetHandle1;
#else
   VIDDEC_fpo fpGetHandle;
   char* error;
#endif
	
#ifndef UNDER_CE
    pComponentPrivate->pModLCML = dlopen("libLCML.so", RTLD_LAZY);
    if (!pComponentPrivate->pModLCML) {
        VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
        fputs(dlerror(), stderr);
        eError = OMX_ErrorBadParameter;
        goto EXIT;
    }
    fpGetHandle = dlsym(pComponentPrivate->pModLCML, "GetHandle");
    if ((error = dlerror()) != NULL) {
        VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
        fputs(error, stderr);
        dlclose(pComponentPrivate->pModLCML);
        pComponentPrivate->pModLCML = NULL;
        eError = OMX_ErrorBadParameter;
        goto EXIT;
    }
    eError = (*fpGetHandle)(&hLCML);
    if (eError != OMX_ErrorNone) {
        VIDDECODER_EPRINT("OMX_ErrorBadParameter\n");
        dlclose(pComponentPrivate->pModLCML);
        pComponentPrivate->pModLCML = NULL;
        eError = OMX_ErrorBadParameter;
        goto EXIT;
    }

#endif

    pComponentPrivate->eLCMLState = VidDec_LCML_State_Load;
    VIDDECODER_DPRINT("LCML Handler 0x%x\n",hLCML);
    /*(LCML_DSP_INTERFACE*)pComponentPrivate->pLCML = (LCML_DSP_INTERFACE*)hLCML;*/
    pComponentPrivate->pLCML = (LCML_DSP_INTERFACE*)hLCML;
    pComponentPrivate->pLCML->pComponentPrivate = pComponentPrivate;


#ifdef __PERF_INSTRUMENTATION__
    pComponentPrivate->lcml_nCntOpReceived = 0;
#endif
    eError = OMX_ErrorNone;
#ifndef UNDER_CE
    pComponentPrivate->bLCMLOut = OMX_TRUE;
#endif
    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
        eError = VIDDEC_InitDSP_H264Dec(pComponentPrivate);
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG4 ||
             pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingH263) {
        eError = VIDDEC_InitDSP_Mpeg4Dec(pComponentPrivate);
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2) {
        eError = VIDDEC_InitDSP_Mpeg2Dec(pComponentPrivate);
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV) {
        eError = VIDDEC_InitDSP_WMVDec(pComponentPrivate);
    }
#ifdef VIDDEC_SPARK_CODE
    else if (VIDDEC_SPARKCHECK) {
        eError = VIDDEC_InitDSP_SparkDec(pComponentPrivate);
    }
#endif
    else {
        VIDDECODER_EPRINT("OMX_ErrorUnsupportedSetting\n");
        eError = OMX_ErrorUnsupportedSetting;
        pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                               pComponentPrivate->pHandle->pApplicationPrivate,
                                               OMX_EventError,
                                               OMX_ErrorUnsupportedSetting,
                                               OMX_TI_ErrorMinor,
                                               "DSP Initialization");
        goto EXIT;
    }
	#ifndef UNDER_CE
    pComponentPrivate->bLCMLOut = OMX_FALSE;
#endif
    pComponentPrivate->bLCMLHalted = OMX_FALSE;
    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    pComponentPrivate->eLCMLState = VidDec_LCML_State_Init;

#ifdef VIDDEC_FLAGGED_EOS
    if (pComponentPrivate->bUseFlaggedEos) {
        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
            pComponentPrivate->pLCML != NULL &&
            pComponentPrivate->bLCMLHalted != OMX_TRUE){
            VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlUsnEos 0x%x\n",pLcmlHandle);
            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlUsnEos, NULL);
            if (eError != OMX_ErrorNone) {
                VIDDECODER_EPRINT("error in EMMCodecControlUsnEos %x\n",eError);
                eError = OMX_ErrorHardware;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       OMX_ErrorHardware,
                                                       OMX_TI_ErrorSevere,
                                                       "LCML_ControlCodec EMMCodecControlUsnEos function");
                VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
                goto EXIT;
            }
        }
    }
#endif
   if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat != OMX_VIDEO_CodingWMV) {
        if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC) {
            message[1] = 4;
        }
        else {
            message[1] = 100;
        }
        message[0] = 0x400;
        message[2] = 0;
        p = (void*)&message;
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlSendDspMessage 0x%x\n",pLcmlHandle);
        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlSendDspMessage, (void *)p);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT("error in EMMCodecControlSendDspMessage %x\n",eError);
            eError = OMX_ErrorHardware;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   OMX_ErrorHardware,
                                                   OMX_TI_ErrorSevere,
                                                   "LCML_ControlCodec function");
            VIDDECODER_EPRINT("OMX_ErrorHardware 0x%x\n",eError);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
            goto EXIT;
        }
        VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
   }

    pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
    pComponentPrivate->bIsPaused = 0;
    pComponentPrivate->bFirstBuffer = 1;

        VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlStart 0x%x\n",pLcmlHandle);
        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,EMMCodecControlStart,NULL);
        if (eError != OMX_ErrorNone) {
            eError = OMX_ErrorHardware;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   OMX_ErrorHardware,
                                                   OMX_TI_ErrorSevere,
                                                   "LCML_ControlCodec Start");
            goto EXIT;
            VIDDECODER_EPRINT("Occurred in Codec Start... 0x%x\n",eError);
        	}
    pComponentPrivate->eLCMLState = VidDec_LCML_State_Start;

    if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingAVC &&
        pComponentPrivate->eState == OMX_StateIdle) {
        H264_Iualg_Cmd_SetStatus* pDynParams = NULL;
        char* pTmp = NULL;
        OMX_U32 cmdValues[3] = {0, 0, 0};

        /*VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);*/
        VIDDECODER_DPRINT("Initializing DSP for h264 eCompressionFormat 0x%x\n",
        pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
        OMX_MALLOC_STRUCT_SIZED(pDynParams, H264_Iualg_Cmd_SetStatus, sizeof(H264_Iualg_Cmd_SetStatus) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
        if (pDynParams == NULL) {
           VIDDECODER_EPRINT("Error: Malloc failed\n");
           eError = OMX_ErrorInsufficientResources;
           goto EXIT;
       }
        memset(pDynParams, 0, sizeof(H264_Iualg_Cmd_SetStatus) + VIDDEC_PADDING_FULL);
        pTmp = (char*)pDynParams;
        pTmp += VIDDEC_PADDING_HALF;
        pDynParams = (H264_Iualg_Cmd_SetStatus*)pTmp;
#ifdef VIDDEC_SN_R8_14
        pDynParams->size = sizeof(H264_Iualg_Cmd_SetStatus);
#endif
        pDynParams->ulDecodeHeader = 1;
        pDynParams->ulDisplayWidth = 0;
        pDynParams->ulFrameSkipMode = 0;
        pDynParams->ulPPType = 0;

        cmdValues[0] = IUALG_CMD_SETSTATUS;
        cmdValues[1] = (OMX_U32)(pDynParams);
        cmdValues[2] = sizeof(H264_Iualg_Cmd_SetStatus);

        p = (void*)&cmdValues;
        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
            pComponentPrivate->pLCML != NULL &&
            pComponentPrivate->bLCMLHalted != OMX_TRUE){
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                       EMMCodecControlAlgCtrl,
                                       (void*)p);
            if (eError != OMX_ErrorNone) {
                eError = OMX_ErrorHardware;
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                goto EXIT;
            }
            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        }
        pTmp = (char*)pDynParams;
        pTmp -= VIDDEC_PADDING_HALF;
        pDynParams = (H264_Iualg_Cmd_SetStatus*)pTmp;
        free(pDynParams);

        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
            goto EXIT;
        }
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingWMV &&
        pComponentPrivate->eState == OMX_StateIdle) {
        WMV9DEC_UALGDynamicParams* pDynParams = NULL;
        char* pTmp = NULL;
        OMX_U32 cmdValues[3] = {0, 0, 0};

        /*VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);*/
        VIDDECODER_DPRINT("Initializing DSP for wmv9 eCompressionFormat 0x%x\n",
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
        OMX_MALLOC_STRUCT_SIZED(pDynParams, WMV9DEC_UALGDynamicParams, sizeof(WMV9DEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
        if (pDynParams == NULL) {
           VIDDECODER_EPRINT("Error: Malloc failed\n");
           eError = OMX_ErrorInsufficientResources;
           goto EXIT;
        }
        memset(pDynParams, 0, sizeof(WMV9DEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
        pTmp = (char*)pDynParams;
        pTmp += VIDDEC_PADDING_HALF;
        pDynParams = (WMV9DEC_UALGDynamicParams*)pTmp;

#ifdef VIDDEC_SN_R8_14
        pDynParams->size = sizeof(WMV9DEC_UALGDynamicParams);
#endif
        pDynParams->ulDecodeHeader = 0;
        pDynParams->ulDisplayWidth = 0;
        pDynParams->ulFrameSkipMode = 0;
        pDynParams->ulPPType = 0;

        if (pComponentPrivate->nWMVFileType == VIDDEC_WMV_ELEMSTREAM) {
            pDynParams->usIsElementaryStream = VIDDEC_SN_WMV_ELEMSTREAM;
        }
        else {
            pDynParams->usIsElementaryStream = VIDDEC_SN_WMV_RCVSTREAM;
        }

        cmdValues[0] = IUALG_CMD_SETSTATUS; /* add #define IUALG_CMD_SETSTATUS 3 */
        cmdValues[1] = (OMX_U32)(pDynParams);
        cmdValues[2] = sizeof(WMV9DEC_UALGDynamicParams);

        p = (void*)&cmdValues;
        if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
            pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
            pComponentPrivate->pLCML != NULL &&
            pComponentPrivate->bLCMLHalted != OMX_TRUE){
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                       EMMCodecControlAlgCtrl,
                                       (void*)p);
             if(eError != OMX_ErrorNone){
                eError = OMX_ErrorHardware;
                pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                       pComponentPrivate->pHandle->pApplicationPrivate,
                                                       OMX_EventError,
                                                       eError,
                                                       OMX_TI_ErrorSevere,
                                                       "LCML_ControlCodec function");
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                goto EXIT;
            }
            VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        }

        pTmp = (char*)pDynParams;
        pTmp -= VIDDEC_PADDING_HALF;
        pDynParams = (WMV9DEC_UALGDynamicParams*)pTmp;
        free(pDynParams);

        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
            goto EXIT;
        }
    }
    else if (pComponentPrivate->pInPortDef->format.video.eCompressionFormat == OMX_VIDEO_CodingMPEG2 &&
        pComponentPrivate->eState == OMX_StateIdle) {
        /*VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);*/
    }
#ifdef VIDDEC_SPARK_CODE
    else if (VIDDEC_SPARKCHECK) {
        if(pComponentPrivate->eState == OMX_StateIdle) {
            SPARKVDEC_UALGDynamicParams* pDynParams = NULL;
            char* pTmp = NULL;
            OMX_U32 cmdValues[3] = {0, 0, 0};

            /*VIDDEC_Load_Defaults(pComponentPrivate, VIDDEC_INIT_IDLEEXECUTING);*/
            VIDDECODER_DPRINT("Initializing DSP for mpeg4 and h263 eCompressionFormat 0x%x\n",
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
            OMX_MALLOC_STRUCT_SIZED(pDynParams, SPARKVDEC_UALGDynamicParams, sizeof(SPARKVDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
            if (pDynParams == NULL) {
               VIDDECODER_EPRINT("Error: Malloc failed\n");
               eError = OMX_ErrorInsufficientResources;
               goto EXIT;
            }
            memset(pDynParams, 0, sizeof(SPARKVDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
            pTmp = (char*)pDynParams;
            pTmp += VIDDEC_PADDING_HALF;
            pDynParams = (SPARKVDEC_UALGDynamicParams*)pTmp;
#ifdef VIDDEC_SN_R8_14
            pDynParams->size = sizeof(SPARKVDEC_UALGDynamicParams);
#endif
            pDynParams->ulDecodeHeader = 0;
            pDynParams->ulDisplayWidth = 0;
            pDynParams->ulFrameSkipMode = 0;
            pDynParams->ulPPType = 0;

            cmdValues[0] = IUALG_CMD_SETSTATUS;
            cmdValues[1] = (OMX_U32)(pDynParams);
            cmdValues[2] = sizeof(SPARKVDEC_UALGDynamicParams);

            /*pComponentPrivate->bTransPause = 0;*//*flag to wait for the generated event*/
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            p = (void*)&cmdValues;
            if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                pComponentPrivate->pLCML != NULL &&
                pComponentPrivate->bLCMLHalted != OMX_TRUE){
                eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                           EMMCodecControlAlgCtrl,
                                           (void*)p);
                if (eError != OMX_ErrorNone) {
                    eError = OMX_ErrorHardware;
                    goto EXIT;
                }
                VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
            }

            pTmp = (char*)pDynParams;
            pTmp -= VIDDEC_PADDING_HALF;
            pDynParams = (SPARKVDEC_UALGDynamicParams*)pTmp;
            free(pDynParams);

            if (eError != OMX_ErrorNone) {
                VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                goto EXIT;
            }
        }
    }
#endif
    else {
        if(pComponentPrivate->eState == OMX_StateIdle) {
            MP4VDEC_UALGDynamicParams* pDynParams = NULL;
            char* pTmp = NULL;
            OMX_U32 cmdValues[3] = {0, 0, 0};

            VIDDECODER_DPRINT("Initializing DSP for mpeg4 and h263 eCompressionFormat 0x%x\n",
            pComponentPrivate->pInPortDef->format.video.eCompressionFormat);
            OMX_MALLOC_STRUCT_SIZED(pDynParams, MP4VDEC_UALGDynamicParams, sizeof(MP4VDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL,pComponentPrivate->nMemUsage[VIDDDEC_Enum_MemLevel4]);
            if (pDynParams == NULL) {
               VIDDECODER_EPRINT("Error: Malloc failed\n");
               eError = OMX_ErrorInsufficientResources;
               goto EXIT;
            }
            memset(pDynParams, 0, sizeof(MP4VDEC_UALGDynamicParams) + VIDDEC_PADDING_FULL);
            pTmp = (char*)pDynParams;
            pTmp += VIDDEC_PADDING_HALF;
            pDynParams = (MP4VDEC_UALGDynamicParams*)pTmp;
#ifdef VIDDEC_SN_R8_14
            pDynParams->size = sizeof(MP4VDEC_UALGDynamicParams);
#endif
            pDynParams->ulDecodeHeader = 0;
            pDynParams->ulDisplayWidth = 0;
            pDynParams->ulFrameSkipMode = 0;
            pDynParams->ulPPType = 0;

            cmdValues[0] = IUALG_CMD_SETSTATUS;
            cmdValues[1] = (OMX_U32)(pDynParams);
            cmdValues[2] = sizeof(MP4VDEC_UALGDynamicParams);

            /*pComponentPrivate->bTransPause = 0;*//*flag to wait for the generated event*/
            VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
            p = (void*)&cmdValues;
            if(pComponentPrivate->eLCMLState != VidDec_LCML_State_Unload &&
                pComponentPrivate->eLCMLState != VidDec_LCML_State_Destroy &&
                pComponentPrivate->pLCML != NULL &&
                pComponentPrivate->bLCMLHalted != OMX_TRUE){
                eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle,
                                           EMMCodecControlAlgCtrl,
                                           (void*)p);
				
                if (eError != OMX_ErrorNone) {
                    eError = OMX_ErrorHardware;
                    VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
                    goto EXIT;
                }
                VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
                VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
            }

            pTmp = (char*)pDynParams;
            pTmp -= VIDDEC_PADDING_HALF;
            pDynParams = (MP4VDEC_UALGDynamicParams*)pTmp;
            free(pDynParams);

            if (eError != OMX_ErrorNone) {
                VIDDECODER_EPRINT("Codec AlgCtrl 0x%x\n",eError);
                goto EXIT;
            }
			
        }
    }

	eError = OMX_ErrorNone;
	
EXIT:
	return eError;
}



/* ========================================================================== */
/**
  *  VIDDEC_UnloadCodec() function will stop & destroy the codec. LCML resource is also been freed.
  *
  * @param 
  *		pComponentPrivate            Component private structure
  *
  * @retval OMX_NoError              Success, ready to roll
  *			OMX_ErrorUndefined		 
  *         OMX_ErrorInsufficientResources   Not enough memory
 **/
/* ========================================================================== */

OMX_ERRORTYPE VIDDEC_UnloadCodec(VIDDEC_COMPONENT_PRIVATE* pComponentPrivate)
{
	OMX_ERRORTYPE eError = OMX_ErrorUndefined;
	LCML_DSP_INTERFACE *pLcmlHandle = NULL;
	
		pLcmlHandle = (LCML_DSP_INTERFACE*)pComponentPrivate->pLCML;
        VIDDEC_PTHREAD_MUTEX_LOCK(pComponentPrivate->sMutex);
        VIDDECODER_DPRINT("LCML_ControlCodec called MMCodecControlStop 0x%x\n",eError);
        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, MMCodecControlStop, NULL);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT("Occurred in Codec Stop...\n");
            eError = OMX_ErrorHardware;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   eError,
                                                   OMX_TI_ErrorCritical,
                                                   NULL);
            VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
            goto EXIT;
        }
		
        VIDDEC_PTHREAD_MUTEX_WAIT(pComponentPrivate->sMutex);
        VIDDEC_PTHREAD_MUTEX_UNLOCK(pComponentPrivate->sMutex);
        pComponentPrivate->eLCMLState = VidDec_LCML_State_Stop;
		
        eError = LCML_ControlCodec(((LCML_DSP_INTERFACE*)pLcmlHandle)->pCodecinterfacehandle, EMMCodecControlDestroy, NULL);

        VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlDestroy 0x%x\n",pLcmlHandle);
        if (eError != OMX_ErrorNone) {
            VIDDECODER_EPRINT("Occurred in Codec Destroy...\n");
			/*__android_log_print(ANDROID_LOG_VERBOSE, __FILE__,"%s: Occurred in Codec Destroy...", __FUNCTION__);					*/
            eError = OMX_ErrorHardware;
            pComponentPrivate->cbInfo.EventHandler(pComponentPrivate->pHandle,
                                                   pComponentPrivate->pHandle->pApplicationPrivate,
                                                   OMX_EventError,
                                                   eError,
                                                   OMX_TI_ErrorCritical,
                                                   NULL);
            VIDDECODER_EPRINT("Incorrect State Transition 0x%x\n", eError);
            goto EXIT;
        	}
	
        pComponentPrivate->eLCMLState = VidDec_LCML_State_Destroy;
        VIDDECODER_DPRINT("LCML_ControlCodec called EMMCodecControlDestroy 0x%x\n",pLcmlHandle);

#ifndef UNDER_CE
            if(pComponentPrivate->pModLCML != NULL){
                dlclose(pComponentPrivate->pModLCML);
                pComponentPrivate->pModLCML = NULL;
                pComponentPrivate->pLCML = NULL;
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
            }
#else
            if(pComponentPrivate->pModLCML != NULL){
                FreeLibrary(pComponentPrivate->pModLCML);
                pComponentPrivate->pModLCML = NULL;
                pComponentPrivate->pLCML = NULL;
                pComponentPrivate->eLCMLState = VidDec_LCML_State_Unload;
            }
#endif

            pComponentPrivate->bLCMLHalted = OMX_TRUE;

	eError = OMX_ErrorNone;
EXIT:
	return eError;
}
