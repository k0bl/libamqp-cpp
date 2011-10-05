#ifndef _LIBAMQPP_CONNECTION_IMPL_H_INCLUDED_
#define _LIBAMQPP_CONNECTION_IMPL_H_INCLUDED_

#include "connection.h"
#include "channel_impl.h"
#include "frame.h"
#include "frame_builder.h"
#include "frame_handler.h"
#include "frame_writer.h"

#ifndef BOOST_ALL_NO_LIB
# define BOOST_ALL_NO_LIB
#endif

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/future.hpp>

#include <queue>
#include <string>
#include <vector>

namespace amqpp
{
namespace detail
{
class frame;
}
namespace impl
{

class channel_impl;

class connection_impl : public amqpp::connection, public boost::noncopyable, public boost::enable_shared_from_this<connection_impl>
{
public:
  explicit connection_impl(const std::string& host, uint16_t port, const std::string& username, const std::string& password, const std::string& vhost);
  virtual ~connection_impl();

  virtual boost::shared_ptr<channel> open_channel();
  virtual void close();

public:
  // Internal interface
  void connect(const std::string& host, uint16_t port, const std::string& username, const std::string& password, const std::string& vhost);

  void begin_write_method(uint16_t channel_id, const detail::method::ptr_t& method);

private:
  boost::shared_ptr<detail::frame> read_frame();
  void write_frame(const boost::shared_ptr<detail::frame>& frame);

  class channel0 : boost::noncopyable, public detail::frame_handler
  {
  public:
    virtual ~channel0() {}

    virtual void process_frame(const detail::frame::ptr_t& frame);
  };

  class connection_thread : boost::noncopyable
  {
  public: // Called from other threads
    explicit connection_thread(connection_impl& impl);
    virtual ~connection_thread();

    inline boost::asio::io_service& get_io_service() { return m_ioservice; }
    inline boost::asio::ip::tcp::socket& get_socket() { return m_socket; }

    void start_async_read_loop();

    typedef boost::unique_future<channel_impl::ptr_t> channel_future_t;
    channel_future_t begin_open_channel();

  public: // Stuff that is only ever called from within the io_service thread

    void begin_frame_read();
    void on_frame_header_read(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_frame_body_read(const boost::system::error_code& ec, size_t bytes_transferred);
    void dispatch_frame(const detail::frame::ptr_t& fr);

    void begin_write_frame(const detail::frame::ptr_t& fr);
    void on_write_frame(const boost::system::error_code& ec, size_t bytes_transferred);

    typedef boost::promise<channel_impl::ptr_t> channel_promise_t;
    typedef boost::shared_ptr<channel_promise_t> channel_promise_ptr_t;

    void start_open_channel(channel_promise_ptr_t channel_promise);
    channel_impl::ptr_t create_next_channel(const channel_promise_ptr_t& promise);


  private:
    boost::asio::io_service m_ioservice;
    boost::asio::ip::tcp::socket m_socket;
    std::vector<boost::weak_ptr<detail::frame_handler> > m_channels;
    boost::shared_ptr<channel0> m_channel0;
    std::queue<detail::frame::ptr_t> m_write_queue;

    detail::frame_builder m_builder;
    detail::frame_writer m_writer;
    detail::frame::ptr_t m_current_write_frame;
    connection_impl& m_connection;

  };

  connection_thread m_thread;

};

} // namespace impl
} // namespace amqpp
#endif // _LIBAMQPP_CONNECTION_IMPL_H_INCLUDED_
