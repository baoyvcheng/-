#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread> // 用于多线程
#include <chrono> // 用于时间相关操作
#include <stdexcept> // 用于异常处理

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
        cout << "Elevator " << elevator_id << " moving from floor " << current_floor << " to floor " << target_floor << "." << endl;
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
                cout << "Passenger exits elevator " << elevator_id << "." << endl;
            }

            // 显示电梯状态
            print_elevator_status();
        }
    }

    // 开门
    void open_door() {
        cout << "Elevator " << elevator_id << " opening door." << endl;
        this_thread::sleep_for(chrono::seconds(1)); // 模拟开门时间
    }

    // 关门
    void close_door() {
        cout << "Elevator " << elevator_id << " closing door." << endl;
        this_thread::sleep_for(chrono::seconds(1)); // 模拟关门时间
    }

    // 添加乘客
    bool add_passenger(const Passenger& passenger) {
        // 检查是否允许停靠在当前楼层
        if ((elevator_id == 2 && current_floor % 2 == 0) || (elevator_id == 3 && current_floor % 2 != 0)) {
            cout << "Elevator " << elevator_id << " can only operate on " << (elevator_id == 2 ? "odd" : "even") << " floors." << endl;
            return false;
        }

        // 检查是否达到最大载客量和最大载重量
        if (passengers.size() < max_passengers && current_weight + passenger.weight <= max_weight) {
            passengers.push_back(passenger);
            current_weight += passenger.weight;
            cout << "Passenger enters elevator " << elevator_id << "." << endl;
            return true;
        }
        else {
            cout << "Elevator is full or overweight, cannot add passenger." << endl;
            return false;
        }
    }


    // 打印电梯状态
    void print_elevator_status() {
        cout << "Elevator " << elevator_id << ": Floor " << current_floor << ", Weight " << current_weight << " kg, Passengers " << passengers.size() << "/" << max_passengers << endl;
    }

    
    // LOOK算法移动电梯
    void move_look(int target_floor) {
        // 检查是否允许停靠在当前楼层
        if ((elevator_id == 2 && target_floor % 2 == 0) || (elevator_id == 3 && target_floor % 2 != 0)) {
            cout << "Elevator " << elevator_id << " can only operate on " << (elevator_id == 2 ? "odd" : "even") << " floors." << endl;
            return;
        }

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

    // 打印电梯状态
    void print_elevator_status() {
        for (const auto& elevator : elevators) {
            cout << "Elevator " << elevator.elevator_id << ": Floor " << elevator.current_floor << ", Weight " << elevator.current_weight << " kg, Passengers " << elevator.passengers.size() << "/" << elevator.max_passengers << endl;
        }
    }
};

// 模拟用户输入的函数
void simulate_user_input(ElevatorControlSystem& ecs) {
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        int num_requests;
        cout << "Enter the number of requests: ";
        cin >> num_requests;

        // 检查输入请求数量是否合理
        if (num_requests <= 0) {
            cout << "Invalid number of requests. Please enter a valid number." << endl;
            continue;
        }

        vector<pair<int, vector<Passenger>>> requests; // 每个请求包含当前楼层和乘客信息的pair
        for (int i = 0; i < num_requests; ++i) {
            int floor, num_passengers;
            cout << "Enter current floor and number of passengers for request " << i + 1 << ": ";
            cin >> floor >> num_passengers;

            // 检查当前楼层是否合理
            if (floor < 0 || floor > ecs.elevators[0].max_floors) {
                cout << "Invalid floor number. Please enter a valid floor number." << endl;
                continue;
            }

            // 检查当前楼层是否可达
            bool reachable = false;
            for (const auto& elevator : ecs.elevators) {
                if (elevator.max_floors >= floor) {
                    reachable = true;
                    break;
                }
            }
            if (!reachable) {
                cout << "No elevator can reach the requested floor. Please enter a valid floor number." << endl;
                continue;
            }

            vector<Passenger> passengers;
            for (int j = 0; j < num_passengers; ++j) {
                int weight, dest_floor;
                cout << "Enter weight and destination floor of passenger " << j + 1 << ": ";
                cin >> weight >> dest_floor;

                // 检查乘客的重量和目的楼层是否合理
                if (weight <= 0 || dest_floor < 0 || dest_floor > ecs.elevators[0].max_floors) {
                    cout << "Invalid passenger weight or destination floor. Please enter valid values." << endl;
                    continue;
                }

                passengers.push_back(Passenger(weight, dest_floor));
            }

            requests.push_back(make_pair(floor, passengers));
        }

        // 请求电梯并进行操作
        try {
            for (const auto& request : requests) {
                Elevator& selected_elevator = ecs.request_elevator(request.first, request.second);
                selected_elevator.open_door();
                for (const auto& passenger : request.second) {
                    selected_elevator.add_passenger(passenger);
                }
                selected_elevator.close_door();
                selected_elevator.move_look(request.second[0].destination_floor); // 使用LOOK算法移动电梯

                // 打印电梯状态
                ecs.print_elevator_status();
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Do you want to continue? (y/n): ";
        cin >> choice;
    }
}

// 主函数
int main() {
    ElevatorControlSystem ecs;

    // 创建一个线程来模拟用户输入
    thread user_input_thread(simulate_user_input, ref(ecs));

    // 模拟电梯运行
    while (true) {
        // 进行电梯控制逻辑，这里可以使用LOOK算法等实现实时调度

        // 休眠一段时间模拟电梯运行
        this_thread::sleep_for(chrono::seconds(3));
    }

    // 等待用户输入线程结束
    user_input_thread.join();

    return 0;
}
