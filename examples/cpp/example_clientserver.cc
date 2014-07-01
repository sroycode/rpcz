// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// S Roychowdhury <sroycode@gmail.com>

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "rpcz/rpcz.hpp"
#include "cpp/search.pb.h"
#include "cpp/search.rpcz.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "zmq.hpp"

#define USE_ADDRESS "tcp://127.0.0.1:5555"
// #define USE_ADDRESS "inproc://thisoneisgood"


namespace examples {

class SearchServiceImpl : public SearchService {

	virtual void Search(
	    const SearchRequest& request,
	    rpcz::reply<SearchResponse> reply) {
		std::cerr << "Got request for '" << request.query() << "'" << std::endl;
		SearchResponse response;
		response.add_results("result1 for " + request.query());
		response.add_results("this is result2");
		reply.send(response);
		std::cerr << "Reply Sent" << std::endl;
	}

};

class fclient {
private:
	rpcz::application* application;
public:
	fclient(zmq::context_t* context) {
		rpcz::application::options options;
		options.zeromq_context = context;
		options.zeromq_io_threads = 1;
		options.connection_manager_threads = 1; // ignored
		application = new rpcz::application(options);
	}
	~fclient() {
		delete application;
	}
	void send(std::string inss) {
		try {
  		google::protobuf::scoped_ptr<rpcz::rpc_channel> channel(application->create_rpc_channel(USE_ADDRESS));
			examples::SearchService_Stub search_stub(channel.get(),false);
			examples::SearchRequest request;
			examples::SearchResponse response;
			request.set_query(inss);
			search_stub.Search(request, &response, 1000);
			std::cerr << response.DebugString() << std::endl;
			channel.release();
		} catch (rpcz::rpc_error &e) {
			std::cerr << "SEND Error: " << e.what() << std::endl;;
		} catch (std::exception &e) {
			std::cerr << "SEND Error: " << e.what() << std::endl;;
		} catch (...) {
			std::cerr << "SEND Error: UNKNOWN ..." << std::endl;;
		}
	}
};

class fserver {
private:
	rpcz::application* application;
public:
	fserver(zmq::context_t* context) {
		rpcz::application::options options;
		options.zeromq_context = context;
		options.zeromq_io_threads = 1;
		options.connection_manager_threads = 1; // ignored
		application = new rpcz::application(options);
	}
	~fserver() {
		delete application;
	}
	void run() {
		rpcz::server server(*application);
		examples::SearchServiceImpl search_service;
		server.register_service(&search_service);
		std::cerr << "Serving requests on port 5555." << std::endl;
		server.bind(USE_ADDRESS);
		application->run();
	}
	void stop() {
		application->terminate();
	}
};
}  // namespace examples

int main()
{

	zmq::context_t* context = new zmq::context_t(2);
	examples::fserver* s = new examples::fserver(context);
	boost::thread thr(boost::bind(&examples::fserver::run, s));
	// client preparation
	sleep(1);
	examples::fclient c(context);

	for(int i=0;i<10000;++i) {
		c.send("data " + boost::lexical_cast<std::string>(i));
	}
	s->stop();
	thr.join();
	delete s;
	delete context;
}

