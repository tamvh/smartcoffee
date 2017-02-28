/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiHTTPCoffeeMachineRequestHandler.cpp
 * Author: tamvh
 * 
 * Created on April 13, 2016, 10:26 AM
 */
#include <stdint.h>
#include <sstream>

#include <Poco/RegularExpression.h>
#include <Poco/Util/Application.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/NumberParser.h>

#include <zcommon/ZServiceLocator.h>
#include <zsession/ZSessionService.h>
#include <zworker/ZWorker.h>

#include "zapihttp/ZApiHTTPCoffeeMachineRequestHandler.h"

using namespace Poco::JSON;
using namespace Poco::Dynamic;
using Poco::DynamicStruct;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;


void ReportError::reportTo(HttpApiError error, Poco::JSON::Object::Ptr& responseData) {
    responseData->set("error", static_cast<int> (error));
}

ZApiHTTPCoffeeMachineRequestHandler::ZApiHTTPCoffeeMachineRequestHandler(const std::string& path) :
ZApiHTTPRequestBaseHandler(path) {
}

ZApiHTTPCoffeeMachineRequestHandler::~ZApiHTTPCoffeeMachineRequestHandler() {
}

bool ZApiHTTPCoffeeMachineRequestHandler::CanHandleRequest(const std::string& path, const std::string& method) {
    if (path.compare("/coffee/choose") == 0 && method.compare("POST") == 0) {
        return true;
    }
    return false;
}

void ZApiHTTPCoffeeMachineRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    Application& app = Application::instance();
    app.logger().information("Request from " + request.clientAddress().toString());
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");

    Poco::JSON::Object::Ptr responseData = new Poco::JSON::Object;
    const std::string& method = request.getMethod();

    if (requestPath().compare("/coffee/choose") == 0) {
        if (method.compare("POST") == 0) {
            handleChooseCoffee(request, responseData);
        }
    }
    std::ostream& ostr = response.send();
    Poco::JSON::Stringifier::stringify(responseData, ostr);               
}

void ZApiHTTPCoffeeMachineRequestHandler::handleChooseCoffee(Poco::Net::HTTPServerRequest& request, Poco::JSON::Object::Ptr& responseData) {
    using namespace Poco::JSON;
    using namespace Poco::Dynamic;
    Application& app = Application::instance();
    try {
        Var result = parseServerRequest(request, responseData);
        if (result.isEmpty()) {
            return;
        }
        Object::Ptr object = result.extract<Object::Ptr>();
        int32_t coffee_type = object->optValue("type", -1);
        if (coffee_type < 0) {
            ReportError::reportTo(HttpApiError::ParameterMissing, responseData);
            return;
        }
        ZWorker *worker = ZServiceLocator::instance()->get<ZWorker>(ZServiceLocator::ServiceId::Worker);
        //worker choose coffee type - push data to mqtt service
        ZWorkerResult zresult = worker->ChooseCoffee(coffee_type);            
        //write log
            
    } catch (Poco::Exception &ex) {
        app.logger().error("Exception while processing message: %s", ex.displayText());
        ReportError::reportTo(HttpApiError::Unknown, responseData);
        return;
    }
}

