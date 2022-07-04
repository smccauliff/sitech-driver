

#include <boost/asio.hpp>

#include <fake/serial-port-simulator.hpp>

#include <iostream>
#include <memory>

using namespace sidriver::fake;

int main(int argc, char** argv) {
  auto io_context = std::make_shared<boost::asio::io_context>();

  FakeSerialPortConfig fake_serial_port_config;
  fake_serial_port_config.uart_buffer_size = 64;
  fake_serial_port_config.bit_flip_probability = 0.0;

  FakeSerialPort fake_serial_port(io_context, fake_serial_port_config);
  return 0;
}
