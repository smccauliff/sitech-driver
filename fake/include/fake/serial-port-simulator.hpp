#pragma once

/// A placeholder.
constexpr const char kNoData = '~';

// This is where this value comes from
// https://stackoverflow.com/questions/2324658/how-to-determine-the-version-of-the-c-standard-used-by-the-compiler
#if __cplusplus != 202002L
#error "Invalid c++ language specification"
#endif

#include <cstdint>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/circular_buffer.hpp>

#include <common/consistent-assert.hpp>

namespace sidriver::fake {

struct FakeSerialPortConfig {
  /// Inject errors into bits.
  float bit_flip_probability;
  uint32_t uart_buffer_size;

  FakeSerialPortConfig() {
    bit_flip_probability = 1.0;
  }
};

/** This class is not multithread safe.
 * This exists to simulate serial port behavior like limited buffers, corrupted data, etc.
 *
 */
class FakeSerialPort {
 public:

  explicit FakeSerialPort(std::shared_ptr<boost::asio::io_context> io_context, FakeSerialPortConfig fake_serial_port_config) :
  io_context_(std::move(io_context)), fake_serial_port_config_(std::move(fake_serial_port_config)), incoming_buffer_queue_(fake_serial_port_config_.uart_buffer_size),
  outgoing_buffer_queue_(fake_serial_port_config_.uart_buffer_size) {

    CONSISTENT_ASSERT(fake_serial_port_config.uart_buffer_size > 0);
  }

  /**
   *
   * @tparam HANDLER This is a function which takes (char byte_read, boost_error_code)
   * @param handler
   */
  template<typename HANDLER>
  void ReadByte(HANDLER&& handler) {
    if (incoming_buffer_queue_.full()) {
      handler(kNoData, std::make_error_code(std::errc::no_buffer_space));
      return;
    }
    incoming_buffer_queue_.push_back(kNoData);
    char& back_ref = incoming_buffer_queue_.back();
    boost::asio::async_read(*io_context_, boost::asio::mutable_buffer(&back_ref, 1), [handler = std::move(handler)](boost::asio::mutable_buffer buf, auto error_code) mutable {
      char read_char = *static_cast<char*>(buf.data());
      handler(read_char, error_code);
    });
  }

  /**
   *
   * @tparam HANDLER This is a function which takes (boost_error_code)
   * @param handler
   */
  template<typename HANDLER>
  void WriteByte(char b, HANDLER&& handler) {
    if (outgoing_buffer_queue_.full()) {
      handler(std::make_error_code(std::errc::no_buffer_space));
      return;
    }

    outgoing_buffer_queue_.push_back(b);
    char& back_ref = outgoing_buffer_queue_.back();
    boost::asio::async_write(*io_context_, boost::asio::mutable_buffer(&back_ref, 1), [handler = std::move(handler)](const auto& ec, size_t bytes_transferred) mutable {
      CONSISTENT_ASSERT(ec || bytes_transferred == 1);
      handler(ec);
    });
  }

 private:
  std::shared_ptr<boost::asio::io_context> io_context_;
  FakeSerialPortConfig fake_serial_port_config_;
  boost::circular_buffer<char> incoming_buffer_queue_;
  boost::circular_buffer<char> outgoing_buffer_queue_;
};

} // end namespace sidriver::fake