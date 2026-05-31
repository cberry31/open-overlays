#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>

#include "console.h"

// g++ -I /usr/include/boost -pthread websocket.cpp 
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{

	auto const address = net::ip::make_address("0.0.0.0");
	auto const port = static_cast<unsigned short>(std::atoi("8080"));

	net::io_context ioc{ 1 };

	tcp::acceptor acceptor{ ioc, {address, port} };
	for (;;)
	{

		tcp::socket socket{ ioc };

		acceptor.accept(socket);

		std::thread{ std::bind(
			[q = std::move(socket)]() mutable {
				websocket::stream<tcp::socket> ws{std::move(q)};

				// Set a decorator to change the Server of the handshake
				// no need to set. It ıs not necessary
				ws.set_option(websocket::stream_base::decorator(
					[](websocket::response_type& res)
					{
						res.set(http::field::server,
						std::string(BOOST_BEAST_VERSION_STRING) +
							" websocket-server-sync");
					}
				));

				// Accept the websocket handshake
				ws.accept();
				int flag = 0;
				int gear = 0;

				while (true)
				{
					try
					{
						vector<tuple<string, string>> dataArr;
						int newFlag;
						string colorFlag;
						tuple<string, int> newFlagTuple = getFlags(flag);
						tie(colorFlag, newFlag) = newFlagTuple;

						if (flag != newFlag) {
							dataArr.push_back({ "flag", colorFlag });
							flag = newFlag;
						}

						int newGear = getGear();
						if (newGear == gear) {
							dataArr.push_back({ "gear", std::to_string(gear)});
						}
						gear = newGear;


						// Build jsonStr
						string jsonStr = "{ ";
						for (int i = 0; i < dataArr.size(); i++) {
							string key;
							string val;
							tie(key, val) = dataArr[i];
							jsonStr += "\"";
							jsonStr += key;
							jsonStr += "\"";
							jsonStr += ": ";
							jsonStr += "\"";
							jsonStr += val;
							jsonStr += "\"";
							jsonStr += ",";
						}
						jsonStr[jsonStr.size() - 1] = '}';
						// std::cout << jsonStr << endl;

						ws.write(boost::asio::buffer(jsonStr));

					}


					//	// This buffer will hold the incoming message
					//	// buffer types https://www.boost.org/doc/libs/1_75_0/libs/beast/doc/html/beast/using_io/buffer_types.html
					//	// check for the best one
					//	//beast::multi_buffer buffer;
					//	beast::flat_buffer buffer;

					//	// Read a message
					//	ws.read(buffer);

					//	auto out = beast::buffers_to_string(buffer.cdata());
					//	std::cout << out << std::endl;

					//	// Echo the message back
					//	//ws.text(ws.got_text());
					//	//bost::beast::ostream(buffer) << "something";


					//	std::string my_message = "Hello from server!";
					//	ws.write(boost::asio::buffer(my_message));
					//	}
						catch (beast::system_error const& se)
						{
							if (se.code() != websocket::error::closed)
							{
								std::cerr << "Error: " << se.code().message() << std::endl;
								break;
							}
						}
					}
			}
		) }.detach();
	}
}