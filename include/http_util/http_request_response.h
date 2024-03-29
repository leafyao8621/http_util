#ifndef HTTP_REQUEST_RESPONSE_HTTP_REQUEST_RESPONSE_H_
#define HTTP_REQUEST_RESPONSE_HTTP_REQUEST_RESPONSE_H_

#include <http_util/containers.h>
#include <http_util/url.h>
#include <http_util/http_header.h>
#include <json/document.h>

typedef enum HTTPMethod {
    HTTP_METHOD_GET,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_PATCH
} HTTPMethod;

typedef enum HTTPBodyType {
    BODY_TYPE_NONE,
    BODY_TYPE_TEXT,
    BODY_TYPE_URL_ENCODED,
    BODY_TYPE_JSON
} HTTPBodyType;

typedef struct HTTPRequest {
    HTTPMethod method;
    URL url;
    URLParams params;
    HTTPHeader header;
    HTTPBodyType body_type;
    union {
        String text;
        URLParams url_encoded;
        JSONDocument json;
    } body;
} HTTPRequest;

typedef enum HTTPResponseCode {
    HTTP_RESPONSE_100,
    HTTP_RESPONSE_101,
    HTTP_RESPONSE_102,
    HTTP_RESPONSE_103,
    HTTP_RESPONSE_200,
    HTTP_RESPONSE_201,
    HTTP_RESPONSE_202,
    HTTP_RESPONSE_203,
    HTTP_RESPONSE_204,
    HTTP_RESPONSE_205,
    HTTP_RESPONSE_206,
    HTTP_RESPONSE_207,
    HTTP_RESPONSE_208,
    HTTP_RESPONSE_226,
    HTTP_RESPONSE_300,
    HTTP_RESPONSE_301,
    HTTP_RESPONSE_302,
    HTTP_RESPONSE_303,
    HTTP_RESPONSE_304,
    HTTP_RESPONSE_305,
    HTTP_RESPONSE_306,
    HTTP_RESPONSE_307,
    HTTP_RESPONSE_308,
    HTTP_RESPONSE_400,
    HTTP_RESPONSE_401,
    HTTP_RESPONSE_402,
    HTTP_RESPONSE_403,
    HTTP_RESPONSE_404,
    HTTP_RESPONSE_405,
    HTTP_RESPONSE_406,
    HTTP_RESPONSE_407,
    HTTP_RESPONSE_408,
    HTTP_RESPONSE_410,
    HTTP_RESPONSE_411,
    HTTP_RESPONSE_412,
    HTTP_RESPONSE_413,
    HTTP_RESPONSE_414,
    HTTP_RESPONSE_415,
    HTTP_RESPONSE_416,
    HTTP_RESPONSE_417,
    HTTP_RESPONSE_418,
    HTTP_RESPONSE_421,
    HTTP_RESPONSE_422,
    HTTP_RESPONSE_423,
    HTTP_RESPONSE_424,
    HTTP_RESPONSE_425,
    HTTP_RESPONSE_426,
    HTTP_RESPONSE_428,
    HTTP_RESPONSE_429,
    HTTP_RESPONSE_431,
    HTTP_RESPONSE_451,
    HTTP_RESPONSE_500,
    HTTP_RESPONSE_501,
    HTTP_RESPONSE_502,
    HTTP_RESPONSE_503,
    HTTP_RESPONSE_504,
    HTTP_RESPONSE_505,
    HTTP_RESPONSE_506,
    HTTP_RESPONSE_507,
    HTTP_RESPONSE_508,
    HTTP_RESPONSE_510,
    HTTP_RESPONSE_511
} HTTPResponseCode;

typedef struct HTTPResponse {
    HTTPResponseCode response_code;
    HTTPHeader header;
    HTTPBodyType body_type;
    union {
        String text;
        URLParams url_encoded;
        JSONDocument json;
    } body;
} HTTPResponse;

int HTTPRequest_initialize(HTTPRequest *request);
int HTTPRequest_parse(HTTPRequest *request, char *data);
int HTTPRequest_finalize(HTTPRequest *request);
int HTTPRequest_serialize(HTTPRequest *request, String *buf);

int HTTPResponse_initialize(
    HTTPResponse *response,
    HTTPResponseCode response_code,
    HTTPBodyType body_type);
int HTTPResponse_finalize(HTTPResponse *response);
int HTTPResponse_serialize(HTTPResponse *response, String *buf);

#endif
