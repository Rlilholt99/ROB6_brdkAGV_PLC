# Use import and sys.path.insert if this .py file is moved.
# The path should point to the directory containing _libros_topics_typ.so
import sys
sys.path.insert(1, '/home/user/ros_topics_typ/')
import libros_topics_typ
import threading
from math import pi, cos, sin
import time
import rclpy
from rclpy.node import Node
from tf2_ros import TransformBroadcaster
import tf_transformations
from geometry_msgs.msg import Quaternion, Twist, TransformStamped
from nav_msgs.msg import Odometry


#delete me 
from std_msgs.msg import String



__author__ = "buhl@brommeweb.dk (Jacob Buh)"

"""
libros_topics_typ datamodel features:

initialize and setup callback handler:
    ros_topics_typ_datamodel = libros_topics_typ.libros_topics_typ_init()
    handler = ros_topics_typEventHandler()
    libros_topics_typ.add_event_handler(ros_topics_typ_datamodel, handler)

main methods:
    ros_topics_typ_datamodel.connect()
    ros_topics_typ_datamodel.disconnect()
    ros_topics_typ_datamodel.process()
    ros_topics_typ_datamodel.set_operational()
    ros_topics_typ_datamodel.dispose()
    ros_topics_typ_datamodel.get_nettime() : (int32_t) get current nettime

def user callbacks in class ros_topics_typEventHandler:
    on_connected
    on_disconnected
    on_operational

boolean values:
    ros_topics_typ_datamodel.is_connected
    ros_topics_typ_datamodel.is_operational

logging methods:
    ros_topics_typ_datamodel.log.error(str)
    ros_topics_typ_datamodel.log.warning(str)
    ros_topics_typ_datamodel.log.success(str)
    ros_topics_typ_datamodel.log.info(str)
    ros_topics_typ_datamodel.log.debug(str)
    ros_topics_typ_datamodel.log.verbose(str)

dataset odemetry:
    ros_topics_typEventHandler:on_change_odemetry : void(void) user callback function
    ros_topics_typ_datamodel.odemetry.nettime : (int32_t) nettime @ time of publish
    ros_topics_typ_datamodel.odemetry.value : (ros_topic_odemety_typ)  actual dataset values

dataset twist:
    ros_topics_typ_datamodel.twist.publish()
    ros_topics_typ_datamodel.twist.value : (ros_topic_twist_typ)  actual dataset values

dataset config:
    ros_topics_typ_datamodel.config.publish()
    ros_topics_typ_datamodel.config.value : (ros_config_typ)  actual dataset values
"""
gUpdateTwist = False
gTwist = Twist()

odometryGlobal = [float(0),float(0),float(0),float(0),float(0)]
clock = 1678190064.848563021
 
class ros_topics_typEventHandler(libros_topics_typ.ros_topics_typEventHandler):

    def __init__(self,node):
        libros_topics_typ.ros_topics_typEventHandler.__init__(self)
        self.node = node
        self.node.get_logger().info("Init ExOs eventhandler");
        self.node.get_logger().info("Test 6");

    def on_connected(self):
        self.ros_topics_typ_datamodel.log.success("python ros_topics_typ_datamodel connected!")
        self.node.get_logger().info("Connected to PLC");
        self.ros_topics_typ_datamodel.log.info("Linux: Motor control exos is running")

    def on_disconnected(self):
        self.node.publish_odom(self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.x,self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.y, self.ros_topics_typ_datamodel.odemetry.value.pose.pose.orientation.z, 0.0, 0.0)
        self.node.get_logger().info("Disconnected to PLC");
        
    def on_operational(self):
        self.node.get_logger().info("Communication is operational");   
        self.ros_topics_typ_datamodel.config.value.maxSpeed = float(self.node.get_parameter("max_speed")._value)
        self.ros_topics_typ_datamodel.config.value.minSpeed = float(self.node.get_parameter("min_speed")._value)
        self.ros_topics_typ_datamodel.config.value.baseWidth = float(self.node.get_parameter("base_width")._value)
        self.ros_topics_typ_datamodel.config.publish()
        #self.TICKS_PER_METER = float(rospy.get_param("~tick_per_meter", "105860"))
        
    def on_change_odemetry(self):
        #print("Odemetry has changed")
        # self.ros_topics_typ_datamodel.log.verbose("python dataset odemetry changed!")
        self.ros_topics_typ_datamodel.log.info("on_change: ros_topics_typ_datamodel.odemetry: " + str(self.ros_topics_typ_datamodel.odemetry.value))
        #self.node.publish_odom(self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.x,self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.y, self.ros_topics_typ_datamodel.odemetry.value.pose.pose.orientation.z, self.ros_topics_typ_datamodel.odemetry.value.twist.twist.linear.x, self.ros_topics_typ_datamodel.odemetry.value.twist.twist.angular.z) 
        self.node.publish_odom(self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.x,self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.y, self.ros_topics_typ_datamodel.odemetry.value.pose.pose.orientation.z) 
        #self.node.sendTransform(self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.x,self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.y, self.ros_topics_typ_datamodel.odemetry.value.pose.pose.orientation.z) 
        

        # odometryGlobal[0] = self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.x
        # odometryGlobal[1] = self.ros_topics_typ_datamodel.odemetry.value.pose.pose.position.y 
        # odometryGlobal[2] = self.ros_topics_typ_datamodel.odemetry.value.pose.pose.orientation.z 
        # odometryGlobal[3] = self.ros_topics_typ_datamodel.odemetry.value.twist.twist.linear.x 
        # odometryGlobal[4] = self.ros_topics_typ_datamodel.odemetry.value.twist.twist.angular.z


class exOsThread (threading.Thread):

    def __init__(self,node):
        self.node_ = node;
        threading.Thread.__init__(self)
        self.ros_topics_typ_datamodel = libros_topics_typ.libros_topics_typ_init()
        self.handler = ros_topics_typEventHandler(node)
        libros_topics_typ.add_event_handler(self.ros_topics_typ_datamodel, self.handler)
        self.ros_topics_typ_datamodel.connect()

   
    
    def sendCmdVel(self):
        global gTwist
        if self.ros_topics_typ_datamodel.is_connected:
            self.ros_topics_typ_datamodel.twist.value.angular.z = gTwist.angular.z
            self.ros_topics_typ_datamodel.twist.value.linear.x = gTwist.linear.x
            self.ros_topics_typ_datamodel.twist.publish()
            #self.node_.get_logger().debug('Write to angular z: %f and linear x: %f' % (twist.linear.x,twist.angular.z))
        else:
            self.node_.get_logger().info("Not connected to PLC")
            
    def run (self):
        global gUpdateTwist, odometryGlobal
        oldtime = 0
        while(True):
            nowtime = time.time_ns()
            if gUpdateTwist:
                gUpdateTwist = False
                #motorCtrl.publish_odom(odometryGlobal[0],odometryGlobal[1], odometryGlobal[2], odometryGlobal[3], odometryGlobal[4])
                self.node_.get_logger().info("run, running")
                #print("Run [ns]: %dms %f %f" %((nowtime-oldtime)/1000000,gTwist.linear.x,gTwist.angular.z))
                self.sendCmdVel()
            self.ros_topics_typ_datamodel.process()
            oldtime = nowtime
            
            




class motorCtrl(Node):
    def __init__(self):
        super().__init__('motor_control_exos')

        #rclpy.on_shutdown(self.shutdown)
        self.get_logger().info("Connecting to motor controller ExOs")
        
        
        
        
        self.declare_parameters(
            namespace='',
            parameters=[
                ('min_speed', -3),
                ('max_speed', 3),
                ('ticks_per_meter', 105860),
                ('base_width', 0.355),
                ('vel_topic', 'cmd_vel')
            ]
        )

        self.VEL_TOPIC = '/'+self.get_parameter("vel_topic")._value    
        self.last_set_speed_time = self.get_clock().now()
        self.get_logger().info('topic %s' % self.VEL_TOPIC)
        self.subscription = self.create_subscription(Twist,self.VEL_TOPIC, self.vel_callback,0)
        self.subscription

        self.publisher_ = self.create_publisher(Odometry, '/odom', 10)
        #self.publishing = self.create_publisher(String, '/topic', 10)

        #self.timer = self.create_timer(timer_period, self.publish_odom(odometryGlobal[0],odometryGlobal[1],odometryGlobal[2],odometryGlobal[3],odometryGlobal[4],))
        #self.timer = self.create_timer(timer_period, self.tester)
        self.br = TransformBroadcaster(self)

        self.get_logger().info('max_speed %f' % self.get_parameter('max_speed')._value)
        self.get_logger().info('ticks_per_meter %f' % self.get_parameter('ticks_per_meter')._value)
        self.get_logger().info('base_width %f' % self.get_parameter('base_width')._value)
        self.get_logger().info('min_speed %f' % self.get_parameter('min_speed')._value)
        self.get_logger().info('vel topic %s' % self.get_parameter('vel_topic')._value)
        
        #exOs communication
        self.exOs = exOsThread(self)
        self.exOs.daemon = True 
        self.exOs.start()


    @staticmethod
    def normalize_angle(angle):
        while angle > pi:
            angle -= 2.0 * pi
        while angle < -pi:
            angle += 2.0 * pi
        return angle
    
    # def tester(self):
    #     print(odometryGlobal[0])
    #     msg = String()
    #     msg.data = str(odometryGlobal[0])
    #     print("still going")
    #     self.publishing.publish(msg)
    #     self.get_logger().info('Publishing: "%s"' % msg.data)




    def sendTransform(self, cur_x, cur_y,cur_theta):
        t = TransformStamped()

        # Read message content and assign it to
        # corresponding tf variables
        t.header.stamp = self.get_clock().now().to_msg()
        t.child_frame_id = 'odom'
        t.header.frame_id = 'base_link'

        # Turtle only exists in 2D, thus we get x and y translation
        # coordinates from the message and set the z coordinate to 0
        t.transform.translation.x = cur_x
        t.transform.translation.y = cur_y
        t.transform.translation.z = 0.0

        # For the same reason, turtle can only rotate around one axis
        # and this why we set rotation in x and y to 0 and obtain
        # rotation in z axis from the message
        q = tf_transformations.quaternion_from_euler(0, 0, cur_theta)
        t.transform.rotation.x = float(q[0])
        t.transform.rotation.y = float(q[1])
        t.transform.rotation.z = float(q[2])
        t.transform.rotation.w = float(q[3])

        # Send the transformation
        self.br.sendTransform(t)


    def publish_odom(self, cur_x, cur_y, cur_theta):#, vx, vth
        try:
            cur_theta_neg = -cur_theta
            quat = tf_transformations.quaternion_from_euler(0, 0, cur_theta_neg)
            
            #self.sendTransform(cur_x,cur_y,cur_theta)
            #print(str(quat))
            
            odom = Odometry()
            odom.header.stamp = self.get_clock().now().to_msg()
            odom.header.frame_id = 'odom'
            odom.pose.pose.position.x = cur_x
            odom.pose.pose.position.y = cur_y
            odom.pose.pose.position.z = 0.0
            odom.pose.pose.orientation.x = float(quat[0])     
            odom.pose.pose.orientation.y = float(quat[1])     
            odom.pose.pose.orientation.z = float(quat[2])     
            odom.pose.pose.orientation.w = float(quat[3])     
            
           
          
            odom.pose.covariance[0] = 0.01
            odom.pose.covariance[7] = 0.01
            odom.pose.covariance[14] = 99999
            odom.pose.covariance[21] = 99999
            odom.pose.covariance[28] = 99999
            odom.pose.covariance[35] = 0.01
            
            odom.child_frame_id = 'base_link'
            odom.twist.twist.linear.x = 0.0 #vx
            odom.twist.twist.linear.y = 0.0
            odom.twist.twist.angular.z = 0.0 #vth
            odom.twist.covariance = odom.pose.covariance
            self.publisher_.publish(odom)


#dirty fix
            t = TransformStamped()

            # Read message content and assign it to
            # corresponding tf variables
            t.header.stamp = self.get_clock().now().to_msg()
            t.child_frame_id = 'odom'
            t.header.frame_id = 'base_link'

            # Turtle only exists in 2D, thus we get x and y translation
            # coordinates from the message and set the z coordinate to 0
            t.transform.translation.x = cur_x
            t.transform.translation.y = cur_y
            t.transform.translation.z = 0.0

            # For the same reason, turtle can only rotate around one axis
            # and this why we set rotation in x and y to 0 and obtain
            # rotation in z axis from the message
            
            t.transform.rotation.x = float(quat[0])
            t.transform.rotation.y = float(quat[1])
            t.transform.rotation.z = float(quat[2])
            t.transform.rotation.w = float(quat[3])

            # Send the transformation
            self.br.sendTransform(t)
#end of dirty fix       
        except Exception as e:
            print(traceback.format_exc())
             
    def vel_callback(self, twist):
        global gTwist,gUpdateTwist
        #print('vel_callback %f %f' %(twist.linear.x, twist.angular.z))
        gTwist = twist
        gUpdateTwist = True
        #self.exOs.sendCmdVel(twist) 
        

    # TODO: need clean shutdown so motors stop even if new msgs are arriving
    def destroy_node(self):
        self.exOs.terminate()
        self.get_logger().info("Shutting down")
        self.ros_topics_typ_datamodel.disconnect()
        self.ros_topics_typ_datamodel.dispose()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
 
    motorCtrlNode = motorCtrl()
    rclpy.spin(motorCtrlNode)
    
    motorCtrlNode.get_logger().info("Shutting down")
    motorCtrlNode.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()    

