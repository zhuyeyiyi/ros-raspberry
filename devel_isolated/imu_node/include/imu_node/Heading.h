// Generated by gencpp from file imu_node/Heading.msg
// DO NOT EDIT!


#ifndef IMU_NODE_MESSAGE_HEADING_H
#define IMU_NODE_MESSAGE_HEADING_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace imu_node
{
template <class ContainerAllocator>
struct Heading_
{
  typedef Heading_<ContainerAllocator> Type;

  Heading_()
    : header()
    , heading(0.0)  {
    }
  Heading_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , heading(0.0)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef double _heading_type;
  _heading_type heading;




  typedef boost::shared_ptr< ::imu_node::Heading_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::imu_node::Heading_<ContainerAllocator> const> ConstPtr;

}; // struct Heading_

typedef ::imu_node::Heading_<std::allocator<void> > Heading;

typedef boost::shared_ptr< ::imu_node::Heading > HeadingPtr;
typedef boost::shared_ptr< ::imu_node::Heading const> HeadingConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::imu_node::Heading_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::imu_node::Heading_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace imu_node

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'imu_node': ['/home/pi/catkinws/src/imu_node/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::imu_node::Heading_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::imu_node::Heading_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::imu_node::Heading_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::imu_node::Heading_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::imu_node::Heading_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::imu_node::Heading_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::imu_node::Heading_<ContainerAllocator> >
{
  static const char* value()
  {
    return "99a6e32aaf44ebb75a8836a630ce410e";
  }

  static const char* value(const ::imu_node::Heading_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x99a6e32aaf44ebb7ULL;
  static const uint64_t static_value2 = 0x5a8836a630ce410eULL;
};

template<class ContainerAllocator>
struct DataType< ::imu_node::Heading_<ContainerAllocator> >
{
  static const char* value()
  {
    return "imu_node/Heading";
  }

  static const char* value(const ::imu_node::Heading_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::imu_node::Heading_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
\n\
float64 heading\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
";
  }

  static const char* value(const ::imu_node::Heading_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::imu_node::Heading_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.heading);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Heading_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::imu_node::Heading_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::imu_node::Heading_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "heading: ";
    Printer<double>::stream(s, indent + "  ", v.heading);
  }
};

} // namespace message_operations
} // namespace ros

#endif // IMU_NODE_MESSAGE_HEADING_H
