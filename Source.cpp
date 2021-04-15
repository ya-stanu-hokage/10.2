#define BOOST_DATE_TIME_NO_LIB

#include <iostream>
#include <string>
#include <thread>

#include <boost/asio.hpp>

void write_data(boost::asio::ip::tcp::socket& socket, const std::string& name)
{
	std::string data;

	std::getline(std::cin, data);

	boost::asio::write(socket, boost::asio::buffer("User join the channel!"));

	while (std::getline(std::cin, data))
	{
		if (data == "END")
		{
			boost::asio::write(socket, boost::asio::buffer("User left the channel!"));
			break;
		}
		boost::asio::write(socket, boost::asio::buffer(name + ": " + data + '!'));
	}
}

void read_data_until(boost::asio::ip::tcp::socket& socket)
{

	boost::asio::streambuf buffer;

	while (boost::asio::read_until(socket, buffer, '!'))
	{
		std::string message;

		std::istream input_stream(&buffer);

		std::getline(input_stream, message, '!');

		std::cout << message << std::endl;

		if (message == "User left the channel")
		{
			boost::asio::write(socket, boost::asio::buffer("User left the channel!"));
			break;
		}
	}
}


int main(int argc, char** argv)
{
	system("chcp 1251");

	system("pause");

	std::string raw_ip_address = "127.0.0.1";

	auto port = 3333;

	try
	{
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::address::from_string(raw_ip_address), port);

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

		socket.connect(endpoint);

		std::string name;

		std::cout << "Enter your name" << std::endl;

		std::cin >> name;

		std::thread Thread(read_data_until, std::ref(socket));

		write_data(socket, name);

		Thread.join();
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}

	system("pause");

	return 0;
}