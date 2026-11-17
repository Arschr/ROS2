
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

class TurtleController : public rclcpp::Node {
public:
    TurtleController() : Node("turtle_controller") {
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        pose_subscription_ = this->create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose", 10,
            std::bind(&TurtleController::on_pose_received_, this,
                      std::placeholders::_1));
    }

private:
    void on_pose_received_(const turtlesim::msg::Pose::SharedPtr pose) {
        // TODO: 收到位置计算误差，发布速度指令
        auto message = geometry_msgs::msg::Twist();

        double current_x = pose -> x;
        double current_y = pose -> y;
        RCLCPP_INFO(this -> get_logger(), "Current Position : (x = %f, y = %f)", current_x, current_y);

        double distance = std::sqrt((target_x_ - current_x) * (target_x_ - current_x) + (target_y_ - current_y) * (target_y_ - current_y));
        double angle = std::atan2(target_y_ - current_y, target_x_ - current_x) - pose -> theta;

        if(distance > 0.1){
            if(fabs(angle) > 0.2){
                message.angular.z = fabs(angle);
            }else{
                message.linear.x = k_ * distance;
            }
        }

        if(message.linear.x > max_speed_){
            message.linear.x = max_speed_;
        }

        velocity_publisher_ -> publish(message);

    }

private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;

    double target_x_{2.0};  // 目标位置x，设置默认值1.0
    double target_y_{7.0};  // 目标位置y，设置默认值1.0
    double k_{1.0};         // 比例系数，控制输出 = 误差 × 比例系数
    double max_speed_{3.0}; // 最大线速度，设置默认值3.0
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
