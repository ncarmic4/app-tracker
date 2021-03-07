#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>

using namespace std;

long interval = 1000; // ms

class Application {
    private:
        string name;
        long activeTime;
        long backgroundTime;
        unordered_map<string, Application*> subApplications;

    public:
        Application(string name) {
            this->name = name;
            activeTime = 0;
            backgroundTime = 0;
        }

        string getName() const {
            return name;
        }

        long getActiveTime() const {
            return activeTime;
        }

        long getBackgroundTime() const {
            return backgroundTime;
        }

        void putApplication(string str, Application* app) {
            subApplications.insert(pair<string, Application*>(str, app));
        }

        Application* getApplication(string name) {
            return subApplications[name];
        }

        void incrementTime(bool active) {
            if (active) {
                activeTime += interval;
            }
            backgroundTime += interval;
        }

        bool findKey(string key) {
            return subApplications.find(key) != subApplications.end();
        }

        string toString() {
            stringstream ss;
            ss << left << setw(128) << name << " | " << setw(16) << activeTime << " | " << setw(16) << backgroundTime << endl;
            for (auto& x: subApplications) {
                Application* app = x.second;
                ss << "      * " << left << setw(120) << app->getName() << " | " << setw(16) << app->getActiveTime() << " | " << setw(16) << app->getBackgroundTime() << endl;
            }
            return ss.str();
        }
};