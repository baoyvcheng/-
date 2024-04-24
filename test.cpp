#include <iostream>
#include "pch.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread> // ���ڶ��߳�
#include <chrono> // ����ʱ����ز���
#include <stdexcept> // �����쳣����
#include <gtest/gtest.h> // Google Test ͷ�ļ�

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
            }
        }
    }

    // ��ӳ˿�
    bool add_passenger(const Passenger& passenger) {
        // ����Ƿ�����ͣ���ڵ�ǰ¥��
        if ((elevator_id == 2 && current_floor % 2 == 0) || (elevator_id == 3 && current_floor % 2 != 0)) {
            return false;
        }

        // ����Ƿ�ﵽ����ؿ��������������
        if (passengers.size() < max_passengers && current_weight + passenger.weight <= max_weight) {
            passengers.push_back(passenger);
            current_weight += passenger.weight;
            return true;
        }
        else {
            return false;
        }
    }

    // LOOK�㷨�ƶ�����
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
};

// ���Գ˿���
TEST(PassengerTest, Constructor) {
    Passenger passenger(70, 5);
    EXPECT_EQ(passenger.weight, 70);
    EXPECT_EQ(passenger.destination_floor, 5);
}

// ���Ե�������ƶ�����
TEST(ElevatorTest, MoveToFloor) {
    Elevator elevator(1, 20, 800, 10, false);
    elevator.move_to_floor(5);
    EXPECT_EQ(elevator.current_floor, 5);
}

// ���Ե��������ӳ˿͹���
TEST(ElevatorTest, AddPassenger) {
    Elevator elevator(1, 20, 800, 10, false);
    Passenger passenger(70, 5);
    elevator.add_passenger(passenger);
    EXPECT_EQ(elevator.passengers.size(), 1);
}

// ���Ե������LOOK�㷨�ƶ�����
TEST(ElevatorTest, MoveLookAlgorithm) {
    Elevator elevator(1, 20, 800, 10, false);
    elevator.move_look(5);
    EXPECT_EQ(elevator.current_floor, 5);
}

// ���Ե��ݿ���ϵͳ���������ݹ���
TEST(ElevatorControlSystemTest, RequestElevator) {
    ElevatorControlSystem ecs;
    vector<Passenger> passengers;
    Passenger passenger1(70, 5);
    passengers.push_back(passenger1);
    Elevator& elevator = ecs.request_elevator(5, passengers);
    EXPECT_EQ(elevator.current_floor, 0); // �������ݿ�ʼ��0��
}

// ���Ե��ݿ���ϵͳ��ļ���˿�����������
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
// ���Ե��������ӳ˿͹��� - �������˿�����
TEST(ElevatorTest, AddPassenger_ExceedMaxPassengers) {
    Elevator elevator(1, 20, 800, 10, false);
    // ���21���˿�
    for (int i = 0; i < 21; ++i) {
        Passenger passenger(70, 5); // ����ÿ���˿͵���������70kg��Ŀ�ĵ�¥��Ϊ5
        elevator.add_passenger(passenger);
    }
    // �����˿��б��Ȳ��������˿�����
    EXPECT_LE(elevator.passengers.size(), elevator.max_passengers);
}
// ���Ե��������ӳ˿͹��� - �˿�������������������
TEST(ElevatorTest, AddPassenger_ExceedMaxWeight) {
    Elevator elevator(1, 20, 800, 10, false);
    Passenger passenger(2500, 5); // ����˿�����Ϊ2500kg��Ŀ�ĵ�¥��Ϊ5
    bool added = elevator.add_passenger(passenger);
    // �������ʧ�ܣ�����ֵΪ false
    EXPECT_FALSE(added);
    // �����˿��б���Ϊ 0
    EXPECT_EQ(elevator.passengers.size(), 0);
    // ������ǰ����Ϊ 0
    EXPECT_EQ(elevator.current_weight, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}