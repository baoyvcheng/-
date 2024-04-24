#include <iostream>
#include "pch.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread> // 用于多线程
#include <chrono> // 用于时间相关操作
#include <stdexcept> // 用于异常处理
#include <gtest/gtest.h> // Google Test 头文件

using namespace std;

// 乘客类，表示乘客
class Passenger {
public:
    int weight;
    int destination_floor;
    Passenger(int w, int dest) : weight(w), destination_floor(dest) {}
};

// 电梯类，表示一个电梯
class Elevator {
public:
    int elevator_id; // 电梯编号
    int current_floor; // 当前楼层
    int max_floors; // 最大楼层数
    int max_weight; // 最大承载重量
    int current_weight; // 当前载重
    int max_passengers; // 最大乘客人数
    vector<Passenger> passengers; // 乘客列表
    bool odd_only; // 仅允许操作奇数楼层

    // 构造函数，初始化电梯
    Elevator(int id, int floors, int max_w, int max_p, bool odd) : elevator_id(id), current_floor(0), max_floors(floors), max_weight(max_w), current_weight(0), max_passengers(max_p), odd_only(odd) {}

    // 移动电梯到指定楼层
    void move_to_floor(int target_floor) {
        if (target_floor != current_floor) {
            current_floor = target_floor;

            // 检查是否有乘客需要在目标楼层下车
            vector<Passenger> removed_passengers;
            for (auto it = passengers.begin(); it != passengers.end();) {
                if (it->destination_floor == target_floor) {
                    removed_passengers.push_back(*it);
                    it = passengers.erase(it);
                }
                else {
                    ++it;
                }
            }

            // 更新当前载重
            for (const auto& passenger : removed_passengers) {
                current_weight -= passenger.weight;
            }
        }
    }

    // 添加乘客
    bool add_passenger(const Passenger& passenger) {
        // 检查是否允许停靠在当前楼层
        if ((elevator_id == 2 && current_floor % 2 == 0) || (elevator_id == 3 && current_floor % 2 != 0)) {
            return false;
        }

        // 检查是否达到最大载客量和最大载重量
        if (passengers.size() < max_passengers && current_weight + passenger.weight <= max_weight) {
            passengers.push_back(passenger);
            current_weight += passenger.weight;
            return true;
        }
        else {
            return false;
        }
    }

    // LOOK算法移动电梯
    void move_look(int target_floor) {
        if (target_floor > current_floor) {
            while (current_floor < target_floor) {
                move_to_floor(current_floor + 1);
            }
        }
        else if (target_floor < current_floor) {
            while (current_floor > target_floor) {
                move_to_floor(current_floor - 1);
            }
        }

        // 在电梯移动的过程中检查是否有乘客请求，动态调整移动方向
        while (true) {
            bool has_request_above = false;
            bool has_request_below = false;
            for (const auto& passenger : passengers) {
                if (passenger.destination_floor > current_floor) {
                    has_request_above = true;
                }
                else if (passenger.destination_floor < current_floor) {
                    has_request_below = true;
                }
            }

            if (has_request_above) {
                move_to_floor(current_floor + 1);
            }
            else if (has_request_below) {
                move_to_floor(current_floor - 1);
            }
            else {
                break; // 没有乘客请求，结束移动
            }
        }
    }
};

// 电梯控制系统类，管理电梯
class ElevatorControlSystem {
public:
    vector<Elevator> elevators; // 电梯列表

    // 构造函数，初始化电梯列表
    ElevatorControlSystem() {
        // 初始化电梯
        elevators.push_back(Elevator(1, 20, 800, 10, false));
        elevators.push_back(Elevator(2, 10, 800, 10, true)); // 单层电梯
        elevators.push_back(Elevator(3, 10, 800, 10, false)); // 双层电梯
        elevators.push_back(Elevator(4, 20, 2000, 20, true));
    }

    // 请求电梯
    Elevator& request_elevator(int floor, const vector<Passenger>& passengers) {
        // 检查请求楼层是否合理
        if (floor < 0 || floor > elevators[0].max_floors) {
            throw invalid_argument("Invalid floor number");
        }

        // 计算每个电梯到达请求楼层的距离
        vector<pair<int, int>> distances; // 电梯编号和距离的对
        for (auto& elevator : elevators) {
            int distance = abs(elevator.current_floor - floor);
            distances.push_back(make_pair(elevator.elevator_id, distance));
        }

        // 按距离排序
        sort(distances.begin(), distances.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second;
            });

        // 选择距离最小且可用的电梯
        for (const auto& pair : distances) {
            Elevator& elevator = elevators[pair.first - 1];
            if (elevator.current_weight + total_passenger_weight(passengers) <= elevator.max_weight && elevator.passengers.size() + passengers.size() <= elevator.max_passengers) {
                return elevator;
            }
        }

        // 如果没有可用电梯，则抛出异常
        throw runtime_error("No available elevator to handle the request");
    }

    // 计算乘客总重量
    int total_passenger_weight(const vector<Passenger>& passengers) {
        int total_weight = 0;
        for (const auto& passenger : passengers) {
            total_weight += passenger.weight;
        }
        return total_weight;
    }
};

// 测试乘客类
TEST(PassengerTest, Constructor) {
    Passenger passenger(70, 5);
    EXPECT_EQ(passenger.weight, 70);
    EXPECT_EQ(passenger.destination_floor, 5);
}

// 测试电梯类的移动功能
TEST(ElevatorTest, MoveToFloor) {
    Elevator elevator(1, 20, 800, 10, false);
    elevator.move_to_floor(5);
    EXPECT_EQ(elevator.current_floor, 5);
}

// 测试电梯类的添加乘客功能
TEST(ElevatorTest, AddPassenger) {
    Elevator elevator(1, 20, 800, 10, false);
    Passenger passenger(70, 5);
    elevator.add_passenger(passenger);
    EXPECT_EQ(elevator.passengers.size(), 1);
}

// 测试电梯类的LOOK算法移动功能
TEST(ElevatorTest, MoveLookAlgorithm) {
    Elevator elevator(1, 20, 800, 10, false);
    elevator.move_look(5);
    EXPECT_EQ(elevator.current_floor, 5);
}

// 测试电梯控制系统类的请求电梯功能
TEST(ElevatorControlSystemTest, RequestElevator) {
    ElevatorControlSystem ecs;
    vector<Passenger> passengers;
    Passenger passenger1(70, 5);
    passengers.push_back(passenger1);
    Elevator& elevator = ecs.request_elevator(5, passengers);
    EXPECT_EQ(elevator.current_floor, 0); // 期望电梯开始于0层
}

// 测试电梯控制系统类的计算乘客总重量功能
TEST(ElevatorControlSystemTest, TotalPassengerWeight) {
    ElevatorControlSystem ecs;
    vector<Passenger> passengers;
    Passenger passenger1(70, 5);
    Passenger passenger2(80, 6);
    passengers.push_back(passenger1);
    passengers.push_back(passenger2);
    int total_weight = ecs.total_passenger_weight(passengers);
    EXPECT_EQ(total_weight, 150);
}
// 测试电梯类的添加乘客功能 - 超过最大乘客人数
TEST(ElevatorTest, AddPassenger_ExceedMaxPassengers) {
    Elevator elevator(1, 20, 800, 10, false);
    // 添加21个乘客
    for (int i = 0; i < 21; ++i) {
        Passenger passenger(70, 5); // 假设每个乘客的重量都是70kg，目的地楼层为5
        elevator.add_passenger(passenger);
    }
    // 期望乘客列表长度不超过最大乘客人数
    EXPECT_LE(elevator.passengers.size(), elevator.max_passengers);
}
// 测试电梯类的添加乘客功能 - 乘客重量超过最大承载重量
TEST(ElevatorTest, AddPassenger_ExceedMaxWeight) {
    Elevator elevator(1, 20, 800, 10, false);
    Passenger passenger(2500, 5); // 假设乘客重量为2500kg，目的地楼层为5
    bool added = elevator.add_passenger(passenger);
    // 期望添加失败，返回值为 false
    EXPECT_FALSE(added);
    // 期望乘客列表长度为 0
    EXPECT_EQ(elevator.passengers.size(), 0);
    // 期望当前载重为 0
    EXPECT_EQ(elevator.current_weight, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}