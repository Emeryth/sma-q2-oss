#include "UartLogger.h"
static void uart_putstring(const uint8_t* str)
{
    uint32_t err_code;

    uint8_t len = strlen((char*)str);
    for (uint8_t i = 0; i < len; i++) {
        err_code = app_uart_put(str[i]);
        APP_ERROR_CHECK(err_code);
    }
}
static void myvprint(const char* fmt, va_list argp)
{
    char string[200];
    if (0 < vsprintf(string, fmt, argp)) // build string
    {
        uart_putstring(string); // send message via UART
    }
}

static const char* level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};
static const char* level_colors[] = {
    "\x1b[94m",
    "\x1b[36m",
    "\x1b[32m",
    "\x1b[33m",
    "\x1b[31m",
    "\x1b[35m"
};
static u8 last_log_level = 0;
static void log_log(bool append, bool terminate, int level, const char* file, const char* function, int line, const char* fmt, ...)
{

    if (append) {
        level = last_log_level;
    } else {
        last_log_level = level;
    }
#ifdef DEBUG_LEVEL

    va_list args;
    char buf[100] = { 0 };
    if (level >= DEBUG_LEVEL) {
        if (!append) {
            snprintf(buf,
                100,
                "%s%s %s%-5s\x1b[0m \x1b[90m%s:%s:%d:\x1b[0m ",
                // cppcheck-suppress sprintfOverlappingData symbolName=buf
                buf,
                "\r\n",
                level_colors[level],
                level_names[level],
                file,
                function,
                line);
            uart_putstring(buf);
        }

        va_start(args, fmt);
        myvprint(fmt, args);
        va_end(args);
        if (terminate) {

            snprintf(buf,
                2,
                "\r\n");
            uart_putstring(buf);
        }
    }
#endif
}

volatile bool scheduled = false;
static void uart_event_handler(void* p_event_data, uint16_t event_size)
{
    u8 in_byte[256];
    u32 index = 0;
    while (app_uart_get(&in_byte[index]) == NRF_SUCCESS) {
        index++;
    }
    scheduled = false;
}
void uart_event_schedule(app_uart_evt_t* p_event)
{
    uint32_t err_code;

    u8 in_byte[6];
    u32 index = 0;
    switch (p_event->evt_type) {
    case APP_UART_DATA_READY:
        // We can unconditionally post the event since this function is only
        // called on first byte put into FIFO
        if (!scheduled) {
            err_code = app_sched_event_put(NULL, 0, uart_event_handler);
            APP_ERROR_CHECK(err_code);
            scheduled = true;
        }
        break;

    case APP_UART_COMMUNICATION_ERROR:
        APP_ERROR_HANDLER(p_event->data.error_communication);
        break;

    case APP_UART_FIFO_ERROR:
        APP_ERROR_HANDLER(p_event->data.error_code);
        break;

    default:
        break;
    }
}
static void init()
{
    uint32_t err_code;

    app_uart_comm_params_t const comm_params = {
        .tx_pin_no = UART_TX_PIN,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity = false,
        .baud_rate = NRF_UART_BAUDRATE_115200
    };

    APP_UART_FIFO_INIT(&comm_params,
        UART_RX_BUF_SIZE,
        UART_TX_BUF_SIZE,
        uart_event_schedule,
        APP_IRQ_PRIORITY_LOW,
        err_code);
    APP_ERROR_CHECK(err_code);
}

const struct uartlogger UartLogger = {
    .init = init,
    .log_log = log_log,
};
