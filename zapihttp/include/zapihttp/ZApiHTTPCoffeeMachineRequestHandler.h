/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZApiHTTPCoffeeMachineRequestHandler.h
 * Author: tamvh
 *
 * Created on April 13, 2016, 10:26 AM
 */

#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>

#include <zcommon/ErrorCode.h>

#include <zapihttp/ZApiHTTPRequestBaseHandler.h>

#ifndef ZAPIHTTPCOFFEEMACHINEREQUESTHANDLER_H
#define ZAPIHTTPCOFFEEMACHINEREQUESTHANDLER_H

class ZApiHTTPCoffeeMachineRequestHandler : public ZApiHTTPRequestBaseHandler {
public:
    ZApiHTTPCoffeeMachineRequestHandler(const std::string& path);
    virtual ~ZApiHTTPCoffeeMachineRequestHandler();
public:
    static bool CanHandleRequest(const std::string& path, const std::string& method);
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    void handleChooseCoffee(Poco::Net::HTTPServerRequest& request, Poco::JSON::Object::Ptr& responseData);
private:
    int doing = 1; //0: doing, 1: finish.
};

#endif /* ZAPIHTTPCOFFEEMACHINEREQUESTHANDLER_H */

