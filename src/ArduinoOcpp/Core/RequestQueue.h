// matth-x/ArduinoOcpp
// Copyright Matthias Akstaller 2019 - 2023
// MIT License

#ifndef AO_REQUESTQUEUE_H
#define AO_REQUESTQUEUE_H

#include <ArduinoOcpp/Core/RequestQueueStorageStrategy.h>

#include <deque>
#include <memory>
#include <ArduinoJson.h>

namespace ArduinoOcpp {

class OperationRegistry;
class Model;
class Connection;
class Request;
class FilesystemAdapter;

class RequestQueue {
private:
    OperationRegistry& operationRegistry;
    
    std::unique_ptr<RequestQueueStorageStrategy> initiatedRequests;
    std::deque<std::unique_ptr<Request>> receivedRequests;

    void receiveRequest(JsonArray json);
    void receiveRequest(JsonArray json, std::unique_ptr<Request> op);
    void receiveResponse(JsonArray json);

    unsigned long sendBackoffTime = 0;
    unsigned long sendBackoffPeriod = 0;
    unsigned long sockTrackLastRecv = 0;
    const unsigned long BACKOFF_PERIOD_MAX = 65536;
    const unsigned long BACKOFF_PERIOD_INCREMENT = BACKOFF_PERIOD_MAX / 4;
public:
    RequestQueue(OperationRegistry& operationRegistry, Model *baseModel, std::shared_ptr<FilesystemAdapter> filesystem = nullptr);

    void setConnection(Connection& sock);

    void loop(Connection& ocppSock);

    void sendRequest(std::unique_ptr<Request> o); //send an OCPP operation request to the server
    
    bool receiveMessage(const char* payload, size_t length); //receive from  server: either a request or response
};

} //end namespace ArduinoOcpp
#endif
