#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
	//////////////
	// Listen for t0 to camera_link transform
	ros::init(argc, argv, "my_tf_listener");

	// Init a node handler
	ros::NodeHandle node;

	// Listener to listen for specific transform we want
  	tf::TransformListener listener;
	// Rate at which to check for the transform
  	ros::Rate rate(20.0);

		// Hack solution to not finding certain transforms
		rate.sleep();

 	while (node.ok()){
  	 	 tf::StampedTransform t0_to_camera_link_tf;
    try{
      	listener.lookupTransform("/t0", "/camera_link",  
                               ros::Time(0), t0_to_camera_link_tf);
    }
    catch (tf::TransformException ex){
			ROS_INFO("Didn't find the transform between t0 and camera link. Trying again in .1 s.");
      ROS_ERROR("%s",ex.what());
			rate.sleep();
			continue;
    }

	// Set up transform broadcaster for our new TF
	static tf::TransformBroadcaster broadcaster;

	// Make explicit that the inverse of cam_link to tool0_controller is the tf betwen
	// to and camera_ink
	tf::Transform tool0_controller_to_camera_link_tf = t0_to_camera_link_tf;

	// Broadcast the transform
	broadcaster.sendTransform(tf::StampedTransform(tool0_controller_to_camera_link_tf,
																									 ros::Time::now(), "tool0_controller", "camera_link"));
	// broadcaster.sendTransform(tool0_controller_to_camera_link_tf, "tool0_controller" "camera_link");

    rate.sleep();
	}
	return 0;
}
