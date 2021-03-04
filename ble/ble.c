/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "ble_init.h"
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "sma-q2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "app_timer.h"
#include "app_time.h"
#include "status.h"
#include "ble_protocol.h"
#include "ble_watch_service.h"
#include "peer_manager.h"
#include "fds.h"
#include "fstorage.h"
#include "ecc.h"
#include "ble_conn_state.h"

#define APP_FEATURE_NOT_SUPPORTED   BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                           /**< Include the service_changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#define CENTRAL_LINK_COUNT              0                                           /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                           /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                     "SMAQ2"                                     /**< Name of device. Will be included in the advertising data. Device ID will be appended.*/
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_ADV_INTERVAL                640                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS      180                                         /**< The advertising timeout (in units of seconds). */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define BLE_GAP_LESC_P256_SK_LEN 32
/**@brief GAP LE Secure Connections P-256 Private Key. */
typedef struct
{
  uint8_t   sk[BLE_GAP_LESC_P256_SK_LEN];        /**< LE Secure Connections Elliptic Curve Diffie-Hellman P-256 Private Key in little-endian. */
} ble_gap_lesc_p256_sk_t;

static __ALIGN(4) ble_gap_lesc_p256_sk_t m_lesc_sk;    /**< LESC ECC Private Key */
static __ALIGN(4) ble_gap_lesc_p256_pk_t m_lesc_pk;    /**< LESC ECC Public Key */
static __ALIGN(4) ble_gap_lesc_dhkey_t m_lesc_dhkey;   /**< LESC ECC DH Key*/

static uint8_t m_dhkey_req = 0;             /**< LESC DHKEY requested */

uint8_t passkey[BLE_GAP_PASSKEY_LEN + 1];

//static ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */
static ble_watchs_t                     m_watchs;
static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */
//static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. */
static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_WATCH_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN}};  /**< Universally unique service identifier. */

static SemaphoreHandle_t m_ble_event_ready;  /**< Semaphore raised if there is a new event to be processed in the BLE thread. */

TaskHandle_t  m_ble_stack_thread;     /**< Definition of BLE stack thread. */

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    // using more unique name
    // see https://devzone.nordicsemi.com/f/nordic-q-a/16389/unique-ble-device-name
    // length of prefix
    uint16_t device_name_prefix_length = strlen(DEVICE_NAME);

    // final device name - 9 bytes added for the suffix
    uint16_t final_device_name_length = device_name_prefix_length+9;

    // buffer for the final device name
    char final_device_name[final_device_name_length];

    memcpy(final_device_name, DEVICE_NAME, device_name_prefix_length);

    // append the device ID as unique device name suffix
    sprintf(&final_device_name[device_name_prefix_length], "-%04lX%04lX", (unsigned long)NRF_FICR->DEVICEID[0], (unsigned long)NRF_FICR->DEVICEID[1]);

    // set the unique advertising name
    err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *) final_device_name, final_device_name_length);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@snippet [Handling the data received over BLE] */

void ble_send(uint8_t *data, uint16_t length){

	ble_watchs_send(&m_watchs, data, length);
}


/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void)
{
    uint32_t       err_code;

    err_code = ble_watchs_init(&m_watchs);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling File Data Storage events.
 *
 * @param[in] p_evt  Peer Manager event.
 * @param[in] cmd
 */
static void fds_evt_handler(fds_evt_t const * const p_fds_evt)
{
    if (p_fds_evt->id == FDS_EVT_GC)
    {
        NRF_LOG_PRINTF("GC completed\n");
    }
}

/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const * p_evt)
{
    ret_code_t err_code;

    switch(p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
            NRF_LOG_PRINTF("PM_EVT_BONDED_PEER_CONNECTED: peer_id=%d\n", p_evt->peer_id);
            err_code = pm_peer_rank_highest(p_evt->peer_id);
            NRF_LOG_PRINTF("peer_rank err_code: %d\r\n", err_code);
            break;

        case PM_EVT_CONN_SEC_START:
            NRF_LOG_PRINTF("PM_EVT_CONN_SEC_START: peer_id=%d\n", p_evt->peer_id);
            break;

        case PM_EVT_CONN_SEC_SUCCEEDED:
            NRF_LOG_PRINTF("PM_EVT_CONN_SEC_SUCCEEDED Role: %d. conn_handle: %d, Procedure: %d\r\n",
                           ble_conn_state_role(p_evt->conn_handle),
                           p_evt->conn_handle,
                           p_evt->params.conn_sec_succeeded.procedure);
            err_code = pm_peer_rank_highest(p_evt->peer_id);
            NRF_LOG_PRINTF("peer_rank err_code: %d\r\n", err_code);
            break;

        case PM_EVT_CONN_SEC_FAILED:
            NRF_LOG_PRINTF("PM_EVT_CONN_SEC_FAILED: peer_id=%d, error=%d\n", p_evt->peer_id, p_evt->params.conn_sec_failed.error);
            switch (p_evt->params.conn_sec_failed.error)
            {
                case PM_CONN_SEC_ERROR_PIN_OR_KEY_MISSING:
                    // Rebond if one party has lost its keys.
                    err_code = pm_conn_secure(p_evt->conn_handle, true);
                    if (err_code != NRF_ERROR_INVALID_STATE)
                    {
                        APP_ERROR_CHECK(err_code);
                    }
                    break;

                default:
                    break;
            }
            break;

        case PM_EVT_CONN_SEC_CONFIG_REQ:
        {
            // Reject pairing request from an already bonded peer.
            pm_conn_sec_config_t conn_sec_config = {.allow_repairing = false};
            pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
        }

        case PM_EVT_STORAGE_FULL:
            // Run garbage collection on the flash.
            err_code = fds_gc();
            if (err_code == FDS_ERR_BUSY || err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                // Retry.
            }
            break;

        case PM_EVT_ERROR_UNEXPECTED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
            break;

        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
            NRF_LOG_PRINTF("PM_EVT_PEER_DATA_UPDATE_SUCCEEDED: peer_id=%d data_id=0x%x action=0x%x\n", p_evt->peer_id, p_evt->params.peer_data_update_succeeded.data_id, p_evt->params.peer_data_update_succeeded.action);
            break;

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
            // Assert.
            APP_ERROR_CHECK_BOOL(false);
            break;

        case PM_EVT_PEER_DELETE_SUCCEEDED:
            break;

        case PM_EVT_PEER_DELETE_FAILED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
            break;

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
        	ble_advertising_start(BLE_ADV_MODE_SLOW);
            break;

        case PM_EVT_PEERS_DELETE_FAILED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
            break;

        case PM_EVT_LOCAL_DB_CACHE_APPLIED:
            break;

        case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
            // The local database has likely changed, send service changed indications.
            pm_local_database_has_changed();
            break;

        case PM_EVT_SERVICE_CHANGED_IND_SENT:
            break;

        case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
            break;
    }
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
//    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            break;
        case BLE_ADV_EVT_IDLE:
            ble_advertising_start(BLE_ADV_MODE_SLOW);
            break;
        default:
            break;
    }
}

void ble_accept_passkey(void) {
	uint32_t err_code;
	err_code = sd_ble_gap_auth_key_reply(m_conn_handle,
			BLE_GAP_AUTH_KEY_TYPE_PASSKEY, NULL);
	APP_ERROR_CHECK(err_code);

	/* Due to DRGN-7235, dhkey_reply() must come after auth_key_reply() */
	APP_ERROR_CHECK_BOOL(m_dhkey_req);
	err_code = sd_ble_gap_lesc_dhkey_reply(m_conn_handle, &m_lesc_dhkey);
	APP_ERROR_CHECK(err_code);
	m_dhkey_req = 0;

}

void ble_reject_passkey(void) {
	uint32_t err_code;
	err_code = sd_ble_gap_auth_key_reply(m_conn_handle,
			BLE_GAP_AUTH_KEY_TYPE_NONE, NULL);
	APP_ERROR_CHECK(err_code);
}

/**@brief Function for the application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                         err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
        	status_set_ble_connected(true);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
        	status_set_ble_connected(false);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
//            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
//            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PASSKEY_DISPLAY:
            memcpy(passkey, p_ble_evt->evt.gap_evt.params.passkey_display.passkey, BLE_GAP_PASSKEY_LEN);
            passkey[BLE_GAP_PASSKEY_LEN] = 0x00;
            status_set_pairing_request(true);
            break;
        case BLE_GAP_EVT_AUTH_KEY_REQUEST:
            NRF_LOG_PRINTF("BLE_GAP_EVT_AUTH_KEY_REQUEST\n");
            break;
        case BLE_GAP_EVT_LESC_DHKEY_REQUEST:
            NRF_LOG_PRINTF("BLE_GAP_EVT_LESC_DHKEY_REQUEST\n");
            err_code = ecc_p256_shared_secret_compute(&m_lesc_sk.sk[0], &p_ble_evt->evt.gap_evt.params.lesc_dhkey_request.p_pk_peer->pk[0], &m_lesc_dhkey.key[0]);
            APP_ERROR_CHECK(err_code);
            m_dhkey_req = 1;
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
//            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
//            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        {
            ble_gatts_rw_authorize_reply_params_t auth_reply;
            if(p_ble_evt->evt.gatts_evt.params.authorize_request.type
               != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if ((p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                     == BLE_GATTS_OP_PREP_WRITE_REQ)
                    || (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                     == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
                    || (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op
                     == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
                {
                    if (p_ble_evt->evt.gatts_evt.params.authorize_request.type
                        == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                    }
                    else
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
                    }
                    auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
                    err_code = sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gap_evt.conn_handle,&auth_reply);
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;
        }


        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a SoftDevice event to all modules with a SoftDevice
 *        event handler.
 *
 * @details This function is called from the SoftDevice event interrupt handler after a
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  SoftDevice event.
 */
void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    /** The Connection state module has to be fed BLE events in order to function correctly
     * Remember to call ble_conn_state_on_ble_evt before calling any ble_conns_state_* functions. */
    ble_conn_state_on_ble_evt(p_ble_evt);

    pm_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);

    ble_watchs_on_ble_evt(&m_watchs,p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);

}

/**
 * @brief Event handler for new BLE events
 *
 * This function is called from the SoftDevice handler.
 * It is called from interrupt level.
 *
 * @return The returned value is checked in the softdevice_handler module,
 *         using the APP_ERROR_CHECK macro.
 */
static uint32_t ble_new_event_handler(void)
{
    BaseType_t yield_req = pdFALSE;
    // The returned value may be safely ignored, if error is returned it only means that
    // the semaphore is already given (raised).
    UNUSED_VARIABLE(xSemaphoreGiveFromISR(m_ble_event_ready, &yield_req));
    portYIELD_FROM_ISR(yield_req);
    return NRF_SUCCESS;
}

/**@brief Function for the Peer Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Peer Manager.
 */
void peer_manager_init(bool erase_bonds)
{
    ble_gap_sec_params_t sec_params;
    ret_code_t err_code;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    if (erase_bonds)
    {
        (void) pm_peers_delete();
    }

    memset(&sec_params, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_params.bond           = 1;
    sec_params.mitm           = 1;
    sec_params.lesc           = 1;
    sec_params.keypress       = 0;
    sec_params.io_caps        = BLE_GAP_IO_CAPS_DISPLAY_YESNO;
    sec_params.oob            = 0;
    sec_params.min_key_size   = 7;
    sec_params.max_key_size   = 16;
    sec_params.kdist_own.enc  = 1;
    sec_params.kdist_own.id   = 1;
    sec_params.kdist_peer.enc = 1;
    sec_params.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_params);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = fds_register(fds_evt_handler);
    APP_ERROR_CHECK(err_code);

    ecc_init();

    err_code = ecc_p256_keypair_gen(m_lesc_sk.sk, m_lesc_pk.pk);
    APP_ERROR_CHECK(err_code);

    /* Set the public key */
    err_code = pm_lesc_public_key_set(&m_lesc_pk);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    /** Dispatch the system event to the Flash Storage module, where it will be
     *  dispatched to the Flash Data Storage module and from there to the Peer Manager. */
    fs_sys_event_handler(sys_evt);
    ble_advertising_on_sys_evt(sys_evt);

}

/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize SoftDevice.
//    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, ble_new_event_handler);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for System events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertising functionality.
 */
void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_slow_enabled  = BLE_ADV_SLOW_ENABLED;
    options.ble_adv_slow_interval = APP_ADV_INTERVAL;
    options.ble_adv_slow_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}

void ble_stack_thread(void * arg)
{
    uint32_t  err_code;

    UNUSED_PARAMETER(arg);

    bool erase_bonds=false;
//    if (!nrf_gpio_pin_read(BUTTON_OK_PIN)){
//    	erase_bonds=true;
//    }

    m_ble_event_ready = xSemaphoreCreateBinary();
    // Initialize.
    ble_stack_init();
    peer_manager_init(erase_bonds);
    gap_params_init();
    conn_params_init();
    services_init();
    advertising_init();


    err_code = ble_advertising_start(BLE_ADV_MODE_SLOW);
    APP_ERROR_CHECK(err_code);

    while (1)
    {
        /* Wait for event from SoftDevice */
        while(pdFALSE == xSemaphoreTake(m_ble_event_ready, portMAX_DELAY))
        {
            // Just wait again in the case when INCLUDE_vTaskSuspend is not enabled
        }

        // This function gets events from the SoftDevice and processes them by calling the function
        // registered by softdevice_ble_evt_handler_set during stack initialization.
        // In this code ble_evt_dispatch would be called for every event found.
        intern_softdevice_events_execute();
    }
}
