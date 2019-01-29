#include "utils.h"

namespace mc_rtc_rviz
{

geometry_msgs::Point rosPoint(const Eigen::Vector3d& vec)
{
  geometry_msgs::Point p;
  p.x = vec.x();
  p.y = vec.y();
  p.z = vec.z();
  return p;
}

vm::Marker makeVisual(int t, double baseScale)
{
  vm::Marker ret;
  ret.type = t;
  ret.scale.x = baseScale*0.45;
  ret.scale.y = baseScale*0.45;
  ret.scale.z = baseScale*0.45;
  ret.color.r = 1.0;
  ret.color.g = 0.0;
  ret.color.b = 0.0;
  ret.color.a = 1.0;
  ret.pose.orientation.w = 1.0;
  return ret;
}

std::vector<vm::Marker> makeAxisMarker(double scale)
{
  const Eigen::Vector3d t0 = scale * Eigen::Vector3d{0., 0., 0.};
  const Eigen::Vector3d tx = scale * Eigen::Vector3d{1., 0., 0.};
  const Eigen::Vector3d ty = scale * Eigen::Vector3d{0., 1., 0.};
  const Eigen::Vector3d tz = scale * Eigen::Vector3d{0., 0., 1.};

  vm::Marker m;
  m.type = vm::Marker::ARROW;
  m.action = vm::Marker::ADD;
  // Arrow shaft diameter
  m.scale.x = scale * 0.15;
  // arrow head diameter
  m.scale.y = scale * 0.15;
  // arrow head length
  m.scale.z = scale * 0.5;
  m.pose.orientation.w = 1.0;


  std::vector<vm::Marker> ret(3);
  // X axis
  m.points.push_back(rosPoint(t0));
  m.points.push_back(rosPoint(tx));
  m.color.a = 1.;
  m.color.r = 1.;
  m.color.g = 0.;
  m.color.b = 0.;
  ret.push_back(m);
  // Y axis
  m.points.clear();
  m.points.push_back(rosPoint(t0));
  m.points.push_back(rosPoint(ty));
  m.color.a = 1.;
  m.color.r = 0.;
  m.color.g = 1.;
  m.color.b = 0.;
  ret.push_back(m);
  // Z axis
  m.points.clear();
  m.points.push_back(rosPoint(t0));
  m.points.push_back(rosPoint(tz));
  m.color.a = 1.;
  m.color.r = 0.;
  m.color.g = 0.;
  m.color.b = 1.;
  ret.push_back(m);
  return ret;
}

vm::InteractiveMarkerControl & makeVisualControl(const std::vector<vm::Marker>& visual_makers,
    vm::InteractiveMarker & marker)
{
  vm::InteractiveMarkerControl ret;
  ret.always_visible = true;
  ret.orientation.w = 1.0;
  ret.markers = visual_makers;
  marker.controls.push_back(ret);
  return marker.controls.back();
}

vm::InteractiveMarker make6DMarker(const std::string & name,
                                   bool control_position,
                                   bool control_orientation,
                                   const std::vector<vm::Marker>& visual_markers,
                                   bool move_x, bool move_y, bool move_z,
                                   bool rotate_x, bool rotate_y, bool rotate_z)
{
  vm::InteractiveMarker ret;
  ret.header.frame_id = "robot_map";
  ret.scale = 0.15;
  ret.name = name;
  ret.description = "";
  makeVisualControl(visual_markers, ret);

  vm::InteractiveMarkerControl control;
  control.orientation.w = 0.707107;
  control.orientation.x = 0.707107;
  control.orientation.y = 0;
  control.orientation.z = 0;
  if(control_orientation && rotate_x)
  {
    control.name = "rotate_x";
    control.interaction_mode = vm::InteractiveMarkerControl::ROTATE_AXIS;
    ret.controls.push_back(control);
  }
  if(control_position && move_x)
  {
    control.name = "move_x";
    control.interaction_mode = vm::InteractiveMarkerControl::MOVE_AXIS;
    ret.controls.push_back(control);
  }

  control.orientation.w = 0.707107;
  control.orientation.x = 0;
  control.orientation.y = 0.707107;
  control.orientation.z = 0;
  if(control_orientation && rotate_z)
  {
    control.name = "rotate_z";
    control.interaction_mode = vm::InteractiveMarkerControl::ROTATE_AXIS;
    ret.controls.push_back(control);
  }
  if(control_position && move_z)
  {
    control.name = "move_z";
    control.interaction_mode = vm::InteractiveMarkerControl::MOVE_AXIS;
    ret.controls.push_back(control);
  }

  control.orientation.w = 0.707107;
  control.orientation.x = 0;
  control.orientation.y = 0;
  control.orientation.z = 0.707107;
  if(control_orientation && rotate_y)
  {
    control.name = "rotate_y";
    control.interaction_mode = vm::InteractiveMarkerControl::ROTATE_AXIS;
    ret.controls.push_back(control);
  }
  if(control_position && move_y)
  {
    control.name = "move_y";
    control.interaction_mode = vm::InteractiveMarkerControl::MOVE_AXIS;
    ret.controls.push_back(control);
  }

  return ret;
}

vm::InteractiveMarker makeXYThetaMarker(const std::string & name)
{
  vm::InteractiveMarker int_marker;
  int_marker.header.frame_id = "robot_map";
  int_marker.scale = 0.25;
  int_marker.name = name;
  int_marker.description = "";
  makeVisualControl(makeAxisMarker(0.15*0.9),int_marker);

  vm::InteractiveMarkerControl control;
  control.orientation.w = 1;
  control.orientation.x = 0;
  control.orientation.y = 1;
  control.orientation.z = 0;
  control.interaction_mode = vm::InteractiveMarkerControl::MOVE_ROTATE;
  int_marker.controls.push_back(control);
  return int_marker;
}

visualization_msgs::Marker getPointMarker(const std::string & ns, const Eigen::Vector3d & pos, const mc_rtc::gui::Color& color, double scale = 0.02)
{
  visualization_msgs::Marker m;
  m.type = visualization_msgs::Marker::SPHERE;
  m.action = visualization_msgs::Marker::ADD;
  m.pose.position.x = pos(0);
  m.pose.position.y = pos(1);
  m.pose.position.z = pos(2);
  m.scale.x = scale;
  m.scale.y = scale;
  m.scale.z = scale;
  m.color.a = 1.0;
  m.color.r = color.r;
  m.color.g = color.g;
  m.color.b = color.b;
  m.header.stamp = ros::Time();
  m.header.frame_id = "robot_map";
  m.ns = ns;
  m.lifetime = ros::Duration(1.);
  return m;
}



SharedMarker::SharedMarker(interactive_markers::InteractiveMarkerServer & server,
                           const std::string & name, const vm::InteractiveMarker& marker,
                           interactive_markers::InteractiveMarkerServer::FeedbackCallback callback)
: server_(server),
  marker_(marker),
  callback_(callback)
{
  server_.insert(marker_, callback_);
}

SharedMarker::~SharedMarker()
{
  if(!hidden_)
  {
    server_.erase(marker_.name);
  }
}

void SharedMarker::toggle()
{
  if(hidden_)
  {
    hidden_ = false;
    server_.insert(marker_, callback_);
  }
  else
  {
    hidden_ = true;
    server_.erase(marker_.name);
  }
}

void SharedMarker::update(const Eigen::Vector3d & t)
{
  if(!hidden_)
  {
    geometry_msgs::Pose pose;
    pose.orientation.w = 1.0;
    pose.position.x = t.x();
    pose.position.y = t.y();
    pose.position.z = t.z();
    server_.setPose(marker_.name, pose);
  }
}

void SharedMarker::update(const sva::PTransformd & pos)
{
  if(!hidden_)
  {
    geometry_msgs::Pose pose;
    Eigen::Quaterniond q {pos.rotation().transpose()};
    pose.orientation.w = q.w();
    pose.orientation.x = q.x();
    pose.orientation.y = q.y();
    pose.orientation.z = q.z();
    pose.position.x = pos.translation().x();
    pose.position.y = pos.translation().y();
    pose.position.z = pos.translation().z();
    server_.setPose(marker_.name, pose);
  }
}

}