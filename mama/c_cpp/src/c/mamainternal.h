/* $Id$
 *
 * OpenMAMA: The open middleware agnostic messaging API
 * Copyright (C) 2011 NYSE Technologies, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef MamaInternalH__
#define MamaInternalH__

#include <property.h>
#include "wombat/wtable.h"
#include "mama/types.h"
#include "mama/status.h"
#include "entitlementinternal.h"
#include "mama/mama.h"
#include "wombat/strutils.h"
#include <mama/version.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define MAMA_PAYLOAD_MAX        CHAR_MAX
#define MAMA_MAX_MIDDLEWARES    CHAR_MAX
#define MAMA_MAX_ENTITLEMENTS   CHAR_MAX
#define MAX_ENTITLEMENT_BRIDGES CHAR_MAX

/* Maximum internal property length */
#define MAX_INTERNAL_PROP_LEN   1024

/* These are the bare parameters which may be reused in other properties */
#define MAMA_PROP_BARE_ENT_DEFERRED        "entitlements.deferred"
#define MAMA_PROP_BARE_COMPILE_TIME_VER    "compile_version"

/* These properties will be set by MAMA */
#define MAMA_PROP_MAMA_RUNTIME_VER         "mama.runtime_version"

/* %s = Bridge Name - these properties may be populated by bridge */
#define MAMA_PROP_BRIDGE_ENT_DEFERRED      "mama.%s." MAMA_PROP_BARE_ENT_DEFERRED
#define MAMA_PROP_BRIDGE_COMPILE_TIME_VER  "mama.%s." MAMA_PROP_BARE_COMPILE_TIME_VER

#define MAMA_SET_BRIDGE_COMPILE_TIME_VERSION(bridgeName)                       \
do                                                                             \
{                                                                              \
    char valString[MAX_INTERNAL_PROP_LEN];                                     \
    char propString[MAX_INTERNAL_PROP_LEN];                                    \
                                                                               \
    /* Advise MAMA which version of MAMA the bridge was compiled against */    \
    snprintf (propString,                                                      \
              sizeof(propString),                                              \
              MAMA_PROP_BRIDGE_COMPILE_TIME_VER,                               \
              bridgeName);                                                     \
    /* Advise MAMA which version of MAMA the bridge was compiled against */    \
    snprintf (valString,                                                       \
              sizeof(valString),                                               \
              "%d.%d.%d",                                                      \
              MAMA_VERSION_MAJOR,                                              \
              MAMA_VERSION_MINOR,                                              \
              MAMA_VERSION_RELEASE);                                           \
    mamaInternal_setMetaProperty (                                             \
              propString,                                                      \
              valString);                                                      \
}                                                                              \
while(0)

/**
 * @brief MAMA Session Statistics information
 *
 * Stores the session stats and configuration for the current MAMA session
 */
typedef struct mamaSessionStats_ {

    /* Session Stats Configuration*/
    int gLogQueueStats          = 1;
    int gLogTransportStats      = 1;
    int gLogGlobalStats         = 1;
    int gLogLbmStats            = 1;
    int gLogUserStats           = 1;

    int gGenerateQueueStats     = 0;
    int gGenerateTransportStats = 0;
    int gGenerateGlobalStats    = 0;
    int gGenerateLbmStats       = 0;
    int gGenerateUserStats       = 0;

    int gPublishQueueStats      = 0;
    int gPublishTransportStats  = 0;
    int gPublishGlobalStats     = 0;
    int gPublishLbmStats        = 0;
    int gPublishUserStats       = 0;

    mamaStatsLogger  gStatsPublisher  = NULL;

    mamaStatsGenerator      gStatsGenerator         = NULL;
    mamaStatsCollector      gGlobalStatsCollector   = NULL;

    /* Session Statistics */
    mamaStat                gInitialStat;
    mamaStat                gRecapStat;
    mamaStat                gUnknownMsgStat;
    mamaStat                gMessageStat;
    mamaStat                gFtTakeoverStat;
    mamaStat                gSubscriptionStat;
    mamaStat                gTimeoutStat;
    mamaStat                gWombatMsgsStat;
    mamaStat                gFastMsgsStat;
    mamaStat                gRvMsgsStat;
    mamaStat                gPublisherSend;
    mamaStat                gPublisherInboxSend;
    mamaStat                gPublisherReplySend;

} mamaSessionStats;

/**
 * @brief Structure for storing combined mamaBridge and LIB_HANDLE data
 */
typedef struct mamaMiddlewareLib_
{
    mamaBridge  bridge;
    LIB_HANDLE  library;
} mamaMiddlewareLib;

/**
 * @brief Structure for storing combined mamaPayloadBridge and LIB_HANDLE data.
 */
typedef struct mamaPayloadLib_
{
    mamaPayloadBridge bridge;
    LIB_HANDLE        library;
    char              id;

    /* Indcates if the bridge was allocated by MAMA or the bridge */
    mama_bool_t       mamaAllocated;
} mamaPayloadLib;

/*
 * @brief Structure for storing combined mamaEntitlementBridge and LIB_HANDLE data.
 */
 typedef struct mamaEntitlementLib_
{
    mamaEntitlementBridge bridge;
    LIB_HANDLE            library;
} mamaEntitlementLib;

/**
 * @brief Structure for managing the loaded middleware libraries
 */
typedef struct mamaMiddlewares_
{
    /* wtable of loaded middlewares, keyed by middleware name */
    wtable_t            table;

    /* Array of loaded middleware libraries, indexed by order of load */
    mamaMiddlewareLib*  byIndex[MAMA_MAX_MIDDLEWARES];

    /* Count of number of currently loaded middlewares */
    mama_i32_t          count;
} mamaMiddlewares;

/**
 * @brief Structure for managing loaded payload libraries
 */
typedef struct mamaPayloads_
{
    /* wtable of loaded payloads, keyed by payload name */
    wtable_t            table;

    /* Array indexed by char id, for rapid access when required. */
    mamaPayloadLib*     byChar[MAMA_PAYLOAD_MAX];

    /* Count of number of currently loaded payloads */
    mama_i32_t          count;
} mamaPayloads;

/**
 * @brief Structure for managing loaded entitlement libraries
 */
typedef struct mamaEntitlements_
{
    /* wtable of loaded entitlement libraries, keyed by name */
    wtable_t            table;

    /* Array of loaded entitlement libraries, indexed by order loaded */
    mamaEntitlementLib* byIndex[MAMA_MAX_ENTITLEMENTS];

    /* Count of number of currently loaded entitlement libraries */
    mama_i32_t          count;
} mamaEntitlements;

/**
 * struct mamaApplicationGroup
 * Contains the name of the application and its class name.
 */
typedef struct mamaAppContext_
{
    const char* mApplicationName;
    const char* mApplicationClass;
} mamaApplicationContext;

/**
 * *NOTE* The MamaImpl structure is being replaced with the mamaSession
 * We'll keep it around while we decide if we need any additional global state
 *
 * This structure contains data needed to control starting and stopping of
 * mama.
 */
typedef struct mamaImpl_
{
    int PLACEHOLDER = 0;
} mamaImpl;

/**
 *  @brief MamaSession object stores per session MAMA information. 
 *
 *  We're attempting to extract the core of MAMA's global state into a single
 *  reusable structure. The idea is to be able to spin up multiple distinct
 *  MAMA libraries within a single process space (fully separated). 
 *
 *  Items which make up part of the MAMA Session object:
 *      - Properties objects
 *      - MAMA Middleware Libraries
 *      - MAMA Payload Libraries
 *      - Session stats configuration and stats logs. 
 */ 
typedef struct mamaSessionImpl_ {

    /* Session Stats */
    mamaSessionStats        sessionStats;

    /* Struct containing loaded middleware bridges. */
    mamaMiddlewares         middlewares;

    /* Struct containing loaded payload bridges */
    mamaPayloads            payloads;

    /* Struct containing loaded entitlement bridges */
    mamaEntitlements        entitlements;

    unsigned int            mRefCount;

    /* wInterlockedInt indicating if the struct has been successfully initialised */
    wInterlockedInt         init;

    wthread_static_mutex_t  mLock;

    /* Internal properties */
    wproperty_t             internalProperties;

    /* Version Information */
    versionInfo             version;

    mama_bool_t             gAllowMsgModify = 0;

    int                     gCatchCallbackExceptions = 0;

    wproperty_t             gProperties      = 0;

    mamaPayloadBridge       gDefaultPayload = NULL;

    wthread_key_t           last_err_key;

    mamaApplicationContext  appContext;

    char mama_ver_string[256];
} mamaSessionImpl;

/************************************************************************/
/* Globals from mama.c */
/* By the time this is complete, we should not have anything left in this
 * section
 */

/* Need to discuss the entitlements setup - might be worth keeping
 * these as part of the global state.
 */
mamaEntitlementCallbacks  gEntitlementCallbacks;
extern const char*        gEntitlementBridges[MAX_ENTITLEMENT_BRIDGES];


//  static mamaImpl gImpl = {
//                            { NULL, {0}, 0 },         /* middlewares */
//                            { NULL, {0}, 0 },         /* payloads */
//                            { NULL, {0}, 0 },         /* entitlements */
//                            0,                        /* myRefCount */
//                            0,                        /* init */
//                            WSTATIC_MUTEX_INITIALIZER,/* myLock */
//                            NULL
//                        };

/**************************************************************************/

/**
* Check whether Callbacks are run in 'debug' catch exceptions mode
*/
MAMAExpDLL
extern int
mamaInternal_getCatchCallbackExceptions (void);

/**
 * Exposes the property object from mama
 */
MAMAExpDLL
extern wproperty_t
mamaInternal_getProperties (void);

/* Used by the bridges to register themselves with MAMA. Should not
   be called from anywhere else */
MAMAExpDLL
extern void
mamaInternal_registerBridge (mamaBridge     bridge,
                             const char*    middleware);

/**
 * getVersion for use when mama is wrapped
 */
const char*
mama_wrapperGetVersion(mamaBridge     bridge);

MAMAExpDLL
extern mamaStatsGenerator
mamaInternal_getStatsGenerator (void);

MAMAExpDLL
extern mamaStatsCollector
mamaInternal_getGlobalStatsCollector (void);

MAMAExpDLL
extern int
mamaInternal_generateLbmStats (void);

MAMAExpDLL
mamaBridge
mamaInternal_findBridge (void);

MAMAExpDLL
mamaPayloadBridge
mamaInternal_findPayload (char id);

mamaPayloadBridge
mamaInternal_getDefaultPayload (void);

mama_bool_t
mamaInternal_getAllowMsgModify (void);

/**
 * @brief Helper function for initialization of internal wtables used by
 * OpenMAMA core.
 *
 * @param table Pointer to the wtable to be initialised.
 * @param name Name to be attached to the table
 * @param size Size of the table to be initialised.
 *
 * @return mama_status indicating the success or failure of the initialisation.
 */
mama_status
mamaInternal_initialiseTable (wtable_t*   table,
                              const char* name,
                              mama_size_t size);

/**
 * @brief Initialise core MAMA data structures.
 *
 * mamaInternal_init should be triggered once, and initialises the various data
 * structures used by MAMA internally. This includes:
 *     - Loading properties.
 *     - Initialising the bridges wtable structure.
 *
 * @return A mama_status indicating the success or failure of the initialisation.
 *
 */
mama_status
mamaInternal_init (void);

/**
 * @brief Return payload character for the passed payload.
 *
 * Method checks various mechanisms for the payload character associated with
 * the named payload. Firstly, it will check the global properties object for
 * a property matching the pattern mama.payload.<name>.payloadId=
 *
 * If no character is set in properties, the method will then check with the
 * payload bridge itself, via the <name>Payload_getPayloadType method.
 *
 * The response is then stored in the payloadChar parameter.
 *
 * @param[in] payloadName The name of the payload for which the char is required.
 * @param[in] payload The payloadBridge struct  for which the char is required
 * @param[out] payloadChar The character into which the response is passed.
 *
 * @return A mama_status indicating the success or failure of the method.
 */
MAMAExpDLL
mama_status
mamaInternal_getPayloadId (const char*       payloadName,
                           mamaPayloadBridge payload,
                           char*             payloadChar);

/**
 * @brief Return count of entitlementBridges loaded.
 * @return Integer number of loaded bridges.
 */
mama_i32_t
mamaInternal_getEntitlementBridgeCount (void);

/**
 * @brief Find loaded entitlement bridge by name.
 *
 * @param[in] name The name of the entitlement bridge to be found.
 * @param[out] entBridge The loaded entitlement bridge (if found).
 *
 * @return MAMA_STATUS_OK if successful.
 */
 mama_status
mamaInternal_getEntitlementBridgeByName(mamaEntitlementBridge* entBridge, const char* name);

MAMAExpDLL
const char*
mamaInternal_getMetaProperty (const char* name);

MAMAExpDLL
mama_status
mamaInternal_setMetaProperty (const char* name, const char* value);

/* ************************************************************************* */
/* Callbacks. */
/* ************************************************************************* */
void MAMACALLTYPE mamaImpl_entitlementDisconnectCallback(
                            const  sessionDisconnectReason reason,
                            const  char * const            userId,
                            const  char * const            host,
                            const  char * const            appName);
void MAMACALLTYPE mamaImpl_entitlementUpdatedCallback (void);
void MAMACALLTYPE mamaImpl_entitlementCheckingSwitchCallback (
                            int isEntitlementsCheckingDisabled);

#if defined(__cplusplus)
}
#endif

#endif /*MamaInternalH__ */
