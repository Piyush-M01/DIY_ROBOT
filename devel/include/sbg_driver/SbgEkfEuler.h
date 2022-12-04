// Generated by gencpp from file sbg_driver/SbgEkfEuler.msg
// DO NOT EDIT!


#ifndef SBG_DRIVER_MESSAGE_SBGEKFEULER_H
#define SBG_DRIVER_MESSAGE_SBGEKFEULER_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Vector3.h>
#include <sbg_driver/SbgEkfStatus.h>

namespace sbg_driver
{
template <class ContainerAllocator>
struct SbgEkfEuler_
{
  typedef SbgEkfEuler_<ContainerAllocator> Type;

  SbgEkfEuler_()
    : header()
    , time_stamp(0)
    , angle()
    , accuracy()
    , status()  {
    }
  SbgEkfEuler_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , time_stamp(0)
    , angle(_alloc)
    , accuracy(_alloc)
    , status(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef uint32_t _time_stamp_type;
  _time_stamp_type time_stamp;

   typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _angle_type;
  _angle_type angle;

   typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _accuracy_type;
  _accuracy_type accuracy;

   typedef  ::sbg_driver::SbgEkfStatus_<ContainerAllocator>  _status_type;
  _status_type status;





  typedef boost::shared_ptr< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> const> ConstPtr;

}; // struct SbgEkfEuler_

typedef ::sbg_driver::SbgEkfEuler_<std::allocator<void> > SbgEkfEuler;

typedef boost::shared_ptr< ::sbg_driver::SbgEkfEuler > SbgEkfEulerPtr;
typedef boost::shared_ptr< ::sbg_driver::SbgEkfEuler const> SbgEkfEulerConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::sbg_driver::SbgEkfEuler_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::sbg_driver::SbgEkfEuler_<ContainerAllocator1> & lhs, const ::sbg_driver::SbgEkfEuler_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.time_stamp == rhs.time_stamp &&
    lhs.angle == rhs.angle &&
    lhs.accuracy == rhs.accuracy &&
    lhs.status == rhs.status;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::sbg_driver::SbgEkfEuler_<ContainerAllocator1> & lhs, const ::sbg_driver::SbgEkfEuler_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace sbg_driver

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4862d08d71471abacf02c798eaa2e18f";
  }

  static const char* value(const ::sbg_driver::SbgEkfEuler_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4862d08d71471abaULL;
  static const uint64_t static_value2 = 0xcf02c798eaa2e18fULL;
};

template<class ContainerAllocator>
struct DataType< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
{
  static const char* value()
  {
    return "sbg_driver/SbgEkfEuler";
  }

  static const char* value(const ::sbg_driver::SbgEkfEuler_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# SBG Ellipse Messages\n"
"\n"
"Header header\n"
"\n"
"# Time since sensor is powered up micro [us]\n"
"uint32 time_stamp\n"
"\n"
"# Angle [Roll, Pitch, Yaw (heading)] [rad]\n"
"#\n"
"# The right-hand convention applies, where positive rotation is indicated by the direction in which\n"
"# the fingers on your right hand curl when your thumb is pointing in the direction of the axis of rotation.\n"
"#\n"
"# NED convention:\n"
"#   Roll: Rotation about the X axis\n"
"#   Pitch: Rotation about the Y axis\n"
"#   Yaw: Rotation about the down axis. Zero when the X axis is pointing North.\n"
"#\n"
"# ENU convention:\n"
"#   Roll: Rotation around X axis\n"
"#   Pitch: Rotation around Y axis (opposite sign compared to NED)\n"
"#   Yaw: Rotation about the up axis. Zero when the X axis is pointing East. (opposite sign compared to NED)\n"
"geometry_msgs/Vector3 angle\n"
"\n"
"# Angle accuracy (Roll, Pitch, Yaw (heading)) (1 sigma) [rad]\n"
"geometry_msgs/Vector3 accuracy\n"
"\n"
"#  Global solution status\n"
"SbgEkfStatus status\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Vector3\n"
"# This represents a vector in free space. \n"
"# It is only meant to represent a direction. Therefore, it does not\n"
"# make sense to apply a translation to it (e.g., when applying a \n"
"# generic rigid transformation to a Vector3, tf2 will only apply the\n"
"# rotation). If you want your data to be translatable too, use the\n"
"# geometry_msgs/Point message instead.\n"
"\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
"================================================================================\n"
"MSG: sbg_driver/SbgEkfStatus\n"
"# SBG Ellipse Messages\n"
"# Submessage\n"
"\n"
"# Defines the Kalman filter computation mode (see the table 4 below)\n"
"# 0 UNINITIALIZED	The Kalman filter is not initialized and the returned data are all invalid.\n"
"# 1 VERTICAL_GYRO	The Kalman filter only rely on a vertical reference to compute roll and pitch angles. Heading and navigation data drift freely.\n"
"# 2 AHRS			A heading reference is available, the Kalman filter provides full orientation but navigation data drift freely.\n"
"# 3 NAV_VELOCITY	The Kalman filter computes orientation and velocity. Position is freely integrated from velocity estimation.\n"
"# 4 NAV_POSITION	Nominal mode, the Kalman filter computes all parameters (attitude, velocity, position). Absolute position is provided. \n"
"uint8 solution_mode \n"
"\n"
"# True if Attitude data is reliable (Roll/Pitch error < 0,5 deg)\n"
"bool attitude_valid\n"
"\n"
"# True if Heading data is reliable (Heading error < 1 deg)\n"
"bool heading_valid\n"
"\n"
"# True if Velocity data is reliable (velocity error < 1.5 m/s)\n"
"bool velocity_valid\n"
"\n"
"# True if Position data is reliable (Position error < 10m)\n"
"bool position_valid\n"
"\n"
"# True if vertical reference is used in solution (data used and valid since 3s)\n"
"bool vert_ref_used\n"
"\n"
"# True if magnetometer is used in solution (data used and valid since 3s)\n"
"bool mag_ref_used\n"
"\n"
"# True if GPS velocity is used in solution (data used and valid since 3s)\n"
"bool gps1_vel_used\n"
"\n"
"# True if GPS Position is used in solution (data used and valid since 3s)\n"
"bool gps1_pos_used\n"
"\n"
"# True if GPS Course is used in solution (data used and valid since 3s)\n"
"bool gps1_course_used\n"
"\n"
"# True if GPS True Heading is used in solution (data used and valid since 3s)\n"
"bool gps1_hdt_used\n"
"\n"
"# True if GPS2 velocity is used in solution (data used and valid since 3s)\n"
"bool gps2_vel_used\n"
"\n"
"# True if GPS2 Position is used in solution (data used and valid since 3s)\n"
"bool gps2_pos_used\n"
"\n"
"# True if GPS2 Course is used in solution (data used and valid since 3s)\n"
"bool gps2_course_used\n"
"\n"
"# True if GPS2 True Heading is used in solution (data used and valid since 3s)\n"
"bool gps2_hdt_used\n"
"\n"
"# True if Odometer is used in solution (data used and valid since 3s)\n"
"bool odo_used\n"
;
  }

  static const char* value(const ::sbg_driver::SbgEkfEuler_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.time_stamp);
      stream.next(m.angle);
      stream.next(m.accuracy);
      stream.next(m.status);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct SbgEkfEuler_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::sbg_driver::SbgEkfEuler_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::sbg_driver::SbgEkfEuler_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "time_stamp: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.time_stamp);
    s << indent << "angle: ";
    s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.angle);
    s << indent << "accuracy: ";
    s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.accuracy);
    s << indent << "status: ";
    s << std::endl;
    Printer< ::sbg_driver::SbgEkfStatus_<ContainerAllocator> >::stream(s, indent + "  ", v.status);
  }
};

} // namespace message_operations
} // namespace ros

#endif // SBG_DRIVER_MESSAGE_SBGEKFEULER_H
