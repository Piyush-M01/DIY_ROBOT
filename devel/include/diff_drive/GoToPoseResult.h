// Generated by gencpp from file diff_drive/GoToPoseResult.msg
// DO NOT EDIT!


#ifndef DIFF_DRIVE_MESSAGE_GOTOPOSERESULT_H
#define DIFF_DRIVE_MESSAGE_GOTOPOSERESULT_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace diff_drive
{
template <class ContainerAllocator>
struct GoToPoseResult_
{
  typedef GoToPoseResult_<ContainerAllocator> Type;

  GoToPoseResult_()
    : success(false)  {
    }
  GoToPoseResult_(const ContainerAllocator& _alloc)
    : success(false)  {
  (void)_alloc;
    }



   typedef uint8_t _success_type;
  _success_type success;





  typedef boost::shared_ptr< ::diff_drive::GoToPoseResult_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::diff_drive::GoToPoseResult_<ContainerAllocator> const> ConstPtr;

}; // struct GoToPoseResult_

typedef ::diff_drive::GoToPoseResult_<std::allocator<void> > GoToPoseResult;

typedef boost::shared_ptr< ::diff_drive::GoToPoseResult > GoToPoseResultPtr;
typedef boost::shared_ptr< ::diff_drive::GoToPoseResult const> GoToPoseResultConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::diff_drive::GoToPoseResult_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::diff_drive::GoToPoseResult_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::diff_drive::GoToPoseResult_<ContainerAllocator1> & lhs, const ::diff_drive::GoToPoseResult_<ContainerAllocator2> & rhs)
{
  return lhs.success == rhs.success;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::diff_drive::GoToPoseResult_<ContainerAllocator1> & lhs, const ::diff_drive::GoToPoseResult_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace diff_drive

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::diff_drive::GoToPoseResult_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::diff_drive::GoToPoseResult_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::diff_drive::GoToPoseResult_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "358e233cde0c8a8bcfea4ce193f8fc15";
  }

  static const char* value(const ::diff_drive::GoToPoseResult_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x358e233cde0c8a8bULL;
  static const uint64_t static_value2 = 0xcfea4ce193f8fc15ULL;
};

template<class ContainerAllocator>
struct DataType< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "diff_drive/GoToPoseResult";
  }

  static const char* value(const ::diff_drive::GoToPoseResult_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"# result definition\n"
"bool success\n"
;
  }

  static const char* value(const ::diff_drive::GoToPoseResult_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.success);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GoToPoseResult_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::diff_drive::GoToPoseResult_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::diff_drive::GoToPoseResult_<ContainerAllocator>& v)
  {
    s << indent << "success: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.success);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DIFF_DRIVE_MESSAGE_GOTOPOSERESULT_H
