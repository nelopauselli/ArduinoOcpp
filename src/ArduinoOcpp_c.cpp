// matth-x/ArduinoOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#include "ArduinoOcpp_c.h"
#include "ArduinoOcpp.h"

#include <ArduinoOcpp/Debug.h>

ArduinoOcpp::Connection *ocppSocket = nullptr;

void ao_initialize(AO_Connection *conn, const char *chargePointModel, const char *chargePointVendor, struct AO_FilesystemOpt fsopt) {
    ao_initialize_full(conn, ChargerCredentials(chargePointModel, chargePointVendor), fsopt);
}

void ao_initialize_full(AO_Connection *conn, const char *bootNotificationCredentials, struct AO_FilesystemOpt fsopt) {
    if (!conn) {
        AO_DBG_ERR("conn is null");
    }

    ocppSocket = reinterpret_cast<ArduinoOcpp::Connection*>(conn);

    ArduinoOcpp::FilesystemOpt adaptFsopt = fsopt;

    ocpp_initialize(*ocppSocket, bootNotificationCredentials, adaptFsopt);
}

void ao_deinitialize() {
    ocpp_deinitialize();
}

void ao_loop() {
    ocpp_loop();
}

/*
 * Helper functions for transforming callback functions from C-style to C++style
 */

ArduinoOcpp::PollResult<bool> adaptScl(enum OptionalBool v) {
    if (v == OptionalTrue) {
        return true;
    } else if (v == OptionalFalse) {
        return false;
    } else if (v == OptionalNone) {
        return ArduinoOcpp::PollResult<bool>::Await();
    } else {
        AO_DBG_ERR("illegal argument");
        return false;
    }
}

std::function<bool()> adaptFn(InputBool fn) {
    return fn;
}

std::function<bool()> adaptFn(unsigned int connectorId, InputBool_m fn) {
    return [fn, connectorId] () {return fn(connectorId);};
}

std::function<const char*()> adaptFn(InputString fn) {
    return fn;
}

std::function<const char*()> adaptFn(unsigned int connectorId, InputString_m fn) {
    return [fn, connectorId] () {return fn(connectorId);};
}

std::function<float()> adaptFn(InputFloat fn) {
    return fn;
}

std::function<float()> adaptFn(unsigned int connectorId, InputFloat_m fn) {
    return [fn, connectorId] () {return fn(connectorId);};
}

std::function<int()> adaptFn(InputInt fn) {
    return fn;
}

std::function<int()> adaptFn(unsigned int connectorId, InputInt_m fn) {
    return [fn, connectorId] () {return fn(connectorId);};
}

std::function<void(float)> adaptFn(OutputFloat fn) {
    return fn;
}

std::function<void(float, float, int)> adaptFn(OutputSmartCharging fn) {
    return fn;
}

std::function<void(float, float, int)> adaptFn(unsigned int connectorId, OutputSmartCharging_m fn) {
    return [fn, connectorId] (float power, float current, int nphases) {fn(connectorId, power, current, nphases);};
}

std::function<void(float)> adaptFn(unsigned int connectorId, OutputFloat_m fn) {
    return [fn, connectorId] (float value) {return fn(connectorId, value);};
}

std::function<void(void)> adaptFn(void (*fn)(void)) {
    return fn;
}

#ifndef AO_RECEIVE_PAYLOAD_BUFSIZE
#define AO_RECEIVE_PAYLOAD_BUFSIZE 512
#endif

char ao_recv_payload_buff [AO_RECEIVE_PAYLOAD_BUFSIZE] = {'\0'};

std::function<void(JsonObject)> adaptFn(OnMessage fn) {
    if (!fn) return nullptr;
    return [fn] (JsonObject payload) {
        auto len = serializeJson(payload, ao_recv_payload_buff, AO_RECEIVE_PAYLOAD_BUFSIZE);
        if (len <= 0) {
            AO_DBG_WARN("Received payload buffer exceeded. Continue without payload");
        }
        fn(len > 0 ? ao_recv_payload_buff : "", len);
    };
}

ArduinoOcpp::OnReceiveErrorListener adaptFn(OnCallError fn) {
    if (!fn) return nullptr;
    return [fn] (const char *code, const char *description, JsonObject details) {
        auto len = serializeJson(details, ao_recv_payload_buff, AO_RECEIVE_PAYLOAD_BUFSIZE);
        if (len <= 0) {
            AO_DBG_WARN("Received payload buffer exceeded. Continue without payload");
        }
        fn(code, description, len > 0 ? ao_recv_payload_buff : "", len);
    };
}

std::function<ArduinoOcpp::PollResult<bool>()> adaptFn(PollBool fn) {
    return [fn] () {return adaptScl(fn());};
}

std::function<ArduinoOcpp::PollResult<bool>()> adaptFn(unsigned int connectorId, PollBool_m fn) {
    return [fn, connectorId] () {return adaptScl(fn(connectorId));};
}

void ao_beginTransaction(const char *idTag) {
    beginTransaction(idTag);
}
void ao_beginTransaction_m(unsigned int connectorId, const char *idTag) {
    beginTransaction(idTag, connectorId);
}

void ao_beginTransaction_authorized(const char *idTag, const char *parentIdTag) {
    beginTransaction_authorized(idTag, parentIdTag);
}
void ao_beginTransaction_authorized_m(unsigned int connectorId, const char *idTag, const char *parentIdTag) {
    beginTransaction_authorized(idTag, parentIdTag, connectorId);
}

bool ao_endTransaction(const char *idTag, const char *reason) {
    return endTransaction(idTag, reason);
}
bool ao_endTransaction_m(unsigned int connectorId, const char *idTag, const char *reason) {
    return endTransaction(idTag, reason, connectorId);
}

bool ao_isTransactionActive() {
    return isTransactionActive();
}
bool ao_isTransactionActive_m(unsigned int connectorId) {
    return isTransactionActive(connectorId);
}

bool ao_isTransactionRunning() {
    return isTransactionRunning();
}
bool ao_isTransactionRunning_m(unsigned int connectorId) {
    return isTransactionRunning(connectorId);
}

const char *ao_getTransactionIdTag() {
    return getTransactionIdTag();
}
const char *ao_getTransactionIdTag_m(unsigned int connectorId) {
    return getTransactionIdTag(connectorId);
}

AO_Transaction *ao_getTransaction() {
    return ao_getTransaction_m(1);
}
AO_Transaction *ao_getTransaction_m(unsigned int connectorId) {
    if (getTransaction(connectorId)) {
        return reinterpret_cast<AO_Transaction*>(getTransaction(connectorId).get());
    } else {
        return NULL;
    }
}

bool ao_ocppPermitsCharge() {
    return ocppPermitsCharge();
}
bool ao_ocppPermitsCharge_m(unsigned int connectorId) {
    return ocppPermitsCharge(connectorId);
}

void ao_setConnectorPluggedInput(InputBool pluggedInput) {
    setConnectorPluggedInput(adaptFn(pluggedInput));
}
void ao_setConnectorPluggedInput_m(unsigned int connectorId, InputBool_m pluggedInput) {
    setConnectorPluggedInput(adaptFn(connectorId, pluggedInput), connectorId);
}

void ao_setEnergyMeterInput(InputInt energyInput) {
    setEnergyMeterInput(adaptFn(energyInput));
}
void ao_setEnergyMeterInput_m(unsigned int connectorId, InputInt_m energyInput) {
    setEnergyMeterInput(adaptFn(connectorId, energyInput), connectorId);
}

void ao_setPowerMeterInput(InputFloat powerInput) {
    setPowerMeterInput(adaptFn(powerInput));
}
void ao_setPowerMeterInput_m(unsigned int connectorId, InputFloat_m powerInput) {
    setPowerMeterInput(adaptFn(connectorId, powerInput), connectorId);
}

void ao_setSmartChargingPowerOutput(OutputFloat maxPowerOutput) {
    setSmartChargingPowerOutput(adaptFn(maxPowerOutput));
}
void ao_setSmartChargingPowerOutput_m(unsigned int connectorId, OutputFloat_m maxPowerOutput) {
    setSmartChargingPowerOutput(adaptFn(connectorId, maxPowerOutput), connectorId);
}
void ao_setSmartChargingCurrentOutput(OutputFloat maxCurrentOutput) {
    setSmartChargingCurrentOutput(adaptFn(maxCurrentOutput));
}
void ao_setSmartChargingCurrentOutput_m(unsigned int connectorId, OutputFloat_m maxCurrentOutput) {
    setSmartChargingCurrentOutput(adaptFn(connectorId, maxCurrentOutput), connectorId);
}
void ao_setSmartChargingOutput(OutputSmartCharging chargingLimitOutput) {
    setSmartChargingOutput(adaptFn(chargingLimitOutput));
}
void ao_setSmartChargingOutput_m(unsigned int connectorId, OutputSmartCharging_m chargingLimitOutput) {
    setSmartChargingOutput(adaptFn(connectorId, chargingLimitOutput), connectorId);
}

void ao_setEvReadyInput(InputBool evReadyInput) {
    setEvReadyInput(adaptFn(evReadyInput));
}
void ao_setEvReadyInput_m(unsigned int connectorId, InputBool_m evReadyInput) {
    setEvReadyInput(adaptFn(connectorId, evReadyInput), connectorId);
}

void ao_setEvseReadyInput(InputBool evseReadyInput) {
    setEvseReadyInput(adaptFn(evseReadyInput));
}
void ao_setEvseReadyInput_m(unsigned int connectorId, InputBool_m evseReadyInput) {
    setEvseReadyInput(adaptFn(connectorId, evseReadyInput), connectorId);
}

void ao_addErrorCodeInput(InputString errorCodeInput) {
    addErrorCodeInput(adaptFn(errorCodeInput));
}
void ao_addErrorCodeInput_m(unsigned int connectorId, InputString_m errorCodeInput) {
    addErrorCodeInput(adaptFn(connectorId, errorCodeInput), connectorId);
}

void ao_addMeterValueInputInt(InputInt valueInput, const char *measurand, const char *unit, const char *location, const char *phase) {
    addMeterValueInput(adaptFn(valueInput), measurand, unit, location, phase, 1);
}
void ao_addMeterValueInputInt_m(unsigned int connectorId, InputInt_m valueInput, const char *measurand, const char *unit, const char *location, const char *phase) {
    addMeterValueInput(adaptFn(connectorId, valueInput), measurand, unit, location, phase, connectorId);
}

void ao_addMeterValueInput(MeterValueInput *meterValueInput) {
    ao_addMeterValueInput_m(1, meterValueInput);
}
void ao_addMeterValueInput_m(unsigned int connectorId, MeterValueInput *meterValueInput) {
    auto svs = std::unique_ptr<ArduinoOcpp::SampledValueSampler>(
        reinterpret_cast<ArduinoOcpp::SampledValueSampler*>(meterValueInput));
    
    addMeterValueInput(std::move(svs), connectorId);
}

void ao_setOnUnlockConnectorInOut(PollBool onUnlockConnectorInOut) {
    setOnUnlockConnectorInOut(adaptFn(onUnlockConnectorInOut));
}
void ao_setOnUnlockConnectorInOut_m(unsigned int connectorId, PollBool_m onUnlockConnectorInOut) {
    setOnUnlockConnectorInOut(adaptFn(connectorId, onUnlockConnectorInOut), connectorId);
}

void ao_setStartTxReadyInput(InputBool startTxReady) {
    setStartTxReadyInput(adaptFn(startTxReady));
}
void ao_setStartTxReadyInput_m(unsigned int connectorId, InputBool_m startTxReady) {
    setStartTxReadyInput(adaptFn(connectorId, startTxReady), connectorId);
}

void ao_setStopTxReadyInput(InputBool stopTxReady) {
    setStopTxReadyInput(adaptFn(stopTxReady));
}
void ao_setStopTxReadyInput_m(unsigned int connectorId, InputBool_m stopTxReady) {
    setStopTxReadyInput(adaptFn(connectorId, stopTxReady), connectorId);
}

void ao_setTxNotificationOutput(void (*notificationOutput)(enum AO_TxNotification, AO_Transaction*)) {
    setTxNotificationOutput([notificationOutput] (ArduinoOcpp::TxNotification notification, ArduinoOcpp::Transaction *tx) {
        notificationOutput(convertTxNotification(notification), reinterpret_cast<AO_Transaction*>(tx));
    });
}
void ao_setTxNotificationOutput_m(unsigned int connectorId, void (*notificationOutput)(unsigned int, enum AO_TxNotification, AO_Transaction*)) {
    setTxNotificationOutput([notificationOutput, connectorId] (ArduinoOcpp::TxNotification notification, ArduinoOcpp::Transaction *tx) {
        notificationOutput(connectorId, convertTxNotification(notification), reinterpret_cast<AO_Transaction*>(tx));
    }, connectorId);
}

void ao_setOccupiedInput(InputBool occupied) {
    setOccupiedInput(adaptFn(occupied));
}
void ao_setOccupiedInput_m(unsigned int connectorId, InputBool_m occupied) {
    setOccupiedInput(adaptFn(connectorId, occupied), connectorId);
}

bool ao_isOperative() {
    return isOperative();
}
bool ao_isOperative_m(unsigned int connectorId) {
    return isOperative(connectorId);
}
void ao_setOnResetNotify(bool (*onResetNotify)(bool)) {
    setOnResetNotify([onResetNotify] (bool isHard) {return onResetNotify(isHard);});
}

void ao_setOnResetExecute(void (*onResetExecute)(bool)) {
    setOnResetExecute([onResetExecute] (bool isHard) {onResetExecute(isHard);});
}

void ao_setOnReceiveRequest(const char *operationType, OnMessage onRequest) {
    setOnReceiveRequest(operationType, adaptFn(onRequest));
}

void ao_setOnSendConf(const char *operationType, OnMessage onConfirmation) {
    setOnSendConf(operationType, adaptFn(onConfirmation));
}

void ao_set_console_out_c(void (*console_out)(const char *msg)) {
    ao_set_console_out(console_out);
}

void ao_authorize(const char *idTag, AuthorizeConfCallback onConfirmation, AuthorizeAbortCallback onAbort, AuthorizeTimeoutCallback onTimeout, AuthorizeErrorCallback onError, void *user_data) {
    
    std::string idTag_capture = idTag;

    authorize(idTag,
            onConfirmation ? [onConfirmation, idTag_capture, user_data] (JsonObject payload) {
                    auto len = serializeJson(payload, ao_recv_payload_buff, AO_RECEIVE_PAYLOAD_BUFSIZE);
                    if (len <= 0) {AO_DBG_WARN("Received payload buffer exceeded. Continue without payload");}
                    onConfirmation(idTag_capture.c_str(), len > 0 ? ao_recv_payload_buff : "", len, user_data);
                } : OnReceiveConfListener(nullptr),
            onAbort ? [onAbort, idTag_capture, user_data] () -> void {
                    onAbort(idTag_capture.c_str(), user_data);
                } : OnAbortListener(nullptr),
            onTimeout ? [onTimeout, idTag_capture, user_data] () {
                    onTimeout(idTag_capture.c_str(), user_data);
                } : OnTimeoutListener(nullptr),
            onError ? [onError, idTag_capture, user_data] (const char *code, const char *description, JsonObject details) {
                    auto len = serializeJson(details, ao_recv_payload_buff, AO_RECEIVE_PAYLOAD_BUFSIZE);
                    if (len <= 0) {AO_DBG_WARN("Received payload buffer exceeded. Continue without payload");}
                    onError(idTag_capture.c_str(), code, description, len > 0 ? ao_recv_payload_buff : "", len, user_data);
                } : OnReceiveErrorListener(nullptr));
}

void ao_startTransaction(const char *idTag, OnMessage onConfirmation, OnAbort onAbort, OnTimeout onTimeout, OnCallError onError) {
    startTransaction(idTag, adaptFn(onConfirmation), adaptFn(onAbort), adaptFn(onTimeout), adaptFn(onError));
}

void ao_stopTransaction(OnMessage onConfirmation, OnAbort onAbort, OnTimeout onTimeout, OnCallError onError) {
    stopTransaction(adaptFn(onConfirmation), adaptFn(onAbort), adaptFn(onTimeout), adaptFn(onError));
}
