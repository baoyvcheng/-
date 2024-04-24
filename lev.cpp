#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread> // ���ڶ��߳�
#include <chrono> // ����ʱ����ز���
#include <stdexcept> // �����쳣����

using namespace std;

// �˿��࣬��ʾ�˿�
class Passenger {
public:
    int weight;
    int destination_floor;
    Passenger(int w, int dest) : weight(w), destination_floor(dest) {}
};

// �����࣬��ʾһ������
class Elevator {
public:
    int elevator_id; // ���ݱ��
    int current_floor; // ��ǰ¥��
    int max_floors; // ���¥����
    int max_weight; // ����������
    int current_weight; // ��ǰ����
    int max_passengers; // ���˿�����
    vector<Passenger> passengers; // �˿��б�
    bool odd_only; // �������������¥��

    // ���캯������ʼ������
    Elevator(int id, int floors, int max_w, int max_p, bool odd) : elevator_id(id), current_floor(0), max_floors(floors), max_weight(max_w), current_weight(0), max_passengers(max_p), odd_only(odd) {}

    // �ƶ����ݵ�ָ��¥��
    void move_to_floor(int target_floor) {
        cout << "Elevator " << elevator_id << " moving from floor " << current_floor << " to floor " << target_floor << "." << endl;
        if (target_floor != current_floor) {
            current_floor = target_floor;

            // ����Ƿ��г˿���Ҫ��Ŀ��¥���³�
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

            // ���µ�ǰ����
            for (const auto& passenger : removed_passengers) {
                current_weight -= passenger.weight;
                cout << "Passenger exits elevator " << elevator_id << "." << endl;
            }

            // ��ʾ����״̬
            print_elevator_status();
        }
    }

    // ����
    void open_door() {
        cout << "Elevator " << elevator_id << " opening door." << endl;
        this_thread::sleep_for(chrono::seconds(1)); // ģ�⿪��ʱ��
    }

    // ����
    void close_door() {
        cout << "Elevator " << elevator_id << " closing door." << endl;
        this_thread::sleep_for(chrono::seconds(1)); // ģ�����ʱ��
    }

    // ��ӳ˿�
    bool add_passenger(const Passenger& passenger) {
        // ����Ƿ�����ͣ���ڵ�ǰ¥��
        if ((elevator_id == 2 && current_floor % 2 == 0) || (elevator_id == 3 && current_floor % 2 != 0)) {
            cout << "Elevator " << elevator_id << " can only operate on " << (elevator_id == 2 ? "odd" : "even") << " floors." << endl;
            return false;
        }

        // ����Ƿ�ﵽ����ؿ��������������
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


    // ��ӡ����״̬
    void print_elevator_status() {
        cout << "Elevator " << elevator_id << ": Floor " << current_floor << ", Weight " << current_weight << " kg, Passengers " << passengers.size() << "/" << max_passengers << endl;
    }

    
    // LOOK�㷨�ƶ�����
    void move_look(int target_floor) {
        // ����Ƿ�����ͣ���ڵ�ǰ¥��
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

        // �ڵ����ƶ��Ĺ����м���Ƿ��г˿����󣬶�̬�����ƶ�����
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
                break; // û�г˿����󣬽����ƶ�
            }
        }
    }


};

// ���ݿ���ϵͳ�࣬�������
class ElevatorControlSystem {
public:
    vector<Elevator> elevators; // �����б�

    // ���캯������ʼ�������б�
    ElevatorControlSystem() {
        // ��ʼ������
        elevators.push_back(Elevator(1, 20, 800, 10, false));
        elevators.push_back(Elevator(2, 10, 800, 10, true)); // �������
        elevators.push_back(Elevator(3, 10, 800, 10, false)); // ˫�����
        elevators.push_back(Elevator(4, 20, 2000, 20, true));
    }

    // �������
    Elevator& request_elevator(int floor, const vector<Passenger>& passengers) {
        // �������¥���Ƿ����
        if (floor < 0 || floor > elevators[0].max_floors) {
            throw invalid_argument("Invalid floor number");
        }

        // ����ÿ�����ݵ�������¥��ľ���
        vector<pair<int, int>> distances; // ���ݱ�ź;���Ķ�
        for (auto& elevator : elevators) {
            int distance = abs(elevator.current_floor - floor);
            distances.push_back(make_pair(elevator.elevator_id, distance));
        }

        // ����������
        sort(distances.begin(), distances.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second;
            });

        // ѡ�������С�ҿ��õĵ���
        for (const auto& pair : distances) {
            Elevator& elevator = elevators[pair.first - 1];
            if (elevator.current_weight + total_passenger_weight(passengers) <= elevator.max_weight && elevator.passengers.size() + passengers.size() <= elevator.max_passengers) {
                return elevator;
            }
        }

        // ���û�п��õ��ݣ����׳��쳣
        throw runtime_error("No available elevator to handle the request");
    }

    // ����˿�������
    int total_passenger_weight(const vector<Passenger>& passengers) {
        int total_weight = 0;
        for (const auto& passenger : passengers) {
            total_weight += passenger.weight;
        }
        return total_weight;
    }

    // ��ӡ����״̬
    void print_elevator_status() {
        for (const auto& elevator : elevators) {
            cout << "Elevator " << elevator.elevator_id << ": Floor " << elevator.current_floor << ", Weight " << elevator.current_weight << " kg, Passengers " << elevator.passengers.size() << "/" << elevator.max_passengers << endl;
        }
    }
};

// ģ���û�����ĺ���
void simulate_user_input(ElevatorControlSystem& ecs) {
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        int num_requests;
        cout << "Enter the number of requests: ";
        cin >> num_requests;

        // ����������������Ƿ����
        if (num_requests <= 0) {
            cout << "Invalid number of requests. Please enter a valid number." << endl;
            continue;
        }

        vector<pair<int, vector<Passenger>>> requests; // ÿ�����������ǰ¥��ͳ˿���Ϣ��pair
        for (int i = 0; i < num_requests; ++i) {
            int floor, num_passengers;
            cout << "Enter current floor and number of passengers for request " << i + 1 << ": ";
            cin >> floor >> num_passengers;

            // ��鵱ǰ¥���Ƿ����
            if (floor < 0 || floor > ecs.elevators[0].max_floors) {
                cout << "Invalid floor number. Please enter a valid floor number." << endl;
                continue;
            }

            // ��鵱ǰ¥���Ƿ�ɴ�
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

                // ���˿͵�������Ŀ��¥���Ƿ����
                if (weight <= 0 || dest_floor < 0 || dest_floor > ecs.elevators[0].max_floors) {
                    cout << "Invalid passenger weight or destination floor. Please enter valid values." << endl;
                    continue;
                }

                passengers.push_back(Passenger(weight, dest_floor));
            }

            requests.push_back(make_pair(floor, passengers));
        }

        // ������ݲ����в���
        try {
            for (const auto& request : requests) {
                Elevator& selected_elevator = ecs.request_elevator(request.first, request.second);
                selected_elevator.open_door();
                for (const auto& passenger : request.second) {
                    selected_elevator.add_passenger(passenger);
                }
                selected_elevator.close_door();
                selected_elevator.move_look(request.second[0].destination_floor); // ʹ��LOOK�㷨�ƶ�����

                // ��ӡ����״̬
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

// ������
int main() {
    ElevatorControlSystem ecs;

    // ����һ���߳���ģ���û�����
    thread user_input_thread(simulate_user_input, ref(ecs));

    // ģ���������
    while (true) {
        // ���е��ݿ����߼����������ʹ��LOOK�㷨��ʵ��ʵʱ����

        // ����һ��ʱ��ģ���������
        this_thread::sleep_for(chrono::seconds(3));
    }

    // �ȴ��û������߳̽���
    user_input_thread.join();

    return 0;
}
