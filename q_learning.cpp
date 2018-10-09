#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
using namespace std;

#define GANMA 0.8
#define ROOM 6
#define EPISODE 200
#define TARGET 5

class Q_learning
{

  public:
    Q_learning(int);
    void addPath(int, int, int); // (room1,room2, reward) 
    void showMat(char); // c = 'r' print  reward matrix c='q' print Q matrix
    void train();
    void test(int); // return the path from start to target

  private:
    int numRoom;
    // [state][action]
    int rMat[10][10];
    double qMat[10][10];
    unordered_map<int, vector<int>> adjancentRoom;
};

int main()
{
    Q_learning q(ROOM);
    q.addPath(0, 4, 0);
    q.addPath(1, 5, 100);
    q.addPath(1, 3, 0);
    q.addPath(2, 3, 0);
    q.addPath(3, 1, 0);
    q.addPath(3, 2, 0);
    q.addPath(3, 4, 0);
    q.addPath(4, 5, 100);
    q.addPath(4, 0, 0);
    q.addPath(4, 3, 0);
    q.addPath(5, 5, 100);
    q.addPath(5, 1, 0);
    q.addPath(5, 4, 0);
    q.showMat('r');
    q.showMat('q');
    // train model
    q.train();
    // q(s,a)  of covergence
    q.showMat('q');

    //print path from 0 ~ 5
    for(int i = 0; i < ROOM; ++i){
        q.test(i);
    }
    return 0;
}

Q_learning::Q_learning(int n)
{
    assert(n > 0);
    numRoom = n;
    for (int i = 0; i < numRoom; ++i)
    {
        for (int j = 0; j < numRoom; ++j)
        {
            // r(s,a)
            rMat[i][j] = -1;
            //q(s,a) initialize to zero
            qMat[i][j] = 0;
        }
    }
}

void Q_learning::addPath(int r1, int r2, int reward)
{
    assert(r1 < numRoom && r2 < numRoom);
    rMat[r1][r2] = reward;
    adjancentRoom[r1].push_back(r2);
}

void Q_learning::showMat(char c)
{

    int _min = 0;
    int _max = qMat[0][0];

    if (c == 'r')
    {
        cout << "reward matrix: "
             << "\n\n";
        for (int i = 0; i < numRoom; i++)
        {
            for (int j = 0; j < numRoom; j++)
            {
                cout << rMat[i][j] << '\t';
            }
            cout << '\n';
        }
    }
    else
    {
        cout << "Q(s,a) matrix: "
             << "\n\n";
        for (int i = 0; i < numRoom; i++)
        {
            for (int j = 0; j < numRoom; j++)
            {
                _max = qMat[i][j] > _max ? qMat[i][j] : _max;
            }
        }

        for (int i = 0; i < numRoom; i++)
        {
            for (int j = 0; j < numRoom; j++)
            {
                cout << floor ((qMat[i][j] / _max) * 100) << '\t';
            }
            cout << '\n';
        }
    }
}

void Q_learning::train()
{

    // 1. 随机选择一个厨师状态
    int state = rand() % numRoom;
    cout << "the robot start at state: " << state << '\n';
    int steps = 1;
    vector<int> action_list;

    while (steps <= EPISODE)
    {
        cout << "Training episode: " << steps << '\n';

        // 2. 在当前state 下， 随机选择一个行为action
        action_list.clear();
        for (int a = 0; a < numRoom; ++a)
        {
            if (rMat[state][a] >= 0)
                action_list.push_back(a);
        }
        int action = rand() % action_list.size();
        // cout << "robot take action " << state << " to " << action_list[action] << '\n';
        action = action_list[action];

        double _max_Q = 0.0;
        for (auto next : adjancentRoom[action])
        {
            _max_Q = qMat[action][next] > _max_Q ? qMat[action][next] : _max_Q;
        }

        qMat[state][action] = rMat[state][action] + GANMA * _max_Q;
        // cout << "Q update like : \n";
        // showMat('q');
        state = action;
        steps++;
    }
}

void Q_learning::test(int start){

    cout << start;
    int state = start;
    int action = 0;
    int _max_q = 0;


    while(state != TARGET){
        _max_q = 0;
        for(auto next: adjancentRoom[state]){
            if (qMat[state][next] > _max_q){
                _max_q = qMat[state][next];
                action = next;
            }
        }
        cout << "-> " << action ;
        state = action;
    }
    cout << "\n";
}