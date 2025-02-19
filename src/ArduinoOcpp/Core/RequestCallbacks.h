// matth-x/ArduinoOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#ifndef AO_REQUESTCALLBACKS_H
#define AO_REQUESTCALLBACKS_H

#include <ArduinoJson.h>
#include <functional>

namespace ArduinoOcpp {

using OnReceiveConfListener = std::function<void(JsonObject payload)>;
using OnReceiveReqListener = std::function<void(JsonObject payload)>;
using OnSendConfListener = std::function<void(JsonObject payload)>;
using OnTimeoutListener = std::function<void()>;
using OnReceiveErrorListener = std::function<void(const char *code, const char *description, JsonObject details)>; //will be called if OCPP communication partner returns error code
using OnAbortListener = std::function<void()>; //will be called whenever the engine will stop trying to execute the operation normallythere is a timeout or error (onAbort = onTimeout || onReceiveError)

} //end namespace ArduinoOcpp
#endif
