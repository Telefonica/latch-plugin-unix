'''
 This library offers an API to use Latch in a python environment.
 Copyright (C) 2013 Eleven Paths

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
'''


import json
import logging

class Error(object):

    def __init__(self, json_data):
        '''
        Constructor
        '''
        
        self.code = json_data['code']
        self.message = json_data['message'] 
        
        
    def get_code(self):
        return self.code
    
    def get_message(self):
        return self.message
    
    def to_json(self):
        return {"code" : self.code, "message" : self.message}
    
    def __repr__(self):
        return json.dumps(self.to_json())
    
    def __str__(self):
        return self.__repr__()
  

   
class LatchResponse(object):
    '''
    This class models a response from any of the endpoints in the Latch API.
    It consists of a "data" and an "error" elements. Although normally only one of them will be
    present, they are not mutually exclusive, since errors can be non fatal, and therefore a response
    could have valid information in the data field and at the same time inform of an error.
    '''

    def __init__(self, json_string):
        '''
        @param $json a json string received from one of the methods of the Latch API
        '''
        json_object = json.loads(json_string)
        if "data" in json_object:
            self.data = json_object["data"]
        else:
            self.data = ""
        
        if "error" in json_object:
            self.error = Error(json_object["error"])
        else:
            self.error = ""
    
    def get_data(self):
        '''
        @return JsonObject the data part of the API response
        '''
        return self.data
    
    
    def set_data(self, data):
        '''
        @param $data the data to include in the API response
        '''
        self.data = json.loads(data)
    

    def get_error(self):
        '''
        @return Error the error part of the API response, consisting of an error code and an error message
        '''
        return self.error

    
    def set_error(self, error):
        '''
        @param $error an error to include in the API response
        '''
        self.error = Error(error)


    def to_json(self):
        '''
        @return a Json object with the data and error parts set if they exist
        '''
        json_response = {}
        
        if hasattr(self, "data"):
            json_response["data"] = self.data
        
        if hasattr(self, "error"):
            json_response["error"] = self.error

        return json_response;  


class Latch(object):
    
    API_HOST = "latch.elevenpaths.com";
    API_PORT = 443;
    API_HTTPS = True
    API_PROXY = None;
    API_PROXY_PORT = None;
    API_CHECK_STATUS_URL = "/api/0.6/status";
    API_PAIR_URL = "/api/0.6/pair";
    API_PAIR_WITH_ID_URL = "/api/0.6/pairWithId";
    API_UNPAIR_URL = "/api/0.6/unpair";

    
    AUTHORIZATION_HEADER_NAME = "Authorization";
    DATE_HEADER_NAME = "X-11Paths-Date";
    AUTHORIZATION_METHOD = "11PATHS";
    AUTHORIZATION_HEADER_FIELD_SEPARATOR = " ";

    UTC_STRING_FORMAT = "%Y-%m-%d %H:%M:%S";
    
    X_11PATHS_HEADER_PREFIX = "X-11paths-";
    X_11PATHS_HEADER_SEPARATOR = ":";
    
    @staticmethod
    def set_host(host):
        '''
        @param $host The host to be connected with (http://hostname) or (https://hostname)
        '''
        if host.startswith("http://"):
            Latch.API_HOST = host[len("http://"):]
            Latch.API_PORT = 80
            Latch.API_HTTPS = False
        elif host.startswith("https://"):
            Latch.API_HOST = host[len("https://"):]
            Latch.API_PORT = 443
            Latch.API_HTTPS = True

    @staticmethod
    def set_proxy(proxy, port):
        '''
        Enable using a Proxy to connect through
        @param $proxy The proxy server
        @param $port The proxy port number
        '''
        Latch.API_PROXY = proxy
        Latch.API_PROXY_PORT = port

    @staticmethod
    def get_part_from_header(part, header):
        '''
        The custom header consists of three parts, the method, the appId and the signature.
        This method returns the specified part if it exists.
        @param $part The zero indexed part to be returned
        @param $header The HTTP header value from which to extract the part
        @return string the specified part from the header or an empty string if not existent
        '''
        if (header):
            parts = header.split(Latch.AUTHORIZATION_HEADER_FIELD_SEPARATOR);
            if(len(parts) >= part):
                return parts[part]
        return ""
    
    @staticmethod
    def get_auth_method_from_header(authorizationHeader):
        '''
        @param $authorizationHeader Authorization HTTP Header
        @return string the Authorization method. Typical values are "Basic", "Digest" or "11PATHS"
        '''
        return Latch.get_part_from_header(0, authorizationHeader)

    @staticmethod  
    def get_app_id_from_header(authorizationHeader):
        '''
        @param $authorizationHeader Authorization HTTP Header
        @return string the requesting application Id. Identifies the application using the API
        '''
        return Latch.get_part_from_header(1, authorizationHeader)

    @staticmethod
    def get_signature_from_header(authorizationHeader):
        '''
        @param $authorizationHeader Authorization HTTP Header
        @return string the signature of the current request. Verifies the identity of the application using the API
        '''
        return Latch.get_part_from_header(2, authorizationHeader)


    def __init__(self, appId, secretKey):
        '''
        Create an instance of the class with the Application ID and secret obtained from Eleven Paths
        @param $appId
        @param $secretKey
        '''
        self.appId = appId
        self.secretKey = secretKey




    def _http_get(self, url, xHeaders=None):
        '''
        HTTP GET Request to the specified API endpoint
        @param $string $url
        @param $string $xHeaders
        @return LatchResponse
        '''
        try:
            # Try to use the new Python3 HTTP library if available
            import http.client as http
        except:
            # Must be using Python2 so use the appropriate library
            import httplib as http

        authHeaders = self.authentication_headers("GET", url, xHeaders)
        #print(headers)
        if Latch.API_PROXY != None:
            if Latch.API_HTTPS:
                conn = http.HTTPSConnection(Latch.API_PROXY, Latch.API_PROXY_PORT)
                conn.set_tunnel(Latch.API_HOST, Latch.API_PORT) 
            else: 
                conn = http.HTTPConnection(Latch.API_PROXY, Latch.API_PROXY_PORT)
                url = "http://" + Latch.API_HOST + url                  
        else:
            if Latch.API_HTTPS:
                conn = http.HTTPSConnection(Latch.API_HOST, Latch.API_PORT)
            else: 
                conn = http.HTTPConnection(Latch.API_HOST, Latch.API_PORT)

        try:
            conn.request("GET", url, headers=authHeaders)
            response = conn.getresponse()
           
            responseData = response.read().decode('utf8')
            #print("response:" + responseData)
            conn.close();
            ret = LatchResponse(responseData)
        except:
            ret = LatchResponse("{}")

        return ret
        

    def pairWithId(self, accountId):
        return self._http_get(self.API_PAIR_WITH_ID_URL + "/" + accountId)

    def pair(self, token):
        return self._http_get(self.API_PAIR_URL + "/" + token)

    def status(self, accountId):
        return self._http_get(self.API_CHECK_STATUS_URL + "/" + accountId)

    def operationStatus(self, accountId, operationId):
        return self._http_get(self.API_CHECK_STATUS_URL + "/" + accountId + "/op/" + operationId)

    def unpair(self, accountId):
        return self._http_get(self.API_UNPAIR_URL + "/" + accountId)



    
    def sign_data(self, data):
        '''
        @param $data the string to sign
        @return string base64 encoding of the HMAC-SHA1 hash of the data parameter using {@code secretKey} as cipher key.
        '''
        from hashlib import sha1
        import hmac
        import binascii
        
        sha1Hash = hmac.new(self.secretKey.encode(), data.encode(), sha1)
        return binascii.b2a_base64(sha1Hash.digest())[:-1].decode('utf8')
       
    
    def authentication_headers(self, HTTPMethod, queryString, xHeaders=None, utc=None):
        '''
        Calculate the authentication headers to be sent with a request to the API
        @param $HTTPMethod the HTTP Method, currently only GET is supported
        @param $queryString the urlencoded string including the path (from the first forward slash) and the parameters
        @param $xHeaders HTTP headers specific to the 11-paths API. null if not needed.
        @param $utc the Universal Coordinated Time for the Date HTTP header
        @return array a map with the Authorization and Date headers needed to sign a Latch API request
        '''
        if (not utc):
            utc = Latch.get_current_UTC()
        
        utc = utc.strip()
        
        #logging.debug(HTTPMethod);
        #logging.debug(queryString);
        #logging.debug(utc);

        stringToSign = (HTTPMethod.upper().strip() + "\n" + 
                        utc + "\n" + 
                        self.get_serialized_headers(xHeaders) + "\n" +
                        queryString.strip())
               
        authorizationHeader = (Latch.AUTHORIZATION_METHOD + Latch.AUTHORIZATION_HEADER_FIELD_SEPARATOR + 
                               self.appId + Latch.AUTHORIZATION_HEADER_FIELD_SEPARATOR + 
                               self.sign_data(stringToSign))

        headers = dict()
        headers[Latch.AUTHORIZATION_HEADER_NAME] = authorizationHeader;
        headers[Latch.DATE_HEADER_NAME] = utc;
        return headers

    
    def get_serialized_headers(self, xHeaders):
        '''
        Prepares and returns a string ready to be signed from the 11-paths specific HTTP headers received
        @param $xHeaders a non neccesarily ordered map (array without duplicates) of the HTTP headers to be ordered.
        @return string The serialized headers, an empty string if no headers are passed, or None if there's a problem such as non 11paths specific headers
        '''
        if (xHeaders):
            headers = dict((k.lower(), v) for k, v in xHeaders.iteritems())
            headers.sort()
            serializedHeaders = ""
            for key, value in headers:
                if (not key.startsWith(Latch.X_11PATHS_HEADER_PREFIX.lower())):
                    logging.error("Error serializing headers. Only specific " + Latch.X_11PATHS_HEADER_PREFIX + " headers need to be singed")
                    return None
                serializedHeaders += key + Latch.X_11PATHS_HEADER_SEPARATOR + value + " "
            return serializedHeaders.strip()
        else:
            return ""
    
    @staticmethod
    def get_current_UTC():
        '''
        @return a string representation of the current time in UTC to be used in a Date HTTP Header
        '''
        import time
        return time.strftime(Latch.UTC_STRING_FORMAT, time.gmtime())
    

