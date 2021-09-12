/** \file vp_api_types.h
 * vp_api_types.h
 *
 *  This file is the header for all standard types used in the API code.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */

#ifndef VP_API_TYPES_H
#define VP_API_TYPES_H

#ifdef VXWORKS
#else
#include "linux/string.h"
#endif

#include "vp_api_profile_type.h"
/*#include "limits.h" */  /* For maximum that can be stored in an int */

/* VpDeviceIdType defines the type for the deviceId in the VpDevCtxType type.
 * This information is passed through the API to the HAL to communicate
 * with a specific device.  The values assigned via VpMakeDeviceObject()
 * are user defined and may be simple device indexing (0, 1, .. (n-1)):
 * where n = device number in system
 */

typedef unsigned char VpDeviceIdType;
typedef unsigned char VpLineIdType;

/*
 * Macros for displaying VpDeviceIdType and VpLineIdType values.  If you have
 * defined these (in vp_api_types.h) as something other than simple integers,
 * you should modify the printf format strings as needed:
 */
#define VP_PRINT_DEVICE_ID(deviceId)  VpSysDebugPrintf(" (dev 0x%2.2X)", (int)deviceId)
#define VP_PRINT_LINE_ID(lineId)      VpSysDebugPrintf(" (line %d)", (int)lineId)

#ifndef NULL
    #define NULL (0)
#endif

#define VP_NULL NULL

#ifdef EXTERN
#undef EXTERN
#error EXTERN was redefined!
#endif /* undef EXTERN */

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif /* __cplusplus */

/********************* DECLARATIONS ***************************/
/* Constants */
#ifndef FALSE
#define FALSE   (0)     /* Boolean constant */
#endif
#ifndef TRUE
#define TRUE    (1)     /* Boolean constant */
#endif

#ifndef __cplusplus
/* C++ language provides a boolean data type; So no need to define
 * one more data type; Make use of it
 * NOTE: The 'C' potions of the VP-API assume C++ "bool" to be of the
 * same size as that of "char". Please make sure this assumption is correct.
 */
#ifdef VXWORKS
typedef unsigned char bool;
#else
// boolean data type is defined in Linux since version 2.6.19
//#include <linux/version.h>
#include <generated/version.h>
#include <common.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
typedef unsigned char bool;
#endif
#endif /* VXWORKS */
#endif /* __cplusplus */

/****************** typedefs ***********************************/
/* These are the basic number types used */
/* for uint8, uint16, uint32, int8, int16, int32, bool */
typedef unsigned char   uchar;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

typedef signed char    int8;
typedef signed short int int16;
typedef signed long  int32;

typedef   uint8*  uint8p;
typedef   uint16* uint16p;
typedef   uint32* uint32p;

typedef   int8*   int8p;
typedef   int16*  int16p;
typedef   int32*  int32p;

typedef const VpProfileDataType * VpProfilePtrType;

typedef uint8p VpImagePtrType;

typedef uint16p VpVectorPtrType;

typedef uint8 VpPktDataType;
typedef VpPktDataType* VpPktDataPtrType;

/* Some compilers optimize the size of enumeration data types based on
 * the maximum data value assigned to the members of that data type.
 * 'Standard C' requires enumeration data types to be of the same size
 * as that of native 'int' implementation.
 * The VP-API from a portability persepective adds a 'dummy' member to
 * all enumeration data types that force the compilers to allocate the size
 * of enumeration data types to be equal to that of native 'int'
 * implementation */
#ifdef VXWORKS
#include <Limits.h>
#else
#include <linux/kernel.h>
#endif

#ifndef SHRT_MAX
#define SHRT_MAX (32767)
#endif
#ifndef SHRT_MIN
#define SHRT_MIN (-32768)
#endif

#define FORCE_STANDARD_C_ENUM_SIZE  (INT_MAX)
#define FORCE_SIGNED_ENUM	(INT_MIN)

/* Define any API specific basic data type ranges (that are necessary) */
#define VP_INT16_MAX    (SHRT_MAX)
#define VP_INT16_MIN    (SHRT_MIN)
#define VP_INT32_MAX    (LONG_MAX)
#define VP_INT32_MIN    (LONG_MIN)

#endif /* VP_API_TYPES_H */
