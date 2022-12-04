// Generated by gencpp from file diff_drive/GoToPoseFeedback.msg
// DO NOT EDIT!


#ifndef DIFF_DRIVE_MESSAGE_GOTOPOSEFEEDBACK_H
#define DIFF_DRIVE_MESSAGE_GOTOPOSEFEEDBACK_H


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
struct GoToPoseFeedback_
{
  typedef GoToPoseFeedback_<ContainerAllocator> Type;

  GoToPoseFeedback_()
    : processing(false)  {
    }
  GoToPoseFeedback_(const ContainerAllocator& _alloc)
    : processing(false)  {
  (void)_alloc;
    }



   typedef uint8_t _processing_type;
  _processing_type processing;





  typedef boost::shared_ptr< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> const> ConstPtr;

}; // struct GoToPoseFeedback_

typedef ::diff_drive::GoToPoseFeedback_<std::allocator<void> > GoToPoseFeedback;

typedef boost::shared_ptr< ::diff_drive::GoToPoseFeedback > GoToPoseFeedbackPtr;
typedef boost::shared_ptr< ::diff_drive::GoToPoseFeedback const> GoToPoseFeedbackConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::diff_drive::GoToPoseFeedback_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::diff_drive::GoToPoseFeedback_<ContainerAllocator1> & lhs, const ::diff_drive::GoToPoseFeedback_<ContainerAllocator2> & rhs)
{
  return lhs.processing == rhs.processing;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::diff_drive::GoToPoseFeedback_<ContainerAllocator1> & lhs, const ::diff_drive::GoToPoseFeedback_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace diff_drive

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "14c43fbec2ed75ad470309f9d3538198";
  }

  static const char* value(const ::diff_drive::GoToPoseFeedback_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x14c43fbec2ed75adULL;
  static const uint64_t static_value2 = 0x470309f9d3538198ULL;
};

template<class ContainerAllocator>
struct DataType< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "diff_drive/GoToPoseFeedback";
  }

  static const char* value(const ::diff_drive::GoToPoseFeedback_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"# feedback\n"
"bool processing\n"
"\n"
"\n"
;
  }

  static const char* value(const ::diff_drive::GoToPoseFeedback_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.processing);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GoToPoseFeedback_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::diff_drive::GoToPoseFeedback_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::diff_drive::GoToPoseFeedback_<ContainerAllocator>& v)
  {
    s << indent << "processing: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.processing);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DIFF_DRIVE_MESSAGE_GOTOPOSEFEEDBACK_H
