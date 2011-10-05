#ifndef _LIBAMQPP_FRAME_HANDLER_H_INCLUDED_
#define _LIBAMQPP_FRAME_HANDLER_H_INCLUDED_

#include "frame.h"

#include <boost/shared_ptr.hpp>

namespace amqpp
{
namespace detail
{

class frame_handler
{
public:
  typedef boost::shared_ptr<frame_handler> ptr_t;

  virtual ~frame_handler() {}

  virtual void process_frame(const detail::frame::ptr_t& frame) = 0;
};
}
} // namespace amqpp
#endif // _LIBAMQPP_FRAME_HANDLER_H_INCLUDED_